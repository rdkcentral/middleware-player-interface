/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2025 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#pragma once

#include <memory>
#include <queue>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <mutex>
#include <condition_variable>
#include <atomic>

#ifdef SUBTEC_PACKET_DEBUG

#undef MW_LOG_WARN
#undef MW_LOG_INFO
#undef MW_LOG_TRACE
#undef MW_LOG_ERR
#undef MW_LOG_DEBUG

#define MW_LOG_WARN(...) printf
#define MW_LOG_INFO(...) printf
#define MW_LOG_TRACE(...) printf
#define MW_LOG_ERR(...) printf
#define MW_LOG_DEBUG(...) printf

#endif

#include "SubtecPacket.hpp"

#if defined (__APPLE__) || defined(UBUNTU)
const constexpr char *SOCKET_PATH = "/tmp/pes_data_main"; // simulator
#else
const constexpr char *SOCKET_PATH = "/run/subttx/pes_data_main"; // device
#endif

void runWorkerTask(void *ctx);

class PacketSender
{
public:    
    ~PacketSender();
    /**
     * @brief Closes the player and releases associated resources.
     *
     * This function stops any ongoing playback, cleans up internal
     * resources, and resets the player to an uninitialized state.
     * It should be called before destroying the player instance
     * or when playback is no longer required.
     *
     * @return None
     */
    void Close();
    /**
    * @brief Flushes any pending packets from the sender queue.
    *
    * This function ensures that all queued packets are sent or discarded
    * before resetting or shutting down the sender.
    *
    * @return None.
    */
    void Flush();
    
    /**
    * @brief Initializes the packet sender with default configuration.
    *
    * This function prepares the packet sender for operation using
    * default initialization parameters.
    *
    * @return True if initialization succeeds, false otherwise.
    */
    bool Init();
    
    /**
    * @brief Initializes the packet sender with a specific socket path.
    *
    * This overload of Init() prepares the packet sender for communication
    * using the provided UNIX domain socket path.
    *
    * @param[in] socket_path  Path to the socket used for packet transmission.
    *
    * @return True if initialization succeeds, false otherwise.
    */
    bool Init(const char *socket_path);
    
    /**
    * @brief Sends a packet to the destination.
    *
    * This function transmits a packet using the underlying communication
    * mechanism. Ownership of the packet is transferred to the sender.
    *
    * @param[in] packet  The packet to be sent (passed as an rvalue reference).
    *
    * @return None.
    */
    void SendPacket(PacketPtr && packet);
    
    /**
    * @brief Executes the sender task responsible for transmitting packets.
    *
    * This function runs the internal task loop that handles sending
    * packets asynchronously or from a queue.
    *
    * @return None.
    */
    void senderTask();
    
    /**
    * @brief Checks if the packet sender is currently running.
    *
    * @return True if the sender is active, false otherwise.
    */
    bool IsRunning();
    
    /**
    * @brief Returns the singleton instance of the PacketSender.
    *
    * This static method provides access to the global PacketSender
    * instance, ensuring a single point of packet transmission.
    *
    * @return Pointer to the PacketSender instance.
    */
    static PacketSender *Instance();    

private:
    /**
     * @brief Closes the sender task and releases related resources.
     *
     * This function terminates the active sender task responsible for
     * transmitting packets and performs necessary cleanup operations.
     *
     * @return None.
     */
    void closeSenderTask();
    
    /**
     * @brief Flushes all pending packets from the transmission queue.
     *
     * This function clears any unsent packets from the internal queue,
     * ensuring that no stale or outdated packets remain before shutdown
     * or reinitialization.
     *
     * @return None.
     */
    void flushPacketQueue();
    
    /**
     * @brief Sends a packet through the established connection.
     *
     * This function enqueues or directly transmits the provided packet
     * using the active socket or communication channel.
     * Ownership of the packet is transferred to the sender.
     *
     * @param[in] pkt  The packet to be sent (passed as an rvalue reference).
     *
     * @return None.
     */
    void sendPacket(PacketPtr && pkt);
    
    /**
     * @brief Initializes and starts the sender task.
     *
     * This function sets up and launches the internal sender task that
     * handles packet transmission in the background.
     *
     * @return True if the sender task was successfully initialized, false otherwise.
     */
    bool initSenderTask();
    
    /**
     * @brief Initializes the socket for packet transmission.
     *
     * This function creates and configures a socket connection for
     * communication using the specified socket path.
     *
     * @param[in] socket_path  Path to the UNIX domain socket used for sending packets.
     *
     * @return True if the socket was successfully initialized, false otherwise.
     */
    bool initSocket(const char *socket_path);


    std::thread mSendThread;
    int mSubtecSocketHandle;
    std::atomic_bool running;
    std::queue<PacketPtr> mPacketQueue;
    std::mutex mPktMutex;
    std::condition_variable mCv;
    std::mutex mStartMutex;
    int mSockBufSize;
    int mPktWriteFailCtr;
    std::string mSocketPath;
protected:
    PacketSender() : 
        mSendThread(), 
        mSubtecSocketHandle(-1), 
        running(false), 
        mPacketQueue(), 
        mPktMutex(), 
        mCv(),
        mStartMutex(),
        mSockBufSize(0),
        mPktWriteFailCtr(0),
        mSocketPath("")
        {}
};
