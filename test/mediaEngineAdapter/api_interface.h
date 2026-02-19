#ifndef API_INTERFACE_H
#define API_INTERFACE_H

#include <string>
#include <memory>
#include "InterfacePlayerRDK.h"

// MIDDLEWARE_API macro marks functions to be exposed as HTTP endpoints
#define MIDDLEWARE_API

/**
 * @brief BusinessLogic class - Main player logic implementing basic playback functionality
 * This class provides a clean API for UI to control playback using InterfacePlayerRDK
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
    // Private helper functions
    void internalHelper();
    void InitializePlayer();
    void ConfigurePipeline();
    void RegisterCallbacks();
    void HandleBusMessage(const BusEventData busEvent);
    void HandleBufferUnderflow(int mediaType);
    void HandleDecodeError(int errorCount);
    
    // Player state
    enum class PlayerState {
        IDLE,
        INITIALIZING,
        READY,
        PLAYING,
        PAUSED,
        STOPPED,
        ERROR
    };
    
    InterfacePlayerRDK *playerInstance;
    PlayerState currentState;
    std::string currentManifestUrl;
    bool isPipelineConfigured;
    int currentVolume;
};

#endif
