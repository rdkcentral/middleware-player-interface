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
 * @file ClosedCaptionsPacketTests.cpp
 * @brief Comprehensive unit tests for ClosedCaptionsPacket, ClosedCaptionsActiveTypePacket,
 *        and ClosedCaptionsChannel.
 *
 * Covers:
 *   - ClosedCaptionsPacket (no-PTS constructor): buffer layout, type, size field, channel type,
 *     PTS-presence flag, data encoding
 *   - ClosedCaptionsPacket (PTS constructor): PTS value encoding, presence flag set to 1
 *   - ClosedCaptionsActiveTypePacket: CEA 608/708 types, service field, buffer layout
 *   - ClosedCaptionsChannel: SendDataPacketWithPTS, SendDataPacketNoPTS, SendActiveTypePacket
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <limits>
#include <vector>
#include "ClosedCaptionsPacket.hpp"

// ---------------------------------------------------------------------------
// Helper utilities
// ---------------------------------------------------------------------------

/** @brief Decode a 4-byte little-endian word from buffer at the given offset. */
static uint32_t readLE32(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint32_t>(buf[offset])
         | (static_cast<uint32_t>(buf[offset + 1]) << 8)
         | (static_cast<uint32_t>(buf[offset + 2]) << 16)
         | (static_cast<uint32_t>(buf[offset + 3]) << 24);
}

// PacketType numeric constants
static constexpr uint32_t TYPE_CC_DATA           = 10u;
static constexpr uint32_t TYPE_SUBTITLE_SELECTION =  5u;

// Fixed values from header
static constexpr uint32_t CC_CHANNEL_TYPE        =  3u;  ///< ClosedCaptionsPacket::CC_CHANNEL_TYPE
static constexpr uint32_t CC_DATA_HEADER_LEN     = 16u;  ///< ClosedCaptionsPacket::CC_DATA_HEADER_LEN
static constexpr uint32_t CC_SELECTION_LEN       = 16u;  ///< ClosedCaptionsActiveTypePacket::CC_SELECTION_LEN
static constexpr uint32_t CC_USERDATA_TYPE       =  3u;  ///< CC_USERDATA_SUBTITLE_TYPE

// ===========================================================================
// ClosedCaptionsPacket (no-PTS constructor) tests
// ===========================================================================

/**
 * @brief ClosedCaptionsPacket (no-PTS) construction with valid data does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                           | Expected              |
 * |:----:|---------------------------------------|-----------------------|
 * |  01  | Construct with 3-byte data array      | No exception thrown   |
 */
TEST(ClosedCaptionsPacketNoPTS, ConstructionNoThrow)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.ConstructionNoThrow] - START" << std::endl;
    uint8_t data[] = {0x01, 0x02, 0x03};
    EXPECT_NO_THROW({
        ClosedCaptionsPacket pkt(1u, 0u, data, sizeof(data));
    });
    std::cout << "[ClosedCaptionsPacketNoPTS.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) with 0-byte data yields 28-byte buffer.
 *
 * Buffer layout: type(4) + counter(4) + size(4) + channelId(4) + ccType(4) + pts_presence(4) + pts_val(4) = 28
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, EmptyDataBufferSizeIs28)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.EmptyDataBufferSizeIs28] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, nullptr, 0u);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size = " << sz << " (expected 28)" << std::endl;
    EXPECT_EQ(sz, 28u);
    std::cout << "[ClosedCaptionsPacketNoPTS.EmptyDataBufferSizeIs28] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) with N-byte data yields (28+N)-byte buffer.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, BufferSizeIncludesPayload)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.BufferSizeIncludesPayload] - START" << std::endl;
    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
    const size_t N = sizeof(data);
    ClosedCaptionsPacket pkt(1u, 0u, data, N);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size with " << N << " bytes payload = " << sz << " (expected " << (28+N) << ")" << std::endl;
    EXPECT_EQ(sz, 28u + N);
    std::cout << "[ClosedCaptionsPacketNoPTS.BufferSizeIncludesPayload] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) reports type CC_DATA (10).
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, TypeIsCCData)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.TypeIsCCData] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, nullptr, 0u);
    uint32_t t = pkt.getType();
    std::cout << "  getType() = " << t << " (expected " << TYPE_CC_DATA << ")" << std::endl;
    EXPECT_EQ(t, TYPE_CC_DATA);
    std::cout << "[ClosedCaptionsPacketNoPTS.TypeIsCCData] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) counter is stored at bytes [4..7].
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, CounterEncodedAtBytes4to7)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.CounterEncodedAtBytes4to7] - START" << std::endl;
    const uint32_t counter = 99u;
    ClosedCaptionsPacket pkt(1u, counter, nullptr, 0u);
    uint32_t decoded = readLE32(pkt.getBytes(), 4);
    std::cout << "  counter=" << counter << ", buf[4..7]=" << decoded << std::endl;
    EXPECT_EQ(decoded, counter);
    std::cout << "[ClosedCaptionsPacketNoPTS.CounterEncodedAtBytes4to7] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) size field equals CC_DATA_HEADER_LEN + dataLen.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, SizeFieldIsHeaderLenPlusData)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.SizeFieldIsHeaderLenPlusData] - START" << std::endl;
    uint8_t data[] = {0x01, 0x02, 0x03};
    const size_t N = sizeof(data);
    ClosedCaptionsPacket pkt(1u, 0u, data, N);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    uint32_t expected  = CC_DATA_HEADER_LEN + static_cast<uint32_t>(N);
    std::cout << "  size field=" << sizeField << " (expected " << expected << ")" << std::endl;
    EXPECT_EQ(sizeField, expected);
    std::cout << "[ClosedCaptionsPacketNoPTS.SizeFieldIsHeaderLenPlusData] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) channelId encoded at bytes [12..15].
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, ChannelIdEncodedCorrectly)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.ChannelIdEncodedCorrectly] - START" << std::endl;
    const uint32_t chId = 17u;
    ClosedCaptionsPacket pkt(chId, 0u, nullptr, 0u);
    uint32_t decoded = readLE32(pkt.getBytes(), 12);
    std::cout << "  channelId=" << chId << ", buf[12..15]=" << decoded << std::endl;
    EXPECT_EQ(decoded, chId);
    std::cout << "[ClosedCaptionsPacketNoPTS.ChannelIdEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) CC_CHANNEL_TYPE (3) encoded at bytes [16..19].
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 008
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, ChannelTypeFieldIs3)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.ChannelTypeFieldIs3] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, nullptr, 0u);
    uint32_t ccType = readLE32(pkt.getBytes(), 16);
    std::cout << "  CC channel type field at buf[16..19] = " << ccType << " (expected " << CC_CHANNEL_TYPE << ")" << std::endl;
    EXPECT_EQ(ccType, CC_CHANNEL_TYPE);
    std::cout << "[ClosedCaptionsPacketNoPTS.ChannelTypeFieldIs3] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) PTS presence flag is 0 and PTS value is 0.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 009
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, PTSPresenceFlagIsZero)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.PTSPresenceFlagIsZero] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, nullptr, 0u);
    uint32_t presence = readLE32(pkt.getBytes(), 20);
    uint32_t ptsVal   = readLE32(pkt.getBytes(), 24);
    std::cout << "  pts_presence=" << presence << " (expected 0), pts_val=" << ptsVal << " (expected 0)" << std::endl;
    EXPECT_EQ(presence, 0u);
    EXPECT_EQ(ptsVal,   0u);
    std::cout << "[ClosedCaptionsPacketNoPTS.PTSPresenceFlagIsZero] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) payload bytes appear starting at offset 28.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 010
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketNoPTS, PayloadBytesAtOffset28)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.PayloadBytesAtOffset28] - START" << std::endl;
    uint8_t data[] = {0x11, 0x22, 0x33};
    ClosedCaptionsPacket pkt(1u, 0u, data, sizeof(data));
    const auto& buf = pkt.getBytes();
    ASSERT_EQ(buf.size(), 31u);
    EXPECT_EQ(buf[28], 0x11);
    EXPECT_EQ(buf[29], 0x22);
    EXPECT_EQ(buf[30], 0x33);
    std::cout << "  Payload at [28,29,30] = 0x11, 0x22, 0x33 verified" << std::endl;
    std::cout << "[ClosedCaptionsPacketNoPTS.PayloadBytesAtOffset28] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) with max counter value.
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 011
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsPacketNoPTS, MaxCounterValue)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.MaxCounterValue] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    ClosedCaptionsPacket pkt(MAX, MAX, nullptr, 0u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 4),  MAX);  // counter
    EXPECT_EQ(readLE32(pkt.getBytes(), 12), MAX);  // channelId
    std::cout << "  UINT32_MAX counter and channelId OK" << std::endl;
    std::cout << "[ClosedCaptionsPacketNoPTS.MaxCounterValue] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (no-PTS) getTypeString returns "CC_DATA".
 *
 * @par Test Group ID  : ClosedCaptionsPacketNoPTS
 * @par Test Case ID   : 012
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsPacketNoPTS, TypeStringIsCCData)
{
    std::cout << "[ClosedCaptionsPacketNoPTS.TypeStringIsCCData] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, nullptr, 0u);
    std::string ts = pkt.getTypeString(pkt.getType());
    std::cout << "  getTypeString() = \"" << ts << "\"" << std::endl;
    EXPECT_EQ(ts, "CC_DATA");
    std::cout << "[ClosedCaptionsPacketNoPTS.TypeStringIsCCData] - PASS" << std::endl;
}

// ===========================================================================
// ClosedCaptionsPacket (with-PTS constructor) tests
// ===========================================================================

/**
 * @brief ClosedCaptionsPacket (with-PTS) construction does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, ConstructionNoThrow)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.ConstructionNoThrow] - START" << std::endl;
    uint8_t data[] = {0xAA, 0xBB};
    EXPECT_NO_THROW({
        ClosedCaptionsPacket pkt(1u, 0u, 12345u, data, sizeof(data));
    });
    std::cout << "[ClosedCaptionsPacketWithPTS.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) with 0-byte data yields 28-byte buffer.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, EmptyDataBufferSizeIs28)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.EmptyDataBufferSizeIs28] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, 0u, nullptr, 0u);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size = " << sz << " (expected 28)" << std::endl;
    EXPECT_EQ(sz, 28u);
    std::cout << "[ClosedCaptionsPacketWithPTS.EmptyDataBufferSizeIs28] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) PTS presence flag is 1.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, PTSPresenceFlagIsOne)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.PTSPresenceFlagIsOne] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, 999u, nullptr, 0u);
    uint32_t presence = readLE32(pkt.getBytes(), 20);
    std::cout << "  pts_presence at buf[20..23] = " << presence << " (expected 1)" << std::endl;
    EXPECT_EQ(presence, 1u);
    std::cout << "[ClosedCaptionsPacketWithPTS.PTSPresenceFlagIsOne] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) PTS value is encoded at bytes [24..27].
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, PTSValueEncodedCorrectly)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.PTSValueEncodedCorrectly] - START" << std::endl;
    const uint32_t pts = 900000u;
    ClosedCaptionsPacket pkt(1u, 0u, pts, nullptr, 0u);
    uint32_t decoded = readLE32(pkt.getBytes(), 24);
    std::cout << "  ptsValue=" << pts << ", buf[24..27]=" << decoded << std::endl;
    EXPECT_EQ(decoded, pts);
    std::cout << "[ClosedCaptionsPacketWithPTS.PTSValueEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) type is still CC_DATA (10).
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, TypeIsCCData)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.TypeIsCCData] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, 100u, nullptr, 0u);
    EXPECT_EQ(pkt.getType(), TYPE_CC_DATA);
    std::cout << "  getType()=" << pkt.getType() << " (expected " << TYPE_CC_DATA << ")" << std::endl;
    std::cout << "[ClosedCaptionsPacketWithPTS.TypeIsCCData] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) CC_CHANNEL_TYPE field is 3.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, ChannelTypeFieldIs3)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.ChannelTypeFieldIs3] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, 0u, nullptr, 0u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 16), CC_CHANNEL_TYPE);
    std::cout << "  CC_CHANNEL_TYPE=3 verified at buf[16..19]" << std::endl;
    std::cout << "[ClosedCaptionsPacketWithPTS.ChannelTypeFieldIs3] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) size field equals CC_DATA_HEADER_LEN + dataLen.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, SizeFieldIsHeaderLenPlusData)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.SizeFieldIsHeaderLenPlusData] - START" << std::endl;
    uint8_t data[] = {0xAA, 0xBB, 0xCC};
    const size_t N = sizeof(data);
    ClosedCaptionsPacket pkt(1u, 0u, 5000u, data, N);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    uint32_t expected  = CC_DATA_HEADER_LEN + static_cast<uint32_t>(N);
    std::cout << "  size field=" << sizeField << " (expected " << expected << ")" << std::endl;
    EXPECT_EQ(sizeField, expected);
    std::cout << "[ClosedCaptionsPacketWithPTS.SizeFieldIsHeaderLenPlusData] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) payload bytes start at offset 28.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 008
 * @par Priority       : High
 */
TEST(ClosedCaptionsPacketWithPTS, PayloadBytesAtOffset28)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.PayloadBytesAtOffset28] - START" << std::endl;
    uint8_t data[] = {0x55, 0x66};
    ClosedCaptionsPacket pkt(1u, 0u, 1234u, data, sizeof(data));
    const auto& buf = pkt.getBytes();
    ASSERT_EQ(buf.size(), 30u);
    EXPECT_EQ(buf[28], 0x55);
    EXPECT_EQ(buf[29], 0x66);
    std::cout << "  Payload at [28,29] = 0x55, 0x66 verified" << std::endl;
    std::cout << "[ClosedCaptionsPacketWithPTS.PayloadBytesAtOffset28] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) with zero PTS value.
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsPacketWithPTS, ZeroPTSValue)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.ZeroPTSValue] - START" << std::endl;
    ClosedCaptionsPacket pkt(1u, 0u, 0u, nullptr, 0u);
    // presence=1, value=0
    EXPECT_EQ(readLE32(pkt.getBytes(), 20), 1u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 24), 0u);
    std::cout << "  Zero PTS value: presence=1, value=0 verified" << std::endl;
    std::cout << "[ClosedCaptionsPacketWithPTS.ZeroPTSValue] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsPacket (with-PTS) with max PTS value (UINT32_MAX).
 *
 * @par Test Group ID  : ClosedCaptionsPacketWithPTS
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsPacketWithPTS, MaxPTSValue)
{
    std::cout << "[ClosedCaptionsPacketWithPTS.MaxPTSValue] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    ClosedCaptionsPacket pkt(1u, 0u, MAX, nullptr, 0u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 20), 1u);
    EXPECT_EQ(readLE32(pkt.getBytes(), 24), MAX);
    std::cout << "  UINT32_MAX PTS value encoded correctly" << std::endl;
    std::cout << "[ClosedCaptionsPacketWithPTS.MaxPTSValue] - PASS" << std::endl;
}

// ===========================================================================
// ClosedCaptionsActiveTypePacket tests
// ===========================================================================

/**
 * @brief ClosedCaptionsActiveTypePacket construction does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, ConstructionNoThrow)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    });
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket buffer is exactly 28 bytes.
 *
 * Layout: type(4)+counter(4)+size(4)+channelId(4)+ccUserData(4)+ceaType(4)+service(4) = 28
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, BufferSizeIs28)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.BufferSizeIs28] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size = " << sz << " (expected 28)" << std::endl;
    EXPECT_EQ(sz, 28u);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.BufferSizeIs28] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket type is SUBTITLE_SELECTION (5).
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, TypeIsSubtitleSelection)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.TypeIsSubtitleSelection] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_708, 1);
    uint32_t t = pkt.getType();
    std::cout << "  getType() = " << t << " (expected " << TYPE_SUBTITLE_SELECTION << ")" << std::endl;
    EXPECT_EQ(t, TYPE_SUBTITLE_SELECTION);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.TypeIsSubtitleSelection] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket size field at [8..11] is CC_SELECTION_LEN (16).
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, SizeFieldIsSelectionLen)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.SizeFieldIsSelectionLen] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 2);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    std::cout << "  size field=" << sizeField << " (expected " << CC_SELECTION_LEN << ")" << std::endl;
    EXPECT_EQ(sizeField, CC_SELECTION_LEN);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.SizeFieldIsSelectionLen] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket channelId encoded at [12..15].
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, ChannelIdEncodedCorrectly)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ChannelIdEncodedCorrectly] - START" << std::endl;
    const uint32_t chId = 5u;
    ClosedCaptionsActiveTypePacket pkt(chId, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    uint32_t decoded = readLE32(pkt.getBytes(), 12);
    std::cout << "  channelId=" << chId << ", buf[12..15]=" << decoded << std::endl;
    EXPECT_EQ(decoded, chId);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ChannelIdEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket CC_USERDATA_TYPE (3) encoded at [16..19].
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, UserDataTypeFieldIs3)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.UserDataTypeFieldIs3] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_708, 1);
    uint32_t udType = readLE32(pkt.getBytes(), 16);
    std::cout << "  CC userdata type at buf[16..19] = " << udType << " (expected " << CC_USERDATA_TYPE << ")" << std::endl;
    EXPECT_EQ(udType, CC_USERDATA_TYPE);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.UserDataTypeFieldIs3] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket CEA::type_608 encoded as 0 at [20..23].
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, CEA608EncodedAs0)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CEA608EncodedAs0] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    uint32_t ceaField = readLE32(pkt.getBytes(), 20);
    std::cout << "  CEA type field at buf[20..23] = " << ceaField << " (expected 0 for type_608)" << std::endl;
    EXPECT_EQ(ceaField, 0u);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CEA608EncodedAs0] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket CEA::type_708 encoded as 1 at [20..23].
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 008
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, CEA708EncodedAs1)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CEA708EncodedAs1] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_708, 1);
    uint32_t ceaField = readLE32(pkt.getBytes(), 20);
    std::cout << "  CEA type field at buf[20..23] = " << ceaField << " (expected 1 for type_708)" << std::endl;
    EXPECT_EQ(ceaField, 1u);
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CEA708EncodedAs1] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket service value encoded at [24..27].
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 009
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, ServiceValueEncodedCorrectly)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ServiceValueEncodedCorrectly] - START" << std::endl;
    const int service = 3;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_708, service);
    uint32_t svcField = readLE32(pkt.getBytes(), 24);
    std::cout << "  service=" << service << ", buf[24..27]=" << svcField << std::endl;
    EXPECT_EQ(svcField, static_cast<uint32_t>(service));
    std::cout << "[ClosedCaptionsActiveTypePacketTests.ServiceValueEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket counter stored correctly.
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 010
 * @par Priority       : High
 */
TEST(ClosedCaptionsActiveTypePacketTests, CounterStoredCorrectly)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CounterStoredCorrectly] - START" << std::endl;
    const uint32_t ctr = 88u;
    ClosedCaptionsActiveTypePacket pkt(1u, ctr, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    // ClosedCaptionsActiveTypePacket uses the default Packet() ctor, so getCounter() returns UINT32_MAX.
    // The counter argument is written directly into the serialized buffer at offset 4.
    EXPECT_EQ(pkt.getCounter(), std::numeric_limits<uint32_t>::max());
    EXPECT_EQ(readLE32(pkt.getBytes(), 4), ctr);
    std::cout << "  counter=" << ctr << " verified in buf[4..7]; getCounter()=UINT32_MAX (default ctor)" << std::endl;
    std::cout << "[ClosedCaptionsActiveTypePacketTests.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsActiveTypePacket getTypeString returns "SUBTITLE_SELECTION".
 *
 * @par Test Group ID  : ClosedCaptionsActiveTypePacket
 * @par Test Case ID   : 011
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsActiveTypePacketTests, TypeStringIsSubtitleSelection)
{
    std::cout << "[ClosedCaptionsActiveTypePacketTests.TypeStringIsSubtitleSelection] - START" << std::endl;
    ClosedCaptionsActiveTypePacket pkt(1u, 0u, ClosedCaptionsActiveTypePacket::CEA::type_608, 1);
    std::string ts = pkt.getTypeString(pkt.getType());
    std::cout << "  getTypeString() = \"" << ts << "\"" << std::endl;
    EXPECT_EQ(ts, "SUBTITLE_SELECTION");
    std::cout << "[ClosedCaptionsActiveTypePacketTests.TypeStringIsSubtitleSelection] - PASS" << std::endl;
}

// ===========================================================================
// ClosedCaptionsChannel tests
// ===========================================================================

/**
 * @brief ClosedCaptionsChannel can be constructed without throwing.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(ClosedCaptionsChannel, ConstructionNoThrow)
{
    std::cout << "[ClosedCaptionsChannel.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ ClosedCaptionsChannel ch; });
    std::cout << "[ClosedCaptionsChannel.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketWithPTS does not throw or hang.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(ClosedCaptionsChannel, SendDataPacketWithPTSNoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendDataPacketWithPTSNoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0x01, 0x02, 0x03};
    EXPECT_NO_THROW({ ch.SendDataPacketWithPTS(90000u, data, sizeof(data)); });
    std::cout << "  SendDataPacketWithPTS(pts=90000, 3 bytes) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendDataPacketWithPTSNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketNoPTS does not throw or hang.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(ClosedCaptionsChannel, SendDataPacketNoPTSNoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendDataPacketNoPTSNoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[] = {0xAA, 0xBB};
    EXPECT_NO_THROW({ ch.SendDataPacketNoPTS(data, sizeof(data)); });
    std::cout << "  SendDataPacketNoPTS(2 bytes) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendDataPacketNoPTSNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendActiveTypePacket with CEA 608 type does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(ClosedCaptionsChannel, SendActiveTypePacket608NoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendActiveTypePacket608NoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW({ ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, 1); });
    std::cout << "  SendActiveTypePacket(type_608, ch=1) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendActiveTypePacket608NoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendActiveTypePacket with CEA 708 type does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(ClosedCaptionsChannel, SendActiveTypePacket708NoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendActiveTypePacket708NoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW({ ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, 2); });
    std::cout << "  SendActiveTypePacket(type_708, ch=2) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendActiveTypePacket708NoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketWithPTS with empty data does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 006
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsChannel, SendDataPacketWithPTSEmptyDataNoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendDataPacketWithPTSEmptyDataNoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW({ ch.SendDataPacketWithPTS(0u, nullptr, 0u); });
    std::cout << "  SendDataPacketWithPTS(pts=0, nullptr, 0) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendDataPacketWithPTSEmptyDataNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel::SendDataPacketNoPTS with empty data does not throw.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 007
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsChannel, SendDataPacketNoPTSEmptyDataNoThrow)
{
    std::cout << "[ClosedCaptionsChannel.SendDataPacketNoPTSEmptyDataNoThrow] - START" << std::endl;
    ClosedCaptionsChannel ch;
    EXPECT_NO_THROW({ ch.SendDataPacketNoPTS(nullptr, 0u); });
    std::cout << "  SendDataPacketNoPTS(nullptr, 0) OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.SendDataPacketNoPTSEmptyDataNoThrow] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel: multiple consecutive sends of all 3 methods.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 008
 * @par Priority       : Medium (stress)
 */
TEST(ClosedCaptionsChannel, MultipleConsecutiveSendsNoHang)
{
    std::cout << "[ClosedCaptionsChannel.MultipleConsecutiveSendsNoHang] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[4] = {0x11, 0x22, 0x33, 0x44};
    const int N = 10;
    for (int i = 0; i < N; ++i)
    {
        EXPECT_NO_THROW({ ch.SendDataPacketWithPTS(static_cast<uint32_t>(i * 3000), data, sizeof(data)); });
        EXPECT_NO_THROW({ ch.SendDataPacketNoPTS(data, sizeof(data)); });
        EXPECT_NO_THROW({ ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_608, i % 4 + 1); });
    }
    std::cout << "  " << N << " iterations of all 3 send methods OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.MultipleConsecutiveSendsNoHang] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel destructor is safe after multiple sends.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(ClosedCaptionsChannel, DestructorSafeAfterSends)
{
    std::cout << "[ClosedCaptionsChannel.DestructorSafeAfterSends] - START" << std::endl;
    EXPECT_NO_THROW({
        ClosedCaptionsChannel ch;
        uint8_t d[] = {0xFF};
        ch.SendDataPacketWithPTS(100u, d, 1);
        ch.SendDataPacketNoPTS(d, 1);
        ch.SendActiveTypePacket(ClosedCaptionsActiveTypePacket::CEA::type_708, 3);
    });
    std::cout << "  ClosedCaptionsChannel destructor safe" << std::endl;
    std::cout << "[ClosedCaptionsChannel.DestructorSafeAfterSends] - PASS" << std::endl;
}

/**
 * @brief ClosedCaptionsChannel: stress 50 SendDataPacketWithPTS calls.
 *
 * @par Test Group ID  : ClosedCaptionsChannel
 * @par Test Case ID   : 010
 * @par Priority       : Low (stress)
 */
TEST(ClosedCaptionsChannel, Stress50SendDataWithPTS)
{
    std::cout << "[ClosedCaptionsChannel.Stress50SendDataWithPTS] - START" << std::endl;
    ClosedCaptionsChannel ch;
    uint8_t data[8] = {0};
    for (int i = 0; i < 50; ++i)
    {
        data[0] = static_cast<uint8_t>(i);
        EXPECT_NO_THROW({ ch.SendDataPacketWithPTS(static_cast<uint32_t>(i * 90000), data, sizeof(data)); });
    }
    std::cout << "  50 SendDataPacketWithPTS calls OK" << std::endl;
    std::cout << "[ClosedCaptionsChannel.Stress50SendDataWithPTS] - PASS" << std::endl;
}
