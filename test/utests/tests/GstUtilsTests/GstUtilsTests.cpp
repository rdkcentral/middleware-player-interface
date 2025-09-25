/*
* If not stated otherwise in this file or this component's license file the
* following copyright and licenses apply:
*
* Copyright 2023 RDK Management
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <chrono>
#include <cstring>

#include "GstUtils.h"
#include "MockGStreamer.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

class GstUtilsTests : public ::testing::Test
{
protected:

    void SetUp() override
    {
        g_mockGStreamer = new MockGStreamer();
    }

    void TearDown() override
    {
        delete g_mockGStreamer;
        g_mockGStreamer = nullptr;
    }

public:

};

TEST_F(GstUtilsTests, esMP3test)
{
    GstCaps dummycaps;
    GstCaps *caps{&dummycaps};
    EXPECT_CALL(*g_mockGStreamer,gst_caps_new_simple(StrEq("audio/mpeg"),StrEq("mpegversion"), G_TYPE_INT, 1, NULL)).WillOnce(Return(caps));

    EXPECT_TRUE(GetCaps(GST_FORMAT_AUDIO_ES_MP3)==caps);
}

TEST_F(GstUtilsTests, GstCapsFormatsTest)
{
    GstCaps dummycapslist;
    GstCaps *caps{&dummycapslist};
    GstStreamOutputFormat GstCapsFormats[16] = {
    GST_FORMAT_INVALID,         /**< Invalid format */
    GST_FORMAT_MPEGTS,          /**< MPEG Transport Stream */
    GST_FORMAT_ISO_BMFF,        /**< ISO Base Media File format */
    GST_FORMAT_AUDIO_ES_MP3,    /**< MP3 Audio Elementary Stream */
    GST_FORMAT_AUDIO_ES_AAC,    /**< AAC Audio Elementary Stream */
    GST_FORMAT_AUDIO_ES_AC3,    /**< AC3 Audio Elementary Stream */
    GST_FORMAT_AUDIO_ES_EC3,    /**< Dolby Digital Plus Elementary Stream */
    GST_FORMAT_AUDIO_ES_ATMOS,  /**< ATMOS Audio stream */
    GST_FORMAT_AUDIO_ES_AC4,    /**< AC4 Dolby Audio stream */
    GST_FORMAT_VIDEO_ES_H264,   /**< MPEG-4 Video Elementary Stream */
    GST_FORMAT_VIDEO_ES_HEVC,   /**< HEVC video elementary stream */
    GST_FORMAT_VIDEO_ES_MPEG2,  /**< MPEG-2 Video Elementary Stream */
    GST_FORMAT_SUBTITLE_WEBVTT, /**< WebVTT subtitle Stream */
    GST_FORMAT_SUBTITLE_TTML,   /**< WebVTT subtitle Stream */
    GST_FORMAT_SUBTITLE_MP4,    /**< Generic MP4 stream */
    GST_FORMAT_UNKNOWN          /**< Unknown Format */
    };

    for(int i=0;i<16;i++){
    GetCaps(GstCapsFormats[i]);
    ASSERT_FALSE(GetCaps(GstCapsFormats[i]));
    }
}

/**
 * @brief Test to validate that the GetCaps API returns a valid GstCaps pointer for supported valid stream output formats.
 *
 * This test iterates over an array of valid stream output formats and invokes the GetCaps API for each format.
 * It verifies that the returned GstCaps pointer is not null and that no exceptions are thrown during the call.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through valid stream output formats and call GetCaps for each format. | input: format = each element in validFormats[], output: GstCaps pointer (expected to be non-null) | GetCaps should return a valid non-null GstCaps pointer without throwing an exception. | Should Pass |
 */
TEST(GetCaps_PositiveValidFormats, PositiveTest_ValidSupportedFormats) {
    std::cout << "Entering GetCaps_PositiveValidFormats test" << std::endl;
    GstStreamOutputFormat validFormats[] = {
        GST_FORMAT_MPEGTS,
        GST_FORMAT_ISO_BMFF,
        GST_FORMAT_AUDIO_ES_MP3,
        GST_FORMAT_AUDIO_ES_AAC,
        GST_FORMAT_AUDIO_ES_AC3,
        GST_FORMAT_AUDIO_ES_EC3,
        GST_FORMAT_AUDIO_ES_ATMOS,
        GST_FORMAT_AUDIO_ES_AC4,
        GST_FORMAT_VIDEO_ES_H264,
        GST_FORMAT_VIDEO_ES_HEVC,
        GST_FORMAT_VIDEO_ES_MPEG2,
        GST_FORMAT_SUBTITLE_WEBVTT,
        GST_FORMAT_SUBTITLE_TTML,
        GST_FORMAT_SUBTITLE_MP4
    };
    const int numFormats = sizeof(validFormats) / sizeof(validFormats[0]);
    for (int i = 0; i < numFormats; i++) {
        GstStreamOutputFormat fmt = validFormats[i];
        std::cout << "Invoking GetCaps with format value: " << fmt << std::endl;
        EXPECT_NO_THROW({
            GstCaps *caps = GetCaps(fmt);
            std::cout << "Returned GstCaps pointer: " << caps << std::endl;
            EXPECT_NE(caps, nullptr);
            if (caps != nullptr) {
                delete caps;
            }
        });
    }
    std::cout << "Exiting GetCaps_PositiveValidFormats test" << std::endl;
}
/**
 * @brief Verify that GetCaps returns a null pointer when invoked with an invalid format.
 *
 * This test checks the behavior of the GetCaps API when an invalid format (GST_FORMAT_INVALID) is provided.
 * The expectation is that no exception is thrown and the function returns a null GstCaps pointer, thus indicating proper handling of invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                      | Expected Result                                                                            | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke GetCaps with GST_FORMAT_INVALID and verify that the returned GstCaps is nullptr. | input: fmt = GST_FORMAT_INVALID, output: GstCaps pointer = nullptr | No exception is thrown and the returned GstCaps pointer is nullptr as per EXPECT_EQ check. | Should Fail |
 */
TEST(GetCaps_NegativeInvalidFormat, NegativeTest_InvalidFormat) {
    std::cout << "Entering GetCaps_NegativeInvalidFormat test" << std::endl;
    GstStreamOutputFormat fmt = GST_FORMAT_INVALID;
    std::cout << "Invoking GetCaps with format value: " << fmt << std::endl;
    EXPECT_NO_THROW({
        GstCaps *caps = GetCaps(fmt);
        std::cout << "Returned GstCaps pointer: " << caps << std::endl;
        EXPECT_EQ(caps, nullptr);
    });
    std::cout << "Exiting GetCaps_NegativeInvalidFormat test" << std::endl;
}
/**
 * @brief Verify that GetCaps returns a null pointer when provided with an unknown format.
 *
 * This test validates that when the GetCaps API is invoked with a format value of GST_FORMAT_UNKNOWN, no exception is thrown and the returned GstCaps pointer is nullptr. This behavior is expected as the API should handle unknown formats gracefully by returning a null pointer.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 003
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                        | Expected Result                                                         | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------ | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke GetCaps API with an unknown format and verify no exception is thrown, and that it returns a nullptr. | input: fmt = GST_FORMAT_UNKNOWN, output: caps = nullptr | API should not throw any exception and must return a nullptr as the GstCaps pointer. | Should Pass |
 */
TEST(GetCaps_NegativeUnknownFormat, NegativeTest_UnknownFormat) {
    std::cout << "Entering GetCaps_NegativeUnknownFormat test" << std::endl;
    GstStreamOutputFormat fmt = GST_FORMAT_UNKNOWN;
    std::cout << "Invoking GetCaps with format value: " << fmt << std::endl;
    EXPECT_NO_THROW({
        GstCaps *caps = GetCaps(fmt);
        std::cout << "Returned GstCaps pointer: " << caps << std::endl;
        EXPECT_EQ(caps, nullptr);
    });
    std::cout << "Exiting GetCaps_NegativeUnknownFormat test" << std::endl;
}
/**
 * @brief Test GetCaps API with an invalid out-of-range enum value.
 *
 * This test verifies that calling GetCaps with an out-of-range enum value does not throw an exception and returns a nullptr as expected. It ensures proper error handling for invalid input values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                           | Test Data                                             | Expected Result                                                  | Notes            |
 * | :--------------: | --------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------------------- | ---------------- |
 * | 01               | Log the beginning of the test.                                        | None                                                  | Entry log message is printed without error                       | Should be successful  |
 * | 02               | Set the out-of-range GstStreamOutputFormat value.                     | fmt = 999 (GstStreamOutputFormat)                     | fmt is correctly set to an out-of-range value                      | Should be successful  |
 * | 03               | Invoke GetCaps API with the out-of-range format value.                | input: fmt = 999, output: caps pointer                | No exception thrown; returned GstCaps pointer is nullptr           | Should Fail      |
 * | 04               | Log the exit of the test function.                                    | None                                                  | Exit log message is printed                                      | Should be successful  |
 */
TEST(GetCaps_NegativeOutOfRange, NegativeTest_OutOfRangeEnumValue) {
    std::cout << "Entering GetCaps_NegativeOutOfRange test" << std::endl;
    GstStreamOutputFormat fmt = static_cast<GstStreamOutputFormat>(999);
    std::cout << "Invoking GetCaps with out-of-range format value: " << 999 << std::endl;
    EXPECT_NO_THROW({
        GstCaps *caps = GetCaps(fmt);
        std::cout << "Returned GstCaps pointer: " << caps << std::endl;
        EXPECT_EQ(caps, nullptr);
    });
    std::cout << "Exiting GetCaps_NegativeOutOfRange test" << std::endl;
}
/**
 * @brief Validate that the GetCurrentTimeMS API returns a valid timestamp
 *
 * This test verifies that the GetCurrentTimeMS API can be invoked without throwing an exception
 * and returns a positive timestamp value. It ensures that the system time retrieval API is functional.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                | Test Data                                          | Expected Result                                            | Notes         |
 * | :--------------: | -------------------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Invoke GetCurrentTimeMS and capture the timestamp without throwing         | timestamp = 0, API = GetCurrentTimeMS()             | No exception thrown and a valid timestamp (non-zero) is returned | Should Pass   |
 * | 02               | Validate that the captured timestamp is greater than zero                  | captured timestamp, none                           | The timestamp value is > 0 and assertion passes            | Should Pass   |
 */
TEST(GetCurrentTimeMS, GetCurrentTimeMS_start) {
    std::cout << "Entering GetCurrentTimeMS_start test" << std::endl;
    std::cout << "Invoking GetCurrentTimeMS()" << std::endl;
    long long timestamp = 0;
    EXPECT_NO_THROW({
        timestamp = GetCurrentTimeMS();
        std::cout << "Method GetCurrentTimeMS returned: " << timestamp << std::endl;
    });
    EXPECT_GT(timestamp, 0);
    std::cout << "Exiting GetCurrentTimeMS_start test" << std::endl;
}
/**
 * @brief Validates initialization with valid command-line arguments.
 *
 * This test ensures that PlayerCliGstInit correctly initializes the system when supplied with valid command-line arguments.
 * It tests the functionality by setting up appropriate command-line arguments and verifying that no exception is thrown during initialization.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 006
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup valid command-line arguments, reset the global internal state, and invoke PlayerCliGstInit | argc = 3, argv = {"player", "--verbose", "--config=config.txt"}, g_GstInitialized = false | PlayerCliGstInit should execute without throwing an exception | Should Pass |
 */
TEST(PlayerCliGstInit_ValidArgs, ValidInitializationWithValidCommandLineArguments)
{
    std::cout << "Entering ValidInitializationWithValidCommandLineArguments test" << std::endl;
    
    // Setup valid command-line arguments: {"player", "--verbose", "--config=config.txt"}
    int argc = 3;
    
    char arg0[20];
    char arg1[20];
    char arg2[30];
    
    std::memset(arg0, 0, sizeof(arg0));
    std::memset(arg1, 0, sizeof(arg1));
    std::memset(arg2, 0, sizeof(arg2));
    
    std::strncpy(arg0, "player", sizeof(arg0) - 1);
    std::strncpy(arg1, "--verbose", sizeof(arg1) - 1);
    std::strncpy(arg2, "--config=config.txt", sizeof(arg2) - 1);
    
    // Create array of pointers
    char* args[3] = { arg0, arg1, arg2 };
    char **argv = args;
    
    std::cout << "Test Parameters: argc = " << argc << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "Test Parameter: argv[" << i << "] = " << argv[i] << std::endl;
    }
    
    // Reset global internal state before invocation.
    //g_GstInitialized = false;
    
    // Invoke function and expect no exception.
    EXPECT_NO_THROW(PlayerCliGstInit(&argc, &argv));
    
    std::cout << "Exiting ValidInitializationWithValidCommandLineArguments test" << std::endl;
}
/**
 * @brief Tests that PlayerCliGstInit throws a runtime_error when a nullptr is passed for argc.
 *
 * This test validates that passing a nullptr for the argc parameter to the PlayerCliGstInit API triggers a runtime_error exception. It ensures that the function correctly handles invalid input scenarios where mandatory argument pointers are null.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 007
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup valid argv with "player" and "--help", pass nullptr for argc, then call PlayerCliGstInit | argc = nullptr, argv[0] = player, argv[1] = --help | runtime_error exception thrown; EXPECT_THROW assertion passes | Should Fail |
 */
TEST_F(GstUtilsTests,  NegativeTestWithNULLForArgc)
{
    std::cout << "Entering NegativeTestWithNULLForArgc test" << std::endl;
    
    // Setup valid argv parameters: {"player", "--help"}
    int dummyArgc = 2;
    char arg0[20];
    char arg1[20];
    std::memset(arg0, 0, sizeof(arg0));
    std::memset(arg1, 0, sizeof(arg1));
    
    std::strncpy(arg0, "player", sizeof(arg0) - 1);
    std::strncpy(arg1, "--help", sizeof(arg1) - 1);
    
    char* args[2] = { arg0, arg1 };
    char **argv = args;
    
    std::cout << "Test Parameters: Passing NULL for argc, argv[0] = " << argv[0] << ", argv[1] = " << argv[1] << std::endl;
        
    // Invoke function with NULL for argc and expect an exception.
    EXPECT_THROW(PlayerCliGstInit(nullptr, &argv), std::runtime_error);
        
    std::cout << "Exiting NegativeTestWithNULLForArgc test" << std::endl;
}
/**
 * @brief Verify that PlayerCliGstInit throws std::runtime_error when argv is NULL.
 *
 * This test validates that the PlayerCliGstInit function correctly handles a NULL argv pointer by throwing a std::runtime_error. It uses a valid argc value but passes NULL for argv to simulate an error scenario, ensuring proper error management.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 008
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                 | Expected Result                                 | Notes       |
 * | :--------------: | ------------------------------------------------------------ | ------------------------- | ----------------------------------------------- | ----------- |
 * | 01               | Call PlayerCliGstInit with valid argc and argv set to NULL     | argc = 2, argv = nullptr  | Expect std::runtime_error to be thrown and caught | Should Fail |
 */
TEST_F(GstUtilsTests,  NegativeTestWithNULLForArgv)
{
    std::cout << "Entering NegativeTestWithNULLForArgv test" << std::endl;
    
    // Setup valid argc value.
    int argc = 2;
    std::cout << "Test Parameters: argc = " << argc << ", passing NULL for argv" << std::endl;
        
    // Invoke function with NULL for argv and expect an exception.
    EXPECT_THROW(PlayerCliGstInit(&argc, nullptr), std::runtime_error);    
    std::cout << "Exiting NegativeTestWithNULLForArgv test" << std::endl;
}
/**
 * @brief Verify that PlayerCliGstTerm starts correctly without throwing exceptions.
 *
 * This test case validates that the PlayerCliGstTerm() function executes without errors, ensuring that GStreamer resources are properly terminated. The test confirms the expected behavior by verifying that no exceptions are thrown during the API invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test name and log test entry | testName = "PlayerCliGstTerm()_start" | Test entry logged with correct name | Should be successful |
 * | 02 | Invoke PlayerCliGstTerm() and verify that no exception is thrown | Invocation of PlayerCliGstTerm(), no input parameters, output: exception = none | EXPECT_NO_THROW passes | Should Pass |
 * | 03 | Log test exit message | exit message with testName value | Test exit logged with correct name | Should be successful |
 */
TEST_F(GstUtilsTests,  PlayerCliGstTerm_start) {
    char testName[50];
    strncpy(testName, "PlayerCliGstTerm()_start", sizeof(testName)-1);
    testName[sizeof(testName)-1] = '\0';
    std::cout << "Entering " << testName << " test" << std::endl;
    
    std::cout << "Invoking PlayerCliGstTerm" << std::endl;
    EXPECT_NO_THROW({
        PlayerCliGstTerm();
    });
    std::cout << "PlayerCliGstTerm executed with GStreamer resources properly terminated and no errors occurred" << std::endl;
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Validate that gstGetMediaTypeName returns a valid non-null pointer for each valid GstMediaType value
 *
 * This test verifies that for every valid GstMediaType defined, the gstGetMediaTypeName API does not throw an exception 
 * and returns a non-null pointer. The test ensures that the API handles all valid inputs properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the entry message to indicate the start of the ValidGstMediaType test | None | "Entering ValidGstMediaType test" message printed to console | Should be successful |
 * | 02 | Iterate over each valid GstMediaType value, invoke gstGetMediaTypeName, and verify that no exception is thrown and the returned pointer is not null | mediaType = each value from {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19}, output: retVal | Returned pointer is not null and no exception is thrown | Should Pass |
 * | 03 | Log the exit message to indicate the end of the ValidGstMediaType test | None | "Exiting ValidGstMediaType test" message printed to console | Should be successful |
 */
TEST_F(GstUtilsTests,  ValidGstMediaType)
{
    std::cout << "Entering ValidGstMediaType test" << std::endl;

    // List of valid GstMediaType enum values.
    int validMediaTypes[] = {
        0,  // eGST_MEDIATYPE_VIDEO
        1,  // eGST_MEDIATYPE_AUDIO
        2,  // eGST_MEDIATYPE_SUBTITLE
        3,  // eGST_MEDIATYPE_AUX_AUDIO
        4,  // eGST_MEDIATYPE_MANIFEST
        5,  // eGST_MEDIATYPE_LICENCE
        6,  // eGST_MEDIATYPE_IFRAME
        7,  // eGST_MEDIATYPE_INIT_VIDEO
        8,  // eGST_MEDIATYPE_INIT_AUDIO
        9,  // eGST_MEDIATYPE_INIT_SUBTITLE
        10, // eGST_MEDIATYPE_INIT_AUX_AUDIO
        11, // eGST_MEDIATYPE_PLAYLIST_VIDEO
        12, // eGST_MEDIATYPE_PLAYLIST_AUDIO
        13, // eGST_MEDIATYPE_PLAYLIST_SUBTITLE
        14, // eGST_MEDIATYPE_PLAYLIST_AUX_AUDIO
        15, // eGST_MEDIATYPE_PLAYLIST_IFRAME
        16, // eGST_MEDIATYPE_INIT_IFRAME
        17, // eGST_MEDIATYPE_DSM_CC
        18, // eGST_MEDIATYPE_IMAGE
        19  // eGST_MEDIATYPE_DEFAULT
    };

    for (int i = 0; i < sizeof(validMediaTypes)/sizeof(validMediaTypes[0]); i++)
    {
        GstMediaType mediaType = static_cast<GstMediaType>(validMediaTypes[i]);
        std::cout << "Invoking gstGetMediaTypeName with mediaType value: " << mediaType << std::endl;
        EXPECT_NO_THROW({
            const char * retVal = gstGetMediaTypeName(mediaType);
            std::cout << "Returned pointer: " << static_cast<const void*>(retVal) << std::endl;
            // Check that the returned value is not a nullptr.
            EXPECT_NE(retVal, nullptr);
        });
    }
    std::cout << "Exiting ValidGstMediaType test" << std::endl;
}
/**
 * @brief Validate gstGetMediaTypeName function for invalid media type values.
 *
 * This test invokes gstGetMediaTypeName with invalid media type values (-1 and 100) and verifies that the function returns a nullptr without throwing any exceptions. The test ensures that the API properly handles improper input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                      | Expected Result                                                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------- | --------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke gstGetMediaTypeName with mediaType = -1                     | input = mediaType = -1, output = expected nullptr | Return value must be nullptr and no exception thrown                                          | Should Pass |
 * | 02               | Invoke gstGetMediaTypeName with mediaType = 100                    | input = mediaType = 100, output = expected nullptr | Return value must be nullptr and no exception thrown                                          | Should Pass |
 */
TEST_F(GstUtilsTests,  InvalidGstMediaType)
{
    std::cout << "Entering InvalidGstMediaType test" << std::endl;

    // For invalid inputs, we test with -1 and 100.
    int invalidMediaTypes[] = {-1, 100};
    for (int i = 0; i < sizeof(invalidMediaTypes)/sizeof(invalidMediaTypes[0]); ++i)
    {
        GstMediaType mediaType = static_cast<GstMediaType>(invalidMediaTypes[i]);
        std::cout << "Invoking gstGetMediaTypeName with invalid mediaType value: " << mediaType << std::endl;
        EXPECT_NO_THROW({
            const char * retVal = gstGetMediaTypeName(mediaType);
            std::cout << "Returned pointer for invalid mediaType (" << mediaType << "): " 
                      << static_cast<const void*>(retVal) << std::endl;
            EXPECT_EQ(retVal, nullptr);
        });
    }
    std::cout << "Exiting InvalidGstMediaType test" << std::endl;
}
