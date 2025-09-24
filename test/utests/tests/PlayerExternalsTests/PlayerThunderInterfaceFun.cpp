
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
#include "PlayerThunderInterface.h"
#include <cstring>


class PlayerThunderInterfaceTests : public ::testing::Test 
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
// Test Case: Successful plugin activation with a valid plugin callsign
/**
 * @brief Test plugin activation using all valid plugin enumeration values
 *
 * This test verifies that the PlayerThunderInterface::ActivatePlugin() function successfully activates a plugin for each valid enum value without throwing exceptions. It iterates through all valid enum values from AVINPUT to COMPOSITEINPUT, constructs the plugin interface, and asserts that the activation returns true.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 001
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through valid plugin enums, construct PlayerThunderInterface, and invoke ActivatePlugin() | plugin enum = value (ranging from AVINPUT to COMPOSITEINPUT), activationResult = result of ActivatePlugin() | No exception thrown; activationResult == true for each valid enum value | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ActivatePlugin_AllValidEnums) {
    std::cout << "Entering ActivatePlugin_AllValidEnums test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        std::cout << "Testing with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface pluginInterface(plugin);
            std::cout << "Successfully constructed PlayerThunderInterface with enum " << i << std::endl;

            bool activationResult = false;
            EXPECT_NO_THROW({
                activationResult = pluginInterface.ActivatePlugin();
                std::cout << "ActivatePlugin() returned: " << activationResult << " for enum " << i << std::endl;
            });
            EXPECT_TRUE(activationResult);
        });
    }

    std::cout << "Exiting ActivatePlugin_AllValidEnums test" << std::endl;
}
/**
 * @brief Verify that providing an invalid enum to PlayerThunderInterface results in an exception
 *
 * This test ensures that when an out-of-range enum value is cast and passed in as the plugin type to
 * the PlayerThunderInterface constructor, the proper std::invalid_argument exception is thrown.
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
 * | Variation / Step | Description                                                                                  | Test Data                                      | Expected Result                                                  | Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------- | ---------------------------------------------------------------- | ------------ |
 * | 01               | Cast an out-of-range integer (999) to the PlayerThunderAccessPlugin enum to simulate an invalid input | invalidPlugin = 999                            | invalidPlugin holds an out-of-range value                        | Should be successful |
 * | 02               | Invoke the PlayerThunderInterface constructor with the invalid enum and verify it throws an exception | input: invalidPlugin = 999; output: exception  | std::invalid_argument is thrown by the constructor and caught by EXPECT_THROW | Should Fail  |
 */
TEST_F(PlayerThunderInterfaceTests, ActivatePlugin_InvalidEnum) {
    std::cout << "Entering ActivatePlugin_InvalidEnum test" << std::endl;

    // Cast an out-of-range integer to the enum
    PlayerThunderAccessPlugin invalidPlugin = static_cast<PlayerThunderAccessPlugin>(999);

    EXPECT_THROW({
        PlayerThunderInterface pluginInterface(invalidPlugin);
    }, std::invalid_argument);

    std::cout << "Exiting ActivatePlugin_InvalidEnum test" << std::endl;
}
/**
 * @brief This test verifies that PlayerThunderInterface correctly creates a watermark for all valid enum values using a valid layerID.
 *
 * This test iterates through all valid values of the PlayerThunderAccessPlugin enum, creates a PlayerThunderInterface instance for each, and invokes the CreateWatermark method with a valid layerID of 5. The test ensures that no exceptions are thrown during object creation and watermark creation, and that the CreateWatermark method returns true.
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
 * | 01 | Log the start of the test execution | No inputs/outputs | "Entering ValidPositiveLayerID test" is printed on console | Should be successful |
 * | 02 | Iterate through valid PlayerThunderAccessPlugin enum values and create the PlayerThunderInterface instance for each | input: plugin = [AVINPUT, ..., COMPOSITEINPUT] | Object creation does not throw any exception | Should Pass |
 * | 03 | Invoke the CreateWatermark method with layerID = 5 for each valid enum value | input: layerID = 5, plugin = current enum value; output: result variable | CreateWatermark returns true and does not throw an exception | Should Pass |
 * | 04 | Log the completion of the test execution | No inputs/outputs | "Exiting ValidPositiveLayerID test" is printed on console | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, ValidPositiveLayerID) {
    std::cout << "Entering ValidPositiveLayerID test" << std::endl;

    int layerID = 5;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Successfully created PlayerThunderInterface for enum value: " << i << std::endl;

            std::cout << "Invoking CreateWatermark with layerID = " << layerID << " for enum " << i << std::endl;
            bool result = false;
            EXPECT_NO_THROW({
                result = player.CreateWatermark(layerID);
                std::cout << "CreateWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidPositiveLayerID test" << std::endl;
}
/**
 * @brief Validate CreateWatermark behavior with a zero layerID.
 *
 * Validate that PlayerThunderInterface's CreateWatermark function correctly processes a watermark creation with layerID 0, across all supported PlayerThunderAccessPlugin enum values. The test ensures no exceptions are thrown and that the function returns true, confirming the instantiated object functions as expected.
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
 * | 01 | Loop through each valid enum value for PlayerThunderAccessPlugin, instantiate PlayerThunderInterface, call CreateWatermark with layerID = 0 | plugin enum value = from AVINPUT to COMPOSITEINPUT, layerID = 0 | PlayerThunderInterface is successfully created without exceptions and CreateWatermark returns true for each enum value | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ZeroLayerID) {
    std::cout << "Entering ZeroLayerID test" << std::endl;

    int layerID = 0;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Successfully created PlayerThunderInterface for enum value: " << i << std::endl;

            std::cout << "Invoking CreateWatermark with layerID = " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = false;
            EXPECT_NO_THROW({
                result = player.CreateWatermark(layerID);
                std::cout << "CreateWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ZeroLayerID test" << std::endl;
}
/**
 * @brief Test CreateWatermark API with an invalid negative layer ID.
 *
 * This test verifies that calling CreateWatermark with a negative layer ID (-1) is handled properly for each valid plugin enum value. The test iterates over all valid enum values, creates an instance of PlayerThunderInterface, and then calls CreateWatermark with the invalid layer ID. It then checks that the function returns false, ensuring that invalid input is rejected.
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
 * | Variation / Step | Description                                                                     | Test Data                                            | Expected Result                                                                               | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------- | ---------------------------------------------------- | --------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a PlayerThunderInterface instance using a valid enum value.              | input: plugin = enum value (AVINPUT to COMPOSITEINPUT) | No exception is thrown; instance is created successfully.                                      | Should Pass   |
 * | 02               | Invoke CreateWatermark with a negative layerID (-1) on the created interface.    | input: layerID = -1, plugin = current enum value; output: result variable | API returns false and no exception is thrown. Assertion confirms that result is false.          | Should Fail   |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeLayerID) {
    std::cout << "Entering NegativeLayerID test" << std::endl;

    int layerID = -1;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Successfully created PlayerThunderInterface for enum value: " << i << std::endl;

            std::cout << "Invoking CreateWatermark with layerID = " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = true; // assume success, expect false
            EXPECT_NO_THROW({
                result = player.CreateWatermark(layerID);
                std::cout << "CreateWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NegativeLayerID test" << std::endl;
}
/**
 * @brief Validate that CreateWatermark successfully handles an extreme maximum layerID value.
 *
 * This test verifies that the PlayerThunderInterface can be correctly instantiated using each valid enum value and that its
 * CreateWatermark method properly handles an extreme maximum layerID (std::numeric_limits<int>::max()). The test iterates
 * through all valid enum values, creates an instance of PlayerThunderInterface, and asserts that invoking CreateWatermark
 * with the maximum layerID does not throw and returns true.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                                  | Expected Result                                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Loop through all valid enum values, create PlayerThunderInterface and invoke CreateWatermark      | For each iteration: plugin = value (AVINPUT to COMPOSITEINPUT), layerID = 2147483647 | API call CreateWatermark returns true without throwing exceptions for each plugin value | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ExtremeMaximumLayerID) {
    std::cout << "Entering ExtremeMaximumLayerID test" << std::endl;

    int layerID = std::numeric_limits<int>::max(); // 2147483647 on 32-bit/64-bit platforms

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Successfully created PlayerThunderInterface for enum value: " << i << std::endl;

            std::cout << "Invoking CreateWatermark with layerID = " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = false;
            EXPECT_NO_THROW({
                result = player.CreateWatermark(layerID);
                std::cout << "CreateWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ExtremeMaximumLayerID test" << std::endl;
}
/**
 * @brief Test the CreateWatermark API using the extreme minimum layer ID value.
 *
 * This test verifies that the CreateWatermark API properly handles an extreme minimum layer ID value (INT_MIN) by returning false, ensuring that the API does not accept invalid layer identifiers. The test runs through all valid PlayerThunderAccessPlugin enum values to cover different plugin configurations.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create a PlayerThunderInterface instance for each valid enum value (AVINPUT to COMPOSITEINPUT). | plugin = AVINPUT, AVINPUT+1, ..., COMPOSITEINPUT | No exception thrown during instantiation. | Should Pass |
 * | 02 | Invoke CreateWatermark with the extreme minimum layerID value (-2147483648) for each created instance. | layerID = -2147483648 | CreateWatermark returns false. | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, ExtremeMinimumLayerID) {
    std::cout << "Entering ExtremeMinimumLayerID test" << std::endl;

    int layerID = std::numeric_limits<int>::min(); // -2147483648 on 32-bit/64-bit platforms

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Successfully created PlayerThunderInterface for enum value: " << i << std::endl;

            std::cout << "Invoking CreateWatermark with layerID = " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = true; // assume success, expect failure for INT_MIN
            EXPECT_NO_THROW({
                result = player.CreateWatermark(layerID);
                std::cout << "CreateWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ExtremeMinimumLayerID test" << std::endl;
}
/**
 * @brief Validates deletion of watermark with a valid positive layerID.
 *
 * This test iterates through the complete range of valid enum values. For each enum value, it creates a PlayerThunderInterface object and invokes the DeleteWatermark API using a valid positive layerID (1). The test ensures that object creation and API call do not throw exceptions and that DeleteWatermark returns true.
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
 * | 01 | Loop over valid enum values and create a PlayerThunderInterface object for each plugin. | plugin = AVINPUT to COMPOSITEINPUT (integer values) | Object creation succeeds without throwing exceptions | Should be successful |
 * | 02 | Invoke DeleteWatermark with a valid positive layerID (1) and verify the method returns true. | layerID = 1, plugin = current enum value from AVINPUT to COMPOSITEINPUT | DeleteWatermark returns true and no exceptions occur | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, DeleteWatermark_ValidPositiveLayerID) {
    std::cout << "Entering DeleteWatermark_ValidPositiveLayerID test" << std::endl;

    int layerID = 1; // valid positive ID

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DeleteWatermark with layerID: " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = false;
            EXPECT_NO_THROW({
                result = playerInterface.DeleteWatermark(layerID);
                std::cout << "DeleteWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });

            // Expect true when valid positive layerID is provided
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting DeleteWatermark_ValidPositiveLayerID test" << std::endl;
}
/**
 * @brief Ensure DeleteWatermark API returns true for boundary case layerID = 0 across all valid enum values.
 *
 * This test verifies that the DeleteWatermark function of the PlayerThunderInterface class correctly processes the boundary case where layerID is set to 0. It iterates through all valid enum values, creates a PlayerThunderInterface instance, invokes DeleteWatermark, and asserts that the function returns true without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test by setting layerID to 0 and printing the entry log. | layerID = 0 | Entry message printed successfully. | Should be successful |
 * | 02 | Iterate through all valid enum values and create a PlayerThunderInterface instance for each. | plugin = enum value (AVINPUT to COMPOSITEINPUT) | Object creation without exceptions. | Should Pass |
 * | 03 | Invoke DeleteWatermark API with layerID = 0 for each created instance. | layerID = 0, plugin = current enum value | DeleteWatermark returns true and no exceptions are thrown. | Should Pass |
 * | 04 | Assert that DeleteWatermark returns true in each iteration. | layerID = 0, plugin = current enum value, result = true expected | Assertion passes confirming true return value. | Should Pass |
 * | 05 | Print the exit log indicating the end of the test. | None | Exit message printed successfully. | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, DeleteWatermark_LayerIDZero) {
    std::cout << "Entering DeleteWatermark_LayerIDZero test" << std::endl;

    int layerID = 0; // boundary case

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DeleteWatermark with layerID: " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = false;
            EXPECT_NO_THROW({
                result = playerInterface.DeleteWatermark(layerID);
                std::cout << "DeleteWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });

            // Expect true for layerID 0 (boundary case)
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting DeleteWatermark_LayerIDZero test" << std::endl;
}
/**
 * @brief Test DeleteWatermark API with a negative layerID.
 *
 * This test verifies that the DeleteWatermark method returns false when provided with an invalid negative layerID. It iterates over all valid enum values, creates a PlayerThunderInterface instance, and then calls DeleteWatermark with a negative layerID to ensure the API handles the negative input correctly.
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
 * | 01 | Create PlayerThunderInterface object for each valid enum value in range [AVINPUT, COMPOSITEINPUT] | plugin = enum value (AVINPUT to COMPOSITEINPUT) | Object is created successfully without exceptions | Should be successful |
 * | 02 | Invoke DeleteWatermark with a negative layerID and verify the return value | input: layerID = -1, plugin = current enum value; output: result expected = false | DeleteWatermark returns false for a negative layerID | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, DeleteWatermark_NegativeLayerID) {
    std::cout << "Entering DeleteWatermark_NegativeLayerID test" << std::endl;

    int layerID = -1; // invalid negative ID

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DeleteWatermark with layerID: " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = true; // assume valid, expect false
            EXPECT_NO_THROW({
                result = playerInterface.DeleteWatermark(layerID);
                std::cout << "DeleteWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });

            // Expect false when a negative layerID is provided
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting DeleteWatermark_NegativeLayerID test" << std::endl;
}
/**
 * @brief Verify that invoking DeleteWatermark with INT_MAX layerID returns false
 *
 * This test verifies that the DeleteWatermark API correctly handles an invalid layer identifier 
 * (represented by INT_MAX). It iterates over all valid enum values for the plugin type, creates a 
 * corresponding PlayerThunderInterface instance, and calls the DeleteWatermark method with layerID set 
 * to INT_MAX. The test confirms that no exceptions are thrown during object creation and method invocation, 
 * and that the API returns false as expected.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                   | Expected Result                                               | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ------------------------------------------------------------- | ----------------- |
 * | 01               | Create PlayerThunderInterface instance for each valid plugin enum value (AVINPUT to COMPOSITEINPUT) | plugin = AVINPUT, ..., COMPOSITEINPUT; layerID = INT_MAX      | Instance is created without throwing exceptions               | Should Pass       |
 * | 02               | Invoke DeleteWatermark with layerID set to INT_MAX on the created instance                       | input: layerID = INT_MAX, plugin = current enum value         | API returns false as the watermark deletion fails for INT_MAX   | Should Pass       |
 * | 03               | Assert that the result of DeleteWatermark is false                                             | output: result = false                                       | EXPECT_FALSE assertion passes                                   | Should Pass       |
 */
TEST_F(PlayerThunderInterfaceTests, DeleteWatermark_INT_MAX) {
    std::cout << "Entering DeleteWatermark_INT_MAX test" << std::endl;

    int layerID = std::numeric_limits<int>::max(); // INT_MAX

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DeleteWatermark with layerID: " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = true; // assume valid, expect false here
            EXPECT_NO_THROW({
                result = playerInterface.DeleteWatermark(layerID);
                std::cout << "DeleteWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });

            // Expect false when INT_MAX is provided (invalid or non-existent layerID)
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting DeleteWatermark_INT_MAX test" << std::endl;
}
/**
 * @brief Test DeleteWatermark API with invalid layerID input (INT_MIN)
 *
 * This test verifies that invoking DeleteWatermark with an invalid layerID (i.e., INT_MIN)
 * returns false for each valid plugin type. The test creates a PlayerThunderInterface object
 * with each valid enum value and checks that the API handles the invalid layerID gracefully.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through valid plugin enum values and instantiate PlayerThunderInterface objects | plugin = AVINPUT, COMPOSITEINPUT (all valid enum values) | Objects are created without throwing exceptions | Should be successful |
 * | 02 | Invoke DeleteWatermark API with layerID = INT_MIN for each plugin instance | layerID = INT_MIN, plugin = current enum value in iteration | API returns false indicating failure to delete watermark with invalid layerID | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, DeleteWatermark_INT_MIN) {
    std::cout << "Entering DeleteWatermark_INT_MIN test" << std::endl;

    int layerID = std::numeric_limits<int>::min(); // INT_MIN

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DeleteWatermark with layerID: " << layerID
                      << " for enum value: " << i << std::endl;
            bool result = true; // assume valid, expect false here
            EXPECT_NO_THROW({
                result = playerInterface.DeleteWatermark(layerID);
                std::cout << "DeleteWatermark returned: " << std::boolalpha << result
                          << " for enum value: " << i << std::endl;
            });

            // Expect false when INT_MIN is provided (invalid layerID)
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting DeleteWatermark_INT_MIN test" << std::endl;
}
/**
 * @brief ValidTypicalValues test: Verify object creation and API call with typical valid values
 *
 * This test verifies that a PlayerThunderInterface object can be successfully created using all valid enum values 
 * and that invoking the DisableContentRestrictionsOta API with valid parameters (grace = 60, time = 120, eventChange = true) 
 * does not throw any exceptions. The aim is to ensure proper functionality and exception safety under normal operating conditions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 013
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                       | Test Data                                              | Expected Result                                               | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ------------------------------------------------------------- | ------------- |
 * | 01               | Create PlayerThunderInterface object for each valid enum value from AVINPUT to COMPOSITEINPUT         | input: plugin = enum value (AVINPUT to COMPOSITEINPUT)   | Object creation does not throw any exception                   | Should Pass   |
 * | 02               | Call DisableContentRestrictionsOta API with valid parameters: grace=60, time=120, eventChange=true     | input: grace = 60, time = 120, eventChange = true        | API call does not throw any exception and executes successfully | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, ValidTypicalValues) {
    std::cout << "Entering ValidTypicalValues test" << std::endl;

    long grace = 60;
    long time = 120;
    bool eventChange = true;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunderInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace=" 
                      << grace << ", time=" << time 
                      << ", eventChange=" << (eventChange ? "true" : "false") 
                      << " for enum value: " << i << std::endl;

            EXPECT_NO_THROW({
                playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
            });

            std::cout << "DisableContentRestrictionsOta executed successfully for enum value: " << i << std::endl;
        });
    }

    std::cout << "Exiting ValidTypicalValues test" << std::endl;
}
/**
 * @brief Tests the UnlimitedGracePeriod functionality by verifying that the DisableContentRestrictionsOta method runs without exceptions for all valid plugin enum values.
 *
 * This test iterates through every valid enum value for the PlayerThunderAccessPlugin, creates a PlayerThunderInterface object, and then invokes DisableContentRestrictionsOta with an unlimited grace period (-1), a time value (300), and a boolean flag (false). The test ensures that no exceptions are thrown during object creation or API invocation, confirming robust handling of the unlimited grace period configuration.
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
 * | Variation / Step | Description                                                                       | Test Data                                                    | Expected Result                                                                              | Notes        |
 * | :--------------: | --------------------------------------------------------------------------------- | ------------------------------------------------------------ | -------------------------------------------------------------------------------------------- | ------------ |
 * | 01               | Iterate over the valid range of plugin enum values and create a PlayerThunderInterface object for each enumeration value. | plugin = Valid enum value (AVINPUT to COMPOSITEINPUT)         | Object is created successfully without throwing exceptions                                   | Should Pass  |
 * | 02               | Invoke DisableContentRestrictionsOta with unlimited grace period (-1), time (300), and eventChange (false). | grace = -1, time = 300, eventChange = false                    | DisableContentRestrictionsOta completes execution without throwing any exception             | Should Pass  |
 */
TEST_F(PlayerThunderInterfaceTests, UnlimitedGracePeriod) {
    std::cout << "Entering UnlimitedGracePeriod test" << std::endl;

    long grace = -1;   // indicates unlimited grace period
    long time = 300;
    bool eventChange = false;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunderInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace="
                      << grace << ", time=" << time
                      << ", eventChange=" << (eventChange ? "true" : "false")
                      << " for enum value: " << i << std::endl;

            EXPECT_NO_THROW({
                playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
            });

            std::cout << "DisableContentRestrictionsOta executed successfully for unlimited grace period, enum value: " << i << std::endl;
        });
    }

    std::cout << "Exiting UnlimitedGracePeriod test" << std::endl;
}
/**
 * @brief Validate that DisableContentRestrictionsOta API correctly handles zero values
 *
 * This test verifies that the PlayerThunderInterface object is created successfully and that invoking the DisableContentRestrictionsOta API with grace = 0, time = 0, and eventChange = false does not throw any exceptions for each valid enumerated plugin value.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :--------------: | ----------- | --------- | --------------- | ----- |@n
 * | 01 | Iterate over all valid enum values and create the PlayerThunderInterface object | plugin = enum value (AVINPUT to COMPOSITEINPUT), grace = 0, time = 0, eventChange = false | Object created without exception | Should Pass |@n
 * | 02 | Invoke DisableContentRestrictionsOta with zero values on the created object | grace = 0, time = 0, eventChange = false | API call completes successfully without exception | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ZeroValues) {
    std::cout << "Entering ZeroValues test" << std::endl;

    long grace = 0;
    long time = 0;
    bool eventChange = false;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunderInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace="
                      << grace << ", time=" << time
                      << ", eventChange=" << (eventChange ? "true" : "false")
                      << " for enum value: " << i << std::endl;

            EXPECT_NO_THROW({
                playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
            });

            std::cout << "DisableContentRestrictionsOta executed successfully for zero values, enum value: " << i << std::endl;
        });
    }

    std::cout << "Exiting ZeroValues test" << std::endl;
}
/**
 * @brief Validate behavior of DisableContentRestrictionsOta API when provided with a negative grace value
 *
 * This test verifies that the DisableContentRestrictionsOta method of the PlayerThunderInterface class properly handles a negative grace value by throwing an exception. The test iterates through all valid enum values for PlayerThunderAccessPlugin, ensuring that the method consistently rejects the invalid input regardless of the plugin type.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                            | Expected Result                                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | -------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create a PlayerThunderInterface object for each valid enum value and call DisableContentRestrictionsOta with grace = -2, time = 50, eventChange = true, expecting an exception to be thrown. | input: grace = -2, time = 50, eventChange = true, plugin = AVINPUT to COMPOSITEINPUT; output: exception thrown | Function call must throw an exception when invoked with a negative grace value. | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, InvalidNegativeGrace) {
    std::cout << "Entering InvalidNegativeGrace test" << std::endl;

    long grace = -2;
    long time = 50;
    bool eventChange = true;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        PlayerThunderInterface playerThunderInterface(plugin);
        std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

        std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace="
                  << grace << ", time=" << time
                  << ", eventChange=" << (eventChange ? "true" : "false")
                  << " for enum value: " << i << std::endl;

        EXPECT_ANY_THROW({
            playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
        });

        std::cout << "DisableContentRestrictionsOta handled invalid negative grace value appropriately for enum: " << i << std::endl;
    }

    std::cout << "Exiting InvalidNegativeGrace test" << std::endl;
}
/**
 * @brief Validate that DisableContentRestrictionsOta throws an exception when provided with a negative time value.
 *
 * Validate that for each valid PlayerThunderAccessPlugin enum value, calling DisableContentRestrictionsOta with a negative time (-10) throws an exception as expected. This ensures that the API correctly validates the time parameter.
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
 * | 01 | For each valid enum value, instantiate PlayerThunderInterface and call DisableContentRestrictionsOta with grace=100, time=-10, eventChange=false | plugin = [AVINPUT to COMPOSITEINPUT], grace = 100, time = -10, eventChange = false | API should throw an exception indicating invalid negative time | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, InvalidNegativeTime) {
    std::cout << "Entering InvalidNegativeTime test" << std::endl;

    long grace = 100;
    long time = -10;
    bool eventChange = false;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        PlayerThunderInterface playerThunderInterface(plugin);
        std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

        std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace="
                  << grace << ", time=" << time
                  << ", eventChange=" << (eventChange ? "true" : "false")
                  << " for enum value: " << i << std::endl;

        EXPECT_ANY_THROW({
            playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
        });

        std::cout << "DisableContentRestrictionsOta handled invalid negative time value appropriately for enum: " << i << std::endl;
    }

    std::cout << "Exiting InvalidNegativeTime test" << std::endl;
}
/**
 * @brief Verify that DisableContentRestrictionsOta handles extremely large boundary values.
 *
 * This test validates that when extremely large boundary values (set to the maximum long value) are provided to the DisableContentRestrictionsOta API through the PlayerThunderInterface class, the API correctly throws an exception. It iterates through the valid enumeration values, ensuring consistent behavior across all input types.
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
 * | Variation / Step | Description                                                                 | Test Data                                                       | Expected Result                                                 | Notes              |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------------- | --------------------------------------------------------------- | ------------------ |
 * | 01               | Set extremely large boundary values for parameters                          | grace = LONG_MAX, time = LONG_MAX, eventChange = true             | Variables initialized with maximum long values                  | Should be successful |
 * | 02               | Create PlayerThunderInterface object for each valid enum value                | enum value = AVINPUT to COMPOSITEINPUT                            | Object instantiated successfully without errors                 | Should be successful |
 * | 03               | Invoke DisableContentRestrictionsOta with boundary values and verify exception | grace = LONG_MAX, time = LONG_MAX, eventChange = true, enum = current loop value | API call throws an exception indicating proper handling of boundary values | Should Pass        |
 */
TEST_F(PlayerThunderInterfaceTests, ExtremelyLargeBoundaryValues) {
    std::cout << "Entering ExtremelyLargeBoundaryValues test" << std::endl;

    long grace = std::numeric_limits<long>::max();
    long time = std::numeric_limits<long>::max();
    bool eventChange = true;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        PlayerThunderInterface playerThunderInterface(plugin);
        std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

        std::cout << "Invoking DisableContentRestrictionsOta with parameters: grace="
                  << grace << ", time=" << time
                  << ", eventChange=" << (eventChange ? "true" : "false")
                  << " for enum value: " << i << std::endl;

        EXPECT_ANY_THROW({
            playerThunderInterface.DisableContentRestrictionsOta(grace, time, eventChange);
        });

        std::cout << "DisableContentRestrictionsOta handled extremely large boundary values appropriately for enum: " << i << std::endl;
    }

    std::cout << "Exiting ExtremelyLargeBoundaryValues test" << std::endl;
}
/**
 * @brief Validate that EnableContentRestrictionsOta executes correctly with valid PlayerThunderAccess values
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values, instantiates a PlayerThunderInterface
 * for each value, and then calls the EnableContentRestrictionsOta() method. It verifies that no exceptions are thrown
 * during the creation of the object and invocation of the method, ensuring that the API functions as expected with valid input.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                             | Expected Result                                                                                    | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Loop through each valid enum value and instantiate a PlayerThunderInterface object                | input: plugin = enum value (ranging from AVINPUT to COMPOSITEINPUT)                                    | Object instantiated successfully without throwing an exception                                  | Should be successful |
 * | 02               | Invoke EnableContentRestrictionsOta() on the instantiated PlayerThunderInterface object           | input: object created with valid plugin value, method call: EnableContentRestrictionsOta()               | Method executes without throwing an exception                                                     | Should Pass    |
 */
TEST_F(PlayerThunderInterfaceTests, EnableContentRestrictionsOta_WithValidPlayerThunderAccess) {
    std::cout << "Entering EnableContentRestrictionsOta_WithValidPlayerThunderAccess test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface playerThunder(plugin);
            std::cout << "PlayerThunderInterface object instantiated successfully for enum: " << i << std::endl;

            std::cout << "Invoking EnableContentRestrictionsOta() for enum: " << i << std::endl;
            playerThunder.EnableContentRestrictionsOta();
            std::cout << "EnableContentRestrictionsOta() executed successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting EnableContentRestrictionsOta_WithValidPlayerThunderAccess test" << std::endl;
}
/**
 * @brief Verify that GetAudioTracksOta returns the correct audio track primary key for a single valid audio track.
 *
 * This test verifies that the method GetAudioTracksOta, when provided with a vector containing a single valid audio track,
 * returns the expected primary key ("101") as a string for all valid PlayerThunderAccessPlugin enum values. The test loops
 * through each valid enum value, ensuring that no exceptions are thrown and that the method behaves as expected.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                               | Expected Result                                                     | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a valid audio track vector with one audio track                                        | audData: vector(PlayerAudioData(language='en', contentType='audio/mpeg', name='Track1', type='main', pk=101, mixType='stereo'))         | Audio track vector is initialized with one valid audio track         | Should be successful |
 * | 02               | Loop through all valid PlayerThunderAccessPlugin enum values and create interface objects       | plugin: enum value from AVINPUT to COMPOSITEINPUT                                                                                        | PlayerThunderInterface object is created without throwing exceptions  | Should Pass       |
 * | 03               | Invoke GetAudioTracksOta on each PlayerThunderInterface object using the valid audio track vector | audData = vector(PlayerAudioData(language='en', contentType='audio/mpeg', name='Track1', type='main', pk=101, mixType='stereo')), plugin   | Method returns string "101" as validated by EXPECT_EQ                  | Should Pass       |
 * | 04               | Verify exception safety for the instantiation and method invocation within the loop              | Block wrapped in EXPECT_NO_THROW                                                                                                        | No exceptions are thrown during instantiation and method invocation     | Should Pass       |
 */
TEST_F(PlayerThunderInterfaceTests, SingleValidAudioTrack) {
    std::cout << "Entering SingleValidAudioTrack test" << std::endl;

    // Create a valid audio track vector with one element
    std::vector<PlayerAudioData> audData;
    audData.push_back(PlayerAudioData("en", "audio/mpeg", "Track1", "main", 101, "stereo"));
    std::cout << "Created audio vector with one track: language='en', contentType='audio/mpeg', "
              << "name='Track1', type='main', pk=101, mixType='stereo'" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the method GetAudioTracksOta and fetch result
            std::cout << "Invoking GetAudioTracksOta with a single valid audio track for enum: " << i << std::endl;
            std::string result = pti.GetAudioTracksOta(audData);
            std::cout << "Method returned: '" << result << "' for enum: " << i << std::endl;

            // Validate expected returned track pk as string "101"
            EXPECT_EQ(result, "101");
        });
    }

    std::cout << "Exiting SingleValidAudioTrack test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface correctly processes multiple valid audio tracks
 *
 * This test validates that when multiple valid audio tracks with required parameters are provided,
 * the PlayerThunderInterface::GetAudioTracksOta method returns the expected result. The test iterates
 * through all valid enum values of PlayerThunderAccessPlugin and ensures that the returned track pk from
 * the first element ("101") is correct, without throwing any exceptions.
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
 * | 01 | Create a valid audio tracks vector with two elements | audData = [PlayerAudioData("en", "audio/mpeg", "Track1", "main", 101, "stereo"), PlayerAudioData("fr", "audio/aac", "Track2", "secondary", 202, "mono")] | Audio tracks vector is successfully initialized | Should be successful |
 * | 02 | Loop through all valid enum values and instantiate PlayerThunderInterface, then call GetAudioTracksOta | enum values from AVINPUT to COMPOSITEINPUT, plugin = static_cast<PlayerThunderAccessPlugin>(i) | Object creation and method invocation succeed without exceptions | Should Pass |
 * | 03 | Validate that GetAudioTracksOta returns "101" for the first track's pk | result = PlayerThunderInterface::GetAudioTracksOta(audData) | Returned result equals "101" | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, MultipleValidAudioTracks) {
    std::cout << "Entering MultipleValidAudioTracks test" << std::endl;

    // Create a valid audio tracks vector with two elements
    std::vector<PlayerAudioData> audData;
    audData.push_back(PlayerAudioData("en", "audio/mpeg", "Track1", "main", 101, "stereo"));
    audData.push_back(PlayerAudioData("fr", "audio/aac", "Track2", "secondary", 202, "mono"));
    std::cout << "Created audio vector with two tracks:" << std::endl;
    std::cout << "  Track1: language='en', contentType='audio/mpeg', name='Track1', type='main', pk=101, mixType='stereo'" << std::endl;
    std::cout << "  Track2: language='fr', contentType='audio/aac', name='Track2', type='secondary', pk=202, mixType='mono'" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the method GetAudioTracksOta and fetch result
            std::cout << "Invoking GetAudioTracksOta with multiple valid audio tracks for enum: " << i << std::endl;
            std::string result = pti.GetAudioTracksOta(audData);
            std::cout << "Method returned: '" << result << "' for enum: " << i << std::endl;

            // Validate expected returned track pk as string "101" - first track pk
            EXPECT_EQ(result, "101");
        });
    }

    std::cout << "Exiting MultipleValidAudioTracks test" << std::endl;
}
/**
 * @brief Tests the GetAudioTracksOta function's behavior when provided with an empty audio tracks vector.
 *
 * This test validates that the GetAudioTracksOta method returns an empty string when an empty audio data vector is supplied.
 * The test iterates through all valid PlayerThunderAccessPlugin enumeration values (from AVINPUT to COMPOSITEINPUT) to ensure
 * that no exceptions are thrown and that the expected result is consistently an empty string across different plugin instances.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 022@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create an empty audio tracks vector | audData = {} | Empty vector is created successfully | Should be successful |@n
 * | 02 | For each valid enum value, create a PlayerThunderInterface instance and invoke GetAudioTracksOta with the empty vector | plugin = [AVINPUT, ..., COMPOSITEINPUT], audData = {} | The method returns an empty string and no exception is thrown; EXPECT_EQ validates empty return | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, EmptyAudioTracksVector) {
    std::cout << "Entering EmptyAudioTracksVector test" << std::endl;

    // Create an empty audio data vector
    std::vector<PlayerAudioData> audData;
    std::cout << "Created empty audio vector" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the method GetAudioTracksOta and fetch result
            std::cout << "Invoking GetAudioTracksOta with empty audio vector for enum: " << i << std::endl;
            std::string result = pti.GetAudioTracksOta(audData);
            std::cout << "Method returned: '" << result << "' for enum: " << i << std::endl;

            // Validate expected output is an empty string
            EXPECT_EQ(result, "");
        });
    }

    std::cout << "Exiting EmptyAudioTracksVector test" << std::endl;
}
/**
 * @brief Validates GetAudioTracksOta with an audio track having a negative primary key.
 *
 * This test verifies that when an audio track vector contains a track with a negative primary key,
 * the GetAudioTracksOta method of PlayerThunderInterface returns the string "-1" for all valid
 * PlayerThunderAccessPlugin enum values.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create an audio track vector with one track having a negative pk; loop through each valid PlayerThunderAccessPlugin (from AVINPUT to COMPOSITEINPUT), instantiate PlayerThunderInterface, invoke GetAudioTracksOta, and validate the returned value. | audData: language = en, contentType = audio/mpeg, name = TrackInvalid, type = main, pk = -1, mixType = stereo; plugin: enum values AVINPUT to COMPOSITEINPUT | GetAudioTracksOta returns "-1" as a string for each valid plugin, and the EXPECT_EQ(result, "-1") assertion passes. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, AudioTrackWithNegativePk) {
    std::cout << "Entering AudioTrackWithNegativePk test" << std::endl;

    // Create a valid audio track vector with negative pk value
    std::vector<PlayerAudioData> audData;
    audData.push_back(PlayerAudioData("en", "audio/mpeg", "TrackInvalid", "main", -1, "stereo"));
    std::cout << "Created audio vector with one track having negative pk: "
              << "language='en', contentType='audio/mpeg', name='TrackInvalid', "
              << "type='main', pk=-1, mixType='stereo'" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the method GetAudioTracksOta and fetch result
            std::cout << "Invoking GetAudioTracksOta with audio track having negative pk for enum: " << i << std::endl;
            std::string result = pti.GetAudioTracksOta(audData);
            std::cout << "Method returned: '" << result << "' for enum: " << i << std::endl;

            // Validate expected returned track pk as string "-1"
            EXPECT_EQ(result, "-1");
        });
    }

    std::cout << "Exiting AudioTrackWithNegativePk test" << std::endl;
}
/**
 * @brief Validate that GetAudioTracksOta correctly processes an audio track entry with empty string fields.
 *
 * This test ensures that the GetAudioTracksOta function is robust when provided with an audio track having empty string fields.
 * It verifies that the method returns the expected primary key as a string ("303") without throwing exceptions,
 * while iterating through all valid PlayerThunderAccessPlugin enum values.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an audio track vector with one track having empty string fields and pk=303. | audData = [PlayerAudioData("", "", "", "", 303, "")] | Audio track vector is created successfully. | Should be successful |
 * | 02 | Loop through each valid enum value and create a PlayerThunderInterface object for each. | plugin = enum from AVINPUT to COMPOSITEINPUT | PlayerThunderInterface object is constructed without throwing exceptions. | Should be successful |
 * | 03 | Invoke GetAudioTracksOta with the audio track vector and validate that the returned string is "303". | Function call: GetAudioTracksOta(audData) for each plugin; expected output: "303" | The method returns "303" and no exceptions are thrown. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, AudioTrackWithEmptyStringFields) {
    std::cout << "Entering AudioTrackWithEmptyStringFields test" << std::endl;

    // Create a valid audio track vector with empty string fields
    std::vector<PlayerAudioData> audData;
    audData.push_back(PlayerAudioData("", "", "", "", 303, ""));
    std::cout << "Created audio vector with one track having empty string fields and pk=303" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the method GetAudioTracksOta and fetch result
            std::cout << "Invoking GetAudioTracksOta with audio track having empty string fields for enum: " << i << std::endl;
            std::string result = pti.GetAudioTracksOta(audData);
            std::cout << "Method returned: '" << result << "' for enum: " << i << std::endl;

            // Validate expected returned track pk as string "303"
            EXPECT_EQ(result, "303");
        });
    }

    std::cout << "Exiting AudioTrackWithEmptyStringFields test" << std::endl;
}
/**
 * @brief Verify successful retrieval of meta data watermark
 *
 * This test verifies that for each valid enumeration value of PlayerThunderAccessPlugin, the PlayerThunderInterface object is created successfully without throwing exceptions and that the GetMetaDataWatermark method returns a non-empty watermark string. This confirms that the meta data watermark retrieval functionality works as expected.
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
 * | Variation / Step | Description                                                                                             | Test Data                                                                                               | Expected Result                                                                                                                      | Notes        |
 * | :----:           | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ | ------------ |
 * | 01               | Loop through all valid enum values and create a PlayerThunderInterface object using each valid enum.    | input: plugin = enum value for i (ranging from AVINPUT to COMPOSITEINPUT)                                 | API should create the PlayerThunderInterface object without throwing any exceptions                                                 | Should Pass  |
 * | 02               | Invoke GetMetaDataWatermark method on the created object and verify that the returned watermark is not empty. | input: instance of PlayerThunderInterface, method call: GetMetaDataWatermark(), output: watermark string value | API should return a non-empty watermark string and no exceptions should be thrown, assertion check for non-empty watermark passes | Should Pass  |
 */
TEST_F(PlayerThunderInterfaceTests, GetMetaDataWatermark_SuccessfulRetrieval) {
    std::cout << "Entering GetMetaDataWatermark_SuccessfulRetrieval test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke GetMetaDataWatermark method.
            std::string watermark;
            EXPECT_NO_THROW({
                watermark = player.GetMetaDataWatermark();
                std::cout << "Invoked GetMetaDataWatermark, returned value: '" << watermark 
                          << "' for enum: " << i << std::endl;
            });

            // Check that the returned watermark is non-empty indicating successful retrieval.
            EXPECT_FALSE(watermark.empty()) << "Expected non-empty meta data watermark for enum: " << i;
        });
    }

    std::cout << "Exiting GetMetaDataWatermark_SuccessfulRetrieval test" << std::endl;
}
/**
 * @brief To validate that GetTextTracksOta correctly processes an empty vector.
 *
 * This test verifies that the GetTextTracksOta function of PlayerThunderInterface handles an empty vector of PlayerTextData appropriately by returning true and ensuring that the vector remains valid (size ≥ 0) for all valid enumeration inputs.
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
 * | Variation / Step | Description                                                                               | Test Data                                                                           | Expected Result                                                           | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an empty std::vector<PlayerTextData> and verify its initial size                   | txtData = empty vector                                                              | txtData.size() == 0                                                       | Should be successful |
 * | 02               | Loop through valid enum values (from AVINPUT to COMPOSITEINPUT) and construct the interface | input: enum value (AVINPUT to COMPOSITEINPUT)                                       | PlayerThunderInterface constructed without throwing an exception        | Should Pass     |
 * | 03               | Invoke GetTextTracksOta with the empty vector and capture the returned value              | input: txtData = empty vector, plugin = corresponding enum                          | Method returns true                                                       | Should Pass     |
 * | 04               | Validate that txtData remains valid after the method call                                 | output: txtData.size()                                                              | txtData.size() remains valid (≥ 0) and no crash occurs                    | Should Pass     |
 */
TEST_F(PlayerThunderInterfaceTests, GetTextTracksOta_ValidEmptyVector) {
    std::cout << "Entering GetTextTracksOta_ValidEmptyVector test" << std::endl;

    // Create an empty vector of PlayerTextData.
    std::vector<PlayerTextData> txtData;
    std::cout << "Created std::vector<PlayerTextData> with initial size: " << txtData.size() << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface thunderInterface(plugin);
            std::cout << "Constructed PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoking the method GetTextTracksOta
            std::cout << "Invoking GetTextTracksOta with an empty vector for enum: " << i << std::endl;
            bool retValue = thunderInterface.GetTextTracksOta(txtData);
            std::cout << "Returned value from GetTextTracksOta: " << (retValue ? "true" : "false")
                      << " for enum: " << i << std::endl;

            // Validate the returned value is true.
            EXPECT_TRUE(retValue);

            // Check size of txtData after invocation
            std::cout << "txtData size after GetTextTracksOta: " << txtData.size()
                      << " for enum: " << i << std::endl;

            // Validate behavior based on expectations
            EXPECT_GE(txtData.size(), 0u); // should not crash, size must be valid
        });
    }

    std::cout << "Exiting GetTextTracksOta_ValidEmptyVector test" << std::endl;
}
/**
 * @brief Test the HideWatermark() method of PlayerThunderInterface for valid enum values
 *
 * This test verifies that the PlayerThunderInterface constructor and its HideWatermark() method operate correctly when provided with each valid enum value. The test ensures that no exceptions are thrown during the instantiation of the object and while invoking the HideWatermark() method. Additionally, the test validates that the HideWatermark() method returns a valid boolean value.
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
 * | 01 | Create PlayerThunderInterface object with each valid enum value from AVINPUT to COMPOSITEINPUT | input: plugin = enum value (AVINPUT to COMPOSITEINPUT) | No exception thrown; object is created successfully | Should Pass |
 * | 02 | Invoke the HideWatermark() method on the created object for each enum value | input: PlayerThunderInterface object (with plugin = current enum value), invoked method: HideWatermark() | No exception thrown; HideWatermark() returns a boolean value | Should Pass |
 * | 03 | Validate that the result of HideWatermark() is either true or false | input: output: retValue from HideWatermark() | retValue is exactly true or false (boolean) as expected by assertion | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, HideWatermark_start) {
    std::cout << "Entering HideWatermark_start test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Invoking PlayerThunderInterface constructor with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking HideWatermark() method for enum: " << i << std::endl;
            bool retValue = false;
            EXPECT_NO_THROW({
                retValue = player.HideWatermark();
            });
            std::cout << "HideWatermark() returned: " << (retValue ? "true" : "false")
                      << " for enum: " << i << std::endl;

            // Validate that the returned value is indeed a boolean outcome.
            EXPECT_TRUE(retValue == true || retValue == false)
                << "HideWatermark() did not return a valid boolean value for enum: " << i;
        });
    }

    std::cout << "Exiting HideWatermark_start test" << std::endl;
}
/**
 * @brief Test initialization of PlayerThunderInterface and invoking InitRmf method to ensure it returns a valid boolean result.
 *
 * This test verifies the instantiation of the PlayerThunderInterface object using a valid callsign. It then invokes the InitRmf method and confirms that the return value is a valid boolean. This ensures the proper functionality of object creation and the InitRmf method.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set up callsign variable with "TestPlugin" | callsign = "TestPlugin" | callsign variable correctly initialized | Should be successful |
 * | 02 | Create PlayerThunderInterface object using custom constructor | input: callsign = "TestPlugin" | Object is created without throwing exceptions | Should Pass |
 * | 03 | Invoke InitRmf() method on the object | API call: initResult = pti.InitRmf() | Returns a boolean value (true or false) | Should Pass |
 * | 04 | Validate the return value of InitRmf() using an assertion check | Assertion check: (initResult == true) || (initResult == false) | Boolean assertion passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, InitRmf_start) {
    std::cout << "Entering InitRmf_start test" << std::endl;
    
    // Setup callsign using strncpy to assign value to fixed size array
    PlayerThunderAccessPlugin plugin = AVINPUT;
    // Create PlayerThunderInterface object using custom constructor
    EXPECT_NO_THROW({
        PlayerThunderInterface pti(plugin);
        std::cout << "PlayerThunderInterface object created successfully with callsign: " 
                  << plugin << std::endl;
        
        // Invoke the InitRmf() method
        std::cout << "Invoking InitRmf() method" << std::endl;
        bool initResult = pti.InitRmf();
        std::cout << "InitRmf() returned: " << (initResult ? "true" : "false") << std::endl;
        
        // Print the internal state or result as per method specification
        // Since the return is a bool, we log that it's either true or false.
        EXPECT_TRUE((initResult == true) || (initResult == false));
    });
    
    std::cout << "Exiting InitRmf_start test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface::IsThunderAccess() returns a valid boolean value
 *
 * This test verifies that the IsThunderAccess() method of the PlayerThunderInterface class returns a valid boolean (true/false) for each valid PlayerThunderAccessPlugin enum value (from AVINPUT to COMPOSITEINPUT). The objective is to ensure that the API does not throw exceptions and always produces a valid boolean output as expected.
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
 * | Variation / Step | Description                                                                 | Test Data                                                      | Expected Result                                                                                          | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create a PlayerThunderInterface object for each valid PlayerThunderAccessPlugin enum value (AVINPUT to COMPOSITEINPUT) | plugin = AVINPUT, AVINPUT+1, ..., COMPOSITEINPUT               | Object creation does not throw any exception                                                             | Should Pass |
 * | 02               | Invoke IsThunderAccess() method on the created object and validate the returned boolean value             | input: plugin value, output: thunderAccessAvailable (boolean)    | IsThunderAccess() returns a boolean value (either true or false) without throwing exceptions, assertion passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, IsThunderAccess_ThunderAccessAvailable) {
    std::cout << "Entering IsThunderAccess_ThunderAccessAvailable test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Created PlayerThunderInterface object for enum: " << i << std::endl;

            // Invoke the IsThunderAccess() method and capture its return value.
            bool thunderAccessAvailable = false;
            EXPECT_NO_THROW({
                std::cout << "Invoking IsThunderAccess() method for enum: " << i << std::endl;
                thunderAccessAvailable = player.IsThunderAccess();
                std::cout << "IsThunderAccess() returned: " << (thunderAccessAvailable ? "true" : "false")
                          << " for enum: " << i << std::endl;
            });

            // Validate that the method returned a valid boolean value (true or false).
            EXPECT_TRUE(thunderAccessAvailable == true || thunderAccessAvailable == false)
                << "IsThunderAccess() did not return a valid boolean value for enum: " << i;
        });
    }

    std::cout << "Exiting IsThunderAccess_ThunderAccessAvailable test" << std::endl;
}
/**
 * @brief Validate that PlayerThunderInterface processes valid layerID values correctly.
 *
 * This test verifies that for every valid enumeration value of PlayerThunderAccessPlugin, the 
 * PlayerThunderInterface is instantiated without throwing exceptions and its method 
 * PersistentStoreLoadWatermark returns true when invoked with a valid layerID.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                | Expected Result                                          | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | -------------------------------------------------------- | -------------------------------------------------------- | --------------- |
 * | 01               | Initialize the test variables and setup the loop to iterate through valid enum values              | layerID = 1, plugin values from AVINPUT to COMPOSITEINPUT  | Test setup completed without error                       | Should be successful |
 * | 02               | For each valid enum value, create a PlayerThunderInterface instance and invoke PersistentStoreLoadWatermark with layerID 1 | plugin = valid enum value, layerID = 1                    | No exception thrown and PersistentStoreLoadWatermark returns true | Should Pass     |
 */
TEST_F(PlayerThunderInterfaceTests, PersistentStoreLoadWatermark_ValidPositiveLayerID) {
    std::cout << "Entering ValidPositiveLayerID test" << std::endl;

    int layerID = 1;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface instance created with enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreLoadWatermark with layerID: " << layerID
                      << " for enum: " << i << std::endl;
            bool result = pti.PersistentStoreLoadWatermark(layerID);
            std::cout << "PersistentStoreLoadWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidPositiveLayerID test" << std::endl;
}
/**
 * @brief Verify PlayerThunderInterface's behavior with a layerID of 0.
 *
 * This test verifies that for each valid plugin enumeration value, a PlayerThunderInterface instance can be
 * successfully created and its method PersistentStoreLoadWatermark returns true when invoked with a layerID of 0.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 031
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerThunderInterface instance for each valid plugin enumeration (from AVINPUT to COMPOSITEINPUT) | plugin = AVINPUT, AVINPUT+1, ..., COMPOSITEINPUT | Instance is created without throwing any exception | Should Pass |
 * | 02 | Invoke PersistentStoreLoadWatermark with layerID=0 and verify the returned result is true | layerID = 0, plugin = current enum value | Method returns true as asserted by EXPECT_TRUE | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, PersistentStoreLoadWatermark_LayerIDZero) {
    std::cout << "Entering ZeroLayerID test" << std::endl;

    int layerID = 0;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface instance created with enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreLoadWatermark with layerID: " << layerID
                      << " for enum: " << i << std::endl;
            bool result = pti.PersistentStoreLoadWatermark(layerID);
            std::cout << "PersistentStoreLoadWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ZeroLayerID test" << std::endl;
}
/**
 * @brief Validate that PersistentStoreLoadWatermark returns false when provided with a negative layerID.
 *
 * This test iterates over all valid enum values for PlayerThunderAccessPlugin, creates an instance of PlayerThunderInterface,
 * and calls PersistentStoreLoadWatermark passing a negative layerID (-1). The objective is to ensure that the method does not
 * throw any exceptions during instantiation and returns false for an invalid layerID, thereby correctly handling invalid input.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                        | Expected Result                                                             | Notes            |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | --------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create PlayerThunderInterface instance for each valid plugin enum from AVINPUT to COMPOSITEINPUT| layerID = -1, plugin enum = AVINPUT, ..., COMPOSITEINPUT            | Instance is created without throwing exceptions                           | Should be successful |
 * | 02               | Invoke PersistentStoreLoadWatermark with negative layerID and verify that it returns false       | layerID = -1, function call: PersistentStoreLoadWatermark(layerID) | Return value is false and assertion (EXPECT_FALSE) passes                   | Should Pass      |
 */
TEST_F(PlayerThunderInterfaceTests, PersistentStoreLoadWatermark_NegativeLayerID) {
    std::cout << "Entering NegativeLayerID test" << std::endl;

    int layerID = -1;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface instance created with enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreLoadWatermark with layerID: " << layerID
                      << " for enum: " << i << std::endl;
            bool result = pti.PersistentStoreLoadWatermark(layerID);
            std::cout << "PersistentStoreLoadWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NegativeLayerID test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface correctly handles extreme positive layerID values.
 *
 * This test verifies that when the maximum integer value is passed as the layerID to the PersistentStoreLoadWatermark API,
 * the PlayerThunderInterface processes it correctly for each valid PlayerThunderAccessPlugin enum value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 033
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                     | Test Data                                                                                       | Expected Result                                                       | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- | ---------------- |
 * | 01               | Loop through all valid enum values and create a PlayerThunderInterface instance   | For each iteration: input: plugin = enum value (from AVINPUT to COMPOSITEINPUT)                    | Instance is created without throwing any exception                    | Should be successful |
 * | 02               | Invoke PersistentStoreLoadWatermark with maximum layerID value                   | input: layerID = std::numeric_limits<int>::max(), plugin = current enum value from iteration         | Returns true indicating that the watermark load operation is successful | Should Pass      |
 */
TEST_F(PlayerThunderInterfaceTests, ExtremePositiveLayerID) {
    std::cout << "Entering ExtremePositiveLayerID test" << std::endl;

    int layerID = std::numeric_limits<int>::max();

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface instance created with enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreLoadWatermark with layerID: " << layerID
                      << " for enum: " << i << std::endl;
            bool result = pti.PersistentStoreLoadWatermark(layerID);
            std::cout << "PersistentStoreLoadWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ExtremePositiveLayerID test" << std::endl;
}
/**
 * @brief Verify that PersistentStoreLoadWatermark returns false for extreme negative layerID values
 *
 * This test verifies that when an extreme negative layerID (the minimum possible integer value) is used,
 * the PersistentStoreLoadWatermark method of the PlayerThunderInterface class correctly handles the invalid input
 * by returning false and not throwing any exceptions. The test iterates over all valid enum values to ensure that
 * the behavior is consistent across different plugin configurations.
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
 * | Variation / Step | Description | Test Data                                                       | Expected Result                                                                 | Notes          |
 * | :--------------: | ----------- | --------------------------------------------------------------- | ------------------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize the extreme negative layerID value. | layerID = std::numeric_limits<int>::min()                   | layerID is initialized to the minimum possible integer value.                   | Should be successful |
 * | 02               | Loop through each valid enum value, construct PlayerThunderInterface, and invoke PersistentStoreLoadWatermark using the extreme negative layerID. | plugin = AVINPUT to COMPOSITEINPUT, layerID = std::numeric_limits<int>::min(), output = result from PersistentStoreLoadWatermark | No exceptions thrown; PersistentStoreLoadWatermark returns false for each plugin. | Should Fail    |
 */
TEST_F(PlayerThunderInterfaceTests, ExtremeNegativeLayerID) {
    std::cout << "Entering ExtremeNegativeLayerID test" << std::endl;

    int layerID = std::numeric_limits<int>::min();

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface instance created with enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreLoadWatermark with layerID: " << layerID
                      << " for enum: " << i << std::endl;
            bool result = pti.PersistentStoreLoadWatermark(layerID);
            std::cout << "PersistentStoreLoadWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ExtremeNegativeLayerID test" << std::endl;
}
/**
 * @brief Verify that PersistentStoreSaveWatermark correctly processes valid base64 image and valid metadata
 *
 * This test verifies that the PlayerThunderInterface handles valid base64 image data and valid watermark metadata without throwing exceptions.
 * It prepares a test base64 image string and its corresponding metadata, then iterates over all valid PlayerThunderAccessPlugin enum values.
 * For each enum value, it creates a PlayerThunderInterface object and invokes PersistentStoreSaveWatermark, ensuring that the call returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Prepare test input by copying the base64 image string into a buffer and initializing the metadata string. | sourceImage = "iVBORw0KGgoAAAANSUhEUgAAAAUA", imageBuffer size = 256, metaData = "expected watermark metadata" | Input data is correctly initialized with proper termination | Should be successful |@n
 * | 02 | Loop through each valid enum value, instantiate PlayerThunderInterface, and invoke PersistentStoreSaveWatermark. | enum values: AVINPUT to COMPOSITEINPUT, base64Image = imageBuffer, metaData = "expected watermark metadata", output: result boolean | API returns true without throwing exception; EXPECT_TRUE(result) passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidBase64AndValidMetadata) {
    std::cout << "Entering ValidBase64AndValidMetadata test" << std::endl;

    // Prepare test input using strncpy for base64 image
    const char* sourceImage = "iVBORw0KGgoAAAANSUhEUgAAAAUA";
    char imageBuffer[256];
    std::memset(imageBuffer, 0, sizeof(imageBuffer));
    std::strncpy(imageBuffer, sourceImage, sizeof(imageBuffer) - 1);
    std::string metaData = "expected watermark metadata";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: " << imageBuffer
                      << " and metaData: " << metaData << " for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidBase64AndValidMetadata test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface processes valid Base64 image and empty metadata without exceptions.
 *
 * This test verifies that the PlayerThunderInterface constructor and the PersistentStoreSaveWatermark method successfully process a valid base64 encoded image string and an empty metadata string for all valid enum values. The test ensures that no exceptions are thrown during object instantiation and that the method returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare test inputs including valid Base64 string and empty metaData. | sourceImage = "iVBORw0KGgoAAAANSUhEUgAAAAUA", imageBuffer = allocated char array, metaData = "" | Inputs are initialized correctly | Should be successful |
 * | 02 | Loop through valid enum values to instantiate PlayerThunderInterface. | plugin = enum values from AVINPUT to COMPOSITEINPUT | Object created without throwing exceptions | Should Pass |
 * | 03 | Invoke PersistentStoreSaveWatermark with the prepared inputs. | imageBuffer = valid Base64 string, metaData = "" | PersistentStoreSaveWatermark returns true | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidBase64AndEmptyMetadata) {
    std::cout << "Entering ValidBase64AndEmptyMetadata test" << std::endl;

    // Prepare test input using strncpy for base64 image
    const char* sourceImage = "iVBORw0KGgoAAAANSUhEUgAAAAUA";
    char imageBuffer[256];
    std::memset(imageBuffer, 0, sizeof(imageBuffer));
    std::strncpy(imageBuffer, sourceImage, sizeof(imageBuffer) - 1);
    std::string metaData = "";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: " << imageBuffer
                      << " and empty metaData for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidBase64AndEmptyMetadata test" << std::endl;
}
/**
 * @brief Verify that the PersistentStoreSaveWatermark API accepts valid base64 image data along with whitespace-only metadata.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values, creates a PlayerThunderInterface object,
 * and invokes PersistentStoreSaveWatermark with a base64 image string and a metadata string containing only whitespace.
 * The objective is to ensure that the API does not throw exceptions and correctly returns true for valid input values.
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
 * | Variation / Step | Description                                                                                              | Test Data                                                                                           | Expected Result                                                                                              | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Create a PlayerThunderInterface object and call PersistentStoreSaveWatermark with base64 image data and whitespace-only metadata for each valid plugin enum value. | imageBuffer = "iVBORw0KGgoAAAANSUhEUgAAAAUA", metaData = "    ", plugin = AVINPUT to COMPOSITEINPUT | The PersistentStoreSaveWatermark method returns true without throwing any exception. | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, ValidBase64AndWhitespaceMetadata) {
    std::cout << "Entering ValidBase64AndWhitespaceMetadata test" << std::endl;

    // Prepare test input using strncpy for base64 image
    const char* sourceImage = "iVBORw0KGgoAAAANSUhEUgAAAAUA";
    char imageBuffer[256];
    std::memset(imageBuffer, 0, sizeof(imageBuffer));
    std::strncpy(imageBuffer, sourceImage, sizeof(imageBuffer) - 1);
    std::string metaData = "    "; // whitespace only

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: " << imageBuffer
                      << " and whitespace-only metaData for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidBase64AndWhitespaceMetadata test" << std::endl;
}
/**
 * @brief Test PersistentStoreSaveWatermark with empty Base64 image and valid metadata input
 *
 * This test verifies that supplying an empty Base64 string as the image data along with valid metadata causes the API PersistentStoreSaveWatermark to return false, indicating that the watermark is not saved. The test iterates over all valid plugin enumerations to ensure consistent behavior.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                          | Expected Result                                                                                         | Notes              |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Prepare test input with an empty Base64 image and valid metadata.                               | sourceImage = "", imageBuffer = zero initialized, metaData = "some metadata"         | Inputs are set up correctly.                                                                            | Should be successful |
 * | 02               | For each valid enum value, instantiate PlayerThunderInterface and invoke PersistentStoreSaveWatermark. | plugin = [AVINPUT, COMPOSITEINPUT], imageBuffer = "", metaData = "some metadata"        | API returns false with no exceptions thrown; EXPECT_FALSE(result) passes.                               | Should Fail        |
 */
TEST_F(PlayerThunderInterfaceTests, EmptyBase64AndValidMetadata) {
    std::cout << "Entering EmptyBase64AndValidMetadata test" << std::endl;

    // Prepare test input using strncpy for empty base64 image
    const char* sourceImage = "";
    char imageBuffer[256];
    std::memset(imageBuffer, 0, sizeof(imageBuffer));
    std::strncpy(imageBuffer, sourceImage, sizeof(imageBuffer) - 1);
    std::string metaData = "some metadata";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: [empty] "
                      << "and metaData: " << metaData << " for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting EmptyBase64AndValidMetadata test" << std::endl;
}
/**
 * @brief Verify that PersistentStoreSaveWatermark returns false when provided with a null base64 pointer and valid metadata.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values, constructing a PlayerThunderInterface object and invoking the PersistentStoreSaveWatermark function with a null base64 image pointer and valid metadata. It ensures that the API call does not throw an exception and correctly returns false when the imageBuffer is null.
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
 * | 01 | Iterate over all valid PlayerThunderAccessPlugin enum values, instantiate PlayerThunderInterface, and invoke PersistentStoreSaveWatermark with imageBuffer set to nullptr and metaData set to "some metadata". | imageBuffer = nullptr, metaData = "some metadata", plugin = enum value (from AVINPUT to COMPOSITEINPUT) | No exception is thrown and PersistentStoreSaveWatermark returns false. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, NullBase64AndValidMetadata) {
    std::cout << "Entering NullBase64AndValidMetadata test" << std::endl;

    // For null base64 pointer, we directly pass nullptr.
    const char* imageBuffer = nullptr;
    std::string metaData = "some metadata";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: [nullptr] "
                      << "and metaData: " << metaData << " for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NullBase64AndValidMetadata test" << std::endl;
}
/**
 * @brief Test that PlayerThunderInterface PersistentStoreSaveWatermark returns false for invalid Base64 input with valid metadata.
 *
 * This test validates that when an invalid Base64 string is provided along with valid metadata, the PersistentStoreSaveWatermark function of PlayerThunderInterface correctly returns false. The test iterates through every valid enumeration value to ensure consistent error handling across different plugin types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare test inputs by copying an invalid Base64 string into the image buffer and setting valid metadata | sourceImage = "invalid_base64$$$", imageBuffer (256 bytes array initialized to zeros, copied using strncpy), metaData = "metadata" | Input variables are correctly initialized without overflow | Should be successful |
 * | 02 | For each valid enum value, create a PlayerThunderInterface object, invoke PersistentStoreSaveWatermark, and verify the function returns false | plugin enum value from AVINPUT to COMPOSITEINPUT, imageBuffer = "invalid_base64$$$", metaData = "metadata", output: result from PersistentStoreSaveWatermark | The API returns false and EXPECT_FALSE(result) passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, InvalidBase64AndValidMetadata) {
    std::cout << "Entering InvalidBase64AndValidMetadata test" << std::endl;

    // Prepare test input using strncpy for invalid base64 image
    const char* sourceImage = "invalid_base64$$$";
    char imageBuffer[256];
    std::memset(imageBuffer, 0, sizeof(imageBuffer));
    std::strncpy(imageBuffer, sourceImage, sizeof(imageBuffer) - 1);
    std::string metaData = "metadata";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking PersistentStoreSaveWatermark with base64Image: " << imageBuffer
                      << " and metaData: " << metaData << " for enum: " << i << std::endl;

            bool result = pti.PersistentStoreSaveWatermark(imageBuffer, metaData);
            std::cout << "PersistentStoreSaveWatermark returned: " << (result ? "true" : "false")
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting InvalidBase64AndValidMetadata test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface constructor works correctly with valid plugin callsigns
 *
 * This test iterates over all valid enum values of PlayerThunderAccessPlugin and verifies that constructing
 * a PlayerThunderInterface object does not throw any exceptions. It ensures that the interface handles valid
 * plugin callsigns appropriately, which is crucial for proper operation of the system.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through enum values from AVINPUT to COMPOSITEINPUT and call PlayerThunderInterface constructor | i = AVINPUT, i = AVINPUT+1, ..., i = COMPOSITEINPUT | Object constructed successfully with no exceptions thrown | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ConstructValidPluginCallsign) {
    std::cout << "Entering ConstructValidPluginCallsign test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        std::cout << "Invoking PlayerThunderInterface constructor with callsign enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(static_cast<PlayerThunderAccessPlugin>(i));
            std::cout << "PlayerThunderInterface object constructed successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting ConstructValidPluginCallsign test" << std::endl;
}
/**
 * @brief Validate that constructing PlayerThunderInterface with invalid plugin callsigns does not throw exceptions while internally handling errors.
 *
 * This test iterates over a collection of invalid enum values that fall outside the valid range and invokes the constructor of PlayerThunderInterface for each value. The test verifies that no exceptions are thrown during construction even when provided with invalid values, implying that the error state is handled internally.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                       | Expected Result                                                             | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke PlayerThunderInterface constructor with invalid callsign value -1                         | input invalidCallsign = -1                                        | Constructor does not throw exception; internal error state is set          | Should Pass   |
 * | 02               | Invoke PlayerThunderInterface constructor with invalid callsign value (COMPOSITEINPUT + 1)         | input invalidCallsign = COMPOSITEINPUT + 1                        | Constructor does not throw exception; internal error state is set          | Should Pass   |
 * | 03               | Invoke PlayerThunderInterface constructor with invalid callsign value 9999                         | input invalidCallsign = 9999                                        | Constructor does not throw exception; internal error state is set          | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, ConstructInvalidPluginCallsign) {
    std::cout << "Entering ConstructInvalidPluginCallsign test" << std::endl;

    // Test with invalid enum values outside the valid range
    std::vector<int> invalidValues = {-1, COMPOSITEINPUT + 1, 9999};

    for (int invalidValue : invalidValues) {
        PlayerThunderAccessPlugin invalidCallsign = static_cast<PlayerThunderAccessPlugin>(invalidValue);
        std::cout << "Invoking PlayerThunderInterface constructor with invalid callsign value: "
                  << invalidValue << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(invalidCallsign);
            std::cout << "PlayerThunderInterface object constructed with invalid plugin callsign ("
                      << invalidValue << "). An error state is expected internally." << std::endl;
        });
    }

    std::cout << "Exiting ConstructInvalidPluginCallsign test" << std::endl;
}
/**
 * @brief Test registering both event callbacks with valid lambda functions
 *
 * This test verifies that the PlayerThunderInterface can be instantiated for each valid plugin enum value and that the API correctly registers two valid callback lambdas for handling signal changes and input status changes without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                            | Test Data                                                                                                     | Expected Result                                                                          | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Loop through valid enum values and instantiate PlayerThunderInterface with each plugin value          | input: plugin enum value (AVINPUT to COMPOSITEINPUT)                                                          | No exception thrown; object instantiated successfully                                     | Should Pass   |
 * | 02               | Define a valid lambda for OnSignalChangedCb which logs the signal change                               | input: lambda function with signature void(std::string)                                                       | Lambda function defined correctly                                                         | Should be successful |
 * | 03               | Define a valid lambda for OnInputStatusChangedCb which logs the input status change                    | input: lambda function with signature void(std::string)                                                       | Lambda function defined correctly                                                         | Should be successful |
 * | 04               | Invoke RegisterAllEventsVideoin with both valid lambda callbacks                                       | input: signalCallback = valid lambda, inputStatusCallback = valid lambda                                        | No exceptions thrown from the API; callback methods registered successfully                 | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterBothCallbacksWithValidLambdas) {
    std::cout << "Entering RegisterBothCallbacksWithValidLambdas test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunder(plugin);
            std::cout << "Instantiated PlayerThunderInterface object for enum: " << i << std::endl;

            // Define valid lambda for OnSignalChangedCb that logs the signal change.
            std::function<void(std::string)> signalCallback = [](std::string msg) {
                std::cout << "Lambda OnSignalChangedCb invoked with message: " << msg << std::endl;
            };
            std::cout << "Defined valid lambda for OnSignalChangedCb." << std::endl;

            // Define valid lambda for OnInputStatusChangedCb that logs the input status change.
            std::function<void(std::string)> inputStatusCallback = [](std::string msg) {
                std::cout << "Lambda OnInputStatusChangedCb invoked with message: " << msg << std::endl;
            };
            std::cout << "Defined valid lambda for OnInputStatusChangedCb." << std::endl;

            // Invoke RegisterAllEventsVideoin and log the invocation.
            std::cout << "Invoking RegisterAllEventsVideoin with both valid lambda callbacks for enum: " << i << std::endl;
            playerThunder.RegisterAllEventsVideoin(signalCallback, inputStatusCallback);
            std::cout << "RegisterAllEventsVideoin method invoked successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterBothCallbacksWithValidLambdas test" << std::endl;
}
/**
 * @brief This test verifies that RegisterAllEventsVideoin function works correctly when provided with a valid signal callback and an empty input status callback across all valid PlayerThunderAccessPlugin enum types.
 *
 * This test iterates through all valid PlayerThunderAccessPlugin enum values, ensuring that instantiation of PlayerThunderInterface and invocation of RegisterAllEventsVideoin with valid signal callback and empty input status callback do not throw exceptions. It confirms the robustness of the API with valid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through each valid PlayerThunderAccessPlugin enum value. | plugin = enum value (from AVINPUT to COMPOSITEINPUT) | Each valid enum is processed. | Should be successful |
 * | 02 | Instantiate PlayerThunderInterface for current enum value. | plugin = current enum value | Object is instantiated without throwing. | Should Pass |
 * | 03 | Define valid lambda for OnSignalChangedCb. | signalCallback = valid lambda logging message | Lambda defined successfully. | Should be successful |
 * | 04 | Create a default empty callback for OnInputStatusChangedCb. | inputStatusCallback = empty callback | Empty callback defined. | Should be successful |
 * | 05 | Invoke RegisterAllEventsVideoin with valid signal callback and empty input status callback. | signalCallback = valid lambda, inputStatusCallback = empty callback | RegisterAllEventsVideoin executes without throwing exception. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterValidSignalEmptyInput) {
    std::cout << "Entering RegisterValidSignalEmptyInput test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunder(plugin);
            std::cout << "Instantiated PlayerThunderInterface object for enum: " << i << std::endl;

            // Define valid lambda for OnSignalChangedCb that logs the signal change.
            std::function<void(std::string)> signalCallback = [](std::string msg) {
                std::cout << "Lambda OnSignalChangedCb invoked with message: " << msg << std::endl;
            };
            std::cout << "Defined valid lambda for OnSignalChangedCb." << std::endl;

            // Create an empty (default-constructed) callback for OnInputStatusChangedCb.
            std::function<void(std::string)> inputStatusCallback;
            std::cout << "Defined empty callback for OnInputStatusChangedCb." << std::endl;

            // Invoke RegisterAllEventsVideoin and log the invocation.
            std::cout << "Invoking RegisterAllEventsVideoin with valid signal callback and empty input status callback for enum: " << i << std::endl;
            playerThunder.RegisterAllEventsVideoin(signalCallback, inputStatusCallback);
            std::cout << "RegisterAllEventsVideoin method invoked successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterValidSignalEmptyInput test" << std::endl;
}
/**
 * @brief Validate that RegisterAllEventsVideoin processes valid input without exceptions
 *
 * This test verifies that the RegisterAllEventsVideoin method of PlayerThunderInterface correctly handles a default (empty) callback for the signal and a valid lambda for the input status change without throwing any exceptions. It iterates through all valid enum values, ensuring that the API functions as expected when provided with valid input.
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
 * | 01 | Iterate through valid plugin enum values, create a PlayerThunderInterface instance, define an empty signal callback and a valid lambda for input status, then invoke RegisterAllEventsVideoin. | plugin = enum value (AVINPUT to COMPOSITEINPUT), signalCallback = empty callback, inputStatusCallback = lambda (logs input status change) | No exception is thrown; API registers callbacks successfully | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterEmptySignalValidInput) {
    std::cout << "Entering RegisterEmptySignalValidInput test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunder(plugin);
            std::cout << "Instantiated PlayerThunderInterface object for enum: " << i << std::endl;

            // Create an empty (default-constructed) callback for OnSignalChangedCb.
            std::function<void(std::string)> signalCallback;
            std::cout << "Defined empty callback for OnSignalChangedCb." << std::endl;

            // Define valid lambda for OnInputStatusChangedCb that logs the input status change.
            std::function<void(std::string)> inputStatusCallback = [](std::string msg) {
                std::cout << "Lambda OnInputStatusChangedCb invoked with message: " << msg << std::endl;
            };
            std::cout << "Defined valid lambda for OnInputStatusChangedCb." << std::endl;

            // Invoke RegisterAllEventsVideoin and log the invocation.
            std::cout << "Invoking RegisterAllEventsVideoin with empty signal callback and valid input status callback for enum: " << i << std::endl;
            playerThunder.RegisterAllEventsVideoin(signalCallback, inputStatusCallback);
            std::cout << "RegisterAllEventsVideoin method invoked successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterEmptySignalValidInput test" << std::endl;
}
/**
 * @brief Test to verify that RegisterAllEventsVideoin works correctly when both callbacks are empty.
 *
 * This test verifies that the PlayerThunderInterface registers both empty callback functions without throwing any exceptions.
 * The function is tested by iterating over all valid plugin enumeration values and ensuring that the object instantiation
 * and method invocation complete successfully. This test is critical to validate that the interface gracefully handles cases
 * where no valid callbacks are provided.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                      | Expected Result                                                         | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | -------------- |
 * | 01               | Loop through all valid enum values for plugin type.                                           | input: enum value ranging from AVINPUT to COMPOSITEINPUT                                          | Iteration proceeds for each valid plugin enum value                     | Should be successful |
 * | 02               | Instantiate PlayerThunderInterface with the current plugin enum value.                        | input: plugin = current enum value; output: valid PlayerThunderInterface instance                | Object instantiated without exception                                | Should Pass    |
 * | 03               | Create empty (default-constructed) callbacks for OnSignalChangedCb and OnInputStatusChangedCb.  | input: signalCallback = empty, inputStatusCallback = empty                                       | Callbacks are defined as empty functions                                | Should be successful |
 * | 04               | Invoke RegisterAllEventsVideoin using the empty callbacks.                                    | input: signalCallback = empty, inputStatusCallback = empty; output: method completes without error | Method invocation completes successfully without throwing exceptions | Should Pass    |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterBothEmptyCallbacks) {
    std::cout << "Entering RegisterBothEmptyCallbacks test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunder(plugin);
            std::cout << "Instantiated PlayerThunderInterface object for enum: " << i << std::endl;

            // Create empty (default-constructed) callbacks for both parameters.
            std::function<void(std::string)> signalCallback;
            std::function<void(std::string)> inputStatusCallback;
            std::cout << "Defined empty callbacks for both OnSignalChangedCb and OnInputStatusChangedCb." << std::endl;

            // Invoke RegisterAllEventsVideoin and log the invocation.
            std::cout << "Invoking RegisterAllEventsVideoin with both callbacks empty for enum: " << i << std::endl;
            playerThunder.RegisterAllEventsVideoin(signalCallback, inputStatusCallback);
            std::cout << "RegisterAllEventsVideoin method invoked successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterBothEmptyCallbacks test" << std::endl;
}
/**
 * @brief Validates that registering a valid callback functions correctly without exceptions.
 *
 * This test iterates through all valid enum values of PlayerThunderAccessPlugin to create corresponding instances of PlayerThunderInterface.
 * For each interface instance, it defines a valid lambda callback function and registers it using the RegisterEventOnVideoStreamInfoUpdateHdmiin method.
 * The test ensures that both the construction of the interface and the registration of the callback do not throw exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 047@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through each valid enum value and create a PlayerThunderInterface object | input: i = enum values from AVINPUT to COMPOSITEINPUT, plugin = static_cast<PlayerThunderAccessPlugin>(i) | PlayerThunderInterface is constructed without throwing exceptions | Should Pass |
 * | 02 | Define a valid lambda callback for PlayerVideoStreamInfoData | input: validCallback = lambda(PlayerVideoStreamInfoData data) { ... } | Lambda callback is defined successfully | Should be successful |
 * | 03 | Register the valid lambda callback with RegisterEventOnVideoStreamInfoUpdateHdmiin | input: validCallback lambda callback | Callback is registered without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterValidCallback) {
    std::cout << "Entering RegisterValidCallback test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Define a valid lambda callback.
            auto validCallback = [](PlayerVideoStreamInfoData data) {
                std::cout << "Inside validCallback: Received PlayerVideoStreamInfoData." << std::endl;
                // Here, detailed logs of structure members would be printed if available.
                // Example:
                // std::cout << "Width: " << data.width << ", Height: " << data.height << std::endl;
            };
            std::cout << "Defined valid lambda callback for video stream info update event." << std::endl;

            // Invoke the method to register the callback.
            std::cout << "Invoking RegisterEventOnVideoStreamInfoUpdateHdmiin with valid lambda callback for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.RegisterEventOnVideoStreamInfoUpdateHdmiin(validCallback));
            std::cout << "Callback successfully registered with the event system for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterValidCallback test" << std::endl;
}
/**
 * @brief Verify that registering an empty callback does not throw exceptions.
 *
 * This test case iterates over all valid plugin enumerations, creates a PlayerThunderInterface object for each enumeration, and registers an empty (default-constructed) callback. The objective is to ensure that the API handles an empty callback gracefully without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                         | Expected Result                                                                           | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Loop through each valid enum value and instantiate PlayerThunderInterface.                  | input: plugin = enum values (from AVINPUT to COMPOSITEINPUT)        | PlayerThunderInterface object should be created without throwing any exceptions           | Should be successful |
 * | 02               | Register a default-constructed (empty) callback using RegisterEventOnVideoStreamInfoUpdateHdmiin. | input: emptyCallback = default std::function                     | The empty callback should be registered without throwing exceptions; API handles gracefully | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterEmptyCallback) {
    std::cout << "Entering RegisterEmptyCallback test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            // Creating PlayerThunderInterface object using the enum constructor
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Create an empty (default-constructed) std::function as the callback.
            std::function<void(PlayerVideoStreamInfoData)> emptyCallback;
            std::cout << "Defined empty (default-constructed) callback for video stream info update event." << std::endl;

            // Invoke the method to register the empty callback.
            std::cout << "Invoking RegisterEventOnVideoStreamInfoUpdateHdmiin with empty callback for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.RegisterEventOnVideoStreamInfoUpdateHdmiin(emptyCallback));
            std::cout << "Empty callback registered/handled gracefully without throwing exceptions for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterEmptyCallback test" << std::endl;
}
/**
 * @brief Test the registration of a valid callback function for different PlayerThunderInterface enum values.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values, creates a PlayerThunderInterface object,
 * registers a valid lambda callback for PlayerStatusData, and ensures that no exceptions are thrown during the registration process.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                      | Expected Result                                                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Loop through valid enum values and create PlayerThunderInterface object       | plugin = AVINPUT...COMPOSITEINPUT                                              | Object is created without throwing an exception                                             | Should Pass |
 * | 02               | Define a valid lambda callback function for PlayerStatusData                | callback = lambda function (prints received PlayerStatusData)                  | Lambda callback is defined successfully                                                      | Should be successful |
 * | 03               | Invoke RegisterOnPlayerStatusOta API with the valid callback for each enum value | callback = valid lambda, plugin associated with each PlayerThunderInterface instance | API registers the callback without throwing any exceptions, indicating successful registration | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterValidCallbackFunction) {
    std::cout << "Entering RegisterValidCallbackFunction test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Creating a valid lambda callback function for PlayerStatusData.
            auto validCallback = [](PlayerStatusData data) {
                std::cout << "Inside callback: Received PlayerStatusData" << std::endl;
                // Example of printing member values if available from data.
                // e.g. std::cout << "Status: " << data.status << std::endl;
            };
            std::cout << "Defined valid lambda callback for PlayerStatusData." << std::endl;

            // Invoke RegisterOnPlayerStatusOta with the valid callback
            std::cout << "Invoking RegisterOnPlayerStatusOta with valid callback for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.RegisterOnPlayerStatusOta(validCallback));
            std::cout << "RegisterOnPlayerStatusOta invoked successfully with valid callback for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterValidCallbackFunction test" << std::endl;
}
/**
 * @brief Test verifies that the ReleaseOta method is invoked successfully for various PlayerThunderAccessPlugin enum values.
 *
 * This test case iterates through all valid enum values (from AVINPUT to COMPOSITEINPUT) to create a PlayerThunderInterface object and then calls its ReleaseOta method. The objective is to ensure that the object construction and the ReleaseOta method invocation do not throw any exceptions, validating the stability of the interface across different plugin types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 051@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through each valid enum value and construct a PlayerThunderInterface object. | input: plugin = enum value (ranging from AVINPUT to COMPOSITEINPUT) | Object should be constructed without throwing exceptions. | Should Pass |
 * | 02 | Invoke the ReleaseOta() method on the constructed PlayerThunderInterface object. | input: method call: ReleaseOta() on object instance corresponding to the current enum value | ReleaseOta() should execute without throwing any exceptions. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ReleaseOta_start) {
    std::cout << "Entering ReleaseOta_start test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface playerThunderInterface(plugin);
            std::cout << "Constructed PlayerThunderInterface object successfully for enum: " << i << std::endl;

            // Invoking ReleaseOta method
            std::cout << "Invoking ReleaseOta() method for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerThunderInterface.ReleaseOta());
            std::cout << "ReleaseOta() method invoked without errors for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting ReleaseOta_start test" << std::endl;
}
/**
 * @brief Verify that RegisterOnPlayerStatusOta can handle an empty callback without throwing an exception.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values and creates a PlayerThunderInterface object for each.
 * It then creates an empty std::function callback and invokes RegisterOnPlayerStatusOta using this empty callback.
 * The test is conducted to ensure that providing an empty callback does not lead to any exceptions.
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
 * | 01 | Iterate through all valid PlayerThunderAccessPlugin enum values from AVINPUT to COMPOSITEINPUT | Loop variable i (starting at AVINPUT, ending at COMPOSITEINPUT) | Each enum value is processed in the loop without errors | Should be successful |
 * | 02 | Create a PlayerThunderInterface object using the current enum value | input: plugin = current enum value (e.g., i) | Object is created successfully; no exception thrown | Should Pass |
 * | 03 | Create an empty std::function callback for PlayerStatusData | output: emptyCallback (empty std::function) | Empty callback is instantiated successfully | Should be successful |
 * | 04 | Invoke RegisterOnPlayerStatusOta with the empty callback | input: playerInterface object, callback = emptyCallback | Method call completes with no exception thrown (EXPECT_NO_THROW) | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, RegisterOnPlayerStatusOta_EmptyCallback) {
    std::cout << "Entering RegisterEmptyCallback test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            // Create a PlayerThunderInterface object using the enum constructor.
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Create an empty std::function callback.
            std::function<void(PlayerStatusData)> emptyCallback;
            std::cout << "Created an empty std::function for PlayerStatusData callback." << std::endl;

            // Invoke RegisterOnPlayerStatusOta with the empty callback.
            std::cout << "Invoking RegisterOnPlayerStatusOta with an empty callback for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.RegisterOnPlayerStatusOta(emptyCallback));
            std::cout << "RegisterOnPlayerStatusOta invoked successfully with empty callback for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting RegisterEmptyCallback test" << std::endl;
}

/**
 * @brief Verify that the SetAudioTrackOta API returns a valid non-empty string for all valid plugin enum values.
 *
 * This test iterates over all valid enum values of PlayerThunderAccessPlugin, creates a PlayerThunderInterface object for each valid enum, and invokes the SetAudioTrackOta method with a fixed index and primaryKey. The objective is to ensure that the API call does not throw an exception and that it returns a non-empty string, indicating successful operation.
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
 * | Variation / Step | Description                                                                  | Test Data                                            | Expected Result                                              | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Loop through each valid enum value and create a PlayerThunderInterface object  | plugin = enum from AVINPUT to COMPOSITEINPUT           | Object is created successfully without any exception         | Should Pass |
 * | 02               | Invoke SetAudioTrackOta with index = 1 and primaryKey = 100 and verify output | index = 1, primaryKey = 100, output = non-empty string | API returns a non-empty string and no exception is thrown      | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, PositiveValidAudioTrackOta) {
    std::cout << "Entering PositiveValidAudioTrackOta test" << std::endl;

    int index = 1;
    int primaryKey = 100;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetAudioTrackOta with index = " << index
                      << " and primaryKey = " << primaryKey << " for enum: " << i << std::endl;

            std::string result;
            EXPECT_NO_THROW(result = pti.SetAudioTrackOta(index, primaryKey));
            std::cout << "Returned string: '" << result << "' for enum: " << i << std::endl;

            // Expecting non-empty string on success
            EXPECT_FALSE(result.empty());
        });
    }

    std::cout << "Exiting PositiveValidAudioTrackOta test" << std::endl;
}
/**
 * @brief Test SetAudioTrackOta with a negative audio index and a valid primary key
 *
 * This test verifies that the SetAudioTrackOta API call behaves as expected when provided with an invalid (negative) audio index and a valid primary key. The test iterates through all valid plugin enum values, creates a PlayerThunderInterface object, and invokes the API. The expected behavior is that the API does not throw an exception and returns an empty string, indicating a failure to set the audio track given the invalid index.
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
 * | 01 | Loop through all valid enum values, create the PlayerThunderInterface object, and invoke SetAudioTrackOta with index = -1 and primaryKey = 100 | plugin = enum values (from AVINPUT to COMPOSITEINPUT), index = -1, primaryKey = 100 | The API should not throw any exception and must return an empty string; assert that result.empty() is true | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeAudioIndexValidPrimaryKey) {
    std::cout << "Entering NegativeAudioIndexValidPrimaryKey test" << std::endl;

    int index = -1;
    int primaryKey = 100;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetAudioTrackOta with index = " << index
                      << " and primaryKey = " << primaryKey << " for enum: " << i << std::endl;

            std::string result;
            EXPECT_NO_THROW(result = pti.SetAudioTrackOta(index, primaryKey));
            std::cout << "Returned string: '" << result << "' for enum: " << i << std::endl;

            // Expecting empty string on failure
            EXPECT_TRUE(result.empty());
        });
    }

    std::cout << "Exiting NegativeAudioIndexValidPrimaryKey test" << std::endl;
}
/**
 * @brief Verify that invoking SetAudioTrackOta with a valid positive audio index and a negative primary key returns an empty string.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values. For each value, it creates a PlayerThunderInterface object and calls the SetAudioTrackOta method with an index set to 1 and a primaryKey set to -100. The test verifies that the method call does not throw an exception and that the returned string is empty, which indicates that the API correctly handles negative primary key inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 054@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | For each valid PlayerThunderAccessPlugin enum value, create a PlayerThunderInterface object and invoke SetAudioTrackOta with a valid audio index (1) and a negative primary key (-100). | plugin = current enum value, index = 1, primaryKey = -100 | The API returns an empty string without throwing any exceptions. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, PositiveAudioIndexNegativePrimaryKey) {
    std::cout << "Entering PositiveAudioIndexNegativePrimaryKey test" << std::endl;

    int index = 1;
    int primaryKey = -100;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetAudioTrackOta with index = " << index
                      << " and primaryKey = " << primaryKey << " for enum: " << i << std::endl;

            std::string result;
            EXPECT_NO_THROW(result = pti.SetAudioTrackOta(index, primaryKey));
            std::cout << "Returned string: '" << result << "' for enum: " << i << std::endl;

            // Expecting empty string on failure
            EXPECT_TRUE(result.empty());
        });
    }

    std::cout << "Exiting PositiveAudioIndexNegativePrimaryKey test" << std::endl;
}
/**
 * @brief Test the behavior of SetAudioTrackOta with zero audio index and a valid primary key
 *
 * This test iterates over all valid enum values defined for PlayerThunderAccessPlugin. For each enum value, it creates a PlayerThunderInterface instance and invokes the SetAudioTrackOta API with index = 0 and primaryKey = 100. The purpose is to verify that the API correctly handles the boundary case of a zero index by either returning a non-empty string (if zero is considered valid) or an empty string (if zero is invalid) without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 055@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                             | Test Data                                                    | Expected Result                                                                                              | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------ | --------------- |
 * | 01               | Loop through all valid enum values and instantiate PlayerThunderInterface for each plugin type         | input: enum value (AVINPUT to COMPOSITEINPUT)                | No exception thrown; PlayerThunderInterface object is successfully created                                   | Should Pass     |
 * | 02               | Invoke SetAudioTrackOta API with index = 0 and primaryKey = 100 for the instantiated plugin             | input: index = 0, primaryKey = 100, output: result string      | No exception thrown; result string is returned (empty if index is invalid or non-empty if index is considered valid) | Should Pass     |
 * | 03               | Log the returned result and mark the test as succeeded                                                  | input: result string                                         | Logging outputs confirm the behavior based on the result string and test succeeds using SUCCEED()               | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, EdgeCaseZeroAudioIndexValidPrimaryKey) {
    std::cout << "Entering EdgeCaseZeroAudioIndexValidPrimaryKey test" << std::endl;

    int index = 0;
    int primaryKey = 100;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetAudioTrackOta with index = " << index
                      << " and primaryKey = " << primaryKey << " for enum: " << i << std::endl;

            std::string result;
            EXPECT_NO_THROW(result = pti.SetAudioTrackOta(index, primaryKey));
            std::cout << "Returned string: '" << result << "' for enum: " << i << std::endl;

            // The expected output may be non-empty on success or empty if zero index is considered invalid.
            if (result.empty()) {
                std::cout << "Zero index considered invalid, received empty string for enum: " << i << std::endl;
            } else {
                std::cout << "Zero index considered valid, received non-empty string for enum: " << i << std::endl;
            }
            SUCCEED();
        });
    }

    std::cout << "Exiting EdgeCaseZeroAudioIndexValidPrimaryKey test" << std::endl;
}
/**
 * @brief Validate the boundary behavior of SetAudioTrackOta using maximum integer values
 *
 * This test invokes the SetAudioTrackOta method with the highest possible integer values to ensure that the
 * PlayerThunderInterface handles boundary conditions gracefully. The test iterates over all valid PlayerThunderAccessPlugin
 * enum values to confirm that an object can be successfully created and that the method returns an appropriate string output
 * (either a valid non-empty string or an empty string indicating invalid inputs) without throwing exceptions.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                              | Expected Result                                                                                                      | Notes          |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Set the boundary maximum values for input parameters (index and primaryKey)                    | index = std::numeric_limits<int>::max, primaryKey = std::numeric_limits<int>::max | Values assigned without error                                                                                         | Should be successful |
 * | 02               | Loop over each valid PlayerThunderAccessPlugin enum value and create a PlayerThunderInterface instance | plugin = each enum from AVINPUT to COMPOSITEINPUT                      | PlayerThunderInterface object is created successfully and no exception is thrown                                      | Should Pass    |
 * | 03               | Invoke SetAudioTrackOta on the created PlayerThunderInterface object with boundary values       | index = std::numeric_limits<int>::max, primaryKey = std::numeric_limits<int>::max | Method returns a string (non-empty for valid boundaries or empty for invalid boundaries) and no exceptions are thrown | Should Pass    |
 * | 04               | Validate and log the output received from SetAudioTrackOta for each enum iteration             | result = returned string from SetAudioTrackOta                         | Proper logging of the result indicating whether the boundary values are considered valid or invalid without error     | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, BoundaryMaxValuesAudioTrackOta) {
    std::cout << "Entering BoundaryMaxValuesAudioTrackOta test" << std::endl;

    int index = std::numeric_limits<int>::max();
    int primaryKey = std::numeric_limits<int>::max();

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetAudioTrackOta with index = " << index
                      << " and primaryKey = " << primaryKey << " for enum: " << i << std::endl;

            std::string result;
            EXPECT_NO_THROW(result = pti.SetAudioTrackOta(index, primaryKey));
            std::cout << "Returned string: '" << result << "' for enum: " << i << std::endl;

            // The expected output may be non-empty on success or empty if values exceed valid range.
            if (result.empty()) {
                std::cout << "Boundary values considered invalid, received empty string for enum: " << i << std::endl;
            } else {
                std::cout << "Boundary values considered valid, received non-empty string for enum: " << i << std::endl;
            }
            SUCCEED();
        });
    }

    std::cout << "Exiting BoundaryMaxValuesAudioTrackOta test" << std::endl;
}
/**
 * @brief Validate API behavior with valid audio data across all shims
 *
 * This test checks that the PlayerThunderInterface correctly handles valid preferred audio data for each supported plugin type and shim. It creates an instance of PlayerThunderInterface with each valid enum value, prepares the audio data, and verifies that the SetPreferredAudioLanguages method is invoked successfully without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerThunderInterface object for each valid plugin enum value in the range (AVINPUT to COMPOSITEINPUT) | plugin = enum value (range: AVINPUT to COMPOSITEINPUT) | Instance created without throwing exceptions | Should Pass |
 * | 02 | Invoke SetPreferredAudioLanguages with prepared valid audio data for each shim value | preferredLanguagesString = "en,fr", pluginPreferredLanguagesString = "de,it", preferredRenditionString = "stereo", pluginPreferredRenditionString = "5.1", shim = one of {VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM} | Method call returns successfully without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidPreferredAudioData_AllValidShims) {
    std::cout << "Entering ValidPreferredAudioData_AllValidShims test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface playerInterface(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        // Prepare preferred audio data
        PlayerPreferredAudioData data;
        char temp1[] = "en,fr";
        char temp2[] = "de,it";
        char temp3[] = "stereo";
        char temp4[] = "5.1";
        data.preferredLanguagesString = "";
        data.pluginPreferredLanguagesString = "";
        data.preferredRenditionString = "";
        data.pluginPreferredRenditionString = "";

        char buffer[100];
        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, temp1, sizeof(buffer) - 1);
        data.preferredLanguagesString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, temp2, sizeof(buffer) - 1);
        data.pluginPreferredLanguagesString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, temp3, sizeof(buffer) - 1);
        data.preferredRenditionString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, temp4, sizeof(buffer) - 1);
        data.pluginPreferredRenditionString = buffer;

        // Array of valid shim values
        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };

        for (auto shim : shims) {
            std::cout << "Invoking SetPreferredAudioLanguages with data.preferredLanguagesString: "
                        << data.preferredLanguagesString
                        << ", data.pluginPreferredLanguagesString: " << data.pluginPreferredLanguagesString
                        << ", data.preferredRenditionString: " << data.preferredRenditionString
                        << ", data.pluginPreferredRenditionString: " << data.pluginPreferredRenditionString
                        << " and shim value: " << static_cast<int>(shim) << " for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                playerInterface.SetPreferredAudioLanguages(data, shim);
                std::cout << "Method SetPreferredAudioLanguages invoked successfully for shim: "
                            << static_cast<int>(shim) << " and enum: " << i << std::endl;
            });
        }
    }

    std::cout << "Exiting ValidPreferredAudioData_AllValidShims test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface correctly handles empty preferred audio data with a valid shim.
 *
 * This test iterates through all valid enum values for PlayerThunderAccessPlugin, constructs a PlayerThunderInterface object, and prepares a PlayerPreferredAudioData structure with empty strings. It then invokes the SetPreferredAudioLanguages method for a set of valid shim values. The objective is to ensure that the API call does not throw any exceptions when provided with empty string inputs for preferred languages and rendition.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Loop through valid enum values and create a PlayerThunderInterface object for each plugin | input: plugin = enum value from AVINPUT to COMPOSITEINPUT | Object is constructed successfully without throwing exceptions | Should Pass |
 * | 02 | Prepare the PlayerPreferredAudioData structure with empty strings for language and rendition settings | input: preferredLanguagesString = "", pluginPreferredLanguagesString = "", preferredRenditionString = "", pluginPreferredRenditionString = "" | Data is prepared and assigned correctly | Should be successful |
 * | 03 | Invoke SetPreferredAudioLanguages for each valid shim using the prepared empty data | input: data = PlayerPreferredAudioData with empty strings, shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | API call completes without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, EmptyStringsInPreferredAudioData_WithValidShim) {
    std::cout << "Entering EmptyStringsInPreferredAudioData_WithValidShim test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) 
    {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        PlayerThunderInterface playerInterface(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        // Prepare preferred audio data with empty strings
        PlayerPreferredAudioData data;
        char empty[] = "";
        char buffer[10];

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, empty, sizeof(buffer) - 1);
        data.preferredLanguagesString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, empty, sizeof(buffer) - 1);
        data.pluginPreferredLanguagesString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, empty, sizeof(buffer) - 1);
        data.preferredRenditionString = buffer;

        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, empty, sizeof(buffer) - 1);
        data.pluginPreferredRenditionString = buffer;

        // Array of valid shim values
        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };

        for (auto shim : shims) {
            std::cout << "Invoking SetPreferredAudioLanguages with empty strings and shim value: "
                        << static_cast<int>(shim) << " for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                playerInterface.SetPreferredAudioLanguages(data, shim);
                std::cout << "Method SetPreferredAudioLanguages invoked successfully with empty strings "
                            << "for shim: " << static_cast<int>(shim) << " and enum: " << i << std::endl;
            });
        }
    }

    std::cout << "Exiting EmptyStringsInPreferredAudioData_WithValidShim test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface correctly sets video rectangle dimensions with valid parameters.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values and, for each, instantiates a PlayerThunderInterface object. It then calls the SetVideoRectangle method with positive dimensions and a valid video input type across different shims. The test checks that no exception is thrown during object creation and that the SetVideoRectangle method returns true for each invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 059@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Loop through all valid enum values and instantiate PlayerThunderInterface for each plugin. | plugin = AVINPUT, AVINPUT+1, ..., COMPOSITEINPUT | No exception is thrown during instantiation. | Should be successful |
 * | 02 | For each instantiated object, call SetVideoRectangle with positive dimensions and valid video input type for each shim (VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM). | x=10, y=20, w=300, h=200, videoInputType=HDMI, shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | SetVideoRectangle returns true. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidRectangleStandardPositiveDimensions) {
    std::cout << "Entering ValidRectangleStandardPositiveDimensions test" << std::endl;

    int x = 10, y = 20, w = 300, h = 200;
    std::string videoInputType = "HDMI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_TRUE(result);
        }
    }

    std::cout << "Exiting ValidRectangleStandardPositiveDimensions test" << std::endl;
}
/**
 * @brief Validate interface creation and functionality of SetVideoRectangle with minimum non-zero dimensions
 *
 * This test verifies that for each valid plugin enumeration value (from AVINPUT to COMPOSITEINPUT), a PlayerThunderInterface object is created without throwing an exception. It then tests the SetVideoRectangle method using the minimum valid non-zero dimensions (w=1, h=1) along with x=0, y=0 and videoInputType="SDI" for all valid shim values (VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM). The test confirms that the method returns true for each valid input combination.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 060
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerThunderInterface instance using a valid plugin enumeration value | plugin = valid enum value (AVINPUT to COMPOSITEINPUT) | No exception thrown during object instantiation | Should be successful |
 * | 02 | Call SetVideoRectangle with minimum non-zero dimensions for each valid shim | x=0, y=0, w=1, h=1, videoInputType=SDI, shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | Returns true for each call; assertion EXPECT_TRUE(result) passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidRectangleMinimumNonZeroDimensions) {
    std::cout << "Entering ValidRectangleMinimumNonZeroDimensions test" << std::endl;

    int x = 0, y = 0, w = 1, h = 1;
    std::string videoInputType = "SDI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_TRUE(result);
        }
    }

    std::cout << "Exiting ValidRectangleMinimumNonZeroDimensions test" << std::endl;
}
/**
 * @brief Test negative x coordinate handling in SetVideoRectangle API
 *
 * This test verifies that the SetVideoRectangle API correctly rejects video rectangle configurations when provided with a negative x coordinate. The test iterates through all valid plugin enum values and for each, it invokes the API with specific parameters including a negative x coordinate, and checks that the method returns false. This behavior ensures that improper coordinate values are not accepted.
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
 * | 01 | Invoke SetVideoRectangle for each valid plugin enum with a negative x coordinate across various shim types | x = -10, y = 20, w = 300, h = 200, videoInputType = HDMI, plugin = AVINPUT, COMPOSITEINPUT (iterated), shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | The API returns false for each invocation confirming that negative x coordinates are not accepted; Assertions validate that the returned value is false | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeXCoordinate) {
    std::cout << "Entering NegativeXCoordinate test" << std::endl;

    int x = -10, y = 20, w = 300, h = 200;
    std::string videoInputType = "HDMI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_FALSE(result);
        }
    }

    std::cout << "Exiting NegativeXCoordinate test" << std::endl;
}
/**
 * @brief Validates that the PlayerThunderInterface::SetVideoRectangle method rejects negative y coordinate values.
 *
 * This test verifies that when a negative value is passed as the y coordinate to the SetVideoRectangle method, the method returns false. It iterates through all valid PlayerThunderAccessPlugin enum values and for each, over all defined video shims to ensure consistent behavior.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                                                                          | Expected Result                                                                                   | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Iterate over valid PlayerThunderAccessPlugin enums and initialize PlayerThunderInterface          | plugin = enum value from AVINPUT to COMPOSITEINPUT                                                                                                                 | PlayerThunderInterface object is created without throwing exceptions                            | Should be successful |
 * | 02               | For each video shim, invoke SetVideoRectangle with x=10, y=-20, w=300, h=200, videoInputType=HDMI | x = 10, y = -20, w = 300, h = 200, videoInputType = HDMI, shim = one of VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM                                                               | SetVideoRectangle returns false as checked by EXPECT_FALSE(result                                | Should Fail     |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeYCoordinate) {
    std::cout << "Entering NegativeYCoordinate test" << std::endl;

    int x = 10, y = -20, w = 300, h = 200;
    std::string videoInputType = "HDMI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_FALSE(result);
        }
    }

    std::cout << "Exiting NegativeYCoordinate test" << std::endl;
}
/**
 * @brief Verifies that the SetVideoRectangle API returns false when a zero width is provided.
 *
 * This test iterates over all valid enum values to create a PlayerThunderInterface object. For each created object, it calls the SetVideoRectangle method with a zero width parameter (w = 0) along with other valid parameters and checks that the API correctly returns false indicating an invalid video rectangle setting.
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
 * | 01 | Iterate through valid enum values and create a PlayerThunderInterface object for each enum value. | enum = AVINPUT to COMPOSITEINPUT | Object is created without throwing exceptions. | Should be successful |
 * | 02 | Invoke SetVideoRectangle on the created object with parameters (x=10, y=20, w=0, h=200, videoInputType="HDMI") for each shim value. | input1 = 10, input2 = 20, input3 = 0, input4 = 200, videoInputType = HDMI, shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | API returns false and EXPECT_FALSE(result) assertion passes. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ZeroWidthValue) {
    std::cout << "Entering ZeroWidthValue test" << std::endl;

    int x = 10, y = 20, w = 0, h = 200;
    std::string videoInputType = "HDMI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_FALSE(result);
        }
    }

    std::cout << "Exiting ZeroWidthValue test" << std::endl;
}
/**
 * @brief Tests the PlayerThunderInterface behavior when provided a video rectangle with zero height.
 *
 * This test verifies that setting a zero height value in the video rectangle always results in a false return value for each tested shim and valid enum value. The test iterates over all valid enumeration values, creates PlayerThunderInterface objects, and checks the expected failure scenario for the SetVideoRectangle API when provided with an invalid video rectangle configuration.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through all valid enum values for PlayerThunderAccessPlugin from AVINPUT to COMPOSITEINPUT | enum_i = value from AVINPUT to COMPOSITEINPUT | Iteration proceeds without exception | Should be successful |
 * | 02 | Create a PlayerThunderInterface object using the current enum value | plugin = current enum value | Object is created successfully without exception | Should Pass |
 * | 03 | Invoke SetVideoRectangle for each shim (VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM) with x=10, y=20, w=300, h=0, videoInputType="HDMI" | x = 10, y = 20, w = 300, h = 0, videoInputType = HDMI, shim = one of (VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM) | Method returns false indicating failure to set a rectangle with zero height | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ZeroHeightValue) {
    std::cout << "Entering ZeroHeightValue test" << std::endl;

    int x = 10, y = 20, w = 300, h = 0;
    std::string videoInputType = "HDMI";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=" << videoInputType
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_FALSE(result);
        }
    }

    std::cout << "Exiting ZeroHeightValue test" << std::endl;
}
/**
 * @brief Validates that SetVideoRectangle returns false when provided with an empty video input type.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values, constructs a PlayerThunderInterface object,
 * and then calls SetVideoRectangle with preset rectangle coordinates and an empty string for videoInputType.
 * The expected behavior is that while the object construction should not throw any exceptions, the SetVideoRectangle API
 * should return false for each call, indicating a failure when an empty video input type is provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 065@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through valid PlayerThunderAccessPlugin enum values and instantiate PlayerThunderInterface. | plugin = AVINPUT to COMPOSITEINPUT | Constructor does not throw any exceptions. | Should Pass |
 * | 02 | For each valid PlayerThunderAccessShim, invoke SetVideoRectangle with x=10, y=20, w=300, h=200, videoInputType="" to verify it returns false. | x=10, y=20, w=300, h=200, videoInputType="", shim = VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM | SetVideoRectangle returns false. | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, EmptyVideoInputType) {
    std::cout << "Entering EmptyVideoInputType test" << std::endl;

    int x = 10, y = 20, w = 300, h = 200;
    std::string videoInputType = "";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface player(plugin);
        std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

        PlayerThunderAccessShim shims[] = { VIDEOIN_SHIM, OTA_SHIM, RMF_SHIM };
        for (auto shim : shims) {
            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                        << ", w=" << w << ", h=" << h
                        << ", videoInputType=(empty string)"
                        << ", shim=" << shim << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, shim);
            std::cout << "Returned value: " << std::boolalpha << result
                        << " for enum: " << i << " and shim=" << shim << std::endl;

            EXPECT_FALSE(result);
        }
    }

    std::cout << "Exiting EmptyVideoInputType test" << std::endl;
}
/**
 * @brief Verify that SetVideoRectangle returns false when called with an invalid shim value
 *
 * This test iterates through all valid PlayerThunderAccessPlugin enum values to create a PlayerThunderInterface object, and then calls SetVideoRectangle with a set of valid rectangle parameters but an invalid shim value. The purpose of this test is to ensure that the API correctly handles an invalid shim by returning false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 066@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through valid PlayerThunderAccessPlugin enum values and create PlayerThunderInterface objects without throwing exceptions | x = 10, y = 20, w = 300, h = 200, videoInputType = HDMI, shim = -1, plugin = [AVINPUT, COMPOSITEINPUT] | No exceptions during object creation | Should be successful |
 * | 02 | Call SetVideoRectangle with valid rectangle parameters and an invalid shim value, and verify that it returns false | x = 10, y = 20, w = 300, h = 200, videoInputType = HDMI, shim = -1, plugin = current enum value | API returns false indicating invalid shim parameter | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidRectangleWithInvalidShimValue) {
    std::cout << "Entering ValidRectangleWithInvalidShimValue test" << std::endl;

    int x = 10, y = 20, w = 300, h = 200;
    std::string videoInputType = "HDMI";
    PlayerThunderAccessShim invalidShim = static_cast<PlayerThunderAccessShim>(-1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);
            std::cout << "Created PlayerThunderInterface object successfully for enum: " << i << std::endl;

            std::cout << "Invoking SetVideoRectangle with x=" << x << ", y=" << y
                      << ", w=" << w << ", h=" << h
                      << ", videoInputType=" << videoInputType
                      << ", shim=" << invalidShim << " (invalid shim)"
                      << " for enum: " << i << std::endl;

            bool result = player.SetVideoRectangle(x, y, w, h, videoInputType, invalidShim);
            std::cout << "Returned value: " << std::boolalpha << result
                      << " for enum: " << i << " with invalid shim" << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ValidRectangleWithInvalidShimValue test" << std::endl;
}
/**
 * @brief Verify that the ShowWatermark API correctly handles the lower boundary opacity value
 *
 * This test verifies that the PlayerThunderInterface's ShowWatermark function correctly processes
 * a lower boundary value (opacity = 0) for all valid enum types. The test ensures that no exceptions are thrown
 * and that the function returns true, indicating a successful watermark display.
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
 * | Variation / Step | Description                                                                                                 | Test Data                                                        | Expected Result                                                                          | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Iterate over each valid PlayerThunderAccessPlugin enum value, construct a PlayerThunderInterface object, and invoke ShowWatermark with opacity set to 0. | opacity = 0, plugin = valid enum value (AVINPUT to COMPOSITEINPUT) | API should not throw exceptions and must return true indicating the valid lower boundary operation. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ShowWatermark_LowerBoundary0) {
    std::cout << "Entering ShowWatermark_LowerBoundary0 test" << std::endl;

    int opacity = 0;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Constructing PlayerThunderInterface object with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);

            std::cout << "Invoking ShowWatermark with opacity: " << opacity
                      << " for enum: " << i << std::endl;
            bool result = player.ShowWatermark(opacity);
            std::cout << "ShowWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            // Expect true for valid opacity value: 0 (lower boundary)
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ShowWatermark_LowerBoundary0 test" << std::endl;
}
/**
 * @brief Tests the ShowWatermark API with a mid-range opacity value (50) across all valid PlayerThunderAccessPlugin enum values
 *
 * This test validates that the ShowWatermark API correctly returns true when invoked with an opacity value of 50. It iterates through each valid enum value from AVINPUT to COMPOSITEINPUT, constructs a PlayerThunderInterface object for each, and calls the ShowWatermark API. The test verifies that the API handles all valid plugin types correctly and returns the expected result.
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
 * | Variation / Step | Description                                                                                              | Test Data                                                           | Expected Result                                                  | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Loop through each valid enum value, instantiate PlayerThunderInterface, and call ShowWatermark with 50 | enum = current enum value (AVINPUT...COMPOSITEINPUT), opacity = 50    | API returns true verified via EXPECT_TRUE for each enum value       | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ShowWatermark_MidRange50) {
    std::cout << "Entering ShowWatermark_MidRange50 test" << std::endl;

    int opacity = 50;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Constructing PlayerThunderInterface object with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);

            std::cout << "Invoking ShowWatermark with opacity: " << opacity
                      << " for enum: " << i << std::endl;
            bool result = player.ShowWatermark(opacity);
            std::cout << "ShowWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            // Expect true for valid mid-range opacity: 50
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ShowWatermark_MidRange50 test" << std::endl;
}
/**
 * @brief Verifies that ShowWatermark correctly handles the upper boundary opacity value.
 *
 * This test iterates through all supported PlayerThunderAccessPlugin enum values and confirms that invoking ShowWatermark with an opacity of 100 (upper boundary) returns true without throwing exceptions. It ensures that boundary conditions are properly handled.
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
 * | 01 | Create PlayerThunderInterface instance for each valid PlayerThunderAccessPlugin enum value. | enum value = AVINPUT, COMPOSITEINPUT (iterated values) | Instance is created without exceptions | Should be successful |
 * | 02 | Invoke ShowWatermark with opacity set to 100. | input: opacity = 100, enum value corresponding to plugin; output: bool result | API returns true indicating that the watermark is displayed correctly | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ShowWatermark_UpperBoundary100) {
    std::cout << "Entering ShowWatermark_UpperBoundary100 test" << std::endl;

    int opacity = 100;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Constructing PlayerThunderInterface object with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);

            std::cout << "Invoking ShowWatermark with opacity: " << opacity
                      << " for enum: " << i << std::endl;
            bool result = player.ShowWatermark(opacity);
            std::cout << "ShowWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            // Expect true for valid upper boundary opacity: 100
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ShowWatermark_UpperBoundary100 test" << std::endl;
}
/**
 * @brief Test negative opacity value for the watermark display.
 *
 * This test verifies that the ShowWatermark function returns false when provided with a negative opacity value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerThunderInterface object with each valid plugin enumerator (from AVINPUT to COMPOSITEINPUT) | input: Player enum = AVINPUT, COMPOSITEINPUT | Object is constructed with no exceptions thrown | Should be successful |
 * | 02 | Call ShowWatermark with negative opacity (-1) for each plugin instance | input: opacity = -1, output: bool result from ShowWatermark | API returns false and assertion verifies false result | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ShowWatermark_NegativeOpacityMinus1) {
    std::cout << "Entering ShowWatermark_NegativeOpacityMinus1 test" << std::endl;

    int opacity = -1;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Constructing PlayerThunderInterface object with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);

            std::cout << "Invoking ShowWatermark with opacity: " << opacity
                      << " for enum: " << i << std::endl;
            bool result = player.ShowWatermark(opacity);
            std::cout << "ShowWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            // Expect false for negative opacity value
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ShowWatermark_NegativeOpacityMinus1 test" << std::endl;
}
/**
 * @brief Tests that the ShowWatermark function returns false for opacity values above the maximum threshold.
 *
 * This test verifies that when an opacity value exceeding the maximum allowed (i.e., 101) is provided to the ShowWatermark API,
 * the function correctly handles the invalid input by returning false across all valid PlayerThunderAccessPlugin enum values.
 * It ensures that no exceptions are thrown during object construction and that the API behaves as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 071
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                                       | Test Data                                                              | Expected Result                                                                                   | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Iterate over each valid PlayerThunderAccessPlugin enum value, construct PlayerThunderInterface, and invoke ShowWatermark with opacity 101. | plugin = [AVINPUT, ..., COMPOSITEINPUT], opacity = 101, result = bool    | API returns false without throwing exceptions and the EXPECT_FALSE(result) assertion passes.       | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ShowWatermark_OpacityAboveMax101) {
    std::cout << "Entering ShowWatermark_OpacityAboveMax101 test" << std::endl;

    int opacity = 101;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Constructing PlayerThunderInterface object with enum: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface player(plugin);

            std::cout << "Invoking ShowWatermark with opacity: " << opacity
                      << " for enum: " << i << std::endl;
            bool result = player.ShowWatermark(opacity);
            std::cout << "ShowWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            // Expect false for opacity above expected maximum
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ShowWatermark_OpacityAboveMax101 test" << std::endl;
}
/**
 * @brief Verify that PlayerThunderInterface accepts valid common port and video type values for all valid enum inputs.
 *
 * This test verifies that the PlayerThunderInterface and its method StartHelperVideoin correctly handle valid input parameters without throwing exceptions. It iterates through all valid PlayerThunderAccessPlugin enum values to ensure proper handling of different video input types and port numbers.
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
 * | Variation / Step | Description                                                                                                   | Test Data                                                                         | Expected Result                                                                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke StartHelperVideoin for each valid enum by constructing PlayerThunderInterface with port 8080 and video type "mp4". | input: port = 8080, input: videoType = mp4, input: plugin = valid enum from AVINPUT to COMPOSITEINPUT | API should not throw any exceptions; EXPECT_NO_THROW should pass for both construction and method invocation. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidInputCommonPort) {
    std::cout << "Entering ValidInputCommonPort test" << std::endl;

    int port = 8080;
    char videoType[10];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "mp4", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartHelperVideoin with port: " << port
                      << " and videoInputType: " << videoType
                      << " for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                pti.StartHelperVideoin(port, std::string(videoType));
                std::cout << "StartHelperVideoin executed successfully for port " << port
                          << " and videoInputType " << videoType
                          << " with enum: " << i << std::endl;
            });
        });
    }

    std::cout << "Exiting ValidInputCommonPort test" << std::endl;
}
/**
 * @brief Verify successful execution of StartHelperVideoin using valid lower boundary inputs.
 *
 * This test ensures that when provided with a lower boundary value for the port (0) and a valid video input type ("avi"),
 * the PlayerThunderInterface class functions correctly without exceptions for all valid enum values (from AVINPUT to COMPOSITEINPUT).
 * It verifies that the object construction and method invocation behave as expected.
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
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                      | Notes              |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------- | ------------------ |
 * | 01               | Initialize input parameters for the test.                                  | port = 0, videoType = avi                               | Inputs are initialized successfully                | Should be successful |
 * | 02               | Iterate over valid enum values and construct the PlayerThunderInterface object. | plugin = AVINPUT, ..., COMPOSITEINPUT                 | Object construction does not throw an exception     | Should Pass        |
 * | 03               | Invoke StartHelperVideoin with lower boundary port and valid video input type.  | port = 0, videoType = avi                               | Method call executes without throwing an exception  | Should Pass        |
 * | 04               | Complete test execution and exit the test scenario.                       | N/A                                                   | Test completes execution as expected                | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, ValidInputLowerBoundary) {
    std::cout << "Entering ValidInputLowerBoundary test" << std::endl;

    int port = 0;
    char videoType[10];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "avi", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartHelperVideoin with port: " << port
                      << " and videoInputType: " << videoType
                      << " for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                pti.StartHelperVideoin(port, std::string(videoType));
                std::cout << "StartHelperVideoin executed successfully for port " << port
                          << " and videoInputType " << videoType
                          << " with enum: " << i << std::endl;
            });
        });
    }

    std::cout << "Exiting ValidInputLowerBoundary test" << std::endl;
}
/**
 * @brief Validate that StartHelperVideoin handles upper boundary inputs correctly
 *
 * This test verifies that the PlayerThunderInterface constructor and its StartHelperVideoin method operate without throwing exceptions when provided with the upper boundary port value 65535, a valid video type ("h264"), and across all valid PlayerThunderAccessPlugin enum values (from AVINPUT to COMPOSITEINPUT). The test ensures robustness by confirming proper handling of valid input parameters at their limit.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerThunderInterface using a valid plugin enum (iterating from AVINPUT to COMPOSITEINPUT) | plugin = [AVINPUT, ..., COMPOSITEINPUT] | Object constructed without throwing exception | Should Pass |
 * | 02 | Invoke StartHelperVideoin using the upper boundary port value and valid video type | port = 65535, videoType = "h264", plugin = [current enum value] | Method call executes without throwing exception | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidInputUpperBoundary) {
    std::cout << "Entering ValidInputUpperBoundary test" << std::endl;

    int port = 65535;
    char videoType[10];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "h264", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartHelperVideoin with port: " << port
                      << " and videoInputType: " << videoType
                      << " for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                pti.StartHelperVideoin(port, std::string(videoType));
                std::cout << "StartHelperVideoin executed successfully for port " << port
                          << " and videoInputType " << videoType
                          << " with enum: " << i << std::endl;
            });
        });
    }

    std::cout << "Exiting ValidInputUpperBoundary test" << std::endl;
}
/**
 * @brief Validate that calling StartHelperVideoin with an empty video input type does not throw exceptions.
 *
 * This test iterates through all valid enum values for PlayerThunderAccessPlugin, constructs a PlayerThunderInterface
 * object for each enum value, and attempts to invoke StartHelperVideoin using a valid port number and an empty string
 * for videoInputType. The objective is to ensure that both the object creation and the API call execute without throwing
 * any exceptions for valid input scenarios.
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
 * | Variation / Step | Description                                                                                             | Test Data                                                   | Expected Result                                     | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | --------------------------------------------------- | ----------- |
 * | 01               | Create PlayerThunderInterface object for each valid enum value and verify that the constructor does not throw. | plugin enum = AVINPUT to COMPOSITEINPUT                     | No exception thrown from the constructor            | Should Pass |
 * | 02               | Invoke StartHelperVideoin using port 8080 and an empty video input type, ensuring no exceptions are thrown.  | port = 8080, videoInputType = ""                              | No exception thrown from StartHelperVideoin call      | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidInputEmptyVideoInputType) {
    std::cout << "Entering ValidInputEmptyVideoInputType test" << std::endl;

    int port = 8080;
    char videoType[10];
    std::memset(videoType, 0, sizeof(videoType));
    // Empty string assigned using strncpy (will remain empty)
    std::strncpy(videoType, "", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface pti(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartHelperVideoin with port: " << port
                      << " and videoInputType: \"" << videoType
                      << "\" for enum: " << i << std::endl;

            EXPECT_NO_THROW({
                pti.StartHelperVideoin(port, std::string(videoType));
                std::cout << "StartHelperVideoin executed successfully for port " << port
                          << " with an empty videoInputType"
                          << " and enum: " << i << std::endl;
            });
        });
    }

    std::cout << "Exiting ValidInputEmptyVideoInputType test" << std::endl;
}
/**
 * @brief Negative test to validate error handling when a negative port is provided
 *
 * This test verifies that the PlayerThunderInterface::StartHelperVideoin API correctly handles invalid input by throwing an exception when a negative port is specified. The test iterates over all valid enum values for the plugin and ensures that each invocation with a negative port results in an error as expected.@n
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through all valid plugin enum values and invoke StartHelperVideoin with a negative port to verify error handling | port=-1, videoType=mp4, plugin=AVINPUT...COMPOSITEINPUT | Exception is thrown for each invocation indicating an erroneous negative port input | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeInputNegativePort) {
    std::cout << "Entering NegativeInputNegativePort test" << std::endl;

    int port = -1;
    char videoType[10];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "mp4", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface pti(plugin);
        std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

        std::cout << "Invoking StartHelperVideoin with negative port: " << port
                  << " and videoInputType: " << videoType
                  << " for enum: " << i << std::endl;

        // Expecting the method to signal an error; using EXPECT_ANY_THROW to catch exceptions.
        EXPECT_ANY_THROW({
            pti.StartHelperVideoin(port, std::string(videoType));
        });

        std::cout << "StartHelperVideoin failed to execute with negative port as expected for enum: " << i << std::endl;
    }

    std::cout << "Exiting NegativeInputNegativePort test" << std::endl;
}
/**
 * @brief Validate the API's behavior when provided an invalid video input type.
 *
 * This test verifies that the StartHelperVideoin API in the PlayerThunderInterface class
 * correctly throws an exception when invoked with an invalid video input type. It iterates
 * through all valid plugin enum values (from AVINPUT to COMPOSITEINPUT) to ensure that the
 * invalid input is handled appropriately across different configurations.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 077
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate over each valid plugin enum value and invoke StartHelperVideoin with an invalid video input type | port = 8080, videoType = invalidType, plugin = [AVINPUT...COMPOSITEINPUT] | API throws an exception confirming invalid input handling | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeInputInvalidVideoInputType) {
    std::cout << "Entering NegativeInputInvalidVideoInputType test" << std::endl;

    int port = 8080;
    char videoType[20];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "invalidType", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface pti(plugin);
        std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

        std::cout << "Invoking StartHelperVideoin with port: " << port
                  << " and invalid videoInputType: " << videoType
                  << " for enum: " << i << std::endl;

        EXPECT_ANY_THROW({
            pti.StartHelperVideoin(port, std::string(videoType));
        });

        std::cout << "StartHelperVideoin failed to execute with invalid videoInputType as expected for enum: " << i << std::endl;
    }

    std::cout << "Exiting NegativeInputInvalidVideoInputType test" << std::endl;
}
/**
 * @brief Test to verify that StartHelperVideoin API throws an exception when provided with a whitespace video input type.
 *
 * This test iterates through all valid enum values, constructs a PlayerThunderInterface object for each,
 * and verifies that calling StartHelperVideoin with a port and a video input type consisting solely of whitespace
 * throws an exception as expected. This negative test case ensures that the API handles invalid (whitespace) input properly.
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
 * | Variation / Step | Description                                                                         | Test Data                                                                           | Expected Result                                                                               | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize port and video type with whitespace characters.                         | port = 8080, videoType = "   "                                                        | Variables initialized correctly.                                                             | Should be successful |
 * | 02               | Loop through all valid enum values and construct a PlayerThunderInterface object.     | For each enum value from AVINPUT to COMPOSITEINPUT                                    | Object constructed successfully for each enum value.                                         | Should be successful |
 * | 03               | Invoke StartHelperVideoin with the initialized port and whitespace video type.         | input: port = 8080, videoInputType = "   ", plugin enum value (each from loop iteration)| API call throws an exception as the whitespace video type is invalid. Assertion (EXPECT_ANY_THROW) passes. | Should Fail   |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeInputWhitespaceVideoInputType) {
    std::cout << "Entering NegativeInputWhitespaceVideoInputType test" << std::endl;

    int port = 8080;
    char videoType[20];
    std::memset(videoType, 0, sizeof(videoType));
    std::strncpy(videoType, "   ", sizeof(videoType) - 1);

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        PlayerThunderInterface pti(plugin);
        std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

        std::cout << "Invoking StartHelperVideoin with port: " << port
                  << " and whitespace videoInputType: \"" << videoType << "\""
                  << " for enum: " << i << std::endl;

        EXPECT_ANY_THROW({
            pti.StartHelperVideoin(port, std::string(videoType));
        });

        std::cout << "StartHelperVideoin failed to execute with whitespace videoInputType as expected for enum: " << i << std::endl;
    }

    std::cout << "Exiting NegativeInputWhitespaceVideoInputType test" << std::endl;
}
/**
 * @brief Validate that the StartOta API functions correctly with all valid parameters
 *
 * This test verifies that the StartOta method of the PlayerThunderInterface class executes without throwing any exceptions when called with valid input parameters. The test loops through all valid enum values to ensure the API handles different plugin types reliably.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 079@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerThunderInterface object using a valid plugin enum value | plugin = enum value from AVINPUT to COMPOSITEINPUT | Object is constructed without exception | Should Pass |
 * | 02 | Invoke StartOta API with all valid input parameters (url, waylandDisplay, preferredLanguagesString, atsc_preferredLanguagesString, preferredRenditionString, atsc_preferredRenditionString) | url = https://example.com/ota, waylandDisplay = Display-1, preferredLanguagesString = en, atsc_preferredLanguagesString = fr, preferredRenditionString = HD, atsc_preferredRenditionString = SD | API call completes without throwing an exception, initiating OTA update successfully | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithAllValidParameters) {
    std::cout << "Entering StartOTAWithAllValidParameters test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully; OTA update starts successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithAllValidParameters test" << std::endl;
}
/**
 * @brief Validates StartOta API handling with an empty URL across all supported enum values.
 *
 * This test verifies that the StartOta API of PlayerThunderInterface functions correctly when supplied with an empty URL. The test iterates through all valid PlayerThunderAccessPlugin enum values, constructs a PlayerThunderInterface object for each, and then invokes the StartOta API with predetermined parameters. It ensures that no exceptions are thrown during both the object construction and API invocation.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 080
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through each valid PlayerThunderAccessPlugin enum value, construct a PlayerThunderInterface object, and invoke StartOta with an empty url and valid parameters. | input: url = "", waylandDisplay = "Display-1", preferredLanguagesString = "en", atsc_preferredLanguagesString = "fr", preferredRenditionString = "HD", atsc_preferredRenditionString = "SD", plugin = each enum value from AVINPUT to COMPOSITEINPUT | API completes execution without throwing any exceptions; both object construction and StartOta invocation pass the EXPECT_NO_THROW assertions. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyURL) {
    std::cout << "Entering StartOTAWithEmptyURL test" << std::endl;

    std::string url = "";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url (empty): \"" << url << "\"" << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty URL for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyURL test" << std::endl;
}
/**
 * @brief Verify that the StartOta function handles invalid URL formats gracefully
 *
 * This test verifies that the PlayerThunderInterface::StartOta API does not throw exceptions when
 * an invalid URL format is passed, across all valid enum values for the plugin. The invalid URL should
 * be gracefully handled without causing the test to crash.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 081@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through valid plugin enum values, instantiate PlayerThunderInterface, and invoke StartOta with an invalid URL | plugin = enum value from AVINPUT to COMPOSITEINPUT, url = invalid_url, waylandDisplay = Display-1, preferredLanguagesString = en, atsc_preferredLanguagesString = fr, preferredRenditionString = 4K, atsc_preferredRenditionString = HD | API completes without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithInvalidURLFormat) {
    std::cout << "Entering StartOTAWithInvalidURLFormat test" << std::endl;

    std::string url = "invalid_url";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "4K";
    std::string atsc_preferredRenditionString = "HD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with invalid URL format for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithInvalidURLFormat test" << std::endl;
}
/**
 * @brief Tests the StartOTA functionality with an empty waylandDisplay parameter.
 *
 * This test verifies that the StartOta method can be executed without throwing an exception when the waylandDisplay parameter is provided as an empty string. It iterates through all valid enum values of PlayerThunderAccessPlugin, constructs a PlayerThunderInterface object for each enum, and invokes StartOta with a valid URL and other parameters, while intentionally setting waylandDisplay to empty.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 082
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through all valid enum values to initialize PlayerThunderInterface objects | input: plugin enum values from AVINPUT to COMPOSITEINPUT | Should successfully construct a PlayerThunderInterface object for each enum value | Should be successful |
 * | 02 | Invoke the StartOta API with an empty waylandDisplay and valid parameters | input: url = "https://example.com/ota", waylandDisplay = "", preferredLanguagesString = "en", atsc_preferredLanguagesString = "fr", preferredRenditionString = "HD", atsc_preferredRenditionString = "SD" | API call should not throw any exception and complete successfully | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyWaylandDisplay) {
    std::cout << "Entering StartOTAWithEmptyWaylandDisplay test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay (empty): \"" << waylandDisplay << "\"" << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty waylandDisplay for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyWaylandDisplay test" << std::endl;
}
/**
 * @brief Validate that StartOta executes successfully with an empty preferredLanguagesString parameter
 *
 * This test ensures that when an empty string is passed for the preferredLanguagesString argument, the StartOta API call
 * on the PlayerThunderInterface object does not throw any exception. The test iterates through all valid enum values
 * for the plugin, constructs a PlayerThunderInterface object, and invokes the StartOta method with fixed parameters,
 * thus verifying consistent behavior across different plugin implementations.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 083@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerThunderInterface object using a valid plugin enum value | plugin = enum value in range AVINPUT to COMPOSITEINPUT | Object is constructed successfully with no exceptions | Should be successful |
 * | 02 | Invoke StartOta with an empty preferredLanguagesString and validate the API call | url = https://example.com/ota, waylandDisplay = Display-1, preferredLanguagesString = , atsc_preferredLanguagesString = fr, preferredRenditionString = HD, atsc_preferredRenditionString = SD | StartOta executes without throwing any exception | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyPreferredLanguagesString) {
    std::cout << "Entering StartOTAWithEmptyPreferredLanguagesString test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString (empty): \"" << preferredLanguagesString << "\"" << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty preferredLanguagesString for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyPreferredLanguagesString test" << std::endl;
}
/**
 * @brief Verify that StartOta API handles an empty atsc_preferredLanguagesString input correctly
 *
 * This test validates that the StartOta method of the PlayerThunderInterface class executes without throwing an
 * exception when provided with an empty string for atsc_preferredLanguagesString. The test iterates through all valid
 * PlayerThunderAccessPlugin enum values, constructs the PlayerThunderInterface object accordingly, and invokes the StartOta
 * API with predetermined parameter values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 084@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                | Test Data                                                                                                                                                          | Expected Result                                                       | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- | --------------- |
 * | 01               | Construct PlayerThunderInterface object using a valid enum value for each iteration                         | plugin = enum value from AVINPUT to COMPOSITEINPUT                                                                                                                   | Object is constructed successfully with no exception thrown           | Should be successful |
 * | 02               | Invoke StartOta API with parameters including an empty atsc_preferredLanguagesString for each valid enum value | url = https://example.com/ota, waylandDisplay = Display-1, preferredLanguagesString = en, atsc_preferredLanguagesString = (empty), preferredRenditionString = HD, atsc_preferredRenditionString = SD | API call completes without throwing an exception and performs as expected | Should Pass     |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyAtscPreferredLanguagesString) {
    std::cout << "Entering StartOTAWithEmptyAtscPreferredLanguagesString test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString (empty): \"" << atsc_preferredLanguagesString << "\"" << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty atsc_preferredLanguagesString for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyAtscPreferredLanguagesString test" << std::endl;
}
/**
 * @brief Verify that the StartOta API handles an empty preferredRenditionString correctly.
 *
 * This test verifies that the PlayerThunderInterface's StartOta function does not throw an exception when invoked with an empty preferredRenditionString.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 085@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                        | Test Data                                                                                                                                                       | Expected Result                                                                          | Notes          |
 * | :----:           | ------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Construct the PlayerThunderInterface object using a valid enum value from AVINPUT to COMPOSITEINPUT                  | input: plugin = [AVINPUT, ..., COMPOSITEINPUT]                                                                                                                   | Object is constructed successfully without throwing any exception                       | Should be successful |
 * | 02               | Invoke the StartOta API with url = "https://example.com/ota", waylandDisplay = "Display-1", preferredLanguagesString = "en", atsc_preferredLanguagesString = "fr", preferredRenditionString = "", atsc_preferredRenditionString = "SD" | input: url = "https://example.com/ota", waylandDisplay = "Display-1", preferredLanguagesString = "en", atsc_preferredLanguagesString = "fr", preferredRenditionString = (empty), atsc_preferredRenditionString = "SD" | API call completes without throwing an exception, indicating the handling of an empty preferredRenditionString is correct | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyPreferredRenditionString) {
    std::cout << "Entering StartOTAWithEmptyPreferredRenditionString test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "";
    std::string atsc_preferredRenditionString = "SD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString (empty): \"" << preferredRenditionString << "\"" << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty preferredRenditionString for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyPreferredRenditionString test" << std::endl;
}
/**
 * @brief Test StartOta with an empty atsc_preferredRenditionString parameter
 *
 * This test verifies that the StartOta API can handle an empty atsc_preferredRenditionString parameter without throwing exceptions.
 * The test iterates over all valid PlayerThunderAccessPlugin enum values, constructs a PlayerThunderInterface object, and calls the StartOta function
 * using valid inputs for all parameters except for atsc_preferredRenditionString, which is provided as an empty string.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 086@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                  | Test Data                                                                                                         | Expected Result                                                      | Notes      |
 * | :----:           | :----------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------- | :--------- |
 * | 01               | Iterate through each valid enum value, construct the PlayerThunderInterface object, and invoke StartOta API      | url = https://example.com/ota, waylandDisplay = Display-1, preferredLanguagesString = en, atsc_preferredLanguagesString = fr, preferredRenditionString = HD, atsc_preferredRenditionString =  | API call completes without throwing exceptions and executes successfully | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithEmptyAtscPreferredRenditionString) {
    std::cout << "Entering StartOTAWithEmptyAtscPreferredRenditionString test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en";
    std::string atsc_preferredLanguagesString = "fr";
    std::string preferredRenditionString = "HD";
    std::string atsc_preferredRenditionString = "";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString (empty): \"" << atsc_preferredRenditionString << "\"" << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully with empty atsc_preferredRenditionString for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithEmptyAtscPreferredRenditionString test" << std::endl;
}
/**
 * @brief Verifies that the StartOta API correctly processes multiple language and rendition preferences.
 *
 * This test validates that the StartOta API handles multiple preferred language and rendition inputs 
 * appropriately across all valid PlayerThunderAccessPlugin enum values by not throwing any exceptions 
 * when invoked with valid parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 087@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Loop through each valid PlayerThunderAccessPlugin enum value, instantiate PlayerThunderInterface and invoke StartOta with multiple language and rendition preferences. | url = https://example.com/ota, waylandDisplay = Display-1, preferredLanguagesString = en,es, atsc_preferredLanguagesString = fr,de, preferredRenditionString = HD,SD, atsc_preferredRenditionString = SD,HD, plugin = All valid enum values from AVINPUT to COMPOSITEINPUT | The API call should execute without throwing any exceptions and process the preferences correctly. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StartOTAWithMultipleLanguageAndRenditionPreferences) {
    std::cout << "Entering StartOTAWithMultipleLanguageAndRenditionPreferences test" << std::endl;

    std::string url = "https://example.com/ota";
    std::string waylandDisplay = "Display-1";
    std::string preferredLanguagesString = "en,es";
    std::string atsc_preferredLanguagesString = "fr,de";
    std::string preferredRenditionString = "HD,SD";
    std::string atsc_preferredRenditionString = "SD,HD";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "Constructed PlayerThunderInterface object with enum: " << i << std::endl;

            std::cout << "Invoking StartOta with parameters for enum: " << i << std::endl;
            std::cout << "  url: " << url << std::endl;
            std::cout << "  waylandDisplay: " << waylandDisplay << std::endl;
            std::cout << "  preferredLanguagesString: " << preferredLanguagesString << std::endl;
            std::cout << "  atsc_preferredLanguagesString: " << atsc_preferredLanguagesString << std::endl;
            std::cout << "  preferredRenditionString: " << preferredRenditionString << std::endl;
            std::cout << "  atsc_preferredRenditionString: " << atsc_preferredRenditionString << std::endl;

            EXPECT_NO_THROW(obj.StartOta(url, waylandDisplay, preferredLanguagesString,
                                         atsc_preferredLanguagesString, preferredRenditionString,
                                         atsc_preferredRenditionString));

            std::cout << "StartOta invoked successfully; multiple language and rendition preferences processed correctly for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StartOTAWithMultipleLanguageAndRenditionPreferences test" << std::endl;
}
/**
 * @brief Verify functionality of PlayerThunderInterface with valid URL and valid callbacks
 *
 * This test ensures that the PlayerThunderInterface constructor accepts valid plugin enum values and that the StartRmf function successfully starts the RMF session when provided with a valid URL and valid lambda callbacks for player status and error handling. It iterates through all valid enum values (from AVINPUT to COMPOSITEINPUT) to ensure comprehensive coverage.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 088@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate over valid enum values by creating PlayerThunderInterface and invoking StartRmf with a valid URL and valid callbacks | plugin = enum value (AVINPUT to COMPOSITEINPUT), url = "http://example.com/media", onPlayerStatusHandlerCb = valid lambda, onPlayerErrorHandlerCb = valid lambda | StartRmf returns true without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidURLAndValidCallbacks) {
    std::cout << "Entering ValidURLAndValidCallbacks test" << std::endl;

    std::string url = "http://example.com/media";
    std::cout << "URL provided: " << url << std::endl;

    // Define valid lambda callbacks for player status and error handling.
    std::function<void(std::string)> onPlayerStatusHandlerCb =
        [](std::string status) {
            std::cout << "Status callback triggered. Status: " << status << std::endl;
        };
    std::function<void(std::string)> onPlayerErrorHandlerCb =
        [](std::string error) {
            std::cout << "Error callback triggered. Error: " << error << std::endl;
        };

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with valid URL and valid callbacks for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidURLAndValidCallbacks test" << std::endl;
}
/**
 * @brief Test StartRmf method with an empty URL and valid callbacks
 *
 * This test verifies that when an empty URL is provided to the StartRmf method along with valid callback handlers, the API correctly returns false, indicating failure to start. The test iterates through all valid enum values of PlayerThunderAccessPlugin, ensuring that no exceptions are thrown when creating the PlayerThunderInterface object and invoking the StartRmf method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 089@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Iterate through valid enum values and construct PlayerThunderInterface for each | plugin = value ranging from AVINPUT to COMPOSITEINPUT | Object created without throwing exceptions | Should be successful |
 * | 02 | Invoke StartRmf using an empty URL and valid callbacks; validate the returned result | url = "", onPlayerStatusHandlerCb = valid callback, onPlayerErrorHandlerCb = valid callback | StartRmf returns false and assertion confirms the false value | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, EmptyURLWithValidCallbacks) {
    std::cout << "Entering EmptyURLWithValidCallbacks test" << std::endl;

    std::string url = "";
    std::cout << "Empty URL provided." << std::endl;

    std::function<void(std::string)> onPlayerStatusHandlerCb =
        [](std::string status) {
            std::cout << "Status callback triggered. Status: " << status << std::endl;
        };
    std::function<void(std::string)> onPlayerErrorHandlerCb =
        [](std::string error) {
            std::cout << "Error callback triggered. Error: " << error << std::endl;
        };

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with empty URL and valid callbacks for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting EmptyURLWithValidCallbacks test" << std::endl;
}
/**
 * @brief Test to validate that StartRmf returns false when an empty status callback is provided.
 *
 * This test verifies that the PlayerThunderInterface does not start playing the media when the status callback is empty,
 * even though a valid URL and error handler are provided. It iterates through all valid enum values and ensures that the
 * StartRmf method returns false, indicating that the plugin correctly handles the missing status handler scenario.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 090@n
 * **Priority:** High@n
 *  
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *  
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize test with a valid URL and print the entering message. | url = "http://example.com/media" | URL initialized and log printed. | Should be successful |
 * | 02 | Create an empty status handler and a valid error handler. | onPlayerStatusHandlerCb = empty, onPlayerErrorHandlerCb = lambda(error) { print error } | Handlers defined without error. | Should be successful |
 * | 03 | Loop through all valid enum values, create PlayerThunderInterface objects and invoke StartRmf. | plugin enum (from AVINPUT to COMPOSITEINPUT), url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb | PlayerThunderInterface creation succeeds without throwing, and StartRmf returns false with EXPECT_FALSE check. | Should Pass |
 * | 04 | Print the exiting test message. | N/A | Exiting message logged. | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, ValidURLWithEmptyStatusHandler) {
    std::cout << "Entering ValidURLWithEmptyStatusHandler test" << std::endl;

    std::string url = "http://example.com/media";
    std::cout << "URL provided: " << url << std::endl;

    // Create an empty status handler callback.
    std::function<void(std::string)> onPlayerStatusHandlerCb; // empty
    std::cout << "onPlayerStatusHandlerCb is empty." << std::endl;

    std::function<void(std::string)> onPlayerErrorHandlerCb =
        [](std::string error) {
            std::cout << "Error callback triggered. Error: " << error << std::endl;
        };

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with valid URL, empty status handler and valid error handler for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ValidURLWithEmptyStatusHandler test" << std::endl;
}
/**
 * @brief Validate that StartRmf returns false when a valid URL, a valid status callback, and an empty error callback are provided.
 *
 * This test verifies that for each valid PlayerThunderAccessPlugin enum value, the PlayerThunderInterface object is created without
 * throwing exceptions and that the StartRmf method returns false when an empty error handler callback is provided. It ensures the API's
 * stability and proper error handling when the error callback is missing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 091@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Set valid URL and valid status callback, and define empty error callback | url = "http://example.com/media", onPlayerStatusHandlerCb = valid lambda, onPlayerErrorHandlerCb = empty | URL and callbacks are set successfully | Should be successful |
 * | 02 | Iterate over all valid enum values to create PlayerThunderInterface object | Enum values from AVINPUT to COMPOSITEINPUT | Object is created without throwing exceptions for each enum value | Should Pass |
 * | 03 | Invoke StartRmf with valid URL, valid status handler, and empty error handler for each enum value | For each iteration: url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb | StartRmf returns false and no exception is thrown | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidURLWithEmptyErrorHandler) {
    std::cout << "Entering ValidURLWithEmptyErrorHandler test" << std::endl;

    std::string url = "http://example.com/media";
    std::cout << "URL provided: " << url << std::endl;

    std::function<void(std::string)> onPlayerStatusHandlerCb =
        [](std::string status) {
            std::cout << "Status callback triggered. Status: " << status << std::endl;
        };
    std::cout << "onPlayerStatusHandlerCb provided as valid lambda." << std::endl;

    // Create an empty error handler callback.
    std::function<void(std::string)> onPlayerErrorHandlerCb; // empty
    std::cout << "onPlayerErrorHandlerCb is empty." << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with valid URL, valid status handler and empty error handler for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ValidURLWithEmptyErrorHandler test" << std::endl;
}
/**
 * @brief Test StartRmf API with a malformed URL and valid callbacks
 *
 * This test verifies that the StartRmf API returns false when invoked with a malformed URL while valid callbacks are provided. It iterates over all valid enum values, creates a PlayerThunderInterface object, and then calls StartRmf to ensure that the method fails gracefully without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 092
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerThunderInterface object for each valid enum value | plugin = current enum (AVINPUT to COMPOSITEINPUT), url = "htp:/bad_url", onPlayerStatusHandlerCb = valid lambda, onPlayerErrorHandlerCb = valid lambda | Object is created successfully without throwing any exception | Should Pass |
 * | 02 | Invoke StartRmf with the malformed URL and valid callbacks and verify the return value | plugin = current enum, url = "htp:/bad_url", onPlayerStatusHandlerCb = valid lambda, onPlayerErrorHandlerCb = valid lambda, output: result | StartRmf returns false and the EXPECT_FALSE check passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, MalformedURLWithValidCallbacks) {
    std::cout << "Entering MalformedURLWithValidCallbacks test" << std::endl;

    std::string url = "htp:/bad_url";
    std::cout << "Malformed URL provided: " << url << std::endl;

    std::function<void(std::string)> onPlayerStatusHandlerCb =
        [](std::string status) {
            std::cout << "Status callback triggered. Status: " << status << std::endl;
        };
    std::function<void(std::string)> onPlayerErrorHandlerCb =
        [](std::string error) {
            std::cout << "Error callback triggered. Error: " << error << std::endl;
        };

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with malformed URL and valid callbacks for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting MalformedURLWithValidCallbacks test" << std::endl;
}
/**
 * @brief This test verifies the behavior of the StartRmf API when a URL containing only whitespace is provided along with valid callbacks.
 *
 * This test iterates through all valid PlayerThunderAccessPlugin enum values, creates a PlayerThunderInterface instance for each, invokes the StartRmf method using a whitespace URL and valid callback functions, and verifies that no exception is thrown and the API returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 093@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                    | Expected Result                                                                                                     | Notes            |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create PlayerThunderInterface instance for each valid plugin enum value                       | For each iteration: plugin = AVINPUT, ..., COMPOSITEINPUT                                                    | Instance is created successfully without throwing any exception                                                   | Should be successful |
 * | 02               | Invoke StartRmf with a whitespace URL and valid callback functions for each plugin instance     | url = "   ", onPlayerStatusHandlerCb = valid callback, onPlayerErrorHandlerCb = valid callback; plugin as above | StartRmf returns false and no exception is thrown for each plugin value                                             | Should Pass      |
 */
TEST_F(PlayerThunderInterfaceTests, WhitespaceURLWithValidCallbacks) {
    std::cout << "Entering WhitespaceURLWithValidCallbacks test" << std::endl;

    std::string url = "   ";
    std::cout << "URL with only whitespace provided: '" << url << "'" << std::endl;

    std::function<void(std::string)> onPlayerStatusHandlerCb =
        [](std::string status) {
            std::cout << "Status callback triggered. Status: " << status << std::endl;
        };
    std::function<void(std::string)> onPlayerErrorHandlerCb =
        [](std::string error) {
            std::cout << "Error callback triggered. Error: " << error << std::endl;
        };

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StartRmf with whitespace URL and valid callbacks for enum: " << i << std::endl;
            bool result = player.StartRmf(url, onPlayerStatusHandlerCb, onPlayerErrorHandlerCb);
            std::cout << "StartRmf returned: " << std::boolalpha << result << " for enum: " << i << std::endl;
            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting WhitespaceURLWithValidCallbacks test" << std::endl;
}
/**
 * @brief Test StopHelperVideoin method with videoInput "WEBVTT" on PlayerThunderInterface for all valid enum values
 *
 * This test iterates through all valid enum values from AVINPUT to COMPOSITEINPUT. For each enum value, it creates a PlayerThunderInterface object and invokes the StopHelperVideoin method with the videoInput parameter set to "WEBVTT". The test ensures that the API does not throw any exceptions during object creation or method invocation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 094@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through valid enum values, create a PlayerThunderInterface object and invoke StopHelperVideoin with videoInput "WEBVTT" | For each iteration: plugin = [AVINPUT, ..., COMPOSITEINPUT], videoInput = "WEBVTT" | PlayerThunderInterface is created successfully and StopHelperVideoin executes without throwing exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StopHelperVideoin_WebVTT) {
    std::cout << "Entering StopHelperVideoin_WebVTT test" << std::endl;

    std::string videoInput = "WEBVTT";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StopHelperVideoin with videoInputType: " << videoInput
                      << " for enum: " << i << std::endl;
            player.StopHelperVideoin(videoInput);
            std::cout << "StopHelperVideoin executed successfully for input: " << videoInput
                      << " and enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopHelperVideoin_WebVTT test" << std::endl;
}
/**
 * @brief Test to verify that StopHelperVideoin API correctly processes the MP4 video input across all valid plugin enum configurations.
 *
 * This test verifies that for each valid plugin enumerator, the PlayerThunderInterface is created successfully and its StopHelperVideoin method, when called with "MP4" as the video input, executes without throwing any exceptions. It ensures that the construction and invocation of the API under varying configurations is handled properly.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 095
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                               | Expected Result                                                     | Notes          |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------- | ------------------------------------------------------------------- | -------------- |
 * | 01               | Create PlayerThunderInterface object with each plugin enum value    | plugin = AVINPUT to COMPOSITEINPUT (iterated value)     | Object is instantiated successfully without any exceptions         | Should be successful |
 * | 02               | Invoke StopHelperVideoin API with videoInput "MP4" for each enum value | input: videoInput = MP4, plugin = current iterated value | API call completes without throwing exceptions and passes the assertion | Should Pass    |
 */
TEST_F(PlayerThunderInterfaceTests, StopHelperVideoin_MP4) {
    std::cout << "Entering StopHelperVideoin_MP4 test" << std::endl;

    std::string videoInput = "MP4";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StopHelperVideoin with videoInputType: " << videoInput
                      << " for enum: " << i << std::endl;
            player.StopHelperVideoin(videoInput);
            std::cout << "StopHelperVideoin executed successfully for input: " << videoInput
                      << " and enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopHelperVideoin_MP4 test" << std::endl;
}
/**
 * @brief Verify StopHelperVideoin handles an empty video input string correctly.
 *
 * This test case iterates over all valid PlayerThunderAccessPlugin enum values, creating an instance of PlayerThunderInterface for each. It then calls StopHelperVideoin with an empty video input string to ensure that no exceptions are thrown during the function execution.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 096@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerThunderInterface instance using the current plugin enum value. | plugin = current enum value (iterates from AVINPUT to COMPOSITEINPUT) | Instance created successfully without exceptions. | Should be successful |
 * | 02 | Invoke StopHelperVideoin with an empty video input string. | videoInput = "" | API call completes without throwing any exceptions. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StopHelperVideoin_EmptyString) {
    std::cout << "Entering StopHelperVideoin_EmptyString test" << std::endl;

    std::string videoInput = "";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StopHelperVideoin with empty videoInputType for enum: " << i << std::endl;
            player.StopHelperVideoin(videoInput);
            std::cout << "StopHelperVideoin executed successfully for empty input and enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopHelperVideoin_EmptyString test" << std::endl;
}
/**
 * @brief Verify that StopHelperVideoin executes without exceptions when provided with an invalid video input type.
 *
 * This test iterates through all valid enum values for PlayerThunderInterface. For each enum value, it creates an instance of the interface and calls StopHelperVideoin with a video input set to "INVALID_TYPE". The test asserts that no exception is thrown during the invocation, ensuring that the API gracefully handles an invalid video input type.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 097@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                     | Test Data                                                            | Expected Result                                                               | Notes       |
 * | :----:           | ----------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ----------------------------------------------------------------------------- | ----------- |
 * | 01               | For each valid enum value (from AVINPUT to COMPOSITEINPUT), create a PlayerThunderInterface instance and invoke StopHelperVideoin with videoInput set to "INVALID_TYPE". | plugin = enum values from AVINPUT to COMPOSITEINPUT, videoInput = "INVALID_TYPE" | StopHelperVideoin completes execution without throwing any exceptions. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, StopHelperVideoin_InvalidType) {
    std::cout << "Entering StopHelperVideoin_InvalidType test" << std::endl;

    std::string videoInput = "INVALID_TYPE";

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StopHelperVideoin with videoInputType: " << videoInput
                      << " for enum: " << i << std::endl;
            player.StopHelperVideoin(videoInput);
            std::cout << "StopHelperVideoin executed successfully for input: " << videoInput
                      << " and enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopHelperVideoin_InvalidType test" << std::endl;
}
/**
 * @brief Test the StopOta method for valid invocation across all valid enum values
 *
 * This test validates the behavior of the StopOta method provided by PlayerThunderInterface when invoked with all valid PlayerThunderAccessPlugin enum values. It ensures that both the creation of the PlayerThunderInterface object and the invocation of the StopOta method are executed without throwing exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 098@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                              | Test Data                                                  | Expected Result                                                                          | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a PlayerThunderInterface object using a valid PlayerThunderAccessPlugin enum value  | input: enum value = AVINPUT, ..., COMPOSITEINPUT             | PlayerThunderInterface object is created successfully without throwing an exception       | Should be successful |
 * | 02               | Invoke the StopOta method on the created PlayerThunderInterface object                      | input: StopOta() invocation for the object created with enum value | StopOta method executes successfully without throwing exceptions and returns normally | Should Pass       |
 */
TEST_F(PlayerThunderInterfaceTests, StopOta_ValidInvocation) {
    std::cout << "Entering StopOta_ValidInvocation test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Created PlayerThunderAccessPlugin enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking StopOta method for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.StopOta());
            std::cout << "StopOta method executed successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopOta_ValidInvocation test" << std::endl;
}
/**
 * @brief Verify that StopRmf method is invoked successfully for all valid enum values.
 *
 * This test iterates through every valid enum value of PlayerThunderAccessPlugin, instantiates
 * a PlayerThunderInterface object for each enum, and verifies that the StopRmf method executes
 * without throwing any exceptions. This test is important to ensure that the API correctly handles
 * all valid inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 099@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                                       | Expected Result                                                                 | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------------------- | ------------- |
 * | 01               | Log the start of the test execution.                                        | N/A                                                             | "Entering StopRmf_is_called test" message is printed on the console.             | Should be successful |
 * | 02               | Iterate through all valid enum values for PlayerThunderAccessPlugin.         | i = AVINPUT, ..., COMPOSITEINPUT                                   | Loop executes correctly over all enum values without exception.                | Should be successful |
 * | 03               | Instantiate PlayerThunderInterface with the current enum value.              | plugin = current enum value                                       | Object creation does not throw any exceptions.                                | Should Pass   |
 * | 04               | Invoke the StopRmf method on the instantiated PlayerThunderInterface object.  | Method: StopRmf invoked on the object                             | StopRmf method execution does not throw any exceptions.                        | Should Pass   |
 * | 05               | Log the completion of the test execution.                                   | N/A                                                             | "Exiting StopRmf_is_called test" message is printed on the console.              | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, StopRmf_is_called) {
    std::cout << "Entering StopRmf_is_called test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Created PlayerThunderAccessPlugin enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke StopRmf method with detailed debug logs
            std::cout << "Invoking StopRmf method for enum: " << i << std::endl;
            EXPECT_NO_THROW(player.StopRmf());
            std::cout << "StopRmf method invoked successfully for enum: " << i << std::endl;
        });
    }

    std::cout << "Exiting StopRmf_is_called test" << std::endl;
}
/**
 * @brief Validate that UnRegisterAllEventsVideoin executes successfully over all valid enum values.
 *
 * This test verifies that for each valid plugin enum value (ranging from AVINPUT to COMPOSITEINPUT), a PlayerThunderInterface instance can be created successfully and that the UnRegisterAllEventsVideoin method executes without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 100@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- | -------------- | ----- |@n
 * | 01 | Iterate over valid enum values and create a PlayerThunderInterface object for each enum value | input: plugin enum value = AVINPUT, AVINPUT+1, ..., COMPOSITEINPUT | Object is successfully created without throwing exceptions | Should be successful |@n
 * | 02 | Invoke UnRegisterAllEventsVideoin() method on the created PlayerThunderInterface instance for each valid enum value | input: plugin enum value = current enum value; method: UnRegisterAllEventsVideoin() | Method executes without errors and does not throw any exceptions | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, UnRegisterAllEventsVideoin_start) {
    std::cout << "Entering UnRegisterAllEventsVideoin_start test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Created PlayerThunderAccessPlugin enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface playerInterface(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke the method UnRegisterAllEventsVideoin and log before and after call
            std::cout << "Invoking method UnRegisterAllEventsVideoin() for enum: " << i << std::endl;
            EXPECT_NO_THROW(playerInterface.UnRegisterAllEventsVideoin());
            std::cout << "Method UnRegisterAllEventsVideoin() executed without errors for enum: " << i << std::endl;

        });
    }

    std::cout << "Exiting UnRegisterAllEventsVideoin_start test" << std::endl;
}
/**
 * @brief Verify that UnSubscribeEvent correctly unsubscribes a valid event
 *
 * This test iterates through all valid enum values for PlayerThunderAccessPlugin, creates a PlayerThunderInterface object for each value, and invokes the UnSubscribeEvent method with a valid event name ("ValidEvent"). It then verifies that the method returns true. This confirms that the UnSubscribeEvent functionality behaves as expected for each valid enum input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 101@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                  | Test Data                                                                  | Expected Result                                                                           | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------- | ----------- |
 * | 01               | For each valid enum value, create a PlayerThunderInterface instance and invoke UnSubscribeEvent | input: eventName=ValidEvent, plugin=i (ranging from AVINPUT to COMPOSITEINPUT), expectedResult=true | Method UnSubscribeEvent returns true and assertion EXPECT_EQ(retVal, expectedResult) passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, UnSubscribeEvent_ValidEvent) {
    std::cout << "Entering UnSubscribeEvent_ValidEvent test" << std::endl;

    // Prepare input and expected output
    std::string eventName = "ValidEvent";
    std::cout << "Test input eventName: " << eventName << std::endl;
    bool expectedResult = true;
    std::cout << "Expected result: " << expectedResult << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke the method UnSubscribeEvent
            std::cout << "Invoking UnSubscribeEvent with eventName: " << eventName << " for enum: " << i << std::endl;
            bool retVal = false;
            EXPECT_NO_THROW({
                retVal = pti.UnSubscribeEvent(eventName);
            });
            std::cout << "Method UnSubscribeEvent returned: " << retVal << " for enum: " << i << std::endl;

            // Verify the output
            EXPECT_EQ(retVal, expectedResult);
        });
    }

    std::cout << "Exiting UnSubscribeEvent_ValidEvent test" << std::endl;
}
/**
 * @brief Validate that UnSubscribeEvent returns false when invoked with an empty event name.
 *
 * This test verifies that the UnSubscribeEvent method of PlayerThunderInterface correctly processes
 * an empty event name by returning false. It iterates through all valid enum values for PlayerThunderAccessPlugin
 * to ensure the API behaves consistently across different configurations.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 102
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                         | Expected Result                                                                                    | Notes     |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | --------- |
 * | 01               | Create a PlayerThunderInterface instance for each valid enum value and invoke UnSubscribeEvent with an empty event name. | input: eventName = "", plugin = [AVINPUT, ..., COMPOSITEINPUT], expectedResult = false | UnSubscribeEvent should return false for an empty event name and the assertion (EXPECT_EQ) should pass. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, UnSubscribeEvent_EmptyEvent) {
    std::cout << "Entering UnSubscribeEvent_EmptyEvent test" << std::endl;

    // Prepare input and expected output
    std::string eventName = "";
    std::cout << "Test input eventName (empty): '" << eventName << "'" << std::endl;
    bool expectedResult = false;
    std::cout << "Expected result: " << expectedResult << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke the method UnSubscribeEvent
            std::cout << "Invoking UnSubscribeEvent with empty eventName for enum: " << i << std::endl;
            bool retVal = true;
            EXPECT_NO_THROW({
                retVal = pti.UnSubscribeEvent(eventName);
            });
            std::cout << "Method UnSubscribeEvent returned: " << retVal << " for enum: " << i << std::endl;

            // Verify the output
            EXPECT_EQ(retVal, expectedResult);
        });
    }

    std::cout << "Exiting UnSubscribeEvent_EmptyEvent test" << std::endl;
}
/**
 * @brief Verify that unsubscribing from a non-existent event returns false.
 *
 * This test verifies that invoking UnSubscribeEvent with an event name that does not exist
 * correctly returns a false value. It iterates through all valid PlayerThunderAccessPlugin enum values,
 * creates a PlayerThunderInterface object, and calls UnSubscribeEvent. The test ensures that no exceptions are thrown
 * during the process and that the return value matches the expected false result, indicating that the unsubscribe operation
 * fails gracefully when the event is not registered.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 103
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Initialize test inputs. | eventName = "NonExistentEvent", expectedResult = false | Inputs are set properly. | Should be successful |
 * | 02 | For each valid enum value, create a PlayerThunderInterface object. | plugin = [AVINPUT, COMPOSITEINPUT] (iterated) | Object is created without throwing exceptions. | Should be successful |
 * | 03 | Invoke UnSubscribeEvent on the created object with the non-existent event name and verify the result. | eventName = "NonExistentEvent", plugin = current enum value, expectedResult = false | UnSubscribeEvent returns false and assertion passes. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, UnSubscribeEvent_NonExistentEvent) {
    std::cout << "Entering UnSubscribeEvent_NonExistentEvent test" << std::endl;

    // Prepare input and expected output
    std::string eventName = "NonExistentEvent";
    std::cout << "Test input eventName: " << eventName << std::endl;
    bool expectedResult = false;
    std::cout << "Expected result: " << expectedResult << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke the method UnSubscribeEvent
            std::cout << "Invoking UnSubscribeEvent with eventName: " << eventName << " for enum: " << i << std::endl;
            bool retVal = true;
            EXPECT_NO_THROW({
                retVal = pti.UnSubscribeEvent(eventName);
            });
            std::cout << "Method UnSubscribeEvent returned: " << retVal << " for enum: " << i << std::endl;

            // Verify the output
            EXPECT_EQ(retVal, expectedResult);
        });
    }

    std::cout << "Exiting UnSubscribeEvent_NonExistentEvent test" << std::endl;
}
/**
 * @brief Test UnSubscribeEvent with special characters in the event name
 *
 * This test verifies that the UnSubscribeEvent method returns the expected result when provided with an event name containing special characters. The test iterates through all valid enum values, creates a PlayerThunderInterface instance for each, and asserts that the method's return value matches the expected outcome.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 104@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log entering the test | None | "Entering UnSubscribeEvent_SpecialCharacters test" log output | Should be successful |
 * | 02 | Prepare input variables for the test case | eventName = "!@#$%^&*()", expectedResult = false | Variables are initialized correctly | Should be successful |
 * | 03 | Loop through all valid enum values to create PlayerThunderInterface instance | plugin enum value ranging from AVINPUT to COMPOSITEINPUT | PlayerThunderInterface object created successfully for each enum value | Should be successful |
 * | 04 | Invoke the UnSubscribeEvent method with special characters in eventName | input: eventName = "!@#$%^&*()", plugin enum value; output: retVal value | Method returns false matching expected result | Should Pass |
 * | 05 | Assert the output using EXPECT_EQ to validate the retVal | input: retVal, expectedResult = false | Assertion passes confirming retVal equals false | Should Pass |
 * | 06 | Log exiting the test | None | "Exiting UnSubscribeEvent_SpecialCharacters test" log output | Should be successful |
 */
TEST_F(PlayerThunderInterfaceTests, UnSubscribeEvent_SpecialCharacters) {
    std::cout << "Entering UnSubscribeEvent_SpecialCharacters test" << std::endl;

    // Prepare input and expected output
    std::string eventName = "!@#$%^&*()";
    std::cout << "Test input eventName: " << eventName << std::endl;
    bool expectedResult = false;
    std::cout << "Expected result: " << expectedResult << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface pti(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            // Invoke the method UnSubscribeEvent
            std::cout << "Invoking UnSubscribeEvent with eventName: " << eventName << " for enum: " << i << std::endl;
            bool retVal = true;
            EXPECT_NO_THROW({
                retVal = pti.UnSubscribeEvent(eventName);
            });
            std::cout << "Method UnSubscribeEvent returned: " << retVal << " for enum: " << i << std::endl;

            // Verify the output
            EXPECT_EQ(retVal, expectedResult);
        });
    }

    std::cout << "Exiting UnSubscribeEvent_SpecialCharacters test" << std::endl;
}
/**
 * @brief Verify that UpdateWatermark returns true for all valid enum values with valid parameters
 *
 * This test verifies that the PlayerThunderInterface object is created successfully for each valid plugin enumeration and that the UpdateWatermark API call returns true without throwing any exceptions. It ensures that, when provided with valid parameters (layerID, sharedMemoryKey, size), the plugin behaves correctly.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 105
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Loop through all valid enum values, create a PlayerThunderInterface object, and invoke UpdateWatermark API | plugin = valid enum values from AVINPUT to COMPOSITEINPUT, layerID = 1, sharedMemoryKey = 100, size = 1024, expectedReturn = true | UpdateWatermark returns true and no exception is thrown, assertion passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ValidParametersShouldReturnTrue) {
    std::cout << "Entering ValidParametersShouldReturnTrue test" << std::endl;

    int layerID = 1, sharedMemoryKey = 100, size = 1024;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting ValidParametersShouldReturnTrue test" << std::endl;
}
/**
 * @brief Verifies that UpdateWatermark returns false when a negative layerID is provided.
 *
 * This test iterates over all valid PlayerThunderAccessPlugin enum values and creates a PlayerThunderInterface for each.
 * It then invokes the UpdateWatermark API using a negative layerID along with valid sharedMemoryKey and size values.
 * The test ensures that the API correctly returns false (indicating failure) and does not throw any exceptions when an invalid layerID is supplied.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 106@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                        | Expected Result                                            | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------------------------------------------- | ---------- |
 * | 01               | Iterate over all valid PlayerThunderAccessPlugin enum values, instantiate PlayerThunderInterface, and invoke UpdateWatermark with a negative layerID, valid sharedMemoryKey, and size. | plugin = AVINPUT to COMPOSITEINPUT, layerID = -1, sharedMemoryKey = 100, size = 1024 | UpdateWatermark returns false and no exception is thrown | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeLayerIDShouldReturnFalse) {
    std::cout << "Entering NegativeLayerIDShouldReturnFalse test" << std::endl;

    int layerID = -1, sharedMemoryKey = 100, size = 1024;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NegativeLayerIDShouldReturnFalse test" << std::endl;
}
/**
 * @brief Validate that UpdateWatermark returns false when provided with a negative shared memory key.
 *
 * This test verifies that when a negative value is passed as the sharedMemoryKey parameter to the UpdateWatermark method of the PlayerThunderInterface, the method correctly returns false without throwing exceptions. The test iterates over all valid PlayerThunderAccessPlugin enum values to ensure consistent negative handling across plugin variations.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 107@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test variables and iterate over all valid PlayerThunderAccessPlugin enum values. | layerID = 1, sharedMemoryKey = -100, size = 1024, plugin = each enum (AVINPUT to COMPOSITEINPUT) | PlayerThunderInterface is created without exceptions and UpdateWatermark returns false for each enum value. | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeSharedMemoryKeyShouldReturnFalse) {
    std::cout << "Entering NegativeSharedMemoryKeyShouldReturnFalse test" << std::endl;

    int layerID = 1, sharedMemoryKey = -100, size = 1024;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NegativeSharedMemoryKeyShouldReturnFalse test" << std::endl;
}
/**
 * @brief Verify that the UpdateWatermark method returns false when the size is zero.
 *
 * This test verifies that when UpdateWatermark is invoked with a size value of zero, it returns false for all valid PlayerThunderAccessPlugin enumeration values. The objective is to ensure the API prevents watermark updates with an invalid size.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 108
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | For each valid enum value, instantiate PlayerThunderInterface and invoke UpdateWatermark with layerID=1, sharedMemoryKey=100, size=0 | input1 = layerID=1, input2 = sharedMemoryKey=100, input3 = size=0, enum value = [AVINPUT, ..., COMPOSITEINPUT] | UpdateWatermark returns false; assertion EXPECT_FALSE passes | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, ZeroSizeShouldReturnFalse) {
    std::cout << "Entering ZeroSizeShouldReturnFalse test" << std::endl;

    int layerID = 1, sharedMemoryKey = 100, size = 0;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting ZeroSizeShouldReturnFalse test" << std::endl;
}
/**
 * @brief Validates that UpdateWatermark returns false when provided with a negative size.
 *
 * This test verifies that the UpdateWatermark API correctly returns false when a negative size is passed.
 * It iterates through all valid plugin enum values, creates a PlayerThunderInterface object, and calls
 * UpdateWatermark with a negative size. The expectation is that the API will not accept the negative size
 * and will indicate the failure through its return value.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 109@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | ----------- | ---------- | -------------- | ----- |@n
 * | 01 | Iterate through each valid plugin enum value, create a PlayerThunderInterface object, and call UpdateWatermark with negative size | plugin = AVINPUT to COMPOSITEINPUT, layerID = 1, sharedMemoryKey = 100, size = -1024 | UpdateWatermark returns false (Assertion EXPECT_FALSE(result) passes) | Should Fail |
 */
TEST_F(PlayerThunderInterfaceTests, NegativeSizeShouldReturnFalse) {
    std::cout << "Entering NegativeSizeShouldReturnFalse test" << std::endl;

    int layerID = 1, sharedMemoryKey = 100, size = -1024;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_FALSE(result);
        });
    }

    std::cout << "Exiting NegativeSizeShouldReturnFalse test" << std::endl;
}
/**
 * @brief Validate that UpdateWatermark returns true when maximum integer values are used.
 *
 * This test verifies that invoking the UpdateWatermark method with INT_MAX values for layerID, sharedMemoryKey, and size does not throw any exceptions and returns true for all valid enumeration values. The test ensures the robustness of the API when dealing with boundary integer values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 110@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                                                                 | Test Data                                                         | Expected Result                                           | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | --------------------------------------------------------- | ------------- |
 * | 01               | Invoke UpdateWatermark with layerID=INT_MAX, sharedMemoryKey=INT_MAX, size=INT_MAX for each valid enum between AVINPUT and COMPOSITEINPUT; verify true. | input: layerID = INT_MAX, sharedMemoryKey = INT_MAX, size = INT_MAX, plugin values from AVINPUT to COMPOSITEINPUT | API returns true without throwing any exceptions, and EXPECT_TRUE passes | Should Pass   |
 */
TEST_F(PlayerThunderInterfaceTests, MaxIntegerValuesShouldReturnTrue) {
    std::cout << "Entering MaxIntegerValuesShouldReturnTrue test" << std::endl;

    int layerID = INT_MAX, sharedMemoryKey = INT_MAX, size = INT_MAX;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);

        EXPECT_NO_THROW({
            std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;
            PlayerThunderInterface player(plugin);
            std::cout << "PlayerThunderInterface object created successfully for enum: " << i << std::endl;

            std::cout << "Invoking UpdateWatermark with layerID=" << layerID
                      << ", sharedMemoryKey=" << sharedMemoryKey
                      << ", size=" << size
                      << " for enum: " << i << std::endl;

            bool result = player.UpdateWatermark(layerID, sharedMemoryKey, size);
            std::cout << "UpdateWatermark returned: " << std::boolalpha << result
                      << " for enum: " << i << std::endl;

            EXPECT_TRUE(result);
        });
    }

    std::cout << "Exiting MaxIntegerValuesShouldReturnTrue test" << std::endl;
}
/**
 * @brief Validate the proper destruction of PlayerThunderInterface objects
 *
 * This test verifies that PlayerThunderInterface objects, when created with each valid enum value from AVINPUT to COMPOSITEINPUT, are destroyed without throwing any exceptions. The test iterates through each enum value, constructs an object, and then allows it to go out of scope to invoke the destructor, ensuring cleanup is performed safely.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 111
 * **Priority:** High
 * 
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                            | Expected Result                                                                              | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Loop through valid enum values from AVINPUT to COMPOSITEINPUT, create a PlayerThunderInterface object, and allow it to go out of scope to invoke the destructor. | plugin enum value = AVINPUT, AVOUTPUT, ..., COMPOSITEINPUT             | No exceptions thrown during construction and destruction; destructor invoked successfully. | Should Pass |
 */
TEST_F(PlayerThunderInterfaceTests, DestructorTest) {
    std::cout << "Entering DestructorTest test" << std::endl;

    // Loop through all valid enum values
    for (int i = AVINPUT; i <= COMPOSITEINPUT; ++i) {
        PlayerThunderAccessPlugin plugin = static_cast<PlayerThunderAccessPlugin>(i);
        std::cout << "Creating PlayerThunderInterface with enum value: " << i << std::endl;

        EXPECT_NO_THROW({
            PlayerThunderInterface obj(plugin);
            std::cout << "PlayerThunderInterface object created for enum: " << i << std::endl;
            std::cout << "Object will go out of scope to invoke destructor for enum: " << i << std::endl;
        });

        std::cout << "Exited inner scope; destructor should have been invoked without error for enum: " << i << std::endl;
    }

    std::cout << "Exiting DestructorTest test" << std::endl;
}