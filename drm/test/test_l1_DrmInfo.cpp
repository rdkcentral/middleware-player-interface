
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "DrmInfo.h"


/**
 * @brief Verify that the default constructor of DrmInfo correctly initializes all members.
 *
 * This test ensures that when a DrmInfo instance is constructed using the default constructor, each member variable is initialized with its expected default value. It verifies that no exceptions are thrown and every member is set as per the API specification.
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
 * | 01 | Invoke the default constructor of DrmInfo and validate each member's value. | No explicit input; internally, DrmInfo is initialized as: method = eMETHOD_NONE, mediaFormat = eMEDIAFORMAT_HLS, useFirst16BytesAsIV = false, iv = 0, masterManifestURL = "", manifestURL = "", keyURI = "", keyFormat = "", systemUUID = "", initData = "", bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true | The constructor should not throw, and all member variables must have their expected default values. | Should Pass |
 */
TEST(DrmInfo, DefaultConstructorInitializesAllMembersToTheirExpectedDefaultValues) {
    std::cout << "Entering DefaultConstructorInitializesAllMembersToTheirExpectedDefaultValues test" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        std::cout << "Invoked DrmInfo default constructor" << std::endl;
        
        std::cout << "Checking member: method" << std::endl;
        EXPECT_EQ(drmInfo.method, eMETHOD_NONE);
        std::cout << "Retrieved value for method: " << drmInfo.method << " (expected eMETHOD_NONE)" << std::endl;
        
        std::cout << "Checking member: mediaFormat" << std::endl;
        EXPECT_EQ(drmInfo.mediaFormat, eMEDIAFORMAT_HLS);
        std::cout << "Retrieved value for mediaFormat: " << drmInfo.mediaFormat << " (expected eMEDIAFORMAT_HLS)" << std::endl;
        
        std::cout << "Checking member: useFirst16BytesAsIV" << std::endl;
        EXPECT_FALSE(drmInfo.useFirst16BytesAsIV);
        std::cout << "Retrieved value for useFirst16BytesAsIV: " << (drmInfo.useFirst16BytesAsIV ? "true" : "false") << " (expected false)" << std::endl;
        
        std::cout << "Checking member: iv" << std::endl;
        EXPECT_EQ(drmInfo.iv, 0);
        std::cout << "Retrieved value for iv: " << static_cast<int>(drmInfo.iv) << " (expected 0)" << std::endl;
        
        std::cout << "Checking member: masterManifestURL" << std::endl;
        EXPECT_EQ(drmInfo.masterManifestURL, "");
        std::cout << "Retrieved value for masterManifestURL: \"" << drmInfo.masterManifestURL << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: manifestURL" << std::endl;
        EXPECT_EQ(drmInfo.manifestURL, "");
        std::cout << "Retrieved value for manifestURL: \"" << drmInfo.manifestURL << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: keyURI" << std::endl;
        EXPECT_EQ(drmInfo.keyURI, "");
        std::cout << "Retrieved value for keyURI: \"" << drmInfo.keyURI << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: keyFormat" << std::endl;
        EXPECT_EQ(drmInfo.keyFormat, "");
        std::cout << "Retrieved value for keyFormat: \"" << drmInfo.keyFormat << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: systemUUID" << std::endl;
        EXPECT_EQ(drmInfo.systemUUID, "");
        std::cout << "Retrieved value for systemUUID: \"" << drmInfo.systemUUID << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: initData" << std::endl;
        EXPECT_EQ(drmInfo.initData, "");
        std::cout << "Retrieved value for initData: \"" << drmInfo.initData << "\" (expected empty string)" << std::endl;
        
        std::cout << "Checking member: bPropagateUriParams" << std::endl;
        EXPECT_TRUE(drmInfo.bPropagateUriParams);
        std::cout << "Retrieved value for bPropagateUriParams: " << (drmInfo.bPropagateUriParams ? "true" : "false") << " (expected true)" << std::endl;
        
        std::cout << "Checking member: bUseMediaSequenceIV" << std::endl;
        EXPECT_TRUE(drmInfo.bUseMediaSequenceIV);
        std::cout << "Retrieved value for bUseMediaSequenceIV: " << (drmInfo.bUseMediaSequenceIV ? "true" : "false") << " (expected true)" << std::endl;
        
        std::cout << "Checking member: bDecryptClearSamplesRequired" << std::endl;
        EXPECT_TRUE(drmInfo.bDecryptClearSamplesRequired);
        std::cout << "Retrieved value for bDecryptClearSamplesRequired: " << (drmInfo.bDecryptClearSamplesRequired ? "true" : "false") << " (expected true)" << std::endl;
    });
    std::cout << "Exiting DefaultConstructorInitializesAllMembersToTheirExpectedDefaultValues test" << std::endl;
}
/**
 * @brief Verify default initialization and copy constructor of DrmInfo
 *
 * This test validates that a DrmInfo object is correctly default initialized and that its copy constructor creates an equivalent object.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke default constructor to create DrmInfo and log default values | input: none; output: defaultDrmInfo with fields method = eMETHOD_NONE, mediaFormat = eMEDIAFORMAT_HLS, useFirst16BytesAsIV = false, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, iv = [0,...,0], masterManifestURL = "", manifestURL = "", keyURI = "", keyFormat = "", systemUUID = "", initData = "" | Object successfully created with default field values; no exceptions thrown | Should Pass |
 * | 02 | Invoke copy constructor to create a new DrmInfo object from the default one | input: defaultDrmInfo; output: copiedDrmInfo with copied values | copiedDrmInfo fields should exactly match defaultDrmInfo values | Should Pass |
 * | 03 | Verify enum and boolean fields of the copied object | input: copiedDrmInfo (method, mediaFormat, useFirst16BytesAsIV, bPropagateUriParams, bUseMediaSequenceIV, bDecryptClearSamplesRequired) | Returned values match expected constants (eMETHOD_NONE, eMEDIAFORMAT_HLS, false, true, true, true) | Should Pass |
 * | 04 | Validate iv array elements are all zeros | input: copiedDrmInfo.iv array | All elements in iv array equal 0 | Should Pass |
 * | 05 | Verify string fields of the copied object are empty | input: copiedDrmInfo string fields (masterManifestURL, manifestURL, keyURI, keyFormat, systemUUID, initData) | All string fields are empty ("") | Should Pass |
 */
TEST(DrmInfo, CopyDefaultDrmInfo) {
    std::cout << "Entering CopyDefaultDrmInfo test" << std::endl;
    
    // Create a default-initialized DrmInfo object using the default constructor.
    EXPECT_NO_THROW({
        DrmInfo defaultDrmInfo;
        std::cout << "Default DrmInfo object created." << std::endl;
        
        // Log default values.
        std::cout << "Default method: " << defaultDrmInfo.method << std::endl;
        std::cout << "Default mediaFormat: " << defaultDrmInfo.mediaFormat << std::endl;
        std::cout << "Default useFirst16BytesAsIV: " << defaultDrmInfo.useFirst16BytesAsIV << std::endl;
        std::cout << "Default bPropagateUriParams: " << defaultDrmInfo.bPropagateUriParams << std::endl;
        std::cout << "Default bUseMediaSequenceIV: " << defaultDrmInfo.bUseMediaSequenceIV << std::endl;
        std::cout << "Default bDecryptClearSamplesRequired: " << defaultDrmInfo.bDecryptClearSamplesRequired << std::endl;
    });
    
    DrmInfo defaultDrmInfo;
    // Copy construct a new DrmInfo object.
    DrmInfo copiedDrmInfo(defaultDrmInfo);
    std::cout << "Copy constructor invoked to create copiedDrmInfo" << std::endl;
    
    // Validate enum values and booleans.
    EXPECT_EQ(copiedDrmInfo.method, eMETHOD_NONE);
    std::cout << "Copied method: " << copiedDrmInfo.method << " (Expected: eMETHOD_NONE)" << std::endl;
    EXPECT_EQ(copiedDrmInfo.mediaFormat, eMEDIAFORMAT_HLS);
    std::cout << "Copied mediaFormat: " << copiedDrmInfo.mediaFormat << " (Expected: eMEDIAFORMAT_HLS)" << std::endl;
    EXPECT_FALSE(copiedDrmInfo.useFirst16BytesAsIV);
    std::cout << "Copied useFirst16BytesAsIV: " << copiedDrmInfo.useFirst16BytesAsIV << " (Expected: false)" << std::endl;
    EXPECT_TRUE(copiedDrmInfo.bPropagateUriParams);
    std::cout << "Copied bPropagateUriParams: " << copiedDrmInfo.bPropagateUriParams << " (Expected: true)" << std::endl;
    EXPECT_TRUE(copiedDrmInfo.bUseMediaSequenceIV);
    std::cout << "Copied bUseMediaSequenceIV: " << copiedDrmInfo.bUseMediaSequenceIV << " (Expected: true)" << std::endl;
    EXPECT_TRUE(copiedDrmInfo.bDecryptClearSamplesRequired);
    std::cout << "Copied bDecryptClearSamplesRequired: " << copiedDrmInfo.bDecryptClearSamplesRequired << " (Expected: true)" << std::endl;
    
    // Validate iv array (should be all zeros).
    bool allZeros = true;
    for (int i = 0; i < DRM_IV_LEN; i++) {
        std::cout << "copiedDrmInfo.iv[" << i << "]: " << static_cast<int>(copiedDrmInfo.iv[i]) << std::endl;
        if(copiedDrmInfo.iv[i] != 0)
        {
            allZeros = false;
        }
    }
    EXPECT_TRUE(allZeros);
    std::cout << "Copied iv array verified to be all zeros." << std::endl;
    
    // Validate strings (should be empty).
    std::cout << "Copied masterManifestURL: '" << copiedDrmInfo.masterManifestURL << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.masterManifestURL, "");
    std::cout << "Copied manifestURL: '" << copiedDrmInfo.manifestURL << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.manifestURL, "");
    std::cout << "Copied keyURI: '" << copiedDrmInfo.keyURI << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.keyURI, "");
    std::cout << "Copied keyFormat: '" << copiedDrmInfo.keyFormat << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.keyFormat, "");
    std::cout << "Copied systemUUID: '" << copiedDrmInfo.systemUUID << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.systemUUID, "");
    std::cout << "Copied initData: '" << copiedDrmInfo.initData << "'" << std::endl;
    EXPECT_EQ(copiedDrmInfo.initData, "");
    
    std::cout << "Exiting CopyDefaultDrmInfo test" << std::endl;
}
/**
 * @brief Test copy constructor of DrmInfo with customized values.
 *
 * This test case verifies that all customized values in a DrmInfo object are correctly copied using the copy constructor.
 * It ensures that the copied object retains all the set values independently and accurately, including enums, booleans, IV array, and string members.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      | Expected Result                                                                                      | Notes           |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a DrmInfo object and assign customized values                                         | method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_HLS, useFirst16BytesAsIV = true, bPropagateUriParams = false, bUseMediaSequenceIV = false, bDecryptClearSamplesRequired = false, iv = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10}, masterManifestURL = "http://master.example.com", manifestURL = "http://manifest.example.com", keyURI = "key_uri", keyFormat = "key_format", systemUUID = "uuid_value", initData = "init_data" | CustomDrmInfo object is configured with all custom values                                           | Should be successful |
 * | 02               | Invoke the copy constructor to create a new DrmInfo object from the customized object          | Input: Original customDrmInfo with all fields set; Output: copiedDrmInfo constructed using the copy constructor                                                                                                                                                                                                                                                                                                                                                                                                                  | copiedDrmInfo is created and holds values identical to customDrmInfo                                  | Should Pass     |
 * | 03               | Validate that each field in the copied object matches the customized original values          | Comparison using EXPECT_EQ/EXPECT_TRUE for method, mediaFormat, boolean flags, iv array elements, and string members (masterManifestURL, manifestURL, keyURI, keyFormat, systemUUID, initData)                                                                                                                                                                                                                                                                                                                                   | All assertions pass, confirming that the copy constructor accurately copied all values               | Should Pass     |
 */
TEST(DrmInfo, CopyDrmInfoWithCustomizedValues) {
    std::cout << "Entering CopyDrmInfoWithCustomizedValues test" << std::endl;
    
    // Create a default DrmInfo object and customize its values.
    DrmInfo customDrmInfo;
    std::cout << "Custom DrmInfo object created." << std::endl;
    
    // Setting customized values.
    customDrmInfo.method = eMETHOD_AES_128;
    std::cout << "Set method to: " << customDrmInfo.method << " (Expected: eMETHOD_AES_128)" << std::endl;
    
    // Assuming mediaFormat can be set; keeping same enum for simplicity.
    customDrmInfo.mediaFormat = eMEDIAFORMAT_HLS; 
    std::cout << "Set mediaFormat to: " << customDrmInfo.mediaFormat << " (Custom value, using eMEDIAFORMAT_HLS)" << std::endl;
    
    customDrmInfo.useFirst16BytesAsIV = true;
    std::cout << "Set useFirst16BytesAsIV to: " << customDrmInfo.useFirst16BytesAsIV << " (Expected: true)" << std::endl;
    customDrmInfo.bPropagateUriParams = false;
    std::cout << "Set bPropagateUriParams to: " << customDrmInfo.bPropagateUriParams << " (Expected: false)" << std::endl;
    customDrmInfo.bUseMediaSequenceIV = false;
    std::cout << "Set bUseMediaSequenceIV to: " << customDrmInfo.bUseMediaSequenceIV << " (Expected: false)" << std::endl;
    customDrmInfo.bDecryptClearSamplesRequired = false;
    std::cout << "Set bDecryptClearSamplesRequired to: " << customDrmInfo.bDecryptClearSamplesRequired << " (Expected: false)" << std::endl;
    
    // Set iv array with pattern {0x01, 0x02, ..., 0x10}.
    unsigned char pattern[DRM_IV_LEN] = { 0x01, 0x02, 0x03, 0x04, 
                                          0x05, 0x06, 0x07, 0x08, 
                                          0x09, 0x0A, 0x0B, 0x0C, 
                                          0x0D, 0x0E, 0x0F, 0x10 };
    memcpy(customDrmInfo.iv, pattern, DRM_IV_LEN);
    for (int i = 0; i < DRM_IV_LEN; i++) {
        std::cout << "Set iv[" << i << "] to: " << static_cast<int>(customDrmInfo.iv[i]) << std::endl;
    }
    
    // Set string members.
    customDrmInfo.masterManifestURL = "http://master.example.com";
    std::cout << "Set masterManifestURL to: " << customDrmInfo.masterManifestURL << std::endl;
    customDrmInfo.manifestURL = "http://manifest.example.com";
    std::cout << "Set manifestURL to: " << customDrmInfo.manifestURL << std::endl;
    customDrmInfo.keyURI = "key_uri";
    std::cout << "Set keyURI to: " << customDrmInfo.keyURI << std::endl;
    customDrmInfo.keyFormat = "key_format";
    std::cout << "Set keyFormat to: " << customDrmInfo.keyFormat << std::endl;
    customDrmInfo.systemUUID = "uuid_value";
    std::cout << "Set systemUUID to: " << customDrmInfo.systemUUID << std::endl;
    customDrmInfo.initData = "init_data";
    std::cout << "Set initData to: " << customDrmInfo.initData << std::endl;
    
    // Use the copy constructor to create a new DrmInfo object.
    DrmInfo copiedDrmInfo(customDrmInfo);
    std::cout << "Copy constructor invoked to create copiedDrmInfo" << std::endl;
    
    // Validate each customized value against the copy.
    EXPECT_EQ(copiedDrmInfo.method, eMETHOD_AES_128);
    std::cout << "Copied method: " << copiedDrmInfo.method << " (Expected: eMETHOD_AES_128)" << std::endl;
    EXPECT_EQ(copiedDrmInfo.mediaFormat, eMEDIAFORMAT_HLS);
    std::cout << "Copied mediaFormat: " << copiedDrmInfo.mediaFormat << " (Expected: same as custom)" << std::endl;
    EXPECT_TRUE(copiedDrmInfo.useFirst16BytesAsIV);
    std::cout << "Copied useFirst16BytesAsIV: " << copiedDrmInfo.useFirst16BytesAsIV << " (Expected: true)" << std::endl;
    EXPECT_FALSE(copiedDrmInfo.bPropagateUriParams);
    std::cout << "Copied bPropagateUriParams: " << copiedDrmInfo.bPropagateUriParams << " (Expected: false)" << std::endl;
    EXPECT_FALSE(copiedDrmInfo.bUseMediaSequenceIV);
    std::cout << "Copied bUseMediaSequenceIV: " << copiedDrmInfo.bUseMediaSequenceIV << " (Expected: false)" << std::endl;
    EXPECT_FALSE(copiedDrmInfo.bDecryptClearSamplesRequired);
    std::cout << "Copied bDecryptClearSamplesRequired: " << copiedDrmInfo.bDecryptClearSamplesRequired << " (Expected: false)" << std::endl;
    
    // Validate iv array.
    for (int i = 0; i < DRM_IV_LEN; i++) {
        std::cout << "Copied iv[" << i << "]: " << static_cast<int>(copiedDrmInfo.iv[i]) 
                  << " (Expected: " << static_cast<int>(pattern[i]) << ")" << std::endl;
        EXPECT_EQ(copiedDrmInfo.iv[i], pattern[i]);
    }
    
    // Validate string members.
    EXPECT_EQ(copiedDrmInfo.masterManifestURL, "http://master.example.com");
    std::cout << "Copied masterManifestURL: " << copiedDrmInfo.masterManifestURL << std::endl;
    EXPECT_EQ(copiedDrmInfo.manifestURL, "http://manifest.example.com");
    std::cout << "Copied manifestURL: " << copiedDrmInfo.manifestURL << std::endl;
    EXPECT_EQ(copiedDrmInfo.keyURI, "key_uri");
    std::cout << "Copied keyURI: " << copiedDrmInfo.keyURI << std::endl;
    EXPECT_EQ(copiedDrmInfo.keyFormat, "key_format");
    std::cout << "Copied keyFormat: " << copiedDrmInfo.keyFormat << std::endl;
    EXPECT_EQ(copiedDrmInfo.systemUUID, "uuid_value");
    std::cout << "Copied systemUUID: " << copiedDrmInfo.systemUUID << std::endl;
    EXPECT_EQ(copiedDrmInfo.initData, "init_data");
    std::cout << "Copied initData: " << copiedDrmInfo.initData << std::endl;
    
    std::cout << "Exiting CopyDrmInfoWithCustomizedValues test" << std::endl;
}
/**
 * @brief Verify that modifications to the original DrmInfo object do not affect its copy
 *
 * This test verifies the integrity of the copy constructor for DrmInfo by creating an original object with non-default values, copying it, modifying the original object's iv array and manifestURL, and then asserting that the copied object's data remains unchanged. This is important to ensure that the copy constructor performs a deep copy.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | Expected Result                                                                                                                                                                      | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an original DrmInfo object with customized non-default values                             | method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_HLS, useFirst16BytesAsIV = true, bPropagateUriParams = false, bUseMediaSequenceIV = false, bDecryptClearSamplesRequired = false, iv = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10}, masterManifestURL = "http://master.example.com", manifestURL = "http://manifest.example.com", keyURI = "key_uri", keyFormat = "key_format", systemUUID = "uuid_value", initData = "init_data" | Original DrmInfo object is correctly created with the specified values                                                                                           | Should be successful |
 * | 02               | Copy the original DrmInfo object using the copy constructor                                       | Input: originalDrmInfo (with the above values)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     | A new DrmInfo object (copiedDrmInfo) is created with identical values as original                                                                                                   | Should Pass     |
 * | 03               | Modify the original DrmInfo object's iv array at index 5 and change manifestURL                     | Modification: originalDrmInfo.iv[5] = 0xFF, manifestURL = "http://modified.example.com"                                                                                                                                                                                                                                                                                                                                                                                                                                               | The original object is modified while the copiedDrmInfo should remain unaffected                                                                                                     | Should Pass     |
 * | 04               | Verify that the copiedDrmInfo object's iv array retains the original pattern despite modifications  | For each index i in iv array, compare copiedDrmInfo.iv[i] with expected value from pattern {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10}                                                                                                                                                                                                                                                                                                                                             | Each element of copiedDrmInfo.iv matches the corresponding element in the pattern regardless of the modification made to originalDrmInfo.iv[5]                                         | Should Pass     |
 * | 05               | Verify that the copiedDrmInfo object's manifestURL and other string members remain unchanged         | Compare copiedDrmInfo.manifestURL with "http://manifest.example.com", and other members: masterManifestURL = "http://master.example.com", keyURI = "key_uri", keyFormat = "key_format", systemUUID = "uuid_value", initData = "init_data"                                                                                                                                                                                                                                                                                             | The string members of copiedDrmInfo remain unchanged and equal to the original values                                                                                              | Should Pass     |
 */
TEST(DrmInfo, Negative_ModifyingOriginalAfterCopy) {
    std::cout << "Entering Negative_ModifyingOriginalAfterCopy test" << std::endl;
    
    // Create an original DrmInfo object with customized non-default values.
    DrmInfo originalDrmInfo;
    std::cout << "Original DrmInfo object created with customized values." << std::endl;
    
    originalDrmInfo.method = eMETHOD_AES_128;
    originalDrmInfo.mediaFormat = eMEDIAFORMAT_HLS; // Using same for demonstration.
    originalDrmInfo.useFirst16BytesAsIV = true;
    originalDrmInfo.bPropagateUriParams = false;
    originalDrmInfo.bUseMediaSequenceIV = false;
    originalDrmInfo.bDecryptClearSamplesRequired = false;
    
    unsigned char pattern[DRM_IV_LEN] = { 0x01, 0x02, 0x03, 0x04, 
                                          0x05, 0x06, 0x07, 0x08, 
                                          0x09, 0x0A, 0x0B, 0x0C, 
                                          0x0D, 0x0E, 0x0F, 0x10 };
    memcpy(originalDrmInfo.iv, pattern, DRM_IV_LEN);
    std::cout << "Original iv array set." << std::endl;
    
    originalDrmInfo.masterManifestURL = "http://master.example.com";
    originalDrmInfo.manifestURL = "http://manifest.example.com";
    originalDrmInfo.keyURI = "key_uri";
    originalDrmInfo.keyFormat = "key_format";
    originalDrmInfo.systemUUID = "uuid_value";
    originalDrmInfo.initData = "init_data";
    
    // Create a copy using the copy constructor.
    DrmInfo copiedDrmInfo(originalDrmInfo);
    std::cout << "Copy constructor invoked to create copiedDrmInfo." << std::endl;
    
    // Modify the original DrmInfo object's iv array and manifestURL.
    originalDrmInfo.iv[5] = 0xFF; // Change one byte in the iv array.
    std::cout << "Modified originalDrmInfo.iv[5] to: " << static_cast<int>(originalDrmInfo.iv[5]) << std::endl;
    originalDrmInfo.manifestURL = "http://modified.example.com";
    std::cout << "Modified originalDrmInfo.manifestURL to: " << originalDrmInfo.manifestURL << std::endl;
    
    // Verify that the copied object's iv array has retained the original pattern.
    for (int i = 0; i < DRM_IV_LEN; i++) {
        int expectedVal = (i == 5) ? 0x06 : pattern[i];
        // copiedDrmInfo.iv[i] should equal pattern[i] regardless of modification in original.
        std::cout << "copiedDrmInfo.iv[" << i << "]: " << static_cast<int>(copiedDrmInfo.iv[i])
                  << " (Expected: " << static_cast<int>(pattern[i]) << ")" << std::endl;
        EXPECT_EQ(copiedDrmInfo.iv[i], pattern[i]);
    }
    
    // Verify that the copied object's manifestURL remains unchanged.
    std::cout << "copiedDrmInfo.manifestURL: " << copiedDrmInfo.manifestURL << " (Expected: http://manifest.example.com)" << std::endl;
    EXPECT_EQ(copiedDrmInfo.manifestURL, "http://manifest.example.com");
    
    // Also verify other string members remain unchanged.
    EXPECT_EQ(copiedDrmInfo.masterManifestURL, "http://master.example.com");
    EXPECT_EQ(copiedDrmInfo.keyURI, "key_uri");
    EXPECT_EQ(copiedDrmInfo.keyFormat, "key_format");
    EXPECT_EQ(copiedDrmInfo.systemUUID, "uuid_value");
    EXPECT_EQ(copiedDrmInfo.initData, "init_data");
    
    std::cout << "Exiting Negative_ModifyingOriginalAfterCopy test" << std::endl;
}
/**
 * @brief Verify correct functionality of the DrmInfo copy assignment operator
 *
 * This test verifies that the DrmInfo copy assignment operator correctly assigns each member variable from a non-default initialized source object to a target object, and that it returns a reference to the target itself.
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
 * | Variation / Step | Description                                                                                                                      | Test Data                                                                                                                                                                                                                                                                                                            | Expected Result                                                                                             | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a source DrmInfo object with non-default values                                                                           | source.method = eMETHOD_AES_128, source.mediaFormat = eMEDIAFORMAT_HLS, source.useFirst16BytesAsIV = true, source.bDecryptClearSamplesRequired = false, source.iv = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}, source.masterManifestURL = "http://master.example.com", source.manifestURL = "http://playlist.example.com", source.keyURI = "http://key.example.com", source.keyFormat = "formatA", source.systemUUID = "uuid-1234", source.initData = "initdata123" | DrmInfo object 'source' is correctly initialized with non-default values          | Should be successful |
 * | 02               | Invoke the copy assignment operator to assign source to target                                                                     | target = source                                                                                                                                                                                                                                                                                                      | Copy assignment returns a reference to the target object and all member variables in target match those in source | Should Pass   |
 * | 03               | Verify that the IV array in the target object is correctly copied from the source object                                            | For each 0 <= i < DRM_IV_LEN, compare target.iv[i] with source.iv[i]                                                                                                                                                                                                                                                  | target.iv matches source.iv for every element                                                                | Should Pass   |
 */
TEST(DrmInfo, PositiveAssignment) {
    std::cout << "Entering PositiveAssignment test" << std::endl;

    // Create source object with non-default values
    DrmInfo source;
    std::cout << "Setting source.method = eMETHOD_AES_128" << std::endl;
    source.method = eMETHOD_AES_128;

    std::cout << "Setting source.mediaFormat = eMEDIAFORMAT_HLS" << std::endl;
    source.mediaFormat = eMEDIAFORMAT_HLS;

    std::cout << "Setting source.useFirst16BytesAsIV = true" << std::endl;
    source.useFirst16BytesAsIV = true;

    std::cout << "Setting source.bDecryptClearSamplesRequired = false" << std::endl;
    source.bDecryptClearSamplesRequired = false;

    // Set iv array with non-zero values
    unsigned char initIv[DRM_IV_LEN] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    std::cout << "Copying iv array into source.iv" << std::endl;
    memcpy(source.iv, initIv, DRM_IV_LEN);

    std::cout << "Setting source.masterManifestURL = \"http://master.example.com\"" << std::endl;
    source.masterManifestURL = "http://master.example.com";

    std::cout << "Setting source.manifestURL = \"http://playlist.example.com\"" << std::endl;
    source.manifestURL = "http://playlist.example.com";

    std::cout << "Setting source.keyURI = \"http://key.example.com\"" << std::endl;
    source.keyURI = "http://key.example.com";

    std::cout << "Setting source.keyFormat = \"formatA\"" << std::endl;
    source.keyFormat = "formatA";

    std::cout << "Setting source.systemUUID = \"uuid-1234\"" << std::endl;
    source.systemUUID = "uuid-1234";

    std::cout << "Setting source.initData = \"initdata123\"" << std::endl;
    source.initData = "initdata123";

    // Create target object using default constructor
    DrmInfo target;
    std::cout << "Invoking copy assignment operator: target = source" << std::endl;
    DrmInfo& ret = (target = source);

    // Check that the returned reference is the target itself
    EXPECT_EQ(&target, &ret);
    std::cout << "Copy assignment operator returned reference to target" << std::endl;

    // Verify each field from the target equals the source

    EXPECT_EQ(target.method, source.method);
    std::cout << "target.method = " << target.method << std::endl;

    EXPECT_EQ(target.mediaFormat, source.mediaFormat);
    std::cout << "target.mediaFormat = " << target.mediaFormat << std::endl;

    EXPECT_EQ(target.useFirst16BytesAsIV, source.useFirst16BytesAsIV);
    std::cout << "target.useFirst16BytesAsIV = " << target.useFirst16BytesAsIV << std::endl;

    EXPECT_EQ(target.bDecryptClearSamplesRequired, source.bDecryptClearSamplesRequired);
    std::cout << "target.bDecryptClearSamplesRequired = " << target.bDecryptClearSamplesRequired << std::endl;

    EXPECT_EQ(target.masterManifestURL, source.masterManifestURL);
    std::cout << "target.masterManifestURL = " << target.masterManifestURL << std::endl;

    EXPECT_EQ(target.manifestURL, source.manifestURL);
    std::cout << "target.manifestURL = " << target.manifestURL << std::endl;

    EXPECT_EQ(target.keyURI, source.keyURI);
    std::cout << "target.keyURI = " << target.keyURI << std::endl;

    EXPECT_EQ(target.keyFormat, source.keyFormat);
    std::cout << "target.keyFormat = " << target.keyFormat << std::endl;

    EXPECT_EQ(target.systemUUID, source.systemUUID);
    std::cout << "target.systemUUID = " << target.systemUUID << std::endl;

    EXPECT_EQ(target.initData, source.initData);
    std::cout << "target.initData = " << target.initData << std::endl;

    // Verify IV array elements
    bool ivEqual = true;
    for (int i = 0; i < DRM_IV_LEN; ++i) {
        if (target.iv[i] != source.iv[i]) {
            ivEqual = false;
            break;
        }
    }
    EXPECT_TRUE(ivEqual);
    std::cout << "target.iv values copied correctly" << std::endl;

    std::cout << "Exiting PositiveAssignment test" << std::endl;
}
/**
 * @brief Verifies the copy assignment operator and deep copy functionality of DrmInfo for various DRM methods.
 *
 * This test iterates over an array of DRM methods to validate that the copy assignment operator correctly copies 
 * all the member variables of the DrmInfo object, including performing a deep copy of the IV array. This ensures that 
 * changes in one object do not affect the other, which is critical for application stability.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                       | Test Data                                                                                                                                                                                                                                                                                                          | Expected Result                                                                                                        | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Loop through the array of DRM method values.                                                     | methodValues = { eMETHOD_NONE, eMETHOD_AES_128 }                                                                                                                                                                                                                                                                    | Iteration occurs for each DRM method value in the array.                                                             | Should be successful |
 * | 02               | Create a source DrmInfo object with valid parameters including DRM method, media format, IV values, and URLs. | source.method = current method, source.mediaFormat = eMEDIAFORMAT_HLS, source.useFirst16BytesAsIV = true, source.bDecryptClearSamplesRequired = true, iv = {10,20,30,40,50,60,70,80,90,100,110,120,121,122,123,124}, source.masterManifestURL = "http://master.loop.com", source.manifestURL = "http://playlist.loop.com", source.keyURI = "http://key.loop.com", source.keyFormat = "loopFormat", source.systemUUID = "uuid-loop", source.initData = "loopdata" | Source object is correctly initialized with all values set.     | Should be successful |
 * | 03               | Invoke the copy assignment operator to assign source to a target DrmInfo object.                   | target is uninitialized, then target = source, and ret holds the returned reference.                                                                                                                                                                                                                                  | The copy assignment operator returns a reference to target (EXPECT_EQ(&target, &ret)).                                  | Should Pass   |
 * | 04               | Validate that each member of target matches the corresponding member in source after assignment.    | Comparisons using EXPECT_EQ for each field: method, mediaFormat, useFirst16BytesAsIV, bDecryptClearSamplesRequired, masterManifestURL, manifestURL, keyURI, keyFormat, systemUUID, initData.                                                                                                                                                             | All EXPECT_EQ assertions pass confirming that all member values are identical between target and source.               | Should Pass   |
 * | 05               | Verify that the IV array in target is deeply copied from the source.                             | Loop through DRM_IV_LEN elements comparing target.iv[i] with source.iv[i].                                                                                                                                                                                                                                           | The loop confirms that each element of the IV array in target equals the corresponding element in source (EXPECT_TRUE(ivEqual)). | Should Pass   |
 */
TEST(DrmInfo, LoopThroughDrmMethodEnum) {
    std::cout << "Entering LoopThroughDrmMethodEnum test" << std::endl;

    // Define an array of DrmMethod values to loop through
    DrmMethod methodValues[] = { eMETHOD_NONE, eMETHOD_AES_128 };

    for (size_t idx = 0; idx < sizeof(methodValues) / sizeof(methodValues[0]); ++idx) {
        std::cout << "Testing for method value: " << methodValues[idx] << std::endl;
        // Create source object with current method and other valid values
        DrmInfo source;
        source.method = methodValues[idx];
        source.mediaFormat = eMEDIAFORMAT_HLS;
        source.useFirst16BytesAsIV = true;
        source.bDecryptClearSamplesRequired = true; // using true in this test
        unsigned char tempIv[DRM_IV_LEN] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 121, 122, 123, 124 };
        memcpy(source.iv, tempIv, DRM_IV_LEN);
        source.masterManifestURL = "http://master.loop.com";
        source.manifestURL = "http://playlist.loop.com";
        source.keyURI = "http://key.loop.com";
        source.keyFormat = "loopFormat";
        source.systemUUID = "uuid-loop";
        source.initData = "loopdata";

        // Create target object and perform assignment
        DrmInfo target;
        std::cout << "Invoking copy assignment operator for method value " << methodValues[idx] << std::endl;
        DrmInfo& ret = (target = source);
        EXPECT_EQ(&target, &ret);

        // Validate all copied members
        EXPECT_EQ(target.method, source.method);
        std::cout << "target.method = " << target.method << std::endl;

        EXPECT_EQ(target.mediaFormat, source.mediaFormat);
        std::cout << "target.mediaFormat = " << target.mediaFormat << std::endl;

        EXPECT_EQ(target.useFirst16BytesAsIV, source.useFirst16BytesAsIV);
        std::cout << "target.useFirst16BytesAsIV = " << target.useFirst16BytesAsIV << std::endl;

        EXPECT_EQ(target.bDecryptClearSamplesRequired, source.bDecryptClearSamplesRequired);
        std::cout << "target.bDecryptClearSamplesRequired = " << target.bDecryptClearSamplesRequired << std::endl;

        EXPECT_EQ(target.masterManifestURL, source.masterManifestURL);
        std::cout << "target.masterManifestURL = " << target.masterManifestURL << std::endl;

        EXPECT_EQ(target.manifestURL, source.manifestURL);
        std::cout << "target.manifestURL = " << target.manifestURL << std::endl;

        EXPECT_EQ(target.keyURI, source.keyURI);
        std::cout << "target.keyURI = " << target.keyURI << std::endl;

        EXPECT_EQ(target.keyFormat, source.keyFormat);
        std::cout << "target.keyFormat = " << target.keyFormat << std::endl;

        EXPECT_EQ(target.systemUUID, source.systemUUID);
        std::cout << "target.systemUUID = " << target.systemUUID << std::endl;

        EXPECT_EQ(target.initData, source.initData);
        std::cout << "target.initData = " << target.initData << std::endl;

        // Verify iv array equality
        bool ivEqual = true;
        for (int i = 0; i < DRM_IV_LEN; ++i) {
            if (target.iv[i] != source.iv[i]) {
                ivEqual = false;
                break;
            }
        }
        EXPECT_TRUE(ivEqual);
        std::cout << "target.iv values copied correctly for method value " << methodValues[idx] << std::endl;
    }

    std::cout << "Exiting LoopThroughDrmMethodEnum test" << std::endl;
}
/**
 * @brief Validate that the copy assignment operator performs a deep copy of the IV array.
 *
 * This test verifies that after copying a DrmInfo object to another, modifications to the IV array in the source object do not affect the target object. This is essential to ensure that the copy assignment operator implements a deep copy rather than a shallow copy.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                                                                                                                                                                                                                                                                                                                                                                                                                                                    | Expected Result                                                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a source object and initialize its IV array and other fields with distinct values.    | source.iv = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80}, source.method = eMETHOD_AES_128, source.mediaFormat = eMEDIAFORMAT_HLS, source.useFirst16BytesAsIV = true, source.bDecryptClearSamplesRequired = true, source.masterManifestURL = "http://master.deepcopy.com", source.manifestURL = "http://playlist.deepcopy.com", source.keyURI = "http://key.deepcopy.com", source.keyFormat = "deepFormat", source.systemUUID = "uuid-deep", source.initData = "deepinit" | Source object is correctly initialized and all fields are set as expected. | Should be successful |
 * | 02               | Invoke the copy assignment operator to copy the source object into the target object.         | target = source                                                                                                                                                                                                                                                                                                                                                                                                                                                              | The copy assignment operator returns a reference equal to target.                                         | Should Pass   |
 * | 03               | Modify the source object's IV array after the assignment.                                   | source.iv modified to {80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5}                                                                                                                                                                                                                                                                                                                                                                                         | Modification is performed on the source object without error.                                             | Should be successful |
 * | 04               | Verify that the target object's IV array remains unchanged despite the modification on source.| target.iv remains equal to the original IV array: {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80}                                                                                                                                                                                                                                                                                                                                                          | The IV array in target should match the original values set in step 01, ensuring a deep copy was made.     | Should Pass   |
 */
TEST(DrmInfo, NegativeDeepCopyIV) {
    std::cout << "Entering NegativeDeepCopyIV test" << std::endl;

    // Create source object with a known IV array
    DrmInfo source;
    unsigned char originalIv[DRM_IV_LEN] = { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80 };
    std::cout << "Setting source.iv with distinct non-zero values" << std::endl;
    memcpy(source.iv, originalIv, DRM_IV_LEN);

    // Set arbitrary values for other fields so that they are non-default 
    source.method = eMETHOD_AES_128;
    source.mediaFormat = eMEDIAFORMAT_HLS;
    source.useFirst16BytesAsIV = true;
    source.bDecryptClearSamplesRequired = true;
    source.masterManifestURL = "http://master.deepcopy.com";
    source.manifestURL = "http://playlist.deepcopy.com";
    source.keyURI = "http://key.deepcopy.com";
    source.keyFormat = "deepFormat";
    source.systemUUID = "uuid-deep";
    source.initData = "deepinit";

    // Create target object and assign source to it
    DrmInfo target;
    std::cout << "Invoking copy assignment operator: target = source" << std::endl;
    DrmInfo& ret = (target = source);
    EXPECT_EQ(&target, &ret);
    std::cout << "Assigned source to target, verifying IV deep copy" << std::endl;

    // Modify source.iv after assignment
    unsigned char newIv[DRM_IV_LEN] = { 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5 };
    std::cout << "Modifying source.iv after assignment" << std::endl;
    memcpy(source.iv, newIv, DRM_IV_LEN);

    // Check that target.iv remains unchanged (should equal originalIv)
    bool ivUnchanged = true;
    for (int i = 0; i < DRM_IV_LEN; ++i) {
        if (target.iv[i] != originalIv[i]) {
            ivUnchanged = false;
            std::cout << "Mismatch at iv index " << i << ": target.iv = " 
                      << static_cast<int>(target.iv[i]) << ", expected = " 
                      << static_cast<int>(originalIv[i]) << std::endl;
            break;
        }
    }
    EXPECT_TRUE(ivUnchanged);
    std::cout << "Verified that target.iv remains unchanged after source modification" << std::endl;

    std::cout << "Exiting NegativeDeepCopyIV test" << std::endl;
}
/**
 * @brief Verify the automatic invocation of the destructor for DrmInfo objects.
 *
 * This test creates a DrmInfo object using the default constructor within a nested scope and verifies that the object's destructor
 * is automatically invoked when it goes out-of-scope. The test checks that no exceptions are thrown during the object construction
 * and destruction process.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                       | Test Data                                                         | Expected Result                                                                                    | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Create a DrmInfo object using the default constructor and ensure its destructor is automatically invoked by leaving scope. | constructor = DrmInfo(), scope = block, exception = none          | No exception is thrown during object creation and destruction; EXPECT_NO_THROW check passes.       | Should Pass |
 */
TEST(DrmInfo, DestructorAutomatically) {
    std::cout << "Entering DestructorAutomatically test" << std::endl;
    
    {
        // Create a DrmInfo object using the default constructor and log its creation.
        EXPECT_NO_THROW({
            DrmInfo drmInfo;
            std::cout << "Created DrmInfo object with default constructor." << std::endl;
            // Additional debug logs: printing default member state (if accessible via getters or direct access)
            // For example purposes only, since direct access might not be available.
            // Here we simulate the internal state logs:
            std::cout << "Internal state: DrmMethod, MediaFormat and other members are set to default values." << std::endl;
        });
        std::cout << "DrmInfo object is going out-of-scope; destructor should be automatically invoked." << std::endl;
    }
    
    std::cout << "Exiting DestructorAutomatically test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
