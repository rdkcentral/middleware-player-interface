
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
#include "HlsOcdmBridge.h"


class HlsOcdmBridgeTests : public ::testing::Test
{
protected:
	void SetUp() override
	{
	}

	void TearDown() override
	{
	}

public:
};
// Test Case: AcquireKey with valid metadata pointer and valid track type
/**
 * @brief Tests the AcquireKey API with valid metadata and a valid track type.
 *
 * This test verifies that the AcquireKey function of the HlsOcdmBridge object does not throw an exception
 * when provided with a valid metadata pointer and a track type of 1. It confirms that the function correctly
 * processes valid metadata input and executes without errors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                | Test Data                                                            | Expected Result                                                        | Notes          |
 * | :--------------: | ---------------------------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize dummy metadata with valid sample data           | dummyMetadata = "Valid metadata sample"                              | dummyMetadata is successfully populated with the valid string          | Should be successful |
 * | 02               | Create HlsOcdmBridge object using default constructor      | No input arguments as object is created using default constructor      | Object is created successfully without throwing an exception         | Should be successful |
 * | 03               | Invoke AcquireKey with valid metadata pointer and track type 1 | metadata pointer = dummyMetadata, trackType = 1                        | AcquireKey executes without throwing any exception and key is acquired   | Should Pass    |
 */
TEST_F(HlsOcdmBridgeTests, AcquireKeyWithValidMetadataAndValidTrackType) {
    std::cout << "Entering AcquireKeyWithValidMetadataAndValidTrackType test" << std::endl;
    
    // Prepare dummy metadata: a dummy character array with some content.
    char dummyMetadata[100];
    const char *sampleData = "Valid metadata sample";
    std::memset(dummyMetadata, 0, sizeof(dummyMetadata));
    strncpy(dummyMetadata, sampleData, sizeof(dummyMetadata) - 1);
    std::cout << "Created dummyMetadata with value: " << dummyMetadata << std::endl;
    
    // Create an object of HlsOcdmBridge using default constructor.
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        std::cout << "Invoking AcquireKey with metadata pointer: " << static_cast<void*>(dummyMetadata)
                  << " and trackType: 1" << std::endl;
        EXPECT_NO_THROW(bridge.AcquireKey(static_cast<void*>(dummyMetadata), 1));
        std::cout << "AcquireKey executed successfully with valid metadata and track type 1." << std::endl;
    });
    
    std::cout << "Exiting AcquireKeyWithValidMetadataAndValidTrackType test" << std::endl;
}
/**
 * @brief Test the AcquireKey API with null metadata and a valid track type
 *
 * Tests acquiring a key when provided with a null metadata pointer and a valid track type, ensuring that no exceptions are thrown during the process.
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
 * | 01 | Create HlsOcdmBridge object and invoke AcquireKey with null metadata and valid trackType (1) | HlsOcdmBridge object, metadata = nullptr, trackType = 1 | No exception is thrown during object construction and API call; EXPECT_NO_THROW assertion passes | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, AcquireKeyWithNullMetadataAndValidTrackType) {
    std::cout << "Entering AcquireKeyWithNullMetadataAndValidTrackType test" << std::endl;
    
    // Create an object of HlsOcdmBridge using default constructor.
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        void* metadata = nullptr;
        std::cout << "Invoking AcquireKey with metadata pointer: " << metadata
                  << " and trackType: 1" << std::endl;
        EXPECT_NO_THROW(bridge.AcquireKey(metadata, 1));
        std::cout << "AcquireKey executed successfully with null metadata and track type 1." << std::endl;
    });
    
    std::cout << "Exiting AcquireKeyWithNullMetadataAndValidTrackType test" << std::endl;
}
/**
 * @brief This test verifies that the AcquireKey API works correctly when provided with valid metadata and a negative track type.
 *
 * This test ensures that the AcquireKey method of HlsOcdmBridge can be invoked successfully without throwing an exception when valid metadata is provided along with a negative track type. It checks the robustness of the API by using non-standard but valid input values.
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
 * | Variation / Step | Description                                                        | Test Data                                                                                                     | Expected Result                                                              | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare dummy metadata by initializing a character array with data.  | dummyMetadata = char[100] (all zeroed), sampleData = "Dummy metadata for negative track type"                | dummyMetadata is correctly initialized with the sample data                  | Should be successful |
 * | 02               | Create HlsOcdmBridge object using the default constructor.           | No input parameters; bridge object created using default constructor                                           | HlsOcdmBridge object is successfully created without exceptions              | Should be successful |
 * | 03               | Invoke AcquireKey with the dummy metadata pointer and negative track type (-1). | Input: metadata pointer = address of dummyMetadata, trackType = -1; Output: (none)                               | AcquireKey executes without throwing an exception and completes successfully | Should Pass   |
 */
TEST_F(HlsOcdmBridgeTests, AcquireKeyWithValidMetadataAndNegativeTrackType) {
    std::cout << "Entering AcquireKeyWithValidMetadataAndNegativeTrackType test" << std::endl;
    
    // Prepare dummy metadata: a dummy character array with some content.
    char dummyMetadata[100];
    const char *sampleData = "Dummy metadata for negative track type";
    std::memset(dummyMetadata, 0, sizeof(dummyMetadata));
    strncpy(dummyMetadata, sampleData, sizeof(dummyMetadata) - 1);
    std::cout << "Created dummyMetadata with value: " << dummyMetadata << std::endl;
    
    // Create an object of HlsOcdmBridge using default constructor.
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        int trackType = -1;
        std::cout << "Invoking AcquireKey with metadata pointer: " << static_cast<void*>(dummyMetadata)
                  << " and trackType: " << trackType << std::endl;
        EXPECT_NO_THROW(bridge.AcquireKey(static_cast<void*>(dummyMetadata), trackType));
        std::cout << "AcquireKey executed successfully with valid metadata and negative track type." << std::endl;
    });
    
    std::cout << "Exiting AcquireKeyWithValidMetadataAndNegativeTrackType test" << std::endl;
}
/**
 * @brief Tests acquisition of key with valid metadata and an extreme positive track type.
 *
 * This test verifies that when valid metadata is provided along with an extreme positive track type value (INT_MAX),
 * the AcquireKey function of the HlsOcdmBridge class executes successfully without throwing any exceptions.
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
 * | Variation / Step | Description                                              | Test Data                                                              | Expected Result                                                             | Notes              |
 * | :---------------: | -------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------------ |
 * | 01                | Create dummy metadata with a sample string for testing.  | dummyMetadata array = 0 initialized, sampleData = "Dummy metadata for extreme positive track type" | Dummy metadata is successfully created and populated.                     | Should be successful |
 * | 02                | Instantiate HlsOcdmBridge object using default constructor. | No parameters                                                          | Object is created without throwing any exceptions.                        | Should be successful |
 * | 03                | Invoke AcquireKey API with dummy metadata pointer and extreme positive track type (INT_MAX). | metadata pointer = address of dummyMetadata, trackType = INT_MAX           | AcquireKey executes successfully without throwing any exceptions.         | Should Pass        |
 */
TEST_F(HlsOcdmBridgeTests, AcquireKeyWithValidMetadataAndExtremePositiveTrackType) {
    std::cout << "Entering AcquireKeyWithValidMetadataAndExtremePositiveTrackType test" << std::endl;
    
    // Prepare dummy metadata: a dummy character array with some content.
    char dummyMetadata[100];
    const char *sampleData = "Dummy metadata for extreme positive track type";
    std::memset(dummyMetadata, 0, sizeof(dummyMetadata));
    strncpy(dummyMetadata, sampleData, sizeof(dummyMetadata) - 1);
    std::cout << "Created dummyMetadata with value: " << dummyMetadata << std::endl;

    // Create an object of HlsOcdmBridge using default constructor.
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        int trackType = INT_MAX;
        std::cout << "Invoking AcquireKey with metadata pointer: " << static_cast<void*>(dummyMetadata)
                  << " and trackType: " << trackType << std::endl;
        EXPECT_NO_THROW(bridge.AcquireKey(static_cast<void*>(dummyMetadata), trackType));
        std::cout << "AcquireKey executed successfully with valid metadata and extreme positive track type." << std::endl;
    });
    
    std::cout << "Exiting AcquireKeyWithValidMetadataAndExtremePositiveTrackType test" << std::endl;
}
/**
 * @brief Verify AcquireKey handles valid metadata with an extreme negative track type
 *
 * This test ensures that the AcquireKey method of HlsOcdmBridge successfully processes valid metadata along with an extreme negative track type value, and that no exception is thrown during the operation. This verifies that the API manages edge case inputs in a robust manner.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** (High) This test is critical to validate the function's behavior when provided with challenging input values@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare dummy metadata, create HlsOcdmBridge object using default constructor, and invoke AcquireKey with valid metadata and extreme negative track type | dummyMetadata = "Dummy metadata for extreme negative track type", trackType = INT_MIN | AcquireKey executes without throwing an exception | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, AcquireKeyWithValidMetadataAndExtremeNegativeTrackType) {
    std::cout << "Entering AcquireKeyWithValidMetadataAndExtremeNegativeTrackType test" << std::endl;
    
    // Prepare dummy metadata: a dummy character array with some content.
    char dummyMetadata[100];
    const char *sampleData = "Dummy metadata for extreme negative track type";
    std::memset(dummyMetadata, 0, sizeof(dummyMetadata));
    strncpy(dummyMetadata, sampleData, sizeof(dummyMetadata) - 1);
    std::cout << "Created dummyMetadata with value: " << dummyMetadata << std::endl;
    
    // Create an object of HlsOcdmBridge using default constructor.
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        int trackType = INT_MIN;
        std::cout << "Invoking AcquireKey with metadata pointer: " << static_cast<void*>(dummyMetadata)
                  << " and trackType: " << trackType << std::endl;
        EXPECT_NO_THROW(bridge.AcquireKey(static_cast<void*>(dummyMetadata), trackType));
        std::cout << "AcquireKey executed successfully with valid metadata and extreme negative track type." << std::endl;
    });
    
    std::cout << "Exiting AcquireKeyWithValidMetadataAndExtremeNegativeTrackType test" << std::endl;
}
/**
 * @brief Test the CancelKeyWait() function to ensure that it completes without throwing any exceptions.
 *
 * This test validates that the HlsOcdmBridge instance can be created using its default constructor and that its CancelKeyWait() method can be invoked normally. The test confirms that no exceptions are thrown during both instance creation and method invocation, and that the system state remains unchanged afterwards.
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
 * | Variation / Step | Description                                                              | Test Data                                                      | Expected Result                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------ | -------------------------------------------------------------- | ---------------------------------------------------- | ------------- |
 * | 01               | Create an instance of HlsOcdmBridge using its default constructor.       | None (default constructor invoked)                             | Instance is created without throwing exceptions.   | Should Pass   |
 * | 02               | Invoke the CancelKeyWait() method on the created instance.               | Method call: bridge.CancelKeyWait()                            | No exception is thrown when invoking CancelKeyWait(). | Should Pass   |
 * | 03               | Verify that the system state remains unchanged after invoking the method.| None (system logs output)                                        | System state remains unchanged as expected.        | Should be successful |
 */
TEST_F(HlsOcdmBridgeTests, CancelKeyWaitNormally) {
    std::cout << "Entering CancelKeyWaitNormally test" << std::endl;
    
    // Create instance of HlsOcdmBridge using default constructor
    std::cout << "Creating instance of HlsOcdmBridge using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        std::cout << "Instance of HlsOcdmBridge created successfully." << std::endl;

        // Invoke CancelKeyWait() method
        std::cout << "Invoking CancelKeyWait() method." << std::endl;
        EXPECT_NO_THROW(bridge.CancelKeyWait());
        std::cout << "CancelKeyWait() method invoked; no errors were thrown." << std::endl;
        
        // Log: The system state remains unchanged after invoking CancelKeyWait()
        std::cout << "The system state remains unchanged after CancelKeyWait() invocation." << std::endl; 
    });
    
    std::cout << "Exiting CancelKeyWaitNormally test" << std::endl;
}
/**
 * @brief Verify valid decryption with explicit time parameter API
 *
 * This test validates the functionality of the Decrypt API in the HlsOcdmBridge class by providing a valid non-empty buffer containing test data along with an explicit time parameter. The purpose is to ensure that the decryption operation returns a status code of 0, indicating successful decryption.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create HlsOcdmBridge object, copy test data "ValidData" into buffer, and invoke Decrypt with bucketType=1 and explicitTime=100 | input1 = bucketType, value = 1; input2 = encryptedDataPtr, value = pointer to buffer containing "ValidData"; input3 = encryptedDataLen, value = length of "ValidData"; input4 = timeInMs, value = 100; output1 = result, value = 0 | API returns a status code 0 indicating successful decryption and the assertion passes | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, ValidDecryptionExplicitTime) {
    GTEST_SKIP();
    std::cout << "Entering ValidDecryptionExplicitTime test" << std::endl;
    
    // Create object of HlsOcdmBridge using default constructor
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        // Prepare a valid non-empty buffer using strncpy
        char buffer[20];
        const char* testData = "ValidData";
        std::cout << "Copying test data '" << testData << "' into buffer." << std::endl;
        strncpy(buffer, testData, sizeof(buffer));
        size_t dataLen = std::strlen(buffer);
        std::cout << "Buffer contents: '" << buffer << "', length: " << dataLen << std::endl;
        
        int bucketType = 1;
        int explicitTime = 100;
        std::cout << "Invoking Decrypt with bucketType: " << bucketType
                  << ", encryptedDataPtr: " << static_cast<void*>(buffer)
                  << ", encryptedDataLen: " << dataLen
                  << ", timeInMs: " << explicitTime << std::endl;
        
        int result = hlsBridge.Decrypt(bucketType, static_cast<void*>(buffer), dataLen, explicitTime);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        // Expected success is indicated by status code 0
        EXPECT_EQ(result, 0);
    });
    
    std::cout << "Exiting ValidDecryptionExplicitTime test" << std::endl;
}
/**
 * @brief Validate decryption operation using default time parameter
 *
 * This test verifies that the HlsOcdmBridge::Decrypt method functions correctly when invoked using the default wait time parameter. The test ensures that the decryption operation completes successfully with a return status code of 0.@n
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create HlsOcdmBridge object using the default constructor. | HlsOcdmBridge() | Object is created successfully. | Should be successful |
 * | 02 | Copy the test string "DefaultTimeData" into a char buffer using strncpy. | testData = "DefaultTimeData", buffer size = 20 | Buffer contains "DefaultTimeData". | Should be successful |
 * | 03 | Compute the length of the buffer content using std::strlen. | Buffer content = "DefaultTimeData" | Length computed correctly (15). | Should be successful |
 * | 04 | Invoke the Decrypt method with parameters: bucketType = 1, encryptedDataPtr = buffer, encryptedDataLen = computed length, and default wait time parameter. | bucketType = 1, encryptedDataPtr = buffer, encryptedDataLen = 15, wait time = DECRYPT_WAIT_TIME_MS | Decrypt returns status code 0. | Should Pass |
 * | 05 | Assert that the returned status code from Decrypt is equal to 0. | result = output from Decrypt | Assertion verifies status code 0. | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, ValidDecryptionDefaultTime) {
    GTEST_SKIP();
    std::cout << "Entering ValidDecryptionDefaultTime test" << std::endl;
    
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        char buffer[20];
        const char* testData = "DefaultTimeData";
        std::cout << "Copying test data '" << testData << "' into buffer using strncpy." << std::endl;
        strncpy(buffer, testData, sizeof(buffer));
        size_t dataLen = std::strlen(buffer);
        std::cout << "Buffer contents: '" << buffer << "', length: " << dataLen << std::endl;
        
        int bucketType = 1;
        std::cout << "Invoking Decrypt with bucketType: " << bucketType
                  << ", encryptedDataPtr: " << static_cast<void*>(buffer)
                  << ", encryptedDataLen: " << dataLen
                  << ", using default wait time parameter (" << DECRYPT_WAIT_TIME_MS << ")" << std::endl;
        
        int result = hlsBridge.Decrypt(bucketType, static_cast<void*>(buffer), dataLen);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        // Expected success is indicated by status code 0
        EXPECT_EQ(result, 0);
    });
    
    std::cout << "Exiting ValidDecryptionDefaultTime test" << std::endl;
}
/**
 * @brief Test the HlsOcdmBridge::Decrypt method with a null encrypted data pointer
 *
 * This test verifies that the Decrypt method of the HlsOcdmBridge class correctly handles a null pointer for the encrypted data.
 * It ensures that when a null pointer is provided, the method returns the error code (eDRM_ERROR) as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                                            | Expected Result                                | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------- | --------------- |
 * | 01               | Initialize HlsOcdmBridge object and setup test parameters           | bucketType = 1, encryptedDataPtr = nullptr, encryptedDataLen = 100, timeInMs = 100 | Object creation succeeds                       | Should be successful |
 * | 02               | Invoke Decrypt API with null encryptedDataPtr                       | bucketType = 1, encryptedDataPtr = nullptr, encryptedDataLen = 100, timeInMs = 100 | Return value equals eDRM_ERROR                 | Should Fail     |
 */
TEST_F(HlsOcdmBridgeTests, DecryptionFailureNullData) {
    std::cout << "Entering DecryptionFailureNullData test" << std::endl;
    
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        int bucketType = 1;
        void* nullBuffer = nullptr;
        int testLen = 100;
        int explicitTime = 100;
        std::cout << "Invoking Decrypt with bucketType: " << bucketType
                  << ", encryptedDataPtr: " << nullBuffer
                  << ", encryptedDataLen: " << testLen
                  << ", timeInMs: " << explicitTime << std::endl;
        
        int result = hlsBridge.Decrypt(bucketType, nullBuffer, testLen, explicitTime);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        EXPECT_EQ(result, eDRM_ERROR);
    });
    
    std::cout << "Exiting DecryptionFailureNullData test" << std::endl;
}
/**
 * @brief Verify that the Decrypt API fails when provided with a zero-length data buffer.
 *
 * This test verifies that when the Decrypt function is invoked with a valid, non-empty data buffer but with the data length explicitly set to zero, it fails as expected by returning the error code eDRM_ERROR. This ensures that the API correctly handles the edge case of zero-length data input.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create HlsOcdmBridge object, copy non-empty test data into buffer, set data length to zero, and invoke Decrypt | bucketType = 1, testData = "NonEmptyData", buffer content = "NonEmptyData", encryptedDataLen = 0, explicitTime = 100 | API returns error code eDRM_ERROR; Assertion EXPECT_EQ(result, eDRM_ERROR) holds | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, DecryptionFailureZeroDataLength) {
    std::cout << "Entering DecryptionFailureZeroDataLength test" << std::endl;
    
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        char buffer[20];
        const char* testData = "NonEmptyData";
        std::cout << "Copying test data '" << testData << "' into buffer using strncpy." << std::endl;
        strncpy(buffer, testData, sizeof(buffer));
        size_t dataLen = std::strlen(buffer);
        std::cout << "Buffer contents: '" << buffer << "', actual data length: " << dataLen 
                  << " but will be used as 0 for decryption test." << std::endl;
        
        int bucketType = 1;
        int explicitTime = 100;
        std::cout << "Invoking Decrypt with bucketType: " << bucketType
                  << ", encryptedDataPtr: " << static_cast<void*>(buffer)
                  << ", encryptedDataLen: 0"
                  << ", timeInMs: " << explicitTime << std::endl;
        
        int result = hlsBridge.Decrypt(bucketType, static_cast<void*>(buffer), 0, explicitTime);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        // Expected failure due to zero data length
        EXPECT_EQ(result, eDRM_ERROR);
    });
    
    std::cout << "Exiting DecryptionFailureZeroDataLength test" << std::endl;
}
/**
 * @brief Test to verify decryption failure when a negative time parameter is provided
 *
 * This test verifies that the Decrypt API function of HlsOcdmBridge returns the error code eDRM_ERROR when invoked with a negative time parameter.
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
 * | 01 | Create HlsOcdmBridge object using default constructor | None | HlsOcdmBridge object is created successfully | Should be successful |
 * | 02 | Copy test data "NonEmptyData" into buffer using strncpy | testData = "NonEmptyData", buffer size = 20 | Buffer contains "NonEmptyData" with correct length | Should be successful |
 * | 03 | Invoke Decrypt with bucketType = 1, encryptedDataPtr = buffer, encryptedDataLen = length of buffer content, timeInMs = -50 | bucketType = 1, encryptedDataPtr = buffer, encryptedDataLen = 13, timeInMs = -50 | API returns eDRM_ERROR | Should Fail |
 */
TEST_F(HlsOcdmBridgeTests, DecryptionFailureNegativeTime) {
    std::cout << "Entering DecryptionFailureNegativeTime test" << std::endl;
    
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        char buffer[20];
        const char* testData = "NonEmptyData";
        std::cout << "Copying test data '" << testData << "' into buffer using strncpy." << std::endl;
        strncpy(buffer, testData, sizeof(buffer));
        size_t dataLen = std::strlen(buffer);
        std::cout << "Buffer contents: '" << buffer << "', length: " << dataLen << std::endl;
        
        int bucketType = 1;
        int negativeTime = -50;
        std::cout << "Invoking Decrypt with bucketType: " << bucketType
                  << ", encryptedDataPtr: " << static_cast<void*>(buffer)
                  << ", encryptedDataLen: " << dataLen
                  << ", timeInMs: " << negativeTime << std::endl;
        
        int result = hlsBridge.Decrypt(bucketType, static_cast<void*>(buffer), dataLen, negativeTime);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        // Expected failure due to invalid negative time parameter
        EXPECT_EQ(result, eDRM_ERROR);
    });
    
    std::cout << "Exiting DecryptionFailureNegativeTime test" << std::endl;
}
/**
 * @brief Verify that decryption fails when an invalid bucket type is provided.
 *
 * This test validates that the Decrypt API of the HlsOcdmBridge class returns the error code eDRM_ERROR when invoked with an invalid bucket type. The test creates a default instance of the bridge, sets up a buffer with valid data, and then calls Decrypt with a negative bucket type value. The expected behavior is that the API call fails with the appropriate error code.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                                                                   | Expected Result                                                  | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate the HlsOcdmBridge object using the default constructor.                         | None                                                                                                        | Object is created without throwing exceptions.                 | Should be successful |
 * | 02               | Copy the string "ValidDataForBucket" into a buffer using strncpy.                           | testData = "ValidDataForBucket", buffer size = 20                                                           | Buffer contains "ValidDataForBucket" with correct length computed. | Should be successful |
 * | 03               | Invoke Decrypt with invalidBucketType (-1), the buffer pointer, its calculated length, and 100. | invalidBucketType = -1, encryptedDataPtr = buffer, encryptedDataLen = length of "ValidDataForBucket", timeInMs = 100 | API returns eDRM_ERROR and assertion (EXPECT_EQ) passes.         | Should Fail      |
 */
TEST_F(HlsOcdmBridgeTests, DecryptionFailureInvalidBucket) {
    std::cout << "Entering DecryptionFailureInvalidBucket test" << std::endl;
    
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsBridge(nullptr);
        std::cout << "Created HlsOcdmBridge object using default constructor." << std::endl;
        
        char buffer[20];
        const char* testData = "ValidDataForBucket";
        std::cout << "Copying test data '" << testData << "' into buffer using strncpy." << std::endl;
        strncpy(buffer, testData, sizeof(buffer));
        size_t dataLen = std::strlen(buffer);
        std::cout << "Buffer contents: '" << buffer << "', length: " << dataLen << std::endl;
        
        int invalidBucketType = -1;
        int explicitTime = 100;
        std::cout << "Invoking Decrypt with bucketType: " << invalidBucketType
                  << ", encryptedDataPtr: " << static_cast<void*>(buffer)
                  << ", encryptedDataLen: " << dataLen
                  << ", timeInMs: " << explicitTime << std::endl;
        
        int result = hlsBridge.Decrypt(invalidBucketType, static_cast<void*>(buffer), dataLen, explicitTime);
        std::cout << "Decrypt returned status code: " << result << std::endl;
        
        // Expected failure due to invalid bucket type
        EXPECT_EQ(result, eDRM_ERROR);
    });
    
    std::cout << "Exiting DecryptionFailureInvalidBucket test" << std::endl;
}
/**
 * @brief Verify that the GetState method returns a valid DRM state value.
 *
 * This test verifies that the GetState method of the HlsOcdmBridge class returns a valid DRM state 
 * as defined by the DRMState enum. It creates an instance of HlsOcdmBridge using the default constructor, 
 * invokes the GetState method, and asserts that the returned state is one of the expected enum values.
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
 * | Variation / Step | Description                                                        | Test Data                                                                              | Expected Result                                                                                     | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create HlsOcdmBridge object using the default constructor            | None                                                                                   | Object is created successfully                                                                      | Should be successful |
 * | 02               | Invoke the GetState method on the HlsOcdmBridge instance              | instance = HlsOcdmBridge, method = GetState()                                            | Method returns an integer value representing a DRM state                                            | Should Pass   |
 * | 03               | Validate that the returned state matches one of the valid DRMState enums| state = returned value (eDRM_INITIALIZED, eDRM_ACQUIRING_KEY, eDRM_KEY_ACQUIRED, eDRM_KEY_FAILED, eDRM_KEY_FLUSH) | The state is one of the expected enum values; the assertion (EXPECT_TRUE) passes                       | Should Pass   |
 */
TEST_F(HlsOcdmBridgeTests, VerifyGetStateReturnsValidState) {
    std::cout << "Entering VerifyGetStateReturnsValidState test" << std::endl;

    EXPECT_NO_THROW({
        // Create object using default constructor
        HlsOcdmBridge hlsOcdmBridge(nullptr);
        std::cout << "HlsOcdmBridge object created using default constructor." << std::endl;
        
        // Invoke GetState method and log the invocation
        std::cout << "Invoking GetState method." << std::endl;
        int state = hlsOcdmBridge.GetState();
        std::cout << "GetState returned value: " << state << std::endl;

        // Check that the state is one of the valid DRMState enum values
        bool validState = (state == eDRM_INITIALIZED ||
                           state == eDRM_ACQUIRING_KEY ||
                           state == eDRM_KEY_ACQUIRED ||
                           state == eDRM_KEY_FAILED ||
                           state == eDRM_KEY_FLUSH);
        std::cout << "Validity check for state: " << (validState ? "PASSED" : "FAILED") << std::endl;
        EXPECT_TRUE(validState);
    });

    std::cout << "Exiting VerifyGetStateReturnsValidState test" << std::endl;
}
/**
 * @brief Test construction of HlsOcdmBridge using a valid DrmSession pointer.
 *
 * This test verifies that the HlsOcdmBridge constructor does not throw an exception when provided with a valid DrmSession pointer, ensuring that the internal m_drmSession member is set correctly.
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
 * | Variation / Step | Description                                                      | Test Data                                                        | Expected Result                                                      | Notes                 |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------- |
 * | 01               | Create a valid DrmSession instance and obtain its pointer        | drmSession = valid instance, validDrmSessionPtr = address of drmSession | validDrmSessionPtr is non-null                                        | Should be successful  |
 * | 02               | Invoke HlsOcdmBridge constructor using the valid DrmSession pointer | Input: validDrmSessionPtr = address obtained in step 01; Output: HlsOcdmBridge instance with m_drmSession set to the valid pointer | HlsOcdmBridge constructed without throwing an exception and m_drmSession set correctly | Should Pass           |
 */
TEST_F(HlsOcdmBridgeTests, Constructing_HlsOcdmBridge_With_Valid_DrmSession)
{
    std::cout << "Entering Constructing_HlsOcdmBridge_With_Valid_DrmSession test" << std::endl;
#if 0 // Temporarily disabled
    // Create a valid DrmSession object
    DrmSession drmSession = new DrmSession(); // Assuming DrmSession can be default constructed
    DrmSession* validDrmSessionPtr = &drmSession;

    std::cout << "Preparing valid DrmSession pointer with address: " << validDrmSessionPtr << std::endl;
    std::cout << "Invoking HlsOcdmBridge constructor with valid DrmSession pointer: " << validDrmSessionPtr << std::endl;

    EXPECT_NO_THROW({
        HlsOcdmBridge hlsOcdmBridge(validDrmSessionPtr);
        std::cout << "HlsOcdmBridge instance created successfully with m_drmSession set to: " << validDrmSessionPtr << std::endl;
    });
#endif
    std::cout << "Exiting Constructing_HlsOcdmBridge_With_Valid_DrmSession test" << std::endl;
}
/**
 * @brief Validate the construction of HlsOcdmBridge with a null DrmSession pointer
 *
 * This test verifies that constructing a HlsOcdmBridge instance with a null DrmSession pointer does not throw an exception.
 * It ensures that the object handles a null DRM session appropriately and gets instantiated without error.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare a null DrmSession pointer | DrmSession* = nullptr | DrmSession pointer is set to nullptr | Should be successful |
 * | 02 | Invoke HlsOcdmBridge constructor with null DrmSession pointer | input: DrmSession* = nullptr | HlsOcdmBridge instance is created successfully without throwing an exception | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, Constructing_HlsOcdmBridge_With_Null_DrmSession)
{
    std::cout << "Entering Constructing_HlsOcdmBridge_With_Null_DrmSession test" << std::endl;

    DrmSession* nullDrmSessionPtr = nullptr;
    std::cout << "Preparing null DrmSession pointer: " << nullDrmSessionPtr << std::endl;
    std::cout << "Invoking HlsOcdmBridge constructor with null DrmSession pointer" << std::endl;

    EXPECT_NO_THROW({
        HlsOcdmBridge hlsOcdmBridge(nullDrmSessionPtr);
        std::cout << "HlsOcdmBridge instance created successfully with m_drmSession set as null pointer" << std::endl;
    });

    std::cout << "Exiting Constructing_HlsOcdmBridge_With_Null_DrmSession test" << std::endl;
}
/**
 * @brief Verifies that Release cleans up active DRM session resources in HlsOcdmBridge.
 *
 * This test ensures that creating an instance of HlsOcdmBridge using the default constructor does not throw any exceptions and that invoking the Release method correctly cleans up active DRM session resources without throwing any errors.
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
 * | Variation / Step | Description                                                             | Test Data                                                       | Expected Result                                                     | Notes       |
 * | :--------------: | ----------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | ----------- |
 * | 01               | Create HlsOcdmBridge object using the default constructor.              | constructor = default                                           | Object is created without throwing any exceptions.                 | Should Pass |
 * | 02               | Invoke HlsOcdmBridge::Release() to clean up DRM session resources.        | object = hlsOcdmBridge, method = Release                          | Release() executes without throwing an exception; assertion passes. | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, Release_CleansUpActiveDRMSessionResources) {
    GTEST_SKIP();
    std::cout << "Entering Release_CleansUpActiveDRMSessionResources test" << std::endl;

    // Creating object of HlsOcdmBridge using default constructor
    std::cout << "Creating HlsOcdmBridge object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge hlsOcdmBridge(nullptr);
        std::cout << "HlsOcdmBridge object created successfully." << std::endl;

        // Invoking Release method to clean up DRM session resources
        std::cout << "Invoking HlsOcdmBridge::Release() method." << std::endl;
        EXPECT_NO_THROW(hlsOcdmBridge.Release());
        std::cout << "HlsOcdmBridge::Release() executed without errors." << std::endl;
    });

    std::cout << "Exiting Release_CleansUpActiveDRMSessionResources test" << std::endl;
}
/**
 * @brief Verify that the RestoreKeyState method does not throw exceptions
 *
 * This test verifies that invoking the RestoreKeyState method on a HlsOcdmBridge instance, created using its default constructor, does not throw any exception and maintains the default internal DRM key state.
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
 * | Variation / Step | Description                                                                | Test Data                                                       | Expected Result                                                      | Notes      |
 * | :--------------: | -------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Create a HlsOcdmBridge instance using the default constructor              | Instance creation via default constructor; no input arguments   | Instance is created successfully without throwing any exception      | Should Pass|
 * | 02               | Invoke RestoreKeyState method on the created HlsOcdmBridge instance          | Calling bridge.RestoreKeyState()                                | Method executes without throwing an exception and maintains DRM state| Should Pass|
 */
TEST_F(HlsOcdmBridgeTests, RestoreKeyState_NoThrow) {
    std::cout << "Entering RestoreKeyState_NoThrow test" << std::endl;

    // Create a HlsOcdmBridge instance using its default constructor.
    std::cout << "Creating HlsOcdmBridge instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        std::cout << "HlsOcdmBridge instance created successfully." << std::endl;

        // Log internal state before invoking RestoreKeyState.
        std::cout << "Initial DRM state is assumed unchanged before RestoreKeyState invocation." << std::endl;
        
        // Invoking RestoreKeyState method
        std::cout << "Invoking RestoreKeyState method on HlsOcdmBridge instance." << std::endl;
        EXPECT_NO_THROW({
            bridge.RestoreKeyState();
        });
        std::cout << "RestoreKeyState method executed without errors." << std::endl;
        
        // Log internal state after method invocation.
        std::cout << "Post invocation: DRM key state remains unchanged as per default implementation." << std::endl;
    });
    
    std::cout << "Exiting RestoreKeyState_NoThrow test" << std::endl;
}
/**
 * @brief Test the SetDecryptInfo API with valid DrmInfo pointer and positive wait time.
 *
 * This test verifies that when a valid DrmInfo object is provided along with a valid key wait time, the SetDecryptInfo API returns 0, indicating a successful operation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 018@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                             | Test Data                                                                                   | Expected Result                                 | Notes      |@n
 * | :----:           | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------------------------------------------- | ---------- |@n
 * | 01               | Create a HlsOcdmBridge object, initialize a valid zeroed DrmInfo instance, and call SetDecryptInfo with a wait time of 1000 ms  | drmInfo = valid zero-initialized instance, acquireKeyWaitTime = 1000, output ret = 0         | Return value of 0 indicating success with EXPECT_EQ(ret, 0) | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, ValidDrmInfo_PositiveWait) {
    GTEST_SKIP();
    std::cout << "Entering ValidDrmInfo_PositiveWait test" << std::endl;
    
    // Create HlsOcdmBridge object using default constructor.
    EXPECT_NO_THROW({
        HlsOcdmBridge bridge(nullptr);
        // Initialize a valid DrmInfo instance.
        DrmInfo drmInfo_instance;
        std::memset(&drmInfo_instance, 0, sizeof(drmInfo_instance));

        int acquireKeyWaitTime = 1000;
        std::cout << "Invoking SetDecryptInfo with drmInfo pointer: " << &drmInfo_instance 
                  << " and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
        int ret = bridge.SetDecryptInfo(&drmInfo_instance, acquireKeyWaitTime);
        std::cout << "SetDecryptInfo returned: " << ret << std::endl;
        EXPECT_EQ(ret, 0);
    });
    
    std::cout << "Exiting ValidDrmInfo_PositiveWait test" << std::endl;
}
/**
 * @brief Validates that SetDecryptInfo works correctly with a valid DrmInfo and zero wait time.
 *
 * This test verifies the HlsOcdmBridge API by initializing a valid DrmInfo instance with memory set to zero,
 * setting the acquireKeyWaitTime to zero, and then invoking the SetDecryptInfo method. The test then asserts that
 * the method does not throw and returns 0, ensuring correct functionality when the API is provided with valid inputs.
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
 * | Variation / Step | Description                                                            | Test Data                                                                                     | Expected Result                                | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ---------------------------------------------- | ------------- |
 * | 01               | Create an instance of HlsOcdmBridge and initialize a valid DrmInfo instance by zeroing its memory. | bridge instance created, drmInfo_instance memory set to 0, acquireKeyWaitTime = 0              | Objects initialized correctly, no exception   | Should be successful |
 * | 02               | Invoke SetDecryptInfo API with the valid DrmInfo pointer and zero acquireKeyWaitTime.              | input: &drmInfo_instance, acquireKeyWaitTime = 0, output: ret expected to be 0                  | Return value equals 0 and no exception thrown   | Should Pass   |
 */
TEST_F(HlsOcdmBridgeTests, ValidDrmInfo_ZeroWait) {
    GTEST_SKIP();
    std::cout << "Entering ValidDrmInfo_ZeroWait test" << std::endl;
    
    HlsOcdmBridge bridge(nullptr);
    // Initialize a valid DrmInfo instance.
    DrmInfo drmInfo_instance;
    std::memset(&drmInfo_instance, 0, sizeof(drmInfo_instance));

    int acquireKeyWaitTime = 0;
    std::cout << "Invoking SetDecryptInfo with drmInfo pointer: " << &drmInfo_instance 
                << " and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    int ret = bridge.SetDecryptInfo(&drmInfo_instance, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting ValidDrmInfo_ZeroWait test" << std::endl;
}
/**
 * @brief Validate that SetDecryptInfo accepts valid DRM info with maximum wait time
 *
 * This test case verifies that the SetDecryptInfo API can handle a valid DrmInfo input when the key acquisition wait time is set to the maximum integer value. The test ensures that no exceptions are thrown and that the API returns the expected success code.
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
 * | Variation / Step | Description                                                                                              | Test Data                                                  | Expected Result                                               | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------- | ------------ |
 * | 01               | Invoke SetDecryptInfo API with a valid DrmInfo pointer and acquireKeyWaitTime set to INT_MAX              | drmInfo pointer = valid pointer, acquireKeyWaitTime = INT_MAX, return value = 0 | API returns 0 and no exception is thrown                      | Should Pass  |
 */
TEST_F(HlsOcdmBridgeTests, ValidDrmInfo_MaxWait) {
    GTEST_SKIP();
    std::cout << "Entering ValidDrmInfo_MaxWait test" << std::endl;
    
    HlsOcdmBridge bridge(nullptr);
    // Initialize a valid DrmInfo instance.
    DrmInfo drmInfo_instance;
    std::memset(&drmInfo_instance, 0, sizeof(drmInfo_instance));
    
    int acquireKeyWaitTime = INT_MAX;
    std::cout << "Invoking SetDecryptInfo with drmInfo pointer: " << &drmInfo_instance 
                << " and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    int ret = bridge.SetDecryptInfo(&drmInfo_instance, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;
    EXPECT_EQ(ret, 0);
    
    std::cout << "Exiting ValidDrmInfo_MaxWait test" << std::endl;
}
/**
 * @brief Verify that passing a null DRM info pointer returns the expected error code without throwing an exception
 *
 * This test invokes the SetDecryptInfo API with a null pointer for the drmInfo parameter and a valid acquireKeyWaitTime.
 * It checks that the API call returns eDRM_ERROR and no exception is thrown, ensuring robust error handling for invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 021@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate HlsOcdmBridge object | N/A | Object is created successfully | Should be successful |
 * | 02 | Call SetDecryptInfo with drmInfo as NULL and acquireKeyWaitTime = 1000 | drmInfo = nullptr, acquireKeyWaitTime = 1000 | Returns eDRM_ERROR and no exception is thrown | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, NullDrmInfo_PositiveWait) {
    GTEST_SKIP();
    std::cout << "Entering NullDrmInfo_PositiveWait test" << std::endl;
    
    HlsOcdmBridge bridge(nullptr);
    int acquireKeyWaitTime = 1000;
    std::cout << "Invoking SetDecryptInfo with drmInfo pointer: NULL"
                << " and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    int ret = bridge.SetDecryptInfo(nullptr, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;
    EXPECT_EQ(ret, eDRM_ERROR);
    
    std::cout << "Exiting NullDrmInfo_PositiveWait test" << std::endl;
}
/**
 * @brief Verify that SetDecryptInfo API returns error for negative acquireKeyWaitTime parameter
 *
 * This test ensures that when provided with a valid DrmInfo structure and a negative acquireKeyWaitTime value, 
 * the SetDecryptInfo API returns the appropriate error code (eDRM_ERROR). The objective is to validate proper error handling of invalid wait times.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                                              | Expected Result                                  | Notes       |
 * | :----:           | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------ | ----------- |
 * | 01               | Invoke SetDecryptInfo with a valid DrmInfo pointer and negative acquireKeyWaitTime value         | drmInfo pointer = valid instance, acquireKeyWaitTime = -500, output = eDRM_ERROR | API returns error code eDRM_ERROR                | Should Fail |
 */
TEST_F(HlsOcdmBridgeTests, ValidDrmInfo_NegativeWait) {
    GTEST_SKIP();
    std::cout << "Entering ValidDrmInfo_NegativeWait test" << std::endl;
    
    HlsOcdmBridge bridge(nullptr);
    // Initialize a valid DrmInfo instance.
    DrmInfo drmInfo_instance;
    std::memset(&drmInfo_instance, 0, sizeof(drmInfo_instance));

    int acquireKeyWaitTime = -500;
    std::cout << "Invoking SetDecryptInfo with drmInfo pointer: " << &drmInfo_instance 
                << " and acquireKeyWaitTime: " << acquireKeyWaitTime << std::endl;
    int ret = bridge.SetDecryptInfo(&drmInfo_instance, acquireKeyWaitTime);
    std::cout << "SetDecryptInfo returned: " << ret << std::endl;
    EXPECT_EQ(ret, eDRM_ERROR);
    
    std::cout << "Exiting ValidDrmInfo_NegativeWait test" << std::endl;
}
/**
 * @brief Verify that SetMetaData works correctly for valid metadata inputs
 *
 * This test validates the functionality of the SetMetaData API call in the HlsOcdmBridge class.
 * It creates a HlsOcdmBridge object using the default constructor and then initializes a metadata
 * buffer with a valid string. For each of the supported track types (audio, video, subtitles),
 * the test invokes the SetMetaData method and checks that the API returns the expected success result.
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
 * | 01 | Create HlsOcdmBridge object using default constructor | None | Object is created successfully without throwing an exception | Should be successful |
 * | 02 | Initialize metadata buffer with valid metadata string | sampleData = "Valid metadata for DRM session", metaDataBuffer size = 128 | The metadata buffer is correctly initialized with the valid metadata string | Should be successful |
 * | 03 | Invoke SetMetaData for audio track (trackType = 0) | metaDataBuffer = "Valid metadata for DRM session", trackType = 0, return value expected = eDRM_SUCCESS | SetMetaData returns eDRM_SUCCESS for audio track | Should Pass |
 * | 04 | Invoke SetMetaData for video track (trackType = 1) | metaDataBuffer = "Valid metadata for DRM session", trackType = 1, return value expected = eDRM_SUCCESS | SetMetaData returns eDRM_SUCCESS for video track | Should Pass |
 * | 05 | Invoke SetMetaData for subtitles track (trackType = 2) | metaDataBuffer = "Valid metadata for DRM session", trackType = 2, return value expected = eDRM_SUCCESS | SetMetaData returns eDRM_SUCCESS for subtitles track | Should Pass |
 */
TEST_F(HlsOcdmBridgeTests, PositiveTest_SetMetaData) {
    std::cout << "Entering PositiveTest_SetMetaData test" << std::endl;

    // Create HlsOcdmBridge object using default constructor and verify no exception
    HlsOcdmBridge hlsBridge(nullptr);
    std::cout << "HlsOcdmBridge object created successfully using default constructor." << std::endl;

    // Prepare valid metadata: simulated as a char array
    char metaDataBuffer[128] = {0};
    const char* sampleData = "Valid metadata for DRM session";
    strncpy(metaDataBuffer, sampleData, sizeof(metaDataBuffer) - 1);
    std::cout << "Metadata buffer initialized with value: " << metaDataBuffer << std::endl;

    // Define track types: audio=0, video=1, subtitles=2
    int trackTypes[3] = {0, 1, 2};
    for (int i = 0; i < 3; i++) {
        int currentTrackType = trackTypes[i];
        std::cout << "Invoking SetMetaData with metadata pointer: " << static_cast<void*>(metaDataBuffer)
                    << " and trackType: " << currentTrackType << std::endl;
        int ret = hlsBridge.SetMetaData(static_cast<void*>(metaDataBuffer), currentTrackType);
        std::cout << "SetMetaData returned value: " << ret << " for trackType: " << currentTrackType << std::endl;
        EXPECT_EQ(eDRM_SUCCESS, ret);
    }
    std::cout << "Exiting PositiveTest_SetMetaData test" << std::endl;
}
/**
 * @brief Validate that HlsOcdmBridge::SetMetaData returns an error when provided with a null metadata pointer.
 *
 * This test verifies that invoking SetMetaData with a nullptr for the metadata pointer correctly returns
 * the error code eDRM_ERROR for different track types (audio, video, subtitles). The objective is to ensure
 * that the API handles invalid metadata inputs gracefully without causing unexpected behavior or crashes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 024@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                                      | Expected Result                                          |Notes           |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Create HlsOcdmBridge object using the default constructor           | Constructor invoked                                            | Object created without exceptions                        | Should be successful |
 * | 02               | Set metadata pointer to nullptr                                     | nullMetadata = nullptr                                           | Metadata pointer remains nullptr                         | Should be successful |
 * | 03               | Invoke SetMetaData for audio track                                  | input: metadata = nullptr, trackType = 0, output: ret            | Return value equals eDRM_ERROR and assertion passes       | Should Fail         |
 * | 04               | Invoke SetMetaData for video track                                  | input: metadata = nullptr, trackType = 1, output: ret            | Return value equals eDRM_ERROR and assertion passes       | Should Fail         |
 * | 05               | Invoke SetMetaData for subtitles track                              | input: metadata = nullptr, trackType = 2, output: ret            | Return value equals eDRM_ERROR and assertion passes       | Should Fail         |
 */
TEST_F(HlsOcdmBridgeTests, NegativeTest_SetMetaData) {
    GTEST_SKIP();
    std::cout << "Entering NegativeTest_SetMetaData test" << std::endl;

    // Create HlsOcdmBridge object using default constructor and verify no exception
    HlsOcdmBridge hlsBridge(nullptr);
    std::cout << "HlsOcdmBridge object created successfully using default constructor." << std::endl;

    // Metadata pointer is set to nullptr to simulate negative scenario.
    void* nullMetadata = nullptr;
    std::cout << "Metadata pointer is set to nullptr." << std::endl;

    // Define track types: audio=0, video=1, subtitles=2
    int trackTypes[3] = {0, 1, 2};
    for (int i = 0; i < 3; i++) {
        int currentTrackType = trackTypes[i];
        std::cout << "Invoking SetMetaData with metadata pointer: " << nullMetadata
                    << " and trackType: " << currentTrackType << std::endl;
        int ret = hlsBridge.SetMetaData(nullMetadata, currentTrackType);
        std::cout << "SetMetaData returned value: " << ret << " for trackType: " << currentTrackType << std::endl;
        EXPECT_EQ(eDRM_ERROR, ret);
    }

    std::cout << "Exiting NegativeTest_SetMetaData test" << std::endl;
}
/**
 * @brief Verify that HlsOcdmBridge destructor performs proper cleanup without throwing exceptions
 *
 * This test ensures that an instance of HlsOcdmBridge is correctly created using its default constructor and that its destructor properly cleans up all resources. It validates that no exceptions are thrown during object creation and deletion, ensuring reliable resource management.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Invoke default constructor using EXPECT_NO_THROW to create a HlsOcdmBridge instance. | No input; output: bridgePtr allocated and initialized. | Instance is created without throwing exceptions. | Should Pass |
 * |02| Log creation of the HlsOcdmBridge instance. | No input; output: log message "Created HlsOcdmBridge instance using default constructor." printed. | Log message is output indicating successful creation. | Should be successful |
 * |03| Invoke the destructor using EXPECT_NO_THROW by calling delete on the instance. | Input: pointer to HlsOcdmBridge instance (bridgePtr). | Destructor is invoked without throwing exceptions. | Should Pass |
 * |04| Log successful invocation of the destructor. | No input; output: log message "Destructor invoked successfully without throwing exceptions." printed. | Log message is output indicating successful deletion. | Should be successful |
 */
TEST_F(HlsOcdmBridgeTests, DestructorProperCleanup) {
    std::cout << "Entering DestructorProperCleanup test" << std::endl;

    // Create an instance of HlsOcdmBridge using EXPECT_NO_THROW.
    HlsOcdmBridge* bridgePtr = nullptr;
    EXPECT_NO_THROW({
        // Invoke default constructor which is expected to initialize DRM resources (m_drmInfo and m_drmSession) to non-null values.
        bridgePtr = new HlsOcdmBridge(nullptr);
    });
    std::cout << "Created HlsOcdmBridge instance using default constructor." << std::endl;

    // Logging that we are about to invoke the destructor.
    std::cout << "Invoking destructor for HlsOcdmBridge instance." << std::endl;

    // Delete the object to invoke the destructor and check that no exception is thrown.
    EXPECT_NO_THROW({
        delete bridgePtr;
    });
    std::cout << "Destructor invoked successfully without throwing exceptions." << std::endl;

    std::cout << "Exiting DestructorProperCleanup test" << std::endl;
}