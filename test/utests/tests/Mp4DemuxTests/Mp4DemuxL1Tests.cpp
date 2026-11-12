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

/**
 * @file Mp4DemuxL1Tests.cpp
 * @brief Dedicated L1 suite for the header-only ISO-BMFF fragment parser
 *        mp4demux/mp4demux.hpp (class Mp4Demux, struct Mp4Sample,
 *        Mp4Demux::AdjustMediaDecodeTime).
 *
 *        This suite is intentionally hermetic: it links the real GStreamer /
 *        GLib libraries and calls gst_init() once, so that boxes produce real
 *        GstBuffer / GstEvent / GstCaps / GstStructure objects whose contents
 *        can be verified.  The only symbol stubbed is gst_app_src_set_caps(),
 *        which requires a live GstAppSrc element; the stub captures the caps
 *        produced by Mp4Demux::setCaps() for inspection.
 *
 *        The parser asserts on malformed input (assert()), so only well
 *        formed box trees are fed to it.
 */

#include <gtest/gtest.h>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>
#include <cstdlib>
#include <tuple>

#include "mp4demux/mp4demux.hpp"

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/* ------------------------------------------------------------------ */
/* Capture hook for the only GStreamer call that needs a live element  */
/* ------------------------------------------------------------------ */

static GstCaps *gCapturedCaps = nullptr;
static int gSetCapsCallCount = 0;

void gst_app_src_set_caps(GstAppSrc *appsrc, const GstCaps *caps)
{
	(void)appsrc;
	if (gCapturedCaps)
	{
		gst_caps_unref(gCapturedCaps);
	}
	gCapturedCaps = gst_caps_ref(const_cast<GstCaps *>(caps));
	gSetCapsCallCount++;
}

/* ------------------------------------------------------------------ */
/* Byte stream helpers                                                  */
/* ------------------------------------------------------------------ */

static void U8(std::vector<uint8_t> &v, uint8_t x)
{
	v.push_back(x);
}

static void U16(std::vector<uint8_t> &v, uint16_t x)
{
	v.push_back((x >> 8) & 0xff);
	v.push_back(x & 0xff);
}

static void U24(std::vector<uint8_t> &v, uint32_t x)
{
	v.push_back((x >> 16) & 0xff);
	v.push_back((x >> 8) & 0xff);
	v.push_back(x & 0xff);
}

static void U32(std::vector<uint8_t> &v, uint32_t x)
{
	v.push_back((x >> 24) & 0xff);
	v.push_back((x >> 16) & 0xff);
	v.push_back((x >> 8) & 0xff);
	v.push_back(x & 0xff);
}

static void U64(std::vector<uint8_t> &v, uint64_t x)
{
	for (int i = 7; i >= 0; i--)
	{
		v.push_back((x >> (i * 8)) & 0xff);
	}
}

static void FourCC(std::vector<uint8_t> &v, const char *cc)
{
	for (int i = 0; i < 4; i++)
	{
		v.push_back((uint8_t)cc[i]);
	}
}

static void Bytes(std::vector<uint8_t> &v, const uint8_t *data, size_t len)
{
	v.insert(v.end(), data, data + len);
}

static void Zeroes(std::vector<uint8_t> &v, size_t n)
{
	v.insert(v.end(), n, 0);
}

/**
 * @brief Open a box header: size(4B, includes header) + fourcc(4B).
 *        Caller appends exactly payloadLen payload bytes.
 */
static void BoxHeader(std::vector<uint8_t> &v, const char *type, size_t payloadLen)
{
	U32(v, (uint32_t)(8 + payloadLen));
	FourCC(v, type);
}

/** @brief Full-box header: version(1B) + flags(3B) */
static void FullBoxHeader(std::vector<uint8_t> &v, uint8_t version, uint32_t flags)
{
	U8(v, version);
	U24(v, flags);
}

/* ------------------------------------------------------------------ */
/* Box builders                                                         */
/* ------------------------------------------------------------------ */

static std::vector<uint8_t> Box(const char *type, const std::vector<uint8_t> &payload)
{
	std::vector<uint8_t> out;
	BoxHeader(out, type, payload.size());
	out.insert(out.end(), payload.begin(), payload.end());
	return out;
}

static std::vector<uint8_t> Concat(std::initializer_list<std::vector<uint8_t>> parts)
{
	std::vector<uint8_t> out;
	for (auto &p : parts)
	{
		out.insert(out.end(), p.begin(), p.end());
	}
	return out;
}

/** @brief mvhd v0: creation,modification,timescale,duration,rate,volume + fixed matrix */
static std::vector<uint8_t> MvhdPayload(uint8_t version, uint32_t timescale, uint32_t duration)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, version, 0);
	if (version == 1)
	{
		U64(p, 1); // creation_time
		U64(p, 2); // modification_time
	}
	else
	{
		U32(p, 1);
		U32(p, 2);
	}
	U32(p, timescale);
	U32(p, duration);
	U32(p, 0x00010000); // rate = 1.0
	U32(p, 0x00000100); // volume = 1.0
	Zeroes(p, 8);       // reserved
	for (int i = 0; i < 9; i++)
	{
		U32(p, (i % 4 == 0) ? 0x00010000 : 0); // identity-ish matrix
	}
	Zeroes(p, 24); // pre_defined
	U32(p, 2);     // next_track_id
	return p;
}

/** @brief mdhd v0: creation,mod,timescale,duration,language */
static std::vector<uint8_t> MdhdPayload(uint8_t version, uint32_t timescale, uint32_t duration)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, version, 0);
	if (version == 1)
	{
		U64(p, 0);
		U64(p, 0);
	}
	else
	{
		U32(p, 0);
		U32(p, 0);
	}
	U32(p, timescale);
	U32(p, duration);
	U16(p, 0x55c4); // 'und' packed language
	U16(p, 0);      // pre_defined
	return p;
}

/** @brief tkhd v0 with track id / width / height */
static std::vector<uint8_t> TkhdPayload(uint32_t trackId, uint32_t width, uint32_t height)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0x000007); // enabled|in_movie|in_preview
	U32(p, 0);                     // creation_time
	U32(p, 0);                     // modification_time
	U32(p, trackId);
	Zeroes(p, 4 + 4 + 8 + 2 + 2 + 2 + 2); // reserved, duration, reserved, layer, alt_group, volume, reserved
	for (int i = 0; i < 9; i++)
	{
		U32(p, 0);
	}
	U32(p, width);  // width fixed-point 16.16
	U32(p, height); // height fixed-point 16.16
	return p;
}

/** @brief trex: track_id, desc idx, default dur/size/flags */
static std::vector<uint8_t> TrexPayload(uint32_t trackId, uint32_t dur, uint32_t size, uint32_t flags)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	U32(p, trackId);
	U32(p, 1); // default_sample_description_index
	U32(p, dur);
	U32(p, size);
	U32(p, flags);
	return p;
}

/** @brief mehd v0: fragment_duration */
static std::vector<uint8_t> MehdPayload(uint32_t fragDuration)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	U32(p, fragDuration);
	return p;
}

/** @brief video sample entry body (inside avc1/hev1/hvc1/encv box), 78 bytes */
static std::vector<uint8_t> VideoSampleEntryBody(uint16_t width, uint16_t height)
{
	std::vector<uint8_t> p;
	Zeroes(p, 4);    // reserved
	U32(p, 1);       // data_reference_index (parser reads U32)
	Zeroes(p, 16);   // pre_defined + reserved
	U16(p, width);
	U16(p, height);
	U32(p, 0x00480000); // horizresolution
	U32(p, 0x00480000); // vertresolution
	Zeroes(p, 4);       // reserved
	U16(p, 1);          // frame_count
	Zeroes(p, 32);      // compressor_name
	U16(p, 0x0018);     // depth
	U16(p, 0xffff);     // pad - parser asserts this
	return p;
}

/** @brief audio sample entry body (inside mp4a/ec-3/enca box), 28 bytes */
static std::vector<uint8_t> AudioSampleEntryBody(uint16_t channels, uint16_t samplerate)
{
	std::vector<uint8_t> p;
	Zeroes(p, 4);  // reserved
	U32(p, 1);     // data_reference_index
	Zeroes(p, 8);  // reserved
	U16(p, channels);
	U16(p, 16);        // samplesize
	Zeroes(p, 4);      // pre_defined + reserved
	U16(p, samplerate);
	U16(p, 0); // fixed point fraction
	return p;
}

/** @brief stsd box containing a single sample entry box */
static std::vector<uint8_t> Stsd(const std::vector<uint8_t> &sampleEntry)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	U32(p, 1); // entry_count == 1 asserted by parser
	p.insert(p.end(), sampleEntry.begin(), sampleEntry.end());
	return Box("stsd", p);
}

/** @brief generic codec config box (avcC/hvcC/dec3) with opaque payload */
static std::vector<uint8_t> CodecConfigBox(const char *type, const std::vector<uint8_t> &data)
{
	return Box(type, data);
}

/** @brief esds box with a well-formed descriptor chain carrying codec_data */
static std::vector<uint8_t> EsdsBox(const std::vector<uint8_t> &decSpecificInfo)
{
	std::vector<uint8_t> decSpecific; // tag 0x05
	U8(decSpecific, 0x05);
	U8(decSpecific, (uint8_t)decSpecificInfo.size());
	decSpecific.insert(decSpecific.end(), decSpecificInfo.begin(), decSpecificInfo.end());

	std::vector<uint8_t> slConfig; // tag 0x06
	U8(slConfig, 0x06);
	U8(slConfig, 1);
	U8(slConfig, 0x02); // predefined

	std::vector<uint8_t> decConfig; // tag 0x04
	U8(decConfig, 0x04);
	size_t decConfigLen = 13 + decSpecific.size() + slConfig.size();
	U8(decConfig, (uint8_t)decConfigLen);
	U8(decConfig, 0x40);        // object_type_id (AAC LC)
	U8(decConfig, 0x15);        // stream_type
	U8(decConfig, 0x00);        // upStream
	U16(decConfig, 0);          // buffer_size
	U32(decConfig, 128000);     // maxBitrate
	U32(decConfig, 96000);      // avgBitrate
	decConfig.insert(decConfig.end(), decSpecific.begin(), decSpecific.end());
	decConfig.insert(decConfig.end(), slConfig.begin(), slConfig.end());

	std::vector<uint8_t> esDescriptor; // tag 0x03
	U8(esDescriptor, 0x03);
	size_t esLen = 3 + decConfig.size();
	U8(esDescriptor, (uint8_t)esLen);
	U16(esDescriptor, 1); // ES_ID
	U8(esDescriptor, 0);  // flags
	esDescriptor.insert(esDescriptor.end(), decConfig.begin(), decConfig.end());

	std::vector<uint8_t> payload;
	U32(payload, 0); // version + flags (skipped by parser)
	payload.insert(payload.end(), esDescriptor.begin(), esDescriptor.end());
	return Box("esds", payload);
}

/* ---- protection boxes ---- */

static std::vector<uint8_t> Frma(const char *origFormat)
{
	std::vector<uint8_t> p;
	FourCC(p, origFormat);
	return Box("frma", p);
}

static std::vector<uint8_t> Schm(const char *scheme, uint32_t version)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	FourCC(p, scheme);
	U32(p, version);
	return Box("schm", p);
}

/**
 * @brief tenc box. For cbcs appends constant_iv_size + constant_iv.
 */
static std::vector<uint8_t> Tenc(const char *scheme, uint8_t pattern, uint8_t isEncrypted,
								 uint8_t ivSize, const uint8_t kid[16], const std::vector<uint8_t> &constIv = {})
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	U8(p, 0);       // reserved
	U8(p, pattern); // crypt/skip nibble pattern
	U8(p, isEncrypted);
	U8(p, ivSize);
	Bytes(p, kid, 16);
	if (0 == strcmp(scheme, "cbcs"))
	{
		U8(p, (uint8_t)constIv.size());
		p.insert(p.end(), constIv.begin(), constIv.end());
	}
	return Box("tenc", p);
}

static std::vector<uint8_t> Pssh(const uint8_t systemId[16], const std::vector<uint8_t> &data)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	Bytes(p, systemId, 16);
	p.insert(p.end(), data.begin(), data.end());
	return Box("pssh", p);
}

/* ---- fragment boxes ---- */

static std::vector<uint8_t> Mfhd(uint32_t seq)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0);
	U32(p, seq);
	return Box("mfhd", p);
}

static std::vector<uint8_t> Tfhd(uint32_t trackId, uint32_t flags,
								 uint32_t defDur = 0, uint32_t defSize = 0)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, flags);
	U32(p, trackId);
	if (flags & 0x00001)
	{
		U64(p, 0); // base_data_offset
	}
	if (flags & 0x00002)
	{
		U32(p, 1); // default_sample_description_index
	}
	if (flags & 0x00008)
	{
		U32(p, defDur);
	}
	if (flags & 0x00010)
	{
		U32(p, defSize);
	}
	if (flags & 0x00020)
	{
		U32(p, 0); // default_sample_flags
	}
	return Box("tfhd", p);
}

static std::vector<uint8_t> Tfdt(uint8_t version, uint64_t decodeTime)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, version, 0);
	if (version == 1)
	{
		U64(p, decodeTime);
	}
	else
	{
		U32(p, (uint32_t)decodeTime);
	}
	return Box("tfdt", p);
}

/**
 * @brief trun box; flags must include 0x0001 (data_offset present).
 *        Optional per-sample fields driven by flags 0x100/0x200/0x400/0x800.
 */
static std::vector<uint8_t> Trun(uint32_t flags, int32_t dataOffset,
								 const std::vector<std::tuple<uint32_t, uint32_t, int32_t>> &samples)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, flags);
	U32(p, (uint32_t)samples.size());
	if (flags & 0x0001)
	{
		U32(p, (uint32_t)dataOffset); // signed, but emitted as bytes
	}
	if (flags & 0x0004)
	{
		U32(p, 0x02000000); // first_sample_flags: keyframe
	}
	for (auto &s : samples)
	{
		if (flags & 0x0100)
		{
			U32(p, std::get<0>(s));
		}
		if (flags & 0x0200)
		{
			U32(p, std::get<1>(s));
		}
		if (flags & 0x0400)
		{
			U32(p, 0); // per-sample flags
		}
		if (flags & 0x0800)
		{
			U32(p, (uint32_t)std::get<2>(s));
		}
	}
	return Box("trun", p);
}

static std::vector<uint8_t> Saiz(uint8_t defaultInfoSize, uint32_t sampleCount,
								 const std::vector<uint8_t> &perSampleSizes = {})
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, 0); // flags=0: no aux_info_type
	U8(p, defaultInfoSize);
	U32(p, sampleCount);
	if (defaultInfoSize == 0)
	{
		p.insert(p.end(), perSampleSizes.begin(), perSampleSizes.end());
	}
	return Box("saiz", p);
}

static std::vector<uint8_t> Saio(uint8_t version, uint64_t auxOffset)
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, version, 0);
	U32(p, 1); // entry_count == 1 asserted by parser
	if (version == 0)
	{
		U32(p, (uint32_t)auxOffset);
	}
	else
	{
		U64(p, auxOffset);
	}
	return Box("saio", p);
}

static std::vector<uint8_t> Senc(uint32_t flags, uint32_t sampleCount,
								 const std::vector<std::vector<uint8_t>> &ivs,
								 const std::vector<std::vector<uint8_t>> &subsampleData = {})
{
	std::vector<uint8_t> p;
	FullBoxHeader(p, 0, flags);
	U32(p, sampleCount);
	for (uint32_t i = 0; i < sampleCount; i++)
	{
		p.insert(p.end(), ivs[i].begin(), ivs[i].end());
		if (flags & 0x02)
		{
			// subsample data: count + pairs of (clearU16, encU32)
			U16(p, (uint16_t)(subsampleData[i].size() / 6));
			p.insert(p.end(), subsampleData[i].begin(), subsampleData[i].end());
		}
	}
	return Box("senc", p);
}

/* ---- composite trees ---- */

/** @brief moov containing mvhd only */
static std::vector<uint8_t> MoovWithMvhd(uint8_t version, uint32_t timescale, uint32_t duration)
{
	return Box("moov", Box("mvhd", MvhdPayload(version, timescale, duration)));
}

/**
 * @brief init segment: moov{mvhd + trak{tkhd + mdia{mdhd + minf{stbl{stsd}}}}} + mvex{trex}
 */
static std::vector<uint8_t> InitSegment(uint32_t timescale,
									  const std::vector<uint8_t> &stsdBox,
									  uint32_t trexDur = 0, uint32_t trexSize = 0)
{
	std::vector<uint8_t> stbl = Box("stbl", stsdBox);
	std::vector<uint8_t> minf = Box("minf", stbl);
	std::vector<uint8_t> mdia = Box("mdia", Concat({Box("mdhd", MdhdPayload(0, timescale, 0)), minf}));
	std::vector<uint8_t> trak = Box("trak", Concat({Box("tkhd", TkhdPayload(1, 1280 << 16, 720 << 16)), mdia}));

	std::vector<uint8_t> mvexChildren = Box("trex", TrexPayload(1, trexDur, trexSize, 0));
	std::vector<uint8_t> mvex = Box("mvex", mvexChildren);

	return Box("moov", Concat({Box("mvhd", MvhdPayload(0, timescale, 0)), trak, mvex}));
}

/**
 * @brief media segment: moof{mfhd + traf{children}} followed by an mdat box.
 *        The mdat box wrapper is required: raw trailing bytes would be parsed
 *        as a box header (size 0 would walk the parser backwards).
 *        trun data_offset is relative to the start of the moof box, so sample
 *        data inside mdat lives at offset == moof.size() + 8 (mdat header).
 */
static std::vector<uint8_t> Segment(const std::vector<uint8_t> &trafChildren, size_t mdatPayloadLen,
									size_t *moofLenOut = nullptr)
{
	std::vector<uint8_t> traf = Box("traf", trafChildren);
	std::vector<uint8_t> moof = Box("moof", Concat({Mfhd(1), traf}));
	if (moofLenOut)
	{
		*moofLenOut = moof.size();
	}
	std::vector<uint8_t> seg = moof;
	std::vector<uint8_t> payload(mdatPayloadLen, 0);
	auto mdat = Box("mdat", payload);
	seg.insert(seg.end(), mdat.begin(), mdat.end());
	return seg;
}

/* ------------------------------------------------------------------ */
/* Fixture                                                              */
/* ------------------------------------------------------------------ */

class Mp4DemuxL1Tests : public ::testing::Test
{
protected:
	Mp4Demux demux;

	static void SetUpTestSuite()
	{
		gst_init(nullptr, nullptr); // real GStreamer object model
	}

	void TearDown() override
	{
		if (gCapturedCaps)
		{
			gst_caps_unref(gCapturedCaps);
		}
		gCapturedCaps = nullptr;
		gSetCapsCallCount = 0;
	}

	static std::vector<uint8_t> Kid()
	{
		return {0xed, 0xef, 0x8b, 0xa9, 0x79, 0xd6, 0x4a, 0xce,
				0xa3, 0xc8, 0x27, 0xdc, 0xd5, 0x1d, 0x21, 0xed};
	}
};

/* ================================================================== */
/* Constructor / empty-parse contract                                   */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, Constructor_DefaultState)
{
	EXPECT_EQ(demux.count(), 0);
	EXPECT_EQ(demux.getTimeScale(), 0u);
	EXPECT_EQ(demux.getNumProtectionEvents(), 0u);
}

TEST_F(Mp4DemuxL1Tests, Parse_NullPointerIsNoOp)
{
	EXPECT_NO_FATAL_FAILURE(demux.Parse(nullptr, 128));
	EXPECT_EQ(demux.count(), 0);
}

TEST_F(Mp4DemuxL1Tests, Parse_ZeroLength)
{
	uint8_t buf[8] = {0};
	EXPECT_NO_FATAL_FAILURE(demux.Parse(buf, 0));
	EXPECT_EQ(demux.count(), 0);
}

TEST_F(Mp4DemuxL1Tests, Parse_UnknownBoxSkipped)
{
	auto data = Box("zzzz", {0x01, 0x02, 0x03, 0x04});
	EXPECT_NO_FATAL_FAILURE(demux.Parse(data.data(), data.size()));
	EXPECT_EQ(demux.count(), 0);
}

TEST_F(Mp4DemuxL1Tests, Parse_FtypSkipped)
{
	std::vector<uint8_t> ftyp;
	FourCC(ftyp, "isom");
	U32(ftyp, 0);
	FourCC(ftyp, "iso6");
	FourCC(ftyp, "mp41");
	auto box = Box("ftyp", ftyp);
	EXPECT_NO_FATAL_FAILURE(demux.Parse(box.data(), box.size()));
	EXPECT_EQ(demux.count(), 0);
}

/* ================================================================== */
/* Movie / media / track header parsing                                 */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, Parse_MvhdV0_Timescale)
{
	auto data = MoovWithMvhd(0, 90000, 180000);
	demux.Parse(data.data(), data.size());
	EXPECT_EQ(demux.getTimeScale(), 90000u);
}

TEST_F(Mp4DemuxL1Tests, Parse_MvhdV1_Timescale)
{
	auto data = MoovWithMvhd(1, 48000, 0);
	demux.Parse(data.data(), data.size());
	EXPECT_EQ(demux.getTimeScale(), 48000u);
}

TEST_F(Mp4DemuxL1Tests, Parse_MdhdV0_Timescale)
{
	std::vector<uint8_t> mdia = Box("mdia", Box("mdhd", MdhdPayload(0, 44100, 0)));
	std::vector<uint8_t> trak = Box("trak", mdia);
	auto data = Box("moov", trak);
	demux.Parse(data.data(), data.size());
	EXPECT_EQ(demux.getTimeScale(), 44100u);
}

TEST_F(Mp4DemuxL1Tests, Parse_MdhdV1_Timescale)
{
	std::vector<uint8_t> mdia = Box("mdia", Box("mdhd", MdhdPayload(1, 96000, 0)));
	auto data = Box("moov", Box("trak", mdia));
	demux.Parse(data.data(), data.size());
	EXPECT_EQ(demux.getTimeScale(), 96000u);
}

TEST_F(Mp4DemuxL1Tests, Parse_Tkhd_NoCrashNoSamples)
{
	auto data = Box("moov", Box("trak", Box("tkhd", TkhdPayload(7, 1920 << 16, 1080 << 16))));
	EXPECT_NO_FATAL_FAILURE(demux.Parse(data.data(), data.size()));
	EXPECT_EQ(demux.count(), 0);
}

TEST_F(Mp4DemuxL1Tests, Parse_Mehd_Smoke)
{
	std::vector<uint8_t> mvex = Box("mvex", Box("mehd", MehdPayload(960960)));
	auto data = Box("moov", mvex);
	EXPECT_NO_FATAL_FAILURE(demux.Parse(data.data(), data.size()));
}

TEST_F(Mp4DemuxL1Tests, Parse_NestedContainersTraverse)
{
	// moov->trak->mdia->minf->stbl chain all recurse via DemuxHelper
	auto stsd = Stsd(Concat({Box("avc1", Concat({VideoSampleEntryBody(640, 480),
											   CodecConfigBox("avcC", {1, 42, 0, 31})}))}));
	auto data = InitSegment(90000, stsd);
	EXPECT_NO_FATAL_FAILURE(demux.Parse(data.data(), data.size()));
}

/* ================================================================== */
/* stsd / codec configuration -> setCaps                                */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, SetCaps_Avc1AvcC_ProducesH264Caps)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(1280, 720),
										 CodecConfigBox("avcC", {0x01, 0x64, 0x00, 0x2a, 0xff, 0xe0})})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());

	GstAppSrc *appsrc = nullptr;
	demux.setCaps(appsrc);
	ASSERT_EQ(gSetCapsCallCount, 1);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GstStructure *s = gst_caps_get_structure(gCapturedCaps, 0);
	ASSERT_NE(s, nullptr);
	EXPECT_STREQ(gst_structure_get_name(s), "video/x-h264");
	EXPECT_STREQ(gst_structure_get_string(s, "stream-format"), "avc");
	EXPECT_STREQ(gst_structure_get_string(s, "alignment"), "au");
	gint w = 0, h = 0;
	EXPECT_TRUE(gst_structure_get_int(s, "width", &w));
	EXPECT_TRUE(gst_structure_get_int(s, "height", &h));
	EXPECT_EQ(w, 1280);
	EXPECT_EQ(h, 720);
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Avc1_CodecDataPresent)
{
	std::vector<uint8_t> avcc = {0x01, 0x64, 0x00, 0x2a};
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(320, 240), CodecConfigBox("avcC", avcc)})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GValue *v = gst_structure_get_value(gst_caps_get_structure(gCapturedCaps, 0), "codec_data");
	ASSERT_NE(v, nullptr);
	GstBuffer *buf = gst_value_get_buffer(v);
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(gst_buffer_get_size(buf), avcc.size());
	guint8 out[16] = {0};
	gst_buffer_extract(buf, 0, out, avcc.size());
	EXPECT_EQ(0, memcmp(out, avcc.data(), avcc.size()));
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Hvc1HvcC_ProducesH265Caps)
{
	auto stsd = Stsd(Box("hvc1", Concat({VideoSampleEntryBody(3840, 2160),
										 CodecConfigBox("hvcC", {0x01, 0x01, 0x60})})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	EXPECT_STREQ(gst_structure_get_name(gst_caps_get_structure(gCapturedCaps, 0)), "video/x-h265");
	EXPECT_STREQ(gst_structure_get_string(gst_caps_get_structure(gCapturedCaps, 0), "stream-format"), "hvc1");
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Hev1HvcC_ProducesH265Caps)
{
	auto stsd = Stsd(Box("hev1", Concat({VideoSampleEntryBody(1920, 1080),
										 CodecConfigBox("hvcC", {0x01})})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	EXPECT_STREQ(gst_structure_get_name(gst_caps_get_structure(gCapturedCaps, 0)), "video/x-h265");
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Mp4aEsds_ProducesAudioMpegCaps)
{
	auto stsd = Stsd(Box("mp4a", Concat({AudioSampleEntryBody(2, 48000),
										 EsdsBox({0x11, 0x90})})));
	auto init = InitSegment(48000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GstStructure *s = gst_caps_get_structure(gCapturedCaps, 0);
	EXPECT_STREQ(gst_structure_get_name(s), "audio/mpeg");
	gint mpeg = 0;
	EXPECT_TRUE(gst_structure_get_int(s, "mpegversion", &mpeg));
	EXPECT_EQ(mpeg, 4);
	gboolean framed = FALSE;
	EXPECT_TRUE(gst_structure_get_boolean(s, "framed", &framed));
	EXPECT_TRUE(framed);
	EXPECT_STREQ(gst_structure_get_string(s, "stream-format"), "raw");
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Mp4aEsds_CodecDataExtracted)
{
	std::vector<uint8_t> dsi = {0x11, 0x90, 0x56, 0xe5, 0x00};
	auto stsd = Stsd(Box("mp4a", Concat({AudioSampleEntryBody(2, 44100), EsdsBox(dsi)})));
	auto init = InitSegment(44100, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GValue *v = gst_structure_get_value(gst_caps_get_structure(gCapturedCaps, 0), "codec_data");
	ASSERT_NE(v, nullptr);
	GstBuffer *buf = gst_value_get_buffer(v);
	ASSERT_NE(buf, nullptr);
	EXPECT_EQ(gst_buffer_get_size(buf), dsi.size());
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Ec3Dec3_ProducesEac3Caps)
{
	auto stsd = Stsd(Box("ec-3", Concat({AudioSampleEntryBody(6, 48000),
										 CodecConfigBox("dec3", {0x0b, 0x77})})));
	auto init = InitSegment(48000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GstStructure *s = gst_caps_get_structure(gCapturedCaps, 0);
	EXPECT_STREQ(gst_structure_get_name(s), "audio/x-eac3");
	gint rate = 0, ch = 0;
	EXPECT_TRUE(gst_structure_get_int(s, "rate", &rate));
	EXPECT_TRUE(gst_structure_get_int(s, "channels", &ch));
	EXPECT_EQ(rate, 48000);
	EXPECT_EQ(ch, 6);
}

TEST_F(Mp4DemuxL1Tests, SetCaps_UnknownCodecType_NoCapsSet)
{
	// codec_type never set (no stsd parse) -> default branch prints and returns
	EXPECT_NO_FATAL_FAILURE(demux.setCaps(nullptr));
	EXPECT_EQ(gSetCapsCallCount, 0);
	EXPECT_EQ(gCapturedCaps, nullptr);
}

/* ================================================================== */
/* Encryption: encv/enca + sinf{frma,schm,schi{tenc}}                   */
/* ================================================================== */

static std::vector<uint8_t> EncVideoEntry(const char *scheme, uint8_t pattern, uint8_t ivSize,
										const uint8_t kid[16], const std::vector<uint8_t> &constIv = {})
{
	auto sinf = Box("sinf", Concat({Frma("avc1"), Schm(scheme, 0x00010000),
									Box("schi", Tenc(scheme, pattern, 1, ivSize, kid, constIv))}));
	return Box("encv", Concat({VideoSampleEntryBody(1280, 720), CodecConfigBox("avcC", {0x01}), sinf}));
}

TEST_F(Mp4DemuxL1Tests, Parse_EncvCenc_InternallyEncrypted)
{
	uint8_t kid[16] = {0};
	memcpy(kid, Kid().data(), 16);
	auto stsd = Stsd(EncVideoEntry("cenc", 0, 8, kid));
	auto init = InitSegment(90000, stsd);
	EXPECT_NO_FATAL_FAILURE(demux.Parse(init.data(), init.size()));
}

TEST_F(Mp4DemuxL1Tests, SetCaps_Encrypted_RenamesToCenc)
{
	uint8_t kid[16] = {0};
	memcpy(kid, Kid().data(), 16);
	auto stsd = Stsd(EncVideoEntry("cenc", 0, 8, kid));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	const GstStructure *s = gst_caps_get_structure(gCapturedCaps, 0);
	// is_encrypted path: original media type preserved, structure renamed
	EXPECT_STREQ(gst_structure_get_name(s), "application/x-cenc");
	EXPECT_STREQ(gst_structure_get_string(s, "original-media-type"), "video/x-h264");
	EXPECT_STREQ(gst_structure_get_string(s, GST_PROTECTION_SYSTEM_ID_CAPS_FIELD),
				 "edef8ba9-79d6-4ace-a3c8-27dcd51d21ed");
}

TEST_F(Mp4DemuxL1Tests, GetDrmMetadata_NotEncrypted_ReturnsNull)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());
	EXPECT_EQ(demux.getDrmMetadata(0), nullptr);
}

/* ================================================================== */
/* pssh protection events                                               */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, Parse_Pssh_CreatesProtectionEvent)
{
	uint8_t systemId[16];
	memcpy(systemId, Kid().data(), 16);
	auto pssh = Pssh(systemId, {0x01, 0x02, 0x03, 0x04});
	auto moov = Box("moov", pssh);
	demux.Parse(moov.data(), moov.size());
	ASSERT_EQ(demux.getNumProtectionEvents(), 1u);
	GstEvent *evt = demux.getProtectionEvent(0);
	ASSERT_NE(evt, nullptr);
	const gchar *sysId = nullptr;
	GstBuffer *dataBuf = nullptr;
	const gchar *origin = nullptr;
	gst_event_parse_protection(evt, &sysId, &dataBuf, &origin);
	(void)dataBuf;
	ASSERT_NE(sysId, nullptr);
	EXPECT_STREQ(sysId, "edef8ba9-79d6-4ace-a3c8-27dcd51d21ed");
}

TEST_F(Mp4DemuxL1Tests, Parse_TwoPsshEventsAccumulate)
{
	uint8_t sysA[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t sysB[16] = {0xaa,0xbb,0xcc,0xdd,0xee,0xff,0,1,2,3,4,5,6,7,8,9};
	auto moov = Box("moov", Concat({Pssh(sysA, {1}), Pssh(sysB, {2})}));
	demux.Parse(moov.data(), moov.size());
	EXPECT_EQ(demux.getNumProtectionEvents(), 2u);
	EXPECT_NE(demux.getProtectionEvent(0), nullptr);
	EXPECT_NE(demux.getProtectionEvent(1), nullptr);
}

TEST_F(Mp4DemuxL1Tests, Parse_PsshEventsSurviveFragmentParse)
{
	uint8_t systemId[16];
	memcpy(systemId, Kid().data(), 16);
	auto moov = Box("moov", Pssh(systemId, {0x42}));
	demux.Parse(moov.data(), moov.size());
	// Parse() clears samples but not protectionEvents
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x01 | 0x0200, 0, {std::make_tuple(0u, 16u, 0)})}),
					   64);
	demux.Parse(seg.data(), seg.size());
	EXPECT_EQ(demux.getNumProtectionEvents(), 1u);
}

/* ================================================================== */
/* Fragments: moof/traf/tfhd/tfdt/trun                                  */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, Fragment_SingleSampleFromTrun)
{
	// trun flags 0x301 = data_offset(0x1) + duration(0x100) + size(0x200)
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 90000),
							   Trun(0x0301, 0, {std::make_tuple(3000u, 500u, 0)})}),
					   512);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
}

TEST_F(Mp4DemuxL1Tests, Fragment_SampleTimingFromTfdtV0)
{
	// timescale from init segment applies to fragment sample times
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000 /*timescale*/, stsd);
	demux.Parse(init.data(), init.size());

	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 5000 /*decode time units*/),
							   Trun(0x0301, 0, {std::make_tuple(2000u, 100u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_DOUBLE_EQ(demux.getDts(0), 5.0);        // 5000/1000 s
	EXPECT_DOUBLE_EQ(demux.getPts(0), 5.0);        // cts offset 0
	EXPECT_DOUBLE_EQ(demux.getDuration(0), 2.0);   // 2000/1000 s
	EXPECT_EQ(demux.getLen(0), 100u);
}

TEST_F(Mp4DemuxL1Tests, Fragment_SampleTimingFromTfdtV1)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(90000, stsd);
	demux.Parse(init.data(), init.size());

	uint64_t decodeTime = 0x123456789ULL;
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(1, decodeTime),
							   Trun(0x0301, 0, {std::make_tuple(90000u, 42u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_DOUBLE_EQ(demux.getDts(0), (double)decodeTime / 90000.0);
	EXPECT_DOUBLE_EQ(demux.getDuration(0), 1.0);
}

TEST_F(Mp4DemuxL1Tests, Fragment_CompositionOffsetShiftsPts)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	// flags 0x901 = data_offset + duration + cts_offset
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 1000),
							   Trun(0x0901, 0, {std::make_tuple(500u, 64u, 250)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_DOUBLE_EQ(demux.getDts(0), 1.0);        // 1000/1000
	EXPECT_DOUBLE_EQ(demux.getPts(0), 1.25);       // (1000+250)/1000
}

TEST_F(Mp4DemuxL1Tests, Fragment_MultipleSamplesAccumulateDts)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0),
												std::make_tuple(200u, 20u, 0),
												std::make_tuple(300u, 30u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 3);
	EXPECT_DOUBLE_EQ(demux.getDts(0), 0.0);
	EXPECT_DOUBLE_EQ(demux.getDts(1), 0.1);   // 0 + 100
	EXPECT_DOUBLE_EQ(demux.getDts(2), 0.3);   // 100 + 200
	EXPECT_EQ(demux.getLen(0), 10u);
	EXPECT_EQ(demux.getLen(1), 20u);
	EXPECT_EQ(demux.getLen(2), 30u);
}

TEST_F(Mp4DemuxL1Tests, Fragment_SamplePtrTracksDataOffset)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	// data_offset=0 -> sample ptr lands on the moof header itself (pointer math only)
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0301, 0, {std::make_tuple(100u, 32u, 0)})}),
					   64);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_EQ(demux.getPtr(0), seg.data());       // moof_ptr + 0
	EXPECT_EQ(demux.getLen(0), 32u);
}

TEST_F(Mp4DemuxL1Tests, Fragment_TfhdDefaultsProvideSizeAndDuration)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	// trun flags 0x01 only: per-sample dur/size absent -> tfhd defaults used
	auto seg = Segment(Concat({Tfhd(1, 0x0018, 400, 77), Tfdt(0, 0),
							   Trun(0x0001, 0, {std::make_tuple(0u, 0u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_EQ(demux.getLen(0), 77u);
	EXPECT_DOUBLE_EQ(demux.getDuration(0), 0.4);
}

TEST_F(Mp4DemuxL1Tests, Fragment_TrexDefaultsUsedWhenTfhdOmitsThem)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd, /*trexDur*/ 33, /*trexSize*/ 55);
	demux.Parse(init.data(), init.size());

	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0001, 0, {std::make_tuple(0u, 0u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_EQ(demux.getLen(0), 55u);
	EXPECT_DOUBLE_EQ(demux.getDuration(0), 0.033);
}

TEST_F(Mp4DemuxL1Tests, Fragment_TwoMoofsInSingleParse)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	auto moofA = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
								 Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)})}),
						 256);
	auto moofB = Segment(Concat({Tfhd(1, 0), Tfdt(0, 500),
								 Trun(0x0301, 0, {std::make_tuple(100u, 20u, 0)})}),
						 256);
	std::vector<uint8_t> both = moofA;
	both.insert(both.end(), moofB.begin(), moofB.end());
	demux.Parse(both.data(), both.size());
	ASSERT_EQ(demux.count(), 2);
	EXPECT_DOUBLE_EQ(demux.getDts(1), 0.5);
}

TEST_F(Mp4DemuxL1Tests, Parse_ClearsSamplesButKeepsCodecConfig)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	EXPECT_EQ(demux.count(), 1);

	// second Parse of another segment: samples reset, codec_type retained
	auto seg2 = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
								Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0),
												 std::make_tuple(100u, 10u, 0)})}),
						256);
	demux.Parse(seg2.data(), seg2.size());
	EXPECT_EQ(demux.count(), 2);

	// caps still generated from init segment's codec type
	demux.setCaps(nullptr);
	EXPECT_EQ(gSetCapsCallCount, 1);
	ASSERT_NE(gCapturedCaps, nullptr);
	EXPECT_STREQ(gst_structure_get_name(gst_caps_get_structure(gCapturedCaps, 0)), "video/x-h264");
}

TEST_F(Mp4DemuxL1Tests, Fragment_NegativeDataOffsetHandledAsSigned)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	// data_offset -8 -> sample ptr = moof_ptr - 8 (parser stores raw pointer)
	auto trun = Trun(0x0301, -8, {std::make_tuple(100u, 10u, 0)});
	auto traf = Box("traf", Concat({Tfhd(1, 0), Tfdt(0, 0), trun}));
	auto moof = Box("moof", Concat({Mfhd(1), traf}));
	std::vector<uint8_t> seg = moof;
	auto mdat = Box("mdat", std::vector<uint8_t>(64, 0xAB));
	seg.insert(seg.end(), mdat.begin(), mdat.end());
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_EQ((uintptr_t)demux.getPtr(0), (uintptr_t)seg.data() - 8);
}

/* ================================================================== */
/* Encrypted fragments: senc / saio+saiz                                */
/* ================================================================== */

static std::vector<uint8_t> EncryptedInit(uint8_t ivSize, bool cbcs = false,
										const std::vector<uint8_t> &constIv = {})
{
	uint8_t kid[16] = {0};
	auto sinf = Box("sinf", Concat({Frma("avc1"), Schm(cbcs ? "cbcs" : "cenc", 0x00010000),
									Box("schi", Tenc(cbcs ? "cbcs" : "cenc", cbcs ? 0x21 : 0, 1, ivSize, kid, constIv))}));
	auto stsd = Stsd(Box("encv", Concat({VideoSampleEntryBody(640, 360),
										 CodecConfigBox("avcC", {0x01}), sinf})));
	return InitSegment(1000, stsd);
}

TEST_F(Mp4DemuxL1Tests, Senc_PopulatesSampleIv)
{
	auto init = EncryptedInit(8);
	demux.Parse(init.data(), init.size());

	std::vector<uint8_t> iv = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17};
	auto traf = Concat({Tfhd(1, 0), Tfdt(0, 0),
						Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)}),
						Senc(0, 1, {iv})});
	auto seg = Segment(traf, 256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);

	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	guint ivSize = 0;
	EXPECT_TRUE(gst_structure_get_uint(meta, "iv_size", &ivSize));
	EXPECT_EQ(ivSize, 8u);
	const GValue *ivVal = gst_structure_get_value(meta, "iv");
	ASSERT_NE(ivVal, nullptr);
	GstBuffer *ivBuf = gst_value_get_buffer(ivVal);
	ASSERT_NE(ivBuf, nullptr);
	EXPECT_EQ(gst_buffer_get_size(ivBuf), 8u);
	guint8 out[8] = {0};
	gst_buffer_extract(ivBuf, 0, out, 8);
	EXPECT_EQ(0, memcmp(out, iv.data(), 8));
	gst_structure_free(meta);
}

TEST_F(Mp4DemuxL1Tests, Senc_NoIvInMetadataWhenIvSizeZero)
{
	auto init = EncryptedInit(0);
	demux.Parse(init.data(), init.size());

	auto traf = Concat({Tfhd(1, 0), Tfdt(0, 0),
						Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)}),
						Senc(0, 1, {{}})});
	auto seg = Segment(traf, 256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	EXPECT_FALSE(gst_structure_has_field(meta, "iv_size"));
	gst_structure_free(meta);
}

TEST_F(Mp4DemuxL1Tests, Senc_SubsampleEncryptionMetadata)
{
	auto init = EncryptedInit(8);
	demux.Parse(init.data(), init.size());

	std::vector<uint8_t> iv = {1, 2, 3, 4, 5, 6, 7, 8};
	// one subsample pair: clear=100 bytes, encrypted=2000 bytes
	std::vector<uint8_t> subsample = {0x00, 0x64, 0x00, 0x00, 0x07, 0xD0};
	auto traf = Concat({Tfhd(1, 0), Tfdt(0, 0),
						Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)}),
						Senc(0x02, 1, {iv}, {subsample})});
	auto seg = Segment(traf, 256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	guint cnt = 0;
	EXPECT_TRUE(gst_structure_get_uint(meta, "subsample_count", &cnt));
	EXPECT_EQ(cnt, 1u);
	const GValue *sv = gst_structure_get_value(meta, "subsamples");
	ASSERT_NE(sv, nullptr);
	EXPECT_EQ(gst_buffer_get_size(gst_value_get_buffer(sv)), 6u);
	gst_structure_free(meta);
}

TEST_F(Mp4DemuxL1Tests, DrmMetadata_KidCipherAndOriginalType)
{
	uint8_t kid[16];
	auto kidVec = Kid();
	memcpy(kid, kidVec.data(), 16);
	auto init = EncryptedInit(8);
	demux.Parse(init.data(), init.size());
	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)}),
							   Senc(0, 1, {std::vector<uint8_t>(8, 0xAA)})}),
					   256);
	demux.Parse(seg.data(), seg.size());

	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	gboolean enc = FALSE;
	EXPECT_TRUE(gst_structure_get_boolean(meta, "encrypted", &enc));
	EXPECT_TRUE(enc);
	EXPECT_STREQ(gst_structure_get_string(meta, "cipher-mode"), "cenc");
	EXPECT_STREQ(gst_structure_get_string(meta, "original-media-type"), "avc1");
	const GValue *kidVal = gst_structure_get_value(meta, "kid");
	ASSERT_NE(kidVal, nullptr);
	EXPECT_EQ(gst_buffer_get_size(gst_value_get_buffer(kidVal)), 16u);
	gst_structure_free(meta);
}

TEST_F(Mp4DemuxL1Tests, SaioSaiz_AuxInfoSuppliesIv)
{
	auto init = EncryptedInit(8);
	demux.Parse(init.data(), init.size());

	// Build moof manually so saio offset can point at aux data appended
	// after the moof box (aux region comes first, then the mdat box).
	std::vector<uint8_t> iv = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8};
	auto trafBefore = Concat({Tfhd(1, 0), Tfdt(0, 0),
							  Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)})});
	auto saiz = Saiz(8, 1);
	auto saioProbe = Saio(0, 0); // same size regardless of offset value
	size_t trafLen = 8 + trafBefore.size() + saiz.size() + saioProbe.size();
	size_t auxOffset = 8 /*moof hdr*/ + Mfhd(1).size() + trafLen;
	auto traf = Box("traf", Concat({trafBefore, saiz, Saio(0, auxOffset)}));
	auto moof = Box("moof", Concat({Mfhd(1), traf}));
	ASSERT_EQ(auxOffset, moof.size()); // IVs sit immediately after moof
	std::vector<uint8_t> seg = moof;
	seg.insert(seg.end(), iv.begin(), iv.end()); // aux region at auxOffset
	auto mdat = Box("mdat", std::vector<uint8_t>(64, 0xAB));
	seg.insert(seg.end(), mdat.begin(), mdat.end());
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	guint ivSize = 0;
	EXPECT_TRUE(gst_structure_get_uint(meta, "iv_size", &ivSize));
	EXPECT_EQ(ivSize, 8u);
	gst_structure_free(meta);
}

TEST_F(Mp4DemuxL1Tests, Cbcs_MetadataHasPatternAndConstantIv)
{
	std::vector<uint8_t> constIv(8, 0x5A);
	auto init = EncryptedInit(0, /*cbcs*/ true, constIv);
	demux.Parse(init.data(), init.size());

	auto seg = Segment(Concat({Tfhd(1, 0), Tfdt(0, 0),
							   Trun(0x0301, 0, {std::make_tuple(100u, 10u, 0)})}),
					   256);
	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	GstStructure *meta = demux.getDrmMetadata(0);
	ASSERT_NE(meta, nullptr);
	EXPECT_STREQ(gst_structure_get_string(meta, "cipher-mode"), "cbcs");
	guint crypt = 0, skip = 0, ivSize = 0;
	EXPECT_TRUE(gst_structure_get_uint(meta, "crypt_byte_block", &crypt));
	EXPECT_TRUE(gst_structure_get_uint(meta, "skip_byte_block", &skip));
	EXPECT_EQ(crypt, 2u); // pattern 0x21 -> crypt=2, skip=1
	EXPECT_EQ(skip, 1u);
	EXPECT_TRUE(gst_structure_get_uint(meta, "iv_size", &ivSize));
	EXPECT_EQ(ivSize, 8u); // constant_iv_size
	const GValue *ivv = gst_structure_get_value(meta, "iv");
	ASSERT_NE(ivv, nullptr);
	EXPECT_EQ(gst_buffer_get_size(gst_value_get_buffer(ivv)), 8u);
	gst_structure_free(meta);
}

/* ================================================================== */
/* AdjustMediaDecodeTime (static PTS restamping helper)                 */
/* ================================================================== */

static std::vector<uint8_t> WritableTfdt(uint8_t version, uint64_t decodeTime)
{
	return Tfdt(version, decodeTime);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_V0Rewrites)
{
	auto buf = WritableTfdt(0, 1000);
	uint64_t old = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), 500);
	EXPECT_EQ(old, 1500u); // returns new value after += delta
	uint32_t stored = ((uint32_t)buf[12] << 24) | (buf[13] << 16) | (buf[14] << 8) | buf[15];
	EXPECT_EQ(stored, 1500u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_V1Rewrites)
{
	auto buf = WritableTfdt(1, 0x100000000ULL);
	uint64_t old = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), 7);
	EXPECT_EQ(old, 0x100000007ULL);
	uint64_t stored = 0;
	for (int i = 0; i < 8; i++)
	{
		stored = (stored << 8) | buf[12 + i];
	}
	EXPECT_EQ(stored, 0x100000007ULL);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_NegativeDeltaWraps)
{
	auto buf = WritableTfdt(0, 100);
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), -50);
	EXPECT_EQ(rc, 50u);
	uint32_t stored = ((uint32_t)buf[12] << 24) | (buf[13] << 16) | (buf[14] << 8) | buf[15];
	EXPECT_EQ(stored, 50u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_ZeroDecodeTimeReturnsZero)
{
	// loop guards on !baseMediaDecodeTime - a tfdt with value 0 + delta != 0 still rewrites
	auto buf = WritableTfdt(0, 0);
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), 100);
	EXPECT_EQ(rc, 100u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_NoTfdtReturnsZero)
{
	auto buf = Box("moof", Box("mfhd", Mfhd(1)));
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), 500);
	EXPECT_EQ(rc, 0u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_NestedInMoofTraf)
{
	auto tfdt = WritableTfdt(0, 90000);
	auto traf = Box("traf", tfdt);
	auto moof = Box("moof", Concat({Mfhd(1), traf}));
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(moof.data(), moof.size(), 10);
	EXPECT_EQ(rc, 90010u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_NonContainerSiblingSkipped)
{
	// ftyp before moof must be skipped (not a container)
	auto moof = Box("moof", Box("traf", WritableTfdt(0, 42)));
	auto buf = Concat({Box("ftyp", {0, 0, 0, 0}), moof});
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(buf.data(), buf.size(), 1);
	EXPECT_EQ(rc, 43u);
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_FirstTfdtWins)
{
	auto traf1 = Box("traf", WritableTfdt(0, 10));
	auto traf2 = Box("traf", WritableTfdt(0, 20));
	auto moof = Box("moof", Concat({traf1, traf2}));
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(moof.data(), moof.size(), 5);
	EXPECT_EQ(rc, 15u); // only first tfdt adjusted
}

TEST_F(Mp4DemuxL1Tests, AdjustMediaDecodeTime_UnknownLeafSkipped)
{
	// a leaf box inside moof (not a container, not tfdt) is skipped
	auto traf = Box("traf", Concat({Box("xxxx", {0, 0, 0, 0}), WritableTfdt(0, 7)}));
	auto moof = Box("moof", traf);
	uint64_t rc = Mp4Demux::AdjustMediaDecodeTime(moof.data(), moof.size(), 3);
	EXPECT_EQ(rc, 10u);
}

/* ================================================================== */
/* Sample pointer verification                                          */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, Fragment_SamplePtrPointsIntoMdatRegion)
{
	auto stsd = Stsd(Box("avc1", Concat({VideoSampleEntryBody(640, 360), CodecConfigBox("avcC", {0x01})})));
	auto init = InitSegment(1000, stsd);
	demux.Parse(init.data(), init.size());

	// data_offset points into the mdat payload: moofLen + mdat header (8)
	auto trunProbe = Trun(0x0301, 0, {std::make_tuple(100u, 8u, 0)});
	auto trafProbe = Box("traf", Concat({Tfhd(1, 0), Tfdt(0, 0), trunProbe}));
	auto moofProbe = Box("moof", Concat({Mfhd(1), trafProbe}));
	size_t moofLen = moofProbe.size();

	auto trun = Trun(0x0301, (int32_t)(moofLen + 8), {std::make_tuple(100u, 8u, 0)});
	auto traf = Box("traf", Concat({Tfhd(1, 0), Tfdt(0, 0), trun}));
	auto moof = Box("moof", Concat({Mfhd(1), traf}));
	std::vector<uint8_t> seg = moof;
	auto mdat = Box("mdat", std::vector<uint8_t>(8, 0xEE));
	seg.insert(seg.end(), mdat.begin(), mdat.end());

	demux.Parse(seg.data(), seg.size());
	ASSERT_EQ(demux.count(), 1);
	EXPECT_EQ(demux.getPtr(0), seg.data() + moofLen + 8);
	EXPECT_EQ(demux.getLen(0), 8u);
}

/* ================================================================== */
/* Encrypted audio path                                                 */
/* ================================================================== */

TEST_F(Mp4DemuxL1Tests, EncaAudioEntry_ParsesAudioInformation)
{
	uint8_t kid[16] = {0};
	auto sinf = Box("sinf", Concat({Frma("mp4a"), Schm("cenc", 0x00010000),
									Box("schi", Tenc("cenc", 0, 1, 8, kid))}));
	auto stsd = Stsd(Box("enca", Concat({AudioSampleEntryBody(2, 48000),
										 EsdsBox({0x11, 0x90}), sinf})));
	auto init = InitSegment(48000, stsd);
	EXPECT_NO_FATAL_FAILURE(demux.Parse(init.data(), init.size()));
	// enca + esds -> codec_type esds -> audio/mpeg caps
	demux.setCaps(nullptr);
	ASSERT_NE(gCapturedCaps, nullptr);
	EXPECT_STREQ(gst_structure_get_name(gst_caps_get_structure(gCapturedCaps, 0)), "application/x-cenc");
}
