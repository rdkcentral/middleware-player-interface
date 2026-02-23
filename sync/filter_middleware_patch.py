#!/usr/bin/env python3
"""
Script to filter a patch file to include only middleware-related changes
and adjust paths for application within the middleware directory.
"""

import re
import sys
from typing import List, TextIO

def process_patch_file(input_file: str, output_file: str) -> None:
    """
    Process the patch file to:
    1. Remove header information (everything before first "diff --git")
    2. Keep only diff sections for middleware folder
    3. Adjust paths to be relative to middleware directory
    """
    
    with open(input_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # Find the first "diff --git" line
    start_index = None
    for i, line in enumerate(lines):
        if line.startswith('diff --git'):
            start_index = i
            break
    
    if start_index is None:
        print("No 'diff --git' found in the patch file")
        return
    
    # Process from first diff onwards
    filtered_lines = []
    i = start_index
    
    while i < len(lines):
        line = lines[i]
        
        # Check if this is a diff line for middleware
        if line.startswith('diff --git'):
            # Check if it's a middleware path
            middleware_match = re.match(r'diff --git a/middleware/(.*) b/middleware/(.*)', line)
            if middleware_match:
                # This is a middleware diff section, process it
                section_lines = collect_diff_section(lines, i)
                processed_section = process_middleware_section(section_lines, middleware_match.group(1), middleware_match.group(2))
                filtered_lines.extend(processed_section)
                i += len(section_lines)
            else:
                # This is not a middleware diff, skip to next diff
                i = skip_to_next_diff(lines, i + 1)
        else:
            i += 1
    
    # Write the filtered content
    with open(output_file, 'w', encoding='utf-8') as f:
        f.writelines(filtered_lines)
    
    print(f"Filtered patch written to: {output_file}")
    print(f"Processed {len([line for line in filtered_lines if line.startswith('diff --git')])} middleware diff sections")

def collect_diff_section(lines: List[str], start_index: int) -> List[str]:
    """
    Collect all lines belonging to a single diff section.
    """
    section_lines = [lines[start_index]]  # Include the diff --git line
    i = start_index + 1
    
    while i < len(lines):
        line = lines[i]
        # If we hit another "diff --git", we've reached the next section
        if line.startswith('diff --git'):
            break
        section_lines.append(line)
        i += 1
    
    return section_lines

def skip_to_next_diff(lines: List[str], start_index: int) -> int:
    """
    Skip to the next "diff --git" line or end of file.
    """
    for i in range(start_index, len(lines)):
        if lines[i].startswith('diff --git'):
            return i
    return len(lines)

def process_middleware_section(section_lines: List[str], path_a: str, path_b: str) -> List[str]:
    """
    Process a middleware diff section to adjust paths.
    """
    processed_lines = []
    
    for line in section_lines:
        if line.startswith('diff --git'):
            # Adjust the diff --git line
            new_line = f"diff --git a/{path_a} b/{path_b}\n"
            processed_lines.append(new_line)
        elif line.startswith('--- a/middleware/'):
            # Adjust the --- line
            new_path = line.replace('--- a/middleware/', '--- a/')
            processed_lines.append(new_path)
        elif line.startswith('+++ b/middleware/'):
            # Adjust the +++ line
            new_path = line.replace('+++ b/middleware/', '+++ b/')
            processed_lines.append(new_path)
        else:
            # Keep all other lines as-is
            processed_lines.append(line)
    
    return processed_lines

def main():
    """
    Main function to run the patch filter.
    """
    if len(sys.argv) != 3:
        print("Usage: python filter_middleware_patch.py <input_patch_file> <output_patch_file>")
        print("Example: python filter_middleware_patch.py sync101225.patch middleware_only.patch")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    try:
        process_patch_file(input_file, output_file)
    except FileNotFoundError:
        print(f"Error: Input file '{input_file}' not found")
        sys.exit(1)
    except Exception as e:
        print(f"Error processing patch file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()