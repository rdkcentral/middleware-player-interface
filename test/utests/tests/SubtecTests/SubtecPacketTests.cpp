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
 * @file SubtecPacketTests.cpp
 * @brief Comprehensive unit tests for all Subtec packet types.
 *
 * Covers: Packet base class, DummyPacket, PausePacket, ResumePacket,
 *         MutePacket, UnmutePacket, ResetAllPacket, ResetChannelPacket,
 *         CCSetAttributePacket — constructors, buffer layout, type values,
 *         counter accessors, edge cases and stress tests.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <limits>
#include <array>
#include <cstdint>
#include "SubtecPacket.hpp"

// ---------------------------------------------------------------------------
// Helper utilities
// ---------------------------------------------------------------------------

/** @brief Decode a 4-byte little-endian word from a byte buffer. */
static uint32_t readLE32(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint32_t>(buf[offset])
         | (static_cast<uint32_t>(buf[offset + 1]) << 8)
         | (static_cast<uint32_t>(buf[offset + 2]) << 16)
         | (static_cast<uint32_t>(buf[offset + 3]) << 24);
}

/** @brief Decode a 8-byte little-endian word from a byte buffer. */
static uint64_t readLE64(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint64_t>(readLE32(buf, offset))
         | (static_cast<uint64_t>(readLE32(buf, offset + 4)) << 32);
}

// PacketType numeric constants (mirror of Packet::PacketType enum)
static constexpr uint32_t TYPE_ZERO              = 0;
static constexpr uint32_t TYPE_PES_DATA          = 1;
static constexpr uint32_t TYPE_TIMESTAMP         = 2;
static constexpr uint32_t TYPE_RESET_ALL         = 3;
static constexpr uint32_t TYPE_RESET_CHANNEL     = 4;
static constexpr uint32_t TYPE_SUBTITLE_SELECTION= 5;
static constexpr uint32_t TYPE_TELETEXT_SELECTION= 6;
static constexpr uint32_t TYPE_TTML_SELECTION    = 7;
static constexpr uint32_t TYPE_TTML_DATA         = 8;
static constexpr uint32_t TYPE_TTML_TIMESTAMP    = 9;
static constexpr uint32_t TYPE_CC_DATA           = 10;
static constexpr uint32_t TYPE_PAUSE             = 11;
static constexpr uint32_t TYPE_RESUME            = 12;
static constexpr uint32_t TYPE_MUTE              = 13;
static constexpr uint32_t TYPE_UNMUTE            = 14;
static constexpr uint32_t TYPE_WEBVTT_SELECTION  = 15;
static constexpr uint32_t TYPE_WEBVTT_DATA       = 16;
static constexpr uint32_t TYPE_WEBVTT_TIMESTAMP  = 17;
static constexpr uint32_t TYPE_CC_SET_ATTRIBUTE  = 18;

// ===========================================================================
// PacketBase tests — base Packet class
// ===========================================================================

/**
 * @brief Default constructor sets counter to UINT32_MAX.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 001
 * @par Priority: High
 *
 * | Step | Description | Expected |
 * |:----:|-------------|---------|
 * | 01 | Construct DummyPacket (inherits Packet()) | getCounter() == UINT32_MAX |
 */
TEST(PacketBase, DefaultConstructorCounterIsUINT32MAX)
{
    std::cout << "[PacketBase.DefaultConstructorCounterIsUINT32MAX] - START" << std::endl;
    DummyPacket pkt;
    const uint32_t expected = std::numeric_limits<uint32_t>::max();
    std::cout << "  Expected counter: " << expected << std::endl;
    std::cout << "  Actual counter:   " << pkt.getCounter() << std::endl;
    EXPECT_EQ(pkt.getCounter(), expected);
    std::cout << "[PacketBase.DefaultConstructorCounterIsUINT32MAX] - PASS" << std::endl;
}

/**
 * @brief Counter constructor stores the given counter value.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 002
 * @par Priority: High
 *
 * | Step | Description | Expected |
 * |:----:|-------------|---------|
 * | 01 | Construct PausePacket with counter=42 | getCounter()==42 |
 */
TEST(PacketBase, CounterConstructorStoresValue)
{
    std::cout << "[PacketBase.CounterConstructorStoresValue] - START" << std::endl;
    PausePacket pkt(1u, 42u);
    std::cout << "  Constructed PausePacket(channelId=1, counter=42)" << std::endl;
    std::cout << "  getCounter() = " << pkt.getCounter() << std::endl;
    EXPECT_EQ(pkt.getCounter(), 42u);
    std::cout << "[PacketBase.CounterConstructorStoresValue] - PASS" << std::endl;
}

/**
 * @brief Default Packet (via DummyPacket) has an empty buffer.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(PacketBase, DefaultConstructorBufferIsEmpty)
{
    std::cout << "[PacketBase.DefaultConstructorBufferIsEmpty] - START" << std::endl;
    DummyPacket pkt;
    std::cout << "  Buffer size: " << pkt.getBytes().size() << std::endl;
    EXPECT_TRUE(pkt.getBytes().empty());
    std::cout << "[PacketBase.DefaultConstructorBufferIsEmpty] - PASS" << std::endl;
}

/**
 * @brief getTypeString returns correct string for all known packet types.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(PacketBase, GetTypeStringAllKnownTypes)
{
    std::cout << "[PacketBase.GetTypeStringAllKnownTypes] - START" << std::endl;
    DummyPacket helper;

    struct { uint32_t type; const char* expected; } cases[] = {
        { TYPE_PES_DATA,           "PES_DATA"           },
        { TYPE_TIMESTAMP,          "TIMESTAMP"          },
        { TYPE_RESET_ALL,          "RESET_ALL"          },
        { TYPE_RESET_CHANNEL,      "RESET_CHANNEL"      },
        { TYPE_SUBTITLE_SELECTION, "SUBTITLE_SELECTION" },
        { TYPE_TELETEXT_SELECTION, "TELETEXT_SELECTION" },
        { TYPE_TTML_SELECTION,     "TTML_SELECTION"     },
        { TYPE_TTML_DATA,          "TTML_DATA"          },
        { TYPE_TTML_TIMESTAMP,     "TTML_TIMESTAMP"     },
        { TYPE_CC_DATA,            "CC_DATA"            },
        { TYPE_PAUSE,              "PAUSE"              },
        { TYPE_RESUME,             "RESUME"             },
        { TYPE_MUTE,               "MUTE"               },
        { TYPE_UNMUTE,             "UNMUTE"             },
        { TYPE_WEBVTT_SELECTION,   "WEBVTT_SELECTION"   },
        { TYPE_WEBVTT_DATA,        "WEBVTT_DATA"        },
        { TYPE_WEBVTT_TIMESTAMP,   "WEBVTT_TIMESTAMP"   },
        { TYPE_CC_SET_ATTRIBUTE,   "CC_SET_ATTRIBUTE"   },
    };

    for (auto& c : cases)
    {
        std::string result = helper.getTypeString(c.type);
        std::cout << "  type=" << c.type << " -> \"" << result << "\" (expected \"" << c.expected << "\")" << std::endl;
        EXPECT_EQ(result, c.expected) << "  Failed for type " << c.type;
    }
    std::cout << "[PacketBase.GetTypeStringAllKnownTypes] - PASS" << std::endl;
}

/**
 * @brief getTypeString returns "UNKNOWN" for an unrecognised type.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(PacketBase, GetTypeStringUnknownType)
{
    std::cout << "[PacketBase.GetTypeStringUnknownType] - START" << std::endl;
    DummyPacket helper;
    std::string result = helper.getTypeString(9999u);
    std::cout << "  getTypeString(9999) = \"" << result << "\"" << std::endl;
    EXPECT_EQ(result, "UNKNOWN");
    std::cout << "[PacketBase.GetTypeStringUnknownType] - PASS" << std::endl;
}

/**
 * @brief getTypeString returns "INVALID" for 0xFFFFFFFF.
 *
 * @par Test Group ID: PacketBase
 * @par Test Case ID: 006
 * @par Priority: Low
 */
TEST(PacketBase, GetTypeStringInvalidType)
{
    std::cout << "[PacketBase.GetTypeStringInvalidType] - START" << std::endl;
    DummyPacket helper;
    std::string result = helper.getTypeString(0xFFFFFFFFu);
    std::cout << "  getTypeString(0xFFFFFFFF) = \"" << result << "\"" << std::endl;
    EXPECT_EQ(result, "INVALID");
    std::cout << "[PacketBase.GetTypeStringInvalidType] - PASS" << std::endl;
}

// ===========================================================================
// DummyPacket tests
// ===========================================================================

/**
 * @brief DummyPacket default constructor does not throw.
 *
 * @par Test Group ID: DummyPacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(DummyPacket, DefaultConstructorNoThrow)
{
    std::cout << "[DummyPacket.DefaultConstructorNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        DummyPacket pkt;
        std::cout << "  DummyPacket constructed OK" << std::endl;
    });
    std::cout << "[DummyPacket.DefaultConstructorNoThrow] - PASS" << std::endl;
}

/**
 * @brief DummyPacket buffer is empty after default construction.
 *
 * @par Test Group ID: DummyPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(DummyPacket, BufferIsEmpty)
{
    std::cout << "[DummyPacket.BufferIsEmpty] - START" << std::endl;
    DummyPacket pkt;
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << std::endl;
    EXPECT_TRUE(pkt.getBytes().empty());
    EXPECT_EQ(pkt.getBytes().size(), 0u);
    std::cout << "[DummyPacket.BufferIsEmpty] - PASS" << std::endl;
}

/**
 * @brief DummyPacket getCounter returns UINT32_MAX.
 *
 * @par Test Group ID: DummyPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(DummyPacket, CounterIsUINT32MAX)
{
    std::cout << "[DummyPacket.CounterIsUINT32MAX] - START" << std::endl;
    DummyPacket pkt;
    const uint32_t expected = std::numeric_limits<uint32_t>::max();
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected " << expected << ")" << std::endl;
    EXPECT_EQ(pkt.getCounter(), expected);
    std::cout << "[DummyPacket.CounterIsUINT32MAX] - PASS" << std::endl;
}

/**
 * @brief Multiple DummyPacket instances are independent.
 *
 * @par Test Group ID: DummyPacket
 * @par Test Case ID: 004
 * @par Priority: Medium
 */
TEST(DummyPacket, MultipleInstancesAreIndependent)
{
    std::cout << "[DummyPacket.MultipleInstancesAreIndependent] - START" << std::endl;
    DummyPacket a, b, c;
    EXPECT_EQ(a.getCounter(), b.getCounter());
    EXPECT_EQ(b.getCounter(), c.getCounter());
    EXPECT_TRUE(a.getBytes().empty());
    EXPECT_TRUE(b.getBytes().empty());
    EXPECT_TRUE(c.getBytes().empty());
    std::cout << "  Three independent DummyPacket instances all have empty buffers  and UINT32_MAX counters" << std::endl;
    std::cout << "[DummyPacket.MultipleInstancesAreIndependent] - PASS" << std::endl;
}

// ===========================================================================
// PausePacket tests
// ===========================================================================

/**
 * @brief PausePacket construction does not throw.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(PausePacket, ConstructionNoThrow)
{
    std::cout << "[PausePacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        PausePacket pkt(1u, 10u);
        std::cout << "  PausePacket(channelId=1, counter=10) constructed OK" << std::endl;
    });
    std::cout << "[PausePacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief PausePacket buffer is exactly 16 bytes.
 *
 * Layout: type(4) + counter(4) + size(4) + channelId(4) = 16
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(PausePacket, BufferSizeIs16)
{
    std::cout << "[PausePacket.BufferSizeIs16] - START" << std::endl;
    PausePacket pkt(1u, 10u);
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 16)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 16u);
    std::cout << "[PausePacket.BufferSizeIs16] - PASS" << std::endl;
}

/**
 * @brief PausePacket getType() returns TYPE_PAUSE (11).
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(PausePacket, TypeIsPause)
{
    std::cout << "[PausePacket.TypeIsPause] - START" << std::endl;
    PausePacket pkt(1u, 10u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_PAUSE << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_PAUSE);
    std::cout << "[PausePacket.TypeIsPause] - PASS" << std::endl;
}

/**
 * @brief PausePacket getCounter() returns the counter passed to the constructor.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(PausePacket, CounterStoredCorrectly)
{
    std::cout << "[PausePacket.CounterStoredCorrectly] - START" << std::endl;
    PausePacket pkt(5u, 77u);
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 77)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 77u);
    std::cout << "[PausePacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief PausePacket buffer encodes channelId at bytes 12-15 in little-endian.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 005
 * @par Priority: High
 */
TEST(PausePacket, ChannelIdEncodedAtCorrectOffset)
{
    std::cout << "[PausePacket.ChannelIdEncodedAtCorrectOffset] - START" << std::endl;
    const uint32_t channelId = 42u;
    PausePacket pkt(channelId, 1u);
    const auto& buf = pkt.getBytes();
    uint32_t decoded = readLE32(buf, 12);
    std::cout << "  channelId in constructor: " << channelId
              << ", decoded from buf[12..15]: " << decoded << std::endl;
    EXPECT_EQ(decoded, channelId);
    std::cout << "[PausePacket.ChannelIdEncodedAtCorrectOffset] - PASS" << std::endl;
}

/**
 * @brief PausePacket size field at bytes 8-11 is always 4.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 006
 * @par Priority: Medium
 */
TEST(PausePacket, SizeFieldIs4)
{
    std::cout << "[PausePacket.SizeFieldIs4] - START" << std::endl;
    PausePacket pkt(1u, 1u);
    const auto& buf = pkt.getBytes();
    uint32_t sizeField = readLE32(buf, 8);
    std::cout << "  size field at buf[8..11]: " << sizeField << " (expected 4)" << std::endl;
    EXPECT_EQ(sizeField, 4u);
    std::cout << "[PausePacket.SizeFieldIs4] - PASS" << std::endl;
}

/**
 * @brief PausePacket accepts zero channelId and zero counter without issues.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 007
 * @par Priority: Medium
 */
TEST(PausePacket, ZeroChannelIdAndCounter)
{
    std::cout << "[PausePacket.ZeroChannelIdAndCounter] - START" << std::endl;
    EXPECT_NO_THROW({
        PausePacket pkt(0u, 0u);
        EXPECT_EQ(pkt.getBytes().size(), 16u);
        EXPECT_EQ(pkt.getCounter(), 0u);
        EXPECT_EQ(readLE32(pkt.getBytes(), 12), 0u);
        std::cout << "  PausePacket(0, 0) OK" << std::endl;
    });
    std::cout << "[PausePacket.ZeroChannelIdAndCounter] - PASS" << std::endl;
}

/**
 * @brief PausePacket accepts UINT32_MAX for both channelId and counter.
 *
 * @par Test Group ID: PausePacket
 * @par Test Case ID: 008
 * @par Priority: Medium
 */
TEST(PausePacket, MaxChannelIdAndCounter)
{
    std::cout << "[PausePacket.MaxChannelIdAndCounter] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({
        PausePacket pkt(MAX, MAX);
        EXPECT_EQ(pkt.getBytes().size(), 16u);
        EXPECT_EQ(pkt.getCounter(), MAX);
        EXPECT_EQ(readLE32(pkt.getBytes(), 12), MAX);
        std::cout << "  PausePacket(UINT32_MAX, UINT32_MAX) OK" << std::endl;
    });
    std::cout << "[PausePacket.MaxChannelIdAndCounter] - PASS" << std::endl;
}

// ===========================================================================
// ResumePacket tests
// ===========================================================================

/**
 * @brief ResumePacket construction does not throw.
 *
 * @par Test Group ID: ResumePacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(ResumePacket, ConstructionNoThrow)
{
    std::cout << "[ResumePacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ ResumePacket pkt(1u, 10u); });
    std::cout << "[ResumePacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ResumePacket buffer is exactly 16 bytes.
 *
 * @par Test Group ID: ResumePacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(ResumePacket, BufferSizeIs16)
{
    std::cout << "[ResumePacket.BufferSizeIs16] - START" << std::endl;
    ResumePacket pkt(2u, 5u);
    EXPECT_EQ(pkt.getBytes().size(), 16u);
    std::cout << "  Buffer size: " << pkt.getBytes().size() << std::endl;
    std::cout << "[ResumePacket.BufferSizeIs16] - PASS" << std::endl;
}

/**
 * @brief ResumePacket getType() returns TYPE_RESUME (12).
 *
 * @par Test Group ID: ResumePacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(ResumePacket, TypeIsResume)
{
    std::cout << "[ResumePacket.TypeIsResume] - START" << std::endl;
    ResumePacket pkt(1u, 1u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_RESUME << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_RESUME);
    std::cout << "[ResumePacket.TypeIsResume] - PASS" << std::endl;
}

/**
 * @brief ResumePacket stores counter and channelId correctly.
 *
 * @par Test Group ID: ResumePacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(ResumePacket, CounterAndChannelIdCorrect)
{
    std::cout << "[ResumePacket.CounterAndChannelIdCorrect] - START" << std::endl;
    ResumePacket pkt(99u, 55u);
    EXPECT_EQ(pkt.getCounter(), 55u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 12), 99u);
    std::cout << "  counter=55, channelId=99 verified in buffer" << std::endl;
    std::cout << "[ResumePacket.CounterAndChannelIdCorrect] - PASS" << std::endl;
}

/**
 * @brief ResumePacket handles zero and max values.
 *
 * @par Test Group ID: ResumePacket
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(ResumePacket, ZeroAndMaxValues)
{
    std::cout << "[ResumePacket.ZeroAndMaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({ ResumePacket pkt(0u, 0u); });
    EXPECT_NO_THROW({ ResumePacket pkt(MAX, MAX); });
    std::cout << "  Zero and max value construction OK" << std::endl;
    std::cout << "[ResumePacket.ZeroAndMaxValues] - PASS" << std::endl;
}

// ===========================================================================
// MutePacket tests
// ===========================================================================

/**
 * @brief MutePacket construction does not throw.
 *
 * @par Test Group ID: MutePacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(MutePacket, ConstructionNoThrow)
{
    std::cout << "[MutePacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ MutePacket pkt(1u, 10u); });
    std::cout << "[MutePacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief MutePacket buffer is exactly 16 bytes.
 *
 * @par Test Group ID: MutePacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(MutePacket, BufferSizeIs16)
{
    std::cout << "[MutePacket.BufferSizeIs16] - START" << std::endl;
    MutePacket pkt(1u, 1u);
    EXPECT_EQ(pkt.getBytes().size(), 16u);
    std::cout << "  Buffer size: " << pkt.getBytes().size() << std::endl;
    std::cout << "[MutePacket.BufferSizeIs16] - PASS" << std::endl;
}

/**
 * @brief MutePacket getType() returns TYPE_MUTE (13).
 *
 * @par Test Group ID: MutePacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(MutePacket, TypeIsMute)
{
    std::cout << "[MutePacket.TypeIsMute] - START" << std::endl;
    MutePacket pkt(1u, 1u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_MUTE << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_MUTE);
    std::cout << "[MutePacket.TypeIsMute] - PASS" << std::endl;
}

/**
 * @brief MutePacket counter and channelId are encoded correctly.
 *
 * @par Test Group ID: MutePacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(MutePacket, CounterAndChannelIdCorrect)
{
    std::cout << "[MutePacket.CounterAndChannelIdCorrect] - START" << std::endl;
    MutePacket pkt(11u, 22u);
    EXPECT_EQ(pkt.getCounter(), 22u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 12), 11u);
    std::cout << "  counter=22, channelId=11 verified" << std::endl;
    std::cout << "[MutePacket.CounterAndChannelIdCorrect] - PASS" << std::endl;
}

/**
 * @brief MutePacket handles zero and max values.
 *
 * @par Test Group ID: MutePacket
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(MutePacket, ZeroAndMaxValues)
{
    std::cout << "[MutePacket.ZeroAndMaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({ MutePacket pkt(0u, 0u); });
    EXPECT_NO_THROW({ MutePacket pkt(MAX, MAX); });
    std::cout << "[MutePacket.ZeroAndMaxValues] - PASS" << std::endl;
}

// ===========================================================================
// UnmutePacket tests
// ===========================================================================

/**
 * @brief UnmutePacket construction does not throw.
 *
 * @par Test Group ID: UnmutePacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(UnmutePacket, ConstructionNoThrow)
{
    std::cout << "[UnmutePacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ UnmutePacket pkt(1u, 10u); });
    std::cout << "[UnmutePacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief UnmutePacket buffer is exactly 16 bytes.
 *
 * @par Test Group ID: UnmutePacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(UnmutePacket, BufferSizeIs16)
{
    std::cout << "[UnmutePacket.BufferSizeIs16] - START" << std::endl;
    UnmutePacket pkt(1u, 1u);
    EXPECT_EQ(pkt.getBytes().size(), 16u);
    std::cout << "  Buffer size: " << pkt.getBytes().size() << std::endl;
    std::cout << "[UnmutePacket.BufferSizeIs16] - PASS" << std::endl;
}

/**
 * @brief UnmutePacket getType() returns TYPE_UNMUTE (14).
 *
 * @par Test Group ID: UnmutePacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(UnmutePacket, TypeIsUnmute)
{
    std::cout << "[UnmutePacket.TypeIsUnmute] - START" << std::endl;
    UnmutePacket pkt(1u, 1u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_UNMUTE << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_UNMUTE);
    std::cout << "[UnmutePacket.TypeIsUnmute] - PASS" << std::endl;
}

/**
 * @brief UnmutePacket counter and channelId are encoded correctly.
 *
 * @par Test Group ID: UnmutePacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(UnmutePacket, CounterAndChannelIdCorrect)
{
    std::cout << "[UnmutePacket.CounterAndChannelIdCorrect] - START" << std::endl;
    UnmutePacket pkt(33u, 44u);
    EXPECT_EQ(pkt.getCounter(), 44u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 12), 33u);
    std::cout << "  counter=44, channelId=33 verified" << std::endl;
    std::cout << "[UnmutePacket.CounterAndChannelIdCorrect] - PASS" << std::endl;
}

/**
 * @brief UnmutePacket handles zero and max values.
 *
 * @par Test Group ID: UnmutePacket
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(UnmutePacket, ZeroAndMaxValues)
{
    std::cout << "[UnmutePacket.ZeroAndMaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({ UnmutePacket pkt(0u, 0u); });
    EXPECT_NO_THROW({ UnmutePacket pkt(MAX, MAX); });
    std::cout << "[UnmutePacket.ZeroAndMaxValues] - PASS" << std::endl;
}

// ===========================================================================
// ResetAllPacket tests
// ===========================================================================

/**
 * @brief ResetAllPacket construction does not throw.
 *
 * @par Test Group ID: ResetAllPacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(ResetAllPacket, ConstructionNoThrow)
{
    std::cout << "[ResetAllPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ ResetAllPacket pkt; });
    std::cout << "[ResetAllPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ResetAllPacket buffer is exactly 12 bytes.
 *
 * Layout: type(4) + counter(4) + size(4) = 12.
 *
 * @par Test Group ID: ResetAllPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(ResetAllPacket, BufferSizeIs12)
{
    std::cout << "[ResetAllPacket.BufferSizeIs12] - START" << std::endl;
    ResetAllPacket pkt;
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 12)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 12u);
    std::cout << "[ResetAllPacket.BufferSizeIs12] - PASS" << std::endl;
}

/**
 * @brief ResetAllPacket getType() returns TYPE_RESET_ALL (3).
 *
 * @par Test Group ID: ResetAllPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(ResetAllPacket, TypeIsResetAll)
{
    std::cout << "[ResetAllPacket.TypeIsResetAll] - START" << std::endl;
    ResetAllPacket pkt;
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_RESET_ALL << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_RESET_ALL);
    std::cout << "[ResetAllPacket.TypeIsResetAll] - PASS" << std::endl;
}

/**
 * @brief ResetAllPacket counter is zero (fixed value).
 *
 * @par Test Group ID: ResetAllPacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(ResetAllPacket, CounterIsZero)
{
    std::cout << "[ResetAllPacket.CounterIsZero] - START" << std::endl;
    ResetAllPacket pkt;
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 0)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 0u);
    std::cout << "[ResetAllPacket.CounterIsZero] - PASS" << std::endl;
}

/**
 * @brief ResetAllPacket counter and size fields in buffer are both zero.
 *
 * @par Test Group ID: ResetAllPacket
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(ResetAllPacket, BufferFieldsAreZero)
{
    std::cout << "[ResetAllPacket.BufferFieldsAreZero] - START" << std::endl;
    ResetAllPacket pkt;
    const auto& buf = pkt.getBytes();
    uint32_t counterField = readLE32(buf, 4);
    uint32_t sizeField    = readLE32(buf, 8);
    std::cout << "  counter field: " << counterField << " (expected 0)" << std::endl;
    std::cout << "  size field:    " << sizeField    << " (expected 0)" << std::endl;
    EXPECT_EQ(counterField, 0u);
    EXPECT_EQ(sizeField,    0u);
    std::cout << "[ResetAllPacket.BufferFieldsAreZero] - PASS" << std::endl;
}

// ===========================================================================
// ResetChannelPacket tests
// ===========================================================================

/**
 * @brief ResetChannelPacket construction does not throw.
 *
 * @par Test Group ID: ResetChannelPacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(ResetChannelPacket, ConstructionNoThrow)
{
    std::cout << "[ResetChannelPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ ResetChannelPacket pkt(1u, 5u); });
    std::cout << "[ResetChannelPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ResetChannelPacket buffer is exactly 16 bytes.
 *
 * @par Test Group ID: ResetChannelPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(ResetChannelPacket, BufferSizeIs16)
{
    std::cout << "[ResetChannelPacket.BufferSizeIs16] - START" << std::endl;
    ResetChannelPacket pkt(1u, 5u);
    EXPECT_EQ(pkt.getBytes().size(), 16u);
    std::cout << "  Buffer size: " << pkt.getBytes().size() << std::endl;
    std::cout << "[ResetChannelPacket.BufferSizeIs16] - PASS" << std::endl;
}

/**
 * @brief ResetChannelPacket getType() returns TYPE_RESET_CHANNEL (4).
 *
 * @par Test Group ID: ResetChannelPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(ResetChannelPacket, TypeIsResetChannel)
{
    std::cout << "[ResetChannelPacket.TypeIsResetChannel] - START" << std::endl;
    ResetChannelPacket pkt(1u, 1u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_RESET_CHANNEL << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_RESET_CHANNEL);
    std::cout << "[ResetChannelPacket.TypeIsResetChannel] - PASS" << std::endl;
}

/**
 * @brief ResetChannelPacket counter and channelId are encoded correctly.
 *
 * @par Test Group ID: ResetChannelPacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(ResetChannelPacket, CounterAndChannelIdCorrect)
{
    std::cout << "[ResetChannelPacket.CounterAndChannelIdCorrect] - START" << std::endl;
    ResetChannelPacket pkt(77u, 88u);
    EXPECT_EQ(pkt.getCounter(), 88u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 12), 77u);
    std::cout << "  counter=88, channelId=77 verified in buffer" << std::endl;
    std::cout << "[ResetChannelPacket.CounterAndChannelIdCorrect] - PASS" << std::endl;
}

/**
 * @brief ResetChannelPacket handles zero and max values.
 *
 * @par Test Group ID: ResetChannelPacket
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(ResetChannelPacket, ZeroAndMaxValues)
{
    std::cout << "[ResetChannelPacket.ZeroAndMaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({ ResetChannelPacket pkt(0u, 0u); });
    EXPECT_NO_THROW({ ResetChannelPacket pkt(MAX, MAX); });
    std::cout << "[ResetChannelPacket.ZeroAndMaxValues] - PASS" << std::endl;
}

// ===========================================================================
// CCSetAttributePacket tests
// ===========================================================================

/** Helper: build a default attributesType filled with a given value. */
static attributesType makeAttribs(uint32_t fill = 0u)
{
    attributesType arr;
    arr.fill(fill);
    return arr;
}

/**
 * @brief CCSetAttributePacket construction does not throw.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(CCSetAttributePacket, ConstructionNoThrow)
{
    std::cout << "[CCSetAttributePacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        CCSetAttributePacket pkt(1u, 1u, 0u, 0u, makeAttribs(0u));
        std::cout << "  CCSetAttributePacket constructed OK" << std::endl;
    });
    std::cout << "[CCSetAttributePacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket buffer is exactly 80 bytes.
 *
 * Layout: type(4)+counter(4)+size(4)+channelId(4)+ccType(4)+attribType(4)+attributes(14*4=56) = 80.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(CCSetAttributePacket, BufferSizeIs80)
{
    std::cout << "[CCSetAttributePacket.BufferSizeIs80] - START" << std::endl;
    CCSetAttributePacket pkt(1u, 1u, 0u, 0u, makeAttribs(0u));
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 80)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 80u);
    std::cout << "[CCSetAttributePacket.BufferSizeIs80] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket getType() returns TYPE_CC_SET_ATTRIBUTE (18).
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(CCSetAttributePacket, TypeIsCCSetAttribute)
{
    std::cout << "[CCSetAttributePacket.TypeIsCCSetAttribute] - START" << std::endl;
    CCSetAttributePacket pkt(1u, 1u, 0u, 0u, makeAttribs(0u));
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_CC_SET_ATTRIBUTE << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_CC_SET_ATTRIBUTE);
    std::cout << "[CCSetAttributePacket.TypeIsCCSetAttribute] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket stores counter correctly.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(CCSetAttributePacket, CounterStoredCorrectly)
{
    std::cout << "[CCSetAttributePacket.CounterStoredCorrectly] - START" << std::endl;
    CCSetAttributePacket pkt(1u, 99u, 0u, 0u, makeAttribs(0u));
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 99)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 99u);
    std::cout << "[CCSetAttributePacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket size field = (14+3)*4 = 68.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 005
 * @par Priority: High
 */
TEST(CCSetAttributePacket, SizeFieldIs68)
{
    std::cout << "[CCSetAttributePacket.SizeFieldIs68] - START" << std::endl;
    CCSetAttributePacket pkt(1u, 1u, 0u, 0u, makeAttribs(0u));
    const auto& buf = pkt.getBytes();
    uint32_t sizeField = readLE32(buf, 8);
    std::cout << "  size field: " << sizeField << " (expected 68)" << std::endl;
    EXPECT_EQ(sizeField, 68u);
    std::cout << "[CCSetAttributePacket.SizeFieldIs68] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket encodes channelId, ccType, attribType at correct offsets.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 006
 * @par Priority: High
 */
TEST(CCSetAttributePacket, FieldsEncodedAtCorrectOffsets)
{
    std::cout << "[CCSetAttributePacket.FieldsEncodedAtCorrectOffsets] - START" << std::endl;
    const uint32_t ch = 7u, ctr = 3u, ccT = 1u, attT = 0xFu;
    CCSetAttributePacket pkt(ch, ctr, ccT, attT, makeAttribs(0u));
    const auto& buf = pkt.getBytes();
    // type(4) + counter(4) + size(4) + channelId(4) + ccType(4) + attribType(4)
    EXPECT_EQ(readLE32(buf, 12), ch);
    EXPECT_EQ(readLE32(buf, 16), ccT);
    EXPECT_EQ(readLE32(buf, 20), attT);
    std::cout << "  channelId=" << ch << " ccType=" << ccT << " attribType=" << attT << " all verified" << std::endl;
    std::cout << "[CCSetAttributePacket.FieldsEncodedAtCorrectOffsets] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket attribute values are encoded starting at byte 24.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 007
 * @par Priority: High
 */
TEST(CCSetAttributePacket, AttributeValuesEncodedCorrectly)
{
    std::cout << "[CCSetAttributePacket.AttributeValuesEncodedCorrectly] - START" << std::endl;
    attributesType attribs;
    for (size_t i = 0; i < attribs.size(); ++i)
        attribs[i] = static_cast<uint32_t>(100u * (i + 1));

    CCSetAttributePacket pkt(1u, 1u, 0u, 0u, attribs);
    const auto& buf = pkt.getBytes();
    for (size_t i = 0; i < attribs.size(); ++i)
    {
        uint32_t decoded = readLE32(buf, 24 + i * 4);
        std::cout << "  attribs[" << i << "]=" << attribs[i] << " decoded=" << decoded << std::endl;
        EXPECT_EQ(decoded, attribs[i]);
    }
    std::cout << "[CCSetAttributePacket.AttributeValuesEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket handles all-zero attribute values.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 008
 * @par Priority: Medium
 */
TEST(CCSetAttributePacket, AllZeroAttributeValues)
{
    std::cout << "[CCSetAttributePacket.AllZeroAttributeValues] - START" << std::endl;
    CCSetAttributePacket pkt(1u, 1u, 0u, 0u, makeAttribs(0u));
    const auto& buf = pkt.getBytes();
    for (size_t i = 0; i < NUMBER_OF_ATTRIBUTES; ++i)
        EXPECT_EQ(readLE32(buf, 24 + i * 4), 0u) << "  attrib[" << i << "] not zero";
    std::cout << "  All 14 attribute fields are zero" << std::endl;
    std::cout << "[CCSetAttributePacket.AllZeroAttributeValues] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket handles UINT32_MAX for all parameters.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 009
 * @par Priority: Medium
 */
TEST(CCSetAttributePacket, MaxValues)
{
    std::cout << "[CCSetAttributePacket.MaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({
        CCSetAttributePacket pkt(MAX, MAX, MAX, MAX, makeAttribs(MAX));
        EXPECT_EQ(pkt.getBytes().size(), 80u);
        EXPECT_EQ(pkt.getCounter(), MAX);
        std::cout << "  UINT32_MAX for all fields: OK" << std::endl;
    });
    std::cout << "[CCSetAttributePacket.MaxValues] - PASS" << std::endl;
}

/**
 * @brief CCSetAttributePacket handles zero channelId.
 *
 * @par Test Group ID: CCSetAttributePacket
 * @par Test Case ID: 010
 * @par Priority: Medium
 */
TEST(CCSetAttributePacket, ZeroChannelId)
{
    std::cout << "[CCSetAttributePacket.ZeroChannelId] - START" << std::endl;
    EXPECT_NO_THROW({
        CCSetAttributePacket pkt(0u, 5u, 1u, 2u, makeAttribs(100u));
        EXPECT_EQ(readLE32(pkt.getBytes(), 12), 0u);
        std::cout << "  channelId=0 encoded correctly" << std::endl;
    });
    std::cout << "[CCSetAttributePacket.ZeroChannelId] - PASS" << std::endl;
}

// ===========================================================================
// Stress tests
// ===========================================================================

/**
 * @brief Create 100 each of all packet types without resource leaks.
 *
 * @par Test Group ID: SubtecPacketStress
 * @par Test Case ID: 001
 * @par Priority: Medium
 */
TEST(SubtecPacketStress, Create100OfEachPacketType)
{
    std::cout << "[SubtecPacketStress.Create100OfEachPacketType] - START" << std::endl;
    const int N = 100;
    for (int i = 0; i < N; ++i)
    {
        DummyPacket         d;
        PausePacket         pause(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        ResumePacket        resume(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        MutePacket          mute(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        UnmutePacket        unmute(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        ResetAllPacket      resetAll;
        ResetChannelPacket  resetCh(static_cast<uint32_t>(i), static_cast<uint32_t>(i));
        CCSetAttributePacket ccAttr(static_cast<uint32_t>(i), static_cast<uint32_t>(i),
                                    0u, 0u, makeAttribs(static_cast<uint32_t>(i)));
    }
    std::cout << "  Successfully created " << N << " instances of each packet type" << std::endl;
    std::cout << "[SubtecPacketStress.Create100OfEachPacketType] - PASS" << std::endl;
}

/**
 * @brief getTypeString handles the full range of uint32_t values without crashing.
 *
 * @par Test Group ID: SubtecPacketStress
 * @par Test Case ID: 002
 * @par Priority: Low
 */
TEST(SubtecPacketStress, GetTypeStringBoundaryValues)
{
    std::cout << "[SubtecPacketStress.GetTypeStringBoundaryValues] - START" << std::endl;
    DummyPacket p;
    const uint32_t testVals[] = { 0u, 1u, 18u, 19u, 100u, 0xFFFFFFFFu };
    for (auto v : testVals)
    {
        EXPECT_NO_THROW({
            std::string s = p.getTypeString(v);
            std::cout << "  getTypeString(" << v << ") = \"" << s << "\"" << std::endl;
        });
    }
    std::cout << "[SubtecPacketStress.GetTypeStringBoundaryValues] - PASS" << std::endl;
}

// ===========================================================================
// PacketProtectedMethods tests
// Access protected helpers via a thin test subclass.
// ===========================================================================

/**
 * @brief Thin subclass that exposes Packet's protected helpers for testing.
 *
 * The helpers getBuffer(), append32(), append64() and appendType() are
 * protected — they can only be exercised through a class that inherits from
 * Packet.  PacketTestHelper makes them public without modifying production
 * code.
 */
class PacketTestHelper : public Packet
{
public:
    using Packet::getBuffer;
    using Packet::append32;
    using Packet::append64;
    using Packet::appendType;
    using PacketType = Packet::PacketType;  // expose protected enum
};

/**
 * @brief getBuffer() on a freshly-constructed helper returns an empty buffer.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(PacketProtectedMethods, GetBufferInitiallyEmpty)
{
    std::cout << "[PacketProtectedMethods.GetBufferInitiallyEmpty] - START" << std::endl;
    PacketTestHelper h;
    const auto& buf = h.getBuffer();
    std::cout << "  getBuffer().size() = " << buf.size() << " (expected 0)" << std::endl;
    EXPECT_TRUE(buf.empty());
    std::cout << "[PacketProtectedMethods.GetBufferInitiallyEmpty] - PASS" << std::endl;
}

/**
 * @brief getBuffer() returns the same underlying storage as getBytes().
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 002
 * @par Priority       : Medium
 */
TEST(PacketProtectedMethods, GetBufferMatchesGetBytes)
{
    std::cout << "[PacketProtectedMethods.GetBufferMatchesGetBytes] - START" << std::endl;
    PacketTestHelper h;
    h.append32(0xDEADBEEFu);
    EXPECT_EQ(h.getBuffer().size(), h.getBytes().size());
    EXPECT_EQ(h.getBuffer(), h.getBytes());
    std::cout << "  getBuffer() and getBytes() refer to same data after append32()" << std::endl;
    std::cout << "[PacketProtectedMethods.GetBufferMatchesGetBytes] - PASS" << std::endl;
}

/**
 * @brief append32() encodes a uint32_t as 4 little-endian bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(PacketProtectedMethods, Append32EncodesLittleEndian)
{
    std::cout << "[PacketProtectedMethods.Append32EncodesLittleEndian] - START" << std::endl;
    PacketTestHelper h;
    const uint32_t val = 0x01020304u;
    h.append32(val);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 4u);
    EXPECT_EQ(buf[0], 0x04u);
    EXPECT_EQ(buf[1], 0x03u);
    EXPECT_EQ(buf[2], 0x02u);
    EXPECT_EQ(buf[3], 0x01u);
    EXPECT_EQ(readLE32(buf, 0), val);
    std::cout << "  append32(0x01020304) encoded as LE bytes [04 03 02 01]" << std::endl;
    std::cout << "[PacketProtectedMethods.Append32EncodesLittleEndian] - PASS" << std::endl;
}

/**
 * @brief append32() called multiple times grows the buffer correctly.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(PacketProtectedMethods, Append32MultipleCallsGrowBuffer)
{
    std::cout << "[PacketProtectedMethods.Append32MultipleCallsGrowBuffer] - START" << std::endl;
    PacketTestHelper h;
    h.append32(0xAAAAAAAAu);
    h.append32(0xBBBBBBBBu);
    h.append32(0xCCCCCCCCu);
    const auto& buf = h.getBytes();
    EXPECT_EQ(buf.size(), 12u);
    EXPECT_EQ(readLE32(buf, 0), 0xAAAAAAAAu);
    EXPECT_EQ(readLE32(buf, 4), 0xBBBBBBBBu);
    EXPECT_EQ(readLE32(buf, 8), 0xCCCCCCCCu);
    std::cout << "  3x append32() produced 12-byte buffer with correct values" << std::endl;
    std::cout << "[PacketProtectedMethods.Append32MultipleCallsGrowBuffer] - PASS" << std::endl;
}

/**
 * @brief append32() with zero value encodes four zero bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 005
 * @par Priority       : Low
 */
TEST(PacketProtectedMethods, Append32ZeroValueEncodes4Zeros)
{
    std::cout << "[PacketProtectedMethods.Append32ZeroValueEncodes4Zeros] - START" << std::endl;
    PacketTestHelper h;
    h.append32(0u);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 4u);
    for (auto b : buf)
        EXPECT_EQ(b, 0x00u);
    std::cout << "  append32(0) encodes [00 00 00 00]" << std::endl;
    std::cout << "[PacketProtectedMethods.Append32ZeroValueEncodes4Zeros] - PASS" << std::endl;
}

/**
 * @brief append64() encodes an int64_t as 8 little-endian bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(PacketProtectedMethods, Append64EncodesLittleEndian)
{
    std::cout << "[PacketProtectedMethods.Append64EncodesLittleEndian] - START" << std::endl;
    PacketTestHelper h;
    const int64_t val = 0x0102030405060708LL;
    h.append64(val);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 8u);
    EXPECT_EQ(readLE64(buf, 0), static_cast<uint64_t>(val));
    std::cout << "  append64(0x0102030405060708) encoded as 8 LE bytes" << std::endl;
    std::cout << "[PacketProtectedMethods.Append64EncodesLittleEndian] - PASS" << std::endl;
}

/**
 * @brief append64() with zero value encodes eight zero bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 007
 * @par Priority       : Low
 */
TEST(PacketProtectedMethods, Append64ZeroValueEncodes8Zeros)
{
    std::cout << "[PacketProtectedMethods.Append64ZeroValueEncodes8Zeros] - START" << std::endl;
    PacketTestHelper h;
    h.append64(0LL);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 8u);
    for (auto b : buf)
        EXPECT_EQ(b, 0x00u);
    std::cout << "  append64(0) encodes 8 zero bytes" << std::endl;
    std::cout << "[PacketProtectedMethods.Append64ZeroValueEncodes8Zeros] - PASS" << std::endl;
}

/**
 * @brief append64() with negative value encodes correct two's-complement LE bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 008
 * @par Priority       : Medium
 */
TEST(PacketProtectedMethods, Append64NegativeValue)
{
    std::cout << "[PacketProtectedMethods.Append64NegativeValue] - START" << std::endl;
    PacketTestHelper h;
    const int64_t val = -1LL;
    h.append64(val);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 8u);
    // -1 in two's complement is all 0xFF bytes
    for (auto b : buf)
        EXPECT_EQ(b, 0xFFu);
    std::cout << "  append64(-1) encodes 8x 0xFF bytes" << std::endl;
    std::cout << "[PacketProtectedMethods.Append64NegativeValue] - PASS" << std::endl;
}

/**
 * @brief appendType() encodes the packet type enum value as 4 little-endian bytes.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 009
 * @par Priority       : High
 */
TEST(PacketProtectedMethods, AppendTypeEncodesPauseType)
{
    std::cout << "[PacketProtectedMethods.AppendTypeEncodesPauseType] - START" << std::endl;
    PacketTestHelper h;
    h.appendType(PacketTestHelper::PacketType::PAUSE);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 4u);
    EXPECT_EQ(readLE32(buf, 0), TYPE_PAUSE);
    std::cout << "  appendType(PAUSE) encodes value=" << TYPE_PAUSE << std::endl;
    std::cout << "[PacketProtectedMethods.AppendTypeEncodesPauseType] - PASS" << std::endl;
}

/**
 * @brief appendType() encodes RESET_ALL type correctly.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(PacketProtectedMethods, AppendTypeEncodesResetAllType)
{
    std::cout << "[PacketProtectedMethods.AppendTypeEncodesResetAllType] - START" << std::endl;
    PacketTestHelper h;
    h.appendType(PacketTestHelper::PacketType::RESET_ALL);
    const auto& buf = h.getBytes();
    ASSERT_EQ(buf.size(), 4u);
    EXPECT_EQ(readLE32(buf, 0), TYPE_RESET_ALL);
    std::cout << "  appendType(RESET_ALL) encodes value=" << TYPE_RESET_ALL << std::endl;
    std::cout << "[PacketProtectedMethods.AppendTypeEncodesResetAllType] - PASS" << std::endl;
}

/**
 * @brief Mixed sequence: appendType + append32 + append64 produces correct layout.
 *
 * @par Test Group ID  : PacketProtectedMethods
 * @par Test Case ID   : 011
 * @par Priority       : High
 */
TEST(PacketProtectedMethods, MixedAppendSequenceCorrectLayout)
{
    std::cout << "[PacketProtectedMethods.MixedAppendSequenceCorrectLayout] - START" << std::endl;
    PacketTestHelper h;
    h.appendType(PacketTestHelper::PacketType::TIMESTAMP);
    h.append32(0xDEADBEEFu);
    h.append64(0x0102030405060708LL);
    const auto& buf = h.getBytes();
    // 4 (type) + 4 (uint32) + 8 (int64) = 16 bytes
    ASSERT_EQ(buf.size(), 16u);
    EXPECT_EQ(readLE32(buf, 0), TYPE_TIMESTAMP);
    EXPECT_EQ(readLE32(buf, 4), 0xDEADBEEFu);
    EXPECT_EQ(readLE64(buf, 8), static_cast<uint64_t>(0x0102030405060708LL));
    std::cout << "  Mixed appendType+append32+append64 produced correct 16-byte layout" << std::endl;
    std::cout << "[PacketProtectedMethods.MixedAppendSequenceCorrectLayout] - PASS" << std::endl;
}
