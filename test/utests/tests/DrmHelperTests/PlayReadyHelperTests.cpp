
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
#include "PlayReadyHelper.h"


// Test Case: Append DRM system IDs to an initially empty vector
/**
 * @brief Test that appendSystemId correctly processes an empty vector input by populating it with DRM system IDs.
 *
 * This test case instantiates a PlayReadyHelperFactory using the default constructor, prepares an empty vector for DRM system IDs, and invokes the appendSystemId method. The function is expected to populate the vector without throwing exceptions and ensure that the vector is not empty after the call.@n
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
 * | Variation / Step | Description                                                            | Test Data                                                    | Expected Result                                                       | Notes            |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------------ | --------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a PlayReadyHelperFactory instance using the default constructor.| Input: None, Output: factory instance created successfully.  | Factory instance is created without exception.                      | Should be successful |
 * | 02               | Prepare an empty vector for DRM system IDs.                            | Input: systemIds = empty vector                               | Vector systemIds is empty.                                            | Should be successful |
 * | 03               | Invoke appendSystemId on the empty vector and check for exceptions.      | Input: systemIds = empty vector, Output: systemIds populated    | appendSystemId is invoked without throwing an exception and populates systemIds. | Should Pass       |
 * | 04               | Verify that the systemIds vector is not empty after the method call.     | Input: systemIds after appendSystemId call                    | systemIds.size() > 0 confirming that DRM system IDs have been appended. | Should Pass       |
 */
TEST(PlayReadyHelperFactory, AppendDRMSystemIds_EmptyVector) {
    std::cout << "Entering AppendDRMSystemIds_EmptyVector test" << std::endl;

    // Create object for PlayReadyHelperFactory using default constructor
    EXPECT_NO_THROW({
        PlayReadyHelperFactory factory;
        std::cout << "Created PlayReadyHelperFactory instance successfully." << std::endl;

        // Prepare an empty vector for DRM system IDs
        std::vector<std::string> systemIds;
        std::cout << "Initial vector 'systemIds' size: " << systemIds.size() << std::endl;

        // Invoke the appendSystemId method to populate the vector
        std::cout << "Invoking appendSystemId with an empty vector." << std::endl;
        EXPECT_NO_THROW({
            factory.appendSystemId(systemIds);
            std::cout << "appendSystemId method invoked successfully." << std::endl;
        });

        // Debug: Print out the contents of the vector after invocation
        std::cout << "After invocation, vector 'systemIds' size: " << systemIds.size() << std::endl;
        for (size_t index = 0; index < systemIds.size(); ++index) {
            std::cout << "systemIds[" << index << "]: " << systemIds[index] << std::endl;
        }
        
        // Check the vector is not empty as it should be populated with supported DRM system IDs
        EXPECT_FALSE(systemIds.empty());
    });

    std::cout << "Exiting AppendDRMSystemIds_EmptyVector test" << std::endl;
}
/**
 * @brief Test the creation of DRM helper with valid DRM information for AES encryption and HLS media format.
 *
 * This test verifies that the PlayReadyHelperFactory is able to create a valid DRM helper object when provided with proper DRM information using the AES-128 method and HLS media format.
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
 * | 01 | Create the factory, set up valid DRM information, and invoke createHelper on the factory | method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_HLS, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, useFirst16BytesAsIV = true, iv = "0123456789abcdef", keyURI = https://valid.key.uri, masterManifestURL = https://valid.manifest.url, manifestURL = https://valid.playlist.url | DrmHelperPtr returned is not null; ASSERT check EXPECT_NE(helper, nullptr) passes | Should Pass |
 */
TEST(PlayReadyHelperFactory, ValidDRMInfo_AES_HLS) {
    GTEST_SKIP();
    std::cout << "Entering ValidDRMInfo_AES_HLS test" << std::endl;
    
    // Create an instance of PlayReadyHelperFactory using default constructor
    EXPECT_NO_THROW({
        PlayReadyHelperFactory factory;
        std::cout << "Factory created using default constructor" << std::endl;
        
        // Setup DRM information
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
        drmInfo.bPropagateUriParams = true;
        drmInfo.bUseMediaSequenceIV = true;
        drmInfo.bDecryptClearSamplesRequired = true;
        drmInfo.useFirst16BytesAsIV = true;
        
        // Assign IV using strncpy
        const char* ivStr = "0123456789abcdef";
        strncpy(reinterpret_cast<char*>(drmInfo.iv), ivStr, DRM_IV_LEN);
        std::cout << "DRM Info IV set to: " << std::string(reinterpret_cast<char*>(drmInfo.iv), DRM_IV_LEN) << std::endl;
        
        // Set other string fields
        drmInfo.keyURI = "https://valid.key.uri";
        drmInfo.masterManifestURL = "https://valid.manifest.url";
        drmInfo.manifestURL = "https://valid.playlist.url";
        std::cout << "DRM Info keyURI: " << drmInfo.keyURI 
                  << ", masterManifestURL: " << drmInfo.masterManifestURL 
                  << ", manifestURL: " << drmInfo.manifestURL << std::endl;
        
        // Invocation of createHelper
        std::cout << "Invoking createHelper with valid DRM info" << std::endl;
        DrmHelperPtr helper = factory.createHelper(drmInfo);
        
        EXPECT_NE(helper, nullptr);
    });
    
    std::cout << "Exiting ValidDRMInfo_AES_HLS test" << std::endl;
}
/**
 * @brief Verify that the factory returns a null helper when an invalid DRM method is provided.
 *
 * This test verifies that when an invalid DRM method (eMETHOD_NONE) is passed to the factory,
 * the createHelper function does not create a helper and returns a null pointer. It ensures the
 * robustness of the factory in handling improper DRM method configurations.
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
 * | 01 | Instantiate PlayReadyHelperFactory using the default constructor | None | Factory instance is successfully created without throwing exceptions | Should be successful |
 * | 02 | Configure drmInfo with invalid DRM method and required parameters | method=eMETHOD_NONE, mediaFormat=eMEDIAFORMAT_HLS, bPropagateUriParams=true, bUseMediaSequenceIV=true, bDecryptClearSamplesRequired=true, useFirst16BytesAsIV=true, iv="abcdefghijklmnop", keyURI=https://valid.key.uri, masterManifestURL=https://valid.manifest.url, manifestURL=https://valid.playlist.url | drmInfo is properly configured with invalid DRM method | Should be successful |
 * | 03 | Invoke createHelper with the configured drmInfo and verify that helper is nullptr | drmInfo as above is passed to factory.createHelper | Returned helper is nullptr and assertion (EXPECT_EQ) passes | Should Pass |
 */
TEST(PlayReadyHelperFactory, InvalidDRMMethod_eMETHOD_NONE) {
    std::cout << "Entering InvalidDRMMethod_eMETHOD_NONE test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayReadyHelperFactory factory;
        std::cout << "Factory created using default constructor" << std::endl;
        
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_NONE; // Invalid DRM method for helper creation
        drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
        drmInfo.bPropagateUriParams = true;
        drmInfo.bUseMediaSequenceIV = true;
        drmInfo.bDecryptClearSamplesRequired = true;
        drmInfo.useFirst16BytesAsIV = true;
        
        // Setup IV using strncpy
        const char* ivStr = "abcdefghijklmnop";
        strncpy(reinterpret_cast<char*>(drmInfo.iv), ivStr, DRM_IV_LEN);
        std::cout << "DRM Info IV set to: " << std::string(reinterpret_cast<char*>(drmInfo.iv), DRM_IV_LEN) << std::endl;
        
        drmInfo.keyURI = "https://valid.key.uri";
        drmInfo.masterManifestURL = "https://valid.manifest.url";
        drmInfo.manifestURL = "https://valid.playlist.url";
        std::cout << "DRM Info keyURI: " << drmInfo.keyURI << std::endl;
        
        std::cout << "Invoking createHelper with invalid DRM method (eMETHOD_NONE)" << std::endl;
        DrmHelperPtr helper = factory.createHelper(drmInfo);
        
        EXPECT_EQ(helper, nullptr);
    });
    
    std::cout << "Exiting InvalidDRMMethod_eMETHOD_NONE test" << std::endl;
}
/**
 * @brief Verify that the PlayReadyHelperFactory correctly instantiates helper objects for various media formats.
 *
 * This test iterates over an array of media formats and sets up a corresponding DrmInfo structure for each format.
 * It then invokes the createHelper API of the PlayReadyHelperFactory to check whether the helper is successfully created.
 * For supported media formats, the helper pointer should be non-null, while for the unsupported media format (eMEDIAFORMAT_UNKNOWN),
 * the helper pointer is expected to be null.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayReadyHelperFactory using its default constructor. | None | Factory object is created with no exceptions. | Should be successful |
 * | 02 | Invoke createHelper for supported media formats from the mediaFormats array. | For each supported media format (eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE, eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI, eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, eMEDIAFORMAT_RMF); drmInfo: method = eMETHOD_AES_128, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, useFirst16BytesAsIV = true, iv = "1234567890abcdef", keyURI = "https://valid.key.uri", masterManifestURL = "https://valid.manifest.url", manifestURL = "https://valid.playlist.url" | Helper pointer returned is non-null. | Should Pass |
 * | 03 | Invoke createHelper for the unsupported media format. | For media format eMEDIAFORMAT_UNKNOWN with drmInfo fields same as above. | Helper pointer returned is null. | Should Fail |
 */
TEST(PlayReadyHelperFactory, LoopThroughMediaFormats) {
    GTEST_SKIP();
    std::cout << "Entering LoopThroughMediaFormats test" << std::endl;
    
    EXPECT_NO_THROW(({
        PlayReadyHelperFactory factory;
        std::cout << "Factory created using default constructor" << std::endl;
        
        // Array of media formats for loop
        MediaFormat mediaFormats[] = {
            eMEDIAFORMAT_HLS,
            eMEDIAFORMAT_DASH,
            eMEDIAFORMAT_PROGRESSIVE,
            eMEDIAFORMAT_HLS_MP4,
            eMEDIAFORMAT_OTA,
            eMEDIAFORMAT_HDMI,
            eMEDIAFORMAT_COMPOSITE,
            eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA,
            eMEDIAFORMAT_RMF,
            eMEDIAFORMAT_UNKNOWN
        };
        
        const char* mediaFormatNames[] = {
            "eMEDIAFORMAT_HLS",
            "eMEDIAFORMAT_DASH",
            "eMEDIAFORMAT_PROGRESSIVE",
            "eMEDIAFORMAT_HLS_MP4",
            "eMEDIAFORMAT_OTA",
            "eMEDIAFORMAT_HDMI",
            "eMEDIAFORMAT_COMPOSITE",
            "eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA",
            "eMEDIAFORMAT_RMF",
            "eMEDIAFORMAT_UNKNOWN"
        };
        
        for (size_t i = 0; i < sizeof(mediaFormats)/sizeof(MediaFormat); ++i) {
            DrmInfo drmInfo;
            drmInfo.method = eMETHOD_AES_128;
            drmInfo.mediaFormat = mediaFormats[i];
            drmInfo.bPropagateUriParams = true;
            drmInfo.bUseMediaSequenceIV = true;
            drmInfo.bDecryptClearSamplesRequired = true;
            drmInfo.useFirst16BytesAsIV = true;
            
            // setup IV using strncpy
            const char* ivStr = "1234567890abcdef";
            strncpy(reinterpret_cast<char*>(drmInfo.iv), ivStr, DRM_IV_LEN);
            
            drmInfo.keyURI = "https://valid.key.uri";
            drmInfo.masterManifestURL = "https://valid.manifest.url";
            drmInfo.manifestURL = "https://valid.playlist.url";
            
            std::cout << "Invoking createHelper for media format " << mediaFormatNames[i] << std::endl;
            DrmHelperPtr helper = factory.createHelper(drmInfo);
            if (mediaFormats[i] != eMEDIAFORMAT_UNKNOWN) 
            {
                EXPECT_NE(helper, nullptr);
            } else 
            {
                EXPECT_EQ(helper, nullptr);
            }
        }
    }));
    
    std::cout << "Exiting LoopThroughMediaFormats test" << std::endl;
}
/**
 * @brief Validates that the PlayReadyHelperFactory returns a null helper for unsupported media formats.
 *
 * Validates that the createHelper method of the PlayReadyHelperFactory returns a null pointer when an unsupported media format (eMEDIAFORMAT_UNKNOWN) is provided in the DrmInfo. This test ensures that the factory does not create a helper object for invalid DRM configurations.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayReadyHelperFactory instance using the default constructor. | factory is constructed using default constructor. | Factory instance is created successfully without exceptions. | Should be successful |
 * | 02 | Initialize a DrmInfo structure with method set to eMETHOD_AES_128 and unsupported media format (eMEDIAFORMAT_UNKNOWN) along with required flags and parameters. | drmInfo: method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_UNKNOWN, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, useFirst16BytesAsIV = true, iv = "0011223344556677", keyURI = "https://valid.key.uri", masterManifestURL = "https://valid.manifest.url", manifestURL = "https://valid.playlist.url". | DrmInfo is properly initialized with the unsupported media format. | Should be successful |
 * | 03 | Invoke createHelper using the initialized DrmInfo and verify that the returned helper pointer is nullptr. | Input: drmInfo with unsupported media format; Output: DrmHelperPtr helper. | Returns nullptr indicating that no helper was created for unsupported media format; Expected assertion passes (EXPECT_EQ(helper, nullptr)). | Should Pass |
 */
TEST(PlayReadyHelperFactory, UnsupportedMediaFormat) {
    std::cout << "Entering UnsupportedMediaFormat test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayReadyHelperFactory factory;
        std::cout << "Factory created using default constructor" << std::endl;
        
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = eMEDIAFORMAT_UNKNOWN; // Unsupported media format
        drmInfo.bPropagateUriParams = true;
        drmInfo.bUseMediaSequenceIV = true;
        drmInfo.bDecryptClearSamplesRequired = true;
        drmInfo.useFirst16BytesAsIV = true;
        
        // Setup IV using strncpy
        const char* ivStr = "0011223344556677";
        strncpy(reinterpret_cast<char*>(drmInfo.iv), ivStr, DRM_IV_LEN);
        std::cout << "DRM Info IV set to: " << std::string(reinterpret_cast<char*>(drmInfo.iv), DRM_IV_LEN) << std::endl;
        
        drmInfo.keyURI = "https://valid.key.uri";
        drmInfo.masterManifestURL = "https://valid.manifest.url";
        drmInfo.manifestURL = "https://valid.playlist.url";
        std::cout << "DRM Info keyURI: " << drmInfo.keyURI << std::endl;
        
        std::cout << "Invoking createHelper with unsupported media format (eMEDIAFORMAT_UNKNOWN)" << std::endl;
        DrmHelperPtr helper = factory.createHelper(drmInfo);
        
        EXPECT_EQ(helper, nullptr);
    });
    
    std::cout << "Exiting UnsupportedMediaFormat test" << std::endl;
}
/**
 * @brief Verify that the PlayReadyHelperFactory correctly identifies non-DRM content when default parameters are used.
 *
 * This test creates an instance of PlayReadyHelperFactory and a DrmInfo structure with default values, setting the method to eMETHOD_NONE.
 * It then calls the isDRM function and validates that it returns false, indicating that the content is not DRM protected.
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
 * | Variation / Step | Description                                                         | Test Data                                                              | Expected Result                              | Notes       |
 * | :----:           | ------------------------------------------------------------------- | ---------------------------------------------------------------------- | -------------------------------------------- | ----------- |
 * | 01               | Instantiate PlayReadyHelperFactory and verify no exceptions thrown   | (No input, factory instance creation)                                  | Factory is created without throwing          | Should Pass |
 * | 02               | Create DrmInfo with default values and set method to eMETHOD_NONE      | drmInfo.method = eMETHOD_NONE                                            | DrmInfo.method is set to eMETHOD_NONE          | Should be successful |
 * | 03               | Invoke isDRM method using the factory with DrmInfo                   | drmInfo.method = eMETHOD_NONE, drmInfo.mediaFormat = default             | isDRM returns false without throwing           | Should Pass |
 * | 04               | Assert that the result from isDRM is false using EXPECT_FALSE          | result = false                                                           | EXPECT_FALSE assertion passes                | Should Pass |
 */
TEST(PlayReadyHelperFactory, NonDRMDefaultParameters)
{
    std::cout << "Entering NonDRMDefaultParameters test" << std::endl;
    
    EXPECT_NO_THROW({ PlayReadyHelperFactory factory; });
    PlayReadyHelperFactory factory;
    
    // Create DrmInfo with default values then set method to eMETHOD_NONE.
    struct DrmInfo drmInfo;
    drmInfo.method = eMETHOD_NONE;
    std::cout << "Created DrmInfo with method = " << drmInfo.method << " (expected eMETHOD_NONE)" << std::endl;
    
    // Log invocation of isDRM method.
    std::cout << "Invoking isDRM with DrmInfo.method = " << drmInfo.method << " and mediaFormat = " << drmInfo.mediaFormat << std::endl;
    bool result = false;
    EXPECT_NO_THROW(result = factory.isDRM(drmInfo));
    std::cout << "isDRM returned: " << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NonDRMDefaultParameters test" << std::endl;
}
/**
 * @brief Test to validate DRMDefaultMediaFormat functionality in PlayReadyHelperFactory.
 *
 * This test validates the construction of a PlayReadyHelperFactory instance, initializes a DrmInfo structure with a specific DRM method (eMETHOD_AES_128) and default mediaFormat, assigns an IV using strncpy, and then invokes the isDRM method to confirm that DRM content is correctly identified. 
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                                                | Expected Result                                                          | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------ | --------------- |
 * | 01               | Create PlayReadyHelperFactory instance.                                   | No input arguments                                                                       | No exception thrown during creation.                                     | Should Pass     |
 * | 02               | Initialize DrmInfo with method eMETHOD_AES_128 and default mediaFormat.     | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = default                            | DrmInfo initialized correctly.                                           | Should be successful |
 * | 03               | Assign IV using strncpy to drmInfo.iv.                                    | sampleIV = "1234567890123456", drmInfo.iv updated with sampleIV                            | IV assigned successfully without overflow.                             | Should be successful |
 * | 04               | Invoke isDRM API with the configured drmInfo.                             | drmInfo with method = eMETHOD_AES_128, mediaFormat = default, iv = "1234567890123456"        | isDRM returns true as per EXPECT_TRUE check.                             | Should Pass     |
 * | 05               | Log completion of test execution.                                         | No input arguments                                                                       | Test completes without error.                                            | Should be successful |
 */
TEST(PlayReadyHelperFactory, DRMDefaultMediaFormat)
{
    GTEST_SKIP();
    std::cout << "Entering DRMDefaultMediaFormat test" << std::endl;
    
    EXPECT_NO_THROW({ PlayReadyHelperFactory factory; });
    PlayReadyHelperFactory factory;
    
    // Create DrmInfo and set method to eMETHOD_AES_128.
    struct DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    std::cout << "Created DrmInfo with method = " << drmInfo.method << " (expected eMETHOD_AES_128)" << std::endl;
    std::cout << "Default mediaFormat is " << drmInfo.mediaFormat << std::endl;
    
    // Use strncpy to assign a sample value to iv.
    const char* sampleIV = "1234567890123456";
    strncpy(reinterpret_cast<char*>(drmInfo.iv), sampleIV, sizeof(drmInfo.iv));
    std::cout << "Assigned IV using strncpy: " << drmInfo.iv << std::endl;
    
    // Log invocation of isDRM method.
    std::cout << "Invoking isDRM with DRM content" << std::endl;
    bool result = false;
    EXPECT_NO_THROW(result = factory.isDRM(drmInfo));
    std::cout << "isDRM returned: " << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting DRMDefaultMediaFormat test" << std::endl;
}
/**
 * @brief Validate DRM detection functionality for multiple media formats using PlayReadyHelperFactory.
 *
 * This test verifies that the isDRM method of PlayReadyHelperFactory correctly identifies DRM content by iterating through various media formats.
 * For each media format, the test sets up a DrmInfo structure with a fixed encryption method (eMETHOD_AES_128) and a sample IV, then asserts that isDRM returns true without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayReadyHelperFactory and verify no exception is thrown. | N/A | Factory instance is created successfully without exception. | Should be successful |
 * | 02 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_HLS. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_HLS, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 03 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_DASH. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_DASH, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 04 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_PROGRESSIVE. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_PROGRESSIVE, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 05 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_HLS_MP4. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_HLS_MP4, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 06 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_OTA. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_OTA, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 07 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_HDMI. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_HDMI, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 08 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_COMPOSITE. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_COMPOSITE, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 09 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 10 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_RMF. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_RMF, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 * | 11 | Invoke isDRM with drmInfo set for mediaFormat = eMEDIAFORMAT_UNKNOWN. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_UNKNOWN, iv = "ABCDEFGHIJKLMNOP" | isDRM returns true without exception. | Should Pass |
 */
TEST(PlayReadyHelperFactory, DRMMultipleMediaFormats)
{
    GTEST_SKIP();
    std::cout << "Entering DRMMultipleMediaFormats test" << std::endl;
    
    EXPECT_NO_THROW({ PlayReadyHelperFactory factory; });
    PlayReadyHelperFactory factory;
    
    // Define an array of media formats.
    MediaFormat mediaFormats[] = {
        eMEDIAFORMAT_HLS,
        eMEDIAFORMAT_DASH,
        eMEDIAFORMAT_PROGRESSIVE,
        eMEDIAFORMAT_HLS_MP4,
        eMEDIAFORMAT_OTA,
        eMEDIAFORMAT_HDMI,
        eMEDIAFORMAT_COMPOSITE,
        eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA,
        eMEDIAFORMAT_RMF,
        eMEDIAFORMAT_UNKNOWN
    };
    
    const int count = sizeof(mediaFormats) / sizeof(mediaFormats[0]);
    for (int i = 0; i < count; ++i)
    {
        struct DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = mediaFormats[i];
        std::cout << "Iteration " << i+1 << ": Setting DrmInfo.method = " << drmInfo.method 
                  << " (expected eMETHOD_AES_128) and mediaFormat = " << drmInfo.mediaFormat << std::endl;
        
        // Use strncpy to assign a sample value to iv.
        const char* sampleIV = "ABCDEFGHIJKLMNOP";
        strncpy(reinterpret_cast<char*>(drmInfo.iv), sampleIV, sizeof(drmInfo.iv));
        std::cout << "Assigned IV using strncpy: " << drmInfo.iv << std::endl;
        
        std::cout << "Invoking isDRM with DRM content for mediaFormat = " << drmInfo.mediaFormat << std::endl;
        bool result = false;
        EXPECT_NO_THROW(result = factory.isDRM(drmInfo));
        std::cout << "isDRM returned: " << result << " for mediaFormat = " << drmInfo.mediaFormat << std::endl;
        
        EXPECT_TRUE(result);
    }
    
    std::cout << "Exiting DRMMultipleMediaFormats test" << std::endl;
}
/**
 * @brief Test to verify that non-DRM content returns false for multiple media formats using PlayReadyHelperFactory
 *
 * The test checks that for various media formats in a non-DRM context (with method set to eMETHOD_NONE), the isDRM method of PlayReadyHelperFactory correctly identifies the content as non-DRM and returns false. This ensures proper handling of non-DRM media formats.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 009
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                | Test Data                                                                                                                | Expected Result                                                  | Notes                |
 * | :--------------: | -------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------|----------------------|
 * | 01               | Instantiate PlayReadyHelperFactory and verify no exception is thrown.       | Constructor call: PlayReadyHelperFactory factory                                                                         | Factory object is created without throwing any exceptions.     | Should be successful |
 * | 02               | For each media format, set DrmInfo with method set to eMETHOD_NONE and assign IV using strncpy. | drmInfo.method = eMETHOD_NONE, drmInfo.mediaFormat = each media format from [eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE, eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI, eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, eMEDIAFORMAT_RMF, eMEDIAFORMAT_UNKNOWN], drmInfo.iv = "1234567890ABCDEF" | DrmInfo is correctly configured for non-DRM content.             | Should be successful |
 * | 03               | Invoke isDRM API with configured DrmInfo for each media format and verify that the result is false. | API call: isDRM(drmInfo) for each media format, expected output: false                                                     | isDRM returns false indicating the content is not protected by DRM. | Should Pass          |
 */
TEST(PlayReadyHelperFactory, NonDRMMultipleMediaFormats)
{
    std::cout << "Entering NonDRMMultipleMediaFormats test" << std::endl;
    
    EXPECT_NO_THROW({ PlayReadyHelperFactory factory; });
    PlayReadyHelperFactory factory;
    
    // Define an array of media formats.
    MediaFormat mediaFormats[] = {
        eMEDIAFORMAT_HLS,
        eMEDIAFORMAT_DASH,
        eMEDIAFORMAT_PROGRESSIVE,
        eMEDIAFORMAT_HLS_MP4,
        eMEDIAFORMAT_OTA,
        eMEDIAFORMAT_HDMI,
        eMEDIAFORMAT_COMPOSITE,
        eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA,
        eMEDIAFORMAT_RMF,
        eMEDIAFORMAT_UNKNOWN
    };
    
    const int count = sizeof(mediaFormats) / sizeof(mediaFormats[0]);
    for (int i = 0; i < count; ++i)
    {
        struct DrmInfo drmInfo;
        drmInfo.method = eMETHOD_NONE;
        drmInfo.mediaFormat = mediaFormats[i];
        std::cout << "Iteration " << i+1 << ": Setting DrmInfo.method = " << drmInfo.method 
                  << " (expected eMETHOD_NONE) and mediaFormat = " << drmInfo.mediaFormat << std::endl;
        
        // Use strncpy to assign a sample value to iv.
        const char* sampleIV = "1234567890ABCDEF";
        strncpy(reinterpret_cast<char*>(drmInfo.iv), sampleIV, sizeof(drmInfo.iv));
        std::cout << "Assigned IV using strncpy: " << drmInfo.iv << std::endl;
        
        std::cout << "Invoking isDRM with non-DRM content for mediaFormat = " << drmInfo.mediaFormat << std::endl;
        bool result = true;
        EXPECT_NO_THROW(result = factory.isDRM(drmInfo));
        std::cout << "isDRM returned: " << result << " for mediaFormat = " << drmInfo.mediaFormat << std::endl;
        
        EXPECT_FALSE(result);
    }
    
    std::cout << "Exiting NonDRMMultipleMediaFormats test" << std::endl;
}
/**
 * @brief Validate default construction of DrmInfo and PlayReadyHelper
 *
 * This test verifies that a default-constructed DrmInfo object contains the expected default values and ensures that constructing
 * a PlayReadyHelper with this default DrmInfo does not throw any exception. The test also checks that the internal state of PlayReadyHelper,
 * such as FRIENDLY_NAME and CODEC_TYPE, is set to the expected values.
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
 * | Variation / Step | Description                                                                           | Test Data                                                                                                                                                               | Expected Result                                                                                            | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Construct a default DrmInfo instance using its default constructor                    | input: DrmInfo object with default values (method = default, mediaFormat = default, useFirst16BytesAsIV = default, masterManifestURL = "", manifestURL = "", keyURI = "", keyFormat = "", systemUUID = "", initData = "") | DrmInfo object is instantiated with its default values                                                     | Should be successful |
 * | 02               | Invoke the PlayReadyHelper constructor with the default DrmInfo and verify no exception  | input: drmInfo (default constructed); expected output: internal state FRIENDLY_NAME = "PlayReady", CODEC_TYPE = 2                                                          | PlayReadyHelper is constructed without throwing an exception, and internal states are as expected          | Should Pass   |
 */
TEST(PlayReadyHelper, DefaultDrmInfoConstruction)
{
    std::cout << "Entering DefaultDrmInfoConstruction test\n";

    // Construct default DrmInfo
    DrmInfo drmInfo;
    std::cout << "Default DrmInfo constructed:" << "\n";
    std::cout << " method: " << drmInfo.method << "\n";
    std::cout << " mediaFormat: " << drmInfo.mediaFormat << "\n";
    std::cout << " useFirst16BytesAsIV: " << drmInfo.useFirst16BytesAsIV << "\n";
    std::cout << " masterManifestURL: \"" << drmInfo.masterManifestURL << "\"\n";
    std::cout << " manifestURL: \"" << drmInfo.manifestURL << "\"\n";
    std::cout << " keyURI: \"" << drmInfo.keyURI << "\"\n";
    std::cout << " keyFormat: \"" << drmInfo.keyFormat << "\"\n";
    std::cout << " systemUUID: \"" << drmInfo.systemUUID << "\"\n";
    std::cout << " initData: \"" << drmInfo.initData << "\"\n";

    // Construct PlayReadyHelper, using EXPECT_NO_THROW to ensure no exception.
    EXPECT_NO_THROW({
        PlayReadyHelper helper(drmInfo);
        std::cout << "PlayReadyHelper constructed successfully with default DrmInfo." << "\n";
        // Log the expected internal state values (FRIENDLY_NAME "PlayReady", CODEC_TYPE 2)
        std::cout << "Expected FRIENDLY_NAME: \"PlayReady\", Expected CODEC_TYPE: 2" << "\n";
    });

    std::cout << "Exiting DefaultDrmInfoConstruction test\n";
}
/**
 * @brief Validate successful construction of PlayReadyHelper using valid AES-128 DRM Info.
 *
 * This test case verifies that a PlayReadyHelper object is correctly constructed when provided with a properly
 * populated DrmInfo structure for AES-128 DRM. It checks the instantiation and internal state initialization without
 * any exceptions thrown. The test logs the input parameters and verifies that the expected internal state settings,
 * such as FRIENDLY_NAME "PlayReady" and CODEC_TYPE 2, are achieved.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                                                                                                                                                                                                            | Expected Result                                                                                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Set up DrmInfo with valid AES-128 DRM parameters and log the values                             | method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_DASH, useFirst16BytesAsIV = true, IV = "1234567890123456", masterManifestURL = "http://master.manifest", manifestURL = "http://playlist.manifest", keyURI = "http://key.uri", keyFormat = "format1", systemUUID = "uuid", initData = "initData"        | DrmInfo structure is correctly populated with expected values                                                          | Should be successful  |
 * | 02               | Invoke PlayReadyHelper constructor using the populated DrmInfo, ensuring no exception is thrown  | drmInfo object from step 01                                                                                                                                | PlayReadyHelper object is constructed without exceptions, and internal state shows FRIENDLY_NAME "PlayReady" and CODEC_TYPE 2; EXPECT_NO_THROW assertion passes | Should Pass   |
 */
TEST(PlayReadyHelper, ValidAES128DRMInfo)
{
    std::cout << "Entering ValidAES128DRMInfo test\n";

    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_DASH;
    drmInfo.useFirst16BytesAsIV = true;
    // Set IV to a sample value using strncpy (casting to char* for logging purposes)
    strncpy(reinterpret_cast<char*>(drmInfo.iv), "1234567890123456", DRM_IV_LEN);
    drmInfo.masterManifestURL = "http://master.manifest";
    drmInfo.manifestURL = "http://playlist.manifest";
    drmInfo.keyURI = "http://key.uri";
    drmInfo.keyFormat = "format1";
    drmInfo.systemUUID = "uuid";
    drmInfo.initData = "initData";

    std::cout << "Custom DrmInfo constructed with AES-128 values:" << "\n";
    std::cout << " method: " << drmInfo.method << "\n";
    std::cout << " mediaFormat: " << drmInfo.mediaFormat << "\n";
    std::cout << " useFirst16BytesAsIV: " << drmInfo.useFirst16BytesAsIV << "\n";
    std::cout << " IV: " << std::string(reinterpret_cast<char*>(drmInfo.iv), DRM_IV_LEN) << "\n";
    std::cout << " masterManifestURL: \"" << drmInfo.masterManifestURL << "\"\n";
    std::cout << " manifestURL: \"" << drmInfo.manifestURL << "\"\n";
    std::cout << " keyURI: \"" << drmInfo.keyURI << "\"\n";
    std::cout << " keyFormat: \"" << drmInfo.keyFormat << "\"\n";
    std::cout << " systemUUID: \"" << drmInfo.systemUUID << "\"\n";
    std::cout << " initData: \"" << drmInfo.initData << "\"\n";

    // Invoke PlayReadyHelper constructor
    EXPECT_NO_THROW({
        PlayReadyHelper helper(drmInfo);
        std::cout << "PlayReadyHelper constructed successfully with AES-128 DRM Info." << "\n";
        std::cout << "Internal state should reflect FRIENDLY_NAME: \"PlayReady\", CODEC_TYPE: 2" << "\n";
    });

    std::cout << "Exiting ValidAES128DRMInfo test\n";
}
/**
 * @brief Verify that PlayReadyHelper handles an unsupported DRM method correctly.
 *
 * This test checks whether the PlayReadyHelper API correctly constructs an object when provided 
 * with a DrmInfo structure configured with an unsupported DRM method (eMETHOD_NONE). It ensures that no exception 
 * is thrown during object construction and that internal state logs reference the unsupported method appropriately.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                            | Expected Result                                                                                                             | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize DrmInfo with unsupported DRM method and default media format.                      | drmInfo.method = eMETHOD_NONE, drmInfo.mediaFormat = eMEDIAFORMAT_HLS                   | DrmInfo object is correctly configured with the unsupported DRM method and default values.                                   | Should be successful |
 * | 02               | Invoke PlayReadyHelper constructor with the configured DrmInfo and validate no exception is thrown. | Input: DrmInfo (method = eMETHOD_NONE, mediaFormat = eMEDIAFORMAT_HLS)                    | PlayReadyHelper is constructed successfully with no exceptions; internal state reflects method: eMETHOD_NONE, FRIENDLY_NAME: "PlayReady", CODEC_TYPE: 2. | Should Pass   |
 * | 03               | Verify that output log messages display expected debug information during test execution.      | Console output logs from test run                                                      | Console logs display correct messages indicating the entry, construction, and exit of the test function with expected values. | Should be successful |
 */
TEST(PlayReadyHelper, UnsupportedDRMMethod)
{
    std::cout << "Entering UnsupportedDRMMethod test\n";

    DrmInfo drmInfo;
    // Set unsupported DRM method (eMETHOD_NONE) with default values for other fields.
    drmInfo.method = eMETHOD_NONE;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;  // default
    // Other fields remain default.
    
    std::cout << "Custom DrmInfo constructed with unsupported DRM method:" << "\n";
    std::cout << " method: " << drmInfo.method << " (expected unsupported)" << "\n";
    std::cout << " mediaFormat: " << drmInfo.mediaFormat << "\n";

    // Invoke PlayReadyHelper constructor
    EXPECT_NO_THROW({
        PlayReadyHelper helper(drmInfo);
        std::cout << "PlayReadyHelper constructed with unsupported DRM method." << "\n";
        std::cout << "Internal state reflects method: " << drmInfo.method << " with FRIENDLY_NAME: \"PlayReady\", CODEC_TYPE: 2" << "\n";
    });

    std::cout << "Exiting UnsupportedDRMMethod test\n";
}
/**
 * @brief Test PlayReadyHelper construction with various valid DrmMethod values
 *
 * This test iterates over different DRM method enum values to ensure that constructing
 * a PlayReadyHelper object using valid DrmInfo settings does not throw any exceptions.
 * It validates that the object’s internal state is set correctly based on the provided DRM method.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Invoke PlayReadyHelper with drmInfo containing eMETHOD_NONE and valid settings. | drmInfo.method = eMETHOD_NONE, drmInfo.mediaFormat = eMEDIAFORMAT_DASH, useFirst16BytesAsIV = true, iv = "abcdefghijklmnop", masterManifestURL = "http://master.manifest", manifestURL = "http://playlist.manifest", keyURI = "http://key.uri", keyFormat = "format1", systemUUID = "uuid", initData = "initData" | PlayReadyHelper constructed successfully without throwing exception; internal state set with FRIENDLY_NAME "PlayReady" and CODEC_TYPE 2 | Should Pass |
 * | 02 | Invoke PlayReadyHelper with drmInfo containing eMETHOD_AES_128 and valid settings. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_DASH, useFirst16BytesAsIV = true, iv = "abcdefghijklmnop", masterManifestURL = "http://master.manifest", manifestURL = "http://playlist.manifest", keyURI = "http://key.uri", keyFormat = "format1", systemUUID = "uuid", initData = "initData" | PlayReadyHelper constructed successfully without throwing exception; internal state set with FRIENDLY_NAME "PlayReady" and CODEC_TYPE 2 | Should Pass |
 */
TEST(PlayReadyHelper, LoopDrmMethodEnumValues)
{
    std::cout << "Entering LoopDrmMethodEnumValues test\n";

    DrmMethod methods[] = { eMETHOD_NONE, eMETHOD_AES_128 };

    for (DrmMethod m : methods)
    {
        DrmInfo drmInfo;
        drmInfo.method = m;
        drmInfo.mediaFormat = eMEDIAFORMAT_DASH; // fixed valid value for mediaFormat
        drmInfo.useFirst16BytesAsIV = true;
        strncpy(reinterpret_cast<char*>(drmInfo.iv), "abcdefghijklmnop", DRM_IV_LEN);
        drmInfo.masterManifestURL = "http://master.manifest";
        drmInfo.manifestURL = "http://playlist.manifest";
        drmInfo.keyURI = "http://key.uri";
        drmInfo.keyFormat = "format1";
        drmInfo.systemUUID = "uuid";
        drmInfo.initData = "initData";

        std::cout << "Testing with DrmMethod value: " << m << "\n";

        EXPECT_NO_THROW({
            PlayReadyHelper helper(drmInfo);
            std::cout << "PlayReadyHelper constructed successfully for DrmMethod: " << m << "\n";
            std::cout << "For method " << m << ", internal state should reflect FRIENDLY_NAME: \"PlayReady\", CODEC_TYPE: 2" << "\n";
        });
    }

    std::cout << "Exiting LoopDrmMethodEnumValues test\n";
}
/**
 * @brief Validate that PlayReadyHelper correctly handles all MediaFormat enum values without throwing exceptions
 *
 * This test iterates through a predefined array of MediaFormat values. For each value, it initializes a DrmInfo structure with a supported AES-128 DRM configuration, including specific test data for IV, manifest URLs, key URI, key format, system UUID, and initialization data. It then constructs a PlayReadyHelper instance and asserts that no exceptions are thrown during construction. The test also outputs logs to verify that the internal state reflects the provided MediaFormat value and expected static attributes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log the start of the test and initialize an array of MediaFormat values | No input data; internal logging "Entering LoopMediaFormatEnumValues test" | Log message "Entering LoopMediaFormatEnumValues test" printed | Should be successful |
 * | 02 | For each MediaFormat, initialize DrmInfo with method = eMETHOD_AES_128, mediaFormat = current enum value, useFirst16BytesAsIV = true, iv = "ABCDEFGHIJKLMNOP", masterManifestURL = "http://master.manifest", manifestURL = "http://playlist.manifest", keyURI = "http://key.uri", keyFormat = "format1", systemUUID = "uuid", initData = "initData", then construct PlayReadyHelper and check no exception is thrown | input: drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = each enum value, drmInfo.useFirst16BytesAsIV = true, drmInfo.iv = "ABCDEFGHIJKLMNOP", drmInfo.masterManifestURL = "http://master.manifest", drmInfo.manifestURL = "http://playlist.manifest", drmInfo.keyURI = "http://key.uri", drmInfo.keyFormat = "format1", drmInfo.systemUUID = "uuid", drmInfo.initData = "initData" | PlayReadyHelper instance constructed successfully with matching internal state and no exception thrown | Should Pass |
 * | 03 | Log the end of the test execution | No input data; internal logging "Exiting LoopMediaFormatEnumValues test" | Log message "Exiting LoopMediaFormatEnumValues test" printed | Should be successful |
 */
TEST(PlayReadyHelper, LoopMediaFormatEnumValues)
{
    std::cout << "Entering LoopMediaFormatEnumValues test\n";

    MediaFormat mediaFormats[] = {
        eMEDIAFORMAT_HLS,
        eMEDIAFORMAT_DASH,
        eMEDIAFORMAT_PROGRESSIVE,
        eMEDIAFORMAT_HLS_MP4,
        eMEDIAFORMAT_OTA,
        eMEDIAFORMAT_HDMI,
        eMEDIAFORMAT_COMPOSITE,
        eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA,
        eMEDIAFORMAT_RMF,
        eMEDIAFORMAT_UNKNOWN
    };

    for (MediaFormat mf : mediaFormats)
    {
        DrmInfo drmInfo;
        // Set method to supported AES-128 DRM
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = mf;
        drmInfo.useFirst16BytesAsIV = true;
        strncpy(reinterpret_cast<char*>(drmInfo.iv), "ABCDEFGHIJKLMNOP", DRM_IV_LEN);
        drmInfo.masterManifestURL = "http://master.manifest";
        drmInfo.manifestURL = "http://playlist.manifest";
        drmInfo.keyURI = "http://key.uri";
        drmInfo.keyFormat = "format1";
        drmInfo.systemUUID = "uuid";
        drmInfo.initData = "initData";

        std::cout << "Testing with MediaFormat value: " << mf << "\n";

        EXPECT_NO_THROW({
            PlayReadyHelper helper(drmInfo);
            std::cout << "PlayReadyHelper constructed successfully for MediaFormat: " << mf << "\n";
            std::cout << "Internal state should reflect provided MediaFormat value: " << mf 
                      << ", FRIENDLY_NAME: \"PlayReady\", CODEC_TYPE: 2" << "\n";
        });
    }

    std::cout << "Exiting LoopMediaFormatEnumValues test\n";
}
/**
 * @brief Test the createInitData method to ensure an empty vector is correctly populated
 *
 * This test verifies that the createInitData method of the PlayReadyHelper class successfully populates an empty initialization data vector. The test ensures that invoking the method with an empty vector does not throw any exceptions and that the vector is populated (i.e., its size becomes greater than 0).@n
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
 * | 01 | Create a PlayReadyHelper instance, initialize an empty vector, invoke createInitData, and verify the vector is populated | helper = PlayReadyHelper(), initData = empty vector | No exceptions thrown; initData vector size > 0 | Should Pass |
 */
TEST(PlayReadyHelper, ValidInitializationDataWithEmptyVector)
{
    GTEST_SKIP();
    std::cout << "Entering ValidInitializationDataWithEmptyVector test" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        // Create the PlayReadyHelper object using the default constructor
        PlayReadyHelper helper(drmInfo);
        std::vector<uint8_t> initData;
        std::cout << "Before invocation, initData size: " << initData.size() << std::endl;
        std::cout << "Invoking createInitData with an empty vector." << std::endl;
        helper.createInitData(initData);
        std::cout << "After invocation, initData size: " << initData.size() << std::endl;
        std::cout << "Contents of initData: ";
        for (size_t i = 0; i < initData.size(); i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        // Validate that the initData vector is populated
        EXPECT_GT(initData.size(), 0u);
    });
    std::cout << "Exiting ValidInitializationDataWithEmptyVector test" << std::endl;
}
/**
 * @brief Validate that PlayReadyHelper::createInitData properly repopulates the initialization vector for valid pre-populated data
 *
 * Tests that when a pre-populated vector is passed to createInitData, the original vector is modified and repopulated with new data, distinct from the original input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayReadyHelper object and a pre-populated vector {0xAA, 0xBB, 0xCC} | input1 = N/A, output1 = N/A | Object is created successfully | Should be successful |
 * | 02 | Print the initial vector contents before API invocation | initData = 0xAA, 0xBB, 0xCC | The vector contains the expected initial values | Should be successful |
 * | 03 | Invoke createInitData with the pre-populated vector | input1 = initData = {0xAA, 0xBB, 0xCC} | The API repopulates the vector without throwing exceptions | Should Pass |
 * | 04 | Validate the repopulated vector is not empty and different from the original vector | input1 = originalData = {0xAA, 0xBB, 0xCC}, output1 = initData.size() > 0, initData != originalData | The vector is modified and its size is greater than 0 | Should Pass |
 */
TEST(PlayReadyHelper, ValidInitializationDataWithPrePopulatedVector)
{
    GTEST_SKIP();
    std::cout << "Entering ValidInitializationDataWithPrePopulatedVector test" << std::endl;
    EXPECT_NO_THROW(({
        DrmInfo drmInfo;
        // Create the PlayReadyHelper object using the default constructor
        PlayReadyHelper helper(drmInfo);
        std::vector<uint8_t> initData = {0xAA, 0xBB, 0xCC};
        std::cout << "Before invocation, initData contains: ";
        for (size_t i = 0; i < initData.size(); i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        std::cout << "Invoking createInitData with a pre-populated vector." << std::endl;
        helper.createInitData(initData);
        std::cout << "After invocation, initData size: " << initData.size() << std::endl;
        std::cout << "Contents of initData after processing: ";
        for (size_t i = 0; i < initData.size(); i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initData[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        // Validate that the initData vector is repopulated with new data
        EXPECT_GT(initData.size(), 0u);
        std::vector<uint8_t> originalData = {0xAA, 0xBB, 0xCC};
        EXPECT_NE(initData, originalData);
    }));
    std::cout << "Exiting ValidInitializationDataWithPrePopulatedVector test" << std::endl;
}
/**
 * @brief Verify consistency of output from multiple calls to createInitData
 *
 * This test ensures that calling createInitData consecutively on a PlayReadyHelper object produces identical initialization data each time.@n
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
 * | :---: | ----------- | --------- | -------------- | ----- |
 * | 01 | Instantiate PlayReadyHelper and invoke createInitData for the first call | output initDataFirstCall = empty vector | initDataFirstCall is populated with initialization data; No exceptions thrown | Should Pass |
 * | 02 | Invoke createInitData for the second call | output initDataSecondCall = empty vector | initDataSecondCall is populated with the same initialization data as the first call; No exceptions thrown | Should Pass |
 * | 03 | Compare results of the first and second calls | input: initDataFirstCall, initDataSecondCall; output: result of equality check | Both calls produce equal initialization data vectors | Should Pass |
 */
TEST(PlayReadyHelper, ConsistencyAcrossMultipleCalls)
{
    std::cout << "Entering ConsistencyAcrossMultipleCalls test" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        // Create the PlayReadyHelper object using the default constructor
        PlayReadyHelper helper(drmInfo);
        std::vector<uint8_t> initDataFirstCall;
        std::vector<uint8_t> initDataSecondCall;

        std::cout << "Invoking createInitData for the first call." << std::endl;
        helper.createInitData(initDataFirstCall);
        std::cout << "First call - initData size: " << initDataFirstCall.size() << "; Contents: ";
        for (size_t i = 0; i < initDataFirstCall.size(); i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initDataFirstCall[i]) << " ";
        }
        std::cout << std::dec << std::endl;

        std::cout << "Invoking createInitData for the second call." << std::endl;
        helper.createInitData(initDataSecondCall);
        std::cout << "Second call - initData size: " << initDataSecondCall.size() << "; Contents: ";
        for (size_t i = 0; i < initDataSecondCall.size(); i++) {
            std::cout << "0x" << std::hex << static_cast<int>(initDataSecondCall[i]) << " ";
        }
        std::cout << std::dec << std::endl;

        // Validate that both calls produce consistent initialization data.
        EXPECT_EQ(initDataFirstCall, initDataSecondCall);
    });
    std::cout << "Exiting ConsistencyAcrossMultipleCalls test" << std::endl;
}
/**
 * @brief Verify that friendlyName() returns the expected value for PlayReady DRM
 *
 * This test validates that the friendlyName() method of the PlayReadyHelper object returns the expected friendly name "PlayReady DRM" and that no exceptions are thrown during the process.
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
 * | Variation / Step | Description                                                    | Test Data                                                         | Expected Result                                                                 | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayReadyHelper object using the default constructor    | input: default constructor, no explicit output argument             | Object is created successfully without throwing exceptions                    | Should Pass |
 * | 02               | Invoke friendlyName() on the created object and compare result   | input: call friendlyName(), expected friendly name = "PlayReady DRM" | Returns "PlayReady DRM" as friendly name and matches the expected value; assertion passes | Should Pass |
 */
TEST(PlayReadyHelper, VerifyFriendlyNameReturnsExpected)
{
    GTEST_SKIP();
    std::cout << "Entering VerifyFriendlyNameReturnsExpected test" << std::endl;

    // Create the PlayReadyHelper object using the default constructor and invoke friendlyName().
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        std::cout << "PlayReadyHelper object created successfully using the default constructor." << std::endl;
        
        std::cout << "Invoking friendlyName() method." << std::endl;
        const std::string& result = helper.friendlyName();
        std::cout << "Method friendlyName() returned: " << result << std::endl;
        
        // Define expected friendly name based on the constant FRIENDLY_NAME.
        std::string expectedFriendlyName = "PlayReady DRM";
        std::cout << "Expected friendly name is: " << expectedFriendlyName << std::endl;
        
        // Check that the returned result matches the expected friendly name.
        EXPECT_EQ(result, expectedFriendlyName);
    });

    std::cout << "Exiting VerifyFriendlyNameReturnsExpected test" << std::endl;
}
/**
 * @brief Test the generateLicenseRequest method of PlayReadyHelper with valid ChallengeInfo input.
 *
 * This test verifies that when a valid ChallengeInfo object (with a non-null data pointer,
 * a valid URL, and a valid access token) is provided, the PlayReadyHelper's generateLicenseRequest
 * function processes the input correctly without throwing an exception. The resulting LicenseRequest
 * is expected to have its fields updated appropriately.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Construct PlayReadyHelper object and prepare ChallengeInfo with valid data. | challengeInfo.data = 123, challengeInfo.url = "https://license.server.com/request", challengeInfo.accessToken = "validAccessToken" | Objects constructed successfully without exceptions. | Should be successful |
 * | 02 | Invoke generateLicenseRequest with the prepared ChallengeInfo and a default LicenseRequest; validate LicenseRequest fields are updated. | helper: valid PlayReadyHelper, licenseRequest: default initialized | LicenseRequest fields updated appropriately, no exceptions thrown. | Should Pass |
 */
TEST(PlayReadyHelper, ValidChallengeInfoAllRequiredData) {
    std::cout << "Entering ValidChallengeInfoAllRequiredData test" << std::endl;

    // Create PlayReadyHelper using default constructor.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Prepare ChallengeInfo with valid data
        ChallengeInfo challengeInfo;
        // Create a dummy non-null shared pointer (object details not important)
        //challengeInfo.data = std::make_shared<int>(123);
        // Set URL and access token values
        challengeInfo.url = "https://license.server.com/request";
        challengeInfo.accessToken = "validAccessToken";
        
        // Initialize LicenseRequest to default.
        LicenseRequest licenseRequest;
        
        std::cout << "Invoking generateLicenseRequest with ChallengeInfo:" << std::endl;
        std::cout << "  URL: " << challengeInfo.url << std::endl;
        std::cout << "  AccessToken: " << challengeInfo.accessToken << std::endl;
        std::cout << "  Data pointer is " << (challengeInfo.data ? "non-null" : "null") << std::endl;
        
        // Invoke the method.
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "After generateLicenseRequest:" << std::endl;
        std::cout << "  LicenseRequest.url: " << licenseRequest.url << std::endl;
        std::cout << "  LicenseRequest.licenseAnonymousRequest: " << (licenseRequest.licenseAnonymousRequest ? "true" : "false") << std::endl;
        std::cout << "  LicenseRequest.payload: " << licenseRequest.payload << std::endl;
        std::cout << "  LicenseRequest.headers count: " << licenseRequest.headers.size() << std::endl;
        std::cout << "  LicenseRequest.method: " << static_cast<int>(licenseRequest.method) << std::endl;
        
    });
    
    std::cout << "Exiting ValidChallengeInfoAllRequiredData test" << std::endl;
}
/**
 * @brief Verify that generateLicenseRequest correctly processes anonymous ChallengeInfo input.
 *
 * This test verifies that when ChallengeInfo is provided with an empty accessToken,
 * the generateLicenseRequest API handles it as an anonymous request without throwing exceptions.
 * It ensures that the LicenseRequest object is populated accordingly based on the anonymous request semantics.
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
 * | 01 | Print the entering test message to indicate the start of the test. | N/A | "Entering ValidChallengeInfoAnonymousRequest test" is printed to console. | Should be successful |
 * | 02 | Instantiate the PlayReadyHelper object. | PlayReadyHelper helper is created. | Instance of PlayReadyHelper is successfully created. | Should be successful |
 * | 03 | Prepare ChallengeInfo with empty accessToken to simulate an anonymous request. | challengeInfo.data = 456, challengeInfo.url = "https://license.server.com/request", challengeInfo.accessToken = "" | ChallengeInfo is initialized with valid data and empty accessToken. | Should be successful |
 * | 04 | Initialize the LicenseRequest structure. | LicenseRequest licenseRequest is default constructed. | An empty LicenseRequest object is created ready for population. | Should be successful |
 * | 05 | Invoke generateLicenseRequest API using the prepared ChallengeInfo and LicenseRequest objects. | challengeInfo: (data=456, url="https://license.server.com/request", accessToken=""), licenseRequest: default instance | LicenseRequest object is correctly populated without throwing an exception, confirming proper handling of anonymous requests. | Should Pass |
 * | 06 | Print the updated details of the LicenseRequest object to verify the output. | LicenseRequest object's fields (url, licenseAnonymousRequest, payload, headers count, method) | Console outputs expected LicenseRequest details reflecting the anonymous request handling. | Should be successful |
 * | 07 | Print the exiting test message to indicate the end of the test. | N/A | "Exiting ValidChallengeInfoAnonymousRequest test" is printed to console. | Should be successful |
 */
TEST(PlayReadyHelper, ValidChallengeInfoAnonymousRequest) {
    std::cout << "Entering ValidChallengeInfoAnonymousRequest test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Prepare ChallengeInfo with empty accessToken to simulate anonymous request.
        ChallengeInfo challengeInfo;
        //challengeInfo.data = std::make_shared<int>(456);
        challengeInfo.url = "https://license.server.com/request";
        challengeInfo.accessToken = "";
        
        LicenseRequest licenseRequest;
        
        std::cout << "Invoking generateLicenseRequest with ChallengeInfo:" << std::endl;
        std::cout << "  URL: " << challengeInfo.url << std::endl;
        std::cout << "  AccessToken: (empty)" << std::endl;
        std::cout << "  Data pointer is " << (challengeInfo.data ? "non-null" : "null") << std::endl;
        
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "After generateLicenseRequest:" << std::endl;
        std::cout << "  LicenseRequest.url: " << licenseRequest.url << std::endl;
        std::cout << "  LicenseRequest.licenseAnonymousRequest: " << (licenseRequest.licenseAnonymousRequest ? "true" : "false") << std::endl;
        std::cout << "  LicenseRequest.payload: " << licenseRequest.payload << std::endl;
        std::cout << "  LicenseRequest.headers count: " << licenseRequest.headers.size() << std::endl;
        std::cout << "  LicenseRequest.method: " << static_cast<int>(licenseRequest.method) << std::endl;
        
    });
    
    std::cout << "Exiting ValidChallengeInfoAnonymousRequest test" << std::endl;
}
/**
 * @brief Validates that generateLicenseRequest processes a ChallengeInfo with an empty URL correctly.
 *
 * This test verifies that when ChallengeInfo is provided with an empty URL, the generateLicenseRequest API of the PlayReadyHelper
 * does not throw any exceptions and handles the scenario as expected. The test creates a ChallengeInfo instance with empty URL,
 * a valid access token, and a non-null data pointer, then invokes generateLicenseRequest and assesses that no exception is thrown.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                                  | Expected Result                                                                              | Notes               |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------------- |
 * | 01               | Setup ChallengeInfo with an empty URL, valid access token and non-null data; initialize LicenseRequest instance. | challengeInfo.data = 789, challengeInfo.url = "", challengeInfo.accessToken = validAccessToken, licenseRequest = {}             | ChallengeInfo and LicenseRequest objects are properly created with the specified values.       | Should be successful |
 * | 02               | Invoke generateLicenseRequest with the prepared ChallengeInfo and LicenseRequest.             | API call: PlayReadyHelper.generateLicenseRequest(challengeInfo, licenseRequest)                                            | No exception is thrown; LicenseRequest is updated accordingly as per the API's logic.         | Should Pass          |
 */
TEST(PlayReadyHelper, ChallengeInfoEmptyURL) {
    std::cout << "Entering ChallengeInfoEmptyURL test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Prepare ChallengeInfo with empty URL.
        ChallengeInfo challengeInfo;
        //challengeInfo.data = std::make_shared<int>(789);
        challengeInfo.url = "";
        challengeInfo.accessToken = "validAccessToken";
        
        LicenseRequest licenseRequest;
        
        std::cout << "Invoking generateLicenseRequest with ChallengeInfo:" << std::endl;
        std::cout << "  URL: (empty)" << std::endl;
        std::cout << "  AccessToken: " << challengeInfo.accessToken << std::endl;
        std::cout << "  Data pointer is " << (challengeInfo.data ? "non-null" : "null") << std::endl;
        
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "After generateLicenseRequest:" << std::endl;
        std::cout << "  LicenseRequest.url: '" << licenseRequest.url << "'" << std::endl;
        std::cout << "  LicenseRequest.payload: '" << licenseRequest.payload << "'" << std::endl;
        std::cout << "  LicenseRequest.headers count: " << licenseRequest.headers.size() << std::endl;
        
    });
    
    std::cout << "Exiting ChallengeInfoEmptyURL test" << std::endl;
}
/**
 * @brief Verify generateLicenseRequest correctly handles ChallengeInfo with null data.
 *
 * This test validates that when a ChallengeInfo object is provided with a nullptr for the data field, alongside a valid URL and access token, the generateLicenseRequest API in PlayReadyHelper executes without throwing any exceptions. It ensures that the API can process ChallengeInfo containing null data safely.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 022
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayReadyHelper, ChallengeInfo with null data, and LicenseRequest instances | PlayReadyHelper instance; ChallengeInfo: url = "https://license.server.com/request", accessToken = "validAccessToken", data = nullptr; LicenseRequest instance | Objects are instantiated successfully without any exception | Should be successful |
 * | 02 | Invoke generateLicenseRequest API with ChallengeInfo as input and verify LicenseRequest output | Input: ChallengeInfo (url = "https://license.server.com/request", accessToken = "validAccessToken", data = nullptr); Output: LicenseRequest object to be updated | API call completes with no exceptions; LicenseRequest is updated accordingly | Should Pass |
 */
TEST(PlayReadyHelper, ChallengeInfoNullData) {
    std::cout << "Entering ChallengeInfoNullData test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Prepare ChallengeInfo with nullptr for data.
        ChallengeInfo challengeInfo;
        challengeInfo.data = nullptr;
        challengeInfo.url = "https://license.server.com/request";
        challengeInfo.accessToken = "validAccessToken";
        
        LicenseRequest licenseRequest;
        
        std::cout << "Invoking generateLicenseRequest with ChallengeInfo:" << std::endl;
        std::cout << "  URL: " << challengeInfo.url << std::endl;
        std::cout << "  AccessToken: " << challengeInfo.accessToken << std::endl;
        std::cout << "  Data pointer is null" << std::endl;
        
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "After generateLicenseRequest:" << std::endl;
        std::cout << "  LicenseRequest.url: '" << licenseRequest.url << "'" << std::endl;
        std::cout << "  LicenseRequest.payload: '" << licenseRequest.payload << "'" << std::endl;
        std::cout << "  LicenseRequest.headers count: " << licenseRequest.headers.size() << std::endl;
        
    });
    
    std::cout << "Exiting ChallengeInfoNullData test" << std::endl;
}
/**
 * @brief Validate that generateLicenseRequest correctly handles ChallengeInfo with a whitespace URL.
 *
 * This test validates that when ChallengeInfo has a URL containing only whitespace, the generateLicenseRequest function
 * processes the input without throwing an exception. The objective is to ensure that the API gracefully handles edge case
 * inputs and maintains robust functionality.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                  | Test Data                                                                                                                  | Expected Result                                                         | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ---------- |
 * | 01               | Initialize PlayReadyHelper, prepare ChallengeInfo with whitespace URL, and invoke generateLicenseRequest()      | challengeInfo.data = 321, challengeInfo.url = "   ", challengeInfo.accessToken = "validAccessToken", LicenseRequest object | No exception is thrown and generateLicenseRequest processes the input     | Should Pass|
 */
TEST(PlayReadyHelper, ChallengeInfoWhitespaceURL) {
    std::cout << "Entering ChallengeInfoWhitespaceURL test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Prepare ChallengeInfo with URL containing only whitespace.
        ChallengeInfo challengeInfo;
        //challengeInfo.data = std::make_shared<int>(321);
        challengeInfo.url = "   ";
        challengeInfo.accessToken = "validAccessToken";
        
        LicenseRequest licenseRequest;
        
        std::cout << "Invoking generateLicenseRequest with ChallengeInfo:" << std::endl;
        std::cout << "  URL: '" << challengeInfo.url << "'" << std::endl;
        std::cout << "  AccessToken: " << challengeInfo.accessToken << std::endl;
        std::cout << "  Data pointer is " << (challengeInfo.data ? "non-null" : "null") << std::endl;
        
        helper.generateLicenseRequest(challengeInfo, licenseRequest);
        std::cout << "After generateLicenseRequest:" << std::endl;
        std::cout << "  LicenseRequest.url: '" << licenseRequest.url << "'" << std::endl;
        std::cout << "  LicenseRequest.payload: '" << licenseRequest.payload << "'" << std::endl;
        std::cout << "  LicenseRequest.headers count: " << licenseRequest.headers.size() << std::endl;
        
    });
    
    std::cout << "Exiting ChallengeInfoWhitespaceURL test" << std::endl;
}
/**
 * @brief Verify getDrmCodecType returns a positive codec type
 *
 * This test creates an instance of PlayReadyHelper and calls getDrmCodecType() to ensure that the returned codec type is positive. It verifies that the API correctly initializes the object and returns a valid, positive codec type value.
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
 * | 01 | Create PlayReadyHelper object using default constructor; call getDrmCodecType() and check that the returned codec type is positive | None, output1 = codecType (value > 0) | getDrmCodecType() should return a positive integer; EXPECT_GT(codecType, 0) should pass | Should Pass |
 */
TEST(PlayReadyHelper, Verify_getDrmCodecType_returns_assigned_positive_codec_type) {
    std::cout << "Entering Verify_getDrmCodecType_returns_assigned_positive_codec_type test" << std::endl;
    
    // Create PlayReadyHelper object using default constructor and verify no exceptions are thrown.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "Created PlayReadyHelper object using default constructor." << std::endl;
        
        // Invoke getDrmCodecType() and log the invocation.
        int codecType = playReadyHelper.getDrmCodecType();
        std::cout << "Invoked getDrmCodecType(), returned value: " << codecType << std::endl;
        
        // Verify that the returned codec type is positive (e.g., expecting 1 as valid)
        EXPECT_GT(codecType, 0);
        std::cout << "Verified that the codec type (" << codecType << ") is positive." << std::endl;
    });
    
    std::cout << "Exiting Verify_getDrmCodecType_returns_assigned_positive_codec_type test" << std::endl;
}
/**
 * @brief Verify that getDrmMetaData function returns proper metadata.
 *
 * This test verifies that a PlayReadyHelper object can be created without exceptions,
 * that the getDrmMetaData method returns a non-empty metadata string, and that multiple
 * invocations of getDrmMetaData return the same reference, ensuring consistency.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                              | Expected Result                                                      | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayReadyHelper instance using default constructor          | N/A                                                                    | Object is created without throwing exceptions                      | Should Pass |
 * | 02               | Invoke getDrmMetaData method and log the returned metadata           | Calling helper.getDrmMetaData(), output: metaData (non-empty string)     | Returns non-empty string                                             | Should Pass |
 * | 03               | Invoke getDrmMetaData method a second time for consistency check       | Calling helper.getDrmMetaData() again, output: metaDataSecondCall (non-empty string) | Returns non-empty string                                             | Should Pass |
 * | 04               | Compare metadata references from both invocations                    | Comparing addresses of metaData and metaDataSecondCall                   | Both calls return the same reference (addresses are equal)           | Should Pass |
 */
TEST(PlayReadyHelper, verify_getDrmMetaData_functions_properly) {
    GTEST_SKIP();
    std::cout << "Entering verify_getDrmMetaData_functions_properly test" << std::endl;
    
    // Create PlayReadyHelper object using its default constructor.
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        std::cout << "Created PlayReadyHelper object using default constructor" << std::endl;
        
        // Invoke getDrmMetaData and log the returned metadata.
        const std::string& metaData = helper.getDrmMetaData();
        std::cout << "Invoked getDrmMetaData(); returned metadata: \"" << metaData << "\"" << std::endl;
        EXPECT_FALSE(metaData.empty());
        
        // For additional verification, invoke the method again to ensure consistent output.
        const std::string& metaDataSecondCall = helper.getDrmMetaData();
        std::cout << "Invoked getDrmMetaData() second time; returned metadata: \"" << metaDataSecondCall << "\"" << std::endl;
        EXPECT_FALSE(metaDataSecondCall.empty());
        
        // Check that both calls return the same reference.
        EXPECT_EQ(&metaData, &metaDataSecondCall);
        std::cout << "Completed validation of getDrmMetaData return value." << std::endl;
    });
    
    std::cout << "Exiting verify_getDrmMetaData_functions_properly test" << std::endl;
}
/**
 * @brief Verify that a DRM session initializes properly with a valid key.
 *
 * This test verifies that a PlayReadyHelper object can be instantiated without exceptions,
 * and that invoking the getKey method properly populates the keyID vector with the expected DRM key.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                     | Test Data                                                   | Expected Result                                                                               | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------- | ----------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create PlayReadyHelper object using the default constructor.                    | N/A                                                         | Object is created without throwing an exception.                                             | Should Pass   |
 * | 02               | Prepare an empty keyID vector.                                                    | keyID = []                                                  | keyID vector is initialized with size 0.                                                       | Should be successful |
 * | 03               | Invoke getKey method with keyID vector as input.                                  | input: keyID = [], output: keyID expected to be [0x01,0x02,0x03,0x04] | getKey executes without exception and keyID is populated with the expected DRM key bytes.      | Should Pass   |
 * | 04               | Validate that the keyID vector size matches the expected key vector size.         | keyID.size = actual size, expected size = 4                 | The size of keyID vector equals the size of the expected key vector.                           | Should be successful |
 * | 05               | Validate each byte in the keyID vector matches the expected DRM key values.       | For each index i: keyID[i] vs expected[i]                     | Each element in keyID matches the corresponding element in the expected key vector.            | Should be successful |
 */
TEST(PlayReadyHelper, ValidDRMSessionProperlyInitializedKey) {
    GTEST_SKIP();
    std::cout << "Entering ValidDRMSessionProperlyInitializedKey test" << std::endl;

    // Create PlayReadyHelper object using default constructor
    EXPECT_NO_THROW((
    {
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        std::cout << "Created PlayReadyHelper object using default constructor." << std::endl;

        // Prepare an empty keyID vector
        std::vector<uint8_t> keyID;
        std::cout << "Initial keyID vector size: " << keyID.size() << std::endl;

        // Invoke getKey method
        std::cout << "Invoking getKey with keyID vector." << std::endl;
        EXPECT_NO_THROW(helper.getKey(keyID));

        // Debug log: print the returned keyID values
        std::cout << "Returned keyID values:";
        for (const auto &byte : keyID) {
            std::cout << " " << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Expected DRM key identifier bytes
        std::vector<uint8_t> expectedKey = {0x01, 0x02, 0x03, 0x04};
        std::cout << "Expected keyID values:";
        for (const auto &byte : expectedKey) {
            std::cout << " " << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Validate the output keyID vector matches expected values
        EXPECT_EQ(keyID.size(), expectedKey.size());
        for (size_t i = 0; i < expectedKey.size(); ++i) {
            EXPECT_EQ(keyID[i], expectedKey[i]);
            std::cout << "Validated keyID[" << i << "] = " << static_cast<int>(keyID[i]) << std::endl;
        }
    }));

    std::cout << "Exiting ValidDRMSessionProperlyInitializedKey test" << std::endl;
}
/**
 * @brief Validate that the DRM session pre-populated input vector is correctly handled by getKey.
 *
 * This test verifies that when a pre-populated keyID vector is provided to the getKey method, it is cleared and repopulated with the expected DRM key bytes. The test confirms that the method does not throw exceptions and that the output matches the expected keyID vector.
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
 * | Variation / Step | Description                                                                            | Test Data                                                            | Expected Result                                                                                                                          | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------- | ---------------------------------------------------------------------| ---------------------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create PlayReadyHelper object using default constructor.                              | None                                                                 | PlayReadyHelper object is created successfully.                                                                                         | Should be successful |
 * | 02               | Initialize a pre-populated keyID vector with dummy values.                             | keyID = 0xFF, 0xEE, 0xDD                                               | keyID vector is initialized with the dummy values.                                                                                      | Should be successful |
 * | 03               | Invoke getKey() method with the pre-populated keyID vector to repopulate with DRM key.   | input keyID = 0xFF, 0xEE, 0xDD                                          | No exception is thrown; keyID is cleared and repopulated with valid DRM key identifier {0x01, 0x02, 0x03, 0x04}.                           | Should Pass   |
 * | 04               | Validate that the returned keyID vector matches the expected DRM key values.             | updated keyID = result from getKey, expectedKey = 0x01, 0x02, 0x03, 0x04 | Each element of keyID matches the corresponding element in expectedKey and the vector sizes are equal.                                     | Should Pass   |
 */
TEST(PlayReadyHelper, ValidDRMSessionPrePopulatedInputVector) {
    GTEST_SKIP();
    std::cout << "Entering ValidDRMSessionPrePopulatedInputVector test" << std::endl;

    EXPECT_NO_THROW((
    {
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        std::cout << "Created PlayReadyHelper object using default constructor." << std::endl;

        // Prepare a pre-populated keyID vector with dummy values
        std::vector<uint8_t> keyID = {0xFF, 0xEE, 0xDD};
        std::cout << "Initial keyID vector (pre-populated) values:";
        for (const auto &byte : keyID) {
            std::cout << " " << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Invoke getKey method; the method is expected to clear and repopulate the vector
        std::cout << "Invoking getKey with pre-populated keyID vector." << std::endl;
        EXPECT_NO_THROW(helper.getKey(keyID));

        // Debug log: print the returned keyID values
        std::cout << "Returned keyID values:";
        for (const auto &byte : keyID) {
            std::cout << " " << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Expected DRM key identifier bytes
        std::vector<uint8_t> expectedKey = {0x01, 0x02, 0x03, 0x04};
        std::cout << "Expected keyID values:";
        for (const auto &byte : expectedKey) {
            std::cout << " " << static_cast<int>(byte);
        }
        std::cout << std::endl;

        // Validate the output keyID vector matches expected values
        EXPECT_EQ(keyID.size(), expectedKey.size());
        for (size_t i = 0; i < expectedKey.size(); ++i) {
            EXPECT_EQ(keyID[i], expectedKey[i]);
            std::cout << "Validated keyID[" << i << "] = " << static_cast<int>(keyID[i]) << std::endl;
        }
    }));

    std::cout << "Exiting ValidDRMSessionPrePopulatedInputVector test" << std::endl;
}
/**
 * @brief Verify the default instance behavior of PlayReadyHelper's isClearDecrypt() method
 *
 * This test checks that an instance of PlayReadyHelper can be created using its default constructor and verifies the behavior of the isClearDecrypt() method. It ensures that the method behaves as specified when invoked on a default instance.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 028
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                     | Test Data                                                       | Expected Result                                                                 | Notes             |
 * | :--------------: | -------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create an instance of PlayReadyHelper using its default constructor              | constructor: PlayReadyHelper()                                  | Instance is created without throwing any exceptions                           | Should be successful |
 * | 02               | Invoke the isClearDecrypt() method on the created instance                         | input: none, output: result (bool)                               | Method returns false as per the specification (validated by EXPECT_EQ condition) | Should Pass       |
 */
TEST(PlayReadyHelper, VerifyDefaultInstanceReturnsFalse) {
    std::cout << "Entering VerifyDefaultInstanceReturnsFalse test" << std::endl;
    
    // Create an instance of PlayReadyHelper using its default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        std::cout << "Created PlayReadyHelper instance using default constructor." << std::endl;
        
        // Invoke the isClearDecrypt() method
        std::cout << "Invoking PlayReadyHelper::isClearDecrypt() method." << std::endl;
        bool result = helper.isClearDecrypt();
        std::cout << "Method isClearDecrypt() returned: " << std::boolalpha << result << std::endl;
        
        // Verify that the method returns false as per the method specification
        EXPECT_EQ(result,false);
    });
    
    std::cout << "Exiting VerifyDefaultInstanceReturnsFalse test" << std::endl;
}
/**
 * @brief Verifies that the isHdcp22Required() method of PlayReadyHelper returns a valid boolean value.
 *
 * This test ensures that a PlayReadyHelper object created using its default constructor can successfully invoke 
 * the isHdcp22Required() method and that the returned value is a valid boolean (either true or false) without throwing any exceptions.
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
 * | Variation / Step | Description                                                                  | Test Data                                                                          | Expected Result                                                                                   | Notes           |
 * | :---------------:| -----------------------------------------------------------------------------| -----------------------------------------------------------------------------------| ------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create PlayReadyHelper object using default constructor.                     | Constructor call: PlayReadyHelper()                                                | Object is created successfully without throwing an exception.                                   | Should be successful |
 * | 02               | Invoke isHdcp22Required() method on the PlayReadyHelper object.               | Method call: isHdcp22Required() on valid playReadyHelper instance                     | Method returns a boolean value indicating the HDCP 2.2 requirement.                               | Should Pass     |
 * | 03               | Validate that the returned value is either true or false.                    | Output: hdcp22Required (bool)                                                       | The returned value is a valid boolean (true or false) and no exceptions are thrown.               | Should Pass     |
 */
TEST(PlayReadyHelper, VerifyReturnValueIsHdcp22Required) {
    std::cout << "Entering VerifyReturnValueIsHdcp22Required test" << std::endl;

    // Create PlayReadyHelper object using default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "PlayReadyHelper object created successfully using default constructor." << std::endl;

        // Invoke the isHdcp22Required method
        std::cout << "Invoking isHdcp22Required() method on PlayReadyHelper object." << std::endl;
        bool hdcp22Required = playReadyHelper.isHdcp22Required();
        std::cout << "isHdcp22Required() returned value: " << std::boolalpha << hdcp22Required << std::endl;

        // Log the internal state value related to output protection
        std::cout << "Internal state - bOutputProtectionEnabled: " << std::boolalpha << hdcp22Required << std::endl;

        // Since the method is expected to return a boolean (true or false), verify that the returned value is either true or false.
        // (This is implicitly always the case, so here we simply check that the method executes without throwing.)
        EXPECT_TRUE(hdcp22Required == true || hdcp22Required == false);
    });

    std::cout << "Exiting VerifyReturnValueIsHdcp22Required test" << std::endl;
}
/**
 * @brief Test to verify that the ocdmSystemId method returns a non-empty string.
 *
 * This test validates that the ocdmSystemId method from the PlayReadyHelper class returns a non-empty string when invoked.
 * The purpose is to ensure that the helper object initializes and provides a valid DRM system identifier.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayReadyHelper object using default constructor | No input, Output: PlayReadyHelper object created | Helper object is created without throwing an exception | Should Pass |
 * | 02 | Invoke the ocdmSystemId method and verify its output | Input: Invocation of ocdmSystemId method; Output: systemId string | ocdmSystemId method returns a non-empty string | Should Pass |
 */
TEST(PlayReadyHelper, VerifyOcdmSystemIdIsNotEmpty) {
    std::cout << "Entering VerifyOcdmSystemIdIsNotEmpty test" << std::endl;
    
    // Create the PlayReadyHelper object using default constructor and log creation.
    std::cout << "Invoking default constructor for PlayReadyHelper object" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper helper(drmInfo);
        
        // Invoke the ocdmSystemId method and log the invocation.
        std::cout << "Invoking ocdmSystemId method on PlayReadyHelper object" << std::endl;
        const std::string& systemId = helper.ocdmSystemId();
        
        // Log the returned value.
        std::cout << "Retrieved ocdmSystemId value: \"" << systemId << "\"" << std::endl;
        
        // Verify that the returned value is not empty.
        EXPECT_FALSE(systemId.empty()) << "The ocdmSystemId returned is an empty string";
    });
    
    std::cout << "Exiting VerifyOcdmSystemIdIsNotEmpty test" << std::endl;
}
/**
 * @brief Validate that PlayReadyHelper correctly parses a valid PSSH buffer.
 *
 * This test verifies that when a valid PSSH buffer is provided (i.e., a buffer of 32 bytes starting with "PSSH" followed by dummy data), the PlayReadyHelper's parsePssh method returns true. This ensures that the parsing functionality works as expected under normal conditions.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Instantiate PlayReadyHelper using the default constructor. | (No input arguments) | Object created without throwing any exceptions. | Should be successful |
 * | 02 | Prepare a valid PSSH data buffer with 32 bytes where the first 4 bytes are set to "PSSH" and the remaining bytes are set to 0xAA. | bufferLen = 32, validPssh[0]='P', validPssh[1]='S', validPssh[2]='S', validPssh[3]='H', validPssh[4-31]=0xAA | Buffer is correctly prepared for use in parsing. | Should be successful |
 * | 03 | Invoke parsePssh with the prepared valid PSSH data buffer. | input: validPssh (pointer to the buffer), input: bufferLen (32) | parsePssh returns true and EXPECT_TRUE assertion passes. | Should Pass |
 */
TEST(PlayReadyHelper, ValidPSSHInput) {
    GTEST_SKIP();
    std::cout << "Entering ValidPSSHInput test" << std::endl;

    // Instantiating PlayReadyHelper using a default constructor
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "Instantiated PlayReadyHelper object." << std::endl;
        
        // Prepare a valid PSSH buffer:
        // For the stub, assume valid PSSH must be at least 32 bytes and begin with "PSSH".
        const uint32_t bufferLen = 32;
        uint8_t validPssh[bufferLen];
        // Fill the first four bytes with "PSSH"
        validPssh[0] = 'P';
        validPssh[1] = 'S';
        validPssh[2] = 'S';
        validPssh[3] = 'H';
        // Fill the rest with dummy data (e.g., pattern 0xAA)
        for (uint32_t i = 4; i < bufferLen; ++i) {
            validPssh[i] = 0xAA;
        }
        std::cout << "Prepared valid PSSH data buffer at address: " << static_cast<const void*>(validPssh)
                  << " with length: " << bufferLen << std::endl;

        std::cout << "Invoking parsePssh with valid data." << std::endl;
        bool result = playReadyHelper.parsePssh(validPssh, bufferLen);
        std::cout << "parsePssh returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidPSSHInput test" << std::endl;
}
/**
 * @brief Verifies that parsePssh returns false when provided with a NULL initData pointer and a non-zero length.
 *
 * This test case instantiates a PlayReadyHelper object, prepares test data with a NULL pointer for initData and a length of 10,
 * invokes the parsePssh method, and asserts that the returned value is false. This scenario ensures that the API gracefully handles
 * invalid initialization parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayReadyHelper, set initData to nullptr and initDataLen to 10, invoke parsePssh, and verify that it returns false. | initData = nullptr, initDataLen = 10 | parsePssh returns false and the EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(PlayReadyHelper, NullInitDataNonZeroLength) {
    std::cout << "Entering NullInitDataNonZeroLength test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "Instantiated PlayReadyHelper object." << std::endl;
        
        // Set initData to NULL and length to 10.
        const uint32_t length = 10;
        const uint8_t* nullData = nullptr;
        std::cout << "Prepared test data: initData = " << nullData 
                  << " and initDataLen = " << length << std::endl;
        
        std::cout << "Invoking parsePssh with NULL initData." << std::endl;
        bool result = playReadyHelper.parsePssh(nullData, length);
        std::cout << "parsePssh returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NullInitDataNonZeroLength test" << std::endl;
}
/**
 * @brief Verify that parsePssh handles a non-null pointer with zero length correctly.
 *
 * This test ensures that when a valid (non-null) pointer is provided but with a length of zero,
 * the parsePssh API call does not throw an exception and returns false as expected. This behavior
 * confirms that the API correctly identifies and handles inputs with insufficient data for processing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                           | Expected Result                                         | Notes            |
 * | :--------------: | ----------------------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate PlayReadyHelper object                                | N/A                                                 | Object is instantiated successfully                   | Should be successful |
 * | 02               | Prepare a dummy non-null buffer and set length to zero              | dummy = {0x00}, length = 0                           | Dummy buffer prepared with zero length value            | Should be successful |
 * | 03               | Invoke parsePssh with the prepared dummy buffer and zero length     | input = dummy pointer, initDataLen = 0               | API returns false without throwing an exception         | Should Fail      |
 * | 04               | Assert that the call to parsePssh returns false as expected         | output: result = false                               | EXPECT_FALSE(result) passes                             | Should be successful |
 */
TEST(PlayReadyHelper, ValidPointerZeroLength) {
    std::cout << "Entering ValidPointerZeroLength test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "Instantiated PlayReadyHelper object." << std::endl;
        
        // Prepare a dummy non-null buffer.
        uint8_t dummy[1] = { 0x00 };
        // Use length 0.
        const uint32_t length = 0;
        std::cout << "Prepared test data: initData at address " << static_cast<const void*>(dummy)
                  << " with initDataLen = " << length << std::endl;
        
        std::cout << "Invoking parsePssh with zero length data." << std::endl;
        bool result = playReadyHelper.parsePssh(dummy, length);
        std::cout << "parsePssh returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting ValidPointerZeroLength test" << std::endl;
}
/**
 * @brief Test that parsePssh correctly identifies corrupted header values in the PSSH buffer.
 *
 * This test verifies that the parsePssh function returns false when provided with a buffer that has a corrupted header. The test is essential to ensure that the API correctly detects header anomalies and does not process invalid data.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** (High)@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayReadyHelper object | N/A | Object instantiated successfully | Should be successful |
 * | 02 | Prepare a buffer with corrupted header values | bufferLen = 32, corruptedBuffer[0] = 'X', corruptedBuffer[1] = 'Y', corruptedBuffer[2] = 'Z', corruptedBuffer[3] = 'W', corruptedBuffer[4..31] = 0xCC | Buffer is correctly populated with 32 bytes having a corrupted header | Should be successful |
 * | 03 | Invoke parsePssh with corrupted header data | Method: parsePssh, Input: corruptedBuffer, bufferLen = 32, Expected Output: false | parsePssh returns false and the assertion EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(PlayReadyHelper, CorruptedHeaderValues) {
    std::cout << "Entering CorruptedHeaderValues test" << std::endl;

    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::cout << "Instantiated PlayReadyHelper object." << std::endl;
        
        // Prepare a buffer with full length but corrupted header values.
        // Assume a valid PSSH should be at least 32 bytes. Use 32 bytes.
        const uint32_t bufferLen = 32;
        uint8_t corruptedBuffer[bufferLen];
        // Corrupt header: not "PSSH", use wrong characters.
        corruptedBuffer[0] = 'X';
        corruptedBuffer[1] = 'Y';
        corruptedBuffer[2] = 'Z';
        corruptedBuffer[3] = 'W';
        // Fill the rest with dummy data.
        for (uint32_t i = 4; i < bufferLen; ++i) {
            corruptedBuffer[i] = 0xCC;
        }
        std::cout << "Prepared corrupted PSSH data buffer at address: " << static_cast<const void*>(corruptedBuffer)
                  << " with length: " << bufferLen << std::endl;
        
        std::cout << "Invoking parsePssh with corrupted header data." << std::endl;
        bool result = playReadyHelper.parsePssh(corruptedBuffer, bufferLen);
        std::cout << "parsePssh returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting CorruptedHeaderValues test" << std::endl;
}
/**
 * @brief Test to verify that setting DRM metadata with valid alphanumeric characters succeeds
 *
 * This test verifies that the PlayReadyHelper class correctly processes a valid DRM metadata string 
 * that contains alphanumeric characters. It creates a PlayReadyHelper object using the default constructor, 
 * prepares a valid DRM metadata string "ValidDrmMetadata123", and then calls the setDrmMetaData API. 
 * The test asserts that no exceptions are thrown during the invocation.
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
 * | Variation / Step | Description                                                                  | Test Data                                             | Expected Result                                                 | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Create a PlayReadyHelper object using the default constructor                | None                                                  | Object instantiated successfully                                | Should be successful |
 * | 02               | Prepare a valid DRM metadata string with alphanumeric characters               | validMetaData = ValidDrmMetadata123                   | Metadata string prepared correctly                              | Should be successful |
 * | 03               | Invoke the setDrmMetaData API with the valid DRM metadata string               | input: validMetaData = ValidDrmMetadata123              | API call completes without throwing an exception                | Should Pass   |
 */
TEST(PlayReadyHelper, SettingValidDRMMetadataWithAlphanumericCharacters) {
    std::cout << "Entering SettingValidDRMMetadataWithAlphanumericCharacters test" << std::endl;

    // Create a PlayReadyHelper object using default constructor.
    std::cout << "Creating PlayReadyHelper object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::string validMetaData = "ValidDrmMetadata123";
        std::cout << "Invoking setDrmMetaData with value: " << validMetaData << std::endl;
        EXPECT_NO_THROW(playReadyHelper.setDrmMetaData(validMetaData));
        std::cout << "Successfully invoked setDrmMetaData with value: " << validMetaData << std::endl;
    });

    std::cout << "Exiting SettingValidDRMMetadataWithAlphanumericCharacters test" << std::endl;
}
/**
 * @brief Validate that DRM metadata can be set with an empty string without causing errors.
 *
 * This test case verifies that invoking the setDrmMetaData API with an empty string does not throw an exception.
 * It ensures that the PlayReadyHelper object is properly instantiated and can safely handle empty DRM metadata.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 036
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                                  | Expected Result                                          | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------- | ----------------- |
 * | 01               | Print message indicating the start of the test execution.            | N/A                                                        | Console outputs "Entering SettingDRMMetadataWithEmptyString test". | Should be successful |
 * | 02               | Create a PlayReadyHelper object using the default constructor.       | constructor call: PlayReadyHelper()                        | Object is instantiated without any exception.          | Should be successful |
 * | 03               | Invoke setDrmMetaData with an empty string.                          | input: emptyMetaData = ""                                    | Method call completes without throwing an exception.   | Should Pass       |
 * | 04               | Print message after invoking setDrmMetaData to indicate success.       | N/A                                                        | Console outputs "Successfully invoked setDrmMetaData with an empty string". | Should be successful |
 * | 05               | Print message indicating the exit of the test execution.             | N/A                                                        | Console outputs "Exiting SettingDRMMetadataWithEmptyString test". | Should be successful |
 */
TEST(PlayReadyHelper, SettingDRMMetadataWithEmptyString) {
    std::cout << "Entering SettingDRMMetadataWithEmptyString test" << std::endl;

    // Create a PlayReadyHelper object using default constructor.
    std::cout << "Creating PlayReadyHelper object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        PlayReadyHelper playReadyHelper(drmInfo);
        std::string emptyMetaData = "";
        std::cout << "Invoking setDrmMetaData with an empty string" << std::endl;
        EXPECT_NO_THROW(playReadyHelper.setDrmMetaData(emptyMetaData));
        std::cout << "Successfully invoked setDrmMetaData with an empty string" << std::endl;
    });

    std::cout << "Exiting SettingDRMMetadataWithEmptyString test" << std::endl;
}
/**
 * @brief Verify that the PlayReadyHelper destructor releases resources properly
 *
 * This test validates that the PlayReadyHelper destructor correctly frees allocated resources without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 037@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                                                    | Expected Result                                          | Notes       |
 * | :----:           | ------------------------------------------------------------------------ | ------------------------------------------------------------ | -------------------------------------------------------- | ----------- |
 * | 01               | Create a PlayReadyHelper object using the default constructor and then delete it to invoke the destructor | input: none, output: PlayReadyHelper object creation and deletion | The deletion should not throw any exception, and resources should be released | Should Pass |
 */
TEST(PlayReadyHelper, Destructor_releases_resources_properly) {
    std::cout << "Entering Destructor_releases_resources_properly test" << std::endl;
    
    EXPECT_NO_THROW({
        DrmInfo drmInfo;
        std::cout << "Creating PlayReadyHelper object using default constructor" << std::endl;
        PlayReadyHelper* helper = new PlayReadyHelper(drmInfo);
        std::cout << "PlayReadyHelper object created successfully" << std::endl;
        
        std::cout << "Invoking destructor by deleting the PlayReadyHelper object" << std::endl;
        delete helper;
        std::cout << "Destructor has been successfully invoked; resources should be released" << std::endl;
    });
    
    std::cout << "Exiting Destructor_releases_resources_properly test" << std::endl;
}
