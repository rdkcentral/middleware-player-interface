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
 * @file PlayerIsoBmffBoxParseTests.cpp
 * @brief Unit tests that drive the ISO BMFF box parsers in playerisobmffbox.cpp
 *        using synthetic, big-endian box byte buffers.
 *
 * Coverage focus: constructBox dispatch and size correction, container
 * traversal, the typed construct*() parsers for every box type (including
 * version and flag branches), setters that write back into the buffer, and
 * the truncate() helpers for trun, senc, saiz and mdat.
 */

#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include "playerisobmffbox.h"

using namespace player_isobmff;

namespace
{
using Bytes = std::vector<uint8_t>;

// ------------------------------------------------------------------------------
// Byte building helpers (big-endian, matching the ISO BMFF wire format)
// ------------------------------------------------------------------------------
void Put8(Bytes &b, uint32_t v) { b.push_back(static_cast<uint8_t>(v & 0xFFu)); }
void Put16(Bytes &b, uint32_t v) { Put8(b, v >> 8); Put8(b, v); }
void Put32(Bytes &b, uint32_t v) { Put16(b, v >> 16); Put16(b, v); }
void Put64(Bytes &b, uint64_t v)
{
	Put32(b, static_cast<uint32_t>(v >> 32));
	Put32(b, static_cast<uint32_t>(v));
}

void PutStr(Bytes &b, const char *s)
{
	for (; *s != '\0'; ++s)
	{
		Put8(b, static_cast<uint8_t>(*s));
	}
	Put8(b, 0); // null terminator is part of the C-string field
}

void PutVerFlags(Bytes &b, uint8_t version, uint32_t flags)
{
	Put8(b, version);
	Put8(b, flags >> 16);
	Put8(b, flags >> 8);
	Put8(b, flags);
}

void Append(Bytes &dst, const Bytes &src)
{
	dst.insert(dst.end(), src.begin(), src.end());
}

Bytes Concat(const Bytes &a, const Bytes &b)
{
	Bytes out = a;
	Append(out, b);
	return out;
}

// Builds a complete box: size(4) + type(4) + payload
Bytes MakeBox(const char *type, const Bytes &payload)
{
	Bytes box;
	Put32(box, static_cast<uint32_t>(8 + payload.size()));
	for (int i = 0; i < 4; ++i)
	{
		Put8(box, static_cast<uint8_t>(type[i]));
	}
	Append(box, payload);
	return box;
}

// Parses the first box in the buffer. The buffer must outlive the returned box.
std::unique_ptr<IsoBmffBox> ParseBox(Bytes &buf, bool correctBoxSize = false, int newTrackId = -1)
{
	return std::unique_ptr<IsoBmffBox>(
		IsoBmffBox::constructBox(buf.data(), static_cast<uint32_t>(buf.size()), correctBoxSize, newTrackId));
}

uint32_t ReadBe32(const Bytes &b, size_t off)
{
	return (static_cast<uint32_t>(b[off]) << 24) |
	       (static_cast<uint32_t>(b[off + 1]) << 16) |
	       (static_cast<uint32_t>(b[off + 2]) << 8) |
	       static_cast<uint32_t>(b[off + 3]);
}

bool TypeIs(const IsoBmffBox *box, const char *type)
{
	return std::memcmp(box->getType(), type, 4) == 0;
}

char *DupStr(const char *s)
{
	size_t n = std::strlen(s) + 1;
	char *d = static_cast<char *>(malloc(n));
	std::memcpy(d, s, n);
	return d;
}

// ------------------------------------------------------------------------------
// Payload builders (payload = everything after the 8-byte box header)
// ------------------------------------------------------------------------------

// mvhd / mdhd: creation, modification, timescale, duration
Bytes TimescalePayload(uint8_t version, uint32_t timeScale)
{
	Bytes p;
	PutVerFlags(p, version, 0);
	if (1 == version)
	{
		Put64(p, 0);
		Put64(p, 0);
	}
	else
	{
		Put32(p, 0);
		Put32(p, 0);
	}
	Put32(p, timeScale);
	Put32(p, 0); // duration
	return p;
}

// tkhd: creation, modification, track_id
Bytes TkhdPayload(uint8_t version, uint32_t trackId)
{
	Bytes p;
	PutVerFlags(p, version, 0);
	if (1 == version)
	{
		Put64(p, 0);
		Put64(p, 0);
	}
	else
	{
		Put32(p, 0);
		Put32(p, 0);
	}
	Put32(p, trackId);
	return p;
}

// trun with data-offset, first-sample-flags, duration, size, sample-flags and cto present (flags 0x000F05)
Bytes TrunAllFieldsPayload()
{
	Bytes p;
	PutVerFlags(p, 0, 0x000F05);
	Put32(p, 2);          // sample_count
	Put32(p, 0x40);       // data_offset
	Put32(p, 0x02000000); // first_sample_flags
	// sample 1: duration, size, flags, cto
	Put32(p, 100);
	Put32(p, 500);
	Put32(p, 0x01010000);
	Put32(p, 0);
	// sample 2: duration, size, flags, cto
	Put32(p, 200);
	Put32(p, 600);
	Put32(p, 0x01010000);
	Put32(p, 0);
	return p;
}

// Version 0 emsg: scheme, value, timescale, presentation_time_delta, event_duration, id, message
Bytes EmsgV0Payload(const char *scheme, const char *value, const Bytes &msg)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	PutStr(p, scheme);
	PutStr(p, value);
	Put32(p, 1000); // timescale
	Put32(p, 5);    // presentation_time_delta
	Put32(p, 200);  // event_duration
	Put32(p, 7);    // id
	Append(p, msg);
	return p;
}

// Version 1 emsg: timescale, presentation_time, event_duration, id, scheme, value, message
Bytes EmsgV1Payload(const char *scheme, const char *value, const Bytes &msg)
{
	Bytes p;
	PutVerFlags(p, 1, 0);
	Put32(p, 90000);            // timescale
	Put64(p, 0x0000000100000002ULL); // presentation_time
	Put32(p, 300);              // event_duration
	Put32(p, 9);                // id
	PutStr(p, scheme);
	PutStr(p, value);
	Append(p, msg);
	return p;
}

// sidx version 0 with two references
Bytes SidxV0Payload(uint32_t timeScale, uint32_t dur1, uint32_t dur2)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put32(p, 1);          // reference_ID
	Put32(p, timeScale);  // timescale
	Put32(p, 0);          // earliest_presentation_time
	Put32(p, 0);          // first_offset
	Put16(p, 0);          // reserved
	Put16(p, 2);          // reference_count
	Put32(p, 0x00000100); // ref 1: type/size
	Put32(p, dur1);       //        duration
	Put32(p, 0x90000000); //        SAP
	Put32(p, 0x00000200); // ref 2: type/size
	Put32(p, dur2);       //        duration
	Put32(p, 0x90000000); //        SAP
	return p;
}

// sidx version 1 with one reference (64-bit times)
Bytes SidxV1Payload(uint32_t timeScale, uint32_t dur)
{
	Bytes p;
	PutVerFlags(p, 1, 0);
	Put32(p, 1);         // reference_ID
	Put32(p, timeScale); // timescale
	Put64(p, 0);         // earliest_presentation_time
	Put64(p, 0);         // first_offset
	Put16(p, 0);         // reserved
	Put16(p, 1);         // reference_count
	Put32(p, 0x00000100);
	Put32(p, dur);
	Put32(p, 0x90000000);
	return p;
}

} // namespace

// ==============================================================================
// constructBox dispatch and size handling
// ==============================================================================

TEST(IsoBmffBoxParse, ConstructBox_BufferSmallerThanHeader_ReturnsUnknownBox)
{
	Bytes buf{0x00, 0x00, 0x00};
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);
	EXPECT_EQ(box->getSize(), 3u);
	EXPECT_TRUE(std::memcmp(box->getType(), "UKWN", 4) == 0);
}

TEST(IsoBmffBoxParse, ConstructBox_BufferBetween4And7Bytes_ReturnsUnknownBox)
{
	Bytes buf(6, 0x00);
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);
	EXPECT_EQ(box->getSize(), 6u);
	EXPECT_TRUE(std::memcmp(box->getType(), "UKWN", 4) == 0);
}

TEST(IsoBmffBoxParse, ConstructBox_SizeExceedsBuffer_CorrectionEnabled_RewritesSizeAndParses)
{
	Bytes buf = MakeBox("mdat", Bytes(8, 0xAB));
	// Declare a size far larger than the buffer
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;
	buf[3] = 100;

	auto box = ParseBox(buf, true);
	ASSERT_NE(box, nullptr);
	EXPECT_EQ(box->getSize(), 16u);
	EXPECT_EQ(ReadBe32(buf, 0), 16u); // header rewritten to the real size
	EXPECT_NE(dynamic_cast<MdatIsoBmffBox *>(box.get()), nullptr);
}

TEST(IsoBmffBoxParse, ConstructBox_SizeExceedsBuffer_CorrectionDisabled_ReturnsGenericBox)
{
	Bytes buf = MakeBox("moov", Bytes(8, 0x00));
	buf[3] = 100;

	auto box = ParseBox(buf, false);
	ASSERT_NE(box, nullptr);
	EXPECT_EQ(box->getSize(), 100u);
	EXPECT_TRUE(TypeIs(box.get(), "moov"));
	EXPECT_EQ(dynamic_cast<GenericContainerIsoBmffBox *>(box.get()), nullptr);
	EXPECT_FALSE(box->hasChildren());
}

TEST(IsoBmffBoxParse, ConstructBox_UnknownType_ReturnsGenericBox)
{
	Bytes buf = MakeBox("free", Bytes{1, 2, 3, 4});
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);
	EXPECT_TRUE(TypeIs(box.get(), "free"));
	EXPECT_EQ(box->getSize(), 12u);
	EXPECT_EQ(dynamic_cast<GenericContainerIsoBmffBox *>(box.get()), nullptr);
}

TEST(IsoBmffBoxParse, ConstructBox_EmptyContainers_HaveNoChildren)
{
	const char *types[] = {"moof", "traf", "mdia"};
	for (const char *type : types)
	{
		Bytes buf = MakeBox(type, Bytes());
		auto box = ParseBox(buf);
		ASSERT_NE(box, nullptr) << type;
		EXPECT_TRUE(TypeIs(box.get(), type)) << type;
		ASSERT_NE(box->getChildren(), nullptr) << type;
		EXPECT_TRUE(box->getChildren()->empty()) << type;
	}
}

// ==============================================================================
// Container traversal
// ==============================================================================

TEST(IsoBmffBoxParse, Container_MoovWithMvhd_ExposesChildrenAndTimescale)
{
	Bytes buf = MakeBox("moov", MakeBox("mvhd", TimescalePayload(0, 600)));
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);
	ASSERT_TRUE(box->hasChildren());

	const auto *children = box->getChildren();
	ASSERT_NE(children, nullptr);
	ASSERT_EQ(children->size(), 1u);

	IsoBmffBox *child = children->at(0);
	EXPECT_TRUE(TypeIs(child, "mvhd"));

	auto *mvhd = dynamic_cast<MvhdIsoBmffBox *>(child);
	ASSERT_NE(mvhd, nullptr);
	EXPECT_EQ(mvhd->getTimeScale(), 600u);
}

TEST(IsoBmffBoxParse, Container_MultipleChildren_AreParsedInOrder)
{
	Bytes payload = Concat(MakeBox("mvhd", TimescalePayload(0, 1000)),
	                       MakeBox("mdhd", TimescalePayload(1, 48000)));
	Bytes buf = MakeBox("moov", payload);
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);

	const auto *children = box->getChildren();
	ASSERT_NE(children, nullptr);
	ASSERT_EQ(children->size(), 2u);
	EXPECT_TRUE(TypeIs(children->at(0), "mvhd"));
	EXPECT_TRUE(TypeIs(children->at(1), "mdhd"));

	auto *mdhd = dynamic_cast<MdhdIsoBmffBox *>(children->at(1));
	ASSERT_NE(mdhd, nullptr);
	EXPECT_EQ(mdhd->getTimeScale(), 48000u);
}

// ==============================================================================
// Trak / tkhd track id handling
// ==============================================================================

TEST(IsoBmffBoxParse, Trak_TkhdVersion0_ReadsTrackId)
{
	Bytes buf = MakeBox("trak", Concat(MakeBox("tkhd", TkhdPayload(0, 3)), MakeBox("mdia", Bytes())));
	auto box = ParseBox(buf);
	auto *trak = dynamic_cast<TrakIsoBmffBox *>(box.get());
	ASSERT_NE(trak, nullptr);
	EXPECT_EQ(trak->getTrack_Id(), 3u);
}

TEST(IsoBmffBoxParse, Trak_TkhdVersion1_ReadsTrackId)
{
	Bytes buf = MakeBox("trak", Concat(MakeBox("tkhd", TkhdPayload(1, 11)), MakeBox("mdia", Bytes())));
	auto box = ParseBox(buf);
	auto *trak = dynamic_cast<TrakIsoBmffBox *>(box.get());
	ASSERT_NE(trak, nullptr);
	EXPECT_EQ(trak->getTrack_Id(), 11u);
}

TEST(IsoBmffBoxParse, Trak_OverrideTrackId_WritesNewIdIntoBuffer)
{
	Bytes buf = MakeBox("trak", MakeBox("tkhd", TkhdPayload(0, 3)));
	// trak header(8) + tkhd header(8) + version/flags(4) + creation(4) + modification(4) = 28
	auto box = ParseBox(buf, false, 42);
	auto *trak = dynamic_cast<TrakIsoBmffBox *>(box.get());
	ASSERT_NE(trak, nullptr);
	EXPECT_EQ(trak->getTrack_Id(), 42u);
	EXPECT_EQ(ReadBe32(buf, 28), 42u);
}

TEST(IsoBmffBoxParse, Trak_WithoutTkhd_ConstructsTrakBox)
{
	Bytes buf = MakeBox("trak", MakeBox("mdia", Bytes()));
	auto box = ParseBox(buf);
	auto *trak = dynamic_cast<TrakIsoBmffBox *>(box.get());
	ASSERT_NE(trak, nullptr);
}

// ==============================================================================
// mvhd / mdhd
// ==============================================================================

TEST(IsoBmffBoxParse, Mvhd_Version0_ReadsAndWritesTimeScale)
{
	Bytes buf = MakeBox("mvhd", TimescalePayload(0, 1000));
	auto box = ParseBox(buf);
	auto *mvhd = dynamic_cast<MvhdIsoBmffBox *>(box.get());
	ASSERT_NE(mvhd, nullptr);
	EXPECT_EQ(mvhd->getTimeScale(), 1000u);

	mvhd->setTimeScale(2000);
	EXPECT_EQ(mvhd->getTimeScale(), 2000u);
	// timescale sits at header(8) + version/flags(4) + creation(4) + modification(4)
	EXPECT_EQ(ReadBe32(buf, 20), 2000u);
}

TEST(IsoBmffBoxParse, Mvhd_Version1_ReadsTimeScaleAfterWideTimes)
{
	Bytes buf = MakeBox("mvhd", TimescalePayload(1, 90000));
	auto box = ParseBox(buf);
	auto *mvhd = dynamic_cast<MvhdIsoBmffBox *>(box.get());
	ASSERT_NE(mvhd, nullptr);
	EXPECT_EQ(mvhd->getTimeScale(), 90000u);
}

TEST(IsoBmffBoxParse, Mdhd_Version1_ReadsAndWritesTimeScale)
{
	Bytes buf = MakeBox("mdhd", TimescalePayload(1, 48000));
	auto box = ParseBox(buf);
	auto *mdhd = dynamic_cast<MdhdIsoBmffBox *>(box.get());
	ASSERT_NE(mdhd, nullptr);
	EXPECT_EQ(mdhd->getTimeScale(), 48000u);

	mdhd->setTimeScale(44100);
	EXPECT_EQ(mdhd->getTimeScale(), 44100u);
	// timescale sits at header(8) + version/flags(4) + creation(8) + modification(8)
	EXPECT_EQ(ReadBe32(buf, 28), 44100u);
}

TEST(IsoBmffBoxParse, Mdhd_Version0_ReadsTimeScale)
{
	Bytes buf = MakeBox("mdhd", TimescalePayload(0, 25));
	auto box = ParseBox(buf);
	auto *mdhd = dynamic_cast<MdhdIsoBmffBox *>(box.get());
	ASSERT_NE(mdhd, nullptr);
	EXPECT_EQ(mdhd->getTimeScale(), 25u);
}

// ==============================================================================
// tfdt
// ==============================================================================

TEST(IsoBmffBoxParse, Tfdt_Version0_ReadsAndWritesBaseMediaDecodeTime)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put32(p, 0x01020304);
	Bytes buf = MakeBox("tfdt", p);
	auto box = ParseBox(buf);
	auto *tfdt = dynamic_cast<TfdtIsoBmffBox *>(box.get());
	ASSERT_NE(tfdt, nullptr);
	EXPECT_EQ(tfdt->getBaseMDT(), 0x01020304u);

	tfdt->setBaseMDT(0x0A0B0C0D);
	EXPECT_EQ(tfdt->getBaseMDT(), 0x0A0B0C0Du);
	EXPECT_EQ(ReadBe32(buf, 12), 0x0A0B0C0Du);
}

TEST(IsoBmffBoxParse, Tfdt_Version1_ReadsAndWritesBaseMediaDecodeTime64)
{
	Bytes p;
	PutVerFlags(p, 1, 0);
	Put64(p, 0x0102030405060708ULL);
	Bytes buf = MakeBox("tfdt", p);
	auto box = ParseBox(buf);
	auto *tfdt = dynamic_cast<TfdtIsoBmffBox *>(box.get());
	ASSERT_NE(tfdt, nullptr);
	EXPECT_EQ(tfdt->getBaseMDT(), 0x0102030405060708ULL);

	tfdt->setBaseMDT(0x1122334455667788ULL);
	EXPECT_EQ(tfdt->getBaseMDT(), 0x1122334455667788ULL);

	// Re-parse the modified buffer to confirm the write landed in the right bytes
	auto reparsed = ParseBox(buf);
	auto *tfdt2 = dynamic_cast<TfdtIsoBmffBox *>(reparsed.get());
	ASSERT_NE(tfdt2, nullptr);
	EXPECT_EQ(tfdt2->getBaseMDT(), 0x1122334455667788ULL);
}

// ==============================================================================
// emsg
// ==============================================================================

TEST(IsoBmffBoxParse, Emsg_Version0_ParsesStringsFieldsAndMessage)
{
	Bytes buf = MakeBox("emsg", EmsgV0Payload("urn:x", "1", Bytes{'h', 'i'}));
	auto box = ParseBox(buf);
	auto *emsg = dynamic_cast<EmsgIsoBmffBox *>(box.get());
	ASSERT_NE(emsg, nullptr);

	ASSERT_NE(emsg->getSchemeIdUri(), nullptr);
	EXPECT_STREQ(emsg->getSchemeIdUri(), "urn:x");
	ASSERT_NE(emsg->getValue(), nullptr);
	EXPECT_EQ(emsg->getValue()[0], '1');
	EXPECT_EQ(emsg->getTimeScale(), 1000u);
	EXPECT_EQ(emsg->getPresentationTimeDelta(), 5u);
	EXPECT_EQ(emsg->getEventDuration(), 200u);
	EXPECT_EQ(emsg->getId(), 7u);
	ASSERT_NE(emsg->getMessage(), nullptr);
	EXPECT_EQ(emsg->getMessageLen(), 2u);
	EXPECT_EQ(emsg->getMessage()[0], 'h');
	EXPECT_EQ(emsg->getMessage()[1], 'i');
}

TEST(IsoBmffBoxParse, Emsg_Version0_WithoutMessage_HasNoMessageData)
{
	Bytes buf = MakeBox("emsg", EmsgV0Payload("urn:x", "", Bytes()));
	auto box = ParseBox(buf);
	auto *emsg = dynamic_cast<EmsgIsoBmffBox *>(box.get());
	ASSERT_NE(emsg, nullptr);
	EXPECT_STREQ(emsg->getSchemeIdUri(), "urn:x");
	EXPECT_EQ(emsg->getMessage(), nullptr);
	EXPECT_EQ(emsg->getMessageLen(), 0u);
}

TEST(IsoBmffBoxParse, Emsg_Version1_ParsesPresentationTimeAndMessage)
{
	Bytes buf = MakeBox("emsg", EmsgV1Payload("s", "v", Bytes{'a', 'b', 'c'}));
	auto box = ParseBox(buf);
	auto *emsg = dynamic_cast<EmsgIsoBmffBox *>(box.get());
	ASSERT_NE(emsg, nullptr);

	EXPECT_EQ(emsg->getTimeScale(), 90000u);
	EXPECT_EQ(emsg->getPresentationTime(), 0x0000000100000002ULL);
	EXPECT_EQ(emsg->getEventDuration(), 300u);
	EXPECT_EQ(emsg->getId(), 9u);
	EXPECT_STREQ(emsg->getSchemeIdUri(), "s");
	ASSERT_NE(emsg->getValue(), nullptr);
	EXPECT_EQ(emsg->getValue()[0], 'v');
	EXPECT_EQ(emsg->getMessageLen(), 3u);
	ASSERT_NE(emsg->getMessage(), nullptr);
	EXPECT_EQ(emsg->getMessage()[2], 'c');
}

TEST(IsoBmffBoxParse, Emsg_UnsupportedVersion_ReturnsBoxWithZeroedFields)
{
	Bytes p;
	PutVerFlags(p, 2, 0);
	Put32(p, 1234);
	Bytes buf = MakeBox("emsg", p);
	auto box = ParseBox(buf);
	auto *emsg = dynamic_cast<EmsgIsoBmffBox *>(box.get());
	ASSERT_NE(emsg, nullptr);
	EXPECT_TRUE(TypeIs(emsg, "emsg"));
	EXPECT_EQ(emsg->getTimeScale(), 0u);
	EXPECT_EQ(emsg->getMessage(), nullptr);
}

TEST(IsoBmffBoxParse, Emsg_SettersAndGetters_RoundTrip)
{
	std::unique_ptr<EmsgIsoBmffBox> emsg(new EmsgIsoBmffBox(20, 1, 2, 3));

	emsg->setTimeScale(5);
	EXPECT_EQ(emsg->getTimeScale(), 5u);
	emsg->setEventDuration(9);
	EXPECT_EQ(emsg->getEventDuration(), 9u);
	emsg->setId(8);
	EXPECT_EQ(emsg->getId(), 8u);
	emsg->setPresentationTimeDelta(7);
	EXPECT_EQ(emsg->getPresentationTimeDelta(), 7u);
	emsg->setPresentationTime(6);
	EXPECT_EQ(emsg->getPresentationTime(), 6u);

	// Ownership of these heap strings transfers to the box; the destructor frees them
	emsg->setSchemeIdUri(DupStr("urn:scheme"));
	EXPECT_STREQ(emsg->getSchemeIdUri(), "urn:scheme");
	emsg->setValue(reinterpret_cast<uint8_t *>(DupStr("v")));
	EXPECT_EQ(emsg->getValue()[0], 'v');

	uint8_t *msg = static_cast<uint8_t *>(malloc(4));
	std::memcpy(msg, "abcd", 4);
	emsg->setMessage(msg, 4);
	EXPECT_EQ(emsg->getMessage(), msg);
	EXPECT_EQ(emsg->getMessageLen(), 4u);
}

// ==============================================================================
// trun
// ==============================================================================

TEST(IsoBmffBoxParse, Trun_AllOptionalFields_ParsesSamplesAndFirstSampleSize)
{
	Bytes buf = MakeBox("trun", TrunAllFieldsPayload());
	auto box = ParseBox(buf);
	auto *trun = dynamic_cast<TrunIsoBmffBox *>(box.get());
	ASSERT_NE(trun, nullptr);

	EXPECT_EQ(trun->getSampleCount(), 2u);
	EXPECT_EQ(trun->getSampleDuration(), 300u);
	EXPECT_EQ(trun->getFirstSampleSize(), 500u);
	EXPECT_TRUE(trun->sampleDurationPresent());
}

TEST(IsoBmffBoxParse, Trun_SetFirstSampleDuration_WritesIntoBuffer)
{
	Bytes buf = MakeBox("trun", TrunAllFieldsPayload());
	auto box = ParseBox(buf);
	auto *trun = dynamic_cast<TrunIsoBmffBox *>(box.get());
	ASSERT_NE(trun, nullptr);

	trun->setFirstSampleDuration(777);
	// first sample duration: header(8) + version/flags(4) + sample_count(4) + data_offset(4) + first_flags(4)
	EXPECT_EQ(ReadBe32(buf, 24), 777u);
}

TEST(IsoBmffBoxParse, Trun_TruncateWithRoom_KeepsOneSampleAndInsertsSkipBox)
{
	// flags 0x000300: sample duration + sample size per sample (8 bytes per sample)
	Bytes p;
	PutVerFlags(p, 0, 0x000300);
	Put32(p, 2);
	Put32(p, 100);
	Put32(p, 500);
	Put32(p, 200);
	Put32(p, 600);
	Bytes buf = MakeBox("trun", p);
	ASSERT_EQ(buf.size(), 32u);

	auto box = ParseBox(buf);
	auto *trun = dynamic_cast<TrunIsoBmffBox *>(box.get());
	ASSERT_NE(trun, nullptr);

	trun->truncate();
	EXPECT_EQ(trun->getSampleCount(), 1u);
	EXPECT_EQ(ReadBe32(buf, 0), 24u); // new trun size: 32 - 16 + 8
	EXPECT_EQ(ReadBe32(buf, 24), 8u); // skip box size
	EXPECT_TRUE(std::memcmp(&buf[28], "skip", 4) == 0);
}

TEST(IsoBmffBoxParse, Trun_TruncateWithoutRoom_OnlyResetsSampleCount)
{
	// flags 0x000100: sample duration only (4 bytes per sample) - not enough room for a skip box
	Bytes p;
	PutVerFlags(p, 0, 0x000100);
	Put32(p, 2);
	Put32(p, 100);
	Put32(p, 200);
	Bytes buf = MakeBox("trun", p);
	ASSERT_EQ(buf.size(), 24u);

	auto box = ParseBox(buf);
	auto *trun = dynamic_cast<TrunIsoBmffBox *>(box.get());
	ASSERT_NE(trun, nullptr);

	trun->truncate();
	EXPECT_EQ(trun->getSampleCount(), 1u);
	EXPECT_EQ(ReadBe32(buf, 0), 24u); // size unchanged
	EXPECT_EQ(ReadBe32(buf, 12), 1u); // sample count rewritten
}

TEST(IsoBmffBoxParse, Trun_TruncateSingleSample_IsNoOp)
{
	Bytes p;
	PutVerFlags(p, 0, 0x000100);
	Put32(p, 1);
	Put32(p, 100);
	Bytes buf = MakeBox("trun", p);

	auto box = ParseBox(buf);
	auto *trun = dynamic_cast<TrunIsoBmffBox *>(box.get());
	ASSERT_NE(trun, nullptr);

	trun->truncate();
	EXPECT_EQ(trun->getSampleCount(), 1u);
	EXPECT_EQ(ReadBe32(buf, 0), 20u);
}

// ==============================================================================
// tfhd
// ==============================================================================

TEST(IsoBmffBoxParse, Tfhd_AllOptionalFields_ReadsDefaultsAndWritesDuration)
{
	// flags 0x1B: base data offset, sample description index, default duration, default size
	Bytes p;
	PutVerFlags(p, 0, 0x00001B);
	Put32(p, 1);     // track_ID
	Put64(p, 0);     // base_data_offset
	Put32(p, 1);     // sample_description_index
	Put32(p, 1001);  // default_sample_duration
	Put32(p, 512);   // default_sample_size
	Bytes buf = MakeBox("tfhd", p);

	auto box = ParseBox(buf);
	auto *tfhd = dynamic_cast<TfhdIsoBmffBox *>(box.get());
	ASSERT_NE(tfhd, nullptr);
	EXPECT_TRUE(tfhd->defaultSampleDurationPresent());
	EXPECT_EQ(tfhd->getDefaultSampleDuration(), 1001u);
	EXPECT_EQ(tfhd->getDefaultSampleSize(), 512u);

	tfhd->setDefaultSampleDuration(2002);
	EXPECT_EQ(tfhd->getDefaultSampleDuration(), 2002u);
	// default duration: header(8) + version/flags(4) + track_ID(4) + base_data_offset(8) + desc_index(4)
	EXPECT_EQ(ReadBe32(buf, 28), 2002u);
}

TEST(IsoBmffBoxParse, Tfhd_OnlyDefaultSize_ReportsDurationNotPresent)
{
	Bytes p;
	PutVerFlags(p, 0, 0x000010);
	Put32(p, 2);   // track_ID
	Put32(p, 64);  // default_sample_size
	Bytes buf = MakeBox("tfhd", p);

	auto box = ParseBox(buf);
	auto *tfhd = dynamic_cast<TfhdIsoBmffBox *>(box.get());
	ASSERT_NE(tfhd, nullptr);
	EXPECT_FALSE(tfhd->defaultSampleDurationPresent());
	EXPECT_EQ(tfhd->getDefaultSampleDuration(), 0u);
	EXPECT_EQ(tfhd->getDefaultSampleSize(), 64u);

	// No location was recorded for the duration, so only the in-memory value changes
	tfhd->setDefaultSampleDuration(5);
	EXPECT_EQ(tfhd->getDefaultSampleDuration(), 5u);
}

// ==============================================================================
// prft
// ==============================================================================

TEST(IsoBmffBoxParse, Prft_ParsesFieldsAndSettersUpdateValues)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put32(p, 5);
	Put64(p, 0x1111);
	Put64(p, 0x2222);
	Bytes buf = MakeBox("prft", p);

	auto box = ParseBox(buf);
	auto *prft = dynamic_cast<PrftIsoBmffBox *>(box.get());
	ASSERT_NE(prft, nullptr);
	EXPECT_EQ(prft->getTrackId(), 5u);
	EXPECT_EQ(prft->getNtpTs(), 0x1111u);
	EXPECT_EQ(prft->getMediaTime(), 0x2222u);

	prft->setTrackId(6);
	prft->setNtpTs(0x3333);
	prft->setMediaTime(0x4444);
	EXPECT_EQ(prft->getTrackId(), 6u);
	EXPECT_EQ(prft->getNtpTs(), 0x3333u);
	EXPECT_EQ(prft->getMediaTime(), 0x4444u);
}

// ==============================================================================
// sidx
// ==============================================================================

TEST(IsoBmffBoxParse, Sidx_Version0_SumsReferenceDurations)
{
	Bytes buf = MakeBox("sidx", SidxV0Payload(90000, 1000, 2000));
	auto box = ParseBox(buf);
	auto *sidx = dynamic_cast<SidxIsoBmffBox *>(box.get());
	ASSERT_NE(sidx, nullptr);
	EXPECT_EQ(sidx->getTimeScale(), 90000u);
	EXPECT_EQ(sidx->getSampleDuration(), 3000u);

	sidx->setTimeScale(48000);
	EXPECT_EQ(sidx->getTimeScale(), 48000u);
}

TEST(IsoBmffBoxParse, Sidx_Version1_ReadsWideTimesAndDuration)
{
	Bytes buf = MakeBox("sidx", SidxV1Payload(90000, 4000));
	auto box = ParseBox(buf);
	auto *sidx = dynamic_cast<SidxIsoBmffBox *>(box.get());
	ASSERT_NE(sidx, nullptr);
	EXPECT_EQ(sidx->getTimeScale(), 90000u);
	EXPECT_EQ(sidx->getSampleDuration(), 4000u);
}

// ==============================================================================
// mdat
// ==============================================================================

TEST(IsoBmffBoxParse, Mdat_Truncate_RewritesBoxSize)
{
	Bytes buf = MakeBox("mdat", Bytes(24, 0x55));
	auto box = ParseBox(buf);
	auto *mdat = dynamic_cast<MdatIsoBmffBox *>(box.get());
	ASSERT_NE(mdat, nullptr);

	mdat->truncate(16);
	EXPECT_EQ(ReadBe32(buf, 0), 16u);
}

TEST(IsoBmffBoxParse, Box_RewriteAsSkipBox_ChangesTypeInBuffer)
{
	Bytes buf = MakeBox("mdat", Bytes(8, 0x11));
	auto box = ParseBox(buf);
	ASSERT_NE(box, nullptr);
	ASSERT_TRUE(TypeIs(box.get(), "mdat"));

	box->rewriteAsSkipBox();
	EXPECT_TRUE(std::memcmp(&buf[4], "skip", 4) == 0);
	EXPECT_TRUE(TypeIs(box.get(), "skip"));
}

// ==============================================================================
// senc
// ==============================================================================

TEST(IsoBmffBoxParse, Senc_Truncate_KeepsFirstSampleAndInsertsSkipBox)
{
	// version/flags(4) + sample_count(4) + 30 bytes of sample data
	Bytes p;
	PutVerFlags(p, 0, 0x2);
	Put32(p, 3);
	Append(p, Bytes(30, 0x11));
	Bytes buf = MakeBox("senc", p);
	ASSERT_EQ(buf.size(), 46u);

	auto box = ParseBox(buf);
	auto *senc = dynamic_cast<SencIsoBmffBox *>(box.get());
	ASSERT_NE(senc, nullptr);

	// first sample is 10 bytes: new end = sample_count field(12) + 4 + 10 = 26
	senc->truncate(10);
	EXPECT_EQ(ReadBe32(buf, 0), 26u);
	EXPECT_EQ(ReadBe32(buf, 12), 1u);
	EXPECT_EQ(ReadBe32(buf, 26), 20u); // skip box size
	EXPECT_TRUE(std::memcmp(&buf[30], "skip", 4) == 0);
}

TEST(IsoBmffBoxParse, Senc_TruncateWithoutRoom_OnlyResetsSampleCount)
{
	Bytes p;
	PutVerFlags(p, 0, 0x2);
	Put32(p, 2);
	Append(p, Bytes(10, 0x22));
	Bytes buf = MakeBox("senc", p);
	ASSERT_EQ(buf.size(), 26u);

	auto box = ParseBox(buf);
	auto *senc = dynamic_cast<SencIsoBmffBox *>(box.get());
	ASSERT_NE(senc, nullptr);

	// new end = 12 + 4 + 4 = 20, leaving only 6 bytes (< 8 needed for a skip box)
	senc->truncate(4);
	EXPECT_EQ(ReadBe32(buf, 0), 26u);
	EXPECT_EQ(ReadBe32(buf, 12), 1u);
}

TEST(IsoBmffBoxParse, Senc_TruncateOutOfBounds_LeavesSizeUnchanged)
{
	Bytes p;
	PutVerFlags(p, 0, 0x2);
	Put32(p, 2);
	Append(p, Bytes(10, 0x22));
	Bytes buf = MakeBox("senc", p);

	auto box = ParseBox(buf);
	auto *senc = dynamic_cast<SencIsoBmffBox *>(box.get());
	ASSERT_NE(senc, nullptr);

	senc->truncate(100);
	EXPECT_EQ(ReadBe32(buf, 0), 26u);
	EXPECT_EQ(ReadBe32(buf, 12), 1u);
}

TEST(IsoBmffBoxParse, Senc_SingleSampleOrZeroFirstSize_DoesNotResizeBox)
{
	// Single sample: truncate returns early, count untouched
	Bytes p1;
	PutVerFlags(p1, 0, 0x2);
	Put32(p1, 1);
	Append(p1, Bytes(10, 0x33));
	Bytes buf1 = MakeBox("senc", p1);
	auto box1 = ParseBox(buf1);
	static_cast<SencIsoBmffBox *>(box1.get())->truncate(5);
	EXPECT_EQ(ReadBe32(buf1, 12), 1u);
	EXPECT_EQ(ReadBe32(buf1, 0), buf1.size());

	// Multiple samples but zero first-sample size: count is reset, box is not resized
	Bytes p2;
	PutVerFlags(p2, 0, 0x2);
	Put32(p2, 3);
	Append(p2, Bytes(30, 0x33));
	Bytes buf2 = MakeBox("senc", p2);
	auto box2 = ParseBox(buf2);
	static_cast<SencIsoBmffBox *>(box2.get())->truncate(0);
	EXPECT_EQ(ReadBe32(buf2, 12), 1u);
	EXPECT_EQ(ReadBe32(buf2, 0), buf2.size());
}

// ==============================================================================
// saiz
// ==============================================================================

TEST(IsoBmffBoxParse, Saiz_DefaultSampleInfoSize_ReportsFirstSampleSize)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put8(p, 8);  // default_sample_info_size
	Put32(p, 4); // sample_count
	Bytes buf = MakeBox("saiz", p);

	auto box = ParseBox(buf);
	auto *saiz = dynamic_cast<SaizIsoBmffBox *>(box.get());
	ASSERT_NE(saiz, nullptr);
	EXPECT_EQ(saiz->getFirstSampleInfoSize(), 8u);
}

TEST(IsoBmffBoxParse, Saiz_PerSampleTable_ReportsFirstEntry)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put8(p, 0);  // default_sample_info_size = 0 => per-sample table follows
	Put32(p, 3); // sample_count
	Put8(p, 5);
	Put8(p, 6);
	Put8(p, 7);
	Bytes buf = MakeBox("saiz", p);

	auto box = ParseBox(buf);
	auto *saiz = dynamic_cast<SaizIsoBmffBox *>(box.get());
	ASSERT_NE(saiz, nullptr);
	EXPECT_EQ(saiz->getFirstSampleInfoSize(), 5u);
}

TEST(IsoBmffBoxParse, Saiz_AuxInfoTypePresent_SkipsAuxFields)
{
	Bytes p;
	PutVerFlags(p, 0, 0x000001); // SAIZ_FLAG_AUX_INFO_TYPE_PRESENT
	Put32(p, 0x63656E63);        // aux_info_type 'cenc'
	Put32(p, 0);                 // aux_info_type_parameter
	Put8(p, 4);                  // default_sample_info_size
	Put32(p, 2);                 // sample_count
	Bytes buf = MakeBox("saiz", p);

	auto box = ParseBox(buf);
	auto *saiz = dynamic_cast<SaizIsoBmffBox *>(box.get());
	ASSERT_NE(saiz, nullptr);
	EXPECT_EQ(saiz->getFirstSampleInfoSize(), 4u);
}

TEST(IsoBmffBoxParse, Saiz_Truncate_WithRoom_ShrinksBoxAndInsertsSkip)
{
	// default size 0 with 10 per-sample entries: box is 27 bytes, sample count at offset 13
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put8(p, 0);
	Put32(p, 10);
	for (int i = 0; i < 10; ++i)
	{
		Put8(p, 4);
	}
	Bytes buf = MakeBox("saiz", p);
	ASSERT_EQ(buf.size(), 27u);

	auto box = ParseBox(buf);
	auto *saiz = dynamic_cast<SaizIsoBmffBox *>(box.get());
	ASSERT_NE(saiz, nullptr);

	saiz->truncate();
	// new size = 27 - (10 - 1) = 18; skip box of size 9 at offset 18
	EXPECT_EQ(ReadBe32(buf, 0), 18u);
	EXPECT_EQ(ReadBe32(buf, 18), 9u);
	EXPECT_TRUE(std::memcmp(&buf[22], "skip", 4) == 0);
	EXPECT_EQ(ReadBe32(buf, 13), 1u);
}

TEST(IsoBmffBoxParse, Saiz_Truncate_WithoutRoom_OnlyResetsSampleCount)
{
	Bytes p;
	PutVerFlags(p, 0, 0);
	Put8(p, 4);  // default size, no per-sample table
	Put32(p, 3); // sample_count => only 2 bytes would be trimmed
	Bytes buf = MakeBox("saiz", p);
	ASSERT_EQ(buf.size(), 17u);

	auto box = ParseBox(buf);
	auto *saiz = dynamic_cast<SaizIsoBmffBox *>(box.get());
	ASSERT_NE(saiz, nullptr);

	saiz->truncate();
	EXPECT_EQ(ReadBe32(buf, 0), 17u);
	EXPECT_EQ(ReadBe32(buf, 13), 1u);
}
