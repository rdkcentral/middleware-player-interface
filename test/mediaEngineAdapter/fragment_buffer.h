#ifndef FRAGMENT_BUFFER_H
#define FRAGMENT_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <cstdint>
#include <atomic>

/**
 * @brief Represents a downloaded media fragment ready for injection
 */
struct MediaFragment
{
    std::vector<uint8_t> data;    // Fragment binary data
    double pts;                   // Presentation timestamp in seconds
    double dts;                   // Decode timestamp in seconds
    double duration;              // Duration in seconds
    int sequenceNumber;           // Sequence number
    bool isInitSegment;           // True if this is an initialization segment
    bool isDiscontinuity;         // True if discontinuity
    int mediaType;                // 0=Video, 1=Audio, 2=Subtitle
    
    MediaFragment()
        : data(), pts(0.0), dts(0.0), duration(0.0),
          sequenceNumber(0), isInitSegment(false),
          isDiscontinuity(false), mediaType(0) {}
};

/**
 * @brief Thread-safe buffer for media fragments
 * Manages separate queues for video, audio, and subtitle tracks
 */
class FragmentBuffer
{
public:
    FragmentBuffer();
    ~FragmentBuffer();
    
    /**
     * @brief Push a fragment to the buffer
     * @param fragment Fragment to add
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle
     */
    void Push(const MediaFragment& fragment, int mediaType);
    
    /**
     * @brief Pop a fragment from the buffer (blocking)
     * @param fragment Output fragment
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle
     * @param timeoutMs Timeout in milliseconds (0 = wait forever)
     * @return True if fragment retrieved, false if timeout or aborted
     */
    bool Pop(MediaFragment& fragment, int mediaType, int timeoutMs = 0);
    
    /**
     * @brief Get the current buffer size
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle
     * @return Number of fragments in buffer
     */
    size_t Size(int mediaType) const;
    
    /**
     * @brief Check if buffer is empty
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle
     * @return True if empty
     */
    bool IsEmpty(int mediaType) const;
    
    /**
     * @brief Clear all fragments from buffer
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle, -1=All
     */
    void Clear(int mediaType = -1);
    
    /**
     * @brief Abort all waiting operations
     */
    void Abort();
    
    /**
     * @brief Reset abort flag
     */
    void ResetAbort();
    
    /**
     * @brief Get total buffered duration in seconds
     * @param mediaType 0=Video, 1=Audio, 2=Subtitle
     * @return Total duration of buffered fragments
     */
    double GetBufferedDuration(int mediaType) const;

private:
    std::queue<MediaFragment> videoQueue;
    std::queue<MediaFragment> audioQueue;
    std::queue<MediaFragment> subtitleQueue;
    
    mutable std::mutex videoMutex;
    mutable std::mutex audioMutex;
    mutable std::mutex subtitleMutex;
    
    std::condition_variable videoCv;
    std::condition_variable audioCv;
    std::condition_variable subtitleCv;
    
    std::atomic<bool> aborted;
    
    std::queue<MediaFragment>& GetQueue(int mediaType);
    const std::queue<MediaFragment>& GetQueue(int mediaType) const;
    std::mutex& GetMutex(int mediaType) const;
    std::condition_variable& GetCondVar(int mediaType);
};

#endif // FRAGMENT_BUFFER_H
