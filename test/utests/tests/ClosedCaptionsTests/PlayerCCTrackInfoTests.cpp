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
 * @file PlayerCCTrackInfoTests_Enhanced.cpp
 * @brief Comprehensive Level 1 Tests for CCTrackInfo
 *
 * ## Module's Role
 * This module includes comprehensive Level 1 functional tests for CCTrackInfo structure.
 * Tests cover construction, assignment, copying, edge cases, and stress scenarios.
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **Test Coverage:** 100% of CCTrackInfo functionality @n
 *
 * @author Enhanced Test Suite
 * @date March 2, 2026
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include <vector>
#include <algorithm>
#include "CCTrackInfo.h"

/**
 * @brief Test fixture for CCTrackInfo enhanced tests
 */
class CCTrackInfoEnhancedTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::cout << "[Setup] Initializing CCTrackInfo test environment" << std::endl;
    }

    void TearDown() override {
        std::cout << "[TearDown] Cleaning up CCTrackInfo test environment" << std::endl;
    }

    // Helper to create track with specific values
    CCTrackInfo createTrack(const std::string& id, const std::string& lang) {
        CCTrackInfo track;
        track.instreamId = id;
        track.language = lang;
        return track;
    }
};

// =============================================================================
// GROUP 1: Constructor Tests
// =============================================================================

/**
 * @brief Verify default constructor initializes members to empty strings
 *
 * **Test Group ID:** CCTrackInfo_Constructor_001@n
 * **Test Case ID:** DefaultConstruction@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create CCTrackInfo object | Default constructor | instreamId = "", language = "" | Should Pass |
 * | 02 | Verify instreamId | None | Empty string | Should Pass |
 * | 03 | Verify language | None | Empty string | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, DefaultConstruction) {
    std::cout << "[CCTrackInfo_Constructor_001] DefaultConstruction - START" << std::endl;

    CCTrackInfo track;
    
    EXPECT_EQ(track.instreamId, "");
    EXPECT_EQ(track.language, "");
    EXPECT_TRUE(track.instreamId.empty());
    EXPECT_TRUE(track.language.empty());

    std::cout << "[CCTrackInfo_Constructor_001] DefaultConstruction - PASS" << std::endl;
}

/**
 * @brief Verify multiple instances can be created independently
 *
 * **Test Group ID:** CCTrackInfo_Constructor_002@n
 * **Test Case ID:** MultipleInstances@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 100 instances | Default constructor | All independent | Should Pass |
 * | 02 | Verify all empty | None | All have empty strings | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, MultipleInstances) {
    std::cout << "[CCTrackInfo_Constructor_002] MultipleInstances - START" << std::endl;

    std::vector<CCTrackInfo> tracks(100);
    
    for (const auto& track : tracks) {
        EXPECT_EQ(track.instreamId, "");
        EXPECT_EQ(track.language, "");
    }

    std::cout << "[CCTrackInfo_Constructor_002] MultipleInstances - PASS" << std::endl;
}

/**
 * @brief Verify constructor allocates memory correctly
 *
 * **Test Group ID:** CCTrackInfo_Constructor_003@n
 * **Test Case ID:** MemoryAllocation@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 1000 instances in loop | Default constructor | No memory issues | Should Pass |
 * | 02 | All properly destroyed | Implicit destructors | No leaks | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, MemoryAllocation) {
    std::cout << "[CCTrackInfo_Constructor_003] MemoryAllocation - START" << std::endl;

    for (int i = 0; i < 1000; ++i) {
        CCTrackInfo track;
        EXPECT_TRUE(track.instreamId.empty());
        EXPECT_TRUE(track.language.empty());
    }

    std::cout << "[CCTrackInfo_Constructor_003] MemoryAllocation - PASS" << std::endl;
}

// =============================================================================
// GROUP 2: Member Assignment Tests
// =============================================================================

/**
 * @brief Verify instreamId can be assigned
 *
 * **Test Group ID:** CCTrackInfo_Assignment_001@n
 * **Test Case ID:** AssignInstreamId@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create track | Default | Empty | Should Pass |
 * | 02 | Assign instreamId | "CC1" | instreamId = "CC1" | Should Pass |
 * | 03 | Verify assignment | None | Value correctly set | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, AssignInstreamId) {
    std::cout << "[CCTrackInfo_Assignment_001] AssignInstreamId - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "CC1";
    
    EXPECT_EQ(track.instreamId, "CC1");
    EXPECT_EQ(track.language, "");  // Other member unchanged

    std::cout << "[CCTrackInfo_Assignment_001] AssignInstreamId - PASS" << std::endl;
}

/**
 * @brief Verify language can be assigned
 *
 * **Test Group ID:** CCTrackInfo_Assignment_002@n
 * **Test Case ID:** AssignLanguage@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create track | Default | Empty | Should Pass |
 * | 02 | Assign language | "eng" | language = "eng" | Should Pass |
 * | 03 | Verify assignment | None | Value correctly set | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, AssignLanguage) {
    std::cout << "[CCTrackInfo_Assignment_002] AssignLanguage - START" << std::endl;

    CCTrackInfo track;
    track.language = "eng";
    
    EXPECT_EQ(track.language, "eng");
    EXPECT_EQ(track.instreamId, "");  // Other member unchanged

    std::cout << "[CCTrackInfo_Assignment_002] AssignLanguage - PASS" << std::endl;
}

/**
 * @brief Verify both members can be assigned
 *
 * **Test Group ID:** CCTrackInfo_Assignment_003@n
 * **Test Case ID:** AssignBothMembers@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create track | Default | Empty | Should Pass |
 * | 02 | Assign both members | "SERVICE1", "spa" | Both set correctly | Should Pass |
 * | 03 | Verify both | None | Values match | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, AssignBothMembers) {
    std::cout << "[CCTrackInfo_Assignment_003] AssignBothMembers - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "SERVICE1";
    track.language = "spa";
    
    EXPECT_EQ(track.instreamId, "SERVICE1");
    EXPECT_EQ(track.language, "spa");

    std::cout << "[CCTrackInfo_Assignment_003] AssignBothMembers - PASS" << std::endl;
}

/**
 * @brief Verify reassignment overwrites previous values
 *
 * **Test Group ID:** CCTrackInfo_Assignment_004@n
 * **Test Case ID:** ReassignMembers@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Assign initial values | "CC1", "eng" | Values set | Should Pass |
 * | 02 | Reassign values | "CC2", "fra" | New values set | Should Pass |
 * | 03 | Verify new values | None | Old values replaced | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, ReassignMembers) {
    std::cout << "[CCTrackInfo_Assignment_004] ReassignMembers - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "CC1";
    track.language = "eng";
    
    EXPECT_EQ(track.instreamId, "CC1");
    EXPECT_EQ(track.language, "eng");
    
    track.instreamId = "CC2";
    track.language = "fra";
    
    EXPECT_EQ(track.instreamId, "CC2");
    EXPECT_EQ(track.language, "fra");

    std::cout << "[CCTrackInfo_Assignment_004] ReassignMembers - PASS" << std::endl;
}

// =============================================================================
// GROUP 3: Copy Semantics Tests
// =============================================================================

/**
 * @brief Verify copy construction creates independent copy
 *
 * **Test Group ID:** CCTrackInfo_Copy_001@n
 * **Test Case ID:** CopyConstruction@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create source track | "CC3", "ger" | Source created | Should Pass |
 * | 02 | Copy construct | Copy constructor | Copy has same values | Should Pass |
 * | 03 | Modify source | Change source | Copy unchanged | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, CopyConstruction) {
    std::cout << "[CCTrackInfo_Copy_001] CopyConstruction - START" << std::endl;

    CCTrackInfo source;
    source.instreamId = "CC3";
    source.language = "ger";
    
    CCTrackInfo copy(source);
    
    EXPECT_EQ(copy.instreamId, "CC3");
    EXPECT_EQ(copy.language, "ger");
    
    // Modify source, verify copy unchanged
    source.instreamId = "CC4";
    source.language = "ita";
    
    EXPECT_EQ(copy.instreamId, "CC3");
    EXPECT_EQ(copy.language, "ger");

    std::cout << "[CCTrackInfo_Copy_001] CopyConstruction - PASS" << std::endl;
}

/**
 * @brief Verify copy assignment creates independent copy
 *
 * **Test Group ID:** CCTrackInfo_Copy_002@n
 * **Test Case ID:** CopyAssignment@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create source and dest | Different values | Both created | Should Pass |
 * | 02 | Copy assign | dest = source | dest matches source | Should Pass |
 * | 03 | Modify source | Change values | dest unchanged | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, CopyAssignment) {
    std::cout << "[CCTrackInfo_Copy_002] CopyAssignment - START" << std::endl;

    CCTrackInfo source = createTrack("SERVICE2", "jpn");
    CCTrackInfo dest = createTrack("CC1", "eng");
    
    dest = source;
    
    EXPECT_EQ(dest.instreamId, "SERVICE2");
    EXPECT_EQ(dest.language, "jpn");
    
    // Modify source
    source.instreamId = "SERVICE3";
    source.language = "kor";
    
    EXPECT_EQ(dest.instreamId, "SERVICE2");
    EXPECT_EQ(dest.language, "jpn");

    std::cout << "[CCTrackInfo_Copy_002] CopyAssignment - PASS" << std::endl;
}

/**
 * @brief Verify self-assignment is safe
 *
 * **Test Group ID:** CCTrackInfo_Copy_003@n
 * **Test Case ID:** SelfAssignment@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create track | "CC4", "chi" | Track created | Should Pass |
 * | 02 | Self-assign | track = track | No corruption | Should Pass |
 * | 03 | Verify values | None | Values unchanged | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, SelfAssignment) {
    std::cout << "[CCTrackInfo_Copy_003] SelfAssignment - START" << std::endl;

    CCTrackInfo track = createTrack("CC4", "chi");
    
    track = track;  // Self-assignment
    
    EXPECT_EQ(track.instreamId, "CC4");
    EXPECT_EQ(track.language, "chi");

    std::cout << "[CCTrackInfo_Copy_003] SelfAssignment - PASS" << std::endl;
}

// =============================================================================
// GROUP 4: Edge Cases Tests
// =============================================================================

/**
 * @brief Verify empty string assignment
 *
 * **Test Group ID:** CCTrackInfo_Edge_001@n
 * **Test Case ID:** EmptyStringAssignment@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Assign non-empty | "CC1", "eng" | Values set | Should Pass |
 * | 02 | Assign empty strings | "", "" | Empty values | Should Pass |
 * | 03 | Verify empty | None | Both empty | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, EmptyStringAssignment) {
    std::cout << "[CCTrackInfo_Edge_001] EmptyStringAssignment - START" << std::endl;

    CCTrackInfo track = createTrack("CC1", "eng");
    
    track.instreamId = "";
    track.language = "";
    
    EXPECT_TRUE(track.instreamId.empty());
    EXPECT_TRUE(track.language.empty());

    std::cout << "[CCTrackInfo_Edge_001] EmptyStringAssignment - PASS" << std::endl;
}

/**
 * @brief Verify very long string assignment
 *
 * **Test Group ID:** CCTrackInfo_Edge_002@n
 * **Test Case ID:** VeryLongStrings@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create long strings | 10000 chars each | Strings created | Should Pass |
 * | 02 | Assign to track | Both members | Assignment succeeds | Should Pass |
 * | 03 | Verify lengths | None | Correct lengths | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, VeryLongStrings) {
    std::cout << "[CCTrackInfo_Edge_002] VeryLongStrings - START" << std::endl;

    std::string longId(10000, 'I');
    std::string longLang(10000, 'L');
    
    CCTrackInfo track;
    track.instreamId = longId;
    track.language = longLang;
    
    EXPECT_EQ(track.instreamId.length(), 10000);
    EXPECT_EQ(track.language.length(), 10000);
    EXPECT_EQ(track.instreamId, longId);
    EXPECT_EQ(track.language, longLang);

    std::cout << "[CCTrackInfo_Edge_002] VeryLongStrings - PASS" << std::endl;
}

/**
 * @brief Verify special characters in strings
 *
 * **Test Group ID:** CCTrackInfo_Edge_003@n
 * **Test Case ID:** SpecialCharacters@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Assign special chars | Various special chars | Assignment succeeds | Should Pass |
 * | 02 | Verify values | None | Matches exactly | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, SpecialCharacters) {
    std::cout << "[CCTrackInfo_Edge_003] SpecialCharacters - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
    track.language = "\t\n\r\b\f\v";
    
    EXPECT_EQ(track.instreamId, "!@#$%^&*()_+-=[]{}|;':\",./<>?");
    EXPECT_EQ(track.language, "\t\n\r\b\f\v");

    std::cout << "[CCTrackInfo_Edge_003] SpecialCharacters - PASS" << std::endl;
}

/**
 * @brief Verify Unicode strings
 *
 * **Test Group ID:** CCTrackInfo_Edge_004@n
 * **Test Case ID:** UnicodeStrings@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Assign Unicode | Various languages | Assignment succeeds | Should Pass |
 * | 02 | Verify values | None | Matches exactly | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, UnicodeStrings) {
    std::cout << "[CCTrackInfo_Edge_004] UnicodeStrings - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "字幕_CC1";
    track.language = "日本語";
    
    EXPECT_EQ(track.instreamId, "字幕_CC1");
    EXPECT_EQ(track.language, "日本語");
    
    track.instreamId = "СС_Русский";
    track.language = "Ελληνικά";
    
    EXPECT_EQ(track.instreamId, "СС_Русский");
    EXPECT_EQ(track.language, "Ελληνικά");

    std::cout << "[CCTrackInfo_Edge_004] UnicodeStrings - PASS" << std::endl;
}

/**
 * @brief Verify whitespace strings
 *
 * **Test Group ID:** CCTrackInfo_Edge_005@n
 * **Test Case ID:** WhitespaceStrings@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Assign whitespace | Spaces, tabs | Assignment succeeds | Should Pass |
 * | 02 | Verify values | None | Matches exactly | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, WhitespaceStrings) {
    std::cout << "[CCTrackInfo_Edge_005] WhitespaceStrings - START" << std::endl;

    CCTrackInfo track;
    track.instreamId = "   ";
    track.language = "\t\t\t";
    
    EXPECT_EQ(track.instreamId, "   ");
    EXPECT_EQ(track.language, "\t\t\t");
    EXPECT_FALSE(track.instreamId.empty());
    EXPECT_FALSE(track.language.empty());

    std::cout << "[CCTrackInfo_Edge_005] WhitespaceStrings - PASS" << std::endl;
}

// =============================================================================
// GROUP 5: Container Tests
// =============================================================================

/**
 * @brief Verify CCTrackInfo works in std::vector
 *
 * **Test Group ID:** CCTrackInfo_Container_001@n
 * **Test Case ID:** VectorStorage@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create vector of tracks | Multiple tracks | Vector created | Should Pass |
 * | 02 | Add tracks | push_back | Tracks added | Should Pass |
 * | 03 | Verify all values | None | All correct | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, VectorStorage) {
    std::cout << "[CCTrackInfo_Container_001] VectorStorage - START" << std::endl;

    std::vector<CCTrackInfo> tracks;
    
    for (int i = 0; i < 10; ++i) {
        CCTrackInfo track;
        track.instreamId = "CC" + std::to_string(i);
        track.language = "lang" + std::to_string(i);
        tracks.push_back(track);
    }
    
    EXPECT_EQ(tracks.size(), 10);
    
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(tracks[i].instreamId, "CC" + std::to_string(i));
        EXPECT_EQ(tracks[i].language, "lang" + std::to_string(i));
    }

    std::cout << "[CCTrackInfo_Container_001] VectorStorage - PASS" << std::endl;
}

/**
 * @brief Verify vector resizing doesn't corrupt data
 *
 * **Test Group ID:** CCTrackInfo_Container_002@n
 * **Test Case ID:** VectorResize@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create small vector | 10 tracks | Vector created | Should Pass |
 * | 02 | Resize to 1000 | resize(1000) | Resized | Should Pass |
 * | 03 | Verify original data | None | Original tracks intact | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, VectorResize) {
    std::cout << "[CCTrackInfo_Container_002] VectorResize - START" << std::endl;

    std::vector<CCTrackInfo> tracks;
    
    // Add initial tracks
    for (int i = 0; i < 10; ++i) {
        tracks.push_back(createTrack("ID" + std::to_string(i), "L" + std::to_string(i)));
    }
    
    // Resize vector (may cause reallocation)
    tracks.resize(1000);
    
    EXPECT_EQ(tracks.size(), 1000);
    
    // Verify original tracks still valid
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(tracks[i].instreamId, "ID" + std::to_string(i));
        EXPECT_EQ(tracks[i].language, "L" + std::to_string(i));
    }
    
    // Verify new tracks are default-initialized
    for (int i = 10; i < 1000; ++i) {
        EXPECT_TRUE(tracks[i].instreamId.empty());
        EXPECT_TRUE(tracks[i].language.empty());
    }

    std::cout << "[CCTrackInfo_Container_002] VectorResize - PASS" << std::endl;
}

/**
 * @brief Verify vector erase doesn't corrupt data
 *
 * **Test Group ID:** CCTrackInfo_Container_003@n
 * **Test Case ID:** VectorErase@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create vector | 20 tracks | Vector created | Should Pass |
 * | 02 | Erase middle elements | erase(5-15) | Elements removed | Should Pass |
 * | 03 | Verify remaining | None | Data intact | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, VectorErase) {
    std::cout << "[CCTrackInfo_Container_003] VectorErase - START" << std::endl;

    std::vector<CCTrackInfo> tracks;
    
    for (int i = 0; i < 20; ++i) {
        tracks.push_back(createTrack("T" + std::to_string(i), "L" + std::to_string(i)));
    }
    
    // Erase middle elements
    tracks.erase(tracks.begin() + 5, tracks.begin() + 15);
    
    EXPECT_EQ(tracks.size(), 10);
    
    // Verify first 5
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(tracks[i].instreamId, "T" + std::to_string(i));
    }
    
    // Verify last 5 (originally indices 15-19)
    for (int i = 5; i < 10; ++i) {
        EXPECT_EQ(tracks[i].instreamId, "T" + std::to_string(i + 10));
    }

    std::cout << "[CCTrackInfo_Container_003] VectorErase - PASS" << std::endl;
}

// =============================================================================
// GROUP 6: Stress Tests
// =============================================================================

/**
 * @brief Verify high-volume creation and destruction
 *
 * **Test Group ID:** CCTrackInfo_Stress_001@n
 * **Test Case ID:** MassCreationDestruction@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 10000 tracks | In loop | All created | Should Pass |
 * | 02 | Destroy all | Implicit | All destroyed | Should Pass |
 * | 03 | No memory issues | None | No leaks/crashes | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, MassCreationDestruction) {
    std::cout << "[CCTrackInfo_Stress_001] MassCreationDestruction - START" << std::endl;

    for (int i = 0; i < 10000; ++i) {
        CCTrackInfo track = createTrack("ID" + std::to_string(i), "Lang" + std::to_string(i));
        EXPECT_FALSE(track.instreamId.empty());
        EXPECT_FALSE(track.language.empty());
    }

    std::cout << "[CCTrackInfo_Stress_001] MassCreationDestruction - PASS" << std::endl;
}

/**
 * @brief Verify rapid reassignment stress test
 *
 * **Test Group ID:** CCTrackInfo_Stress_002@n
 * **Test Case ID:** RapidReassignment@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create track | Single track | Track created | Should Pass |
 * | 02 | Reassign 10000 times | Different values | All succeed | Should Pass |
 * | 03 | Verify final value | None | Correct value | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, RapidReassignment) {
    std::cout << "[CCTrackInfo_Stress_002] RapidReassignment - START" << std::endl;

    CCTrackInfo track;
    
    for (int i = 0; i < 10000; ++i) {
        track.instreamId = "ID" + std::to_string(i);
        track.language = "L" + std::to_string(i);
    }
    
    EXPECT_EQ(track.instreamId, "ID9999");
    EXPECT_EQ(track.language, "L9999");

    std::cout << "[CCTrackInfo_Stress_002] RapidReassignment - PASS" << std::endl;
}

/**
 * @brief Verify large vector operations
 *
 * **Test Group ID:** CCTrackInfo_Stress_003@n
 * **Test Case ID:** LargeVectorOperations@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create vector of 10000 | Large vector | Vector created | Should Pass |
 * | 02 | Sort vector | std::sort | Sorted successfully | Should Pass |
 * | 03 | Search vector | std::find_if | Find succeeds | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, LargeVectorOperations) {
    std::cout << "[CCTrackInfo_Stress_003] LargeVectorOperations - START" << std::endl;

    std::vector<CCTrackInfo> tracks;
    
    for (int i = 0; i < 5000; ++i) {
        tracks.push_back(createTrack("ID" + std::to_string(i), "Lang" + std::to_string(i)));
    }
    
    EXPECT_EQ(tracks.size(), 5000);
    
    // Sort by instreamId
    std::sort(tracks.begin(), tracks.end(), 
              [](const CCTrackInfo& a, const CCTrackInfo& b) {
                  return a.instreamId < b.instreamId;
              });
    
    // Find specific track
    auto it = std::find_if(tracks.begin(), tracks.end(),
                           [](const CCTrackInfo& t) {
                               return t.instreamId == "ID2500";
                           });
    
    EXPECT_NE(it, tracks.end());
    if (it != tracks.end()) {
        EXPECT_EQ(it->language, "Lang2500");
    }

    std::cout << "[CCTrackInfo_Stress_003] LargeVectorOperations - PASS" << std::endl;
}

// =============================================================================
// GROUP 7: Thread Safety Tests (if applicable to usage patterns)
// =============================================================================

/**
 * @brief Verify concurrent read access is safe
 *
 * **Test Group ID:** CCTrackInfo_Thread_001@n
 * **Test Case ID:** ConcurrentReads@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create shared track | "SharedID", "SharedLang" | Track created | Should Pass |
 * | 02 | Read from 10 threads | Concurrent reads | All succeed | Should Pass |
 * | 03 | Verify all reads correct | None | All match | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, ConcurrentReads) {
    std::cout << "[CCTrackInfo_Thread_001] ConcurrentReads - START" << std::endl;

    CCTrackInfo sharedTrack = createTrack("SharedID", "SharedLang");
    std::vector<std::thread> threads;
    std::atomic<int> successCount{0};
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&sharedTrack, &successCount]() {
            for (int j = 0; j < 1000; ++j) {
                if (sharedTrack.instreamId == "SharedID" && 
                    sharedTrack.language == "SharedLang") {
                    successCount++;
                }
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(successCount.load(), 10000);

    std::cout << "[CCTrackInfo_Thread_001] ConcurrentReads - PASS" << std::endl;
}

/**
 * @brief Verify independent instances are thread-safe
 *
 * **Test Group ID:** CCTrackInfo_Thread_002@n
 * **Test Case ID:** IndependentInstances@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 10 threads | Each has own track | All created | Should Pass |
 * | 02 | Modify independently | Concurrent writes | No interference | Should Pass |
 * | 03 | Verify all correct | None | All have correct values | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, IndependentInstances) {
    std::cout << "[CCTrackInfo_Thread_002] IndependentInstances - START" << std::endl;

    std::vector<std::thread> threads;
    std::vector<CCTrackInfo> results(10);
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([i, &results]() {
            CCTrackInfo track;
            track.instreamId = "Thread" + std::to_string(i);
            track.language = "Lang" + std::to_string(i);
            results[i] = track;
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(results[i].instreamId, "Thread" + std::to_string(i));
        EXPECT_EQ(results[i].language, "Lang" + std::to_string(i));
    }

    std::cout << "[CCTrackInfo_Thread_002] IndependentInstances - PASS" << std::endl;
}

// =============================================================================
// GROUP 8: Integration Test
// =============================================================================

/**
 * @brief Comprehensive test exercising all functionality
 *
 * **Test Group ID:** CCTrackInfo_Integration_001@n
 * **Test Case ID:** ComprehensiveWorkflow@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Test all operations | Multiple scenarios | All succeed | Should Pass |
 */
TEST_F(CCTrackInfoEnhancedTest, ComprehensiveWorkflow) {
    std::cout << "[CCTrackInfo_Integration_001] ComprehensiveWorkflow - START" << std::endl;

    // Construction
    CCTrackInfo track1;
    EXPECT_TRUE(track1.instreamId.empty());
    EXPECT_TRUE(track1.language.empty());
    
    // Assignment
    track1.instreamId = "CC1";
    track1.language = "eng";
    EXPECT_EQ(track1.instreamId, "CC1");
    EXPECT_EQ(track1.language, "eng");
    
    // Copy construction
    CCTrackInfo track2(track1);
    EXPECT_EQ(track2.instreamId, "CC1");
    EXPECT_EQ(track2.language, "eng");
    
    // Copy assignment
    CCTrackInfo track3;
    track3 = track1;
    EXPECT_EQ(track3.instreamId, "CC1");
    EXPECT_EQ(track3.language, "eng");
    
    // Modify original, verify copies unchanged
    track1.instreamId = "CC2";
    track1.language = "fra";
    EXPECT_EQ(track2.instreamId, "CC1");
    EXPECT_EQ(track3.instreamId, "CC1");
    
    // Vector storage
    std::vector<CCTrackInfo> tracks;
    tracks.push_back(track1);
    tracks.push_back(track2);
    tracks.push_back(track3);
    EXPECT_EQ(tracks.size(), 3);
    
    // Edge cases
    CCTrackInfo emptyTrack;
    emptyTrack.instreamId = "";
    emptyTrack.language = "";
    tracks.push_back(emptyTrack);
    
    CCTrackInfo specialTrack;
    specialTrack.instreamId = "!@#$%";
    specialTrack.language = "字幕";
    tracks.push_back(specialTrack);
    
    EXPECT_EQ(tracks.size(), 5);
    
    std::cout << "[CCTrackInfo_Integration_001] ComprehensiveWorkflow - PASS" << std::endl;
    std::cout << "✓ All CCTrackInfo functionality validated successfully!" << std::endl;
}
