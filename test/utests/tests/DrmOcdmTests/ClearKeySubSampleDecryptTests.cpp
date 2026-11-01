/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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
 * @file ClearKeySubSampleDecryptTests.cpp
 * @brief Tests for ClearKeySession::decrypt() handling of CENC subsample tables.
 *
 * The subsample table comes from the media segment and is untrusted, so the
 * decrypt path must reject tables whose ranges do not fit within the sample.
 * Crypto is faked (FakeCrypto.cpp): a successful decrypt replaces the encrypted
 * ranges with zeros and leaves the clear ranges untouched.
 */

#include <gtest/gtest.h>
#include <cstring>
#include <string>
#include <vector>
#include "ClearKeyDrmSession.h"
#include "DrmData.h"
#include "gstMocks.h"

namespace
{
const uint8_t kSampleFill = 0xA5;
const uint8_t kKeyId[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                            0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
const char *kLicenseResponse =
	"{\"keys\":[{\"kty\":\"oct\",\"k\":\"ISIjJCUmJygpKissLS4vMA\",\"kid\":\"AQIDBAUGBwgJCgsMDQ4PEA\"}]}";

struct SubSample
{
	uint16_t clear;
	uint32_t encrypted;
};

std::vector<uint8_t> BuildSubSampleTable(const std::vector<SubSample> &entries)
{
	std::vector<uint8_t> table;
	for (const auto &entry : entries)
	{
		table.push_back((uint8_t)(entry.clear >> 8));
		table.push_back((uint8_t)(entry.clear));
		table.push_back((uint8_t)(entry.encrypted >> 24));
		table.push_back((uint8_t)(entry.encrypted >> 16));
		table.push_back((uint8_t)(entry.encrypted >> 8));
		table.push_back((uint8_t)(entry.encrypted));
	}
	return table;
}
}

class ClearKeySubSampleDecryptTests : public ::testing::Test
{
protected:
	ClearKeySession mSession;
	GstBuffer mKeyIdBuffer{};
	GstBuffer mIvBuffer{};
	GstBuffer mSampleBuffer{};
	GstBuffer mSubSamplesBuffer{};
	GstCaps mCaps{};
	uint8_t mIv[16]{};
	uint8_t *mSample = nullptr;
	size_t mSampleSize = 0;
	std::vector<uint8_t> mSubSampleTable;

	void SetUp() override
	{
		MockGstBufferClearMapData();

		std::vector<uint8_t> initData(32, 0);
		initData.insert(initData.end(), kKeyId, kKeyId + sizeof(kKeyId));
		std::string customData;
		mSession.generateDRMSession(initData.data(), (uint32_t)initData.size(), customData);

		std::string url;
		DrmData *challenge = mSession.generateKeyRequest(url, 0);
		ASSERT_NE(challenge, nullptr);
		delete challenge;

		DrmData license(kLicenseResponse, strlen(kLicenseResponse));
		ASSERT_EQ(mSession.processDRMKey(&license, 0), 1);
		ASSERT_EQ(mSession.getState(), KEY_READY);

		MockGstBufferSetMapData(&mIvBuffer, mIv, sizeof(mIv));
	}

	void TearDown() override
	{
		MockGstBufferClearMapData();
		free(mSample);
	}

	/* Exact-size heap allocation so ASan reports any access past the sample. */
	void SetSample(size_t size)
	{
		free(mSample);
		mSampleSize = size;
		mSample = (uint8_t *)malloc(size);
		memset(mSample, kSampleFill, size);
		MockGstBufferSetMapData(&mSampleBuffer, mSample, size);
	}

	void SetSubSamples(const std::vector<SubSample> &entries)
	{
		mSubSampleTable = BuildSubSampleTable(entries);
		MockGstBufferSetMapData(&mSubSamplesBuffer, mSubSampleTable.data(), mSubSampleTable.size());
	}

	int Decrypt(unsigned subSampleCount)
	{
		return mSession.decrypt(&mKeyIdBuffer, &mIvBuffer, &mSampleBuffer, subSampleCount, &mSubSamplesBuffer, &mCaps);
	}

	void ExpectSampleUnchanged()
	{
		for (size_t i = 0; i < mSampleSize; i++)
		{
			ASSERT_EQ(mSample[i], kSampleFill) << "sample modified at offset " << i;
		}
	}
};

TEST_F(ClearKeySubSampleDecryptTests, ValidSubSamplesDecryptOnlyEncryptedRanges)
{
	SetSample(64);
	SetSubSamples({{8, 24}, {16, 16}});

	EXPECT_EQ(Decrypt(2), 0);

	for (size_t i = 0; i < mSampleSize; i++)
	{
		bool encrypted = (i >= 8 && i < 32) || (i >= 48 && i < 64);
		EXPECT_EQ(mSample[i], encrypted ? 0x00 : kSampleFill) << "offset " << i;
	}
}

TEST_F(ClearKeySubSampleDecryptTests, SubSamplesExactlyFillingBufferAreAccepted)
{
	SetSample(64);
	SetSubSamples({{0, 32}, {0, 32}});

	EXPECT_EQ(Decrypt(2), 0);

	for (size_t i = 0; i < mSampleSize; i++)
	{
		EXPECT_EQ(mSample[i], 0x00) << "offset " << i;
	}
}

TEST_F(ClearKeySubSampleDecryptTests, EncryptedLengthLargerThanSampleIsRejected)
{
	SetSample(64);
	SetSubSamples({{0, 0x100000}});

	EXPECT_NE(Decrypt(1), 0);
	ExpectSampleUnchanged();
}

TEST_F(ClearKeySubSampleDecryptTests, ClearLengthLargerThanSampleIsRejected)
{
	SetSample(64);
	SetSubSamples({{0xFFFF, 1}});

	EXPECT_NE(Decrypt(1), 0);
	ExpectSampleUnchanged();
}

TEST_F(ClearKeySubSampleDecryptTests, CumulativeLengthLargerThanSampleIsRejected)
{
	SetSample(64);
	SetSubSamples({{8, 24}, {8, 24}, {8, 1}});

	EXPECT_NE(Decrypt(3), 0);
	ExpectSampleUnchanged();
}

TEST_F(ClearKeySubSampleDecryptTests, LengthsWrappingUint32AreRejected)
{
	SetSample(64);
	SetSubSamples({{0x10, 0xFFFFFFF0}, {0, 0x20}});

	EXPECT_NE(Decrypt(2), 0);
	ExpectSampleUnchanged();
}

TEST_F(ClearKeySubSampleDecryptTests, TruncatedSubSampleTableIsRejected)
{
	SetSample(64);
	SetSubSamples({{8, 8}, {8, 8}});

	EXPECT_NE(Decrypt(3), 0);
	ExpectSampleUnchanged();
}
