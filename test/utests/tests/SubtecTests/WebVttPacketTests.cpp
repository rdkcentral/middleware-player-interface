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
 * @file WebVttPacketTests.cpp
 * @brief Comprehensive unit tests for WebVtt packet types and channel.
 *
 * Covers: WebVttSelectionPacket, WebVttDataPacket, WebVttTimestampPacket,
 *         WebVttChannel — constructors, buffer layout, type/size verification,
 *         all Send* methods, edge cases, and stress tests.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <limits>
#include <vector>
#include <cstdint>
#include "WebVttPacket.hpp"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static uint32_t readLE32(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint32_t>(buf[offset])
         | (static_cast<uint32_t>(buf[offset + 1]) << 8)
         | (static_cast<uint32_t>(buf[offset + 2]) << 16)
         | (static_cast<uint32_t>(buf[offset + 3]) << 24);
}

static uint64_t readLE64(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint64_t>(readLE32(buf, offset))
         | (static_cast<uint64_t>(readLE32(buf, offset + 4)) << 32);
}

// PacketType numeric constants relevant to WebVtt
static constexpr uint32_t TYPE_WEBVTT_SELECTION = 15u;
static constexpr uint32_t TYPE_WEBVTT_DATA      = 16u;
static constexpr uint32_t TYPE_WEBVTT_TIMESTAMP = 17u;

// ===========================================================================
// WebVttSelectionPacket tests
// ===========================================================================

/**
 * @brief WebVttSelectionPacket construction does not throw.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 001
 * @par Priority: High
 *
 * | Step | Description | Expected |
 * |:----:|-------------|---------|
 * | 01 | Construct WebVttSelectionPacket with typical arguments | No exception |
 */
TEST(WebVttSelectionPacket, ConstructionNoThrow)
{
    std::cout << "[WebVttSelectionPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket pkt(1u, 1u, 1920u, 1080u);
        std::cout << "  Constructed OK (ch=1, ctr=1, w=1920, h=1080)" << std::endl;
    });
    std::cout << "[WebVttSelectionPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket buffer is exactly 24 bytes.
 *
 * Layout: type(4)+counter(4)+size(4)+channelId(4)+width(4)+height(4) = 24.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(WebVttSelectionPacket, BufferSizeIs24)
{
    std::cout << "[WebVttSelectionPacket.BufferSizeIs24] - START" << std::endl;
    WebVttSelectionPacket pkt(1u, 1u, 1920u, 1080u);
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 24)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 24u);
    std::cout << "[WebVttSelectionPacket.BufferSizeIs24] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket getType() returns WEBVTT_SELECTION (15).
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(WebVttSelectionPacket, TypeIsWebVttSelection)
{
    std::cout << "[WebVttSelectionPacket.TypeIsWebVttSelection] - START" << std::endl;
    WebVttSelectionPacket pkt(1u, 1u, 1920u, 1080u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_WEBVTT_SELECTION << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_WEBVTT_SELECTION);
    std::cout << "[WebVttSelectionPacket.TypeIsWebVttSelection] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket stores counter correctly.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(WebVttSelectionPacket, CounterStoredCorrectly)
{
    std::cout << "[WebVttSelectionPacket.CounterStoredCorrectly] - START" << std::endl;
    WebVttSelectionPacket pkt(1u, 55u, 640u, 480u);
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 55)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 55u);
    std::cout << "[WebVttSelectionPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket size field in buffer is 12.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 005
 * @par Priority: High
 */
TEST(WebVttSelectionPacket, SizeFieldIs12)
{
    std::cout << "[WebVttSelectionPacket.SizeFieldIs12] - START" << std::endl;
    WebVttSelectionPacket pkt(1u, 1u, 1920u, 1080u);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    std::cout << "  size field at buf[8-11] = " << sizeField << " (expected 12)" << std::endl;
    EXPECT_EQ(sizeField, 12u);
    std::cout << "[WebVttSelectionPacket.SizeFieldIs12] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket encodes channelId, width, height at correct offsets.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 006
 * @par Priority: High
 */
TEST(WebVttSelectionPacket, DimensionsAndChannelIdEncoded)
{
    std::cout << "[WebVttSelectionPacket.DimensionsAndChannelIdEncoded] - START" << std::endl;
    const uint32_t ch = 3u, ctr = 7u, w = 800u, h = 600u;
    WebVttSelectionPacket pkt(ch, ctr, w, h);
    const auto& buf = pkt.getBytes();
    EXPECT_EQ(readLE32(buf, 12), ch);
    EXPECT_EQ(readLE32(buf, 16), w);
    EXPECT_EQ(readLE32(buf, 20), h);
    std::cout << "  ch=" << ch << " w=" << w << " h=" << h << " all verified in buffer" << std::endl;
    std::cout << "[WebVttSelectionPacket.DimensionsAndChannelIdEncoded] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket handles zero dimensions.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 007
 * @par Priority: Medium
 */
TEST(WebVttSelectionPacket, ZeroDimensions)
{
    std::cout << "[WebVttSelectionPacket.ZeroDimensions] - START" << std::endl;
    EXPECT_NO_THROW({
        WebVttSelectionPacket pkt(0u, 0u, 0u, 0u);
        EXPECT_EQ(pkt.getBytes().size(), 24u);
        std::cout << "  Zero dimensions: OK" << std::endl;
    });
    std::cout << "[WebVttSelectionPacket.ZeroDimensions] - PASS" << std::endl;
}

/**
 * @brief WebVttSelectionPacket handles max values.
 *
 * @par Test Group ID: WebVttSelectionPacket
 * @par Test Case ID: 008
 * @par Priority: Medium
 */
TEST(WebVttSelectionPacket, MaxValues)
{
    std::cout << "[WebVttSelectionPacket.MaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({
        WebVttSelectionPacket pkt(MAX, MAX, MAX, MAX);
        EXPECT_EQ(pkt.getBytes().size(), 24u);
        std::cout << "  Max values: OK" << std::endl;
    });
    std::cout << "[WebVttSelectionPacket.MaxValues] - PASS" << std::endl;
}

// ===========================================================================
// WebVttDataPacket tests
// ===========================================================================

/**
 * @brief WebVttDataPacket construction with empty data does not throw.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(WebVttDataPacket, ConstructionEmptyDataNoThrow)
{
    std::cout << "[WebVttDataPacket.ConstructionEmptyDataNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        std::vector<uint8_t> emptyData;
        WebVttDataPacket pkt(1u, 1u, 0LL, std::move(emptyData));
        std::cout << "  Empty data construction OK" << std::endl;
    });
    std::cout << "[WebVttDataPacket.ConstructionEmptyDataNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket with empty data has buffer size 24.
 *
 * Layout: type(4)+counter(4)+size(4)+channelId(4)+timeOffset(8) = 24 + 0 data bytes.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(WebVttDataPacket, EmptyDataBufferSizeIs24)
{
    std::cout << "[WebVttDataPacket.EmptyDataBufferSizeIs24] - START" << std::endl;
    std::vector<uint8_t> emptyData;
    WebVttDataPacket pkt(1u, 1u, 0LL, std::move(emptyData));
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 24)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 24u);
    std::cout << "[WebVttDataPacket.EmptyDataBufferSizeIs24] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket with 3-byte data has buffer size 27.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(WebVttDataPacket, SmallDataBufferSize)
{
    std::cout << "[WebVttDataPacket.SmallDataBufferSize] - START" << std::endl;
    std::vector<uint8_t> data = {0x01u, 0x02u, 0x03u};
    WebVttDataPacket pkt(1u, 1u, 0LL, std::move(data));
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 27)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 27u);
    std::cout << "[WebVttDataPacket.SmallDataBufferSize] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket getType() returns TYPE_WEBVTT_DATA (16).
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(WebVttDataPacket, TypeIsWebVttData)
{
    std::cout << "[WebVttDataPacket.TypeIsWebVttData] - START" << std::endl;
    std::vector<uint8_t> data = {0xAAu};
    WebVttDataPacket pkt(1u, 2u, 0LL, std::move(data));
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_WEBVTT_DATA << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_WEBVTT_DATA);
    std::cout << "[WebVttDataPacket.TypeIsWebVttData] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket stores counter correctly.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 005
 * @par Priority: High
 */
TEST(WebVttDataPacket, CounterStoredCorrectly)
{
    std::cout << "[WebVttDataPacket.CounterStoredCorrectly] - START" << std::endl;
    std::vector<uint8_t> data = {0x01u};
    WebVttDataPacket pkt(1u, 88u, 0LL, std::move(data));
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 88)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 88u);
    std::cout << "[WebVttDataPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket size field reflects data length (12 + dataLen).
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 006
 * @par Priority: High
 */
TEST(WebVttDataPacket, SizeFieldEncodesDataLength)
{
    std::cout << "[WebVttDataPacket.SizeFieldEncodesDataLength] - START" << std::endl;
    std::vector<uint8_t> data = {0x01u, 0x02u, 0x03u, 0x04u, 0x05u};  // 5 bytes
    const size_t dataLen = data.size();
    WebVttDataPacket pkt(1u, 1u, 0LL, std::move(data));
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    std::cout << "  dataLen=" << dataLen << " sizeField=" << sizeField
              << " (expected " << (12u + dataLen) << ")" << std::endl;
    EXPECT_EQ(sizeField, static_cast<uint32_t>(12u + dataLen));
    std::cout << "[WebVttDataPacket.SizeFieldEncodesDataLength] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket handles positive time offset.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 007
 * @par Priority: High
 */
TEST(WebVttDataPacket, PositiveTimeOffset)
{
    std::cout << "[WebVttDataPacket.PositiveTimeOffset] - START" << std::endl;
    std::vector<uint8_t> data = {0xDEu, 0xADu};
    EXPECT_NO_THROW({
        WebVttDataPacket pkt(1u, 1u, 100LL, std::move(data));
        EXPECT_EQ(pkt.getBytes().size(), 26u);
        std::cout << "  timeOffset=100ms, buffer size=26 OK" << std::endl;
    });
    std::cout << "[WebVttDataPacket.PositiveTimeOffset] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket handles negative time offset.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 008
 * @par Priority: Medium
 */
TEST(WebVttDataPacket, NegativeTimeOffset)
{
    std::cout << "[WebVttDataPacket.NegativeTimeOffset] - START" << std::endl;
    std::vector<uint8_t> data = {0x01u};
    EXPECT_NO_THROW({
        WebVttDataPacket pkt(1u, 1u, -500LL, std::move(data));
        std::cout << "  timeOffset=-500ms, no throw OK" << std::endl;
    });
    std::cout << "[WebVttDataPacket.NegativeTimeOffset] - PASS" << std::endl;
}

/**
 * @brief WebVttDataPacket with 100-byte payload.
 *
 * @par Test Group ID: WebVttDataPacket
 * @par Test Case ID: 009
 * @par Priority: Medium
 */
TEST(WebVttDataPacket, LargeDataPayload)
{
    std::cout << "[WebVttDataPacket.LargeDataPayload] - START" << std::endl;
    std::vector<uint8_t> data(100u, 0xFFu);
    WebVttDataPacket pkt(1u, 1u, 0LL, std::move(data));
    std::cout << "  100-byte payload, buffer size = " << pkt.getBytes().size()
              << " (expected 124)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 124u);
    std::cout << "[WebVttDataPacket.LargeDataPayload] - PASS" << std::endl;
}

// ===========================================================================
// WebVttTimestampPacket tests
// ===========================================================================

/**
 * @brief WebVttTimestampPacket construction does not throw.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(WebVttTimestampPacket, ConstructionNoThrow)
{
    std::cout << "[WebVttTimestampPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        WebVttTimestampPacket pkt(1u, 5u, 1000u);
        std::cout << "  Constructed OK (ch=1, ctr=5, ts=1000)" << std::endl;
    });
    std::cout << "[WebVttTimestampPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket buffer is exactly 24 bytes.
 *
 * Layout: type(4)+counter(4)+size(4)+channelId(4)+timestamp(8) = 24.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(WebVttTimestampPacket, BufferSizeIs24)
{
    std::cout << "[WebVttTimestampPacket.BufferSizeIs24] - START" << std::endl;
    WebVttTimestampPacket pkt(1u, 1u, 500u);
    std::cout << "  getBytes().size() = " << pkt.getBytes().size() << " (expected 24)" << std::endl;
    EXPECT_EQ(pkt.getBytes().size(), 24u);
    std::cout << "[WebVttTimestampPacket.BufferSizeIs24] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket getType() returns WEBVTT_TIMESTAMP (17).
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(WebVttTimestampPacket, TypeIsWebVttTimestamp)
{
    std::cout << "[WebVttTimestampPacket.TypeIsWebVttTimestamp] - START" << std::endl;
    WebVttTimestampPacket pkt(1u, 1u, 0u);
    std::cout << "  getType() = " << pkt.getType() << " (expected " << TYPE_WEBVTT_TIMESTAMP << ")" << std::endl;
    EXPECT_EQ(pkt.getType(), TYPE_WEBVTT_TIMESTAMP);
    std::cout << "[WebVttTimestampPacket.TypeIsWebVttTimestamp] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket stores counter correctly.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(WebVttTimestampPacket, CounterStoredCorrectly)
{
    std::cout << "[WebVttTimestampPacket.CounterStoredCorrectly] - START" << std::endl;
    WebVttTimestampPacket pkt(1u, 42u, 1000u);
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected 42)" << std::endl;
    EXPECT_EQ(pkt.getCounter(), 42u);
    std::cout << "[WebVttTimestampPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket timestamp is encoded as 8-byte LE at bytes 16-23.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 005
 * @par Priority: High
 */
TEST(WebVttTimestampPacket, TimestampEncodedAtCorrectOffset)
{
    std::cout << "[WebVttTimestampPacket.TimestampEncodedAtCorrectOffset] - START" << std::endl;
    const uint64_t ts = 0x123456789ABCu;
    WebVttTimestampPacket pkt(1u, 1u, ts);
    const auto& buf = pkt.getBytes();
    uint64_t decoded = readLE64(buf, 16);
    std::cout << "  ts=0x" << std::hex << ts << " decoded=0x" << decoded << std::dec << std::endl;
    EXPECT_EQ(decoded, ts);
    std::cout << "[WebVttTimestampPacket.TimestampEncodedAtCorrectOffset] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket handles zero timestamp.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 006
 * @par Priority: Medium
 */
TEST(WebVttTimestampPacket, ZeroTimestamp)
{
    std::cout << "[WebVttTimestampPacket.ZeroTimestamp] - START" << std::endl;
    WebVttTimestampPacket pkt(0u, 0u, 0u);
    EXPECT_EQ(pkt.getBytes().size(), 24u);
    EXPECT_EQ(pkt.getCounter(), 0u);
    EXPECT_EQ(readLE64(pkt.getBytes(), 16), 0u);
    std::cout << "  Zero timestamp verified" << std::endl;
    std::cout << "[WebVttTimestampPacket.ZeroTimestamp] - PASS" << std::endl;
}

/**
 * @brief WebVttTimestampPacket handles UINT64_MAX timestamp without truncation.
 *
 * @par Test Group ID: WebVttTimestampPacket
 * @par Test Case ID: 007
 * @par Priority: Medium
 */
TEST(WebVttTimestampPacket, MaxTimestampNoTruncation)
{
    std::cout << "[WebVttTimestampPacket.MaxTimestampNoTruncation] - START" << std::endl;
    const uint64_t MAX_TS = std::numeric_limits<uint64_t>::max();
    WebVttTimestampPacket pkt(1u, 1u, MAX_TS);
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    std::cout << "  UINT64_MAX decoded from buffer = " << std::hex << decoded << std::dec << std::endl;
    EXPECT_EQ(decoded, MAX_TS);
    std::cout << "[WebVttTimestampPacket.MaxTimestampNoTruncation] - PASS" << std::endl;
}

// ===========================================================================
// WebVttChannel tests
// ===========================================================================

/**
 * @brief WebVttChannel default construction does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 001
 * @par Priority: High
 */
TEST(WebVttChannel, DefaultConstructionNoThrow)
{
    std::cout << "[WebVttChannel.DefaultConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({
        WebVttChannel ch;
        std::cout << "  WebVttChannel constructed OK" << std::endl;
    });
    std::cout << "[WebVttChannel.DefaultConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendSelectionPacket does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 002
 * @par Priority: High
 */
TEST(WebVttChannel, SendSelectionPacketNoThrow)
{
    std::cout << "[WebVttChannel.SendSelectionPacketNoThrow] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({
        ch.SendSelectionPacket(1920u, 1080u);
        std::cout << "  SendSelectionPacket(1920, 1080) OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendSelectionPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendDataPacket with default offset does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 003
 * @par Priority: High
 */
TEST(WebVttChannel, SendDataPacketDefaultOffsetNoThrow)
{
    std::cout << "[WebVttChannel.SendDataPacketDefaultOffsetNoThrow] - START" << std::endl;
    WebVttChannel ch;
    std::vector<uint8_t> data = {0x01u, 0x02u, 0x03u};
    EXPECT_NO_THROW({
        ch.SendDataPacket(std::move(data));
        std::cout << "  SendDataPacket default offset OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendDataPacketDefaultOffsetNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendDataPacket with positive offset does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 004
 * @par Priority: High
 */
TEST(WebVttChannel, SendDataPacketPositiveOffsetNoThrow)
{
    std::cout << "[WebVttChannel.SendDataPacketPositiveOffsetNoThrow] - START" << std::endl;
    WebVttChannel ch;
    std::vector<uint8_t> data = {0xDEu, 0xADu, 0xBEu, 0xEFu};
    EXPECT_NO_THROW({
        ch.SendDataPacket(std::move(data), 1000LL);
        std::cout << "  SendDataPacket offset=1000ms OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendDataPacketPositiveOffsetNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendDataPacket with negative offset does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 005
 * @par Priority: Medium
 */
TEST(WebVttChannel, SendDataPacketNegativeOffsetNoThrow)
{
    std::cout << "[WebVttChannel.SendDataPacketNegativeOffsetNoThrow] - START" << std::endl;
    WebVttChannel ch;
    std::vector<uint8_t> data = {0x01u};
    EXPECT_NO_THROW({
        ch.SendDataPacket(std::move(data), -200LL);
        std::cout << "  SendDataPacket offset=-200ms OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendDataPacketNegativeOffsetNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendDataPacket with empty data does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 006
 * @par Priority: Medium
 */
TEST(WebVttChannel, SendDataPacketEmptyDataNoThrow)
{
    std::cout << "[WebVttChannel.SendDataPacketEmptyDataNoThrow] - START" << std::endl;
    WebVttChannel ch;
    std::vector<uint8_t> emptyData;
    EXPECT_NO_THROW({
        ch.SendDataPacket(std::move(emptyData));
        std::cout << "  SendDataPacket empty data OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendDataPacketEmptyDataNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendTimestampPacket does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 007
 * @par Priority: High
 */
TEST(WebVttChannel, SendTimestampPacketNoThrow)
{
    std::cout << "[WebVttChannel.SendTimestampPacketNoThrow] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({
        ch.SendTimestampPacket(90000u);
        std::cout << "  SendTimestampPacket(90000) OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendTimestampPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendTimestampPacket with zero timestamp does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 008
 * @par Priority: Medium
 */
TEST(WebVttChannel, SendTimestampPacketZeroNoThrow)
{
    std::cout << "[WebVttChannel.SendTimestampPacketZeroNoThrow] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({
        ch.SendTimestampPacket(0u);
        std::cout << "  SendTimestampPacket(0) OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendTimestampPacketZeroNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendTimestampPacket with max uint64_t does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 009
 * @par Priority: Medium
 */
TEST(WebVttChannel, SendTimestampPacketMaxNoThrow)
{
    std::cout << "[WebVttChannel.SendTimestampPacketMaxNoThrow] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({
        ch.SendTimestampPacket(std::numeric_limits<uint64_t>::max());
        std::cout << "  SendTimestampPacket(UINT64_MAX) OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendTimestampPacketMaxNoThrow] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel can call all three Send methods in sequence.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 010
 * @par Priority: High
 */
TEST(WebVttChannel, AllSendMethodsInSequence)
{
    std::cout << "[WebVttChannel.AllSendMethodsInSequence] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({ ch.SendSelectionPacket(1280u, 720u); });
    EXPECT_NO_THROW({ ch.SendTimestampPacket(1000u); });
    std::vector<uint8_t> data = {0x01u, 0x02u};
    EXPECT_NO_THROW({ ch.SendDataPacket(std::move(data), 500LL); });
    EXPECT_NO_THROW({ ch.SendTimestampPacket(2000u); });
    std::cout << "  All three sends in sequence OK" << std::endl;
    std::cout << "[WebVttChannel.AllSendMethodsInSequence] - PASS" << std::endl;
}

/**
 * @brief WebVttChannel SendSelectionPacket with zero dimensions does not throw.
 *
 * @par Test Group ID: WebVttChannel
 * @par Test Case ID: 011
 * @par Priority: Medium
 */
TEST(WebVttChannel, SendSelectionPacketZeroDimensionsNoThrow)
{
    std::cout << "[WebVttChannel.SendSelectionPacketZeroDimensionsNoThrow] - START" << std::endl;
    WebVttChannel ch;
    EXPECT_NO_THROW({
        ch.SendSelectionPacket(0u, 0u);
        std::cout << "  SendSelectionPacket(0, 0) OK" << std::endl;
    });
    std::cout << "[WebVttChannel.SendSelectionPacketZeroDimensionsNoThrow] - PASS" << std::endl;
}

// ===========================================================================
// Stress tests
// ===========================================================================

/**
 * @brief Create 50 WebVttSelectionPacket instances in a loop.
 *
 * @par Test Group ID: WebVttPacketStress
 * @par Test Case ID: 001
 * @par Priority: Medium
 */
TEST(WebVttPacketStress, Create50SelectionPackets)
{
    std::cout << "[WebVttPacketStress.Create50SelectionPackets] - START" << std::endl;
    for (int i = 0; i < 50; ++i)
    {
        WebVttSelectionPacket pkt(static_cast<uint32_t>(i), static_cast<uint32_t>(i),
                                  1920u, 1080u);
        EXPECT_EQ(pkt.getBytes().size(), 24u);
    }
    std::cout << "  50 WebVttSelectionPackets created OK" << std::endl;
    std::cout << "[WebVttPacketStress.Create50SelectionPackets] - PASS" << std::endl;
}

/**
 * @brief Send 50 timestamps via WebVttChannel in a stress loop.
 *
 * @par Test Group ID: WebVttPacketStress
 * @par Test Case ID: 002
 * @par Priority: Medium
 */
TEST(WebVttPacketStress, Send50TimestampsViaChannel)
{
    std::cout << "[WebVttPacketStress.Send50TimestampsViaChannel] - START" << std::endl;
    WebVttChannel ch;
    for (int i = 0; i < 50; ++i)
    {
        EXPECT_NO_THROW(ch.SendTimestampPacket(static_cast<uint64_t>(i * 90u)));
    }
    std::cout << "  50 timestamps sent via WebVttChannel OK" << std::endl;
    std::cout << "[WebVttPacketStress.Send50TimestampsViaChannel] - PASS" << std::endl;
}
