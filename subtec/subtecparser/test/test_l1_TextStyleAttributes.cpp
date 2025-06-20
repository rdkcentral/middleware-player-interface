
/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2024 RDK Management
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
* @file test_l1_TextStyleAttributes.cpp
* @page TextStyleAttributes Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the TextStyleAttributes methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/
     
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>

#include "TextStyleAttributes.h"


/**
* @brief Test to validate all supported pen size attributes in TextStyleAttributes
*
* This test verifies that the TextStyleAttributes class correctly parses and sets the pen size attribute
* when provided with a valid JSON options string. It iterates through all supported values of the "penSize"
* property: "small", "standard", "medium", "large", "extra_large", and "auto". The test ensures that the
* corresponding attribute values and attribute mask are set as expected.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 001@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object | attributesValues = {0}, attributesMask = 0 | Object initialized successfully | Should be successful |
* | 02 | Call getAttributes with penSize = "small" | options = "{\"penSize\": \"small\"}" | result = 0, attributesValues[5] = FONT_SIZE_SMALL, attributesMask = (1 << 5) | Should Pass |
* | 03 | Call getAttributes with penSize = "standard" | options = "{\"penSize\": \"standard\"}" | result = 0, attributesValues[5] = FONT_SIZE_STANDARD, attributesMask = (1 << 5) | Should Pass |
* | 04 | Call getAttributes with penSize = "medium" | options = "{\"penSize\": \"medium\"}" | result = 0, attributesValues[5] = FONT_SIZE_STANDARD, attributesMask = (1 << 5) | "medium" maps to "standard" |
* | 05 | Call getAttributes with penSize = "large" | options = "{\"penSize\": \"large\"}" | result = 0, attributesValues[5] = FONT_SIZE_LARGE, attributesMask = (1 << 5) | Should Pass |
* | 06 | Call getAttributes with penSize = "extra_large" | options = "{\"penSize\": \"extra_large\"}" | result = 0, attributesValues[5] = FONT_SIZE_EXTRALARGE, attributesMask = (1 << 5) | Should Pass |
* | 07 | Call getAttributes with penSize = "auto" | options = "{\"penSize\": \"auto\"}" | result = 0, attributesValues[5] = FONT_SIZE_EMBEDDED, attributesMask = (1 << 5) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidPenSizeAttribute) {
    std::cout << "Entering ValidPenSizeAttribute" << std::endl;

    struct TestCase {
        std::string penSizeStr;
        TextStyleAttributes::FontSize expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"small", TextStyleAttributes::FONT_SIZE_SMALL},
        {"standard", TextStyleAttributes::FONT_SIZE_STANDARD},
        {"medium", TextStyleAttributes::FONT_SIZE_STANDARD}, 
        {"large", TextStyleAttributes::FONT_SIZE_LARGE},
        {"extra_large", TextStyleAttributes::FONT_SIZE_EXTRALARGE},
        {"auto", TextStyleAttributes::FONT_SIZE_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing penSize: " << test.penSizeStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"penSize\": \"" + test.penSizeStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[5], test.expectedValue) << "Failed for penSize: " << test.penSizeStr;
        EXPECT_EQ(attributesMask, (1 << 5));

        std::cout << "Passed penSize: " << test.penSizeStr << std::endl;
    }

    std::cout << "Exiting ValidPenSizeAttribute" << std::endl;
}

/**
* @brief Test to validate all supported font style attribute values in TextStyleAttributes class
*
* This test checks if the TextStyleAttributes class correctly parses and sets the font style attribute
* from a given JSON string. It iterates through all supported values of the "fontStyle" property:
* "default", "monospaced_serif"/"monospaced serif", "proportional_serif"/"proportional serif",
* "monospaced_sanserif"/"monospaced sans serif", "proportional_sanserif"/"proportional sans serif",
* "casual", "cursive", "smallcaps"/"small capital", and "auto". The test ensures that the corresponding
* attribute values and attribute mask are set correctly for each case.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 002@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object | attributesValues = {0}, attributesMask = 0 | Object initialized successfully | Should be successful |
* | 02 | Call getAttributes with fontStyle = "default" | options = "{\"fontStyle\": \"default\"}" | result = 0, attributesValues[4] = FONT_STYLE_DEFAULT, attributesMask = (1 << 4) | Should Pass |
* | 03 | Call getAttributes with fontStyle = "monospaced_serif" | options = "{\"fontStyle\": \"monospaced_serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_MONOSPACED_SERIF, attributesMask = (1 << 4) | Should Pass |
* | 04 | Call getAttributes with fontStyle = "monospaced serif" | options = "{\"fontStyle\": \"monospaced serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_MONOSPACED_SERIF, attributesMask = (1 << 4) | Should Pass |
* | 05 | Call getAttributes with fontStyle = "proportional_serif" | options = "{\"fontStyle\": \"proportional_serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_PROPORTIONAL_SERIF, attributesMask = (1 << 4) | Should Pass |
* | 06 | Call getAttributes with fontStyle = "proportional serif" | options = "{\"fontStyle\": \"proportional serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_PROPORTIONAL_SERIF, attributesMask = (1 << 4) | Should Pass |
* | 07 | Call getAttributes with fontStyle = "monospaced_sanserif" | options = "{\"fontStyle\": \"monospaced_sanserif\"}" | result = 0, attributesValues[4] = FONT_STYLE_MONOSPACED_SANSSERIF, attributesMask = (1 << 4) | Should Pass |
* | 08 | Call getAttributes with fontStyle = "monospaced sans serif" | options = "{\"fontStyle\": \"monospaced sans serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_MONOSPACED_SANSSERIF, attributesMask = (1 << 4) | Should Pass |
* | 09 | Call getAttributes with fontStyle = "proportional_sanserif" | options = "{\"fontStyle\": \"proportional_sanserif\"}" | result = 0, attributesValues[4] = FONT_STYLE_PROPORTIONAL_SANSSERIF, attributesMask = (1 << 4) | Should Pass |
* | 10 | Call getAttributes with fontStyle = "proportional sans serif" | options = "{\"fontStyle\": \"proportional sans serif\"}" | result = 0, attributesValues[4] = FONT_STYLE_PROPORTIONAL_SANSSERIF, attributesMask = (1 << 4) | Should Pass |
* | 11 | Call getAttributes with fontStyle = "casual" | options = "{\"fontStyle\": \"casual\"}" | result = 0, attributesValues[4] = FONT_STYLE_CASUAL, attributesMask = (1 << 4) | Should Pass |
* | 12 | Call getAttributes with fontStyle = "cursive" | options = "{\"fontStyle\": \"cursive\"}" | result = 0, attributesValues[4] = FONT_STYLE_CURSIVE, attributesMask = (1 << 4) | Should Pass |
* | 13 | Call getAttributes with fontStyle = "smallcaps" | options = "{\"fontStyle\": \"smallcaps\"}" | result = 0, attributesValues[4] = FONT_STYLE_SMALL_CAPITALS, attributesMask = (1 << 4) | Should Pass |
* | 14 | Call getAttributes with fontStyle = "small capital" | options = "{\"fontStyle\": \"small capital\"}" | result = 0, attributesValues[4] = FONT_STYLE_SMALL_CAPITALS, attributesMask = (1 << 4) | Should Pass |
* | 15 | Call getAttributes with fontStyle = "auto" | options = "{\"fontStyle\": \"auto\"}" | result = 0, attributesValues[4] = FONT_STYLE_EMBEDDED, attributesMask = (1 << 4) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidFontStyleAttribute) {
    std::cout << "Entering ValidFontStyleAttribute" << std::endl;

    struct TestCase {
        std::string fontStyleStr;
        TextStyleAttributes::FontStyle expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"default", TextStyleAttributes::FONT_STYLE_DEFAULT},
        {"monospaced_serif", TextStyleAttributes::FONT_STYLE_MONOSPACED_SERIF},
        {"monospaced serif", TextStyleAttributes::FONT_STYLE_MONOSPACED_SERIF},
        {"proportional_serif", TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SERIF},
        {"proportional serif", TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SERIF},
        {"monospaced_sanserif", TextStyleAttributes::FONT_STYLE_MONOSPACED_SANSSERIF},
        {"monospaced sans serif", TextStyleAttributes::FONT_STYLE_MONOSPACED_SANSSERIF},
        {"proportional_sanserif", TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SANSSERIF},
        {"proportional sans serif", TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SANSSERIF},
        {"casual", TextStyleAttributes::FONT_STYLE_CASUAL},
        {"cursive", TextStyleAttributes::FONT_STYLE_CURSIVE},
        {"smallcaps", TextStyleAttributes::FONT_STYLE_SMALL_CAPITALS},
        {"small capital", TextStyleAttributes::FONT_STYLE_SMALL_CAPITALS},
        {"auto", TextStyleAttributes::FONT_STYLE_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing fontStyle: " << test.fontStyleStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"fontStyle\": \"" + test.fontStyleStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[4], test.expectedValue) << "Failed for fontStyle: " << test.fontStyleStr;
        EXPECT_EQ(attributesMask, (1 << 4));

        std::cout << "Passed fontStyle: " << test.fontStyleStr << std::endl;
    }

    std::cout << "Exiting ValidFontStyleAttribute" << std::endl;
}

/**
* @brief Test to validate all supported text foreground color attribute values
*
* This test verifies that the `getAttributes` method correctly parses and sets the `textForegroundColor`
* attribute when provided with a valid JSON options string. It iterates through all supported color values:
* "black", "white", "red", "green", "blue", "yellow", "magenta", "cyan", and "auto". The test ensures that
* the method returns the expected result and sets the appropriate `attributesValues` and `attributesMask`.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 003@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object | attributesValues = {0}, attributesMask = 0 | Object initialized successfully | Should be successful |
* | 02 | Call getAttributes with textForegroundColor = "black" | options = "{\"textForegroundColor\": \"black\"}" | result = 0, attributesValues[0] = COLOR_BLACK, attributesMask = (1 << 0) | Should Pass |
* | 03 | Call getAttributes with textForegroundColor = "white" | options = "{\"textForegroundColor\": \"white\"}" | result = 0, attributesValues[0] = COLOR_WHITE, attributesMask = (1 << 0) | Should Pass |
* | 04 | Call getAttributes with textForegroundColor = "red" | options = "{\"textForegroundColor\": \"red\"}" | result = 0, attributesValues[0] = COLOR_RED, attributesMask = (1 << 0) | Should Pass |
* | 05 | Call getAttributes with textForegroundColor = "green" | options = "{\"textForegroundColor\": \"green\"}" | result = 0, attributesValues[0] = COLOR_GREEN, attributesMask = (1 << 0) | Should Pass |
* | 06 | Call getAttributes with textForegroundColor = "blue" | options = "{\"textForegroundColor\": \"blue\"}" | result = 0, attributesValues[0] = COLOR_BLUE, attributesMask = (1 << 0) | Should Pass |
* | 07 | Call getAttributes with textForegroundColor = "yellow" | options = "{\"textForegroundColor\": \"yellow\"}" | result = 0, attributesValues[0] = COLOR_YELLOW, attributesMask = (1 << 0) | Should Pass |
* | 08 | Call getAttributes with textForegroundColor = "magenta" | options = "{\"textForegroundColor\": \"magenta\"}" | result = 0, attributesValues[0] = COLOR_MAGENTA, attributesMask = (1 << 0) | Should Pass |
* | 09 | Call getAttributes with textForegroundColor = "cyan" | options = "{\"textForegroundColor\": \"cyan\"}" | result = 0, attributesValues[0] = COLOR_CYAN, attributesMask = (1 << 0) | Should Pass |
* | 10 | Call getAttributes with textForegroundColor = "auto" | options = "{\"textForegroundColor\": \"auto\"}" | result = 0, attributesValues[0] = COLOR_EMBEDDED, attributesMask = (1 << 0) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextForegroundColorAttribute) {
    std::cout << "Entering ValidTextForegroundColorAttribute" << std::endl;

    struct TestCase {
        std::string colorStr;
        TextStyleAttributes::SupportedColors expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"black", TextStyleAttributes::COLOR_BLACK},
        {"white", TextStyleAttributes::COLOR_WHITE},
        {"red", TextStyleAttributes::COLOR_RED},
        {"green", TextStyleAttributes::COLOR_GREEN},
        {"blue", TextStyleAttributes::COLOR_BLUE},
        {"yellow", TextStyleAttributes::COLOR_YELLOW},
        {"magenta", TextStyleAttributes::COLOR_MAGENTA},
        {"cyan", TextStyleAttributes::COLOR_CYAN},
        {"auto", TextStyleAttributes::COLOR_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textForegroundColor: " << test.colorStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textForegroundColor\": \"" + test.colorStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[0], test.expectedValue) << "Failed for color: " << test.colorStr;
        EXPECT_EQ(attributesMask, (1 << 0));

        std::cout << "Passed textForegroundColor: " << test.colorStr << std::endl;
    }

    std::cout << "Exiting ValidTextForegroundColorAttribute" << std::endl;
}

/**
* @brief Test to validate all supported text background color attribute values
*
* This test checks if the `getAttributes` method correctly parses and sets the `textBackgroundColor`
* attribute from a given JSON string. It iterates through all supported color values:
* "black", "white", "red", "green", "blue", "yellow", "magenta", "cyan", and "auto".
* The test ensures that the method returns the expected result and sets the appropriate
* values in `attributesValues[1]` and the `attributesMask`.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 004@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object | attributesValues = {0}, attributesMask = 0 | Object initialized and ready | Should be successful |
* | 02 | Call getAttributes with textBackgroundColor = "black" | options = "{\"textBackgroundColor\": \"black\"}" | result = 0, attributesValues[1] = COLOR_BLACK, attributesMask = (1 << 1) | Should Pass |
* | 03 | Call getAttributes with textBackgroundColor = "white" | options = "{\"textBackgroundColor\": \"white\"}" | result = 0, attributesValues[1] = COLOR_WHITE, attributesMask = (1 << 1) | Should Pass |
* | 04 | Call getAttributes with textBackgroundColor = "red" | options = "{\"textBackgroundColor\": \"red\"}" | result = 0, attributesValues[1] = COLOR_RED, attributesMask = (1 << 1) | Should Pass |
* | 05 | Call getAttributes with textBackgroundColor = "green" | options = "{\"textBackgroundColor\": \"green\"}" | result = 0, attributesValues[1] = COLOR_GREEN, attributesMask = (1 << 1) | Should Pass |
* | 06 | Call getAttributes with textBackgroundColor = "blue" | options = "{\"textBackgroundColor\": \"blue\"}" | result = 0, attributesValues[1] = COLOR_BLUE, attributesMask = (1 << 1) | Should Pass |
* | 07 | Call getAttributes with textBackgroundColor = "yellow" | options = "{\"textBackgroundColor\": \"yellow\"}" | result = 0, attributesValues[1] = COLOR_YELLOW, attributesMask = (1 << 1) | Should Pass |
* | 08 | Call getAttributes with textBackgroundColor = "magenta" | options = "{\"textBackgroundColor\": \"magenta\"}" | result = 0, attributesValues[1] = COLOR_MAGENTA, attributesMask = (1 << 1) | Should Pass |
* | 09 | Call getAttributes with textBackgroundColor = "cyan" | options = "{\"textBackgroundColor\": \"cyan\"}" | result = 0, attributesValues[1] = COLOR_CYAN, attributesMask = (1 << 1) | Should Pass |
* | 10 | Call getAttributes with textBackgroundColor = "auto" | options = "{\"textBackgroundColor\": \"auto\"}" | result = 0, attributesValues[1] = COLOR_EMBEDDED, attributesMask = (1 << 1) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextBackgroundColorAttribute) {
    std::cout << "Entering ValidTextBackgroundColorAttribute" << std::endl;

    struct TestCase {
        std::string colorStr;
        TextStyleAttributes::SupportedColors expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"black", TextStyleAttributes::COLOR_BLACK},
        {"white", TextStyleAttributes::COLOR_WHITE},
        {"red", TextStyleAttributes::COLOR_RED},
        {"green", TextStyleAttributes::COLOR_GREEN},
        {"blue", TextStyleAttributes::COLOR_BLUE},
        {"yellow", TextStyleAttributes::COLOR_YELLOW},
        {"magenta", TextStyleAttributes::COLOR_MAGENTA},
        {"cyan", TextStyleAttributes::COLOR_CYAN},
        {"auto", TextStyleAttributes::COLOR_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textBackgroundColor: " << test.colorStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textBackgroundColor\": \"" + test.colorStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[1], test.expectedValue) << "Failed for color: " << test.colorStr;
        EXPECT_EQ(attributesMask, (1 << 1));

        std::cout << "Passed textBackgroundColor: " << test.colorStr << std::endl;
    }

    std::cout << "Exiting ValidTextBackgroundColorAttribute" << std::endl;
}

/**
* @brief Test to validate all supported text edge color attribute values
*
* This test verifies that the `TextStyleAttributes` class correctly processes the `textEdgeColor` attribute
* when provided in the JSON options string. It iterates over all supported color values:
* "black", "white", "red", "green", "blue", "yellow", "magenta", "cyan", and "auto".
* The test ensures that each color is correctly parsed, the expected value is set in `attributesValues[13]`,
* and the corresponding bit in the `attributesMask` is updated correctly.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 005@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Create TextStyleAttributes object and initialize values | attributesValues = {0}, attributesMask = 0 | Object and variables initialized successfully | Should be successful |
* | 02 | Call getAttributes with textEdgeColor = "black" | options = "{\"textEdgeColor\": \"black\"}" | result = 0, attributesValues[13] = COLOR_BLACK, attributesMask = (1 << 13) | Should Pass |
* | 03 | Call getAttributes with textEdgeColor = "white" | options = "{\"textEdgeColor\": \"white\"}" | result = 0, attributesValues[13] = COLOR_WHITE, attributesMask = (1 << 13) | Should Pass |
* | 04 | Call getAttributes with textEdgeColor = "red" | options = "{\"textEdgeColor\": \"red\"}" | result = 0, attributesValues[13] = COLOR_RED, attributesMask = (1 << 13) | Should Pass |
* | 05 | Call getAttributes with textEdgeColor = "green" | options = "{\"textEdgeColor\": \"green\"}" | result = 0, attributesValues[13] = COLOR_GREEN, attributesMask = (1 << 13) | Should Pass |
* | 06 | Call getAttributes with textEdgeColor = "blue" | options = "{\"textEdgeColor\": \"blue\"}" | result = 0, attributesValues[13] = COLOR_BLUE, attributesMask = (1 << 13) | Should Pass |
* | 07 | Call getAttributes with textEdgeColor = "yellow" | options = "{\"textEdgeColor\": \"yellow\"}" | result = 0, attributesValues[13] = COLOR_YELLOW, attributesMask = (1 << 13) | Should Pass |
* | 08 | Call getAttributes with textEdgeColor = "magenta" | options = "{\"textEdgeColor\": \"magenta\"}" | result = 0, attributesValues[13] = COLOR_MAGENTA, attributesMask = (1 << 13) | Should Pass |
* | 09 | Call getAttributes with textEdgeColor = "cyan" | options = "{\"textEdgeColor\": \"cyan\"}" | result = 0, attributesValues[13] = COLOR_CYAN, attributesMask = (1 << 13) | Should Pass |
* | 10 | Call getAttributes with textEdgeColor = "auto" | options = "{\"textEdgeColor\": \"auto\"}" | result = 0, attributesValues[13] = COLOR_EMBEDDED, attributesMask = (1 << 13) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextEdgeColorAttribute) {
    std::cout << "Entering ValidTextEdgeColorAttribute" << std::endl;

    struct TestCase {
        std::string colorStr;
        TextStyleAttributes::SupportedColors expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"black", TextStyleAttributes::COLOR_BLACK},
        {"white", TextStyleAttributes::COLOR_WHITE},
        {"red", TextStyleAttributes::COLOR_RED},
        {"green", TextStyleAttributes::COLOR_GREEN},
        {"blue", TextStyleAttributes::COLOR_BLUE},
        {"yellow", TextStyleAttributes::COLOR_YELLOW},
        {"magenta", TextStyleAttributes::COLOR_MAGENTA},
        {"cyan", TextStyleAttributes::COLOR_CYAN},
        {"auto", TextStyleAttributes::COLOR_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textEdgeColor: " << test.colorStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textEdgeColor\": \"" + test.colorStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[13], test.expectedValue) << "Failed for color: " << test.colorStr;
        EXPECT_EQ(attributesMask, (1 << 13));

        std::cout << "Passed textEdgeColor: " << test.colorStr << std::endl;
    }

    std::cout << "Exiting ValidTextEdgeColorAttribute" << std::endl;
}

/**
* @brief Test to validate all supported window fill color attribute values in TextStyleAttributes
*
* This test verifies that the `TextStyleAttributes` class correctly parses and sets the `windowFillColor`
* attribute from a given JSON string. It iterates through all supported color values:
* "black", "white", "red", "green", "blue", "yellow", "magenta", "cyan", and "auto".
* The test ensures that each color is correctly mapped to the corresponding value in `attributesValues[10]`
* and that the appropriate bit in the `attributesMask` (bit 10) is set.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 006@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Create TextStyleAttributes object and initialize state | attributesValues = {0}, attributesMask = 0 | Object created and initial state ready | Should be successful |
* | 02 | Call getAttributes with windowFillColor = "black" | options = "{\"windowFillColor\": \"black\"}" | result = 0, attributesValues[10] = COLOR_BLACK, attributesMask = (1 << 10) | Should Pass |
* | 03 | Call getAttributes with windowFillColor = "white" | options = "{\"windowFillColor\": \"white\"}" | result = 0, attributesValues[10] = COLOR_WHITE, attributesMask = (1 << 10) | Should Pass |
* | 04 | Call getAttributes with windowFillColor = "red" | options = "{\"windowFillColor\": \"red\"}" | result = 0, attributesValues[10] = COLOR_RED, attributesMask = (1 << 10) | Should Pass |
* | 05 | Call getAttributes with windowFillColor = "green" | options = "{\"windowFillColor\": \"green\"}" | result = 0, attributesValues[10] = COLOR_GREEN, attributesMask = (1 << 10) | Should Pass |
* | 06 | Call getAttributes with windowFillColor = "blue" | options = "{\"windowFillColor\": \"blue\"}" | result = 0, attributesValues[10] = COLOR_BLUE, attributesMask = (1 << 10) | Should Pass |
* | 07 | Call getAttributes with windowFillColor = "yellow" | options = "{\"windowFillColor\": \"yellow\"}" | result = 0, attributesValues[10] = COLOR_YELLOW, attributesMask = (1 << 10) | Should Pass |
* | 08 | Call getAttributes with windowFillColor = "magenta" | options = "{\"windowFillColor\": \"magenta\"}" | result = 0, attributesValues[10] = COLOR_MAGENTA, attributesMask = (1 << 10) | Should Pass |
* | 09 | Call getAttributes with windowFillColor = "cyan" | options = "{\"windowFillColor\": \"cyan\"}" | result = 0, attributesValues[10] = COLOR_CYAN, attributesMask = (1 << 10) | Should Pass |
* | 10 | Call getAttributes with windowFillColor = "auto" | options = "{\"windowFillColor\": \"auto\"}" | result = 0, attributesValues[10] = COLOR_EMBEDDED, attributesMask = (1 << 10) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidWindowFillColorAttribute) {
    std::cout << "Entering ValidWindowFillColorAttribute" << std::endl;

    struct TestCase {
        std::string colorStr;
        TextStyleAttributes::SupportedColors expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"black", TextStyleAttributes::COLOR_BLACK},
        {"white", TextStyleAttributes::COLOR_WHITE},
        {"red", TextStyleAttributes::COLOR_RED},
        {"green", TextStyleAttributes::COLOR_GREEN},
        {"blue", TextStyleAttributes::COLOR_BLUE},
        {"yellow", TextStyleAttributes::COLOR_YELLOW},
        {"magenta", TextStyleAttributes::COLOR_MAGENTA},
        {"cyan", TextStyleAttributes::COLOR_CYAN},
        {"auto", TextStyleAttributes::COLOR_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing windowFillColor: " << test.colorStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"windowFillColor\": \"" + test.colorStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[10], test.expectedValue) << "Failed for color: " << test.colorStr;
        EXPECT_EQ(attributesMask, (1 << 10));

        std::cout << "Passed windowFillColor: " << test.colorStr << std::endl;
    }

    std::cout << "Exiting ValidWindowFillColorAttribute" << std::endl;
}

/**
* @brief Test to validate all supported Text Edge Style Attribute values
*
* This test verifies that the `TextStyleAttributes` class correctly parses and sets the `textEdgeStyle` attribute
* when provided with a valid JSON options string. It covers all supported values including:
* "none", "raised", "depressed", "uniform", "drop_shadow_left", "left drop shadow",
* "drop_shadow_right", "right drop shadow", and "auto".
* It ensures that the correct enum values are written to `attributesValues[12]` and that the corresponding
* bit in `attributesMask` (bit 12) is correctly set.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 007@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object and mask values | attributesValues = {0}, attributesMask = 0 | Object and mask initialized | Should be successful |
* | 02 | Call getAttributes with textEdgeStyle = "none" | options = "{\"textEdgeStyle\": \"none\"}" | result = 0, attributesValues[12] = EDGE_TYPE_NONE, attributesMask = (1 << 12) | Should Pass |
* | 03 | Call getAttributes with textEdgeStyle = "raised" | options = "{\"textEdgeStyle\": \"raised\"}" | result = 0, attributesValues[12] = EDGE_TYPE_RAISED, attributesMask = (1 << 12) | Should Pass |
* | 04 | Call getAttributes with textEdgeStyle = "depressed" | options = "{\"textEdgeStyle\": \"depressed\"}" | result = 0, attributesValues[12] = EDGE_TYPE_DEPRESSED, attributesMask = (1 << 12) | Should Pass |
* | 05 | Call getAttributes with textEdgeStyle = "uniform" | options = "{\"textEdgeStyle\": \"uniform\"}" | result = 0, attributesValues[12] = EDGE_TYPE_UNIFORM, attributesMask = (1 << 12) | Should Pass |
* | 06 | Call getAttributes with textEdgeStyle = "drop_shadow_left" | options = "{\"textEdgeStyle\": \"drop_shadow_left\"}" | result = 0, attributesValues[12] = EDGE_TYPE_SHADOW_LEFT, attributesMask = (1 << 12) | Should Pass |
* | 07 | Call getAttributes with textEdgeStyle = "left drop shadow" | options = "{\"textEdgeStyle\": \"left drop shadow\"}" | result = 0, attributesValues[12] = EDGE_TYPE_SHADOW_LEFT, attributesMask = (1 << 12) | Should Pass |
* | 08 | Call getAttributes with textEdgeStyle = "drop_shadow_right" | options = "{\"textEdgeStyle\": \"drop_shadow_right\"}" | result = 0, attributesValues[12] = EDGE_TYPE_SHADOW_RIGHT, attributesMask = (1 << 12) | Should Pass |
* | 09 | Call getAttributes with textEdgeStyle = "right drop shadow" | options = "{\"textEdgeStyle\": \"right drop shadow\"}" | result = 0, attributesValues[12] = EDGE_TYPE_SHADOW_RIGHT, attributesMask = (1 << 12) | Should Pass |
* | 10 | Call getAttributes with textEdgeStyle = "auto" | options = "{\"textEdgeStyle\": \"auto\"}" | result = 0, attributesValues[12] = EDGE_TYPE_EMBEDDED, attributesMask = (1 << 12) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextEdgeStyleAttribute) {
    std::cout << "Entering ValidTextEdgeStyleAttribute" << std::endl;

    struct TestCase {
        std::string styleStr;
        TextStyleAttributes::EdgeType expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"none", TextStyleAttributes::EDGE_TYPE_NONE},
        {"raised", TextStyleAttributes::EDGE_TYPE_RAISED},
        {"depressed", TextStyleAttributes::EDGE_TYPE_DEPRESSED},
        {"uniform", TextStyleAttributes::EDGE_TYPE_UNIFORM},
        {"drop_shadow_left", TextStyleAttributes::EDGE_TYPE_SHADOW_LEFT},
        {"left drop shadow", TextStyleAttributes::EDGE_TYPE_SHADOW_LEFT},
        {"drop_shadow_right", TextStyleAttributes::EDGE_TYPE_SHADOW_RIGHT},
        {"right drop shadow", TextStyleAttributes::EDGE_TYPE_SHADOW_RIGHT},
        {"auto", TextStyleAttributes::EDGE_TYPE_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textEdgeStyle: " << test.styleStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textEdgeStyle\": \"" + test.styleStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[12], test.expectedValue) << "Failed for textEdgeStyle: " << test.styleStr;
        EXPECT_EQ(attributesMask, (1 << 12));

        std::cout << "Passed textEdgeStyle: " << test.styleStr << std::endl;
    }

    std::cout << "Exiting ValidTextEdgeStyleAttribute" << std::endl;
}

/**
* @brief Test to validate all supported textForegroundOpacity attribute values in TextStyleAttributes
*
* This test checks the functionality of the `getAttributes` method in the `TextStyleAttributes` class. It verifies
* that the method correctly parses all valid values of the `"textForegroundOpacity"` attribute from a JSON string,
* sets the correct value in `attributesValues[2]`, and updates the appropriate bit in `attributesMask` (bit 2).
* Supported values include: "solid", "flash", "translucent", "transparent", and "auto".
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 008@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object and masks | attributesValues = {0}, attributesMask = 0 | Object and values initialized | Should be successful |
* | 02 | Call getAttributes with textForegroundOpacity = "solid" | options = "{\"textForegroundOpacity\": \"solid\"}" | result = 0, attributesValues[2] = OPACITY_SOLID, attributesMask = (1 << 2) | Should Pass |
* | 03 | Call getAttributes with textForegroundOpacity = "flash" | options = "{\"textForegroundOpacity\": \"flash\"}" | result = 0, attributesValues[2] = OPACITY_FLASHING, attributesMask = (1 << 2) | Should Pass |
* | 04 | Call getAttributes with textForegroundOpacity = "translucent" | options = "{\"textForegroundOpacity\": \"translucent\"}" | result = 0, attributesValues[2] = OPACITY_TRANSLUCENT, attributesMask = (1 << 2) | Should Pass |
* | 05 | Call getAttributes with textForegroundOpacity = "transparent" | options = "{\"textForegroundOpacity\": \"transparent\"}" | result = 0, attributesValues[2] = OPACITY_TRANSPARENT, attributesMask = (1 << 2) | Should Pass |
* | 06 | Call getAttributes with textForegroundOpacity = "auto" | options = "{\"textForegroundOpacity\": \"auto\"}" | result = 0, attributesValues[2] = OPACITY_EMBEDDED, attributesMask = (1 << 2) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextForegroundOpacityAttribute) {
    std::cout << "Entering ValidTextForegroundOpacityAttribute" << std::endl;

    struct TestCase {
        std::string opacityStr;
        TextStyleAttributes::Opacity expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"solid", TextStyleAttributes::OPACITY_SOLID},
        {"flash", TextStyleAttributes::OPACITY_FLASHING},
        {"translucent", TextStyleAttributes::OPACITY_TRANSLUCENT},
        {"transparent", TextStyleAttributes::OPACITY_TRANSPARENT},
        {"auto", TextStyleAttributes::OPACITY_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textForegroundOpacity: " << test.opacityStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textForegroundOpacity\": \"" + test.opacityStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[2], test.expectedValue) << "Failed for opacity: " << test.opacityStr;
        EXPECT_EQ(attributesMask, (1 << 2));

        std::cout << "Passed textForegroundOpacity: " << test.opacityStr << std::endl;
    }

    std::cout << "Exiting ValidTextForegroundOpacityAttribute" << std::endl;
}

/**
* @brief Test to validate all supported textBackgroundOpacity attribute values in TextStyleAttributes
*
* This test verifies that the `getAttributes` method in the `TextStyleAttributes` class correctly parses
* and sets the `"textBackgroundOpacity"` attribute from a valid JSON string. It iterates over all supported
* values: "solid", "flash", "translucent", "transparent", and "auto". The test ensures that the correct
* enum value is written to `attributesValues[3]` and that the appropriate bit (bit 3) is set in `attributesMask`.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 009@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Create TextStyleAttributes object and initialize state | attributesValues = {0}, attributesMask = 0 | Object initialized | Should be successful |
* | 02 | Call getAttributes with textBackgroundOpacity = "solid" | options = "{\"textBackgroundOpacity\": \"solid\"}" | result = 0, attributesValues[3] = OPACITY_SOLID, attributesMask = (1 << 3) | Should Pass |
* | 03 | Call getAttributes with textBackgroundOpacity = "flash" | options = "{\"textBackgroundOpacity\": \"flash\"}" | result = 0, attributesValues[3] = OPACITY_FLASHING, attributesMask = (1 << 3) | Should Pass |
* | 04 | Call getAttributes with textBackgroundOpacity = "translucent" | options = "{\"textBackgroundOpacity\": \"translucent\"}" | result = 0, attributesValues[3] = OPACITY_TRANSLUCENT, attributesMask = (1 << 3) | Should Pass |
* | 05 | Call getAttributes with textBackgroundOpacity = "transparent" | options = "{\"textBackgroundOpacity\": \"transparent\"}" | result = 0, attributesValues[3] = OPACITY_TRANSPARENT, attributesMask = (1 << 3) | Should Pass |
* | 06 | Call getAttributes with textBackgroundOpacity = "auto" | options = "{\"textBackgroundOpacity\": \"auto\"}" | result = 0, attributesValues[3] = OPACITY_EMBEDDED, attributesMask = (1 << 3) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidTextBackgroundOpacityAttribute) {
    std::cout << "Entering ValidTextBackgroundOpacityAttribute" << std::endl;

    struct TestCase {
        std::string opacityStr;
        TextStyleAttributes::Opacity expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"solid", TextStyleAttributes::OPACITY_SOLID},
        {"flash", TextStyleAttributes::OPACITY_FLASHING},
        {"translucent", TextStyleAttributes::OPACITY_TRANSLUCENT},
        {"transparent", TextStyleAttributes::OPACITY_TRANSPARENT},
        {"auto", TextStyleAttributes::OPACITY_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing textBackgroundOpacity: " << test.opacityStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"textBackgroundOpacity\": \"" + test.opacityStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[3], test.expectedValue) << "Failed for opacity: " << test.opacityStr;
        EXPECT_EQ(attributesMask, (1 << 3));

        std::cout << "Passed textBackgroundOpacity: " << test.opacityStr << std::endl;
    }

    std::cout << "Exiting ValidTextBackgroundOpacityAttribute" << std::endl;
}

/**
* @brief Test to validate all supported windowFillOpacity attribute values in TextStyleAttributes
*
* This test verifies that the `getAttributes` method correctly parses and sets the `"windowFillOpacity"` attribute
* from a valid JSON string. It tests all supported values: "solid", "flash", "translucent", "transparent", and "auto".
* The test ensures the correct enum value is written to `attributesValues[11]` and that the appropriate bit in
* `attributesMask` (bit 11) is set.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 010@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Create TextStyleAttributes object and initialize state | attributesValues = {0}, attributesMask = 0 | Object initialized | Should be successful |
* | 02 | Call getAttributes with windowFillOpacity = "solid" | options = "{\"windowFillOpacity\": \"solid\"}" | result = 0, attributesValues[11] = OPACITY_SOLID, attributesMask = (1 << 11) | Should Pass |
* | 03 | Call getAttributes with windowFillOpacity = "flash" | options = "{\"windowFillOpacity\": \"flash\"}" | result = 0, attributesValues[11] = OPACITY_FLASHING, attributesMask = (1 << 11) | Should Pass |
* | 04 | Call getAttributes with windowFillOpacity = "translucent" | options = "{\"windowFillOpacity\": \"translucent\"}" | result = 0, attributesValues[11] = OPACITY_TRANSLUCENT, attributesMask = (1 << 11) | Should Pass |
* | 05 | Call getAttributes with windowFillOpacity = "transparent" | options = "{\"windowFillOpacity\": \"transparent\"}" | result = 0, attributesValues[11] = OPACITY_TRANSPARENT, attributesMask = (1 << 11) | Should Pass |
* | 06 | Call getAttributes with windowFillOpacity = "auto" | options = "{\"windowFillOpacity\": \"auto\"}" | result = 0, attributesValues[11] = OPACITY_EMBEDDED, attributesMask = (1 << 11) | Should Pass |
*/
TEST(TextStyleAttributesTest, ValidWindowFillOpacityAttribute) {
    std::cout << "Entering ValidWindowFillOpacityAttribute" << std::endl;

    struct TestCase {
        std::string opacityStr;
        TextStyleAttributes::Opacity expectedValue;
    };

    std::vector<TestCase> testCases = {
        {"solid", TextStyleAttributes::OPACITY_SOLID},
        {"flash", TextStyleAttributes::OPACITY_FLASHING},
        {"translucent", TextStyleAttributes::OPACITY_TRANSLUCENT},
        {"transparent", TextStyleAttributes::OPACITY_TRANSPARENT},
        {"auto", TextStyleAttributes::OPACITY_EMBEDDED}
    };

    for (const auto& test : testCases) {
        std::cout << "Testing windowFillOpacity: " << test.opacityStr << std::endl;

        TextStyleAttributes obj;
        std::string options = "{\"windowFillOpacity\": \"" + test.opacityStr + "\"}";
        attributesType attributesValues = {0};
        uint32_t attributesMask = 0;

        int result = obj.getAttributes(options, attributesValues, attributesMask);

        EXPECT_EQ(result, 0);
        EXPECT_EQ(attributesValues[11], test.expectedValue) << "Failed for windowFillOpacity: " << test.opacityStr;
        EXPECT_EQ(attributesMask, (1 << 11));

        std::cout << "Passed windowFillOpacity: " << test.opacityStr << std::endl;
    }

    std::cout << "Exiting ValidWindowFillOpacityAttribute" << std::endl;
}

/**
* @brief Test to validate multiple valid attributes in TextStyleAttributes
*
* This test checks the functionality of the getAttributes method in the TextStyleAttributes class by providing multiple valid attributes in the options string. It verifies that the method correctly parses the options and sets the appropriate attribute values and mask.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 011@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01| Initialize TextStyleAttributes object and options string | options = "{\"penSize\": \"medium\", \"fontStyle\": \"default\", \"textForegroundColor\": \"white\"}", attributesValues = {0}, attributesMask = 0 | Object and variables initialized | Should be successful |
* | 02| Call getAttributes method with options string | options = "{\"penSize\": \"medium\", \"fontStyle\": \"default\", \"textForegroundColor\": \"white\"}", attributesValues, attributesMask | result = 0 | Should Pass |
* | 03| Verify attribute values for penSize | |attributesValues[5] = TextStyleAttributes::FONT_SIZE_STANDARD | Should Pass |
* | 04| Verify attribute values for fontStyle| | attributesValues[4] = TextStyleAttributes::FONT_STYLE_DEFAULT | Should Pass |
* | 05| Verify attribute values for textForegroundColor| | attributesValues[0] = TextStyleAttributes::COLOR_WHITE | Should Pass |
* | 06| Verify attributesMask | attributesMask| | Should Pass |
*/
TEST(TextStyleAttributesTest, MultipleValidAttributes) {
    std::cout << "Entering MultipleValidAttributes" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{\"penSize\": \"medium\", \"fontStyle\": \"default\", \"textForegroundColor\": \"white\"}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, 0);
    EXPECT_EQ(attributesValues[5], TextStyleAttributes::FONT_SIZE_STANDARD);
    EXPECT_EQ(attributesValues[4], TextStyleAttributes::FONT_STYLE_DEFAULT);
    EXPECT_EQ(attributesValues[0], TextStyleAttributes::COLOR_WHITE);
    EXPECT_EQ(attributesMask, (1 << 5) | (1 << 4) | (1 << 0));
    
    std::cout << "Exiting MultipleValidAttributes" << std::endl;
}

/**
* @brief Test to validate the behavior of getAttributes method when an invalid attribute key is provided.
*
* This test checks the response of the getAttributes method when it is given an invalid attribute key in the options string. The method is expected to return an error code and not modify the attributesMask.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 012@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object and set options with invalid key | options = "{\"invalidKey\": \"value\"}", attributesValues = {0}, attributesMask = 0 | result = -1, attributesMask = 0 | Should Pass |
* | 02 | Call getAttributes method with invalid options | options = "{\"invalidKey\": \"value\"}", attributesValues = {0}, attributesMask = 0 | result = -1, attributesMask = 0 | Should Pass |
* | 03 | Verify the result and attributesMask | result = -1, attributesMask = 0 | result = -1, attributesMask = 0 | Should Pass |
*/
TEST(TextStyleAttributesTest, InvalidAttributeKey) {
    std::cout << "Entering InvalidAttributeKey" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{\"invalidKey\": \"value\"}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);
    EXPECT_EQ(attributesMask, 0);
    std::cout << "Exiting InvalidAttributeKey" << std::endl;
}

/**
* @brief Test to validate the behavior of getAttributes method when an invalid attribute value is provided.
*
* This test checks the response of the getAttributes method when it is given an invalid value for the "penSize" attribute. 
* It ensures that the method correctly identifies the invalid input and returns an appropriate error code.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 013@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01| Call getAttributes with invalid penSize value | options = "{\"penSize\": \"invalidValue\"}", attributesValues = {0}, attributesMask = 0 | result = -1, attributesMask = 0, EXPECT_EQ(result, -1), EXPECT_EQ(attributesMask, 0) | Should Fail |
*/
TEST(TextStyleAttributesTest, InvalidAttributeValue) {
    std::cout << "Entering InvalidAttributeValue" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{\"penSize\": \"invalidValue\"}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);
    EXPECT_EQ(attributesMask, 0);
    std::cout << "Exiting InvalidAttributeValue" << std::endl;
}

/**
* @brief Test to verify the behavior of getAttributes method when an empty options string is provided.
*
* This test checks the getAttributes method of the TextStyleAttributes class with an empty options string. 
* It ensures that the method returns an error code and does not modify the attributes mask.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 014@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Call getAttributes with empty options string | options = "", attributesValues = {0}, attributesMask = 0 | result = -1, attributesMask = 0 | Should Pass |
*/
TEST(TextStyleAttributesTest, EmptyOptionsString) {
    std::cout << "Entering EmptyOptionsString" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);
    EXPECT_EQ(attributesMask, 0);
    
    std::cout << "Exiting EmptyOptionsString" << std::endl;
}

/**
* @brief Test the behavior of getAttributes method with an empty JSON string
*
* This test checks the behavior of the getAttributes method when provided with an empty JSON string as input. It ensures that the method handles this edge case correctly by returning an error code and not modifying the attributes mask.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 015@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Call getAttributes with empty JSON string | options = "{}", attributesValues = {0}, attributesMask = 0 | result = -1, attributesMask = 0 | Should Pass |
*/
TEST(TextStyleAttributesTest, EmptyOptionsJsonString) {
    std::cout << "Entering EmptyOptionsJsonString" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);
    EXPECT_EQ(attributesMask, 0);
    
    std::cout << "Exiting EmptyOptionsJsonString" << std::endl;
}

/**
* @brief Test the handling of mixed valid and invalid attributes in TextStyleAttributes
*
* This test checks the behavior of the TextStyleAttributes class when provided with a mix of valid and invalid attributes in the input JSON string. It ensures that the function correctly identifies the invalid attribute and returns the appropriate error code.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 016@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object and input data | options = "{\"penSize\": \"small\", \"invalidKey\": \"value\"}", attributesValues = {0}, attributesMask = 0 | Should be successful | |
* | 02 | Call getAttributes with mixed valid and invalid attributes | options = "{\"penSize\": \"small\", \"invalidKey\": \"value\"}", attributesValues = {0}, attributesMask = 0 | result = -1, EXPECT_EQ(result, -1) | Should Pass |
*/
TEST(TextStyleAttributesTest, MixedValidAndInvalidAttributes) {
    std::cout << "Entering MixedValidAndInvalidAttributes" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{\"penSize\": \"small\", \"invalidKey\": \"value\"}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);

    std::cout << "Exiting MixedValidAndInvalidAttributes" << std::endl;
}

/**
* @brief Test to verify case insensitivity of attribute keys in TextStyleAttributes
*
* This test checks if the TextStyleAttributes class correctly handles attribute keys in a case-insensitive manner. 
* Specifically, it verifies that the method getAttributes returns the expected result when provided with an attribute key in uppercase.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 017@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data | Expected Result | Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01 | Initialize TextStyleAttributes object and set options with uppercase attribute key | options = "{\"PENSIZE\": \"auto\"}", attributesValues = {0}, attributesMask = 0 | result = -1, EXPECT_EQ(result, -1) | Should Pass |
*/
TEST(TextStyleAttributesTest, CaseInsensitiveAttributeKeys) {
    std::cout << "Entering CaseInsensitiveAttributeKeys" << std::endl;
    
    TextStyleAttributes obj;
    std::string options = "{\"PENSIZE\": \"auto\"}";
    attributesType attributesValues = {0};
    uint32_t attributesMask = 0;
    
    int result = obj.getAttributes(options, attributesValues, attributesMask);
    
    EXPECT_EQ(result, -1);

    std::cout << "Exiting CaseInsensitiveAttributeKeys" << std::endl;
}

/**
* @brief Test to verify the assignment of default values in TextStyleAttributes
*
* This test checks the assignment operator for the TextStyleAttributes class to ensure that default values are correctly assigned from one object to another. This is important to verify that the assignment operator works as expected and that the objects maintain their integrity after assignment.
*
* **Test Group ID:** Basic: 01@n
* **Test Case ID:** 018@n
* **Priority:** High@n
* @n
* **Pre-Conditions:** None@n
* **Dependencies:** None@n
* **User Interaction:** None@n
* @n
* **Test Procedure:**@n
* | Variation / Step | Description | Test Data |Expected Result |Notes |
* | :----: | --------- | ---------- |-------------- | ----- |
* | 01| Create two TextStyleAttributes objects | obj1, obj2 | Objects created successfully | Should be successful |
* | 02| Assign obj1 to obj2 using assignment operator | obj2 = obj1 | obj2 should have the same values as obj1 | Should Pass |
* | 03| Verify the assignment operator returns the correct reference | &obj2, &(obj2 = obj1) | The address of obj2 should be returned | Should Pass |
*/
TEST(TextStyleAttributesTest, AssigningDefaultValues) {
    std::cout << "Entering AssigningDefaultValues" << std::endl;
    
    TextStyleAttributes obj1;
    TextStyleAttributes obj2;
    
    obj2 = obj1;
    EXPECT_EQ(&obj2, &(obj2 = obj1));
    
    std::cout << "Exiting AssigningDefaultValues" << std::endl;
}

int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
}
    
