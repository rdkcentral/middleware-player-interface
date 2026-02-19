#include "fragment_buffer.h"
#include <iostream>
#include <chrono>

FragmentBuffer::FragmentBuffer()
    : videoQueue(), audioQueue(), subtitleQueue(),
      videoMutex(), audioMutex(), subtitleMutex(),
      videoCv(), audioCv(), subtitleCv(),
      aborted(false)
{
}

FragmentBuffer::~FragmentBuffer()
{
    Abort();
    Clear(-1);
}

/**
 * @brief Get queue by media type
 */
std::queue<MediaFragment>& FragmentBuffer::GetQueue(int mediaType)
{
    switch (mediaType) {
        case 0: return videoQueue;
        case 1: return audioQueue;
        case 2: return subtitleQueue;
        default: return videoQueue;
    }
}

/**
 * @brief Get queue by media type (const version)
 */
const std::queue<MediaFragment>& FragmentBuffer::GetQueue(int mediaType) const
{
    switch (mediaType) {
        case 0: return videoQueue;
        case 1: return audioQueue;
        case 2: return subtitleQueue;
        default: return videoQueue;
    }
}

/**
 * @brief Get mutex by media type
 */
std::mutex& FragmentBuffer::GetMutex(int mediaType) const
{
    switch (mediaType) {
        case 0: return videoMutex;
        case 1: return audioMutex;
        case 2: return subtitleMutex;
        default: return videoMutex;
    }
}

/**
 * @brief Get condition variable by media type
 */
std::condition_variable& FragmentBuffer::GetCondVar(int mediaType)
{
    switch (mediaType) {
        case 0: return videoCv;
        case 1: return audioCv;
        case 2: return subtitleCv;
        default: return videoCv;
    }
}

/**
 * @brief Push fragment to buffer
 */
void FragmentBuffer::Push(const MediaFragment& fragment, int mediaType)
{
    std::lock_guard<std::mutex> lock(GetMutex(mediaType));
    GetQueue(mediaType).push(fragment);
    GetCondVar(mediaType).notify_one();
}

/**
 * @brief Pop fragment from buffer (blocking with timeout)
 */
bool FragmentBuffer::Pop(MediaFragment& fragment, int mediaType, int timeoutMs)
{
    std::unique_lock<std::mutex> lock(GetMutex(mediaType));
    
    auto& queue = GetQueue(mediaType);
    auto& cv = GetCondVar(mediaType);
    
    if (timeoutMs == 0) {
        // Wait indefinitely
        cv.wait(lock, [this, &queue]() {
            return !queue.empty() || aborted.load();
        });
    } else {
        // Wait with timeout
        auto timeout = std::chrono::milliseconds(timeoutMs);
        if (!cv.wait_for(lock, timeout, [this, &queue]() {
            return !queue.empty() || aborted.load();
        })) {
            return false; // Timeout
        }
    }
    
    if (aborted.load() || queue.empty()) {
        return false;
    }
    
    fragment = queue.front();
    queue.pop();
    return true;
}

/**
 * @brief Get buffer size
 */
size_t FragmentBuffer::Size(int mediaType) const
{
    std::lock_guard<std::mutex> lock(GetMutex(mediaType));
    return GetQueue(mediaType).size();
}

/**
 * @brief Check if buffer is empty
 */
bool FragmentBuffer::IsEmpty(int mediaType) const
{
    return Size(mediaType) == 0;
}

/**
 * @brief Clear buffer
 */
void FragmentBuffer::Clear(int mediaType)
{
    if (mediaType == -1) {
        // Clear all
        for (int i = 0; i < 3; i++) {
            std::lock_guard<std::mutex> lock(GetMutex(i));
            std::queue<MediaFragment> empty;
            std::swap(GetQueue(i), empty);
        }
    } else {
        std::lock_guard<std::mutex> lock(GetMutex(mediaType));
        std::queue<MediaFragment> empty;
        std::swap(GetQueue(mediaType), empty);
    }
}

/**
 * @brief Abort all waiting operations
 */
void FragmentBuffer::Abort()
{
    aborted.store(true);
    videoCv.notify_all();
    audioCv.notify_all();
    subtitleCv.notify_all();
}

/**
 * @brief Reset abort flag
 */
void FragmentBuffer::ResetAbort()
{
    aborted.store(false);
}

/**
 * @brief Get total buffered duration
 */
double FragmentBuffer::GetBufferedDuration(int mediaType) const
{
    std::lock_guard<std::mutex> lock(GetMutex(mediaType));
    
    double totalDuration = 0.0;
    auto queueCopy = GetQueue(mediaType);
    
    while (!queueCopy.empty()) {
        totalDuration += queueCopy.front().duration;
        queueCopy.pop();
    }
    
    return totalDuration;
}
