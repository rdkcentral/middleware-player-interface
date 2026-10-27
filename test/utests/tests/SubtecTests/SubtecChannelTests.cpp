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
 * @file SubtecChannelTests.cpp
 * @brief Comprehensive unit tests for SubtecChannel, SubtecChannelManager,
 *        TtmlChannel, WebVttChannel, and ClosedCaptionsChannel.
 *
 * All Send* methods delegate to PacketSender::Instance()->SendPacket() which
 * safely enqueues packets even when the socket is not connected (PacketSender
 * not initialized in test environment). The queue is flushed in TearDown to
 * prevent unbounded growth.
 *
 * APIs covered (100%):
 *   SubtecChannelManager:
 *     getInstance(), getNextChannelId()
 *   SubtecChannel:
 *     SubtecChannelFactory(TTML), SubtecChannelFactory(WEBVTT),
 *     SubtecChannelFactory(CC), InitComms(), InitComms(path),
 *     SendResetAllPacket(), SendResetChannelPacket(),
 *     SendPausePacket(), SendResumePacket(),
 *     SendMutePacket(), SendUnmutePacket(),
 *     SendCCSetAttributePacket()
 *   TtmlChannel (via SubtecChannelFactory or direct):
 *     SendSelectionPacket(), SendDataPacket(), SendTimestampPacket()
 *   WebVttChannel (via SubtecChannelFactory or direct):
 *     SendSelectionPacket(), SendDataPacket(), SendTimestampPacket()
 *   ClosedCaptionsChannel:
 *     SendDataPacketWithPTS(), SendDataPacketNoPTS(), SendActiveTypePacket()
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <vector>
#include <cstdint>
#include <array>
#include <string>
#include <iostream>

#include "SubtecChannel.hpp"
#include "TtmlPacket.hpp"
#include "WebVttPacket.hpp"
#include "ClosedCaptionsPacket.hpp"
#include "PacketSender.hpp"
#include "SubtecAttribute.hpp"

// ===========================================================================
// Fixture — flush PacketSender queue between tests to avoid memory growth
// ===========================================================================

class SubtecChannelTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Ensure PacketSender is in a clean, flushed state
        PacketSender::Instance()->Flush();
    }
    void TearDown() override
    {
        PacketSender::Instance()->Flush();
    }
};

// ===========================================================================
// Group: SubtecChannelManager
// ===========================================================================

/**
 * @brief SubtecChannelManager::getInstance() returns a non-null pointer.
 *
 * @par Test Group ID  : SubtecChannelManager
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                            | Expected        |
 * |:----:|----------------------------------------|-----------------|
 * |  01  | Call SubtecChannelManager::getInstance()| Non-null pointer|
 */
TEST(SubtecChannelManager, GetInstanceReturnsNonNull)
{
    std::cout << "[SubtecChannelManager.GetInstanceReturnsNonNull] - START" << std::endl;
    SubtecChannelManager* mgr = SubtecChannelManager::getInstance();
    std::cout << "  getInstance() address = " << (void*)mgr << std::endl;
    ASSERT_NE(mgr, nullptr);
    std::cout << "[SubtecChannelManager.GetInstanceReturnsNonNull] - PASS" << std::endl;
}

/**
 * @brief SubtecChannelManager::getInstance() always returns the same pointer (singleton).
 *
 * @par Test Group ID  : SubtecChannelManager
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description | Expected |
 * |:----:|-------------|---------|
 * |  01  | Call getInstance() 3 times | All pointers identical |
 */
TEST(SubtecChannelManager, GetInstanceIsSingleton)
{
    std::cout << "[SubtecChannelManager.GetInstanceIsSingleton] - START" << std::endl;
    SubtecChannelManager* m1 = SubtecChannelManager::getInstance();
    SubtecChannelManager* m2 = SubtecChannelManager::getInstance();
    SubtecChannelManager* m3 = SubtecChannelManager::getInstance();
    std::cout << "  m1=" << (void*)m1 << " m2=" << (void*)m2 << " m3=" << (void*)m3 << std::endl;
    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m2, m3);
    std::cout << "[SubtecChannelManager.GetInstanceIsSingleton] - PASS" << std::endl;
}

/**
 * @brief SubtecChannelManager::getNextChannelId() returns strictly increasing values.
 *
 * @par Test Group ID  : SubtecChannelManager
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                  | Expected              |
 * |:----:|------------------------------|-----------------------|
 * |  01  | Call getNextChannelId() twice | Second > first        |
 */
TEST(SubtecChannelManager, GetNextChannelIdIsMonotonicallyIncreasing)
{
    std::cout << "[SubtecChannelManager.GetNextChannelIdIsMonotonicallyIncreasing] - START" << std::endl;
    SubtecChannelManager* mgr = SubtecChannelManager::getInstance();
    int id1 = mgr->getNextChannelId();
    int id2 = mgr->getNextChannelId();
    std::cout << "  id1=" << id1 << " id2=" << id2 << std::endl;
    EXPECT_GT(id2, id1);
    std::cout << "[SubtecChannelManager.GetNextChannelIdIsMonotonicallyIncreasing] - PASS" << std::endl;
}

/**
 * @brief getNextChannelId() increments by exactly 1 on each call.
 *
 * @par Test Group ID  : SubtecChannelManager
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 *
 * | Step | Description                 | Expected             |
 * |:----:|-----------------------------|----------------------|
 * |  01  | Call getNextChannelId() x5  | Each is prev + 1     |
 */
TEST(SubtecChannelManager, GetNextChannelIdIncrementsBy1)
{
    std::cout << "[SubtecChannelManager.GetNextChannelIdIncrementsBy1] - START" << std::endl;
    SubtecChannelManager* mgr = SubtecChannelManager::getInstance();
    int base = mgr->getNextChannelId();
    for (int i = 1; i <= 5; ++i)
    {
        int next = mgr->getNextChannelId();
        std::cout << "  getNextChannelId() call " << i << " = " << next << std::endl;
        EXPECT_EQ(next, base + i);
    }
    std::cout << "[SubtecChannelManager.GetNextChannelIdIncrementsBy1] - PASS" << std::endl;
}

// ===========================================================================
// Group: SubtecChannelFactory
// ===========================================================================

/**
 * @brief SubtecChannelFactory(TTML) returns a non-null TtmlChannel.
 *
 * @par Test Group ID  : SubtecChannelFactory
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                         | Expected      |
 * |:----:|-------------------------------------|---------------|
 * |  01  | Call factory with ChannelType::TTML | Non-null ptr  |
 */
TEST(SubtecChannelFactory, TTMLReturnsNonNull)
{
    std::cout << "[SubtecChannelFactory.TTMLReturnsNonNull] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    std::cout << "  TTML channel ptr = " << (void*)ch.get() << std::endl;
    EXPECT_NE(ch, nullptr);
    std::cout << "[SubtecChannelFactory.TTMLReturnsNonNull] - PASS" << std::endl;
}

/**
 * @brief SubtecChannelFactory(WEBVTT) returns a non-null WebVttChannel.
 *
 * @par Test Group ID  : SubtecChannelFactory
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description                           | Expected      |
 * |:----:|---------------------------------------|---------------|
 * |  01  | Call factory with ChannelType::WEBVTT | Non-null ptr  |
 */
TEST(SubtecChannelFactory, WEBVTTReturnsNonNull)
{
    std::cout << "[SubtecChannelFactory.WEBVTTReturnsNonNull] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    std::cout << "  WEBVTT channel ptr = " << (void*)ch.get() << std::endl;
    EXPECT_NE(ch, nullptr);
    std::cout << "[SubtecChannelFactory.WEBVTTReturnsNonNull] - PASS" << std::endl;
}

/**
 * @brief SubtecChannelFactory(CC) returns null (CC channels are not created via factory).
 *
 * @par Test Group ID  : SubtecChannelFactory
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                       | Expected   |
 * |:----:|-----------------------------------|------------|
 * |  01  | Call factory with ChannelType::CC | nullptr    |
 */
TEST(SubtecChannelFactory, CCReturnsNull)
{
    std::cout << "[SubtecChannelFactory.CCReturnsNull] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::CC);
    std::cout << "  CC channel ptr = " << (void*)ch.get() << " (expected nullptr)" << std::endl;
    EXPECT_EQ(ch, nullptr);
    std::cout << "[SubtecChannelFactory.CCReturnsNull] - PASS" << std::endl;
}

/**
 * @brief Each call to SubtecChannelFactory produces a distinct object.
 *
 * @par Test Group ID  : SubtecChannelFactory
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 *
 * | Step | Description | Expected |
 * |:----:|-------------|---------|
 * |  01  | Create 2 TTML channels | Different addresses |
 */
TEST(SubtecChannelFactory, EachCallProducesDistinctObject)
{
    std::cout << "[SubtecChannelFactory.EachCallProducesDistinctObject] - START" << std::endl;
    auto ch1 = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    auto ch2 = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    std::cout << "  ch1=" << (void*)ch1.get() << " ch2=" << (void*)ch2.get() << std::endl;
    EXPECT_NE(ch1.get(), ch2.get());
    std::cout << "[SubtecChannelFactory.EachCallProducesDistinctObject] - PASS" << std::endl;
}

// ===========================================================================
// Group: SubtecChannelInitComms
// ===========================================================================

/**
 * @brief InitComms() with default socket path returns false (no server in test env).
 *
 * @par Test Group ID  : SubtecChannelInitComms
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description             | Expected |
 * |:----:|-------------------------|---------|
 * |  01  | Call InitComms()        | false   |
 */
TEST(SubtecChannelInitComms, DefaultPathReturnsFalse)
{
    std::cout << "[SubtecChannelInitComms.DefaultPathReturnsFalse] - START" << std::endl;
    // Close any existing sender first to reset running state
    PacketSender::Instance()->Close();
    bool result = SubtecChannel::InitComms();
    std::cout << "  InitComms() = " << result << " (expected false, no socket server)" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "[SubtecChannelInitComms.DefaultPathReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief InitComms(nonexistent_path) returns false (no server at that path).
 *
 * @par Test Group ID  : SubtecChannelInitComms
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description                          | Expected |
 * |:----:|--------------------------------------|---------|
 * |  01  | Call InitComms("/tmp/no_such_socket") | false    |
 */
TEST(SubtecChannelInitComms, NonExistentPathReturnsFalse)
{
    std::cout << "[SubtecChannelInitComms.NonExistentPathReturnsFalse] - START" << std::endl;
    PacketSender::Instance()->Close();
    bool result = SubtecChannel::InitComms("/tmp/no_such_subtec_socket_xyz");
    std::cout << "  InitComms(\"/tmp/no_such...\") = " << result << std::endl;
    EXPECT_FALSE(result);
    std::cout << "[SubtecChannelInitComms.NonExistentPathReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief InitComms("") with empty path returns false.
 *
 * @par Test Group ID  : SubtecChannelInitComms
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 *
 * | Step | Description        | Expected |
 * |:----:|--------------------|---------|
 * |  01  | Call InitComms("") | false   |
 */
TEST(SubtecChannelInitComms, EmptyPathReturnsFalse)
{
    std::cout << "[SubtecChannelInitComms.EmptyPathReturnsFalse] - START" << std::endl;
    PacketSender::Instance()->Close();
    bool result = SubtecChannel::InitComms("");
    std::cout << "  InitComms(\"\") = " << result << std::endl;
    EXPECT_FALSE(result);
    std::cout << "[SubtecChannelInitComms.EmptyPathReturnsFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: SubtecChannelSendReset
// ===========================================================================

/**
 * @brief SendResetAllPacket() does not throw or crash when PacketSender is idle.
 *
 * @par Test Group ID  : SubtecChannelSendReset
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description              | Expected        |
 * |:----:|--------------------------|-----------------|
 * |  01  | Call SendResetAllPacket()| No exception    |
 */
TEST_F(SubtecChannelTest, SendResetAllPacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendReset.SendResetAllPacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendResetAllPacket());
    std::cout << "  SendResetAllPacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendReset.SendResetAllPacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendResetChannelPacket() does not throw when PacketSender is idle.
 *
 * @par Test Group ID  : SubtecChannelSendReset
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description                  | Expected      |
 * |:----:|------------------------------|---------------|
 * |  01  | Call SendResetChannelPacket()| No exception  |
 */
TEST_F(SubtecChannelTest, SendResetChannelPacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendReset.SendResetChannelPacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendResetChannelPacket());
    std::cout << "  SendResetChannelPacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendReset.SendResetChannelPacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendResetAllPacket() called multiple times is stable.
 *
 * @par Test Group ID  : SubtecChannelSendReset
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 *
 * | Step | Description                      | Expected      |
 * |:----:|----------------------------------|---------------|
 * |  01  | Call SendResetAllPacket() 5 times| No exception  |
 */
TEST_F(SubtecChannelTest, SendResetAllPacketMultipleCallsStable)
{
    std::cout << "[SubtecChannelSendReset.SendResetAllPacketMultipleCallsStable] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_NO_THROW(ch->SendResetAllPacket());
    }
    std::cout << "  SendResetAllPacket() called 5 times without exception" << std::endl;
    std::cout << "[SubtecChannelSendReset.SendResetAllPacketMultipleCallsStable] - PASS" << std::endl;
}

// ===========================================================================
// Group: SubtecChannelSendPlayControl
// ===========================================================================

/**
 * @brief SendPausePacket() does not throw.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description          | Expected      |
 * |:----:|----------------------|---------------|
 * |  01  | Call SendPausePacket()| No exception |
 */
TEST_F(SubtecChannelTest, SendPausePacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.SendPausePacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendPausePacket());
    std::cout << "  SendPausePacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.SendPausePacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendResumePacket() does not throw.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description           | Expected      |
 * |:----:|-----------------------|---------------|
 * |  01  | Call SendResumePacket()| No exception |
 */
TEST_F(SubtecChannelTest, SendResumePacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.SendResumePacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendResumePacket());
    std::cout << "  SendResumePacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.SendResumePacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendMutePacket() does not throw.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description         | Expected      |
 * |:----:|---------------------|---------------|
 * |  01  | Call SendMutePacket()| No exception |
 */
TEST_F(SubtecChannelTest, SendMutePacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.SendMutePacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendMutePacket());
    std::cout << "  SendMutePacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.SendMutePacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendUnmutePacket() does not throw.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 004
 * @par Priority       : High
 *
 * | Step | Description           | Expected      |
 * |:----:|-----------------------|---------------|
 * |  01  | Call SendUnmutePacket()| No exception |
 */
TEST_F(SubtecChannelTest, SendUnmutePacketDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.SendUnmutePacketDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendUnmutePacket());
    std::cout << "  SendUnmutePacket() completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.SendUnmutePacketDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Pause+Resume sequence does not throw or crash.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 005
 * @par Priority       : Medium
 *
 * | Step | Description                       | Expected      |
 * |:----:|-----------------------------------|---------------|
 * |  01  | Pause then Resume on TTML channel| No exception  |
 */
TEST_F(SubtecChannelTest, PauseResumeSequenceDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.PauseResumeSequenceDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendPausePacket());
    EXPECT_NO_THROW(ch->SendResumePacket());
    std::cout << "  Pause+Resume completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.PauseResumeSequenceDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Mute+Unmute sequence does not throw or crash.
 *
 * @par Test Group ID  : SubtecChannelSendPlayControl
 * @par Test Case ID   : 006
 * @par Priority       : Medium
 *
 * | Step | Description                      | Expected      |
 * |:----:|----------------------------------|---------------|
 * |  01  | Mute then Unmute on WEBVTT channel| No exception |
 */
TEST_F(SubtecChannelTest, MuteUnmuteSequenceDoesNotThrow)
{
    std::cout << "[SubtecChannelSendPlayControl.MuteUnmuteSequenceDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendMutePacket());
    EXPECT_NO_THROW(ch->SendUnmutePacket());
    std::cout << "  Mute+Unmute completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendPlayControl.MuteUnmuteSequenceDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: SubtecChannelSendCCAttribute
// ===========================================================================

/**
 * @brief SendCCSetAttributePacket() with all-zero attributes does not throw.
 *
 * @par Test Group ID  : SubtecChannelSendCCAttribute
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                            | Expected      |
 * |:----:|----------------------------------------|---------------|
 * |  01  | Call SendCCSetAttributePacket(0,0,all0)| No exception  |
 */
TEST_F(SubtecChannelTest, SendCCSetAttributePacketAllZeroDoesNotThrow)
{
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketAllZeroDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    attributesType attrs{};
    attrs.fill(0);
    EXPECT_NO_THROW(ch->SendCCSetAttributePacket(0, 0, attrs));
    std::cout << "  SendCCSetAttributePacket(0,0,zeros) completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketAllZeroDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendCCSetAttributePacket() with non-zero ccType and attribType mask.
 *
 * @par Test Group ID  : SubtecChannelSendCCAttribute
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description                                  | Expected     |
 * |:----:|----------------------------------------------|--------------|
 * |  01  | Call with ccType=3, attribType=0xFF, values  | No exception |
 */
TEST_F(SubtecChannelTest, SendCCSetAttributePacketNonZeroValuesDoesNotThrow)
{
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketNonZeroValuesDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    attributesType attrs{};
    for (size_t i = 0; i < attrs.size(); ++i)
        attrs[i] = static_cast<uint32_t>(i * 100 + 1);
    EXPECT_NO_THROW(ch->SendCCSetAttributePacket(3, 0xFF, attrs));
    std::cout << "  SendCCSetAttributePacket(3, 0xFF, values) completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketNonZeroValuesDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SendCCSetAttributePacket() with all-bits-set attribType mask.
 *
 * @par Test Group ID  : SubtecChannelSendCCAttribute
 * @par Test Case ID   : 003
 * @par Priority       : Medium
 *
 * | Step | Description                                     | Expected     |
 * |:----:|-------------------------------------------------|--------------|
 * |  01  | Call with attribType=0xFFFFFFFF (all bits set)  | No exception |
 */
TEST_F(SubtecChannelTest, SendCCSetAttributePacketAllBitsMaskDoesNotThrow)
{
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketAllBitsMaskDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    attributesType attrs{};
    attrs.fill(0xDEADBEEF);
    EXPECT_NO_THROW(ch->SendCCSetAttributePacket(1, 0xFFFFFFFF, attrs));
    std::cout << "  SendCCSetAttributePacket(all-bits-set) completed without exception" << std::endl;
    std::cout << "[SubtecChannelSendCCAttribute.SendCCSetAttributePacketAllBitsMaskDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: TtmlChannel
// ===========================================================================

/**
 * @brief TtmlChannel::SendSelectionPacket() with 1920x1080 does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                              | Expected     |
 * |:----:|------------------------------------------|--------------|
 * |  01  | Create TtmlChannel, call SendSelectionPacket(1920,1080) | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendSelectionPacket1920x1080)
{
    std::cout << "[TtmlChannel.TtmlChannelSendSelectionPacket1920x1080] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(1920, 1080));
    std::cout << "  SendSelectionPacket(1920, 1080) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendSelectionPacket1920x1080] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendSelectionPacket() with zero dimensions does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 *
 * | Step | Description                         | Expected     |
 * |:----:|-------------------------------------|--------------|
 * |  01  | Call SendSelectionPacket(0, 0)      | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendSelectionPacketZeroDimensions)
{
    std::cout << "[TtmlChannel.TtmlChannelSendSelectionPacketZeroDimensions] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(0, 0));
    std::cout << "  SendSelectionPacket(0, 0) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendSelectionPacketZeroDimensions] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket() with non-empty data and offset does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                               | Expected     |
 * |:----:|-------------------------------------------|--------------|
 * |  01  | Call SendDataPacket with 100-byte payload  | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendDataPacketNonEmptyData)
{
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketNonEmptyData] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    std::vector<uint8_t> data(100, 0xAB);
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 12345LL));
    std::cout << "  SendDataPacket(100 bytes, offset=12345) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketNonEmptyData] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket() with empty data vector does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 *
 * | Step | Description               | Expected     |
 * |:----:|---------------------------|--------------|
 * |  01  | Call SendDataPacket({})    | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendDataPacketEmptyData)
{
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketEmptyData] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    std::vector<uint8_t> data;
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 0LL));
    std::cout << "  SendDataPacket(empty, offset=0) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketEmptyData] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket() with negative time offset does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 005
 * @par Priority       : Medium
 *
 * | Step | Description                          | Expected     |
 * |:----:|--------------------------------------|--------------|
 * |  01  | Call SendDataPacket with offset = -1  | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendDataPacketNegativeOffset)
{
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketNegativeOffset] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), -1LL));
    std::cout << "  SendDataPacket(3 bytes, offset=-1) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendDataPacketNegativeOffset] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendTimestampPacket() with valid timestamp does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 006
 * @par Priority       : High
 *
 * | Step | Description                           | Expected     |
 * |:----:|---------------------------------------|--------------|
 * |  01  | Call SendTimestampPacket(90000)        | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendTimestampPacketValid)
{
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketValid] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendTimestampPacket(90000ULL));
    std::cout << "  SendTimestampPacket(90000) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketValid] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendTimestampPacket() with zero timestamp does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 007
 * @par Priority       : Low
 *
 * | Step | Description                  | Expected     |
 * |:----:|------------------------------|--------------|
 * |  01  | Call SendTimestampPacket(0)   | No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendTimestampPacketZero)
{
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketZero] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendTimestampPacket(0ULL));
    std::cout << "  SendTimestampPacket(0) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketZero] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendTimestampPacket() with max uint64 does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 008
 * @par Priority       : Low
 *
 * | Step | Description                       | Expected     |
 * |:----:|-----------------------------------|--------------|
 * |  01  | Call SendTimestampPacket(UINT64_MAX)| No exception |
 */
TEST_F(SubtecChannelTest, TtmlChannelSendTimestampPacketMaxValue)
{
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketMaxValue] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendTimestampPacket(UINT64_MAX));
    std::cout << "  SendTimestampPacket(UINT64_MAX) completed without exception" << std::endl;
    std::cout << "[TtmlChannel.TtmlChannelSendTimestampPacketMaxValue] - PASS" << std::endl;
}

// ===========================================================================
// Group: WebVttChannel
// ===========================================================================

/**
 * @brief WebVttChannel::SendSelectionPacket() with 1280x720 does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                              | Expected     |
 * |:----:|------------------------------------------|--------------|
 * |  01  | Create WebVttChannel via factory, call SendSelectionPacket | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendSelectionPacket1280x720)
{
    std::cout << "[WebVttChannel.WebVttChannelSendSelectionPacket1280x720] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(1280, 720));
    std::cout << "  SendSelectionPacket(1280, 720) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendSelectionPacket1280x720] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel::SendSelectionPacket() with max dimensions does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 002
 * @par Priority       : Low
 *
 * | Step | Description                              | Expected     |
 * |:----:|------------------------------------------|--------------|
 * |  01  | Call SendSelectionPacket(UINT32_MAX, UINT32_MAX) | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendSelectionPacketMaxDimensions)
{
    std::cout << "[WebVttChannel.WebVttChannelSendSelectionPacketMaxDimensions] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(UINT32_MAX, UINT32_MAX));
    std::cout << "  SendSelectionPacket(UINT32_MAX, UINT32_MAX) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendSelectionPacketMaxDimensions] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel::SendDataPacket() with non-empty data does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                           | Expected     |
 * |:----:|---------------------------------------|--------------|
 * |  01  | Call SendDataPacket(50-byte vector)    | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendDataPacketNonEmptyData)
{
    std::cout << "[WebVttChannel.WebVttChannelSendDataPacketNonEmptyData] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    std::vector<uint8_t> data(50, 0xCC);
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 100LL));
    std::cout << "  SendDataPacket(50 bytes, offset=100) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendDataPacketNonEmptyData] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel::SendDataPacket() with empty data and zero offset does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 *
 * | Step | Description                    | Expected     |
 * |:----:|--------------------------------|--------------|
 * |  01  | Call SendDataPacket({}, 0)      | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendDataPacketEmptyData)
{
    std::cout << "[WebVttChannel.WebVttChannelSendDataPacketEmptyData] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    std::vector<uint8_t> data;
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 0LL));
    std::cout << "  SendDataPacket(empty, 0) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendDataPacketEmptyData] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel::SendTimestampPacket() with typical PTS value does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 005
 * @par Priority       : High
 *
 * | Step | Description                   | Expected     |
 * |:----:|-------------------------------|--------------|
 * |  01  | Call SendTimestampPacket(5000) | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendTimestampPacketValid)
{
    std::cout << "[WebVttChannel.WebVttChannelSendTimestampPacketValid] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendTimestampPacket(5000ULL));
    std::cout << "  SendTimestampPacket(5000) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendTimestampPacketValid] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel::SendTimestampPacket() with zero does not throw.
 *
 * @par Test Group ID  : WebVttChannel
 * @par Test Case ID   : 006
 * @par Priority       : Low
 *
 * | Step | Description                  | Expected     |
 * |:----:|------------------------------|--------------|
 * |  01  | Call SendTimestampPacket(0)   | No exception |
 */
TEST_F(SubtecChannelTest, WebVttChannelSendTimestampPacketZero)
{
    std::cout << "[WebVttChannel.WebVttChannelSendTimestampPacketZero] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendTimestampPacket(0ULL));
    std::cout << "  SendTimestampPacket(0) completed without exception" << std::endl;
    std::cout << "[WebVttChannel.WebVttChannelSendTimestampPacketZero] - PASS" << std::endl;
}

// ===========================================================================
// Group: ClosedCaptionsChannel
// ===========================================================================

/**
 * @brief ClosedCaptionsChannel::SendDataPacketWithPTS() does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                              | Expected     |
 * |:----:|------------------------------------------|--------------|
 * |  01  | Create CC channel, SendDataPacketWithPTS | No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendDataPacketWithPTSDoesNotThrow)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketWithPTSDoesNotThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0x00, 0x01, 0x02, 0xFF};
    EXPECT_NO_THROW(ch.SendDataPacketWithPTS(12345, data, sizeof(data)));
    std::cout << "  SendDataPacketWithPTS(pts=12345, 4 bytes) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketWithPTSDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketWithPTS() with zero PTS.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 *
 * | Step | Description                          | Expected     |
 * |:----:|--------------------------------------|--------------|
 * |  01  | Call SendDataPacketWithPTS(pts=0)    | No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendDataPacketWithPTSZero)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketWithPTSZero] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0xAA, 0xBB};
    EXPECT_NO_THROW(ch.SendDataPacketWithPTS(0, data, sizeof(data)));
    std::cout << "  SendDataPacketWithPTS(pts=0, 2 bytes) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketWithPTSZero] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketNoPTS() does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                          | Expected     |
 * |:----:|--------------------------------------|--------------|
 * |  01  | Call SendDataPacketNoPTS(4 bytes)     | No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendDataPacketNoPTSDoesNotThrow)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketNoPTSDoesNotThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_NO_THROW(ch.SendDataPacketNoPTS(data, sizeof(data)));
    std::cout << "  SendDataPacketNoPTS(4 bytes) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketNoPTSDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketNoPTS() with 1 byte does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 004
 * @par Priority       : Low
 *
 * | Step | Description                         | Expected     |
 * |:----:|-------------------------------------|--------------|
 * |  01  | Call SendDataPacketNoPTS(1 byte)     | No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendDataPacketNoPTSSingleByte)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketNoPTSSingleByte] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0xFF};
    EXPECT_NO_THROW(ch.SendDataPacketNoPTS(data, 1));
    std::cout << "  SendDataPacketNoPTS(1 byte=0xFF) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendDataPacketNoPTSSingleByte] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendActiveTypePacket() with CEA_608 type.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 005
 * @par Priority       : High
 *
 * | Step | Description                                 | Expected     |
 * |:----:|---------------------------------------------|--------------|
 * |  01  | Call SendActiveTypePacket(CEA_608, service=1)| No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendActiveTypePacketCEA608)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketCEA608] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW(ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 1));
    std::cout << "  SendActiveTypePacket(CEA608, channel=1) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketCEA608] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendActiveTypePacket() with CEA_708 type.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 006
 * @par Priority       : High
 *
 * | Step | Description                                 | Expected     |
 * |:----:|---------------------------------------------|--------------|
 * |  01  | Call SendActiveTypePacket(CEA_708, service=2)| No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendActiveTypePacketCEA708)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketCEA708] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW(ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, 2));
    std::cout << "  SendActiveTypePacket(CEA708, channel=2) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketCEA708] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendActiveTypePacket() with service=0.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 007
 * @par Priority       : Medium
 *
 * | Step | Description                                 | Expected     |
 * |:----:|---------------------------------------------|--------------|
 * |  01  | Call SendActiveTypePacket(CEA_608, service=0)| No exception |
 */
TEST_F(SubtecChannelTest, CCChannelSendActiveTypePacketServiceZero)
{
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketServiceZero] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW(ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 0));
    std::cout << "  SendActiveTypePacket(CEA608, channel=0) completed without exception" << std::endl;
    std::cout << "[ClosedCaptionsChannel.CCChannelSendActiveTypePacketServiceZero] - PASS" << std::endl;
}

// ===========================================================================
// Group: Integration
// ===========================================================================

/**
 * @brief Full TTML channel lifecycle: Selection → Data → Timestamp → Reset.
 *
 * @par Test Group ID  : SubtecChannelIntegration
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                                | Expected     |
 * |:----:|--------------------------------------------|--------------|
 * |  01  | Create TTML channel via factory            | Non-null ptr |
 * |  02  | Send Selection, Data, Timestamp, Reset     | No exception |
 */
TEST_F(SubtecChannelTest, TtmlFullLifecycleDoesNotThrow)
{
    std::cout << "[SubtecChannelIntegration.TtmlFullLifecycleDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(1920, 1080));
    std::vector<uint8_t> data(200, 0x55);
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 0LL));
    EXPECT_NO_THROW(ch->SendTimestampPacket(90000ULL));
    EXPECT_NO_THROW(ch->SendPausePacket());
    EXPECT_NO_THROW(ch->SendResumePacket());
    EXPECT_NO_THROW(ch->SendResetChannelPacket());
    EXPECT_NO_THROW(ch->SendResetAllPacket());
    std::cout << "  TTML full lifecycle completed without exception" << std::endl;
    std::cout << "[SubtecChannelIntegration.TtmlFullLifecycleDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Full WebVTT channel lifecycle: Selection → Data → Timestamp → Mute → Unmute → Reset.
 *
 * @par Test Group ID  : SubtecChannelIntegration
 * @par Test Case ID   : 002
 * @par Priority       : High
 *
 * | Step | Description                                | Expected     |
 * |:----:|--------------------------------------------|--------------|
 * |  01  | Create WEBVTT channel via factory          | Non-null ptr |
 * |  02  | Send Selection, Data, Timestamp, Mute... | No exception |
 */
TEST_F(SubtecChannelTest, WebVttFullLifecycleDoesNotThrow)
{
    std::cout << "[SubtecChannelIntegration.WebVttFullLifecycleDoesNotThrow] - START" << std::endl;
    auto ch = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
    ASSERT_NE(ch, nullptr);
    EXPECT_NO_THROW(ch->SendSelectionPacket(640, 480));
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF};
    EXPECT_NO_THROW(ch->SendDataPacket(std::move(data), 500LL));
    EXPECT_NO_THROW(ch->SendTimestampPacket(1000ULL));
    EXPECT_NO_THROW(ch->SendMutePacket());
    EXPECT_NO_THROW(ch->SendUnmutePacket());
    EXPECT_NO_THROW(ch->SendResetAllPacket());
    std::cout << "  WebVTT full lifecycle completed without exception" << std::endl;
    std::cout << "[SubtecChannelIntegration.WebVttFullLifecycleDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Full CC channel lifecycle: WithPTS → NoPTS → ActiveType → Reset.
 *
 * @par Test Group ID  : SubtecChannelIntegration
 * @par Test Case ID   : 003
 * @par Priority       : High
 *
 * | Step | Description                            | Expected     |
 * |:----:|----------------------------------------|--------------|
 * |  01  | Create CC channel, send all packet types| No exception |
 */
TEST_F(SubtecChannelTest, CCFullLifecycleDoesNotThrow)
{
    std::cout << "[SubtecChannelIntegration.CCFullLifecycleDoesNotThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0x47, 0x61, 0x6D, 0x65};
    EXPECT_NO_THROW(ch.SendDataPacketWithPTS(99, data, sizeof(data)));
    EXPECT_NO_THROW(ch.SendDataPacketNoPTS(data, sizeof(data)));
    EXPECT_NO_THROW(ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 1));
    EXPECT_NO_THROW(ch.SendPausePacket());
    EXPECT_NO_THROW(ch.SendResumePacket());
    EXPECT_NO_THROW(ch.SendResetAllPacket());
    std::cout << "  CC full lifecycle completed without exception" << std::endl;
    std::cout << "[SubtecChannelIntegration.CCFullLifecycleDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Create all three channel types and use each: stress across 10 iterations.
 *
 * @par Test Group ID  : SubtecChannelIntegration
 * @par Test Case ID   : 004
 * @par Priority       : Low
 *
 * | Step | Description                                  | Expected     |
 * |:----:|----------------------------------------------|--------------|
 * |  01  | 10x create TTML, WEBVTT channels; send packets| No exception |
 */
TEST_F(SubtecChannelTest, StressMultipleChannelCreationAndSend)
{
    std::cout << "[SubtecChannelIntegration.StressMultipleChannelCreationAndSend] - START" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        auto ttml = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::TTML);
        ASSERT_NE(ttml, nullptr);
        EXPECT_NO_THROW(ttml->SendPausePacket());
        EXPECT_NO_THROW(ttml->SendResumePacket());

        auto webvtt = SubtecChannel::SubtecChannelFactory(SubtecChannel::ChannelType::WEBVTT);
        ASSERT_NE(webvtt, nullptr);
        EXPECT_NO_THROW(webvtt->SendMutePacket());
        EXPECT_NO_THROW(webvtt->SendUnmutePacket());

        // Flush after each iteration to avoid queue bloat
        PacketSender::Instance()->Flush();
    }
    std::cout << "  Stress 10x create+send completed without exception" << std::endl;
    std::cout << "[SubtecChannelIntegration.StressMultipleChannelCreationAndSend] - PASS" << std::endl;
}
