#ifndef API_INTERFACE_H
#define API_INTERFACE_H

#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include "InterfacePlayerRDK.h"
#include "hls_parser.h"
#include "fragment_buffer.h"

// MIDDLEWARE_API macro marks functions to be exposed as HTTP endpoints
#define MIDDLEWARE_API

/**
 * @brief Player state enum
 */
enum class PlayerState {
    IDLE = 0,
    INITIALIZING = 1,
    READY = 2,
    PLAYING = 3,
    PAUSED = 4,
    STOPPED = 5,
    ERROR = 6
};

/**
 * @brief BusinessLogic class - Complete streaming player with manifest parsing and fragment injection
 * This class provides a standalone player architecture for testing middleware-player-interface
 */
class BusinessLogic {
public:
    BusinessLogic();
    ~BusinessLogic();
    
    // ========== Core Playback APIs ==========
    
    /**
     * @brief Load a manifest URL for playback
     * @param url Manifest URL (HLS/DASH)
     * @return Status message
     */
    MIDDLEWARE_API std::string LoadManifest(std::string url);
    
    /**
     * @brief Start or resume playback
     * @return Status message
     */
    MIDDLEWARE_API std::string Play();
    
    /**
     * @brief Pause playback
     * @return Status message
     */
    MIDDLEWARE_API std::string Pause();
    
    /**
     * @brief Stop playback
     * @return Status message
     */
    MIDDLEWARE_API std::string Stop();
    
    /**
     * @brief Seek to a specific position
     * @param positionSeconds Position in seconds
     * @return Status message
     */
    MIDDLEWARE_API std::string Seek(double positionSeconds);
    
    /**
     * @brief Get current playback position
     * @return Position in milliseconds
     */
    MIDDLEWARE_API long long GetPosition();
    
    /**
     * @brief Get total duration
     * @return Duration in milliseconds
     */
    MIDDLEWARE_API long GetDuration();
    
    // ========== Audio/Video Control APIs ==========
    
    /**
     * @brief Set audio volume
     * @param volume Volume level (0-100)
     * @return Status message
     */
    MIDDLEWARE_API std::string SetVolume(int volume);
    
    /**
     * @brief Set video rectangle for display
     * @param x X coordinate
     * @param y Y coordinate
     * @param w Width
     * @param h Height
     * @return Status message
     */
    MIDDLEWARE_API std::string SetVideoRectangle(int x, int y, int w, int h);
    
    /**
     * @brief Set video mute state
     * @param muted True to mute, false to unmute
     * @return Status message
     */
    MIDDLEWARE_API std::string SetVideoMute(bool muted);
    
    /**
     * @brief Get video dimensions
     * @return String with width and height
     */
    MIDDLEWARE_API std::string GetVideoSize();
    
    /**
     * @brief Set playback rate
     * @param rate Playback rate (1.0 = normal, 2.0 = 2x, etc.)
     * @return Status message
     */
    MIDDLEWARE_API std::string SetPlaybackRate(double rate);
    
    // ========== Configuration APIs ==========
    
    /**
     * @brief Set player name for identification
     * @param playerName Name of the player
     */
    MIDDLEWARE_API void SetPlayerName(std::string playerName);
    
    /**
     * @brief Enable GStreamer debug logging
     * @param debugLevel Debug level (0-5)
     * @return Status message
     */
    MIDDLEWARE_API std::string EnableDebugLogging(std::string debugLevel);
    
    /**
     * @brief Get player status
     * @return Status string with playback info
     */
    MIDDLEWARE_API std::string GetStatus();
    
    // ========== Example/Test APIs ==========
    MIDDLEWARE_API std::string hello(std::string name);
    MIDDLEWARE_API std::string bye(std::string name);
    MIDDLEWARE_API int calculate(int a, int b);
    MIDDLEWARE_API std::string processUser(std::string username, int age, bool isActive);
    
private:
    // ========== Internal Methods ==========
    void InitializePlayer();
    void ConfigurePipeline();
    void RegisterCallbacks();
    void HandleBusMessage(const BusEventData busEvent);
    void HandleBufferUnderflow(int mediaType);
    void HandleDecodeError(int errorCount);
    
    // Thread functions
    void FragmentCollectorThreadFunc();
    void FragmentInjectorThreadFunc();
    void DownloadAndBufferSegments();
    
    // Injection helpers
    void InjectFragment(const MediaFragment& fragment);
    bool InjectVideoSegment(const MediaFragment& fragment);
    bool InjectAudioSegment(const MediaFragment& fragment);
    
    // Manifest handling
    bool DownloadManifest(const std::string& url, std::string& content, std::string& errorMsg);
    bool ParseAndSelectVariant(const std::string& manifestUrl);
    
    // Thread management
    void StartThreads();
    void StopThreads();
    
    // Player instance
    InterfacePlayerRDK* playerInstance;
    
    // Player state
    PlayerState currentState;
    std::string currentManifestUrl;
    std::string currentMediaPlaylistUrl;
    bool isPipelineConfigured;
    int currentVolume;
    
    // HLS playlist data
    HLSPlaylist currentPlaylist;
    int currentSegmentIndex;
    double currentPts;          // Current presentation timestamp
    double currentDts;          // Current decode timestamp
    
    // Fragment buffer
    std::unique_ptr<FragmentBuffer> fragmentBuffer;
    
    // Thread management
    std::thread fragmentCollectorThread;
    std::thread fragmentInjectorThread;
    std::atomic<bool> threadsRunning;
    std::atomic<bool> downloadEnabled;
    std::atomic<bool> injectionEnabled;
    
    // Synchronization
    std::mutex stateMutex;
};

#endif // API_INTERFACE_H
