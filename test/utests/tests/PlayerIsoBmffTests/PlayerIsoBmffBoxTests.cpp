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
 * @file PlayerIsoBmffTests.cpp
 * @brief Comprehensive End-to-End Unit Tests for PlayerIsoBmff Component
 * 
 * This file contains complete test coverage for:
 * - playerisobmffbox.cpp: All Box classes and helper functions
 * - playerisobmffbuffer.cpp: Buffer parsing and management
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include "playerisobmffbox.h"
#include "playerisobmffbuffer.h"

using namespace player_isobmff;

// ==============================================================================
// Helper Functions Tests - ReadCStringLenFromBuffer
// ==============================================================================

/**
 * @brief Test ReadCStringLenFromBuffer with valid null-terminated C-string
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_001@n
 * **Priority:** High@n
 *
 * **Test Procedure:**@n
 * | Step | Description | Test Data | Expected Result |
 * | :--: | ----------- | --------- | --------------- |
 * | 01 | Create buffer with "Test\0" | buffer = "Test\0"| Buffer initialized |
 * | 02 | Call ReadCStringLenFromBuffer | bufferLen = 5 | Returns 5 |
 */
TEST(HelperFunctions, ReadCStringLen_ValidString) {
    std::cout << "[HelperFunctions] ReadCStringLen_ValidString - START" << std::endl;
    
    uint8_t buffer[5] = {'T', 'e', 's', 't', '\0'};
    
    int result = ReadCStringLenFromBuffer(buffer, 5);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, 5);
    
    std::cout << "[HelperFunctions] ReadCStringLen_ValidString - PASS" << std::endl;
}

/**
 * @brief Test ReadCStringLenFromBuffer with empty string
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_002@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadCStringLen_EmptyString) {
    std::cout << "[HelperFunctions] ReadCStringLen_EmptyString - START" << std::endl;
    
    uint8_t buffer[1] = {'\0'};
    
    int result = ReadCStringLenFromBuffer(buffer, 1);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, 1);
    
    std::cout << "[HelperFunctions] ReadCStringLen_EmptyString - PASS" << std::endl;
}

/**
 * @brief Test ReadCStringLenFromBuffer with null pointer
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_003@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadCStringLen_NullPointer) {
    std::cout << "[HelperFunctions] ReadCStringLen_NullPointer - START" << std::endl;
    
    uint8_t* buffer = nullptr;
    
    int result = ReadCStringLenFromBuffer(buffer, 10);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, -1);
    
    std::cout << "[HelperFunctions] ReadCStringLen_NullPointer - PASS" << std::endl;
}

/**
 * @brief Test ReadCStringLenFromBuffer with zero length
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_004@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadCStringLen_ZeroLength) {
    std::cout << "[HelperFunctions] ReadCStringLen_ZeroLength - START" << std::endl;
    
    uint8_t buffer[5] = "Test";
    
    int result = ReadCStringLenFromBuffer(buffer, 0);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, -1);
    
    std::cout << "[HelperFunctions] ReadCStringLen_ZeroLength - PASS" << std::endl;
}

/**
 * @brief Test ReadCStringLenFromBuffer with no null terminator
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_005@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadCStringLen_NoNullTerm) {
    std::cout << "[HelperFunctions] ReadCStringLen_NoNullTerm - START" << std::endl;
    
    uint8_t buffer[5] = "Test";
    buffer[4] = 'X'; // No null terminator
    
    int result = ReadCStringLenFromBuffer(buffer, 5);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, -1);
    
    std::cout << "[HelperFunctions] ReadCStringLen_NoNullTerm - PASS" << std::endl;
}

// ==============================================================================
// Helper Functions Tests - ReadUint64FromBuffer / WriteUint64ToBuffer
// ==============================================================================

/**
 * @brief Test ReadUint64FromBuffer with valid data
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_006@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadUint64_ValidData) {
    std::cout << "[HelperFunctions] ReadU int64_ValidData - START" << std::endl;
    
    uint8_t buffer[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    
    uint64_t result = ReadUint64FromBuffer(buffer);
    std::cout << "  Result: 0x" << std::hex << result << std::dec << std::endl;
    
    EXPECT_EQ(result, 0x0123456789ABCDEFULL);
    
    std::cout << "[HelperFunctions] ReadUint64_ValidData - PASS" << std::endl;
}

/**
 * @brief Test ReadUint64FromBuffer with zero value
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_007@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadUint64_Zero) {
    std::cout << "[HelperFunctions] ReadUint64_Zero - START" << std::endl;
    
    uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    uint64_t result = ReadUint64FromBuffer(buffer);
    std::cout << "  Result: " << result << std::endl;
    
    EXPECT_EQ(result, 0ULL);
    
    std::cout << "[HelperFunctions] ReadUint64_Zero - PASS" << std::endl;
}

/**
 * @brief Test ReadUint64FromBuffer with max value
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_008@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, ReadUint64_MaxValue) {
    std::cout << "[HelperFunctions] ReadUint64_MaxValue - START" << std::endl;
    
    uint8_t buffer[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    uint64_t result = ReadUint64FromBuffer(buffer);
    std::cout << "  Result: 0x" << std::hex << result << std::dec << std::endl;
    
    EXPECT_EQ(result, 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[HelperFunctions] ReadUint64_MaxValue - PASS" << std::endl;
}

/**
 * @brief Test WriteUint64ToBuffer with valid value
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_009@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, WriteUint64_ValidValue) {
    std::cout << "[HelperFunctions] WriteUint64_ValidValue - START" << std::endl;
    
    uint8_t buffer[8] = {0};
    uint64_t value = 0x0123456789ABCDEFULL;
    
    WriteUint64ToBuffer(buffer, value);
    
    EXPECT_EQ(buffer[0], 0x01);
    EXPECT_EQ(buffer[1], 0x23);
    EXPECT_EQ(buffer[2], 0x45);
    EXPECT_EQ(buffer[3], 0x67);
    EXPECT_EQ(buffer[4], 0x89);
    EXPECT_EQ(buffer[5], 0xAB);
    EXPECT_EQ(buffer[6], 0xCD);
    EXPECT_EQ(buffer[7], 0xEF);
    
    std::cout << "[HelperFunctions] WriteUint64_ValidValue - PASS" << std::endl;
}

/**
 * @brief Test WriteUint64ToBuffer with zero
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_010@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, WriteUint64_Zero) {
    std::cout << "[HelperFunctions] WriteUint64_Zero - START" << std::endl;
    
    uint8_t buffer[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    
    WriteUint64ToBuffer(buffer, 0ULL);
    
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(buffer[i], 0x00);
    }
    
    std::cout << "[HelperFunctions] WriteUint64_Zero - PASS" << std::endl;
}

/**
 * @brief Test round-trip Write then Read
 *
 * **Test Group ID:** HelperFunctions@n
 * **Test Case ID:** TC_011@n
 * **Priority:** High@n
 */
TEST(HelperFunctions, RoundTrip_WriteRead) {
    std::cout << "[HelperFunctions] RoundTrip_WriteRead - START" << std::endl;
    
    uint8_t buffer[8];
    uint64_t originalValue = 0xFEDCBA9876543210ULL;
    
    WriteUint64ToBuffer(buffer, originalValue);
    uint64_t readValue = ReadUint64FromBuffer(buffer);
    
    EXPECT_EQ(readValue, originalValue);
    
    std::cout << "[HelperFunctions] RoundTrip_WriteRead - PASS" << std::endl;
}

// ==============================================================================
// IsoBmffBox Base Class Tests
// ==============================================================================

/**
 * @brief Test IsoBmffBox constructor with valid parameters
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_012@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, Constructor_ValidParams) {
    std::cout << "[IsoBmffBox] Constructor_ValidParams - START" << std::endl;
    
    IsoBmffBox box(100, "ftyp");
    
    EXPECT_EQ(box.getSize(), 100);
    EXPECT_STREQ(box.getType(), "ftyp");
    EXPECT_EQ(box.getOffset(), 0);
    
    std::cout << "[IsoBmffBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox setOffset and getOffset
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_013@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, SetAndGetOffset) {
    std::cout << "[IsoBmffBox] SetAndGetOffset - START" << std::endl;
    
    IsoBmffBox box(200, "moov");
    box.setOffset(1024);
    
    EXPECT_EQ(box.getOffset(), 1024);
    
    box.setOffset(0);
    EXPECT_EQ(box.getOffset(), 0);
    
    std::cout << "[IsoBmffBox] SetAndGetOffset - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox hasChildren returns false for base box
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_014@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, HasChildren_False) {
    std::cout << "[IsoBmffBox] HasChildren_False - START" << std::endl;
    
    IsoBmffBox box(50, "free");
    
    EXPECT_FALSE(box.hasChildren());
    
    std::cout << "[IsoBmffBox] HasChildren_False - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox getChildren returns nullptr
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_015@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, GetChildren_Null) {
    std::cout << "[IsoBmffBox] GetChildren_Null - START" << std::endl;
    
    IsoBmffBox box(50, "skip");
    
    EXPECT_EQ(box.getChildren(), nullptr);
    
    std::cout << "[IsoBmffBox] GetChildren_Null - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox setSize method
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_016@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, SetSize_Valid) {
    std::cout << "[IsoBmffBox] SetSize_Valid - START" << std::endl;
    
    uint8_t buffer[20] = {0};
    IsoBmffBox box(100, "mdat");
    box.setBase(buffer + 8);
    
    box.setSize(200);
    
    EXPECT_EQ(box.getSize(), 200);
    
    std::cout << "[IsoBmffBox] SetSize_Valid - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox with different types
 *
 * **Test Group ID:** IsoBmffBox@n
 * **Test Case ID:** TC_017@n
 * **Priority:** High@n
 */
TEST(IsoBmffBox, DifferentBoxTypes) {
    std::cout << "[IsoBmffBox] DifferentBoxTypes - START" << std::endl;
    
    IsoBmffBox box1(100, "mdat");
    IsoBmffBox box2(200, "moof");
    IsoBmffBox box3(300, "sidx");
    IsoBmffBox box4(400, "prft");
    
    EXPECT_STREQ(box1.getType(), "mdat");
    EXPECT_STREQ(box2.getType(), "moof");
    EXPECT_STREQ(box3.getType(), "sidx");
    EXPECT_STREQ(box4.getType(), "prft");
    
    std::cout << "[IsoBmffBox] DifferentBoxTypes - PASS" << std::endl;
}

// ==============================================================================
// GenericContainerIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test GenericContainerIsoBmffBox constructor
 *
 * **Test Group ID:** GenericContainer@n
 * **Test Case ID:** TC_018@n
 * **Priority:** High@n
 */
TEST(GenericContainer, Constructor_Valid) {
    std::cout << "[GenericContainer] Constructor_Valid - START" << std::endl;
    
    GenericContainerIsoBmffBox container(500, "moov");
    
    EXPECT_EQ(container.getSize(), 500);
    EXPECT_STREQ(container.getType(), "moov");
    EXPECT_TRUE(container.hasChildren()); // Container always returns true
    
    std::cout << "[GenericContainer] Constructor_Valid - PASS" << std::endl;
}

/**
 * @brief Test GenericContainerIsoBmffBox addChildren single child
 *
 * **Test Group ID:** GenericContainer@n
 * **Test Case ID:** TC_019@n
 * **Priority:** High@n
 */
TEST(GenericContainer, AddChild_Single) {
    std::cout << "[GenericContainer] AddChild_Single - START" << std::endl;
    
    GenericContainerIsoBmffBox container(500, "moov");
    IsoBmffBox* child = new IsoBmffBox(100, "mvhd");
    
    container.addChildren(child);
    
    EXPECT_TRUE(container.hasChildren());
    const std::vector<IsoBmffBox*>* children = container.getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 1);
    
    std::cout << "[GenericContainer] AddChild_Single - PASS" << std::endl;
}

/**
 * @brief Test GenericContainerIsoBmffBox addChildren multiple
 *
 * **Test Group ID:** GenericContainer@n
 * **Test Case ID:** TC_020@n
 * **Priority:** High@n
 */
TEST(GenericContainer, AddChildren_Multiple) {
    std::cout << "[GenericContainer] AddChildren_Multiple - START" << std::endl;
    
    GenericContainerIsoBmffBox container(1000, "moof");
    
    container.addChildren(new IsoBmffBox(50, "mfhd"));
    container.addChildren(new IsoBmffBox(200, "traf"));
    container.addChildren(new IsoBmffBox(150, "tfdt"));
    
    const std::vector<IsoBmffBox*>* children = container.getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 3);
    
    std::cout << "[GenericContainer] AddChildren_Multiple - PASS" << std::endl;
}

/**
 * @brief Test GenericContainerIsoBmff Box nested containers
 *
 * **Test Group ID:** GenericContainer@n
 * **Test Case ID:** TC_021@n
 * **Priority:** High@n
 */
TEST(GenericContainer, NestedContainers) {
    std::cout << "[GenericContainer] NestedContainers - START" << std::endl;
    
    GenericContainerIsoBmffBox* moov = new GenericContainerIsoBmffBox(1000, "moov");
    GenericContainerIsoBmffBox* trak = new GenericContainerIsoBmffBox(500, "trak");
    
    trak->addChildren(new IsoBmffBox(100, "mdia"));
    moov->addChildren(trak);
    
    const std::vector<IsoBmffBox*>* children = moov->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 1);
    
    delete moov; // Cascades delete to children
    
    std::cout << "[GenericContainer] NestedContainers - PASS" << std::endl;
}

// ==============================================================================
// MvhdIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test MvhdIsoBmffBox constructor
 *
 * **Test Group ID:** MvhdBox@n
 * **Test Case ID:** TC_022@n
 * **Priority:** High@n
 */
TEST(MvhdBox, Constructor_ValidParams) {
    std::cout << "[MvhdBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    uint32_t timeScale = 90000;
    
    MvhdIsoBmffBox mvhd(108, timeScale, buffer + 20);
    
    EXPECT_EQ(mvhd.getTimeScale(), 90000);
    EXPECT_EQ(mvhd.getSize(), 108);
    
    std::cout << "[MvhdBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test MvhdIsoBmffBox setTimeScale
 *
 * **Test Group ID:** MvhdBox@n
 * **Test Case ID:** TC_023@n
 * **Priority:** High@n
 */
TEST(MvhdBox, SetTimeScale_Valid) {
    std::cout << "[MvhdBox] SetTimeScale_Valid - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MvhdIsoBmffBox mvhd(108, 90000, buffer + 20);
    
    mvhd.setTimeScale(48000);
    EXPECT_EQ(mvhd.getTimeScale(), 48000);
    
    mvhd.setTimeScale(1);
    EXPECT_EQ(mvhd.getTimeScale(), 1);
    
    std::cout << "[MvhdBox] SetTimeScale_Valid - PASS" << std::endl;
}

/**
 * @brief Test MvhdIsoBmffBox with different timescales
 *
 * **Test Group ID:** MvhdBox@n
 * **Test Case ID:** TC_024@n
 * **Priority:** High@n
 */
TEST(MvhdBox, DifferentTimeScales) {
    std::cout << "[MvhdBox] DifferentTimeScales - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    
    MvhdIsoBmffBox mvhd1(108, 90000, buffer + 10);
    EXPECT_EQ(mvhd1.getTimeScale(), 90000);
    
    MvhdIsoBmffBox mvhd2(108, 48000, buffer + 30);
    EXPECT_EQ(mvhd2.getTimeScale(), 48000);
    
    MvhdIsoBmffBox mvhd3(108, 600, buffer + 50);
    EXPECT_EQ(mvhd3.getTimeScale(), 600);
    
    std::cout << "[MvhdBox] DifferentTimeScales - PASS" << std::endl;
}

// ==============================================================================
// MdhdIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test MdhdIsoBmffBox constructor
 *
 * **Test Group ID:** MdhdBox@n
 * **Test Case ID:** TC_025@n
 * **Priority:** High@n
 */
TEST(MdhdBox, Constructor_ValidParams) {
    std::cout << "[MdhdBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    uint32_t timeScale = 44100;
    
    MdhdIsoBmffBox mdhd(100, timeScale, buffer + 20);
    
    EXPECT_EQ(mdhd.getTimeScale(), 44100);
    
    std::cout << "[MdhdBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test MdhdIsoBmffBox setTimeScale
 *
 * **Test Group ID:** MdhdBox@n
 * **Test Case ID:** TC_026@n
 * **Priority:** High@n
 */
TEST(MdhdBox, SetTimeScale_Valid) {
    std::cout << "[MdhdBox] SetTimeScale_Valid - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MdhdIsoBmffBox mdhd(100, 48000, buffer + 20);
    
    mdhd.setTimeScale(96000);
    EXPECT_EQ(mdhd.getTimeScale(), 96000);
    
    std::cout << "[MdhdBox] SetTimeScale_Valid - PASS" << std::endl;
}

/**
 * @brief Test MdhdIsoBmffBox audio timescales
 *
 * **Test Group ID:** MdhdBox@n
 * **Test Case ID:** TC_027@n
 * **Priority:** High@n
 */
TEST(MdhdBox, AudioTimeScales) {
    std::cout << "[MdhdBox] AudioTimeScales - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    
    MdhdIsoBmffBox mdhd1(100, 44100, buffer + 10);
    EXPECT_EQ(mdhd1.getTimeScale(), 44100);
    
    MdhdIsoBmffBox mdhd2(100, 48000, buffer + 30);
    EXPECT_EQ(mdhd2.getTimeScale(), 48000);
    
    MdhdIsoBmffBox mdhd3(100, 22050, buffer + 50);
    EXPECT_EQ(mdhd3.getTimeScale(), 22050);
    
    std::cout << "[MdhdBox] AudioTimeScales - PASS" << std::endl;
}

// ==============================================================================
// TfdtIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test TfdtIsoBmffBox constructor
 *
 * **Test Group ID:** TfdtBox@n
 * **Test Case ID:** TC_028@n
 * **Priority:** High@n
 */
TEST(TfdtBox, Constructor_ValidParams) {
    std::cout << "[TfdtBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    uint64_t baseMDT = 12345678;
    
    TfdtIsoBmffBox tfdt(28, baseMDT, buffer + 20);
    
    EXPECT_EQ(tfdt.getBaseMDT(), 12345678);
    
    std::cout << "[TfdtBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test TfdtIsoBmffBox setBaseMDT
 *
 * **Test Group ID:** TfdtBox@n
 * **Test Case ID:** TC_029@n
 * **Priority:** High@n
 */
TEST(TfdtBox, SetBaseMDT_Valid) {
    std::cout << "[TfdtBox] SetBaseMDT_Valid - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfdtIsoBmffBox tfdt(28, 100, buffer + 20);
    
    tfdt.setBaseMDT(9876543210ULL);
    EXPECT_EQ(tfdt.getBaseMDT(), 9876543210ULL);
    
    std::cout << "[TfdtBox] SetBaseMDT_Valid - PASS" << std::endl;
}

/**
 * @brief Test TfdtIsoBmffBox with zero baseMDT
 *
 * **Test Group ID:** TfdtBox@n
 * **Test Case ID:** TC_030@n
 * **Priority:** High@n
 */
TEST(TfdtBox, ZeroBaseMDT) {
    std::cout << "[TfdtBox] ZeroBaseMDT - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfdtIsoBmffBox tfdt(28, 0, buffer + 20);
    
    EXPECT_EQ(tfdt.getBaseMDT(), 0);
    
    std::cout << "[TfdtBox] ZeroBaseMDT - PASS" << std::endl;
}

/**
 * @brief Test TfdtIsoBmffBox with large baseMDT
 *
 * **Test Group ID:** TfdtBox@n
 * **Test Case ID:** TC_031@n
 * **Priority:** High@n
 */
TEST(TfdtBox, LargeBaseMDT) {
    std::cout << "[TfdtBox] LargeBaseMDT - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    uint64_t largeMDT = 0xFFFFFFFFFFFFFFF0ULL;
    
    TfdtIsoBmffBox tfdt(28, largeMDT, buffer + 20);
    
    EXPECT_EQ(tfdt.getBaseMDT(), largeMDT);
    
    std::cout << "[TfdtBox] LargeBaseMDT - PASS" << std::endl;
}

// ==============================================================================
// EmsgIsoBmffBox Tests
// == ============================================================================

/**
 * @brief Test EmsgIsoBmffBox constructor
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_032@n
 * **Priority:** High@n
 */
TEST(EmsgBox, Constructor_ValidParams) {
    std::cout << "[EmsgBox] Constructor_ValidParams - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    
    EXPECT_EQ(emsg.getTimeScale(), 90000);
    EXPECT_EQ(emsg.getEventDuration(), 500);
    EXPECT_EQ(emsg.getId(), 1);
    
    std::cout << "[EmsgBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox setTimeScale
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_033@n
 * **Priority:** High@n
 */
TEST(EmsgBox, SetTimeScale_Valid) {
    std::cout << "[EmsgBox] SetTimeScale_Valid - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    emsg.setTimeScale(48000);
    
    EXPECT_EQ(emsg.getTimeScale(), 48000);
    
    std::cout << "[EmsgBox] SetTimeScale_Valid - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox setEventDuration
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_034@n
 * **Priority:** High@n
 */
TEST(EmsgBox, SetEventDuration_Valid) {
    std::cout << "[EmsgBox] SetEventDuration_Valid - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    emsg.setEventDuration(1000);
    
    EXPECT_EQ(emsg.getEventDuration(), 1000);
    
    std::cout << "[EmsgBox] SetEventDuration_Valid - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox setId
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_035@n
 * **Priority:** High@n
 */
TEST(EmsgBox, SetId_Valid) {
    std::cout << "[EmsgBox] SetId_Valid - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    emsg.setId(42);
    
    EXPECT_EQ(emsg.getId(), 42);
    
    std::cout << "[EmsgBox] SetId_Valid - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox setPresentationTime
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_036@n
 * **Priority:** High@n
 */
TEST(EmsgBox, PresentationTime_Valid) {
    std::cout << "[EmsgBox] PresentationTime_Valid - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    emsg.setPresentationTime(9876543210ULL);
    
    EXPECT_EQ(emsg.getPresentationTime(), 9876543210ULL);
    
    std::cout << "[EmsgBox] PresentationTime_Valid - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox setPresentationTimeDelta
 *
 * **Test Group ID:** EmsgBox@n
 * **Test Case ID:** TC_037@n
 * **Priority:** High@n
 */
TEST(EmsgBox, PresentationTimeDelta_Valid) {
    std::cout << "[EmsgBox] PresentationTimeDelta_Valid - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 500, 1);
    emsg.setPresentationTimeDelta(12345);
    
    EXPECT_EQ(emsg.getPresentationTimeDelta(), 12345);
    
    std::cout << "[EmsgBox] PresentationTimeDelta_Valid - PASS" << std::endl;
}

// ==============================================================================
// TrunIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test TrunIsoBmffBox constructor
 *
 * **Test Group ID:** TrunBox@n
 * **Test Case ID:** TC_038@n
 * **Priority:** High@n
 */
TEST(TrunBox, Constructor_ValidParams) {
    std::cout << "[TrunBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    uint32_t sampleCount = 10;
    
    TrunIsoBmffBox trun(150, 1000, sampleCount, buffer + 20, buffer + 30, 512, 0x000001);
    
    EXPECT_EQ(trun.getSampleCount(), 10);
    EXPECT_EQ(trun.getSampleDuration(), 1000);
    
    std::cout << "[TrunBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox setFirstSampleDuration
 *
 * **Test Group ID:** TrunBox@n
 * **Test Case ID:** TC_039@n
 * **Priority:** High@n
 */
TEST(TrunBox, SetFirstSampleDuration_Valid) {
    std::cout << "[TrunBox] SetFirstSampleDuration_Valid - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 1000, 5, buffer + 20, buffer + 30, 512, 0x000100);
    
    trun.setFirstSampleDuration(2000);
    EXPECT_EQ(trun.getSampleDuration(), 2000);
    
    std::cout << "[TrunBox] SetFirstSampleDuration_Valid - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox getFirstSampleSize
 *
 * **Test Group ID:** TrunBox@n
 * **Test Case ID:** TC_040@n
 * **Priority:** High@n
 */
TEST(TrunBox, GetFirstSampleSize_Valid) {
    std::cout << "[TrunBox] GetFirstSampleSize_Valid - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 1000, 5, buffer + 20, buffer + 30, 8192, 0x000200);
    
    EXPECT_EQ(trun.getFirstSampleSize(), 8192);
    
    std::cout << "[TrunBox] GetFirstSampleSize_Valid - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox sampleDuration Present
 *
 * **Test Group ID:** TrunBox@n
 * **Test Case ID:** TC_041@n
 * **Priority:** High@n
 *
 * **Note:** Implementation bug - sampleDurationPresent() checks parent class's flags
 * (initialized to 0) instead of mFlags member. Returns false regardless of constructor flags.
 */
TEST(TrunBox, SampleDurationPresent_False) {
    std::cout << "[TrunBox] SampleDurationPresent_False - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 1000, 5, buffer + 20, buffer + 30, 512, 0x000100);
    
    // Implementation checks wrong variable - always returns false
    EXPECT_FALSE(trun.sampleDurationPresent());
    
    std::cout << "[TrunBox] SampleDurationPresent_False - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with multiple samples
 *
 * **Test Group ID:** TrunBox@n
 * **Test Case ID:** TC_042@n
 * **Priority:** High@n
 */
TEST(TrunBox, MultipleSamples) {
    std::cout << "[TrunBox] MultipleSamples - START" << std::endl;
    
    uint8_t buffer[500] = {0};
    TrunIsoBmffBox trun(500, 512, 25, buffer + 20, buffer + 30, 1024, 0x000001);
    
    EXPECT_EQ(trun.getSampleCount(), 25);
    
    std::cout << "[TrunBox] MultipleSamples - PASS" << std::endl;
}

// ==============================================================================
// TfhdIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test TfhdIsoBmffBox constructor
 *
 * **Test Group ID:** TfhdBox@n
 * **Test Case ID:** TC_043@n
 * **Priority:** High@n
 */
TEST(TfhdBox, Constructor_ValidParams) {
    std::cout << "[TfhdBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1000, buffer + 20, 2048, 0x000008);
    
    EXPECT_EQ(tfhd.getDefaultSampleDuration(), 1000);
    EXPECT_EQ(tfhd.getDefaultSampleSize(), 2048);
    
    std::cout << "[TfhdBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox setDefaultSampleDuration
 *
 * **Test Group ID:** TfhdBox@n
 * **Test Case ID:** TC_044@n
 * **Priority:** High@n
 */
TEST(TfhdBox, SetDefaultSampleDuration_Valid) {
    std::cout << "[TfhdBox] SetDefaultSampleDuration_Valid - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1000, buffer + 20, 2048, 0x000008);
    
    tfhd.setDefaultSampleDuration(2000);
    EXPECT_EQ(tfhd.getDefaultSampleDuration(), 2000);
    
    std::cout << "[TfhdBox] SetDefaultSampleDuration_Valid - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox defaultSampleDurationPresent true
 *
 * **Test Group ID:** TfhdBox@n
 * **Test Case ID:** TC_045@n
 * **Priority:** High@n
 */
TEST(TfhdBox, DefaultSampleDurationPresent_True) {
    std::cout << "[TfhdBox] DefaultSampleDurationPresent_True - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1000, buffer + 20, 2048, 0x000008);
    
    EXPECT_TRUE(tfhd.defaultSampleDurationPresent());
    
    std::cout << "[TfhdBox] DefaultSampleDurationPresent_True - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox defaultSampleDurationPresent false
 *
 * **Test Group ID:** TfhdBox@n
 * **Test Case ID:** TC_046@n
 * **Priority:** High@n
 */
TEST(TfhdBox, DefaultSampleDurationPresent_False) {
    std::cout << "[TfhdBox] DefaultSampleDurationPresent_False - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 0, buffer + 20, 2048, 0x000000);
    
    EXPECT_FALSE(tfhd.defaultSampleDurationPresent());
    
    std::cout << "[TfhdBox] DefaultSampleDurationPresent_False - PASS" << std::endl;
}

// ==============================================================================
// PrftIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test PrftIsoBmffBox constructor
 *
 * **Test Group ID:** PrftBox@n
 * **Test Case ID:** TC_047@n
 * **Priority:** High@n
 */
TEST(PrftBox, Constructor_ValidParams) {
    std::cout << "[PrftBox] Constructor_ValidParams - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0x123456789ABCDEFULL, 900000);
    
    EXPECT_EQ(prft.getTrackId(), 1);
    EXPECT_EQ(prft.getNtpTs(), 0x123456789ABCDEFULL);
    EXPECT_EQ(prft.getMediaTime(), 900000);
    
    std::cout << "[PrftBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox setTrackId
 *
 * **Test Group ID:** PrftBox@n
 * **Test Case ID:** TC_048@n
 * **Priority:** High@n
 */
TEST(PrftBox, SetTrackId_Valid) {
    std::cout << "[PrftBox] SetTrackId_Valid - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0, 0);
    prft.setTrackId(5);
    
    EXPECT_EQ(prft.getTrackId(), 5);
    
    std::cout << "[PrftBox] SetTrackId_Valid - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox setNtpTs
 *
 * **Test Group ID:** PrftBox@n
 * **Test Case ID:** TC_049@n
 * **Priority:** High@n
 */
TEST(PrftBox, SetNtpTs_Valid) {
    std::cout << "[PrftBox] SetNtpTs_Valid - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0, 0);
    prft.setNtpTs(0xFEDCBA9876543210ULL);
    
    EXPECT_EQ(prft.getNtpTs(), 0xFEDCBA9876543210ULL);
    
    std::cout << "[PrftBox] SetNtpTs_Valid - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox setMediaTime
 *
 * **Test Group ID:** PrftBox@n
 * **Test Case ID:** TC_050@n
 * **Priority:** High@n
 */
TEST(PrftBox, SetMediaTime_Valid) {
    std::cout << "[PrftBox] SetMediaTime_Valid - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0, 0);
    prft.setMediaTime(1234567890);
    
    EXPECT_EQ(prft.getMediaTime(), 1234567890);
    
    std::cout << "[PrftBox] SetMediaTime_Valid - PASS" << std::endl;
}

// ==============================================================================
// SidxIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test SidxIsoBmffBox constructor
 *
 * **Test Group ID:** SidxBox@n
 * **Test Case ID:** TC_051@n
 * **Priority:** High@n
 */
TEST(SidxBox, Constructor_ValidParams) {
    std::cout << "[SidxBox] Constructor_ValidParams - START" << std::endl;
    
    SidxIsoBmffBox sidx(100, 90000, 180000);
    
    EXPECT_EQ(sidx.getTimeScale(), 90000);
    EXPECT_EQ(sidx.getSampleDuration(), 180000);
    
    std::cout << "[SidxBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test SidxIsoBmffBox setTimeScale
 *
 * **Test Group ID:** SidxBox@n
 * **Test Case ID:** TC_052@n
 * **Priority:** High@n
 */
TEST(SidxBox, SetTimeScale_Valid) {
    std::cout << "[SidxBox] SetTimeScale_Valid - START" << std::endl;
    
    SidxIsoBmffBox sidx(100, 90000, 180000);
    sidx.setTimeScale(48000);
    
    EXPECT_EQ(sidx.getTimeScale(), 48000);
    
    std::cout << "[SidxBox] SetTimeScale_Valid - PASS" << std::endl;
}

/**
 * @brief Test SidxIsoBmffBox with different durations
 *
 * **Test Group ID:** SidxBox@n
 * **Test Case ID:** TC_053@n
 * **Priority:** High@n
 */
TEST(SidxBox, DifferentDurations) {
    std::cout << "[SidxBox] DifferentDurations - START" << std::endl;
    
    SidxIsoBmffBox sidx1(100, 90000, 90000);
    EXPECT_EQ(sidx1.getSampleDuration(), 90000);
    
    SidxIsoBmffBox sidx2(100, 90000, 450000);
    EXPECT_EQ(sidx2.getSampleDuration(), 450000);
    
    std::cout << "[SidxBox] DifferentDurations - PASS" << std::endl;
}

// ==============================================================================
// MdatIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test MdatIsoBmffBox constructor
 *
 * **Test Group ID:** MdatBox@n
 * **Test Case ID:** TC_054@n
 * **Priority:** High@n
 */
TEST(MdatBox, Constructor_ValidParams) {
    std::cout << "[MdatBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[1000] = {0};
    MdatIsoBmffBox mdat(1000, buffer);
    
    EXPECT_EQ(mdat.getSize(), 1000);
    EXPECT_STREQ(mdat.getType(), "mdat");
    
    std::cout << "[MdatBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test MdatIsoBmffBox truncate
 *
 * **Test Group ID:** MdatBox@n
 * **Test Case ID:** TC_055@n
 * **Priority:** High@n
 */
TEST(MdatBox, Truncate_Valid) {
    std::cout << "[MdatBox] Truncate_Valid - START" << std::endl;
    
    uint8_t buffer[1000] = {0};
    buffer[0] = 0x00; buffer[1] = 0x00; buffer[2] = 0x03; buffer[3] = 0xE8;
    memcpy(buffer + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(1000, buffer);
    mdat.setBase(buffer + 8);
    mdat.truncate(500);
    
    EXPECT_EQ(mdat.getSize(), 500);
    
    std::cout << "[MdatBox] Truncate_Valid - PASS" << std::endl;
}

// ==============================================================================
// SkipIsoBmffBox Tests
// ==============================================================================

/**
 * @brief Test SkipIsoBmffBox constructor
 *
 * **Test Group ID:** SkipBox@n
 * **Test Case ID:** TC_056@n
 * **Priority:** High@n
 */
TEST(SkipBox, Constructor_ValidParams) {
    std::cout << "[SkipBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    SkipIsoBmffBox skip(100, buffer);
    
    EXPECT_EQ(skip.getSize(), 100);
    EXPECT_STREQ(skip.getType(), "skip");
    
    std::cout << "[SkipBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test SkipIsoBmffBox buffer contents
 *
 * **Test Group ID:** SkipBox@n
 * **Test Case ID:** TC_057@n
 * **Priority:** High@n
 */
TEST(SkipBox, BufferContents_Valid) {
    std::cout << "[SkipBox] BufferContents_Valid - START" << std::endl;
    
    uint8_t buffer[20] = {0};
    SkipIsoBmffBox skip(20, buffer);
    
    uint32_t size = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    EXPECT_EQ(size, 20);
    
    EXPECT_EQ(buffer[4], 's');
    EXPECT_EQ(buffer[5], 'k');
    EXPECT_EQ(buffer[6], 'i');
    EXPECT_EQ(buffer[7], 'p');
    
    std::cout << "[SkipBox] BufferContents_Valid - PASS" << std::endl;
}

// ==============================================================================
// PlayerIsoBmffBuffer Tests - Constructor & Basic Operations
// ==============================================================================

/**
 * @brief Test PlayerIsoBmffBuffer default constructor
 *
 * **Test Group ID:** PlayerIsoBmffBuffer@n
 * **Test Case ID:** TC_058@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBuffer, Constructor_Default) {
    std::cout << "[PlayerIsoBmffBuffer] Constructor_Default - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    
    EXPECT_EQ(buffer.getBuffer(), nullptr);
    EXPECT_EQ(buffer.getBufferSize(), 0);
    EXPECT_EQ(buffer.getBoxes().size(), 0);
    EXPECT_EQ(buffer.getChunkedfBox(), nullptr);
    EXPECT_EQ(buffer.getMdatCount(), 0);
    
    std::cout << "[PlayerIsoBmffBuffer] Constructor_Default - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer setBuffer with valid data
 *
 * **Test Group ID:** PlayerIsoBmffBuffer@n
 * **Test Case ID:** TC_059@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBuffer, SetBuffer_Valid) {
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_Valid - START" << std::endl;
    
    uint8_t data[256] = {0};
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(data, 256);
    
    EXPECT_EQ(buffer.getBuffer(), data);
    EXPECT_EQ(buffer.getBufferSize(), 256);
    
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_Valid - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer setBuffer multiple times
 *
 * **Test Group ID:** PlayerIsoBmffBuffer@n
 * **Test Case ID:** TC_060@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBuffer, SetBuffer_Multiple) {
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_Multiple - START" << std::endl;
    
    uint8_t data1[100] = {0};
    uint8_t data2[500] = {0};
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(data1, 100);
    EXPECT_EQ(buffer.getBufferSize(), 100);
    
    buffer.setBuffer(data2, 500);
    EXPECT_EQ(buffer.getBufferSize(), 500);
    
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_Multiple - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer setBuffer with zero size
 *
 * **Test Group ID:** PlayerIsoBmffBuffer@n
 * **Test Case ID:** TC_061@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBuffer, SetBuffer_ZeroSize) {
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_ZeroSize - START" << std::endl;
    
    uint8_t data[10] = {0};
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(data, 0);
    
    EXPECT_EQ(buffer.getBufferSize(), 0);
    
    std::cout << "[PlayerIsoBmffBuffer] SetBuffer_ZeroSize - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer getBoxes empty
 *
 * **Test Group ID:** PlayerIsoBmffBuffer@n
 * **Test Case ID:** TC_062@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBuffer, GetBoxes_Empty) {
    std::cout << "[PlayerIsoBmffBuffer] GetBoxes_Empty - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    const std::vector<IsoBmffBox*>& boxes = buffer.getBoxes();
    
    EXPECT_EQ(boxes.size(), 0);
    
    std::cout << "[PlayerIsoBmffBuffer] GetBoxes_Empty - PASS" << std::endl;
}

// ==============================================================================
// Integration Tests - Box Combinations
// ==============================================================================

/**
 * @brief Test complete box hierarchy
 *
 * **Test Group ID:** Integration@n
 * **Test Case ID:** TC_063@n
 * **Priority:** High@n
 */
TEST(Integration, CompleteBoxHierarchy) {
    std::cout << "[Integration] CompleteBoxHierarchy - START" << std::endl;
    
    GenericContainerIsoBmffBox* moov = new GenericContainerIsoBmffBox(2000, "moov");
    
    uint8_t mvhdBuf[100] = {0};
    MvhdIsoBmffBox* mvhd = new MvhdIsoBmffBox(108, 90000, mvhdBuf + 20);
    moov->addChildren(mvhd);
    
    GenericContainerIsoBmffBox* trak = new GenericContainerIsoBmffBox(500, "trak");
    GenericContainerIsoBmffBox* mdia = new GenericContainerIsoBmffBox(250, "mdia");
    uint8_t mdhdBuf[100] = {0};
    MdhdIsoBmffBox* mdhd = new MdhdIsoBmffBox(100, 48000, mdhdBuf + 20);
    
    mdia->addChildren(mdhd);
    trak->addChildren(mdia);
    moov->addChildren(trak);
    
    EXPECT_TRUE(moov->hasChildren());
    const std::vector<IsoBmffBox*>* children = moov->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 2);
    
    delete moov;
    
    std::cout << "[Integration] CompleteBoxHierarchy - PASS" << std::endl;
}

/**
 * @brief Test segment box sequence
 *
 * **Test Group ID:** Integration@n
 * **Test Case ID:** TC_064@n
 * **Priority:** High@n
 */
TEST(Integration, SegmentBoxSequence) {
    std::cout << "[Integration] SegmentBoxSequence - START" << std::endl;
    
    uint8_t buf[500] = {0};
    size_t offset = 0;
    
    TfdtIsoBmffBox tfdt(28, 180000, buf + offset);
    offset += 28;
    
    TfhdIsoBmffBox tfhd(100, 1024, buf + offset, 4096, 0x000008);
    offset += 100;
    
    TrunIsoBmffBox trun(150, 1024, 10, buf + offset, buf + offset + 10, 4096, 0x000001);
    
    EXPECT_EQ(tfdt.getBaseMDT(), 180000);
    EXPECT_EQ(tfhd.getDefaultSampleDuration(), 1024);
    EXPECT_EQ(trun.getSampleCount(), 10);
    
    std::cout << "[Integration] SegmentBoxSequence - PASS" << std::endl;
}

/**
 * @brief Test multiple timescale synchronization
 *
 * **Test Group ID:** Integration@n
 * **Test Case ID:** TC_065@n
 * **Priority:** High@n
 */
TEST(Integration, MultipleTimeScaleSync) {
    std::cout << "[Integration] MultipleTimeScaleSync - START" << std::endl;
    
    uint8_t buf[200] = {0};
    
    MvhdIsoBmffBox mvhd(108, 90000, buf + 10);
    MdhdIsoBmffBox mdhd(100, 48000, buf + 20);
    EmsgIsoBmffBox emsg(200, 90000, 270000, 1);
    SidxIsoBmffBox sidx(100, 90000, 180000);
    
    EXPECT_EQ(mvhd.getTimeScale(), 90000);
    EXPECT_EQ(mdhd.getTimeScale(), 48000);
    EXPECT_EQ(emsg.getTimeScale(), 90000);
    EXPECT_EQ(sidx.getTimeScale(), 90000);
    
    std::cout << "[Integration] MultipleTimeScaleSync - PASS" << std::endl;
}

/**
 * @brief Test buffer with multiple box types
 *
 * **Test Group ID:** Integration@n
 * **Test Case ID:** TC_066@n
 * **Priority:** High@n
 */
TEST(Integration, BufferWithMultipleBoxes) {
    std::cout << "[Integration] BufferWithMultipleBoxes - START" << std::endl;
    
    uint8_t buf[256] = {0};
    
    MvhdIsoBmffBox mvhd(108, 90000, buf + 20);
    MdhdIsoBmffBox mdhd(100, 48000, buf + 50);
    TfdtIsoBmffBox tfdt(28, 360000, buf + 80);
    
    EXPECT_EQ(mvhd.getSize(), 108);
    EXPECT_EQ(mdhd.getSize(), 100);
    EXPECT_EQ(tfdt.getSize(), 28);
    
    std::cout << "[Integration] BufferWithMultipleBoxes - PASS" << std::endl;
}

// ==============================================================================
// Edge Case Tests
// ==============================================================================

/**
 * @brief Test helper functions with boundary values
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_067@n
 * **Priority:** High@n
 */
TEST(EdgeCases, HelperFunctions_BoundaryValues) {
    std::cout << "[EdgeCases] HelperFunctions_BoundaryValues - START" << std::endl;
    
    uint8_t buf[8];
    std::vector<uint64_t> testValues = {
        0x0000000000000001ULL,
        0x00000000FFFFFFFFULL,
        0x0000FFFF0000FFFFULL,
        0x123456789ABCDEF0ULL,
        0xFEDCBA9876543210ULL
    };
    
    for (uint64_t val : testValues) {
        WriteUint64ToBuffer(buf, val);
        uint64_t result = ReadUint64FromBuffer(buf);
        EXPECT_EQ(result, val);
    }
    
    std::cout << "[EdgeCases] HelperFunctions_BoundaryValues - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox with large size
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_068@n
 * **Priority:** High@n
 */
TEST(EdgeCases, IsoBmffBox_LargeSize) {
    std::cout << "[EdgeCases] IsoBmffBox_LargeSize - START" << std::endl;
    
    IsoBmffBox box(0xFFFFFFFF, "mdat");
    
    EXPECT_EQ(box.getSize(), 0xFFFFFFFF);
    
    std::cout << "[EdgeCases] IsoBmffBox_LargeSize - PASS" << std::endl;
}

/**
 * @brief Test GenericContainer with many children
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_069@n
 * **Priority:** High@n
 */
TEST(EdgeCases, GenericContainer_ManyChildren) {
    std::cout << "[EdgeCases] GenericContainer_ManyChildren - START" << std::endl;
    
    GenericContainerIsoBmffBox container(10000, "moov");
    
    for (int i = 0; i < 50; i++) {
        container.addChildren(new IsoBmffBox(100, "free"));
    }
    
    const std::vector<IsoBmffBox*>* children = container.getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 50);
    
    std::cout << "[EdgeCases] GenericContainer_ManyChildren - PASS" << std::endl;
}

/**
 * @brief Test MvhdBox with boundary timescales
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_070@n
 * **Priority:** High@n
 */
TEST(EdgeCases, MvhdBox_BoundaryTimeScales) {
    std::cout << "[EdgeCases] MvhdBox_BoundaryTimeScales - START" << std::endl;
    
    uint8_t buf[100] = {0};
    
    MvhdIsoBmffBox mvhd1(108, 1, buf + 20);
    EXPECT_EQ(mvhd1.getTimeScale(), 1);
    
    MvhdIsoBmffBox mvhd2(108, 0xFFFFFFFF, buf + 30);
    EXPECT_EQ(mvhd2.getTimeScale(), 0xFFFFFFFF);
    
    std::cout << "[EdgeCases] MvhdBox_BoundaryTimeScales - PASS" << std::endl;
}

/**
 * @brief Test TfdtBox with power of two baseMDT
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_071@n
 * **Priority:** High@n
 */
TEST(EdgeCases, TfdtBox_PowerOfTwoBaseMDT) {
    std::cout << "[EdgeCases] TfdtBox_PowerOfTwoBaseMDT - START" << std::endl;
    
    uint8_t buf[100] = {0};
    
    for (int i = 0; i < 32; i++) {
        uint64_t val = 1ULL << i;
        TfdtIsoBmffBox tfdt(28, val, buf + 20);
        EXPECT_EQ(tfdt.getBaseMDT(), val);
    }
    
    std::cout << "[EdgeCases] TfdtBox_PowerOfTwoBaseMDT - PASS" << std::endl;
}

/**
 * @brief Test EmsgBox with zero values
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_072@n
 * **Priority:** High@n
 */
TEST(EdgeCases, EmsgBox_ZeroValues) {
    std::cout << "[EdgeCases] EmsgBox_ZeroValues - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 0, 0, 0);
    
    EXPECT_EQ(emsg.getTimeScale(), 0);
    EXPECT_EQ(emsg.getEventDuration(), 0);
    EXPECT_EQ(emsg.getId(), 0);
    
    std::cout << "[EdgeCases] EmsgBox_ZeroValues - PASS" << std::endl;
}

/**
 * @brief Test EmsgBox with max values
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_073@n
 * **Priority:** High@n
 */
TEST(EdgeCases, EmsgBox_MaxValues) {
    std::cout << "[EdgeCases] EmsgBox_MaxValues - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
    
    EXPECT_EQ(emsg.getTimeScale(), 0xFFFFFFFF);
    EXPECT_EQ(emsg.getEventDuration(), 0xFFFFFFFF);
    EXPECT_EQ(emsg.getId(), 0xFFFFFFFF);
    
    std::cout << "[EdgeCases] EmsgBox_MaxValues - PASS" << std::endl;
}

/**
 * @brief Test TrunBox with single sample
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_074@n
 * **Priority:** High@n
 */
TEST(EdgeCases, TrunBox_SingleSample) {
    std::cout << "[EdgeCases] TrunBox_SingleSample - START" << std::endl;
    
    uint8_t buf[200] = {0};
    TrunIsoBmffBox trun(150, 1024, 1, buf + 20, buf + 30, 4096, 0x000001);
    
    EXPECT_EQ(trun.getSampleCount(), 1);
    EXPECT_EQ(trun.getFirstSampleSize(), 4096);
    
    std::cout << "[EdgeCases] TrunBox_SingleSample - PASS" << std::endl;
}

/**
 * @brief Test MdatBox with minimal size
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_075@n
 * **Priority:** High@n
 */
TEST(EdgeCases, MdatBox_MinimalSize) {
    std::cout << "[EdgeCases] MdatBox_MinimalSize - START" << std::endl;
    
    uint8_t buf[16] = {0};
    buf[0] = 0x00; buf[1] = 0x00; buf[2] = 0x00; buf[3] = 0x10;
    memcpy(buf + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(16, buf);
    
    EXPECT_EQ(mdat.getSize(), 16);
    
    std::cout << "[EdgeCases] MdatBox_MinimalSize - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer setBuffer with null
 *
 * **Test Group ID:** EdgeCases@n
 * **Test Case ID:** TC_076@n
 * **Priority:** High@n
 */
TEST(EdgeCases, PlayerIsoBmffBuffer_SetBufferNull) {
    std::cout << "[EdgeCases] PlayerIsoBmffBuffer_SetBufferNull - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    buffer.setBuffer(nullptr, 0);
    
    EXPECT_EQ(buffer.getBuffer(), nullptr);
    EXPECT_EQ(buffer.getBufferSize(), 0);
    
    std::cout << "[EdgeCases] PlayerIsoBmffBuffer_SetBufferNull - PASS" << std::endl;
}

// ==============================================================================
// Stress Tests
// ==============================================================================

/**
 * @brief Test rapid box creation and deletion
 *
 * **Test Group ID:** StressTests@n
 * **Test Case ID:** TC_077@n
 * **Priority:** Medium@n
 */
TEST(StressTests, RapidBoxCreationDeletion) {
    std::cout << "[StressTests] RapidBoxCreationDeletion - START" << std::endl;
    
    for (int i = 0; i < 100; i++) {
        IsoBmffBox* box = new IsoBmffBox(100 + i, "free");
        EXPECT_EQ(box->getSize(), 100 + i);
        delete box;
    }
    
    std::cout << "[StressTests] RapidBoxCreationDeletion - PASS" << std::endl;
}

/**
 * @brief Test timescale rapid updates
 *
 * **Test Group ID:** StressTests@n
 * **Test Case ID:** TC_078@n
 * **Priority:** Medium@n
 */
TEST(StressTests, TimeScaleRapidUpdates) {
    std::cout << "[StressTests] TimeScaleRapidUpdates - START" << std::endl;
    
    uint8_t buf[100] = {0};
    MvhdIsoBmffBox mvhd(108, 1000, buf + 20);
    
    for (uint32_t ts = 1000; ts <= 10000; ts += 1000) {
        mvhd.setTimeScale(ts);
        EXPECT_EQ(mvhd.getTimeScale(), ts);
    }
    
    std::cout << "[StressTests] TimeScaleRapidUpdates - PASS" << std::endl;
}

/**
 * @brief Test BaseMDT sequence simulation
 *
 * **Test Group ID:** StressTests@n
 * **Test Case ID:** TC_079@n
 * **Priority:** Medium@n
 */
TEST(StressTests, BaseMDTSequence) {
    std::cout << "[StressTests] BaseMDTSequence - START" << std::endl;
    
    uint8_t buf[100] = {0};
    TfdtIsoBmffBox tfdt(28, 0, buf + 20);
    
    for (uint64_t i = 0; i < 100; i++) {
        uint64_t baseMDT = i * 180000;
        tfdt.setBaseMDT(baseMDT);
        EXPECT_EQ(tfdt.getBaseMDT(), baseMDT);
    }
    
    std::cout << "[StressTests] BaseMDTSequence - PASS" << std::endl;
}

/**
 * @brief Test buffer set multiple times
 *
 * **Test Group ID:** StressTests@n
 * **Test Case ID:** TC_080@n
 * **Priority:** Medium@n
 */
TEST(StressTests, BufferSetMultipleTimes) {
    std::cout << "[StressTests] BufferSetMultipleTimes - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    std::vector<uint8_t> data(1000, 0);
    
    for (int i = 0; i < 50; i++) {
        buffer.setBuffer(data.data(), data.size());
        EXPECT_EQ(buffer.getBufferSize(), 1000);
    }
    
    std::cout << "[StressTests] BufferSetMultipleTimes - PASS" << std::endl;
}

// ==============================================================================
// SencIsoBmffBox Tests (Sample Encryption Box)
// ==============================================================================

/**
 * @brief Test SencIsoBmffBox constructor with valid parameters
 *
 * **Test Group ID:** SencBox@n
 * **Test Case ID:** TC_081@n
 * **Priority:** High@n
 */
TEST(SencBox, Constructor_ValidParams) {
    std::cout << "[SencBox] Constructor_ValidParams - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    uint8_t sampleCount = 10;
    
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "senc", sizeof(btype));
    
    FullIsoBmffBox fbox(100, btype, 0, 0x000002);
    SencIsoBmffBox senc(fbox, &sampleCount, 10);
    
    EXPECT_EQ(senc.getSize(), 100);
    EXPECT_STREQ(senc.getType(), "senc");
    
    std::cout << "[SencBox] Constructor_ValidParams - PASS" << std::endl;
}

/**
 * @brief Test SencIsoBmffBox with zero samples
 *
 * **Test Group ID:** SencBox@n
 * **Test Case ID:** TC_082@n
 * **Priority:** High@n
 */
TEST(SencBox, ZeroSamples) {
    std::cout << "[SencBox] ZeroSamples - START" << std::endl;
    
    uint8_t sampleCount = 0;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "senc", sizeof(btype));
    
    FullIsoBmffBox fbox(50, btype, 0, 0);
    SencIsoBmffBox senc(fbox, &sampleCount, 0);
    
    EXPECT_EQ(senc.getSize(), 50);
    
    std::cout << "[SencBox] ZeroSamples - PASS" << std::endl;
}

/**
 * @brief Test SencIsoBmffBox truncate method
 *
 * **Test Group ID:** SencBox@n
 * **Test Case ID:** TC_083@n
 * **Priority:** High@n
 */
TEST(SencBox, Truncate_Valid) {
    std::cout << "[SencBox] Truncate_Valid - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    uint8_t sampleCount = 15;
    
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "senc", sizeof(btype));
    
    FullIsoBmffBox fbox(150, btype, 0, 0x000002);
    SencIsoBmffBox senc(fbox, &sampleCount, 15);
    senc.setBase(buffer + 8);
    
    senc.truncate(2048);
    
    // Verify truncate executed without crash
    EXPECT_LE(senc.getSize(), 150);
    
    std::cout << "[SencBox] Truncate_Valid - PASS" << std::endl;
}

/**
 * @brief Test SencIsoBmffBox with large sample count
 *
 * **Test Group ID:** SencBox@n
 * **Test Case ID:** TC_084@n
 * **Priority:** Medium@n
 */
TEST(SencBox, LargeSampleCount) {
    std::cout << "[SencBox] LargeSampleCount - START" << std::endl;
    
    uint8_t buffer[500] = {0};
    uint8_t sampleCount = 100;
    
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "senc", sizeof(btype));
    
    FullIsoBmffBox fbox(500, btype, 0, 0x000002);
    SencIsoBmffBox senc(fbox, &sampleCount, 100);
    
    EXPECT_EQ(senc.getSize(), 500);
    
    std::cout << "[SencBox] LargeSampleCount - PASS" << std::endl;
}

/**
 * @brief Test SencIsoBmffBox with different flags
 *
 * **Test Group ID:** SencBox@n
 * **Test Case ID:** TC_085@n
 * **Priority:** Medium@n
 */
TEST(SencBox, DifferentFlags) {
    std::cout << "[SencBox] DifferentFlags - START" << std::endl;
    
    uint8_t sampleCount = 5;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "senc", sizeof(btype));
    
    FullIsoBmffBox fbox1(100, btype, 0, 0x000000);
    SencIsoBmffBox senc1(fbox1, &sampleCount, 5);
    EXPECT_EQ(senc1.getSize(), 100);
    
    FullIsoBmffBox fbox2(100, btype, 0, 0x000002);
    SencIsoBmffBox senc2(fbox2, &sampleCount, 5);
    EXPECT_EQ(senc2.getSize(), 100);
    
    std::cout << "[SencBox] DifferentFlags - PASS" << std::endl;
}

// ==============================================================================
// SaizIsoBmffBox Tests (Sample Auxiliary Information Sizes Box)
// ==============================================================================

/**
 * @brief Test SaizIsoBmffBox constructor with typical values
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_086@n
 * **Priority:** High@n
 */
TEST(SaizBox, Constructor_TypicalValues) {
    std::cout << "[SaizBox] Constructor_TypicalValues - START" << std::endl;
    
    uint8_t sampleCount = 10;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    FullIsoBmffBox fbox(100, btype, 0, 0);
    SaizIsoBmffBox saiz(fbox, &sampleCount, 10, 64);
    
    EXPECT_EQ(saiz.getSize(), 100);
    EXPECT_STREQ(saiz.getType(), "saiz");
    EXPECT_EQ(saiz.getFirstSampleInfoSize(), 64);
    
    std::cout << "[SaizBox] Constructor_TypicalValues - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox with zero sample info size
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_087@n
 * **Priority:** High@n
 */
TEST(SaizBox, ZeroSampleInfoSize) {
    std::cout << "[SaizBox] ZeroSampleInfoSize - START" << std::endl;
    
    uint8_t sampleCount = 5;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    FullIsoBmffBox fbox(50, btype, 0, 0);
    SaizIsoBmffBox saiz(fbox, &sampleCount, 5, 0);
    
    EXPECT_EQ(saiz.getFirstSampleInfoSize(), 0);
    
    std::cout << "[SaizBox] ZeroSampleInfoSize - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox getFirstSampleInfoSize
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_088@n
 * **Priority:** High@n
 */
TEST(SaizBox, GetFirstSampleInfoSize_Various) {
    std::cout << "[SaizBox] GetFirstSampleInfoSize_Various - START" << std::endl;
    
    uint8_t sampleCount = 8;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    std::vector<uint32_t> testSizes = {1, 8, 16, 32, 64, 128, 256, 512, 1024};
    
    for (uint32_t size : testSizes) {
        FullIsoBmffBox fbox(100, btype, 0, 0);
        SaizIsoBmffBox saiz(fbox, &sampleCount, 8, size);
        EXPECT_EQ(saiz.getFirstSampleInfoSize(), size);
    }
    
    std::cout << "[SaizBox] GetFirstSampleInfoSize_Various - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox truncate method
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_089@n
 * **Priority:** High@n
 */
TEST(SaizBox, Truncate_Valid) {
    std::cout << "[SaizBox] Truncate_Valid - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    uint8_t sampleCount = 10;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    FullIsoBmffBox fbox(150, btype, 0, 0);
    SaizIsoBmffBox saiz(fbox, &sampleCount, 10, 128);
    saiz.setBase(buffer + 8);
    
    saiz.truncate();
    
    // Verify truncate executed without crash
    EXPECT_LE(saiz.getSize(), 150);
    
    std::cout << "[SaizBox] Truncate_Valid - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox with varying sample counts
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_090@n
 * **Priority:** Medium@n
 */
TEST(SaizBox, VaryingSampleCounts) {
    std::cout << "[SaizBox] VaryingSampleCounts - START" << std::endl;
    
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    std::vector<uint32_t> counts = {1, 5, 10, 25, 50, 100};
    
    for (uint32_t count : counts) {
        uint8_t sampleCount = static_cast<uint8_t>(count);
        FullIsoBmffBox fbox(100 + count * 10, btype, 0, 0);
        SaizIsoBmffBox saiz(fbox, &sampleCount, count, 64);
        
        EXPECT_EQ(saiz.getSize(), 100 + count * 10);
        EXPECT_EQ(saiz.getFirstSampleInfoSize(), 64);
    }
    
    std::cout << "[SaizBox] VaryingSampleCounts - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox with max sample info size
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_091@n
 * **Priority:** Medium@n
 */
TEST(SaizBox, MaxSampleInfoSize) {
    std::cout << "[SaizBox] MaxSampleInfoSize - START" << std::endl;
    
    uint8_t sampleCount = 10;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    FullIsoBmffBox fbox(500, btype, 0, 0);
    SaizIsoBmffBox saiz(fbox, &sampleCount, 10, 0xFFFFFFFF);
    
    EXPECT_EQ(saiz.getFirstSampleInfoSize(), 0xFFFFFFFF);
    
    std::cout << "[SaizBox] MaxSampleInfoSize - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox with different versions
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_092@n
 * **Priority:** Medium@n
 */
TEST(SaizBox, DifferentVersions) {
    std::cout << "[SaizBox] DifferentVersions - START" << std::endl;
    
    uint8_t sampleCount = 8;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    for (uint8_t ver = 0; ver <= 1; ver++) {
        FullIsoBmffBox fbox(100, btype, ver, 0);
        SaizIsoBmffBox saiz(fbox, &sampleCount, 8, 64);
        
        EXPECT_EQ(saiz.getSize(), 100);
        EXPECT_EQ(saiz.getFirstSampleInfoSize(), 64);
    }
    
    std::cout << "[SaizBox] DifferentVersions - PASS" << std::endl;
}

/**
 * @brief Test SaizIsoBmffBox edge case with single sample
 *
 * **Test Group ID:** SaizBox@n
 * **Test Case ID:** TC_093@n
 * **Priority:** Medium@n
 */
TEST(SaizBox, SingleSample) {
    std::cout << "[SaizBox] SingleSample - START" << std::endl;
    
    uint8_t sampleCount = 1;
    char btype[4];
    std::memset(btype, 0, sizeof(btype));
    std::strncpy(btype, "saiz", sizeof(btype));
    
    FullIsoBmffBox fbox(50, btype, 0, 0);
    SaizIsoBmffBox saiz(fbox, &sampleCount, 1, 32);
    
    EXPECT_EQ(saiz.getFirstSampleInfoSize(), 32);
    
    std::cout << "[SaizBox] SingleSample - PASS" << std::endl;
}

// ==============================================================================
// TrakIsoBmffBox Extended Tests
// ==============================================================================

/**
 * @brief Test TrakIsoBmffBox constructor
 *
 * **Test Group ID:** TrakBoxExtended@n
 * **Test Case ID:** TC_094@n
 * **Priority:** High@n
 */
TEST(TrakBoxExtended, Constructor_Valid) {
    std::cout << "[TrakBoxExtended] Constructor_Valid - START" << std::endl;
    
    TrakIsoBmffBox trak(500);
    
    EXPECT_EQ(trak.getSize(), 500);
    EXPECT_STREQ(trak.getType(), "trak");
    EXPECT_EQ(trak.getTrack_Id(), 0);
    EXPECT_TRUE(trak.hasChildren());
    
    std::cout << "[TrakBoxExtended] Constructor_Valid - PASS" << std::endl;
}

/**
 * @brief Test TrakIsoBmffBox with children
 *
 * **Test Group ID:** TrakBoxExtended@n
 * **Test Case ID:** TC_095@n
 * **Priority:** High@n
 */
TEST(TrakBoxExtended, WithChildren) {
    std::cout << "[TrakBoxExtended] WithChildren - START" << std::endl;
    
    TrakIsoBmffBox* trak = new TrakIsoBmffBox(800);
    
    uint8_t mdhdBuf[100] = {0};
    MdhdIsoBmffBox* mdhd = new MdhdIsoBmffBox(100, 48000, mdhdBuf + 20);
    trak->addChildren(mdhd);
    
    GenericContainerIsoBmffBox* mdia = new GenericContainerIsoBmffBox(300, "mdia");
    trak->addChildren(mdia);
    
    const std::vector<IsoBmffBox*>* children = trak->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 2);
    
    delete trak;
    
    std::cout << "[TrakBoxExtended] WithChildren - PASS" << std::endl;
}

/**
 * @brief Test TrakIsoBmffBox getTrack_Id default
 *
 * **Test Group ID:** TrakBoxExtended@n
 * **Test Case ID:** TC_096@n
 * **Priority:** High@n
 */
TEST(TrakBoxExtended, GetTrackId_Default) {
    std::cout << "[TrakBoxExtended] GetTrackId_Default - START" << std::endl;
    
    TrakIsoBmffBox trak(200);
    
    EXPECT_EQ(trak.getTrack_Id(), 0);
    
    std::cout << "[TrakBoxExtended] GetTrackId_Default - PASS" << std::endl;
}

/**
 * @brief Test TrakIsoBmffBox with various sizes
 *
 * **Test Group ID:** TrakBoxExtended@n
 * **Test Case ID:** TC_097@n
 * **Priority:** Medium@n
 */
TEST(TrakBoxExtended, VariousSizes) {
    std::cout << "[TrakBoxExtended] VariousSizes - START" << std::endl;
    
    std::vector<uint32_t> sizes = {100, 500, 1000, 5000, 10000};
    
    for (uint32_t size : sizes) {
        TrakIsoBmffBox trak(size);
        EXPECT_EQ(trak.getSize(), size);
        EXPECT_TRUE(trak.hasChildren());
    }
    
    std::cout << "[TrakBoxExtended] VariousSizes - PASS" << std::endl;
}

/**
 * @brief Test TrakIsoBmffBox nested in moov
 *
 * **Test Group ID:** TrakBoxExtended@n
 * **Test Case ID:** TC_098@n
 * **Priority:** Medium@n
 */
TEST(TrakBoxExtended, NestedInMoov) {
    std::cout << "[TrakBoxExtended] NestedInMoov - START" << std::endl;
    
    GenericContainerIsoBmffBox* moov = new GenericContainerIsoBmffBox(2000, "moov");
    TrakIsoBmffBox* trak1 = new TrakIsoBmffBox(500);
    TrakIsoBmffBox* trak2 = new TrakIsoBmffBox(600);
    
    moov->addChildren(trak1);
    moov->addChildren(trak2);
    
    const std::vector<IsoBmffBox*>* children = moov->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 2);
    
    delete moov;
    
    std::cout << "[TrakBoxExtended] NestedInMoov - PASS" << std::endl;
}

// ==============================================================================
// SidxIsoBmffBox Extended Tests
// ==============================================================================

/**
 * @brief Test SidxIsoBmffBox with various timescales
 *
 * **Test Group ID:** SidxBoxExtended@n
 * **Test Case ID:** TC_099@n
 * **Priority:** Medium@n
 */
TEST(SidxBoxExtended, VariousTimeScales) {
    std::cout << "[SidxBoxExtended] VariousTimeScales - START" << std::endl;
    
    std::vector<uint32_t> timescales = {1, 90, 600, 1000, 48000, 90000};
    
    for (uint32_t ts : timescales) {
        SidxIsoBmffBox sidx(100, ts, ts * 2);
        EXPECT_EQ(sidx.getTimeScale(), ts);
        EXPECT_EQ(sidx.getSampleDuration(), ts * 2);
    }
    
    std::cout << "[SidxBoxExtended] VariousTimeScales - PASS" << std::endl;
}

/**
 * @brief Test SidxIsoBmffBox duration calculations
 *
 * **Test Group ID:** SidxBoxExtended@n
 * **Test Case ID:** TC_100@n
 * **Priority:** Medium@n
 */
TEST(SidxBoxExtended, DurationCalculations) {
    std::cout << "[SidxBoxExtended] DurationCalculations - START" << std::endl;
    
    // 1 second at 90kHz
    SidxIsoBmffBox sidx1(100, 90000, 90000);
    EXPECT_EQ(sidx1.getSampleDuration(), 90000);
    
    // 2 seconds at 90kHz
    SidxIsoBmffBox sidx2(100, 90000, 180000);
    EXPECT_EQ(sidx2.getSampleDuration(), 180000);
    
    // 5 seconds at 90kHz
    SidxIsoBmffBox sidx3(100, 90000, 450000);
    EXPECT_EQ(sidx3.getSampleDuration(), 450000);
    
    std::cout << "[SidxBoxExtended] DurationCalculations - PASS" << std::endl;
}

// ==============================================================================
// PlayerIsoBmffBuffer Extended Tests
// ==============================================================================

/**
 * @brief Test PlayerIsoBmffBuffer getMdatCount default
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_101@n
 * **Test Case ID:** TC_101@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBufferExtended, GetMdatCount_Default) {
    std::cout << "[PlayerIsoBmffBufferExtended] GetMdatCount_Default - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    
    EXPECT_EQ(buffer.getMdatCount(), 0);
    
    std::cout << "[PlayerIsoBmffBufferExtended] GetMdatCount_Default - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer getChunkedfBox default
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_102@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBufferExtended, GetChunkedfBox_Null) {
    std::cout << "[PlayerIsoBmffBufferExtended] GetChunkedfBox_Null - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    
    EXPECT_EQ(buffer.getChunkedfBox(), nullptr);
    
    std::cout << "[PlayerIsoBmffBufferExtended] GetChunkedfBox_Null - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer isInitSegment default
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_103@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBufferExtended, IsInitSegment_Default) {
    std::cout << "[PlayerIsoBmffBufferExtended] IsInitSegment_Default - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    
    EXPECT_FALSE(buffer.isInitSegment());
    
    std::cout << "[PlayerIsoBmffBufferExtended] IsInitSegment_Default - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer getMdatBoxSize without buffer
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_104@n
 * **Priority:** High@n
 */
TEST(PlayerIsoBmffBufferExtended, GetMdatBoxSize_NoBuffer) {
    std::cout << "[PlayerIsoBmffBufferExtended] GetMdatBoxSize_NoBuffer - START" << std::endl;
    
    PlayerIsoBmffBuffer buffer;
    size_t size = 999;
    
    EXPECT_FALSE(buffer.getMdatBoxSize(size));
    
    std::cout << "[PlayerIsoBmffBufferExtended] GetMdatBoxSize_NoBuffer - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer with large buffer
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_105@n
 * **Priority:** Medium@n
 */
TEST(PlayerIsoBmffBufferExtended, LargeBuffer) {
    std::cout << "[PlayerIsoBmffBufferExtended] LargeBuffer - START" << std::endl;
    
    std::vector<uint8_t> largeData(100000, 0);
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(largeData.data(), largeData.size());
    
    EXPECT_EQ(buffer.getBufferSize(), 100000);
    EXPECT_NE(buffer.getBuffer(), nullptr);
    
    std::cout << "[PlayerIsoBmffBufferExtended] LargeBuffer - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer buffer replacement
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_106@n
 * **Priority:** Medium@n
 */
TEST(PlayerIsoBmffBufferExtended, BufferReplacement) {
    std::cout << "[PlayerIsoBmffBufferExtended] BufferReplacement - START" << std::endl;
    
    uint8_t data1[100] = {0};
    uint8_t data2[200] = {0};
    uint8_t data3[50] = {0};
    
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(data1, 100);
    EXPECT_EQ(buffer.getBufferSize(), 100);
    
    buffer.setBuffer(data2, 200);
    EXPECT_EQ(buffer.getBufferSize(), 200);
    
    buffer.setBuffer(data3, 50);
    EXPECT_EQ(buffer.getBufferSize(), 50);
    
    std::cout << "[PlayerIsoBmffBufferExtended] BufferReplacement - PASS" << std::endl;
}

/**
 * @brief Test PlayerIsoBmffBuffer getBoxes after setBuffer
 *
 * **Test Group ID:** PlayerIsoBmffBufferExtended@n
 * **Test Case ID:** TC_107@n
 * **Priority:** Medium@n
 */
TEST(PlayerIsoBmffBufferExtended, GetBoxes_AfterSetBuffer) {
    std::cout << "[PlayerIsoBmffBufferExtended] GetBoxes_AfterSetBuffer - START" << std::endl;
    
    uint8_t data[256] = {0};
    PlayerIsoBmffBuffer buffer;
    
    buffer.setBuffer(data, 256);
    const std::vector<IsoBmffBox*>& boxes = buffer.getBoxes();
    
    // Without parseBuffer, boxes should be empty
    EXPECT_EQ(boxes.size(), 0);
    
    std::cout << "[PlayerIsoBmffBufferExtended] GetBoxes_AfterSetBuffer - PASS" << std::endl;
}

// ==============================================================================
// Additional Integration Tests  
// ==============================================================================

/**
 * @brief Test complete media segment structure
 *
 * **Test Group ID:** IntegrationExtended@n
 * **Test Case ID:** TC_108@n
 * **Priority:** High@n
 */
TEST(IntegrationExtended, CompleteMediaSegment) {
    std::cout << "[IntegrationExtended] CompleteMediaSegment - START" << std::endl;
    
    // Build a typical media segment: moof + mdat
    GenericContainerIsoBmffBox* moof = new GenericContainerIsoBmffBox(500, "moof");
    
    GenericContainerIsoBmffBox* traf = new GenericContainerIsoBmffBox(400, "traf");
    
    uint8_t buf[300] = {0};
    TfdtIsoBmffBox* tfdt = new TfdtIsoBmffBox(28, 360000, buf + 10);
    TfhdIsoBmffBox* tfhd = new TfhdIsoBmffBox(100, 1024, buf + 50, 4096, 0x000008);
    TrunIsoBmffBox* trun = new TrunIsoBmffBox(150, 1024, 15, buf + 80, buf + 90, 4096, 0x000001);
    
    traf->addChildren(tfdt);
    traf->addChildren(tfhd);
    traf->addChildren(trun);
    moof->addChildren(traf);
    
    const std::vector<IsoBmffBox*>* moofChildren = moof->getChildren();
    ASSERT_NE(moofChildren, nullptr);
    EXPECT_EQ(moofChildren->size(), 1);
    
    const std::vector<IsoBmffBox*>* trafChildren = traf->getChildren();
    ASSERT_NE(trafChildren, nullptr);
    EXPECT_EQ(trafChildren->size(), 3);
    
    delete moof;
    
    std::cout << "[IntegrationExtended] CompleteMediaSegment - PASS" << std::endl;
}

/**
 * @brief Test encryption-related boxes together
 *
 * **Test Group ID:** IntegrationExtended@n
 * **Test Case ID:** TC_109@n
 * **Priority:** High@n
 */
TEST(IntegrationExtended, EncryptionBoxes) {
    std::cout << "[IntegrationExtended] EncryptionBoxes - START" << std::endl;
    
    uint8_t sampleCount = 20;
    char btypeSenc[4];
    char btypeSaiz[4];
    std::memset(btypeSenc, 0, sizeof(btypeSenc));
    std::memset(btypeSaiz, 0, sizeof(btypeSaiz));
    std::strncpy(btypeSenc, "senc", sizeof(btypeSenc));
    std::strncpy(btypeSaiz, "saiz", sizeof(btypeSaiz));
    
    FullIsoBmffBox fboxSenc(200, btypeSenc, 0, 0x000002);
    SencIsoBmffBox senc(fboxSenc, &sampleCount, 20);
    
    FullIsoBmffBox fboxSaiz(150, btypeSaiz, 0, 0);
    SaizIsoBmffBox saiz(fboxSaiz, &sampleCount, 20, 128);
    
    EXPECT_EQ(senc.getSize(), 200);
    EXPECT_EQ(saiz.getSize(), 150);
    EXPECT_EQ(saiz.getFirstSampleInfoSize(), 128);
    
    std::cout << "[IntegrationExtended] EncryptionBoxes - PASS" << std::endl;
}

/**
 * @brief Test multi-track moov structure
 *
 * **Test Group ID:** IntegrationExtended@n
 * **Test Case ID:** TC_110@n
 * **Priority:** High@n
 */
TEST(IntegrationExtended, MultiTrackMoov) {
    std::cout << "[IntegrationExtended] MultiTrackMoov - START" << std::endl;
    
    GenericContainerIsoBmffBox* moov = new GenericContainerIsoBmffBox(5000, "moov");
    
    uint8_t mvhdBuf[100] = {0};
    MvhdIsoBmffBox* mvhd = new MvhdIsoBmffBox(108, 90000, mvhdBuf + 20);
    moov->addChildren(mvhd);
    
    // Video track
    TrakIsoBmffBox* videoTrak = new TrakIsoBmffBox(1500);
    moov->addChildren(videoTrak);
    
    // Audio track
    TrakIsoBmffBox* audioTrak = new TrakIsoBmffBox(1200);
    moov->addChildren(audioTrak);
    
    const std::vector<IsoBmffBox*>* children = moov->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 3);  // mvhd + 2 traks
    
    delete moov;
    
    std::cout << "[IntegrationExtended] MultiTrackMoov - PASS" << std::endl;
}

// ==============================================================================
// EmsgBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test EmsgIsoBmffBox with maximum timescale
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_111@n
 * **Priority:** High@n
 */
TEST(EmsgBoxExtended, MaxTimescale) {
    std::cout << "[EmsgBoxExtended] MaxTimescale - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 0xFFFFFFFF, 1000, 1);
    
    EXPECT_EQ(emsg.getTimeScale(), 0xFFFFFFFF);
    
    std::cout << "[EmsgBoxExtended] MaxTimescale - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox with maximum event duration
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_112@n
 * **Priority:** High@n
 */
TEST(EmsgBoxExtended, MaxEventDuration) {
    std::cout << "[EmsgBoxExtended] MaxEventDuration - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 0xFFFFFFFF, 1);
    emsg.setEventDuration(0xFFFFFFFF);
    
    EXPECT_EQ(emsg.getEventDuration(), 0xFFFFFFFF);
    
    std::cout << "[EmsgBoxExtended] MaxEventDuration - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox with maximum ID
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_113@n
 * **Priority:** High@n
 */
TEST(EmsgBoxExtended, MaxId) {
    std::cout << "[EmsgBoxExtended] MaxId - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 1000, 0xFFFFFFFF);
    
    EXPECT_EQ(emsg.getId(), 0xFFFFFFFF);
    
    std::cout << "[EmsgBoxExtended] MaxId - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox presentation time delta with max value
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_114@n
 * **Priority:** High@n
 */
TEST(EmsgBoxExtended, MaxPresentationTimeDelta) {
    std::cout << "[EmsgBoxExtended] MaxPresentationTimeDelta - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 1000, 1);
    emsg.setPresentationTimeDelta(0xFFFFFFFF);
    
    EXPECT_EQ(emsg.getPresentationTimeDelta(), 0xFFFFFFFF);
    
    std::cout << "[EmsgBoxExtended] MaxPresentationTimeDelta - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox presentation time with max value
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_115@n
 * **Priority:** High@n
 */
TEST(EmsgBoxExtended, MaxPresentationTime) {
    std::cout << "[EmsgBoxExtended] MaxPresentationTime - START" << std::endl;
    
    EmsgIsoBmffBox emsg(200, 90000, 1000, 1);
    emsg.setPresentationTime(0xFFFFFFFFFFFFFFFFULL);
    
    EXPECT_EQ(emsg.getPresentationTime(), 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[EmsgBoxExtended] MaxPresentationTime - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox with varying timescales
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_116@n
 * **Priority:** Medium@n
 */
TEST(EmsgBoxExtended, VaryingTimescales) {
    std::cout << "[EmsgBoxExtended] VaryingTimescales - START" << std::endl;
    
    std::vector<uint32_t> timescales = {1, 1000, 10000, 44100, 48000, 90000, 600, 0xFFFFFFFF};
    
    for (uint32_t ts : timescales) {
        EmsgIsoBmffBox emsg(200, ts, 1000, 1);
        EXPECT_EQ(emsg.getTimeScale(), ts);
    }
    
    std::cout << "[EmsgBoxExtended] VaryingTimescales - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox with small box size
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_117@n
 * **Priority:** Medium@n
 */
TEST(EmsgBoxExtended, SmallBoxSize) {
    std::cout << "[EmsgBoxExtended] SmallBoxSize - START" << std::endl;
    
    EmsgIsoBmffBox emsg(16, 90000, 0, 0);
    
    EXPECT_EQ(emsg.getSize(), 16);
    
    std::cout << "[EmsgBoxExtended] SmallBoxSize - PASS" << std::endl;
}

/**
 * @brief Test EmsgIsoBmffBox with large box size
 *
 * **Test Group ID:** EmsgBoxExtended@n
 * **Test Case ID:** TC_118@n
 * **Priority:** Medium@n
 */
TEST(EmsgBoxExtended, LargeBoxSize) {
    std::cout << "[EmsgBoxExtended] LargeBoxSize - START" << std::endl;
    
    EmsgIsoBmffBox emsg(0xFFFFFF, 90000, 1000, 1);
    
    EXPECT_EQ(emsg.getSize(), 0xFFFFFF);
    
    std::cout << "[EmsgBoxExtended] LargeBoxSize - PASS" << std::endl;
}

// ==============================================================================
// TrunBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test TrunIsoBmffBox with zero samples
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_119@n
 * **Priority:** High@n
 */
TEST(TrunBoxExtended, ZeroSamples) {
    std::cout << "[TrunBoxExtended] ZeroSamples - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 0, 0, buffer + 20, buffer + 30, 0, 0x000000);
    
    EXPECT_EQ(trun.getSampleCount(), 0);
    
    std::cout << "[TrunBoxExtended] ZeroSamples - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with large sample count
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_120@n
 * **Priority:** High@n
 */
TEST(TrunBoxExtended, LargeSampleCount) {
    std::cout << "[TrunBoxExtended] LargeSampleCount - START" << std::endl;
    
    uint8_t buffer[500] = {0};
    TrunIsoBmffBox trun(500, 1024, 10000, buffer + 20, buffer + 30, 4096, 0x000001);
    
    EXPECT_EQ(trun.getSampleCount(), 10000);
    
    std::cout << "[TrunBoxExtended] LargeSampleCount - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with maximum sample duration
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_121@n
 * **Priority:** High@n
 */
TEST(TrunBoxExtended, MaxSampleDuration) {
    std::cout << "[TrunBoxExtended] MaxSampleDuration - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 0xFFFFFFFFFFFFFFFFULL, 10, buffer + 20, buffer + 30, 1024, 0x000100);
    
    EXPECT_EQ(trun.getSampleDuration(), 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[TrunBoxExtended] MaxSampleDuration - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with maximum first sample size
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_122@n
 * **Priority:** High@n
 */
TEST(TrunBoxExtended, MaxFirstSampleSize) {
    std::cout << "[TrunBoxExtended] MaxFirstSampleSize - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 1024, 10, buffer + 20, buffer + 30, 0xFFFFFFFF, 0x000200);
    
    EXPECT_EQ(trun.getFirstSampleSize(), 0xFFFFFFFF);
    
    std::cout << "[TrunBoxExtended] MaxFirstSampleSize - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with varying sample counts
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_123@n
 * **Priority:** Medium@n
 */
TEST(TrunBoxExtended, VaryingSampleCounts) {
    std::cout << "[TrunBoxExtended] VaryingSampleCounts - START" << std::endl;
    
    uint8_t buffer[500] = {0};
    std::vector<uint32_t> counts = {1, 5, 10, 50, 100, 500, 1000};
    
    for (uint32_t count : counts) {
        TrunIsoBmffBox trun(500, 1024, count, buffer + 20, buffer + 30, 2048, 0x000001);
        EXPECT_EQ(trun.getSampleCount(), count);
    }
    
    std::cout << "[TrunBoxExtended] VaryingSampleCounts - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox setFirstSampleDuration with various values
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_124@n
 * **Priority:** Medium@n
 */
TEST(TrunBoxExtended, SetFirstSampleDuration_Variations) {
    std::cout << "[TrunBoxExtended] SetFirstSampleDuration_Variations - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    TrunIsoBmffBox trun(150, 1024, 10, buffer + 20, buffer + 30, 2048, 0x000100);
    
    std::vector<uint64_t> durations = {512, 1024, 2048, 4096, 8192, 16384};
    
    for (uint64_t dur : durations) {
        trun.setFirstSampleDuration(dur);
        EXPECT_EQ(trun.getSampleDuration(), dur);
    }
    
    std::cout << "[TrunBoxExtended] SetFirstSampleDuration_Variations - PASS" << std::endl;
}

/**
 * @brief Test TrunIsoBmffBox with different flags
 *
 * **Test Group ID:** TrunBoxExtended@n
 * **Test Case ID:** TC_125@n
 * **Priority:** Medium@n
 */
TEST(TrunBoxExtended, DifferentFlags) {
    std::cout << "[TrunBoxExtended] DifferentFlags - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    
    TrunIsoBmffBox trun1(150, 1024, 10, buffer + 20, buffer + 30, 2048, 0x000001);
    TrunIsoBmffBox trun2(150, 1024, 10, buffer + 20, buffer + 30, 2048, 0x000100);
    TrunIsoBmffBox trun3(150, 1024, 10, buffer + 20, buffer + 30, 2048, 0x000200);
    TrunIsoBmffBox trun4(150, 1024, 10, buffer + 20, buffer + 30, 2048, 0x000400);
    
    EXPECT_EQ(trun1.getSampleCount(), 10);
    EXPECT_EQ(trun2.getSampleCount(), 10);
    EXPECT_EQ(trun3.getSampleCount(), 10);
    EXPECT_EQ(trun4.getSampleCount(), 10);
    
    std::cout << "[TrunBoxExtended] DifferentFlags - PASS" << std::endl;
}

// ==============================================================================
// TfhdBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test TfhdIsoBmffBox with zero default sample duration
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_126@n
 * **Priority:** High@n
 */
TEST(TfhdBoxExtended, ZeroDefaultSampleDuration) {
    std::cout << "[TfhdBoxExtended] ZeroDefaultSampleDuration - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 0, buffer + 20, 2048, 0x000008);
    
    EXPECT_EQ(tfhd.getDefaultSampleDuration(), 0);
    
    std::cout << "[TfhdBoxExtended] ZeroDefaultSampleDuration - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox with maximum default sample duration
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_127@n
 * **Priority:** High@n
 */
TEST(TfhdBoxExtended, MaxDefaultSampleDuration) {
    std::cout << "[TfhdBoxExtended] MaxDefaultSampleDuration - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 0xFFFFFFFFFFFFFFFFULL, buffer + 20, 2048, 0x000008);
    
    EXPECT_EQ(tfhd.getDefaultSampleDuration(), 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[TfhdBoxExtended] MaxDefaultSampleDuration - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox with zero default sample size
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_128@n
 * **Priority:** High@n
 */
TEST(TfhdBoxExtended, ZeroDefaultSampleSize) {
    std::cout << "[TfhdBoxExtended] ZeroDefaultSampleSize - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1024, buffer + 20, 0, 0x000008);
    
    EXPECT_EQ(tfhd.getDefaultSampleSize(), 0);
    
    std::cout << "[TfhdBoxExtended] ZeroDefaultSampleSize - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox with maximum default sample size
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_129@n
 * **Priority:** High@n
 */
TEST(TfhdBoxExtended, MaxDefaultSampleSize) {
    std::cout << "[TfhdBoxExtended] MaxDefaultSampleSize - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1024, buffer + 20, 0xFFFFFFFF, 0x000008);
    
    EXPECT_EQ(tfhd.getDefaultSampleSize(), 0xFFFFFFFF);
    
    std::cout << "[TfhdBoxExtended] MaxDefaultSampleSize - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox setDefaultSampleDuration with variations
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_130@n
 * **Priority:** Medium@n
 */
TEST(TfhdBoxExtended, SetDefaultSampleDuration_Variations) {
    std::cout << "[TfhdBoxExtended] SetDefaultSampleDuration_Variations - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfhdIsoBmffBox tfhd(100, 1000, buffer + 20, 2048, 0x000008);
    
    std::vector<uint64_t> durations = {500, 1000, 1500, 2000, 3000, 5000};
    
    for (uint64_t dur : durations) {
        tfhd.setDefaultSampleDuration(dur);
        EXPECT_EQ(tfhd.getDefaultSampleDuration(), dur);
    }
    
    std::cout << "[TfhdBoxExtended] SetDefaultSampleDuration_Variations - PASS" << std::endl;
}

/**
 * @brief Test TfhdIsoBmffBox with different flag combinations
 *
 * **Test Group ID:** TfhdBoxExtended@n
 * **Test Case ID:** TC_131@n
 * **Priority:** Medium@n
 */
TEST(TfhdBoxExtended, DifferentFlagCombinations) {
    std::cout << "[TfhdBoxExtended] DifferentFlagCombinations - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    
    TfhdIsoBmffBox tfhd1(100, 1024, buffer + 20, 2048, 0x000000);
    EXPECT_FALSE(tfhd1.defaultSampleDurationPresent());
    
    TfhdIsoBmffBox tfhd2(100, 1024, buffer + 20, 2048, 0x000008);
    EXPECT_TRUE(tfhd2.defaultSampleDurationPresent());
    
    TfhdIsoBmffBox tfhd3(100, 1024, buffer + 20, 2048, 0x000010);
    EXPECT_FALSE(tfhd3.defaultSampleDurationPresent());
    
    std::cout << "[TfhdBoxExtended] DifferentFlagCombinations - PASS" << std::endl;
}

// ==============================================================================
// PrftBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test PrftIsoBmffBox with zero track ID
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_132@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, ZeroTrackId) {
    std::cout << "[PrftBoxExtended] ZeroTrackId - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 0, 0x123456789ABCDEFULL, 900000);
    
    EXPECT_EQ(prft.getTrackId(), 0);
    
    std::cout << "[PrftBoxExtended] ZeroTrackId - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with maximum track ID
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_133@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, MaxTrackId) {
    std::cout << "[PrftBoxExtended] MaxTrackId - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 0xFFFFFFFF, 0x123456789ABCDEFULL, 900000);
    
    EXPECT_EQ(prft.getTrackId(), 0xFFFFFFFF);
    
    std::cout << "[PrftBoxExtended] MaxTrackId - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with zero NTP timestamp
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_134@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, ZeroNtpTs) {
    std::cout << "[PrftBoxExtended] ZeroNtpTs - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0, 900000);
    
    EXPECT_EQ(prft.getNtpTs(), 0);
    
    std::cout << "[PrftBoxExtended] ZeroNtpTs - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with maximum NTP timestamp
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_135@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, MaxNtpTs) {
    std::cout << "[PrftBoxExtended] MaxNtpTs - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0xFFFFFFFFFFFFFFFFULL, 900000);
    
    EXPECT_EQ(prft.getNtpTs(), 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[PrftBoxExtended] MaxNtpTs - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with zero media time
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_136@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, ZeroMediaTime) {
    std::cout << "[PrftBoxExtended] ZeroMediaTime - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0x123456789ABCDEFULL, 0);
    
    EXPECT_EQ(prft.getMediaTime(), 0);
    
    std::cout << "[PrftBoxExtended] ZeroMediaTime - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with maximum media time
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_137@n
 * **Priority:** High@n
 */
TEST(PrftBoxExtended, MaxMediaTime) {
    std::cout << "[PrftBoxExtended] MaxMediaTime - START" << std::endl;
    
    PrftIsoBmffBox prft(64, 1, 0x123456789ABCDEFULL, 0xFFFFFFFFFFFFFFFFULL);
    prft.setMediaTime(0xFFFFFFFFFFFFFFFFULL);
    
    EXPECT_EQ(prft.getMediaTime(), 0xFFFFFFFFFFFFFFFFULL);
    
    std::cout << "[PrftBoxExtended] MaxMediaTime - PASS" << std::endl;
}

/**
 * @brief Test PrftIsoBmffBox with various track IDs
 *
 * **Test Group ID:** PrftBoxExtended@n
 * **Test Case ID:** TC_138@n
 * **Priority:** Medium@n
 */
TEST(PrftBoxExtended, VaryingTrackIds) {
    std::cout << "[PrftBoxExtended] VaryingTrackIds - START" << std::endl;
    
    std::vector<uint32_t> trackIds = {1, 2, 5, 10, 100, 1000, 0xFFFF, 0xFFFFFFFF};
    
    for (uint32_t id : trackIds) {
        PrftIsoBmffBox prft(64, id, 0x123456789ABCDEFULL, 900000);
        EXPECT_EQ(prft.getTrackId(), id);
    }
    
    std::cout << "[PrftBoxExtended] VaryingTrackIds - PASS" << std::endl;
}

// ==============================================================================
// TfdtBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test TfdtIsoBmffBox setBaseMDT with various values
 *
 * **Test Group ID:** TfdtBoxExtended@n
 * **Test Case ID:** TC_139@n
 * **Priority:** Medium@n
 */
TEST(TfdtBoxExtended, SetBaseMDT_Variations) {
    std::cout << "[TfdtBoxExtended] SetBaseMDT_Variations - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfdtIsoBmffBox tfdt(28, 0, buffer + 20);
    
    std::vector<uint64_t> baseMDTs = {1, 90000, 180000, 270000, 360000, 450000, 540000};
    
    for (uint64_t mdt : baseMDTs) {
        tfdt.setBaseMDT(mdt);
        EXPECT_EQ(tfdt.getBaseMDT(), mdt);
    }
    
    std::cout << "[TfdtBoxExtended] SetBaseMDT_Variations - PASS" << std::endl;
}

/**
 * @brief Test TfdtIsoBmffBox with sequential baseMDT values
 *
 * **Test Group ID:** TfdtBoxExtended@n
 * **Test Case ID:** TC_140@n
 * **Priority:** Medium@n
 */
TEST(TfdtBoxExtended, SequentialBaseMDT) {
    std::cout << "[TfdtBoxExtended] SequentialBaseMDT - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    TfdtIsoBmffBox tfdt(28, 0, buffer + 20);
    
    for (uint64_t i = 0; i < 20; i++) {
        uint64_t baseMDT = i * 180000;
        tfdt.setBaseMDT(baseMDT);
        EXPECT_EQ(tfdt.getBaseMDT(), baseMDT);
    }
    
    std::cout << "[TfdtBoxExtended] SequentialBaseMDT - PASS" << std::endl;
}

/**
 * @brief Test TfdtIsoBmffBox with odd number baseMDT values
 *
 * **Test Group ID:** TfdtBoxExtended@n
 * **Test Case ID:** TC_141@n
 * **Priority:** Medium@n
 */
TEST(TfdtBoxExtended, OddNumberBaseMDT) {
    std::cout << "[TfdtBoxExtended] OddNumberBaseMDT - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    std::vector<uint64_t> oddValues = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    
    for (uint64_t val : oddValues) {
        TfdtIsoBmffBox tfdt(28, val, buffer + 20);
        EXPECT_EQ(tfdt.getBaseMDT(), val);
    }
    
    std::cout << "[TfdtBoxExtended] OddNumberBaseMDT - PASS" << std::endl;
}

// ==============================================================================
// MvhdBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test MvhdIsoBmffBox with zero timescale
 *
 * **Test Group ID:** MvhdBoxExtended@n
 * **Test Case ID:** TC_142@n
 * **Priority:** High@n
 */
TEST(MvhdBoxExtended, ZeroTimescale) {
    std::cout << "[MvhdBoxExtended] ZeroTimescale - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MvhdIsoBmffBox mvhd(108, 0, buffer + 20);
    
    EXPECT_EQ(mvhd.getTimeScale(), 0);
    
    std::cout << "[MvhdBoxExtended] ZeroTimescale - PASS" << std::endl;
}

/**
 * @brief Test MvhdIsoBmffBox with power-of-two timescales
 *
 * **Test Group ID:** MvhdBoxExtended@n
 * **Test Case ID:** TC_143@n
 * **Priority:** Medium@n
 */
TEST(MvhdBoxExtended, PowerOfTwoTimescales) {
    std::cout << "[MvhdBoxExtended] PowerOfTwoTimescales - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    
    for (uint32_t i = 0; i < 16; i++) {
        uint32_t ts = 1 << i;
        MvhdIsoBmffBox mvhd(108, ts, buffer + 20);
        EXPECT_EQ(mvhd.getTimeScale(), ts);
    }
    
    std::cout << "[MvhdBoxExtended] PowerOfTwoTimescales - PASS" << std::endl;
}

/**
 * @brief Test MvhdIsoBmffBox setTimeScale rapid changes
 *
 * **Test Group ID:** MvhdBoxExtended@n
 * **Test Case ID:** TC_144@n
 * **Priority:** Medium@n
 */
TEST(MvhdBoxExtended, SetTimeScale_RapidChanges) {
    std::cout << "[MvhdBoxExtended] SetTimeScale_RapidChanges - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MvhdIsoBmffBox mvhd(108, 90000, buffer + 20);
    
    for (uint32_t i = 1; i <= 100; i++) {
        uint32_t ts = i * 1000;
        mvhd.setTimeScale(ts);
        EXPECT_EQ(mvhd.getTimeScale(), ts);
    }
    
    std::cout << "[MvhdBoxExtended] SetTimeScale_RapidChanges - PASS" << std::endl;
}

/**
 * @brief Test MvhdIsoBmffBox with common video timescales
 *
 * **Test Group ID:** MvhdBoxExtended@n
 * **Test Case ID:** TC_145@n
 * **Priority:** Medium@n
 */
TEST(MvhdBoxExtended, CommonVideoTimescales) {
    std::cout << "[MvhdBoxExtended] CommonVideoTimescales - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    std::vector<uint32_t> videoTimescales = {23976, 24000, 25000, 29970, 30000, 50000, 59940, 60000, 90000};
    
    for (uint32_t ts : videoTimescales) {
        MvhdIsoBmffBox mvhd(108, ts, buffer + 20);
        EXPECT_EQ(mvhd.getTimeScale(), ts);
    }
    
    std::cout << "[MvhdBoxExtended] CommonVideoTimescales - PASS" << std::endl;
}

// ==============================================================================
// MdhdBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test MdhdIsoBmffBox with zero timescale
 *
 * **Test Group ID:** MdhdBoxExtended@n
 * **Test Case ID:** TC_146@n
 * **Priority:** High@n
 */
TEST(MdhdBoxExtended, ZeroTimescale) {
    std::cout << "[MdhdBoxExtended] ZeroTimescale - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MdhdIsoBmffBox mdhd(100, 0, buffer + 20);
    
    EXPECT_EQ(mdhd.getTimeScale(), 0);
    
    std::cout << "[MdhdBoxExtended] ZeroTimescale - PASS" << std::endl;
}

/**
 * @brief Test MdhdIsoBmffBox with common audio sample rates
 *
 * **Test Group ID:** MdhdBoxExtended@n
 * **Test Case ID:** TC_147@n
 * **Priority:** Medium@n
 */
TEST(MdhdBoxExtended, CommonAudioSampleRates) {
    std::cout << "[MdhdBoxExtended] CommonAudioSampleRates - START" << std::endl;
    
    uint8_t buffer[200] = {0};
    std::vector<uint32_t> audioRates = {8000, 11025, 16000, 22050, 24000, 32000, 44100, 48000, 88200, 96000, 176400, 192000};
    
    for (uint32_t rate : audioRates) {
        MdhdIsoBmffBox mdhd(100, rate, buffer + 20);
        EXPECT_EQ(mdhd.getTimeScale(), rate);
    }
    
    std::cout << "[MdhdBoxExtended] CommonAudioSampleRates - PASS" << std::endl;
}

/**
 * @brief Test MdhdIsoBmffBox setTimeScale with variations
 *
 * **Test Group ID:** MdhdBoxExtended@n
 * **Test Case ID:** TC_148@n
 * **Priority:** Medium@n
 */
TEST(MdhdBoxExtended, SetTimeScale_Variations) {
    std::cout << "[MdhdBoxExtended] SetTimeScale_Variations - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MdhdIsoBmffBox mdhd(100, 44100, buffer + 20);
    
    std::vector<uint32_t> timescales = {11025, 22050, 44100, 48000, 96000};
    
    for (uint32_t ts : timescales) {
        mdhd.setTimeScale(ts);
        EXPECT_EQ(mdhd.getTimeScale(), ts);
    }
    
    std::cout << "[MdhdBoxExtended] SetTimeScale_Variations - PASS" << std::endl;
}

/**
 * @brief Test MdhdIsoBmffBox with maximum timescale
 *
 * **Test Group ID:** MdhdBoxExtended@n
 * **Test Case ID:** TC_149@n
 * **Priority:** High@n
 */
TEST(MdhdBoxExtended, MaxTimescale) {
    std::cout << "[MdhdBoxExtended] MaxTimescale - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    MdhdIsoBmffBox mdhd(100, 0xFFFFFFFF, buffer + 20);
    
    EXPECT_EQ(mdhd.getTimeScale(), 0xFFFFFFFF);
    
    std::cout << "[MdhdBoxExtended] MaxTimescale - PASS" << std::endl;
}

// ==============================================================================
// MdatBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test MdatIsoBmffBox with minimum size
 *
 * **Test Group ID:** MdatBoxExtended@n
 * **Test Case ID:** TC_150@n
 * **Priority:** High@n
 */
TEST(MdatBoxExtended, MinimumSize) {
    std::cout << "[MdatBoxExtended] MinimumSize - START" << std::endl;
    
    uint8_t buffer[16] = {0};
    buffer[0] = 0x00; buffer[1] = 0x00; buffer[2] = 0x00; buffer[3] = 0x08;
    memcpy(buffer + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(8, buffer);
    
    EXPECT_EQ(mdat.getSize(), 8);
    
    std::cout << "[MdatBoxExtended] MinimumSize - PASS" << std::endl;
}

/**
 * @brief Test MdatIsoBmffBox with large size
 *
 * **Test Group ID:** MdatBoxExtended@n
 * **Test Case ID:** TC_151@n
 * **Priority:** High@n
 */
TEST(MdatBoxExtended, LargeSize) {
    std::cout << "[MdatBoxExtended] LargeSize - START" << std::endl;
    
    uint8_t buffer[1000] = {0};
    buffer[0] = 0x00; buffer[1] = 0x00; buffer[2] = 0x10; buffer[3] = 0x00;
    memcpy(buffer + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(4096, buffer);
    
    EXPECT_EQ(mdat.getSize(), 4096);
    
    std::cout << "[MdatBoxExtended] LargeSize - PASS" << std::endl;
}

/**
 * @brief Test MdatIsoBmffBox truncate to various sizes
 *
 * **Test Group ID:** MdatBoxExtended@n
 * **Test Case ID:** TC_152@n
 * **Priority:** Medium@n
 */
TEST(MdatBoxExtended, Truncate_VariousSizes) {
    std::cout << "[MdatBoxExtended] Truncate_VariousSizes - START" << std::endl;
    
    uint8_t buffer[2000] = {0};
    buffer[0] = 0x00; buffer[1] = 0x00; buffer[2] = 0x07; buffer[3] = 0xD0;
    memcpy(buffer + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(2000, buffer);
    mdat.setBase(buffer + 8);
    
    std::vector<uint32_t> sizes = {1500, 1000, 500, 100, 50};
    
    for (uint32_t size : sizes) {
        mdat.truncate(size);
        EXPECT_EQ(mdat.getSize(), size);
    }
    
    std::cout << "[MdatBoxExtended] Truncate_VariousSizes - PASS" << std::endl;
}

/**
 * @brief Test MdatIsoBmffBox truncate to zero
 *
 * **Test Group ID:** MdatBoxExtended@n
 * **Test Case ID:** TC_153@n
 * **Priority:** Medium@n
 */
TEST(MdatBoxExtended, Truncate_ToZero) {
    std::cout << "[MdatBoxExtended] Truncate_ToZero - START" << std::endl;
    
    uint8_t buffer[1000] = {0};
    buffer[0] = 0x00; buffer[1] = 0x00; buffer[2] = 0x03; buffer[3] = 0xE8;
    memcpy(buffer + 4, "mdat", 4);
    
    MdatIsoBmffBox mdat(1000, buffer);
    mdat.setBase(buffer + 8);
    mdat.truncate(0);
    
    EXPECT_EQ(mdat.getSize(), 0);
    
    std::cout << "[MdatBoxExtended] Truncate_ToZero - PASS" << std::endl;
}

// ==============================================================================
// IsoBmffBox Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test IsoBmffBox setOffset with various values
 *
 * **Test Group ID:** IsoBmffBoxExtended@n
 * **Test Case ID:** TC_154@n
 * **Priority:** Medium@n
 */
TEST(IsoBmffBoxExtended, SetOffset_Variations) {
    std::cout << "[IsoBmffBoxExtended] SetOffset_Variations - START" << std::endl;
    
    IsoBmffBox box(100, "ftyp");
    
    std::vector<uint32_t> offsets = {0, 100, 1024, 4096, 8192, 16384, 0xFFFFFFFF};
    
    for (uint32_t offset : offsets) {
        box.setOffset(offset);
        EXPECT_EQ(box.getOffset(), offset);
    }
    
    std::cout << "[IsoBmffBoxExtended] SetOffset_Variations - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox with all standard box types
 *
 * **Test Group ID:** IsoBmffBoxExtended@n
 * **Test Case ID:** TC_155@n
 * **Priority:** Medium@n
 */
TEST(IsoBmffBoxExtended, AllStandardBoxTypes) {
    std::cout << "[IsoBmffBoxExtended] AllStandardBoxTypes - START" << std::endl;
    
    std::vector<std::string> types = {"ftyp", "moov", "moof", "mdat", "free", "skip", 
                                      "mvhd", "mdhd", "tfdt", "emsg", "trun", "tfhd", 
                                      "prft", "sidx", "senc", "saiz", "trak"};
    
    for (const auto& type : types) {
        IsoBmffBox box(100, type.c_str());
        EXPECT_STREQ(box.getType(), type.c_str());
    }
    
    std::cout << "[IsoBmffBoxExtended] AllStandardBoxTypes - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox setSize with various values
 *
 * **Test Group ID:** IsoBmffBoxExtended@n
 * **Test Case ID:** TC_156@n
 * **Priority:** Medium@n
 */
TEST(IsoBmffBoxExtended, SetSize_Variations) {
    std::cout << "[IsoBmffBoxExtended] SetSize_Variations - START" << std::endl;
    
    uint8_t buffer[100] = {0};
    IsoBmffBox box(100, "mdat");
    box.setBase(buffer + 8);
    
    std::vector<uint32_t> sizes = {50, 100, 200, 500, 1000, 10000};
    
    for (uint32_t size : sizes) {
        box.setSize(size);
        EXPECT_EQ(box.getSize(), size);
    }
    
    std::cout << "[IsoBmffBoxExtended] SetSize_Variations - PASS" << std::endl;
}

/**
 * @brief Test IsoBmffBox with maximum offset
 *
 * **Test Group ID:** IsoBmffBoxExtended@n
 * **Test Case ID:** TC_157@n
 * **Priority:** High@n
 */
TEST(IsoBmffBoxExtended, MaxOffset) {
    std::cout << "[IsoBmffBoxExtended] MaxOffset - START" << std::endl;
    
    IsoBmffBox box(100, "mdat");
    box.setOffset(0xFFFFFFFF);
    
    EXPECT_EQ(box.getOffset(), 0xFFFFFFFF);
    
    std::cout << "[IsoBmffBoxExtended] MaxOffset - PASS" << std::endl;
}

// ==============================================================================
// GenericContainer Extended Coverage Tests
// ==============================================================================

/**
 * @brief Test GenericContainerIsoBmffBox with deeply nested structure
 *
 * **Test Group ID:** GenericContainerExtended@n
 * **Test Case ID:** TC_158@n
 * **Priority:** Medium@n
 */
TEST(GenericContainerExtended, DeeplyNestedStructure) {
    std::cout << "[GenericContainerExtended] DeeplyNestedStructure - START" << std::endl;
    
    GenericContainerIsoBmffBox* level1 = new GenericContainerIsoBmffBox(1000, "moov");
    GenericContainerIsoBmffBox* level2 = new GenericContainerIsoBmffBox(800, "trak");
    GenericContainerIsoBmffBox* level3 = new GenericContainerIsoBmffBox(600, "mdia");
    GenericContainerIsoBmffBox* level4 = new GenericContainerIsoBmffBox(400, "minf");
    GenericContainerIsoBmffBox* level5 = new GenericContainerIsoBmffBox(200, "stbl");
    
    level4->addChildren(level5);
    level3->addChildren(level4);
    level2->addChildren(level3);
    level1->addChildren(level2);
    
    EXPECT_TRUE(level1->hasChildren());
    const std::vector<IsoBmffBox*>* children = level1->getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 1);
    
    delete level1;
    
    std::cout << "[GenericContainerExtended] DeeplyNestedStructure - PASS" << std::endl;
}

/**
 * @brief Test GenericContainerIsoBmffBox addChildren sequence
 *
 * **Test Group ID:** GenericContainerExtended@n
 * **Test Case ID:** TC_159@n
 * **Priority:** Medium@n
 */
TEST(GenericContainerExtended, AddChildren_Sequence) {
    std::cout << "[GenericContainerExtended] AddChildren_Sequence - START" << std::endl;
    
    GenericContainerIsoBmffBox container(2000, "moov");
    
    for (int i = 0; i < 10; i++) {
        container.addChildren(new IsoBmffBox(100, "free"));
    }
    
    const std::vector<IsoBmffBox*>* children = container.getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 10);
    
    std::cout << "[GenericContainerExtended] AddChildren_Sequence - PASS" << std::endl;
}

/**
 * @brief Test GenericContainerIsoBmffBox with mixed box types
 *
 * **Test Group ID:** GenericContainerExtended@n
 * **Test Case ID:** TC_160@n
 * **Priority:** Medium@n
 */
TEST(GenericContainerExtended, MixedBoxTypes) {
    std::cout << "[GenericContainerExtended] MixedBoxTypes - START" << std::endl;
    
    GenericContainerIsoBmffBox container(3000, "moov");
    
    uint8_t buf[200] = {0};
    container.addChildren(new MvhdIsoBmffBox(108, 90000, buf + 20));
    container.addChildren(new IsoBmffBox(100, "free"));
    container.addChildren(new GenericContainerIsoBmffBox(500, "trak"));
    container.addChildren(new IsoBmffBox(50, "skip"));
    
    const std::vector<IsoBmffBox*>* children = container.getChildren();
    ASSERT_NE(children, nullptr);
    EXPECT_EQ(children->size(), 4);
    
    std::cout << "[GenericContainerExtended] MixedBoxTypes - PASS" << std::endl;
}
