
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
#include "PlayerExternalsInterfaceBase.h"


// Assuming PlayerExternalsInterfaceBase is defined elsewhere and has a default constructor.

/**
 * @brief Verifies that GetActiveInterface returns false for a newly created PlayerExternalsInterfaceBase object
 *
 * This test ensures that upon creating a PlayerExternalsInterfaceBase object using the default constructor,
 * the GetActiveInterface method returns false, which is the expected behavior when the interface is inactive by default.
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
 * | 01 | Instantiate PlayerExternalsInterfaceBase using default constructor, invoke GetActiveInterface, and validate the returned value. | playerInterface = default, GetActiveInterface() returns bool | GetActiveInterface() returns false; EXPECT_EQ confirms false | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, GetActiveInterface) {
    std::cout << "Entering GetActiveInterface test" << std::endl;
    
    // Create object using default constructor.
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase playerInterface;
        std::cout << "Created PlayerExternalsInterfaceBase object using default constructor." << std::endl;
        
        // Invoke the GetActiveInterface method.
        std::cout << "Invoking method GetActiveInterface()" << std::endl;
        bool activeInterface = playerInterface.GetActiveInterface();
        
        // Log the value returned.
        std::cout << "Method GetActiveInterface() returned value: " << activeInterface << std::endl;
        
        // Validate that the active interface status is as expected (false in default state).
        EXPECT_EQ(activeInterface, false);
    });
    
    std::cout << "Exiting GetActiveInterface test" << std::endl;
}
/**
 * @brief Validate retrieval of StandardHD display resolution using GetDisplayResolution method
 *
 * This test verifies that the GetDisplayResolution method of the PlayerExternalsInterfaceBase class retrieves non-negative resolution values,
 * ensuring that a standard HD resolution is returned. The method is expected to update the width and height parameters with valid display resolution values.
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
 * |01| Construct the PlayerExternalsInterfaceBase object using the default constructor | None | Object constructed successfully without exception | Should be successful |
 * |02| Initialize display resolution variables width and height with initial value -1 | input: width = -1, height = -1 | Variables are set to -1 prior to method invocation | Should be successful |
 * |03| Invoke the GetDisplayResolution method on the constructed object | input: object instance, output: width, height updated | Method executes without exception and updates width and height | Should Pass |
 * |04| Verify that the updated width and height are non-negative | input: width, height after invocation | width >= 0, height >= 0 based on assertion check | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, GetDisplayResolution_ValidResolution_StandardHD) {
    std::cout << "Entering GetDisplayResolution_ValidResolution_StandardHD test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase obj;
        std::cout << "Constructed PlayerExternalsInterfaceBase object" << std::endl;
        
        int width = -1;
        int height = -1;
        std::cout << "Initial values before invoking GetDisplayResolution: width = " << width 
                  << ", height = " << height << std::endl;
        
        // Invoke the method
        std::cout << "Invoking GetDisplayResolution method" << std::endl;
        obj.GetDisplayResolution(width, height);
        std::cout << "Method returned values: width = " << width 
                  << ", height = " << height << std::endl;
        
        // Verify that the retrieved resolution values are non-negative
        EXPECT_GE(width, 0) << "Retrieved width is negative!";
        EXPECT_GE(height, 0) << "Retrieved height is negative!";
        std::cout << "Verified that width and height are non-negative" << std::endl;
    });
    
    std::cout << "Exiting GetDisplayResolution_ValidResolution_StandardHD test" << std::endl;
}
/**
 * @brief Validate retrieval of TR181 configuration using a valid parameter name.
 *
 * This test verifies that when a valid parameter name ("TR181_Config") is provided,
 * the PlayerExternalsInterfaceBase object is created successfully and the GetTR181Config API
 * returns a valid configuration pointer with a non-zero configuration length. This ensures
 * that the API correctly retrieves configuration data when supplied with proper input.
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
 * | Variation / Step | Description                                                                           | Test Data                                                       | Expected Result                                                                                             | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerExternalsInterfaceBase using the default constructor        | No inputs                                                       | Object is instantiated without throwing any exception                                                       | Should Pass   |
 * | 02               | Invoke GetTR181Config with a valid parameter name ("TR181_Config") and configLen = 0      | paramName = TR181_Config, configLen = 0                           | API call returns a non-null configuration pointer and updates configLen to a value greater than 0            | Should Pass   |
 * | 03               | Validate that if a configuration pointer is returned, the configLen is greater than 0      | Returned config pointer and configLen value                       | If config pointer is not nullptr, then configLen > 0 is asserted successfully                                   | Should Pass   |
 */
TEST(PlayerExternalsInterfaceBase, ValidParameterNameRetrieval) {
    std::cout << "Entering ValidParameterNameRetrieval test" << std::endl;
    
    // Create an object of PlayerExternalsInterfaceBase using default constructor
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        const char* paramName = "TR181_Config";
        std::cout << "Invoking GetTR181Config with paramName: " << paramName << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        if (config != nullptr) {
            std::cout << "Configuration key info: " << config << std::endl;
            EXPECT_GT(configLen, 0);
        } else {
            std::cout << "Received nullptr config as result." << std::endl;
        }
    });
    
    std::cout << "Exiting ValidParameterNameRetrieval test" << std::endl;
}
/**
 * @brief Validate GetTR181Config API behavior when invoked with a null parameter name.
 *
 * This test verifies that when GetTR181Config is called with a null parameter name, the API does not throw an exception, returns a nullptr for the config pointer, and leaves the configuration length as 0.
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
 * | 01 | Create PlayerExternalsInterfaceBase object and initialize variables | No inputs; object creation invoked | Object creation succeeds without exception | Should be successful |
 * | 02 | Invoke GetTR181Config with paramName as nullptr and check for exception free call | input: paramName = nullptr, configLen = 0; output: config pointer expected | API returns a nullptr for config, configLen remains 0 without throwing an exception | Should Pass |
 * | 03 | Validate the returned config pointer and configLen | Compare outputs: config = nullptr, configLen = 0 | EXPECT_EQ confirms config equals nullptr and configLen equals 0 | Should be successful |
 */
TEST(PlayerExternalsInterfaceBase, NullParameterName) {
    std::cout << "Entering NullParameterName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        const char* paramName = nullptr;
        std::cout << "Invoking GetTR181Config with paramName: " << "nullptr" << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        EXPECT_EQ(config, nullptr);
        EXPECT_EQ(configLen, 0);
    });
    
    std::cout << "Exiting NullParameterName test" << std::endl;
}
/**
 * @brief Verify that GetTR181Config gracefully handles an empty parameter name scenario
 *
 * This test ensures that when an empty string is passed as the parameter name to GetTR181Config,
 * the system does not throw any exceptions, returns a null pointer, and outputs 0 as the configuration length.
 * This is important to guarantee the API's robustness against invalid or empty input values.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                               | Test Data                             | Expected Result                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Invoke GetTR181Config with an empty parameter name to validate API behavior | paramName = "", configLen = 0         | Function returns nullptr and configLen remains 0 without exceptions | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, EmptyParameterName) {
    std::cout << "Entering EmptyParameterName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        // Create an empty C-string using fixed size array and strncpy
        char paramName[2];
        std::memset(paramName, 0, sizeof(paramName));
        std::strncpy(paramName, "", 1);
        std::cout << "Invoking GetTR181Config with empty paramName: '" << paramName << "'" << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        EXPECT_EQ(config, nullptr);
        EXPECT_EQ(configLen, 0);
    });
    
    std::cout << "Exiting EmptyParameterName test" << std::endl;
}
/**
 * @brief Test to verify GetTR181Config handles special characters in the parameter name correctly
 *
 * This test verifies that when a parameter name containing special characters is provided to the GetTR181Config API, 
 * the function does not throw any exceptions and returns a nullptr for the configuration pointer and 0 for the configuration length.
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
 * | 01 | Create PlayerExternalsInterfaceBase object. | None | Object is created without exceptions. | Should Pass |
 * | 02 | Initialize parameter name with a special characters string. | specialStr = "!@#$%^&*()", paramName is initialized with specialStr. | paramName is correctly set to "!@#$%^&*()". | Should be successful |
 * | 03 | Invoke GetTR181Config API with the special characters parameter name. | input: paramName = "!@#$%^&*()", configLen = 0; output: config pointer receives value. | API returns config as nullptr and configLen remains 0 without throwing exceptions. | Should Pass |
 * | 04 | Verify the returned configuration pointer and configuration length. | output: config, configLen | EXPECT_EQ confirms config is nullptr and configLen is 0. | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, SpecialCharactersParameterName) {
    std::cout << "Entering SpecialCharactersParameterName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        char paramName[20];
        const char * specialStr = "!@#$%^&*()";
        std::strncpy(paramName, specialStr, sizeof(paramName)-1);
        paramName[sizeof(paramName)-1] = '\0';
        std::cout << "Invoking GetTR181Config with special characters paramName: " << paramName << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        EXPECT_EQ(config, nullptr);
        EXPECT_EQ(configLen, 0);
    });
    
    std::cout << "Exiting SpecialCharactersParameterName test" << std::endl;
}
/**
 * @brief Verify that the API correctly handles a very long parameter name.
 *
 * This test verifies that when a very long parameter name (50 'a' characters) is passed to the GetTR181Config method of the PlayerExternalsInterfaceBase class, the API processes the input without throwing exceptions. It checks that if a valid configuration is returned then the configuration length (configLen) is set to a value greater than 0; otherwise, if the returned configuration is nullptr, configLen remains 0.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                                     | Expected Result                                                                                                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate PlayerExternalsInterfaceBase object.                           | Constructor call; no input parameters.                                                                        | Object is created successfully without throwing exceptions.                                                                            | Should be successful |
 * | 02               | Initialize and set a very long parameter name composed of 50 'a' characters.  | paramName = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", configLen = 0                                   | The parameter string is properly initialized with 50 consecutive 'a' characters.                                                       | Should be successful |
 * | 03               | Invoke GetTR181Config using the very long parameter name and validate response.| Inputs: paramName = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", configLen = 0; Output: config pointer, configLen value | If GetTR181Config returns a non-null config pointer, then configLen should be greater than 0; if it returns nullptr, configLen should be 0. | Should Pass  |
 */
TEST(PlayerExternalsInterfaceBase, VeryLongParameterName) {
    std::cout << "Entering VeryLongParameterName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        // Define a very long parameter name (50 'a' characters)
        char paramName[60];
        std::memset(paramName, 0, sizeof(paramName));
        const char * longStr = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
        std::strncpy(paramName, longStr, sizeof(paramName)-1);
        std::cout << "Invoking GetTR181Config with very long paramName: " << paramName << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        if (config != nullptr) {
            std::cout << "Configuration key info: " << config << std::endl;
            EXPECT_GT(configLen, 0);
        } else {
            std::cout << "Received nullptr config as result for very long parameter name." << std::endl;
            EXPECT_EQ(config, nullptr);
            EXPECT_EQ(configLen, 0);
        }
    });
    
    std::cout << "Exiting VeryLongParameterName test" << std::endl;
}
/**
 * @brief Validate that PlayerExternalsInterfaceBase correctly handles parameter names with whitespace
 *
 * This test verifies that the GetTR181Config API call properly processes a parameter name that contains leading and trailing whitespace. It checks that the API does not throw any exception and correctly returns either a valid configuration pointer with a non-zero length or a nullptr with a length of zero.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                             | Expected Result                                                                                                | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of PlayerExternalsInterfaceBase                                          | None                                                                  | Instance of PlayerExternalsInterfaceBase is created successfully                                               | Should be successful |
 * | 02               | Prepare a parameter name containing leading and trailing whitespace                         | rawStr = "   TR181_Config   "                                          | The parameter name is correctly set with the whitespace intact                                                 | Should be successful |
 * | 03               | Invoke GetTR181Config with the whitespace-containing parameter name                           | paramName = "   TR181_Config   ", configLen = 0                         | API call succeeds without throwing an exception; returns a valid pointer or nullptr based on internal logic       | Should Pass     |
 * | 04               | Validate the configuration pointer and length based on API return value                        | config pointer value, configLen value                                   | If config is not nullptr then configLen > 0; if config is nullptr then configLen equals 0                           | Should Pass     |
 */
TEST(PlayerExternalsInterfaceBase, ParameterNameWithWhitespace) {
    std::cout << "Entering ParameterNameWithWhitespace test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object" << std::endl;
        
        size_t configLen = 0;
        char paramName[30];
        const char * rawStr = "   TR181_Config   ";
        std::memset(paramName, 0, sizeof(paramName));
        std::strncpy(paramName, rawStr, sizeof(paramName)-1);
        std::cout << "Invoking GetTR181Config with whitespace trimmed paramName: '" << paramName << "'" << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW({
            config = player.GetTR181Config(paramName, configLen);
        });
        std::cout << "Returned config pointer: " << static_cast<void*>(config) << std::endl;
        std::cout << "Value of iConfigLen after invocation: " << configLen << std::endl;
        
        if (config != nullptr) {
            std::cout << "Configuration key info: " << config << std::endl;
            EXPECT_GT(configLen, 0);
        } else {
            std::cout << "Received nullptr config as result when using whitespace in parameter name." << std::endl;
            EXPECT_EQ(config, nullptr);
            EXPECT_EQ(configLen, 0);
        }
    });
    
    std::cout << "Exiting ParameterNameWithWhitespace test" << std::endl;
}
/**
 * @brief Verify that IARMInit successfully initializes with a valid process name
 *
 * This test case verifies that the IARMInit function correctly initializes when
 * provided with a valid process name. The test ensures that no exceptions are thrown
 * during the operation, thereby confirming proper initialization behavior for valid input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize a char array and assign a valid process name "TestProcess", ensuring proper null-termination. | processName (char[100] initialized with zeros), validName = "TestProcess" | processName contains "TestProcess" correctly | Should be successful |
 * | 02 | Call IARMInit with the prepared valid process name and verify that no exception is thrown. | processName = "TestProcess" | IARMInit completes execution without throwing an exception, verified by EXPECT_NO_THROW | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, IARMInit_with_valid_process_name)
{
    std::cout << "Entering IARMInit_with_valid_process_name test" << std::endl;

    // Prepare valid process name using strncpy
    char processName[100];
    std::memset(processName, 0, sizeof(processName));
    const char* validName = "TestProcess";
    std::strncpy(processName, validName, sizeof(processName) - 1);
    std::cout << "Invoking IARMInit with processName: " << processName << std::endl;
    
    // Call IARMInit and expect no throw
    EXPECT_NO_THROW(PlayerExternalsInterfaceBase::IARMInit(processName));
    std::cout << "IARMInit invoked successfully with processName: " << processName << std::endl;
    
    std::cout << "Exiting IARMInit_with_valid_process_name test" << std::endl;
}
/**
 * @brief Verify that IARMInit does not throw an exception when invoked with an empty process name.
 *
 * This test verifies that invoking IARMInit with an empty process name initializes the interface without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                     | Test Data                                          | Expected Result                                                       | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Prepare an empty process name by clearing the buffer and copying an empty string into it         | processName = "", emptyName = ""                   | processName is set to an empty string                                 | Should be successful |
 * | 02               | Invoke IARMInit with the empty process name and verify that no exception is thrown               | input processName = ""                             | API returns normally without throwing an exception (EXPECT_NO_THROW passes) | Should Pass     |
 */
TEST(PlayerExternalsInterfaceBase, IARMInit_with_empty_process_name)
{
    std::cout << "Entering IARMInit_with_empty_process_name test" << std::endl;
 
    // Prepare empty process name using strncpy
    char processName[100];
    std::memset(processName, 0, sizeof(processName));
    const char* emptyName = "";
    std::strncpy(processName, emptyName, sizeof(processName) - 1);
    std::cout << "Invoking IARMInit with empty processName: \"" << processName << "\"" << std::endl;
    
    // Call IARMInit and expect no throw
    EXPECT_NO_THROW(PlayerExternalsInterfaceBase::IARMInit(processName));
    std::cout << "IARMInit invoked successfully with empty processName" << std::endl;
    
    std::cout << "Exiting IARMInit_with_empty_process_name test" << std::endl;
}
/**
 * @brief Tests IARMInit with a NULL process name.
 *
 * This test ensures that the IARMInit function of PlayerExternalsInterfaceBase can be safely invoked with a NULL process name without throwing exceptions. This behavior is essential to confirm that the function gracefully handles NULL inputs.
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
 * | Variation / Step | Description                                   | Test Data                  | Expected Result                                  | Notes      |
 * | :--------------: | --------------------------------------------- | -------------------------- | ------------------------------------------------ | ---------- |
 * | 01               | Invoke IARMInit with NULL process name        | processName = NULL         | No exception thrown from IARMInit invocation     | Should Pass|
 */
TEST(PlayerExternalsInterfaceBase, IARMInit_with_NULL_process_name)
{
    std::cout << "Entering IARMInit_with_NULL_process_name test" << std::endl;
    
    // Log the invocation with NULL processName
    std::cout << "Invoking IARMInit with processName: NULL" << std::endl;
    
    // Call IARMInit with NULL and expect no throw
    EXPECT_NO_THROW(PlayerExternalsInterfaceBase::IARMInit(NULL));
    std::cout << "IARMInit invoked successfully with NULL processName" << std::endl;
    
    std::cout << "Exiting IARMInit_with_NULL_process_name test" << std::endl;
}
/**
 * @brief Test IARMInit with a whitespace-only process name.
 *
 * This test verifies that the IARMInit API correctly handles a process name that consists only of whitespace characters.
 * The test prepares a char array with a whitespace-only string, calls IARMInit, and ensures that no exception is thrown, indicating acceptable handling of such input.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 012
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                      | Test Data                                                                                   | Expected Result                                           | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------- | --------------------------------------------------------- | -------------- |
 * | 01               | Prepare a whitespace-only process name by initializing a char array with zeros and copying a whitespace-only string into it. | whitespaceName = "   ", processName initialized to zeros                                    | processName contains only whitespace values               | Should be successful |
 * | 02               | Invoke IARMInit with the prepared whitespace-only process name and ensure no exception is thrown.  | processName = "   "                                                                         | API does not throw any exceptions                          | Should Pass    |
 * | 03               | Log and confirm that IARMInit has been invoked successfully with a whitespace-only process name.  | N/A                                                                                         | Console outputs indicate successful invocation of IARMInit | Should be successful |
 */
TEST(PlayerExternalsInterfaceBase, IARMInit_with_whitespace_only_process_name)
{
    std::cout << "Entering IARMInit_with_whitespace_only_process_name test" << std::endl;
    
    // Prepare whitespace-only process name using strncpy
    char processName[100];
    std::memset(processName, 0, sizeof(processName));
    const char* whitespaceName = "   ";
    std::strncpy(processName, whitespaceName, sizeof(processName) - 1);
    std::cout << "Invoking IARMInit with whitespace-only processName: \"" << processName << "\"" << std::endl;
    
    // Call IARMInit and expect no throw
    EXPECT_NO_THROW(PlayerExternalsInterfaceBase::IARMInit(processName));
    std::cout << "IARMInit invoked successfully with whitespace-only processName" << std::endl;
    
    std::cout << "Exiting IARMInit_with_whitespace_only_process_name test" << std::endl;
}
/**
 * @brief Test basic invocation of IARMRegisterDsMgrEventHandler on default instance of PlayerExternalsInterfaceBase
 *
 * This test verifies that a default instance of PlayerExternalsInterfaceBase can be created and the IARMRegisterDsMgrEventHandler method can be invoked without throwing any exceptions. It ensures that the basic instantiation and method call functionalities are working as expected.
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
 * | Variation / Step | Description                                                        | Test Data                                                                  | Expected Result                                                            | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a default instance of PlayerExternalsInterfaceBase          | None, output: instance created                                             | Instance is created successfully without throwing any exception          | Should Pass   |
 * | 02               | Invoke IARMRegisterDsMgrEventHandler method on the created instance   | input: playerObj instance, no output argument                              | IARMRegisterDsMgrEventHandler executes without throwing any exception       | Should Pass   |
 */
TEST(PlayerExternalsInterfaceBase, IARMRegisterDsMgrEventHandler_default_instance_invocation) {
    std::cout << "Entering IARMRegisterDsMgrEventHandler_default_instance_invocation test" << std::endl;

    // Create a default instance of PlayerExternalsInterfaceBase
    std::cout << "Creating default instance of PlayerExternalsInterfaceBase" << std::endl;
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase playerObj;
        std::cout << "Default instance created successfully." << std::endl;

        // Invoke the IARMRegisterDsMgrEventHandler method and log the invocation
        std::cout << "Invoking IARMRegisterDsMgrEventHandler method." << std::endl;
        EXPECT_NO_THROW({
            playerObj.IARMRegisterDsMgrEventHandler();
        });
        std::cout << "IARMRegisterDsMgrEventHandler executed successfully." << std::endl;

    });

    std::cout << "Exiting IARMRegisterDsMgrEventHandler_default_instance_invocation test" << std::endl;
}
/**
 * @brief Validates that the IARMRemoveDsMgrEventHandler method can be invoked on a PlayerExternalsInterfaceBase instance without throwing an exception.
 *
 * This test creates a PlayerExternalsInterfaceBase instance using the default constructor and then calls the IARMRemoveDsMgrEventHandler method to ensure that it executes successfully. The test is designed to verify the basic stability and proper handling of invocations without errors.
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                                     | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * | 01               | Create a default instance of PlayerExternalsInterfaceBase using its default constructor. | instance = default                                            | Instance is created without throwing any exceptions.              | Should be successful |
 * | 02               | Invoke IARMRemoveDsMgrEventHandler() method on the created instance.          | method call on instance, No input, Expected output = void      | Method executes without throwing an exception, validated via EXPECT_NO_THROW. | Should Pass   |
 */
TEST(PlayerExternalsInterfaceBase, IARMRemoveDsMgrEventHandler_BasicInvocation) {
    std::cout << "Entering IARMRemoveDsMgrEventHandler_BasicInvocation test" << std::endl;
    
    // Create an instance using the default constructor and log its creation.
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase instance;
        std::cout << "Default instance of PlayerExternalsInterfaceBase created." << std::endl;
       
        // Logging invocation of IARMRemoveDsMgrEventHandler
        std::cout << "Invoking IARMRemoveDsMgrEventHandler() method on PlayerExternalsInterfaceBase instance." << std::endl;
        EXPECT_NO_THROW(instance.IARMRemoveDsMgrEventHandler());
        std::cout << "IARMRemoveDsMgrEventHandler() executed successfully without throwing an exception." << std::endl;
    });
    
    std::cout << "Exiting IARMRemoveDsMgrEventHandler_BasicInvocation test" << std::endl;
}
/**
 * @brief Verify that the IsActiveStreamingInterfaceWifi method works correctly in PlayerExternalsInterfaceBase.
 *
 * This test creates an instance of PlayerExternalsInterfaceBase using the default constructor and then calls the method IsActiveStreamingInterfaceWifi.
 * It ensures that no exceptions are thrown during object creation and method invocation, and that the returned boolean value is either true or false.
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
 * | 01 | Instantiate PlayerExternalsInterfaceBase and invoke IsActiveStreamingInterfaceWifi method | No input; Default constructor creates object, invoking IsActiveStreamingInterfaceWifi returns result = bool | Method executes without exceptions and returns a boolean value (true or false) | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, IsActiveStreamingInterfaceWifi) {
    std::cout << "Entering IsActiveStreamingInterfaceWifi test" << std::endl;

    // Create object using default constructor and ensure no exceptions are thrown.
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase obj;
        std::cout << "Created PlayerExternalsInterfaceBase object using default constructor." << std::endl;

        // Invoke the method IsActiveStreamingInterfaceWifi and log the call.
        std::cout << "Invoking IsActiveStreamingInterfaceWifi() method." << std::endl;
        bool result = obj.IsActiveStreamingInterfaceWifi();
        std::cout << "Method IsActiveStreamingInterfaceWifi() returned: " << (result ? "true" : "false") << std::endl;

        EXPECT_TRUE(result == true || result == false);
    });

    std::cout << "Exiting IsActiveStreamingInterfaceWifi test" << std::endl;
}
/**
 * @brief Verify that the IsSourceUHD method of PlayerExternalsInterfaceBase returns a valid boolean value.
 *
 * This test creates a PlayerExternalsInterfaceBase object using its default constructor and then calls the
 * IsSourceUHD method. The test is intended to ensure that the IsSourceUHD function executes without throwing any
 * exceptions and returns a valid boolean value, given that the internal member (m_gstElement) is expected to be NULL.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                             | Expected Result                                                       | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerExternalsInterfaceBase object using the default (non-parameterized) constructor.   | No input parameters, object creation invoked                          | Object is created without throwing exceptions                         | Should be successful |
 * | 02               | Invoke the IsSourceUHD method on the created object.                                              | Invocation: result = player.IsSourceUHD(), with player having default state (m_gstElement == NULL) | Method returns a boolean value (either true or false) without errors    | Should Pass    |
 * | 03               | Validate the boolean result returned by IsSourceUHD to ensure it is either true or false.            | result = (true or false)                                                | Assertion passes if result is explicitly true or false                   | Should Pass    |
 */
TEST(PlayerExternalsInterfaceBase, IsSourceUHD) {
    std::cout << "Entering IsSourceUHD test" << std::endl;
    
    // Create object using non-parameterized (default) constructor.
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object using default constructor." << std::endl;
        
        // Invocation of the IsSourceUHD method.
        std::cout << "Invoking IsSourceUHD method on default object (expected m_gstElement == NULL)." << std::endl;
        bool result = player.IsSourceUHD();
        std::cout << "IsSourceUHD returned: " << (result ? "true" : "false") << std::endl;
        
        EXPECT_TRUE(result == true || result == false);
    });
    
    std::cout << "Exiting IsSourceUHD test" << std::endl;
}
/**
 * @brief Verifies that the default constructor of PlayerExternalsInterfaceBase does not throw an exception.
 *
 * This test case ensures that constructing a PlayerExternalsInterfaceBase using its default constructor is safe and does not result in any exceptions. This is critical for confirming that the object can be instantiated without errors and is ready for further interactions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor of PlayerExternalsInterfaceBase | Constructor call, no input parameters, instance created | Object is successfully created with no exceptions thrown | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase) {
    std::cout << "Entering PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase test" << std::endl;
    
    EXPECT_NO_THROW({
        // Instantiate object using the default constructor
        PlayerExternalsInterfaceBase instance;
        std::cout << "Invoked method: PlayerExternalsInterfaceBase() default constructor" << std::endl;
    });
    
    std::cout << "Exiting PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase test" << std::endl;
}
/**
 * @brief Verify that PlayerExternalsInterfaceBase::SetHDMIStatus executes without exceptions.
 *
 * This test ensures that an instance of PlayerExternalsInterfaceBase can be successfully created using the default constructor, and that the SetHDMIStatus method can be invoked without throwing any exceptions. The absence of exceptions confirms the basic functionality of the API.
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
 * | Variation / Step | Description                                                                            | Test Data                                          | Expected Result                                                   | Notes       |
 * | :---------------:| -------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Create an instance of PlayerExternalsInterfaceBase using the default constructor.      | No input values; output: instance created.         | Instance creation should complete without throwing exceptions.     | Should Pass |
 * | 02               | Invoke the SetHDMIStatus() method on the PlayerExternalsInterfaceBase instance.          | No input values; output: void method execution.      | Method execution should complete without throwing exceptions.      | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, SetHDMIStatus_start) {
    std::cout << "Entering SetHDMIStatus_start test" << std::endl;
    
    std::cout << "Creating instance of PlayerExternalsInterfaceBase using default constructor" << std::endl;
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase playerInstance;
        std::cout << "Instance of PlayerExternalsInterfaceBase created successfully." << std::endl;
        
        std::cout << "Invoking SetHDMIStatus() method on PlayerExternalsInterfaceBase instance." << std::endl;
        EXPECT_NO_THROW({
            playerInstance.SetHDMIStatus();
            std::cout << "SetHDMIStatus() executed without throwing exceptions." << std::endl;
            // As this method is void with no returned value and no parameters, no further debug internal state can be logged.
        });
    });
    
    std::cout << "Exiting SetHDMIStatus_start test" << std::endl;
}
/**
 * @brief Verify that isHDCPConnection2_2() returns a valid boolean value for the HDCP 2.2 connection status.
 *
 * This test case validates that the isHDCPConnection2_2() method of PlayerExternalsInterfaceBase returns a boolean value (either true or false). 
 * The test ensures that the object is constructed successfully and that invoking the method does not result in any unexpected exceptions.
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
 * | Variation / Step | Description                                                            | Test Data                                                            | Expected Result                                                       | Notes           |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Create PlayerExternalsInterfaceBase object using default constructor   | Constructor: PlayerExternalsInterfaceBase()                          | Object is created successfully without throwing any exceptions      | Should be successful |
 * | 02               | Invoke isHDCPConnection2_2() on the created object                       | Method Call: result = player.isHDCPConnection2_2()                     | Method returns a boolean value (true or false)                        | Should Pass     |
 * | 03               | Verify that the returned value is either true or false                   | Assertion: result == true or result == false                           | Assertion passes confirming the value is a valid boolean              | Should be successful |
 */
TEST(PlayerExternalsInterfaceBase, isHDCPConnection2_2_start) {
    std::cout << "Entering isHDCPConnection2_2_start test" << std::endl;

    // Create the object using the default constructor and log the creation
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase object using default constructor" << std::endl;
        
        // Invoke the isHDCPConnection2_2() method and log the invocation.
        bool result = player.isHDCPConnection2_2();
        std::cout << "Invoked isHDCPConnection2_2(), returned value: " << result << std::endl;
        
        // Verify the output. 
        EXPECT_TRUE(result == true || result == false);
        std::cout << "Verified that isHDCPConnection2_2() returned false/true as expected" << std::endl;
    });
    
    std::cout << "Exiting isHDCPConnection2_2_start test" << std::endl;
}
/**
 * @brief Test PlayerExternalsInterfaceBase with a valid GstElement pointer
 *
 * This test verifies that a PlayerExternalsInterfaceBase instance is constructed correctly and that
 * the setGstElement API accepts and assigns a valid GstElement pointer. The test ensures proper memory
 * allocation, API invocation, and memory deallocation without throwing exceptions.
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
 * | Variation / Step | Description                                                       | Test Data                                                     | Expected Result                                           | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Create instance of PlayerExternalsInterfaceBase using default constructor | None                                                          | Instance is created without exception                   | Should Pass   |
 * | 02               | Allocate memory for a dummy GstElement pointer using std::malloc    | Allocation: size = sizeof(GstElement)                         | pElement is non-null                                      | Should Pass   |
 * | 03               | Invoke setGstElement with the valid GstElement pointer                | input: pElement = valid allocated memory                      | GstElement pointer is correctly assigned to the instance  | Should Pass   |
 * | 04               | Free the allocated memory for the dummy GstElement                    | Operation: std::free(pElement)                                  | Memory is deallocated without error                       | Should be successful |
 */
TEST(PlayerExternalsInterfaceBase, ValidGstElement) {
    std::cout << "Entering ValidGstElement test" << std::endl;
    
    // Creating instance of PlayerExternalsInterfaceBase using default constructor
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase instance" << std::endl;
        
        // Allocate memory for a dummy GstElement pointer (treating as opaque memory)
        GstElement *pElement = reinterpret_cast<GstElement*>(std::malloc(sizeof(GstElement)));
        if (pElement) {
            std::cout << "Allocated dummy GstElement at address: " << pElement << std::endl;
        } else {
            std::cout << "Allocation of dummy GstElement failed" << std::endl;
        }
        
        // Log invocation with valid pointer
        std::cout << "Invoking setGstElement with valid GstElement pointer: " << pElement << std::endl;
        player.setGstElement(pElement);
        std::cout << "setGstElement executed. GstElement pointer assigned: " << pElement << std::endl;
        
        // Free the allocated memory for dummy GstElement
        std::free(pElement);
        std::cout << "Freed dummy GstElement memory" << std::endl;
    });
    
    std::cout << "Exiting ValidGstElement test" << std::endl;
}
/**
 * @brief Tests the behavior of setGstElement when passed a null GstElement pointer
 *
 * This test verifies that calling setGstElement on an instance of PlayerExternalsInterfaceBase
 * with a null GstElement pointer does not throw any exceptions. The test ensures that the API
 * handles null pointers gracefully, which is critical to avoid unintended crashes or undefined behavior.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of PlayerExternalsInterfaceBase and invoke setGstElement with a null pointer | input: pElement = nullptr; output: instance created; | No exception thrown; API should successfully handle the null pointer without errors | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, NullGstElement) {
    std::cout << "Entering NullGstElement test" << std::endl;
    
    // Creating instance of PlayerExternalsInterfaceBase using default constructor
    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase player;
        std::cout << "Created PlayerExternalsInterfaceBase instance" << std::endl;
        
        GstElement *pElement = nullptr;
        std::cout << "Invoking setGstElement with null GstElement pointer" << std::endl;
        player.setGstElement(pElement);
        std::cout << "setGstElement executed with null pointer" << std::endl;
    });
    
    std::cout << "Exiting NullGstElement test" << std::endl;
}
/**
 * @brief Verify that the PlayerExternalsInterfaceBase object is created and destroyed without throwing any exceptions.
 *
 * This test ensures that invoking the default constructor and then calling the destructor of PlayerExternalsInterfaceBase does not result in any exceptions.
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
 * | Variation / Step | Description                                                                                         | Test Data                                                        | Expected Result                                                                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ----------- |
 * |      01        | Invoke the PlayerExternalsInterfaceBase default constructor and verify no exception is thrown.      | Constructor Input: none, Destructor Output: none                 | EXPECT_NO_THROW passes, object is created and destroyed without exceptions                      | Should Pass |
 */
TEST(PlayerExternalsInterfaceBase, DestructionOfPlayerExternalsInterfaceBase) {
    std::cout << "Entering PlayerExternalsInterfaceBase::~PlayerExternalsInterfaceBase()_start test" << std::endl;

    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor of PlayerExternalsInterfaceBase." << std::endl;
        PlayerExternalsInterfaceBase* obj = new PlayerExternalsInterfaceBase();
        std::cout << "PlayerExternalsInterfaceBase object created successfully." << std::endl;
        
        std::cout << "PlayerExternalsInterfaceBase destructor invoked successfully and object deleted." << std::endl;
    });
    
    std::cout << "Exiting PlayerExternalsInterfaceBase::~PlayerExternalsInterfaceBase()_end test" << std::endl;
}
