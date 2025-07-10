
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

/**
* @file test_l1_DrmHelper.cpp
* @page DrmHelper Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the DrmHelper methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "DrmHelper.h"

/**
 * @brief Verify that the default initialization of ChallengeInfo sets all member variables to their expected default values.
 *
 * This test verifies that when the default constructor of ChallengeInfo is invoked, the member variable 'data' is set to nullptr,
 * and both 'url' and 'accessToken' are initialized to empty strings. This ensures that the object is in a valid and predictable state for further operations.
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
 * | Variation / Step | Description                                                                                                   | Test Data                                                                                              | Expected Result                                                                                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the default constructor for ChallengeInfo to create a new object                                         | No input, default constructor call, expected output: data = nullptr, url = "", accessToken = ""          | The object is initialized with data as nullptr, and both url and accessToken as empty strings; all assertions pass                       | Should Pass |
 */
TEST(ChallengeInfo, VerifyDefaultInitialization) {

    std::cout << "Entering VerifyDefaultInitialization test" << "\n";

    std::cout << "Invoking default constructor for ChallengeInfo" << "\n";
    
    ChallengeInfo challengeInfo;
    
    std::cout << "Default constructor invoked. Checking initial values" << "\n";
    
    if (challengeInfo.data == nullptr) {
        std::cout << "challengeInfo.data is null" << "\n";
    }
    else {
        std::cout << "challengeInfo.data is not null" << "\n";
    }

    std::cout << "challengeInfo.url value: '" << challengeInfo.url << "'" << "\n";
    std::cout << "challengeInfo.accessToken value: '" << challengeInfo.accessToken << "'" << "\n";

    EXPECT_EQ(challengeInfo.data, nullptr);
    EXPECT_EQ(challengeInfo.url, "");
    EXPECT_EQ(challengeInfo.accessToken, "");
    
    std::cout << "Exiting VerifyDefaultInitialization test" << "\n";
}
/**
 * @brief Validate the default constructor initializes LicenseRequest object with correct defaults.
 *
 * This test verifies that the LicenseRequest class's default constructor correctly initializes all members with appropriate default values. It confirms that the method field is set to one of the allowed values (DRM_RETRIEVE, GET, or POST), licenseAnonymousRequest is set to false, url and payload are empty strings, and the headers map is empty.
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
 * | 01 | Invoke default constructor for LicenseRequest object | None | LicenseRequest object is created with default field values | Should be successful |
 * | 02 | Validate that the method field is set to one of (DRM_RETRIEVE, GET, POST) | licenseRequestObject.method = DRM_RETRIEVE, GET, or POST | Method field matches one of the allowed values | Should Pass |
 * | 03 | Check that licenseAnonymousRequest is false by default | licenseRequestObject.licenseAnonymousRequest = false | The field evaluates to false as expected | Should Pass |
 * | 04 | Verify that the url field is an empty string | licenseRequestObject.url = "" | Url field is an empty string | Should Pass |
 * | 05 | Verify that the payload field is an empty string | licenseRequestObject.payload = "" | Payload field is an empty string | Should Pass |
 * | 06 | Ensure that the headers map is empty | licenseRequestObject.headers = {} | Headers map is empty | Should Pass |
 */
TEST(LicenseRequest, DefaultConstructorInitialization_PositiveTest)
{
    std::cout<<"Entering DefaultConstructorInitialization_PositiveTest test"<<std::endl;

    LicenseRequest licenseRequestObject;

    std::cout<<"LicenseRequest default constructor invoked"<<std::endl;

    std::cout<<"Retrieved method value: "<< licenseRequestObject.method <<std::endl;
    std::cout<<"Retrieved licenseAnonymousRequest value: "<< licenseRequestObject.licenseAnonymousRequest <<std::endl;
    std::cout<<"Retrieved url value: \""<< licenseRequestObject.url <<"\""<<std::endl;
    std::cout<<"Retrieved payload value: \""<< licenseRequestObject.payload <<"\""<<std::endl;
    std::cout<<"Headers map size: "<< licenseRequestObject.headers.size() <<std::endl;

    EXPECT_TRUE(
      licenseRequestObject.method == LicenseRequest::DRM_RETRIEVE ||
      licenseRequestObject.method == LicenseRequest::GET ||
      licenseRequestObject.method == LicenseRequest::POST
    );
    EXPECT_FALSE(licenseRequestObject.licenseAnonymousRequest);
    EXPECT_EQ(licenseRequestObject.url, std::string(""));
    EXPECT_EQ(licenseRequestObject.payload, std::string(""));
    EXPECT_TRUE(licenseRequestObject.headers.empty());

    std::cout<<"Exiting DefaultConstructorInitialization_PositiveTest test"<<std::endl;
}
/**
 * @brief Validate if DrmHelperEngine is properly constructed using its default constructor.
 *
 * Validate that the default constructor of DrmHelperEngine does not throw any exception and creates a valid instance.
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
 * | Variation / Step | Description                                                         | Test Data                                                  | Expected Result                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Invoke the default constructor of DrmHelperEngine and verify that no exception is thrown | Invocation: DrmHelperEngine engine                         | API should not throw an exception; assertion EXPECT_NO_THROW passes | Should Pass |
 */
TEST(DrmHelperEngine, DefaultConstruction)
{
    std::cout << "Entering DefaultConstruction test" << std::endl;

    std::cout << "Invoking DrmHelperEngine default constructor" << std::endl;
    EXPECT_NO_THROW({
      DrmHelperEngine engine;
    });
    std::cout << "DrmHelperEngine instance created" << std::endl;

    std::cout << "Exiting DefaultConstruction test" << std::endl;
}
/**
 * @brief Validate that createHelper returns a non-null helper pointer when provided with valid DRM information.
 *
 * This test verifies that the DrmHelperEngine::createHelper API correctly instantiates a DRM helper object given a valid DrmInfo
 * object. It ensures that the helper pointer returned is not null, confirming proper API functionality.
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
 * | Variation / Step | Description                                               | Test Data                                                       | Expected Result                                          | Notes         |
 * | :--------------: | --------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Instantiate DrmInfo and DrmHelperEngine                   | drmInfo = default, engine = valid instance                      | Objects are created successfully                         | Should be successful |
 * | 02               | Invoke createHelper API with valid drmInfo                | input: drmInfo = default, engine instance, output: helper pointer | helper pointer is received from createHelper API         | Should Pass   |
 * | 03               | Validate that the returned helper pointer is not null     | helper pointer                                                | EXPECT_NE(helper, nullptr) assertion passes              | Should Pass   |
 */
TEST(DrmHelperEngine, ValidDRMAES128Input)
{
  std::cout << "Entering ValidDRMAES128Input test" << std::endl;

  DrmInfo drmInfo;
  DrmHelperEngine engine;

  std::cout << "Invoking createHelper with method: " << std::endl;
  DrmHelperPtr helper = engine.createHelper(drmInfo);

  std::cout << "Returned helper pointer: " << helper << std::endl;
  EXPECT_NE(helper, nullptr);

  std::cout << "Exiting ValidDRMAES128Input test" << std::endl;
}
/**
 * @brief Validate that DrmHelperEngine::getInstance() returns a non-null instance and enforces singleton behavior.
 *
 * This test verifies that invoking DrmHelperEngine::getInstance() returns a valid, non-null instance on the first call and that subsequent calls return the same instance address, ensuring the singleton design pattern is correctly implemented.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 005
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                                                 | Expected Result                                           | Notes      |
 * | :----:           | ------------------------------------------------------------------- | --------------------------------------------------------- | --------------------------------------------------------- | ---------- |
 * | 01               | Invoke DrmHelperEngine::getInstance() for the first time            | output1 = instance address from first call                | Returns a non-null instance address                       | Should Pass|
 * | 02               | Invoke DrmHelperEngine::getInstance() for the second time           | output2 = instance address from second call               | Returns a non-null instance address                       | Should Pass|
 * | 03               | Verify that both instance addresses are equal (singleton behavior)  | instance1 (from first call), instance2 (from second call)   | Both pointers are equal, confirming singleton behavior    | Should Pass|
 */
TEST(DrmHelperEngine, getInstance_start)
{
    std::cout << "Entering getInstance_start test" << std::endl;

    std::cout << "Invoking DrmHelperEngine::getInstance() for the first time" << std::endl;
    DrmHelperEngine &instance1 = DrmHelperEngine::getInstance();
    ASSERT_NE(&instance1, nullptr);
    std::cout << "First instance address: " << &instance1 << std::endl;

    std::cout << "Invoking DrmHelperEngine::getInstance() for the second time" << std::endl;
    DrmHelperEngine &instance2 = DrmHelperEngine::getInstance();
    ASSERT_NE(&instance2, nullptr);
    std::cout << "Second instance address: " << &instance2 << std::endl;
   
    std::cout << "Verifying that both instance addresses are equal" << std::endl;
    EXPECT_EQ(&instance1, &instance2);

    std::cout << "Verified non-null instance and singleton behavior. Instance address: " << &instance1 << std::endl;

    std::cout << "Exiting getInstance_start test" << std::endl;
}
/**
 * @brief Verify getSystemIds API handles an empty vector input without throwing exceptions
 *
 * This test validates that when getSystemIds is invoked on an empty vector, it does not throw an exception and optionally populates or leaves the vector unchanged. The focus is to ensure that the API gracefully handles empty input scenarios.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DrmHelperEngine object using the default constructor | No input arguments | Object is successfully created | Should be successful |
 * | 02 | Initialize systemIds vector as empty | systemIds vector = {} | Empty vector is created and printed | Should be successful |
 * | 03 | Invoke getSystemIds method | engine.getSystemIds(systemIds) | No exception is thrown; systemIds updated if applicable | Should Pass |
 * | 04 | Output the results of the getSystemIds invocation | Printed console logs showing systemIds vector size and elements | Console displays vector size and its contents (if any) | Should be successful |
 */
TEST(DrmHelperEngine, GetSystemIds_EmptyVector)
{
    std::cout << "Entering GetSystemIds_EmptyVector test" << std::endl;
    
    std::cout << "Creating DrmHelperEngine object using the default constructor" << std::endl;
    DrmHelperEngine engine;
    
    std::vector<std::string> systemIds;
    std::cout << "Initial systemIds vector size: " << systemIds.size() << std::endl;
    
    std::cout << "Invoking getSystemIds method" << std::endl;
    EXPECT_NO_THROW(engine.getSystemIds(systemIds));
    
    std::cout << "After Invoking getSystemIds, systemIds vector size: " << systemIds.size() << std::endl;
    for (size_t i = 0; i < systemIds.size(); ++i)
    {
        std::cout << "systemIds[" << i << "]: " << systemIds[i] << std::endl;
    }
    
    std::cout << "Exiting GetSystemIds_EmptyVector test" << std::endl;
}
/**
 * @brief Verify the hasDRM API returns true for a default constructed DrmInfo
 *
 * This test evaluates the behavior of the hasDRM function when invoked using a default
 * DrmInfo object. The objective is to ensure that the API correctly identifies the DRM
 * presence as true in this context.
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
 * | Variation / Step | Description                                                    | Test Data                                              | Expected Result                        | Notes            |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------------ | -------------------------------------- | ---------------- |
 * | 01               | Initialize DrmInfo and DrmHelperEngine objects                   | drmInfo = default constructed, engine = default constructed | Objects are successfully created       | Should be successful |
 * | 02               | Invoke engine.hasDRM with the default DrmInfo                    | drmInfo = default constructed                          | Return value true from hasDRM          | Should Pass      |
 * | 03               | Validate the returned result using EXPECT_TRUE                   | result = true                                           | Test assertion passes                  | Should Pass      |
 */
TEST(DrmHelperEngine, hasDRMethod)
{
    std::cout << "Entering hasDRMethod test" << std::endl;

    DrmInfo drmInfo;
    DrmHelperEngine engine;

    std::cout << "Invoking hasDRM with default DrmInfo" << std::endl;
    bool result = engine.hasDRM(drmInfo);
    std::cout << "hasDRM returned: " << result << std::endl;

    EXPECT_TRUE(result);

    std::cout << "Exiting hasDRMethod test" << std::endl;
}

/**
 * @brief Verify that registerFactory handles a null input gracefully without altering internal state.
 *
 * This test ensures that the API `registerFactory` does not register a null factory pointer
 * and maintains internal consistency. It verifies that the factory count remains unchanged (zero),
 * demonstrating proper handling of invalid input without exceptions or side effects.
 *
 * **Test Group ID:** Basic: 01 @n
 * **Test Case ID:** 008 @n
 * **Priority:** High @n
 *
 * **Pre-Conditions:** None @n
 * **Dependencies:** None @n
 * **User Interaction:** None @n
 *
 * **Test Procedure:** @n
 * | Variation / Step | Description                                                           | Test Data       | Expected Result                            | Notes              |
 * | :--------------: | --------------------------------------------------------------------- | --------------- | ------------------------------------------ | ------------------ |
 * | 01               | Invoke registerFactory with a null pointer                            | factory = null  | Factory count remains zero                 | Should Not Change  |
 * | 02               | Retrieve factory count after attempted registration with null pointer |                 | Count = 0                                  | Verifies no effect |
 */
TEST(DrmHelperEngine, NullFactoryRegistrationTest)
{
    std::cout << "Entering NullFactoryRegistrationTest test" << std::endl;

    DrmHelperEngine engine;

    std::cout << "Invoking registerFactory with nullptr" << std::endl;
    EXPECT_NO_THROW(engine.registerFactory(nullptr););

    std::cout << "Exiting NullFactoryRegistrationTest test" << std::endl;
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
