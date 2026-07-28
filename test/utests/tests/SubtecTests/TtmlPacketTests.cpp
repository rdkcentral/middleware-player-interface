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
 * @file TtmlPacketTests.cpp
 * @brief Comprehensive unit tests for TTML packet types and TtmlChannel.
 *
 * Covers:
 *   - TtmlSelectionPacket  : constructor, buffer layout (24 bytes), type, counter, channelId, width, height
 *   - TtmlDataPacket       : constructor, variable-length buffer, type, channelId, dataOffset, payload
 *   - TtmlTimestampPacket  : constructor, buffer layout (24 bytes), type, channelId, timestamp
 *   - TtmlChannel          : SendSelectionPacket, SendDataPacket, SendTimestampPacket, multiple sends
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <limits>
#include <vector>
#include <string>
#include "TtmlPacket.hpp"

// ---------------------------------------------------------------------------
// Helper utilities
// ---------------------------------------------------------------------------

/** @brief Decode a 4-byte little-endian word from a byte buffer at the given offset. */
static uint32_t readLE32(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint32_t>(buf[offset])
         | (static_cast<uint32_t>(buf[offset + 1]) << 8)
         | (static_cast<uint32_t>(buf[offset + 2]) << 16)
         | (static_cast<uint32_t>(buf[offset + 3]) << 24);
}

/** @brief Decode an 8-byte little-endian doubleword from a byte buffer at the given offset. */
static uint64_t readLE64(const std::vector<uint8_t>& buf, size_t offset)
{
    return static_cast<uint64_t>(readLE32(buf, offset))
         | (static_cast<uint64_t>(readLE32(buf, offset + 4)) << 32);
}

// PacketType numeric constants mirroring the PacketType enum in SubtecPacket.hpp
static constexpr uint32_t TYPE_TTML_SELECTION  = 7u;
static constexpr uint32_t TYPE_TTML_DATA       = 8u;
static constexpr uint32_t TYPE_TTML_TIMESTAMP  = 9u;

// ===========================================================================
// TtmlSelectionPacket tests
// ===========================================================================

/**
 * @brief TtmlSelectionPacket constructor does not throw.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 001
 * @par Priority       : High
 *
 * | Step | Description                          | Expected                   |
 * |:----:|--------------------------------------|---------------------------|
 * |  01  | Construct with typical values        | No exception thrown        |
 */
TEST(TtmlSelectionPacket, ConstructionNoThrow)
{
    std::cout << "[TtmlSelectionPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ TtmlSelectionPacket pkt(1u, 0u, 1920u, 1080u); });
    std::cout << "[TtmlSelectionPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket buffer is exactly 24 bytes.
 *
 * Layout: type(4) + counter(4) + size(4) + channelId(4) + width(4) + height(4) = 24
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(TtmlSelectionPacket, BufferSizeIs24)
{
    std::cout << "[TtmlSelectionPacket.BufferSizeIs24] - START" << std::endl;
    TtmlSelectionPacket pkt(1u, 0u, 1920u, 1080u);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size = " << sz << " (expected 24)" << std::endl;
    EXPECT_EQ(sz, 24u);
    std::cout << "[TtmlSelectionPacket.BufferSizeIs24] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket reports the correct packet type.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(TtmlSelectionPacket, TypeIsTtmlSelection)
{
    std::cout << "[TtmlSelectionPacket.TypeIsTtmlSelection] - START" << std::endl;
    TtmlSelectionPacket pkt(1u, 0u, 640u, 480u);
    uint32_t t = pkt.getType();
    std::cout << "  getType() = " << t << " (expected " << TYPE_TTML_SELECTION << ")" << std::endl;
    EXPECT_EQ(t, TYPE_TTML_SELECTION);
    std::cout << "[TtmlSelectionPacket.TypeIsTtmlSelection] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket getTypeString returns "TTML_SELECTION".
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(TtmlSelectionPacket, TypeStringIsTtmlSelection)
{
    std::cout << "[TtmlSelectionPacket.TypeStringIsTtmlSelection] - START" << std::endl;
    TtmlSelectionPacket pkt(1u, 0u, 640u, 480u);
    std::string ts = pkt.getTypeString(pkt.getType());
    std::cout << "  getTypeString() = \"" << ts << "\"" << std::endl;
    EXPECT_EQ(ts, "TTML_SELECTION");
    std::cout << "[TtmlSelectionPacket.TypeStringIsTtmlSelection] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket stores the counter value correctly.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(TtmlSelectionPacket, CounterStoredCorrectly)
{
    std::cout << "[TtmlSelectionPacket.CounterStoredCorrectly] - START" << std::endl;
    const uint32_t counter = 77u;
    TtmlSelectionPacket pkt(1u, counter, 800u, 600u);
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected " << counter << ")" << std::endl;
    EXPECT_EQ(pkt.getCounter(), counter);
    // Also verify counter field at bytes [4..7]
    uint32_t inBuf = readLE32(pkt.getBytes(), 4);
    std::cout << "  buf[4..7] = " << inBuf << " (expected " << counter << ")" << std::endl;
    EXPECT_EQ(inBuf, counter);
    std::cout << "[TtmlSelectionPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket encodes channelId at bytes [12..15].
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(TtmlSelectionPacket, ChannelIdEncodedCorrectly)
{
    std::cout << "[TtmlSelectionPacket.ChannelIdEncodedCorrectly] - START" << std::endl;
    const uint32_t channelId = 42u;
    TtmlSelectionPacket pkt(channelId, 0u, 800u, 600u);
    uint32_t decoded = readLE32(pkt.getBytes(), 12);
    std::cout << "  channelId=" << channelId << ", buf[12..15]=" << decoded << std::endl;
    EXPECT_EQ(decoded, channelId);
    std::cout << "[TtmlSelectionPacket.ChannelIdEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket encodes width at bytes [16..19] and height at [20..23].
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(TtmlSelectionPacket, WidthAndHeightEncoded)
{
    std::cout << "[TtmlSelectionPacket.WidthAndHeightEncoded] - START" << std::endl;
    const uint32_t W = 1920u, H = 1080u;
    TtmlSelectionPacket pkt(1u, 0u, W, H);
    uint32_t w = readLE32(pkt.getBytes(), 16);
    uint32_t h = readLE32(pkt.getBytes(), 20);
    std::cout << "  width=" << w << " (expected " << W << "), height=" << h << " (expected " << H << ")" << std::endl;
    EXPECT_EQ(w, W);
    EXPECT_EQ(h, H);
    std::cout << "[TtmlSelectionPacket.WidthAndHeightEncoded] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket size field at bytes [8..11] is 12.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 008
 * @par Priority       : Medium
 */
TEST(TtmlSelectionPacket, SizeFieldIs12)
{
    std::cout << "[TtmlSelectionPacket.SizeFieldIs12] - START" << std::endl;
    TtmlSelectionPacket pkt(1u, 0u, 800u, 600u);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    std::cout << "  size field = " << sizeField << " (expected 12)" << std::endl;
    EXPECT_EQ(sizeField, 12u);
    std::cout << "[TtmlSelectionPacket.SizeFieldIs12] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket accepts zero values for all parameters.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(TtmlSelectionPacket, ZeroValues)
{
    std::cout << "[TtmlSelectionPacket.ZeroValues] - START" << std::endl;
    EXPECT_NO_THROW({
        TtmlSelectionPacket pkt(0u, 0u, 0u, 0u);
        EXPECT_EQ(pkt.getBytes().size(), 24u);
        EXPECT_EQ(readLE32(pkt.getBytes(), 12), 0u);
        EXPECT_EQ(readLE32(pkt.getBytes(), 16), 0u);
        EXPECT_EQ(readLE32(pkt.getBytes(), 20), 0u);
    });
    std::cout << "[TtmlSelectionPacket.ZeroValues] - PASS" << std::endl;
}

/**
 * @brief TtmlSelectionPacket accepts UINT32_MAX for all parameters.
 *
 * @par Test Group ID  : TtmlSelectionPacket
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(TtmlSelectionPacket, MaxValues)
{
    std::cout << "[TtmlSelectionPacket.MaxValues] - START" << std::endl;
    const uint32_t MAX = std::numeric_limits<uint32_t>::max();
    EXPECT_NO_THROW({
        TtmlSelectionPacket pkt(MAX, MAX, MAX, MAX);
        EXPECT_EQ(pkt.getBytes().size(), 24u);
        EXPECT_EQ(readLE32(pkt.getBytes(), 12), MAX);
        EXPECT_EQ(readLE32(pkt.getBytes(), 16), MAX);
        EXPECT_EQ(readLE32(pkt.getBytes(), 20), MAX);
    });
    std::cout << "[TtmlSelectionPacket.MaxValues] - PASS" << std::endl;
}

// ===========================================================================
// TtmlDataPacket tests
// ===========================================================================

/**
 * @brief TtmlDataPacket construction with non-empty payload does not throw.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(TtmlDataPacket, ConstructionNoThrow)
{
    std::cout << "[TtmlDataPacket.ConstructionNoThrow] - START" << std::endl;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    EXPECT_NO_THROW({
        TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    });
    std::cout << "[TtmlDataPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket with 0-byte payload yields a 24-byte buffer.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(TtmlDataPacket, EmptyDataBuffer24Bytes)
{
    std::cout << "[TtmlDataPacket.EmptyDataBuffer24Bytes] - START" << std::endl;
    std::vector<uint8_t> empty;
    TtmlDataPacket pkt(1u, 0u, 0, std::move(empty));
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size with 0 data bytes: " << sz << " (expected 24)" << std::endl;
    EXPECT_EQ(sz, 24u);
    std::cout << "[TtmlDataPacket.EmptyDataBuffer24Bytes] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket with N-byte payload yields a (24+N)-byte buffer.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(TtmlDataPacket, BufferSizeIncludesPayload)
{
    std::cout << "[TtmlDataPacket.BufferSizeIncludesPayload] - START" << std::endl;
    const size_t N = 7;
    std::vector<uint8_t> data(N, 0xAB);
    TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size with " << N << " data bytes: " << sz << " (expected " << (24 + N) << ")" << std::endl;
    EXPECT_EQ(sz, 24u + N);
    std::cout << "[TtmlDataPacket.BufferSizeIncludesPayload] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket reports the correct packet type.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(TtmlDataPacket, TypeIsTtmlData)
{
    std::cout << "[TtmlDataPacket.TypeIsTtmlData] - START" << std::endl;
    std::vector<uint8_t> data = {0x01};
    TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    uint32_t t = pkt.getType();
    std::cout << "  getType() = " << t << " (expected " << TYPE_TTML_DATA << ")" << std::endl;
    EXPECT_EQ(t, TYPE_TTML_DATA);
    std::cout << "[TtmlDataPacket.TypeIsTtmlData] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket stores counter correctly.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(TtmlDataPacket, CounterStoredCorrectly)
{
    std::cout << "[TtmlDataPacket.CounterStoredCorrectly] - START" << std::endl;
    const uint32_t counter = 55u;
    std::vector<uint8_t> data = {0x00};
    TtmlDataPacket pkt(1u, counter, 0, std::move(data));
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected " << counter << ")" << std::endl;
    EXPECT_EQ(pkt.getCounter(), counter);
    EXPECT_EQ(readLE32(pkt.getBytes(), 4), counter);
    std::cout << "[TtmlDataPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket encodes channelId at bytes [12..15].
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(TtmlDataPacket, ChannelIdEncodedCorrectly)
{
    std::cout << "[TtmlDataPacket.ChannelIdEncodedCorrectly] - START" << std::endl;
    const uint32_t channelId = 99u;
    std::vector<uint8_t> data = {0x01, 0x02};
    TtmlDataPacket pkt(channelId, 0u, 0, std::move(data));
    uint32_t decoded = readLE32(pkt.getBytes(), 12);
    std::cout << "  channelId=" << channelId << ", buf[12..15]=" << decoded << std::endl;
    EXPECT_EQ(decoded, channelId);
    std::cout << "[TtmlDataPacket.ChannelIdEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket encodes dataOffset at bytes [16..23] as 64-bit LE.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(TtmlDataPacket, DataOffsetEncodedCorrectly)
{
    std::cout << "[TtmlDataPacket.DataOffsetEncodedCorrectly] - START" << std::endl;
    const int64_t offset = 123456789LL;
    std::vector<uint8_t> data = {0xAA};
    TtmlDataPacket pkt(1u, 0u, offset, std::move(data));
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    std::cout << "  dataOffset=" << offset << ", decoded from buf[16..23]=" << (int64_t)decoded << std::endl;
    EXPECT_EQ(static_cast<int64_t>(decoded), offset);
    std::cout << "[TtmlDataPacket.DataOffsetEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket payload bytes appear after the 24-byte header.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 008
 * @par Priority       : High
 */
TEST(TtmlDataPacket, PayloadBytesAtCorrectOffset)
{
    std::cout << "[TtmlDataPacket.PayloadBytesAtCorrectOffset] - START" << std::endl;
    std::vector<uint8_t> data = {0xDE, 0xAD, 0xBE, 0xEF};
    TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    const auto& buf = pkt.getBytes();
    ASSERT_EQ(buf.size(), 28u);
    for (size_t i = 0; i < 4; ++i)
    {
        std::cout << "  buf[" << (24+i) << "] = 0x" << std::hex << (int)buf[24+i] << std::dec << std::endl;
    }
    EXPECT_EQ(buf[24], 0xDE);
    EXPECT_EQ(buf[25], 0xAD);
    EXPECT_EQ(buf[26], 0xBE);
    EXPECT_EQ(buf[27], 0xEF);
    std::cout << "[TtmlDataPacket.PayloadBytesAtCorrectOffset] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket size field equals (8 + 4 + dataLen).
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 009
 * @par Priority       : High
 */
TEST(TtmlDataPacket, SizeFieldIsCorrect)
{
    std::cout << "[TtmlDataPacket.SizeFieldIsCorrect] - START" << std::endl;
    const size_t N = 5;
    std::vector<uint8_t> data(N, 0x00);
    TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    uint32_t expected = 8u + 4u + static_cast<uint32_t>(N);
    std::cout << "  size field=" << sizeField << " (expected " << expected << ")" << std::endl;
    EXPECT_EQ(sizeField, expected);
    std::cout << "[TtmlDataPacket.SizeFieldIsCorrect] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket with negative dataOffset is encoded correctly.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(TtmlDataPacket, NegativeDataOffset)
{
    std::cout << "[TtmlDataPacket.NegativeDataOffset] - START" << std::endl;
    const int64_t offset = -1000LL;
    std::vector<uint8_t> data = {0x01};
    TtmlDataPacket pkt(1u, 0u, offset, std::move(data));
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    std::cout << "  offset=" << offset << ", decoded as int64_t=" << static_cast<int64_t>(decoded) << std::endl;
    EXPECT_EQ(static_cast<int64_t>(decoded), offset);
    std::cout << "[TtmlDataPacket.NegativeDataOffset] - PASS" << std::endl;
}

/**
 * @brief TtmlDataPacket with large payload (1 KB) — verifies size and content.
 *
 * @par Test Group ID  : TtmlDataPacket
 * @par Test Case ID   : 011
 * @par Priority       : Medium (stress)
 */
TEST(TtmlDataPacket, LargePayload1KB)
{
    std::cout << "[TtmlDataPacket.LargePayload1KB] - START" << std::endl;
    const size_t N = 1024;
    std::vector<uint8_t> data(N);
    for (size_t i = 0; i < N; ++i) data[i] = static_cast<uint8_t>(i & 0xFF);
    TtmlDataPacket pkt(1u, 0u, 0, std::move(data));
    EXPECT_EQ(pkt.getBytes().size(), 24u + N);
    // Check first and last payload bytes
    EXPECT_EQ(pkt.getBytes()[24], 0x00u);
    EXPECT_EQ(pkt.getBytes()[24 + N - 1], static_cast<uint8_t>((N - 1) & 0xFF));
    std::cout << "  Large payload 1KB OK, total buf size=" << pkt.getBytes().size() << std::endl;
    std::cout << "[TtmlDataPacket.LargePayload1KB] - PASS" << std::endl;
}

// ===========================================================================
// TtmlTimestampPacket tests
// ===========================================================================

/**
 * @brief TtmlTimestampPacket construction does not throw.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, ConstructionNoThrow)
{
    std::cout << "[TtmlTimestampPacket.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ TtmlTimestampPacket pkt(1u, 0u, 90000ull); });
    std::cout << "[TtmlTimestampPacket.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket buffer is exactly 24 bytes.
 *
 * Layout: type(4) + counter(4) + size(4) + channelId(4) + timestamp(8) = 24
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, BufferSizeIs24)
{
    std::cout << "[TtmlTimestampPacket.BufferSizeIs24] - START" << std::endl;
    TtmlTimestampPacket pkt(1u, 0u, 1000ull);
    size_t sz = pkt.getBytes().size();
    std::cout << "  Buffer size = " << sz << " (expected 24)" << std::endl;
    EXPECT_EQ(sz, 24u);
    std::cout << "[TtmlTimestampPacket.BufferSizeIs24] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket reports the correct packet type.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, TypeIsTtmlTimestamp)
{
    std::cout << "[TtmlTimestampPacket.TypeIsTtmlTimestamp] - START" << std::endl;
    TtmlTimestampPacket pkt(1u, 0u, 0ull);
    uint32_t t = pkt.getType();
    std::cout << "  getType() = " << t << " (expected " << TYPE_TTML_TIMESTAMP << ")" << std::endl;
    EXPECT_EQ(t, TYPE_TTML_TIMESTAMP);
    std::cout << "[TtmlTimestampPacket.TypeIsTtmlTimestamp] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket getTypeString returns "TTML_TIMESTAMP".
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 004
 * @par Priority       : Medium
 */
TEST(TtmlTimestampPacket, TypeStringIsTtmlTimestamp)
{
    std::cout << "[TtmlTimestampPacket.TypeStringIsTtmlTimestamp] - START" << std::endl;
    TtmlTimestampPacket pkt(1u, 0u, 0ull);
    std::string ts = pkt.getTypeString(pkt.getType());
    std::cout << "  getTypeString() = \"" << ts << "\"" << std::endl;
    EXPECT_EQ(ts, "TTML_TIMESTAMP");
    std::cout << "[TtmlTimestampPacket.TypeStringIsTtmlTimestamp] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket stores the counter correctly.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, CounterStoredCorrectly)
{
    std::cout << "[TtmlTimestampPacket.CounterStoredCorrectly] - START" << std::endl;
    const uint32_t counter = 13u;
    TtmlTimestampPacket pkt(1u, counter, 0ull);
    std::cout << "  getCounter() = " << pkt.getCounter() << " (expected " << counter << ")" << std::endl;
    EXPECT_EQ(pkt.getCounter(), counter);
    EXPECT_EQ(readLE32(pkt.getBytes(), 4), counter);
    std::cout << "[TtmlTimestampPacket.CounterStoredCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket encodes channelId at bytes [12..15].
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 006
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, ChannelIdEncodedCorrectly)
{
    std::cout << "[TtmlTimestampPacket.ChannelIdEncodedCorrectly] - START" << std::endl;
    const uint32_t channelId = 7u;
    TtmlTimestampPacket pkt(channelId, 0u, 0ull);
    uint32_t decoded = readLE32(pkt.getBytes(), 12);
    std::cout << "  channelId=" << channelId << ", buf[12..15]=" << decoded << std::endl;
    EXPECT_EQ(decoded, channelId);
    std::cout << "[TtmlTimestampPacket.ChannelIdEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket encodes timestamp at bytes [16..23] as 64-bit LE.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 007
 * @par Priority       : High
 */
TEST(TtmlTimestampPacket, TimestampEncodedCorrectly)
{
    std::cout << "[TtmlTimestampPacket.TimestampEncodedCorrectly] - START" << std::endl;
    const uint64_t ts = 987654321ull;
    TtmlTimestampPacket pkt(1u, 0u, ts);
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    std::cout << "  timestamp=" << ts << ", decoded=" << decoded << std::endl;
    EXPECT_EQ(decoded, ts);
    std::cout << "[TtmlTimestampPacket.TimestampEncodedCorrectly] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket size field at bytes [8..11] is 12.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 008
 * @par Priority       : Medium
 */
TEST(TtmlTimestampPacket, SizeFieldIs12)
{
    std::cout << "[TtmlTimestampPacket.SizeFieldIs12] - START" << std::endl;
    TtmlTimestampPacket pkt(1u, 0u, 0ull);
    uint32_t sizeField = readLE32(pkt.getBytes(), 8);
    std::cout << "  size field = " << sizeField << " (expected 12)" << std::endl;
    EXPECT_EQ(sizeField, 12u);
    std::cout << "[TtmlTimestampPacket.SizeFieldIs12] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket with zero timestamp.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(TtmlTimestampPacket, ZeroTimestamp)
{
    std::cout << "[TtmlTimestampPacket.ZeroTimestamp] - START" << std::endl;
    TtmlTimestampPacket pkt(0u, 0u, 0ull);
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    EXPECT_EQ(decoded, 0ull);
    EXPECT_EQ(pkt.getBytes().size(), 24u);
    std::cout << "  Zero timestamp encoded correctly" << std::endl;
    std::cout << "[TtmlTimestampPacket.ZeroTimestamp] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket with UINT64_MAX timestamp.
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 010
 * @par Priority       : Medium
 */
TEST(TtmlTimestampPacket, MaxTimestamp)
{
    std::cout << "[TtmlTimestampPacket.MaxTimestamp] - START" << std::endl;
    const uint64_t MAX_TS = std::numeric_limits<uint64_t>::max();
    TtmlTimestampPacket pkt(1u, 0u, MAX_TS);
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    std::cout << "  UINT64_MAX timestamp encoded correctly, decoded=" << decoded << std::endl;
    EXPECT_EQ(decoded, MAX_TS);
    std::cout << "[TtmlTimestampPacket.MaxTimestamp] - PASS" << std::endl;
}

/**
 * @brief TtmlTimestampPacket with large typical media timestamp (90 kHz PTS).
 *
 * @par Test Group ID  : TtmlTimestampPacket
 * @par Test Case ID   : 011
 * @par Priority       : Medium
 */
TEST(TtmlTimestampPacket, LargeMediaTimestamp)
{
    std::cout << "[TtmlTimestampPacket.LargeMediaTimestamp] - START" << std::endl;
    // 1 hour at 90 kHz: 1 * 3600 * 90000 = 324000000
    const uint64_t ts = 324000000ull;
    TtmlTimestampPacket pkt(1u, 0u, ts);
    uint64_t decoded = readLE64(pkt.getBytes(), 16);
    EXPECT_EQ(decoded, ts);
    std::cout << "  90kHz 1-hour PTS=" << ts << " OK" << std::endl;
    std::cout << "[TtmlTimestampPacket.LargeMediaTimestamp] - PASS" << std::endl;
}

// ===========================================================================
// TtmlChannel tests
// ===========================================================================

/**
 * @brief TtmlChannel can be constructed without throwing.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 001
 * @par Priority       : High
 */
TEST(TtmlChannel, ConstructionNoThrow)
{
    std::cout << "[TtmlChannel.ConstructionNoThrow] - START" << std::endl;
    EXPECT_NO_THROW({ TtmlChannel ch; });
    std::cout << "[TtmlChannel.ConstructionNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendSelectionPacket does not throw or hang.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 002
 * @par Priority       : High
 */
TEST(TtmlChannel, SendSelectionPacketNoThrow)
{
    std::cout << "[TtmlChannel.SendSelectionPacketNoThrow] - START" << std::endl;
    TtmlChannel ch;
    EXPECT_NO_THROW({ ch.SendSelectionPacket(1920u, 1080u); });
    std::cout << "  SendSelectionPacket(1920, 1080) OK" << std::endl;
    std::cout << "[TtmlChannel.SendSelectionPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket with non-empty data does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 003
 * @par Priority       : High
 */
TEST(TtmlChannel, SendDataPacketNoThrow)
{
    std::cout << "[TtmlChannel.SendDataPacketNoThrow] - START" << std::endl;
    TtmlChannel ch;
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04, 0x05};
    EXPECT_NO_THROW({ ch.SendDataPacket(std::move(data)); });
    std::cout << "  SendDataPacket({5 bytes}) OK" << std::endl;
    std::cout << "[TtmlChannel.SendDataPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket with time_offset_ms does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 004
 * @par Priority       : High
 */
TEST(TtmlChannel, SendDataPacketWithOffsetNoThrow)
{
    std::cout << "[TtmlChannel.SendDataPacketWithOffsetNoThrow] - START" << std::endl;
    TtmlChannel ch;
    std::vector<uint8_t> data = {0xDE, 0xAD};
    EXPECT_NO_THROW({ ch.SendDataPacket(std::move(data), 500LL); });
    std::cout << "  SendDataPacket with offset=500ms OK" << std::endl;
    std::cout << "[TtmlChannel.SendDataPacketWithOffsetNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendTimestampPacket does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 005
 * @par Priority       : High
 */
TEST(TtmlChannel, SendTimestampPacketNoThrow)
{
    std::cout << "[TtmlChannel.SendTimestampPacketNoThrow] - START" << std::endl;
    TtmlChannel ch;
    EXPECT_NO_THROW({ ch.SendTimestampPacket(90000ull); });
    std::cout << "  SendTimestampPacket(90000) OK" << std::endl;
    std::cout << "[TtmlChannel.SendTimestampPacketNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel::SendDataPacket with empty data does not throw.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 006
 * @par Priority       : Medium
 */
TEST(TtmlChannel, SendDataPacketEmptyDataNoThrow)
{
    std::cout << "[TtmlChannel.SendDataPacketEmptyDataNoThrow] - START" << std::endl;
    TtmlChannel ch;
    std::vector<uint8_t> empty;
    EXPECT_NO_THROW({ ch.SendDataPacket(std::move(empty)); });
    std::cout << "  SendDataPacket with empty vector OK" << std::endl;
    std::cout << "[TtmlChannel.SendDataPacketEmptyDataNoThrow] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel: multiple consecutive sends do not hang.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 007
 * @par Priority       : Medium (stress)
 */
TEST(TtmlChannel, MultipleConsecutiveSendsNoHang)
{
    std::cout << "[TtmlChannel.MultipleConsecutiveSendsNoHang] - START" << std::endl;
    TtmlChannel ch;
    const int N = 20;
    for (int i = 0; i < N; ++i)
    {
        std::vector<uint8_t> data(4, static_cast<uint8_t>(i));
        EXPECT_NO_THROW({ ch.SendSelectionPacket(800u, 600u); });
        EXPECT_NO_THROW({ ch.SendDataPacket(std::move(data), static_cast<int64_t>(i * 40)); });
        EXPECT_NO_THROW({ ch.SendTimestampPacket(static_cast<uint64_t>(i * 1000)); });
    }
    std::cout << "  " << N << " iterations of all 3 send methods OK" << std::endl;
    std::cout << "[TtmlChannel.MultipleConsecutiveSendsNoHang] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel destructor is safe after sends.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 008
 * @par Priority       : Medium
 */
TEST(TtmlChannel, DestructorSafeAfterSends)
{
    std::cout << "[TtmlChannel.DestructorSafeAfterSends] - START" << std::endl;
    {
        TtmlChannel ch;
        EXPECT_NO_THROW({ ch.SendSelectionPacket(1280u, 720u); });
        std::vector<uint8_t> data = {0x11, 0x22};
        EXPECT_NO_THROW({ ch.SendDataPacket(std::move(data)); });
        EXPECT_NO_THROW({ ch.SendTimestampPacket(5000ull); });
        // Channel destroyed at end of scope
    }
    std::cout << "  TtmlChannel destructor safe after 3 sends" << std::endl;
    std::cout << "[TtmlChannel.DestructorSafeAfterSends] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel via base SubtecChannel pointer dispatches virtual methods correctly.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 009
 * @par Priority       : Medium
 */
TEST(TtmlChannel, VirtualDispatchViaBasePointer)
{
    std::cout << "[TtmlChannel.VirtualDispatchViaBasePointer] - START" << std::endl;
    {
        std::unique_ptr<SubtecChannel> ch = std::make_unique<TtmlChannel>();
        EXPECT_NO_THROW({ ch->SendSelectionPacket(640u, 480u); });
        std::vector<uint8_t> data = {0xAA, 0xBB};
        EXPECT_NO_THROW({ ch->SendDataPacket(std::move(data)); });
        EXPECT_NO_THROW({ ch->SendTimestampPacket(1000ull); });
    }
    std::cout << "  Virtual dispatch via SubtecChannel* works correctly" << std::endl;
    std::cout << "[TtmlChannel.VirtualDispatchViaBasePointer] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel: SendSelectionPacket with zero dimensions.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 010
 * @par Priority       : Low (edge case)
 */
TEST(TtmlChannel, SendSelectionPacketZeroDimensions)
{
    std::cout << "[TtmlChannel.SendSelectionPacketZeroDimensions] - START" << std::endl;
    TtmlChannel ch;
    EXPECT_NO_THROW({ ch.SendSelectionPacket(0u, 0u); });
    std::cout << "  SendSelectionPacket(0, 0) does not crash" << std::endl;
    std::cout << "[TtmlChannel.SendSelectionPacketZeroDimensions] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel: SendTimestampPacket with zero timestamp.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 011
 * @par Priority       : Low (edge case)
 */
TEST(TtmlChannel, SendTimestampPacketZero)
{
    std::cout << "[TtmlChannel.SendTimestampPacketZero] - START" << std::endl;
    TtmlChannel ch;
    EXPECT_NO_THROW({ ch.SendTimestampPacket(0ull); });
    std::cout << "  SendTimestampPacket(0) OK" << std::endl;
    std::cout << "[TtmlChannel.SendTimestampPacketZero] - PASS" << std::endl;
}

/**
 * @brief TtmlChannel stress: 100 rapid selection+data+timestamp cycles.
 *
 * @par Test Group ID  : TtmlChannel
 * @par Test Case ID   : 012
 * @par Priority       : Low (stress)
 */
TEST(TtmlChannel, Stress100Cycles)
{
    std::cout << "[TtmlChannel.Stress100Cycles] - START" << std::endl;
    TtmlChannel ch;
    for (int i = 0; i < 100; ++i)
    {
        EXPECT_NO_THROW({
            ch.SendSelectionPacket(1280u, 720u);
            std::vector<uint8_t> d(8, static_cast<uint8_t>(i));
            ch.SendDataPacket(std::move(d), static_cast<int64_t>(i * 33));
            ch.SendTimestampPacket(static_cast<uint64_t>(i) * 3000ull);
        });
    }
    std::cout << "  100 selection+data+timestamp cycles OK" << std::endl;
    std::cout << "[TtmlChannel.Stress100Cycles] - PASS" << std::endl;
}
