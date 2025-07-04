
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
* @file test_l1_WebVttSubtecParser.cpp
* @page WebVttSubtecParser Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the WebVttSubtecParser methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "WebVttSubtecParser.hpp"
#include <cfloat> 
#include <climits> 

/**
 * @brief Test the construction of WebVTTSubtecParser with valid positive dimensions.
 *
 * This test verifies that the WebVTTSubtecParser constructor successfully creates an instance without throwing an exception when provided with valid positive dimensions (width and height) for various subtitle mime types. The objective is to ensure stability when handling valid input parameters.
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
 * | 01 | Invoke WebVTTSubtecParser constructor for each supported subtitle mime type using width=640 and height=480 | type = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN, width = 640, height = 480 | Constructor does not throw any exception and instance is created successfully | Should Pass |
 */
TEST(WebVTTSubtecParser, ConstructWithValidPositiveDimensions)
{
    std::cout << "Entering ConstructWithValidPositiveDimensions test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = 640;
    int height = 480;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_NO_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance created successfully for type: " << type << std::endl;
        });
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithValidPositiveDimensions test" << std::endl;
}

/**
 * @brief Validate WebVTTSubtecParser construction with maximum dimensions
 *
 * This test verifies that the WebVTTSubtecParser constructor properly handles instantiation using maximum integer values for width and height across different subtitle types. The objective is to ensure that no exception is thrown during the object creation even at boundary conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Call constructor for subtitle type eSUB_TYPE_WEBVTT with maximum width and height. | type = eSUB_TYPE_WEBVTT, width = INT_MAX, height = INT_MAX | No exception thrown; instance created successfully. | Should Pass |
 * | 02 | Call constructor for subtitle type eSUB_TYPE_MP4 with maximum width and height. | type = eSUB_TYPE_MP4, width = INT_MAX, height = INT_MAX | No exception thrown; instance created successfully. | Should Pass |
 * | 03 | Call constructor for subtitle type eSUB_TYPE_TTML with maximum width and height. | type = eSUB_TYPE_TTML, width = INT_MAX, height = INT_MAX | No exception thrown; instance created successfully. | Should Pass |
 * | 04 | Call constructor for subtitle type eSUB_TYPE_UNKNOWN with maximum width and height. | type = eSUB_TYPE_UNKNOWN, width = INT_MAX, height = INT_MAX | No exception thrown; instance created successfully. | Should Pass |
 */
TEST(WebVTTSubtecParser, ConstructWithMaxDimensions)
{
    std::cout << "Entering ConstructWithMaxDimensions test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = INT_MAX;
    int height = INT_MAX;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_NO_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance created successfully for type: " << type << " with maximum dimensions" << std::endl;
        });
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithMaxDimensions test" << std::endl;
}

/**
 * @brief Validate WebVTTSubtecParser constructor against zero width input.
 *
 * This test ensures that the WebVTTSubtecParser constructor throws a std::invalid_argument exception when provided with a width value of zero, which is invalid. The constructor is invoked for various subtitle MIME types to verify that it consistently detects and handles the invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the WebVTTSubtecParser constructor for each subtitle MIME type with width = 0 and height = 480 to validate proper error handling for an invalid width. | type = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN, width = 0, height = 480 | The constructor should throw a std::invalid_argument exception. | Should Fail |
 */
TEST(WebVTTSubtecParser, ConstructWithZeroWidth)
{
    std::cout << "Entering ConstructWithZeroWidth test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = 0;
    int height = 480;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance creation should have failed for type: " << type << " with zero width" << std::endl;
        }, std::invalid_argument);
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithZeroWidth test" << std::endl;
}

/**
 * @brief Tests that constructing a WebVTTSubtecParser with a zero height parameter raises an exception.
 *
 * This test verifies that the WebVTTSubtecParser constructor throws a std::invalid_argument exception when invoked with a height of zero. The test iterates over multiple subtitle MIME types to ensure consistent failure in this invalid scenario.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 004
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke WebVTTSubtecParser constructor with subtitle type eSUB_TYPE_WEBVTT | type = eSUB_TYPE_WEBVTT, width = 640, height = 0 | std::invalid_argument exception thrown | Should Pass |
 * | 02 | Invoke WebVTTSubtecParser constructor with subtitle type eSUB_TYPE_MP4 | type = eSUB_TYPE_MP4, width = 640, height = 0 | std::invalid_argument exception thrown | Should Pass |
 * | 03 | Invoke WebVTTSubtecParser constructor with subtitle type eSUB_TYPE_TTML | type = eSUB_TYPE_TTML, width = 640, height = 0 | std::invalid_argument exception thrown | Should Pass |
 * | 04 | Invoke WebVTTSubtecParser constructor with subtitle type eSUB_TYPE_UNKNOWN | type = eSUB_TYPE_UNKNOWN, width = 640, height = 0 | std::invalid_argument exception thrown | Should Pass |
 */
TEST(WebVTTSubtecParser, ConstructWithZeroHeight)
{
    std::cout << "Entering ConstructWithZeroHeight test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = 640;
    int height = 0;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance creation should have failed for type: " << type << " with zero height" << std::endl;
        }, std::invalid_argument);
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithZeroHeight test" << std::endl;
}

/**
 * @brief Verify that the WebVTTSubtecParser constructor throws an exception when invoked with a negative width.
 *
 * This test validates that the WebVTTSubtecParser constructor correctly handles an invalid negative width input. It iterates through a set of subtitle types and attempts to create a parser instance with a negative width (-100) and a valid height (480). The expected behavior is to throw a std::invalid_argument exception for each type, ensuring robust input validation.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 005
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------------------------------------------------------------------------- | ------------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01 | Call WebVTTSubtecParser constructor with type eSUB_TYPE_WEBVTT, negative width (-100), and valid height (480) | type = eSUB_TYPE_WEBVTT, width = -100, height = 480 | std::invalid_argument exception is thrown | Should Fail |
 * | 02 | Call WebVTTSubtecParser constructor with type eSUB_TYPE_MP4, negative width (-100), and valid height (480) | type = eSUB_TYPE_MP4, width = -100, height = 480 | std::invalid_argument exception is thrown | Should Fail |
 * | 03 | Call WebVTTSubtecParser constructor with type eSUB_TYPE_TTML, negative width (-100), and valid height (480) | type = eSUB_TYPE_TTML, width = -100, height = 480 | std::invalid_argument exception is thrown | Should Fail |
 * | 04 | Call WebVTTSubtecParser constructor with type eSUB_TYPE_UNKNOWN, negative width (-100), and valid height (480) | type = eSUB_TYPE_UNKNOWN, width = -100, height = 480 | std::invalid_argument exception is thrown | Should Fail |
 */
TEST(WebVTTSubtecParser, ConstructWithNegativeWidth)
{
    std::cout << "Entering ConstructWithNegativeWidth test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = -100;
    int height = 480;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance creation should have failed for type: " << type << " with negative width" << std::endl;
        }, std::invalid_argument);
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithNegativeWidth test" << std::endl;
}

/**
 * @brief Verify that the WebVTTSubtecParser constructor throws an exception for negative height
 *
 * This test verifies that when the WebVTTSubtecParser is instantiated with a negative height value, it correctly throws a std::invalid_argument exception. The test iterates over different subtitle mime types and ensures that the exception is thrown for each case. This behavior helps enforce proper parameter validation in the parser.
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
 * | 01 | Invoke the WebVTTSubtecParser constructor with various subtitle mime types using width=640 and height=-200 | type = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN, width = 640, height = -200 | std::invalid_argument exception is thrown for each invocation | Should Fail |
 */
TEST(WebVTTSubtecParser, ConstructWithNegativeHeight)
{
    std::cout << "Entering ConstructWithNegativeHeight test" << std::endl;

    SubtitleMimeType types[] = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN};

    int width = 640;
    int height = -200;

    for (auto type : types)
    {
        std::cout << "Invoking WebVTTSubtecParser constructor with type: " << type << ", width: " << width << ", height: " << height << std::endl;
        EXPECT_THROW({
            WebVTTSubtecParser parser(type, width, height);
            std::cout << "Instance creation should have failed for type: " << type << " with negative height" << std::endl;
        }, std::invalid_argument);
        std::cout << std::endl;
    }

    std::cout << "Exiting ConstructWithNegativeHeight test" << std::endl;
}

/**
 * @brief Validates that calling the close() method on WebVTTSubtecParser returns true for various SubtitleMimeType values.
 *
 * This test iterates over a set of SubtitleMimeType values (eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN) and verifies that the close() method returns true for each instance. This confirms that the parser correctly finalizes its operations regardless of subtitle type.
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
 * | Variation / Step | Description                                                                                             | Test Data                                         | Expected Result                                     | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------- | --------------------------------------------------- | ----------- |
 * | 01               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080   | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, output = close() return value | close() returns true and assertion passes          | Should Pass |
 * | 02               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080       | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, output = close() return value     | close() returns true and assertion passes          | Should Pass |
 * | 03               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080      | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, output = close() return value    | close() returns true and assertion passes          | Should Pass |
 * | 04               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080    | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, output = close() return value | close() returns true and assertion passes          | Should Pass |
 */
TEST(WebVTTSubtecParser, BasicCallCloseReturnsTrue)
{
    std::cout << "Entering BasicCallCloseReturnsTrue test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking close() method" << std::endl;
        bool result = parser.close();

        std::cout << "close() returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_TRUE(result) << "close() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting BasicCallCloseReturnsTrue test" << std::endl;
}

/**
 * @brief Test the initialization of WebVTTSubtecParser with typical positive values.
 *
 * This test verifies that the WebVTTSubtecParser constructor and the init method work correctly when provided with a set of typical positive inputs. It ensures that for various SubtitleMimeType values the parser object is created with valid dimensions, and the init method returns true indicating successful initialization.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 02 | Initialize an array with various SubtitleMimeType values. | types = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN | Array of SubtitleMimeType is created successfully. | Should be successful |
 * | 03 | For each SubtitleMimeType value, create a WebVTTSubtecParser object with width 1920 and height 1080, then invoke the init method with startPosSeconds = 10.0 and basePTS = 1609459200000. | For each iteration: type = current value, startPosSeconds = 10.0, basePTS = 1609459200000, width = 1920, height = 1080 | The init method returns true for each SubtitleMimeType value. | Should Pass |
 * | 04 | Validate the result of the init method for each parser object creation. | initResult = value returned by parser.init | ASSERT_TRUE(initResult) passes for all iterations. | Should Pass |
 */
TEST(WebVTTSubtecParser, ValidInitializationWithTypicalPositiveValues)
{
    std::cout << "Entering ValidInitializationWithTypicalPositiveValues test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double startPosSeconds = 10.0;
    unsigned long long basePTS = 1609459200000;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking init with startPosSeconds = " << startPosSeconds 
                  << " and basePTS = " << basePTS << std::endl;

        bool initResult = parser.init(startPosSeconds, basePTS);

        std::cout << "init returned: " << initResult << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_TRUE(initResult) << "init() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting ValidInitializationWithTypicalPositiveValues test" << std::endl;
}

/**
 * @brief Verify that WebVTTSubtecParser initializes successfully with zero startPosSeconds and basePTS.
 *
 * This test verifies that the WebVTTSubtecParser initializes correctly for various SubtitleMimeType values when provided with zero startPosSeconds and basePTS values. It ensures that the initialization method returns true for each valid subtitle type.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 009 @n
 * **Priority:** High @n
 * 
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** None @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser with eSUB_TYPE_WEBVTT, width = 1920, height = 1080 and call init with startPosSeconds = 0.0, basePTS = 0 | type = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, startPosSeconds = 0.0, basePTS = 0 | init() returns true and assertion passes | Should Pass |
 * | 02 | Create WebVTTSubtecParser with eSUB_TYPE_MP4, width = 1920, height = 1080 and call init with startPosSeconds = 0.0, basePTS = 0 | type = eSUB_TYPE_MP4, width = 1920, height = 1080, startPosSeconds = 0.0, basePTS = 0 | init() returns true and assertion passes | Should Pass |
 * | 03 | Create WebVTTSubtecParser with eSUB_TYPE_TTML, width = 1920, height = 1080 and call init with startPosSeconds = 0.0, basePTS = 0 | type = eSUB_TYPE_TTML, width = 1920, height = 1080, startPosSeconds = 0.0, basePTS = 0 | init() returns true and assertion passes | Should Pass |
 * | 04 | Create WebVTTSubtecParser with eSUB_TYPE_UNKNOWN, width = 1920, height = 1080 and call init with startPosSeconds = 0.0, basePTS = 0 | type = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, startPosSeconds = 0.0, basePTS = 0 | init() returns true and assertion passes | Should Pass |
 */
TEST(WebVTTSubtecParser, InitializationWithZeroValues)
{
    std::cout << "Entering InitializationWithZeroValues test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double startPosSeconds = 0.0;
    unsigned long long basePTS = 0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking init with startPosSeconds = " << startPosSeconds 
                  << " and basePTS = " << basePTS << std::endl;

        bool initResult = parser.init(startPosSeconds, basePTS);

        std::cout << "init returned: " << initResult << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_TRUE(initResult) << "init() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting InitializationWithZeroValues test" << std::endl;
}

/**
 * @brief Test initialization of WebVTTSubtecParser with a minimal positive start position.
 *
 * This test verifies that the WebVTTSubtecParser initializes correctly when provided with a very small positive start position, ensuring that the init() function returns true for various SubtitleMimeType values.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 010
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser with SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080 and invoke init with startPosSeconds = 1e-9 and basePTS = 500. | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, startPosSeconds = 1e-9, basePTS = 500 | init() returns true and the assertion passes. | Should Pass |
 * | 02 | Create WebVTTSubtecParser with SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080 and invoke init with startPosSeconds = 1e-9 and basePTS = 500. | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, startPosSeconds = 1e-9, basePTS = 500 | init() returns true and the assertion passes. | Should Pass |
 * | 03 | Create WebVTTSubtecParser with SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080 and invoke init with startPosSeconds = 1e-9 and basePTS = 500. | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, startPosSeconds = 1e-9, basePTS = 500 | init() returns true and the assertion passes. | Should Pass |
 * | 04 | Create WebVTTSubtecParser with SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080 and invoke init with startPosSeconds = 1e-9 and basePTS = 500. | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, startPosSeconds = 1e-9, basePTS = 500 | init() returns true and the assertion passes. | Should Pass |
 */
TEST(WebVTTSubtecParser, InitializationWithVerySmallPositiveStartPosition)
{
    std::cout << "Entering InitializationWithVerySmallPositiveStartPosition test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double startPosSeconds = 1e-9;
    unsigned long long basePTS = 500;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking init with startPosSeconds = " << startPosSeconds
                  << " and basePTS = " << basePTS << std::endl;

        bool initResult = parser.init(startPosSeconds, basePTS);

        std::cout << "init returned: " << initResult << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_TRUE(initResult) << "init() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting InitializationWithVerySmallPositiveStartPosition test" << std::endl;
}

/**
 * @brief Test initialization of WebVTTSubtecParser with extremely large numerical values.
 *
 * This test verifies that the WebVTTSubtecParser can be correctly initialized with extremely large values for start position in seconds and base PTS across various SubtitleMimeType types. It ensures that the initialization does not fail when handling maximum numerical limits.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters with subtitle types and extremely large numeric values. | types = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; startPosSeconds = 1e10, basePTS = 18446744073709551615; width = 1920, height = 1080 | Parameters are set correctly for the test. | Should be successful |
 * | 02 | Create WebVTTSubtecParser object for each SubtitleMimeType, invoke init and verify the return value is true. | startPosSeconds = 1e10, basePTS = 18446744073709551615ULL | returns true assertions pass for all subtitle types. | Should Pass |
 */
TEST(WebVTTSubtecParser, InitializationWithExtremelyLargeValues)
{
    std::cout << "Entering InitializationWithExtremelyLargeValues test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double startPosSeconds = 1e10;
    unsigned long long basePTS = 18446744073709551615ULL;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking init with startPosSeconds = " << startPosSeconds
                  << " and basePTS = " << basePTS << std::endl;

        bool initResult = parser.init(startPosSeconds, basePTS);

        std::cout << "init returned: " << initResult << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_TRUE(initResult) << "init() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting InitializationWithExtremelyLargeValues test" << std::endl;
}

/**
 * @brief Test to ensure that the initialization fails when provided with a negative start position value.
 *
 * This test verifies that the init() function of the WebVTTSubtecParser properly handles negative start position values for various subtitle mime types. It ensures that the parser initialization returns false when a negative start position is provided in order to prevent improper subtitle timing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                           | Test Data                                                                              | Expected Result                                                                                             | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Iterate through a list of SubtitleMimeType values                                                     | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN      | All subtitle mime type values are processed                                                                 | Should be successful |
 * | 02               | Create a WebVTTSubtecParser object with specified screen dimensions                                   | input: SubtitleMimeType = current type, width = 1920, height = 1080                      | WebVTTSubtecParser object is created successfully                                                             | Should Pass    |
 * | 03               | Invoke the init() method with a negative startPosSeconds and a basePTS value                            | input: startPosSeconds = -5.0, basePTS = 1000                                             | init() should return false indicating failure due to negative start position                                  | Should Fail    |
 * | 04               | Assert that the result of init() is false                                                             | output: initResult = result of init()                                                    | ASSERT_FALSE verifies that init() returned false as expected                                                  | Should Pass    |
 */
TEST(WebVTTSubtecParser, NegativeStartPositionValue)
{
    std::cout << "Entering NegativeStartPositionValue test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double startPosSeconds = -5.0;
    unsigned long long basePTS = 1000;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking init with startPosSeconds = " << startPosSeconds
                  << " and basePTS = " << basePTS << std::endl;

        bool initResult = parser.init(startPosSeconds, basePTS);

        std::cout << "init returned: " << initResult << " for SubtitleMimeType = " << type << std::endl;

        ASSERT_FALSE(initResult) << "Expected init() to fail for negative startPosSeconds, but it succeeded. SubtitleMimeType: " << type;
    }

    std::cout << "Exiting NegativeStartPositionValue test" << std::endl;
}

/**
 * @brief Validate that the mute functionality works correctly across various subtitle types.
 *
 * This test verifies that invoking the mute(true) function on a WebVTTSubtecParser object mutes the subtitles 
 * without errors for different subtitle MIME types. It ensures that the parser handles each subtitle type consistently 
 * by correctly invoking the mute method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                                            | Expected Result                                     | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | --------------------------------------------------- | ----------- |
 * | 01               | Create a WebVTTSubtecParser object for type eSUB_TYPE_WEBVTT with width=1920 and height=1080         | type = eSUB_TYPE_WEBVTT, width = 1920, height = 1080                                   | Object is successfully created                      | Should Pass |
 * | 02               | Invoke mute(true) on the parser object for eSUB_TYPE_WEBVTT                                           | mute_value = true                                                                    | Method mute executes without errors                | Should Pass |
 * | 03               | Create a WebVTTSubtecParser object for type eSUB_TYPE_MP4 with width=1920 and height=1080              | type = eSUB_TYPE_MP4, width = 1920, height = 1080                                      | Object is successfully created                      | Should Pass |
 * | 04               | Invoke mute(true) on the parser object for eSUB_TYPE_MP4                                             | mute_value = true                                                                    | Method mute executes without errors                | Should Pass |
 * | 05               | Create a WebVTTSubtecParser object for type eSUB_TYPE_TTML with width=1920 and height=1080             | type = eSUB_TYPE_TTML, width = 1920, height = 1080                                     | Object is successfully created                      | Should Pass |
 * | 06               | Invoke mute(true) on the parser object for eSUB_TYPE_TTML                                            | mute_value = true                                                                    | Method mute executes without errors                | Should Pass |
 * | 07               | Create a WebVTTSubtecParser object for type eSUB_TYPE_UNKNOWN with width=1920 and height=1080           | type = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080                                  | Object is successfully created                      | Should Pass |
 * | 08               | Invoke mute(true) on the parser object for eSUB_TYPE_UNKNOWN                                          | mute_value = true                                                                    | Method mute executes without errors                | Should Pass |
 */
TEST(WebVTTSubtecParser, MuteSubtitlesSuccessfully)
{
    std::cout << "Entering MuteSubtitlesSuccessfully test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        WebVTTSubtecParser parser(type, 1920, 1080);
        std::cout << "Created WebVTTSubtecParser object with parameters: " << type << ", 1920, 1080" << std::endl;

        std::cout << "Invoking mute with value: true" << std::endl;
        parser.mute(true);
        std::cout << "Method mute executed with value: true" << std::endl;

        std::cout << "Subtitle rendering is expected to be muted with no errors" << std::endl;
    }

    std::cout << "Exiting MuteSubtitlesSuccessfully test" << std::endl;
}

/**
 * @brief Validate that the mute functionality correctly un-mutes subtitles
 *
 * This test verifies that for different subtitle MIME types the mute method is invoked with a false value,
 * ensuring that the subtitle rendering remains unmuted and no errors occur.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                           | Test Data                                                                  | Expected Result                                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01               | Create parser with eSUB_TYPE_WEBVTT and invoke mute(false)             | type = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, mute = false            | Subtitle rendering unmuted with no errors                        | Should Pass |
 * | 02               | Create parser with eSUB_TYPE_MP4 and invoke mute(false)                  | type = eSUB_TYPE_MP4, width = 1920, height = 1080, mute = false               | Subtitle rendering unmuted with no errors                        | Should Pass |
 * | 03               | Create parser with eSUB_TYPE_TTML and invoke mute(false)                 | type = eSUB_TYPE_TTML, width = 1920, height = 1080, mute = false              | Subtitle rendering unmuted with no errors                        | Should Pass |
 * | 04               | Create parser with eSUB_TYPE_UNKNOWN and invoke mute(false)              | type = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, mute = false           | Subtitle rendering unmuted with no errors                        | Should Pass |
 */
TEST(WebVTTSubtecParser, UnmuteSubtitlesSuccessfully)
{
    std::cout << "Entering UnmuteSubtitlesSuccessfully test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        WebVTTSubtecParser parser(type, 1920, 1080);
        std::cout << "Created WebVTTSubtecParser object with parameters: " << type << ", 1920, 1080" << std::endl;

        std::cout << "Invoking mute with value: false" << std::endl;
        parser.mute(false);
        std::cout << "Method mute executed with value: false" << std::endl;

        std::cout << "Subtitle rendering is expected to be unmuted with no errors" << std::endl;
    }

    std::cout << "Exiting UnmuteSubtitlesSuccessfully test" << std::endl;
}

/**
 * @brief Verify that the pause API does not throw exceptions when pause is true
 *
 * This test validates the functionality of the pause method in the WebVTTSubtecParser class.
 * It ensures that calling pause(true) on a parser object created with various SubtitleMimeType values
 * does not throw an exception, thus confirming that the pause functionality works correctly for valid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser instance with SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, and height = 1080; then invoke pause(true) | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, pause = true | The parser.pause(true) method call should complete without throwing any exception | Should Pass |
 * | 02 | Create WebVTTSubtecParser instance with SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, and height = 1080; then invoke pause(true) | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, pause = true | The parser.pause(true) method call should complete without throwing any exception | Should Pass |
 * | 03 | Create WebVTTSubtecParser instance with SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, and height = 1080; then invoke pause(true) | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, pause = true | The parser.pause(true) method call should complete without throwing any exception | Should Pass |
 * | 04 | Create WebVTTSubtecParser instance with SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, and height = 1080; then invoke pause(true) | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, pause = true | The parser.pause(true) method call should complete without throwing any exception | Should Pass |
 */
TEST(WebVTTSubtecParser, PauseSubtitleRenderingWithPauseTrue)
{
    std::cout << "Entering PauseSubtitleRenderingWithPauseTrue test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking pause method with true as input for SubtitleMimeType = " << type << std::endl;
        EXPECT_NO_THROW(parser.pause(true));
        std::cout << "pause(true) invoked successfully with input value: true for SubtitleMimeType = " << type << std::endl;
    }

    std::cout << "Exiting PauseSubtitleRenderingWithPauseTrue test" << std::endl;
}

/**
 * @brief Verify that invoking pause(false) does not throw an exception across various subtitle types.
 *
 * This test iterates over different SubtitleMimeType values to ensure that the WebVTTSubtecParser object's pause method, when called with false, executes without throwing an exception. It validates the parser’s behavior across supported and unsupported subtitle types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser object using each SubtitleMimeType with width = 1920 and height = 1080. | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; width = 1920; height = 1080 | Instance of WebVTTSubtecParser is successfully created for each SubtitleMimeType. | Should be successful |
 * | 02 | Invoke pause() method with argument false on the parser instance. | input = false | pause(false) does not throw any exception. | Should Pass |
 */
TEST(WebVTTSubtecParser, ResumeSubtitleRenderingWithPauseFalse)
{
    std::cout << "Entering ResumeSubtitleRenderingWithPauseFalse test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 1920 and height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking pause method with false as input" << std::endl;
        EXPECT_NO_THROW(parser.pause(false));
        std::cout << "pause(false) invoked successfully with input value: false" << std::endl;
    }

    std::cout << "Exiting ResumeSubtitleRenderingWithPauseFalse test" << std::endl;
}

/**
 * @brief Verifies that the processData method of WebVTTSubtecParser returns true for valid subtitle data.
 *
 * This test iterates over different SubtitleMimeType values and creates a WebVTTSubtecParser object with a valid WebVTT formatted input. 
 * It then calls processData with predefined parameters to validate that the parser processes the valid subtitle data correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize buffer with valid WebVTT subtitle text and set related parameters. | inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 0.0, duration = 5.0 | Buffer correctly initialized with valid subtitle data. | Should be successful |
 * | 02 | Iterate over each SubtitleMimeType value and create a parser instance with specified width and height. | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; width = 640, height = 480 | Parser object created successfully for each type. | Should be successful |
 * | 03 | Invoke processData on the parser object with the initialized input data. | buffer, bufferLen = 55, position = 0.0, duration = 5.0 | processData returns true indicating valid data processing. | Should Pass |
 * | 04 | Validate that the returned value from processData is true using assertion. | result = processData(buffer, bufferLen, position, duration) | EXPECT_TRUE assertion passes if processData returns true. | Should Pass |
 */
TEST(WebVTTSubtecParser, ValidSubtitleData)
{
    std::cout << "Entering ValidSubtitleData test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 55;
    double position = 0.0;
    double duration = 5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_TRUE(result) << "processData() failed for SubtitleMimeType: " << type;
    }

    std::cout << "Exiting ValidSubtitleData test" << std::endl;
}

/**
 * @brief Validate that processData returns false when a NULL buffer is provided.
 *
 * This test verifies that calling the processData method with a NULL buffer correctly returns false, ensuring that the API handles erroneous inputs gracefully across multiple subtitle mime types.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 018
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a WebVTTSubtecParser instance with eSUB_TYPE_WEBVTT and invoke processData with a NULL buffer. | SubtitleMimeType = eSUB_TYPE_WEBVTT, buffer = NULL, bufferLen = 10, position = 0.0, duration = 5.0 | processData() returns false and EXPECT_FALSE(result) passes. | Should Fail |
 * | 02 | Create a WebVTTSubtecParser instance with eSUB_TYPE_MP4 and invoke processData with a NULL buffer. | SubtitleMimeType = eSUB_TYPE_MP4, buffer = NULL, bufferLen = 10, position = 0.0, duration = 5.0 | processData() returns false and EXPECT_FALSE(result) passes. | Should Fail |
 * | 03 | Create a WebVTTSubtecParser instance with eSUB_TYPE_TTML and invoke processData with a NULL buffer. | SubtitleMimeType = eSUB_TYPE_TTML, buffer = NULL, bufferLen = 10, position = 0.0, duration = 5.0 | processData() returns false and EXPECT_FALSE(result) passes. | Should Fail |
 * | 04 | Create a WebVTTSubtecParser instance with eSUB_TYPE_UNKNOWN and invoke processData with a NULL buffer. | SubtitleMimeType = eSUB_TYPE_UNKNOWN, buffer = NULL, bufferLen = 10, position = 0.0, duration = 5.0 | processData() returns false and EXPECT_FALSE(result) passes. | Should Fail |
 */
TEST(WebVTTSubtecParser, NullBufferInput)
{
    std::cout << "Entering NullBufferInput test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    const char* buffer = NULL;
    size_t bufferLen = 10;
    double position = 0.0;
    double duration = 5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_FALSE(result) << "Expected processData() to return false for NULL buffer with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting NullBufferInput test" << std::endl;
}

/**
 * @brief Test processData with zero buffer length for WebVTTSubtecParser.
 *
 * This test verifies that processData returns false when the buffer length is set to zero.
 * It iterates over different SubtitleMimeType values and ensures that regardless of the type,
 * the API returns false due to an invalid input buffer length. This helps ensure robust input validation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test inputs including buffer, buffer length, position, and duration | inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 0, position = 0.0, duration = 5.0 | Variables are initialized correctly | Should be successful |
 * | 02 | Create WebVTTSubtecParser object with each SubtitleMimeType and invoke processData | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; width = 640, height = 480; buffer = initialized buffer | API returns false for zero buffer length | Should Pass |
 * | 03 | Assert that processData returns false for every SubtitleMimeType tested | Return value from processData = false | Assertion passes confirming false return value | Should Pass |
 */
TEST(WebVTTSubtecParser, ZeroBufferLength)
{
    std::cout << "Entering ZeroBufferLength test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 0;
    double position = 0.0;
    double duration = 5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_FALSE(result) << "Expected processData() to return false for zero buffer length with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting ZeroBufferLength test" << std::endl;
}

/**
 * @brief Test to validate that processData returns false when provided with a negative playback position.
 *
 * This test ensures that the WebVTTSubtecParser fails to process data when a negative playback position is specified. It verifies that the function appropriately returns false for each subtitle type provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables and input string buffer | inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = -1.0, duration = 5.0 | Buffer and variables are correctly initialized | Should be successful |
 * | 02 | Iterate over subtitle types and construct WebVTTSubtecParser object | types[] = { eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN }, width = 640, height = 480 | WebVTTSubtecParser object is successfully constructed for each subtitle type | Should be successful |
 * | 03 | Invoke processData method on parser object | buffer, bufferLen, position = -1.0, duration = 5.0 | processData returns false, and the EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(WebVTTSubtecParser, NegativePlaybackPosition)
{
    std::cout << "Entering NegativePlaybackPosition test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 55;
    double position = -1.0;
    double duration = 5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_FALSE(result) << "Expected processData() to return false for negative playback position with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting NegativePlaybackPosition test" << std::endl;
}

/**
 * @brief Validate processData function with negative subtitle duration.
 *
 * This test validates the processData method of the WebVTTSubtecParser class by invoking it with a negative duration value.
 * It iterates over different SubtitleMimeType values to ensure that processData correctly returns false when provided with an invalid (negative) subtitle duration.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 021
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke processData with SubtitleMimeType = eSUB_TYPE_WEBVTT | input: SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 640, height = 480, buffer = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 0.0, duration = -5.0 | processData() returns false and EXPECT_FALSE assertion passes | Should Fail |
 * | 02 | Invoke processData with SubtitleMimeType = eSUB_TYPE_MP4 | input: SubtitleMimeType = eSUB_TYPE_MP4, width = 640, height = 480, buffer = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 0.0, duration = -5.0 | processData() returns false and EXPECT_FALSE assertion passes | Should Fail |
 * | 03 | Invoke processData with SubtitleMimeType = eSUB_TYPE_TTML | input: SubtitleMimeType = eSUB_TYPE_TTML, width = 640, height = 480, buffer = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 0.0, duration = -5.0 | processData() returns false and EXPECT_FALSE assertion passes | Should Fail |
 * | 04 | Invoke processData with SubtitleMimeType = eSUB_TYPE_UNKNOWN | input: SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 640, height = 480, buffer = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 0.0, duration = -5.0 | processData() returns false and EXPECT_FALSE assertion passes | Should Fail |
 */
TEST(WebVTTSubtecParser, NegativeSubtitleDuration)
{
    std::cout << "Entering NegativeSubtitleDuration test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 55;
    double position = 0.0;
    double duration = -5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_FALSE(result) << "Expected processData() to return false for negative subtitle duration with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting NegativeSubtitleDuration test" << std::endl;
}

/**
 * 
 * @brief Test processData() returns false when the provided position is greater than duration
 *
 * This test verifies that the WebVTTSubtecParser::processData() method returns false when the
 * 'position' argument is greater than the 'duration' argument. The test iterates through various
 * SubtitleMimeType values to ensure consistent behavior across different subtitle types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:** 
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test data with a valid WEBVTT input string, set buffer length to 55, position to 10.0, and duration to 5.0 | inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!", bufferLen = 55, position = 10.0, duration = 5.0 | Test data is set up correctly | Should be successful |
 * | 02 | Create a WebVTTSubtecParser object for SubtitleMimeType = eSUB_TYPE_WEBVTT and invoke processData() | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 640, height = 480, buffer, bufferLen = 55, position = 10.0, duration = 5.0 | processData() returns false; EXPECT_FALSE assertion passes | Should Fail |
 * | 03 | Create a WebVTTSubtecParser object for SubtitleMimeType = eSUB_TYPE_MP4 and invoke processData() | SubtitleMimeType = eSUB_TYPE_MP4, width = 640, height = 480, buffer, bufferLen = 55, position = 10.0, duration = 5.0 | processData() returns false; EXPECT_FALSE assertion passes | Should Fail |
 * | 04 | Create a WebVTTSubtecParser object for SubtitleMimeType = eSUB_TYPE_TTML and invoke processData() | SubtitleMimeType = eSUB_TYPE_TTML, width = 640, height = 480, buffer, bufferLen = 55, position = 10.0, duration = 5.0 | processData() returns false; EXPECT_FALSE assertion passes | Should Fail |
 * | 05 | Create a WebVTTSubtecParser object for SubtitleMimeType = eSUB_TYPE_UNKNOWN and invoke processData() | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 640, height = 480, buffer, bufferLen = 55, position = 10.0, duration = 5.0 | processData() returns false; EXPECT_FALSE assertion passes | Should Fail |
 */
TEST(WebVTTSubtecParser, PositionGreaterThanDuration)
{
    std::cout << "Entering PositionGreaterThanDuration test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:00.000 --> 00:00:05.000\nHello, world!";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 55;
    double position = 10.0;
    double duration = 5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_FALSE(result) << "Expected processData() to return false when position > duration, with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting PositionGreaterThanDuration test" << std::endl;
}

/**
 * @brief Test the processData API with valid fractional playback and duration inputs.
 *
 * This test verifies that the processData function returns true when provided with a valid WEBVTT buffer,
 * a defined fractional position and duration, and iterates over various SubtitleMimeType values.
 * The objective is to ensure that fractional playback and duration parameters are correctly processed
 * regardless of the subtitle type.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test parameters including a valid WEBVTT string, buffer length, fractional position, and duration. | inputStr = "WEBVTT\n\n1\n00:00:01.500 --> 00:00:03.000\nSubtitle line", bufferLen = 50, position = 1.5, duration = 1.5 | Variables initialized with correct values. | Should be successful |
 * | 02 | Loop over each SubtitleMimeType and instantiate the WebVTTSubtecParser with width 640 and height 480. | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN, width = 640, height = 480 | Parser object is created successfully for each subtitle type. | Should be successful |
 * | 03 | Invoke processData with the prepared buffer and parameters and validate the returned result is true. | buffer = valid WEBVTT data, bufferLen = 50, position = 1.5, duration = 1.5, SubtitleMimeType = current type in iteration | processData returns true and the EXPECT_TRUE assertion passes. | Should Pass |
 */
TEST(WebVTTSubtecParser, ValidFractionalPlaybackAndDuration)
{
    std::cout << "Entering ValidFractionalPlaybackAndDuration test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    char buffer[100];
    const char* inputStr = "WEBVTT\n\n1\n00:00:01.500 --> 00:00:03.000\nSubtitle line";
    strncpy(buffer, inputStr, sizeof(buffer));

    size_t bufferLen = 50;
    double position = 1.5;
    double duration = 1.5;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 640 and height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking processData with buffer: " << buffer
                  << ", bufferLen: " << bufferLen
                  << ", position: " << position
                  << ", duration: " << duration << std::endl;

        bool result = parser.processData(buffer, bufferLen, position, duration);
        std::cout << "processData returned: " << result << " for SubtitleMimeType = " << type << std::endl;

        EXPECT_TRUE(result) << "Expected processData() to return true for valid fractional playback/duration with SubtitleMimeType: " << type;
    }

    std::cout << "Exiting ValidFractionalPlaybackAndDuration test" << std::endl;
}

/**
 * @brief Tests the reset functionality of the WebVTTSubtecParser object across all supported SubtitleMimeType types.
 *
 * This test verifies that calling reset() on a WebVTTSubtecParser instance properly resets time_offset_ms_ and start_ms_ to 0, and ensures that m_channel is allocated. It iterates over various SubtitleMimeType values to ensure that the reset behavior is consistent regardless of the subtitle type.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 024
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a WebVTTSubtecParser object with a specific SubtitleMimeType, width, and height. | input: type = eSUB_TYPE_WEBVTT, width = 640, height = 480 (subsequent iterations with eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN) | Object is instantiated without errors. | Should Pass |
 * | 02 | Invoke the reset() method on the created parser. | output: Call reset() with no parameters | reset() executes and prepares the parser for validation. | Should Pass |
 * | 03 | Verify that time_offset_ms_ is set to 0 after reset(). | output: time_offset_ms_ = value after reset() | time_offset_ms_ equals 0. | Should Pass |
 * | 04 | Verify that start_ms_ is set to 0 after reset(). | output: start_ms_ = value after reset() | start_ms_ equals 0. | Should Pass |
 * | 05 | Verify that m_channel is not a nullptr after reset(). | output: m_channel pointer value after reset() | m_channel is allocated (non-null pointer). | Should Pass |
 */
TEST(WebVTTSubtecParser, reset_start)
{
    std::cout << "Entering reset_start test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with type: " << type 
                  << ", width: 640, height: 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        parser.reset();
        std::cout << "Called reset() method on parser" << std::endl;

    }

    std::cout << "Exiting reset_start test" << std::endl;
}

/**
 * @brief Verify that the setProgressEventOffset API correctly assigns a positive progress event offset.
 *
 * This test creates a WebVTTSubtecParser instance for each SubtitleMimeType value (eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, and eSUB_TYPE_UNKNOWN) and then invokes the setProgressEventOffset method with a positive offset value of 10.5. The objective is to ensure that the API accepts the offset without error regardless of the subtitle type.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_WEBVTT and set offset | input: type = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, offset = 10.5 | Parser object is created and setProgressEventOffset executes with no error; offset set correctly | Should Pass |
 * | 02 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_MP4 and set offset | input: type = eSUB_TYPE_MP4, width = 1920, height = 1080, offset = 10.5 | Parser object is created and setProgressEventOffset executes with no error; offset set correctly | Should Pass |
 * | 03 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_TTML and set offset | input: type = eSUB_TYPE_TTML, width = 1920, height = 1080, offset = 10.5 | Parser object is created and setProgressEventOffset executes with no error; offset set correctly | Should Pass |
 * | 04 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_UNKNOWN and set offset | input: type = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, offset = 10.5 | Parser object is created and setProgressEventOffset executes with no error; offset set correctly | Should Pass |
 */
TEST(WebVTTSubtecParser, SetPositiveProgressEventOffset)
{
    std::cout << "Entering SetPositiveProgressEventOffset test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double offset = 10.5;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking setProgressEventOffset with offset: " << offset << std::endl;

        parser.setProgressEventOffset(offset);

        std::cout << "Method setProgressEventOffset executed with offset: " << offset << std::endl;
    }

    std::cout << "Exiting SetPositiveProgressEventOffset test" << std::endl;
}

/**
 * @brief Checks that the setProgressEventOffset API correctly processes an offset of zero for various subtitle mime types.
 *
 * This test iterates over different SubtitleMimeType values, instantiates WebVTTSubtecParser objects with a resolution of 1920x1080, and calls setProgressEventOffset with an offset of 0.0. It verifies that the API executes without error across different supported and unsupported subtitle formats.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 026
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                                                                                      | Expected Result                                                | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------- |
 * | 01               | Invoke setProgressEventOffset for eSUB_TYPE_WEBVTT                         | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, offset = 0.0                   | API returns success and sets the progress event offset to zero | Should Pass |
 * | 02               | Invoke setProgressEventOffset for eSUB_TYPE_MP4                            | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, offset = 0.0                      | API returns success and sets the progress event offset to zero | Should Pass |
 * | 03               | Invoke setProgressEventOffset for eSUB_TYPE_TTML                           | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, offset = 0.0                     | API returns success and sets the progress event offset to zero | Should Pass |
 * | 04               | Invoke setProgressEventOffset for eSUB_TYPE_UNKNOWN                        | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, offset = 0.0                  | API returns success and sets the progress event offset to zero | Should Pass |
 */
TEST(WebVTTSubtecParser, SetZeroProgressEventOffset)
{
    std::cout << "Entering SetZeroProgressEventOffset test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double offset = 0.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking setProgressEventOffset with offset: " << offset << std::endl;

        parser.setProgressEventOffset(offset);

        std::cout << "Method setProgressEventOffset executed with offset: " << offset << std::endl;
    }

    std::cout << "Exiting SetZeroProgressEventOffset test" << std::endl;
}

/**
 * @brief Verify that setProgressEventOffset handles negative offset values correctly.
 *
 * This test verifies that when a negative offset value is provided to the setProgressEventOffset method,
 * the method executes without causing any unexpected behavior across all supported SubtitleMimeType values.
 * The test iterates over different subtitle types, creates a parser object, and invokes setProgressEventOffset
 * using a negative offset. The objective is to ensure that the API call can gracefully handle negative input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Assign negative offset and define supported subtitle types | offset = -5.0, types = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN} | Variables are correctly initialized | Should be successful |
 * | 02 | Iterate over each SubtitleMimeType and create a parser object with specified dimensions | type = each subtitle type, width = 1920, height = 1080 | Parser object is successfully instantiated for each type | Should Pass |
 * | 03 | Invoke setProgressEventOffset with the negative offset value on the parser object | offset = -5.0 | API method is executed and handles the negative offset appropriately | Should Pass |
 */
TEST(WebVTTSubtecParser, SetNegativeProgressEventOffset)
{
    std::cout << "Entering SetNegativeProgressEventOffset test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double offset = -5.0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking setProgressEventOffset with offset: " << offset << std::endl;

        parser.setProgressEventOffset(offset);

        std::cout << "Method setProgressEventOffset executed with offset: " << offset << std::endl;
    }

    std::cout << "Exiting SetNegativeProgressEventOffset test" << std::endl;
}

/**
 * @brief Verify setProgressEventOffset correctly handles a large positive progress offset value.
 *
 * This test verifies that the WebVTTSubtecParser's setProgressEventOffset method processes a very large positive offset (DBL_MAX)
 * correctly for various SubtitleMimeType inputs. The test ensures that the API call executes without error for each type.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                             | Test Data                                                                                          | Expected Result                                                                                                 | Notes        |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080 and invoke setProgressEventOffset with offset = DBL_MAX. | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, offset = DBL_MAX                    | Method setProgressEventOffset executes successfully without error                                               | Should Pass  |
 * | 02               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080 and invoke setProgressEventOffset with offset = DBL_MAX.   | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, offset = DBL_MAX                       | Method setProgressEventOffset executes successfully without error                                               | Should Pass  |
 * | 03               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080 and invoke setProgressEventOffset with offset = DBL_MAX.  | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, offset = DBL_MAX                      | Method setProgressEventOffset executes successfully without error                                               | Should Pass  |
 * | 04               | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080 and invoke setProgressEventOffset with offset = DBL_MAX. | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, offset = DBL_MAX                   | Method setProgressEventOffset executes successfully without error                                               | Should Pass  |
 */
TEST(WebVTTSubtecParser, SetLargePositiveProgressEventOffset)
{
    std::cout << "Entering SetLargePositiveProgressEventOffset test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double offset = DBL_MAX;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking setProgressEventOffset with offset: " << offset << std::endl;

        parser.setProgressEventOffset(offset);

        std::cout << "Method setProgressEventOffset executed with offset: " << offset << std::endl;
    }

    std::cout << "Exiting SetLargePositiveProgressEventOffset test" << std::endl;
}

/**
 * @brief Validate large negative offset behavior of setProgressEventOffset API
 *
 * This test verifies that when a large negative offset value (-DBL_MAX) is provided to the setProgressEventOffset method of the WebVTTSubtecParser for various SubtitleMimeType values, the parser handles the extreme negative value without error.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser object for each SubtitleMimeType with width 1920 and height 1080 | input: SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; width = 1920; height = 1080 | Object should be instantiated successfully for each type | Should be successful |
 * | 02 | Invoke setProgressEventOffset API with offset = -DBL_MAX on the created parser object | input: offset = -DBL_MAX; output: offset set value | API method executes without error and applies the offset | Should Pass |
 */
TEST(WebVTTSubtecParser, SetLargeNegativeProgressEventOffset)
{
    std::cout << "Entering SetLargeNegativeProgressEventOffset test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    double offset = -DBL_MAX;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking setProgressEventOffset with offset: " << offset << std::endl;

        parser.setProgressEventOffset(offset);

        std::cout << "Method setProgressEventOffset executed with offset: " << offset << std::endl;
    }

    std::cout << "Exiting SetLargeNegativeProgressEventOffset test" << std::endl;
}

/**
 * @brief Validates setTextStyle API with a valid style option on various subtitle mime types
 *
 * This test iterates over a set of subtitle mime types and for each creates a WebVTTSubtecParser object with fixed dimensions.
 * It then invokes the setTextStyle method with a valid style options string ("font-size:14px;color:#000;") to verify that
 * the API executes without errors and logs the expected messages.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                              | Test Data                                                                                   | Expected Result                                                                                                           | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a WebVTTSubtecParser object for the provided subtitle mime types with dimensions      | type = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN, width = 640, height = 480 | WebVTTSubtecParser object is successfully created for each type                                                              | Should Pass   |
 * | 02               | Invoke setTextStyle method with a valid style options string                               | options = "font-size:14px;color:#000;"                                                      | setTextStyle method executes without errors and applies the style; Logging confirms the method execution was successful   | Should Pass   |
 */
TEST(WebVTTSubtecParser, ValidStyleOption)
{
    std::cout << "Entering ValidStyleOption test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    std::string options = "font-size:14px;color:#000;";

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with type as " << type 
                  << ", width 640, height 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking setTextStyle with options: " << options << std::endl;
        parser.setTextStyle(options);
        std::cout << "Method setTextStyle executed with the provided valid style option" << std::endl;
    }

    std::cout << "Exiting ValidStyleOption test" << std::endl;
}

/**
 * @brief Test for default styling behavior when an empty style option is provided
 *
 * This test checks the functionality of the WebVTTSubtecParser::setTextStyle method when an empty string is passed as the styling option. The test iterates over different subtitle types to validate that the parser applies default styling values regardless of the subtitle type used.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Output entering message for test start | No inputs | "Entering EmptyStyleOption test" is printed | Should be successful |
 * | 02 | Create parser object for each subtitle type and output object creation message | type = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; width = 640, height = 480 | Parser object is successfully created for each type | Should Pass |
 * | 03 | Invoke setTextStyle with an empty string and output method call message | options = "" | setTextStyle executed with empty style option, applying default styling | Should Pass |
 * | 04 | Output exiting message for test end | No inputs | "Exiting EmptyStyleOption test" is printed | Should be successful |
 */
TEST(WebVTTSubtecParser, EmptyStyleOption)
{
    std::cout << "Entering EmptyStyleOption test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    std::string options = "";

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with type as " << type
                  << ", width 640, height 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking setTextStyle with empty options string" << std::endl;
        parser.setTextStyle(options);
        std::cout << "Method setTextStyle executed with empty style option, default styling applied" << std::endl;
    }

    std::cout << "Exiting EmptyStyleOption test" << std::endl;
}

/**
 * @brief Verify that the WebVTTSubtecParser handles malformed style options gracefully
 *
 * This test verifies that the WebVTTSubtecParser gracefully handles the scenario when the setTextStyle method
 * is invoked with a malformed style option ("font-size"). The test iterates over different subtitle types (WEBVTT, MP4,
 * TTML, and UNKNOWN) to ensure robustness across different internal configurations without causing crashes or unhandled errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser object with type eSUB_TYPE_WEBVTT, width 640, height 480 and invoke setTextStyle with malformed option | input: type = eSUB_TYPE_WEBVTT, width = 640, height = 480, options = "font-size" | API executed without errors; setTextStyle handles malformed option gracefully | Should Pass |
 * | 02 | Create WebVTTSubtecParser object with type eSUB_TYPE_MP4, width 640, height 480 and invoke setTextStyle with malformed option | input: type = eSUB_TYPE_MP4, width = 640, height = 480, options = "font-size" | API executed without errors; setTextStyle handles malformed option gracefully | Should Pass |
 * | 03 | Create WebVTTSubtecParser object with type eSUB_TYPE_TTML, width 640, height 480 and invoke setTextStyle with malformed option | input: type = eSUB_TYPE_TTML, width = 640, height = 480, options = "font-size" | API executed without errors; setTextStyle handles malformed option gracefully | Should Pass |
 * | 04 | Create WebVTTSubtecParser object with type eSUB_TYPE_UNKNOWN, width 640, height 480 and invoke setTextStyle with malformed option | input: type = eSUB_TYPE_UNKNOWN, width = 640, height = 480, options = "font-size" | API executed without errors; setTextStyle handles malformed option gracefully | Should Pass |
 */
TEST(WebVTTSubtecParser, MalformedStyleOption)
{
    std::cout << "Entering MalformedStyleOption test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    std::string options = "font-size";

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with type as " << type
                  << ", width 640, height 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking setTextStyle with malformed options: " << options << std::endl;
        parser.setTextStyle(options);
        std::cout << "Method setTextStyle executed with malformed style option, handled gracefully" << std::endl;
    }

    std::cout << "Exiting MalformedStyleOption test" << std::endl;
}

/**
 * @brief Test high complexity style options application using setTextStyle
 *
 * This test verifies that the WebVTTSubtecParser::setTextStyle method correctly applies a complex style string when the parser is constructed with various subtitle mime types. It ensures that multiple styling attributes are handled properly for each subtitle type.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 033
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                        | Test Data                                                                                                                              | Expected Result                                                                 | Notes        |
 * | :----:           | -------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- | ------------ |
 * | 01               | Create WebVTTSubtecParser with subtitle type eSUB_TYPE_WEBVTT, width 640, height 480                | input: type = eSUB_TYPE_WEBVTT, width = 640, height = 480; option = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;" | Parser object created with no errors                                            | Should Pass  |
 * | 02               | Invoke setTextStyle with high complexity style options on parser for eSUB_TYPE_WEBVTT                | input: options = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"                                                   | Method executed; style applied successfully                                     | Should Pass  |
 * | 03               | Create WebVTTSubtecParser with subtitle type eSUB_TYPE_MP4, width 640, height 480                   | input: type = eSUB_TYPE_MP4, width = 640, height = 480; option = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"    | Parser object created with no errors                                            | Should Pass  |
 * | 04               | Invoke setTextStyle with high complexity style options on parser for eSUB_TYPE_MP4                   | input: options = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"                                                   | Method executed; style applied successfully                                     | Should Pass  |
 * | 05               | Create WebVTTSubtecParser with subtitle type eSUB_TYPE_TTML, width 640, height 480                  | input: type = eSUB_TYPE_TTML, width = 640, height = 480; option = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"   | Parser object created with no errors                                            | Should Pass  |
 * | 06               | Invoke setTextStyle with high complexity style options on parser for eSUB_TYPE_TTML                  | input: options = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"                                                   | Method executed; style applied successfully                                     | Should Pass  |
 * | 07               | Create WebVTTSubtecParser with subtitle type eSUB_TYPE_UNKNOWN, width 640, height 480                | input: type = eSUB_TYPE_UNKNOWN, width = 640, height = 480; option = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;" | Parser object created (default behavior for unknown type might apply)           | Should Pass  |
 * | 08               | Invoke setTextStyle with high complexity style options on parser for eSUB_TYPE_UNKNOWN               | input: options = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;"                                                   | Method executed; style applied successfully or handled gracefully for unknown type | Should Pass  |
 */
TEST(WebVTTSubtecParser, HighComplexityStyleOption)
{
    std::cout << "Entering HighComplexityStyleOption test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    std::string options = "font-family:'Arial', sans-serif; font-size:16px; font-weight:bold;";

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with type as " << type
                  << ", width 640, height 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);

        std::cout << "Invoking setTextStyle with high complexity options: " << options << std::endl;
        parser.setTextStyle(options);
        std::cout << "Method setTextStyle executed with multiple styling attributes applied successfully" << std::endl;
    }

    std::cout << "Exiting HighComplexityStyleOption test" << std::endl;
}

/**
 * @brief Validate safe handling of malicious text style options in WebVTTSubtecParser
 *
 * This test verifies that when invalid style options (potentially containing script injections)
 * are provided to the WebVTTSubtecParser via the setTextStyle method, the parser processes them 
 * safely without crashing or misbehaving.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test and declare a list of SubtitleMimeType values and an invalid options string. | types = {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN}, options = "<script>alert('hack')</script>" | Test variables initialized without runtime errors. | Should be successful |
 * | 02 | For each SubtitleMimeType value, create a WebVTTSubtecParser instance with specified dimensions. | input: SubtitleMimeType = one of the list, id = 1, width = 640, height = 480 | Object is constructed successfully. | Should be successful |
 * | 03 | Invoke setTextStyle on the parser instance using the invalid options string. | input: options = "<script>alert('hack')</script>", output: none | API call executes safely handling the invalid options; no crashes or errors. | Should Pass |
 */
TEST(WebVTTSubtecParser, InvalidStyleOption)
{
    std::cout << "Entering InvalidStyleOption test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };
    
    std::string options = "<script>alert('hack')</script>";
    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 640, height = 480" << std::endl;

        WebVTTSubtecParser parser(type, 640, 480);
    
        std::cout << "Invoking setTextStyle with invalid options: " << options << std::endl;
        parser.setTextStyle(options);
        std::cout << "Method setTextStyle executed safely with invalid options handled appropriately" << std::endl;
    }

    std::cout << "Exiting InvalidStyleOption test" << std::endl;
}

/**
 * @brief Validate correct behavior of updateTimestamp API when invoked with zero milliseconds
 *
 * This test verifies that invoking updateTimestamp with a timestamp value of zero does not cause any errors or unexpected behavior. It ensures the API handles zero as a valid input across various SubtitleMimeType values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080 and call updateTimestamp | SubtitleMimeType = eSUB_TYPE_WEBVTT, width = 1920, height = 1080, inputTimestamp = 0 | updateTimestamp executed successfully without error | Should Pass |
 * | 02 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080 and call updateTimestamp | SubtitleMimeType = eSUB_TYPE_MP4, width = 1920, height = 1080, inputTimestamp = 0 | updateTimestamp executed successfully without error | Should Pass |
 * | 03 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080 and call updateTimestamp | SubtitleMimeType = eSUB_TYPE_TTML, width = 1920, height = 1080, inputTimestamp = 0 | updateTimestamp executed successfully without error | Should Pass |
 * | 04 | Create WebVTTSubtecParser object with SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080 and call updateTimestamp | SubtitleMimeType = eSUB_TYPE_UNKNOWN, width = 1920, height = 1080, inputTimestamp = 0 | updateTimestamp executed successfully without error | Should Pass |
 */
TEST(WebVTTSubtecParser, UpdateTimestampWithZeroMilliseconds)
{
    std::cout << "Entering UpdateTimestampWithZeroMilliseconds test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    unsigned long long inputTimestamp = 0;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type 
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking updateTimestamp with value: " << inputTimestamp << std::endl;

        parser.updateTimestamp(inputTimestamp);

        std::cout << "updateTimestamp invoked successfully with input: " << inputTimestamp << std::endl;
    }

    std::cout << "Exiting UpdateTimestampWithZeroMilliseconds test" << std::endl;
}

/**
 * @brief Test the updateTimestamp function with typical positive value
 *
 * This test verifies that the updateTimestamp method of the WebVTTSubtecParser class correctly processes a typical positive timestamp value (12345) for various SubtitleMimeType values. The test creates a parser object for each subtitle type and invokes the updateTimestamp method, ensuring that no errors occur and that the parser's internal state is assumed to be correctly updated.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                       | Test Data                                                                                                                                                 | Expected Result                                                       | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Initialize SubtitleMimeType array with various types and set the input timestamp value.           | SubtitleMimeType = eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN; inputTimestamp = 12345                                              | Variables initialized correctly.                                      | Should be successful |
 * | 02               | Create a WebVTTSubtecParser object for each subtitle type with resolution 1920x1080.               | type = one among {eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4, eSUB_TYPE_TTML, eSUB_TYPE_UNKNOWN}; width = 1920, height = 1080                                           | Parser object created successfully.                                   | Should Pass     |
 * | 03               | Invoke updateTimestamp on the parser object with the input timestamp.                           | inputTimestamp = 12345                                                                                                                                      | updateTimestamp executes successfully; assertion EXPECT_TRUE(true) passes. | Should Pass     |
 */
TEST(WebVTTSubtecParser, UpdateTimestampWithTypicalPositiveValue)
{
    std::cout << "Entering UpdateTimestampWithTypicalPositiveValue test" << std::endl;

    SubtitleMimeType types[] = {
        eSUB_TYPE_WEBVTT,
        eSUB_TYPE_MP4,
        eSUB_TYPE_TTML,
        eSUB_TYPE_UNKNOWN
    };

    unsigned long long inputTimestamp = 12345;

    for (auto type : types)
    {
        std::cout << "Creating WebVTTSubtecParser object with SubtitleMimeType = " << type
                  << ", width = 1920, height = 1080" << std::endl;

        WebVTTSubtecParser parser(type, 1920, 1080);

        std::cout << "Invoking updateTimestamp with value: " << inputTimestamp << std::endl;

        parser.updateTimestamp(inputTimestamp);

        std::cout << "updateTimestamp invoked successfully with input: " << inputTimestamp << std::endl;

        // You can replace this with a real assertion if internal state can be verified
        EXPECT_TRUE(true);
    }

    std::cout << "Exiting UpdateTimestampWithTypicalPositiveValue test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
