
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

class PlayerExternalsInterfaceBaseTestImpl
    : public PlayerExternalsInterfaceBase
{
public:
    PlayerExternalsInterfaceBaseTestImpl()
    {
        m_displayWidth  = 1920;
        m_displayHeight = 1080;
    }

    void Initialize() override {}

    void SetUseFireBoltSDK(bool) {}

    void GetDisplayResolution(int &width, int &height)
    {
        width  = m_displayWidth;
        height = m_displayHeight;
    }

    bool GetActiveInterface()
    {
        return false;
    }
    
    ~PlayerExternalsInterfaceBaseTestImpl() override = default;
};
class PlayerExternalsInterfaceBaseTests : public ::testing::Test
{
protected:
    std::unique_ptr<PlayerExternalsInterfaceBase> player;

    void SetUp() override
    {
        player = std::make_unique<PlayerExternalsInterfaceBaseTestImpl>();
    }

    void TearDown() override
    {
        player.reset();
    }
};
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
TEST_F(PlayerExternalsInterfaceBaseTests, GetActiveInterface) {
    std::cout << "Entering GetActiveInterface test" << std::endl;
    
    // Create object using default constructor.
    EXPECT_NO_THROW({        
        // Invoke the GetActiveInterface method.
        std::cout << "Invoking method GetActiveInterface()" << std::endl;
        bool activeInterface = player->GetActiveInterface();
        
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
TEST_F(PlayerExternalsInterfaceBaseTests, GetDisplayResolution_ValidResolution_StandardHD) {
    GTEST_SKIP();
    std::cout << "Entering GetDisplayResolution_ValidResolution_StandardHD test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW({
        std::cout << "Constructed PlayerExternalsInterfaceBase object" << std::endl;
        
        int width = -1;
        int height = -1;
        std::cout << "Initial values before invoking GetDisplayResolution: width = " << width 
                  << ", height = " << height << std::endl;
        
        // Invoke the method
        std::cout << "Invoking GetDisplayResolution method" << std::endl;
        player->GetDisplayResolution(width, height);
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
TEST_F(PlayerExternalsInterfaceBaseTests, PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase) {
    std::cout << "Entering PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase test" << std::endl;
    
    EXPECT_NO_THROW({
        auto obj =
            std::make_unique<PlayerExternalsInterfaceBaseTestImpl>();
            std::cout << "Invoked method: PlayerExternalsInterfaceBase() default constructor" << std::endl;
    });
    
    std::cout << "Exiting PlayerExternalsInterfaceBase_PlayerExternalsInterfaceBase test" << std::endl;
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
TEST_F(PlayerExternalsInterfaceBaseTests, DestructionOfPlayerExternalsInterfaceBase) {
    std::cout << "Entering PlayerExternalsInterfaceBase::~PlayerExternalsInterfaceBase()_start test" << std::endl;

    EXPECT_NO_THROW({
        PlayerExternalsInterfaceBase* obj =
            new PlayerExternalsInterfaceBaseTestImpl();

        delete obj;  // invokes derived + base destructor
    });
    
    std::cout << "Exiting PlayerExternalsInterfaceBase::~PlayerExternalsInterfaceBase()_end test" << std::endl;
}
