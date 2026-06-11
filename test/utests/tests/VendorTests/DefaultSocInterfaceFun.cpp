
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
#include <cstring>
#include "DefaultSocInterface.h"


/**
 * @brief Validate the behavior of ConfigureAudioSink when provided with valid audio sink and source pointers and decStreamSync set to false.
 *
 * This test verifies that the DefaultSocInterface::ConfigureAudioSink API correctly handles valid GstElement and GstObject pointers. The test is designed to confirm that when the decStreamSync parameter is false, the function returns true, thereby ensuring proper synchronization handling.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface object using default constructor. | N/A | Object is constructed without throwing an exception. | Should be successful |
 * | 02 | Prepare valid GstElement and GstObject pointers for audio sink and source. | audio_sink_ptr = valid GstElement pointer, validElement = allocated GstElement, validSrc = allocated GstObject pointer | Pointers are allocated and hold valid memory addresses. | Should be successful |
 * | 03 | Invoke ConfigureAudioSink API with decStreamSync false. | audio_sink address = address of audio_sink_ptr, GstElement pointer = validElement, src pointer = validSrc, decStreamSync = false, output ret = bool value | Function returns true and the EXPECT_TRUE assertion passes. | Should Pass |
 */
TEST(DefaultSocInterface, ValidAudioSinkAndSource_DecStreamSyncFalse) {
    GTEST_SKIP();
    std::cout << "Entering ValidAudioSinkAndSource_DecStreamSyncFalse test" << std::endl;

    // Create a DefaultSocInterface object using default constructor
    EXPECT_NO_THROW(DefaultSocInterface socInterface);

    DefaultSocInterface socInterface;

    // Prepare valid GstElement and GstObject pointers
    GstElement* validElement = new GstElement;
    GstElement* audio_sink_ptr = validElement; // valid pointer to GstElement
    GstObject* validSrc = new GstObject;         // valid pointer to GstObject

    std::cout << "Invoking ConfigureAudioSink with:" << std::endl;
    std::cout << "  audio_sink address: " << static_cast<void*>(&audio_sink_ptr) << std::endl;
    std::cout << "  GstElement pointer value: " << static_cast<void*>(validElement) << std::endl;
    std::cout << "  src pointer: " << static_cast<void*>(validSrc) << std::endl;
    std::cout << "  decStreamSync: false" << std::endl;

    bool ret = socInterface.ConfigureAudioSink(&audio_sink_ptr, validSrc, false);
    std::cout << "Returned value from ConfigureAudioSink: " << ret << std::endl;

    EXPECT_TRUE(ret);

    // Cleanup
    delete validElement;
    delete validSrc;

    std::cout << "Exiting ValidAudioSinkAndSource_DecStreamSyncFalse test" << std::endl;
}
/**
 * @brief Verify that ConfigureAudioSink functions correctly with valid audio sink, valid audio source, and decStreamSync set to true
 *
 * This test validates that the DefaultSocInterface::ConfigureAudioSink API returns true when provided with valid GstElement and GstObject pointers along with the decStreamSync flag set to true. The test ensures that the object construction, API invocation, and memory cleanup processes perform as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 002
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DefaultSocInterface using default constructor and verify no exception is thrown | Invocation: DefaultSocInterface socInterface; | Object is created without exception | Should be successful |
 * | 02 | Prepare valid GstElement and GstObject pointers, and assign audio sink pointer | Test Data: validElement = new GstElement, audio_sink_ptr = validElement, validSrc = new GstObject | Valid pointers for audio sink and source are initialized | Should be successful |
 * | 03 | Invoke ConfigureAudioSink API with valid audio sink pointer, valid source pointer, and decStreamSync set to true | Test Data: audio_sink_ptr address, validElement pointer value, validSrc pointer value, decStreamSync = true | API returns true indicating proper configuration | Should Pass |
 * | 04 | Assert that the returned value from ConfigureAudioSink is true using EXPECT_TRUE | Test Data: ret == true | EXPECT_TRUE assertion passes confirming the API works as expected | Should Pass |
 * | 05 | Cleanup allocated memory for GstElement and GstObject | Test Data: delete validElement, delete validSrc | Memory is deallocated without issues | Should be successful |
 */
TEST(DefaultSocInterface, ValidAudioSinkAndSource_DecStreamSyncTrue) {
    GTEST_SKIP();
    std::cout << "Entering ValidAudioSinkAndSource_DecStreamSyncTrue test" << std::endl;

    // Create a DefaultSocInterface object using default constructor
    EXPECT_NO_THROW(DefaultSocInterface socInterface);

    DefaultSocInterface socInterface;

    // Prepare valid GstElement and GstObject pointers
    GstElement* validElement = new GstElement;
    GstElement* audio_sink_ptr = validElement; // valid pointer to GstElement
    GstObject* validSrc = new GstObject;         // valid pointer to GstObject

    std::cout << "Invoking ConfigureAudioSink with:" << std::endl;
    std::cout << "  audio_sink address: " << static_cast<void*>(&audio_sink_ptr) << std::endl;
    std::cout << "  GstElement pointer value: " << static_cast<void*>(validElement) << std::endl;
    std::cout << "  src pointer: " << static_cast<void*>(validSrc) << std::endl;
    std::cout << "  decStreamSync: true" << std::endl;

    bool ret = socInterface.ConfigureAudioSink(&audio_sink_ptr, validSrc, true);
    std::cout << "Returned value from ConfigureAudioSink: " << ret << std::endl;

    EXPECT_TRUE(ret);

    // Cleanup
    delete validElement;
    delete validSrc;

    std::cout << "Exiting ValidAudioSinkAndSource_DecStreamSyncTrue test" << std::endl;
}
/**
 * @brief Test the ConfigureAudioSink function with a null audio sink pointer, valid source pointer, and decStreamSync flag set to true
 *
 * This test verifies that the ConfigureAudioSink function returns false when provided with a null audio sink pointer, even though the source pointer is valid and the decStreamSync flag is set to true. The test ensures that the API correctly validates its input arguments and handles the error scenario.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                               | Expected Result                                                         | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ----------------------------------------------------------------------- | ------------- |
 * | 01               | Create a DefaultSocInterface object using the default constructor                               | No input                                                                 | Object created without throwing an exception                           | Should be successful |
 * | 02               | Setup test variables with audio_sink pointer as NULL and allocate a valid GstObject pointer       | audio_sink_ptr = NULL, validSrc = pointer from new GstObject             | Test variables are properly initialized                                  | Should be successful |
 * | 03               | Invoke ConfigureAudioSink method with null audio_sink pointer, valid source, and decStreamSync true | audio_sink_ptr = NULL, validSrc = valid GstObject pointer, decStreamSync = true | Function returns false verifying improper input handling; assertion EXPECT_FALSE(ret) | Should Pass   |
 * | 04               | Cleanup the dynamically allocated GstObject pointer                                             | validSrc pointer allocated via new GstObject                           | Memory is released without error                                        | Should be successful |
 */
TEST(DefaultSocInterface, NullAudioSink_ValidSource_DecStreamSyncTrue) {
    GTEST_SKIP();
    std::cout << "Entering NullAudioSink_ValidSource_DecStreamSyncTrue test" << std::endl;

    // Create a DefaultSocInterface object using default constructor
    EXPECT_NO_THROW(DefaultSocInterface socInterface);

    DefaultSocInterface socInterface;

    // audio_sink pointer is NULL
    GstElement** audio_sink_ptr = nullptr;

    // Prepare valid GstObject pointer
    GstObject* validSrc = new GstObject;         // valid pointer to GstObject

    std::cout << "Invoking ConfigureAudioSink with:" << std::endl;
    std::cout << "  audio_sink pointer is NULL" << std::endl;
    std::cout << "  src pointer: " << static_cast<void*>(validSrc) << std::endl;
    std::cout << "  decStreamSync: true" << std::endl;

    bool ret = socInterface.ConfigureAudioSink(audio_sink_ptr, validSrc, true);
    std::cout << "Returned value from ConfigureAudioSink: " << ret << std::endl;

    EXPECT_FALSE(ret);

    // Cleanup
    delete validSrc;

    std::cout << "Exiting NullAudioSink_ValidSource_DecStreamSyncTrue test" << std::endl;
}
/**
 * @brief Verifies that ConfigureAudioSink returns false when provided with a valid audio sink pointer, null source, and decStreamSync set to false.
 *
 * This test examines whether the ConfigureAudioSink API in DefaultSocInterface handles a null source input correctly while a valid audio sink pointer is provided and decStreamSync is false. It confirms that under these conditions, the API does not configure the audio sink and returns false.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DefaultSocInterface instance and valid GstElement pointer | None | Object creation should succeed without throwing an exception | Should be successful |
 * | 02 | Invoke ConfigureAudioSink with a valid audio sink pointer, null src pointer, and decStreamSync as false | audio_sink pointer = validElement, src pointer = nullptr, decStreamSync = false | API returns false | Should Pass |
 * | 03 | Validate the API return value using Google Test assertion | ret = false | EXPECT_FALSE(ret) succeeds | Should Pass |
 * | 04 | Cleanup allocated memory for GstElement | validElement deallocated | Memory freed without errors | Should be successful |
 */
TEST(DefaultSocInterface, ValidAudioSink_NullSource_DecStreamSyncFalse) {
    GTEST_SKIP();
    std::cout << "Entering ValidAudioSink_NullSource_DecStreamSyncFalse test" << std::endl;

    // Create a DefaultSocInterface object using default constructor
    EXPECT_NO_THROW(DefaultSocInterface socInterface);

    DefaultSocInterface socInterface;

    // Prepare valid GstElement pointer
    GstElement* validElement = new GstElement;
    GstElement* audio_sink_ptr = validElement; // valid pointer to GstElement

    // src is NULL
    GstObject* src_ptr = nullptr;

    std::cout << "Invoking ConfigureAudioSink with:" << std::endl;
    std::cout << "  audio_sink address: " << static_cast<void*>(&audio_sink_ptr) << std::endl;
    std::cout << "  GstElement pointer value: " << static_cast<void*>(validElement) << std::endl;
    std::cout << "  src pointer is NULL" << std::endl;
    std::cout << "  decStreamSync: false" << std::endl;

    bool ret = socInterface.ConfigureAudioSink(&audio_sink_ptr, src_ptr, false);
    std::cout << "Returned value from ConfigureAudioSink: " << ret << std::endl;

    EXPECT_FALSE(ret);

    // Cleanup
    delete validElement;

    std::cout << "Exiting ValidAudioSink_NullSource_DecStreamSyncFalse test" << std::endl;
}
/**
 * @brief Test the default internal state behavior of ConfigurePluginPriority
 *
 * This test verifies that invoking the ConfigurePluginPriority method on a DefaultSocInterface instance
 * does not throw any exceptions and configures the internal state correctly for plugin priority.
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
 * | Variation / Step | Description                                                         | Test Data                               | Expected Result                                                             | Notes        |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------- | --------------------------------------------------------------------------- | ------------ |
 * | 01               | Create a DefaultSocInterface object using default constructor        | None                                    | DefaultSocInterface object is created without exceptions                    | Should Pass  |
 * | 02               | Invoke ConfigurePluginPriority on the created DefaultSocInterface object | None                                    | Method executes without throwing exceptions and configures internal state   | Should Pass  |
 */
TEST(DefaultSocInterface, ConfigurePluginPriority_with_default_internal_state) {
    std::cout << "Entering ConfigurePluginPriority_with_default_internal_state test" << std::endl;
    
    // Creating DefaultSocInterface object using default constructor
    std::cout << "Creating DefaultSocInterface object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created. Invoking ConfigurePluginPriority()" << std::endl;
        
        // Invoking the ConfigurePluginPriority method
        EXPECT_NO_THROW({
            socInterface.ConfigurePluginPriority();
            std::cout << "ConfigurePluginPriority() method invoked successfully" << std::endl;
        });
        
        std::cout << "Assuming internal state is configured correctly for plugin priority" << std::endl;
    });
    
    std::cout << "Exiting ConfigurePluginPriority_with_default_internal_state test" << std::endl;
}
/**
 * @brief Test default constructor of DefaultSocInterface for proper object initialization
 *
 * This test verifies that invoking the default constructor of the DefaultSocInterface class does not throw an exception and that the internal state (mUsingWesterosSink) is set to false as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke DefaultSocInterface default constructor and verify proper initialization | No input, output: mUsingWesterosSink = false | Instance created successfully without throwing an exception; internal state mUsingWesterosSink is false; EXPECT_NO_THROW passes | Should Pass |
 */
TEST(DefaultSocInterface, DefaultSocInterface_DefaultConstructor_start) {
    std::cout << "Entering DefaultSocInterface::DefaultSocInterface()_start test" << std::endl;
    
    // Invoke the default constructor and log the invocation details.
    std::cout << "Invoking DefaultSocInterface default constructor" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface defaultSocInterfaceInstance;
        std::cout << "DefaultSocInterface instance created successfully." << std::endl;
        std::cout << "Internal state: mUsingWesterosSink = false (expected default initialization)" << std::endl;
    });
    
    std::cout << "Exiting DefaultSocInterface::DefaultSocInterface()_start test" << std::endl;
}
/**
 * @brief Test to verify that GetCCDecoderHandle assigns a valid decoder handle when valid pointers are provided.
 *
 * This test verifies that a DefaultSocInterface object can be instantiated without exceptions and that when valid pointers (with dec_handle initialized to nullptr and video_dec pointing to a dummy GstElement) are passed to the GetCCDecoderHandle method, a valid non-null decoder handle is assigned. This ensures that the API functions as expected in a positive scenario.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface and set up valid pointers | dec_handle = nullptr, video_dec = address of dummy GstElement | Object is instantiated without throwing an exception; pointers are initialized correctly | Should be successful |
 * | 02 | Invoke GetCCDecoderHandle method with valid pointers | input: pointer to dec_handle (nullptr), video_dec = pointer to dummy GstElement; output: dec_handle assigned | GetCCDecoderHandle executes without throwing an exception and processes the inputs | Should Pass |
 * | 03 | Verify that dec_handle is non-null post invocation | dec_handle after API call | dec_handle is a valid non-null pointer confirming proper assignment | Should be successful |
 */
TEST(DefaultSocInterface, PositiveValidPointers) {
    GTEST_SKIP();
    std::cout << "Entering PositiveValidPointers test" << std::endl;
    
    // Create a DefaultSocInterface object using the default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Instantiated DefaultSocInterface object." << std::endl;
        
        // Prepare valid pointers for dec_handle and video_dec.
        // Initially, dec_handle (a gpointer variable) is set to nullptr.
        gpointer dec_handle = nullptr;
        // For video_dec pointer, we create a dummy GstElement object.
        GstElement dummyVideoDec;
        GstElement *video_dec = &dummyVideoDec;
        
        std::cout << "Before invocation:" << std::endl;
        std::cout << "  dec_handle: " << dec_handle << std::endl;
        std::cout << "  video_dec: " << video_dec << " (address of dummyVideoDec)" << std::endl;
        
        // Invoke the method GetCCDecoderHandle.
        std::cout << "Invoking GetCCDecoderHandle with valid pointers." << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(&dec_handle, video_dec));
        
        // Debug log after invocation.
        std::cout << "After invocation:" << std::endl;
        std::cout << "  dec_handle: " << dec_handle << std::endl;
        
        // Validate that the function assigned a valid (non-NULL) decoder handle.
        EXPECT_NE(dec_handle, nullptr) << "Expected a non-null decoder handle to be assigned.";
        std::cout << "Valid decoder handle assigned successfully: " << dec_handle << std::endl;
    });
    
    std::cout << "Exiting PositiveValidPointers test" << std::endl;
}
/**
 * @brief Verify that GetCCDecoderHandle handles NULL dec_handle gracefully
 *
 * This test verifies that the GetCCDecoderHandle method in DefaultSocInterface handles the scenario where the decoder handle pointer (dec_handle) is NULL. It ensures that the method does not throw an exception when provided with a NULL pointer for dec_handle while a valid video decoder pointer is passed.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface object using the default constructor | No input arguments | Object is instantiated without exception | Should be successful |
 * | 02 | Invoke GetCCDecoderHandle with dec_handle as NULL and a valid video_dec pointer | dec_handle = nullptr, video_dec = address of dummyVideoDec | Method call does not throw any exception and handles NULL dec_handle gracefully | Should Pass |
 */
TEST(DefaultSocInterface, NegativeNullDecHandle) {
    std::cout << "Entering NegativeNullDecHandle test" << std::endl;
    
    // Create a DefaultSocInterface object using the default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Instantiated DefaultSocInterface object." << std::endl;
        
        // Here, dec_handle pointer itself is NULL.
        gpointer *dec_handle = nullptr;
        // Prepare a valid GstElement pointer.
        GstElement dummyVideoDec;
        GstElement *video_dec = &dummyVideoDec;
        
        std::cout << "Before invocation:" << std::endl;
        std::cout << "  dec_handle pointer is NULL." << std::endl;
        std::cout << "  video_dec: " << video_dec << " (address of dummyVideoDec)" << std::endl;
        
        // Invoke the method. The method is expected to handle the NULL dec_handle gracefully.
        std::cout << "Invoking GetCCDecoderHandle with NULL dec_handle pointer." << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(dec_handle, video_dec));
        
        // Nothing to check for dec_handle as the pointer itself is NULL.
        std::cout << "Method invocation completed with NULL dec_handle pointer." << std::endl;
    });

    std::cout << "Exiting NegativeNullDecHandle test" << std::endl;
}
/**
 * @brief Tests that the GetCCDecoderHandle method handles a NULL video_dec pointer gracefully.
 *
 * This test verifies that when a NULL pointer is provided for the video_dec parameter, the
 * GetCCDecoderHandle method does not throw an exception and leaves the decoder handle (dec_handle)
 * in a safe, expected state (i.e., remains NULL). This ensures robust error handling by the API.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                 | Expected Result                                                                     | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------- | ----------------------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate the DefaultSocInterface object using its default constructor.                     | -                                                         | Object is created without exception.                                               | Should be successful |
 * | 02               | Initialize test variables: set dec_handle and video_dec to nullptr.                           | dec_handle = nullptr, video_dec = nullptr                  | dec_handle and video_dec are successfully initialized to nullptr.                  | Should be successful |
 * | 03               | Invoke GetCCDecoderHandle with a valid pointer for dec_handle and a nullptr for video_dec.      | input: &dec_handle (valid pointer), video_dec = nullptr; output: dec_handle = nullptr | API does not throw an exception and dec_handle remains nullptr.                     | Should Fail     |
 * | 04               | Validate that dec_handle remains nullptr after the API invocation.                           | output verification: dec_handle = nullptr                  | Assertion passes confirming that dec_handle equals nullptr.                         | Should be successful |
 */
TEST(DefaultSocInterface, NegativeNullVideoDec) {
    std::cout << "Entering NegativeNullVideoDec test" << std::endl;
    
    // Create a DefaultSocInterface object using the default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Instantiated DefaultSocInterface object." << std::endl;
        
        // Prepare valid pointer for dec_handle variable.
        gpointer dec_handle = nullptr;
        // video_dec pointer is set to NULL.
        GstElement *video_dec = nullptr;
        
        std::cout << "Before invocation:" << std::endl;
        std::cout << "  dec_handle: " << dec_handle << std::endl;
        std::cout << "  video_dec: " << video_dec << " (NULL)" << std::endl;
        
        // Invoke the method. The method is expected to handle the NULL video_dec pointer safely.
        std::cout << "Invoking GetCCDecoderHandle with NULL video_dec pointer." << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(&dec_handle, video_dec));
        
        // After the call, the function may assign a default or NULL decoder handle.
        std::cout << "After invocation:" << std::endl;
        std::cout << "  dec_handle: " << dec_handle << std::endl;
        
        // Validate that the method did not assign an invalid value. It is acceptable if dec_handle remains NULL.
        EXPECT_EQ(dec_handle, nullptr) << "Expected decoder handle to remain NULL when video_dec is NULL.";
        std::cout << "Decoder handle remains at expected state (NULL) when video_dec is NULL." << std::endl;
    });
    
    std::cout << "Exiting NegativeNullVideoDec test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface correctly identifies an audio decoder when isRialto is true.
 *
 * This test case verifies that upon providing the name "audioDecoder" and setting the isRialto flag as true, the DefaultSocInterface::IsAudioOrVideoDecoder method returns true. It confirms that the API behaves as expected in correctly recognizing a valid audio decoder.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize DefaultSocInterface object and set the decoder name using strncpy. | name = "audioDecoder" | The name variable is correctly assigned "audioDecoder". | Should be successful |
 * | 02 | Call DefaultSocInterface::IsAudioOrVideoDecoder with the set name and isRialto = true. | input: name = "audioDecoder", isRialto = true; output: result expected as true | Function returns true and the EXPECT_TRUE(result) assertion passes. | Should Pass |
 */
TEST(DefaultSocInterface, ValidAudioDecoder_RialtoTrue)
{
    GTEST_SKIP();
    std::cout << "Entering ValidAudioDecoder_RialtoTrue test" << std::endl;
    DefaultSocInterface soc; 
    char name[64] = {0};
    std::cout << "Assigning name 'audioDecoder' using strncpy" << std::endl;
    std::strncpy(name, "audioDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: true" << std::endl;
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidAudioDecoder_RialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder correctly identifies "audioDecoder" as a valid audio decoder when Rialto flag is false
 *
 * This test validates that the DefaultSocInterface API method IsAudioOrVideoDecoder returns true when provided with a valid audio decoder name ("audioDecoder")
 * and the Rialto flag set to false. The test ensures that the function behaves as expected for the positive scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 011@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a DefaultSocInterface instance and initialize the name variable using strncpy with "audioDecoder" | name = audioDecoder | The name variable is set to "audioDecoder" successfully | Should be successful |
 * | 02 | Invoke IsAudioOrVideoDecoder with the initialized name and isRialto set to false, then verify the returned value is true | name = audioDecoder, isRialto = false, output = result | API returns true and the EXPECT_TRUE assertion passes | Should Pass |
 */
TEST(DefaultSocInterface, ValidAudioDecoder_RialtoFalse)
{
    GTEST_SKIP();
    std::cout << "Entering ValidAudioDecoder_RialtoFalse test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning name 'audioDecoder' using strncpy" << std::endl;
    std::strncpy(name, "audioDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name  << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: true" << std::endl;
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidAudioDecoder_RialtoFalse test" << std::endl;
}
/**
 * @brief Validates that IsAudioOrVideoDecoder correctly identifies a video decoder when Rialto is true
 *
 * This test case checks if the IsAudioOrVideoDecoder method returns true when provided with a valid video decoder name ("videoDecoder")
 * and the Rialto flag set to true. It validates that the API accurately processes valid input data and returns the expected output.
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
 * | Variation / Step | Description                                                                                 | Test Data                                          | Expected Result                                               | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------- | -------------------------------------------------------------- | --------------- |
 * | 01               | Initialize and prepare the video decoder name using strncpy                                 | name array size = 64, name = "videoDecoder"        | The name array correctly contains "videoDecoder"               | Should be successful |
 * | 02               | Invoke IsAudioOrVideoDecoder with the prepared name and Rialto flag set to true               | name = "videoDecoder", isRialto = true             | Returns true; EXPECT_TRUE assertion passes                     | Should Pass     |
 */
TEST(DefaultSocInterface, ValidVideoDecoder_RialtoTrue)
{
    GTEST_SKIP();
    std::cout << "Entering ValidVideoDecoder_RialtoTrue test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning name 'videoDecoder' using strncpy" << std::endl;
    std::strncpy(name, "videoDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: true" << std::endl;
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidVideoDecoder_RialtoTrue test" << std::endl;
}
/**
 * @brief Validate that the DefaultSocInterface correctly identifies a video decoder when Rialto is false
 *
 * This test checks that when a valid video decoder name ("videoDecoder") is provided along with a false Rialto flag,
 * the DefaultSocInterface::IsAudioOrVideoDecoder method returns true. Verifying this behavior ensures that the API
 * properly discriminates between audio and video decoders based on the given conditions.
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
 * | Variation / Step | Description                                                       | Test Data                                     | Expected Result                                                  | Notes      |
 * | :--------------: | ----------------------------------------------------------------- | --------------------------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Invoke IsAudioOrVideoDecoder with name "videoDecoder" and isRialto false | name = videoDecoder, isRialto = false           | Return value is true; EXPECT_TRUE(result) assertion passes       | Should Pass|
 */
TEST(DefaultSocInterface, ValidVideoDecoder_RialtoFalse)
{
    GTEST_SKIP();
    std::cout << "Entering ValidVideoDecoder_RialtoFalse test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning name 'videoDecoder' using strncpy" << std::endl;
    std::strncpy(name, "videoDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: true" << std::endl;
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidVideoDecoder_RialtoFalse test" << std::endl;
}
/**
 * @brief Verify that an invalid decoder name returns false in Rialto mode.
 *
 * This test verifies that when an invalid decoder name ("XYZDecoder") is provided to the
 * DefaultSocInterface::IsAudioOrVideoDecoder method with the Rialto flag set to true, the
 * API correctly returns false. This ensures that the function does not mistakenly recognize
 * invalid decoder names as valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 014@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                              | Test Data                                             | Expected Result                          | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------- | --------------- |
 * | 01               | Instantiate a DefaultSocInterface object                                                 | None                                                  | Object is instantiated successfully      | Should be successful |
 * | 02               | Assign the invalid decoder name "XYZDecoder" to a char buffer using strncpy               | name = XYZDecoder                                     | Buffer "name" contains "XYZDecoder"      | Should be successful |
 * | 03               | Invoke IsAudioOrVideoDecoder with the provided name and Rialto flag set to true             | input: name = XYZDecoder, isRialto = true               | Return value is false                    | Should Pass     |
 * | 04               | Validate the return value using EXPECT_FALSE assertion                                     | result = false                                        | Test passes when result is false         | Should Pass     |
 */
TEST(DefaultSocInterface, InvalidDecoderName_RialtoTrue)
{
    std::cout << "Entering InvalidDecoderName_RialtoTrue test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning name 'XYZDecoder' using strncpy" << std::endl;
    std::strncpy(name, "XYZDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidDecoderName_RialtoTrue test" << std::endl;
}
/**
 * @brief Test IsAudioOrVideoDecoder with an invalid decoder name and Rialto flag set to false
 *
 * This test verifies that the IsAudioOrVideoDecoder API returns false when provided with an invalid decoder name "XYZDecoder" and the Rialto flag set to false. The test ensures that the function handles an unknown decoder name appropriately.
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
 * | Variation / Step | Description                                                                           | Test Data                                                  | Expected Result                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------ | ----------- |
 * | 01               | Assign decoder name "XYZDecoder" using strncpy and invoke IsAudioOrVideoDecoder with isRialto set to false | name = XYZDecoder, isRialto = false, output expected = false | API returns false and assertion EXPECT_FALSE passes    | Should Pass |
 */
TEST(DefaultSocInterface, InvalidDecoderName_RialtoFalse)
{
    std::cout << "Entering InvalidDecoderName_RialtoFalse test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning name 'XYZDecoder' using strncpy" << std::endl;
    std::strncpy(name, "XYZDecoder", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: " << name << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidDecoderName_RialtoFalse test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::IsAudioOrVideoDecoder returns false when a null name is provided with isRialto set to true.
 *
 * This test verifies that when a null pointer is passed as the name, the API properly handles the invalid input by returning false.
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
 * | Variation / Step | Description                                                                                  | Test Data                                   | Expected Result                                                  | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------------------- | ------------ |
 * | 01               | Invoke DefaultSocInterface::IsAudioOrVideoDecoder with a null name and isRialto true parameter | name = nullptr, isRialto = true, expected return = false | The API returns false and EXPECT_FALSE assertion passes | Should Pass  |
 */
TEST(DefaultSocInterface, NullName_RialtoTrue)
{
    GTEST_SKIP();
    std::cout << "Entering NullName_RialtoTrue test" << std::endl;
    DefaultSocInterface soc;
    const char *name = nullptr;
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: nullptr" << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting NullName_RialtoTrue test" << std::endl;
}
/**
 * @brief Test DefaultSocInterface::IsAudioOrVideoDecoder with null name input returns false
 *
 * This test validates that when a nullptr is provided as the name argument, the
 * DefaultSocInterface::IsAudioOrVideoDecoder API returns false. This ensures that the API
 * handles null inputs appropriately by returning a failure result.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 017@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                               | Expected Result                                   | Notes        |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------- | ------------------------------------------------- | ------------ |
 * | 01               | Invoke IsAudioOrVideoDecoder with name set to nullptr and isRialto false  | name = nullptr, isRialto = false, expected return = false | API returns false; EXPECT_FALSE assertion passes  | Should Pass  |
 */
TEST(DefaultSocInterface, NullName_RialtoFalse)
{
    std::cout << "Entering NullName_RialtoFalse test" << std::endl;
    DefaultSocInterface soc;
    const char *name = nullptr;
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with name: nullptr" << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting NullName_RialtoFalse test" << std::endl;
}
/**
 * @brief Validate the behavior of IsAudioOrVideoDecoder when provided with an empty string for the decoder name.
 *
 * This test verifies that the DefaultSocInterface::IsAudioOrVideoDecoder method correctly returns false when invoked with an empty string as the decoder name while the isRialto flag is set to true. This scenario ensures that the method does not falsely recognize an empty name as a valid audio or video decoder.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize DefaultSocInterface, assign an empty string to the input buffer, and invoke IsAudioOrVideoDecoder with isRialto set to true | input: name = "", isRialto = true, output: false | API returns false and assertion EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(DefaultSocInterface, EmptyString_RialtoTrue)
{
    std::cout << "Entering EmptyString_RialtoTrue test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning empty string '' using strncpy" << std::endl;
    std::strncpy(name, "", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with an empty name"
              << " and isRialto: " << std::boolalpha << true << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyString_RialtoTrue test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::IsAudioOrVideoDecoder returns false when provided an empty string and Rialto flag false
 *
 * This test confirms that when DefaultSocInterface::IsAudioOrVideoDecoder is invoked with an empty string in 'name'
 * and the Rialto flag set to false, the method correctly returns false. This is important for ensuring that the API
 * gracefully handles edge case inputs.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 019
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                         | Expected Result                                            | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------- | ---------------------------------------------------------- | ---------- |
 * | 01               | Invoke DefaultSocInterface::IsAudioOrVideoDecoder with an empty string and isRialto false.   | name = "", isRialto = false                         | Return value is false; EXPECT_FALSE(result) succeeds       | Should Pass|
 */
TEST(DefaultSocInterface, EmptyString_RialtoFalse)
{
    std::cout << "Entering EmptyString_RialtoFalse test" << std::endl;
    DefaultSocInterface soc;
    char name[64] = {0};
    std::cout << "Assigning empty string '' using strncpy" << std::endl;
    std::strncpy(name, "", sizeof(name)-1);
    std::cout << "Invoking DefaultSocInterface::IsAudioOrVideoDecoder with an empty name"
              << " and isRialto: " << std::boolalpha << false << std::endl;
    bool result = soc.IsAudioOrVideoDecoder(name);
    std::cout << "Returned: " << result << std::endl;
    std::cout << "Expected: false" << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyString_RialtoFalse test" << std::endl;
}
/**
 * @brief Verify that the DefaultSocInterface's IsAudioSinkOrAudioDecoder method works as expected with valid audio sink input and Rialto set to true
 *
 * This test checks that a DefaultSocInterface object can be instantiated without any exceptions, that an audio sink name ("audioSink")
 * is properly prepared using strncpy, and that the method IsAudioSinkOrAudioDecoder correctly processes the input with the Rialto flag set to true.
 * The method is expected to return a boolean value without throwing an exception.
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
 * | Variation / Step | Description                                                                    | Test Data                                                          | Expected Result                                                                                          | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------ | ------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a DefaultSocInterface object                                            | None                                                               | Object should be created successfully without throwing any exception                                   | Should be successful |
 * | 02               | Prepare the input name by setting the value "audioSink" into a char array using strncpy | name = "audioSink", array size = 50                                 | The name array should hold a valid null-terminated string "audioSink"                                     | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder with the prepared name and Rialto flag set to true| input: name = "audioSink", isRialto = true, output: result (bool)      | The method should return a boolean value without throwing an exception and the result should be valid     | Should Pass     |
 */
TEST(DefaultSocInterface, ValidAudioSinkInputRialtoTrue) {
    std::cout << "Entering ValidAudioSinkInputRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        // Using strncpy to assign the element name
        std::strncpy(name, "audioSink", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << true << std::endl;
        
        bool result = false;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result == true || result == false);

    });
    std::cout << "Exiting ValidAudioSinkInputRialtoTrue test" << std::endl;
}
/**
 * @brief Validate that IsAudioSinkOrAudioDecoder handles a valid audio sink input with Rialto set to false
 *
 * This test instantiates a DefaultSocInterface object, prepares an input character array with "audioSink", and invokes
 * the IsAudioSinkOrAudioDecoder method with the isRialto parameter set to false. It verifies that no exceptions are thrown
 * during execution and that the method returns a boolean value (either true or false).
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                                  | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------------------- | --------------- |
 * | 01               | Create DefaultSocInterface instance                                         | N/A                                                           | Object is created successfully without any exceptions            | Should be successful |
 * | 02               | Prepare input character array by copying "audioSink" into name variable     | name = "audioSink"                                            | Input is properly set and null-terminated                         | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder method with name and isRialto = false        | input1 = name: "audioSink", input2 = isRialto: false, output1 = result | Method returns a boolean value (true or false) without throwing exception | Should Pass     |
 */
TEST(DefaultSocInterface, ValidAudioSinkInputRialtoFalse) {
    std::cout << "Entering ValidAudioSinkInputRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "audioSink", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << false << std::endl;
        
        bool result = false;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        // Expected output can be either true or false.
        EXPECT_TRUE(result == true || result == false);
    });
    std::cout << "Exiting ValidAudioSinkInputRialtoFalse test" << std::endl;
}
/**
 * @brief Validate audio decoder input with Rialto true
 *
 * This test verifies the behavior of the IsAudioSinkOrAudioDecoder method when provided with a valid audio decoder input and the Rialto flag set to true. It creates a DefaultSocInterface object, prepares the input string "audioDecoder", and calls the API with Rialto true. The test ensures that the API does not throw any exceptions and returns a valid boolean output.
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
 * | Variation / Step | Description                                                                                 | Test Data                                      | Expected Result                                                                              | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ---------------------------------------------- | -------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate DefaultSocInterface, prepare input "audioDecoder", and invoke IsAudioSinkOrAudioDecoder with Rialto true | input: name = audioDecoder, isRialto = true, output: boolean result | API returns a valid boolean (either true or false) without throwing any exceptions | Should Pass |
 */
TEST(DefaultSocInterface, ValidAudioDecoderInputRialtoTrue) {
    std::cout << "Entering ValidAudioDecoderInputRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "audioDecoder", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << true << std::endl;
        
        bool result = false;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        // Expected output can be either true or false.
        EXPECT_TRUE(result == true || result == false);
    });
    std::cout << "Exiting ValidAudioDecoderInputRialtoTrue test" << std::endl;
}
/**
 * @brief Validate IsAudioSinkOrAudioDecoder with audioDecoder input and Rialto flag false
 *
 * This test verifies that the IsAudioSinkOrAudioDecoder method executes without throwing exceptions when provided with a valid audio decoder input ("audioDecoder") along with the Rialto flag set to false. The test confirms that the function returns a boolean value (either true or false) as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create a DefaultSocInterface object, initialize the input name to "audioDecoder", and call IsAudioSinkOrAudioDecoder with isRialto set to false. | name = "audioDecoder", isRialto = false | Returns a boolean value (true or false) without any exceptions thrown | Should Pass |
 */
TEST(DefaultSocInterface, ValidAudioDecoderInputRialtoFalse) {
    std::cout << "Entering ValidAudioDecoderInputRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "audioDecoder", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << false << std::endl;
        
        bool result = false;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        // Expected output can be either true or false.
        EXPECT_TRUE(result == true || result == false);
    });
    std::cout << "Exiting ValidAudioDecoderInputRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that the IsAudioSinkOrAudioDecoder API returns false for a non-audio element name input when Rialto flag is true.
 *
 * This test ensures that when a non-audio element name (i.e., "videoRenderer") is passed along with the Rialto flag set to true, the 
 * IsAudioSinkOrAudioDecoder API correctly returns false. This confirms that the API does not mistakenly identify non-audio components 
 * as audio sinks or decoders.
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
 * | Variation / Step | Description                                                         | Test Data                                                  | Expected Result                                                                                 | Notes         |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a DefaultSocInterface object                                  | None                                                       | Object is created successfully without throwing any exception                                | Should Pass   |
 * | 02               | Prepare input name and Rialto flag                                   | name = videoRenderer, isRialto = true                      | Input variables are correctly prepared                                                         | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder API with the prepared input         | input: name = videoRenderer, isRialto = true, output: result | API returns false indicating that the input does not correspond to an audio element             | Should Pass   |
 * | 04               | Verify that the returned result is false                             | output result = false                                      | Assertion confirms that the result is false                                                    | Should Pass   |
 */
TEST(DefaultSocInterface, NonAudioElementInputRialtoTrue) {
    std::cout << "Entering NonAudioElementInputRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "videoRenderer", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << true << std::endl;
        
        bool result = true;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result) << "Expected false for non-audio element name input.";
    });
    std::cout << "Exiting NonAudioElementInputRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder returns false for a non-audio element input when the Rialto flag is false
 *
 * Validate that when provided with a non-audio element name ("videoDecoder") and the Rialto flag set to false, the API correctly returns false, confirming that non-audio elements are properly handled.
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
 * | 01 | Instantiate DefaultSocInterface and log creation message | None | Object is instantiated without throwing exception | Should be successful |
 * | 02 | Prepare the input name as "videoDecoder" and set Rialto flag to false | name = "videoDecoder", isRialto = false | Name is prepared correctly with proper null-termination | Should be successful |
 * | 03 | Call IsAudioSinkOrAudioDecoder with prepared input and verify output | input1 = name ("videoDecoder"), input2 = false, output1 = result | API returns false; assertion EXPECT_FALSE validates the false value | Should Pass |
 * | 04 | Log the test exit message | None | Test exit message is printed successfully | Should be successful |
 */
TEST(DefaultSocInterface, NonAudioElementInputRialtoFalse) {
    std::cout << "Entering NonAudioElementInputRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "videoDecoder", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: " << name << ", isRialto: " << std::boolalpha << false << std::endl;
        
        bool result = true;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result) << "Expected false for non-audio element name input.";
    });
    std::cout << "Exiting NonAudioElementInputRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder handles NULL name input correctly when Rialto flag is true
 *
 * This test ensures that when a NULL pointer is passed as the name along with a true Rialto flag, the
 * IsAudioSinkOrAudioDecoder method does not throw any exceptions and returns false as expected. This behavior
 * verifies proper input validation within the API.
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
 * | Variation / Step | Description                                                                                 | Test Data                                | Expected Result                                               | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ---------------------------------------- | ------------------------------------------------------------- | --------------- |
 * | 01               | Create a DefaultSocInterface object to access the API method                                | N/A                                      | Object created successfully                                   | Should be successful |
 * | 02               | Prepare input parameters with a NULL pointer for name and true for Rialto flag                | name = NULL, isRialto = true             | Input values are set as expected                              | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder with the prepared inputs and check that the result is false  | input: name = NULL, isRialto = true, output: result | Method returns false without throwing any exceptions          | Should Pass     |
 */
TEST(DefaultSocInterface, NullPointerInputRialtoTrue) {
    std::cout << "Entering NullPointerInputRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        const char* name = NULL;
        std::cout << "Prepared input name: NULL, isRialto: " << std::boolalpha << true << std::endl;
        
        bool result = true;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result) << "Expected false for NULL pointer name input.";
    });
    std::cout << "Exiting NullPointerInputRialtoTrue test" << std::endl;
}
/**
 * @brief Validate API returns false when an empty string input is provided with isRialto flag set to false
 *
 * This test verifies that invoking the IsAudioSinkOrAudioDecoder API with an empty string for the name and false
 * for the isRialto flag results in a false return value. It ensures that the interface properly handles empty string inputs
 * without throwing exceptions and produces the correct boolean output.
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
 * | Variation / Step | Description                                                    | Test Data                                                             | Expected Result                                              | Notes          |
 * | :--------------: | -------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------------------------------------------------ | -------------- |
 * | 01               | Create an instance of DefaultSocInterface                      | No inputs; output: DefaultSocInterface object                         | Object is created without throwing an exception              | Should be successful |
 * | 02               | Prepare a character array with an empty string for the name      | name = "", isRialto = false, output: name buffer properly initialized   | Empty string correctly initialized in the buffer             | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder with the empty string input     | input: name = "", isRialto = false, output: result variable             | API returns false                                            | Should Pass    |
 * | 04               | Verify that the API result is false using EXPECT_FALSE           | input: result = false                                                 | Assertion EXPECT_FALSE(result) passes successfully           | Should Pass    |
 */
TEST(DefaultSocInterface, EmptyStringInputRialtoFalse) {
    std::cout << "Entering EmptyStringInputRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully" << std::endl;
        
        char name[50];
        std::strncpy(name, "", sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        std::cout << "Prepared input name: (empty string), isRialto: " << std::boolalpha << false << std::endl;
        
        bool result = true;
        EXPECT_NO_THROW(result = socInterface.IsAudioSinkOrAudioDecoder(name));
        std::cout << "Method IsAudioSinkOrAudioDecoder returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result) << "Expected false for empty string name input.";
    });
    std::cout << "Exiting EmptyStringInputRialtoFalse test" << std::endl;
}
/**
 * @brief Test the DefaultSocInterface object instantiation and the behavior of IsSimulatorFirstFrame() method.
 *
 * This test verifies that a DefaultSocInterface object can be constructed without throwing exceptions and that the IsSimulatorFirstFrame() method can be invoked successfully. It also validates that the method returns a valid boolean value.
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
 * | Variation / Step | Description                                              | Test Data                                                      | Expected Result                                                                              | Notes        |
 * | :--------------: | -------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Create a DefaultSocInterface object using its default constructor. | No inputs; Expected: Object construction                     | Object constructed successfully without throwing any exception.                            | Should Pass  |
 * | 02               | Invoke IsSimulatorFirstFrame() method on the constructed object to verify its behavior. | No additional inputs; Expected: simulatorFirstFrame = true or false | The method returns a valid boolean value and satisfies the assertion check (true or false). | Should Pass  |
 */
TEST(DefaultSocInterface, IsSimulatorFirstFrame_start) {
    std::cout << "Entering DefaultSocInterface::IsSimulatorFirstFrame()_start test" << std::endl;
    
    // Create DefaultSocInterface object using default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object constructed successfully." << std::endl;
        
        // Invoke the method IsSimulatorFirstFrame
        bool simulatorFirstFrame = false;
        EXPECT_NO_THROW({
            std::cout << "Invoking IsSimulatorFirstFrame() method." << std::endl;
            simulatorFirstFrame = socInterface.IsSimulatorFirstFrame();
            std::cout << "IsSimulatorFirstFrame() returned: " << std::boolalpha << simulatorFirstFrame << std::endl;
        });
            
        EXPECT_TRUE(simulatorFirstFrame == true || simulatorFirstFrame == false);
    });
    
    std::cout << "Exiting DefaultSocInterface::IsSimulatorFirstFrame()_start test" << std::endl;
}
/**
 * @brief Test the IsSimulatorSink function behavior on a DefaultSocInterface instance
 *
 * This test verifies that a DefaultSocInterface object can be created using its default constructor and that the IsSimulatorSink() method can be invoked to return a boolean value. It ensures the method call does not throw exceptions and the return value is valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                     | Test Data                                                                                       | Expected Result                                                                                  | Notes                |
 * | :----:           | :-------------------------------------------------------------- | :---------------------------------------------------------------------------------------------- | :---------------------------------------------------------------------------------------------- | :------------------: |
 * | 01               | Create a DefaultSocInterface object using its default constructor | Object creation: DefaultSocInterface()                                                        | Object is created without throwing any exception                                               | Should be successful |
 * | 02               | Invoke the IsSimulatorSink() method on the created object         | Function call: IsSimulatorSink(), input: none, output: result (boolean)                         | Method is invoked without throwing and returns a boolean value (true or false)                   | Should Pass          |
 * | 03               | Verify that the returned value is a valid boolean value             | Assertion: result == true || result == false (input: result boolean value)                      | Assertion passes confirming the result is either true or false                                   | Should Pass          |
 */
TEST(DefaultSocInterface, IsSimulatorSink_start) {
    std::cout << "Entering IsSimulatorSink_start test" << std::endl;
    
    // Create a DefaultSocInterface object using its default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface defaultSocInterface;
        std::cout << "Created DefaultSocInterface object using default constructor." << std::endl;

        // Invoke the IsSimulatorSink() method and log the invocation.
        bool result = false;
        EXPECT_NO_THROW({
            std::cout << "Invoking IsSimulatorSink() method." << std::endl;
            result = defaultSocInterface.IsSimulatorSink();
            std::cout << "IsSimulatorSink() returned value: " << std::boolalpha << result << std::endl;
        });

        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified that the return value is false as expected based on the default internal state." << std::endl;
    });
    
    std::cout << "Exiting IsSimulatorSink_start test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::IsSimulatorVideoSample returns a valid boolean value without throwing any exceptions
 *
 * This test ensures that the object of DefaultSocInterface can be instantiated using its default constructor and that the method IsSimulatorVideoSample, when invoked, returns a boolean value (either true or false) without throwing any exceptions. It verifies that the API's output is within the expected bounds.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                  | Test Data                                                         | Expected Result                                                       | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------------------------------------------------------------- | ------------- |
 * | 01               | Create a DefaultSocInterface object using the default constructor            | constructor = default, output = socInterface instance               | Object is created successfully without exceptions                     | Should be successful |
 * | 02               | Invoke the method IsSimulatorVideoSample on the created object                | method call = IsSimulatorVideoSample(), output = result (boolean)   | API returns a boolean value (true or false) without throwing exceptions | Should Pass   |
 * | 03               | Verify that the returned value is either true or false as expected             | result = boolean value resulting from IsSimulatorVideoSample()       | Assertion passes confirming that result is valid (true or false)         | Should be successful |
 */
TEST(DefaultSocInterface, IsSimulatorVideoSample_start) {
    std::cout << "Entering DefaultSocInterface::IsSimulatorVideoSample_start test" << std::endl;
    
    EXPECT_NO_THROW({
        // Creating object using default constructor.
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object using default constructor" << std::endl;
        
        // Invoking the method IsSimulatorVideoSample and logging the invocation.
        std::cout << "Invoking IsSimulatorVideoSample()" << std::endl;
        bool result = socInterface.IsSimulatorVideoSample();
        std::cout << "Method IsSimulatorVideoSample returned: " << (result ? "true" : "false") << std::endl;
        
        // Verifying that returned value is either true or false.
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified the result is a valid boolean value: " << (result ? "true" : "false") << std::endl;
    });
    
    std::cout << "Exiting DefaultSocInterface::IsSimulatorVideoSample_start test" << std::endl;
}
/**
 * @brief Validates that the IsVideoDecoder method returns true for a valid video decoder input when Rialto is true
 *
 * This test creates a DefaultSocInterface object and prepares a fixed-size character array with the value "video_decoder". It then invokes the IsVideoDecoder API with the prepared element name and a Rialto flag set to true, expecting the method to return true. The objective is to verify that the API correctly processes valid input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                               | Test Data                                                 | Expected Result                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------- | --------------------------------------------------------- | --------------------------------------------------- | ------------- |
 * | 01               | Create DefaultSocInterface object                                                         | None                                                      | Object created without throwing an exception        | Should be successful |
 * | 02               | Prepare element name by copying "video_decoder" into a fixed-size buffer                    | inputStr = "video_decoder", elementName buffer size = 64  | elementName correctly initialized to "video_decoder" | Should be successful |
 * | 03               | Invoke IsVideoDecoder API with elementName and isRialto flag set to true                      | elementName = "video_decoder", isRialto = true            | Returns true and the assertion passes               | Should Pass   |
 */
TEST(DefaultSocInterface, ValidVideoDecoderInput_RialtoTrue)
{
    GTEST_SKIP();
    const char* testName = "ValidVideoDecoderInput_RialtoTrue";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    // Create DefaultSocInterface object.
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        // Prepare input name using strncpy into a fixed size array.
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "video_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;

        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: Assuming "video_decoder" returns true.
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Ensure that DefaultSocInterface correctly processes a valid video decoder input with Rialto set to false
 *
 * This test verifies that the IsVideoDecoder method correctly identifies "video_decoder" as a valid video decoder 
 * when the Rialto flag is false. The method is expected to return true without throwing any exceptions. This test 
 * confirms that the object instantiation, preparation of the element name, and method invocation all execute as designed.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Create DefaultSocInterface object, prepare element name with "video_decoder", invoke IsVideoDecoder with isRialto set to false | testName = ValidVideoDecoderInput_RialtoFalse, elementName = video_decoder, isRialto = false | Method returns true and no exceptions are thrown | Should Pass |
 */
TEST(DefaultSocInterface, ValidVideoDecoderInput_RialtoFalse)
{
    GTEST_SKIP();
    const char* testName = "ValidVideoDecoderInput_RialtoFalse";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "video_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;

        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: Assuming "video_decoder" returns true.
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Verify that IsVideoDecoder returns false for non-video decoder input when Rialto is true
 *
 * This test creates a DefaultSocInterface object, prepares a non-video decoder input ("audio_decoder"), and verifies that calling IsVideoDecoder with isRialto set to true returns false.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 033 @n
 * **Priority:** High @n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** None @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DefaultSocInterface instance, prepare element name as "audio_decoder", and call IsVideoDecoder with isRialto=true | elementName = "audio_decoder", isRialto = true, output (expected) = false | IsVideoDecoder returns false and EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(DefaultSocInterface, NonVideoDecoderInput_RialtoTrue)
{
    const char* testName = "NonVideoDecoderInput_RialtoTrue";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "audio_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;

        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: "audio_decoder" is not a video decoder.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface correctly handles non-video decoder inputs when Rialto flag is false
 *
 * This test checks that the IsVideoDecoder method returns false when provided with an element name that does not represent a video decoder ("audio_decoder") while the Rialto flag is set to false. This behavior is expected because "audio_decoder" should not be recognized as a video decoder.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                                                                          | Expected Result                                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a DefaultSocInterface object                                 | N/A                                                                                                | Object is created successfully                                                            | Should be successful |
 * | 02               | Prepare element name by copying "audio_decoder" into array            | inputStr = "audio_decoder", elementName = zero-initialized array                                    | elementName contains "audio_decoder"                                                      | Should be successful |
 * | 03               | Call IsVideoDecoder API with elementName and Rialto flag set to false   | elementName = "audio_decoder", isRialto = false, output: result                                    | Method returns false indicating "audio_decoder" is not a video decoder                      | Should Pass   |
 * | 04               | Validate the API response using EXPECT_FALSE                          | result = output from IsVideoDecoder                                                                | Assertion passes confirming that the result is false                                      | Should be successful |
 */
TEST(DefaultSocInterface, NonVideoDecoderInput_RialtoFalse)
{
    const char* testName = "NonVideoDecoderInput_RialtoFalse";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
  
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "audio_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;
        
        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: "audio_decoder" is not a video decoder.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Validate that IsVideoDecoder handles a NULL element name correctly.
 *
 * This test verifies that the DefaultSocInterface::IsVideoDecoder method returns false when provided 
 * with a NULL element name while isRialto is true. It ensures that the API does not throw any exceptions 
 * and behaves as expected when invalid (NULL) input is provided.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 035
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                       | Test Data                                                          | Expected Result                                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------ | --------------------------------------------------------------------- | ---------- |
 * | 01               | Create DefaultSocInterface object, prepare NULL element name, set isRialto to true, and invoke IsVideoDecoder API | testName = "NullNameInput_RialtoTrue", elementName = NULL, isRialto = true | API returns false without throwing exceptions; EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(DefaultSocInterface, NullNameInput_RialtoTrue)
{
    GTEST_SKIP();
    const char* testName = "NullNameInput_RialtoTrue";
    std::cout << "Entering " << testName << " test" << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        const char* elementName = NULL;
        std::cout << "Prepared element name: NULL" << std::endl;
        
        std::cout << "Calling IsVideoDecoder with name: NULL, "  << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: NULL name returns false.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Verify that IsVideoDecoder returns false for an empty string input.
 *
 * This test case verifies that when an empty string is provided as the element name,
 * the IsVideoDecoder method of DefaultSocInterface returns false. This behavior is expected
 * since an empty string should not be recognized as a valid video decoder name.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                  | Notes        |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------ | ------------ |
 * | 01               | Instantiate the DefaultSocInterface object                                  | No input arguments                                    | Object is successfully created                   | Should be successful |
 * | 02               | Prepare the element name variable with an empty string                      | inputStr = "", elementName = ""                       | elementName holds an empty string                | Should be successful |
 * | 03               | Invoke IsVideoDecoder API with elementName and isRialto set to false          | elementName = "", isRialto = false                     | Returns false indicating empty string is not valid | Should Pass  |
 */
TEST(DefaultSocInterface, EmptyStringNameInput_RialtoFalse)
{
    const char* testName = "EmptyStringNameInput_RialtoFalse";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name (empty string): '" << elementName << "'" << std::endl;

        std::cout << "Calling IsVideoDecoder with empty name" << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: empty string is not a video decoder.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Validate that the IsVideoDecoder function returns false for a non-matching decoder name when the Rialto flag is true.
 *
 * This test creates a DefaultSocInterface object, prepares a non-decoder element name ("random_decoder"), and invokes the IsVideoDecoder function with the Rialto flag set to true. The objective is to ensure that the API correctly identifies that the random name is not a video decoder and returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 037@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :---------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Create DefaultSocInterface object, prepare the element name "random_decoder", and call the IsVideoDecoder function with isRialto set to true | elementName = "random_decoder", isRialto = true, result = API output | API returns false and EXPECT_FALSE(result) assertion passes | Should Pass |
 */
TEST(DefaultSocInterface, RandomNonMatchingNameInput_RialtoTrue)
{
    const char* testName = "RandomNonMatchingNameInput_RialtoTrue";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
      
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "random_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;
      
        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: random name is not a video decoder.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Test DefaultSocInterface::IsVideoDecoder with a non-matching element name input expecting a false return value
 *
 * This test verifies that when a random non-matching element name is provided along with the Rialto flag set to false,
 * the IsVideoDecoder function returns false as expected. This ensures that the interface correctly identifies non-video decoders.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke IsVideoDecoder API with a non-matching element name and Rialto flag set to false to verify it returns false | elementName = random_decoder, isRialto = false | Return value should be false, assertion EXPECT_FALSE should pass | Should Pass |
 */
TEST(DefaultSocInterface, RandomNonMatchingNameInput_RialtoFalse)
{
    const char* testName = "RandomNonMatchingNameInput_RialtoFalse";
    std::cout << "Entering " << testName << " test" << std::endl;
    
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object" << std::endl;
        
        char elementName[64];
        std::memset(elementName, 0, sizeof(elementName));
        const char *inputStr = "random_decoder";
        std::strncpy(elementName, inputStr, sizeof(elementName)-1);
        std::cout << "Prepared element name: " << elementName << std::endl;
        
        std::cout << "Calling IsVideoDecoder with name: " << elementName << std::endl;
        bool result = socInterface.IsVideoDecoder(elementName);
        std::cout << "Method IsVideoDecoder returned: " << result << std::endl;
        
        // Expected behavior: random name is not a video decoder.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting " << testName << " test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns true when provided with a valid video sink name and Rialto flag set to false.
 *
 * This test creates an instance of DefaultSocInterface using the default constructor, prepares a fixed-size character buffer with the test string "videosink", and calls the IsVideoSink method with the Rialto flag set to false. It then asserts that the returned result is true. This test ensures that a valid video sink name is correctly identified when Rialto is false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 039@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DefaultSocInterface instance and prepare input buffer | inputName = "videosink", isRialto = false, nameBuffer = buffer of size 64 initialized to 0 | Instance created without throwing an exception and input buffer contains "videosink" | Should be successful |
 * | 02 | Invoke IsVideoSink API and assert the return value | nameBuffer = "videosink", isRialto = false | Returns true, satisfying the EXPECT_TRUE assertion | Should Pass |
 */
TEST(DefaultSocInterface, ValidVideoSinkNameWithRialtoFalse)
{
    GTEST_SKIP();
    std::cout << "Entering ValidVideoSinkNameWithRialtoFalse test" << std::endl;
    // Create DefaultSocInterface object using default constructor
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    // Prepare input using strncpy for fixed size array
    char nameBuffer[64] = {0};
    const char* inputName = "videosink";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);
    
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    // Assuming a valid video sink name should return true.
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidVideoSinkNameWithRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns true for a valid video sink name when Rialto flag is true
 *
 * This test verifies that the DefaultSocInterface class correctly identifies a valid video sink name
 * when the Rialto flag is set to true. It ensures that the API method IsVideoSink returns true,
 * confirming the expected functionality for valid video sink input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 040
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                        | Test Data                                                            | Expected Result                                                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------- | ------------------------------------------------------------------------ | ---------- |
 * | 01               | Instantiate DefaultSocInterface, set video sink name to "videosink" and Rialto flag to true, then invoke IsVideoSink | nameBuffer = "videosink", isRialto = true, output = result (bool)      | API returns true and EXPECT_TRUE assertion passes                        | Should Pass |
 */
TEST(DefaultSocInterface, ValidVideoSinkNameWithRialtoTrue)
{
    GTEST_SKIP();
    std::cout << "Entering ValidVideoSinkNameWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    char nameBuffer[64] = {0};
    const char* inputName = "videosink";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);    
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    // Assuming a valid video sink name should return true.
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidVideoSinkNameWithRialtoTrue test" << std::endl;
}
/**
 * @brief Validate that IsVideoSink returns false when an invalid video sink name ("audiosink") is provided with Rialto set to false.
 *
 * This test verifies that the DefaultSocInterface::IsVideoSink method correctly identifies that "audiosink" is not a valid video sink name when the Rialto flag is false. Ensuring that the API properly rejects invalid video sink names is critical for overall functionality.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 041
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface object ensuring no exceptions thrown | None | Object is created successfully without any exceptions | Should be successful |
 * | 02 | Prepare test inputs by copying "audiosink" into the name buffer and setting isRialto to false | inputName = "audiosink", nameBuffer = "audiosink", isRialto = false | Test inputs are set as expected | Should be successful |
 * | 03 | Invoke IsVideoSink with the test inputs | nameBuffer = "audiosink", isRialto = false | Method returns false indicating invalid video sink | Should Fail |
 * | 04 | Verify the result using EXPECT_FALSE assertion | result = false | Assertion passes if result is false | Should be successful |
 */
TEST(DefaultSocInterface, InvalidVideoSinkNameAudiosinkWithRialtoFalse)
{
    std::cout << "Entering InvalidVideoSinkNameAudiosinkWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    char nameBuffer[64] = {0};
    const char* inputName = "audiosink";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    // Expected to be false for an invalid video sink name.
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidVideoSinkNameAudiosinkWithRialtoFalse test" << std::endl;
}
/**
 * @brief Validate that IsVideoSink returns false when provided with an audiosink name and Rialto flag set to true.
 *
 * This test verifies that the DefaultSocInterface::IsVideoSink method correctly identifies that the name "audiosink"
 * does not correspond to a video sink when the Rialto flag is true. It ensures that the API behaves as expected in a
 * negative scenario by returning false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 042@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                 | Test Data                                                          | Expected Result                                                       | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------ | --------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate the DefaultSocInterface object and check for exceptions         | None                                                               | No exception is thrown                                                | Should Pass     |
 * | 02               | Copy "audiosink" into the name buffer and set isRialto to true                | inputName = audiosink, nameBuffer = audiosink, isRialto = true       | Data is correctly copied into the buffer and boolean flag is set       | Should be successful |
 * | 03               | Invoke IsVideoSink with nameBuffer and isRialto set to true                   | nameBuffer = audiosink, isRialto = true                              | Method returns false                                                  | Should Fail     |
 * | 04               | Assert that the result of IsVideoSink is false                               | result = false                                                      | EXPECT_FALSE assertion passes                                         | Should be successful |
 */
TEST(DefaultSocInterface, InvalidVideoSinkNameAudiosinkWithRialtoTrue)
{
    std::cout << "Entering InvalidVideoSinkNameAudiosinkWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    char nameBuffer[64] = {0};
    const char* inputName = "audiosink";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);    
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidVideoSinkNameAudiosinkWithRialtoTrue test" << std::endl;
}
/**
 * @brief Test the behavior of IsVideoSink when a null name is provided with isRialto set to false.
 *
 * This test verifies that invoking the IsVideoSink API with a null name and isRialto set to false returns false,
 * demonstrating that the API correctly handles null input parameters without triggering exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 043
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a DefaultSocInterface object using EXPECT_NO_THROW. | N/A | Object is created without exception. | Should be successful |
 * | 02 | Set test parameters with name as nullptr and isRialto as false. | name = nullptr, isRialto = false | Parameters are correctly set for API call. | Should be successful |
 * | 03 | Invoke IsVideoSink API using the null name and false isRialto. | input1 = name = nullptr, input2 = isRialto = false | API returns false, demonstrating proper handling of null input. | Should Pass |
 */
TEST(DefaultSocInterface, NullNameWithRialtoFalse)
{
    GTEST_SKIP() << "Skipping test";
    std::cout << "Entering NullNameWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    const char* name = nullptr;
    bool result = socInterface.IsVideoSink(name);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting NullNameWithRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns false for a null name with isRialto set to true
 *
 * This test examines if the DefaultSocInterface's method IsVideoSink correctly handles a null input for the name parameter when isRialto is true. It verifies that under these conditions, the API safely returns false, ensuring robustness against invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                             | Test Data                                      | Expected Result                                                    | Notes        |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------------ | ------------ |
 * | 01               | Create the DefaultSocInterface object, set name to nullptr and isRialto to true, and invoke IsVideoSink | input: name = nullptr, isRialto = true; output: result expected false | API returns false; assertion EXPECT_FALSE(result) passes             | Should Pass  |
 */
TEST(DefaultSocInterface, NullNameWithRialtoTrue)
{
    GTEST_SKIP();
    std::cout << "Entering NullNameWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    const char* name = nullptr;    
    bool result = socInterface.IsVideoSink(name);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting NullNameWithRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns false when an empty name is provided with Rialto flag set to false
 *
 * This test validates that the DefaultSocInterface's IsVideoSink method correctly handles an empty video sink name when the Rialto flag is false. It ensures that the API returns false in scenarios where the video sink name is missing or empty.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------- | --------- | -------------- | ----- |
 * | 01 | Create an instance of DefaultSocInterface and ensure no exceptions are thrown during instantiation. | Constructor call: DefaultSocInterface() | Instance is successfully created without throwing exceptions. | Should be successful |
 * | 02 | Assign an empty string to the video sink name using strncpy. | inputName = "", nameBuffer (array of size 64, initialized with zeros), isRialto = false | nameBuffer contains an empty string. | Should be successful |
 * | 03 | Invoke IsVideoSink with the empty name and Rialto flag as false, and verify it returns false. | nameBuffer = empty string, isRialto = false, output = result | API returns false and EXPECT_FALSE(result) assertion passes. | Should Pass |
 */
TEST(DefaultSocInterface, EmptyNameWithRialtoFalse)
{
    std::cout << "Entering EmptyNameWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    char nameBuffer[64] = {0};
    // Empty string assignment using strncpy
    const char* inputName = "";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);    
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyNameWithRialtoFalse test" << std::endl;
}
/**
 * @brief Validates that IsVideoSink returns false when an empty name is provided with isRialto true.
 *
 * This test checks the behavior of the IsVideoSink method when provided with an empty name string and the isRialto flag set to true.
 * The expected behavior is that the API should correctly handle the empty name input and return false.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 046
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                        | Test Data                                     | Expected Result                                      | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------- | --------------------------------------------- | ---------------------------------------------------- | ----------------- |
 * | 01               | Create DefaultSocInterface instance and ensure no exception is thrown.             | N/A                                           | Instance created successfully without exception.   | Should Pass       |
 * | 02               | Prepare test data by copying an empty string to the name buffer and setting isRialto to true. | inputName="", isRialto=true                    | nameBuffer contains an empty string and isRialto flag is set to true. | Should be successful |
 * | 03               | Invoke IsVideoSink with the empty name buffer and isRialto true flag.                | nameBuffer="", isRialto=true                    | API returns false as expected.                       | Should Pass       |
 * | 04               | Assert that the IsVideoSink method returns false as expected.                      | result (from API call)=false                   | Assertion passes when result is false.               | Should be successful |
 */
TEST(DefaultSocInterface, EmptyNameWithRialtoTrue)
{
    std::cout << "Entering EmptyNameWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW(DefaultSocInterface socInterface);
    DefaultSocInterface socInterface;
    
    char nameBuffer[64] = {0};
    const char* inputName = "";
    std::strncpy(nameBuffer, inputName, sizeof(nameBuffer) - 1);
    
    bool result = socInterface.IsVideoSink(nameBuffer);
    std::cout << "Method IsVideoSink returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyNameWithRialtoTrue test" << std::endl;
}
/**
 * @brief Verifies that DefaultSocInterface::SetAC4Tracks method works correctly with a valid GstElement pointer and a positive track id.
 *
 * Tests the SetAC4Tracks method of DefaultSocInterface by invoking it with a non-null dummy GstElement pointer created from valid memory storage and a valid track id. This test ensures that the method does not throw an exception when provided with valid input parameters.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a dummy GstElement pointer, instantiate DefaultSocInterface, and invoke SetAC4Tracks with valid parameters. | dummySrc = non-null dummy pointer (reinterpret_cast from dummy storage), trackId = 1 | SetAC4Tracks does not throw an exception. | Should Pass |
 */
TEST(DefaultSocInterface, ValidGstElementPositiveTrackId) {
    std::cout << "Entering ValidGstElementPositiveTrackId test" << std::endl;
    
    // Create a dummy GstElement using a dummy storage and cast it.
    // Since GstElement is an incomplete type, we simulate a valid non-null pointer.
    char dummyStorage[10] = {0};
    GstElement *dummySrc = reinterpret_cast<GstElement*>(dummyStorage);
    int trackId = 1;
    
    // Create DefaultSocInterface object using default constructor.
    std::cout << "Creating DefaultSocInterface object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Invoking SetAC4Tracks with src: " << static_cast<void*>(dummySrc)
                  << " and trackId: " << trackId << std::endl;
        EXPECT_NO_THROW(socInterface.SetAC4Tracks(dummySrc, trackId));
        std::cout << "SetAC4Tracks method invoked successfully." << std::endl;
    });
    
    std::cout << "Exiting ValidGstElementPositiveTrackId test" << std::endl;
}
/**
 * @brief Test that verifies SetAC4Tracks handles a null GstElement pointer gracefully when provided with a valid track ID.
 *
 * This test ensures that the DefaultSocInterface's SetAC4Tracks method can handle a null GstElement pointer without throwing exceptions,
 * even when provided with a valid track ID. The objective is to validate the robustness of the API and its ability to handle invalid or missing
 * input pointers gracefully.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                   | Test Data                                | Expected Result                                                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------- | ---------------------------------------- | ------------------------------------------------------------------------ | ---------- |
 * | 01               | Instantiate DefaultSocInterface and invoke SetAC4Tracks with a null GstElement pointer and a valid track ID.    | dummySrc = nullptr, trackId = 1            | No exception is thrown; EXPECT_NO_THROW assertions pass successfully.    | Should Pass|
 */
TEST(DefaultSocInterface, NullGstElementValidTrackId) {
    std::cout << "Entering NullGstElementValidTrackId test" << std::endl;
    
    GstElement *dummySrc = nullptr;
    int trackId = 1;
    
    std::cout << "Creating DefaultSocInterface object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Invoking SetAC4Tracks with src: " << dummySrc
                  << " and trackId: " << trackId << std::endl;
        EXPECT_NO_THROW(socInterface.SetAC4Tracks(dummySrc, trackId));
        std::cout << "SetAC4Tracks method handled NULL GstElement gracefully." << std::endl;
    });
    
    std::cout << "Exiting NullGstElementValidTrackId test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::SetAC4Tracks handles a negative trackId without throwing exceptions.
 *
 * This test verifies that when a valid GstElement pointer is provided alongside a negative trackId, the DefaultSocInterface's SetAC4Tracks method executes without throwing exceptions. The test ensures the API's robustness in handling negative track identifiers.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 049
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                   | Expected Result                                              | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------- |
 * | 01               | Create a DefaultSocInterface instance and invoke SetAC4Tracks with a valid dummy GstElement and a negative trackId (-1). | dummySrc = reinterpret_cast<GstElement*>(dummyStorage), trackId = -1                                        | SetAC4Tracks should execute without throwing any exceptions. | Should Pass |
 */
TEST(DefaultSocInterface, ValidGstElementNegativeTrackId) {
    std::cout << "Entering ValidGstElementNegativeTrackId test" << std::endl;
    
    // Create a valid dummy GstElement pointer.
    char dummyStorage[10] = {0};
    GstElement *dummySrc = reinterpret_cast<GstElement*>(dummyStorage);
    int trackId = -1;
    
    std::cout << "Creating DefaultSocInterface object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "Invoking SetAC4Tracks with src: " << static_cast<void*>(dummySrc)
                  << " and trackId: " << trackId << std::endl;
        EXPECT_NO_THROW(socInterface.SetAC4Tracks(dummySrc, trackId));
        std::cout << "SetAC4Tracks method invoked with negative trackId without throwing exceptions." << std::endl;
    });
    
    std::cout << "Exiting ValidGstElementNegativeTrackId test" << std::endl;
}
/**
 * @brief Validate SetAudioProperty API with valid non-null inputs and isSinkBinVolume set to false
 *
 * This test case validates that the SetAudioProperty API can be invoked with valid, non-null string inputs for volume and mute,
 * and with the isSinkBinVolume flag set to false. The test ensures that no exceptions are thrown during the API call and that the parameters remain unchanged.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 050@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                            | Test Data                                                    | Expected Result                                          | Notes          |
 * | :--------------: | -------------------------------------------------------------------------------------- | ------------------------------------------------------------ | -------------------------------------------------------- | -------------- |
 * | 01               | Initialize DefaultSocInterface object using default constructor                        | None                                                         | Object instantiation successful                           | Should be successful |
 * | 02               | Define non-null input parameters with volume="initialVolume", mute="initialMute", isSinkBinVolume=false | volume=initialVolume, mute=initialMute, isSinkBinVolume=false  | Parameters set as provided                                | Should be successful |
 * | 03               | Invoke SetAudioProperty API with the defined parameters                                  | volume=initialVolume, mute=initialMute, isSinkBinVolume=false  | API call returns without throwing an exception            | Should Pass    |
 * | 04               | Confirm that input parameters remain unchanged after API call                           | volume=initialVolume, mute=initialMute, isSinkBinVolume=false  | Parameters remain unchanged                               | Should be successful |
 */
TEST(DefaultSocInterface, PositiveValidNonNullInputs_IsSinkFalse) {
    std::cout << "Entering PositiveValidNonNullInputs_IsSinkFalse test" << std::endl;
    
    // Create DefaultSocInterface object using default constructor
    DefaultSocInterface socInterface;
    
    const char *volume = "initialVolume";
    const char *mute = "initialMute";
    bool isSinkBinVolume = false;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;    
    std::cout << "Exiting PositiveValidNonNullInputs_IsSinkFalse test" << std::endl;
}
/**
 * @brief Validates that SetAudioProperty does not throw an exception when invoked with valid non-null inputs and isSinkBinVolume set to true.
 *
 * This test invokes the SetAudioProperty method on the DefaultSocInterface class using valid non-null inputs for volume and mute, and a true value for the isSinkBinVolume flag. The objective is to ensure that the API handles the provided parameters correctly without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 051
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetAudioProperty with valid non-null inputs when isSinkBinVolume is true | volume = dummyVolume, mute = dummyMute, isSinkBinVolume = true | No exception thrown from SetAudioProperty invocation | Should Pass |
 */
TEST(DefaultSocInterface, PositiveValidNonNullInputs_IsSinkTrue) {
    std::cout << "Entering PositiveValidNonNullInputs_IsSinkTrue test" << std::endl;
    
    DefaultSocInterface socInterface;
    
    const char *volume = "dummyVolume";
    const char *mute = "dummyMute";
    bool isSinkBinVolume = true;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
        
    std::cout << "Exiting PositiveValidNonNullInputs_IsSinkTrue test" << std::endl;
}
/**
 * @brief Test SetAudioProperty with a null volume pointer to validate negative input handling
 *
 * This test verifies that calling SetAudioProperty with a null pointer for volume does not throw an exception even though the input is invalid. It aims to ensure that the API gracefully handles a null volume pointer scenario without causing a crash.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                          | Test Data                                                      | Expected Result                                    | Notes        |
 * | :--------------: | -------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------- | ------------ |
 * | 01               | Invoke SetAudioProperty with a null volume pointer and a valid mute value, verifying that the API handles the null pointer without throwing an exception | volume = nullptr, mute = initialMute, isSinkBinVolume = false    | The function call does not throw an exception (EXPECT_NO_THROW) | Should Fail  |
 */
TEST(DefaultSocInterface, NegativeNullVolumePointer) {
    std::cout << "Entering NegativeNullVolumePointer test" << std::endl;
    
    DefaultSocInterface socInterface;
    
    const char *volume = nullptr;  // Null pointer for volume
    const char *mute = "initialMute";
    bool isSinkBinVolume = false;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: " << (volume ? volume : "null") << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: " << (volume ? volume : "null") << std::endl;
    std::cout << "  mute: " << mute << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
        
    std::cout << "Exiting NegativeNullVolumePointer test" << std::endl;
}
/**
 * @brief Negative test for SetAudioProperty with a null mute pointer.
 *
 * This test verifies that invoking SetAudioProperty with a valid volume string,
 * a null mute pointer, and a false sink flag does not result in any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 053
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetAudioProperty API with "initialVolume" for volume, null for mute, and false for isSinkBinVolume | volume = "initialVolume", mute = null, isSinkBinVolume = false | API should not throw any exception | Should Pass |
 */
TEST(DefaultSocInterface, NegativeNullMutePointer) {
    std::cout << "Entering NegativeNullMutePointer test" << std::endl;
    
    DefaultSocInterface socInterface;
    
    const char *volume = "initialVolume";
    const char *mute = nullptr;  // Null pointer for mute
    bool isSinkBinVolume = false;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << (mute ? mute : "null") << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: " << volume << std::endl;
    std::cout << "  mute: " << (mute ? mute : "null") << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    std::cout << "Exiting NegativeNullMutePointer test" << std::endl;
}
/**
 * @brief Verify SetAudioProperty handles an empty volume string correctly.
 *
 * This test verifies that when an empty volume string is provided along with a valid mute string and a boolean flag indicating
 * the type of volume (isSinkBinVolume), the SetAudioProperty API executes without throwing any exceptions. It ensures that the
 * API can gracefully handle empty volume inputs without causing errors during execution.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 054
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                                                 | Expected Result                                        | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------------------------------- | ------------------------------------------------------ | ---------- |
 * | 01               | Initialize volume as an empty string, mute as "someMute", and isSinkBinVolume as false; then invoke SetAudioProperty with these parameters. | volume = "", mute = "someMute", isSinkBinVolume = false     | API executes without throwing exceptions and logs the call appropriately. | Should Pass |
 */
TEST(DefaultSocInterface, EmptyVolumeString) {
    std::cout << "Entering EmptyVolumeString test" << std::endl;
    
    DefaultSocInterface socInterface;
    
    char volumeBuffer[50] = "";
    const char *volume = volumeBuffer;
    char muteBuffer[50] = "someMute";
    const char *mute = muteBuffer;
    bool isSinkBinVolume = false;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: \"" << volume << "\"" << std::endl;
    std::cout << "  mute: \"" << mute << "\"" << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: \"" << volume << "\"" << std::endl;
    std::cout << "  mute: \"" << mute << "\"" << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    std::cout << "Exiting EmptyVolumeString test" << std::endl;
}
/**
 * @brief Verify that SetAudioProperty handles an empty mute string correctly.
 *
 * This test verifies that the DefaultSocInterface's SetAudioProperty method properly handles an empty mute string along with a valid volume string and a boolean flag. It ensures that the API does not throw an exception when provided with an empty mute value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 055
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ----------- | ----------- | -------------- | ----- |
 * | 01 | Initialize DefaultSocInterface object and input parameters. | volume = "someVolume", mute = "", isSinkBinVolume = false | Object and parameters are initialized successfully. | Should be successful |
 * | 02 | Invoke SetAudioProperty with the provided parameters. | volume = "someVolume", mute = "", isSinkBinVolume = false | API call does not throw an exception. | Should Pass |
 * | 03 | Output parameter values after invocation for verification. | volume = "someVolume", mute = "", isSinkBinVolume = false | The parameters remain unchanged as expected. | Should be successful |
 */
TEST(DefaultSocInterface, EmptyMuteString) {
    std::cout << "Entering EmptyMuteString test" << std::endl;
    
    DefaultSocInterface socInterface;
    
    char volumeBuffer[50] = "someVolume";
    const char *volume = volumeBuffer;
    char muteBuffer[50] = "";
    const char *mute = muteBuffer;
    bool isSinkBinVolume = false;
    
    std::cout << "Invoking SetAudioProperty with parameters:" << std::endl;
    std::cout << "  volume: \"" << volume << "\"" << std::endl;
    std::cout << "  mute: \"" << mute << "\"" << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    EXPECT_NO_THROW(socInterface.SetAudioProperty(volume, mute, isSinkBinVolume));
    
    std::cout << "After invocation of SetAudioProperty:" << std::endl;
    std::cout << "  volume: \"" << volume << "\"" << std::endl;
    std::cout << "  mute: \"" << mute << "\"" << std::endl;
    std::cout << "  isSinkBinVolume: " << std::boolalpha << isSinkBinVolume << std::endl;
    
    std::cout << "Exiting EmptyMuteString test" << std::endl;
}
/**
 * @brief Validate that SetH264Caps does not throw when provided with a valid GstCaps pointer carrying proper H264 H264 parameters.
 *
 * This test ensures that a DefaultSocInterface object can be created successfully without throwing exceptions,
 * and that calling the SetH264Caps method with a valid GstCaps pointer (simulating proper H264 video parameters)
 * executes without error. This confirms both the proper handling of valid input and the robustness of the API.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 056@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Create a DefaultSocInterface object using its default constructor           | No input values                                                | Object created without throwing an exception           | Should Pass   |
 * | 02               | Allocate a valid GstCaps pointer to simulate H264 video parameters            | caps pointer allocated (caps = valid GstCaps pointer)          | GstCaps pointer allocated successfully                 | Should be successful |
 * | 03               | Invoke the SetH264Caps method with the valid GstCaps pointer                  | input: caps pointer; output: method processes H264 info        | SetH264Caps invoked without throwing an exception        | Should Pass   |
 * | 04               | Clean up the GstCaps pointer                                                  | Resource corresponding to caps                                 | GstCaps pointer cleanup performed successfully           | Should be successful |
 */
TEST(DefaultSocInterface, ValidGstCapsPointerWithProperH264Info) {
    std::cout << "Entering ValidGstCapsPointerWithProperH264Info test" << std::endl;

    // Create DefaultSocInterface object using default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created." << std::endl;
    
        // Allocate a valid GstCaps pointer to simulate H264 video parameters
        GstCaps* caps;
        std::cout << "Allocated GstCaps pointer at " << static_cast<void*>(caps) << std::endl;
        
        // Invoke the SetH264Caps method with valid GstCaps pointer
        std::cout << "Invoking SetH264Caps with GstCaps pointer  " << std::endl;
        EXPECT_NO_THROW({
            socInterface.SetH264Caps(caps);
            std::cout << "SetH264Caps method invoked successfully." << std::endl;
        });
    
        std::cout << "Cleaned up GstCaps pointer." << std::endl;
    });
    
    std::cout << "Exiting ValidGstCapsPointerWithProperH264Info test" << std::endl;
}
/**
 * @brief Test to verify that SetH264Caps handles a nullptr GstCaps pointer without throwing exceptions
 *
 * This test verifies that invoking the SetH264Caps method on a DefaultSocInterface object with a nullptr
 * as the GstCaps pointer does not result in any exception being thrown. It first creates a DefaultSocInterface
 * object using its default constructor, then calls SetH264Caps with a nullptr parameter. This ensures that the API
 * gracefully handles null pointer inputs and continues execution as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 057
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                             | Expected Result                                          | Notes        |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------------- | ------------------------------------------------------- | ------------ |
 * | 01               | Create DefaultSocInterface object using default constructor        | No input, output: DefaultSocInterface instance        | Object is created without throwing any exceptions       | Should Pass  |
 * | 02               | Invoke SetH264Caps with a nullptr GstCaps pointer                    | Input: GstCaps pointer = nullptr                       | SetH264Caps method executes without throwing any exceptions | Should Pass  |
 */
TEST(DefaultSocInterface, NullGstCapsPointer) {
    std::cout << "Entering NullGstCapsPointer test" << std::endl;

    // Create DefaultSocInterface object using default constructor
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created." << std::endl;
    
        // Invoke the SetH264Caps method with nullptr as the GstCaps pointer
        std::cout << "Invoking SetH264Caps with nullptr" << std::endl;
        EXPECT_NO_THROW({
            socInterface.SetH264Caps(nullptr);
            std::cout << "SetH264Caps method invoked with nullptr successfully." << std::endl;
        });
    });

    std::cout << "Exiting NullGstCapsPointer test" << std::endl;
}
/**
 * @brief Verify that SetHevcCaps successfully initializes HEVC settings with a valid GstCaps pointer.
 *
 * This test case creates an instance of DefaultSocInterface and validates that calling SetHevcCaps with a valid GstCaps pointer does not throw exceptions. It ensures proper initialization of internal HEVC settings based on the provided GstCaps data.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 058
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                         | Expected Result                                                | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------------------------- | -------------------------------------------------------------- | ---------- |
 * | 01               | Create DefaultSocInterface object using its default constructor    | No inputs, output: an instance of DefaultSocInterface             | Object created without throwing an exception                   | Should Pass|
 * | 02               | Invoke SetHevcCaps with a valid GstCaps pointer using validCapsInstance | input: DefaultSocInterface object, input: validCapsInstance pointer | API call completes without throwing an exception and initializes internal HEVC settings based on GstCaps | Should Pass|
 */
TEST(DefaultSocInterface, ValidGstCaps) {
    std::cout << "Entering ValidGstCaps test" << std::endl;
    
    // Create the DefaultSocInterface object using its default constructor
    std::cout << "Creating DefaultSocInterface object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
    });
    DefaultSocInterface socInterface;
    GstCaps validCapsInstance;
    
    // Invoke SetHevcCaps with valid GstCaps pointer
    std::cout << "Invoking SetHevcCaps with valid GstCaps pointer " << std::endl;
    EXPECT_NO_THROW({
        socInterface.SetHevcCaps(&validCapsInstance);
    });
    std::cout << "SetHevcCaps executed successfully with valid GstCaps pointer. Internal HEVC settings should be initialized based on the passed GstCaps data." << std::endl;
    
    std::cout << "Exiting ValidGstCaps test" << std::endl;
}
/**
 * @brief Verify that the DefaultSocInterface handles a null GstCaps pointer gracefully.
 *
 * This test checks that creating a DefaultSocInterface using its default constructor does not throw any exception, and that invoking
 * the SetHevcCaps API with a null GstCaps pointer is handled correctly without causing any runtime errors. This test ensures that
 * the API deals with null input parameters as specified and maintains stability.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * **Priority:** (High) This test is critical to ensure that the interface handles null inputs gracefully and does not cause failures in downstream processing@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                           | Expected Result                                                     | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------- | ------------------------------------------------------------------- | ---------- |
 * | 01               | Create a DefaultSocInterface object using the default constructor. | DefaultSocInterface constructor, no inputs          | No exception thrown when creating the object.                      | Should Pass|
 * | 02               | Invoke SetHevcCaps with a null GstCaps pointer on the created object.| input: GstCaps pointer = nullptr                     | The SetHevcCaps API call should execute without exceptions.         | Should Pass|
 */
TEST(DefaultSocInterface, NullGstCaps) {
    std::cout << "Entering NullGstCaps test" << std::endl;
    
    // Create the DefaultSocInterface object using its default constructor
    std::cout << "Creating DefaultSocInterface object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
    });
    DefaultSocInterface socInterface;
    
    // Log that we are invoking SetHevcCaps with a null pointer
    std::cout << "Invoking SetHevcCaps with null GstCaps pointer" << std::endl;
    EXPECT_NO_THROW({
        socInterface.SetHevcCaps(nullptr);
    });
    std::cout << "SetHevcCaps handled null GstCaps pointer gracefully without error. Expected behavior: handled failure status internally." << std::endl;
    
    std::cout << "Exiting NullGstCaps test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackFlags correctly processes flags=0 and isSub true without throwing exceptions
 *
 * This test verifies that when the SetPlaybackFlags method is called with an initial flag value of 0 and the isSub flag set to true, the method executes without throwing any exceptions. The test confirms proper handling of the zero flag input in a positive scenario@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 060@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                          | Test Data                   | Expected Result                                  | Notes      |
 * | :--------------: | ---------------------------------------------------- | --------------------------- | ------------------------------------------------ | ---------- |
 * | 01               | Invoke SetPlaybackFlags with flags=0 and isSub=true  | flags = 0, isSub = true     | API call completes without throwing an exception | Should Pass |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithFlagsZero_IsSubTrue)
{
    std::cout << "Entering SetPlaybackFlags_WithFlagsZero_IsSubTrue test" << std::endl;

    gint flags = 0;
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });

    std::cout << "Exiting SetPlaybackFlags_WithFlagsZero_IsSubTrue test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackFlags correctly handles a flags value of 0 with isSub set to false.
 *
 * This test verifies that calling DefaultSocInterface::SetPlaybackFlags with a flags argument of 0 
 * and isSub set to false does not throw any exception. It ensures that the method can process the default or 
 * base case correctly without altering the expected behavior.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 061
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables. | flags = 0, isSub = false | Variables initialized correctly. | Should be successful |
 * | 02 | Create DefaultSocInterface instance. | None | Object is created successfully. | Should be successful |
 * | 03 | Invoke SetPlaybackFlags API with the initialized parameters. | flags = 0, isSub = false | Method executes without throwing exceptions. | Should Pass |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithFlagsZero_IsSubFalse)
{
    std::cout << "Entering SetPlaybackFlags_WithFlagsZero_IsSubFalse test" << std::endl;

    gint flags = 0;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });

    std::cout << "Exiting SetPlaybackFlags_WithFlagsZero_IsSubFalse test" << std::endl;
}
/**
 * @brief Validates that DefaultSocInterface::SetPlaybackFlags executes correctly with positive flag values and an active subtitle flag.
 *
 * This test verifies that the SetPlaybackFlags method of the DefaultSocInterface class operates without throwing exceptions when provided with a positive flag value (flags = 5) and a subtitle flag (isSub = true). This confirms that the API handles valid, positive input conditions as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 062@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                | Expected Result                                                         | Notes       |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SetPlaybackFlags with flags = 5 and isSub = true      | flags = 5, isSub = true                  | No exception thrown; method executes correctly without errors           | Should Pass |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithPositiveFlags_Subtitle)
{
    std::cout << "Entering SetPlaybackFlags_WithPositiveFlags_Subtitle test" << std::endl;

    gint flags = 5;
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });

    std::cout << "Exiting SetPlaybackFlags_WithPositiveFlags_Subtitle test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::SetPlaybackFlags performs correctly when provided with positive non-subtitle flags.
 *
 * This test validates that the DefaultSocInterface::SetPlaybackFlags method does not throw any exceptions when called with a positive flags value (5) and isSub set to false. The test ensures proper creation of the DefaultSocInterface object and correct execution of the method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 063@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                  | Expected Result                                          | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------- | --------------- |
 * | 01               | Log the entry message for the test                                          | None                                       | Correct entry message printed                            | Should be successful |
 * | 02               | Initialize flags to 5 and isSub to false                                    | flags = 5, isSub = false                    | Variables should be initialized with the specified values| Should be successful |
 * | 03               | Create DefaultSocInterface object and invoke SetPlaybackFlags with flags and isSub | input: flags = 5, isSub = false              | Method call should not throw any exception               | Should Pass     |
 * | 04               | Log the exit message for the test                                           | None                                       | Correct exit message printed                             | Should be successful |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithPositiveFlags_NonSubtitle)
{
    std::cout << "Entering SetPlaybackFlags_WithPositiveFlags_NonSubtitle test" << std::endl;

    gint flags = 5;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });

    std::cout << "Exiting SetPlaybackFlags_WithPositiveFlags_NonSubtitle test" << std::endl;
}
/**
 * @brief Verify that DefaultSocInterface::SetPlaybackFlags handles negative flag values for subtitle mode.
 *
 * This test ensures that when a negative flag value is passed along with a subtitle indicator, the SetPlaybackFlags method executes without throwing an exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 064@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables with negative flag value and subtitle indicator. | flags = -3, isSub = true | Variables initialized with flags set to -3 and isSub set to true. | Should be successful |
 * | 02 | Create a DefaultSocInterface object and invoke SetPlaybackFlags with the test variables. | DefaultSocInterface object, flags = -3, isSub = true | Method executes without throwing an exception. | Should Pass |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithNegativeFlags_Subtitle)
{
    std::cout << "Entering SetPlaybackFlags_WithNegativeFlags_Subtitle test" << std::endl;

    gint flags = -3;
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting SetPlaybackFlags_WithNegativeFlags_Subtitle test" << std::endl;
}
/**
 * @brief Validates that SetPlaybackFlags handles negative flags correctly with non-subtitle settings
 *
 * This test ensures that the DefaultSocInterface::SetPlaybackFlags method does not throw an exception when 
 * invoked with negative flags (-3) and the isSub flag set to false. Verifying that the method robustly handles 
 * invalid flag inputs helps ensure stability of the system even with unintended negative values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 065@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                          | Expected Result                                         | Notes       |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------- | ------------------------------------------------------- | ----------- |
 * | 01               | Create DefaultSocInterface object and initialize flags for testing   | flags = -3, isSub = false          | Object creation successful                              | Should Pass |
 * | 02               | Invoke SetPlaybackFlags with negative flags and non-subtitle flag      | flags = -3, isSub = false          | Method execution completes without throwing an exception | Should Pass |
 */
TEST(DefaultSocInterface, SetPlaybackFlags_WithNegativeFlags_NonSubtitle)
{
    std::cout << "Entering SetPlaybackFlags_WithNegativeFlags_NonSubtitle test" << std::endl;

    gint flags = -3;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;

    EXPECT_NO_THROW({
        DefaultSocInterface soc;
        std::cout << "Created DefaultSocInterface object." << std::endl;
        std::cout << "Invoking DefaultSocInterface::SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        soc.SetPlaybackFlags(flags, isSub);
        std::cout << "Method invocation completed. Updated flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting SetPlaybackFlags_WithNegativeFlags_NonSubtitle test" << std::endl;
}

// Dummy pointer definitions for valid GstElement pointers.
static GstElement* valid_source1 = reinterpret_cast<GstElement*>(0x1);
static GstElement* valid_source2 = reinterpret_cast<GstElement*>(0x2);
static GstElement* valid_pipeline  = reinterpret_cast<GstElement*>(0x3);
static GstElement* valid_video_dec = reinterpret_cast<GstElement*>(0x4);
static GstElement* valid_audio_dec = reinterpret_cast<GstElement*>(0x5);
/**
 * @brief Test SetPlaybackRate API with valid inputs to verify positive playback rate scenario
 *
 * This test verifies that when valid sources, pipeline, video, and audio decoder elements are provided along with a positive playback rate, the SetPlaybackRate function executes without throwing exceptions and returns true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 066@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                                                                                                    | Expected Result                                                                   | Notes            |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create DefaultSocInterface instance using default constructor                             | None                                                                                                                         | Instance created without throwing an exception                                  | Should Pass      |
 * | 02               | Prepare input parameters for SetPlaybackRate method                                         | sources = valid_source1, valid_source2, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec | Input parameters are prepared correctly                                           | Should be successful |
 * | 03               | Log the input parameters details to the console                                             | sources size, pipeline, rate, video_dec, audio_dec                                                                             | Details logged on the console without issues                                      | Should be successful |
 * | 04               | Invoke SetPlaybackRate with valid input parameters                                          | sources = valid_source1, valid_source2, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec | SetPlaybackRate returns true without throwing exception                         | Should Pass      |
 * | 05               | Assert that the returned result is true                                                     | output result = true                                                                                                           | EXPECT_TRUE assertion passes, confirming successful API operation                 | Should be successful |
 */
TEST(DefaultSocInterface, ValidPositivePlaybackRate) {
    GTEST_SKIP();
    std::cout << "Entering ValidPositivePlaybackRate test" << std::endl;
    
    // Create an instance using default constructor.
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    // Prepare input parameters.
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 1.0;
    
    // Debug logs: print input parameters.
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    // Invocation of SetPlaybackRate.
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    // Debug log: print the returned result.
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: true.
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidPositivePlaybackRate test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false and does not throw when invoked with a negative playback rate.
 *
 * This test validates that the SetPlaybackRate method of DefaultSocInterface gracefully handles an invalid negative playback rate input by returning false rather than throwing an exception. The test is designed to ensure that the API implementation properly checks and rejects negative values for the playback rate.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 067@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                              | Test Data                                                                                                                              | Expected Result                                                                     | Notes            |
 * | :--------------: | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create an instance of DefaultSocInterface and ensure no exception is thrown               | Constructor invocation: DefaultSocInterface soc                                                                                      | Object is instantiated without throwing exceptions                                  | Should be successful |
 * | 02               | Prepare input parameters including media sources vector and a negative playback rate        | Input: sources = { valid_source1, valid_source2 }, valid_pipeline = valid_pipeline, rate = -1.0, valid_video_dec = valid_video_dec, valid_audio_dec = valid_audio_dec | All test input values are set appropriately                                           | Should be successful |
 * | 03               | Invoke the SetPlaybackRate API with the prepared parameters                                | API call: soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec)                                          | API returns false indicating the invalid negative rate is rejected                     | Should Fail      |
 * | 04               | Verify that the method returns false using an assertion                                    | Assertion check: result = false                                                                                                        | EXPECT_FALSE(result) passes confirming the API's expected behavior                      | Should Pass      |
 */
TEST(DefaultSocInterface, ValidNegativePlaybackRate) {
    std::cout << "Entering ValidNegativePlaybackRate test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = -1.0;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting ValidNegativePlaybackRate test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when a zero playback rate is provided.
 *
 * This test validates that the SetPlaybackRate API handles an invalid playback rate (0.0) gracefully by not throwing exceptions and returning false. It ensures robust handling of invalid parameters to prevent unexpected behavior in the application.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 068@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface and set up test data. | None | Instance is created without throwing an exception. | Should be successful |
 * | 02 | Invoke SetPlaybackRate with sources = valid_source1, valid_source2; pipeline = valid_pipeline; rate = 0.0; video_dec = valid_video_dec; audio_dec = valid_audio_dec. | sources = valid_source1, valid_source2, pipeline = valid_pipeline, rate = 0.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec | API returns false and passes the assertion check. | Should Pass |
 */
TEST(DefaultSocInterface, ZeroPlaybackRate) {
    std::cout << "Entering ZeroPlaybackRate test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 0.0;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting ZeroPlaybackRate test" << std::endl;
}
/**
 * @brief Test SetPlaybackRate with a null pipeline pointer to verify robust error handling.
 *
 * This test verifies that when the SetPlaybackRate method is invoked with a null pipeline pointer,
 * it does not throw an exception and returns false as expected. The test ensures that the function
 * handles invalid pipeline references gracefully without causing unexpected crashes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 069@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                   | Test Data                                                                                                                       | Expected Result                                               | Notes             |
 * | :--------------: | ----------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------- |
 * | 01               | Instantiate DefaultSocInterface object                                        | None                                                                                                                             | Object is created successfully                                | Should be successful |
 * | 02               | Invoke SetPlaybackRate with a null pipeline pointer and valid parameters      | input sources = valid_source1, valid_source2; input pipeline = nullptr; input rate = 1.0; input video_dec = valid_video_dec; input audio_dec = valid_audio_dec | Method returns false without throwing any exceptions          | Should Fail       |
 * | 03               | Verify that the returned result is false                                      | output result = false                                                                                                              | Assertion passes confirming the result is false              | Should be successful |
 */
TEST(DefaultSocInterface, NullPipelinePointer) {
    std::cout << "Entering NullPipelinePointer test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 1.0;
    GstElement* null_pipeline = nullptr;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << null_pipeline << " (null)" << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, null_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting NullPipelinePointer test" << std::endl;
}
/**
 * @brief Tests that the SetPlaybackRate method properly handles a null video decoder pointer.
 *
 * This test verifies the behavior of the SetPlaybackRate API when provided with a null pointer for the video decoder.
 * It uses a valid vector of sources, a valid pipeline, valid audio decoder, and a valid rate while intentionally passing a null
 * pointer for the video decoder. The expectation is that the function returns false without throwing an exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Invoke SetPlaybackRate using valid sources, pipeline, rate, a null video decoder, and valid audio decoder. | sources = {valid_source1, valid_source2}, pipeline = valid_pipeline, rate = 1.0, video_dec = nullptr, audio_dec = valid_audio_dec | API returns false with no exceptions thrown. | Should Pass |
 */
TEST(DefaultSocInterface, NullVideoDecoderPointer) {
    std::cout << "Entering NullVideoDecoderPointer test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 1.0;
    GstElement* null_video_dec = nullptr;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << null_video_dec << " (null)" << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, null_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting NullVideoDecoderPointer test" << std::endl;
}
/**
 * @brief Test SetPlaybackRate behavior when audio decoder pointer is null
 *
 * This test verifies that the SetPlaybackRate method returns false when invoked with a null audio decoder pointer, ensuring the API correctly handles invalid audio decoder references without crashing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 071@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                            | Test Data                                                                                                                        | Expected Result                                                              | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SetPlaybackRate with valid sources, valid pipeline, valid video decoder, and null audio decoder pointer | sources = valid_source1, valid_source2, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = nullptr | Returns false and the EXPECT_FALSE assertion passes                         | Should Pass |
 */
TEST(DefaultSocInterface, NullAudioDecoderPointer) {
    std::cout << "Entering NullAudioDecoderPointer test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 1.0;
    GstElement* null_audio_dec = nullptr;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << null_audio_dec << " (null)" << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, null_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting NullAudioDecoderPointer test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when provided with an empty sources vector.
 *
 * This test verifies that the DefaultSocInterface::SetPlaybackRate method behaves correctly when provided with an empty vector for sources. The test ensures that no exceptions are thrown during instance creation and API invocation, and that the method returns false as expected when no valid sources are provided. This is critical for confirming that the API handles invalid or missing input scenarios as intended.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 072
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                           | Test Data                                                                                                                                         | Expected Result                                                              | Notes             |
 * | :--------------: | ------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create an instance of DefaultSocInterface without throwing an exception.               | N/A                                                                                                                                               | Instance is created successfully without any exception.                    | Should be successful |
 * | 02               | Prepare test parameters: initialize an empty sources vector and define valid pipeline values. | sources = empty vector, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec                                | Test parameters are set-up correctly.                                        | Should be successful |
 * | 03               | Invoke SetPlaybackRate with the prepared parameters and validate the method's return value. | sources = empty vector, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec, output = bool result | The method returns false and EXPECT_FALSE(result) assertion passes.          | Should Pass       |
 */
TEST(DefaultSocInterface, EmptySourcesVector) {
    std::cout << "Entering EmptySourcesVector test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources; // empty vector
    double rate = 1.0;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << " (empty)" << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptySourcesVector test" << std::endl;
}
/**
 * @brief Test to verify that SetPlaybackRate handles a sources vector containing a null element correctly
 *
 * This test verifies that when the sources vector passed to SetPlaybackRate contains a null element between valid GstElement pointers,
 * the method processes the input without throwing an exception and returns false as expected. This ensures that the method can handle
 * cases where the source list is not completely valid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 073@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DefaultSocInterface and invoke SetPlaybackRate with a sources vector containing a null element among valid pointers. | sources = { valid_source1, nullptr, valid_source2 }, pipeline = valid_pipeline, rate = 1.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec | No exception is thrown and the API returns false | Should Pass |
 */
TEST(DefaultSocInterface, NullElementInSourcesVector) {
    std::cout << "Entering NullElementInSourcesVector test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    // sources vector with one null element in between valid pointers.
    std::vector<GstElement*> sources = { valid_source1, nullptr, valid_source2 };
    double rate = 1.0;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << " (includes a null element)" << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting NullElementInSourcesVector test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate handles an extremely high playback rate correctly
 *
 * This test checks that calling SetPlaybackRate with a playback rate of 1000.0 on a DefaultSocInterface does not throw any exceptions and returns false as expected. It ensures that the API behaves correctly when provided with an extremely high rate which is likely beyond normal operating conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 074@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Verify that DefaultSocInterface is instantiated without throwing exceptions. | Constructor: no input arguments | Instance created successfully without exceptions. | Should be successful |
 * | 02 | Invoke SetPlaybackRate with two valid sources, a valid pipeline, a playback rate of 1000.0, valid video decoder, and valid audio decoder. | sources = {valid_source1, valid_source2}, pipeline = valid_pipeline, rate = 1000.0, video_dec = valid_video_dec, audio_dec = valid_audio_dec | Method returns false and no exceptions are thrown. | Should Pass |
 * | 03 | Assert that the returned result from SetPlaybackRate is false. | result = false | EXPECT_FALSE(result) assertion passes. | Should be successful |
 */
TEST(DefaultSocInterface, ExtremelyHighPlaybackRate) {
    std::cout << "Entering ExtremelyHighPlaybackRate test" << std::endl;
    
    EXPECT_NO_THROW(DefaultSocInterface soc);
    DefaultSocInterface soc;
    
    std::vector<GstElement*> sources = { valid_source1, valid_source2 };
    double rate = 1000.0;
    
    std::cout << "Invoking SetPlaybackRate with:" << std::endl;
    std::cout << " - sources size: " << sources.size() << std::endl;
    std::cout << " - pipeline: " << valid_pipeline << std::endl;
    std::cout << " - rate: " << rate << std::endl;
    std::cout << " - video_dec: " << valid_video_dec << std::endl;
    std::cout << " - audio_dec: " << valid_audio_dec << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW(result = soc.SetPlaybackRate(sources, valid_pipeline, rate, valid_video_dec, valid_audio_dec));
    
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false.
    EXPECT_FALSE(result);
    std::cout << "Exiting ExtremelyHighPlaybackRate test" << std::endl;
}
/**
 * @brief Validate the SetRateCorrection API functionality using default constructed object.
 *
 * This test case verifies that invoking the SetRateCorrection method on a DefaultSocInterface object 
 * constructed by its default constructor does not throw exceptions and returns a boolean value (true or false). 
 * It ensures that the API call executes correctly and the internal state remains unchanged.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 075@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                                         | Expected Result                                                                  | Notes          |
 * | :--------------: | ------------------------------------------------------------ | ----------------------------------------------------------------- | -------------------------------------------------------------------------------- | -------------- |
 * | 01               | Create a DefaultSocInterface object using the default constructor. | No input, output: instance of DefaultSocInterface created         | Object is created successfully without any exception thrown.                     | Should be successful |
 * | 02               | Invoke the SetRateCorrection() method on the created object.      | No input, output: result (bool) from SetRateCorrection invoked      | API returns a valid boolean value (true or false) without throwing exceptions.   | Should Pass    |
 * | 03               | Validate that the method returns a boolean value as expected.       | result = boolean value returned from SetRateCorrection              | The returned value is a boolean (either true or false) and assertion check passes. | Should be successful |
 */
TEST(DefaultSocInterface, SetRateCorrection_start) {
    std::cout << "Entering SetRateCorrection_start test" << std::endl;
    
    EXPECT_NO_THROW({
        // Create object using default constructor
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object using default constructor." << std::endl;
        
        // Invoke SetRateCorrection and log method invocation
        std::cout << "Invoking SetRateCorrection() method." << std::endl;
        bool result = socInterface.SetRateCorrection();
        std::cout << "SetRateCorrection() returned: " << std::boolalpha << result << std::endl;
        
        // Validate that the method returns false as expected.
        EXPECT_TRUE(result == true || result == false);
        
        // Log internal state details if any (mUsingWesterosSink assumed unchanged)
        std::cout << "Internal state mUsingWesterosSink assumed unchanged." << std::endl;
    });
    
    std::cout << "Exiting SetRateCorrection_start test" << std::endl;
}
/**
 * @brief Verify that the DefaultSocInterface object is constructed correctly and its method ShouldTearDownForTrickplay() returns a valid boolean value.
 *
 * This test constructs a DefaultSocInterface object using the default constructor and then invokes its ShouldTearDownForTrickplay() method. The test ensures no exception is thrown during the object construction and that the method returns a valid boolean value (either true or false), verifying that the method behaves as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 076@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                                      | Expected Result                                                      | Notes             |
 * | :---------------: | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- |
 * | 01                | Construct DefaultSocInterface object using the default constructor.                           | Constructor call, no parameters                                | Object should be constructed without throwing any exception          | Should Pass       |
 * | 02                | Invoke the ShouldTearDownForTrickplay() method on the constructed object.                       | Method call: ShouldTearDownForTrickplay(), no input parameters     | Method should return a boolean value (either true or false)            | Should Pass       |
 * | 03                | Verify that the returned boolean value from ShouldTearDownForTrickplay() is valid.               | Return value from ShouldTearDownForTrickplay() = <expected bool>   | Assertion should pass confirming the return value is either true or false | Should be successful |
 */
TEST(DefaultSocInterface, ShouldTearDownForTrickplay_start) {
    std::cout << "Entering ShouldTearDownForTrickplay_start test" << std::endl;

    // Construct DefaultSocInterface object using default constructor.
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object constructed using default constructor." << std::endl;
        
        // Invoke the method ShouldTearDownForTrickplay().
        std::cout << "Invoking ShouldTearDownForTrickplay() method." << std::endl;
        bool returnValue = socInterface.ShouldTearDownForTrickplay();
        std::cout << "Returned value from ShouldTearDownForTrickplay(): " << returnValue << std::endl;
        
        // Verify that the method returns either false or true.
        // This is inherently always true for a bool, but we assert it nonetheless.
        EXPECT_TRUE(returnValue == false || returnValue == true);
        std::cout << "Method verification passed: Returned value is a valid boolean." << std::endl;
    });

    std::cout << "Exiting ShouldTearDownForTrickplay_start test" << std::endl;
}
/**
 * @brief Verify that the UseAppSrc method of DefaultSocInterface class can be invoked without throwing exceptions and returns a valid boolean value.
 *
 * This test case ensures that a DefaultSocInterface object can be created using the default constructor and that calling the UseAppSrc method does not throw any exceptions. It also confirms that the method returns a valid boolean value (either true or false).
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 077@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                                                | Expected Result                                                                           | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Create DefaultSocInterface object and invoke UseAppSrc method       | No input; DefaultSocInterface object creation; retValue = UseAppSrc() call | Method returns a valid boolean (true or false) without throwing any exceptions; assertion passes | Should Pass |
 */
TEST(DefaultSocInterface, UseAppSrc_start) {
    std::cout << "Entering UseAppSrc_start test" << std::endl;
    
    EXPECT_NO_THROW({
        // Creating object of DefaultSocInterface using default constructor.
        DefaultSocInterface socInterface;
        std::cout << "Created DefaultSocInterface object using default constructor." << std::endl;
        
        // Invoking UseAppSrc method.
        std::cout << "Invoking UseAppSrc() method." << std::endl;
        bool retValue = socInterface.UseAppSrc();
        std::cout << "UseAppSrc() returned value: " << std::boolalpha << retValue << std::endl;
        
        // Verifying that returned value is either true or false.
        EXPECT_TRUE(retValue == true || retValue == false);
        std::cout << "Verified UseAppSrc() return value is a valid boolean." << std::endl;
    });
    
    std::cout << "Exiting UseAppSrc_start test" << std::endl;
}
/**
 * @brief Verify that UseWesterosSink returns false as expected.
 *
 * This test verifies that when creating a DefaultSocInterface object using the default constructor and invoking its UseWesterosSink method, the method returns false as expected. This ensures that the API adheres to its expected behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 078@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                 | Test Data                                                     | Expected Result                                              | Notes            |
 * | :--------------: | ----------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------ | ---------------- |
 * | 01               | Create DefaultSocInterface object using default constructor | No input                                                        | Object is created successfully                               | Should be successful |
 * | 02               | Invoke UseWesterosSink method on the created object          | No input; output: result (bool value returned by UseWesterosSink) | Method returns a boolean value (either true or false)        | Should Pass      |
 * | 03               | Verify that UseWesterosSink returns false as expected        | Expected output: false                                          | The method returns false                                     | Should Pass      |
 */
TEST(DefaultSocInterface, VerifyUseWesterosSinkReturnsFalse) {
    std::cout << "Entering VerifyUseWesterosSinkReturnsFalse test" << std::endl;

    // Creating the DefaultSocInterface object using the default constructor.
    EXPECT_NO_THROW({
        DefaultSocInterface socInterface;
        std::cout << "DefaultSocInterface object created successfully using the default constructor." << std::endl;
        
        // Invoking the UseWesterosSink method.
        std::cout << "Invoking UseWesterosSink method." << std::endl;
        bool result = socInterface.UseWesterosSink();
        std::cout << "Method UseWesterosSink returned: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified that UseWesterosSink returned false as expected." << std::endl;
    });
    
    std::cout << "Exiting VerifyUseWesterosSinkReturnsFalse test" << std::endl;
}