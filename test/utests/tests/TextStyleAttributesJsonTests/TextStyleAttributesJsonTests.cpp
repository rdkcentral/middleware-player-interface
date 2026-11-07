/*
* If not stated otherwise in this file or this component's license file the
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

#include <gtest/gtest.h>
#include <string>
#include "TextStyleAttributes.h"

namespace
{
struct Result
{
    int ret{};
    uint32_t mask{};
    attributesType values{};
};

Result run(const std::string& json)
{
    TextStyleAttributes attributes;
    Result result;
    result.ret = attributes.getAttributes(json, result.values, result.mask);
    return result;
}

constexpr uint32_t bit(TextStyleAttributes::AttribPosInArray pos)
{
    return 1u << pos;
}

constexpr uint32_t embedded = static_cast<uint32_t>(-1);
}

TEST(TextStyleAttributesValueTests, EmptyOptionsReturnsError)
{
    Result r = run("");
    EXPECT_EQ(r.ret, -1);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, InvalidJsonReturnsError)
{
    Result r = run("{invalid");
    EXPECT_EQ(r.ret, -1);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, EmptyJsonObjectReturnsSuccessWithNoAttributes)
{
    Result r = run("{}");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, NonStringValueIsIgnored)
{
    Result r = run(R"({"penSize": 5})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, FontSizeSmall)
{
    Result r = run(R"({"penSize": "small"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_SMALL));
}

TEST(TextStyleAttributesValueTests, FontSizeStandardAndMediumMapToStandard)
{
    EXPECT_EQ(run(R"({"penSize": "standard"})").values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_STANDARD));
    EXPECT_EQ(run(R"({"penSize": "medium"})").values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_STANDARD));
}

TEST(TextStyleAttributesValueTests, FontSizeLarge)
{
    Result r = run(R"({"penSize": "large"})");
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_LARGE));
}

TEST(TextStyleAttributesValueTests, FontSizeExtraLarge)
{
    Result r = run(R"({"penSize": "extra_large"})");
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_EXTRALARGE));
}

TEST(TextStyleAttributesValueTests, FontSizeAutoIsEmbedded)
{
    Result r = run(R"({"penSize": "auto"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::FONT_SIZE_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], embedded);
}

TEST(TextStyleAttributesValueTests, FontSizeIsCaseInsensitive)
{
    Result r = run(R"({"penSize": "LARGE"})");
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_LARGE));
}

TEST(TextStyleAttributesValueTests, UnsupportedFontSizeLeavesMaskClear)
{
    Result r = run(R"({"penSize": "huge"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, FontStyleDefault)
{
    Result r = run(R"({"fontStyle": "default"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::FONT_STYLE_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_DEFAULT));
}

TEST(TextStyleAttributesValueTests, FontStyleSerifVariants)
{
    EXPECT_EQ(run(R"({"fontStyle": "monospaced_serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_MONOSPACED_SERIF));
    EXPECT_EQ(run(R"({"fontStyle": "monospaced serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_MONOSPACED_SERIF));
    EXPECT_EQ(run(R"({"fontStyle": "proportional_serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SERIF));
    EXPECT_EQ(run(R"({"fontStyle": "proportional serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SERIF));
}

TEST(TextStyleAttributesValueTests, FontStyleSansSerifVariants)
{
    EXPECT_EQ(run(R"({"fontStyle": "monospaced_sanserif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_MONOSPACED_SANSSERIF));
    EXPECT_EQ(run(R"({"fontStyle": "monospaced sans serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_MONOSPACED_SANSSERIF));
    EXPECT_EQ(run(R"({"fontStyle": "proportional_sanserif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SANSSERIF));
    EXPECT_EQ(run(R"({"fontStyle": "proportional sans serif"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_PROPORTIONAL_SANSSERIF));
}

TEST(TextStyleAttributesValueTests, FontStyleCasualCursiveSmallCapsAndAuto)
{
    EXPECT_EQ(run(R"({"fontStyle": "casual"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_CASUAL));
    EXPECT_EQ(run(R"({"fontStyle": "cursive"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_CURSIVE));
    EXPECT_EQ(run(R"({"fontStyle": "smallcaps"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_SMALL_CAPITALS));
    EXPECT_EQ(run(R"({"fontStyle": "small capital"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_SMALL_CAPITALS));
    EXPECT_EQ(run(R"({"fontStyle": "auto"})").values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], embedded);
}

TEST(TextStyleAttributesValueTests, UnsupportedFontStyleLeavesMaskClear)
{
    Result r = run(R"({"fontStyle": "fancy"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, ForegroundColorIsParsed)
{
    Result r = run(R"({"textForegroundColor": "red"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::FONT_COLOR_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_RED));
}

TEST(TextStyleAttributesValueTests, BackgroundColorIsParsedCaseInsensitively)
{
    Result r = run(R"({"textBackgroundColor": "Blue"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::BACKGROUND_COLOR_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::BACKGROUND_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_BLUE));
}

TEST(TextStyleAttributesValueTests, EdgeColorAutoIsEmbedded)
{
    Result r = run(R"({"textEdgeColor": "auto"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::EDGE_COLOR_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::EDGE_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_EMBEDDED));
}

TEST(TextStyleAttributesValueTests, WindowFillColorIsParsed)
{
    Result r = run(R"({"windowFillColor": "yellow"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::WIN_COLOR_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::WIN_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_YELLOW));
}

TEST(TextStyleAttributesValueTests, UnsupportedColorLeavesMaskClear)
{
    Result r = run(R"({"textForegroundColor": "purple", "textBackgroundColor": "purple", "textEdgeColor": "purple", "windowFillColor": "purple"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, BackgroundOpacityTranslucent)
{
    Result r = run(R"({"textBackgroundOpacity": "translucent"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::BACKGROUND_OPACITY_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::BACKGROUND_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_TRANSLUCENT));
}

TEST(TextStyleAttributesValueTests, ForegroundOpacitySolidIsStoredAsZero)
{
    Result r = run(R"({"textForegroundOpacity": "solid"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::FONT_OPACITY_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_SOLID));
}

TEST(TextStyleAttributesValueTests, WindowOpacityFlashAndTransparent)
{
    EXPECT_EQ(run(R"({"windowFillOpacity": "flash"})").values[TextStyleAttributes::WIN_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_FLASHING));
    EXPECT_EQ(run(R"({"windowFillOpacity": "transparent"})").values[TextStyleAttributes::WIN_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_TRANSPARENT));
    EXPECT_EQ(run(R"({"windowFillOpacity": "auto"})").values[TextStyleAttributes::WIN_OPACITY_ARR_POSITION], embedded);
}

TEST(TextStyleAttributesValueTests, UnsupportedOpacityLeavesMaskClear)
{
    Result r = run(R"({"textBackgroundOpacity": "bogus", "textForegroundOpacity": "bogus", "windowFillOpacity": "bogus"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, EdgeTypeNoneRaisedDepressedUniform)
{
    EXPECT_EQ(run(R"({"textEdgeStyle": "none"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_NONE));
    EXPECT_EQ(run(R"({"textEdgeStyle": "raised"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_RAISED));
    EXPECT_EQ(run(R"({"textEdgeStyle": "depressed"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_DEPRESSED));
    EXPECT_EQ(run(R"({"textEdgeStyle": "uniform"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_UNIFORM));
}

TEST(TextStyleAttributesValueTests, EdgeTypeDropShadowVariants)
{
    EXPECT_EQ(run(R"({"textEdgeStyle": "drop_shadow_left"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_SHADOW_LEFT));
    EXPECT_EQ(run(R"({"textEdgeStyle": "left drop shadow"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_SHADOW_LEFT));
    EXPECT_EQ(run(R"({"textEdgeStyle": "drop_shadow_right"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_SHADOW_RIGHT));
    EXPECT_EQ(run(R"({"textEdgeStyle": "right drop shadow"})").values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_SHADOW_RIGHT));
}

TEST(TextStyleAttributesValueTests, EdgeTypeAutoIsEmbedded)
{
    Result r = run(R"({"textEdgeStyle": "auto"})");
    EXPECT_EQ(r.mask, bit(TextStyleAttributes::EDGE_TYPE_ARR_POSITION));
    EXPECT_EQ(r.values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], embedded);
}

TEST(TextStyleAttributesValueTests, UnsupportedEdgeTypeLeavesMaskClear)
{
    Result r = run(R"({"textEdgeStyle": "bogus"})");
    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, 0u);
}

TEST(TextStyleAttributesValueTests, AllAttributesTogetherSetEachMaskBit)
{
    Result r = run(R"({
        "penSize": "large",
        "fontStyle": "casual",
        "textForegroundColor": "white",
        "textBackgroundColor": "black",
        "textEdgeStyle": "raised",
        "textEdgeColor": "green",
        "textBackgroundOpacity": "translucent",
        "textForegroundOpacity": "solid",
        "windowFillColor": "cyan",
        "windowFillOpacity": "transparent"
    })");

    const uint32_t expectedMask =
        bit(TextStyleAttributes::FONT_SIZE_ARR_POSITION) |
        bit(TextStyleAttributes::FONT_STYLE_ARR_POSITION) |
        bit(TextStyleAttributes::FONT_COLOR_ARR_POSITION) |
        bit(TextStyleAttributes::BACKGROUND_COLOR_ARR_POSITION) |
        bit(TextStyleAttributes::EDGE_TYPE_ARR_POSITION) |
        bit(TextStyleAttributes::EDGE_COLOR_ARR_POSITION) |
        bit(TextStyleAttributes::BACKGROUND_OPACITY_ARR_POSITION) |
        bit(TextStyleAttributes::FONT_OPACITY_ARR_POSITION) |
        bit(TextStyleAttributes::WIN_COLOR_ARR_POSITION) |
        bit(TextStyleAttributes::WIN_OPACITY_ARR_POSITION);

    EXPECT_EQ(r.ret, 0);
    EXPECT_EQ(r.mask, expectedMask);
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_SIZE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_SIZE_LARGE));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_STYLE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::FONT_STYLE_CASUAL));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_WHITE));
    EXPECT_EQ(r.values[TextStyleAttributes::BACKGROUND_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_BLACK));
    EXPECT_EQ(r.values[TextStyleAttributes::EDGE_TYPE_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::EDGE_TYPE_RAISED));
    EXPECT_EQ(r.values[TextStyleAttributes::EDGE_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_GREEN));
    EXPECT_EQ(r.values[TextStyleAttributes::BACKGROUND_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_TRANSLUCENT));
    EXPECT_EQ(r.values[TextStyleAttributes::FONT_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_SOLID));
    EXPECT_EQ(r.values[TextStyleAttributes::WIN_COLOR_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::COLOR_CYAN));
    EXPECT_EQ(r.values[TextStyleAttributes::WIN_OPACITY_ARR_POSITION], static_cast<uint32_t>(TextStyleAttributes::OPACITY_TRANSPARENT));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
