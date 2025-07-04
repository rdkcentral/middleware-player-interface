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

#ifndef __TEXT_STYLE_ATTRIBUTES_H__
#define __TEXT_STYLE_ATTRIBUTES_H__

/**
 * @file  TextStyleAttributes.h
 *
 * @brief This file provides class and other definition related to subtitle text attributes
 *
 */

#include <string.h>
#include <array>
#include <map>
#include "PlayerLogManager.h"
#include "SubtecAttribute.hpp"

class TextStyleAttributes
{

public:
    /**
     * @enum FontSize
     * @brief Available Fontsize
     */
    typedef enum FontSize {
        FONT_SIZE_EMBEDDED = -1,            /* Corresponds to Font size of auto */
        FONT_SIZE_SMALL,
        FONT_SIZE_STANDARD,
        FONT_SIZE_LARGE,
        FONT_SIZE_EXTRALARGE,
        FONT_SIZE_MAX
    } FontSize;

    /**
     *  @enum FontStyle
     *  @brief Available Fontstyles
     */
    typedef enum FontStyle {
        FONT_STYLE_EMBEDDED = -1,
        FONT_STYLE_DEFAULT,
        FONT_STYLE_MONOSPACED_SERIF,
        FONT_STYLE_PROPORTIONAL_SERIF,
        FONT_STYLE_MONOSPACED_SANSSERIF,
        FONT_STYLE_PROPORTIONAL_SANSSERIF,
        FONT_STYLE_CASUAL,
        FONT_STYLE_CURSIVE,
        FONT_STYLE_SMALL_CAPITALS,
        FONT_STYLE_MAX
    } FontStyle;

    /**
     *  @enum SupportedColors
     *  @brief Supported colors
     */
    typedef enum SupportedColors {
        COLOR_EMBEDDED = 0xFF000000,
        COLOR_BLACK = 0x00000000,
        COLOR_WHITE = 0x00FFFFFF,
        COLOR_RED = 0x00FF0000,
        COLOR_GREEN = 0x0000FF00,
        COLOR_BLUE = 0x000000FF,
        COLOR_YELLOW = 0x00FFFF00,
        COLOR_MAGENTA = 0x00FF00FF,
        COLOR_CYAN = 0x0000FFFF
    } SupportedColors;

    
    /**
     * @brief A mapping between color names and their corresponding SupportedColors enum values.
     *
     * This map is used to convert human-readable color names (as strings) into their
     * corresponding enum values defined in SupportedColors. It includes standard colors
     * and a special "auto" value for embedded/default behavior.
     */
    const std::map< std::string, SupportedColors> ColorMapTable {
        { "black", COLOR_BLACK},
        { "white", COLOR_WHITE},
        { "red", COLOR_RED},
        { "green", COLOR_GREEN},
        { "blue", COLOR_BLUE},
        { "yellow", COLOR_YELLOW},
        { "magenta", COLOR_MAGENTA},
        { "cyan", COLOR_CYAN},
        { "auto", COLOR_EMBEDDED}
    };

    /**
     *  @enum EdgeType
     *  @brief Available Edge Types
     */
    typedef enum EdgeType {
        EDGE_TYPE_EMBEDDED = -1,
        EDGE_TYPE_NONE,
        EDGE_TYPE_RAISED,
        EDGE_TYPE_DEPRESSED,
        EDGE_TYPE_UNIFORM,
        EDGE_TYPE_SHADOW_LEFT,
        EDGE_TYPE_SHADOW_RIGHT,
        EDGE_TYPE_MAX
    } EdgeType;

    /**
     *  @enum Opacity
     *  @brief Available Opacity options
     */
    typedef enum Opacity {
        OPACITY_EMBEDDED = -1,
        OPACITY_SOLID,
        OPACITY_FLASHING,
        OPACITY_TRANSLUCENT,
        OPACITY_TRANSPARENT,
        OPACITY_MAX
    } Opacity;

    /**
     * @enum AttribPosInArray
     * @brief Provides the indexing postion in array for attributes
     */
    typedef enum AttribPosInArray {
        FONT_COLOR_ARR_POSITION = 0,
        BACKGROUND_COLOR_ARR_POSITION,
        FONT_OPACITY_ARR_POSITION,
        BACKGROUND_OPACITY_ARR_POSITION,
        FONT_STYLE_ARR_POSITION,
        FONT_SIZE_ARR_POSITION,
        FONT_ITALIC_ARR_POSITION,
        FONT_UNDERLINE_ARR_POSITION,
        BORDER_TYPE_ARR_POSITION,
        BORDER_COLOR_ARR_POSITION,
        WIN_COLOR_ARR_POSITION,
        WIN_OPACITY_ARR_POSITION,
        EDGE_TYPE_ARR_POSITION,
        EDGE_COLOR_ARR_POSITION
    } AttribPosInArray;

    TextStyleAttributes();

    TextStyleAttributes(const TextStyleAttributes&)  = delete;
    TextStyleAttributes& operator=(const TextStyleAttributes&);

	/**
	 * @fn getAttributes
	 *
	 * @param[in] options            A JSON string specifying text style attributes. Recognized keys (case-insensitive) and their expected values:
	 *
	 *                               - "penSize":
	 *                                 - "small", "standard", "medium", "large", "extra_large", "auto"
	 *
	 *                               - "fontStyle":
	 *                                 - "default", "monospaced_serif"/"monospaced serif", "proportional_serif"/"proportional serif",
	 *                                   "monospaced_sanserif"/"monospaced sans serif", "proportional_sanserif"/"proportional sans serif",
	 *                                   "casual", "cursive", "smallcaps"/"small capital", "auto"
	 *
	 *                               - "textForegroundColor", "textBackgroundColor", "textEdgeColor"`, "windowFillColor":
	 *                                 - "black", "white", "red", "green", "blue", "yellow", "magenta", "cyan", "auto"
	 *
	 *                               - "textEdgeStyle":
	 *                                 - "none", "raised", "depressed", "uniform", "drop_shadow_left"/"left drop shadow",
	 *                                   "drop_shadow_right"/"right drop shadow", "auto"
	 *
	 *                               - "textForegroundOpacity", "textBackgroundOpacity", "windowFillOpacity":
	 *                                 - "solid", "flash", "translucent", "transparent", "auto"
	 *
	 * @param[out] attributesValues  A reference to a fixed array of 14 `uint32_t` values, where each index corresponds to a specific attribute.
	 *                               Array index mapping (via AttribPosInArray):
	 *                               - [0] FONT_COLOR_ARR_POSITION           
	 *                               - [1] BACKGROUND_COLOR_ARR_POSITION     
	 *                               - [2] FONT_OPACITY_ARR_POSITION          
	 *                               - [3] BACKGROUND_OPACITY_ARR_POSITION    
	 *                               - [4] FONT_STYLE_ARR_POSITION            
	 *                               - [5] FONT_SIZE_ARR_POSITION             
	 *                               - [6] FONT_ITALIC_ARR_POSITION           
	 *                               - [7] FONT_UNDERLINE_ARR_POSITION        
	 *                               - [8] BORDER_TYPE_ARR_POSITION           
	 *                               - [9] BORDER_COLOR_ARR_POSITION          
	 *                               - [10] WIN_COLOR_ARR_POSITION           
	 *                               - [11] WIN_OPACITY_ARR_POSITION          
	 *                               - [12] EDGE_TYPE_ARR_POSITION            
	 *                               - [13] EDGE_COLOR_ARR_POSITION           
	 *
	 * @param[out] attributesMask    A 32-bit bitmask indicating which attributes were successfully parsed.
	 *                               Each bit i corresponds to index i in attributesValues. If (1 << i) is set,
	 *                               then the value at attributesValues[i] is valid.
	 *
	 * @return int                   Returns 0 if at least one attribute is successfully parsed, or -1 if input is empty,
	 *                               if none of the known attributes could be parsed.
	 */
	int getAttributes(std::string options, attributesType &attributesValues, uint32_t &attributesMask);

private:
    
    /**
     * @struct Attributes
     * @brief Defines the visual styling attributes for text rendering.
     *
     * This structure encapsulates various properties related to font and background
     * styling, including colors, opacity, font characteristics, and edge effects.
     */
    struct Attributes {
        SupportedColors fontColor;           /**< Color of the font text. */
        SupportedColors backgroundColor;     /**< Background color behind the text. */
        Opacity fontOpacity;                 /**< Opacity level of the font text. */
        Opacity backgroundOpacity;           /**< Opacity level of the background. */
        FontStyle fontStyle;                 /**< Style of the font (e.g., italic, bold). */
        FontSize fontSize;                   /**< Size of the font. */
        Opacity windowOpacity;               /**< Opacity of the window or bounding box. */
        SupportedColors windowColor;         /**< Color of the window or bounding box. */
        EdgeType edgeType;                   /**< Type of edge or outline applied to the text. */
        SupportedColors edgeColor;           /**< Color of the text edge or outline. */
    };


	/**
	 * @fn getFontSize
	 *
	 * @param[in] input   -   String representing the font size. Expected values are:
	 *                        - "small"
	 *                        - "standard" or "medium"
	 *                        - "large"
	 *                        - "extra_large"
	 *                        - "auto"
	 *                        The comparison is case-insensitive.
	 *
	 * @param[out] fontSizeOut - font size option for the input value
	 *
	 * @return int Returns 0 on success, -1 on failure
	 */
	int getFontSize(std::string input, FontSize *fontSizeOut);

	/**
	 * @fn getFontStyle
	 *
	 * @param[in] input     -   String representing the font style. Expected values are:
	 *                          - "default"
	 *                          - "monospaced_serif" or "monospaced serif"
	 *                          - "proportional_serif" or "proportional serif"
	 *                          - "monospaced_sanserif" or "monospaced sans serif"
	 *                          - "proportional_sanserif" or "proportional sans serif"
	 *                          - "casual"
	 *                          - "cursive"
	 *                          - "smallcaps" or "small capital"
	 *                          - "auto"
	 *
	 * @param[out] fontStyleOut - font style option for the input value.
	 *
	 * @return int              - Returns 0 on successful parsing, -1 on failure 
	 */
	int getFontStyle(std::string input, FontStyle *fontStyleOut);

	/**
	 * @fn getColor
	 *
	 * @param[in] input       input color value. Expected values are: 
	 *                        - "black"
	 *                        - "white"
	 *                        - "red"
	 *                        - "green"
	 *                        - "blue"
	 *                        - "yellow"
	 *                        - "magenta"
	 *                        - "cyan"
	 *                        - "auto"
	 *
	 * @param[out] colorOut   - color option for the input value.
	 *
	 * @return int            Returns 0 on success, -1 on failure 
	 */
	int getColor(std::string input, SupportedColors *colorOut);

	/**
	 * @brief Converts an input string representing an edge type to the corresponding EdgeType enum value.
	 *
	 * @fn getEdgeType
	 *
	 * @param[in] input         - input edge type value. Expected values are: 
	 *                          - "none"
	 *                          - "raised"
	 *                          - "depressed"
	 *                          - "uniform"
	 *                          - "drop_shadow_left" or "left drop shadow"
	 *                          - "drop_shadow_right" or "right drop shadow"
	 *                          - "auto"
	 *
	 * @param[out] edgeTypeOut  - edge type option for the input value
	 *
	 * @return int              Returns 0 on success, -1 on failure
	 */
	int getEdgeType(std::string input, EdgeType *edgeTypeOut);

	/**
	 * @fn getOpacity
	 *
	 * @param[in] input         input opacity value. Expected values are:
	 *                          - "solid"
	 *                          - "flash"
	 *                          - "translucent"
	 *                          - "transparent"
	 *                          - "auto"
	 *
	 * @param[out] opacityOut   - opacity option for the input value.
	 *
	 * @return int              Returns 0 on success, -1 on failure 
	 */
	int getOpacity(std::string input, Opacity *opacityOut);
};

#endif // __TEXT_STYLE_ATTRIBUTES_H__
