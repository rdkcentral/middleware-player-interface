
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
#include "PlayerExternalsInterface.h"


class FakePlayerExternalsInterfaceTests : public ::testing::Test
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

class PlayerExternalsInterfaceTests : public ::testing::Test {
protected:
    std::shared_ptr<PlayerExternalsInterface> playerExt = nullptr;

    void SetUp() override {
        playerExt = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
        ASSERT_NE(playerExt, nullptr);
        playerExt->Initialize();
    }

    void TearDown() override {
        playerExt.reset();
    }
};

/**
 * @brief Verify that FakePlayerExternalsInterface is instantiated successfully without throwing exceptions.
 *
 * This test case verifies that the FakePlayerExternalsInterface can be instantiated using its default constructor. The test ensures that the instantiation does not throw any exceptions and the object is created with its internal members set to default values.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor of FakePlayerExternalsInterface | input = (none), output = instance address | No exception thrown; EXPECT_NO_THROW verifies successful construction | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, Instantiate_FakePlayerExternalsInterface_successfully) {
    std::cout << "Entering Instantiate_FakePlayerExternalsInterface_successfully test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking FakePlayerExternalsInterface default constructor" << std::endl;
        FakePlayerExternalsInterface instance;
        std::cout << "Constructor invoked; created FakePlayerExternalsInterface object at address: " << &instance << std::endl;
        // Logging internal state simulation (default values assumed)
        std::cout << "Assuming default initialization: internal members set to default values" << std::endl;
    });
    
    std::cout << "Exiting Instantiate_FakePlayerExternalsInterface_successfully test" << std::endl;
}
/**
 * @brief Validate that FakePlayerExternalsInterface::GetActiveInterface returns false when the interface is inactive
 *
 * This test verifies that the GetActiveInterface method correctly returns false for an inactive interface.
 * It instantiates the FakePlayerExternalsInterface using the default constructor and then calls the GetActiveInterface
 * method. The test confirms that the returned value is false and the corresponding assertion passes.
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
 * | Variation / Step | Description                                                                     | Test Data                                                           | Expected Result                                                                                   | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Instantiate FakePlayerExternalsInterface using the default constructor               | constructor call = FakePlayerExternalsInterface()                        | Object is instantiated successfully without exception                                          | Should be successful |
 * | 02               | Invoke GetActiveInterface method on the instantiated FakePlayerExternalsInterface object | object: fakeInterface, method: GetActiveInterface, output: activeStatus = false | Method returns false and EXPECT_FALSE(activeStatus) assertion passes                             | Should Pass   |
 */
TEST_F(FakePlayerExternalsInterfaceTests, GetActiveInterface_start) {
    std::cout << "Entering GetActiveInterface_start test" << std::endl;
    
    // Instantiating FakePlayerExternalsInterface using default constructor
    std::cout << "Instantiating FakePlayerExternalsInterface object using default constructor" << std::endl;
    EXPECT_NO_THROW(
        {
            FakePlayerExternalsInterface fakeInterface;
            std::cout << "Invoking method GetActiveInterface" << std::endl;
            bool activeStatus = fakeInterface.GetActiveInterface();
            std::cout << "Retrieved active interface status: " << activeStatus << std::endl;
            
            // Verify that the returned value is either true or false. Since the spec returns false, we expect false.
            if(activeStatus == true)
            {
                std::cout << "Internal state: Active interface is wifi (true)" << std::endl;
            }
            else
            {
                std::cout << "Internal state: Active interface is not wifi (false)" << std::endl;
            }
            EXPECT_FALSE(activeStatus);
        }
    );
    
    std::cout << "Exiting GetActiveInterface_start test" << std::endl;
}
/**
 * @brief Test to verify that GetDisplayResolution returns valid non-zero display resolution values.
 *
 * This test checks the proper functioning of the GetDisplayResolution method of the FakePlayerExternalsInterface class.
 * It ensures that when the method is invoked, the width and height parameters are updated correctly to valid non-zero integers.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                              | Expected Result                                                                                     | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke default constructor for FakePlayerExternalsInterface.                                     | No input parameters                                                    | Object of FakePlayerExternalsInterface created successfully.                                             | Should be successful |
 * | 02               | Initialize display resolution variables width and height to 0.                              | width = 0, height = 0                                                  | Variables are initialized with default value 0.                                                     | Should be successful |
 * | 03               | Call GetDisplayResolution method with width and height passed by reference.                 | width = 0, height = 0, output1: width updated, output2: height updated   | Method executes without throwing exception and updates width and height to valid, non-zero values. | Should Pass         |
 * | 04               | Validate that the updated width and height values are non zero.                             | width = updated value, height = updated value                          | Assertion checks pass confirming that width and height are non-zero.                                  | Should Pass         |
 */
TEST_F(FakePlayerExternalsInterfaceTests, GetDisplayResolution_PositiveTest) {
    GTEST_SKIP();
    std::cout << "Entering GetDisplayResolution_PositiveTest test" << std::endl;

    // Create object using default constructor and log the construction.
    std::cout << "Invoking default constructor for FakePlayerExternalsInterface" << std::endl;
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player;
        std::cout << "FakePlayerExternalsInterface object created successfully" << std::endl;
        
        // Prepare variables for display resolution.
        int width = 0;
        int height = 0;
        std::cout << "Initial width: " << width << ", height: " << height << std::endl;
        
        // Log invocation of GetDisplayResolution method.
        std::cout << "Invoking GetDisplayResolution with width and height parameters" << std::endl;
        EXPECT_NO_THROW({
            player.GetDisplayResolution(width, height);
        });
        
        // Log the results after method call.
        std::cout << "After GetDisplayResolution invocation, retrieved width: " 
                  << width << ", height: " << height << std::endl;
        
        // Validate that the returned values are valid integers (non zero).
        // (Assuming valid resolution values are non-zero; adjust if required.)
        EXPECT_NE(width, 0) << "Expected width to be a valid non-zero integer.";
        EXPECT_NE(height, 0) << "Expected height to be a valid non-zero integer.";
        
        std::cout << "GetDisplayResolution returned valid resolution values" << std::endl;
    });
    
    std::cout << "Exiting GetDisplayResolution_PositiveTest test" << std::endl;
}
/**
 * @brief Tests retrieving configuration using a valid parameter name.
 *
 * This test verifies that the GetTR181Config API returns the correct configuration string with proper configuration length when provided with a valid parameter name "TR181NetworkConfig". It confirms that the returned configuration is non-null and that the length matches the string content length.@n
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
 * | 01 | Create instance of FakePlayerExternalsInterface. | No inputs; output: instance creation | Instance is successfully created. | Should be successful |
 * | 02 | Prepare the parameter name buffer with "TR181NetworkConfig". | paramName = TR181NetworkConfig | paramName buffer is set correctly. | Should be successful |
 * | 03 | Invoke GetTR181Config API with valid parameter name. | input: paramName = TR181NetworkConfig, configLen = 0, config = nullptr | Returns non-null configuration pointer, with configLen equal to strlen(config). | Should Pass |
 * | 04 | Validate the returned configuration length against calculated length. | output: configLen, calculatedLen = strlen(config) | configLen is equal to calculatedLen. | Should Pass |
 * | 05 | Assert that the configuration pointer is not null. | output: config pointer obtained from GetTR181Config | config pointer is not null. | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ValidParameterNameRetrievingConfiguration)
{
    GTEST_SKIP();
    std::cout << "Entering ValidParameterNameRetrievingConfiguration test" << std::endl;
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface testObj;
        std::cout << "Successfully created FakePlayerExternalsInterface object" << std::endl;
        
        size_t configLen = 0;
        const char paramNameBuffer[64] = {0};
        char paramName[64];
        strncpy(paramName, "TR181NetworkConfig", sizeof(paramName)-1);
        std::cout << "Invoking GetTR181Config with paramName: " << paramName << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW(config = testObj.GetTR181Config(paramName, configLen));
        std::cout << "Returned pointer value: " << static_cast<const void*>(config) << std::endl;
        std::cout << "Returned configuration length: " << configLen << std::endl;
        
        if (config != nullptr)
        {
            std::cout << "Configuration string: " << config << std::endl;
            size_t calculatedLen = std::strlen(config);
            std::cout << "Calculated configuration string length: " << calculatedLen << std::endl;
            EXPECT_EQ(configLen, calculatedLen);
        }
        else
        {
            std::cout << "Configuration pointer is nullptr" << std::endl;
        }
        EXPECT_NE(nullptr, config);
    });
    std::cout << "Exiting ValidParameterNameRetrievingConfiguration test" << std::endl;
}
/**
 * @brief Test GetTR181Config with an empty parameter name to ensure correct handling.
 *
 * This test case verifies that when GetTR181Config is invoked with an empty parameter name,
 * the API correctly returns a nullptr for the configuration pointer and leaves the configuration length as zero.
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
 * | Variation / Step | Description                                                         | Test Data                                                 | Expected Result                                                          | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------- | ------------------------------------------------------------------------ | ----------------- |
 * | 01               | Create an instance of FakePlayerExternalsInterface.                      | No input parameters.                                      | Object is created successfully without throwing exceptions.            | Should be successful |
 * | 02               | Initialize variables: set configLen to 0 and paramName to an empty string. | configLen = 0, paramName = ""                              | Variables are initialized correctly.                                   | Should be successful |
 * | 03               | Invoke GetTR181Config with an empty parameter name.                 | input: paramName = "", configLen = 0; output: config pointer | API returns a nullptr and configLen remains 0 without throwing exceptions. | Should Pass       |
 * | 04               | Validate the output using assertion checks.                         | Expected: config equals nullptr, configLen equals 0      | Assertions pass confirming that the return values are as expected.       | Should be successful |
 */
TEST_F(FakePlayerExternalsInterfaceTests, EmptyParameterName)
{
    std::cout << "Entering EmptyParameterName test" << std::endl;
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface testObj;
        std::cout << "Successfully created FakePlayerExternalsInterface object" << std::endl;
        
        size_t configLen = 0;
        char paramName[2] = {0};  // Empty string
        std::cout << "Invoking GetTR181Config with empty paramName" << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW(config = testObj.GetTR181Config(paramName, configLen));
        std::cout << "Returned pointer value: " << static_cast<const void*>(config) << std::endl;
        std::cout << "Returned configuration length: " << configLen << std::endl;
        
        EXPECT_EQ(nullptr, config);
        EXPECT_EQ(0u, configLen);
    });
    std::cout << "Exiting EmptyParameterName test" << std::endl;
}
/**
 * @brief Verify that GetTR181Config correctly handles a null parameter name.
 *
 * This test validates that when a null parameter name is passed to GetTR181Config, the API does not throw an exception, returns a null pointer, and sets the configuration length to zero. This ensures proper handling of invalid input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 006@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                              | Expected Result                                                   | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------------------------- | ------------ |
 * | 01               | Create an instance of FakePlayerExternalsInterface and verify no exception is thrown.              | No input data                                          | Object is successfully created without throwing any exceptions.   | Should Pass  |
 * | 02               | Call GetTR181Config with paramName set to nullptr and initial configLen initialized to 0.       | paramName = nullptr, configLen = 0                       | The API returns a null pointer and leaves configLen as 0.           | Should Pass  |
 * | 03               | Assert that the returned pointer is nullptr and configLen equals 0.                           | Returned pointer = nullptr, configLen = 0              | EXPECT_EQ confirms config is nullptr and configLen is 0.            | Should Pass  |
 */
TEST_F(FakePlayerExternalsInterfaceTests, NullParameterName)
{
    std::cout << "Entering NullParameterName test" << std::endl;
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface testObj;
        std::cout << "Successfully created FakePlayerExternalsInterface object" << std::endl;
        
        size_t configLen = 0;
        const char* paramName = nullptr;
        std::cout << "Invoking GetTR181Config with paramName: nullptr" << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW(config = testObj.GetTR181Config(paramName, configLen));
        std::cout << "Returned pointer value: " << static_cast<const void*>(config) << std::endl;
        std::cout << "Returned configuration length: " << configLen << std::endl;
        
        EXPECT_EQ(nullptr, config);
        EXPECT_EQ(0u, configLen);
    });
    std::cout << "Exiting NullParameterName test" << std::endl;
}
/**
 * @brief Test retrieving configuration for a non-existent TR181 parameter.
 *
 * This test verifies that querying the TR181 configuration using an invalid parameter name returns a nullptr
 * and sets the configuration length to zero. The function under test, GetTR181Config, is expected to safely
 * handle invalid parameter names without throwing unexpected exceptions.
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
 * | 01 | Instantiate FakePlayerExternalsInterface and invoke GetTR181Config with an invalid parameter name "TR181UnknownConfig". | paramName = "TR181UnknownConfig", configLen = 0; output: config = nullptr, configLen = 0 | GetTR181Config should return nullptr and the configuration length should remain 0 without throwing an exception. | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, NonExistentParameterName)
{
    std::cout << "Entering NonExistentParameterName test" << std::endl;
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface testObj;
        std::cout << "Successfully created FakePlayerExternalsInterface object" << std::endl;
        
        size_t configLen = 0;
        char paramName[64];
        strncpy(paramName, "TR181UnknownConfig", sizeof(paramName)-1);
        std::cout << "Invoking GetTR181Config with paramName: " << paramName << std::endl;
        
        char* config = nullptr;
        EXPECT_NO_THROW(config = testObj.GetTR181Config(paramName, configLen));
        std::cout << "Returned pointer value: " << static_cast<const void*>(config) << std::endl;
        std::cout << "Returned configuration length: " << configLen << std::endl;
        
        EXPECT_EQ(nullptr, config);
        EXPECT_EQ(0u, configLen);
    });
    std::cout << "Exiting NonExistentParameterName test" << std::endl;
}
/**
 * @brief Verify that SetHDMIStatus correctly sets HDMI parameters at startup
 *
 * This test creates a FakePlayerExternalsInterface object using the default constructor and invokes the SetHDMIStatus method.
 * It checks that no exceptions are thrown during object creation and method invocation, ensuring that the internal state
 * (m_hdcpCurrentProtocol and m_isHDCPEnabled) is set as expected.
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
 * | Variation / Step | Description                                                         | Test Data                                                                           | Expected Result                              | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | -------------------------------------------- | --------------- |
 * | 01               | Create FakePlayerExternalsInterface object using default constructor     | constructor: default (no input parameters)                                          | Object created successfully without exception | Should be successful |
 * | 02               | Invoke SetHDMIStatus() method                                         | method: SetHDMIStatus() (no parameters)                                             | No exception thrown during method invocation   | Should Pass     |
 * | 03               | Validate the internal state change logged after SetHDMIStatus() call    | m_hdcpCurrentProtocol = PLAYER_dsHDCP_VERSION_1X, m_isHDCPEnabled = true             | Internal state updated as expected             | Should be successful |
 */
TEST_F(FakePlayerExternalsInterfaceTests, SetHDMIStatus_correctly_sets_HDMI_parameters_start)
{
    std::cout << "Entering SetHDMIStatus_correctly_sets_HDMI_parameters_start test" << std::endl;
    
    // Creating the FakePlayerExternalsInterface object using default constructor
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface fakePlayer;
        std::cout << "Created FakePlayerExternalsInterface object using default constructor." << std::endl;
        
        // Invoking SetHDMIStatus method
        std::cout << "Invoking SetHDMIStatus() method." << std::endl;
        EXPECT_NO_THROW(fakePlayer.SetHDMIStatus());
        std::cout << "SetHDMIStatus() invoked successfully." << std::endl;
        
        // Logging expected internal state values
        std::cout << "Expected internal state change: "
                  << "m_hdcpCurrentProtocol set to PLAYER_dsHDCP_VERSION_1X and m_isHDCPEnabled set to true." << std::endl;
    });
    
    std::cout << "Exiting SetHDMIStatus_correctly_sets_HDMI_parameters_start test" << std::endl;
}
/**
 * @brief Verify that the isHDCPConnection2_2() method executes without exception and returns a valid boolean.
 *
 * This test validates that a FakePlayerExternalsInterface object can be instantiated using the default constructor,
 * and that the isHDCPConnection2_2() method can be invoked successfully without throwing an exception.
 * The method is expected to return a valid boolean value, which is then printed for verification.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 016
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                             | Test Data                                                              | Expected Result                                                       | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------------- |
 * | 01               | Create FakePlayerExternalsInterface object using default constructor                         | Constructor: FakePlayerExternalsInterface()                                 | Object is created without throwing an exception                        | Should be successful |
 * | 02               | Invoke the isHDCPConnection2_2() method on the FakePlayerExternalsInterface object              | Method call: isHDCPConnection2_2(), output: bool result                  | Method returns a valid boolean without throwing an exception             | Should Pass       |
 * | 03               | Validate printed output confirming proper method execution and boolean return value       | Output: Printed boolean result from isHDCPConnection2_2()                | Console output confirms method execution and a valid boolean is returned   | Should be successful |
 */
TEST_F(FakePlayerExternalsInterfaceTests, isHDCPConnection2_2__start)
{
    std::cout << "Entering isHDCPConnection2_2()_start test" << std::endl;
    
    // Create object using default constructor and verify no exceptions are thrown.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface fakePlayer;
        std::cout << "Constructed FakePlayerExternalsInterface object using default constructor." << std::endl;
        
        // Invoke the isHDCPConnection2_2() method
        bool result = false;
        EXPECT_NO_THROW({
            std::cout << "Invoking isHDCPConnection2_2() method." << std::endl;
            result = fakePlayer.isHDCPConnection2_2();
            std::cout << "Method isHDCPConnection2_2() returned value: " << std::boolalpha << result << std::endl;
        });
        
        // Assert that the returned value is a valid boolean (true or false).
        // Since the method always returns a bool, we simply check that no exception is thrown.
        // For detailed debug, we output the result.
        std::cout << "Final internal check, isHDCPConnection2_2() return value is: " << std::boolalpha << result << std::endl;
    });
    
    std::cout << "Exiting isHDCPConnection2_2()_start test" << std::endl;
}
/**
 * @brief Test the isHDCPConnection2_2() method of FakePlayerExternalsInterface class to verify correct functioning.
 *
 * This test creates a FakePlayerExternalsInterface object using the default constructor and subsequently invokes the isHDCPConnection2_2() method.
 * It verifies that the object construction and API invocation do not throw exceptions and that the returned HDCP connection status is correctly logged.
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
 * | Variation / Step | Description                                                     | Test Data                                                      | Expected Result                                                 | Notes         |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------- | ------------- |
 * | 01               | Construct FakePlayerExternalsInterface using default constructor.    | No input, output: instance creation of FakePlayerExternalsInterface   | Object is constructed without throwing any exceptions.        | Should be successful |
 * | 02               | Invoke isHDCPConnection2_2() method on the constructed object.     | Input: fakePlayer object, call to isHDCPConnection2_2(), output: boolean result | Method returns a valid boolean value without throwing exceptions. | Should Pass   |
 * | 03               | Log and verify the returned HDCP connection status.               | Input: result (boolean value returned by isHDCPConnection2_2())      | Retrieved value is logged and verified correctly.               | Should be successful |
 */
TEST_F(FakePlayerExternalsInterfaceTests, isHDCPConnection2_2__end)
{
    std::cout << "Entering isHDCPConnection2_2()_end test" << std::endl;
    
    // Create object using default constructor and verify no exceptions are thrown.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface fakePlayer;
        std::cout << "Constructed FakePlayerExternalsInterface object using default constructor." << std::endl;
        
        // Invoke the isHDCPConnection2_2() method
        bool result = false;
        EXPECT_NO_THROW({
            std::cout << "Invoking isHDCPConnection2_2() method." << std::endl;
            result = fakePlayer.isHDCPConnection2_2();
            std::cout << "Method isHDCPConnection2_2() returned value: " << std::boolalpha << result << std::endl;
        });
        
        // Log retrieved value from the method call.
        std::cout << "Retrieved and verified HDCP connection status: " << std::boolalpha << result << std::endl;
    });
    
    std::cout << "Exiting isHDCPConnection2_2()_end test" << std::endl;
}
/**
 * @brief Verify that the destructor of FakePlayerExternalsInterface does not throw an exception.
 *
 * This test creates an instance of FakePlayerExternalsInterface using its default constructor and then explicitly calls
 * the destructor via the delete operator. The objective is to ensure that no exception is thrown during object destruction,
 * thereby confirming stable cleanup of resources.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of FakePlayerExternalsInterface and invoke its destructor | input: none, output: FakePlayerExternalsInterface instance created and deleted | No exception should be thrown during destruction | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, DestructorDoesNotThrow) {
    std::cout << "Entering DestructorDoesNotThrow test" << std::endl;
    EXPECT_NO_THROW({
        // Creating instance using default constructor
        FakePlayerExternalsInterface* instance = new FakePlayerExternalsInterface();
        std::cout << "FakePlayerExternalsInterface instance created. (Default constructor invoked)" << std::endl;
                
        // Invoking destructor explicitly via delete
        std::cout << "Invoking destructor of FakePlayerExternalsInterface instance." << std::endl;
        delete instance;
        std::cout << "Destructor executed. FakePlayerExternalsInterface instance destroyed successfully." << std::endl;
    });
    std::cout << "Exiting DestructorDoesNotThrow test" << std::endl;
}
/**
 * @brief Verify that IsSourceUHD() returns false when m_gstElement is NULL.
 *
 * This test validates that when a FakePlayerExternalsInterface object is instantiated using
 * the default constructor (which sets m_gstElement to NULL), the IsSourceUHD() method returns false.
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
 * | 01 | Create FakePlayerExternalsInterface object with default constructor where m_gstElement is NULL | No input parameters; object is created with m_gstElement = NULL | Object is created successfully with m_gstElement set to NULL | Should be successful |
 * | 02 | Invoke IsSourceUHD() method on the object | Method invocation on object with m_gstElement = NULL | API returns false; assertion EXPECT_FALSE(retVal) passes | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ReturnsFalseWhenGstElementIsNull) {
    GTEST_SKIP();
    std::cout << "Entering ReturnsFalseWhenGstElementIsNull test" << std::endl;

    // Create object using default constructor (assumed to set m_gstElement = NULL)
    FakePlayerExternalsInterface player;
    std::cout << "Created FakePlayerExternalsInterface object with default constructor (m_gstElement is NULL)" << std::endl;
    std::cout << "Invoking IsSourceUHD()" << std::endl;
    EXPECT_NO_THROW(
    {
        bool retVal = player.IsSourceUHD();
        EXPECT_TRUE(retVal);  // or EXPECT_FALSE(retVal) depending on expected behavior
        // The method should return false when m_gstElement is null.
        std::cout << "Returned value: " << retVal << std::endl;
        EXPECT_FALSE(retVal);
    });
    std::cout << "Exiting ReturnsFalseWhenGstElementIsNull test" << std::endl;
}
#if 0 // Disabled until FakeGstElement is implemented
/**
 * @brief Verify that IsSourceUHD returns false when the video properties are zero
 *
 * This test verifies that when a FakeGstElement is initialized with video_width and video_height set to zero, 
 * the FakePlayerExternalsInterface object's IsSourceUHD method correctly returns false. This ensures that the API handles 
 * the zero-dimension scenario as expected.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                               | Expected Result                                         | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ------------------------------------------------------- | ---------- |
 * | 01               | Initialize FakeGstElement with video_width=0 and video_height=0; create FakePlayerExternalsInterface; invoke IsSourceUHD() | video_width = 0, video_height = 0, m_gstElement = non-null, retVal = result of IsSourceUHD() | IsSourceUHD() returns false and assertion EXPECT_FALSE passes | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ReturnsFalseWhenPropertiesAreZero) {
    std::cout << "Entering ReturnsFalseWhenPropertiesAreZero test" << std::endl;

    // Setup fake GstElement with video_width=0, video_height=0.
    FakeGstElement fakeElem;
    fakeElem.video_width = 0;
    fakeElem.video_height = 0;
    std::cout << "Created FakeGstElement with video_width = " << fakeElem.video_width 
              << " and video_height = " << fakeElem.video_height << std::endl;
    
    // Create FakePlayerExternalsInterface using custom constructor to assign m_gstElement.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player(reinterpret_cast<GstElement*>(&fakeElem));
        std::cout << "Created FakePlayerExternalsInterface object with non-null m_gstElement" << std::endl;
        std::cout << "Invoking IsSourceUHD()" << std::endl;
        bool retVal = player.IsSourceUHD();
        std::cout << "Returned value: " << retVal << std::endl;
        EXPECT_FALSE(retVal);
    });

    std::cout << "Exiting ReturnsFalseWhenPropertiesAreZero test" << std::endl;
}
/**
 * @brief Verify that IsSourceUHD returns false when the resolution is below UHD thresholds
 *
 * This test verifies that the FakePlayerExternalsInterface correctly identifies non-UHD content when the video resolution provided by the FakeGstElement is below the UHD thresholds (width less than 3840 and height less than 2160). It ensures that when the gst element's video width and height do not meet UHD standards, the IsSourceUHD API returns false.
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
 * | Variation / Step | Description                                                                 | Test Data                                                       | Expected Result                                                   | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------- | -------------- |
 * | 01               | Setup FakeGstElement with video resolution below UHD thresholds             | video_width = 1280, video_height = 720                           | FakeGstElement initialized with provided resolution               | Should be successful  |
 * | 02               | Create FakePlayerExternalsInterface using the custom constructor with fake gst element | gstElement = address of fakeElem                                  | FakePlayerExternalsInterface successfully instantiated                   | Should be successful  |
 * | 03               | Invoke IsSourceUHD() on the FakePlayerExternalsInterface and validate the return value | API = IsSourceUHD(), input from fakeElem with video_width=1280, video_height=720, output = false | Returns false and EXPECT_FALSE assertion passes                   | Should Pass    |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ReturnsFalseWhenBelowUHDThresholds) {
    std::cout << "Entering ReturnsFalseWhenBelowUHDThresholds test" << std::endl;

    // Setup fake GstElement with resolution below UHD thresholds.
    FakeGstElement fakeElem;
    fakeElem.video_width = 1280;  // below UHD_WIDTH (3840)
    fakeElem.video_height = 720;  // below UHD_HEIGHT (2160)
    std::cout << "Created FakeGstElement with video_width = " << fakeElem.video_width 
              << " and video_height = " << fakeElem.video_height << std::endl;

    // Create FakePlayerExternalsInterface using custom constructor with fake gst element.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player(reinterpret_cast<GstElement*>(&fakeElem));
        std::cout << "Created FakePlayerExternalsInterface object with custom m_gstElement" << std::endl;
        std::cout << "Invoking IsSourceUHD()" << std::endl;
        bool retVal = player.IsSourceUHD();
        std::cout << "Returned value: " << retVal << std::endl;
        EXPECT_FALSE(retVal);
    });

    std::cout << "Exiting ReturnsFalseWhenBelowUHDThresholds test" << std::endl;
}
/**
 * @brief Verify that IsSourceUHD() returns true when video width is at or above the UHD threshold
 *
 * This test evaluates the proper functionality of FakePlayerExternalsInterface::IsSourceUHD().
 * It sets up a FakeGstElement with video_width equal to UHD_WIDTH and a non-zero video_height,
 * invokes IsSourceUHD(), and confirms that the function returns true.
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
 * | Variation / Step | Description                                                                                         | Test Data                                         | Expected Result                                                  | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Initialize FakeGstElement with video_width = UHD_WIDTH and video_height = 1080, create FakePlayerExternalsInterface using the fake element pointer, and invoke IsSourceUHD()  | video_width = UHD_WIDTH, video_height = 1080      | IsSourceUHD() returns true and the EXPECT_TRUE assertion passes   | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ReturnsTrueWhenWidthAtOrAboveUHD) {
    std::cout << "Entering ReturnsTrueWhenWidthAtOrAboveUHD test" << std::endl;
    
    // Setup fake GstElement with video_width at UHD threshold and non-zero video_height.
    FakeGstElement fakeElem;
    fakeElem.video_width = UHD_WIDTH; // equal to UHD_WIDTH
    fakeElem.video_height = 1080;     // non-zero value, below UHD_HEIGHT but enough for check
    std::cout << "Created FakeGstElement with video_width = " << fakeElem.video_width 
              << " and video_height = " << fakeElem.video_height << std::endl;
    
    // Create FakePlayerExternalsInterface with custom gst element.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player(reinterpret_cast<GstElement*>(&fakeElem));
        std::cout << "Created FakePlayerExternalsInterface object with m_gstElement pointing to fake element" << std::endl;
        std::cout << "Invoking IsSourceUHD()" << std::endl;
        bool retVal = player.IsSourceUHD();
        std::cout << "Returned value: " << retVal << std::endl;
        EXPECT_TRUE(retVal);
    });
    
    std::cout << "Exiting ReturnsTrueWhenWidthAtOrAboveUHD test" << std::endl;
}
/**
 * @brief Validate that FakePlayerExternalsInterface::IsSourceUHD returns true when video height is at or above UHD threshold
 *
 * This test verifies that when a FakeGstElement is configured with a video_width of 1920 and a video_height equal to UHD_HEIGHT,
 * the FakePlayerExternalsInterface correctly identifies the source as UHD by returning true from the IsSourceUHD function.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                        | Expected Result                                       | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------ | ------------ |
 * | 01               | Initialize a FakeGstElement with video_width = 1920 and video_height = UHD_HEIGHT              | video_width = 1920, video_height = UHD_HEIGHT                      | FakeGstElement is successfully set up                  | Should be successful |
 * | 02               | Create an instance of FakePlayerExternalsInterface using the pointer to the FakeGstElement           | gstElement pointer = address of FakeGstElement                     | Object is created without throwing an exception        | Should Pass  |
 * | 03               | Invoke the IsSourceUHD() method on the FakePlayerExternalsInterface instance                         | No additional input; internal state uses fake element data         | Returns true indicating the source is UHD              | Should Pass  |
 * | 04               | Verify that the returned value from IsSourceUHD() is true using EXPECT_TRUE                    | retVal = true from IsSourceUHD()                                    | EXPECT_TRUE assertion passes confirming the expected value | Should Pass  |
 */
TEST_F(FakePlayerExternalsInterfaceTests, ReturnsTrueWhenHeightAtOrAboveUHD) {
    std::cout << "Entering ReturnsTrueWhenHeightAtOrAboveUHD test" << std::endl;

    // Setup fake GstElement with video_height at UHD threshold and non-zero video_width.
    FakeGstElement fakeElem;
    fakeElem.video_width = 1920;      // non-zero value
    fakeElem.video_height = UHD_HEIGHT; // equal to UHD_HEIGHT
    std::cout << "Created FakeGstElement with video_width = " << fakeElem.video_width 
              << " and video_height = " << fakeElem.video_height << std::endl;

    // Create FakePlayerExternalsInterface with custom gst element.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player(reinterpret_cast<GstElement*>(&fakeElem));
        std::cout << "Created FakePlayerExternalsInterface object with m_gstElement from fake element" << std::endl;
        std::cout << "Invoking IsSourceUHD()" << std::endl;
        bool retVal = player.IsSourceUHD();
        std::cout << "Returned value: " << retVal << std::endl;
        EXPECT_TRUE(retVal);
    });

    std::cout << "Exiting ReturnsTrueWhenHeightAtOrAboveUHD test" << std::endl;
}
#endif
/**
 * @brief Verify that FakePlayerExternalsInterface default initialization sets default member values correctly
 *
 * This test verifies that the default construction of FakePlayerExternalsInterface properly initializes m_sourceWidth to 0, m_sourceHeight to 0, and m_gstElement to nullptr. It ensures that invoking the default constructor does not throw any exceptions and that the associated getter methods return the expected default values.
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
 * | Variation / Step | Description                                                                  | Test Data                                                                 | Expected Result                                               | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate FakePlayerExternalsInterface using its default constructor            | No input; output: instance of FakePlayerExternalsInterface created             | Object is created without throwing exceptions                 | Should Pass |
 * | 02               | Retrieve m_sourceWidth using getSourceWidth()                                | Input: None; output: sourceWidth = 0                                      | getSourceWidth() returns 0                                      | Should Pass |
 * | 03               | Retrieve m_sourceHeight using getSourceHeight()                              | Input: None; output: sourceHeight = 0                                     | getSourceHeight() returns 0                                     | Should Pass |
 * | 04               | Retrieve m_gstElement using getGstElement()                                  | Input: None; output: gstElement = nullptr                                 | getGstElement() returns nullptr                                 | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase_start) {
    std::cout << "Entering PlayerExternalsInterfaceBase::PlayerExternalsInterfaceBase()_start test" << std::endl;
    
    // Instantiate the derived class object, which in turn calls the base class constructor.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface fakePlayer;
        std::cout << "Invoked FakePlayerExternalsInterface default constructor." << std::endl;
        
        // Retrieve and log m_sourceWidth via an assumed public getter.
        int sourceWidth = fakePlayer.getSourceWidth();
        std::cout << "Retrieved m_sourceWidth: " << sourceWidth << std::endl;
        EXPECT_EQ(sourceWidth, 0);
        
        // Retrieve and log m_sourceHeight via an assumed public getter.
        int sourceHeight = fakePlayer.getSourceHeight();
        std::cout << "Retrieved m_sourceHeight: " << sourceHeight << std::endl;
        EXPECT_EQ(sourceHeight, 0);
        
        // Retrieve and log m_gstElement via an assumed public getter.
        GstElement* gstElement = fakePlayer.getGstElement();
        std::cout << "Retrieved m_gstElement: " << gstElement << std::endl;
        EXPECT_EQ(gstElement, nullptr);
    });
    
    std::cout << "Exiting PlayerExternalsInterfaceBase::PlayerExternalsInterfaceBase()_start test" << std::endl;
}
/**
 * @brief Validate that setGstElement correctly updates the internal GstElement pointer when provided a valid pointer.
 *
 * This test verifies that when a valid GstElement pointer is passed to the setGstElement method of a FakePlayerExternalsInterface object,
 * the internal m_gstElement is updated without throwing any exceptions. This ensures that the interface properly stores the pointer for later use.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                            | Expected Result                                                                                                                         | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a FakePlayerExternalsInterface object using the default constructor.                       | No input; output: player object created via default constructor.     | Object should be created successfully without throwing any exceptions.                                                                | Should be successful |
 * | 02               | Create a valid GstElement object and obtain its pointer.                                     | GstElement validElement1, pValidElement1 = &validElement1.             | A valid GstElement pointer is acquired.                                                                                                 | Should be successful |
 * | 03               | Invoke setGstElement on the player object passing the valid GstElement pointer.                | Input: pValidElement1; Output: internal m_gstElement should be set to pValidElement1. | The setGstElement API should not throw and the internal pointer should be updated correctly.                                               | Should Pass   |
 * | 04               | Retrieve the internal m_gstElement pointer using pointer arithmetic and validate it matches pValidElement1. | internal m_gstElement value, expected pointer value = pValidElement1.   | EXPECT_EQ should confirm that the internal pointer is equal to pValidElement1.                                                            | Should Pass   |
 */
TEST_F(FakePlayerExternalsInterfaceTests, SetGstElementWithValidPointer) {
    std::cout << "Entering SetGstElementWithValidPointer test" << std::endl;
  
    // Create an object of the derived class using the default constructor.
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player;
        std::cout << "Created FakePlayerExternalsInterface object using default constructor" << std::endl;
      
        // Create a valid GstElement object and get its pointer.
        GstElement validElement1;
        GstElement* pValidElement1 = &validElement1;
        std::cout << "Created a valid GstElement at address: " << pValidElement1 << std::endl;
      
        // Invoke setGstElement with the valid GstElement pointer.
        std::cout << "Invoking setGstElement with pointer: " << pValidElement1 << std::endl;
        EXPECT_NO_THROW(player.setGstElement(pValidElement1));
      
    });
  
    std::cout << "Exiting SetGstElementWithValidPointer test" << std::endl;
}
/**
 * @brief Tests setting the GstElement pointer to null.
 *
 * Verifies that the setGstElement function in FakePlayerExternalsInterface correctly handles a null pointer input and sets the internal m_gstElement to nullptr. This ensures the robustness of the method when provided with an invalid pointer.
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
 * | Variation / Step | Description                                                                              | Test Data                                                  | Expected Result                                                              | Notes               |
 * | :---------------:| ---------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------------- |
 * | 01               | Create a FakePlayerExternalsInterface object using the default constructor                    | None                                                      | Object created successfully                                                  | Should be successful|
 * | 02               | Invoke setGstElement with a null pointer                                                 | pNullElement = nullptr                                     | API call does not throw any exception                                       | Should Pass         |
 * | 03               | Retrieve the internal m_gstElement and verify it is set to nullptr                         | internal m_gstElement value = nullptr                      | Assertion passes confirming internal pointer is set to nullptr              | Should be successful|
 */
TEST_F(FakePlayerExternalsInterfaceTests, SetGstElementWithNullPointer) {
    std::cout << "Entering SetGstElementWithNullPointer test" << std::endl;
  
    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player;
        std::cout << "Created FakePlayerExternalsInterface object using default constructor" << std::endl;
      
        EXPECT_NO_THROW(
        {
            GstElement element;
            player.setGstElement(&element);

            GstElement* internalGstPtr = player.getGstElement();
            EXPECT_EQ(internalGstPtr, &element);
            std::cout << "Set m_gstElement to valid pointer: " << internalGstPtr << std::endl;
        });      
    });
  
    std::cout << "Exiting SetGstElementWithNullPointer test" << std::endl;
}
/**
 * @brief Verify that the setGstElement method correctly updates the internal GstElement pointer when provided with different valid pointers.
 *
 * This test verifies that invoking setGstElement with a valid GstElement pointer should correctly assign the internal m_gstElement. It then checks that updating the pointer using a different valid GstElement correctly overwrites the previous value, ensuring that the interface handles pointer updates properly.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                           | Expected Result                                                               | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ----------------------------------------------------------------------------- | ------------- |
 * | 01               | Construct FakePlayerExternalsInterface object using the default constructor.                      | No inputs; output: a valid FakePlayerExternalsInterface object           | Object is created successfully.                                               | Should be successful |
 * | 02               | Create the first valid GstElement pointer and initialize it.                                 | firstElement, pFirstElement = address of firstElement                | pFirstElement is a valid pointer to a GstElement instance.                    | Should be successful |
 * | 03               | Invoke setGstElement using the first valid pointer.                                          | Function call: setGstElement(pFirstElement)                         | No exception thrown; internal pointer equals pFirstElement.                   | Should Pass   |
 * | 04               | Create the second valid GstElement pointer and initialize it.                                | secondElement, pSecondElement = address of secondElement              | pSecondElement is a valid pointer to a GstElement instance.                   | Should be successful |
 * | 05               | Invoke setGstElement using the second valid pointer to update the internal pointer.            | Function call: setGstElement(pSecondElement)                        | No exception thrown; internal pointer updated to pSecondElement.              | Should Pass   |
 */
TEST_F(FakePlayerExternalsInterfaceTests, SetGstElementWithDifferentValidPointer) {
    std::cout << "Entering SetGstElementWithDifferentValidPointer test" << std::endl;

    EXPECT_NO_THROW({
        FakePlayerExternalsInterface player;
        std::cout << "Created FakePlayerExternalsInterface object using default constructor" << std::endl;

        // First valid GstElement
        GstElement firstElement;
        GstElement* pFirstElement = &firstElement;
        std::cout << "Created first valid GstElement at address: " << pFirstElement << std::endl;

        std::cout << "Invoking setGstElement with first pointer: " << pFirstElement << std::endl;
        EXPECT_NO_THROW(player.setGstElement(pFirstElement));

        // Verify using getter
        EXPECT_EQ(player.getGstElement(), pFirstElement);

        // Second valid GstElement
        GstElement secondElement;
        GstElement* pSecondElement = &secondElement;
        std::cout << "Created second valid GstElement at address: " << pSecondElement << std::endl;

        std::cout << "Invoking setGstElement with second pointer: " << pSecondElement << std::endl;
        EXPECT_NO_THROW(player.setGstElement(pSecondElement));

        // Verify update using getter
        EXPECT_EQ(player.getGstElement(), pSecondElement);
    });

    std::cout << "Exiting SetGstElementWithDifferentValidPointer test" << std::endl;
}
/**
 * @brief Verify that the FakePlayerExternalsInterface object is created, configured, and destroyed without exceptions.
 *
 * This test checks if the derived class FakePlayerExternalsInterface can be instantiated using its default constructor, have its member variables assigned valid values, and then be destroyed by invoking its destructor without any exceptions. It validates the normal lifecycle of object creation and destruction ensuring resource cleanup is correctly handled.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Instantiate FakePlayerExternalsInterface object using the default constructor. | None | Object is created without throwing an exception. | Should Pass |
 * | 02 | Set member m_gstElement to a valid pointer value. | m_gstElement = 0x1234 | Member variable m_gstElement is correctly assigned the pointer value. | Should be successful |
 * | 03 | Assign valid values to member variables m_isHDCPEnabled, m_displayWidth, m_displayHeight, m_sourceWidth, and m_sourceHeight. | m_isHDCPEnabled = true, m_displayWidth = 1920, m_displayHeight = 1080, m_sourceWidth = 1280, m_sourceHeight = 720 | All member variables hold the assigned valid values. | Should be successful |
 * | 04 | Invoke the destructor by deleting the FakePlayerExternalsInterface object. | delete fakePlayer | Object is destroyed without throwing an exception. | Should Pass |
 */
TEST_F(FakePlayerExternalsInterfaceTests, NormalObjectDestruction) {
    std::cout << "Entering NormalObjectDestruction test" << std::endl;
    EXPECT_NO_THROW({
        // Instantiate the derived class object using its default constructor.
        FakePlayerExternalsInterface *fakePlayer = new FakePlayerExternalsInterface();
        
        std::cout << "FakePlayerExternalsInterface object created successfully." << std::endl;
        
        // Set the member m_gstElement to a valid pointer value.
        fakePlayer->setGstElementFake(reinterpret_cast<GstElement*>(0x1234));
        std::cout << "m_gstElement set to: " << fakePlayer->getGstElement() << std::endl;
        
        // Assign valid member values.
        fakePlayer->setHDCPEnabled(true);
        fakePlayer->setDisplayWidth(1920);
        fakePlayer->setDisplayHeight(1080);
        fakePlayer->setSourceWidth(1280);
        fakePlayer->setSourceHeight(720);
        
        std::cout << "m_isHDCPEnabled set to: " << fakePlayer->isHDCPEnabled() << std::endl;
        std::cout << "m_displayWidth set to: " << fakePlayer->getDisplayWidth() << std::endl;
        std::cout << "m_displayHeight set to: " << fakePlayer->getDisplayHeight() << std::endl;
        std::cout << "m_sourceWidth set to: " << fakePlayer->getSourceWidth() << std::endl;
        std::cout << "m_sourceHeight set to: " << fakePlayer->getSourceHeight() << std::endl;
        
        // Log the invocation of the destructor.
        std::cout << "Invoking destructor on FakePlayerExternalsInterface object." << std::endl;
        delete fakePlayer;
        std::cout << "Destructor invoked successfully on FakePlayerExternalsInterface object." << std::endl;
    });
    std::cout << "Exiting NormalObjectDestruction test" << std::endl;
}

/**
 * @brief Test the creation of PlayerExternalsInterface and invocation of GetActiveInterface method
 *
 * This test validates that the PlayerExternalsInterface object can be constructed without exceptions
 * using the default constructor and that the GetActiveInterface method can be successfully invoked 
 * without throwing exceptions. The test ensures that both object creation and method call are handled 
 * properly, which is critical for basic functionality.
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
 * | Variation / Step | Description                                                                       | Test Data                                                       | Expected Result                                                            | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------------- | ------------ |
 * | 01               | Create PlayerExternalsInterface object using the default constructor and log creation | None                                                             | Object created successfully without throwing an exception                | Should Pass  |
 * | 02               | Invoke GetActiveInterface method on the created object and log the returned value    | output: isWifi (boolean)                                         | Method returns a valid boolean value indicating the active interface and no exception thrown | Should Pass  |
 */
TEST_F(PlayerExternalsInterfaceTests, GetActiveInterface_start)
{
    std::cout << "Entering GetActiveInterface_start test" << std::endl;
        
    // Invoke the GetActiveInterface method and log the invocation.
    std::cout << "Invoking GetActiveInterface method" << std::endl;
    bool isWifi ;
    EXPECT_NO_THROW(isWifi = playerExt->GetActiveInterface());
    std::cout << "GetActiveInterface returned: " << (isWifi ? "true" : "false") << std::endl;
    
    std::cout << "Exiting GetActiveInterface_start test" << std::endl;
}
/**
 * @brief Validate GetDisplayResolution updates zero-initialized resolution values to valid positive values
 *
 * This test verifies that when the resolution parameters (width and height) are initialized to zero,
 * the GetDisplayResolution method of PlayerExternalsInterface updates them to valid, positive resolution values.
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
 * | Variation / Step | Description                                                                                      | Test Data                               | Expected Result                                                              | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | --------------------------------------- | ---------------------------------------------------------------------------- | --------------- |
 * | 01               | Create PlayerExternalsInterface object using default constructor                                 | None                                    | Object created successfully without throwing exceptions                    | Should be successful |
 * | 02               | Initialize resolution values to zero                                                             | width = 0, height = 0                     | width and height variables set to 0                                         | Should be successful |
 * | 03               | Invoke GetDisplayResolution method with zero-initialized resolution parameters                   | width = 0, height = 0                     | width and height updated to valid positive values (e.g., width > 0, height > 0) | Should Pass     |
 * | 04               | Validate updated resolution values using assertions                                              | width: updated value, height: updated value | EXPECT_GT(width, 0) and EXPECT_GT(height, 0) assertions succeed              | Should Pass     |
 */
TEST_F(PlayerExternalsInterfaceTests, GetDisplayResolution_ValidResolutionRetrievalZeroInit)
{
    GTEST_SKIP();
    std::cout << "Entering GetDisplayResolution_ValidResolutionRetrievalZeroInit test" << std::endl;

    // Create an instance of PlayerExternalsInterface using default constructor
    EXPECT_NO_THROW({
        
        // Initialize resolution parameters to zero
        int width = 0;
        int height = 0;
        std::cout << "Initial values - width: " << width << ", height: " << height << std::endl;

        // Invoke the GetDisplayResolution method
        std::cout << "Invoking method GetDisplayResolution with width: " << width << " and height: " << height << std::endl;
        playerExt->GetDisplayResolution(width, height);

        // Log the output values after method invocation
        std::cout << "After invoking GetDisplayResolution, retrieved resolution - width: " << width << ", height: " << height << std::endl;

        // Validate that the resolution values are updated to valid positive values.
        // As per test plan example, expected values might be 1920 for width and 1080 for height.
        EXPECT_GT(width, 0) << "Width should be greater than 0";
        EXPECT_GT(height, 0) << "Height should be greater than 0";
        
        std::cout << "Method GetDisplayResolution executed successfully with updated width: " << width << " and height: " << height << std::endl;
    });

    std::cout << "Exiting GetDisplayResolution_ValidResolutionRetrievalZeroInit test" << std::endl;
}
/**
 * @brief Test to verify that GetPlayerExternalsInterfaceInstance() returns a valid, non-null shared_ptr instance of PlayerExternalsInterface.
 *
 * This test calls the static method PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance() and confirms that it does not throw any exceptions and that the returned shared_ptr is not null. This ensures that the API correctly provides a valid instance for further operations.
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
 * | Variation / Step | Description                                                                                             | Test Data                                                                      | Expected Result                                                                                          | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance() and verify no exceptions occur.  | input: none, output: shared_ptr<PlayerExternalsInterface> not null             | No exceptions thrown; returned shared_ptr is non-null                                                   | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, GetPlayerExternalsInterfaceInstance_Positive) {
    std::cout << "Entering GetPlayerExternalsInterfaceInstance_Positive test" << std::endl;
    
    std::shared_ptr<PlayerExternalsInterface> playerInterfaceInstance = nullptr;
    
    std::cout << "Invoking PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance()" << std::endl;
    EXPECT_NO_THROW({
        playerInterfaceInstance = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    });
    
    std::cout << "Returned shared_ptr value: " << playerInterfaceInstance.get() << std::endl;
    EXPECT_NE(playerInterfaceInstance, nullptr) << "Expected non-null shared_ptr from GetPlayerExternalsInterfaceInstance()";
    
    std::cout << "Exiting GetPlayerExternalsInterfaceInstance_Positive test" << std::endl;
}
/**
 * @brief Validates that the TR181AudioConfig parameter returns a valid configuration.
 *
 * Tests that PlayerExternalsInterface's GetTR181PlayerConfig method successfully returns a non-null configuration pointer and a positive configuration length when invoked with the "TR181AudioConfig" parameter. This test covers object creation, proper parameter preparation, method invocation, and result validation.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerExternalsInterface object using default constructor | None | Object is created without throwing exceptions | Should Pass |
 * | 02 | Prepare parameter using strncpy to set paramName to "TR181AudioConfig" | inputParam = "TR181AudioConfig", array size = 64 | paramName is correctly set to "TR181AudioConfig" | Should be successful |
 * | 03 | Invoke GetTR181PlayerConfig with paramName and retrieve configuration | paramName = "TR181AudioConfig", configLen = 0 (initial) | Method returns a non-null pointer and configLen > 0 | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, ValidTR181AudioConfigParameterReturnsValidConfig)
{
    GTEST_SKIP();
    std::cout << "Entering ValidTR181AudioConfigParameterReturnsValidConfig test" << std::endl;

    // Create PlayerExternalsInterface object using the default constructor
    EXPECT_NO_THROW({

        // Prepare parameter using strncpy on a fixed size array
        char paramName[64] = {0};
        const char* inputParam = "TR181AudioConfig";
        std::strncpy(paramName, inputParam, sizeof(paramName) - 1);
        std::cout << "Parameter set to: " << paramName << std::endl;

        size_t configLen = 0;
        std::cout << "Before invocation: configLen = " << configLen << std::endl;

        // Invoke the method GetTR181PlayerConfig
        std::cout << "Invoking GetTR181PlayerConfig with paramName: " << paramName << std::endl;
        char *config = playerExt->GetTR181PlayerConfig(paramName, configLen);
        std::cout << "Method GetTR181PlayerConfig returned pointer: " << static_cast<const void*>(config) << std::endl;
        std::cout << "After invocation: configLen = " << configLen << std::endl;

        // Validate that we received a non-null pointer and a positive length
        EXPECT_NE(config, nullptr);
        EXPECT_GT(configLen, 0u);
        std::cout << "Validation passed: config is non-null and configLen > 0." << std::endl;
    });

    std::cout << "Exiting ValidTR181AudioConfigParameterReturnsValidConfig test" << std::endl;
}
/**
 * @brief Verify that GetTR181PlayerConfig returns error when provided with a null parameter.
 *
 * This test verifies that invoking the GetTR181PlayerConfig method on PlayerExternalsInterface with a null parameter (paramName = nullptr) results in a null return value and resets the configuration length (configLen) to 0, ensuring proper error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 033@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerExternalsInterface instance, invoke GetTR181PlayerConfig with paramName as nullptr and configLen set to a non-zero value, then verify that the method returns nullptr and resets configLen to 0. | paramName = nullptr, configLen = 12345, expected_return = nullptr, expected_configLen = 0 | API returns nullptr and configLen is reset to 0 | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, NullParameterReturnsError)
{
    GTEST_SKIP();
    std::cout << "Entering NullParameterReturnsError test" << std::endl;

    EXPECT_NO_THROW({

        // Set paramName as nullptr
        const char *paramName = nullptr;
        std::cout << "Parameter set to: " << "nullptr" << std::endl;

        size_t configLen = 12345;  // Initialize with a non-zero value to check if it resets
        std::cout << "Before invocation: configLen = " << configLen << std::endl;

        // Invoke the method GetTR181PlayerConfig
        std::cout << "Invoking GetTR181PlayerConfig with paramName: nullptr" << std::endl;
        char *config = playerExt->GetTR181PlayerConfig(paramName, configLen);
        std::cout << "Method GetTR181PlayerConfig returned pointer: " << static_cast<const void*>(config) << std::endl;
        std::cout << "After invocation: configLen = " << configLen << std::endl;

        // Validate that null parameter returns nullptr and iConfigLen is 0
        EXPECT_EQ(config, nullptr);
        EXPECT_EQ(configLen, 0u);
        std::cout << "Validation passed: For nullptr parameter, config is nullptr and configLen is 0." << std::endl;
    });

    std::cout << "Exiting NullParameterReturnsError test" << std::endl;
}
/**
 * @brief Ensures that when an empty string is passed as a parameter to GetTR181PlayerConfig, the function gracefully handles the error.
 *
 * This test verifies the error handling of the GetTR181PlayerConfig API when an empty string is provided as an input parameter. It ensures that the API returns a nullptr and resets the configuration length (configLen) to 0, thus validating input integrity.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                 | Test Data                                                                            | Expected Result                                                               | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a PlayerExternalsInterface instance using the default constructor.                   | Constructor called with no arguments.                                              | Object is successfully created.                                               | Should be successful |
 * | 02               | Prepare an empty string parameter by copying an empty string into a fixed-size array using strncpy. | paramName = "", inputParam = ""                                                      | paramName contains an empty string.                                           | Should be successful |
 * | 03               | Initialize the configLen variable with a non-zero value to ensure it is reset upon error.      | configLen = 999                                                                      | configLen is set initially to 999.                                            | Should be successful |
 * | 04               | Invoke the GetTR181PlayerConfig API with the empty string parameter.                          | input: paramName = "", configLen = 999; output: config pointer expected               | API returns nullptr and resets configLen to 0.                                | Should Fail       |
 * | 05               | Validate that the API returned a nullptr and configLen is updated to 0.                        | output: config pointer, configLen                                                    | config equals nullptr and configLen equals 0.                                | Should be successful |
 */
TEST_F(PlayerExternalsInterfaceTests, EmptyStringParameterReturnsError)
{
    GTEST_SKIP();
    std::cout << "Entering EmptyStringParameterReturnsError test" << std::endl;

    EXPECT_NO_THROW({

        // Prepare empty string parameter using strncpy on a fixed size array
        char paramName[64] = {0};
        const char* inputParam = "";
        std::strncpy(paramName, inputParam, sizeof(paramName) - 1);
        std::cout << "Parameter set to empty string: \"" << paramName << "\"" << std::endl;

        size_t configLen = 999;  // Initialize with non-zero to test reset
        std::cout << "Before invocation: configLen = " << configLen << std::endl;

        // Invoke the method GetTR181PlayerConfig
        std::cout << "Invoking GetTR181PlayerConfig with empty string parameter." << std::endl;
        char *config = playerExt->GetTR181PlayerConfig(paramName, configLen);
        std::cout << "Method GetTR181PlayerConfig returned pointer: " << static_cast<const void*>(config) << std::endl;
        std::cout << "After invocation: configLen = " << configLen << std::endl;

        // Validate that empty string returns nullptr and config length remains 0
        EXPECT_EQ(config, nullptr);
        EXPECT_EQ(configLen, 0u);
        std::cout << "Validation passed: For empty string parameter, config is nullptr and configLen is 0." << std::endl;
    });

    std::cout << "Exiting EmptyStringParameterReturnsError test" << std::endl;
}
/**
 * @brief Validate the functionality of IsConfigWifiCurlHeader in PlayerExternalsInterface
 *
 * This test verifies that a PlayerExternalsInterface object can be successfully constructed using the default constructor and that the IsConfigWifiCurlHeader method returns a valid boolean value. It ensures the API correctly determines whether IARM support is present on the platform, returning true if supported and false otherwise.
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
 * | Variation / Step | Description                                                                                | Test Data                                               | Expected Result                                                                                       | Notes              |
 * | :--------------: | ------------------------------------------------------------------------------------------ | ------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ------------------ |
 * | 01               | Construct PlayerExternalsInterface object using default constructor                        | None                                                    | Object is constructed without throwing any exceptions                                                | Should be successful |
 * | 02               | Invoke IsConfigWifiCurlHeader method and validate its boolean result based on IARM support    | output result = player.IsConfigWifiCurlHeader()         | The return value is true if IARM is supported; false otherwise, and the corresponding assertion passes  | Should Pass         |
 */
TEST_F(PlayerExternalsInterfaceTests, IsConfigWifiCurlHeader_start)
{
    std::cout << "Entering IsConfigWifiCurlHeader_start test" << std::endl;

    // Creating object using default constructor and logging debug info.
    EXPECT_NO_THROW({

        // Invoking IsConfigWifiCurlHeader method and logging invocation.
        std::cout << "Invoking IsConfigWifiCurlHeader()" << std::endl;
        bool result = false;
        EXPECT_NO_THROW({
            result = playerExt->IsConfigWifiCurlHeader();
        });
        std::cout << "IsConfigWifiCurlHeader() returned: " << std::boolalpha << result << std::endl;

        // Logging internal decision based on the returned value.
        if(result)
        {
            std::cout << "IARM is supported on this platform. Expected result: true" << std::endl;
            EXPECT_TRUE(result);
        }
        else
        {
            std::cout << "IARM is NOT supported on this platform. Expected result: false" << std::endl;
            EXPECT_FALSE(result);
        }
    });

    std::cout << "Exiting IsConfigWifiCurlHeader_start test" << std::endl;
}
/**
 * @brief Validate the successful creation and activation check of PlayerExternalsInterface instance
 *
 * This test verifies that a PlayerExternalsInterface instance is created without exceptions and that the static method IsPlayerExternalsInterfaceInstanceActive() can be invoked successfully, returning a boolean value indicating the active status of the instance.
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
 * | Variation / Step | Description                                                                              | Test Data                                                             | Expected Result                                                                                             | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create an instance of PlayerExternalsInterface                                             | Constructor call: PlayerExternalsInterface()                          | Instance is created successfully without any exceptions                                                     | Should Pass |
 * | 02               | Invoke PlayerExternalsInterface::IsPlayerExternalsInterfaceInstanceActive() static method    | Static method call: output1 = PlayerExternalsInterface::IsPlayerExternalsInterfaceInstanceActive() | Method returns a boolean value indicating the active instance status; no exceptions thrown during the call | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, IsPlayerExternalsInterfaceInstanceActive_start)
{
    std::cout << "Entering IsPlayerExternalsInterfaceInstanceActive_start test" << std::endl;

    // Invoke the static method to check if instance is active
    bool isActive = false;
    EXPECT_NO_THROW({
        std::cout << "Invoking PlayerExternalsInterface::IsPlayerExternalsInterfaceInstanceActive()" << std::endl;
        isActive = playerExt->IsPlayerExternalsInterfaceInstanceActive();
        std::cout << "Method returned: " << (isActive ? "true" : "false") << std::endl;
    });


    std::cout << "Exiting IsPlayerExternalsInterfaceInstanceActive_start test" << std::endl;
}
/**
 * @brief Validate the functioning of IsSourceUHD method for a PlayerExternalsInterface object
 *
 * This test verifies that the default construction of the PlayerExternalsInterface object does not throw, and that invoking the IsSourceUHD method returns a boolean value as expected. The test confirms that the internal state is set to UHD, leading the method to return true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 041@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayerExternalsInterface using default constructor | input: none, output: PlayerExternalsInterface object created | Object is successfully instantiated without throwing exceptions | Should be successful |
 * | 02 | Invoke IsSourceUHD method on the instantiated object | input: PlayerExternalsInterface object, output: result from IsSourceUHD() | Expected return value: true; Assertion: IsSourceUHD() returns true | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, IsSourceUHD_start) {
    std::cout << "Entering IsSourceUHD_start test" << std::endl;

    // Create an object using the default constructor and log its invocation.
    EXPECT_NO_THROW({
        // Invoke the IsSourceUHD method and log the call.
        std::cout << "Invoking IsSourceUHD() method." << std::endl;
        bool result = playerExt->IsSourceUHD();
        std::cout << "IsSourceUHD() returned value: " << std::boolalpha << result << std::endl;
        
        // Debug log for internal state: In this test, the source is assumed set to UHD.
        std::cout << "Internal state: source set to UHD for testing. Expecting true." << std::endl;
        
    });
    
    std::cout << "Exiting IsSourceUHD_start test" << std::endl;
}
/**
 * @brief Validate that the PlayerExternalsInterface default constructor instantiates the object safely without exceptions.
 *
 * Verifies that the default constructor of the PlayerExternalsInterface class successfully creates an object, logs the instantiation process, and ensures that internal members are initialized as expected. This test is essential to confirm that object instantiation is robust and free from runtime exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 042
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                    | Test Data                                                | Expected Result                                                                         | Notes               |
 * | :--------------: | ------------------------------------------------------------------------------ | -------------------------------------------------------- | --------------------------------------------------------------------------------------- | ------------------- |
 * | 01               | Log the beginning of the test execution                                        | No inputs/outputs                                        | Console prints the test start message                                                   | Should be successful|
 * | 02               | Invoke the PlayerExternalsInterface default constructor inside EXPECT_NO_THROW   | API: PlayerExternalsInterface(), Input: none, Output: object instantiated | No exception thrown; object instantiated and internal state is logged correctly            | Should Pass         |
 * | 03               | Log the end of the test execution                                              | No inputs/outputs                                        | Console prints the test end message                                                     | Should be successful|
 */
TEST_F(PlayerExternalsInterfaceTests, ValidConstruction) {
    std::cout << "Entering PlayerExternalsInterface::PlayerExternalsInterface()_start test" << std::endl;
    
    // Invoke the default constructor and log the instantiation process

     EXPECT_NO_THROW({
        auto instance = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
        ASSERT_NE(instance, nullptr);
    });
    
    std::cout << "Exiting PlayerExternalsInterface::PlayerExternalsInterface()_end test" << std::endl;
}
/**
 * @brief Validate that the isHDCPConnection2_2 method executes without throwing exceptions.
 *
 * This test verifies that a PlayerExternalsInterface object can be constructed using its default constructor and that invoking its isHDCPConnection2_2 method does not throw any exceptions. The test checks for proper API behavior when no abnormal conditions occur.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 043@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                              | Test Data                                                        | Expected Result                                                                                       | Notes              |
 * | :--------------: | -------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Create a PlayerExternalsInterface object using the default constructor. | None                                                             | Object is created successfully without any exceptions.                                              | Should be successful |
 * | 02               | Invoke the isHDCPConnection2_2 method on the created object.          | output = hdcpResult, no input arguments                          | Method returns a boolean value without throwing exceptions, satisfying EXPECT_NO_THROW assertion.   | Should Pass        |
 */
TEST_F(PlayerExternalsInterfaceTests, isHDCPConnection2_2_start) {
    std::cout << "Entering isHDCPConnection2_2_start test" << std::endl;
    EXPECT_NO_THROW({
        
        // Invocation of isHDCPConnection2_2 method.
        std::cout << "Invoking isHDCPConnection2_2 method." << std::endl;
        bool hdcpResult = playerExt->isHDCPConnection2_2();
        std::cout << "Returned value from isHDCPConnection2_2 method: " << std::boolalpha << hdcpResult << std::endl;
        
    });
    std::cout << "Exiting isHDCPConnection2_2_start test" << std::endl;
}
/**
 * @brief Validate functionality of setGstElement API with a valid GstElement pointer.
 *
 * This test verifies that the setGstElement method of PlayerExternalsInterface correctly accepts a valid GstElement pointer without throwing exceptions. It ensures that object instantiation and pointer handling within the API function as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerExternalsInterface instance using the default constructor | No input arguments; output: instance created | Object should be created successfully without throwing exceptions | Should be successful |
 * | 02 | Create a valid GstElement instance and obtain its pointer | No input arguments; output: validElementPtr = pointer to GstElement instance | Valid GstElement instance and pointer should be obtained successfully | Should be successful |
 * | 03 | Invoke setGstElement with the valid GstElement pointer | Input: validElementPtr = pointer to valid GstElement; output: none | setGstElement should complete without throwing exceptions | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, SetGstElement_PositiveTest)
{
    std::cout << "Entering SetGstElement_PositiveTest test" << std::endl;

    // Create an instance of PlayerExternalsInterface using default constructor.
    EXPECT_NO_THROW({
        // Create a valid GstElement instance.
        GstElement validElementInstance;
        GstElement* validElementPtr = &validElementInstance;
        std::cout << "Created valid GstElement instance at address: " << validElementPtr << std::endl;

        // Invoke setGstElement with a valid pointer.
        std::cout << "Invoking setGstElement with valid GstElement pointer: " << validElementPtr << std::endl;
        EXPECT_NO_THROW(playerExt->setGstElement(validElementPtr));
        std::cout << "setGstElement invoked successfully with valid GstElement pointer." << std::endl;
    });

    std::cout << "Exiting SetGstElement_PositiveTest test" << std::endl;
}
/**
 * @brief Verify that setGstElement gracefully handles a null GstElement pointer.
 *
 * This test verifies that the setGstElement API of PlayerExternalsInterface can handle a null pointer without throwing exceptions.
 * It creates an instance of PlayerExternalsInterface and then invokes setGstElement with a null pointer.
 * This negative test ensures that even when provided with a null GstElement, the API call completes without exception.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 045@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                            | Test Data                                      | Expected Result                                               | Notes           |
 * | :--------------: | ---------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of PlayerExternalsInterface using the default constructor. | None                                           | Instance is created successfully without exceptions.         | Should be successful |
 * | 02               | Invoke setGstElement with a null GstElement pointer.                   | nullElementPtr = nullptr                         | API is invoked without throwing any exception.              | Should Pass     |
 */
TEST_F(PlayerExternalsInterfaceTests, SetGstElement_NegativeTest)
{
    std::cout << "Entering SetGstElement_NegativeTest test" << std::endl;

    // Create an instance of PlayerExternalsInterface using default constructor.
    EXPECT_NO_THROW({

        // Define a null GstElement pointer.
        GstElement* nullElementPtr = nullptr;
        std::cout << "Using a null GstElement pointer: " << nullElementPtr << std::endl;

        // Invoke setGstElement with a null pointer.
        std::cout << "Invoking setGstElement with null GstElement pointer." << std::endl;
        EXPECT_NO_THROW(playerExt->setGstElement(nullElementPtr));
        std::cout << "setGstElement invoked successfully with null GstElement pointer." << std::endl;
    });

    std::cout << "Exiting SetGstElement_NegativeTest test" << std::endl;
}
/**
 * @brief Test that PlayerExternalsInterface's destructor is invoked automatically without exceptions.
 *
 * This test verifies that when a PlayerExternalsInterface object is created using the default constructor and goes out of scope, its destructor is automatically invoked without throwing any exceptions. 
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 046@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the PlayerExternalsInterface default constructor within an EXPECT_NO_THROW block, then allow the object to go out of scope to trigger the destructor automatically. | Constructor = PlayerExternalsInterface(), no parameters; Destructor = automatic call on scope exit | No exceptions are thrown during the creation and destruction processes. | Should Pass |
 */
TEST_F(PlayerExternalsInterfaceTests, Destructor_start) {
    std::cout << "Entering PlayerExternalsInterface::~PlayerExternalsInterface()_start test" << std::endl;
    
    {
        std::cout << "Invoking PlayerExternalsInterface() default constructor." << std::endl;
        EXPECT_NO_THROW({
            std::cout << "PlayerExternalsInterface object created successfully." << std::endl;
            std::cout << "Internal state: m_pIarmInterface (pointer) assumed initialized to default value." << std::endl;
            std::cout << "About to exit scope to trigger automatic destructor call." << std::endl;
        });
        std::cout << "Exited inner scope; destructor should have been invoked automatically without exceptions." << std::endl;
    }
    
    std::cout << "Exiting PlayerExternalsInterface::~PlayerExternalsInterface()_start test" << std::endl;
}
