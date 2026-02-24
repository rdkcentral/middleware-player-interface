#!/bin/bash

#=============================================================================
# AAMP to Middleware Player Interface Sync Script (Internal Version)
# 
# This script synchronizes changes from aamp dev_sprint_25_2 branch to 
# middleware-player-interface feature/dev_sprint_pli branch.
#
# This version is designed to run from within the middleware-player-interface
# repository in the sync/ folder.
#
# Process:
# 1. Navigate to parent directory to access workspace
# 2. Read last synced aamp commit from aamp_sync_cid.txt
# 3. Check/clone aamp repository and get latest commit
# 4. Create patch between last synced and latest aamp commits
# 5. Filter patch for middleware-related changes
# 6. Apply filtered patch to middleware-player-interface
# 7. Update sync tracking file and commit changes
#
# Usage: ./aamp_middleware_sync.sh [start_step]
#   start_step: (optional) Step number to start from (1-24). Default is 1.
#=============================================================================

set -e  # Exit on any error

# Parse command line arguments
start_from_step=1
if [ $# -eq 1 ]; then
    if [[ "$1" =~ ^[0-9]+$ ]] && [ "$1" -ge 1 ] && [ "$1" -le 24 ]; then
        start_from_step=$1
        echo "Starting from step $start_from_step"
    elif [ "$1" = "--help" ] || [ "$1" = "-h" ]; then
        echo "Usage: $0 [start_step]"
        echo ""
        echo "This script syncs dev_sprint_25_2 branch changes from aamp to"
        echo "feature/dev_sprint_pli branch in middleware-player-interface repository."
        echo ""
        echo "Arguments:"
        echo "  start_step    Optional. Step number to start from (1-24)."
        echo "                If not provided, starts from step 1."
        echo ""
        echo "State Management:"
        echo "  The script automatically saves progress to 'aamp_middleware_sync.temp' file."
        echo "  If the script fails, you can resume from the last completed step."
        echo "  When starting from step 1, any existing state file is removed."
        echo "  When resuming from a later step, the state file is loaded automatically."
        echo ""
        echo "Examples:"
        echo "  $0           # Start from step 1 (default)"
        echo "  $0 10        # Start from step 10, skipping steps 1-9"
        echo "  $0 22        # Start from step 22, skipping steps 1-21"
        echo ""
        echo "Steps overview:"
        echo "  1-2:    Directory setup and validation"
        echo "  3-11:   AAMP repository management and commit analysis"
        echo "  12-18:  Patch generation, filtering and application"
        echo "  19-24:  Commit tracking, push and cleanup"
        exit 0
    else
        echo "Error: Invalid step number '$1'. Must be a number between 1 and 24."
        echo "Use --help for usage information."
        exit 1
    fi
elif [ $# -gt 1 ]; then
    echo "Error: Too many arguments provided."
    echo "Use --help for usage information."
    exit 1
fi

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_step() {
    echo -e "${GREEN}[STEP $1]${NC} $2"
}

# Function to check if step should be executed
should_execute_step() {
    local step_num=$1
    if [ $step_num -ge $start_from_step ]; then
        return 0  # Execute step
    else
        return 1  # Skip step
    fi
}

# Function to skip a step
skip_step() {
    local step_num=$1
    local step_desc="$2"
    echo -e "${YELLOW}[SKIP $step_num]${NC} $step_desc (skipped)"
}

# Function to handle errors
handle_error() {
    print_error "Script failed at step $1: $2"
    print_error "Steps completed successfully up to step $((current_step - 1))"
    
    # Save current state before exiting
    save_state
    print_status "State saved to $STATE_FILE - you can resume using: $0 $current_step"
    
    exit 1
}

# State file for persistence - use absolute path to keep it in sync directory
SYNC_DIR="$(pwd)"
STATE_FILE="$SYNC_DIR/aamp_middleware_sync.temp"

# Functions for state management
save_state() {
    cat > "$STATE_FILE" << EOF
current_step=$current_step
current_date="$current_date"
current_time="$current_time"
commit_message="$commit_message"
last_synced_aamp_srcrev="$last_synced_aamp_srcrev"
latest_aamp_commit="$latest_aamp_commit"
latest_aamp_short_commit="$latest_aamp_short_commit"
patch_filename="$patch_filename"
filtered_patch_filename="$filtered_patch_filename"
middleware_final_commit="$middleware_final_commit"
middleware_final_short_commit="$middleware_final_short_commit"
no_new_changes=$no_new_changes
middleware_sections=$middleware_sections
MID_PATCH_REQUIRED=$MID_PATCH_REQUIRED
workspace_dir="$workspace_dir"
EOF
    print_status "State saved to $STATE_FILE (step $current_step)"
}

load_state() {
    if [ -f "$STATE_FILE" ]; then
        print_status "Loading previous state from $STATE_FILE"
        source "$STATE_FILE"
        print_status "State loaded: resuming from step $start_from_step (last completed: step $((current_step - 1)))"
        
        # Display key restored variables for verification
        if [ -n "$last_synced_aamp_srcrev" ]; then
            print_status "Restored last_synced_aamp_srcrev: $last_synced_aamp_srcrev"
        fi
        if [ -n "$latest_aamp_commit" ]; then
            print_status "Restored latest_aamp_commit: $latest_aamp_commit"
        fi
        if [ -n "$patch_filename" ]; then
            print_status "Restored patch_filename: $patch_filename"
        fi
        if [ -n "$workspace_dir" ]; then
            print_status "Restored workspace_dir: $workspace_dir"
        fi
        
        # Set current_step to start_from_step for proper continuation
        current_step=$start_from_step
    else
        print_warning "State file $STATE_FILE not found, cannot restore previous state"
        print_warning "This may cause issues if resuming from step > 1"
    fi
}

initialize_state() {
    if [ $start_from_step -eq 1 ]; then
        # Starting fresh - remove old state file if exists
        if [ -f "$STATE_FILE" ]; then
            print_status "Removing existing state file for fresh start"
            rm "$STATE_FILE"
        fi
        print_status "Starting fresh from step 1"
    else
        # Resuming from specific step - try to load state
        print_status "Attempting to resume from step $start_from_step"
        load_state
    fi
}

# Variables
current_step=1
current_date=$(date +"%Y-%m-%d")
current_time=$(date +"%H:%M:%S")
commit_message="dev_sprint_25_2 sync $current_date $current_time"
last_synced_aamp_srcrev=""
latest_aamp_commit=""
latest_aamp_short_commit=""
patch_filename=""
filtered_patch_filename=""
middleware_final_commit=""
middleware_final_short_commit=""
no_new_changes=false
middleware_sections=0
MID_PATCH_REQUIRED=false
workspace_dir=""

# Print starting information
initialize_state

# Step 1: Navigate to parent directory (workspace)
current_step=1
if should_execute_step $current_step; then
    print_step $current_step "Navigate to workspace directory (../) from sync folder"
    
    # Verify we're in the sync directory
    if [ ! -f "aamp_middleware_sync.sh" ]; then
        handle_error $current_step "Script not found - ensure you're running from the sync directory"
    fi
    
    if ! cd ../; then
        handle_error $current_step "Failed to navigate to parent directory"
    fi
    
    # Store workspace directory for later reference
    workspace_dir="$(pwd)"
    print_status "Now in workspace directory: $workspace_dir"
    
    # Verify we're in middleware-player-interface repository
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "aamp_sync_cid.txt not found - ensure sync script is in middleware-player-interface/sync/"
    fi
    
    save_state
else
    skip_step $current_step "Navigate to workspace directory (../) from sync folder"
fi

# Step 2: Switch to and pull latest changes on feature/dev_sprint_pli branch
current_step=2
if should_execute_step $current_step; then
    print_step $current_step "Checkout and pull latest changes from feature/dev_sprint_pli"
    
    if ! git checkout feature/dev_sprint_pli; then
        handle_error $current_step "Failed to checkout feature/dev_sprint_pli branch"
    fi
    
    if ! git pull; then
        handle_error $current_step "Failed to pull latest changes"
    fi
    print_status "Successfully pulled latest changes on feature/dev_sprint_pli"
    save_state
else
    skip_step $current_step "Checkout and pull latest changes from feature/dev_sprint_pli"
fi

# Step 3: Read last synced aamp commit from aamp_sync_cid.txt
current_step=3
if should_execute_step $current_step; then
    print_step $current_step "Read last synced aamp commit from aamp_sync_cid.txt"
    
    if [ -f "aamp_sync_cid.txt" ]; then
        last_synced_aamp_srcrev=$(cat aamp_sync_cid.txt | tr -d '[:space:]')
        if [ -z "$last_synced_aamp_srcrev" ]; then
            handle_error $current_step "aamp_sync_cid.txt is empty"
        fi
        print_status "Last synced aamp commit: $last_synced_aamp_srcrev"
    else
        handle_error $current_step "aamp_sync_cid.txt file not found"
    fi
    save_state
else
    skip_step $current_step "Read last synced aamp commit from aamp_sync_cid.txt"
fi

# Step 4: Navigate to sync directory to access workspace
current_step=4
if should_execute_step $current_step; then
    print_step $current_step "Navigate to sync directory to access workspace"
    if ! cd ./sync; then
        handle_error $current_step "Failed to navigate to sync directory"
    fi
    print_status "Now in sync directory: $(pwd)"
    save_state
else
    skip_step $current_step "Navigate to sync directory to access workspace"
fi

# Step 5: Check if aamp folder exists
current_step=5
if should_execute_step $current_step; then
    print_step $current_step "Check if aamp folder exists"
    if [ ! -d "aamp" ]; then
        print_status "aamp folder not found"
    else
        print_status "aamp folder found"
    fi
    save_state
else
    skip_step $current_step "Check if aamp folder exists"
fi

# Step 6: Clone aamp repository if not exists
current_step=6
if should_execute_step $current_step; then
    print_step $current_step "Clone aamp repository if needed"
    if [ ! -d "aamp" ]; then
        print_status "Cloning aamp repository..."
        if ! git clone https://github.com/rdkcentral/aamp.git; then
            handle_error $current_step "Failed to clone aamp repository"
        fi
        print_status "Successfully cloned aamp repository"
    else
        print_status "aamp repository already exists, skipping clone"
    fi
    save_state
else
    skip_step $current_step "Clone aamp repository if needed"
fi

# Step 7: Switch to aamp directory and checkout dev_sprint_25_2
current_step=7
if should_execute_step $current_step; then
    print_step $current_step "Switch to aamp and checkout dev_sprint_25_2"
    if ! cd ./aamp; then
        handle_error $current_step "Failed to enter aamp directory"
    fi
    
    if ! git checkout dev_sprint_25_2; then
        handle_error $current_step "Failed to checkout dev_sprint_25_2 branch"
    fi
    print_status "Successfully checked out dev_sprint_25_2 branch"
    save_state
else
    skip_step $current_step "Switch to aamp and checkout dev_sprint_25_2"
fi

# Step 8: Pull latest changes from dev_sprint_25_2
current_step=8
if should_execute_step $current_step; then
    print_step $current_step "Pull latest changes from dev_sprint_25_2"
    # Ensure we're in the right directory
    if [ ! -d ".git" ] && [ -d "../aamp" ]; then
        cd ./aamp
    fi
    
    if ! git pull; then
        handle_error $current_step "Failed to pull latest changes from dev_sprint_25_2"
    fi
    print_status "Successfully pulled latest changes"
    save_state
else
    skip_step $current_step "Pull latest changes from dev_sprint_25_2"
fi

# Step 9: Get latest aamp commit ID
current_step=9
if should_execute_step $current_step; then
    print_step $current_step "Get latest aamp commit ID"
    # Ensure we're in aamp directory
    if [ ! -d ".git" ]; then
        if [ -d "./aamp" ]; then
            cd ./aamp
        else
            handle_error $current_step "Not in a git repository and can't find aamp directory"
        fi
    fi
    
    latest_aamp_commit=$(git log -1 --format="%H")
    latest_aamp_short_commit=$(git log -1 --format="%h")
    
    if [ -z "$latest_aamp_commit" ]; then
        handle_error $current_step "Failed to get latest aamp commit ID"
    fi
    
    print_status "Latest aamp commit: $latest_aamp_commit"
    print_status "Latest aamp commit (short): $latest_aamp_short_commit"
    save_state
else
    skip_step $current_step "Get latest aamp commit ID"
fi

# Step 10: Compare commits and decide if sync is needed
current_step=10
if should_execute_step $current_step; then
    print_step $current_step "Compare last synced commit with latest commit"
    
    if [ "$last_synced_aamp_srcrev" = "$latest_aamp_commit" ]; then
        print_status "No new changes found - aamp already up to date"
        print_status "Last synced: $last_synced_aamp_srcrev"
        print_status "Latest:      $latest_aamp_commit"
        no_new_changes=true
        
        # Clean up state file and exit successfully
        if [ -f "$STATE_FILE" ]; then
            rm "$STATE_FILE"
        fi
        
        echo ""
        echo "============================================"
        echo -e "${GREEN}SUCCESS: No sync needed - already up to date!${NC}"
        echo "============================================"
        exit 0
    else
        print_status "New changes detected - sync required"
        print_status "Last synced: $last_synced_aamp_srcrev"
        print_status "Latest:      $latest_aamp_commit"
        no_new_changes=false
    fi
    save_state
else
    skip_step $current_step "Compare last synced commit with latest commit"
fi

# Step 11: Generate patch file
current_step=11
if should_execute_step $current_step; then
    print_step $current_step "Generate patch file between commits"
    
    # Create patch filename
    patch_filename="${last_synced_aamp_srcrev}_${latest_aamp_commit}.patch"
    
    # Ensure we're in aamp directory
    if [ ! -d ".git" ]; then
        if [ -d "./aamp" ]; then
            cd ./aamp
        else
            handle_error $current_step "Not in a git repository and can't find aamp directory"
        fi
    fi
    
    if ! git diff $last_synced_aamp_srcrev > "../$patch_filename"; then
        handle_error $current_step "Failed to generate patch file"
    fi
    
    # Check if patch file was created and has content
    if [ ! -f "../$patch_filename" ]; then
        handle_error $current_step "Patch file was not created"
    fi
    
    # Check patch file size
    patch_size=$(wc -c < "../$patch_filename")
    if [ $patch_size -eq 0 ]; then
        print_status "Patch file is empty - no changes found"
        rm "../$patch_filename"
        no_new_changes=true
    else
        print_status "Patch file created: $patch_filename ($patch_size bytes)"
    fi
    save_state
else
    skip_step $current_step "Generate patch file between commits"
fi

# Step 12: Return to parent directory
current_step=12
if should_execute_step $current_step; then
    print_step $current_step "Return to parent directory"
    if ! cd ../; then
        handle_error $current_step "Failed to return to parent directory"
    fi
    save_state
else
    skip_step $current_step "Return to parent directory"
fi

# Step 13: Filter patch for middleware components
current_step=13
if should_execute_step $current_step; then
    print_step $current_step "Filter patch for middleware components"
    
    if [ "$no_new_changes" = true ]; then
        print_status "Skipping patch filtering - no new changes"
    else
        # Create filtered patch filename
        filtered_patch_filename="${last_synced_aamp_srcrev}_${latest_aamp_commit}_mid.patch"
        
        # Check if filter script exists in middleware-player-interface/sync/
        filter_script="./filter_middleware_patch.py"
        if [ ! -f "$filter_script" ]; then
            handle_error $current_step "filter_middleware_patch.py not found in middleware-player-interface/sync/"
        fi
        
        # Run the filter script
        if ! python3 "$filter_script" "./$patch_filename" "./$filtered_patch_filename"; then
            handle_error $current_step "Failed to filter patch file"
        fi
        
        # Check if filtered patch was created
        if [ ! -f "$filtered_patch_filename" ]; then
            handle_error $current_step "Filtered patch file was not created"
        fi
        
        print_status "Filtered patch created: $filtered_patch_filename"
    fi
    save_state
else
    skip_step $current_step "Filter patch for middleware components"
fi

# Step 14: Analyze filtered patch to determine if middleware changes exist
current_step=14
if should_execute_step $current_step; then
    print_step $current_step "Analyze filtered patch for middleware changes"
    
    if [ "$no_new_changes" = true ]; then
        print_status "Skipping patch analysis - no new changes"
        MID_PATCH_REQUIRED=false
        middleware_sections=0
    else
        # Count middleware diff sections in the filtered patch
        if [ -f "$filtered_patch_filename" ]; then
            middleware_sections=$(grep -c "^diff --git" "$filtered_patch_filename" 2>/dev/null || echo "0")
            
            if [ "$middleware_sections" -gt 0 ]; then
                MID_PATCH_REQUIRED=true
                print_status "Middleware changes found: $middleware_sections diff sections"
                print_status "Middleware patch application required: true"
            else
                MID_PATCH_REQUIRED=false
                print_status "No middleware changes found in patch"
                print_status "Middleware patch application required: false"
            fi
        else
            handle_error $current_step "Filtered patch file not found for analysis"
        fi
    fi
    save_state
else
    skip_step $current_step "Analyze filtered patch for middleware changes"
fi

# Step 15: Remove original patch file
current_step=15
if should_execute_step $current_step; then
    print_step $current_step "Remove original patch file"
    
    if [ "$no_new_changes" = true ]; then
        print_status "Skipping patch file cleanup - no patch was created"
    else
        if [ -f "$patch_filename" ]; then
            rm "$patch_filename"
            print_status "Removed original patch file: $patch_filename"
        else
            print_warning "Original patch file not found: $patch_filename"
        fi
    fi
    save_state
else
    skip_step $current_step "Remove original patch file"
fi

# Step 16: Move filtered patch to middleware-player-interface directory
current_step=16
if should_execute_step $current_step; then
    print_step $current_step "Move filtered patch to middleware-player-interface directory"
    
    if [ "$no_new_changes" = true ] || [ "$MID_PATCH_REQUIRED" = false ]; then
        print_status "Skipping patch move - no middleware changes to apply"
        if ! cd ../; then
            handle_error $current_step "Failed to enter middleware-player-interface directory"
        fi
    else
        if [ -f "$filtered_patch_filename" ]; then
            if ! cd ../; then
                handle_error $current_step "Failed to enter middleware-player-interface directory"
            fi
            
            if ! mv "./sync/$filtered_patch_filename" ./; then
                handle_error $current_step "Failed to move filtered patch file"
            fi
            
            print_status "Moved filtered patch to middleware-player-interface: $filtered_patch_filename"
        else
            handle_error $current_step "Filtered patch file not found: $filtered_patch_filename"
        fi
    fi
    save_state
else
    skip_step $current_step "Move filtered patch to middleware-player-interface directory"
fi

# Step 17: Apply filtered patch
current_step=17
if should_execute_step $current_step; then
    print_step $current_step "Apply filtered patch to middleware-player-interface"

    if [ "$no_new_changes" = true ] || [ "$MID_PATCH_REQUIRED" = false ]; then
        print_status "Skipping patch application - no middleware changes to apply"
        # Still need to navigate to middleware-player-interface for subsequent steps
        if [ ! -f "aamp_sync_cid.txt" ]; then
            if ! cd ../; then
                handle_error $current_step "Failed to navigate to middleware-player-interface directory"
            fi
        fi
    else
        # Ensure we're in middleware-player-interface directory
        if [ ! -f "aamp_sync_cid.txt" ]; then
            handle_error $current_step "Not in middleware-player-interface directory"
        fi
        
        if [ -f "$filtered_patch_filename" ]; then
            if ! git apply "$filtered_patch_filename"; then
                handle_error $current_step "Failed to apply filtered patch"
            fi
            print_status "Successfully applied filtered patch"
        else
            handle_error $current_step "Filtered patch file not found in middleware directory: $filtered_patch_filename"
        fi
    fi
    save_state
else
    skip_step $current_step "Apply filtered patch to middleware-player-interface"
fi

# Step 18: Remove filtered patch file
current_step=18
if should_execute_step $current_step; then
    print_step $current_step "Remove filtered patch file"
    
    if [ "$no_new_changes" = true ] || [ "$MID_PATCH_REQUIRED" = false ]; then
        print_status "Skipping patch cleanup - no patch to remove"
    else
        # Ensure we're in middleware-player-interface directory
        if [ ! -f "aamp_sync_cid.txt" ]; then
            handle_error $current_step "Not in middleware-player-interface directory"
        fi
        
        if [ -f "$filtered_patch_filename" ]; then
            rm "$filtered_patch_filename"
            print_status "Removed filtered patch file: $filtered_patch_filename"
        else
            print_warning "Filtered patch file not found for cleanup: $filtered_patch_filename"
        fi
    fi
    save_state
else
    skip_step $current_step "Remove filtered patch file"
fi

# Step 19: Update aamp_sync_cid.txt with latest commit
current_step=19
if should_execute_step $current_step; then
    print_step $current_step "Update aamp_sync_cid.txt with latest aamp commit"
    
    # Ensure we're in middleware-player-interface directory
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "Not in middleware-player-interface directory"
    fi
    
    if ! echo "$latest_aamp_commit" > aamp_sync_cid.txt; then
        handle_error $current_step "Failed to update aamp_sync_cid.txt"
    fi
    
    print_status "Updated aamp_sync_cid.txt with latest commit: $latest_aamp_commit"
    save_state
else
    skip_step $current_step "Update aamp_sync_cid.txt with latest aamp commit"
fi

# Step 20: Add all changes
current_step=20
if should_execute_step $current_step; then
    print_step $current_step "Add all changes to git"
    
    # Ensure we're in middleware-player-interface directory
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "Not in middleware-player-interface directory"
    fi
    
    if ! git add --all; then
        handle_error $current_step "Failed to add changes to git"
    fi
    
    print_status "Successfully added all changes to git"
    save_state
else
    skip_step $current_step "Add all changes to git"
fi

# Step 21: Commit changes
current_step=21
if should_execute_step $current_step; then
    print_step $current_step "Commit changes to git"
    
    # Ensure we're in middleware-player-interface directory
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "Not in middleware-player-interface directory"
    fi
    
    if ! git commit -m "$commit_message"; then
        # Check if there are actually changes to commit
        if git diff --cached --quiet; then
            print_status "No changes to commit - repository is already up to date"
        else
            handle_error $current_step "Failed to commit changes"
        fi
    else
        print_status "Successfully committed changes with message: $commit_message"
    fi
    save_state
else
    skip_step $current_step "Commit changes to git"
fi

# Step 22: Push changes to feature branch
current_step=22
if should_execute_step $current_step; then
    print_step $current_step "Push changes to feature/dev_sprint_pli"
    
    # Ensure we're in middleware-player-interface directory
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "Not in middleware-player-interface directory"
    fi
    
    if ! git push origin feature/dev_sprint_pli; then
        handle_error $current_step "Failed to push changes to feature/dev_sprint_pli"
    fi
    
    print_status "Successfully pushed changes to feature/dev_sprint_pli"
    save_state
else
    skip_step $current_step "Push changes to feature/dev_sprint_pli"
fi

# Step 23: Get final middleware commit ID
current_step=23
if should_execute_step $current_step; then
    print_step $current_step "Get final middleware commit ID"
    
    # Ensure we're in middleware-player-interface directory
    if [ ! -f "aamp_sync_cid.txt" ]; then
        handle_error $current_step "Not in middleware-player-interface directory"
    fi
    
    middleware_final_commit=$(git log -1 --format="%H")
    middleware_final_short_commit=$(git log -1 --format="%h")
    
    if [ -z "$middleware_final_commit" ]; then
        handle_error $current_step "Failed to get final middleware commit ID"
    fi
    
    print_status "Final middleware commit: $middleware_final_commit"
    print_status "Final middleware commit (short): $middleware_final_short_commit"
    save_state
else
    skip_step $current_step "Get final middleware commit ID"
fi

# Step 24: Generate final report and return to sync directory
current_step=24
if should_execute_step $current_step; then
    print_step $current_step "Generate final sync report and return to sync directory"
    
    # Return to sync directory
    if ! cd ./sync; then
        handle_error $current_step "Failed to return to sync directory"
    fi
    
    # Clean up state file on successful completion
    if [ -f "$STATE_FILE" ]; then
        rm "$STATE_FILE"
        print_status "Removed state file $STATE_FILE after successful completion"
    fi
    
    # Success message and report
    echo ""
    echo "============================================"
    echo -e "${GREEN}SUCCESS: AAMP to Middleware sync completed!${NC}"
    echo "============================================"
    echo ""
    echo "Summary of operations:"
    if [ "$no_new_changes" = true ]; then
        echo "- No new changes to sync - aamp repository already up to date"
        echo "- Last synced commit: $last_synced_aamp_srcrev"
        echo "- Latest aamp commit: $latest_aamp_commit"
    else
        echo "- Synced aamp changes from $last_synced_aamp_srcrev to $latest_aamp_commit"
        if [ "$MID_PATCH_REQUIRED" = true ]; then
            echo "- Applied middleware changes to middleware-player-interface repository"
            echo "- Found and processed $middleware_sections middleware diff sections"
        else
            echo "- No middleware-specific changes found in patch - only tracking file updated"
        fi
        echo "- Updated aamp_sync_cid.txt to track latest sync: $latest_aamp_commit"
        echo "- Committed and pushed changes to feature/dev_sprint_pli"
    fi
    
    echo ""
    echo "Repository status:"
    echo "- Last synced aamp commit: $last_synced_aamp_srcrev"
    echo "- Latest aamp commit: $latest_aamp_commit"
    echo "- Final middleware commit: $middleware_final_commit"
    
    echo ""
    echo "Patch processing:"
    if [ "$no_new_changes" = false ]; then
        echo "- Generated patch: $patch_filename"
        echo "- Filtered middleware patch: $filtered_patch_filename"
        echo "- Middleware sections found: $middleware_sections"
        echo "- Patch application required: $MID_PATCH_REQUIRED"
    else
        echo "- No patch generated - repositories already synchronized"
    fi
    
    echo ""
    echo "Sync completed on: $current_date at $current_time"
    echo "============================================"
    save_state
else
    skip_step $current_step "Generate final sync report and return to sync directory"
fi

# Final success message
echo ""
echo -e "${GREEN}AAMP to Middleware synchronization completed successfully!${NC}"
echo "You are now back in the middleware-player-interface/sync directory."