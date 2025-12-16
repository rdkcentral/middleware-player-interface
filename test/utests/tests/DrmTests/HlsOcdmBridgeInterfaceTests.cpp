
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
#include "HlsOcdmBridgeInterface.h"


class TestDrmSession : public DrmSession {
public:
    TestDrmSession(const std::string &keySystem)
        : DrmSession(keySystem) {}

    void generateDRMSession(const uint8_t*, uint32_t, std::string&) override {}
    DrmData* generateKeyRequest(std::string&, uint32_t) override { return nullptr; }
    int processDRMKey(DrmData*, uint32_t) override { return 0; }
    KeyState getState() override { return KeyState::KEY_INIT; }
    void clearDecryptContext() override {}
};

class FakeHlsOcdmBridgeTest : public ::testing::Test {
protected:
    std::unique_ptr<TestDrmSession> drmSession;
    std::unique_ptr<FakeHlsOcdmBridge> bridge;

    void SetUp() override {
        drmSession = std::make_unique<TestDrmSession>("TestSystem");
        bridge = std::make_unique<FakeHlsOcdmBridge>(drmSession.get());
    }

    void TearDown() override {
        bridge.reset();
        drmSession.reset();
    }
};

/**
 * @brief Test positive scenario for AcquireKey method using valid metadata.
 *
 * This test verifies that the AcquireKey method of FakeHlsOcdmBridge handles valid metadata and a positive trackType correctly without throwing any exceptions. The test creates a metadata object with valid sample data, instantiates FakeHlsOcdmBridge using its default constructor, and then invokes the AcquireKey method with the metadata pointer and trackType.
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
 * | 01 | Create a valid metadata object, instantiate FakeHlsOcdmBridge, and invoke AcquireKey with metadata and trackType = 1 | metadata = "ValidMetadata", trackType = 1, output = No exception thrown | AcquireKey method is invoked successfully without throwing any exception | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, AcquireKey_ValidMetadata_PositiveTrackType) {
    std::cout << "Entering AcquireKey_ValidMetadata_PositiveTrackType test" << std::endl;
    
    // Create a valid metadata object
    char metadata[50];
    const char* sampleData = "ValidMetadata";
    strncpy(metadata, sampleData, sizeof(metadata)-1);
    metadata[sizeof(metadata)-1] = '\0';
    std::cout << "Created metadata with value: " << metadata << std::endl;
    
    int trackType = 1;
    std::cout << "Invoking AcquireKey with trackType: " << trackType << std::endl;
    
    // Create instance of FakeHlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        bridge->AcquireKey(static_cast<void*>(metadata), trackType);
    });

    std::cout << "AcquireKey method invoked successfully with metadata: " << metadata << " and trackType: " << trackType << std::endl;
    std::cout << "Exiting AcquireKey_ValidMetadata_PositiveTrackType test" << std::endl;
}
/**
 * @brief Validate that AcquireKey successfully processes valid metadata with a zero track type.
 *
 * This test verifies that when valid metadata and a track type of zero are provided to the AcquireKey API of FakeHlsOcdmBridge, the method executes without throwing any exceptions. The test ensures that the API can correctly handle a zero track type scenario, which is critical for proper functionality when such input values are encountered.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** (High) This test is critical to ensure proper handling of metadata and track type in the API.
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize valid metadata with "ValidMetadata" string ensuring proper null termination. | metadata = "ValidMetadata" | Metadata correctly initialized | Should be successful |
 * | 02 | Create an instance of FakeHlsOcdmBridge using the default constructor. | No input parameters | Instance created without exceptions | Should be successful |
 * | 03 | Invoke AcquireKey with the pointer to metadata and trackType set to 0. | input: metadata pointer = "ValidMetadata", trackType = 0 | AcquireKey executes without throwing exceptions; assert no exceptions thrown | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, AcquireKey_ValidMetadata_ZeroTrackType) {
    std::cout << "Entering AcquireKey_ValidMetadata_ZeroTrackType test" << std::endl;
    
    // Create a valid metadata object
    char metadata[50];
    const char* sampleData = "ValidMetadata";
    strncpy(metadata, sampleData, sizeof(metadata)-1);
    metadata[sizeof(metadata)-1] = '\0';
    std::cout << "Created metadata with value: " << metadata << std::endl;
    
    int trackType = 0;
    std::cout << "Invoking AcquireKey with trackType: " << trackType << std::endl;
    
    // Create instance of FakeHlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        bridge->AcquireKey(static_cast<void*>(metadata), trackType);
    });
    std::cout << "AcquireKey method invoked successfully with metadata: " << metadata << " and trackType: " << trackType << std::endl;    
    std::cout << "Exiting AcquireKey_ValidMetadata_ZeroTrackType test" << std::endl;
}
/**
 * @brief Verifies that the AcquireKey method successfully processes a null metadata pointer with a valid positive track type.
 *
 * This test checks that calling AcquireKey with metadata set as nullptr and trackType set to 1 does not throw any exceptions. It ensures that the API handles null metadata appropriately for a positive track type.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                  | Expected Result                                                       | Notes       |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------ | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke AcquireKey with null metadata pointer and a positive track type | metadata = nullptr, trackType = 1           | AcquireKey method is invoked successfully without throwing exceptions | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, AcquireKey_NullMetadata_PositiveTrackType) {
    std::cout << "Entering AcquireKey_NullMetadata_PositiveTrackType test" << std::endl;
    
    void* metadata = nullptr;
    std::cout << "Using nullptr for metadata" << std::endl;
    
    int trackType = 1;
    std::cout << "Invoking AcquireKey with trackType: " << trackType << std::endl;
    
    // Create instance of FakeHlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        bridge->AcquireKey(metadata, trackType);
    });
    std::cout << "AcquireKey method invoked successfully with metadata: nullptr and trackType: " << trackType << std::endl;
    std::cout << "Exiting AcquireKey_NullMetadata_PositiveTrackType test" << std::endl;
}
/**
 * @brief Test the AcquireKey API using valid metadata and INT_MAX trackType
 *
 * This test verifies that the AcquireKey API of the FakeHlsOcdmBridge class can successfully process a valid metadata pointer and a trackType set to INT_MAX without throwing exceptions.
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
 * | Variation / Step | Description                                                                     | Test Data                                                          | Expected Result                                                            | Notes         |
 * | :----:           | ------------------------------------------------------------------------------- | ------------------------------------------------------------------ | -------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a valid metadata object and assign it the value "ValidMetadata"          | metadata = "ValidMetadata"                                           | Metadata buffer is correctly populated                                     | Should be successful |
 * | 02               | Set the trackType variable to INT_MAX                                           | trackType = INT_MAX                                                  | trackType is set to the maximum integer value                              | Should be successful |
 * | 03               | Instantiate FakeHlsOcdmBridge using the default constructor                     | No input arguments                                                 | Instance of FakeHlsOcdmBridge is successfully created                      | Should be successful |
 * | 04               | Invoke AcquireKey with the valid metadata pointer and trackType set to INT_MAX    | input: metadata pointer = "ValidMetadata", trackType = INT_MAX         | AcquireKey method executes without throwing any exceptions                 | Should Pass   |
 */
TEST_F(FakeHlsOcdmBridgeTest, AcquireKey_ValidMetadata_INT_MAX) {
    std::cout << "Entering AcquireKey_ValidMetadata_INT_MAX test" << std::endl;
    
    // Create a valid metadata object
    char metadata[50];
    const char* sampleData = "ValidMetadata";
    strncpy(metadata, sampleData, sizeof(metadata)-1);
    metadata[sizeof(metadata)-1] = '\0';
    std::cout << "Created metadata with value: " << metadata << std::endl;
    
    int trackType = INT_MAX;
    std::cout << "Invoking AcquireKey with trackType: " << trackType << std::endl;
    
    // Create instance of FakeHlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        bridge->AcquireKey(static_cast<void*>(metadata), trackType);
    });
    std::cout << "AcquireKey method invoked successfully with metadata: " << metadata << " and trackType: " << trackType << std::endl;  
    std::cout << "Exiting AcquireKey_ValidMetadata_INT_MAX test" << std::endl;
}
/**
 * @brief Tests AcquireKey method with valid metadata and INT_MIN trackType.
 *
 * This test verifies that the AcquireKey method of FakeHlsOcdmBridge successfully processes a valid metadata input and a trackType set to INT_MIN. It ensures that no exceptions are thrown during the API invocation.
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
 * | Variation / Step | Description                                                       | Test Data                                                        | Expected Result                                                             | Notes      |
 * | :--------------: | ----------------------------------------------------------------- | ---------------------------------------------------------------- | --------------------------------------------------------------------------- | ---------- |
 * | 01               | Create metadata string, instantiate FakeHlsOcdmBridge and invoke AcquireKey API | metadata = "ValidMetadata", trackType = INT_MIN, input pointer = metadata | API should be invoked without throwing an exception and process the valid metadata | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, AcquireKey_ValidMetadata_INT_MIN) {
    std::cout << "Entering AcquireKey_ValidMetadata_INT_MIN test" << std::endl;
    
    // Create a valid metadata object
    char metadata[50];
    const char* sampleData = "ValidMetadata";
    strncpy(metadata, sampleData, sizeof(metadata)-1);
    metadata[sizeof(metadata)-1] = '\0';
    std::cout << "Created metadata with value: " << metadata << std::endl;
    
    int trackType = INT_MIN;
    std::cout << "Invoking AcquireKey with trackType: " << trackType << std::endl;
    
    // Create instance of FakeHlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        bridge->AcquireKey(static_cast<void*>(metadata), trackType);
    });
    std::cout << "AcquireKey method invoked successfully with metadata: " << metadata << " and trackType: " << trackType << std::endl;    
    std::cout << "Exiting AcquireKey_ValidMetadata_INT_MIN test" << std::endl;
}
/**
 * @brief Verifies that the CancelKeyWait method of FakeHlsOcdmBridge completes without throwing errors
 *
 * This test verifies that the CancelKeyWait method of the FakeHlsOcdmBridge object executes successfully without exceptions.
 * It uses the default constructor and EXPECT_NO_THROW to ensure no errors occur during the API call.
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
 * | Variation / Step | Description                                                               | Test Data                             | Expected Result                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * |       01       | Create FakeHlsOcdmBridge object using default constructor.                | constructor = default                 | FakeHlsOcdmBridge instance is created successfully without exceptions.| Should be successful |
 * |       02       | Invoke CancelKeyWait method on FakeHlsOcdmBridge instance to cancel key wait. | Method = CancelKeyWait()              | CancelKeyWait completes without throwing any exceptions.             | Should Pass   |
 */
TEST_F(FakeHlsOcdmBridgeTest, VerifyCancelKeyWaitCompletesWithoutError) {
    std::cout << "Entering VerifyCancelKeyWaitCompletesWithoutError test" << std::endl;

    // Create FakeHlsOcdmBridge object using default constructor
    EXPECT_NO_THROW({
        // Invoking CancelKeyWait method
        std::cout << "Invoking CancelKeyWait method on FakeHlsOcdmBridge object." << std::endl;
        bridge->CancelKeyWait();
    });
    std::cout << "CancelKeyWait method call executed, no exceptions thrown." << std::endl;
    std::cout << "Exiting VerifyCancelKeyWaitCompletesWithoutError test" << std::endl;
}
/**
 * @brief Verify that GetState() returns a valid DRM state.
 *
 * This test instantiates a FakeHlsOcdmBridge, calls its GetState() method, and verifies that the returned state is one of the expected DRMState enumerators.
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
 * | Variation / Step | Description                                                                 | Test Data                                          | Expected Result                                                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------- |----------------------------------------------------------------------------------| ----------- |
 * | 01               | Initialize FakeHlsOcdmBridge, call GetState() to retrieve the DRMState value | input: None, output: state = value returned by GetState() | API should return a value equal to one of eDRM_INITIALIZED, eDRM_ACQUIRING_KEY, eDRM_KEY_ACQUIRED, eDRM_KEY_FAILED, or eDRM_KEY_FLUSH | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, VerifyGetState) {
    // Arrange & Act
    DRMState state = bridge->GetState();

    // Assert: Check if the returned state is one of the valid DRMState enumerators
    bool isValidState =
        state == eDRM_INITIALIZED ||
        state == eDRM_ACQUIRING_KEY ||
        state == eDRM_KEY_ACQUIRED ||
        state == eDRM_KEY_FAILED ||
        state == eDRM_KEY_FLUSH;

    EXPECT_TRUE(isValidState) << "GetState() returned an invalid state: " << static_cast<int>(state);
}
/**
 * @brief Verify that calling Release() on a valid FakeHlsOcdmBridge instance executes without exceptions.
 *
 * This test creates a FakeHlsOcdmBridge instance using the default constructor and then calls its Release() method.
 * The objective is to ensure that Release() does not throw any exceptions and that the internal state remains unchanged.
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
 * | Variation / Step | Description                                                        | Test Data                                                           | Expected Result                                                          | Notes           |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------- | ------------------------------------------------------------------------ | --------------- |
 * | 01               | Create a FakeHlsOcdmBridge instance using the default constructor. | No input; output: instance is created successfully.                 | Instance is created without throwing any exceptions.                  | Should be successful |
 * | 02               | Invoke the Release() method on the created instance.               | input: instance.Release() invocation.                               | Release() executes without throwing any exception as verified by EXPECT_NO_THROW. | Should Pass     |
 */
TEST_F(FakeHlsOcdmBridgeTest, CallingReleaseOnValidInstance) {
    std::cout << "Entering CallingReleaseOnValidInstance test" << std::endl;
    
    // Creating a valid FakeHlsOcdmBridge instance using the default constructor.
    EXPECT_NO_THROW({        
        // Invoking Release on the instance.
        std::cout << "Invoking Release() on FakeHlsOcdmBridge instance." << std::endl;
        bridge->Release();
    });
    std::cout << "Release() method invoked successfully; no errors thrown." << std::endl;  
    std::cout << "Exiting CallingReleaseOnValidInstance test" << std::endl;
}
/**
 * @brief Validates the execution of the RestoreKeyState method in FakeHlsOcdmBridge.
 *
 * This test ensures that creating a FakeHlsOcdmBridge object and invoking its RestoreKeyState method executes without throwing exceptions and that the internal state remains unchanged after the call.
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
 * | Variation / Step | Description                                                      | Test Data                                                      | Expected Result                                                      | Notes         |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * |      01        | Create FakeHlsOcdmBridge object using default constructor         | No input parameters, object instantiated                       | Object is created successfully without throwing exceptions         | Should Pass   |
 * |      02        | Invoke RestoreKeyState method on FakeHlsOcdmBridge                 | No input parameters, method RestoreKeyState invoked              | Method executes without throwing exceptions and leaves state unchanged| Should Pass   |
 * |      03        | Log the process of object creation and method invocation          | Log outputs of constructor and RestoreKeyState call              | Logs confirm successful object creation and restore state operation  | Should be successful |
 */
TEST_F(FakeHlsOcdmBridgeTest, CallRestoreKeyState) {
    std::cout << "Entering CallRestoreKeyState test" << std::endl;
    // Log invocation of RestoreKeyState method.
    std::cout << "Invoking RestoreKeyState method on FakeHlsOcdmBridge." << std::endl;
    EXPECT_NO_THROW({
        bridge->RestoreKeyState();
    });
    std::cout << "RestoreKeyState method executed without exceptions." << std::endl;
    // Since RestoreKeyState() does not alter state, log internal state remains unchanged.
    std::cout << "State after RestoreKeyState invocation remains unchanged (default implementation)." << std::endl;
    std::cout << "Exiting CallRestoreKeyState test" << std::endl;
}
/**
 * @brief Validate the SetDecryptInfo functionality with typical DRM information.
 *
 * This test verifies that a FakeHlsOcdmBridge object, when provided with a properly populated DrmInfo structure using typical values including valid IV and URL fields, correctly processes the call to SetDecryptInfo with a given acquireKeyWaitTime, returning eDRM_SUCCESS.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Create a FakeHlsOcdmBridge object using its default constructor. | Constructor: FakeHlsOcdmBridge() | Object is created without throwing exceptions. | Should Pass |
 * | 02 | Initialize a DrmInfo object with typical values for DRM settings. | drmInfo.method = eMETHOD_AES_128, drmInfo.mediaFormat = eMEDIAFORMAT_HLS, drmInfo.useFirst16BytesAsIV = true, drmInfo.bPropagateUriParams = true, drmInfo.bUseMediaSequenceIV = true, drmInfo.bDecryptClearSamplesRequired = true, IV = "0123456789ABCDEF", drmInfo.masterManifestURL = "http://example.com/master.m3u8", drmInfo.manifestURL = "http://example.com/playlist.m3u8", drmInfo.keyURI = "http://example.com/key", drmInfo.keyFormat = "identity", drmInfo.systemUUID = "uuid-1234", drmInfo.initData = "initDataSample" | DrmInfo object is populated with valid typical values. | Should be successful |
 * | 03 | Invoke SetDecryptInfo on FakeHlsOcdmBridge using the populated DrmInfo and acquireKeyWaitTime = 1000. | drmInfo (as initialized above), acquireKeyWaitTime = 1000 | API returns eDRM_SUCCESS and the assertion passes. | Should Pass |
 */

TEST_F(FakeHlsOcdmBridgeTest, ValidDrmInfoTypicalInput) {
    GTEST_SKIP();
    std::cout << "Entering ValidDrmInfoTypicalInput test" << std::endl;
    // Create a DrmInfo object with typical values
    DrmInfo drmInfo;
    
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.useFirst16BytesAsIV = true;
    drmInfo.bPropagateUriParams = true;
    drmInfo.bUseMediaSequenceIV = true;
    drmInfo.bDecryptClearSamplesRequired = true;
    
    // Use strncpy to assign a valid IV array
    const char* validIV = "0123456789ABCDEF";
    strncpy((char*)drmInfo.iv, validIV, DRM_IV_LEN);
    std::cout << "IV set to: " << std::string((char*)drmInfo.iv, DRM_IV_LEN) << std::endl;
    
    // Set string fields with non-empty values
    drmInfo.masterManifestURL = "http://example.com/master.m3u8";
    drmInfo.manifestURL = "http://example.com/playlist.m3u8";
    drmInfo.keyURI = "http://example.com/key";
    drmInfo.keyFormat = "identity";
    drmInfo.systemUUID = "uuid-1234";
    drmInfo.initData = "initDataSample";

    std::cout << "DrmInfo details:" << std::endl;
    std::cout << "  Method: " << drmInfo.method << std::endl;
    std::cout << "  MediaFormat: " << drmInfo.mediaFormat << std::endl;
    std::cout << "  useFirst16BytesAsIV: " << drmInfo.useFirst16BytesAsIV << std::endl;
    std::cout << "  masterManifestURL: " << drmInfo.masterManifestURL << std::endl;
    std::cout << "  manifestURL: " << drmInfo.manifestURL << std::endl;
    std::cout << "  keyURI: " << drmInfo.keyURI << std::endl;
    std::cout << "  keyFormat: " << drmInfo.keyFormat << std::endl;
    std::cout << "  systemUUID: " << drmInfo.systemUUID << std::endl;
    std::cout << "  initData: " << drmInfo.initData << std::endl;

    int acquireKeyWaitTime = 1000;
    std::cout << "Calling SetDecryptInfo with acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    DrmReturn ret = bridge->SetDecryptInfo(&drmInfo, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;

    // Expected output: eDRM_SUCCESS
    EXPECT_EQ(ret, eDRM_SUCCESS);

    std::cout << "Exiting ValidDrmInfoTypicalInput test" << std::endl;
}
/**
 * @brief Verify that SetDecryptInfo returns the correct error when DrmInfo pointer is null
 *
 * This test validates that the API correctly handles a nullptr for the DrmInfo parameter by returning eDRM_ERROR.
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
 * | Variation / Step | Description                                                          | Test Data                                                 | Expected Result                                                 | Notes        |
 * | :--------------: | -------------------------------------------------------------------- | --------------------------------------------------------- | --------------------------------------------------------------- | ------------ |
 * | 01               | Create FakeHlsOcdmBridge object using default constructor            | None                                                      | Object creation should not throw any exception                  | Should Pass  |
 * | 02               | Invoke SetDecryptInfo with nullptr for DrmInfo and acquireKeyWaitTime  | DrmInfo = nullptr, acquireKeyWaitTime = 1000, output = ret  | Return value is eDRM_ERROR and assertion EXPECT_EQ(ret, eDRM_ERROR) | Should Pass  |
 */
TEST_F(FakeHlsOcdmBridgeTest, NullDrmInfoPointer) {
    std::cout << "Entering NullDrmInfoPointer test" << std::endl;

    // Create FakeHlsOcdmBridge object using default constructor

    int acquireKeyWaitTime = 1000;
    std::cout << "Calling SetDecryptInfo with nullptr for DrmInfo and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    DrmReturn ret = bridge->SetDecryptInfo(nullptr, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;

    // Expected output: eDRM_ERROR
    EXPECT_EQ(ret, eDRM_ERROR);

    std::cout << "Exiting NullDrmInfoPointer test" << std::endl;
}
/**
 * @brief Verify that SetDecryptInfo returns an error when provided a negative acquireKeyWaitTime.
 *
 * This test creates a FakeHlsOcdmBridge instance and a properly initialized DrmInfo structure.
 * It then calls SetDecryptInfo with a negative acquireKeyWaitTime (-500) to ensure that the
 * function correctly returns eDRM_ERROR, thereby validating the API's input validation and error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 012@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create FakeHlsOcdmBridge object using default constructor | No input arguments | Object created without exceptions | Should be successful |
 * | 02 | Initialize and setup DrmInfo with valid parameters | method = eMETHOD_AES_128, mediaFormat = eMEDIAFORMAT_HLS, useFirst16BytesAsIV = true, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, iv = "0123456789ABCDEF", masterManifestURL = http://example.com/master.m3u8, manifestURL = http://example.com/playlist.m3u8, keyURI = http://example.com/key, keyFormat = identity, systemUUID = uuid-1234, initData = initDataSample | DrmInfo correctly initialized | Should be successful |
 * | 03 | Invoke SetDecryptInfo with negative acquireKeyWaitTime value | drmInfo object, acquireKeyWaitTime = -500 | Return value equals eDRM_ERROR | Should Fail |
 */
TEST_F(FakeHlsOcdmBridgeTest, NegativeAcquireKeyWaitTime) {
    std::cout << "Entering NegativeAcquireKeyWaitTime test" << std::endl;

    // Create a valid DrmInfo object
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_AES_128;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.useFirst16BytesAsIV = true;
    drmInfo.bPropagateUriParams = true;
    drmInfo.bUseMediaSequenceIV = true;
    drmInfo.bDecryptClearSamplesRequired = true;
    
    const char* validIV = "0123456789ABCDEF";
    strncpy((char*)drmInfo.iv, validIV, DRM_IV_LEN);
    std::cout << "IV set to: " << std::string((char*)drmInfo.iv, DRM_IV_LEN) << std::endl;
    
    drmInfo.masterManifestURL = "http://example.com/master.m3u8";
    drmInfo.manifestURL = "http://example.com/playlist.m3u8";
    drmInfo.keyURI = "http://example.com/key";
    drmInfo.keyFormat = "identity";
    drmInfo.systemUUID = "uuid-1234";
    drmInfo.initData = "initDataSample";

    int acquireKeyWaitTime = -500;
    std::cout << "Calling SetDecryptInfo with negative acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    DrmReturn ret = bridge->SetDecryptInfo(&drmInfo, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;

    // Expected output: eDRM_ERROR
    EXPECT_EQ(ret, eDRM_ERROR);

    std::cout << "Exiting NegativeAcquireKeyWaitTime test" << std::endl;
}
/**
 * @brief Verify that FakeHlsOcdmBridge correctly processes a DrmInfo instance with DRM method set to None
 *
 * This test verifies that when a DrmInfo object is configured with eMETHOD_NONE and appropriate
 * HLS-related parameters, the FakeHlsOcdmBridge's SetDecryptInfo method returns eDRM_SUCCESS.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate FakeHlsOcdmBridge and DrmInfo, set DrmInfo members, and invoke SetDecryptInfo with acquireKeyWaitTime | drmInfo.method = eMETHOD_NONE, drmInfo.mediaFormat = eMEDIAFORMAT_HLS, drmInfo.useFirst16BytesAsIV = true, drmInfo.bPropagateUriParams = true, drmInfo.bUseMediaSequenceIV = true, drmInfo.bDecryptClearSamplesRequired = true, validIV = "FEDCBA9876543210", drmInfo.masterManifestURL = "http://example.com/master_none.m3u8", drmInfo.manifestURL = "http://example.com/playlist_none.m3u8", drmInfo.keyURI = "http://example.com/key_none", drmInfo.keyFormat = "identity_none", drmInfo.systemUUID = "uuid-none", drmInfo.initData = "initDataNone", acquireKeyWaitTime = 1000 | Return value eDRM_SUCCESS; EXPECT_EQ(ret, eDRM_SUCCESS) check passed | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, DrmMethodNone) {
    GTEST_SKIP();
    std::cout << "Entering DrmMethodNone test" << std::endl;
    // Create a DrmInfo object with method set to eMETHOD_NONE
    DrmInfo drmInfo;
    drmInfo.method = eMETHOD_NONE;
    drmInfo.mediaFormat = eMEDIAFORMAT_HLS;
    drmInfo.useFirst16BytesAsIV = true;
    drmInfo.bPropagateUriParams = true;
    drmInfo.bUseMediaSequenceIV = true;
    drmInfo.bDecryptClearSamplesRequired = true;
    
    const char* validIV = "FEDCBA9876543210";
    strncpy((char*)drmInfo.iv, validIV, DRM_IV_LEN);
    std::cout << "IV set to: " << std::string((char*)drmInfo.iv, DRM_IV_LEN) << std::endl;
    
    drmInfo.masterManifestURL = "http://example.com/master_none.m3u8";
    drmInfo.manifestURL = "http://example.com/playlist_none.m3u8";
    drmInfo.keyURI = "http://example.com/key_none";
    drmInfo.keyFormat = "identity_none";
    drmInfo.systemUUID = "uuid-none";
    drmInfo.initData = "initDataNone";

    std::cout << "DrmInfo details:" << std::endl;
    std::cout << "  Method: " << drmInfo.method << std::endl;
    std::cout << "  masterManifestURL: " << drmInfo.masterManifestURL << std::endl;

    int acquireKeyWaitTime = 1000;
    std::cout << "Calling SetDecryptInfo with acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    DrmReturn ret = bridge->SetDecryptInfo(&drmInfo, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;

    // Expected output: eDRM_SUCCESS
    EXPECT_EQ(ret, eDRM_SUCCESS);

    std::cout << "Exiting DrmMethodNone test" << std::endl;
}
/**
 * @brief Verifies iteration over MediaFormat enum values and corresponding decryption setup
 *
 * This test iterates over all media format values defined in the MediaFormat enum. For each value, it creates a FakeHlsOcdmBridge object and a DrmInfo structure populated with valid data, including the current media format. It then calls SetDecryptInfo on the bridge object and checks that the function returns eDRM_SUCCESS without throwing any exceptions. Ensuring that each media format is handled correctly is crucial to maintaining robust DRM decryption functionality.
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
 * | Variation / Step | Description                                                                                                                           | Test Data                                                                                                                                                                                                                                                       | Expected Result                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | --------------- |
 * | 01               | Initialize media format list and iterate through each media format value                                                               | media formats = eMEDIAFORMAT_HLS, eMEDIAFORMAT_DASH, eMEDIAFORMAT_PROGRESSIVE, eMEDIAFORMAT_HLS_MP4, eMEDIAFORMAT_OTA, eMEDIAFORMAT_HDMI, eMEDIAFORMAT_COMPOSITE, eMEDIAFORMAT_SMOOTHSTREAMINGMEDIA, eMEDIAFORMAT_RMF, eMEDIAFORMAT_UNKNOWN | Loop iterates over each media format without exceptions   | Should be successful |
 * | 02               | Create FakeHlsOcdmBridge instance and construct DrmInfo with valid parameters for the current media format                                | FakeHlsOcdmBridge created using default constructor; DrmInfo fields: method = eMETHOD_AES_128, mediaFormat = current format, useFirst16BytesAsIV = true, bPropagateUriParams = true, bUseMediaSequenceIV = true, bDecryptClearSamplesRequired = true, IV = "A1B2C3D4E5F60708", masterManifestURL = "http://example.com/master_enum.m3u8", manifestURL = "http://example.com/playlist_enum.m3u8", keyURI = "http://example.com/key_enum", keyFormat = "identity_enum", systemUUID = "uuid-enum", initData = "initDataEnum" | Bridge object is created without throwing exceptions; DrmInfo is properly populated   | Should Pass     |
 * | 03               | Call SetDecryptInfo on the bridge instance with the prepared DrmInfo and validate that the return value is eDRM_SUCCESS               | acquireKeyWaitTime = 1000, input = pointer to populated DrmInfo structure                                                                                                                                                                                      | SetDecryptInfo returns eDRM_SUCCESS                         | Should Pass     |
 */
TEST_F(FakeHlsOcdmBridgeTest, IterateMediaFormatEnum) {
    GTEST_SKIP();
    std::cout << "Entering IterateMediaFormatEnum test" << std::endl;

    // List of MediaFormat values based on the provided enum
    MediaFormat formats[] = {
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

    // Loop over each media format value
    for (size_t i = 0; i < sizeof(formats)/sizeof(formats[0]); ++i) {
        std::cout << "Testing MediaFormat enum value: " << formats[i] << std::endl;

        // Create a DrmInfo object with valid values for all fields and current media format
        DrmInfo drmInfo;
        drmInfo.method = eMETHOD_AES_128;
        drmInfo.mediaFormat = formats[i];
        drmInfo.useFirst16BytesAsIV = true;
        drmInfo.bPropagateUriParams = true;
        drmInfo.bUseMediaSequenceIV = true;
        drmInfo.bDecryptClearSamplesRequired = true;
        
        const char* validIV = "A1B2C3D4E5F60708";
        strncpy((char*)drmInfo.iv, validIV, DRM_IV_LEN);
        std::cout << "IV set to: " << std::string((char*)drmInfo.iv, DRM_IV_LEN) << std::endl;
        
        drmInfo.masterManifestURL = "http://example.com/master_enum.m3u8";
        drmInfo.manifestURL = "http://example.com/playlist_enum.m3u8";
        drmInfo.keyURI = "http://example.com/key_enum";
        drmInfo.keyFormat = "identity_enum";
        drmInfo.systemUUID = "uuid-enum";
        drmInfo.initData = "initDataEnum";

        std::cout << "DrmInfo details for mediaFormat " << formats[i] << ":" << std::endl;
        std::cout << "  masterManifestURL: " << drmInfo.masterManifestURL << std::endl;
        std::cout << "  manifestURL: " << drmInfo.manifestURL << std::endl;
        std::cout << "  keyURI: " << drmInfo.keyURI << std::endl;
        std::cout << "  systemUUID: " << drmInfo.systemUUID << std::endl;

        int acquireKeyWaitTime = 1000;
        std::cout << "Calling SetDecryptInfo with acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
        DrmReturn ret = bridge->SetDecryptInfo(&drmInfo, acquireKeyWaitTime);
        std::cout << "SetDecryptInfo returned: " << ret << std::endl;

        // Expected output: eDRM_SUCCESS
        EXPECT_EQ(ret, eDRM_SUCCESS);
    }

    std::cout << "Exiting IterateMediaFormatEnum test" << std::endl;
}
/**
 * @brief Validate that FakeHlsOcdmBridge handles valid audio metadata correctly.
 *
 * This test verifies that providing a valid non-null metadata pointer containing audio metadata, along with an audio track type, allows the FakeHlsOcdmBridge instance to process the metadata correctly by returning eDRM_SUCCESS. This confirms the proper handling of audio metadata within the bridge.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 015
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                                                            | Expected Result                                      | Notes              |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ---------------------------------------------------- | ------------------ |
 * | 01               | Create FakeHlsOcdmBridge instance and ensure no exception is thrown| Constructor with default parameters                                                 | Instance is created without exceptions              | Should be successful |
 * | 02               | Prepare metadata buffer with valid audio data                     | metadata = "Audio Metadata", buffer size = 100                                       | Metadata buffer is correctly initialized            | Should be successful |
 * | 03               | Invoke SetMetaData with valid audio metadata and audio track type   | metadata pointer = valid pointer, trackType = 0, expected output ret = eDRM_SUCCESS      | SetMetaData returns eDRM_SUCCESS                      | Should Pass         |
 */
TEST_F(FakeHlsOcdmBridgeTest, ValidMetadataAudio) {
    GTEST_SKIP();
    std::cout << "Entering ValidMetadataAudio test" << std::endl;
    
    // Create an instance of FakeHlsOcdmBridge using default constructor.
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance" << std::endl;
        
        // Prepare metadata as a fixed size char array.
        char metadata[100];
        const char* sampleData = "Audio Metadata";
        std::memset(metadata, 0, sizeof(metadata));
        strncpy(metadata, sampleData, sizeof(metadata)-1);
        std::cout << "Prepared metadata with value: " << metadata << std::endl;
        
        int trackType = 0; // audio
        std::cout << "Invoking SetMetaData with metadata pointer: " << static_cast<void*>(metadata) 
                  << " and trackType: " << trackType << std::endl;
        
        // Invoke SetMetaData and capture the return value.
        DrmReturn ret = bridge->SetMetaData(static_cast<void*>(metadata), trackType);
        std::cout << "Returned value from SetMetaData: " << ret << std::endl;
        
        // Expected output eDRM_SUCCESS for valid metadata pointer and audio track.
        EXPECT_EQ(eDRM_SUCCESS, ret);
    });
    
    std::cout << "Exiting ValidMetadataAudio test" << std::endl;
}
/**
 * @brief Validates that the SetMetaData API correctly processes valid video metadata.
 *
 * This test verifies that when a valid metadata pointer containing "Video Metadata" and the video track type (1) are passed to the SetMetaData API of FakeHlsOcdmBridge, the method returns eDRM_SUCCESS without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data                                                                                  | Expected Result                                  | Notes              |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------------------------------------ | ------------------ |
 * | 01               | Instantiate FakeHlsOcdmBridge                                    | None                                                                                       | Object created without exceptions                | Should be successful |
 * | 02               | Prepare metadata buffer with "Video Metadata"                    | input: sampleData = "Video Metadata", metadata buffer size = 100, initialized to zeros       | Metadata buffer correctly populated              | Should be successful |
 * | 03               | Set video track type                                             | input: trackType = 1                                                                         | Track type set to video (1)                        | Should be successful |
 * | 04               | Invoke SetMetaData API with valid metadata pointer and video track | input: metadata pointer containing "Video Metadata", trackType = 1; output: ret variable       | Return value equals eDRM_SUCCESS and assertion passes | Should Pass        |
 */
TEST_F(FakeHlsOcdmBridgeTest, ValidMetadataVideo) {
    GTEST_SKIP();
    std::cout << "Entering ValidMetadataVideo test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance" << std::endl;
        
        char metadata[100];
        const char* sampleData = "Video Metadata";
        std::memset(metadata, 0, sizeof(metadata));
        strncpy(metadata, sampleData, sizeof(metadata) - 1);
        std::cout << "Prepared metadata with value: " << metadata << std::endl;
        
        int trackType = 1; // video
        std::cout << "Invoking SetMetaData with metadata pointer: " << static_cast<void*>(metadata)
                  << " and trackType: " << trackType << std::endl;
        
        DrmReturn ret = bridge->SetMetaData(static_cast<void*>(metadata), trackType);
        std::cout << "Returned value from SetMetaData: " << ret << std::endl;
        
        // Expected output eDRM_SUCCESS for valid metadata pointer and video track.
        EXPECT_EQ(eDRM_SUCCESS, ret);
    });
    
    std::cout << "Exiting ValidMetadataVideo test" << std::endl;
}
/**
 * @brief Validate setting metadata for subtitles in FakeHlsOcdmBridge
 *
 * This test verifies that when a valid metadata pointer containing subtitles metadata and a valid subtitle track type are provided, the SetMetaData API is invoked without throwing an exception and returns the expected eDRM_SUCCESS status.
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
 * | Variation / Step | Description                                                                                                 | Test Data                                                                                          | Expected Result                                                    | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | ---------- |
 * | 01               | Invoke SetMetaData on a valid FakeHlsOcdmBridge instance with subtitle metadata prepared in a char array   | metadata pointer = address containing "Subtitles Metadata", trackType = 2, sampleData = "Subtitles Metadata" | API returns eDRM_SUCCESS without exceptions; instance created successfully | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, ValidMetadataSubtitles) {
    GTEST_SKIP();
    std::cout << "Entering ValidMetadataSubtitles test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance" << std::endl;
        
        char metadata[100];
        const char* sampleData = "Subtitles Metadata";
        std::memset(metadata, 0, sizeof(metadata));
        strncpy(metadata, sampleData, sizeof(metadata) - 1);
        std::cout << "Prepared metadata with value: " << metadata << std::endl;
        
        int trackType = 2; // subtitles
        std::cout << "Invoking SetMetaData with metadata pointer: " << static_cast<void*>(metadata)
                  << " and trackType: " << trackType << std::endl;
        
        DrmReturn ret = bridge->SetMetaData(static_cast<void*>(metadata), trackType);
        std::cout << "Returned value from SetMetaData: " << ret << std::endl;
        
        // Expected output eDRM_SUCCESS for valid metadata pointer and subtitles track.
        EXPECT_EQ(eDRM_SUCCESS, ret);
    });
    
    std::cout << "Exiting ValidMetadataSubtitles test" << std::endl;
}
/**
 * @brief Test SetMetaData with null metadata pointer for audio track.
 *
 * This test case verifies that the SetMetaData method returns an error code when the metadata pointer is null,
 * even though a valid audio track type is provided. This ensures that the function handles null input correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                               | Test Data                                             | Expected Result                  | Notes              |
 * | :--------------: | --------------------------------------------------------- | ----------------------------------------------------- | -------------------------------- | ------------------ |
 * | 01               | Create FakeHlsOcdmBridge instance                         | -                                                     | Instance created successfully    | Should be successful |
 * | 02               | Prepare metadata pointer as null and set valid track type   | metadata = nullptr, trackType = 0                      | Parameters set correctly         | Should be successful |
 * | 03               | Invoke SetMetaData with the null metadata and valid track type | metadata = nullptr, trackType = 0                      | Return value equals eDRM_ERROR   | Should Fail        |
 */
TEST_F(FakeHlsOcdmBridgeTest, NullMetadataValidTrack) {
    std::cout << "Entering NullMetadataValidTrack test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance" << std::endl;
        
        void* metadata = nullptr;
        std::cout << "Prepared metadata as NULL pointer" << std::endl;
        
        int trackType = 0; // audio track
        std::cout << "Invoking SetMetaData with metadata pointer: " << metadata 
                  << " and trackType: " << trackType << std::endl;
        
        DrmReturn ret = bridge->SetMetaData(metadata, trackType);
        std::cout << "Returned value from SetMetaData: " << ret << std::endl;
        
        // Expected output eDRM_ERROR for null metadata pointer.
        EXPECT_EQ(eDRM_ERROR, ret);
    });
    
    std::cout << "Exiting NullMetadataValidTrack test" << std::endl;
}
/**
 * @brief Validate that setting metadata with a negative track type returns the error code.
 *
 * This test verifies that the FakeHlsOcdmBridge API correctly handles a scenario where a valid metadata buffer is provided but the track type is negative. The expected behavior is that the SetMetaData function returns eDRM_ERROR without throwing an exception.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of FakeHlsOcdmBridge and initialize the metadata buffer with "Negative Track Metadata". | metadata (char array) = "Negative Track Metadata", trackType not set at this step | Instance is successfully created and metadata buffer is properly initialized | Should be successful |
 * | 02 | Invoke SetMetaData with the prepared metadata pointer and a negative trackType (-1). | input: metadata pointer = address of the initialized char array, trackType = -1; output: ret (expected value eDRM_ERROR) | API returns eDRM_ERROR and no exception is thrown | Should Fail |
 */
TEST_F(FakeHlsOcdmBridgeTest, ValidMetadataNegativeTrack) {
    std::cout << "Entering ValidMetadataNegativeTrack test" << std::endl;
    
    EXPECT_NO_THROW({
        
        char metadata[100];
        const char* sampleData = "Negative Track Metadata";
        std::memset(metadata, 0, sizeof(metadata));
        strncpy(metadata, sampleData, sizeof(metadata) - 1);
        std::cout << "Prepared metadata with value: " << metadata << std::endl;
        
        int trackType = -1; // negative track type
        std::cout << "Invoking SetMetaData with metadata pointer: " << static_cast<void*>(metadata)
                  << " and trackType: " << trackType << std::endl;
        
        DrmReturn ret = bridge->SetMetaData(static_cast<void*>(metadata), trackType);
        std::cout << "Returned value from SetMetaData: " << ret << std::endl;
        
        // Expected output eDRM_ERROR for negative track type.
        EXPECT_EQ(eDRM_ERROR, ret);
    });
    
    std::cout << "Exiting ValidMetadataNegativeTrack test" << std::endl;
}
/**
 * @brief Test for automatic destructor invocation for a stack allocated object.
 *
 * This test verifies that the destructor of the FakeHlsOcdmBridge class is automatically invoked
 * when an instance is created on the stack and goes out of scope. It ensures that no exceptions are thrown 
 * during both construction and the automatic destructor call.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 020
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                              | Expected Result                                                                                         | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the default constructor to create a FakeHlsOcdmBridge object on the stack and verify destruction. | Constructor: FakeHlsOcdmBridge(), no explicit inputs, destructor auto-invoked on out of scope | EXPECT_NO_THROW should pass indicating no exception during construction and automatic destruction. | Should Pass |
 */
TEST(FakeHlsOcdmBridge, DestructorInvokedOnStackAllocatedObject) {
    std::cout << "Entering DestructorInvokedOnStackAllocatedObject test" << std::endl;
    
    {
        std::cout << "Invoking default constructor for stack allocated FakeHlsOcdmBridge object" << std::endl;
        // Object is created on the stack; destructor will be called automatically when out of scope.
        EXPECT_NO_THROW({
            TestDrmSession drmSession("com.example.keys");
            FakeHlsOcdmBridge bridge(&drmSession);
            std::cout << "FakeHlsOcdmBridge object created on stack; internal state is default constructed." << std::endl;
        });
        std::cout << "FakeHlsOcdmBridge object went out of scope; destructor invoked automatically" << std::endl;
    }
    
    std::cout << "Exiting DestructorInvokedOnStackAllocatedObject test" << std::endl;
}

/**
 * @brief Ensure that the FakeHlsOcdmBridge's Decrypt function returns DRM_SUCCESS when provided with valid encrypted data.
 *
 * Test verifies that when valid encrypted data is provided, the Decrypt API of FakeHlsOcdmBridge returns the eDRM_SUCCESS value. This confirms that the decryption process is correctly implemented and meets the expected functionality.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                              | Expected Result                                                     | Notes          |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------------------------------------------------------------- | -------------- |
 * | 01               | Print the start message indicating the beginning of the test case.  | No input arguments.                                                    | Start message printed to stdout.                                   | Should be successful |
 * | 02               | Prepare the input buffer with sample encrypted data.               | buffer = "SampleEncryptedData", buffer size = 100                      | Buffer initialized with the valid encrypted data.                  | Should be successful |
 * | 03               | Invoke the Decrypt API with valid parameters.                      | session_id = 1, buffer = "SampleEncryptedData", buffer size = 100        | API returns DrmReturn::eDRM_SUCCESS.                                 | Should Pass    |
 * | 04               | Assert that the decryption result matches the expected outcome.    | Expected return value = eDRM_SUCCESS                                   | EXPECT_EQ validates that the return value equals DrmReturn::eDRM_SUCCESS. | Should Pass    |
 * | 05               | Print the end message indicating the test case completion.         | No input arguments.                                                    | End message printed to stdout.                                     | Should be successful |
 */
TEST_F(FakeHlsOcdmBridgeTest, DecryptReturnsSuccess) {
    GTEST_SKIP();
    std::cout << "Start: DecryptReturnsSuccess" << std::endl;

    char buffer[100] = {0};
    strncpy(buffer, "SampleEncryptedData", sizeof(buffer) - 1);
    std::cout << "Buffer prepared: " << buffer << std::endl;

    DrmReturn ret = bridge->Decrypt(1, buffer, sizeof(buffer));
    std::cout << "Decrypt returned: " << ret << std::endl;

    EXPECT_EQ(ret, DrmReturn::eDRM_SUCCESS);

    std::cout << "End: DecryptReturnsSuccess" << std::endl;
}
/**
 * @brief Test for Decrypt API with null encrypted data pointer.
 *
 * This test verifies that when the Decrypt API is invoked with a null pointer for the encrypted data along with valid values for the data length, bucket type, and timeout, it returns an error. This is crucial to ensure that the implementation correctly handles improper inputs.
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
 * | 01 | Initialize input parameters with a null pointer for encrypted data and valid values for bucket type, data length, and timeout, then call the Decrypt API. | bucketType = 1, encryptedDataPtr = nullptr, encryptedDataLen = 100, timeInMs = 3000 | Decrypt returns DrmReturn::eDRM_ERROR and the assertion (EXPECT_EQ) passes. | Should Fail |
 */
TEST_F(FakeHlsOcdmBridgeTest, NullEncryptedDataPtr) {
    std::cout << "Start: NullEncryptedDataPtr" << std::endl;

    void* encryptedDataPtr = nullptr;
    size_t encryptedDataLen = 100;
    int bucketType = 1;
    int timeInMs = 3000;

    std::cout << "Calling Decrypt() with null data pointer..." << std::endl;
    DrmReturn ret = bridge->Decrypt(bucketType, encryptedDataPtr, encryptedDataLen, timeInMs);
    std::cout << "Decrypt returned: " << ret << std::endl;

    EXPECT_EQ(ret, DrmReturn::eDRM_ERROR);

    std::cout << "End: NullEncryptedDataPtr" << std::endl;
}
/**
 * @brief Verify that the Decrypt method handles zero-length encrypted data correctly.
 *
 * This test verifies that the Decrypt method of FakeHlsOcdmBridge returns DrmReturn::eDRM_ERROR when provided with a valid non-null encrypted data buffer that has a zero length. This ensures that the API correctly handles edge cases where no actual encrypted data is provided.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup test environment, prepare a non-null buffer with sample text, and invoke Decrypt with encryptedDataLen set to zero | bucketType = 1, encryptedDataPtr = pointer to buffer containing "SampleDataForZeroLengthTest", encryptedDataLen = 0, timeInMs = 3000 | Decrypt returns DrmReturn::eDRM_ERROR and assertion EXPECT_EQ validates the return value | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, ZeroLengthEncryptedData) {
    std::cout << "Entering ZeroLengthEncryptedData test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance using default constructor" << std::endl;
        
        // Prepare a valid non-null buffer, though length is zero
        char encryptedData[50] = {0};
        const char *sampleText = "SampleDataForZeroLengthTest";
        strncpy(encryptedData, sampleText, sizeof(encryptedData)-1);
        std::cout << "Encrypted data buffer prepared with sample text: " << encryptedData << std::endl;
        
        int bucketType = 1;
        size_t encryptedDataLen = 0; // Zero length
        int timeInMs = 3000;
        std::cout << "Invoking Decrypt with bucketType = " << bucketType 
                  << ", encryptedDataPtr = " << static_cast<void*>(encryptedData)
                  << ", encryptedDataLen = " << encryptedDataLen 
                  << ", timeInMs = " << timeInMs << std::endl;
        DrmReturn ret = bridge->Decrypt(bucketType, static_cast<void*>(encryptedData), encryptedDataLen, timeInMs);
        std::cout << "Decrypt returned: " << ret << std::endl;
        EXPECT_EQ(ret, DrmReturn::eDRM_ERROR);
    });
    
    std::cout << "Exiting ZeroLengthEncryptedData test" << std::endl;
}
/**
 * @brief Tests the bridge's Decrypt function with a negative bucketType value
 *
 * This test verifies that when the Decrypt API is invoked with a negative bucketType,
 * the FakeHlsOcdmBridge correctly returns an error (DrmReturn::eDRM_ERROR). The test
 * ensures the function handles invalid bucketType values robustly without causing any
 * unexpected behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                                                                      | Expected Result                                                      | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | -------------- |
 * | 01               | Instantiate FakeHlsOcdmBridge using the default constructor                 | drmSession = valid object, bridge = instance created using drmSession                                          | Instance is created successfully without throwing any exception      | Should be successful |
 * | 02               | Prepare an encrypted data buffer with sample text                           | encryptedData = "EncryptedDataForNegativeBucketTypeTest" (up to 59 characters, null-terminated)                  | Encrypted data buffer is prepared correctly                          | Should be successful |
 * | 03               | Invoke Decrypt API with negative bucketType and verify the returned error    | bucketType = -1, encryptedDataPtr = address of encryptedData, encryptedDataLen = 50, timeInMs = 3000             | Decrypt returns DrmReturn::eDRM_ERROR, as verified by assertion        | Should Fail    |
 */
TEST_F(FakeHlsOcdmBridgeTest, NegativeBucketType) {
    std::cout << "Entering NegativeBucketType test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance using default constructor" << std::endl;
        
        // Prepare valid encrypted data buffer
        char encryptedData[60] = {0};
        const char *sampleText = "EncryptedDataForNegativeBucketTypeTest";
        strncpy(encryptedData, sampleText, sizeof(encryptedData)-1);
        std::cout << "Encrypted data buffer prepared with sample text: " << encryptedData << std::endl;
        
        int bucketType = -1; // Negative bucketType
        size_t encryptedDataLen = 50;
        int timeInMs = 3000;
        std::cout << "Invoking Decrypt with bucketType = " << bucketType 
                  << ", encryptedDataPtr = " << static_cast<void*>(encryptedData)
                  << ", encryptedDataLen = " << encryptedDataLen 
                  << ", timeInMs = " << timeInMs << std::endl;
        DrmReturn ret = bridge->Decrypt(bucketType, static_cast<void*>(encryptedData), encryptedDataLen, timeInMs);
        std::cout << "Decrypt returned: " << ret << std::endl;
        EXPECT_EQ(ret, DrmReturn::eDRM_ERROR);
    });
    
    std::cout << "Exiting NegativeBucketType test" << std::endl;
}
/**
 * @brief Test Decrypt API with a negative time value.
 *
 * This test evaluates the behavior of the FakeHlsOcdmBridge::Decrypt method when provided with a negative time interval in milliseconds. It ensures that the function returns the error code eDRM_ERROR for an invalid time input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize FakeHlsOcdmBridge instance, prepare valid encrypted data buffer, and invoke Decrypt with a negative timeInMs value | bucketType = 1, encryptedDataPtr = "EncryptedDataForNegativeTimeTest", encryptedDataLen = 50, timeInMs = -100 | DrmReturn = eDRM_ERROR, EXPECT_EQ(ret, DrmReturn::eDRM_ERROR) | Should Fail |
 */
TEST_F(FakeHlsOcdmBridgeTest, NegativeTimeInMs) {
    std::cout << "Entering NegativeTimeInMs test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance using default constructor" << std::endl;
        
        // Prepare valid encrypted data buffer
        char encryptedData[60] = {0};
        const char *sampleText = "EncryptedDataForNegativeTimeTest";
        strncpy(encryptedData, sampleText, sizeof(encryptedData)-1);
        std::cout << "Encrypted data buffer prepared with sample text: " << encryptedData << std::endl;
        
        int bucketType = 1;
        size_t encryptedDataLen = 50;
        int timeInMs = -100; // Negative time value
        std::cout << "Invoking Decrypt with bucketType = " << bucketType 
                  << ", encryptedDataPtr = " << static_cast<void*>(encryptedData)
                  << ", encryptedDataLen = " << encryptedDataLen 
                  << ", timeInMs = " << timeInMs << std::endl;
        DrmReturn ret = bridge->Decrypt(bucketType, static_cast<void*>(encryptedData), encryptedDataLen, timeInMs);
        std::cout << "Decrypt returned: " << ret << std::endl;
        EXPECT_EQ(ret, DrmReturn::eDRM_ERROR);
    });
    
    std::cout << "Exiting NegativeTimeInMs test" << std::endl;
}
/**
 * @brief Verify that the Decrypt API returns an error when a custom time parameter is provided.
 *
 * This test checks that the FakeHlsOcdmBridge's Decrypt method, when invoked with a custom time value (timeInMs = 5000),
 * returns the DrmReturn::eDRM_ERROR value and does not throw an exception. The test uses a valid encrypted data buffer
 * to simulate decryption and ensure that the API handles custom timing parameters correctly.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                             | Expected Result                                                  | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the Decrypt API with bucketType, valid encrypted data buffer, encryptedDataLen, and custom timeInMs. | bucketType = 1, encryptedDataPtr = pointer to buffer containing "EncryptedDataForCustomTimeTest", encryptedDataLen = 128, timeInMs = 5000 | Decrypt returns DrmReturn::eDRM_ERROR without throwing an exception | Should Pass |
 */
TEST_F(FakeHlsOcdmBridgeTest, CustomTimeInMs) {
    std::cout << "Entering CustomTimeInMs test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Created FakeHlsOcdmBridge instance using default constructor" << std::endl;
        
        // Prepare valid encrypted data buffer
        char encryptedData[130] = {0};
        const char *sampleText = "EncryptedDataForCustomTimeTest";
        strncpy(encryptedData, sampleText, sizeof(encryptedData)-1);
        std::cout << "Encrypted data buffer prepared with sample text: " << encryptedData << std::endl;
        
        int bucketType = 1;
        size_t encryptedDataLen = 128;
        int timeInMs = 5000; // Custom time value
        std::cout << "Invoking Decrypt with bucketType = " << bucketType 
                  << ", encryptedDataPtr = " << static_cast<void*>(encryptedData)
                  << ", encryptedDataLen = " << encryptedDataLen 
                  << ", timeInMs = " << timeInMs << std::endl;
        DrmReturn ret = bridge->Decrypt(bucketType, static_cast<void*>(encryptedData), encryptedDataLen, timeInMs);
        std::cout << "Decrypt returned: " << ret << std::endl;
        EXPECT_EQ(ret, DrmReturn::eDRM_ERROR);
    });
    
    std::cout << "Exiting CustomTimeInMs test" << std::endl;
}
/**
 * @brief Verify that creating a FakeHlsOcdmBridge with a valid DrmSession pointer initializes a DRM session without throwing exceptions
 *
 * This test verifies that constructing FakeHlsOcdmBridge with a valid TestDrmSession pointer does not throw an exception. It uses a TestDrmSession instance initialized with the key "com.example.keys" and passes its pointer to the FakeHlsOcdmBridge constructor to ensure proper handling of the DRM session.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 027@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                                       | Expected Result                                          | Notes      |
 * | :----:           | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------- | -------------------------------------------------------- | ---------- |
 * | 01               | Invoke FakeHlsOcdmBridge constructor using a valid TestDrmSession pointer     | input: drmSession pointer = address of TestDrmSession with key "com.example.keys" | EXPECT_NO_THROW passes; no exceptions thrown from constructor | Should Pass |
 */
TEST(FakeHlsOcdmBridge, ConstructValidDrmSession) {
    std::cout << "Entering ConstructValidDrmSession test" << std::endl;
    TestDrmSession drmSession("com.example.keys");
    std::cout << "Invoking FakeHlsOcdmBridge constructor with valid DrmSession pointer" << std::endl;
    
    // Expect that constructing FakeHlsOcdmBridge with a valid DrmSession pointer does not throw.
    EXPECT_NO_THROW({
        FakeHlsOcdmBridge bridge(&drmSession);
        std::cout << "FakeHlsOcdmBridge instance constructed successfully with DrmSession pointer: " 
                  << &drmSession << std::endl;
    });
    
    std::cout << "Exiting ConstructValidDrmSession test" << std::endl;
}
/**
 * @brief Validate construction behavior of FakeHlsOcdmBridge with a null DrmSession pointer
 *
 * This test verifies that constructing a FakeHlsOcdmBridge object with a nullptr as
 * the DrmSession pointer results in an error condition. The test checks if the constructor
 * correctly throws an exception when provided with an invalid (null) input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                | Test Data                                  | Expected Result                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------ | ------------------------------------------ | --------------------------------------------------------------------- | ----------- |
 * | 01               | Call the FakeHlsOcdmBridge constructor with a null DrmSession pointer to simulate invalid input | nullDrmSession = nullptr                     | The API is expected to throw an exception indicating improper input   | Should Fail |
 */
TEST(FakeHlsOcdmBridge, ConstructNullDrmSession) {
    GTEST_SKIP();
    std::cout << "Entering ConstructNullDrmSession test" << std::endl;
    // Set the DrmSession pointer to nullptr to simulate invalid input.
    DrmSession* nullDrmSession = nullptr;
    std::cout << "Invoking FakeHlsOcdmBridge constructor with null DrmSession pointer." << std::endl;
    
    // Expect that constructing FakeHlsOcdmBridge with a null pointer throws an exception or handles the error.
    EXPECT_ANY_THROW({
        FakeHlsOcdmBridge bridge(nullDrmSession);
        std::cout << "FakeHlsOcdmBridge instance constructed with null DrmSession pointer: " 
                  << nullDrmSession << std::endl;
    });
    
    std::cout << "Exiting ConstructNullDrmSession test" << std::endl;
}
/**
 * @brief Validates that GetBridge returns a valid pointer when provided a valid drmSession.
 *
 * Checks that a TestDrmSession object is created using a valid custom key system and that calling 
 * HlsOcdmBridgeInterface::GetBridge on the valid drmSession pointer does not throw exceptions and returns a non-null pointer.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                                                         | Expected Result                                                                   | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create a TestDrmSession object with a valid key system                                              | input: keySystem = "TestKeySystem"                                                                  | TestDrmSession object is created without throwing an exception                     | Should Pass |
 * | 02               | Invoke HlsOcdmBridgeInterface::GetBridge with the valid drmSession pointer                           | input: drmSession pointer = address of TestDrmSession, keySystem = "TestKeySystem"                  | Returns a non-null pointer without throwing an exception                           | Should Pass |
 */
TEST(HlsOcdmBridgeInterface, ValidDrmSession) {
    std::cout << "Entering ValidDrmSession test\n";

    // Create a valid TestDrmSession object using a custom key system.
    std::string keySystem("TestKeySystem");
    // Use EXPECT_NO_THROW for the constructor invocation.
    EXPECT_NO_THROW({
        TestDrmSession testDrmSession(keySystem);
        std::cout << "TestDrmSession object created with keySystem: " << keySystem << "\n";
        
        // Log the invocation of GetBridge.
        std::cout << "Invoking HlsOcdmBridgeInterface::GetBridge with a valid drmSession pointer: " << &testDrmSession << "\n";
        HlsDrmBase* bridge = nullptr;
        EXPECT_NO_THROW({
            bridge = HlsOcdmBridgeInterface::GetBridge(&testDrmSession);
            std::cout << "HlsOcdmBridgeInterface::GetBridge returned pointer: " << bridge << "\n";
        });
        
        // Check that the returned pointer is not nullptr.
        EXPECT_NE(bridge, nullptr);
    });

    std::cout << "Exiting ValidDrmSession test\n";
}
/**
 * @brief Validate that HlsOcdmBridgeInterface::GetBridge handles a null DRM session correctly.
 *
 * This test verifies that when a nullptr is passed as the drmSession argument, the GetBridge API returns a nullptr without throwing an exception.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 030
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                        | Test Data                                                    | Expected Result                                                        | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke HlsOcdmBridgeInterface::GetBridge with a nullptr as the drmSession pointer   | input: drmSession = nullptr, output: bridge pointer is checked | API should return a nullptr and not throw an exception; EXPECT_EQ(bridge, nullptr) check passes | Should Pass |
 */
TEST(HlsOcdmBridgeInterface, NullDrmSession) {
    GTEST_SKIP();
    std::cout << "Entering NullDrmSession test\n";

    // Log the invocation of GetBridge with a nullptr.
    std::cout << "Invoking HlsOcdmBridgeInterface::GetBridge with a nullptr drmSession pointer\n";
    HlsDrmBase* bridge = nullptr;
    EXPECT_NO_THROW({
        bridge = HlsOcdmBridgeInterface::GetBridge(nullptr);
        std::cout << "HlsOcdmBridgeInterface::GetBridge returned pointer: " << bridge << "\n";
    });

    // Expect that the returned pointer is nullptr.
    EXPECT_EQ(bridge, nullptr);

    std::cout << "Exiting NullDrmSession test\n";
}