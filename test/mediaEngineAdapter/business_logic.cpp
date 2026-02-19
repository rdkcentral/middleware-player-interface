#include "api_interface.h"
#include <fcntl.h>      // For open(), O_WRONLY
#include <unistd.h>     // For dup2(), close()
#include <cstdio>
#include <iostream>
#include <sstream>
#include "InterfacePlayerRDK.h"

// Pure C++ Business Logic Implementation
// No JSON, no HTTP - just pure logic

/**
 * @brief Constructor - Initialize player and set up logging
 */
BusinessLogic::BusinessLogic() 
    : playerInstance(new InterfacePlayerRDK()),
      currentState(PlayerState::IDLE),
      currentManifestUrl(""),
      isPipelineConfigured(false),
      currentVolume(100)
{
    // Redirect stdout to log file for debugging
    freopen("/tmp/player_if.log", "w", stdout);
    std::cout << "[INFO] MediaEngineAdapter initialized" << std::endl;
    
    // Initialize player
    InitializePlayer();
}

/**
 * @brief Destructor - Clean up resources
 */
BusinessLogic::~BusinessLogic()
{
    if (playerInstance) {
        if (currentState == PlayerState::PLAYING || currentState == PlayerState::PAUSED) {
            playerInstance->Stop(false);
        }
        delete playerInstance;
        playerInstance = nullptr;
    }
    std::cout << "[INFO] MediaEngineAdapter destroyed" << std::endl;
}

/**
 * @brief Initialize the player instance and register callbacks
 */
void BusinessLogic::InitializePlayer()
{
    std::cout << "[INFO] Initializing player..." << std::endl;
    
    // Set player name for identification
    playerInstance->SetPlayerName("MediaEngineAdapter_Player");
    
    // Register callbacks for player events
    RegisterCallbacks();
    
    currentState = PlayerState::IDLE;
    std::cout << "[INFO] Player initialization complete" << std::endl;
}

/**
 * @brief Register callbacks for GStreamer events
 */
void BusinessLogic::RegisterCallbacks()
{
    // Register bus message callback
    playerInstance->RegisterBusEvent([this](const BusEventData busEvent) {
        this->HandleBusMessage(busEvent);
    });
    
    // Register buffer underflow callback
    playerInstance->RegisterBufferUnderflowCb([this](int mediaType) {
        this->HandleBufferUnderflow(mediaType);
    });
    
    // Register decode error callback
    playerInstance->RegisterGstDecodeErrorCb([this](int errorCount) {
        this->HandleDecodeError(errorCount);
    });
    
    // Register buffering timeout callback
    playerInstance->RegisterBufferingTimeoutCb([this](bool timeoutMet, bool rateCorrection, bool isReady) {
        std::cout << "[INFO] Buffering timeout: " << timeoutMet << std::endl;
    });
    
    std::cout << "[INFO] Callbacks registered" << std::endl;
}

/**
 * @brief Handle GStreamer bus messages
 */
void BusinessLogic::HandleBusMessage(const BusEventData busEvent)
{
    switch(busEvent.msgType) {
        case MESSAGE_ERROR:
            std::cout << "[ERROR] GStreamer error: " << busEvent.msg << std::endl;
            if (!busEvent.dbg_info.empty()) {
                std::cout << "[ERROR] Debug info: " << busEvent.dbg_info << std::endl;
            }
            currentState = PlayerState::ERROR;
            break;
            
        case MESSAGE_WARNING:
            std::cout << "[WARN] GStreamer warning: " << busEvent.msg << std::endl;
            break;
            
        case MESSAGE_EOS:
            std::cout << "[INFO] End of stream reached" << std::endl;
            currentState = PlayerState::STOPPED;
            break;
            
        case MESSAGE_STATE_CHANGE:
            std::cout << "[INFO] State change: " << busEvent.msg << std::endl;
            break;
            
        default:
            break;
    }
}

/**
 * @brief Handle buffer underflow events
 */
void BusinessLogic::HandleBufferUnderflow(int mediaType)
{
    std::cout << "[WARN] Buffer underflow for media type: " << mediaType << std::endl;
}

/**
 * @brief Handle decode errors
 */
void BusinessLogic::HandleDecodeError(int errorCount)
{
    std::cout << "[ERROR] Decode error count: " << errorCount << std::endl;
    currentState = PlayerState::ERROR;
}

/**
 * @brief Configure the GStreamer pipeline
 */
void BusinessLogic::ConfigurePipeline()
{
    if (isPipelineConfigured) {
        std::cout << "[INFO] Pipeline already configured" << std::endl;
        return;
    }
    
    std::cout << "[INFO] Configuring pipeline for URL: " << currentManifestUrl << std::endl;
    
    // Configure pipeline with default video/audio formats
    // Format: 0=Unknown, 1=ISO_BMFF (MP4), 2=MPEG_TS, etc.
    int videoFormat = 1;  // ISO_BMFF for most modern streams
    int audioFormat = 1;  // ISO_BMFF
    int subFormat = 0;    // No subtitles initially
    bool bESChangeStatus = false;
    bool setReadyAfterPipelineCreation = true;
    bool isSubEnable = false;
    int32_t trackId = 0;
    int rate = 1;  // Normal playback rate
    const char* pipelineName = "MediaEngineAdapterPipeline";
    int pipelinePriority = -1;  // Default priority
    bool firstFrameFlag = true;
    
    playerInstance->ConfigurePipeline(
        videoFormat, audioFormat, subFormat,
        bESChangeStatus, setReadyAfterPipelineCreation,
        isSubEnable, trackId, rate, pipelineName, pipelinePriority,
        firstFrameFlag, currentManifestUrl.c_str()
    );
    
    isPipelineConfigured = true;
    currentState = PlayerState::READY;
    std::cout << "[INFO] Pipeline configured successfully" << std::endl;
}

// ========== Public API Implementation ==========

/**
 * @brief Load a manifest URL for playback
 */
std::string BusinessLogic::LoadManifest(std::string url)
{
    std::cout << "[INFO] Loading manifest: " << url << std::endl;
    
    if (url.empty()) {
        return "Error: Empty URL provided";
    }
    
    // Stop current playback if running
    if (currentState == PlayerState::PLAYING || currentState == PlayerState::PAUSED) {
        Stop();
    }
    
    currentManifestUrl = url;
    currentState = PlayerState::INITIALIZING;
    
    try {
        // Configure the pipeline with the new manifest
        ConfigurePipeline();
        return "Success: Manifest loaded - " + url;
    } catch (const std::exception& e) {
        currentState = PlayerState::ERROR;
        return "Error: Failed to load manifest - " + std::string(e.what());
    }
}

/**
 * @brief Start or resume playback
 */
std::string BusinessLogic::Play()
{
    std::cout << "[INFO] Play requested, current state: " << static_cast<int>(currentState) << std::endl;
    
    if (currentState == PlayerState::IDLE || currentState == PlayerState::ERROR) {
        return "Error: No manifest loaded. Call LoadManifest first";
    }
    
    if (currentState == PlayerState::PLAYING) {
        return "Already playing";
    }
    
    try {
        // Resume from pause or start playback
        bool pause = false;  // false means play
        bool forceStopBuffering = false;
        
        playerInstance->Pause(pause, forceStopBuffering);
        currentState = PlayerState::PLAYING;
        
        std::cout << "[INFO] Playback started" << std::endl;
        return "Success: Playback started";
    } catch (const std::exception& e) {
        currentState = PlayerState::ERROR;
        return "Error: Failed to start playback - " + std::string(e.what());
    }
}

/**
 * @brief Pause playback
 */
std::string BusinessLogic::Pause()
{
    std::cout << "[INFO] Pause requested" << std::endl;
    
    if (currentState != PlayerState::PLAYING) {
        return "Error: Not currently playing";
    }
    
    try {
        bool pause = true;  // true means pause
        bool forceStopBuffering = false;
        
        playerInstance->Pause(pause, forceStopBuffering);
        currentState = PlayerState::PAUSED;
        
        std::cout << "[INFO] Playback paused" << std::endl;
        return "Success: Playback paused";
    } catch (const std::exception& e) {
        return "Error: Failed to pause - " + std::string(e.what());
    }
}

/**
 * @brief Stop playback
 */
std::string BusinessLogic::Stop()
{
    std::cout << "[INFO] Stop requested" << std::endl;
    
    if (currentState == PlayerState::IDLE || currentState == PlayerState::STOPPED) {
        return "Already stopped";
    }
    
    try {
        bool keepLastFrame = false;
        playerInstance->Stop(keepLastFrame);
        
        currentState = PlayerState::STOPPED;
        isPipelineConfigured = false;
        
        std::cout << "[INFO] Playback stopped" << std::endl;
        return "Success: Playback stopped";
    } catch (const std::exception& e) {
        return "Error: Failed to stop - " + std::string(e.what());
    }
}

/**
 * @brief Seek to a specific position
 */
std::string BusinessLogic::Seek(double positionSeconds)
{
    std::cout << "[INFO] Seek requested to position: " << positionSeconds << " seconds" << std::endl;
    
    if (currentState != PlayerState::PLAYING && currentState != PlayerState::PAUSED) {
        return "Error: Must be in playing or paused state to seek";
    }
    
    try {
        playerInstance->SetSeekPosition(positionSeconds);
        
        // Flush the pipeline to the new position
        double rate = 1.0;  // Normal playback rate
        bool shouldTearDown = false;
        bool isAppSeek = true;
        
        playerInstance->Flush(positionSeconds, rate, shouldTearDown, isAppSeek);
        
        std::stringstream ss;
        ss << "Success: Seeked to " << positionSeconds << " seconds";
        return ss.str();
    } catch (const std::exception& e) {
        return "Error: Failed to seek - " + std::string(e.what());
    }
}

/**
 * @brief Get current playback position
 */
long long BusinessLogic::GetPosition()
{
    if (currentState == PlayerState::IDLE || !isPipelineConfigured) {
        return 0;
    }
    
    try {
        long long position = playerInstance->GetPositionMilliseconds();
        return position;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Failed to get position: " << e.what() << std::endl;
        return 0;
    }
}

/**
 * @brief Get total duration
 */
long BusinessLogic::GetDuration()
{
    if (currentState == PlayerState::IDLE || !isPipelineConfigured) {
        return 0;
    }
    
    try {
        long duration = playerInstance->GetDurationMilliseconds();
        return duration;
    } catch (const std::exception& e) {
        std::cout << "[ERROR] Failed to get duration: " << e.what() << std::endl;
        return 0;
    }
}

/**
 * @brief Set audio volume
 */
std::string BusinessLogic::SetVolume(int volume)
{
    if (volume < 0 || volume > 100) {
        return "Error: Volume must be between 0 and 100";
    }
    
    try {
        playerInstance->SetAudioVolume(volume);
        currentVolume = volume;
        
        std::stringstream ss;
        ss << "Success: Volume set to " << volume;
        return ss.str();
    } catch (const std::exception& e) {
        return "Error: Failed to set volume - " + std::string(e.what());
    }
}

/**
 * @brief Set video rectangle for display
 */
std::string BusinessLogic::SetVideoRectangle(int x, int y, int w, int h)
{
    try {
        playerInstance->SetVideoRectangle(x, y, w, h);
        
        std::stringstream ss;
        ss << "Success: Video rectangle set to (" << x << "," << y << "," << w << "," << h << ")";
        return ss.str();
    } catch (const std::exception& e) {
        return "Error: Failed to set video rectangle - " + std::string(e.what());
    }
}

/**
 * @brief Set video mute state
 */
std::string BusinessLogic::SetVideoMute(bool muted)
{
    try {
        playerInstance->SetVideoMute(muted);
        return muted ? "Success: Video muted" : "Success: Video unmuted";
    } catch (const std::exception& e) {
        return "Error: Failed to set video mute - " + std::string(e.what());
    }
}

/**
 * @brief Get video dimensions
 */
std::string BusinessLogic::GetVideoSize()
{
    try {
        int width = 0, height = 0;
        playerInstance->GetVideoSize(width, height);
        
        std::stringstream ss;
        ss << "Width: " << width << ", Height: " << height;
        return ss.str();
    } catch (const std::exception& e) {
        return "Error: Failed to get video size - " + std::string(e.what());
    }
}

/**
 * @brief Set playback rate
 */
std::string BusinessLogic::SetPlaybackRate(double rate)
{
    if (rate <= 0) {
        return "Error: Rate must be greater than 0";
    }
    
    try {
        bool success = playerInstance->SetPlayBackRate(rate);
        
        if (success) {
            std::stringstream ss;
            ss << "Success: Playback rate set to " << rate << "x";
            return ss.str();
        } else {
            return "Error: Failed to set playback rate";
        }
    } catch (const std::exception& e) {
        return "Error: Failed to set playback rate - " + std::string(e.what());
    }
}

/**
 * @brief Set player name for identification
 */
void BusinessLogic::SetPlayerName(std::string playerName)
{
    int fd = open("/dev/tty", O_WRONLY);
    const char* test = "[DEBUG] SetPlayerName called\n";
    write(fd, test, strlen(test));
    close(fd);
    
    playerInstance->SetPlayerName(playerName);
    std::cout << "[INFO] Player name set to: " << playerName << std::endl;
}

/**
 * @brief Enable GStreamer debug logging
 */
std::string BusinessLogic::EnableDebugLogging(std::string debugLevel)
{
    try {
        playerInstance->EnableGstDebugLogging(debugLevel);
        return "Success: Debug logging enabled at level " + debugLevel;
    } catch (const std::exception& e) {
        return "Error: Failed to enable debug logging - " + std::string(e.what());
    }
}

/**
 * @brief Get player status
 */
std::string BusinessLogic::GetStatus()
{
    std::stringstream ss;
    
    ss << "Player Status:\n";
    ss << "  State: ";
    switch(currentState) {
        case PlayerState::IDLE: ss << "IDLE"; break;
        case PlayerState::INITIALIZING: ss << "INITIALIZING"; break;
        case PlayerState::READY: ss << "READY"; break;
        case PlayerState::PLAYING: ss << "PLAYING"; break;
        case PlayerState::PAUSED: ss << "PAUSED"; break;
        case PlayerState::STOPPED: ss << "STOPPED"; break;
        case PlayerState::ERROR: ss << "ERROR"; break;
    }
    ss << "\n";
    
    if (!currentManifestUrl.empty()) {
        ss << "  URL: " << currentManifestUrl << "\n";
    }
    
    if (isPipelineConfigured) {
        long long position = GetPosition();
        long duration = GetDuration();
        ss << "  Position: " << position << " ms\n";
        ss << "  Duration: " << duration << " ms\n";
    }
    
    ss << "  Volume: " << currentVolume << "\n";
    
    return ss.str();
}

// ========== Example/Test API Implementation ==========

std::string BusinessLogic::hello(std::string name) {
    return "Hello " + name + "! Welcome to the system.";
}

std::string BusinessLogic::bye(std::string name) {
    return "Goodbye " + name + "! See you later.";
}

int BusinessLogic::calculate(int a, int b) {
    // Complex business logic here
    return a + b + 10; // Some processing
}

std::string BusinessLogic::processUser(std::string username, int age, bool isActive) {
    std::stringstream ss;
    ss << "Processing user: " << username 
       << ", Age: " << age 
       << ", Status: " << (isActive ? "Active" : "Inactive");
    return ss.str();
}

void BusinessLogic::internalHelper() {
    // This won't be exposed as API
}
