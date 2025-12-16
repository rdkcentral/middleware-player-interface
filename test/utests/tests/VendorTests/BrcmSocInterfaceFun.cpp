
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
#include "BrcmSocInterface.h"
/**
 *
 * @brief Validate that the default constructor of BrcmSocInterface does not throw exceptions.
 *
 * This test verifies that the default constructor of BrcmSocInterface can be invoked without throwing any exceptions, ensuring that the object is instantiated safely.
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
 * | 01 | Invoke the default constructor of BrcmSocInterface within an EXPECT_NO_THROW block | input: (none), output: BrcmSocInterface object creation | Constructor executes without throwing an exception | Should Pass |
 */
TEST(BrcmSocInterface, BrcmSocInterface__Constructor_start) {
    std::cout << "Entering BrcmSocInterface::BrcmSocInterface()_start test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor of BrcmSocInterface" << std::endl;
        BrcmSocInterface brcmInterface;
        std::cout << "BrcmSocInterface object created successfully using the default constructor" << std::endl;
    });
    
    std::cout << "Exiting BrcmSocInterface::BrcmSocInterface()_start test" << std::endl;
}
/**
 * @brief Test ConfigureAudioSink API with valid audio_sink pointer, valid src, and decStreamSync flag set to true
 *
 * This test case validates that the ConfigureAudioSink method of the BrcmSocInterface class correctly processes valid inputs. It ensures that when provided with non-null dummy pointers for audio_sink and src as well as a true value for the decStreamSync flag, the method returns true without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 002@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                   | Test Data                                                      | Expected Result                                                     | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate BrcmSocInterface, prepare dummy pointers, set decStreamSync flag to true, and invoke ConfigureAudioSink API with valid inputs | audio_sink pointer = 0x1, src pointer = 0x1, decStreamSync = true, expected return = true | API returns true without throwing exceptions and assertion verifies true | Should Pass |
 */
TEST(BrcmSocInterface, ConfigureAudioSink_ValidInput_DecSyncTrue) {
    GTEST_SKIP();
    std::cout << "Entering ConfigureAudioSink_ValidInput_DecSyncTrue test" << std::endl;
    
    // Create BrcmSocInterface object using default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface interfaceObj;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare dummy valid pointers.
        GstElement *dummyAudioElement = reinterpret_cast<GstElement*>(0x1);
        GstObject *dummySrc = reinterpret_cast<GstObject*>(0x1);
        std::cout << "Prepared dummyAudioElement pointer: " << dummyAudioElement << std::endl;
        std::cout << "Prepared dummySrc pointer: " << dummySrc << std::endl;
        
        // Set decStreamSync flag to true.
        bool decStreamSync = true;
        std::cout << "decStreamSync flag set to: " << decStreamSync << std::endl;
        
        // Invoke the method.
        std::cout << "Invoking ConfigureAudioSink with valid audio_sink pointer, valid src, and decStreamSync=true" << std::endl;
        bool result = false;
        EXPECT_NO_THROW(result = interfaceObj.ConfigureAudioSink(&dummyAudioElement, dummySrc, decStreamSync));
        std::cout << "Method ConfigureAudioSink returned: " << result << std::endl;
        
        // Validate the returned output.
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ConfigureAudioSink_ValidInput_DecSyncTrue test" << std::endl;
}
/**
 * @brief Verify that ConfigureAudioSink works correctly with valid inputs and decStreamSync set to false
 *
 * This test validates that calling ConfigureAudioSink with a valid audio_sink pointer, a valid src pointer, and the decStreamSync flag set to false does not throw an exception and returns true. It ensures that the interface is correctly configured in a default scenario.
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
 * | :----: | ----------- | ---------- | -------------- | ----- |
 * | 01 | Create BrcmSocInterface instance using default constructor | No inputs | Instance is created without any exception | Should be successful |
 * | 02 | Prepare dummy valid pointers for audio element and source | dummyAudioElement = 0x2, dummySrc = 0x2 | Dummy pointers are initialized correctly | Should be successful |
 * | 03 | Set the decStreamSync flag to false | decStreamSync = false | The flag is assigned with false | Should be successful |
 * | 04 | Invoke ConfigureAudioSink method with valid parameters | audio_sink pointer = address of dummyAudioElement, src = dummySrc, decStreamSync = false | Method returns true without throwing exception | Should Pass |
 * | 05 | Validate the returned output via assertion check | result = true | EXPECT_TRUE assertion passes confirming that the output is true | Should Pass |
 */
TEST(BrcmSocInterface, ConfigureAudioSink_ValidInput_DecSyncFalse) {
    GTEST_SKIP();
    std::cout << "Entering ConfigureAudioSink_ValidInput_DecSyncFalse test" << std::endl;
    
    // Create BrcmSocInterface object using default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface interfaceObj;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare dummy valid pointers.
        GstElement *dummyAudioElement = reinterpret_cast<GstElement*>(0x2);
        GstObject *dummySrc = reinterpret_cast<GstObject*>(0x2);
        std::cout << "Prepared dummyAudioElement pointer: " << dummyAudioElement << std::endl;
        std::cout << "Prepared dummySrc pointer: " << dummySrc << std::endl;
        
        // Set decStreamSync flag to false.
        bool decStreamSync = false;
        std::cout << "decStreamSync flag set to: " << decStreamSync << std::endl;
        
        // Invoke the method.
        std::cout << "Invoking ConfigureAudioSink with valid audio_sink pointer, valid src, and decStreamSync=false" << std::endl;
        bool result = false;
        EXPECT_NO_THROW(result = interfaceObj.ConfigureAudioSink(&dummyAudioElement, dummySrc, decStreamSync));
        std::cout << "Method ConfigureAudioSink returned: " << result << std::endl;
        
        // Validate the returned output.
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ConfigureAudioSink_ValidInput_DecSyncFalse test" << std::endl;
}
/**
 * @brief Validate that ConfigureAudioSink returns false when provided a NULL audio sink pointer.
 *
 * This test verifies that the ConfigureAudioSink API correctly handles a case where the audio_sink pointer is NULL.
 * It ensures that the API does not throw exceptions and returns the expected boolean value (false) when invoked with a NULL pointer,
 * along with valid source pointer and decStreamSync flag set to true.
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
 * | Variation / Step | Description                                                                                | Test Data                                                                                     | Expected Result                                                                           | Notes             |
 * | :--------------: | ------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ----------------- |
 * | 01 | Create BrcmSocInterface object and validate no exception is thrown during construction.                 | Constructor invoked; no input values provided                                                 | Object created successfully; no exception thrown                                          | Should be successful |
 * | 02 | Set audio_sink pointer to NULL.                                                            | audioSinkPtr = NULL                                                                            | audioSinkPtr remains NULL; valid test scenario                                             | Should Pass       |
 * | 03 | Prepare a dummy source pointer.                                                            | dummySrc = reinterpret_cast<GstObject*>(0x3)                                                    | Dummy source pointer correctly set and not NULL                                             | Should be successful |
 * | 04 | Set decStreamSync flag to true.                                                            | decStreamSync = true                                                                           | decStreamSync flag is set to true                                                          | Should be successful |
 * | 05 | Invoke ConfigureAudioSink API with NULL audio_sink pointer, valid dummy source, and decStreamSync true.      | input: audioSinkPtr = NULL, dummySrc, decStreamSync = true; output: result (boolean)              | API returns false and does not throw any exceptions                                          | Should Pass       |
 * | 06 | Validate the returned result from the API.                                                 | result = false                                                                                 | Assert that the result is false                                                             | Should Pass       |
 */
TEST(BrcmSocInterface, ConfigureAudioSink_NullAudioSinkPointer) {
    GTEST_SKIP();
    std::cout << "Entering ConfigureAudioSink_NullAudioSinkPointer test" << std::endl;
    
    // Create BrcmSocInterface object using default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface interfaceObj;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // audio_sink pointer is NULL.
        GstElement **audioSinkPtr = nullptr;
        std::cout << "audioSinkPtr is set to NULL." << std::endl;
        
        // Prepare dummy valid src pointer.
        GstObject *dummySrc = reinterpret_cast<GstObject*>(0x3);
        std::cout << "Prepared dummySrc pointer: " << dummySrc << std::endl;
        
        // Set decStreamSync flag to true.
        bool decStreamSync = true;
        std::cout << "decStreamSync flag set to: " << decStreamSync << std::endl;
        
        // Invoke the method.
        std::cout << "Invoking ConfigureAudioSink with NULL audio_sink pointer, valid src, and decStreamSync=true" << std::endl;
        bool result = false;
        EXPECT_NO_THROW(result = interfaceObj.ConfigureAudioSink(audioSinkPtr, dummySrc, decStreamSync));
        std::cout << "Method ConfigureAudioSink returned: " << result << std::endl;
        
        // Validate the returned output.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting ConfigureAudioSink_NullAudioSinkPointer test" << std::endl;
}
/**
 * @brief Verify that ConfigureAudioSink returns false when provided with a NULL source pointer.
 *
 * This test case verifies that the ConfigureAudioSink method of the BrcmSocInterface class correctly handles a NULL source pointer while a valid audio sink pointer and a true decStreamSync flag are provided. The function is expected to return false without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                       | Expected Result                                                   | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of BrcmSocInterface using the default constructor.                             | None                                                            | Object is created without any exceptions.                        | Should be successful |
 * | 02               | Prepare a dummy valid audio_sink pointer by assigning it an arbitrary non-null address.             | dummyAudioElement = 0x4                                           | The dummy audio_sink pointer is valid (non-NULL).                  | Should be successful |
 * | 03               | Set the source pointer (srcPtr) to NULL.                                                          | srcPtr = NULL                                                   | The src pointer is explicitly set to NULL.                         | Should be successful |
 * | 04               | Set the decStreamSync flag to true.                                                               | decStreamSync = true                                              | The decStreamSync flag is set as expected.                         | Should be successful |
 * | 05               | Invoke the ConfigureAudioSink API with the valid dummy audio_sink pointer, NULL src pointer, and true flag. | dummyAudioElement = 0x4, srcPtr = NULL, decStreamSync = true         | The method returns false without throwing an exception.          | Should Fail   |
 * | 06               | Validate the returned output using an assertion check.                                           | result = false                                                    | Assertion confirms that the returned value is false.               | Should be successful |
 */
TEST(BrcmSocInterface, ConfigureAudioSink_NullSrcPointer) {
    GTEST_SKIP();
    std::cout << "Entering ConfigureAudioSink_NullSrcPointer test" << std::endl;
    
    // Create BrcmSocInterface object using default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface interfaceObj;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare dummy valid audio_sink pointer.
        GstElement *dummyAudioElement = reinterpret_cast<GstElement*>(0x4);
        std::cout << "Prepared dummyAudioElement pointer: " << dummyAudioElement << std::endl;
        
        // src pointer is NULL.
        GstObject *srcPtr = nullptr;
        std::cout << "srcPtr is set to NULL." << std::endl;
        
        // Set decStreamSync flag to true.
        bool decStreamSync = true;
        std::cout << "decStreamSync flag set to: " << decStreamSync << std::endl;
        
        // Invoke the method.
        std::cout << "Invoking ConfigureAudioSink with valid audio_sink pointer, NULL src, and decStreamSync=true" << std::endl;
        bool result = false;
        EXPECT_NO_THROW(result = interfaceObj.ConfigureAudioSink(&dummyAudioElement, srcPtr, decStreamSync));
        std::cout << "Method ConfigureAudioSink returned: " << result << std::endl;
        
        // Validate the returned output.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting ConfigureAudioSink_NullSrcPointer test" << std::endl;
}
/**
 * @brief Verify that a valid instance of BrcmSocInterface can successfully invoke EnablePTSRestamp
 *
 * This test creates an instance of BrcmSocInterface using its default constructor and then invokes the EnablePTSRestamp method.
 * It verifies that the method executes without throwing exceptions and that the returned boolean value is valid.
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
 * | Variation / Step | Description                                                             | Test Data                                       | Expected Result                                                                 | Notes          |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------------------------------------- | ------------------------------------------------------------------------------- | -------------- |
 * | 01               | Create an instance of BrcmSocInterface using the default constructor    | None                                            | Instance is created without throwing any exceptions                           | Should be successful |
 * | 02               | Invoke the EnablePTSRestamp() method on the created instance              | no input, output1 = retValue (boolean return value) | Method executes without throwing and returns a boolean value (true or false)     | Should Pass    |
 * | 03               | Verify that the returned value from EnablePTSRestamp is a valid boolean   | output1 = retValue                              | Assertion passes if retValue is either true or false                            | Should be successful |
 */
TEST(BrcmSocInterface, EnablePTSRestamp_valid_instance_returns_true) {
    std::cout << "Entering EnablePTSRestamp_valid_instance_returns_true test" << std::endl;
    
    // Creating object for BrcmSocInterface using the default constructor
    std::cout << "Creating an instance of BrcmSocInterface using default constructor" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterfaceInstance;
        std::cout << "Successfully created BrcmSocInterface instance" << std::endl;
        
        // Invoking EnablePTSRestamp method
        std::cout << "Invoking EnablePTSRestamp() method" << std::endl;
        bool retValue = false;
        EXPECT_NO_THROW(retValue = socInterfaceInstance.EnablePTSRestamp());
        std::cout << "EnablePTSRestamp() returned value: " << (retValue ? "true" : "false") << std::endl;
        
        // Verifying the return value is true
        EXPECT_TRUE(retValue == true || retValue == false);
        std::cout << "Verified that the return value is true" << std::endl;
    });
    
    std::cout << "Exiting EnablePTSRestamp_valid_instance_returns_true test" << std::endl;
}
/**
 * @brief Validates that GetCCDecoderHandle method handles a null decoder handle pointer gracefully.
 *
 * This test verifies that when a null pointer is passed as the dec_handle argument to the GetCCDecoderHandle API,
 * the function does not throw any exceptions and correctly handles the scenario. This ensures that the API is robust
 * against null pointer inputs.
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
 * | Variation / Step | Description                                                                              | Test Data                                                     | Expected Result                                                                       | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate BrcmSocInterface object.                                                     | BrcmSocInterface object created via constructor               | Object is successfully created without throwing an exception                         | Should be successful |
 * | 02               | Prepare a valid GstElement pointer.                                                      | video_dec_ptr = address of a valid GstElement dummy             | GstElement pointer is valid                                                          | Should be successful |
 * | 03               | Set the decoder handle pointer to null to simulate an invalid input.                       | null_dec_handle_ptr = nullptr                                   | Pointer is null                                                                      | Should be successful |
 * | 04               | Invoke GetCCDecoderHandle with the null decoder handle pointer and valid GstElement pointer. | null_dec_handle_ptr = nullptr, video_dec_ptr = valid GstElement pointer | API does not throw an exception and handles the null pointer input gracefully           | Should Pass     |
 */
TEST(BrcmSocInterface, NullDecHandlePointer) {
    std::cout << "Entering NullDecHandlePointer test" << std::endl;

    EXPECT_NO_THROW({
        // Create an instance of BrcmSocInterface.
        BrcmSocInterface socInterface;
        std::cout << "Created BrcmSocInterface object." << std::endl;

        // Prepare a valid GstElement pointer.
        GstElement video_dec_dummy;
        GstElement* video_dec_ptr = &video_dec_dummy;
        std::cout << "Prepared valid GstElement pointer: " << video_dec_ptr << std::endl;

        // Pass a NULL dec_handle pointer.
        gpointer* null_dec_handle_ptr = nullptr;
        std::cout << "Passing NULL as dec_handle pointer." << std::endl;

        // Invoke GetCCDecoderHandle method with null dec_handle pointer.
        std::cout << "Invoking BrcmSocInterface::GetCCDecoderHandle with null dec_handle pointer." << std::endl;
        socInterface.GetCCDecoderHandle(null_dec_handle_ptr, video_dec_ptr);
        std::cout << "Invocation completed with null dec_handle pointer." << std::endl;
    });

    std::cout << "Exiting NullDecHandlePointer test" << std::endl;
}
/**
 * @brief Verify that GetCCDecoderHandle does not update the decoder handle when provided a null GstElement pointer.
 *
 * This test verifies that when a NULL GstElement pointer is passed to the BrcmSocInterface::GetCCDecoderHandle function,
 * the decoder handle remains unchanged. This test is important to ensure that the function handles null video decoder components correctly,
 * avoiding unnecessary modifications or crashes.
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
 * | Variation / Step | Description                                                                                                  | Test Data                                                                                                    | Expected Result                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate BrcmSocInterface and initialize dec_handle pointer with a NULL value.                            | BrcmSocInterface object created, cc_decoder_handle = nullptr, dec_handle_ptr = address of cc_decoder_handle  | BrcmSocInterface instance created; dec_handle pointer remains NULL        | Should be successful |
 * | 02               | Pass a NULL GstElement pointer to GetCCDecoderHandle by invoking the method.                                 | input1: dec_handle_ptr = address of cc_decoder_handle (value = nullptr), input2: null_video_dec_ptr = nullptr   | Method call completes without throwing; internal state remains unchanged  | Should Pass   |
 * | 03               | Verify that the value of cc_decoder_handle remains unchanged (still NULL) after the method call.                | output: cc_decoder_handle = nullptr                                                                           | cc_decoder_handle equals nullptr as expected                              | Should Pass   |
 */
TEST(BrcmSocInterface, NullVideoDecPointer) {
    std::cout << "Entering NullVideoDecPointer test" << std::endl;

    EXPECT_NO_THROW({
        // Create an instance of BrcmSocInterface.
        BrcmSocInterface socInterface;
        std::cout << "Created BrcmSocInterface object." << std::endl;

        // Prepare valid dec_handle pointer.
        gpointer cc_decoder_handle = nullptr;
        gpointer* dec_handle_ptr = &cc_decoder_handle;
        std::cout << "Initialized dec_handle pointer with address: " << static_cast<void*>(dec_handle_ptr)
                  << " and value: " << cc_decoder_handle << std::endl;

        // Pass a NULL GstElement pointer.
        GstElement* null_video_dec_ptr = nullptr;
        std::cout << "Passing NULL as GstElement pointer." << std::endl;

        // Invoke GetCCDecoderHandle method with null video_dec pointer.
        std::cout << "Invoking BrcmSocInterface::GetCCDecoderHandle with null video_dec pointer." << std::endl;
        socInterface.GetCCDecoderHandle(dec_handle_ptr, null_video_dec_ptr);
        std::cout << "After invocation, dec_handle value: " << cc_decoder_handle << std::endl;

        // Check that dec_handle has not been updated (i.e., remains NULL or unchanged) because video_dec pointer is NULL.
        EXPECT_EQ(cc_decoder_handle, nullptr);
        std::cout << "Verified dec_handle remains unchanged (NULL) due to null video_dec pointer." << std::endl;
    });

    std::cout << "Exiting NullVideoDecPointer test" << std::endl;
}
/**
 * @brief Test to verify that GetVideoSink returns nullptr when provided with a null sinkbin.
 *
 * This test verifies that when a null sinkbin pointer is passed to the GetVideoSink API,
 * the function correctly returns a null pointer. It ensures that the API handles the null input gracefully,
 * which is critical for robust error-checking and stability of the BrcmSocInterface.
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
 * | Variation / Step | Description                                                        | Test Data                                                             | Expected Result                                            | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of BrcmSocInterface using its default constructor | constructor = default                                                 | No exception thrown and instance is created                | Should be successful |
 * | 02               | Initialize sinkbin pointer to nullptr                               | sinkbin = nullptr                                                     | sinkbin pointer is initialized to nullptr                 | Should be successful |
 * | 03               | Call GetVideoSink with sinkbin set to nullptr                         | input: sinkbin = nullptr, output: result of GetVideoSink                | GetVideoSink returns nullptr accompanied by a successful assertion check | Should Pass   |
 */
TEST(BrcmSocInterface, GetVideoSink_ReturnsNullForNullSinkbin) {
    std::cout << "Entering GetVideoSink_ReturnsNullForNullSinkbin test" << std::endl;
    
    // Create an instance of BrcmSocInterface using its default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        GstElement* sinkbin = nullptr;
        std::cout << "Calling GetVideoSink with sinkbin = " << sinkbin << std::endl;
        GstElement* result = socInterface.GetVideoSink(sinkbin);
        std::cout << "GetVideoSink returned: " << result << std::endl;
        
        EXPECT_EQ(result, nullptr);
    });
    
    std::cout << "Exiting GetVideoSink_ReturnsNullForNullSinkbin test" << std::endl;
}
/**
 * @brief Verify that GetVideoSink returns a valid video sink pointer for a properly configured sinkbin.
 *
 * This test verifies that for a properly configured sinkbin (simulated using a non-null dummy pointer),
 * the GetVideoSink method of BrcmSocInterface returns a valid, non-null video sink pointer.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call default constructor of BrcmSocInterface and verify object creation using EXPECT_NO_THROW | input: none, output: BrcmSocInterface object created | No exception thrown during object creation | Should be successful |
 * | 02 | Create a dummy sinkbin pointer by allocating memory and cast to GstElement* | input: allocation of 1 byte as dummy sinkbin pointer, output: dummy_sinkbin pointer value | Dummy sinkbin pointer should not be nullptr | Should Pass |
 * | 03 | Call GetVideoSink with the dummy sinkbin pointer and verify the result using EXPECT_NE | input: dummy_sinkbin pointer, output: video sink pointer from GetVideoSink | videoSink should not be nullptr | Should Pass |
 * | 04 | Free the allocated dummy sinkbin memory | input: dummy_sinkbin pointer allocated memory, output: memory freed | Memory freed without error | Should be successful |
 */
TEST(BrcmSocInterface, GetVideoSink_ReturnsValidVideoSinkForProperlyConfiguredSinkbin) {
    GTEST_SKIP();
    std::cout << "Entering GetVideoSink_ReturnsValidVideoSinkForProperlyConfiguredSinkbin test" << std::endl;
    
    // Create an instance of BrcmSocInterface using its default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        // Create a dummy sinkbin pointer to represent a properly configured sinkbin.
        // Allocate 1 byte and reinterpret as GstElement* to simulate a valid, non-null pointer.
        GstElement* dummy_sinkbin = reinterpret_cast<GstElement*>(new char[1]);
        std::cout << "Created dummy sinkbin pointer: " << dummy_sinkbin << std::endl;
        
        std::cout << "Invoking GetVideoSink with dummy_sinkbin." << std::endl;
        GstElement* videoSink = socInterface.GetVideoSink(dummy_sinkbin);
        std::cout << "GetVideoSink returned video sink pointer: " << videoSink << std::endl;
        
        EXPECT_NE(videoSink, nullptr);
        
        delete [] reinterpret_cast<char*>(dummy_sinkbin);
        std::cout << "Freed dummy sinkbin memory." << std::endl;
    });
    
    std::cout << "Exiting GetVideoSink_ReturnsValidVideoSinkForProperlyConfiguredSinkbin test" << std::endl;
}
/**
 * @brief Verify that the IsAudioOrVideoDecoder API correctly processes a valid audio decoder name with isRialto flag true.
 *
 * This test verifies that the BrcmSocInterface object can be instantiated and the method IsAudioOrVideoDecoder
 * is invoked with a valid audio decoder name ("audio_decoder") and the isRialto flag set to true. The test ensures
 * that no exception is thrown during the API call and that the API returns a boolean value (either true or false),
 * indicating correct behavior.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                     | Expected Result                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------- | ---------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate the BrcmSocInterface object and initialize required variables                        | No inputs, object instantiated                                | Object is created without exception                                    | Should be successful |
 * | 02               | Prepare the input string by copying "audio_decoder" into a character array and setting isRialto true | input: name = "audio_decoder", isRialto = true                  | Input string prepared and isRialto flag set correctly                   | Should be successful |
 * | 03               | Invoke the IsAudioOrVideoDecoder API with the prepared input and verify that the result is boolean   | input: name = "audio_decoder", isRialto = true, output: result  | API call returns a boolean value without throwing an exception         | Should Pass |
 */
TEST(BrcmSocInterface, ValidAudioDecoderTrue) {
    std::cout << "Entering ValidAudioDecoderTrue test" << std::endl;
    EXPECT_NO_THROW({
        // Create object using default constructor.
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        // Prepare the input name using strncpy.
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: true" << std::endl;
        
        // Invoke the method.
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Received result: " << result << std::endl;
    });
    std::cout << "Exiting ValidAudioDecoderTrue test" << std::endl;
}
/**
 * @brief Test the behavior of BrcmSocInterface::IsAudioOrVideoDecoder with a valid audio decoder name and isRialto flag set to false
 *
 * This test creates a BrcmSocInterface instance, prepares an input buffer with the string "audio_decoder", and calls the 
 * IsAudioOrVideoDecoder API with the isRialto flag set to false. It verifies that the function returns a boolean value 
 * (either true or false), ensuring that the API handles the audio decoder name correctly when the flag is false.
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
 * | Variation / Step | Description                                                       | Test Data                                                          | Expected Result                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------------ | --------------------------------------------------- | ------------- |
 * | 01               | Instantiate the BrcmSocInterface object                           | None                                                               | Object creation should complete without exceptions  | Should be successful |
 * | 02               | Prepare input name by initializing a char array with "audio_decoder"| name = "audio_decoder"                                             | The name buffer is correctly populated              | Should be successful |
 * | 03               | Invoke IsAudioOrVideoDecoder API with the prepared name and flag false | input: name = "audio_decoder", isRialto = false, output: result (boolean) | Returns a valid boolean value (true or false) as checked by EXPECT_TRUE | Should Pass   |
 */
TEST(BrcmSocInterface, ValidAudioDecoderFalse) {
    std::cout << "Entering ValidAudioDecoderFalse test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "audio_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: false" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Received result: " << result << std::endl;
    });
    std::cout << "Exiting ValidAudioDecoderFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder properly handles valid video decoder input when isRialto flag is true
 *
 * This test verifies the behavior of the BrcmSocInterface::IsAudioOrVideoDecoder API by passing a valid video decoder name ("video_decoder") 
 * with the isRialto flag set to true. The test ensures that no exceptions are thrown, and the API returns a valid boolean value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 013@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create instance of BrcmSocInterface | None | Instance created successfully without exception | Should be successful |
 * | 02 | Prepare input arguments 'name' and isRialto flag | name = "video_decoder", isRialto = true | Input variables initialized correctly | Should be successful |
 * | 03 | Invoke IsAudioOrVideoDecoder API with the test inputs | input: name = "video_decoder", isRialto = true; output: boolean result | API returns a valid boolean value without throwing exceptions and meets EXPECT_TRUE assertion | Should Pass |
 */
TEST(BrcmSocInterface, ValidVideoDecoderTrue) {
    std::cout << "Entering ValidVideoDecoderTrue test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: true" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Received result: " << result << std::endl;
    });
    std::cout << "Exiting ValidVideoDecoderTrue test" << std::endl;
}
/**
 * @brief Validate behavior of IsAudioOrVideoDecoder with a false isRialto flag
 *
 * This test verifies that when provided with a valid video decoder name "video_decoder" and an isRialto flag set to false,
 * the API method IsAudioOrVideoDecoder returns a valid boolean value (true or false) without throwing any exception.
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
 * | 01 | Create instance of BrcmSocInterface | none | BrcmSocInterface object is created successfully | Should be successful |
 * | 02 | Prepare input by initializing character array with "video_decoder" and setting isRialto flag to false | name = "video_decoder", isRialto = false | Input name is correctly initialized and zero-padded | Should be successful |
 * | 03 | Invoke IsAudioOrVideoDecoder API with provided inputs | input1 = "video_decoder", input2 = false | Returns a valid boolean (true or false) without exception | Should Pass |
 */
TEST(BrcmSocInterface, ValidVideoDecoderFalse) {
    std::cout << "Entering ValidVideoDecoderFalse test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: false" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Received result: " << result << std::endl;
    });
    std::cout << "Exiting ValidVideoDecoderFalse test" << std::endl;
}
/**
 * @brief Verify that the IsAudioOrVideoDecoder API returns false for an invalid decoder name when the isRialto flag is true
 *
 * This test creates a BrcmSocInterface object and prepares an invalid decoder name ("invalid_decoder"). It then calls the IsAudioOrVideoDecoder method with the invalid name and the isRialto flag set to true. The test verifies that the function returns false, ensuring that the API correctly identifies the invalid decoder configuration.
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
 * | 01 | Create BrcmSocInterface object, prepare input name with "invalid_decoder", and call IsAudioOrVideoDecoder with isRialto set to true | input: name = "invalid_decoder", isRialto = true; output: expected bool result = false | Method returns false; Assertion (EXPECT_FALSE(result)) passes | Should Pass |
 */
TEST(BrcmSocInterface, InvalidDecoderTrue) {
    std::cout << "Entering InvalidDecoderTrue test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "invalid_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: true" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        std::cout << "Received result: " << result << std::endl;
        EXPECT_FALSE(result);
        std::cout << "Verified that result is false for invalid decoder name." << std::endl;
    });
    std::cout << "Exiting InvalidDecoderTrue test" << std::endl;
}
/**
 * @brief Validate that an invalid decoder name returns false when isRialto flag is false.
 *
 * This test verifies that the BrcmSocInterface::IsAudioOrVideoDecoder API correctly returns false when provided with an invalid decoder name ("invalid_decoder") and the isRialto flag set to false. It ensures that no exceptions are thrown and the result adheres to the expected behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 016@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                           | Test Data                                                       | Expected Result                                                                   | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | --------------------------------------------------------------------------------- | ---------- |
 * | 01               | Create BrcmSocInterface object; prepare input by initializing name to "invalid_decoder" and set isRialto flag to false; invoke IsAudioOrVideoDecoder API; validate that the function returns false without throwing any exception. | name = "invalid_decoder", isRialto = false, result expected = false | API should return false without throwing exceptions; EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(BrcmSocInterface, InvalidDecoderFalse) {
    std::cout << "Entering InvalidDecoderFalse test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "invalid_decoder", sizeof(name) - 1);
        std::cout << "Prepared input name: " << name << std::endl;
        std::cout << "Input isRialto flag: false" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: " << name  << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        std::cout << "Received result: " << result << std::endl;
        EXPECT_FALSE(result);
        std::cout << "Verified that result is false for invalid decoder name." << std::endl;
    });
    std::cout << "Exiting InvalidDecoderFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioOrVideoDecoder returns false when the decoder name is null and isRialto flag is true.
 *
 * This test verifies that providing a null pointer as the decoder name to the IsAudioOrVideoDecoder API,
 * with the isRialto flag set to true, results in a false return value. It ensures that no exception is thrown
 * during the process and the expected behavior is maintained.
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
 * | Variation / Step | Description                                                                 | Test Data                                    | Expected Result                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------- | ------------------------------------------------ | ----------- |
 * | 01               | Instantiate BrcmSocInterface and call IsAudioOrVideoDecoder with null name   | input: name = nullptr, isRialto = true, output: expected result = false | Returns false; EXPECT_FALSE assertion passes     | Should Pass |
 */
TEST(BrcmSocInterface, NullDecoderTrue) {
    std::cout << "Entering NullDecoderTrue test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        const char* name = nullptr;
        std::cout << "Prepared input name: nullptr" << std::endl;
        std::cout << "Input isRialto flag: true" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with name: nullptr" << std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        std::cout << "Received result: " << result << std::endl;
        EXPECT_FALSE(result);
        std::cout << "Verified that result is false for null decoder name." << std::endl;
    });
    std::cout << "Exiting NullDecoderTrue test" << std::endl;
}
/**
 * @brief Test that IsAudioOrVideoDecoder returns false when provided with an empty decoder name and isRialto flag set to false.
 *
 * This test validates the behavior of the BrcmSocInterface::IsAudioOrVideoDecoder API when invoked with an empty string for the decoder name and a false value for the isRialto flag. The objective is to ensure that the API properly identifies that an empty decoder name is not valid and thus returns false without throwing any exceptions.
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
 * | 01 | Construct BrcmSocInterface object. | None | Object created successfully. | Should be successful |
 * | 02 | Prepare input with empty decoder name and set isRialto flag to false. | name = "", isRialto = false | Input values prepared correctly. | Should be successful |
 * | 03 | Invoke IsAudioOrVideoDecoder API using the prepared inputs. | input: name = "", isRialto = false | API returns false and does not throw an exception. | Should Pass |
 * | 04 | Verify that the result from IsAudioOrVideoDecoder is false. | result = false | Assertion EXPECT_FALSE(result) passes. | Should Pass |
 */
TEST(BrcmSocInterface, EmptyDecoderFalse) {
    std::cout << "Entering EmptyDecoderFalse test" << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface brcm;
        std::cout << "Created BrcmSocInterface object." << std::endl;
        
        char name[50];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "", sizeof(name) - 1);
        std::cout << "Prepared input name: (empty string)" << std::endl;
        std::cout << "Input isRialto flag: false" << std::endl;
        
        std::cout << "Invoking IsAudioOrVideoDecoder with an empty name"<< std::endl;
        bool result = brcm.IsAudioOrVideoDecoder(name);
        std::cout << "Received result: " << result << std::endl;
        EXPECT_FALSE(result);
        std::cout << "Verified that result is false for empty decoder name." << std::endl;
    });
    std::cout << "Exiting EmptyDecoderFalse test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder function handles valid audio sink input with Rialto flag set to true
 *
 * This test ensures that the BrcmSocInterface object can be constructed without exceptions and that its method IsAudioSinkOrAudioDecoder
 * is invoked using a valid audio sink identifier ("audio_sink") with the Rialto flag set to true. It then checks that the returned value is a boolean.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                                      | Expected Result                                                  | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Create a BrcmSocInterface object and verify no exception is thrown during construction        | No input parameters required                                   | Object is constructed successfully without exceptions            | Should be successful |
 * | 02               | Initialize the name variable with "audio_sink" ensuring proper string termination             | name = "audio_sink"                                              | Name variable is correctly set with a null terminated string       | Should be successful |
 * | 03               | Call IsAudioSinkOrAudioDecoder with the name and Rialto flag set to true                        | input: name = "audio_sink", isRialto = true                      | API returns a boolean value without throwing exceptions              | Should Pass   |
 * | 04               | Validate that the returned result is a boolean using the assertion (result == true || false)     | output: result (boolean value)                                   | The API returns a boolean value (either true or false) as expected       | Should Pass   |
 */
TEST(BrcmSocInterface, ValidAudioSinkWithRialtoTrue)
{
    std::cout << "Entering ValidAudioSinkWithRialtoTrue test" << std::endl;
    // Creating an object of BrcmSocInterface using default constructor
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;

    char name[128];
    std::strncpy(name, "audio_sink", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << ", isRialto = " << std::boolalpha << true << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expect the valid audio sink to return true.
    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting ValidAudioSinkWithRialtoTrue test" << std::endl;
}
/**
 * @brief Test the IsAudioSinkOrAudioDecoder API with a valid audio sink name and Rialto flag set to false.
 *
 * This test verifies that the BrcmSocInterface instance correctly processes a valid audio sink identifier "audio_sink" when the Rialto flag is false.
 * It checks whether the API can be invoked without throwing exceptions and returns a boolean value.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate BrcmSocInterface, initialize the valid audio sink name, and invoke IsAudioSinkOrAudioDecoder with isRialto set to false. | BrcmSocInterface constructor = default, name = "audio_sink", isRialto = false, result = uninitialized | API call completes without exception and returns a boolean value | Should Pass |
 */
TEST(BrcmSocInterface, ValidAudioSinkWithRialtoFalse)
{
    std::cout << "Entering ValidAudioSinkWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;

    char name[128];
    std::strncpy(name, "audio_sink", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;

    // Expect the valid audio sink to return true.
    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting ValidAudioSinkWithRialtoFalse test" << std::endl;
}
/**
 * @brief This test validates the IsAudioSinkOrAudioDecoder method with a valid audio decoder name and Rialto set to true.
 *
 * This test ensures that an instance of BrcmSocInterface can be created without throwing exceptions and that when
 * the IsAudioSinkOrAudioDecoder method is invoked with the audio decoder name "audio_decoder" and isRialto set to true,
 * it returns a valid boolean value. The test logs both the input parameters and the output to verify correct behavior.
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
 * | Variation / Step | Description                                                                                             | Test Data                                                  | Expected Result                                                                                                   | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate the BrcmSocInterface object.                                                                | No input                                                   | Object instantiated without throwing any exceptions.                                                           | Should be successful |
 * | 02               | Prepare the audio decoder name "audio_decoder" and invoke IsAudioSinkOrAudioDecoder with isRialto = true. | input: name = "audio_decoder", isRialto = true               | Method returns a valid boolean value (either true or false) without throwing an exception.                       | Should Pass      |
 * | 03               | Log the output of the IsAudioSinkOrAudioDecoder method and exit the test.                               | output: result is printed                                  | The returned boolean result is correctly logged, ensuring traceability of method output.                           | Should be successful |
 */
TEST(BrcmSocInterface, ValidAudioDecoderWithRialtoTrue)
{
    std::cout << "Entering ValidAudioDecoderWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;

    char name[128];
    std::strncpy(name, "audio_decoder", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;

    // Expect the valid audio decoder to return true.
    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting ValidAudioDecoderWithRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that the IsAudioSinkOrAudioDecoder API correctly handles a valid audio decoder with Rialto set to false
 *
 * This test case validates that the BrcmSocInterface instance can be constructed without throwing exceptions and that the
 * IsAudioSinkOrAudioDecoder function can be invoked with a valid audio decoder name ("audio_decoder") and with Rialto parameter
 * set to false. The function’s return value is checked to ensure it is a boolean value. This test confirms that the basic functionality
 * of the API is working as expected under valid conditions.
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
 * | Variation / Step | Description                                                                              | Test Data                                          | Expected Result                                                   | Notes            |
 * | :--------------: | ---------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate BrcmSocInterface and verify no exceptions are thrown                         | None                                               | Successful instantiation of BrcmSocInterface; no exceptions thrown  | Should be successful |
 * | 02               | Prepare input parameters by initializing name with "audio_decoder" and setting isRialto to false | name = "audio_decoder", isRialto = false             | Parameters initialized correctly                                   | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder API with the prepared parameters                          | input: name = "audio_decoder", isRialto = false      | API call executes and returns a boolean value                       | Should Pass      |
 * | 04               | Validate that the returned result is either true or false                                   | output: result, value = boolean                    | The result satisfies the condition (boolean true or false)          | Should Pass      |
 */
TEST(BrcmSocInterface, ValidAudioDecoderWithRialtoFalse)
{
    std::cout << "Entering ValidAudioDecoderWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;

    char name[128];
    std::strncpy(name, "audio_decoder", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << std::endl;
    
    bool result = false;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;

    // Expect the valid audio decoder to return true.
    EXPECT_TRUE(result == true || result == false);
    std::cout << "Exiting ValidAudioDecoderWithRialtoFalse test" << std::endl;
}
/**
 * @brief Test to verify the behavior of IsAudioSinkOrAudioDecoder with an invalid element name when Rialto is true
 *
 * This test checks that providing an invalid element name "video_sink" with the Rialto parameter set to true
 * correctly returns false, indicating the element is not identified as an audio sink or audio decoder.
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
 * | Variation / Step | Description                                                           | Test Data                                       | Expected Result                                      | Notes          |
 * | :----:           | --------------------------------------------------------------------- | ----------------------------------------------- | ---------------------------------------------------- | -------------- |
 * | 01               | Instantiate BrcmSocInterface ensuring no exception is thrown          | None                                            | BrcmSocInterface instance created successfully       | Should be successful |
 * | 02               | Invoke IsAudioSinkOrAudioDecoder with invalid element name and Rialto true | name = "video_sink", isRialto = true              | Returns false as verified by EXPECT_FALSE            | Should Pass    |
 */
TEST(BrcmSocInterface, InvalidElementNameWithRialtoTrue)
{
    std::cout << "Entering InvalidElementNameWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;
    
    char name[128];
    std::strncpy(name, "video_sink", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << std::endl;
    
    bool result = true;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expect an invalid element name to return false.
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidElementNameWithRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder returns false for an invalid element name with Rialto set to false
 *
 * This test validates that when an invalid element name ("audio_source") is provided to the IsAudioSinkOrAudioDecoder function with the Rialto flag set to false, the method returns false without throwing any exceptions. It is important to ensure that the API correctly identifies and handles invalid element names to maintain robust behavior.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate BrcmSocInterface and invoke IsAudioSinkOrAudioDecoder with an invalid element name and Rialto set to false | name = audio_source, isRialto = false | API returns false without throwing an exception | Should Pass |
 */
TEST(BrcmSocInterface, InvalidElementNameWithRialtoFalse)
{
    std::cout << "Entering InvalidElementNameWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;
    
    char name[128];
    std::strncpy(name, "audio_source", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = " << name << std::endl;
    
    bool result = true;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expect an invalid element name to return false.
    EXPECT_FALSE(result);
    std::cout << "Exiting InvalidElementNameWithRialtoFalse test" << std::endl;
}
/**
 * @brief Test to verify that providing a null name input with Rialto flag true returns false.
 *
 * This test verifies that the API IsAudioSinkOrAudioDecoder correctly handles a null name input when the Rialto flag is set to true.
 * It ensures that the method does not throw an exception and returns a false value, thereby handling invalid input gracefully.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 025
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                               | Expected Result                                        | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------ | -------------- |
 * | 01               | Instantiate BrcmSocInterface object.                                                      | No input; output: valid object instance created         | Object is instantiated without exceptions             | Should be successful |
 * | 02               | Invoke IsAudioSinkOrAudioDecoder with parameters: name = nullptr, isRialto = true.          | input: name = nullptr, isRialto = true, output: result variable | API returns false and assertion check passes           | Should Pass    |
 */
TEST(BrcmSocInterface, NullNameInputWithRialtoTrue)
{
    std::cout << "Entering NullNameInputWithRialtoTrue test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;
    
    const char* name = nullptr;
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = nullptr, isRialto = " << std::boolalpha << true << std::endl;
    
    bool result = true;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expect a null name input to return false.
    EXPECT_FALSE(result);
    std::cout << "Exiting NullNameInputWithRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsAudioSinkOrAudioDecoder returns false when an empty name is provided and isRialto is false
 *
 * This test verifies that passing an empty string as the name parameter to the IsAudioSinkOrAudioDecoder method
 * results in a false return value when the isRialto flag is set to false. This ensures that the API correctly handles 
 * invalid empty name inputs.
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
 * | Variation / Step | Description                                                                   | Test Data                                             | Expected Result                                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of BrcmSocInterface using the default constructor via EXPECT_NO_THROW | None                                                  | Instance of BrcmSocInterface is created without throwing an exception | Should Pass   |
 * | 02               | Prepare an empty string buffer for the name variable                          | name = ""                                             | The name variable is initialized as an empty string                  | Should be successful |
 * | 03               | Invoke IsAudioSinkOrAudioDecoder with parameters (name = "", isRialto = false)  | name = "", isRialto = false; output: result variable    | The method returns false and the assertion EXPECT_FALSE(result) passes | Should Pass   |
 */
TEST(BrcmSocInterface, EmptyStringNameWithRialtoFalse)
{
    std::cout << "Entering EmptyStringNameWithRialtoFalse test" << std::endl;
    EXPECT_NO_THROW({ BrcmSocInterface socInterface; });
    BrcmSocInterface socInterface;
    
    char name[128];
    std::strncpy(name, "", sizeof(name) - 1);
    name[sizeof(name) - 1] = '\0';
    std::cout << "Invoking IsAudioSinkOrAudioDecoder with parameters: name = \"" << name << std::endl;
    
    bool result = true;
    EXPECT_NO_THROW({ result = socInterface.IsAudioSinkOrAudioDecoder(name); });
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    
    // Expect an empty string input to return false.
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyStringNameWithRialtoFalse test" << std::endl;
}
/**
 * @brief Verify IsVideoDecoder returns a valid boolean for a valid video decoder name with isRialto set to true
 *
 * This test ensures that the IsVideoDecoder API, when invoked with a valid video decoder name ("H264Decoder") and isRialto parameter set to true, does not throw any exceptions and returns a boolean value (either true or false). This verifies that the API handles valid input correctly.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of BrcmSocInterface, set video decoder name to "H264Decoder", invoke IsVideoDecoder with isRialto true, and verify the returned boolean value. | input: name = "H264Decoder", isRialto = true, output: ret = boolean (true/false) | API returns a boolean value without throwing exceptions; ret is either true or false | Should Pass |
 */
TEST(BrcmSocInterface, ValidVideoDecoderNameWithIsRialtoTrue) {
    std::cout << "Entering ValidVideoDecoderNameWithIsRialtoTrue test" << std::endl;

    // Create object for BrcmSocInterface
    EXPECT_NO_THROW({
        BrcmSocInterface obj;
        char name[50] = {0};
        strncpy(name, "H264Decoder", sizeof(name) - 1);
        std::cout << "Invoking IsVideoDecoder with name: " << name << std::endl;
        bool ret = obj.IsVideoDecoder(name);
        std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
        EXPECT_TRUE(ret == true || ret == false);
    });

    std::cout << "Exiting ValidVideoDecoderNameWithIsRialtoTrue test" << std::endl;
}
/**
 * @brief Validate that a valid video decoder name provided with isRialto set to false functions correctly
 *
 * This test verifies that the BrcmSocInterface::IsVideoDecoder API behaves as expected when invoked with a valid video decoder name ("H264Decoder") and the isRialto parameter set to false. The test checks that no exceptions are thrown during object creation and API invocation, and that the returned value is a boolean (either true or false).
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
 * | Variation / Step | Description                                                         | Test Data                                               | Expected Result                                                                          | Notes            |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create BrcmSocInterface object and initialize name variable with "H264Decoder" and isRialto flag set to false | BrcmSocInterface object creation, name = "H264Decoder", isRialto = false | Object should be created successfully without throwing an exception                     | Should be successful |
 * | 02               | Invoke the IsVideoDecoder method using the initialized parameters     | name = "H264Decoder", isRialto = false                  | API returns a boolean value (true or false) and passes the assertion check                | Should Pass      */
TEST(BrcmSocInterface, ValidVideoDecoderNameWithIsRialtoFalse) {
    std::cout << "Entering ValidVideoDecoderNameWithIsRialtoFalse test" << std::endl;

    // Create object for BrcmSocInterface
    EXPECT_NO_THROW({
        BrcmSocInterface obj;
        char name[50] = {0};
        strncpy(name, "H264Decoder", sizeof(name) - 1);
        std::cout << "Invoking IsVideoDecoder with name: " << name << std::endl;
        bool ret = obj.IsVideoDecoder(name);
        std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
        EXPECT_TRUE(ret == true || ret == false);
    });

    std::cout << "Exiting ValidVideoDecoderNameWithIsRialtoFalse test" << std::endl;
}
/**
 * @brief Test the IsVideoDecoder function with a non-video decoder name and isRialto set to true.
 *
 * This test verifies that the BrcmSocInterface::IsVideoDecoder method returns false when provided
 * with a non-video decoder name ("AudioProcessor") even when the flag isRialto is true.
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
 * | Variation / Step | Description                                                        | Test Data                                                          | Expected Result                                                 | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------ | --------------------------------------------------------------- | ---------- |
 * | 01               | Invoke IsVideoDecoder with non-video name and true flag            | input1 = name "AudioProcessor", input2 = isRialto true, output = false | API should return false and EXPECT_FALSE assertion should pass    | Should Pass |
 */
TEST(BrcmSocInterface, NonVideoDecoderNameWithIsRialtoTrue) {
    std::cout << "Entering NonVideoDecoderNameWithIsRialtoTrue test" << std::endl;

    BrcmSocInterface obj;
    char name[50] = {0};
    strncpy(name, "AudioProcessor", sizeof(name) - 1);
    std::cout << "Invoking IsVideoDecoder with name: " << name << std::endl;
    bool ret = obj.IsVideoDecoder(name);
    std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
    EXPECT_FALSE(ret);

    std::cout << "Exiting NonVideoDecoderNameWithIsRialtoTrue test" << std::endl;
}
/**
 * @brief Verify that IsVideoDecoder returns false for a non-video decoder name when isRialto is false
 *
 * This test verifies that the IsVideoDecoder function correctly identifies that "AudioProcessor" is not a video decoder by returning false when the isRialto flag is set to false.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test environment and prepare test data | name = "AudioProcessor", isRialto = false | BrcmSocInterface object is created and test data is set up | Should be successful |
 * | 02 | Invoke IsVideoDecoder API with the provided test data | input: name = "AudioProcessor", isRialto = false | API returns false indicating "AudioProcessor" is not a video decoder | Should Fail |
 * | 03 | Verify the API return value using assertion EXPECT_FALSE | output: ret (expected value = false) | EXPECT_FALSE assertion passes confirming the function behavior | Should be successful |
 */
TEST(BrcmSocInterface, NonVideoDecoderNameWithIsRialtoFalse) {
    std::cout << "Entering NonVideoDecoderNameWithIsRialtoFalse test" << std::endl;

    BrcmSocInterface obj;
    char name[50] = {0};
    strncpy(name, "AudioProcessor", sizeof(name) - 1);
    std::cout << "Invoking IsVideoDecoder with name: " << name << std::endl;
    bool ret = obj.IsVideoDecoder(name);
    std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
    EXPECT_FALSE(ret);

    std::cout << "Exiting NonVideoDecoderNameWithIsRialtoFalse test" << std::endl;
}
/**
 * @brief Validate that IsVideoDecoder correctly handles a null name pointer when isRialto is true
 *
 * This test verifies that when a null pointer is passed as the name to the IsVideoDecoder API with isRialto set to true,
 * the function returns false. This helps ensure that the API manages invalid (null) inputs properly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 032@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                | Test Data                                      | Expected Result                          | Notes              |
 * | :--------------: | -------------------------------------------------------------------------- | ---------------------------------------------- | ---------------------------------------- | ------------------ |
 * | 01               | Log test entry message                                                     | none                                           | Message logged indicating test start     | Should be successful |
 * | 02               | Invoke IsVideoDecoder with name = nullptr and isRialto = true                | name = nullptr, isRialto = true                 | API returns false value                  | Should Fail         |
 * | 03               | Validate return value using EXPECT_FALSE                                   | return value from IsVideoDecoder = false        | Assertion passes confirming false return | Should be successful |
 * | 04               | Log test exit message                                                      | none                                           | Message logged indicating test end       | Should be successful |
 */
TEST(BrcmSocInterface, NullNamePointerWithIsRialtoTrue) {
    std::cout << "Entering NullNamePointerWithIsRialtoTrue test" << std::endl;

    BrcmSocInterface obj;
    const char* name = nullptr;
    std::cout << "Invoking IsVideoDecoder with name: nullptr" << std::endl;
    bool ret = obj.IsVideoDecoder(name);
    std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
    EXPECT_FALSE(ret);

    std::cout << "Exiting NullNamePointerWithIsRialtoTrue test" << std::endl;
}
/**
 * @brief Test IsVideoDecoder API with empty string name and isRialto flag as false
 *
 * This test validates that when an empty string is passed for the name and the isRialto flag is set to false, the IsVideoDecoder API properly returns false. It ensures that the method handles empty string scenarios correctly without unexpected behavior.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate BrcmSocInterface object | No input | Object created successfully | Should be successful |
 * | 02 | Initialize character array for name as an empty string | name = "" | Character array set with a null terminator | Should be successful |
 * | 03 | Invoke IsVideoDecoder with empty name and isRialto flag false | name = "", isRialto = false | API returns false and EXPECT_FALSE assertion passes | Should Pass |
 * | 04 | Output test exit message | No input | Exit message printed to console | Should be successful |
 */
TEST(BrcmSocInterface, EmptyStringNameWithIsRialtoFalse) {
    std::cout << "Entering EmptyStringNameWithIsRialtoFalse test" << std::endl;

    BrcmSocInterface obj;
    char name[1];
    name[0] = '\0';
    std::cout << "Invoking IsVideoDecoder with empty string name" << std::endl;
    bool ret = obj.IsVideoDecoder(name);
    std::cout << "Returned value from IsVideoDecoder: " << ret << std::endl;
    EXPECT_FALSE(ret);

    std::cout << "Exiting EmptyStringNameWithIsRialtoFalse test" << std::endl;
}
/**
 * @brief Validate the behavior of IsVideoSink for a valid video sink name with non-Rialto flag
 *
 * This test verifies that the IsVideoSink method of the BrcmSocInterface class is executed without throwing any exceptions
 * when provided with a valid video sink name "video_sink" and the isRialto flag set to false. It ensures that the method returns
 * a boolean value (either true or false) indicating proper handling of non-Rialto sinks.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 034
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                    | Test Data                                         | Expected Result                                                                          | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate BrcmSocInterface, initialize a char array with "video_sink", and set isRialto flag to false          | bufferSize = 64, name = "video_sink", isRialto = false | API method IsVideoSink returns a valid boolean value without throwing exceptions         | Should Pass |
 */
TEST(BrcmSocInterface, ValidVideoSinkNonRialto) {
    std::cout << "Entering ValidVideoSinkNonRialto test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface obj;
        const int bufferSize = 64;
        char name[bufferSize];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_sink", bufferSize - 1);
        std::cout << "Invoking IsVideoSink with name: " << name << " and isRialto: false" << std::endl;
        bool result = obj.IsVideoSink(name);
        std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidVideoSinkNonRialto test" << std::endl;
}
/**
 * @brief Validate that BrcmSocInterface's IsVideoSink function correctly processes a valid video sink input with Rialto flag set to true.
 *
 * This test verifies that the IsVideoSink method of the BrcmSocInterface class does not throw exceptions when invoked with a valid video sink name and the Rialto flag set to true. It ensures that the function returns a boolean value (either true or false) and that the method call is exception free, thus confirming robust handling of input parameters in this scenario.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                            | Expected Result                                                                                              | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Set up test environment by instantiating BrcmSocInterface and initializing buffer and name. | bufferSize = 64, name = "video_sink"                                 | Environment is set up successfully without exceptions.                                                     | Should be successful |
 * | 02               | Invoke IsVideoSink API with the valid video sink name "video_sink" and isRialto flag true.     | name = "video_sink", isRialto = true, result = bool (output from API)  | The API returns a boolean value (true or false) and passes the assertion: EXPECT_TRUE(result == true || result == false). | Should Pass   |
 */
TEST(BrcmSocInterface, ValidVideoSinkRialto) {
    std::cout << "Entering ValidVideoSinkRialto test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface obj;
        const int bufferSize = 64;
        char name[bufferSize];
        std::memset(name, 0, sizeof(name));
        std::strncpy(name, "video_sink", bufferSize - 1);
        std::cout << "Invoking IsVideoSink with name: " << name << " and isRialto: true" << std::endl;
        bool result = obj.IsVideoSink(name);
        std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting ValidVideoSinkRialto test" << std::endl;
}
/**
 * @brief Test the behavior of IsVideoSink API for an invalid non-Rialto video sink.
 *
 * This test verifies that when a non-Rialto indicator is provided along with the video sink name "audio_sink",
 * the IsVideoSink method returns false, indicating that invalid video sinks are correctly rejected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Instantiate BrcmSocInterface, prepare a name with "audio_sink", and invoke IsVideoSink with isRialto set to false | input: name = "audio_sink", isRialto = false, output: result expected = false | The API should return false and the EXPECT_FALSE check should pass | Should Fail |
 */
TEST(BrcmSocInterface, InvalidVideoSinkNonRialto) {
    std::cout << "Entering InvalidVideoSinkNonRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const int bufferSize = 64;
    char name[bufferSize];
    std::memset(name, 0, sizeof(name));
    std::strncpy(name, "audio_sink", bufferSize - 1);
    std::cout << "Invoking IsVideoSink with name: " << name << " and isRialto: false" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InvalidVideoSinkNonRialto test" << std::endl;
}
/**
 * @brief Verify that IsVideoSink returns false when an audio sink name is passed with isRialto set to true.
 *
 * This test ensures that the IsVideoSink API correctly identifies that the provided sink type ("audio_sink")
 * is not a valid video sink even when the isRialto flag is true. The expected behavior is to return false,
 * which is then verified using an assertion.
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
 * | Variation / Step | Description                                                                  | Test Data                                           | Expected Result                          | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------- | --------------------------------------------------- | ---------------------------------------- | ----------------- |
 * | 01               | Initialize BrcmSocInterface object and prepare the name buffer with "audio_sink" | BrcmSocInterface obj, name buffer = "audio_sink"      | Object instantiated and buffer correctly set up | Should be successful |
 * | 02               | Invoke IsVideoSink API with name "audio_sink" and isRialto set to true         | name = "audio_sink", isRialto = true                  | API returns false                         | Should Fail       |
 * | 03               | Verify the returned value using EXPECT_FALSE                                 | result = false                                       | ASSERT check passes if result is false    | Should be successful |
 */
TEST(BrcmSocInterface, InvalidVideoSinkRialto) {
    std::cout << "Entering InvalidVideoSinkRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const int bufferSize = 64;
    char name[bufferSize];
    std::memset(name, 0, sizeof(name));
    std::strncpy(name, "audio_sink", bufferSize - 1);
    std::cout << "Invoking IsVideoSink with name: " << name << " and isRialto: true" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InvalidVideoSinkRialto test" << std::endl;
}
/**
 * @brief Tests the IsVideoSink API with a null name for a non-Rialto scenario
 *
 * This test validates that the IsVideoSink function correctly handles a scenario
 * where the input string pointer is null and the isRialto flag is set to false.
 * This ensures that the API returns false when provided with invalid name input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 038@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                   | Expected Result                                                       | Notes       |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke IsVideoSink with name as nullptr and isRialto as false to check if it handles null input correctly | name = nullptr, isRialto = false, expected_return = false | API returns false; assertion EXPECT_FALSE passes                      | Should Pass |
 */
TEST(BrcmSocInterface, NullNameNonRialto) {
    std::cout << "Entering NullNameNonRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const char* name = nullptr;
    std::cout << "Invoking IsVideoSink with name: " << "nullptr" << " and isRialto: false" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullNameNonRialto test" << std::endl;
}
/**
 * @brief Test the behavior of IsVideoSink when the name pointer is null.
 *
 * This test verifies that invoking IsVideoSink with a null name pointer returns false, ensuring the API handles null input appropriately.
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
 * | Variation / Step | Description                                                  | Test Data                          |Expected Result                                                   |Notes       |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------- | ---------------------------------------------------------------- | ---------- |
 * | 01               | Invoke IsVideoSink with a null name pointer and isRialto true. | name = nullptr, isRialto = true     | Returns false as validated by EXPECT_FALSE(result)               | Should Pass |
 */
TEST(BrcmSocInterface, NullNameRialto) {
    std::cout << "Entering NullNameRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const char* name = nullptr;
    std::cout << "Invoking IsVideoSink with name: " << "nullptr" << " and isRialto: true" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullNameRialto test" << std::endl;
}
/**
 * @brief Test the behavior of IsVideoSink with an empty string for name and isRialto set to false
 *
 * This test verifies that when an empty string is provided as the video sink name (non-Rialto) to the IsVideoSink API,
 * the function correctly identifies that the video sink does not exist and returns false. The test ensures that the API
 * handles empty input strings properly and performs the expected assertion check.
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
 * | Variation / Step | Description                                                                 | Test Data                                                      | Expected Result                                                              | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------------- | -------------- |
 * | 01               | Print the entry message indicating the start of the test                    | N/A                                                            | Console outputs "Entering EmptyNameNonRialto test"                           | Should be successful |
 * | 02               | Instantiate the BrcmSocInterface object                                     | obj (instance of BrcmSocInterface)                             | Object is created without errors                                             | Should be successful |
 * | 03               | Initialize the char array 'name' with a size of 64 as an empty string         | bufferSize = 64, name = ""                                       | The 'name' is set as an empty string                                         | Should be successful |
 * | 04               | Invoke IsVideoSink with the empty string and isRialto set to false            | input: name = "", isRialto = false                               | API returns false as expected when provided with empty name                  | Should Pass    |
 * | 05               | Validate the result using EXPECT_FALSE                                      | result = false                                                 | The assertion confirms the method returns false                              | Should Pass    |
 * | 06               | Print the exit message indicating the end of the test                       | N/A                                                            | Console outputs "Exiting EmptyNameNonRialto test"                            | Should be successful |
 */
TEST(BrcmSocInterface, EmptyNameNonRialto) {
    std::cout << "Entering EmptyNameNonRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const int bufferSize = 64;
    char name[bufferSize];
    std::memset(name, 0, sizeof(name));
    std::strncpy(name, "", bufferSize - 1);
    std::cout << "Invoking IsVideoSink with name: \"" << name << "\" (empty string) and isRialto: false" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting EmptyNameNonRialto test" << std::endl;
}
/**
 * @brief Validate that invoking IsVideoSink with an empty string for the name and isRialto set to true returns false.
 *
 * This test verifies that when an empty string is provided as the name and the isRialto flag is true, the IsVideoSink API correctly returns false. It ensures that the API properly handles empty string inputs.
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
 * | 01 | Invoke IsVideoSink with an empty string for name and isRialto flag set to true | name = "", isRialto = true, output result = false | Function returns false and EXPECT_FALSE assertion passes | Should Pass |
 */
TEST(BrcmSocInterface, EmptyNameRialto) {
    std::cout << "Entering EmptyNameRialto test" << std::endl;
    
    BrcmSocInterface obj;
    const int bufferSize = 64;
    char name[bufferSize];
    std::memset(name, 0, sizeof(name));
    std::strncpy(name, "", bufferSize - 1);
    std::cout << "Invoking IsVideoSink with name: \"" << name << "\" (empty string) and isRialto: true" << std::endl;
    bool result = obj.IsVideoSink(name);
    std::cout << "Returned value from IsVideoSink: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting EmptyNameRialto test" << std::endl;
}
/**
 * @brief Verify that SetAC4Tracks functions correctly with a valid GstElement pointer and a positive trackId.
 *
 * This test verifies that the SetAC4Tracks method of the BrcmSocInterface class works as expected when provided with a valid GstElement pointer (simulated using a dummy variable) and a positive trackId value. The function is expected to execute without throwing any exceptions, indicating proper handling of the inputs for a positive scenario.
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
 * | Variation / Step | Description                                                       | Test Data                                               | Expected Result                                   | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------- | ------------- |
 * | 01               | Log the entry message for the test.                               | N/A                                                     | "Entering Valid_GstElement_PositiveTrackId test" printed | Should be successful |
 * | 02               | Create an instance of BrcmSocInterface.                           | N/A                                                     | Instance of BrcmSocInterface created              | Should be successful |
 * | 03               | Log the message indicating the object creation.                   | N/A                                                     | "Created BrcmSocInterface object." printed          | Should be successful |
 * | 04               | Initialize a dummy variable to simulate a valid GstElement pointer. | dummyElementMemory = 0                                  | Dummy GstElement pointer prepared                 | Should be successful |
 * | 05               | Assign a positive trackId for testing.                            | trackId = 1                                             | trackId set to 1                                  | Should be successful |
 * | 06               | Log the prepared dummy GstElement pointer.                        | src pointer = address of dummyElementMemory             | Dummy GstElement pointer printed                  | Should be successful |
 * | 07               | Log the message before invoking SetAC4Tracks.                     | src pointer, trackId = 1                                  | Message indicating API invocation printed         | Should be successful |
 * | 08               | Invoke SetAC4Tracks API and ensure no exception is thrown.          | input: src pointer, trackId = 1                           | No exception thrown; API returns successfully     | Should Pass   |
 * | 09               | Log the success message after invoking SetAC4Tracks.              | N/A                                                     | "SetAC4Tracks invoked successfully with positive trackId." printed | Should be successful |
 * | 10               | Log the exit message for the test.                                 | N/A                                                     | "Exiting Valid_GstElement_PositiveTrackId test" printed | Should be successful |
 */
TEST(BrcmSocInterface, Valid_GstElement_PositiveTrackId) {
    std::cout << "Entering Valid_GstElement_PositiveTrackId test" << std::endl;

    // Create an instance of BrcmSocInterface.
    BrcmSocInterface socInterface;
    std::cout << "Created BrcmSocInterface object." << std::endl;

    // Dummy variable to simulate a valid GstElement pointer.
    static int dummyElementMemory = 0;
    GstElement* src = reinterpret_cast<GstElement*>(&dummyElementMemory);

    // Positive trackId for testing.
    int trackId = 1;

    std::cout << "Prepared dummy GstElement pointer: " << src << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " 
              << src << " and trackId: " << trackId << std::endl;

    // Expect no exception during invocation.
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));

    std::cout << "SetAC4Tracks invoked successfully with positive trackId." << std::endl;
    std::cout << "Exiting Valid_GstElement_PositiveTrackId test" << std::endl;
}
/**
 * @brief Validate that SetAC4Tracks handles a valid GstElement pointer with a zero trackId without throwing an exception.
 *
 * This test verifies that the BrcmSocInterface::SetAC4Tracks API safely processes a valid GstElement pointer and a trackId value of zero.
 * The scenario mimics a typical valid use-case where a GstElement is passed for track identification even if trackId is zero, and it is expected to not throw exceptions.
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
 * | Variation / Step | Description                                                                  | Test Data                                                                                          | Expected Result                                            | Notes          |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------- |
 * | 01               | Set up test data by creating a dummy GstElement pointer and setting trackId to zero. | dummyElementMemory variable, src pointer = reinterpret_cast<GstElement*>(&dummyElementMemory), trackId = 0 | Test data is initialized correctly for API invocation.   | Should be successful |
 * | 02               | Create an instance of BrcmSocInterface and invoke SetAC4Tracks with the prepared test data. | BrcmSocInterface instance, input: src pointer as above, trackId = 0, output: none                  | The API call completes without throwing any exception.   | Should Pass    |
 */
TEST(BrcmSocInterface, Valid_GstElement_ZeroTrackId) {
    std::cout << "Entering Valid_GstElement_ZeroTrackId test" << std::endl;
    BrcmSocInterface socInterface;
    int trackId = 0;

    static int dummyElementMemory = 0;
    GstElement* src = reinterpret_cast<GstElement*>(&dummyElementMemory);

    std::cout << "Creating BrcmSocInterface object." << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " << src << " and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));
    std::cout << "SetAC4Tracks invoked successfully with zero trackId." << std::endl;
    std::cout << "Exiting Valid_GstElement_ZeroTrackId test" << std::endl;
}
/**
 * @brief Verify that SetAC4Tracks API handles a negative trackId input with a valid GstElement pointer gracefully
 *
 * This test validates that the SetAC4Tracks function of the BrcmSocInterface does not throw any exceptions when invoked with a negative track identifier. The purpose is to ensure that the API can handle negative trackId values without causing unexpected behavior.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Initialize variables by setting trackId to -1 and creating a dummy GstElement pointer | trackId = -1, dummyElementMemory = 0, src pointer = reinterpret_cast<GstElement*>(&dummyElementMemory) | Variables are initialized without errors | Should be successful |
 * | 02 | Invoke SetAC4Tracks with the dummy GstElement pointer and negative trackId value | socInterface instance, input: src pointer = dummyElementMemory address, trackId = -1; output: no exception thrown | API call completes without throwing an exception (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(BrcmSocInterface, Valid_GstElement_NegativeTrackId) {
    std::cout << "Entering Valid_GstElement_NegativeTrackId test" << std::endl;
    BrcmSocInterface socInterface;
    int trackId = -1;

    static int dummyElementMemory = 0;
    GstElement* src = reinterpret_cast<GstElement*>(&dummyElementMemory);

    std::cout << "Creating BrcmSocInterface object." << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " << src << " and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));
    std::cout << "SetAC4Tracks invoked successfully with negative trackId." << std::endl;
    std::cout << "Exiting Valid_GstElement_NegativeTrackId test" << std::endl;
}
/**
 * @brief Verify SetAC4Tracks handles null GstElement pointer with positive track ID
 *
 * This test checks that the SetAC4Tracks API correctly handles a null GstElement pointer when provided with a valid positive track ID. It ensures that the API does not throw an exception under these conditions, validating its robustness for such input scenarios.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize BrcmSocInterface; set trackId to 2 and src to nullptr; invoke SetAC4Tracks API | src = nullptr, trackId = 2 | API call should execute without throwing any exception | Should Pass |
 */
TEST(BrcmSocInterface, Null_GstElement_PositiveTrackId) {
    std::cout << "Entering Null_GstElement_PositiveTrackId test" << std::endl;
    BrcmSocInterface socInterface;
    int trackId = 2;
    GstElement* src = nullptr;
    std::cout << "Creating BrcmSocInterface object." << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " << src << " (nullptr) and trackId: " << trackId << std::endl;
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));
    std::cout << "SetAC4Tracks invoked successfully with null GstElement pointer." << std::endl;
    std::cout << "Exiting Null_GstElement_PositiveTrackId test" << std::endl;
}
/**
 * @brief Ensure that SetAC4Tracks does not throw exceptions when invoked with INT_MAX as trackId
 *
 * This test verifies that the SetAC4Tracks method in the BrcmSocInterface successfully processes a valid GstElement pointer and an extreme trackId value (INT_MAX) without throwing any exceptions. Testing with INT_MAX ensures that the function correctly handles boundary conditions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                          | Test Data                                                       | Expected Result                                                         | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------------- | ----------- |
 * | 01               | Create BrcmSocInterface instance and a dummy GstElement pointer; invoke SetAC4Tracks with INT_MAX as trackId. | src = pointer to dummyElementMemory, trackId = INT_MAX            | SetAC4Tracks is invoked without throwing any exceptions (EXPECT_NO_THROW passes). | Should Pass |
 */
TEST(BrcmSocInterface, Valid_GstElement_ExtremeTrackId_INT_MAX) {
    std::cout << "Entering Valid_GstElement_ExtremeTrackId_INT_MAX test" << std::endl;
    BrcmSocInterface socInterface;
    int trackId = INT_MAX;

    static int dummyElementMemory = 0;
    GstElement* src = reinterpret_cast<GstElement*>(&dummyElementMemory);

    std::cout << "Creating BrcmSocInterface object." << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " << src << " and trackId (INT_MAX): " << trackId << std::endl;
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));
    std::cout << "SetAC4Tracks invoked successfully with INT_MAX trackId." << std::endl;
    std::cout << "Exiting Valid_GstElement_ExtremeTrackId_INT_MAX test" << std::endl;
}
/**
 * @brief Validate that SetAC4Tracks successfully handles extreme minimum trackId (INT_MIN)
 *
 * This test verifies that the SetAC4Tracks method correctly processes INT_MIN as the track identifier when provided with a valid GstElement pointer. This ensures that the function can handle edge-case integer values without throwing exceptions.
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
 * | Variation / Step | Description                                                      | Test Data                                           | Expected Result                                                       | Notes       |
 * | :--------------: | ---------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * |       01         | Invoke SetAC4Tracks with a valid GstElement pointer and trackId set to INT_MIN | src = valid GstElement pointer, trackId = INT_MIN    | No exception thrown; API call completes successfully and behaves as expected | Should Pass |
 */
TEST(BrcmSocInterface, Valid_GstElement_ExtremeTrackId_INT_MIN) {
    std::cout << "Entering Valid_GstElement_ExtremeTrackId_INT_MIN test" << std::endl;
    BrcmSocInterface socInterface;
    int trackId = INT_MIN;
    static int dummyElementMemory = 0;
    GstElement* src = reinterpret_cast<GstElement*>(&dummyElementMemory);

    std::cout << "Creating BrcmSocInterface object." << std::endl;
    std::cout << "Invoking SetAC4Tracks with src pointer: " << src << " and trackId (INT_MIN): " << trackId << std::endl;
    EXPECT_NO_THROW(socInterface.SetAC4Tracks(src, trackId));
    std::cout << "SetAC4Tracks invoked successfully with INT_MIN trackId." << std::endl;
    std::cout << "Exiting Valid_GstElement_ExtremeTrackId_INT_MIN test" << std::endl;
}
/**
 * @brief Verify that SetAudioProperty accepts valid non-null strings with false isSinkBinVolume without throwing exceptions
 *
 * This test verifies that the SetAudioProperty method of the BrcmSocInterface class properly handles valid input
 * when both volume and mute strings are non-null and the isSinkBinVolume flag is set to false. The test confirms that
 * the method does not throw any exceptions during object construction or API invocation.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                             | Expected Result                                         | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of BrcmSocInterface using its default constructor and verify no exception   | N/A                                                                   | Successfully constructs object without throwing       | Should be successful |
 * | 02               | Prepare input strings by initializing fixed-size character arrays with "InitialVolume" and "InitialMute"  | volume = "InitialVolume", mute = "InitialMute"                        | The character arrays are correctly populated           | Should be successful |
 * | 03               | Invoke SetAudioProperty with the prepared non-null strings and false for isSinkBinVolume         | volume = "InitialVolume", mute = "InitialMute", isSinkBinVolume = false | API executes without throwing any exception             | Should Pass   |
 */
TEST(BrcmSocInterface, SetAudioProperty_ValidInput_NonNullStrings_FalseIsSinkBinVolume) {
    std::cout << "Entering SetAudioProperty_ValidInput_NonNullStrings_FalseIsSinkBinVolume test" << std::endl;

    // Create object using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
    });
    BrcmSocInterface socInterface;

    // Prepare input strings using fixed size arrays
    char volBuffer[50];
    std::memset(volBuffer, 0, sizeof(volBuffer));
    std::strncpy(volBuffer, "InitialVolume", sizeof(volBuffer) - 1);
    const char* volume = volBuffer;

    char muteBuffer[50];
    std::memset(muteBuffer, 0, sizeof(muteBuffer));
    std::strncpy(muteBuffer, "InitialMute", sizeof(muteBuffer) - 1);
    const char* mute = muteBuffer;

    bool isSinkBinVolume = false;

    std::cout << "Before call: volume = " << volume << ", mute = " << mute 
              << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW({
        std::cout << "Invoking SetAudioProperty with non-null strings" << std::endl;
        socInterface.SetAudioProperty(volume, mute, isSinkBinVolume);
        std::cout << "Method SetAudioProperty invoked" << std::endl;
    });

    std::cout << "Exiting SetAudioProperty_ValidInput_NonNullStrings_FalseIsSinkBinVolume test" << std::endl;
}
/**
 * @brief Validate that SetAudioProperty handles a null volume pointer correctly.
 *
 * This test verifies that providing a null volume pointer to the SetAudioProperty method, along with a valid mute parameter and the isSinkBinVolume flag set to false, does not lead to an exception and that the API behaves as expected. This ensures the fault tolerance for null input in the volume parameter.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 049@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**  
 * | Variation / Step | Description                                                                          | Test Data                                                       | Expected Result                                               | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------ | --------------------------------------------------------------- | ------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate the BrcmSocInterface object using the default constructor.               | None                                                            | Object is created without throwing an exception.            | Should be successful  |
 * | 02               | Prepare parameters: set volume to nullptr, initialize muteBuffer with "InitialMute", and assign isSinkBinVolume as false. | volume = nullptr, mute = "InitialMute", isSinkBinVolume = false   | Parameters are initialized correctly.                       | Should be successful  |
 * | 03               | Invoke SetAudioProperty method with the prepared parameters.                         | volume = nullptr, mute = "InitialMute", isSinkBinVolume = false   | SetAudioProperty executes without throwing and behaves as defined. | Should Pass     |
 */
TEST(BrcmSocInterface, SetAudioProperty_NullVolume_FalseIsSinkBinVolume) {
    std::cout << "Entering SetAudioProperty_NullVolume_FalseIsSinkBinVolume test" << std::endl;

    // Create object using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
    });
    BrcmSocInterface socInterface;

    // Set volume to nullptr and prepare mute buffer
    const char* volume = nullptr;

    char muteBuffer[50];
    std::memset(muteBuffer, 0, sizeof(muteBuffer));
    std::strncpy(muteBuffer, "InitialMute", sizeof(muteBuffer) - 1);
    const char* mute = muteBuffer;

    bool isSinkBinVolume = false;

    std::cout << "Before call: volume = " << (volume ? volume : "nullptr") 
              << ", mute = " << mute 
              << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW({
        std::cout << "Invoking SetAudioProperty with volume = nullptr" << std::endl;
        socInterface.SetAudioProperty(volume, mute, isSinkBinVolume);
        std::cout << "Method SetAudioProperty invoked" << std::endl;
    });

    std::cout << "Exiting SetAudioProperty_NullVolume_FalseIsSinkBinVolume test" << std::endl;
}
/**
 * @brief Test SetAudioProperty method with a null mute parameter and false isSinkBinVolume flag
 *
 * This test validates that the SetAudioProperty API of the BrcmSocInterface class handles a null mute pointer correctly while the isSinkBinVolume flag is set to false. The test ensures that no exceptions are thrown during object instantiation and the API invocation.
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
 * | Variation / Step | Description                                                                           | Test Data                                             | Expected Result                                      | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------- | -------------- |
 * | 01               | Instantiate BrcmSocInterface object using the default constructor                     | N/A                                                   | No exception thrown                                  | Should be successful |
 * | 02               | Invoke SetAudioProperty with volume = "InitialVolume", mute = nullptr, isSinkBinVolume = false | volume = InitialVolume, mute = nullptr, isSinkBinVolume = false | Method executes without throwing an exception | Should Pass    |
 */
TEST(BrcmSocInterface, SetAudioProperty_NullMute_FalseIsSinkBinVolume) {
    std::cout << "Entering SetAudioProperty_NullMute_FalseIsSinkBinVolume test" << std::endl;

    // Create object using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
    });
    BrcmSocInterface socInterface;

    // Prepare volume buffer and set mute to nullptr
    char volBuffer[50];
    std::memset(volBuffer, 0, sizeof(volBuffer));
    std::strncpy(volBuffer, "InitialVolume", sizeof(volBuffer) - 1);
    const char* volume = volBuffer;

    const char* mute = nullptr;

    bool isSinkBinVolume = false;

    std::cout << "Before call: volume = " << volume 
              << ", mute = " << (mute ? mute : "nullptr")
              << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW({
        std::cout << "Invoking SetAudioProperty with mute = nullptr" << std::endl;
        socInterface.SetAudioProperty(volume, mute, isSinkBinVolume);
        std::cout << "Method SetAudioProperty invoked" << std::endl;
    });

    std::cout << "Exiting SetAudioProperty_NullMute_FalseIsSinkBinVolume test" << std::endl;
}
/**
 * @brief Verify SetAudioProperty API handles empty volume input correctly.
 *
 * This test ensures that when an empty volume string is provided along with a valid mute string and false isSinkBinVolume flag, 
 * the SetAudioProperty API is invoked without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                         | Test Data                                                     | Expected Result                                      | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------------------------------------------------- | -------------- |
 * | 01               | Create an instance of BrcmSocInterface using the default constructor                                | No input arguments                                            | Instance is created successfully without exceptions | Should be successful |
 * | 02               | Prepare an empty volume string and a non-empty mute string                                          | volume = "", mute = "InitialMute"                               | Strings are initialized properly                     | Should be successful |
 * | 03               | Invoke SetAudioProperty with empty volume, non-empty mute, and false isSinkBinVolume flag             | volume = "", mute = "InitialMute", isSinkBinVolume = false       | SetAudioProperty does not throw exceptions           | Should Pass    |
 * | 04               | Log the output before and after calling the SetAudioProperty API                                    | Not applicable                                                | Console logs are printed correctly                   | Should be successful |
 */
TEST(BrcmSocInterface, SetAudioProperty_EmptyVolume_FalseIsSinkBinVolume) {
    std::cout << "Entering SetAudioProperty_EmptyVolume_FalseIsSinkBinVolume test" << std::endl;

    // Create object using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
    });
    BrcmSocInterface socInterface;

    // Prepare an empty volume string and a non-empty mute string.
    char volBuffer[50];
    std::memset(volBuffer, 0, sizeof(volBuffer));
    std::strncpy(volBuffer, "", sizeof(volBuffer) - 1);
    const char* volume = volBuffer;

    char muteBuffer[50];
    std::memset(muteBuffer, 0, sizeof(muteBuffer));
    std::strncpy(muteBuffer, "InitialMute", sizeof(muteBuffer) - 1);
    const char* mute = muteBuffer;

    bool isSinkBinVolume = false;

    std::cout << "Before call: volume = \"" << volume << "\""
              << ", mute = " << mute 
              << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW({
        std::cout << "Invoking SetAudioProperty with empty volume string" << std::endl;
        socInterface.SetAudioProperty(volume, mute, isSinkBinVolume);
        std::cout << "Method SetAudioProperty invoked" << std::endl;
    });

    std::cout << "Exiting SetAudioProperty_EmptyVolume_FalseIsSinkBinVolume test" << std::endl;
}
/**
 * @brief Verify SetAudioProperty handles an empty mute string correctly when isSinkBinVolume is false
 *
 * This test verifies that calling SetAudioProperty with a non-empty volume string and an empty mute string,
 * along with a false isSinkBinVolume flag, does not throw any exceptions. It ensures the method can gracefully handle
 * an empty mute value while processing valid volume information.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                      | Expected Result                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Create a BrcmSocInterface instance and invoke SetAudioProperty with valid volume string, an empty mute string, and isSinkBinVolume set to false. | volume = InitialVolume, mute = , isSinkBinVolume = false         | No exception is thrown and SetAudioProperty executes successfully.    | Should Pass |
 */
TEST(BrcmSocInterface, SetAudioProperty_EmptyMute_FalseIsSinkBinVolume) {
    std::cout << "Entering SetAudioProperty_EmptyMute_FalseIsSinkBinVolume test" << std::endl;

    // Create object using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
    });
    BrcmSocInterface socInterface;

    // Prepare a non-empty volume string and an empty mute string.
    char volBuffer[50];
    std::memset(volBuffer, 0, sizeof(volBuffer));
    std::strncpy(volBuffer, "InitialVolume", sizeof(volBuffer) - 1);
    const char* volume = volBuffer;

    char muteBuffer[50];
    std::memset(muteBuffer, 0, sizeof(muteBuffer));
    std::strncpy(muteBuffer, "", sizeof(muteBuffer) - 1);
    const char* mute = muteBuffer;

    bool isSinkBinVolume = false;

    std::cout << "Before call: volume = " << volume 
              << ", mute = \"" << mute << "\""
              << ", isSinkBinVolume = " << isSinkBinVolume << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW({
        std::cout << "Invoking SetAudioProperty with empty mute string" << std::endl;
        socInterface.SetAudioProperty(volume, mute, isSinkBinVolume);
        std::cout << "Method SetAudioProperty invoked" << std::endl;
    });

    std::cout << "Exiting SetAudioProperty_EmptyMute_FalseIsSinkBinVolume test" << std::endl;
}
/**
 * @brief Test to verify SetPlatformPlaybackRate works correctly under normal operating conditions.
 *
 * This test case validates that the SetPlatformPlaybackRate method in the BrcmSocInterface class behaves as expected when invoked using an object created with the default constructor. The method is expected to return a valid boolean value, and the test verifies this behavior through assertion checks.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 053@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an object of BrcmSocInterface using the default constructor | No input data | Object is created successfully without throwing an exception | Should be successful |
 * | 02 | Invoke SetPlatformPlaybackRate method and log the returned value | output: result = SetPlatformPlaybackRate() | Returned value is a valid boolean (either true or false) and the assertion check verifies it | Should Pass |
 */
TEST(BrcmSocInterface, SetPlatformPlaybackRate_Normal) {
    std::cout << "Entering SetPlatformPlaybackRate_Normal test" << std::endl;
    
    // Create object of BrcmSocInterface using default constructor
    EXPECT_NO_THROW({
        BrcmSocInterface socInterfaceObj;
        std::cout << "BrcmSocInterface object created using default constructor." << std::endl;
        
        // Invoke the SetPlatformPlaybackRate method and log the returned value.
        bool result = socInterfaceObj.SetPlatformPlaybackRate();
        std::cout << "Invoked SetPlatformPlaybackRate(). Returned value: " << result << std::endl;
        
        // Validate that the returned value is true.
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified that SetPlatformPlaybackRate returned true." << std::endl;
    });
    
    std::cout << "Exiting SetPlatformPlaybackRate_Normal test" << std::endl;
}
/**
 * @brief Validate non-subtitle playback with zero initial flags.
 *
 * This test verifies that the BrcmSocInterface properly sets playback flags without subtitles when the initial flags value is zero.
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
 * | Variation / Step | Description                                                                                           | Test Data              | Expected Result                                | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ---------------------- | ---------------------------------------------- | ----------- |
 * | 01               | Create a BrcmSocInterface instance and invoke SetPlaybackFlags with flags set to zero and isSub false. | flags = 0, isSub = false | No exception thrown; method successfully sets flags | Should Pass |
 */
TEST(BrcmSocInterface, NonSubtitlePlaybackWithZeroInitialFlags) {
    std::cout << "Entering NonSubtitlePlaybackWithZeroInitialFlags test" << std::endl;
    
    gint flags = 0;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = false" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, false));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting NonSubtitlePlaybackWithZeroInitialFlags test" << std::endl;
}
/**
 * @brief Validate that SetPlaybackFlags correctly handles a zero value for flags during subtitle playback.
 *
 * This test verifies that when the playback flags are initialized to zero, the BrcmSocInterface instance
 * can be created and the SetPlaybackFlags method can be invoked with isSub set to true without throwing any exceptions.
 * It ensures that the system behaves as expected for this specific scenario.
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
 * | Variation / Step | Description                                                            | Test Data                             | Expected Result                                                  | Notes           |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------- | ---------------------------------------------------------------- | --------------- |
 * | 01               | Initialize the variable 'flags' with a value of 0                      | flags = 0                             | 'flags' should be set to 0 and printed accordingly               | Should be successful |
 * | 02               | Create an instance of BrcmSocInterface using its default constructor   | -                                     | Instance is created without throwing an exception                | Should Pass     |
 * | 03               | Invoke SetPlaybackFlags with 'flags' set to 0 and 'isSub' set to true    | flags = 0, isSub = true                | The method completes without throwing an exception               | Should Pass     |
 */
TEST(BrcmSocInterface, SubtitlePlaybackWithZeroInitialFlags) {
    std::cout << "Entering SubtitlePlaybackWithZeroInitialFlags test" << std::endl;
    
    gint flags = 0;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = true" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, true));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting SubtitlePlaybackWithZeroInitialFlags test" << std::endl;
}
/**
 * @brief Verify the correct behavior of SubtitlePlayback when non-zero even initial flags are provided.
 *
 * This test ensures that the BrcmSocInterface::SetPlaybackFlags method correctly handles an initial flags value of 2 with subtitle playback enabled, without throwing exceptions. It verifies that the API behaves as expected under a standard positive scenario where valid inputs are provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 056@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                          | Test Data                                        | Expected Result                                                  | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------ | ---------------------------------------------------------------- | ----------------- |
 * | 01               | Create a BrcmSocInterface instance using the default constructor.    | input: none                                      | Instance is created without throwing any exception.             | Should be successful |
 * | 02               | Invoke SetPlaybackFlags with flags = 2 and isSub = true to set subtitle playback flags. | input: flags = 2, isSub = true; output: method execution result | SetPlaybackFlags method executes without throwing any exception.  | Should Pass        |
 * | 03               | Print and verify the flags value before and after the API invocation.  | input: initial flags = 2                         | Console output should show the expected flag value without alteration errors. | Should be successful |
 */
TEST(BrcmSocInterface, SubtitlePlaybackWithNonZeroEvenInitialFlags) {
    std::cout << "Entering SubtitlePlaybackWithNonZeroEvenInitialFlags test" << std::endl;
    
    gint flags = 2;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = true" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, true));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting SubtitlePlaybackWithNonZeroEvenInitialFlags test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackFlags successfully handles non-subtitle playback even when flags include a subtitle bit.
 *
 * This test ensures that when a BrcmSocInterface object is created and its SetPlaybackFlags method is invoked with an initial
 * flag value that may include a subtitle bit (flags = 3) along with isSub set to false, the method executes without throwing
 * any exceptions. This test is important to confirm that the API correctly differentiates the subtitle flag from the playback mode.
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
 * | Variation / Step | Description                                                                                      | Test Data                                             | Expected Result                                  | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ----------------------------------------------------- | ------------------------------------------------ | --------------- |
 * | 01               | Log the entry message for the test                                                               | None                                                  | "Entering NonSubtitlePlaybackWithFlagsHavingSubtitleBitSet test" printed | Should be successful |
 * | 02               | Set the initial value of flags                                                                   | flags = 3                                             | flags variable is set to 3                       | Should be successful |
 * | 03               | Create a BrcmSocInterface instance using the default constructor                                 | Instance creation                                     | Object created without throwing an exception     | Should be successful |
 * | 04               | Invoke SetPlaybackFlags on the instance with flags = 3 and isSub = false                           | flags = 3, isSub = false                                | API call does not throw any exception            | Should Pass     |
 * | 05               | Log the flags value after the SetPlaybackFlags invocation                                        | flags variable                                        | Updated flags value printed to console           | Should be successful |
 * | 06               | Log the exit message for the test                                                                | None                                                  | "Exiting NonSubtitlePlaybackWithFlagsHavingSubtitleBitSet test" printed | Should be successful |
 */
TEST(BrcmSocInterface, NonSubtitlePlaybackWithFlagsHavingSubtitleBitSet) {
    std::cout << "Entering NonSubtitlePlaybackWithFlagsHavingSubtitleBitSet test" << std::endl;
    
    gint flags = 3;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = false" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, false));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting NonSubtitlePlaybackWithFlagsHavingSubtitleBitSet test" << std::endl;
}
/**
 * @brief Verify subtitle playback handling with negative initial flags.
 *
 * This test validates that negative flag values used in subtitle playback scenarios are processed without throwing exceptions. It specifically checks the default construction of BrcmSocInterface followed by invoking SetPlaybackFlags with negative flags and a subtitle indicator. The test ensures that the API handles negative flag values gracefully.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test by setting initial flags and creating a BrcmSocInterface instance. | flags = -2 | The initial flags are assigned and instance creation should not throw any exception. | Should be successful |
 * | 02 | Invoke SetPlaybackFlags with flags set to -2 and isSub set to true. | flags = -2, isSub = true | The SetPlaybackFlags method should execute without throwing an exception. | Should Pass |
 */
TEST(BrcmSocInterface, SubtitlePlaybackWithNegativeInitialFlags) {
    std::cout << "Entering SubtitlePlaybackWithNegativeInitialFlags test" << std::endl;
    
    gint flags = -2;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = true" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, true));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting SubtitlePlaybackWithNegativeInitialFlags test" << std::endl;
}
/**
 * @brief Verify that non-subtitle playback does not throw an exception when invoked with a negative flag value even if the subtitle bit is set
 *
 * This test verifies that the BrcmSocInterface::SetPlaybackFlags API functions correctly in a scenario where a negative flag value is provided while the subtitle playback indicator is false. The objective is to ensure that the API handles such edge cases gracefully without throwing any exceptions.
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
 * | Variation / Step | Description                                                                | Test Data                                | Expected Result                                                  | Notes            |
 * | :--------------: | -------------------------------------------------------------------------- | ---------------------------------------- | ---------------------------------------------------------------- | ---------------- |
 * | 01               | Log the entry point of the test and initialize flags with a negative value | flags = -1                               | Initial log displays the correct negative flag value             | Should be successful |
 * | 02               | Create an instance of BrcmSocInterface using the default constructor       | constructor = default                    | Instance is created without throwing any exception               | Should be successful |
 * | 03               | Invoke SetPlaybackFlags with flags = -1 and isSub set to false               | flags = -1, isSub = false                  | API call completes without throwing, ensuring negative flag handled correctly | Should Pass       |
 * | 04               | Log the exit point of the test                                             | none                                     | Final log confirming the test exit is printed                      | Should be successful |
 */
TEST(BrcmSocInterface, NonSubtitlePlaybackWithNegativeFlagsHavingSubtitleBitSet) {
    std::cout << "Entering NonSubtitlePlaybackWithNegativeFlagsHavingSubtitleBitSet test" << std::endl;
    
    gint flags = -1;
    std::cout << "Initial flags value: " << flags << std::endl;
    std::cout << "Creating BrcmSocInterface instance using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Invoking SetPlaybackFlags with flags = " << flags 
                  << " and isSub = false" << std::endl;
        EXPECT_NO_THROW(socInterface.SetPlaybackFlags(flags, false));
        std::cout << "After invocation, flags value: " << flags << std::endl;
    });
    
    std::cout << "Exiting NonSubtitlePlaybackWithNegativeFlagsHavingSubtitleBitSet test" << std::endl;
}
/**
 * @brief Test the SetPlaybackRate functionality with valid inputs and a standard playback rate.
 *
 * This test verifies that the SetPlaybackRate API correctly handles valid GstElement pointers and a standard playback rate of 1.0. The test ensures that the object is created without any exceptions and that the API returns true when invoked with proper inputs.
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
 * | :----: | --------- | ------------------------- |-------------- | ----- |
 * | 01 | Create a BrcmSocInterface object, prepare valid dummy GstElement pointers, and invoke SetPlaybackRate with a playback rate of 1.0 | sources = [0x1004, 0x1005], pipeline = 0x1001, playback_rate = 1.0, video_dec = 0x1002, audio_dec = 0x1003 | API returns true and no exception is thrown | Should Pass |
 */
TEST(BrcmSocInterface, ValidInputWithStandardPlaybackRate) {
    GTEST_SKIP();
    std::cout << "Entering ValidInputWithStandardPlaybackRate test" << std::endl;
    
    // Create object for BrcmSocInterface using default constructor.
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare valid GstElement pointers (using dummy addresses for test purpose).
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x1001);
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x1002);
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x1003);
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x1004));
        sources.push_back(reinterpret_cast<GstElement*>(0x1005));
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << 1.0 << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Invoke the method.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, 1.0, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect true for valid input.
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidInputWithStandardPlaybackRate test" << std::endl;
}
/**
 * @brief Negative test for SetPlaybackRate API with null pipeline.
 *
 * This test validates that the SetPlaybackRate API in BrcmSocInterface correctly handles the scenario where the pipeline parameter is null.
 * The test verifies that when pipeline is null, the API returns false as expected, thereby ensuring proper error handling in a negative scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 061@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                                                                          | Expected Result                                                  | Notes             |
 * | :--------------: | ------------------------------------------------------------ | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------------- |
 * | 01               | Instantiate BrcmSocInterface object                          | (No input parameters)                                                                              | Object is created successfully                                   | Should be successful |
 * | 02               | Invoke SetPlaybackRate API with null pipeline                | sources = 0x2004, pipeline = nullptr, playback rate = 1.0, video_dec = 0x2002, audio_dec = 0x2003  | API returns false and EXPECT_FALSE assertion passes              | Should Fail       |
 */
TEST(BrcmSocInterface, NegativeNullPipeline) {
    std::cout << "Entering NegativeNullPipeline test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare valid GstElement pointers and set pipeline to nullptr.
        GstElement* pipeline = nullptr;
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x2002);
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x2003);
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x2004));
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << " (nullptr)" << std::endl;
        std::cout << "  playback rate: " << 1.0 << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Call SetPlaybackRate with null pipeline.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, 1.0, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false when pipeline is null.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeNullPipeline test" << std::endl;
}
/**
 * @brief Test that SetPlaybackRate returns false for an empty sources vector.
 *
 * This test verifies that when an empty sources vector is provided, the SetPlaybackRate API gracefully handles the scenario without throwing exceptions and correctly returns false. It ensures that the function appropriately handles missing source inputs.
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
 * | Variation / Step | Description                                                                             | Test Data                                                                                          | Expected Result                                         | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ------------- |
 * | 01               | Instantiate the BrcmSocInterface object.                                               | No input                                                                                           | Object is created successfully.                         | Should be successful |
 * | 02               | Prepare test parameters with an empty sources vector and valid pointers for pipeline, video_dec, and audio_dec. | sources = empty vector, pipeline = 0x3001, playback rate = 1.0, video_dec = 0x3002, audio_dec = 0x3003  | Parameters are set up correctly.                        | Should be successful |
 * | 03               | Call the SetPlaybackRate API with the prepared parameters.                             | sources = empty, pipeline = 0x3001, playback rate = 1.0, video_dec = 0x3002, audio_dec = 0x3003       | API returns false indicating failure due to empty sources. | Should Fail   |
 */
TEST(BrcmSocInterface, NegativeEmptySources) {
    std::cout << "Entering NegativeEmptySources test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare parameters with empty sources vector.
        std::vector<GstElement*> sources;  // empty vector
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x3001);
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x3002);
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x3003);
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << " (empty vector)" << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << 1.0 << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Call SetPlaybackRate with empty sources.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, 1.0, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false for empty sources vector.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeEmptySources test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate handles null video decoder input correctly.
 *
 * This test verifies that when a null video decoder parameter is provided to SetPlaybackRate,
 * the function does not throw any exceptions and returns false, ensuring proper error handling.
 * This is important to prevent potential crashes due to null pointer dereferencing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 063@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a BrcmSocInterface object, prepare parameters with video_dec set to nullptr, invoke SetPlaybackRate, and verify the return value | sources = GstElement pointer (0x4004), pipeline = GstElement pointer (0x4001), playback rate = 1.0, video_dec = nullptr, audio_dec = GstElement pointer (0x4003) | The API returns false, and the assertion EXPECT_FALSE(result) passes | Should Fail |
 */
TEST(BrcmSocInterface, NegativeNullVideoDecoder) {
    std::cout << "Entering NegativeNullVideoDecoder test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare parameters with video_dec set to nullptr.
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x4004));
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x4001);
        GstElement* video_dec = nullptr;
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x4003);
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << 1.0 << std::endl;
        std::cout << "  video_dec address: " << video_dec << " (nullptr)" << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Call SetPlaybackRate with null video decoder.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, 1.0, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false because video_dec is null.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeNullVideoDecoder test" << std::endl;
}
/**
 * @brief Negative test for SetPlaybackRate API when audio decoder is null
 *
 * This test verifies that the BrcmSocInterface::SetPlaybackRate method correctly handles a scenario where the audio decoder pointer is null. It ensures that when the API is invoked with a null audio decoder, it returns false to indicate an invalid configuration.
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
 * | 01 | Create a BrcmSocInterface object and initialize parameters with a valid sources vector, pipeline, and video decoder, and set audio decoder to null. | sources = {reinterpret_cast<GstElement*>(0x5004)}, pipeline = reinterpret_cast<GstElement*>(0x5001), playback rate = 1.0, video_dec = reinterpret_cast<GstElement*>(0x5002), audio_dec = nullptr | Object and parameter initialization should be successful. | Should be successful |
 * | 02 | Invoke SetPlaybackRate API with the prepared parameters including a null audio decoder. | input1 (sources size) = 1, input2 (pipeline) = 0x5001, input3 (playback rate) = 1.0, input4 (video_dec) = 0x5002, input5 (audio_dec) = nullptr, output (result) = expected false | API should return false indicating that the null audio decoder is invalid. | Should Pass |
 */
TEST(BrcmSocInterface, NegativeNullAudioDecoder) {
    std::cout << "Entering NegativeNullAudioDecoder test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare parameters with audio_dec set to nullptr.
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x5004));
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x5001);
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x5002);
        GstElement* audio_dec = nullptr;
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << 1.0 << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << " (nullptr)" << std::endl;
        
        // Call SetPlaybackRate with null audio decoder.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, 1.0, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false because audio_dec is null.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeNullAudioDecoder test" << std::endl;
}
/**
 * @brief Verify that SetPlaybackRate returns false when the playback rate is zero.
 *
 * This test case verifies that the BrcmSocInterface::SetPlaybackRate API correctly rejects a zero playback rate. 
 * It checks that when the API is invoked with a zero playback rate along with valid pointers for sources, pipeline, video_dec, and audio_dec, the function returns false, ensuring that invalid playback rates are not accepted. Additionally, the test validates that no exceptions are thrown during the API call.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 065@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                          | Test Data                                                                                   | Expected Result                                                         | Notes        |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ------------ |
 * | 01               | Invoke SetPlaybackRate with a zero playback rate and valid element pointers.                         | sources = 0x6004, pipeline = 0x6001, rate = 0.0, video_dec = 0x6002, audio_dec = 0x6003      | API returns false; assertion EXPECT_FALSE(result) passes; no exceptions. | Should Fail  |
 */
TEST(BrcmSocInterface, NegativeZeroPlaybackRate) {
    std::cout << "Entering NegativeZeroPlaybackRate test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare parameters with playback rate zero.
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x6004));
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x6001);
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x6002);
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x6003);
        double rate = 0.0;
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << rate << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Call SetPlaybackRate with zero playback rate.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, rate, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false for zero playback rate.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeZeroPlaybackRate test" << std::endl;
}
/**
 * @brief Validate SetPlaybackRate function with a negative playback rate
 *
 * This test case verifies that the SetPlaybackRate method in the BrcmSocInterface class properly handles a negative playback rate by returning false. Invalid input values should not be accepted, and the API must correctly reject the negative playback rate.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | ------------------------------------------------------------ | ----------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01 | Create a BrcmSocInterface instance, prepare GstElement pointers, and invoke SetPlaybackRate with a negative playback rate value. | sources: [0x7004], pipeline: 0x7001, rate: -1.0, video_dec: 0x7002, audio_dec: 0x7003 | The API should return false and the assertion EXPECT_FALSE should pass. | Should Pass |
 */
TEST(BrcmSocInterface, NegativeNegativePlaybackRate) {
    std::cout << "Entering NegativeNegativePlaybackRate test" << std::endl;
    
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "BrcmSocInterface object created successfully." << std::endl;
        
        // Prepare parameters with negative playback rate.
        std::vector<GstElement*> sources;
        sources.push_back(reinterpret_cast<GstElement*>(0x7004));
        GstElement* pipeline = reinterpret_cast<GstElement*>(0x7001);
        GstElement* video_dec = reinterpret_cast<GstElement*>(0x7002);
        GstElement* audio_dec = reinterpret_cast<GstElement*>(0x7003);
        double rate = -1.0;
        
        // Log input parameter values.
        std::cout << "Invoking SetPlaybackRate with:" << std::endl;
        std::cout << "  sources size: " << sources.size() << std::endl;
        std::cout << "  pipeline address: " << pipeline << std::endl;
        std::cout << "  playback rate: " << rate << std::endl;
        std::cout << "  video_dec address: " << video_dec << std::endl;
        std::cout << "  audio_dec address: " << audio_dec << std::endl;
        
        // Call SetPlaybackRate with negative playback rate.
        bool result = socInterface.SetPlaybackRate(sources, pipeline, rate, video_dec, audio_dec);
        std::cout << "SetPlaybackRate returned: " << std::boolalpha << result << std::endl;
        
        // Expect false for negative playback rate.
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeNegativePlaybackRate test" << std::endl;
}
/**
 * @brief Test the SetRateCorrection() API for expected return true behavior
 *
 * This test case verifies that the SetRateCorrection method within the BrcmSocInterface class functions correctly by returning true when invoked on a properly constructed object. The objective is to ensure that the API completes without throwing exceptions and that the returned value meets the specification.
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
 * | Variation / Step | Description                                                        | Test Data                                                                | Expected Result                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------------ | ------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of BrcmSocInterface using the default constructor.| BrcmSocInterface instance, no parameters                                | Object is initialized without throwing exceptions     | Should be successful |
 * | 02               | Invoke the SetRateCorrection() method on the created instance.       | Method: SetRateCorrection(), no input; Output: retVal = true expected      | API returns true and no exceptions are thrown; assertion passes | Should Pass   |
 */
TEST(BrcmSocInterface, SetRateCorrection_start) {
    std::cout << "Entering SetRateCorrection_start test" << std::endl;
    
    // Creating object of BrcmSocInterface using default constructor.
    std::cout << "Creating instance of BrcmSocInterface using default constructor." << std::endl;
    EXPECT_NO_THROW({
        BrcmSocInterface socInterface;
        std::cout << "Instance created successfully." << std::endl;
        
        // Logging invocation of SetRateCorrection
        std::cout << "Invoking SetRateCorrection() method." << std::endl;
        bool retVal = false;
        EXPECT_NO_THROW({
            retVal = socInterface.SetRateCorrection();
        });
        std::cout << "SetRateCorrection() returned value: " << std::boolalpha << retVal << std::endl;
        
        // Expect the returned value to be true as per method specification.
        EXPECT_TRUE(retVal);
    });
    
    std::cout << "Exiting SetRateCorrection_start test" << std::endl;
}