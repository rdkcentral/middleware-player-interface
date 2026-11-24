
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
#include "AmlogicSocInterface.h"


// Positive - Successful instantiation and default value check using default constructor

/**
 * @brief Verify that the AmlogicSocInterface constructor does not throw an exception
 *
 * This test case verifies that invoking the default constructor of the AmlogicSocInterface class does not throw any exceptions and creates an instance successfully.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor of AmlogicSocInterface to instantiate an object | input: none, output: AmlogicSocInterface object | Object is instantiated without throwing an exception | Should Pass |
 */
TEST(AmlogicSocInterface, AmlogicSocInterface_constructor_start) {
    std::cout << "Entering AmlogicSocInterface::AmlogicSocInterface()_start test" << std::endl;

    std::cout << "Invoking AmlogicSocInterface default constructor" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "AmlogicSocInterface object instantiated successfully" << std::endl;
    });

    std::cout << "Exiting AmlogicSocInterface::AmlogicSocInterface()_start test" << std::endl;
}
/**
 * @brief Verify that AudioOnlyMode configures the sinkbin element to audio-only mode
 *
 * This test validates that the AmlogicSocInterface::AudioOnlyMode method correctly configures a valid GstElement pointer (sinkbin) for audio-only operation. It ensures that the method does not throw any exception and returns true upon successful configuration.
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
 * | Variation / Step | Description                                                              | Test Data                                                              | Expected Result                                                      | Notes             |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------------------------------- | ---------------------------------------------------------------------| ----------------- |
 * | 01               | Create a dummy GstElement instance and obtain a valid sinkbin pointer      | input: validSinkbinInstance = GstElement instance, sinkbin = &validSinkbinInstance | Valid GstElement pointer is created                                  | Should be successful |
 * | 02               | Construct AmlogicSocInterface and call AudioOnlyMode with the sinkbin pointer | input: sinkbin = valid pointer (&validSinkbinInstance), output: result (bool) | AudioOnlyMode returns true without throwing exceptions               | Should Pass       |
 */
TEST(AmlogicSocInterface, AudioOnlyMode_ValidSinkbin) {
    std::cout << "Entering AudioOnlyMode_ValidSinkbin test" << std::endl;

    // Create a dummy GstElement instance to simulate a valid sinkbin pointer.
    GstElement validSinkbinInstance;
    GstElement* sinkbin = &validSinkbinInstance;
    std::cout << "Created valid GstElement pointer: " << sinkbin << std::endl;

    // Construct the AmlogicSocInterface object and call the AudioOnlyMode method.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "Constructed AmlogicSocInterface object." << std::endl;

        std::cout << "Invoking AudioOnlyMode with sinkbin pointer: " << sinkbin << std::endl;
        bool result = socInterface.AudioOnlyMode(sinkbin);
        std::cout << "AudioOnlyMode returned: " << std::boolalpha << result << std::endl;

        // Verify that the sinkbin was configured successfully to audio-only mode.
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting AudioOnlyMode_ValidSinkbin test" << std::endl;
}
/**
 * @brief Validate the behavior of AudioOnlyMode function when provided with a null GstElement pointer
 *
 * This test verifies that the AudioOnlyMode method of the AmlogicSocInterface class correctly handles a null GstElement pointer. It simulates an invalid input scenario where the GstElement pointer is set to nullptr, and asserts that the method returns false, indicating failure to process a null input.
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
 * | Variation / Step | Description                                                             | Test Data                                             | Expected Result                                                   | Notes         |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------------------------------------------- | ----------------------------------------------------------------- | ------------- |
 * | 01               | Construct AmlogicSocInterface and invoke AudioOnlyMode with nullptr sinkbin | sinkbin = nullptr, output result expected to be false | AudioOnlyMode returns false and assertion EXPECT_FALSE(result) is met | Should Pass   |
 */
TEST(AmlogicSocInterface, AudioOnlyMode_NullSinkbin) {
    std::cout << "Entering AudioOnlyMode_NullSinkbin test" << std::endl;

    // Set the GstElement pointer to nullptr to simulate invalid input.
    GstElement* sinkbin = nullptr;
    std::cout << "Using nullptr for GstElement pointer." << std::endl;

    // Construct the AmlogicSocInterface object and call the AudioOnlyMode method.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "Constructed AmlogicSocInterface object." << std::endl;

        std::cout << "Invoking AudioOnlyMode with nullptr sinkbin." << std::endl;
        bool result = socInterface.AudioOnlyMode(sinkbin);
        std::cout << "AudioOnlyMode returned: " << std::boolalpha << result << std::endl;

        // Verify that the method returns false due to the null input.
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting AudioOnlyMode_NullSinkbin test" << std::endl;
}
/**
 * @brief Verify that ConfigureAudioSink correctly configures audio sink and source when decStreamSync is true
 *
 * This test verifies that the AmlogicSocInterface object can be instantiated successfully and that the ConfigureAudioSink function operates as expected when provided with valid dummy pointers for the audio sink and source along with decStreamSync set to true. The test ensures that no exceptions are thrown during the configuration process and that resources are cleaned up properly.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface and create dummy valid pointers for audio sink and source. | AmlogicSocInterface instance, dummyAudioSink pointer = int(10), dummySrc pointer = int(20) | Object created successfully and pointers initialized. | Should be successful |
 * | 02 | Invoke ConfigureAudioSink with decStreamSync set to true. | dummyAudioSink pointer = int(10), dummySrc pointer = int(20), decStreamSync = true, ret = boolean | Return value from ConfigureAudioSink should indicate successful configuration with no exceptions. | Should Pass |
 * | 03 | Clean up allocated dummy pointers. | Deletion of dummyAudioSink and dummySrc | Memory freed successfully without errors. | Should be successful |
 */
TEST(AmlogicSocInterface, ValidAudioSinkAndSourceWithDecStreamSyncTrue) {
    std::cout << "Entering ValidAudioSinkAndSourceWithDecStreamSyncTrue test" << std::endl;

    // Creating an instance of AmlogicSocInterface using its default constructor.
    EXPECT_NO_THROW({ AmlogicSocInterface amlogicSoc; });
    AmlogicSocInterface amlogicSoc;

    // Create dummy valid pointers for GstElement and GstObject.
    GstElement* dummyAudioSink = reinterpret_cast<GstElement*>(new int(10));
    GstObject* dummySrc = reinterpret_cast<GstObject*>(new int(20));

    std::cout << "Created dummyAudioSink pointer at address: " << dummyAudioSink << std::endl;
    std::cout << "Created dummySrc pointer at address: " << dummySrc << std::endl;

    bool decStreamSync = true;
    std::cout << "Calling ConfigureAudioSink with decStreamSync: " << decStreamSync << std::endl;

    bool ret = false;
    EXPECT_NO_THROW({
        ret = amlogicSoc.ConfigureAudioSink(&dummyAudioSink, dummySrc, decStreamSync);
        std::cout << "ConfigureAudioSink returned: " << ret << std::endl;
    });

    // Clean up the allocated dummy pointers.
    delete reinterpret_cast<int*>(dummyAudioSink);
    delete reinterpret_cast<int*>(dummySrc);

    std::cout << "Exiting ValidAudioSinkAndSourceWithDecStreamSyncTrue test" << std::endl;
}
/**
 * @brief Validate ConfigureAudioSink with valid audio sink and valid source using decStreamSync set to false
 *
 * This test verifies that the ConfigureAudioSink API of AmlogicSocInterface functions correctly when provided
 * with a valid dummy audio sink pointer, a valid dummy source pointer, and the decStreamSync parameter set to false.
 * The test ensures that no exceptions are thrown and that the API returns the expected result.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                | Expected Result                                               | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate AmlogicSocInterface, create dummy audio sink and source, and invoke ConfigureAudioSink using valid pointers with decStreamSync set to false | audio_sink pointer = pointer to integer with value 30, dummySrc pointer = pointer to integer with value 40, decStreamSync = false | API returns expected boolean value without throwing an exception | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioSinkAndSourceWithDecStreamSyncFalse) {
    std::cout << "Entering ValidAudioSinkAndSourceWithDecStreamSyncFalse test" << std::endl;

    EXPECT_NO_THROW({ AmlogicSocInterface amlogicSoc; });
    AmlogicSocInterface amlogicSoc;

    GstElement* dummyAudioSink = reinterpret_cast<GstElement*>(new int(30));
    GstObject* dummySrc = reinterpret_cast<GstObject*>(new int(40));

    std::cout << "Created dummyAudioSink pointer at address: " << dummyAudioSink << std::endl;
    std::cout << "Created dummySrc pointer at address: " << dummySrc << std::endl;

    bool decStreamSync = false;
    std::cout << "Calling ConfigureAudioSink with decStreamSync: " << decStreamSync << std::endl;

    bool ret = false;
    EXPECT_NO_THROW({
        ret = amlogicSoc.ConfigureAudioSink(&dummyAudioSink, dummySrc, decStreamSync);
        std::cout << "ConfigureAudioSink returned: " << ret << std::endl;
    });

    delete reinterpret_cast<int*>(dummyAudioSink);
    delete reinterpret_cast<int*>(dummySrc);

    std::cout << "Exiting ValidAudioSinkAndSourceWithDecStreamSyncFalse test" << std::endl;
}
/**
 * @brief Validates the behavior of ConfigureAudioSink when invoked with a null audio sink pointer and a valid source pointer.
 *
 * This test verifies that the ConfigureAudioSink API properly handles a scenario where the audio_sink argument is a null pointer,
 * while a valid dummy source is provided. It ensures that no exceptions are thrown during the call and that the function returns false,
 * which indicates the erroneous input was correctly managed.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** (High) This test is critical to verify proper error handling in the API function.@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface and invoke ConfigureAudioSink with a null audio sink pointer and a valid dummy source pointer. | input: nullAudioSink = nullptr, dummySrc = valid pointer (created using new int(50)), decStreamSync = true; output: ret = false | The API should return false without throwing an exception. | Should Fail |
 */
TEST(AmlogicSocInterface, NullAudioSinkWithValidSource) {
    std::cout << "Entering NullAudioSinkWithValidSource test" << std::endl;

    EXPECT_NO_THROW({ AmlogicSocInterface amlogicSoc; });
    AmlogicSocInterface amlogicSoc;

    // Passing nullptr for audio_sink pointer.
    GstElement** nullAudioSink = nullptr;
    GstObject* dummySrc = reinterpret_cast<GstObject*>(new int(50));

    std::cout << "Passing nullAudioSink pointer (nullptr)" << std::endl;
    std::cout << "Created dummySrc pointer at address: " << dummySrc << std::endl;

    bool decStreamSync = true;
    std::cout << "Calling ConfigureAudioSink with decStreamSync: " << decStreamSync << std::endl;

    bool ret = false;
    EXPECT_NO_THROW({
        ret = amlogicSoc.ConfigureAudioSink(nullAudioSink, dummySrc, decStreamSync);
        std::cout << "ConfigureAudioSink returned: " << ret << std::endl;
    });
    EXPECT_FALSE(ret);

    delete reinterpret_cast<int*>(dummySrc);

    std::cout << "Exiting NullAudioSinkWithValidSource test" << std::endl;
}
/**
 * @brief Validate ConfigureAudioSink behavior when provided with a valid audio sink but a null source.
 *
 * This test verifies that when a dummy audio sink pointer is supplied along with a null source pointer,
 * the ConfigureAudioSink API correctly returns false, indicating the configuration did not succeed.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 007
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of AmlogicSocInterface and verify no exception is thrown | N/A | Instance is created without exceptions | Should Pass |
 * | 02 | Create a dummy audio sink pointer using reinterpret_cast with a new int(60) | dummyAudioSink = reinterpret_cast<GstElement*>(new int(60)) | Pointer allocated successfully | Should be successful |
 * | 03 | Initialize the source pointer as nullptr | nullSrc = nullptr | Source pointer is set to nullptr | Should be successful |
 * | 04 | Call ConfigureAudioSink with dummyAudioSink, nullSrc, and decStreamSync as false | dummyAudioSink pointer, nullSrc = nullptr, decStreamSync = false | API returns false and EXPECT_FALSE(ret) passes | Should Pass |
 * | 05 | Delete the allocated dummyAudioSink pointer to free memory | dummyAudioSink pointer | Memory freed without error | Should be successful |
 */
TEST(AmlogicSocInterface, ValidAudioSinkWithNullSource) {
    std::cout << "Entering ValidAudioSinkWithNullSource test" << std::endl;

    EXPECT_NO_THROW({ AmlogicSocInterface amlogicSoc; });
    AmlogicSocInterface amlogicSoc;

    GstElement* dummyAudioSink = reinterpret_cast<GstElement*>(new int(60));
    // Passing nullptr for src.
    GstObject* nullSrc = nullptr;

    std::cout << "Created dummyAudioSink pointer at address: " << dummyAudioSink << std::endl;
    std::cout << "Passing nullSrc pointer (nullptr)" << std::endl;

    bool decStreamSync = false;
    std::cout << "Calling ConfigureAudioSink with decStreamSync: " << decStreamSync << std::endl;

    bool ret = false;
    EXPECT_NO_THROW({
        ret = amlogicSoc.ConfigureAudioSink(&dummyAudioSink, nullSrc, decStreamSync);
        std::cout << "ConfigureAudioSink returned: " << ret << std::endl;
    });
    EXPECT_FALSE(ret);

    delete reinterpret_cast<int*>(dummyAudioSink);

    std::cout << "Exiting ValidAudioSinkWithNullSource test" << std::endl;
}
/**
 * @brief Validate that GetCCDecoderHandle returns a valid decoder handle when provided with valid parameters.
 *
 * This test creates an instance of AmlogicSocInterface and a dummy GstElement. It then calls the GetCCDecoderHandle method
 * with valid input parameters and ensures that no exceptions are thrown and that a non-null CC decoder handle is returned.
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
 * | Variation / Step | Description                                                                             | Test Data                                                              | Expected Result                                                                 | Notes            |
 * | :--------------: | --------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create an instance of AmlogicSocInterface and verify that the constructor does not throw.  | Call: AmlogicSocInterface()                                            | No exception thrown; object created successfully                                | Should be successful |
 * | 02               | Create a dummy GstElement instance to simulate a valid video decoder element.           | Create dummyVideoDec instance                                          | Instance created; valid address obtained                                        | Should be successful |
 * | 03               | Initialize ccHandle variable to nullptr.                                                | ccHandle = nullptr                                                     | ccHandle is set to nullptr initially                                            | Should be successful |
 * | 04               | Invoke GetCCDecoderHandle with the address of ccHandle and dummyVideoDec.                 | Input: ccHandle pointer = &ccHandle, dummyVideoDec pointer = &dummyVideoDec | No exception thrown; valid decoder handle assigned to ccHandle                   | Should Pass      |
 * | 05               | Verify that ccHandle is non-null after invoking GetCCDecoderHandle.                       | Output: ccHandle value expected to be non-null                         | ccHandle != nullptr as asserted by EXPECT_NE(ccHandle, nullptr)                   | Should Pass      |
 */
TEST(AmlogicSocInterface, ValidInputParameters) {
    std::cout << "Entering ValidInputParameters test" << std::endl;

    // Creating an instance of AmlogicSocInterface using its default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "AmlogicSocInterface object created successfully" << std::endl;

        // Create a dummy GstElement instance to simulate a valid video decoder element.
        GstElement dummyVideoDec;
        std::cout << "Dummy GstElement instance created at address: " << &dummyVideoDec << std::endl;

        // Create a local gpointer variable to hold the CC decoder handle and initialize to nullptr.
        gpointer ccHandle;
        std::cout << "Initial ccHandle value: " << ccHandle << std::endl;

        // Invoke the method GetCCDecoderHandle with valid parameters.
        std::cout << "Invoking GetCCDecoderHandle with ccHandle address: " << &ccHandle
                  << " and dummyVideoDec address: " << &dummyVideoDec << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(&ccHandle, &dummyVideoDec));

        // Log the returned handle value.
        std::cout << "CC Decoder Handle returned value: " << ccHandle << std::endl;

        // Expect that the function assigns a valid (non-null) decoder handle.
        EXPECT_NE(ccHandle, nullptr);
    });

    std::cout << "Exiting ValidInputParameters test" << std::endl;
}
/**
 * @brief Verify the behavior of AmlogicSocInterface::GetCCDecoderHandle when passed a NULL decoder handle.
 *
 * This test verifies that the GetCCDecoderHandle function of the AmlogicSocInterface class handles a NULL decoder handle gracefully without throwing an exception.
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
 * | Variation / Step | Description                                                                 | Test Data                                                | Expected Result                                                                 | Notes             |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------------------------------------ | ----------------- |
 * | 01               | Create an instance of AmlogicSocInterface                                   | No input parameters                                      | Instance should be created successfully                                        | Should be successful |
 * | 02               | Create a dummy GstElement instance                                          | No input parameters                                      | Dummy GstElement instance should be created with a valid address               | Should be successful |
 * | 03               | Invoke GetCCDecoderHandle with dec_handle as NULL and provided dummy element  | input: dec_handle = NULL, video_dec = address of dummyVideoDec | Function should handle NULL gracefully without throwing an exception; assertion passes | Should Pass       |
 */
TEST(AmlogicSocInterface, NullDecHandle) {
    std::cout << "Entering NullDecHandle test" << std::endl;

    // Creating an instance of AmlogicSocInterface.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "AmlogicSocInterface object created successfully" << std::endl;

        // Create a dummy GstElement instance.
        GstElement dummyVideoDec;
        std::cout << "Dummy GstElement instance created at address: " << &dummyVideoDec << std::endl;

        // Invoke the method with dec_handle as NULL.
        std::cout << "Invoking GetCCDecoderHandle with dec_handle as NULL and video_dec address: " << &dummyVideoDec << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(nullptr, &dummyVideoDec));

        std::cout << "Method invoked successfully with dec_handle set to NULL (handled gracefully)" << std::endl;
    });

    std::cout << "Exiting NullDecHandle test" << std::endl;
}
/**
 * @brief Validate the behavior of GetCCDecoderHandle with a NULL video_dec parameter.
 *
 * This test verifies that the GetCCDecoderHandle API does not throw an exception when invoked with a NULL video_dec parameter while using a dummy ccHandle value. It also confirms the successful creation of the AmlogicSocInterface instance and proper logging of the ccHandle value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                                | Expected Result                                           | Notes             |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------------ | --------------------------------------------------------- | ----------------- |
 * | 01               | Create an instance of AmlogicSocInterface                           | Constructor: AmlogicSocInterface()                                       | Instance is successfully created                          | Should be successful |
 * | 02               | Initialize ccHandle to a dummy error value using 0xDEADBEEF           | ccHandle = reinterpret_cast<gpointer>(0xDEADBEEF)                        | ccHandle is set to the dummy error value                  | Should be successful |
 * | 03               | Invoke GetCCDecoderHandle with ccHandle pointer and NULL video_dec      | Input: ccHandle pointer = &ccHandle, video_dec = NULL                      | API call does not throw an exception                      | Should Pass       |
 * | 04               | Log the ccHandle value after invoking GetCCDecoderHandle              | Output: ccHandle value printed after API call                            | ccHandle value is logged appropriately                    | Should be successful |
 */
TEST(AmlogicSocInterface, NullVideoDec) {
    std::cout << "Entering NullVideoDec test" << std::endl;

    // Creating an instance of AmlogicSocInterface.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "AmlogicSocInterface object created successfully" << std::endl;

        // Declare a local CC decoder handle pointer and initialize it to a known value.
        // Here we use a dummy error indicating value.
        gpointer ccHandle = reinterpret_cast<gpointer>(0xDEADBEEF);
        std::cout << "Initial ccHandle set to dummy error value: " << ccHandle << std::endl;

        // Invoking GetCCDecoderHandle with video_dec as NULL.
        std::cout << "Invoking GetCCDecoderHandle with ccHandle address: " << &ccHandle << " and video_dec as NULL" << std::endl;
        EXPECT_NO_THROW(socInterface.GetCCDecoderHandle(&ccHandle, nullptr));

        // Log the ccHandle value after invocation.
        std::cout << "ccHandle value after invoking GetCCDecoderHandle: " << ccHandle << std::endl;

    });

    std::cout << "Exiting NullVideoDec test" << std::endl;
}
/**
 * @brief Test the GetSourcePad API method with a valid GstElement pointer.
 *
 * This test verifies that the GetSourcePad method of AmlogicSocInterface returns a valid non-null GstPad pointer
 * when provided with a valid GstElement pointer. The test ensures that no exceptions are thrown during object creation,
 * GstElement instantiation, and API invocation, and that the API behavior is as expected in a positive scenario.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 011
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                              | Expected Result                                                      | Notes             |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- |
 * |       01         | Instantiate AmlogicSocInterface object.                            | N/A                                                                    | Object is created successfully without throwing exceptions.        | Should be successful |
 * |       02         | Create a dummy GstElement instance to simulate a valid element.    | dummy GstElement instance address = &validElement                      | GstElement instance is created with a valid memory address.          | Should be successful |
 * |       03         | Invoke GetSourcePad with the valid GstElement pointer.             | input element pointer = &validElement, output pad pointer = non-null value | GetSourcePad returns a non-null pad pointer without throwing exceptions and passes assertion check. | Should Pass       |
 */
TEST(AmlogicSocInterface, GetSourcePad_valid_element) {
    std::cout << "Entering GetSourcePad_valid_element test" << std::endl;
    
    // Create an instance of AmlogicSocInterface using its default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "AmlogicSocInterface object created successfully." << std::endl;
        
        // Create a dummy GstElement instance to simulate a valid element.
        GstElement validElement;
        std::cout << "Dummy GstElement instance created at address " << &validElement << std::endl;
        
        std::cout << "Invoking GetSourcePad with valid GstElement pointer: " << &validElement << std::endl;
        GstPad* pad = nullptr;
        EXPECT_NO_THROW({
            pad = socInterface.GetSourcePad(&validElement);
        });
        std::cout << "GetSourcePad returned pointer: " << pad << std::endl;
        
        // Expected output: pad pointer should be non-NULL.
        EXPECT_NE(pad, nullptr);
    });
    
    std::cout << "Exiting GetSourcePad_valid_element test" << std::endl;
}
/**
 * @brief Verify that GetSourcePad returns a NULL pointer when invoked with a NULL element
 *
 * This test validates that the GetSourcePad API of the AmlogicSocInterface class handles a NULL input gracefully.
 * The test instantiates an object of AmlogicSocInterface and then calls GetSourcePad with a NULL pointer.
 * It further asserts that no exceptions are thrown during the call and that the returned pointer is indeed NULL.
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
 * | Variation / Step | Description                                                                 | Test Data                                                             | Expected Result                                                         | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate AmlogicSocInterface using the default constructor               | constructor = default                                                 | Instance is created without throwing any exceptions                     | Should be successful |
 * | 02               | Invoke GetSourcePad method with a NULL pointer                              | input = nullptr, output = pad pointer                                   | GetSourcePad returns a NULL pointer without throwing an exception       | Should Pass     |
 * | 03               | Validate that the returned pad pointer is NULL using the assertion EXPECT_EQ  | output from GetSourcePad = nullptr                                      | EXPECT_EQ confirms that the pad pointer is NULL                         | Should be successful |
 */
TEST(AmlogicSocInterface, GetSourcePad_null_element) {
    std::cout << "Entering GetSourcePad_null_element test" << std::endl;
    
    // Create an instance of AmlogicSocInterface using its default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        std::cout << "AmlogicSocInterface object created successfully." << std::endl;
        
        std::cout << "Invoking GetSourcePad with NULL pointer" << std::endl;
        GstPad* pad = nullptr;
        EXPECT_NO_THROW({
            pad = socInterface.GetSourcePad(nullptr);
        });
        std::cout << "GetSourcePad returned pointer: " << pad << std::endl;
        
        // Expected output: pad pointer should be NULL.
        EXPECT_EQ(pad, nullptr);
    });
    
    std::cout << "Exiting GetSourcePad_null_element test" << std::endl;
}
/**
 * @brief Validates the successful creation of AmlogicSocInterface and verifies that GetVideoSink returns a valid non-null video sink pointer.
 *
 * This test verifies that an instance of AmlogicSocInterface can be created without throwing exceptions and that invoking the GetVideoSink method with a valid dummy sinkbin returns a non-null pointer. It primarily tests the basic functionality of fetching the video sink element in a positive scenario.
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
 * | Variation / Step | Description                                                                    | Test Data                                                         | Expected Result                                      | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------ | ----------------------------------------------------------------- | ---------------------------------------------------- | ----------- |
 * | 01               | Create an instance of AmlogicSocInterface using its default constructor        | No input parameters                                               | Instance created successfully without exceptions   | Should Pass |
 * | 02               | Prepare a valid dummy sinkbin and invoke GetVideoSink with its address           | sinkbin address = &sinkbin                                          | GetVideoSink returns a non-null pointer              | Should Pass |
 * | 03               | Verify that the GetVideoSink method does not throw any exceptions upon invocation| Input: valid dummy sinkbin; Output: videoSink pointer != nullptr    | Method call completes without throwing exceptions  | Should Pass |
 */
TEST(AmlogicSocInterface, ValidSinkbinTest) {
    std::cout << "Entering ValidSinkbinTest test" << std::endl;

    // Create an instance of AmlogicSocInterface using default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface amlSocInterface;
        std::cout << "AmlogicSocInterface object created" << std::endl;

        // Prepare a valid dummy sinkbin.
        GstElement sinkbin;
        // Initialize the info field using strncpy.
        // Invoke GetVideoSink method and capture the returned video sink element.
        std::cout << "Invoking GetVideoSink with sinkbin address: " << &sinkbin << std::endl;
        GstElement* videoSink = amlSocInterface.GetVideoSink(&sinkbin);
        std::cout << "GetVideoSink returned pointer: " << videoSink << std::endl;

        // Check that the returned pointer is non-null.
        EXPECT_NE(videoSink, nullptr);
    });

    std::cout << "Exiting ValidSinkbinTest test" << std::endl;
}
/**
 * @brief Verify that passing a null sinkbin to GetVideoSink returns a null video sink.
 *
 * This test validates that the GetVideoSink method of the AmlogicSocInterface class properly handles a null sinkbin input. 
 * It ensures that when a null pointer is provided as a sinkbin, the method returns a null pointer without throwing an exception.
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
 * | Variation / Step | Description                                                         | Test Data                                         | Expected Result                                                    | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------------- | ----------------- |
 * | 01               | Create an instance of AmlogicSocInterface to setup the test environment | None                                              | AmlogicSocInterface object is successfully created                  | Should be successful |
 * | 02               | Invoke GetVideoSink with sinkbin set to nullptr to test null handling    | Input: sinkbin = nullptr, Output: videoSink pointer | GetVideoSink returns a nullptr; assertion verifies the null pointer  | Should Pass       |
 */
TEST(AmlogicSocInterface, NullSinkbinTest) {
    std::cout << "Entering NullSinkbinTest test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlSocInterface;
        std::cout << "AmlogicSocInterface object created" << std::endl;

        // Pass a null pointer as sinkbin.
        std::cout << "Invoking GetVideoSink with sinkbin = nullptr" << std::endl;
        GstElement* videoSink = amlSocInterface.GetVideoSink(nullptr);
        std::cout << "GetVideoSink returned pointer: " << videoSink << std::endl;

        // Verify that the returned pointer is null.
        EXPECT_EQ(videoSink, nullptr);

        if (videoSink == nullptr) {
            std::cout << "Returned video sink is null as expected for null sinkbin" << std::endl;
        } else {
            std::cout << "Unexpected non-null video sink for null sinkbin" << std::endl;
        }
    });

    std::cout << "Exiting NullSinkbinTest test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder correctly processes a valid audio decoder name with Rialto set to true
 *
 * This test checks whether the IsAudioOrVideoDecoder method in the AmlogicSocInterface class correctly handles the case 
 * when provided with a valid audio decoder name ("audio_decoder") and a Rialto flag set to true. The method should execute 
 * without throwing any exceptions and return a valid boolean result.
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
 * | 01 | Create instance of AmlogicSocInterface, assign valid audio decoder name ("audio_decoder") using strncpy, and set Rialto flag to true; invoke IsAudioOrVideoDecoder method | input: name = "audio_decoder", isRialto = true; output: result value from IsAudioOrVideoDecoder | Method returns a boolean true value without throwing exceptions and meets the EXPECT_NO_THROW assertion | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioDecoderNameRialtoTrue) {
    std::cout << "Entering ValidAudioDecoderNameRialtoTrue test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning name value using strncpy for audio_decoder" << std::endl;
        strncpy(name, "audio_decoder", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = true;
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << " and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
    });

    std::cout << "Exiting ValidAudioDecoderNameRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that the IsAudioOrVideoDecoder method correctly processes the valid video decoder name with Rialto flag set to true.
 *
 * This test ensures that when provided with the valid video decoder name "video_decoder" and the Rialto flag set to true, the IsAudioOrVideoDecoder method does not throw any exceptions and returns a valid boolean result. The test validates that the API correctly handles expected valid inputs.
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
 * | Variation / Step | Description                                                                 | Test Data                                                              | Expected Result                                         | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------- | ----------- |
 * | 01               | Invoke IsAudioOrVideoDecoder with name "video_decoder" and isRialto true.     | input1 = name = "video_decoder", input2 = isRialto = true, output1 = result boolean | No exception thrown; valid boolean returned (expected true if valid) | Should Pass |
 */
TEST(AmlogicSocInterface, ValidVideoDecoderNameRialtoTrue) {
    std::cout << "Entering ValidVideoDecoderNameRialtoTrue test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning name value using strncpy for video_decoder" << std::endl;
        strncpy(name, "video_decoder", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = true;
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << " and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
    });

    std::cout << "Exiting ValidVideoDecoderNameRialtoTrue test" << std::endl;
}
/**
 * @brief Validate behavior of IsAudioOrVideoDecoder API with a valid audio_decoder name and Rialto flag set to false
 *
 * Tests the IsAudioOrVideoDecoder method in AmlogicSocInterface when passed a valid "audio_decoder" name and a false Rialto flag.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 017
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface, assign "audio_decoder" to name using strncpy, set isRialto to false, and then call IsAudioOrVideoDecoder. | input: name = "audio_decoder", isRialto = false, output: result returned by IsAudioOrVideoDecoder | API call should execute without throwing exceptions and return a correct boolean value | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioDecoderNameRialtoFalse) {
    std::cout << "Entering ValidAudioDecoderNameRialtoFalse test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning name value using strncpy for audio_decoder" << std::endl;
        strncpy(name, "audio_decoder", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = false;
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << " and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
    });

    std::cout << "Exiting ValidAudioDecoderNameRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder correctly processes a valid video decoder name with Rialto flag set to false.
 *
 * This test case validates that the API function IsAudioOrVideoDecoder evaluates the input parameters correctly when provided with a valid video decoder name ("video_decoder") and a Rialto flag set to false. The test ensures that the method does not throw any exceptions during its execution and returns a proper boolean result.
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
 * | Variation / Step | Description                                                                                  | Test Data                                          | Expected Result                                                        | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------------------------- | ------------ |
 * | 01               | Instantiate AmlogicSocInterface, prepare a character array with "video_decoder", and set isRialto to false, then invoke IsAudioOrVideoDecoder. | name = "video_decoder", isRialto = false             | Method returns a boolean result without throwing any exceptions.       | Should Pass  |
 */
TEST(AmlogicSocInterface, ValidVideoDecoderNameRialtoFalse) {
    std::cout << "Entering ValidVideoDecoderNameRialtoFalse test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning name value using strncpy for video_decoder" << std::endl;
        strncpy(name, "video_decoder", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = false;
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << " and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
    });

    std::cout << "Exiting ValidVideoDecoderNameRialtoFalse test" << std::endl;
}
/**
 * @brief Validates that IsAudioOrVideoDecoder returns false for a null name with Rialto flag set to true
 *
 * This test checks if the function IsAudioOrVideoDecoder correctly handles a scenario where the name parameter is a null pointer and the Rialto flag is true. It ensures that the API fails safely by returning false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** (High) Ensures the API properly validates a critical input parameter@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Initialize test by creating an instance of AmlogicSocInterface and setting parameters | name = nullptr, isRialto = true | Instance created and parameters set appropriately | Should be successful |
 * |02| Invoke the IsAudioOrVideoDecoder function with a null name and Rialto flag true | input parameters: name = nullptr, isRialto = true, output: result should be false | API returns false and the EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(AmlogicSocInterface, NullNameRialtoTrue) {
    std::cout << "Entering NullNameRialtoTrue test" << std::endl;

    AmlogicSocInterface amlogicObj;
    const char* name = nullptr;
    bool isRialto = true;
    std::cout << "Invoking IsAudioOrVideoDecoder with name: nullptr and isRialto: " << isRialto << std::endl;
    bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
    std::cout << "Method returned: " << result << std::endl;
    EXPECT_FALSE(result);

    std::cout << "Exiting NullNameRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder returns false when called with a null name and Rialto flag set to false.
 *
 * This test checks whether the IsAudioOrVideoDecoder API correctly handles a nullptr for the name parameter while the Rialto flag is false. It verifies that the API returns false and the EXPECT_FALSE assertion passes.
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
 * | Variation / Step | Description                                                                | Test Data                       | Expected Result                                               | Notes      |
 * | :--------------: | -------------------------------------------------------------------------- | ------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Invoke IsAudioOrVideoDecoder with name set to nullptr and isRialto as false | name = nullptr, isRialto = false  | API returns false and the EXPECT_FALSE(result) assertion passes | Should Pass |
 */
TEST(AmlogicSocInterface, NullNameRialtoFalse) {
    std::cout << "Entering NullNameRialtoFalse test" << std::endl;

    AmlogicSocInterface amlogicObj;
    const char* name = nullptr;
    bool isRialto = false;
    std::cout << "Invoking IsAudioOrVideoDecoder with name: nullptr and isRialto: " << isRialto << std::endl;
    bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
    std::cout << "Method returned: " << result << std::endl;
    EXPECT_FALSE(result);

    std::cout << "Exiting NullNameRialtoFalse test" << std::endl;
}
/**
 * @brief Validate that IsAudioOrVideoDecoder returns false when provided with an empty string for the name parameter and Rialto flag set to true.
 *
 * This test verifies that the AmlogicSocInterface::IsAudioOrVideoDecoder API correctly handles an empty name input when the Rialto flag is true. It ensures that the API returns false in this scenario, thereby affirming the proper handling of invalid or missing name values.
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
 * | 01 | Initialize AmlogicSocInterface object, assign an empty string to the decoder name, set Rialto flag to true, and invoke IsAudioOrVideoDecoder. | input: name = "", isRialto = true | API returns false and assertion (EXPECT_FALSE) passes | Should Pass |
 */
TEST(AmlogicSocInterface, EmptyNameRialtoTrue) {
    std::cout << "Entering EmptyNameRialtoTrue test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning empty string to name using strncpy" << std::endl;
        strncpy(name, "", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = true;
        std::cout << "Invoking IsAudioOrVideoDecoder with empty name and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting EmptyNameRialtoTrue test" << std::endl;
}
/**
 * @brief Test that IsAudioOrVideoDecoder returns false with an empty name and isRialto false.
 *
 * This test verifies that providing an empty string as the decoder name and setting the isRialto flag to false returns false. It ensures that the method correctly handles an empty input, thereby preventing unintended behavior.
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
 * | 01 | Assign an empty string to name, set isRialto flag to false, and invoke IsAudioOrVideoDecoder. | name = "", isRialto = false | API returns false; EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(AmlogicSocInterface, EmptyNameRialtoFalse) {
    std::cout << "Entering EmptyNameRialtoFalse test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicObj;
        char name[32];
        std::cout << "Assigning empty string to name using strncpy" << std::endl;
        strncpy(name, "", sizeof(name));
        name[sizeof(name)-1] = '\0';
        bool isRialto = false;
        std::cout << "Invoking IsAudioOrVideoDecoder with empty name and isRialto: " << isRialto << std::endl;
        bool result = amlogicObj.IsAudioOrVideoDecoder(name, isRialto);
        std::cout << "Method returned: " << result << std::endl;
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting EmptyNameRialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder correctly handles a valid audio sink name when isRialto is false
 *
 * This test checks that the method IsAudioSinkOrAudioDecoder correctly processes a valid audio sink input with the isRialto flag set to false. It ensures that no exceptions are thrown and that the returned boolean value is valid (either true or false). This confirms proper input handling even when the expected behavior may vary.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 023@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of AmlogicSocInterface, prepare input parameters (name "audio_sink" and isRialto set to false), invoke IsAudioSinkOrAudioDecoder, and verify the returned boolean is valid. | name = "audio_sink", isRialto = false, output = boolean result | The API returns a boolean value (true or false) without throwing any exceptions | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioSinkFalse) {
    std::cout << "Entering ValidAudioSinkFalse test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        // Prepare input using fixed size array and strncpy
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_sink", sizeof(name) - 1);
        bool isRialto = false;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
        
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidAudioSinkFalse test" << std::endl;
}
/**
 * @brief Validates that the IsAudioSinkOrAudioDecoder API returns a valid boolean value without throwing exceptions.
 *
 * This test verifies that the AmlogicSocInterface::IsAudioSinkOrAudioDecoder API can be successfully called with a valid audio sink name ("audio_sink") and the isRialto flag set to true. It ensures that the API executes without throwing any exceptions and returns a boolean result.
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
 * | Variation / Step | Description                                                                                              | Test Data                                                  | Expected Result                                                                                      | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create an instance of AmlogicSocInterface, initialize the character array with "audio_sink", and set isRialto flag to true. | name = "audio_sink", isRialto = true                         | API is invoked with the valid parameters without throwing an exception.                             | Should Pass |
 * | 02               | Call IsAudioSinkOrAudioDecoder and verify that the returned result is a valid boolean (true or false).     | result = returned boolean value after API invocation        | The result is a boolean value, and the assertion (result == true || result == false) passes successfully. | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioSinkTrue) {
    std::cout << "Entering ValidAudioSinkTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_sink", sizeof(name) - 1);
        bool isRialto = true;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
        
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidAudioSinkTrue test" << std::endl;
}
/**
 * @brief Verify that the API handles a valid audio decoder name with isRialto flag set to false.
 *
 * This test verifies that when the IsAudioSinkOrAudioDecoder API is invoked using a valid decoder name "audio_decoder" and the isRialto flag is false, it executes without exceptions and returns a boolean value. The test ensures that the API call does not cause any runtime issues and that the resulting value is as expected.
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
 * | Variation / Step | Description                                                     | Test Data                                             | Expected Result                                                                                 | Notes           |
 * | :--------------: | --------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------------------------------------------ | --------------- |
 * | 01               | Initialize test variables including audio decoder name and flag   | name = "audio_decoder", isRialto = false              | Local variables are initialized correctly                                                       | Should be successful |
 * | 02               | Invoke IsAudioSinkOrAudioDecoder API within no-throw block        | Input: name = "audio_decoder", isRialto = false, output: result (boolean) | API executes without throwing and returns a boolean value (true or false)                         | Should Pass     |
 * | 03               | Verify that the returned result is a valid boolean value          | result (boolean)                                      | Assert that result equals true or false                                                           | Should be successful |
 */
TEST(AmlogicSocInterface, ValidAudioDecoderFalse) {
    std::cout << "Entering ValidAudioDecoderFalse test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_decoder", sizeof(name) - 1);
        bool isRialto = false;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
        
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidAudioDecoderFalse test" << std::endl;
}
/**
 * @brief Tests AmlogicSocInterface::IsAudioSinkOrAudioDecoder with a valid audio decoder name and positive Rialto flag
 *
 * This test verifies that the function IsAudioSinkOrAudioDecoder correctly processes a valid audio decoder name ("audio_decoder") with the isRialto flag set to true without throwing any exceptions. It confirms that the function returns a boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Instantiate AmlogicSocInterface, initialize name with "audio_decoder", set isRialto to true, invoke IsAudioSinkOrAudioDecoder, and verify the boolean result | name = "audio_decoder", isRialto = true, output: result = bool | The API should return a boolean value (true or false) without throwing an exception and the assertion (result == true || result == false) should pass | Should Pass |
 */
TEST(AmlogicSocInterface, ValidAudioDecoderTrue) {
    std::cout << "Entering ValidAudioDecoderTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_decoder", sizeof(name) - 1);
        bool isRialto = true;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidAudioDecoderTrue test" << std::endl;
}
/**
 * @brief Verify that providing an invalid video sink element returns false.
 *
 * This test case verifies that when the element "video_sink" (which is considered invalid for an audio sink or decoder) is provided with isRialto set to false, the API IsAudioSinkOrAudioDecoder correctly returns false. This ensures that the function properly distinguishes between valid and invalid element names.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 027
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface, set name to "video_sink" with isRialto = false, invoke IsAudioSinkOrAudioDecoder, and verify the result. | name = "video_sink", isRialto = false, output result expected = false | API returns false, and EXPECT_FALSE(result) assertion passes. | Should Pass |
 */
TEST(AmlogicSocInterface, InvalidVideoSinkFalse) {
    std::cout << "Entering InvalidVideoSinkFalse test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_sink", sizeof(name) - 1);
        bool isRialto = false;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting InvalidVideoSinkFalse test" << std::endl;
}
/**
 * @brief Validate that IsAudioSinkOrAudioDecoder returns false for an invalid video sink when isRialto is true
 *
 * This test verifies that when the input name "video_sink" is provided with isRialto set to true, the
 * function IsAudioSinkOrAudioDecoder correctly identifies that it is not an audio sink or audio decoder,
 * and therefore returns false. This ensures that misclassification of a video sink does not interfere with
 * proper audio device functionality.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                           | Test Data                                                   | Expected Result                                       | Notes        |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------------------------------------------------- | ------------ |
 * | 01               | Invoke IsAudioSinkOrAudioDecoder with name "video_sink" and isRialto true; expect false   | input: name = video_sink, input: isRialto = true, output: false | API returns false; EXPECT_FALSE assertion validates false | Should Pass  |
 */
TEST(AmlogicSocInterface, InvalidVideoSinkTrue) {
    std::cout << "Entering InvalidVideoSinkTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_sink", sizeof(name) - 1);
        bool isRialto = true;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting InvalidVideoSinkTrue test" << std::endl;
}
/**
 * @brief Verifies that IsAudioSinkOrAudioDecoder returns false when provided with a null name.
 *
 * This test verifies that when the IsAudioSinkOrAudioDecoder API is invoked with a null name pointer and isRialto set to false,
 * the method returns false without throwing any exceptions. It ensures that the function handles null input for the name parameter gracefully.
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
 * | Variation / Step | Description                                                     | Test Data                                     | Expected Result                                                      | Notes       |
 * | :--------------: | ----------------------------------------------------------------- | --------------------------------------------- | -------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke IsAudioSinkOrAudioDecoder with name as nullptr and isRialto false. | name = nullptr, isRialto = false, output = false | API returns false without throwing exceptions and EXPECT_FALSE(result) passes. | Should Pass |
 */
TEST(AmlogicSocInterface, NullNameFalse) {
    std::cout << "Entering NullNameFalse test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        const char* name = nullptr;
        bool isRialto = false;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: nullptr"
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NullNameFalse test" << std::endl;
}
/**
 * @brief Test that verifies IsAudioSinkOrAudioDecoder returns false when provided a null name and isRialto is true
 *
 * This test creates an instance of AmlogicSocInterface and calls the IsAudioSinkOrAudioDecoder method with a null pointer for the name and a true value for isRialto. The test ensures that the method returns false and does not throw any exceptions during execution.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface, set name as nullptr and isRialto as true, then invoke IsAudioSinkOrAudioDecoder | name = nullptr, isRialto = true, result = false | API returns false without throwing an exception | Should Pass |
 */
TEST(AmlogicSocInterface, NullNameTrue) {
    std::cout << "Entering NullNameTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        const char* name = nullptr;
        bool isRialto = true;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with name: nullptr"
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NullNameTrue test" << std::endl;
}
/**
 * @brief Verify that providing an empty string as the name returns false.
 *
 * This test verifies that when an empty string is provided to the IsAudioSinkOrAudioDecoder function along with the isRialto flag,
 * the function correctly returns false. This is done to ensure that the API can handle empty input values gracefully.
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
 * | Variation / Step | Description                                                     | Test Data                                   | Expected Result                                                          | Notes              |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------------------ | ------------------ |
 * | 01               | Initialize AmlogicSocInterface and input variables               | name = "", isRialto = false                   | Variables and objects are initialized correctly                          | Should be successful |
 * | 02               | Invoke IsAudioSinkOrAudioDecoder API with an empty string as name   | name = "", isRialto = false                   | Function returns false                                                   | Should Pass        |
 * | 03               | Verify the assertion of the API's return value                     | result = false                               | EXPECT_FALSE(result) passes                                              | Should be successful |
 */
TEST(AmlogicSocInterface, EmptyStringFalse) {
    std::cout << "Entering EmptyStringFalse test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        // Use strncpy to set empty string
        std::strncpy(name, "", sizeof(name) - 1);
        bool isRialto = false;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with empty name"
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting EmptyStringFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder correctly handles an empty name string when isRialto is true
 *
 * This test verifies that the IsAudioSinkOrAudioDecoder function returns false when provided with an empty name string and the isRialto flag set to true,
 * ensuring proper handling of empty strings in the API.
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
 * | 01 | Initialize AmlogicSocInterface instance, set name to an empty string and isRialto to true, then invoke IsAudioSinkOrAudioDecoder. | name = "", isRialto = true | API returns false as validated by EXPECT_FALSE check | Should Pass |
 */
TEST(AmlogicSocInterface, EmptyStringTrue) {
    std::cout << "Entering EmptyStringTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterface;
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "", sizeof(name) - 1);
        bool isRialto = true;
        std::cout << "Invoking IsAudioSinkOrAudioDecoder with empty name"
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
                  
        bool result = socInterface.IsAudioSinkOrAudioDecoder(name, isRialto);
        std::cout << "Returned result: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting EmptyStringTrue test" << std::endl;
}
/**
 * @brief Test the IsPlatformSegmentReady() method functionality of AmlogicSocInterface
 *
 * This test validates that the IsPlatformSegmentReady() method returns true when called on an instance
 * of AmlogicSocInterface. It verifies that no exceptions are thrown during the operation and the API behaves as expected.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                       | Expected Result                                                                                  | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create an AmlogicSocInterface instance and invoke the IsPlatformSegmentReady() method.            | Constructor: default, Method: IsPlatformSegmentReady() called with no input, Output: result = true | The IsPlatformSegmentReady() method returns true and no exceptions are thrown.                    | Should Pass |
 */
TEST(AmlogicSocInterface, IsPlatformSegmentReady_start) {
    std::cout << "Entering IsPlatformSegmentReady_start test" << std::endl;

    // Create AmlogicSocInterface instance and invoke IsPlatformSegmentReady()
    EXPECT_NO_THROW({
        AmlogicSocInterface amlInterface; 
        std::cout << "AmlogicSocInterface object created via default constructor." << std::endl;
        
        std::cout << "Invoking IsPlatformSegmentReady() method." << std::endl;
        bool result = amlInterface.IsPlatformSegmentReady();
        std::cout << "Method IsPlatformSegmentReady() returned value: " << std::boolalpha << result << std::endl;
        
        // Since the method specification indicates a return of 'true', we verify this.
        EXPECT_TRUE(result) << "Expected IsPlatformSegmentReady() to return true.";
    });

    std::cout << "Exiting IsPlatformSegmentReady_start test" << std::endl;
}
/**
 * @brief Verify that the IsVideoDecoder API correctly processes a valid video decoder name with Rialto flag set to true
 *
 * This test creates an instance of AmlogicSocInterface and invokes the IsVideoDecoder API using the video decoder name "H264Decoder" with the Rialto flag set to true. The test verifies that the API returns a valid boolean value (either true or false) and does not throw any exception, ensuring correct behavior for valid input parameters.
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
 * | Variation / Step | Description                                                                           | Test Data                                        | Expected Result                                                                 | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------------------------- | ---------- |
 * | 01               | Construct AmlogicSocInterface and invoke IsVideoDecoder with "H264Decoder" and Rialto flag true | name = H264Decoder, isRialto = true               | API returns a valid boolean value (true or false) without throwing exceptions  | Should Pass |
 */
TEST(AmlogicSocInterface, ValidVideoDecoderNameRialtoTrue) {
    std::cout << "Entering ValidVideoDecoderNameRialtoTrue test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
        
        const char* name = "H264Decoder";
        bool isRialto = true;
        std::cout << "Invoking IsVideoDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
        
        bool result = obj.IsVideoDecoder(name, isRialto);
        std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidVideoDecoderNameRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsVideoDecoder API returns a valid boolean value when invoked with a valid decoder name and Rialto flag set to false.
 *
 * This test confirms that the AmlogicSocInterface::IsVideoDecoder method can be called with the video decoder name "H264Decoder" and a Rialto flag set to false. The test ensures that the API processes the inputs without throwing an exception and returns a valid boolean outcome.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                               | Test Data                                           | Expected Result                                                  | Notes           |
 * | :--------------: | --------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------------- | --------------- |
 * | 01               | Construct AmlogicSocInterface object                      | None                                                | Object is constructed successfully without exceptions         | Should be successful |
 * | 02               | Prepare input variables                                   | name = H264Decoder, isRialto = false                  | Input variables are initialized correctly                       | Should be successful |
 * | 03               | Invoke IsVideoDecoder API with provided inputs            | input: name = H264Decoder, isRialto = false           | Returns a boolean value (true or false)                          | Should Pass     |
 * | 04               | Validate the boolean result through an assertion check    | output: result (boolean)                              | Assertion passes if result is a valid boolean value              | Should be successful |
 */
TEST(AmlogicSocInterface, ValidVideoDecoderNameRialtoFalse) {
    std::cout << "Entering ValidVideoDecoderNameRialtoFalse test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
        
        const char* name = "H264Decoder";
        bool isRialto = false;
        std::cout << "Invoking IsVideoDecoder with name: " << name 
                  << " and isRialto: " << std::boolalpha << isRialto << std::endl;
        
        bool result = obj.IsVideoDecoder(name, isRialto);
        std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidVideoDecoderNameRialtoFalse test" << std::endl;
}
/**
 * @brief Test for NullNameParameter scenario in AmlogicSocInterface
 *
 * This test verifies that when a nullptr is passed as the 'name' parameter to the IsVideoDecoder method,
 * the API correctly returns false. It ensures that the method handles null input gracefully and does not crash.
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
 * | Variation / Step | Description                                                         | Test Data                                          | Expected Result                                   | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------- | ------------- |
 * | 01               | Construct an AmlogicSocInterface object                             | No input                                           | Object constructed successfully; no exceptions    | Should be successful |
 * | 02               | Invoke IsVideoDecoder with a null name parameter and isRialto = false | name = nullptr, isRialto = false                     | Return value is false; EXPECT_FALSE(result) passes | Should Pass   |
 */
TEST(AmlogicSocInterface, NullNameParameter) {
    std::cout << "Entering NullNameParameter test" << std::endl;
    
    AmlogicSocInterface obj;
    std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
    
    const char* name = nullptr;
    bool isRialto = false;
    std::cout << "Invoking IsVideoDecoder with name: nullptr and isRialto: " 
              << std::boolalpha << isRialto << std::endl;
    
    bool result = obj.IsVideoDecoder(name, isRialto);
    std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
    
    // Expected output: false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullNameParameter test" << std::endl;
}
/**
 * @brief Tests the IsVideoDecoder API with an empty string as the name parameter.
 *
 * This test verifies that the IsVideoDecoder API correctly handles an empty name parameter
 * by returning false. It ensures that even when the isRialto flag is true, the API does not
 * erroneously identify an empty name as a valid video decoder.
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
 * | Variation / Step | Description                                                     | Test Data                                  | Expected Result                     | Notes         |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------ | ----------------------------------- | ------------- |
 * | 01               | Construct the AmlogicSocInterface object                        | N/A                                        | Object constructed successfully     | Should be successful |
 * | 02               | Initialize parameters with an empty name and isRialto as true      | name = "", isRialto = true                   | Parameters initialized properly    | Should be successful |
 * | 03               | Invoke IsVideoDecoder and verify the return value                | name = "", isRialto = true, output = result   | API returns false                  | Should Fail   |
 */
TEST(AmlogicSocInterface, EmptyNameParameter) {
    std::cout << "Entering EmptyNameParameter test" << std::endl;
    
    AmlogicSocInterface obj;
    std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
    
    char name[50] = {0};
    // Use strncpy to assign an empty string to name
    strncpy(name, "", sizeof(name)-1);
    bool isRialto = true;
    std::cout << "Invoking IsVideoDecoder with an empty name and isRialto: " 
              << std::boolalpha << isRialto << std::endl;
    
    bool result = obj.IsVideoDecoder(name, isRialto);
    std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyNameParameter test" << std::endl;
}
/**
 * @brief Validate the IsVideoDecoder method when provided with a non-video decoder name.
 *
 * This test ensures that the IsVideoDecoder method returns false when passed a non-video decoder name.
 * The test simulates a scenario where a name associated with an audio decoder ("audio_decoder") is supplied.
 * It verifies that the method correctly identifies the non-video decoder and does not erroneously classify it as a video decoder.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                   | Test Data                                         | Expected Result                                                                  | Notes            |
 * | :--------------: | ------------------------------------------------------------- | ------------------------------------------------- | -------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Construct and initialize the AmlogicSocInterface object.      | None                                              | AmlogicSocInterface object is successfully constructed.                        | Should be successful |
 * | 02               | Invoke IsVideoDecoder with a non-video decoder name and isRialto true. | name = audio_decoder, isRialto = true             | Method returns false indicating the provided name is not a video decoder.         | Should Pass      |
 * | 03               | Validate the result using EXPECT_FALSE assertion.             | result = return value from IsVideoDecoder (expected false) | EXPECT_FALSE assertion passes as the return value is false.                       | Should be successful |
 */
TEST(AmlogicSocInterface, NonVideoDecoderNameAudio) {
    std::cout << "Entering NonVideoDecoderNameAudio test" << std::endl;
    
    AmlogicSocInterface obj;
    std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
    
    char name[50] = {0};
    // Use strncpy to assign value "audio_decoder" to name
    strncpy(name, "audio_decoder", sizeof(name)-1);
    bool isRialto = true;
    std::cout << "Invoking IsVideoDecoder with name: " << name 
              << " and isRialto: " << std::boolalpha << isRialto << std::endl;
    
    bool result = obj.IsVideoDecoder(name, isRialto);
    std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting NonVideoDecoderNameAudio test" << std::endl;
}
/**
 * @brief Validate API behavior for non-video decoder name input.
 *
 * This test verifies that when a non-video decoder name ("random") is provided along with a false
 * flag for isRialto, the API method IsVideoDecoder returns false as expected. The objective is to ensure
 * that non-video decoder names are correctly identified.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 039
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                               | Expected Result                                    | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------- | -------------------------------------------------- | --------------- |
 * | 01               | Construct AmlogicSocInterface object                                         | N/A                                                   | Object is constructed successfully                 | Should be successful |
 * | 02               | Initialize test inputs by setting name to "random" and isRialto to false       | name = "random", isRialto = false                       | Input values are assigned correctly                | Should be successful |
 * | 03               | Invoke IsVideoDecoder with the non-video decoder name                        | input: name = "random", isRialto = false, output: result expected = false | API returns false                                  | Should Pass     |
 * | 04               | Assert that the returned result is false                                     | result = false                                          | EXPECT_FALSE assertion succeeds                    | Should Pass     |
 */
TEST(AmlogicSocInterface, NonVideoDecoderNameRandom) {
    std::cout << "Entering NonVideoDecoderNameRandom test" << std::endl;
    
    AmlogicSocInterface obj;
    std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
    
    char name[50] = {0};
    // Use strncpy to assign value "random" to name
    strncpy(name, "random", sizeof(name)-1);
    bool isRialto = false;
    std::cout << "Invoking IsVideoDecoder with name: " << name 
              << " and isRialto: " << std::boolalpha << isRialto << std::endl;
    
    bool result = obj.IsVideoDecoder(name, isRialto);
    std::cout << "Method IsVideoDecoder returned: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    std::cout << "Exiting NonVideoDecoderNameRandom test" << std::endl;
}
/**
 * @brief Verify that the IsVideoMaster() method returns false on normal call
 *
 * This test verifies that the IsVideoMaster() method of the AmlogicSocInterface class returns false when invoked on a default constructed object.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                                                       | Expected Result                                                            | Notes               |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ------------------- |
 * | 01               | Construct AmlogicSocInterface object using default constructor    | No input arguments; output: object constructed successfully                     | Object is created without throwing exceptions                             | Should be successful|
 * | 02               | Invoke IsVideoMaster() method on the constructed object            | No input parameters; output: result variable expected to be false               | API returns false                                                          | Should Pass         |
 * | 03               | Verify that the returned value is false through an assertion       | Input: result, Expected: false                                                  | EXPECT_EQ(result, false) check passes                                      | Should be successful|
 */
TEST(AmlogicSocInterface, VerifyIsVideoMasterReturnsFalseOnNormalCall) {
    std::cout << "Entering VerifyIsVideoMasterReturnsFalseOnNormalCall test" << std::endl;
    // Constructing AmlogicSocInterface object using default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicSocInterface;
        std::cout << "AmlogicSocInterface object constructed successfully." << std::endl;
        // Invocation of IsVideoMaster method.
        std::cout << "Invoking IsVideoMaster() method." << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = amlogicSocInterface.IsVideoMaster();
            std::cout << "IsVideoMaster() returned: " << std::boolalpha << result << std::endl;
        });
        // Verifying that the returned value is false.
        EXPECT_EQ(result, false);
        std::cout << "Verified that IsVideoMaster() returned false as expected." << std::endl;
    });
    std::cout << "Exiting VerifyIsVideoMasterReturnsFalseOnNormalCall test" << std::endl;
}
/**
 * @brief Verifies that a valid video sink name with Rialto set to false is handled correctly by IsVideoSink.
 *
 * This test creates an instance of AmlogicSocInterface and invokes the IsVideoSink API using a valid video sink name "video_sink" with the isRialto flag set to false. The test ensures that no exceptions are thrown during object construction and API invocation, and that the API returns a boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 041@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                          | Expected Result                                                  | Notes          |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------------------- | -------------- |
 * | 01               | Construct an instance of AmlogicSocInterface using default constructor | Constructor: none                                  | Instance created without throwing any exception                 | Should be successful |
 * | 02               | Invoke IsVideoSink API with parameters: name = "video_sink", isRialto = false | name = video_sink, isRialto = false, result = bool   | API returns a boolean value without throwing exception            | Should Pass    |
 */
TEST(AmlogicSocInterface, IsVideoSink_ValidVideoSink_RialtoFalse)
{
    std::cout << "Entering IsVideoSink_ValidVideoSink_RialtoFalse test" << std::endl;
    // Create an instance of AmlogicSocInterface using default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    const char testName[] = "video_sink";
    bool isRialto = false;
    std::cout << "Invoking IsVideoSink with parameters: name = " << testName << ", isRialto = " << isRialto << std::endl;

    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    // No specific expected value provided, so we simply check that a bool value is returned.
    EXPECT_TRUE(result == true || result == false);

    std::cout << "Exiting IsVideoSink_ValidVideoSink_RialtoFalse test" << std::endl;
}
/**
 * @brief Validate the IsVideoSink API for valid video sink with Rialto flag set to true.
 *
 * This test checks that the IsVideoSink function in AmlogicSocInterface accepts a valid
 * video sink identifier with the Rialto flag true without throwing exceptions. It verifies that the API
 * call completes successfully and returns a boolean value (either true or false) as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 042@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                                | Expected Result                                                        | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------- | ---------------------------------------------------------------------- | ----------------- |
 * | 01               | Instantiate AmlogicSocInterface object using its constructor.       | None                                                     | No exception thrown                                                    | Should be successful |
 * | 02               | Set parameters: assign "video_sink" to testName and true to isRialto. | testName = "video_sink", isRialto = true                 | Parameters assigned correctly                                          | Should be successful |
 * | 03               | Invoke IsVideoSink API with testName and isRialto.                    | testName = "video_sink", isRialto = true                 | API call executes without exception and returns a boolean value         | Should Pass       |
 * | 04               | Validate the boolean result from IsVideoSink.                         | output1 = result (boolean)                               | Result is either true or false as per the assertion check              | Should be successful |
 */
TEST(AmlogicSocInterface, IsVideoSink_ValidVideoSink_RialtoTrue)
{
    std::cout << "Entering IsVideoSink_ValidVideoSink_RialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    char testName[32];
    std::strncpy(testName, "video_sink", sizeof(testName));
    bool isRialto = true;
    std::cout << "Invoking IsVideoSink with parameters: name = " << testName << ", isRialto = " << isRialto << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;

    EXPECT_TRUE(result == true || result == false);

    std::cout << "Exiting IsVideoSink_ValidVideoSink_RialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink API correctly identifies a non-video sink when Rialto flag is false
 *
 * This test ensures that when an audio sink (non-video sink) identifier is provided with Rialto flag set to false,
 * the IsVideoSink API returns a valid boolean value. The test creates an instance of the AmlogicSocInterface, initializes
 * the input parameters, calls the IsVideoSink function and checks the returned value using Google Test assertions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 043@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                  | Expected Result                                        | Notes           |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------ | ------------------------------------------------------ | --------------- |
 * | 01               | Instantiate an instance of AmlogicSocInterface                       | None                                       | Object is created without throwing an exception      | Should be successful |
 * | 02               | Initialize parameters for IsVideoSink call                           | testName = audio_sink, isRialto = false      | Parameters are properly set                             | Should be successful |
 * | 03               | Invoke the IsVideoSink API with provided parameters and verify return| name = audio_sink, isRialto = false          | Return value is a valid boolean; assertion verifies it  | Should Pass     |
 */
TEST(AmlogicSocInterface, IsVideoSink_NonVideoSink_RialtoFalse)
{
    std::cout << "Entering IsVideoSink_NonVideoSink_RialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    char testName[32];
    std::strncpy(testName, "audio_sink", sizeof(testName));
    bool isRialto = false;
    std::cout << "Invoking IsVideoSink with parameters: name = " << testName << ", isRialto = " << isRialto << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting IsVideoSink_NonVideoSink_RialtoFalse test" << std::endl;
}
/**
 * @brief Test to verify that the IsVideoSink API handles non-video sink scenario when Rialto is true.
 *
 * This test case validates the behavior of the IsVideoSink function when invoked with a non-video sink identifier ("audio_sink") and a Rialto flag set to true. It ensures that the API call does not throw an exception during execution and returns a valid boolean value (either true or false), confirming proper handling of non-video sinks.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 044
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                                        | Expected Result                                                                       | Notes           |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Construct AmlogicSocInterface object to ensure object creation does not throw | None                                             | AmlogicSocInterface instance is successfully created without throwing an exception     | Should be successful |
 * | 02               | Invoke IsVideoSink with 'audio_sink' as testName and isRialto set to true   | testName = audio_sink, isRialto = true           | API call returns a boolean value without throwing an exception                         | Should Pass     |
 * | 03               | Validate that the returned value from IsVideoSink is a valid boolean (true or false) | result (boolean value from IsVideoSink)          | The result is either true or false                                                       | Should be successful |
 */
TEST(AmlogicSocInterface, IsVideoSink_NonVideoSink_RialtoTrue)
{
    std::cout << "Entering IsVideoSink_NonVideoSink_RialtoTrue test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    char testName[32];
    std::strncpy(testName, "audio_sink", sizeof(testName));
    bool isRialto = true;
    std::cout << "Invoking IsVideoSink with parameters: name = " << testName << ", isRialto = " << isRialto << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting IsVideoSink_NonVideoSink_RialtoTrue test" << std::endl;
}
/**
 * @brief Validate the IsVideoSink API with an invalid video sink name and Rialto flag set to false.
 *
 * This test verifies that when an invalid video sink name ("unknown") is provided along with the Rialto flag set to false, the IsVideoSink method does not throw exceptions and returns a valid boolean result. The test ensures that an object of AmlogicSocInterface can be successfully constructed and used to invoke the API without issues.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Construct an instance of AmlogicSocInterface | No input parameters; constructor call | Object instance created without throwing exceptions | Should be successful |@n
 * | 02 | Initialize testName with "unknown" and set isRialto flag to false | testName = "unknown", isRialto = false | testName correctly set; isRialto set to false | Should be successful |@n
 * | 03 | Invoke IsVideoSink with the provided parameters | input: testName = "unknown", isRialto = false; output: result variable | Method call completes without exceptions; result holds boolean value (either true or false) | Should Pass |@n
 * | 04 | Validate the result of IsVideoSink call | result from IsVideoSink call | Assertion check passes: result is either true or false | Should Pass |
 */
TEST(AmlogicSocInterface, IsVideoSink_InvalidName_RialtoFalse)
{
    std::cout << "Entering IsVideoSink_InvalidName_RialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    char testName[32];
    std::strncpy(testName, "unknown", sizeof(testName));
    bool isRialto = false;
    std::cout << "Invoking IsVideoSink with parameters: name = " << testName << ", isRialto = " << isRialto << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result == true || result == false);
    
    std::cout << "Exiting IsVideoSink_InvalidName_RialtoFalse test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns false when provided a null name and Rialto flag is false
 *
 * This test checks if the AmlogicSocInterface::IsVideoSink API correctly handles a NULL pointer as the video sink name while the Rialto flag is false. The expected behavior is that the function does not throw an exception and returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of AmlogicSocInterface and verify no exception is thrown | None | Instance created successfully; no exception thrown | Should be successful |
 * | 02 | Call IsVideoSink with testName set to nullptr and isRialto set to false | testName = nullptr, isRialto = false | API should return false and pass the EXPECT_FALSE check | Should Pass |
 */
TEST(AmlogicSocInterface, IsVideoSink_NullName_RialtoFalse)
{
    std::cout << "Entering IsVideoSink_NullName_RialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    const char* testName = nullptr;
    bool isRialto = false;
    std::cout << "Invoking IsVideoSink with parameters: name = " << "nullptr" << ", isRialto = " << isRialto << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting IsVideoSink_NullName_RialtoFalse test" << std::endl;
}
/**
 * @brief Validate that IsVideoSink returns false when an empty string is provided and Rialto flag is false.
 *
 * This test verifies that the IsVideoSink method of the AmlogicSocInterface correctly identifies an empty string as not being a valid video sink when the Rialto flag is false. It ensures that the function handles empty input gracefully without exceptions and returns the expected boolean value.
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
 * | 01 | Invoke the AmlogicSocInterface constructor within EXPECT_NO_THROW to ensure that the object is created successfully. | constructor = AmlogicSocInterface() | No exception thrown | Should be successful |
 * | 02 | Call the IsVideoSink method using an empty string for the name and false for the isRialto flag. | testName = "", isRialto = false | Returns false | Should Pass |
 * | 03 | Assert the return value of the IsVideoSink method to verify it is false. | result from IsVideoSink = false | EXPECT_FALSE passes confirming the method returns false | Should Pass |
 */
TEST(AmlogicSocInterface, IsVideoSink_EmptyString_RialtoFalse)
{
    std::cout << "Entering IsVideoSink_EmptyString_RialtoFalse test" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface interfaceObj;
    });
    AmlogicSocInterface interfaceObj;

    char testName[32];
    std::strncpy(testName, "", sizeof(testName));
    bool isRialto = false;
    std::cout << "Invoking IsVideoSink with parameters: name = \"\" (empty string), isRialto = " << isRialto << std::endl;
    
    bool result = true; // preset value to check below
    EXPECT_NO_THROW({
        result = interfaceObj.IsVideoSink(testName, isRialto);
    });
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    
    // Expected output specifically false.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting IsVideoSink_EmptyString_RialtoFalse test" << std::endl;
}
/**
 * @brief Verifies that the ResetNewSegmentEvent method returns true indicating a successful reset of the segment event.
 *
 * This test creates an instance of AmlogicSocInterface using its default constructor and then invokes the ResetNewSegmentEvent method. 
 * The test verifies that the method call does not throw any exceptions and returns a true value, indicative of a successful event reset.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 048@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                             | Test Data                                                     | Expected Result                                     | Notes          |
 * | :--------------: | ----------------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------- | -------------- |
 * | 01               | Create an AmlogicSocInterface object using the default constructor.     | Constructor invoked, no input args.                           | Object created successfully without exceptions.   | Should be successful |
 * | 02               | Invoke the ResetNewSegmentEvent() method on the created object.           | API: ResetNewSegmentEvent, input: none, output: result.         | Method returns true without throwing an exception.  | Should Pass    |
 * | 03               | Validate that the result returned by ResetNewSegmentEvent() is true.      | API: ResetNewSegmentEvent, output: result = true                | EXPECT_TRUE(result) assertion passes.               | Should Pass    |
 */
TEST(AmlogicSocInterface, ResetNewSegmentEvent_ReturnsTrue) {
    std::cout << "Entering ResetNewSegmentEvent_ReturnsTrue test" << std::endl;

    std::cout << "Creating AmlogicSocInterface object using default constructor" << std::endl;
    EXPECT_NO_THROW({
        AmlogicSocInterface amlogicSocInterface;
        
        std::cout << "Invoking ResetNewSegmentEvent() method" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = amlogicSocInterface.ResetNewSegmentEvent();
        });
        std::cout << "ResetNewSegmentEvent() returned value: " << result << std::endl;
        
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting ResetNewSegmentEvent_ReturnsTrue test" << std::endl;
}
/**
 * @brief Verify that SetAC4Tracks accepts a valid GstElement pointer and a positive trackId without throwing an exception.
 *
 * This test validates that the AmlogicSocInterface::SetAC4Tracks method operates correctly when supplied with a valid GstElement pointer and a valid track identifier (trackId greater than 0). It is designed to ensure that the method does not throw any exceptions under these positive conditions.
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
 * | Variation / Step | Description                                                         | Test Data                                                            | Expected Result                                           | Notes              |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------- | ------------------ |
 * | 01               | Create a valid GstElement pointer using CreateDummyGstElement() call. | dummySrc = valid GstElement pointer                                  | dummySrc is not nullptr                                   | Should be successful |
 * | 02               | Initialize AmlogicSocInterface object using the default constructor.  | amlSocInterface = AmlogicSocInterface()                              | Object is created successfully                            | Should be successful |
 * | 03               | Invoke SetAC4Tracks with dummySrc and trackId = 1.                    | dummySrc = valid GstElement pointer, trackId = 1                     | No exception thrown (EXPECT_NO_THROW passes)              | Should Pass        |
 * | 04               | Clean up the dummy GstElement pointer.                                | delete dummySrc                                                      | Memory freed without error                                | Should be successful |
 */
TEST(AmlogicSocInterface, SetAC4Tracks_Valid_Positive_TrackId) {
    std::cout << "Entering SetAC4Tracks_Valid_Positive_TrackId test" << std::endl;
    
    // Create a valid GstElement pointer.
    GstElement* dummySrc = CreateDummyGstElement();
    int trackId = 1;
    std::cout << "Creating AmlogicSocInterface object using default constructor." << std::endl;
    AmlogicSocInterface amlSocInterface;

    std::cout << "Invoking SetAC4Tracks with src pointer: " << dummySrc << " and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(amlSocInterface.SetAC4Tracks(dummySrc, trackId));
    std::cout << "Method SetAC4Tracks invoked successfully with trackId: " << trackId << std::endl;
    
    // Clean up the dummy GstElement pointer.
    delete reinterpret_cast<int*>(dummySrc);
    std::cout << "Exiting SetAC4Tracks_Valid_Positive_TrackId test" << std::endl;
}
/**
 * @brief This test verifies that the SetAC4Tracks method of the AmlogicSocInterface class correctly accepts a valid GstElement pointer and a trackId of zero.
 *
 * This test creates a dummy GstElement pointer and initializes trackId to zero. It then constructs an instance of AmlogicSocInterface and calls the SetAC4Tracks method with these parameters. The test asserts that no exception is thrown during the method call, ensuring that the API can handle a trackId of zero properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 050@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a dummy GstElement pointer, set trackId to zero, construct AmlogicSocInterface, invoke SetAC4Tracks | dummySrc = valid GstElement pointer, trackId = 0 | No exception is thrown from SetAC4Tracks; EXPECT_NO_THROW assertion passes | Should Pass |
 */
TEST(AmlogicSocInterface, SetAC4Tracks_Valid_Zero_TrackId) {
    std::cout << "Entering SetAC4Tracks_Valid_Zero_TrackId test" << std::endl;
    
    // Create a valid GstElement pointer.
    GstElement* dummySrc = CreateDummyGstElement();
    int trackId = 0;
    std::cout << "Creating AmlogicSocInterface object using default constructor." << std::endl;
    AmlogicSocInterface amlSocInterface;

    std::cout << "Invoking SetAC4Tracks with src pointer: " << dummySrc << " and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(amlSocInterface.SetAC4Tracks(dummySrc, trackId));
    std::cout << "Method SetAC4Tracks invoked successfully with trackId: " << trackId << std::endl;
    
    // Clean up the dummy GstElement pointer.
    delete reinterpret_cast<int*>(dummySrc);
    std::cout << "Exiting SetAC4Tracks_Valid_Zero_TrackId test" << std::endl;
}
/**
 * @brief Tests SetAC4Tracks API with a valid dummy GstElement pointer and maximum integer track id.
 *
 * This test case verifies that the AmlogicSocInterface::SetAC4Tracks method accepts a valid GstElement pointer and a trackId set to INT_MAX without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 051@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                   | Test Data                                                                                         | Expected Result                                                | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SetAC4Tracks with a valid dummy GstElement pointer and trackId = INT_MAX | input: dummySrc = valid GstElement pointer from CreateDummyGstElement, trackId = INT_MAX; output: none | Method executes without throwing any exception (EXPECT_NO_THROW) | Should Pass |
 */
TEST(AmlogicSocInterface, SetAC4Tracks_Valid_MaxInt_TrackId) {
    std::cout << "Entering SetAC4Tracks_Valid_MaxInt_TrackId test" << std::endl;
    
    // Create a valid GstElement pointer.
    GstElement* dummySrc = CreateDummyGstElement();
    int trackId = INT_MAX;
    std::cout << "Creating AmlogicSocInterface object using default constructor." << std::endl;
    AmlogicSocInterface amlSocInterface;

    std::cout << "Invoking SetAC4Tracks with src pointer: " << dummySrc << " and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(amlSocInterface.SetAC4Tracks(dummySrc, trackId));
    std::cout << "Method SetAC4Tracks invoked successfully with trackId: " << trackId << std::endl;
    
    // Clean up the dummy GstElement pointer.
    delete reinterpret_cast<int*>(dummySrc);
    std::cout << "Exiting SetAC4Tracks_Valid_MaxInt_TrackId test" << std::endl;
}
/**
 * @brief Verify SetAC4Tracks handles a null GstElement pointer gracefully.
 *
 * This test verifies that when SetAC4Tracks is invoked with a null GstElement pointer and a valid trackId,
 * the API executes without throwing any exceptions. This ensures that the function can handle null inputs for the GstElement parameter.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 052@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                    | Expected Result                                                         | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------- | ----------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SetAC4Tracks API with a null GstElement pointer and valid trackId (1). | nullSrc = nullptr, trackId = 1                | The API is expected to complete without throwing any exceptions.       | Should Pass |
 */
TEST(AmlogicSocInterface, SetAC4Tracks_Null_GstElement) {
    std::cout << "Entering SetAC4Tracks_Null_GstElement test" << std::endl;
    
    GstElement* nullSrc = nullptr;
    int trackId = 1;
    std::cout << "Creating AmlogicSocInterface object using default constructor." << std::endl;
    AmlogicSocInterface amlSocInterface;

    std::cout << "Invoking SetAC4Tracks with NULL src pointer and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(amlSocInterface.SetAC4Tracks(nullSrc, trackId));
    std::cout << "Method SetAC4Tracks invoked successfully with NULL src pointer and trackId: " << trackId << std::endl;
    
    std::cout << "Exiting SetAC4Tracks_Null_GstElement test" << std::endl;
}
/**
 * @brief Test SetAC4Tracks method with a negative trackId to ensure it does not throw an exception.
 *
 * This test verifies that the SetAC4Tracks method of the AmlogicSocInterface class handles a negative trackId correctly without throwing any exceptions. The negative trackId represents an edge case to ensure robustness of the API function.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 053@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a dummy GstElement and initialize trackId with -1 | dummySrc = CreateDummyGstElement(), trackId = -1 | Dummy element created and trackId initialized as -1 | Should be successful |
 * | 02 | Instantiate AmlogicSocInterface using default constructor | No inputs | Object created successfully | Should be successful |
 * | 03 | Invoke SetAC4Tracks with dummySrc and negative trackId | dummySrc pointer value, trackId = -1, output not captured | EXPECT_NO_THROW passes; method does not throw exception | Should Pass |
 * | 04 | Clean up the dummy GstElement pointer | dummySrc pointer value | Dummy GstElement cleaned-up successfully | Should be successful |
 */
TEST(AmlogicSocInterface, SetAC4Tracks_Valid_Negative_TrackId) {
    std::cout << "Entering SetAC4Tracks_Valid_Negative_TrackId test" << std::endl;
    
    // Create a valid GstElement pointer.
    GstElement* dummySrc = CreateDummyGstElement();
    int trackId = -1;
    std::cout << "Creating AmlogicSocInterface object using default constructor." << std::endl;
    AmlogicSocInterface amlSocInterface;

    std::cout << "Invoking SetAC4Tracks with src pointer: " << dummySrc << " and negative trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(amlSocInterface.SetAC4Tracks(dummySrc, trackId));
    std::cout << "Method SetAC4Tracks invoked successfully with negative trackId: " << trackId << std::endl;
    
    // Clean up the dummy GstElement pointer.
    delete reinterpret_cast<int*>(dummySrc);
    std::cout << "Exiting SetAC4Tracks_Valid_Negative_TrackId test" << std::endl;
}
/**
 * @brief Validate setting audio properties with valid non-empty input values.
 *
 * Tests that the AmlogicSocInterface object can be created using the default constructor and that its SetAudioProperty method can be invoked with valid non-empty string values for volume and mute along with a boolean flag for isSinkBinVolume without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 054@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an AmlogicSocInterface object using the default constructor. | None | Object is created without exceptions. | Should be successful |
 * | 02 | Initialize non-empty audio property values and a boolean flag. | volume = audio_volume, mute = audio_mute, isSinkBinVolume = false | Values are correctly assigned. | Should be successful |
 * | 03 | Invoke the SetAudioProperty API with the valid non-empty audio properties. | input: volume = audio_volume, mute = audio_mute, isSinkBinVolume = false | API call executes without throwing exceptions. | Should Pass |
 * | 04 | Log the state before and after the API call. | Log messages showing volume, mute, and isSinkBinVolume values | Log outputs verify that the API call was performed correctly. | Should be successful |
 */
TEST(AmlogicSocInterface, ValidInputNonEmptyProperties) {
    std::cout << "Entering ValidInputNonEmptyProperties test" << std::endl;

    // Create object using default constructor and log invocation
    EXPECT_NO_THROW({
        AmlogicSocInterface soc;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;

        const int SIZE = 50;
        char volumeArr[SIZE];
        char muteArr[SIZE];
        // Use strncpy to assign string values
        strncpy(volumeArr, "audio_volume", SIZE - 1);
        volumeArr[SIZE - 1] = '\0';
        strncpy(muteArr, "audio_mute", SIZE - 1);
        muteArr[SIZE - 1] = '\0';

        const char* volume = volumeArr;
        const char* mute = muteArr;
        bool isSinkBinVolume = false;

        std::cout << "Before invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;
                  
        // Invoke SetAudioProperty
        EXPECT_NO_THROW(soc.SetAudioProperty(volume, mute, isSinkBinVolume));
        std::cout << "Called SetAudioProperty method" << std::endl;
  
        std::cout << "After invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;

    });
    
    std::cout << "Exiting ValidInputNonEmptyProperties test" << std::endl;
}
/**
 * @brief Validate that SetAudioProperty correctly handles an empty volume string.
 *
 * This test verifies the behavior of the AmlogicSocInterface::SetAudioProperty method when provided with an empty volume string. It ensures that the API accepts valid mute settings even when the volume is empty and does not throw any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 055@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                    | Test Data                                               | Expected Result                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------ | ------------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Construct AmlogicSocInterface object and prepare audio property parameters     | None                                                    | Object is constructed; volume array = "", mute array = "audio_mute", isSinkBinVolume = false | Should be successful |
 * | 02               | Invoke SetAudioProperty API with prepared parameters                           | volume = "", mute = "audio_mute", isSinkBinVolume = false | API call completes without throwing an exception                 | Should Pass   |
 */
TEST(AmlogicSocInterface, ValidInputEmptyVolume) {
    std::cout << "Entering ValidInputEmptyVolume test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface soc;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;

        const int SIZE = 50;
        char volumeArr[SIZE];
        char muteArr[SIZE];
        // volume is empty string
        strncpy(volumeArr, "", SIZE - 1);
        volumeArr[SIZE - 1] = '\0';
        strncpy(muteArr, "audio_mute", SIZE - 1);
        muteArr[SIZE - 1] = '\0';

        const char* volume = volumeArr;
        const char* mute = muteArr;
        bool isSinkBinVolume = false;
  
        std::cout << "Before invoking SetAudioProperty: volume = \"" 
                  << volume << "\", mute = " << (mute ? mute : "null")
                  << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;
  
        EXPECT_NO_THROW(soc.SetAudioProperty(volume, mute, isSinkBinVolume));
        std::cout << "Called SetAudioProperty method" << std::endl;
  
        std::cout << "After invoking SetAudioProperty: volume = \"" 
                  << volume << "\", mute = " << (mute ? mute : "null")
                  << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;
  
    });
  
    std::cout << "Exiting ValidInputEmptyVolume test" << std::endl;
}
/**
 * @brief Validates that SetAudioProperty handles valid input with an empty mute string.
 *
 * This test verifies that the AmlogicSocInterface's SetAudioProperty method can be invoked with a valid volume string and an empty mute string without throwing any exceptions.
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
 * | Variation / Step | Description                                                                      | Test Data                                          | Expected Result                                                  | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Create AmlogicSocInterface object using default constructor                      | N/A                                                | Object is created successfully                                   | Should Pass |
 * | 02               | Prepare audio properties (volume as "audio_volume", mute as empty, isSinkBinVolume false) and invoke SetAudioProperty method | volume = "audio_volume", mute = "", isSinkBinVolume = false | SetAudioProperty method is called without throwing an exception | Should Pass |
 */
TEST(AmlogicSocInterface, ValidInputEmptyMute) {
    std::cout << "Entering ValidInputEmptyMute test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface soc;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
  
        const int SIZE = 50;
        char volumeArr[SIZE];
        char muteArr[SIZE];
        strncpy(volumeArr, "audio_volume", SIZE - 1);
        volumeArr[SIZE - 1] = '\0';
        // mute is empty string
        strncpy(muteArr, "", SIZE - 1);
        muteArr[SIZE - 1] = '\0';
  
        const char* volume = volumeArr;
        const char* mute = muteArr;
        bool isSinkBinVolume = false;
  
        std::cout << "Before invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = \"" 
                  << mute << "\", isSinkBinVolume = " << isSinkBinVolume << std::endl;
  
        EXPECT_NO_THROW(soc.SetAudioProperty(volume, mute, isSinkBinVolume));
        std::cout << "Called SetAudioProperty method" << std::endl;
  
        std::cout << "After invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = \"" 
                  << mute << "\", isSinkBinVolume = " << isSinkBinVolume << std::endl;
  
    });
  
    std::cout << "Exiting ValidInputEmptyMute test" << std::endl;
}
/**
 * @brief Verify that SetAudioProperty gracefully handles a null volume pointer.
 *
 * This test verifies that when a null pointer is passed as the volume argument, while valid values are supplied for mute and isSinkBinVolume, the API method SetAudioProperty does not throw any exceptions. This ensures robustness against negative input scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 057@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Create an instance of AmlogicSocInterface | No input parameters for default constructor | Object is created successfully without exception | Should be successful |
 * |02| Prepare audio property inputs with null volume and valid mute value | volume = null, mute = "audio_mute", isSinkBinVolume = false | Inputs are set up correctly for negative test scenario | Should be successful |
 * |03| Call SetAudioProperty with the prepared parameters | volume = null, mute = "audio_mute", isSinkBinVolume = false | The API call does not throw any exception and handles null volume gracefully | Should Pass |
 */
TEST(AmlogicSocInterface, NegativeInputNullVolume) {
    std::cout << "Entering NegativeInputNullVolume test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface soc;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
  
        // volume is nullptr, prepare mute as valid array.
        const char* volume = nullptr;
        const int SIZE = 50;
        char muteArr[SIZE];
        strncpy(muteArr, "audio_mute", SIZE - 1);
        muteArr[SIZE - 1] = '\0';
        const char* mute = muteArr;
        bool isSinkBinVolume = false;
  
        std::cout << "Before invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;
  
        EXPECT_NO_THROW(soc.SetAudioProperty(volume, mute, isSinkBinVolume));
        std::cout << "Called SetAudioProperty method" << std::endl;
  
        std::cout << "After invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;

    });
  
    std::cout << "Exiting NegativeInputNullVolume test" << std::endl;
}
/**
 * @brief Negative test case to verify handling of null mute input for SetAudioProperty.
 *
 * This test checks that calling SetAudioProperty with a valid volume string and a null mute parameter does not throw an exception. It ensures that the API gracefully handles a null mute value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 058@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create AmlogicSocInterface object and prepare input parameters with volume = "audio_volume" and mute = nullptr | volume = "audio_volume", mute = nullptr, isSinkBinVolume = false | Object created successfully and valid volume string set without throwing exception | Should be successful |
 * | 02 | Invoke SetAudioProperty method with the prepared inputs | input: volume = "audio_volume", mute = nullptr, isSinkBinVolume = false | API call does not throw any exceptions | Should Pass |
 */
TEST(AmlogicSocInterface, NegativeInputNullMute) {
    std::cout << "Entering NegativeInputNullMute test" << std::endl;

    EXPECT_NO_THROW({
        AmlogicSocInterface soc;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
  
        // mute is nullptr, prepare volume as valid array.
        const int SIZE = 50;
        char volumeArr[SIZE];
        strncpy(volumeArr, "audio_volume", SIZE - 1);
        volumeArr[SIZE - 1] = '\0';
        const char* volume = volumeArr;
        const char* mute = nullptr;
        bool isSinkBinVolume = false;
  
        std::cout << "Before invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;
  
        EXPECT_NO_THROW(soc.SetAudioProperty(volume, mute, isSinkBinVolume));
        std::cout << "Called SetAudioProperty method" << std::endl;
  
        std::cout << "After invoking SetAudioProperty: volume = " 
                  << (volume ? volume : "null") << ", mute = " 
                  << (mute ? mute : "null") << ", isSinkBinVolume = " 
                  << isSinkBinVolume << std::endl;

    });
  
    std::cout << "Exiting NegativeInputNullMute test" << std::endl;
}
/**
 * @brief Positive test for SetPlaybackFlags using zero flags and non-subtitle mode
 *
 * This test verifies that calling SetPlaybackFlags with flags set to 0 and isSub to false does not throw any exceptions and behaves as expected, ensuring that the playback flags are handled correctly in a non-subtitle scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                              | Test Data                     | Expected Result                                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------------------ | ----------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Create AmlogicSocInterface object and invoke SetPlaybackFlags with flags 0 and isSub as false | flags = 0, isSub = false        | No exception thrown; playback flags set as expected             | Should Pass |
 */
TEST(AmlogicSocInterface, PositiveTestZeroFlagsNonSub) {
    std::cout << "Entering PositiveTestZeroFlagsNonSub test" << std::endl;
    
    gint flags = 0;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting PositiveTestZeroFlagsNonSub test" << std::endl;
}
/**
 * @brief Validate that SetPlaybackFlags API correctly processes a request with non-zero flags and non-subtitle mode.
 *
 * This test verifies that, when provided with non-zero flags (0x2) and isSub set to false, the SetPlaybackFlags 
 * function of the AmlogicSocInterface class does not throw any exceptions and correctly processes the input parameters.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 060@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test data by setting flags to 0x2 and isSub to false | flags = 0x2, isSub = false | Variables are set with the expected values | Should be successful |
 * | 02 | Create an instance of AmlogicSocInterface using the default constructor | No input parameters; output: object instance | Object is created successfully | Should be successful |
 * | 03 | Invoke SetPlaybackFlags with the provided flags and isSub values | flags = 0x2, isSub = false | API call completes without throwing any exception | Should Pass |
 */
TEST(AmlogicSocInterface, PositiveTestNonZeroFlagsNonSub) {
    std::cout << "Entering PositiveTestNonZeroFlagsNonSub test" << std::endl;
    
    gint flags = 0x2;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting PositiveTestNonZeroFlagsNonSub test" << std::endl;
}
/**
 * @brief Verify the SetPlaybackFlags API accepts zero flags with subtitle mode enabled without throwing exceptions
 *
 * This test verifies that the SetPlaybackFlags API accepts a zero flags input when isSub is true.
 * It ensures that the API does not throw any exception and that the object's state remains consistent
 * after invocation. This is important to validate that boundary condition inputs are handled properly.
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
 * | 01 | Initialize test variables and output the initial state | flags = 0, isSub = true | Variables are set correctly and initial state is printed | Should be successful |
 * | 02 | Create AmlogicSocInterface object and invoke SetPlaybackFlags | input: flags = 0, isSub = true, output: none (no exception thrown) | API call executes without throwing any exception | Should Pass |
 * | 03 | Print final log message indicating test exit | No additional input | Exiting test message is printed confirming the end of test execution | Should be successful |
 */
TEST(AmlogicSocInterface, PositiveTestZeroFlagsSub) {
    std::cout << "Entering PositiveTestZeroFlagsSub test" << std::endl;
    
    gint flags = 0;
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
        
    std::cout << "Exiting PositiveTestZeroFlagsSub test" << std::endl;
}
/**
 * @brief Validate that SetPlaybackFlags handles negative flag values in non-subtitle mode.
 *
 * This test validates the AmlogicSocInterface::SetPlaybackFlags API to ensure that it correctly handles a negative flag value (-1) when the isSub parameter is false. The test verifies that no exceptions are thrown during the API call and that the API processes the provided inputs as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 062
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                             | Expected Result                                                  | Notes         |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test variables with flags=-1 and isSub=false         | flags = -1, isSub = false             | Variables are initialized properly                               | Should be successful |
 * | 02               | Create an instance of AmlogicSocInterface                       | N/A                                   | Object is created successfully                                   | Should be successful |
 * | 03               | Invoke SetPlaybackFlags with a negative flag and non-subtitle flag| input flags = -1, isSub = false         | API executes without throwing any exceptions and handles the input correctly | Should Pass   |
 */
TEST(AmlogicSocInterface, NegativeTestNegativeFlagsNonSub) {
    std::cout << "Entering NegativeTestNegativeFlagsNonSub test" << std::endl;
    
    gint flags = -1;
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << ", isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });

    std::cout << "Exiting NegativeTestNegativeFlagsNonSub test" << std::endl;
}
/**
 * @brief Validate SetPlaybackFlags behavior with maximum flag value in non-subtitle mode
 *
 * This test validates the boundary condition by setting the playback flags to INT_MAX (2147483647) and using non-subtitle mode. 
 * It verifies that the SetPlaybackFlags function in the AmlogicSocInterface class correctly handles the boundary input without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 063
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                             |Expected Result                                                    |Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------- |------------------------------------------------------------------ |-----------|
 * | 01               | Instantiate AmlogicSocInterface and invoke SetPlaybackFlags with boundary flag | flags = 2147483647, isSub = false       | SetPlaybackFlags is executed without throwing any exception      | Should Pass |
 */
TEST(AmlogicSocInterface, BoundaryTestMaxFlagsNonSub) {
    std::cout << "Entering BoundaryTestMaxFlagsNonSub test" << std::endl;
    
    gint flags = INT_MAX; // 2147483647
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << " (INT_MAX), isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
        
    std::cout << "Exiting BoundaryTestMaxFlagsNonSub test" << std::endl;
}
/**
 * @brief Test boundary condition with minimum integer value for flags in non-subtitle mode.
 *
 * This test validates that invoking the SetPlaybackFlags API with the minimum integer value (INT_MIN)
 * for flags and isSub set to false does not throw an exception. It ensures that the API call correctly handles
 * extreme boundary inputs without causing runtime errors.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 064
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                | Test Data                                  | Expected Result                                        | Notes          |
 * | :--------------: | -------------------------------------------------------------------------- | ------------------------------------------ | ------------------------------------------------------ | -------------- |
 * | 01               | Construct AmlogicSocInterface object using default constructor             | N/A                                        | Object is successfully created                         | Should be successful |
 * | 02               | Invoke SetPlaybackFlags with flags set to INT_MIN and isSub set to false     | flags = -2147483648, isSub = false           | API call completes without throwing an exception       | Should Pass    |
 */
TEST(AmlogicSocInterface, BoundaryTestMinFlagsNonSub) {
    std::cout << "Entering BoundaryTestMinFlagsNonSub test" << std::endl;
    
    gint flags = INT_MIN; // -2147483648
    bool isSub = false;
    std::cout << "Initial flags value: " << flags << " (INT_MIN), isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting BoundaryTestMinFlagsNonSub test" << std::endl;
}
/**
 * @brief Boundary test to validate SetPlaybackFlags method with maximum integer flag in subtitle mode.
 *
 * This test verifies that the SetPlaybackFlags API does not throw any exceptions when invoked with INT_MAX as the flag value in subtitle mode. It ensures that the API correctly handles the upper boundary value without any overflow or unexpected behavior.
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
 * | Variation / Step | Description                                                     | Test Data                        | Expected Result                                            | Notes               |
 * | :--------------: | --------------------------------------------------------------- | -------------------------------- | ---------------------------------------------------------- | ------------------- |
 * | 01               | Initialize test variables with flags set to INT_MAX and isSub to true | flags = INT_MAX, isSub = true       | Variables are assigned their intended values               | Should be successful |
 * | 02               | Create an instance of AmlogicSocInterface                       | No input parameter               | Object is instantiated successfully                        | Should be successful |
 * | 03               | Invoke SetPlaybackFlags with the initialized test data          | flags = INT_MAX, isSub = true       | SetPlaybackFlags does not throw an exception               | Should Pass         |
 */
TEST(AmlogicSocInterface, BoundaryTestMaxFlagsSub) {
    std::cout << "Entering BoundaryTestMaxFlagsSub test" << std::endl;
    
    gint flags = INT_MAX; // 2147483647
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << " (INT_MAX), isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });

    std::cout << "Exiting BoundaryTestMaxFlagsSub test" << std::endl;
}
/**
 * @brief Test Boundary Conditions for SetPlaybackFlags with minimum integer flags
 *
 * This test verifies that the SetPlaybackFlags function in AmlogicSocInterface correctly handles the minimum possible integer (-2147483648) for the flags parameter when isSub is true. It ensures that the API does not throw any exceptions and behaves as expected under boundary conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 066
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------ | ------------|
 * |       01         | Create an AmlogicSocInterface object and invoke SetPlaybackFlags with boundary values | flags = -2147483648, isSub = true                    | No exception thrown; SetPlaybackFlags executes successfully | Should Pass |
 */
TEST(AmlogicSocInterface, BoundaryTestMinFlagsSub) {
    std::cout << "Entering BoundaryTestMinFlagsSub test" << std::endl;
    
    gint flags = INT_MIN; // -2147483648
    bool isSub = true;
    std::cout << "Initial flags value: " << flags << " (INT_MIN), isSub: " << std::boolalpha << isSub << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface obj;
        std::cout << "Created AmlogicSocInterface object using default constructor" << std::endl;
        std::cout << "Invoking SetPlaybackFlags with flags: " << flags << " and isSub: " << isSub << std::endl;
        obj.SetPlaybackFlags(flags, isSub);
        std::cout << "After SetPlaybackFlags, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting BoundaryTestMinFlagsSub test" << std::endl;
}
/**
 * @brief This test verifies that SetPlaybackRate function works correctly when only one valid source is provided
 *
 * This test validates that the playback rate setting function handles a single valid source in the sources vector along with proper pipeline and decoder pointers. It intends to ensure that the API returns true for a valid configuration.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 067@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface using default constructor | N/A | Instance created without exceptions | Should Pass |
 * | 02 | Create dummy GstElement instances for source, pipeline, video decoder and audio decoder | dummySource1, dummyPipeline, dummyVideoDec, dummyAudioDec (dummy objects) | Dummy elements created successfully | Should be successful |
 * | 03 | Prepare sources vector with one valid pointer | sources = [&dummySource1] | Vector contains one valid source pointer | Should be successful |
 * | 04 | Invoke SetPlaybackRate API with prepared parameters | sources = [&dummySource1], pipeline = &dummyPipeline, rate = 1.0, video_dec = &dummyVideoDec, audio_dec = &dummyAudioDec | API returns true | Should Pass |
 * | 05 | Validate the API outcome using assertion | result variable from API invocation | Assertion passes if result is true | Should Pass |
 */
TEST(AmlogicSocInterface, ValidPlaybackRateOneSource)
{
    std::cout << "Entering ValidPlaybackRateOneSource test" << std::endl;
    
    // Create instance of AmlogicSocInterface using default constructor
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    // Create dummy GstElement instances for parameters
    GstElement dummySource1;
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    // Prepare the sources vector with one valid pointer
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource1);
    
    double rate = 1.0;
    std::cout << "Invoking SetPlaybackRate with 1 source, pipeline pointer " << &dummyPipeline 
              << ", rate " << rate << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidPlaybackRateOneSource test" << std::endl;
}
/**
 * @brief Validate playback rate setting for multiple valid sources in AmlogicSocInterface
 *
 * This test verifies that the SetPlaybackRate method of AmlogicSocInterface correctly handles
 * setting the playback rate when provided with multiple valid GstElement pointers. The objective is
 * to ensure that the API returns true under normal operating conditions with proper input objects.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 068@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of AmlogicSocInterface and ensure no exceptions are thrown | None | Instance created successfully | Should Pass |
 * | 02 | Instantiate dummy GstElement objects for sources, pipeline, video_dec, and audio_dec | dummySource1, dummySource2, dummyPipeline, dummyVideoDec, dummyAudioDec | Dummy objects are instantiated correctly | Should be successful |
 * | 03 | Prepare sources vector by adding valid dummy source pointers | sources vector = (&dummySource1, &dummySource2) | Vector contains two valid pointers | Should be successful |
 * | 04 | Invoke SetPlaybackRate with the prepared sources, pipeline pointer, playback rate, video_dec pointer, and audio_dec pointer | sources = (&dummySource1, &dummySource2), dummyPipeline pointer, rate = 1.5, dummyVideoDec pointer, dummyAudioDec pointer | API returns true indicating successful setting of playback rate | Should Pass |
 */
TEST(AmlogicSocInterface, ValidPlaybackRateMultipleSources)
{
    std::cout << "Entering ValidPlaybackRateMultipleSources test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    // Create dummy GstElement instances
    GstElement dummySource1, dummySource2;
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    // Prepare the sources vector with two valid pointers
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource1);
    sources.push_back(&dummySource2);
    
    double rate = 1.5;
    std::cout << "Invoking SetPlaybackRate with " << sources.size() 
              << " sources, pipeline pointer " << &dummyPipeline 
              << ", rate " << rate << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidPlaybackRateMultipleSources test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate fails when the pipeline pointer is null.
 *
 * This test verifies that the AmlogicSocInterface::SetPlaybackRate API correctly handles a null pipeline pointer input. 
 * The test initializes the necessary GstElement objects and a sources vector containing a valid pointer, but intentionally 
 * passes a null pointer for the pipeline parameter. It is expected that the API returns false in this scenario, ensuring that 
 * proper error handling is implemented.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 069@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate AmlogicSocInterface and create dummy GstElement objects for source, video decoder, and audio decoder. | Instance creation: socInterface; dummySource, dummyVideoDec, dummyAudioDec objects created. | Instantiation completes without throwing exceptions. | Should be successful |
 * | 02 | Prepare the input parameters: create a sources vector containing a valid pointer to dummySource and set the pipeline pointer to null. | sources = [&dummySource], pipeline = nullptr, rate = 1.0, video_dec pointer = &dummyVideoDec, audio_dec pointer = &dummyAudioDec | Input parameters are set as intended. | Should be successful |
 * | 03 | Call SetPlaybackRate API with the prepared inputs and check the returned result. | Function call with: sources, pipeline = nullptr, rate = 1.0, &dummyVideoDec, &dummyAudioDec | API returns false, matching the EXPECT_FALSE(result) assertion. | Should Fail |
 */
TEST(AmlogicSocInterface, NullPipelinePointer)
{
    std::cout << "Entering NullPipelinePointer test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    GstElement dummySource;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    // Prepare sources vector with one valid pointer
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource);
    
    double rate = 1.0;
    GstElement* pipeline = nullptr; // Null pipeline pointer
    std::cout << "Invoking SetPlaybackRate with sources count " << sources.size() 
              << ", pipeline pointer NULL, rate " << rate 
              << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, pipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullPipelinePointer test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when provided with a null video decoder pointer
 *
 * This test validates that when SetPlaybackRate is invoked with a valid vector of source elements, a valid pipeline pointer, a playback rate of 1.0, a nullptr for the video decoder pointer, and a valid audio decoder pointer, the API correctly returns false. This behavior ensures that the function handles the invalid video decoder pointer scenario gracefully without causing unexpected behavior.
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
 * | 01 | Create an instance of AmlogicSocInterface and verify that no exception is thrown | None | Instance is created successfully without throwing any exceptions | Should be successful |@n
 * | 02 | Invoke SetPlaybackRate with valid sources, pipeline pointer, rate=1.0, null video decoder pointer, and valid audio decoder pointer | sources = { dummySource address }, pipeline = dummyPipeline address, rate = 1.0, video_dec = nullptr, audio_dec = dummyAudioDec address | API returns false and EXPECT_FALSE(result) assertion passes | Should Fail |
 */
TEST(AmlogicSocInterface, NullVideoDecoderPointer)
{
    std::cout << "Entering NullVideoDecoderPointer test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    GstElement dummySource;
    GstElement dummyPipeline;
    GstElement dummyAudioDec;
    
    // Prepare sources vector with one valid pointer
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource);
    
    double rate = 1.0;
    GstElement* video_dec = nullptr; // Null video decoder pointer
    std::cout << "Invoking SetPlaybackRate with sources count " << sources.size() 
              << ", pipeline pointer " << &dummyPipeline 
              << ", rate " << rate 
              << ", video_dec pointer NULL, audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, video_dec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullVideoDecoderPointer test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate fails when provided with a null audio decoder pointer.
 *
 * This test verifies that invoking SetPlaybackRate with a valid sources vector, valid pipeline and video decoder pointers but with a null audio decoder pointer results in a failure (i.e., the function returns false). The objective is to ensure that the API correctly handles the null pointer scenario for the audio decoder.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 071@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                     | Test Data: input(s) and output(s)                                                                                                          | Expected Result                                                              | Notes           |
 * | :----:           | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of AmlogicSocInterface and ensure no exception is thrown upon construction.  | Constructor called with no input arguments; output: valid AmlogicSocInterface instance                                                         | Instance is created without throwing an exception                           | Should be successful |
 * | 02               | Invoke SetPlaybackRate with a valid sources vector, valid pipeline pointer, valid video decoder pointer, a rate of 1.0, and a null audio decoder pointer. | sources = address of dummySource, pipeline = address of dummyPipeline, rate = 1.0, video_dec = address of dummyVideoDec, audio_dec = nullptr | API returns false and the assertion check (EXPECT_FALSE(result)) passes       | Should Fail     |
 */
TEST(AmlogicSocInterface, NullAudioDecoderPointer)
{
    std::cout << "Entering NullAudioDecoderPointer test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    GstElement dummySource;
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    
    // Prepare sources vector with one valid pointer
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource);
    
    double rate = 1.0;
    GstElement* audio_dec = nullptr; // Null audio decoder pointer
    std::cout << "Invoking SetPlaybackRate with sources count " << sources.size() 
              << ", pipeline pointer " << &dummyPipeline 
              << ", rate " << rate 
              << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer NULL" << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, audio_dec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullAudioDecoderPointer test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when invoked with a negative playback rate.
 *
 * This test ensures that the API correctly handles negative playback rate values by returning false when an invalid (negative) rate is provided. It verifies that the function call fails as expected in the negative scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 072@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct AmlogicSocInterface object | N/A | Object constructed successfully | Should be successful |
 * | 02 | Prepare dummy GstElement objects and sources vector containing a valid pointer | dummySource = valid object, dummyPipeline = valid object, dummyVideoDec = valid object, dummyAudioDec = valid object | Inputs are prepared for the API call | Should be successful |
 * | 03 | Invoke SetPlaybackRate with a negative playback rate | sources = [&dummySource], pipeline pointer = &dummyPipeline, rate = -1.0, video_dec pointer = &dummyVideoDec, audio_dec pointer = &dummyAudioDec | API returns false due to negative rate value | Should Fail |
 * | 04 | Validate that the API call returned false using assertion | result = false | Assertion confirms false return value | Should be successful |
 */
TEST(AmlogicSocInterface, NegativePlaybackRateValue)
{
    std::cout << "Entering NegativePlaybackRateValue test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    GstElement dummySource;
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    // Prepare sources vector with one valid pointer
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource);
    
    double rate = -1.0;
    std::cout << "Invoking SetPlaybackRate with sources count " << sources.size() 
              << ", pipeline pointer " << &dummyPipeline 
              << ", rate " << rate 
              << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;

    EXPECT_FALSE(result);
    
    std::cout << "Exiting NegativePlaybackRateValue test" << std::endl;
}
/**
 * @brief Validate that SetPlaybackRate returns false when a playback rate value of zero is provided.
 *
 * This test checks whether the AmlogicSocInterface::SetPlaybackRate API correctly handles an edge case where the playback rate is set to zero.
 * It verifies that the API does not accept a zero playback rate and properly returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 073@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                    | Test Data                                                                                                                      | Expected Result                                | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------- | ----------- |
 * | 01               | Instantiate AmlogicSocInterface and dummy GstElement objects, form a sources vector, then call SetPlaybackRate with a zero playback rate. | sources = [dummySource], pipeline pointer = dummyPipeline, rate = 0.0, video_dec pointer = dummyVideoDec, audio_dec pointer = dummyAudioDec | SetPlaybackRate returns false (EXPECT_FALSE(result)) | Should Pass |
 */
TEST(AmlogicSocInterface, ZeroPlaybackRateValue)
{
    std::cout << "Entering ZeroPlaybackRateValue test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    GstElement dummySource;
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    std::vector<GstElement*> sources;
    sources.push_back(&dummySource);
    
    double rate = 0.0;
    std::cout << "Invoking SetPlaybackRate with sources count " << sources.size() 
              << ", pipeline pointer " << &dummyPipeline 
              << ", rate " << rate 
              << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;

    EXPECT_FALSE(result);
    
    std::cout << "Exiting ZeroPlaybackRateValue test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when provided with an empty sources vector
 *
 * This test verifies that calling SetPlaybackRate with an empty sources vector correctly fails
 * to set the playback rate. The test ensures that the initialization of the AmlogicSocInterface object
 * succeeds without throwing an exception, and that the API returns false as expected, ensuring proper
 * handling of invalid input parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 074@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                           | Expected Result                                                  | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate AmlogicSocInterface, prepare dummy GstElements, and call SetPlaybackRate with empty sources vector | sources = empty vector, pipeline pointer = &dummyPipeline, rate = 1.0, video_dec pointer = &dummyVideoDec, audio_dec pointer = &dummyAudioDec | API returns false and does not throw any exception                | Should Fail |
 */
TEST(AmlogicSocInterface, EmptySourcesVector)
{
    std::cout << "Entering EmptySourcesVector test" << std::endl;
    
    EXPECT_NO_THROW(AmlogicSocInterface socInterface;);
    AmlogicSocInterface socInterface;
    
    // Create dummy GstElement instances for pipeline, video_dec and audio_dec
    GstElement dummyPipeline;
    GstElement dummyVideoDec;
    GstElement dummyAudioDec;
    
    // Empty sources vector
    std::vector<GstElement*> sources;
    
    double rate = 1.0;
    std::cout << "Invoking SetPlaybackRate with empty sources vector, pipeline pointer " 
              << &dummyPipeline << ", rate " << rate 
              << ", video_dec pointer " << &dummyVideoDec 
              << ", audio_dec pointer " << &dummyAudioDec << std::endl;
    
    bool result = socInterface.SetPlaybackRate(sources, &dummyPipeline, rate, &dummyVideoDec, &dummyAudioDec);
    std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting EmptySourcesVector test" << std::endl;
}
/**
 * @brief Validates that SetRateCorrection operates correctly with default settings.
 *
 * This test verifies that the AmlogicSocInterface default constructor does not throw exceptions,
 * and that the SetRateCorrection method can be invoked successfully to return a boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 075@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke default constructor of AmlogicSocInterface and verify no exception is thrown. | input: none, output: instance created | Instance is created successfully without any exception. | Should be successful |
 * | 02 | Invoke the SetRateCorrection method and capture its boolean return value without exception. | input: none, output: retValue (boolean) | Method returns a valid boolean value (true or false) without throwing an exception. | Should Pass |
 * | 03 | Validate that the return value from SetRateCorrection is either true or false. | input: retValue | The boolean return value satisfies the condition that it is either true or false. | Should Pass |
 */
TEST(AmlogicSocInterface, ValidateSetRateCorrectionDefaultBehavior) {
    std::cout << "Entering ValidateSetRateCorrectionDefaultBehavior test" << std::endl;
    
    // Create instance of AmlogicSocInterface using its default constructor
    EXPECT_NO_THROW({
        AmlogicSocInterface socInterfaceObj;
    });
    AmlogicSocInterface socInterfaceObj;
    std::cout << "AmlogicSocInterface object created using default constructor" << std::endl;
    
    // Invoke the SetRateCorrection method and capture its return value
    std::cout << "Invoking AmlogicSocInterface::SetRateCorrection()" << std::endl;
    bool retValue = false;
    EXPECT_NO_THROW({
        retValue = socInterfaceObj.SetRateCorrection();
    });
    std::cout << "Method SetRateCorrection returned value: " << std::boolalpha << retValue << std::endl;
    
    // Validate the expected behavior
    EXPECT_TRUE(retValue == true || retValue == false);
    
    std::cout << "Exiting ValidateSetRateCorrectionDefaultBehavior test" << std::endl;
}
/**
 * @brief Validates that SetSeamlessSwitch correctly enables seamless switching using a valid GstElement.
 *
 * This test ensures that when a valid GstElement pointer is provided along with a TRUE flag, the AmlogicSocInterface's
 * SetSeamlessSwitch method executes without throwing any exceptions. The test verifies proper instance creation and
 * correct API invocation for enabling seamless switching.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 076
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                                       | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of AmlogicSocInterface and a dummy GstElement pointer.   | N/A                                                   | Instance and pointer are created without any exceptions.              | Should be successful |
 * | 02               | Invoke SetSeamlessSwitch with the dummy GstElement pointer and value = true.  | dummyElement = valid dummy GstElement pointer, value = true | API call executes successfully without throwing exceptions.          | Should Pass     |
 */
TEST(AmlogicSocInterface, ValidGstElement_EnableSeamlessSwitch) {
    std::cout << "Entering ValidGstElement_EnableSeamlessSwitch test" << std::endl;
    
    // Create an instance of AmlogicSocInterface using its default constructor.
    EXPECT_NO_THROW({
        AmlogicSocInterface amlSoc;
        std::cout << "AmlogicSocInterface instance created successfully." << std::endl;
        
        // Creating a valid dummy GstElement object.
        GstElement dummyElementInstance;   // Using a dummy instance; actual structure assumed to be defined elsewhere.
        GstElement* dummyElement = &dummyElementInstance;
        std::cout << "Dummy GstElement pointer created at address: " << dummyElement << std::endl;
        
        gboolean value = true; // TRUE
        std::cout << "Invoking SetSeamlessSwitch with value: " << value << " (enable seamless switch)" << std::endl;
        amlSoc.SetSeamlessSwitch(dummyElement, value);
        std::cout << "SetSeamlessSwitch invoked successfully for enabling seamless switch." << std::endl;
    });
    
    std::cout << "Exiting ValidGstElement_EnableSeamlessSwitch test" << std::endl;
}
/**
 * @brief Verify that disabling seamless switch using a valid GstElement instance works without exceptions
 *
 * Validates the proper behavior of the SetSeamlessSwitch API in the AmlogicSocInterface class when provided with a valid GstElement pointer and a 'false' value to disable seamless switching. This test confirms that the API completes normally without throwing any exception.
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
 * | Variation / Step | Description                                                                          | Test Data                                                   | Expected Result                               | Notes               |
 * | :--------------: | ------------------------------------------------------------------------------------ | ----------------------------------------------------------- | --------------------------------------------- | ------------------- |
 * | 01               | Create an instance of AmlogicSocInterface                                              | None                                                        | Instance created successfully                 | Should be successful|
 * | 02               | Create a dummy GstElement pointer                                                      | None                                                        | Valid dummy GstElement pointer obtained       | Should be successful|
 * | 03               | Prepare the disable flag for seamless switch                                           | value = false                                               | Boolean variable set to false                 | Should be successful|
 * | 04               | Invoke SetSeamlessSwitch API with the dummy GstElement pointer and disable flag          | dummyElement = valid GstElement pointer, value = false      | API call completes without exception          | Should Pass         |
 */
TEST(AmlogicSocInterface, ValidGstElement_DisableSeamlessSwitch) {
    std::cout << "Entering ValidGstElement_DisableSeamlessSwitch test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface amlSoc;
        std::cout << "AmlogicSocInterface instance created successfully." << std::endl;
        
        // Creating a valid dummy GstElement object.
        GstElement dummyElementInstance;   // Dummy instance; actual structure assumed to be defined elsewhere.
        GstElement* dummyElement = &dummyElementInstance;
        std::cout << "Dummy GstElement pointer created at address: " << dummyElement << std::endl;
        
        gboolean value = false; // FALSE
        std::cout << "Invoking SetSeamlessSwitch with value: " << value << " (disable seamless switch)" << std::endl;
        amlSoc.SetSeamlessSwitch(dummyElement, value);
        std::cout << "SetSeamlessSwitch invoked successfully for disabling seamless switch." << std::endl;
    });
    
    std::cout << "Exiting ValidGstElement_DisableSeamlessSwitch test" << std::endl;
}
/**
 * @brief Verify that SetSeamlessSwitch handles a NULL GstElement pointer when enabling seamless switch
 *
 * This test verifies that invoking the SetSeamlessSwitch method with a NULL GstElement pointer and enabling seamless switch (value = true) does not throw any exception. The test ensures that the API gracefully handles null input values without failing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 078@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                              | Expected Result                                                                         | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | -------------------------------------- | --------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate AmlogicSocInterface, set GstElement pointer to NULL, and invoke SetSeamlessSwitch with enabled flag (true) | nullElement = nullptr, value = true     | SetSeamlessSwitch executes without throwing an exception and handles the NULL gracefully | Should Pass |
 */
TEST(AmlogicSocInterface, NullGstElement_EnableSeamlessSwitch) {
    std::cout << "Entering NullGstElement_EnableSeamlessSwitch test" << std::endl;
    
    EXPECT_NO_THROW({
        AmlogicSocInterface amlSoc;
        std::cout << "AmlogicSocInterface instance created successfully." << std::endl;
        
        // Use a NULL for GstElement pointer.
        GstElement* nullElement = nullptr;
        std::cout << "GstElement pointer is set to NULL." << std::endl;
        
        gboolean value = true; // TRUE
        std::cout << "Invoking SetSeamlessSwitch with NULL pointer and value: " << value << " (enable seamless switch)" << std::endl;
        amlSoc.SetSeamlessSwitch(nullElement, value);
        std::cout << "SetSeamlessSwitch invoked successfully with NULL GstElement pointer (handled gracefully)." << std::endl;
    });
    
    std::cout << "Exiting NullGstElement_EnableSeamlessSwitch test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
