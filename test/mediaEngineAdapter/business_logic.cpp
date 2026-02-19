#include "api_interface.h"
#include "http_client.h"
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

/**
 * @brief Constructor - Initialize player and set up logging
 */
BusinessLogic::BusinessLogic() 
    : playerInstance(new InterfacePlayerRDK()),
      currentState(PlayerState::IDLE),
      currentManifestUrl(""),
      currentMediaPlaylistUrl(""),
      isPipelineConfigured(false),
      currentVolume(100),
      currentPlaylist(),
      currentSegmentIndex(0),
      currentPts(0.0),
      currentDts(0.0),
      fragmentBuffer(new FragmentBuffer()),
      fragmentCollectorThread(),
      fragmentInjectorThread(),
      threadsRunning(false),
      downloadEnabled(false),
      injectionEnabled(false),
      stateMutex()
{
    // Redirect stdout to log file for debugging
    freopen("/tmp/player_if.log", "w", stdout);
    std::cout << "[INFO] MediaEngineAdapter initialized with standalone architecture" << std::endl;
    
    // Initialize player
    InitializePlayer();
}

/**
 * @brief Destructor - Clean up resources
 */
BusinessLogic::~BusinessLogic()
{
    std::cout << "[INFO] MediaEngineAdapter shutting down..." << std::endl;
    
    // Stop threads first
    StopThreads();
    
    // Stop playback
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
    playerInstance->SetPlayerName("MediaEngineAdapter_StandalonePlayer");
    
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
    
    // Register first frame callback (required for pipeline configuration)
    playerInstance->FirstFrameCallback([this](int mediaType, bool notifyFirstBuffer, bool initCC, bool &requireFirstVideoFrameDisplay, bool &audioOnly) {
        std::cout << "[INFO] First frame received for media type: " << mediaType << std::endl;
        requireFirstVideoFrameDisplay = false;
        audioOnly = false;
    });
    
    // Register stop callback (required for pipeline management)
    playerInstance->StopCallback([this](bool keepLastFrame) {
        std::cout << "[INFO] Stop callback triggered, keepLastFrame: " << keepLastFrame << std::endl;
    });
    
    // Register teardown callback (required for cleanup)
    playerInstance->TearDownCallback([this](bool newTune, int mediaType) {
        std::cout << "[INFO] Teardown callback triggered for media type: " << mediaType << std::endl;
    });
    
    // Register NeedData callback - GStreamer needs more data
    playerInstance->NeedDataCallback([this](int mediaType) {
        std::cout << "[DEBUG] NeedData callback for mediaType: " << mediaType << std::endl;
        // Our downloader thread is already pushing data continuously
    });
    
    // Register EnoughData callback - GStreamer has enough data
    playerInstance->EnoughDataCallback([this](int mediaType) {
        std::cout << "[DEBUG] EnoughData callback for mediaType: " << mediaType << std::endl;
        // Could throttle downloads here if needed
    });
    
    std::cout << "[INFO] All callbacks registered" << std::endl;
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
            StopThreads();
            break;
            
        case MESSAGE_WARNING:
            std::cout << "[WARN] GStreamer warning: " << busEvent.msg << std::endl;
            break;
            
        case MESSAGE_EOS:
            std::cout << "[INFO] End of stream reached" << std::endl;
            currentState = PlayerState::STOPPED;
            StopThreads();
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
    std::cout << "[WARN] Buffer underflow for media type: " << mediaType 
              << " (buffered: " << fragmentBuffer->GetBufferedDuration(mediaType) << "s)" << std::endl;
}

/**
 * @brief Handle decode errors
 */
void BusinessLogic::HandleDecodeError(int errorCount)
{
    std::cout << "[ERROR] Decode error count: " << errorCount << std::endl;
    currentState = PlayerState::ERROR;
    StopThreads();
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
    
    std::cout << "[INFO] Configuring pipeline for manual fragment injection" << std::endl;
    
    // Configure pipeline for fragment injection (NOT URL-based playback)
    // We use FORMAT_MPEG_TS for HLS TS segments
    int videoFormat = 2;  // FORMAT_MPEG_TS
    int audioFormat = 2;  // FORMAT_MPEG_TS
    int subFormat = 0;    // No subtitles initially
    bool bESChangeStatus = false;
    bool setReadyAfterPipelineCreation = true;
    bool isSubEnable = false;
    int32_t trackId = 0;
    int rate = 1;  // Normal playback rate
    const char* pipelineName = "MediaEngineAdapterPipeline";
    int pipelinePriority = -1;  // Default priority
    bool firstFrameFlag = true;
    
    // IMPORTANT: Pass empty string for URL since we're injecting fragments manually
    playerInstance->ConfigurePipeline(
        videoFormat, audioFormat, subFormat,
        bESChangeStatus, setReadyAfterPipelineCreation,
        isSubEnable, trackId, rate, pipelineName, pipelinePriority,
        firstFrameFlag, ""  // Empty URL - we inject fragments manually
    );
    
    isPipelineConfigured = true;
    std::cout << "[INFO] Pipeline configured for fragment injection" << std::endl;
}

// ========== Manifest and Fragment Handling ==========

/**
 * @brief Download manifest from URL
 */
bool BusinessLogic::DownloadManifest(const std::string& url, std::string& content, std::string& errorMsg)
{
    std::cout << "[INFO] Downloading manifest: " << url << std::endl;
    
    int httpCode = HttpClient::DownloadText(url, content, errorMsg);
    
    if (httpCode != 200) {
        std::cout << "[ERROR] Failed to download manifest: HTTP " << httpCode << " - " << errorMsg << std::endl;
        return false;
    }
    
    std::cout << "[INFO] Manifest downloaded: " << content.length() << " bytes" << std::endl;
    return true;
}

/**
 * @brief Parse manifest and select variant
 */
bool BusinessLogic::ParseAndSelectVariant(const std::string& manifestUrl)
{
    std::string manifestContent;
    std::string errorMsg;
    
    if (!DownloadManifest(manifestUrl, manifestContent, errorMsg)) {
        return false;
    }
    
    // Check if master playlist
    if (HLSParser::IsMasterPlaylist(manifestContent)) {
        std::cout << "[INFO] Detected master playlist, selecting variant..." << std::endl;
        
        auto variants = HLSParser::ParseMasterPlaylist(manifestContent, manifestUrl);
        
        if (variants.empty()) {
            std::cout << "[ERROR] No variants found in master playlist" << std::endl;
            return false;
        }
        
        // Select highest bandwidth variant
        auto highestVariant = variants.rbegin();
        currentMediaPlaylistUrl = highestVariant->second;
        
        std::cout << "[INFO] Selected variant: bandwidth=" << highestVariant->first 
                  << " url=" << currentMediaPlaylistUrl << std::endl;
        
        // Download media playlist
        if (!DownloadManifest(currentMediaPlaylistUrl, manifestContent, errorMsg)) {
            return false;
        }
    } else {
        currentMediaPlaylistUrl = manifestUrl;
    }
    
    // Parse media playlist
    currentPlaylist = HLSParser::ParseMediaPlaylist(manifestContent, currentMediaPlaylistUrl);
    
    if (currentPlaylist.segments.empty()) {
        std::cout << "[ERROR] No segments found in media playlist" << std::endl;
        return false;
    }
    
    std::cout << "[INFO] Media playlist parsed: " << currentPlaylist.segments.size() << " segments" << std::endl;
    currentSegmentIndex = 0;
    
    return true;
}

// ========== Thread Functions ==========

/**
 * @brief Fragment Collector Thread - Downloads segments and buffers them
 */
void BusinessLogic::FragmentCollectorThreadFunc()
{
    std::cout << "[THREAD] Fragment Collector started" << std::endl;
    
    while (downloadEnabled.load()) {
        DownloadAndBufferSegments();
        
        // For live streams, periodically refresh playlist
        if (currentPlaylist.isLive) {
            std::this_thread::sleep_for(std::chrono::seconds(currentPlaylist.targetDuration / 2));
            
            // Re-download and parse playlist
            std::string manifestContent, errorMsg;
            if (DownloadManifest(currentMediaPlaylistUrl, manifestContent, errorMsg)) {
                currentPlaylist = HLSParser::ParseMediaPlaylist(manifestContent, currentMediaPlaylistUrl);
                std::cout << "[INFO] Playlist refreshed: " << currentPlaylist.segments.size() << " segments" << std::endl;
            }
        } else {
            // VOD - download all segments once
            break;
        }
    }
    
    std::cout << "[THREAD] Fragment Collector stopped" << std::endl;
}

/**
 * @brief Download and buffer segments
 */
void BusinessLogic::DownloadAndBufferSegments()
{
    while (downloadEnabled.load() && currentSegmentIndex < currentPlaylist.segments.size()) {
        const HLSSegment& segment = currentPlaylist.segments[currentSegmentIndex];
        
        // Check buffer level - don't download too far ahead
        double bufferedDuration = fragmentBuffer->GetBufferedDuration(0); // Video
        if (bufferedDuration > 30.0) { // Max 30 seconds buffer
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        
        std::cout << "[DOWNLOAD] Segment " << currentSegmentIndex << "/" << currentPlaylist.segments.size()
                  << " url=" << segment.url << std::endl;
        
        // Download segment
        std::vector<uint8_t> segmentData;
        std::string errorMsg;
        int httpCode = HttpClient::Download(segment.url, segmentData, errorMsg);
        
        if (httpCode != 200) {
            std::cout << "[ERROR] Failed to download segment: HTTP " << httpCode << " - " << errorMsg << std::endl;
            currentSegmentIndex++;
            continue;
        }
        
        std::cout << "[DOWNLOAD] Downloaded segment: " << segmentData.size() << " bytes" << std::endl;
        
        // Create fragment
        MediaFragment fragment;
        fragment.data = std::move(segmentData);
        fragment.duration = segment.duration;
        fragment.sequenceNumber = segment.sequenceNumber;
        fragment.isDiscontinuity = segment.isDiscontinuity;
        fragment.mediaType = 0; // Video (HLS muxed streams contain both audio/video)
        fragment.pts = currentPts;
        fragment.dts = currentDts;
        
        // Add to buffer (mediaType 0 = video/muxed)
        fragmentBuffer->Push(fragment, 0);
        
        std::cout << "[BUFFER] Buffered segment " << currentSegmentIndex 
                  << " pts=" << fragment.pts << " duration=" << fragment.duration << std::endl;
        
        // Update PTS/DTS
        currentPts += segment.duration;
        currentDts += segment.duration;
        
        currentSegmentIndex++;
    }
}

/**
 * @brief Fragment Injector Thread - Injects buffered segments into GStreamer
 */
void BusinessLogic::FragmentInjectorThreadFunc()
{
    std::cout << "[THREAD] Fragment Injector started" << std::endl;
    
    while (injectionEnabled.load()) {
        MediaFragment fragment;
        
        // Wait for fragment (blocking with 1s timeout)
        if (fragmentBuffer->Pop(fragment, 0, 1000)) {
            InjectFragment(fragment);
        }
    }
    
    std::cout << "[THREAD] Fragment Injector stopped" << std::endl;
}

/**
 * @brief Inject fragment into GStreamer pipeline
 */
void BusinessLogic::InjectFragment(const MediaFragment& fragment)
{
    if (!isPipelineConfigured) {
        std::cout << "[ERROR] Cannot inject - pipeline not configured" << std::endl;
        return;
    }
    
    std::cout << "[INJECT] Injecting fragment: seq=" << fragment.sequenceNumber 
              << " size=" << fragment.data.size() << " pts=" << fragment.pts 
              << " duration=" << fragment.duration << std::endl;
    
    // For muxed HLS streams, inject as video (contains both audio and video)
    bool success = InjectVideoSegment(fragment);
    
    if (!success) {
        std::cout << "[ERROR] Fragment injection failed" << std::endl;
    }
}

/**
 * @brief Inject video segment via SendHelper
 */
bool BusinessLogic::InjectVideoSegment(const MediaFragment& fragment)
{
    // Create MediaSample for InterfacePlayerRDK
    MediaSample sample;
    sample.data = const_cast<uint8_t*>(fragment.data.data());
    sample.dataLength = fragment.data.size();
    sample.pts = fragment.pts;
    sample.dts = fragment.dts;
    sample.duration = fragment.duration;
    sample.isInitSeg = fragment.isInitSegment;
    
    // Inject via SendHelper (mediaType: 0=Video, 1=Audio, 2=Subtitle)
    int mediaType = 0; // eMEDIATYPE_VIDEO
    bool initFragment = fragment.isInitSegment;
    bool discontinuity = fragment.isDiscontinuity;
    
    bool success = playerInstance->SendHelper(mediaType, std::move(sample), initFragment, discontinuity);
    
    if (success) {
        std::cout << "[INJECT] ✓ Video fragment injected successfully" << std::endl;
    } else {
        std::cout << "[INJECT] ✗ Video fragment injection failed" << std::endl;
    }
    
    return success;
}

/**
 * @brief Inject audio segment via SendHelper
 */
bool BusinessLogic::InjectAudioSegment(const MediaFragment& fragment)
{
    MediaSample sample;
    sample.data = const_cast<uint8_t*>(fragment.data.data());
    sample.dataLength = fragment.data.size();
    sample.pts = fragment.pts;
    sample.dts = fragment.dts;
    sample.duration = fragment.duration;
    sample.isInitSeg = fragment.isInitSegment;
    
    int mediaType = 1; // eMEDIATYPE_AUDIO
    bool initFragment = fragment.isInitSegment;
    bool discontinuity = fragment.isDiscontinuity;
    
    bool success = playerInstance->SendHelper(mediaType, std::move(sample), initFragment, discontinuity);
    
    if (success) {
        std::cout << "[INJECT] ✓ Audio fragment injected successfully" << std::endl;
    } else {
        std::cout << "[INJECT] ✗ Audio fragment injection failed" << std::endl;
    }
    
    return success;
}

// ========== Thread Management ==========

/**
 * @brief Start collector and injector threads
 */
void BusinessLogic::StartThreads()
{
    if (threadsRunning.load()) {
        std::cout << "[WARN] Threads already running" << std::endl;
        return;
    }
    
    std::cout << "[INFO] Starting downloader and injector threads..." << std::endl;
    
    fragmentBuffer->ResetAbort();
    downloadEnabled.store(true);
    injectionEnabled.store(true);
    threadsRunning.store(true);
    
    // Start fragment collector thread
    fragmentCollectorThread = std::thread(&BusinessLogic::FragmentCollectorThreadFunc, this);
    
    // Start fragment injector thread
    fragmentInjectorThread = std::thread(&BusinessLogic::FragmentInjectorThreadFunc, this);
    
    std::cout << "[INFO] Threads started" << std::endl;
}

/**
 * @brief Stop collector and injector threads
 */
void BusinessLogic::StopThreads()
{
    if (!threadsRunning.load()) {
        return;
    }
    
    std::cout << "[INFO] Stopping threads..." << std::endl;
    
    downloadEnabled.store(false);
    injectionEnabled.store(false);
    fragmentBuffer->Abort();
    
    if (fragmentCollectorThread.joinable()) {
        fragmentCollectorThread.join();
        std::cout << "[INFO] Fragment collector thread stopped" << std::endl;
    }
    
    if (fragmentInjectorThread.joinable()) {
        fragmentInjectorThread.join();
        std::cout << "[INFO] Fragment injector thread stopped" << std::endl;
    }
    
    threadsRunning.store(false);
    std::cout << "[INFO] All threads stopped" << std::endl;
}

// ========== Public API Implementation ==========

/**
 * @brief Load a manifest URL for playback
 */
std::string BusinessLogic::LoadManifest(std::string url)
{
    std::cout << "[API] LoadManifest called: " << url << std::endl;
    
    if (url.empty()) {
        return "Error: Empty URL provided";
    }
    
    // Stop current playback if running
    if (currentState == PlayerState::PLAYING || currentState == PlayerState::PAUSED) {
        std::cout << "[INFO] Stopping current playback before loading new manifest" << std::endl;
        Stop();
    }
    
    currentManifestUrl = url;
    currentState = PlayerState::INITIALIZING;
    currentPts = 0.0;
    currentDts = 0.0;
    currentSegmentIndex = 0;
    
    // Download and parse manifest
    if (!ParseAndSelectVariant(url)) {
        currentState = PlayerState::ERROR;
        return "Error: Failed to parse manifest";
    }
    
    // Configure pipeline
    try {
        ConfigurePipeline();
        currentState = PlayerState::READY;
        return "Success: Manifest loaded - " + url + " (" + 
               std::to_string(currentPlaylist.segments.size()) + " segments)";
    } catch (const std::exception& e) {
        std::cout << "[ERROR] ConfigurePipeline failed: " << e.what() << std::endl;
        currentState = PlayerState::ERROR;
        return "Error: Failed to configure pipeline - " + std::string(e.what());
    }
}

/**
 * @brief Start or resume playback
 */
std::string BusinessLogic::Play()
{
    std::cout << "[API] Play requested, current state: " << static_cast<int>(currentState) << std::endl;
    
    if (currentState == PlayerState::IDLE) {
        return "Error: No manifest loaded. Call LoadManifest first";
    }
    
    if (currentState == PlayerState::ERROR) {
        return "Error: Player in error state. Call LoadManifest to reset";
    }
    
    if (currentState == PlayerState::PLAYING) {
        return "Info: Already playing";
    }
    
    try {
        // Start threads if not already running
        if (!threadsRunning.load()) {
            StartThreads();
        }
        
        // Resume playback
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
    std::cout << "[API] Pause requested" << std::endl;
    
    if (currentState != PlayerState::PLAYING) {
        return "Error: Not currently playing. Current state: " + std::to_string(static_cast<int>(currentState));
    }
    
    try {
        bool pause = true;  // true means pause
        bool forceStopBuffering = false;
        
        playerInstance->Pause(pause, forceStopBuffering);
        currentState = PlayerState::PAUSED;
        
        // Don't stop threads - keep buffering in background
        
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
    std::cout << "[API] Stop requested" << std::endl;
    
    if (currentState == PlayerState::IDLE || currentState == PlayerState::STOPPED) {
        return "Already stopped";
    }
    
    try {
        // Stop threads
        StopThreads();
        
        // Stop pipeline
        bool keepLastFrame = false;
        playerInstance->Stop(keepLastFrame);
        
        currentState = PlayerState::STOPPED;
        isPipelineConfigured = false;
        
        // Clear buffer
        fragmentBuffer->Clear(-1);
        
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
    std::cout << "[API] Seek requested to position: " << positionSeconds << " seconds" << std::endl;
    
    if (currentState != PlayerState::PLAYING && currentState != PlayerState::PAUSED) {
        return "Error: Must be in playing or paused state to seek. Current state: " + std::to_string(static_cast<int>(currentState));
    }
    
    try {
        // Find segment at target position
        double accumulatedDuration = 0.0;
        int targetSegmentIndex = 0;
        
        for (size_t i = 0; i < currentPlaylist.segments.size(); i++) {
            if (accumulatedDuration + currentPlaylist.segments[i].duration >= positionSeconds) {
                targetSegmentIndex = i;
                break;
            }
            accumulatedDuration += currentPlaylist.segments[i].duration;
        }
        
        std::cout << "[SEEK] Target segment: " << targetSegmentIndex << " at position " << accumulatedDuration << "s" << std::endl;
        
        // Stop threads temporarily
        bool wasPlaying = (currentState == PlayerState::PLAYING);
        StopThreads();
        
        // Clear buffer
        fragmentBuffer->Clear(-1);
        
        // Update segment index and PTS
        currentSegmentIndex = targetSegmentIndex;
        currentPts = accumulatedDuration;
        currentDts = accumulatedDuration;
        
        // Flush pipeline
        double rate = 1.0;
        bool shouldTearDown = false;
        bool isAppSeek = true;
        
        playerInstance->SetSeekPosition(positionSeconds);
        playerInstance->Flush(positionSeconds, rate, shouldTearDown, isAppSeek);
        
        // Restart threads if was playing
        if (wasPlaying) {
            StartThreads();
        }
        
        std::stringstream ss;
        ss << "Success: Seeked to " << positionSeconds << " seconds (segment " << targetSegmentIndex << ")";
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
    if (currentState == PlayerState::IDLE || currentPlaylist.segments.empty()) {
        return 0;
    }
    
    // Calculate total duration from playlist
    double totalDuration = 0.0;
    for (const auto& segment : currentPlaylist.segments) {
        totalDuration += segment.duration;
    }
    
    return static_cast<long>(totalDuration * 1000); // Convert to milliseconds
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
    
    ss << "========== Player Status ==========\n";
    ss << "State: ";
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
        ss << "Manifest URL: " << currentManifestUrl << "\n";
        ss << "Media Playlist: " << currentMediaPlaylistUrl << "\n";
    }
    
    if (!currentPlaylist.segments.empty()) {
        ss << "Total Segments: " << currentPlaylist.segments.size() << "\n";
        ss << "Current Segment: " << currentSegmentIndex << "\n";
        ss << "Playlist Type: " << (currentPlaylist.isLive ? "LIVE" : "VOD") << "\n";
    }
    
    ss << "Threads Running: " << (threadsRunning.load() ? "YES" : "NO") << "\n";
    ss << "Download Enabled: " << (downloadEnabled.load() ? "YES" : "NO") << "\n";
    ss << "Injection Enabled: " << (injectionEnabled.load() ? "YES" : "NO") << "\n";
    
    ss << "Video Buffer: " << fragmentBuffer->Size(0) << " fragments (" 
       << fragmentBuffer->GetBufferedDuration(0) << "s)\n";
    ss << "Audio Buffer: " << fragmentBuffer->Size(1) << " fragments (" 
       << fragmentBuffer->GetBufferedDuration(1) << "s)\n";
    
    if (isPipelineConfigured) {
        long long position = GetPosition();
        long duration = GetDuration();
        ss << "Position: " << position << " ms (" << (position / 1000.0) << "s)\n";
        ss << "Duration: " << duration << " ms (" << (duration / 1000.0) << "s)\n";
    }
    
    ss << "Volume: " << currentVolume << "\n";
    ss << "===================================\n";
    
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
    return a + b + 10;
}

std::string BusinessLogic::processUser(std::string username, int age, bool isActive) {
    std::stringstream ss;
    ss << "Processing user: " << username 
       << ", Age: " << age 
       << ", Status: " << (isActive ? "Active" : "Inactive");
    return ss.str();
}
