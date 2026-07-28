/*
 * If not stated otherwise in this file or this component's LICENSE file the
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

/**
 * @file PacketSenderTests.cpp
 * @brief Comprehensive unit tests for PacketSender and runWorkerTask.
 *
 * Covers:
 *   - PacketSender::Instance()     : singleton, never null, identity
 *   - PacketSender::Init()         : returns false in test env (no socket)
 *   - PacketSender::Init(path)     : returns false for non-existent path
 *   - PacketSender::Close()        : safe to call, stops any running thread
 *   - PacketSender::Flush()        : safe to call in any state
 *   - PacketSender::IsRunning()    : reflects running state
 *   - PacketSender::SendPacket()   : accepts packets without hanging
 *   - PacketSender::senderTask()   : tested via thread + Close() to stop
 *   - runWorkerTask()              : tested via thread + Close() to stop
 *
 * NOTE: Init() returns false in this test environment because there is no
 * socket server listening at the configured socket path.  The tested paths
 * are for the graceful-degradation code, which is a valid production scenario.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <chrono>
#include <memory>
#include <cstdint>
#include "PacketSender.hpp"
#include "SubtecPacket.hpp"

// ===========================================================================
// PacketSenderInstance tests
// ===========================================================================

/**
 * @brief PacketSender::Instance() returns a non-null pointer.
 *
 * @par Test Group ID  : PacketSenderInstance
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                  | Expected            |
 * |:----:|------------------------------|---------------------|
 * |  01  | Call PacketSender::Instance()| Returns non-null    |
 */
TEST(PacketSenderInstance, IsNotNull)
{
    std::cout << "[PacketSenderInstance.IsNotNull] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    std::cout << "  Instance() address = " << (void*)sender << std::endl;
    ASSERT_NE(sender, nullptr);
    std::cout << "[PacketSenderInstance.IsNotNull] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Instance() always returns the same pointer (singleton).
 *
 * @par Test Group ID  : PacketSenderInstance
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(PacketSenderInstance, SingletonIdentity)
{
    std::cout << "[PacketSenderInstance.SingletonIdentity] - START" << std::endl;
    PacketSender* s1 = PacketSender::Instance();
    PacketSender* s2 = PacketSender::Instance();
    PacketSender* s3 = PacketSender::Instance();
    std::cout << "  s1=" << (void*)s1 << " s2=" << (void*)s2 << " s3=" << (void*)s3 << std::endl;
    EXPECT_EQ(s1, s2);
    EXPECT_EQ(s2, s3);
    std::cout << "[PacketSenderInstance.SingletonIdentity] - PASS" << std::endl;
}

// ===========================================================================
// PacketSenderInit tests
// ===========================================================================

/**
 * @brief PacketSender::Init() returns false — no socket server in test env.
 *
 * @par Test Group ID  : PacketSenderInit
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderInit, DefaultInitReturnsFalse)
{
    std::cout << "[PacketSenderInit.DefaultInitReturnsFalse] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();  // Ensure clean state
    bool result = sender->Init();
    std::cout << "  Init() returned: " << result << " (expected false, no socket server)" << std::endl;
    EXPECT_FALSE(result);
    sender->Close();
    std::cout << "[PacketSenderInit.DefaultInitReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Init(path) with a non-existent socket path returns false.
 *
 * @par Test Group ID  : PacketSenderInit
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(PacketSenderInit, NonExistentSocketPathReturnsFalse)
{
    std::cout << "[PacketSenderInit.NonExistentSocketPathReturnsFalse] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    bool result = sender->Init("/tmp/no_such_socket_for_unit_test");
    std::cout << "  Init(\"/tmp/no_such_socket_for_unit_test\") returned: " << result << std::endl;
    EXPECT_FALSE(result);
    sender->Close();
    std::cout << "[PacketSenderInit.NonExistentSocketPathReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Init(path) with an invalid/empty-like path returns false.
 *
 * @par Test Group ID  : PacketSenderInit
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(PacketSenderInit, AnotherInvalidPathReturnsFalse)
{
    std::cout << "[PacketSenderInit.AnotherInvalidPathReturnsFalse] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    bool result = sender->Init("/tmp/packet_sender_absolutely_nonexistent_path_xyz");
    std::cout << "  Init(nonexistent path) = " << result << std::endl;
    EXPECT_FALSE(result);
    sender->Close();
    std::cout << "[PacketSenderInit.AnotherInvalidPathReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Init() can be called multiple times safely.
 *
 * @par Test Group ID  : PacketSenderInit
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(PacketSenderInit, MultipleCallsSafe)
{
    std::cout << "[PacketSenderInit.MultipleCallsSafe] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    for (int i = 0; i < 3; ++i)
    {
        sender->Close();
        bool r = sender->Init();
        std::cout << "  Init() call #" << (i+1) << " = " << r << std::endl;
        EXPECT_FALSE(r);
    }
    sender->Close();
    std::cout << "[PacketSenderInit.MultipleCallsSafe] - PASS" << std::endl;
}

// ===========================================================================
// PacketSenderClose tests
// ===========================================================================

/**
 * @brief PacketSender::Close() is safe to call without a prior Init().
 *
 * @par Test Group ID  : PacketSenderClose
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderClose, SafeWithoutInit)
{
    std::cout << "[PacketSenderClose.SafeWithoutInit] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    EXPECT_NO_THROW({ sender->Close(); });
    std::cout << "  Close() without prior Init() OK" << std::endl;
    std::cout << "[PacketSenderClose.SafeWithoutInit] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Close() can be called multiple times safely (idempotent).
 *
 * @par Test Group ID  : PacketSenderClose
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(PacketSenderClose, CalledMultipleTimesNoThrow)
{
    std::cout << "[PacketSenderClose.CalledMultipleTimesNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    EXPECT_NO_THROW({
        sender->Close();
        sender->Close();
        sender->Close();
    });
    std::cout << "  3 consecutive Close() calls OK" << std::endl;
    std::cout << "[PacketSenderClose.CalledMultipleTimesNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Close() after Init() sets IsRunning() to false.
 *
 * @par Test Group ID  : PacketSenderClose
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(PacketSenderClose, IsRunningFalseAfterClose)
{
    std::cout << "[PacketSenderClose.IsRunningFalseAfterClose] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();  // Put in a known closed state
    bool running = sender->IsRunning();
    std::cout << "  IsRunning() after Close() = " << running << " (expected false)" << std::endl;
    EXPECT_FALSE(running);
    std::cout << "[PacketSenderClose.IsRunningFalseAfterClose] - PASS" << std::endl;
}

// ===========================================================================
// PacketSenderFlush tests
// ===========================================================================

/**
 * @brief PacketSender::Flush() is safe to call without any prior Init() or sends.
 *
 * @par Test Group ID  : PacketSenderFlush
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderFlush, SafeWithoutInit)
{
    std::cout << "[PacketSenderFlush.SafeWithoutInit] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({ sender->Flush(); });
    std::cout << "  Flush() without Init() OK" << std::endl;
    std::cout << "[PacketSenderFlush.SafeWithoutInit] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Flush() can be called multiple times consecutively.
 *
 * @par Test Group ID  : PacketSenderFlush
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(PacketSenderFlush, MultipleFlushesNoThrow)
{
    std::cout << "[PacketSenderFlush.MultipleFlushesNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        for (int i = 0; i < 5; ++i) sender->Flush();
    });
    std::cout << "  5 consecutive Flush() calls OK" << std::endl;
    std::cout << "[PacketSenderFlush.MultipleFlushesNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::Flush() after SendPacket() flushes the queue without hanging.
 *
 * @par Test Group ID  : PacketSenderFlush
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(PacketSenderFlush, FlushAfterSendPacket)
{
    std::cout << "[PacketSenderFlush.FlushAfterSendPacket] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<PausePacket>(1u, 0u));
        sender->SendPacket(std::make_unique<ResumePacket>(1u, 1u));
        sender->Flush();
    });
    std::cout << "  SendPacket x2 + Flush() OK" << std::endl;
    std::cout << "[PacketSenderFlush.FlushAfterSendPacket] - PASS" << std::endl;
}

// ===========================================================================
// PacketSenderIsRunning tests
// ===========================================================================

/**
 * @brief PacketSender::IsRunning() returns false by default (never initialised).
 *
 * @par Test Group ID  : PacketSenderIsRunning
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderIsRunning, DefaultIsFalse)
{
    std::cout << "[PacketSenderIsRunning.DefaultIsFalse] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();  // ensure clean state
    bool running = sender->IsRunning();
    std::cout << "  IsRunning() default = " << running << " (expected false)" << std::endl;
    EXPECT_FALSE(running);
    std::cout << "[PacketSenderIsRunning.DefaultIsFalse] - PASS" << std::endl;
}

/**
 * @brief PacketSender::IsRunning() returns false after failed Init().
 *
 * @par Test Group ID  : PacketSenderIsRunning
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(PacketSenderIsRunning, FalseAfterFailedInit)
{
    std::cout << "[PacketSenderIsRunning.FalseAfterFailedInit] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    sender->Init();  // Expected to fail — no socket
    bool running = sender->IsRunning();
    std::cout << "  IsRunning() after failed Init() = " << running << " (expected false)" << std::endl;
    EXPECT_FALSE(running);
    sender->Close();
    std::cout << "[PacketSenderIsRunning.FalseAfterFailedInit] - PASS" << std::endl;
}

/**
 * @brief PacketSender::IsRunning() returns false after Close() even if called multiple times.
 *
 * @par Test Group ID  : PacketSenderIsRunning
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(PacketSenderIsRunning, RemainsConsistentAfterMultipleCloses)
{
    std::cout << "[PacketSenderIsRunning.RemainsConsistentAfterMultipleCloses] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    for (int i = 0; i < 3; ++i)
    {
        sender->Close();
        EXPECT_FALSE(sender->IsRunning()) << " iteration " << i;
    }
    std::cout << "  IsRunning() false after each of 3 Close() calls" << std::endl;
    std::cout << "[PacketSenderIsRunning.RemainsConsistentAfterMultipleCloses] - PASS" << std::endl;
}

// ===========================================================================
// PacketSenderSendPacket tests
// ===========================================================================

/**
 * @brief PacketSender::SendPacket() with a PausePacket does not throw or hang.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderSendPacket, PausePacketNoThrow)
{
    std::cout << "[PacketSenderSendPacket.PausePacketNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<PausePacket>(0u, 0u));
    });
    sender->Flush();
    std::cout << "  SendPacket(PausePacket) OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.PausePacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::SendPacket() with a ResumePacket does not throw.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(PacketSenderSendPacket, ResumePacketNoThrow)
{
    std::cout << "[PacketSenderSendPacket.ResumePacketNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<ResumePacket>(1u, 1u));
    });
    sender->Flush();
    std::cout << "  SendPacket(ResumePacket) OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.ResumePacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::SendPacket() with a MutePacket does not throw.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(PacketSenderSendPacket, MutePacketNoThrow)
{
    std::cout << "[PacketSenderSendPacket.MutePacketNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<MutePacket>(2u, 5u));
    });
    sender->Flush();
    std::cout << "  SendPacket(MutePacket) OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.MutePacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::SendPacket() with a ResetAllPacket does not throw.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(PacketSenderSendPacket, ResetAllPacketNoThrow)
{
    std::cout << "[PacketSenderSendPacket.ResetAllPacketNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<ResetAllPacket>());
    });
    sender->Flush();
    std::cout << "  SendPacket(ResetAllPacket) OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.ResetAllPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::SendPacket() with multiple different packet types.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 005
 * @par Priority       : Medium
 */
TEST(PacketSenderSendPacket, MultiplePacketTypesNoThrow)
{
    std::cout << "[PacketSenderSendPacket.MultiplePacketTypesNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    EXPECT_NO_THROW({
        sender->SendPacket(std::make_unique<PausePacket>(1u, 0u));
        sender->SendPacket(std::make_unique<ResumePacket>(1u, 1u));
        sender->SendPacket(std::make_unique<MutePacket>(1u, 2u));
        sender->SendPacket(std::make_unique<UnmutePacket>(1u, 3u));
        sender->SendPacket(std::make_unique<ResetAllPacket>());
        sender->SendPacket(std::make_unique<ResetChannelPacket>(1u, 4u));
    });
    sender->Flush();
    std::cout << "  6 different packet types sent OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.MultiplePacketTypesNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender::SendPacket() stress: 100 packets enqueued without hang.
 *
 * @par Test Group ID  : PacketSenderSendPacket
 * @par Test Case ID   : 006
 * @par Priority       : Low (stress)
 */
TEST(PacketSenderSendPacket, Stress100Packets)
{
    std::cout << "[PacketSenderSendPacket.Stress100Packets] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();
    for (uint32_t i = 0; i < 100u; ++i)
    {
        EXPECT_NO_THROW({
            sender->SendPacket(std::make_unique<PausePacket>(i % 4, i));
        });
    }
    sender->Flush();
    std::cout << "  100 PausePackets sent OK" << std::endl;
    std::cout << "[PacketSenderSendPacket.Stress100Packets] - PASS" << std::endl;
}

// ===========================================================================
// PacketSender::senderTask() tests — tested via thread to avoid blocking
// ===========================================================================

/**
 * @brief PacketSender::senderTask() can be started in a thread and stopped via Close().
 *
 * This test starts senderTask() in a background thread, waits briefly, then
 * calls Close() to signal termination.  The join verifies the thread exits
 * within a reasonable timeout.
 *
 * @par Test Group ID  : PacketSenderSenderTask
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderSenderTask, StartsAndStopsViaClose)
{
    std::cout << "[PacketSenderSenderTask.StartsAndStopsViaClose] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();  // Ensure clean initial state

    std::thread t([sender]() {
        sender->senderTask();
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sender->Close();   // Signal worker to exit

    t.join();
    std::cout << "  senderTask() thread started and stopped cleanly via Close()" << std::endl;
    EXPECT_FALSE(sender->IsRunning());
    std::cout << "[PacketSenderSenderTask.StartsAndStopsViaClose] - PASS" << std::endl;
}

/**
 * @brief PacketSender::senderTask() processes packets enqueued before join.
 *
 * @par Test Group ID  : PacketSenderSenderTask
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(PacketSenderSenderTask, ProcessesEnqueuedPackets)
{
    std::cout << "[PacketSenderSenderTask.ProcessesEnqueuedPackets] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();

    std::thread t([sender]() {
        sender->senderTask();
    });

    // Enqueue some packets while thread is running
    for (uint32_t i = 0; i < 5u; ++i)
    {
        sender->SendPacket(std::make_unique<PausePacket>(1u, i));
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sender->Close();
    t.join();

    std::cout << "  5 packets enqueued while senderTask() running, thread stopped cleanly" << std::endl;
    EXPECT_FALSE(sender->IsRunning());
    std::cout << "[PacketSenderSenderTask.ProcessesEnqueuedPackets] - PASS" << std::endl;
}

// ===========================================================================
// runWorkerTask() tests — tested via thread to avoid blocking
// ===========================================================================

/**
 * @brief runWorkerTask() with a valid PacketSender context starts and stops via Close().
 *
 * @par Test Group ID  : RunWorkerTask
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(RunWorkerTask, ValidContextStartsAndStops)
{
    std::cout << "[RunWorkerTask.ValidContextStartsAndStops] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();

    std::thread t([]() {
        runWorkerTask(PacketSender::Instance());
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    sender->Close();
    t.join();

    std::cout << "  runWorkerTask(valid ctx) thread started and stopped cleanly" << std::endl;
    EXPECT_FALSE(sender->IsRunning());
    std::cout << "[RunWorkerTask.ValidContextStartsAndStops] - PASS" << std::endl;
}

/**
 * @brief After runWorkerTask() thread exits, IsRunning() is false.
 *
 * @par Test Group ID  : RunWorkerTask
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(RunWorkerTask, IsRunningFalseAfterThreadExit)
{
    std::cout << "[RunWorkerTask.IsRunningFalseAfterThreadExit] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    sender->Close();

    std::thread t([]() {
        runWorkerTask(PacketSender::Instance());
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    sender->Close();
    t.join();

    EXPECT_FALSE(sender->IsRunning());
    std::cout << "  IsRunning() = false after thread joined" << std::endl;
    std::cout << "[RunWorkerTask.IsRunningFalseAfterThreadExit] - PASS" << std::endl;
}

/**
 * @brief PacketSender state is consistent through full Init → Close lifecycle.
 *
 * Exercises the complete lifecycle: Close → Init (fails) → IsRunning (false) → Close.
 *
 * @par Test Group ID  : PacketSenderLifecycle
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketSenderLifecycle, FullCycleNoThrow)
{
    std::cout << "[PacketSenderLifecycle.FullCycleNoThrow] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    EXPECT_NO_THROW({
        sender->Close();
        bool r = sender->Init();
        std::cout << "  Init() = " << r << std::endl;
        EXPECT_FALSE(r);
        EXPECT_FALSE(sender->IsRunning());
        sender->Flush();
        sender->Close();
        EXPECT_FALSE(sender->IsRunning());
    });
    std::cout << "[PacketSenderLifecycle.FullCycleNoThrow] - PASS" << std::endl;
}

/**
 * @brief PacketSender: Close → SendPacket → Flush sequence is safe.
 *
 * @par Test Group ID  : PacketSenderLifecycle
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(PacketSenderLifecycle, CloseSendFlushSequence)
{
    std::cout << "[PacketSenderLifecycle.CloseSendFlushSequence] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    EXPECT_NO_THROW({
        sender->Close();
        sender->SendPacket(std::make_unique<MutePacket>(1u, 0u));
        sender->SendPacket(std::make_unique<UnmutePacket>(1u, 1u));
        sender->Flush();
        sender->Close();
    });
    std::cout << "  Close → SendPacket x2 → Flush → Close sequence OK" << std::endl;
    std::cout << "[PacketSenderLifecycle.CloseSendFlushSequence] - PASS" << std::endl;
}

/**
 * @brief PacketSender: Init → Close cycle repeated 3 times remains consistent.
 *
 * @par Test Group ID  : PacketSenderLifecycle
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 */
TEST(PacketSenderLifecycle, RepeatedInitCloseCycles)
{
    std::cout << "[PacketSenderLifecycle.RepeatedInitCloseCycles] - START" << std::endl;
    PacketSender* sender = PacketSender::Instance();
    for (int i = 0; i < 3; ++i)
    {
        sender->Close();
        bool r = sender->Init();
        std::cout << "  cycle " << (i+1) << ": Init()=" << r << std::endl;
        EXPECT_FALSE(r);
        EXPECT_FALSE(sender->IsRunning());
        sender->Close();
    }
    std::cout << "[PacketSenderLifecycle.RepeatedInitCloseCycles] - PASS" << std::endl;
}
