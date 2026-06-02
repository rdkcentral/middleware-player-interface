/*
* If not stated otherwise in this file or this component's license file the
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "PlayerLogManager.h"
#include "MockPlayerJsonObject.h"
#include "TextStyleAttributes.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::StrictMock;
using ::testing::An;
using ::testing::DoAll;

class GetTextStyleAttributesTests : public ::testing::Test
{
protected:

    std::unique_ptr<TextStyleAttributes> mAttributes{};

    void SetUp() override
    {
        mAttributes = std::unique_ptr<TextStyleAttributes>(new TextStyleAttributes());

        g_mockPlayerJsonObject = std::make_shared<StrictMock<MockPlayerJsonObject>>();
    }

    void TearDown() override
    {
        mAttributes = nullptr;

        g_mockPlayerJsonObject = nullptr;
    }
};

ACTION(ThrowJsonException)
{
    throw PlayerJsonParseException();
}

/*
    Test the getAttributes function supplying it with empty Json string
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, EmptyJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string options{};
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_EQ(-1, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping EmptyJsonOptionsString test);";
#endif
}

/*
    Test the getAttributes function when PlayerJsonObject throws exception
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, JsonExceptionThrown)
{
#ifdef TEST_SKIP
    std::string options = "{\"fontSize\":\"32.4px\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(ThrowJsonException());
    EXPECT_EQ(-1, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping JsonExceptionThrown test);";
#endif
}

/*
    Test the getAttributes function when PlayerJsonObject unsuccessfully retrieves value
    A wrong key in the Json object (as set in options) is used to test the function.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, JsonValueNotReturned)
{
#ifdef TEST_SKIP
    std::string options = "{\"fontSize\":\"32.4px\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping JsonValueNotReturned test);";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, FontSizeRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP   
    std::string penSizeValue = "32.4px";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));

    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping FontSizeRightKeyInvalidValueJsonOptionsString test);";
#endif
}

/*
    Test the getAttributes with font size small, penSizevalue expressed in lower case
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueSmallLowerCase)
{
#ifdef TEST_SKIP    
    std::string penSizeValue = "small";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_SMALL);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueSmallLowerCase test);";
#endif
}

/*
    Test the getAttributes with font size small, penSizevalue expressed in Upper case
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueSmallUpperCase)
{
#ifdef TEST_SKIP 
    std::string penSizeValue = "SMALL";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_SMALL);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueSmallUpperCase test);";
#endif
}

/*
    Test the getAttributes with font size Medium, penSizevalue expressed in lower case
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueMediumLowerCase)
{
#ifdef  TEST_SKIP    
    std::string penSizeValue = "medium";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_STANDARD);
#else
    GTEST_SKIP()<< "Skipping FontSizeExpectedJsonOptionsStringValueMediumLowerCase test);";
#endif
}

/*
    Test the getAttributes with font size Medium, penSizevalue expressed in Upper case
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
    Two test cases are sufficient to prove that Upper case Json values are handled appropriately
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueMediumUpperCase)
{
#ifdef TEST_SKIP   
    std::string penSizeValue = "MEDIUM";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_STANDARD);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueMediumUpperCase test);";
#endif
}

/*
    Test the getAttributes with font size Standard
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueStandard)
{
#ifdef TEST_SKIP 
    std::string penSizeValue = "standard";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_STANDARD);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueStandard test);";
#endif
}

/*
    Test the getAttributes with font size large
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueLarge)
{
#ifdef TEST_SKIP
    std::string penSizeValue = "large";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_LARGE);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueLarge test);";
#endif
}

/*
    Test the getAttributes with font size FONT_SIZE_EXTRALARGE
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueExtralarge)
{
#ifdef TEST_SKIP
    std::string penSizeValue = "extra_large";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_EXTRALARGE);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueExtralarge test);";
#endif
}

/*
    Test the getAttributes with font size auto i.e. embedded
    This will also test the output expected from the getFontSize function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontSizeExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string penSizeValue = "auto";
    std::string options =  "{\"penSize\":\"" + penSizeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(penSizeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_SIZE_ARR_POSITION], mAttributes->FONT_SIZE_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping FontSizeExpectedJsonOptionsStringValueAuto test);";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, FontStyleRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "italics";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else   
    GTEST_SKIP() << "Skipping FontStyleRightKeyInvalidValueJsonOptionsString test);";
#endif
}

/*
    Test the getAttributes with font size monospaced serif, fontstyle value expressed in lower case
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueMonospacedserifLowerCase)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "monospaced_serif";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_MONOSPACED_SERIF);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueMonospacedser ifLowerCase test;";
#endif

}

/*
    Test the getAttributes with font size monospaced serif, fontstyle value expressed in lower case, separated by space
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueMonospacedserifLowerCaseSpaceSeparated)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "monospaced serif";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_MONOSPACED_SERIF);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueMonospacedserifLowerCaseSpaceSeparated test;";
#endif
}

/*
    Test the getAttributes with font style monospaced serif, fontStyle value expressed in Upper case
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per penSizeValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueMonospacedserifUpperCase)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "MONOSPACED_SERIF";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_MONOSPACED_SERIF);
#else   
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueMonospacedserifUpperCase test;";
#endif
}

/*
    Test the getAttributes with font style proportional serif.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueProportionalserif)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "proportional_serif";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_PROPORTIONAL_SERIF);
#else 
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueProportionalserif test);";
#endif
}

/*
    Test the getAttributes with font style monospaced sans serif.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueMonospacedsansserif)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "monospaced_sanserif";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_MONOSPACED_SANSSERIF);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueMonospacedsansserif test);";
#endif
}

/*
    Test the getAttributes with font style proportional sans serif.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueProportionalsansserif)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "proportional_sanserif";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_PROPORTIONAL_SANSSERIF);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueProportionalsansserif test);";
#endif  
}

/*
    Test the getAttributes with font style casual.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueCasual)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "casual";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_CASUAL);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueCasual test";
#endif
}

/*
    Test the getAttributes with font style cursive.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueCursive)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "cursive";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_CURSIVE);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueCursive test";
#endif
}

/*
    Test the getAttributes with font style small capital.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueSmallcapital)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "small capital";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_SMALL_CAPITALS);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueSmallcapital test";
#endif
}

/*
    Test the getAttributes with font style default.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueDefault)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "default";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_DEFAULT);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueDefault test";
#endif
}

/*
    Test the getAttributes with font style auto.
    This will also test the output expected from the getFontStyle function.
    Expected values are: - a valid attributesMask and attributeValues as per fontStyleValue
*/
TEST_F(GetTextStyleAttributesTests, FontStyleExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string fontStyleValue = "auto";
    std::string options =  "{\"fontStyle\":\"" + fontStyleValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontStyleValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_STYLE_ARR_POSITION], mAttributes->FONT_STYLE_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping FontStyleExpectedJsonOptionsStringValueAuto test);";
#endif
}
/*
    Test the getAttributes with font color black.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueBlackLowerCase)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "black";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueBlackLowerCase test";
#endif
}

/*
    Test the getAttributes with font color black.
    This will also test the output expected from the getColor function. Color value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueBlackUpperCase)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "BLACK";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueBlackUpperCase test";
#endif
}

/*
    Test the getAttributes with font color white.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueWhite)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "white";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_WHITE);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueWhite test";
#endif
}

/*
    Test the getAttributes with font color red.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value.
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueRed)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "red";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_RED);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueRed test";
#endif
}

/*
    Test the getAttributes with font color green.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueGreen)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "green";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_GREEN);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueGreen test";
#endif
}

/*
    Test the getAttributes with font color blue.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value.
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueBlue)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "blue";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_BLUE);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueBlue test";
#endif
}

/*
    Test the getAttributes with font color yellow.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value.
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueBlack)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "yellow";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_YELLOW);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueBlack test";
#endif  
}

/*
    Test the getAttributes with font color magenta.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueMagenta)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "magenta";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_MAGENTA);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueMagenta test";
#endif
}

/*
    Test the getAttributes with font color cyan.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueCyan)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "cyan";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_CYAN);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueCyan test";
#endif
}

/*
    Test the getAttributes with font color auto.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundColor value
*/
TEST_F(GetTextStyleAttributesTests, FontColorExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string fontColorValue = "auto";
    std::string options =  "{\"textForegroundColor\":\"" + fontColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_COLOR_ARR_POSITION], mAttributes->COLOR_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping FontColorExpectedJsonOptionsStringValueAuto test";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "pink";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping BackgroundColorRightKeyInvalidValueJsonOptionsString test";
#endif
}

/*
    Test the getAttributes with background color black.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueBlackLowerCase)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "black";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueBlackLowerCase test";
#endif
}

/*
    Test the getAttributes with background color black.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueBlackUpperCase)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "BLACK";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueBlackUpperCase test";
#endif
}

/*
    Test the getAttributes with background color black.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueWhite)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "white";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_WHITE);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueWhite test"; 
#endif
}

/*
    Test the getAttributes with background color red.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueRed)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "red";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_RED);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueRed test";
#endif
}

/*
    Test the getAttributes with background color green.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueGreen)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "green";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_GREEN);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueGreen test";
#endif
}

/*
    Test the getAttributes with background color blue.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueBlue)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "blue";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_BLUE);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueBlue test";
#endif
}

/*
    Test the getAttributes with background color yellow.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueYellow)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "yellow";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_YELLOW);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueYellow test";
#endif
}

/*
    Test the getAttributes with background color magenta.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueMagenta)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "magenta";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_MAGENTA);
#else       
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueMagenta test";
#endif
}

/*
    Test the getAttributes with background color cyan.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueCyan)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "cyan";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_CYAN);
#else
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueCyan test";
#endif
}

/*
    Test the getAttributes with background color auto.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundColor value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundColorExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string backgroundColorValue = "auto";
    std::string options =  "{\"textBackgroundColor\":\"" + backgroundColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_COLOR_ARR_POSITION], mAttributes->COLOR_EMBEDDED);
#else   
    GTEST_SKIP() << "Skipping BackgroundColorExpectedJsonOptionsStringValueAuto test";
#endif  
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "curved";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping EdgeTypeRightKeyInvalidValueJsonOptionsString test";
#endif
}

/*
    Test the getAttributes with edge style none.
    This will also test the output expected from the getEdgeType function. Edge type value in lower case
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueNoneLowerCase)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "none";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_NONE);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueNoneLowerCase test";
#endif
}

/*
    Test the getAttributes with edge style none.
    This will also test the output expected from the getEdgeType function. Edge type value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueNoneUpperCase)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "NONE";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_NONE);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueNoneUpperCase test";
#endif
}

/*
    Test the getAttributes with edge style raised.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueRaised)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "raised";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_RAISED);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueRaised test";
#endif
}

/*
    Test the getAttributes with edge style depressed.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueDepressed)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "depressed";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_DEPRESSED);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueDepressed test";
#endif
}

/*
    Test the getAttributes with edge style uniform.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueUniform)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "uniform";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_UNIFORM);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueUniform test";
#endif
}

/*
    Test the getAttributes with edge style left drop shadow.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueLeftdropshadow)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "Left drop shadow";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_SHADOW_LEFT);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueLeftdropshadow test";
#endif
}

/*
    Test the getAttributes with edge style right drop shadow.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueRightdropshadow)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "Right drop shadow";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_SHADOW_RIGHT);
#else 
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueRightdropshadow test";
#endif
}

/*
    Test the getAttributes with edge style auto.
    This will also test the output expected from the getEdgeType function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeStyle value
*/
TEST_F(GetTextStyleAttributesTests, EdgeTypeExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string edgeTypeValue = "auto";
    std::string options =  "{\"textEdgeStyle\":\"" + edgeTypeValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeTypeValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_TYPE_ARR_POSITION], mAttributes->EDGE_TYPE_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping EdgeTypeExpectedJsonOptionsStringValueAuto test";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "pink";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping EdgeColorRightKeyInvalidValueJsonOptionsString test"; 
#endif
}

/*
    Test the getAttributes with edge color black.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueBlackLowerCase)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "black";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueBlackLowerCase test";
#endif
}

/*
    Test the getAttributes with edge color black.
    This will also test the output expected from the getColor function. Color value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueBlackUpperCase)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "BLACK";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueBlackUpperCase test";
#endif 
}

/*
    Test the getAttributes with edge color white.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueWhite)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "white";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_WHITE);
#else 
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueWhite test";
#endif
}

/*
    Test the getAttributes with edge color red.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueRed)
{
#ifdef TEST_SKIP  
    std::string edgeColorValue = "red";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_RED);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueRed test";
#endif
}

/*
    Test the getAttributes with edge color green.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueGreen)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "green";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_GREEN);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueGreen test";
#endif
}

/*
    Test the getAttributes with edge color blue.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueBlue)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "blue";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_BLUE);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueBlue test";
#endif
}

/*
    Test the getAttributes with edge color yellow.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueYellow)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "yellow";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_YELLOW);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueYellow test";
#endif
}

/*
    Test the getAttributes with edge color mageta.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueMagenta)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "magenta";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_MAGENTA);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueMagenta test";
#endif
}

/*
    Test the getAttributes with edge color cyan.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueCyan)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "cyan";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_CYAN);
#else 
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueCyan test";    
#endif
}

/*
    Test the getAttributes with edge color auto.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per textEdgeColor value
*/
TEST_F(GetTextStyleAttributesTests, EdgeColorExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string edgeColorValue = "auto";
    std::string options =  "{\"textEdgeColor\":\"" + edgeColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(edgeColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->EDGE_COLOR_ARR_POSITION], mAttributes->COLOR_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping EdgeColorExpectedJsonOptionsStringValueAuto test";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "none";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityRightKeyInvalidValueJsonOptionsString test";
#endif
}

/*
    Test the getAttributes with background opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueSolidLowerCase)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "solid";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueSolidLowerCase test";
#endif
}

/*
    Test the getAttributes with background opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueSolidUpperCase)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "SOLID";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueSolidUpperCase test";
#endif
}

/*
    Test the getAttributes with background opacity flash.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueFlash)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "flash";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_FLASHING);
#else   
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueFlash test";
#endif
}

/*
    Test the getAttributes with background opacity translucent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueTranslucent)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "translucent";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSLUCENT);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueTranslucent test";
#endif
}

/*
    Test the getAttributes with background opacity transparent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueTransparent)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "transparent";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSPARENT);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueTransparent test";
#endif
}

/*
    Test the getAttributes with background opacity auto.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textBackgroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, BackgroundOpacityExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string backgroundOpacityValue = "auto";
    std::string options =  "{\"textBackgroundOpacity\":\"" + backgroundOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(backgroundOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->BACKGROUND_OPACITY_ARR_POSITION], mAttributes->OPACITY_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping BackgroundOpacityExpectedJsonOptionsStringValueAuto test";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityRightKeyInvalidValueJsonOptionsString)
{
#ifdef  TEST_SKIP
    std::string fontOpacityValue = "none";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP() << "Skipping FontOpacityRightKeyInvalidValueJsonOptionsString test";   
#endif         
}

/*
    Test the getAttributes with font opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueSolidLowerCase)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "solid";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueSolidLowerCase test";
#endif
}

/*
    Test the getAttributes with font opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueSolidUpperCase)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "SOLID";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueSolidUpperCase test";
#endif
}

/*
    Test the getAttributes with font opacity flash.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueFlash)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "flash";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_FLASHING);
#else
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueFlash test";
#endif
}

/*
    Test the getAttributes with font opacity translucent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueTranslucent)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "translucent";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSLUCENT);
#else 
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueTranslucent test";
#endif
}

/*
    Test the getAttributes with font opacity transparent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueTransparent)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "transparent";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSPARENT);
#else
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueTransparent test";
#endif
}

/*
    Test the getAttributes with font opacity auto.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per textForegroundOpacity value
*/
TEST_F(GetTextStyleAttributesTests, FontOpacityExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string fontOpacityValue = "auto";
    std::string options =  "{\"textForegroundOpacity\":\"" + fontOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(fontOpacityValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->FONT_OPACITY_ARR_POSITION], mAttributes->OPACITY_EMBEDDED);
#else
    GTEST_SKIP() << "Skipping FontOpacityExpectedJsonOptionsStringValueAuto test";
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, WindowColorRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "pink";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else 
    GTEST_SKIP() << "Skipping WindowColorRightKeyInvalidValueJsonOptionsString test";
#endif
}

/*
    Test the getAttributes with window color black.
    This will also test the output expected from the getColor function. Color value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueBlackLowerCase)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "black";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping WindowColorExpectedJsonOptionsStringValueBlackLowerCase test";
#endif
}

/*
    Test the getAttributes with window color black.
    This will also test the output expected from the getColor function. Color value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueBlackUpperCase)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "BLACK";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_BLACK);
#else
    GTEST_SKIP() << "Skipping WindowColorExpectedJsonOptionsStringValueBlackUpperCase test";
#endif
}

/*
    Test the getAttributes with window color white.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueWhite)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "white";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_WHITE);
#else
    GTEST_SKIP() << "Skipping WindowColorExpectedJsonOptionsStringValueWhite test";
#endif
}

/*
    Test the getAttributes with window color red.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueRed)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "red";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_RED);
#else
    GTEST_SKIP() << "Skipping WindowColorExpectedJsonOptionsStringValueRed test";
#endif
}

/*
    Test the getAttributes with window color green.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueGreen)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "green";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_GREEN);
#else
    GTEST_SKIP() << "skipping WindowColorExpectedJsonOptionsStringValueGreen test";
#endif
}

/*
    Test the getAttributes with window color blue.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueBlue)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "blue";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_BLUE);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with window color yellow.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueYellow)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "yellow";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_YELLOW);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with window color magenta.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueMagenta)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "Magenta";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_MAGENTA);
#else 
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with window color cyan.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueCyan)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "cyan";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_CYAN);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with window color auto.
    This will also test the output expected from the getColor function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillColor value
*/
TEST_F(GetTextStyleAttributesTests, WindowColorExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string windowColorValue = "auto";
    std::string options =  "{\"windowFillColor\":\"" + windowColorValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowColorValue), Return(true)));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>())).WillOnce(Return(false));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_COLOR_ARR_POSITION], mAttributes->COLOR_EMBEDDED);
#else 
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes function supplying it with Right Key but invalid corresponding value.
    In this case getAttributes must set the attributeMask to 0; informing caller nothing to proceed
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityRightKeyInvalidValueJsonOptionsString)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "none";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0x1234;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));
    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, 0);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in lower case.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueSolidLowerCase)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "solid";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity solid.
    This will also test the output expected from the getOpacity function. Opacity value in upper case.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueSolidUpperCase)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "SOLID";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_SOLID);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity flash.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueFlash)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "flash";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_FLASHING);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity translucent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueTranslucent)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "translucent";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSLUCENT);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity transparent.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueTransparent)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "transparent";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_TRANSPARENT);
#else
    GTEST_SKIP();
#endif
}

/*
    Test the getAttributes with font opacity auto.
    This will also test the output expected from the getOpacity function.
    Expected values are: - a valid attributesMask and attributeValues as per windowFillOpacity value
*/
TEST_F(GetTextStyleAttributesTests, WindowOpacityExpectedJsonOptionsStringValueAuto)
{
#ifdef TEST_SKIP
    std::string windowOpacityValue = "auto";
    std::string options =  "{\"windowFillOpacity\":\"" + windowOpacityValue + "\"}";
    std::uint32_t attributesMask = 0;
    attributesType attributesValues = {0};

    EXPECT_CALL(*g_mockPlayerJsonObject, get("penSize", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("fontStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeStyle", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textEdgeColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textBackgroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("textForegroundOpacity", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillColor", An<std::string&>())).WillOnce(Return(false));
    EXPECT_CALL(*g_mockPlayerJsonObject, get("windowFillOpacity", An<std::string&>()))
        .WillOnce(DoAll(SetArgReferee<1>(windowOpacityValue), Return(true)));

    EXPECT_EQ(0, mAttributes->getAttributes(options, attributesValues, attributesMask));
    EXPECT_EQ(attributesMask, (1<<mAttributes->WIN_OPACITY_ARR_POSITION));
    EXPECT_EQ(attributesValues[mAttributes->WIN_OPACITY_ARR_POSITION], mAttributes->OPACITY_EMBEDDED);
#else
    GTEST_SKIP();
#endif
}
