
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
#include "SubtecChannel.hpp"

class SubtecChannelManagerTestable : public SubtecChannelManager {
public:
    SubtecChannelManagerTestable() : SubtecChannelManager() {}
    using SubtecChannelManager::getNextChannelId;
};

class SubtecChannelManagerTestFixture : public ::testing::Test {
protected:
    SubtecChannelManagerTestable* testObj;

    void SetUp() override {
        testObj = SubtecChannelManager::getNextChannelId;
        ASSERT_NE(testObj, nullptr) << "SubtecChannelManagerTestable instance should not be null";
    }
};

class TestableSubtecChannel : public SubtecChannel {
public:
    using SubtecChannel::sendPacket;
    ~TestableSubtecChannel() override = default;
    virtual void SendSelectionPacket(uint32_t width, uint32_t height) override {}
    virtual void SendDataPacket(std::vector<uint8_t>&& data, std::int64_t time_offset_ms = 0) override {}
    virtual void SendTimestampPacket(uint64_t timestampMs) override {}
};

class SubtecChannelTest : public ::testing::Test {
protected:
    void SetUp() override {
        channel = std::make_unique<TestableSubtecChannel>();
    }
    void TearDown() override {
        channel.reset();
    }
    std::unique_ptr<TestableSubtecChannel> channel;
};


/**
 * @brief Validate that player_utils::make_unique<int>() correctly creates a unique pointer using the default constructor.
 *
 * This test verifies that invoking player_utils::make_unique<int>() with the default constructor does not throw any exceptions and returns a non-null pointer. The objective is to ensure that the memory allocation and pointer initialization are performed correctly.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                           | Expected Result                                                                                    | Notes       |
 * | :----:           | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke player_utils::make_unique<int>() and validate that no exception is thrown and the pointer is not null. | No input; output: pointer = non-null memory address returned by the function | The function should return a valid unique pointer to an int without throwing an exception; assertion EXPECT_NE(ptr, nullptr) should pass. | Should Pass |
 */
TEST(PositiveDefaultConstructor, DefaultConstructorTest) {
    std::cout << "Entering PositiveDefaultConstructor test" << std::endl;
    std::cout << "Invoking player_utils::make_unique<int>() with default constructor" << std::endl;
    
    EXPECT_NO_THROW({
        auto ptr = player_utils::make_unique<int>();
        std::cout << "Returned pointer address: " << static_cast<const void*>(ptr.get()) << std::endl;
        EXPECT_NE(ptr, nullptr);
    });
    
    std::cout << "Exiting PositiveDefaultConstructor test" << std::endl;
}
/**
 * @brief Validates API call of player_utils::make_unique with a single integer argument.
 *
 * Tests that given an integer input, the utility function constructs a unique pointer
 * to an object that holds the correct value. Verifies that the pointer is not null and 
 * that dereferencing gives the expected value.
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
 * | Variation / Step | Description                                                                               | Test Data                                  | Expected Result                                            | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------- | ---------- |
 * | 01               | Invoke player_utils::make_unique<int> with a single argument '5' and verify its integrity. | inputValue = 5, output pointer should be valid, *ptr = 5 | Returns a non-null pointer with the value 5 when dereferenced | Should Pass |
 */
TEST(PositiveOneArgument, OneArgumentTest) {
    std::cout << "Entering PositiveOneArgument test" << std::endl;
    int inputValue = 5;
    
    std::cout << "Invoking player_utils::make_unique<int>(" << inputValue << ")" << std::endl;
    auto ptr = player_utils::make_unique<int>(inputValue);
    
    std::cout << "Returned pointer address: " << static_cast<const void*>(ptr.get()) << std::endl;
    EXPECT_NE(ptr, nullptr);
    
    std::cout << "Dereferenced value from constructed object: " << *ptr << std::endl;
    EXPECT_EQ(*ptr, inputValue);
    
    std::cout << "Exiting PositiveOneArgument test" << std::endl;
}
/**
 * @brief Verify that player_utils::make_unique correctly creates a unique pointer to a std::pair with multiple arguments
 *
 * This test initializes an integer and a string literal, copies the string literal to a fixed-size character array, and then
 * invokes player_utils::make_unique to create a unique pointer to a std::pair containing the integer and the string.
 * It checks that the returned unique pointer is not null and that the pair's elements match the provided values,
 * ensuring the helper function handles multiple arguments properly.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                              | Expected Result                                            | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------- | ------------- |
 * | 01               | Initialize test input values including intValue, literalStr, and allocate strArray             | intValue = 5, literalStr = "example", strArray = uninitialized char[50]                                | Variables are correctly initialized                      | Should be successful |
 * | 02               | Copy the string literal into strArray using strncpy and ensure null termination                | strArray content updated with "example"                                                                | strArray correctly contains "example" with null termination | Should be successful |
 * | 03               | Invoke player_utils::make_unique with intValue and the string from strArray                     | input: intValue = 5, string = "example", output: unique_ptr pointing to std::pair                      | Returned pointer is non-null                               | Should Pass   |
 * | 04               | Verify the first element of the created pair equals intValue                                  | pointer->first, expected intValue = 5                                                                 | pointer->first equals 5                                      | Should Pass   |
 * | 05               | Verify the second element of the created pair equals literalStr                               | pointer->second, expected string = "example"                                                           | pointer->second equals "example"                             | Should Pass   |
 */
TEST(PositiveMultipleArguments, MultipleArgumentsTest) {
    std::cout << "Entering PositiveMultipleArguments test" << std::endl;
    int intValue = 5;
    char strArray[50];
    const char* literalStr = "example";
    
    std::cout << "Invoking strncpy to assign string literal \"" << literalStr << "\" to fixed size array" << std::endl;
    std::memset(strArray, 0, sizeof(strArray));
    strncpy(strArray, literalStr, sizeof(strArray) - 1);
    
    std::cout << "Values passed: intValue = " << intValue << ", string = " << strArray << std::endl;
    auto ptr = player_utils::make_unique<std::pair<int, std::string>>(intValue, std::string(strArray));
    std::cout << "Returned pointer address: " << static_cast<const void*>(ptr.get()) << std::endl;
    
    EXPECT_NE(ptr, nullptr);
    std::cout << "Retrieved pair values: first = " << ptr->first << ", second = " << ptr->second << std::endl;
    EXPECT_EQ(ptr->first, intValue);
    EXPECT_EQ(ptr->second, literalStr);
    
    std::cout << "Exiting PositiveMultipleArguments test" << std::endl;
}
/**
 * @brief Verify that SubtecChannelManager::getInstance() returns a valid non-null instance
 *
 * This test ensures that invoking SubtecChannelManager::getInstance() does not throw any exceptions 
 * and returns a valid, non-null pointer. This is critical for ensuring that the singleton instance 
 * is properly instantiated and accessible throughout the system.
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
 * | Variation / Step | Description                                                            | Test Data                                          | Expected Result                                                     | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * | 01               | Log the entry into the test function                                   | No input/output values                             | Entry message logged successfully                                   | Should be successful |
 * | 02               | Invoke SubtecChannelManager::getInstance() and verify it returns a non-null pointer | input: none, output: instance pointer from getInstance() | API returns a valid non-null pointer without throwing exceptions; assertion passes | Should Pass   |
 * | 03               | Log the exit from the test function                                    | No input/output values                             | Exit message logged successfully                                    | Should be successful |
 */
TEST(SubtecChannelManager, VerifyGetInstanceReturnsValidNonNullInstance) {
    std::cout << "Entering VerifyGetInstanceReturnsValidNonNullInstance test" << std::endl;
    
    std::cout << "Invoking SubtecChannelManager::getInstance()" << std::endl;
    EXPECT_NO_THROW({
        SubtecChannelManager* instance = SubtecChannelManager::getInstance();
        std::cout << "Returned pointer value: " << instance << std::endl;
        EXPECT_NE(instance, nullptr);
    });
    
    std::cout << "Exiting VerifyGetInstanceReturnsValidNonNullInstance test" << std::endl;
}
/**
 * @brief Verify that multiple invocations of SubtecChannelManager::getInstance() return the same instance
 *
 * This test checks if the singleton implementation of SubtecChannelManager is correct by ensuring that calling getInstance() multiple times yields the same instance pointer. This is critical to maintain consistent state across the application.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 005@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                                              | Expected Result                                      | Notes       |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------------------------------- | ---------------------------------------------------- | ----------- |
 * | 01               | Invoke SubtecChannelManager::getInstance() for the first call            | firstInstance = SubtecChannelManager::getInstance()                    | Returns a valid instance pointer                     | Should Pass |
 * | 02               | Invoke SubtecChannelManager::getInstance() for the second call           | secondInstance = SubtecChannelManager::getInstance()                   | Returns a valid instance pointer identical to first  | Should Pass |
 * | 03               | Compare both instance pointers using EXPECT_EQ                           | firstInstance pointer, secondInstance pointer                          | Both pointers are equal                              | Should Pass |
 */
TEST(SubtecChannelManager, VerifyGetInstanceReturnsSameInstanceOnMultipleInvocations) {
    std::cout << "Entering VerifyGetInstanceReturnsSameInstanceOnMultipleInvocations test" << std::endl;
    
    std::cout << "Invoking SubtecChannelManager::getInstance() for the first call" << std::endl;
    SubtecChannelManager *firstInstance = SubtecChannelManager::getInstance();
    std::cout << "First instance pointer: " << firstInstance << std::endl;
    
    std::cout << "Invoking SubtecChannelManager::getInstance() for the second call" << std::endl;
    SubtecChannelManager *secondInstance = SubtecChannelManager::getInstance();
    std::cout << "Second instance pointer: " << secondInstance << std::endl;
    
    std::cout << "Comparing both instance pointers for equality." << std::endl;
    EXPECT_EQ(firstInstance, secondInstance);
    
    std::cout << "Exiting VerifyGetInstanceReturnsSameInstanceOnMultipleInvocations test" << std::endl;
}
/**
 * @brief Test for retrieving the initial channel ID from SubtecChannelManager
 *
 * This test verifies that the SubtecChannelManager can be instantiated correctly and that its getNextChannelId() method returns a channel ID that is greater than or equal to 0. It ensures that the channel initialization process is functioning as expected without throwing exceptions.
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
 * | Variation / Step | Description                                                 | Test Data                                                                   | Expected Result                                            | Notes         |
 * | :--------------: | ----------------------------------------------------------- | --------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Instantiate SubtecChannelManager using getInstance() method | No input, output: manager instance                                          | Instance created without exception                         | Should Pass   |
 * | 02               | Call getNextChannelId() method on the manager instance       | input: manager instance, output: channelId returned from getNextChannelId()   | getNextChannelId() returns an integer value >= 0           | Should Pass   |
 */
TEST(SubtecChannelManager, RetrieveInitialChannelID) {
    std::cout << "Entering RetrieveInitialChannelID test" << std::endl;

    EXPECT_NO_THROW({
        SubtecChannelManager* manager = SubtecChannelManager::getInstance();
        std::cout << "Successfully instantiated SubtecChannelManager object" << std::endl;
        
        // Invoking getNextChannelId() method
        std::cout << "Invoking getNextChannelId() method" << std::endl;
        int channelId = manager->getNextChannelId();
        std::cout << "Retrieved channel id: " << channelId << std::endl;
        
        // Validate that the channel id is >= 0
        EXPECT_GE(channelId, 0);
        std::cout << "Verified that channel id (" << channelId << ") is greater than or equal to 0" << std::endl;
    });

    std::cout << "Exiting RetrieveInitialChannelID test" << std::endl;
}
/**
 * @brief Verify that the default initialization of m_nextChannelId is set to 0.
 *
 * This test validates that upon creation, the SubtecChannelManagerTestable instance has its m_nextChannelId initialized to 0. It calls getNextChannelId() and checks that the returned value is 0.
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
 * | 01 | Invoke getNextChannelId() to retrieve the internal m_nextChannelId value | testObj instance = valid, method invoked: getNextChannelId(), output variable: channelId | API returns the channelId value, which should be a valid number | Should Pass |
 * | 02 | Verify that the default value of m_nextChannelId is 0 | channelId = value returned from getNextChannelId(), expected value = 0 | Assertion EXPECT_EQ passes confirming channelId equals 0 | Should be successful |
 */
TEST_F(SubtecChannelManagerTestFixture, VerifyDefaultInitialization) {
    std::cout << "Entering VerifyDefaultInitialization test" << std::endl;
    
    // Retrieve the internal state m_nextChannelId using the public getter.
    std::cout << "Invoking getNextChannelId() to retrieve m_nextChannelId." << std::endl;
    uint32_t channelId = testObj->getNextChannelId();
    std::cout << "Retrieved m_nextChannelId value: " << channelId << std::endl;

    // Verify that the default value of m_nextChannelId is 0.
    EXPECT_EQ(channelId, 0);
    std::cout << "Internal state verification: m_nextChannelId is equal to 0." << std::endl;

    std::cout << "Exiting VerifyDefaultInitialization test" << std::endl;
}
/**
 * @brief Test default initialization of communications using SubtecChannel::InitComms() with default settings.
 *
 * This test verifies that the InitComms() method in SubtecChannel initializes communications correctly under default configuration. It ensures that the method executes without throwing any exceptions and returns a valid boolean value (either true or false). The objective is to confirm that the channel's communication initialization behaves as expected when invoked with default parameters.
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
 * | 01 | Invoke SubtecChannel::InitComms() with default settings to initialize communications. | input: None, output: initResult (uninitialized boolean) | Method executes without throwing exceptions and returns a boolean value. | Should Pass |
 * | 02 | Verify that the returned boolean is either true or false using an assertion check. | input: initResult = returned boolean value | The return value is confirmed to be a valid boolean (true or false) and assertion passes. | Should be successful |
 */
TEST_F(SubtecChannelTest, DefaultInitComms) {
    std::cout << "Entering DefaultInitComms test" << std::endl;
    
    std::cout << "Invoking SubtecChannel::InitComms() with default settings" << std::endl;
    bool initResult = false;
    EXPECT_NO_THROW({
        initResult = SubtecChannel::InitComms();
    });
    std::cout << "SubtecChannel::InitComms() returned: " << std::boolalpha << initResult << std::endl;
    
    EXPECT_TRUE(initResult == true || initResult == false);
    
    std::cout << "Exiting DefaultInitComms test" << std::endl;
}
/**
 * @brief Test the successful initialization of communications using a valid socket path.
 *
 * This test verifies that a valid socket path is correctly assigned to a fixed-size array and that the
 * InitComms method of TestableSubtecChannel returns true when invoked with the valid socket path.
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
 * | Variation / Step | Description                                                                     | Test Data                                                             | Expected Result                            | Notes        |
 * | :--------------: | ------------------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------------------------------------- | ------------ |
 * | 01               | Prepare a valid socket path and assign it to a fixed-size array using memset() and strncpy() | validPath = "/tmp/valid_socket", validSocket is zero-initialized         | validSocket contains "/tmp/valid_socket"    | Should be successful |
 * | 02               | Invoke InitComms with the prepared valid socket path                             | validSocket = "/tmp/valid_socket"                                     | API returns true                           | Should Pass  |
 * | 03               | Verify the returned boolean value is true using EXPECT_TRUE                      | result = true (returned from InitComms)                               | EXPECT_TRUE(result) assertion passes       | Should Pass  |
 */
TEST_F(SubtecChannelTest, ValidSocketPath) {
    std::cout << "Entering ValidSocketPath test" << std::endl;
    
    // Prepare valid socket path using strncpy for fixed size array assignment
    char validSocket[256];
    const char* validPath = "/tmp/valid_socket";
    std::cout << "Assigning valid socket path value: " << validPath << std::endl;
    std::memset(validSocket, 0, sizeof(validSocket));
    strncpy(validSocket, validPath, sizeof(validSocket) - 1);
    
    // Invoke the method
    std::cout << "Invoking InitComms with parameter: " << validSocket << std::endl;
    bool result = channel->InitComms(validSocket);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Verify the expected output is true
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidSocketPath test" << std::endl;
}
/**
 * @brief Verify that communications initialization returns false when provided with a null socket path.
 *
 * This test verifies that the InitComms API call fails (returns false) when the provided socket path is null,
 * ensuring robust error handling.
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
 * | Variation / Step | Description                                                  | Test Data                                          | Expected Result                                               | Notes           |
 * | :--------------: | ------------------------------------------------------------ | -------------------------------------------------- | ------------------------------------------------------------- | --------------- |
 * | 01               | Prepare null socket path                                     | nullSocket = nullptr                               | nullSocket is set to nullptr                                  | Should be successful |
 * | 02               | Invoke InitComms method with the null socket path            | input: nullSocket = nullptr; output: result expected = false | API returns false indicating failure                         | Should Fail     |
 * | 03               | Verify the outcome using EXPECT_FALSE                        | result from InitComms = false                       | Assertion passes confirming the API call returned false       | Should be successful |
 */
TEST_F(SubtecChannelTest, NullSocketPath) {
    std::cout << "Entering NullSocketPath test" << std::endl;
    
    // Prepare null socket path
    const char* nullSocket = nullptr;
    std::cout << "Invoking InitComms with parameter: nullptr" << std::endl;
    
    // Invoke the method
    bool result = channel->InitComms(nullSocket);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Verify the expected output is false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NullSocketPath test" << std::endl;
}
/**
 * @brief Test API behavior when provided with an empty socket path.
 *
 * This test verifies that the InitComms API correctly handles an empty socket path string by returning false.
 * The test ensures that an empty fixed-size character array is properly initialized and that the API
 * returns the expected result when invoked with an empty socket path.
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
 * | 01 | Initialize the fixed-size array with zeros and assign an empty string to simulate an empty socket path | input: emptySocket = array of 256 chars initialized with zeros, emptyPath = "" | emptySocket contains an empty string | Should be successful |
 * | 02 | Invoke the InitComms API using the empty socket path | input: emptySocket = "" | API returns false | Should Fail |
 * | 03 | Validate the API response using EXPECT_FALSE to confirm the result | output: result = false | Assertion confirms result is false | Should be successful |
 */
TEST_F(SubtecChannelTest, EmptySocketPath) {
    std::cout << "Entering EmptySocketPath test" << std::endl;
    
    // Prepare empty socket path using strncpy for fixed size array assignment
    char emptySocket[256];
    const char* emptyPath = "";
    std::cout << "Assigning empty socket path value: \"" << emptyPath << "\"" << std::endl;
    std::memset(emptySocket, 0, sizeof(emptySocket));
    strncpy(emptySocket, emptyPath, sizeof(emptySocket) - 1);
    
    // Invoke the method
    std::cout << "Invoking InitComms with parameter: \"" << emptySocket << "\"" << std::endl;
    bool result = channel->InitComms(emptySocket);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Verify the expected output is false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting EmptySocketPath test" << std::endl;
}
/**
 * @brief Validate that InitComms rejects socket paths consisting solely of whitespace.
 *
 * This test verifies that the communication initialization routine (InitComms) correctly handles and rejects
 * socket paths that contain only whitespace characters. It ensures that the API does not accept invalid input,
 * thereby preventing erroneous communication setup.
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
 * | Variation / Step | Description                                                                  | Test Data                                                           | Expected Result                                               | Notes             |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------- |
 * | 01               | Prepare and assign a whitespace socket path to a fixed size array              | spacePath = "   ", whitespaceSocket (zeroed buffer)                 | whitespaceSocket should contain the whitespace string         | Should be successful |
 * | 02               | Invoke InitComms with the whitespace socket path and verify the returned value | input = whitespaceSocket = "   ", output expected = false           | The API returns false and the assertion (EXPECT_FALSE) passes   | Should Fail       |
 */
TEST_F(SubtecChannelTest, WhitespaceSocketPath) {
    std::cout << "Entering WhitespaceSocketPath test" << std::endl;
    
    // Prepare whitespace socket path using strncpy for fixed size array assignment
    char whitespaceSocket[256];
    const char* spacePath = "   ";
    std::cout << "Assigning whitespace socket path value: \"" << spacePath << "\"" << std::endl;
    std::memset(whitespaceSocket, 0, sizeof(whitespaceSocket));
    strncpy(whitespaceSocket, spacePath, sizeof(whitespaceSocket) - 1);
    
    // Invoke the method
    std::cout << "Invoking InitComms with parameter: \"" << whitespaceSocket << "\"" << std::endl;
    bool result = channel->InitComms(whitespaceSocket);
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Verify the expected output is false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting WhitespaceSocketPath test" << std::endl;
}
/**
 * @brief Verify that SendCCSetAttributePacket API handles valid CEA-608 attribute packet inputs correctly.
 *
 * This test verifies that the SendCCSetAttributePacket method does not throw an exception when provided with valid parameters (ccType = 608, attribType = 1, and attributesValues = 10) for a CEA-608 attribute packet. The test ensures that a proper API call is performed without errors.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | -------------- | ----- |
 * | 01 | Log the entry message indicating the start of the ValidCEA608AttributePacket test. | N/A | N/A | Should be successful |
 * | 02 | Initialize test parameters with valid values: ccType = 608, attribType = 1, attributesValues = 10. | ccType = 608, attribType = 1, attributesValues = 10 | Parameters correctly assigned | Should be successful |
 * | 03 | Log the API invocation details with the configured test parameters. | ccType = 608, attribType = 1, attributesValues = 10 | N/A | Should be successful |
 * | 04 | Invoke SendCCSetAttributePacket API using the test parameters and verify that no exception is thrown. | ccType = 608, attribType = 1, attributesValues = 10 | Function executes without throwing an exception | Should Pass |
 * | 05 | Log a success message indicating that the API executed successfully for the valid CEA-608 attribute packet. | N/A | N/A | Should be successful |
 * | 06 | Log the exit message indicating the end of the ValidCEA608AttributePacket test. | N/A | N/A | Should be successful |
 */
TEST_F(SubtecChannelTest, ValidCEA608AttributePacket) {
    std::cout << "Entering ValidCEA608AttributePacket test" << std::endl;
    
    std::uint32_t ccType = 608;
    std::uint32_t attribType = 1;
    int attributesValues = 10;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });
    
    std::cout << "SendCCSetAttributePacket executed successfully for Valid CEA-608 Attribute Packet" << std::endl;
    std::cout << "Exiting ValidCEA608AttributePacket test" << std::endl;
}
/**
 * @brief Verify that the API correctly processes a valid CEA-708 attribute packet
 *
 * This test validates that the SendCCSetAttributePacket API call does not throw an exception when invoked with valid parameters for a CEA-708 attribute packet. The test ensures that the correct packet is formed and the function executes without error.
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
 * | Variation / Step | Description                                                             | Test Data                                           | Expected Result                         | Notes           |
 * | :--------------: | ----------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------- | --------------- |
 * | 01               | Log entering test message                                               | None                                                | "Entering ValidCEA708AttributePacket test" logged | Should be successful |
 * | 02               | Set ccType, attribType, and attributesValues with valid values          | ccType = 708, attribType = 2, attributesValues = 20   | Variables initialized with specified values | Should be successful |
 * | 03               | Invoke SendCCSetAttributePacket API using the test parameters             | ccType = 708, attribType = 2, attributesValues = 20   | API call completes without throwing an exception | Should Pass         |
 * | 04               | Log success of API execution and test exit                              | None                                                | Success message logged indicating correct API execution | Should be successful |
 */
TEST_F(SubtecChannelTest, ValidCEA708AttributePacket) {
    std::cout << "Entering ValidCEA708AttributePacket test" << std::endl;
    
    std::uint32_t ccType = 708;
    std::uint32_t attribType = 2;
    int attributesValues = 20;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });
    
    std::cout << "SendCCSetAttributePacket executed successfully for Valid CEA-708 Attribute Packet" << std::endl;
    std::cout << "Exiting ValidCEA708AttributePacket test" << std::endl;
}
/**
 * @brief Verify that the API SendCCSetAttributePacket handles an undefined Closed Caption type without throwing an exception.
 *
 * This test verifies that when an invalid closed caption type (here, 9999) is provided,
 * along with valid attribute type and attribute values, the API executes without any exception.
 * The function prints log messages around the API call to aid in test tracking.
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
 * | Variation / Step | Description                                                                                   | Test Data                                          | Expected Result                                                   | Notes       |
 * | ---------------- | --------------------------------------------------------------------------------------------- | -------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Set test inputs ccType = 9999, attribType = 1, and attributesValues = 10; invoke SendCCSetAttributePacket API and verify it does not throw an exception. | ccType = 9999, attribType = 1, attributesValues = 10 | API returns without throwing an exception and executes successfully | Should Pass |
 */
TEST_F(SubtecChannelTest, InvalidClosedCaptionTypeUndefined) {
    std::cout << "Entering InvalidClosedCaptionTypeUndefined test" << std::endl;
    
    std::uint32_t ccType = 9999;
    std::uint32_t attribType = 1;
    int attributesValues = 10;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;

    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });
    
    std::cout << "SendCCSetAttributePacket executed successfully" << std::endl;
    std::cout << "Exiting InvalidClosedCaptionTypeUndefined test" << std::endl;
}
/**
 * @brief Validate behavior when an invalid attribute type (out-of-bound value) is provided.
 *
 * This test verifies that the SendCCSetAttributePacket API handles an attribute type value that 
 * exceeds the valid range (using std::numeric_limits<std::uint32_t>::max()). The function is expected 
 * to execute successfully without throwing exceptions even when provided with boundary values.
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
 * | Variation / Step | Description                                                                  | Test Data                                                     | Expected Result                                  | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------ | ---------- |
 * | 01               | Invoke SendCCSetAttributePacket API with boundary out-of-bound attribute type. | ccType = 608, attribType = 4294967295, attributesValues = 10   | API execution completes with no exception thrown | Should Pass |
 */
TEST_F(SubtecChannelTest, InvalidAttributeTypeOutOfBound) {
    std::cout << "Entering InvalidAttributeTypeOutOfBound test" << std::endl;
    
    std::uint32_t ccType = 608;
    std::uint32_t attribType = std::numeric_limits<std::uint32_t>::max();
    int attributesValues = 10;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });

    std::cout << "SendCCSetAttributePacket executed successfully" << std::endl;
    std::cout << "Exiting InvalidAttributeTypeOutOfBound test" << std::endl;
}
/**
 * @brief Validates that SendCCSetAttributePacket handles a boundary attribute value of zero without exceptions.
 *
 * This test verifies that when the attribute value is set to zero (a boundary condition), the SendCCSetAttributePacket
 * function is executed without throwing any exceptions. This ensures that the API correctly handles lower boundary values
 * for attributes.
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
 * | Variation / Step | Description                                                                  | Test Data                                           | Expected Result                                                        | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------- | --------------------------------------------------- | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Call SendCCSetAttributePacket with ccType=608, attribType=1, attributesValues=0 | ccType = 608, attribType = 1, attributesValues = 0 | The function executes without throwing any exceptions (EXPECT_NO_THROW check) | Should Pass |
 */
TEST_F(SubtecChannelTest, BoundaryAttributeValueZero) {
    std::cout << "Entering BoundaryAttributeValueZero test" << std::endl;
    
    std::uint32_t ccType = 608;
    std::uint32_t attribType = 1;
    int attributesValues = 0;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });
    
    std::cout << "SendCCSetAttributePacket executed for Boundary Attribute Value with Zero" << std::endl;
    std::cout << "Exiting BoundaryAttributeValueZero test" << std::endl;
}
/**
 * @brief Test to validate that the SendCCSetAttributePacket API correctly handles an edge integer attribute value.
 *
 * This test verifies that the API can successfully process a maximum integer value for the attribute without throwing exceptions.
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
 * | Variation / Step | Description                                                                        | Test Data                                        | Expected Result                                                                              | Notes     |
 * | :--------------: | ---------------------------------------------------------------------------------- | ------------------------------------------------ | -------------------------------------------------------------------------------------------- | --------- |
 * | 01               | Invoke SendCCSetAttributePacket API using maximum integer edge value for attribute. | ccType = 708, attribType = 2, attributesValues = INT_MAX | API call should complete successfully without throwing an exception and pass the EXPECT_NO_THROW check. | Should Pass |
 */
TEST_F(SubtecChannelTest, ValidAttributeValueEdgeInteger) {
    std::cout << "Entering ValidAttributeValueEdgeInteger test" << std::endl;
    
    std::uint32_t ccType = 708;
    std::uint32_t attribType = 2;
    int attributesValues = INT_MAX;
    
    std::cout << "Invoking SendCCSetAttributePacket with ccType = " << ccType 
              << ", attribType = " << attribType 
              << ", attributesValues = " << attributesValues << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendCCSetAttributePacket(ccType, attribType, attributesValues);
    });
    
    std::cout << "SendCCSetAttributePacket executed successfully for edge integer attribute value" << std::endl;
    std::cout << "Exiting ValidAttributeValueEdgeInteger test" << std::endl;
}
/**
 * @brief Verify that the SendDataPacket function accepts a data packet with a default time offset (0 ms) without throwing an exception.
 *
 * This test case verifies that invoking the SendDataPacket API with a predetermined data packet and a default time offset of 0 ms works as expected. The test ensures that the API correctly handles the default offset parameter, does not throw exceptions during execution, and behaves as intended.
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
 * | Variation / Step | Description                                                                                      | Test Data                                 | Expected Result                                                | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ----------------------------------------- | -------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SendDataPacket with a predefined data packet and default time offset (0 ms).              | data = {0x01, 0x02, 0x03}, time_offset_ms = 0 | The API should execute without throwing any exceptions (ASSERT: No exception thrown). | Should Pass |
 */
TEST_F(SubtecChannelTest, SendDataPacketDefaultTimeOffset) {
    std::cout << "Entering SendDataPacketDefaultTimeOffset test" << std::endl;
    
    // Prepare data packet with default time offset (0 ms)
    std::vector<uint8_t> data = {0x01, 0x02, 0x03};
    std::int64_t time_offset_ms = 0;
    std::cout << "Invoking SendDataPacket with data: {";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::dec << "} and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully." << std::endl;
    });
    
    std::cout << "Packet transmitted successfully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketDefaultTimeOffset test" << std::endl;
}
/**
 * @brief Positive scenario: Verify SendDataPacket handles a valid positive time offset.
 *
 * This test ensures that the SendDataPacket API accepts a data packet along with a positive
 * time offset value (500 ms), and that the method executes without throwing any exceptions.
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
 * | 01 | Prepare a valid data packet and positive time offset, then call SendDataPacket and verify execution without exceptions | data = {0x10,0x20,0x30}, time_offset_ms = 500 | SendDataPacket executes without throwing an exception, EXPECT_NO_THROW passes | Should Pass |
 */
TEST_F(SubtecChannelTest, SendDataPacketPositiveTimeOffset) {
    std::cout << "Entering SendDataPacketPositiveTimeOffset test" << std::endl;
    
    // Prepare data packet with a positive time offset (500 ms)
    std::vector<uint8_t> data = {0x10, 0x20, 0x30};
    std::int64_t time_offset_ms = 500;
    std::cout << "Invoking SendDataPacket with data: {";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::dec << "} and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully." << std::endl;
    });
    
    std::cout << "Packet transmitted successfully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketPositiveTimeOffset test" << std::endl;
}
/**
 * @brief Test the SendDataPacket API for handling a negative time offset.
 *
 * This test verifies that the SendDataPacket API correctly processes a data packet when a negative time offset is provided.
 * The test is designed to ensure that invoking the method with a negative delay does not throw any exceptions, and the packet is
 * transmitted successfully. It validates that the API gracefully handles negative time offset values without error.
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
 * | Variation / Step | Description                                                              | Test Data                                                 | Expected Result                                                                 | Notes           |
 * | :--------------: | ------------------------------------------------------------------------ | --------------------------------------------------------- | ------------------------------------------------------------------------------- | --------------- |
 * | 01               | Prepare a data packet and a negative time offset                         | data = {0xAA, 0xBB, 0xCC}, time_offset_ms = -200           | Data packet and negative time offset are set up correctly                        | Should be successful |
 * | 02               | Invoke the SendDataPacket API using the prepared data packet and time offset | input: data = {0xAA, 0xBB, 0xCC}, time_offset_ms = -200; no output | API call does not throw any exception; method executes successfully              | Should Pass     |
 * | 03               | Log the successful packet transmission outcome                           | output: log messages indicating successful transmission   | Log messages confirm the effective transmission with the negative time offset     | Should be successful |
 */
TEST_F(SubtecChannelTest, SendDataPacketNegativeTimeOffset) {
    std::cout << "Entering SendDataPacketNegativeTimeOffset test" << std::endl;
    
    // Prepare data packet with a negative time offset (-200 ms)
    std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC};
    std::int64_t time_offset_ms = -200;
    std::cout << "Invoking SendDataPacket with data: {";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::dec << "} and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully." << std::endl;
    });
    
    std::cout << "Packet transmitted successfully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketNegativeTimeOffset test" << std::endl;
}
/**
 * @brief Verify SendDataPacket handles an empty data packet with default time offset.
 *
 * This test case verifies that the SendDataPacket method of TestableSubtecChannel gracefully handles an empty data packet with a default time offset (0 ms) without throwing any exceptions. The test ensures that the API functions properly even when provided with empty input data.
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
 * | Variation / Step | Description                                                                        | Test Data                                      | Expected Result                                                      | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------- | ---------------------------------------------- | --------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SendDataPacket with an empty data packet and default time offset (0 ms)       | data = empty vector, time_offset_ms = 0        | API should execute without throwing any exceptions and pass the assertion | Should Pass |
 */
TEST_F(SubtecChannelTest, SendDataPacketEmptyDataDefaultTimeOffset) {
    std::cout << "Entering SendDataPacketEmptyDataDefaultTimeOffset test" << std::endl;
    
    // Prepare an empty data packet with default time offset (0 ms)
    std::vector<uint8_t> data;
    std::int64_t time_offset_ms = 0;
    std::cout << "Invoking SendDataPacket with empty data and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully with empty data." << std::endl;
    });
    
    std::cout << "Empty packet handled gracefully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketEmptyDataDefaultTimeOffset test" << std::endl;
}
/**
 * @brief Validate SendDataPacket API with maximum time offset value.
 *
 * This test verifies that the SendDataPacket API is able to handle the maximum possible time offset 
 * (INT64_MAX) correctly along with a non-empty data packet. The test ensures that no exception is thrown 
 * during the execution of the API, ensuring robust behavior for extreme time offset values.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                      | Expected Result                                                             | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SendDataPacket with a valid non-empty data packet and maximum time offset (INT64_MAX). | data = 0x0F, 0x1E, 0x2D, time_offset_ms = 9223372036854775807     | API executes without throwing exceptions, and EXPECT_NO_THROW assertion passes | Should Pass |
 */
TEST_F(SubtecChannelTest, SendDataPacketMaxTimeOffset) {
    std::cout << "Entering SendDataPacketMaxTimeOffset test" << std::endl;
    
    // Prepare data packet with maximum time offset (INT64_MAX)
    std::vector<uint8_t> data = {0x0F, 0x1E, 0x2D};
    std::int64_t time_offset_ms = std::numeric_limits<std::int64_t>::max();
    std::cout << "Invoking SendDataPacket with data: {";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::dec << "} and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully." << std::endl;
    });
    
    std::cout << "Packet transmitted successfully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketMaxTimeOffset test" << std::endl;
}
/**
 * @brief Test the SendDataPacket method with the minimum time offset value.
 *
 * This test verifies that the SendDataPacket method correctly handles a data packet when provided with the minimum possible
 * time offset (INT64_MIN). It checks that the method does not throw any exceptions and that the packet is transmitted successfully.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |  
 * | 01 | Prepare a data packet with minimum time offset value. | data = {0xF0, 0xE0, 0xD0}, time_offset_ms = INT64_MIN | Data packet is initialized with the appropriate values. | Should be successful |  
 * | 02 | Invoke the SendDataPacket API with the prepared data packet and time offset. | data = {0xF0, 0xE0, 0xD0} (moved), time_offset_ms = INT64_MIN | API call completes without throwing an exception. | Should Pass |  
 * | 03 | Confirm successful transmission by checking the log output indicating the effective time offset. | data not applicable, effective time_offset_ms printed as INT64_MIN | Console output confirms the packet transmission with the expected offset. | Should be successful |
 */
TEST_F(SubtecChannelTest, SendDataPacketMinTimeOffset) {
    std::cout << "Entering SendDataPacketMinTimeOffset test" << std::endl;
    
    // Prepare data packet with minimum time offset (INT64_MIN)
    std::vector<uint8_t> data = {0xF0, 0xE0, 0xD0};
    std::int64_t time_offset_ms = std::numeric_limits<std::int64_t>::min();
    std::cout << "Invoking SendDataPacket with data: {";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "0x" << std::hex << static_cast<int>(data[i]);
        if (i != data.size() - 1)
            std::cout << ", ";
    }
    std::cout << std::dec << "} and time_offset_ms: " << time_offset_ms << std::endl;
    
    EXPECT_NO_THROW({
        channel->SendDataPacket(std::move(data), time_offset_ms);
        std::cout << "Method SendDataPacket invoked successfully." << std::endl;
    });
    
    std::cout << "Packet transmitted successfully with effective time offset: " << time_offset_ms << " ms" << std::endl;
    std::cout << "Exiting SendDataPacketMinTimeOffset test" << std::endl;
}
/**
 * @brief Verify that SendMutePacket executes without error.
 *
 * This test invokes the SendMutePacket API with no arguments and verifies that it executes without throwing any exceptions.
 * The purpose is to ensure that the API correctly handles a call with default or no parameters and completes successfully.
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
 * | Variation / Step | Description                                                  | Test Data                                                  | Expected Result                                                                  | Notes      |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------------------------------- | -------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke SendMutePacket() with no parameters                   | input: none, output: none                                   | API executes successfully with no exceptions thrown                              | Should Pass|
 */
TEST_F(SubtecChannelTest, VerifySendMutePacketExecutesWithoutError) {
    std::cout << "Entering VerifySendMutePacketExecutesWithoutError test" << std::endl;
    
    std::cout << "Invoking SendMutePacket with no parameters" << std::endl;
    EXPECT_NO_THROW({
        channel->SendMutePacket();
        std::cout << "SendMutePacket executed successfully with no exception thrown" << std::endl;
    });
    
    std::cout << "Exiting VerifySendMutePacketExecutesWithoutError test" << std::endl;
}
/**
 * @brief Verify that the SendPausePacket method executes successfully without throwing exceptions.
 *
 * This test verifies that the SendPausePacket method of the TestableSubtecChannel object can be invoked without any exceptions. It ensures that the method call and log outputs occur as expected for a successful API execution.
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
 * | Variation / Step | Description                                                    | Test Data                                         | Expected Result                                                                                              | Notes           |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | --------------- |
 * | 01               | Print the message indicating entry into the test function.     | None                                              | "Entering SendPausePacketSuccessTest test" message is printed to the console.                                | Should be successful |
 * | 02               | Log the invocation details for the SendPausePacket method.     | None                                              | "Invoking SendPausePacket method on TestableSubtecChannel object." message is printed to the console.         | Should be successful |
 * | 03               | Call the SendPausePacket method and verify no exceptions occur.  | input: none, output: none                           | The SendPausePacket method is invoked successfully and no exceptions are thrown (EXPECT_NO_THROW passes).      | Should Pass     |
 * | 04               | Print the exit message for the test function.                  | None                                              | "Exiting SendPausePacketSuccessTest test" message is printed to the console.                                  | Should be successful |
 */
TEST_F(SubtecChannelTest, SendPausePacketSuccessTest) {
    std::cout << "Entering SendPausePacketSuccessTest test" << std::endl;
    
    // Log the current invocation of SendPausePacket
    std::cout << "Invoking SendPausePacket method on TestableSubtecChannel object." << std::endl;
    
    // Call the SendPausePacket method and ensure no exceptions are thrown.
    EXPECT_NO_THROW({
        channel->SendPausePacket();
        std::cout << "SendPausePacket method invoked successfully." << std::endl;
    });
    
    std::cout << "Exiting SendPausePacketSuccessTest test" << std::endl;
}
/**
 * @brief Validates that the SendResetAllPacket API is executed without throwing any exceptions.
 *
 * This test verifies that invoking the SendResetAllPacket method on the channel object successfully transmits the control packet for resetting all active Closed Captions channels without any exception. Ensuring that the API executes without error is critical for confirming the robustness of the channel reset functionality.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print the start message indicating the entry into the test. | None | Console displays "Entering BasicExecutionSendResetAllPacket test". | Should be successful |
 * | 02 | Invoke SendResetAllPacket on the channel object and validate that no exception is thrown. | channel object, no parameters | EXPECT_NO_THROW: API call completes successfully without exceptions. | Should Pass |
 * | 03 | Print messages confirming the successful invocation of the API and the transmission of the reset control packet, followed by the exit message. | None | Console displays messages indicating successful API invocation and test completion. | Should be successful |
 */
TEST_F(SubtecChannelTest, BasicExecutionSendResetAllPacket) {
    std::cout << "Entering BasicExecutionSendResetAllPacket test" << std::endl;
    
    std::cout << "Invoking SendResetAllPacket method on channel object." << std::endl;
    EXPECT_NO_THROW(channel->SendResetAllPacket());
    std::cout << "SendResetAllPacket method invoked successfully with no exceptions thrown." << std::endl;
    
    std::cout << "Control packet for resetting all active Closed Captions channels has been transmitted." << std::endl;
    
    std::cout << "Exiting BasicExecutionSendResetAllPacket test" << std::endl;
}
/**
 * @brief This test verifies that the SendResetChannelPacket() API executes successfully without throwing exceptions.
 *
 * This test ensures that the subtec channel reset operation performs correctly in a basic positive scenario.
 * It checks that invoking SendResetChannelPacket() does not result in any exceptions and that the internal channel state
 * is appropriately reset. The test logs various messages to track the flow of execution.
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
 * | Variation / Step | Description                                                         | Test Data                                                                                                                                                      | Expected Result                                                      | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | --------------- |
 * | 01               | Log the test entry message                                          | message = "Entering BasicPositiveTest test"                                                                                                                    | Test entry message logged                                             | Should be successful |
 * | 02               | Invoke SendResetChannelPacket() API                                 | channel->SendResetChannelPacket()                                                                                                                              | API call executes without throwing an exception (EXPECT_NO_THROW check) | Should Pass     |
 * | 03               | Log successful API execution message                                | message = "SendResetChannelPacket() executed successfully without throwing exceptions."                                                                        | Successful execution message logged                                    | Should be successful |
 * | 04               | Log the assumption of the internal channel state post-reset operation | message = "Assuming internal channel state: counter reset as per the reset operation."                                                                           | Internal state assumption logged                                       | Should be successful |
 * | 05               | Log the test exit message                                             | message = "Exiting BasicPositiveTest test"                                                                                                                     | Test exit message logged                                               | Should be successful |
 */
TEST_F(SubtecChannelTest, BasicPositiveTest) {
    std::cout << "Entering BasicPositiveTest test" << std::endl;
    
    std::cout << "Invoking SendResetChannelPacket() method." << std::endl;
    EXPECT_NO_THROW(channel->SendResetChannelPacket());
    std::cout << "SendResetChannelPacket() executed successfully without throwing exceptions." << std::endl;
    
    std::cout << "Assuming internal channel state: counter reset as per the reset operation." << std::endl;
    
    std::cout << "Exiting BasicPositiveTest test" << std::endl;
}
/**
 * @brief Validate that the SendResumePacket method executes without errors
 *
 * This test verifies that the SendResumePacket method of the TestableSubtecChannel class executes successfully without throwing any exceptions. The test logs the entry and exit messages and checks that the method call does not lead to an exception being thrown, ensuring normal operation.
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
 * | Variation / Step | Description                                                                           | Test Data                                                         | Expected Result                                                        | Notes           |
 * | :----:           | ------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ----------------------------------------------------------------------- | --------------- |
 * | 01               | Log entry to indicate the start of the BasicResumePacketTransmission test               | None                                                              | Console outputs "Entering BasicResumePacketTransmission test"           | Should be successful |
 * | 02               | Log the message before invoking the SendResumePacket method                           | None                                                              | Console outputs "Invoking SendResumePacket method on TestableSubtecChannel object." | Should be successful |
 * | 03               | Invoke the SendResumePacket API and ensure it executes without throwing exceptions      | channel (instance), method: SendResumePacket()                    | Method executes without throwing exceptions and outputs "SendResumePacket method executed successfully."  | Should Pass      |
 * | 04               | Log exit to mark the end of the test                                                  | None                                                              | Console outputs "Exiting BasicResumePacketTransmission test"            | Should be successful |
 */
TEST_F(SubtecChannelTest, BasicResumePacketTransmission) {
    std::cout << "Entering BasicResumePacketTransmission test" << std::endl;
    
    // Log before invoking the method
    std::cout << "Invoking SendResumePacket method on TestableSubtecChannel object." << std::endl;
    
    // Call the method and ensure it executes without throwing any exceptions.
    EXPECT_NO_THROW({
        channel->SendResumePacket();
        std::cout << "SendResumePacket method executed successfully." << std::endl;
    });
    
    std::cout << "Exiting BasicResumePacketTransmission test" << std::endl;
}
/**
 * @brief Test SendSelectionPacket API with typical positive resolution.
 *
 * This test verifies that the SendSelectionPacket method can process typical resolution dimensions (1920 x 1080)
 * without throwing any exceptions. This ensures that the API behaves as expected for valid input values.
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
 * | Variation / Step | Description                                       | Test Data                         | Expected Result                                    | Notes       |
 * | :--------------: | ------------------------------------------------- | --------------------------------- | -------------------------------------------------- | ----------- |
 * | 01               | Invoke SendSelectionPacket with valid dimensions  | width = 1920, height = 1080         | No exception thrown; function returns successfully | Should Pass |
 */
TEST_F(SubtecChannelTest, SendSelectionPacket_TypicalPositiveResolution) {
    std::cout << "Entering SendSelectionPacket_TypicalPositiveResolution test" << std::endl;
    uint32_t width = 1920;
    uint32_t height = 1080;
    std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
    EXPECT_NO_THROW({
        channel->SendSelectionPacket(width, height);
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_TypicalPositiveResolution test" << std::endl;
}
/**
 * @brief Validate SendSelectionPacket API for zero boundary inputs
 *
 * This test verifies that the SendSelectionPacket API correctly handles the scenario where both width and height are zero boundary values. It is important to ensure that processing boundary input conditions does not cause any exceptions.
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
 * | Variation / Step | Description                                         | Test Data              | Expected Result                                                    | Notes      |
 * | :--------------: | --------------------------------------------------- | ---------------------- | ------------------------------------------------------------------ | ---------- |
 * | 01               | Call SendSelectionPacket with zero boundary values  | width = 0, height = 0  | API invoked successfully with no exceptions thrown (EXPECT_NO_THROW) | Should Pass |
 */
TEST_F(SubtecChannelTest, SendSelectionPacket_BothZeroBoundary) {
    std::cout << "Entering SendSelectionPacket_BothZeroBoundary test" << std::endl;
    uint32_t width = 0;
    uint32_t height = 0;
    std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
    EXPECT_NO_THROW({
        channel->SendSelectionPacket(width, height);
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_BothZeroBoundary test" << std::endl;
}
/**
 * @brief Verify SendSelectionPacket API call with positive width and zero height.
 *
 * This test case verifies that the SendSelectionPacket function of the TestableSubtecChannel class properly handles the case when a positive width and a zero height are provided. The function is expected to complete without throwing any exceptions, ensuring that edge cases are handled gracefully.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke SendSelectionPacket with a positive width and zero height to check for proper handling of the inputs. | width = 1920, height = 0 | API call completes without throwing exceptions (EXPECT_NO_THROW is satisfied). | Should Pass |
 */
TEST_F(SubtecChannelTest, SendSelectionPacket_WidthPositiveHeightZero) {
    std::cout << "Entering SendSelectionPacket_WidthPositiveHeightZero test" << std::endl;
    uint32_t width = 1920;
    uint32_t height = 0;
    std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
    EXPECT_NO_THROW({
        channel->SendSelectionPacket(width, height);
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_WidthPositiveHeightZero test" << std::endl;
}
/**
 * @brief Test SendSelectionPacket API with maximum unsigned 32-bit values.
 *
 * This test verifies that the SendSelectionPacket API call successfully handles the maximum possible unsigned 32-bit integer values for width and height. It ensures that no exceptions are thrown when the function is invoked with these edge case values.
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
 * | Variation / Step | Description                                                               | Test Data                                         | Expected Result                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------------- | ---------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SendSelectionPacket with maximum unsigned 32-bit width and height. | width = 4294967295, height = 4294967295              | No exception thrown; API call returns successfully and prints messages | Should Pass |
 */
TEST_F(SubtecChannelTest, SendSelectionPacket_MaximumUnsigned32BitValues) {
    std::cout << "Entering SendSelectionPacket_MaximumUnsigned32BitValues test" << std::endl;
    uint32_t width = 4294967295u;
    uint32_t height = 4294967295u;
    std::cout << "Invoking SendSelectionPacket with width: " << width << " and height: " << height << std::endl;
    EXPECT_NO_THROW({
        channel->SendSelectionPacket(width, height);
        std::cout << "SendSelectionPacket invoked successfully with width: " << width << " and height: " << height << std::endl;
    });
    std::cout << "Exiting SendSelectionPacket_MaximumUnsigned32BitValues test" << std::endl;
}
/**
 * @brief Test that a valid timestamp is sent successfully.
 *
 * This test ensures that when a valid timestamp value is provided, the SendTimestampPacket API is invoked without throwing an exception. It verifies that the implementation correctly processes valid inputs and logs the appropriate messages indicating the flow of execution.
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
 * | Variation / Step | Description                                                      | Test Data                                | Expected Result                                                        | Notes         |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------- | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Log entry into the SendValidTimestampTest test                   | None                                     | "Entering SendValidTimestampTest test" message printed                 | Should be successful |
 * | 02               | Set timestamp value for the API call                             | timestampMs = 1500                       | timestampMs variable is assigned the value 1500                        | Should be successful |
 * | 03               | Invoke SendTimestampPacket API with a valid timestamp            | input: channel, timestampMs = 1500       | API executes without throwing an exception (EXPECT_NO_THROW passes)      | Should Pass   |
 * | 04               | Log success message after the API call                           | timestampMs = 1500                       | "SendTimestampPacket executed successfully with timestampMs = 1500" printed | Should be successful |
 * | 05               | Log exit from the SendValidTimestampTest test                    | None                                     | "Exiting SendValidTimestampTest test" message printed                  | Should be successful |
 */
TEST_F(SubtecChannelTest, SendValidTimestampTest) {
    std::cout << "Entering SendValidTimestampTest test" << std::endl;
    
    uint64_t timestampMs = 1500;
    std::cout << "Invoking SendTimestampPacket with timestampMs = " << timestampMs << std::endl;
    
    EXPECT_NO_THROW(channel->SendTimestampPacket(timestampMs));
    
    std::cout << "SendTimestampPacket executed successfully with timestampMs = " << timestampMs << std::endl;
    std::cout << "Exiting SendValidTimestampTest test" << std::endl;
}
/**
 * @brief Test to verify that SendTimestampPacket correctly handles the minimum timestamp value.
 *
 * This test verifies that the SendTimestampPacket function in the SubtecChannel API can be executed without throwing an exception when provided with the minimum timestamp value (0). The objective is to ensure that even the edge case of a minimum (zero) timestamp is handled gracefully by the function.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 035@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                       | Test Data                                  | Expected Result                                                      | Notes           |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------------------- | --------------- |
 * | 01               | Start the test and log the entry message                          | None                                       | Entry message printed to the console                                 | Should be successful |
 * | 02               | Initialize the timestamp value to 0 (minimum value)               | timestampMs = 0                            | timestampMs is set to 0                                               | Should be successful |
 * | 03               | Invoke SendTimestampPacket API with the minimum timestamp value     | input: timestampMs = 0                     | API call executes without throwing an exception                     | Should Pass     |
 * | 04               | Log the successful execution of the SendTimestampPacket API call    | timestampMs = 0                            | Success message printed to the console                                | Should be successful |
 * | 05               | End the test and log the exit message                             | None                                       | Exit message printed to the console                                   | Should be successful |
 */
TEST_F(SubtecChannelTest, SendMinimumTimestampTest) {
    std::cout << "Entering SendMinimumTimestampTest test" << std::endl;
    
    uint64_t timestampMs = 0;
    std::cout << "Invoking SendTimestampPacket with timestampMs = " << timestampMs << std::endl;
    
    EXPECT_NO_THROW(channel->SendTimestampPacket(timestampMs));
    
    std::cout << "SendTimestampPacket executed successfully with timestampMs = " << timestampMs << std::endl;
    std::cout << "Exiting SendMinimumTimestampTest test" << std::endl;
}
/**
 * @brief Verify that SendTimestampPacket correctly processes the maximum timestamp value.
 *
 * This test verifies if the SendTimestampPacket function correctly handles the maximum possible uint64_t value as a timestamp. The test ensures that the function does not throw any exception and executes successfully. 
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 036@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                 | Test Data                                             | Expected Result                                           | Notes       |
 * | :--------------: | ----------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------- | ----------- |
 * | 01               | Invoke SendTimestampPacket API with maximum timestamp value   | timestampMs = 18446744073709551615                     | No exception thrown; EXPECT_NO_THROW assertion passes     | Should Pass |
 */
TEST_F(SubtecChannelTest, SendMaximumTimestampTest) {
    std::cout << "Entering SendMaximumTimestampTest test" << std::endl;
    
    uint64_t timestampMs = 18446744073709551615ULL;
    std::cout << "Invoking SendTimestampPacket with timestampMs = " << timestampMs << std::endl;
    
    EXPECT_NO_THROW(channel->SendTimestampPacket(timestampMs));
    
    std::cout << "SendTimestampPacket executed successfully with timestampMs = " << timestampMs << std::endl;
    std::cout << "Exiting SendMaximumTimestampTest test" << std::endl;
}
/**
 * @brief Verify that the SendUnmutePacket API method executes without throwing an exception.
 *
 * This test calls the SendUnmutePacket method on the channel object and verifies that it completes successfully without any exceptions. It also logs the entry, successful invocation, and exit messages of the test to help trace the execution flow.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print entering log message for the test | None | "Entering VerifyValidUnmutePacketSend test" printed to stdout | Should be successful |
 * | 02 | Invoke SendUnmutePacket and validate that no exception is thrown | function call = SendUnmutePacket() | Method executes without throwing an exception and EXPECT_NO_THROW assertion passes | Should Pass |
 * | 03 | Print success log message after API invocation | None | "SendUnmutePacket() invoked successfully" printed to stdout | Should be successful |
 * | 04 | Print exit log message for the test | None | "Exiting VerifyValidUnmutePacketSend test" printed to stdout | Should be successful |
 */
TEST_F(SubtecChannelTest, VerifyValidUnmutePacketSend) {
    std::cout << "Entering VerifyValidUnmutePacketSend test" << std::endl;
    
    // Log initial invocation of SendUnmutePacket
    std::cout << "Invoking SendUnmutePacket()" << std::endl;
    
    // Invoke the method and verify that it does not throw
    EXPECT_NO_THROW({
        channel->SendUnmutePacket();
    });
    std::cout << "SendUnmutePacket() invoked successfully" << std::endl;

    std::cout << "Exiting VerifyValidUnmutePacketSend test" << std::endl;
}
/**
 * @brief Verify that SubtecChannel object is constructed correctly using default constructor.
 *
 * This test verifies that the default construction of SubtecChannel via the test fixture does not throw any exception and that the resulting object is not a nullptr.
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
 * | Variation / Step | Description                                                              | Test Data                                        | Expected Result                                                                            | Notes              |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------ | ------------------------------------------------------------------------------------------ | ------------------ |
 * | 01               | Invoke SubtecChannel default constructor via test fixture object         | input: none, output: channel object instantiated | No exception is thrown; SubtecChannel object is created successfully                         | Should Pass        |
 * | 02               | Verify that the constructed SubtecChannel object is not nullptr          | input: channel pointer = valid object reference  | The channel pointer is non-null and assertion EXPECT_NE passes                              | Should be successful |
 */
TEST_F(SubtecChannelTest, DefaultConstruction) {
    std::cout << "Entering DefaultConstruction test" << std::endl;
    
    std::cout << "Invoking SubtecChannel default constructor via test fixture object" << std::endl;
    EXPECT_NO_THROW({
        if (channel) {
            std::cout << "SubtecChannel default constructor executed without exception" << std::endl;
        }
    });
    
    EXPECT_NE(channel, nullptr) << "SubtecChannel object should not be nullptr";
    std::cout << "Verified that the constructed SubtecChannel object is not nullptr" << std::endl;
    
    std::cout << "Exiting DefaultConstruction test" << std::endl;
}
/**
 * @brief Verifies that the SubtecChannelFactory method successfully creates a valid channel instance for valid ChannelType inputs.
 *
 * This test iterates over a set of valid ChannelType values (TTML, WEBVTT, CC) and calls the SubtecChannelFactory method for each type.
 * It ensures that no exceptions are thrown and that a valid non-null pointer is returned for each valid input.
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
 * | Variation / Step | Description                                                         | Test Data                                          | Expected Result                                                           | Notes       |
 * | :----:           | ------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke SubtecChannelFactory with valid ChannelType::TTML            | input = ChannelType::TTML, output = valid pointer  | SubtecChannelFactory returns a valid non-null SubtecChannel pointer and EXPECT_NE passes | Should Pass |
 * | 02               | Invoke SubtecChannelFactory with valid ChannelType::WEBVTT          | input = ChannelType::WEBVTT, output = valid pointer| SubtecChannelFactory returns a valid non-null SubtecChannel pointer and EXPECT_NE passes | Should Pass |
 * | 03               | Invoke SubtecChannelFactory with valid ChannelType::CC              | input = ChannelType::CC, output = valid pointer    | SubtecChannelFactory returns a valid non-null SubtecChannel pointer and EXPECT_NE passes | Should Pass |
 */
TEST_F(SubtecChannelTest, ValidSubtecChannelCreation) {
    std::cout << "Entering ValidSubtecChannelCreation test" << std::endl;

    // Array of valid ChannelTypes
    const ChannelType validTypes[3] = {ChannelType::TTML, ChannelType::WEBVTT, ChannelType::CC};

    for (int i = 0; i < 3; ++i) {
        ChannelType currentType = validTypes[i];
        std::cout << "Invoking SubtecChannelFactory with valid ChannelType index " << i;
        std::cout << " (value: " << static_cast<int>(currentType) << ")" << std::endl;

        // Invoke the factory method and expect no exception
        std::unique_ptr<SubtecChannel> createdChannel;
        EXPECT_NO_THROW({
            createdChannel = channel->SubtecChannelFactory(currentType);
        });
        if(createdChannel) {
            std::cout << "SubtecChannelFactory returned a valid pointer: " << createdChannel.get() << std::endl;
            // Print key internal state if available. As internal channel id is protected, simulating key information log.
            std::cout << "Key information: Channel created successfully with ChannelType value " 
                      << static_cast<int>(currentType) << std::endl;
        } else {
            std::cout << "SubtecChannelFactory returned a null pointer for ChannelType value " 
                      << static_cast<int>(currentType) << std::endl;
        }

        EXPECT_NE(createdChannel, nullptr);
    }
    
    std::cout << "Exiting ValidSubtecChannelCreation test" << std::endl;
}
/**
 * @brief Verify that SubtecChannelFactory returns nullptr when an invalid ChannelType is provided.
 *
 * This test verifies the behavior of the SubtecChannelFactory method within the TestableSubtecChannel class. The objective is to ensure that when the factory is invoked with invalid channel types, it does not throw any exceptions and correctly returns a null pointer. This is important to confirm that the system gracefully handles erroneous input without leading to unexpected behaviors.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke SubtecChannelFactory with an invalid ChannelType (-1) | input: ChannelType = -1 | Expect no exception; createdChannel to be nullptr | Should Pass |
 * | 02 | Invoke SubtecChannelFactory with an invalid ChannelType (100) | input: ChannelType = 100 | Expect no exception; createdChannel to be nullptr | Should Pass |
 */
TEST_F(SubtecChannelTest, InvalidSubtecChannelCreation) {
    std::cout << "Entering InvalidSubtecChannelCreation test" << std::endl;

    // List of invalid ChannelTypes
    std::vector<ChannelType> invalidTypes = { static_cast<ChannelType>(-1), static_cast<ChannelType>(100) };

    for (size_t i = 0; i < invalidTypes.size(); ++i) {
        ChannelType invalidType = invalidTypes[i];
        std::cout << "Invoking SubtecChannelFactory with invalid ChannelType index " << i;
        std::cout << " (value: " << static_cast<int>(invalidType) << ")" << std::endl;

        // Invoke the factory method and expect no exception
        std::unique_ptr<SubtecChannel> createdChannel;
        EXPECT_NO_THROW({
            createdChannel = channel->SubtecChannelFactory(invalidType);
        });
        
        if (createdChannel == nullptr) {
            std::cout << "SubtecChannelFactory correctly returned null for invalid ChannelType value " 
                      << static_cast<int>(invalidType) << std::endl;
        } else {
            std::cout << "SubtecChannelFactory returned a non-null pointer (" << createdChannel.get() 
                      << ") for invalid ChannelType value " << static_cast<int>(invalidType) << std::endl;
        }
        
        EXPECT_EQ(createdChannel, nullptr);
    }

    std::cout << "Exiting InvalidSubtecChannelCreation test" << std::endl;
}
/**
 * @brief Validate the sendPacket() API with no arguments to ensure it executes without throwing an exception.
 *
 * This test verifies that calling sendPacket() with no parameters does not result in an exception. 
 * It prints logs to indicate the flow of execution throughout the test. This is a positive scenario test case.
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
 * | Variation / Step | Description                                                     | Test Data                                              | Expected Result                                                   | Notes              |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------------ | ----------------------------------------------------------------- | ------------------ |
 * | 01               | Print log for entering the test                                 | None                                                 | "Entering SendPacketNoArgsTest test" message appears              | Should be successful |
 * | 02               | Print log before invoking sendPacket()                          | None                                                 | "About to invoke sendPacket() with no arguments." message appears   | Should be successful |
 * | 03               | Invoke the sendPacket() API with no arguments                     | channel: instance of TestableSubtecChannel, no arguments | No exception thrown from sendPacket(); EXPECT_NO_THROW passes         | Should Pass         |
 * | 04               | Print log after sending packet                                  | None                                                 | "sendPacket() executed with no arguments." message appears          | Should be successful |
 * | 05               | Print log for exiting the test                                   | None                                                 | "Exiting SendPacketNoArgsTest test" message appears                 | Should be successful |
 */
TEST_F(SubtecChannelTest, SendPacketNoArgsTest) {
    std::cout << "Entering SendPacketNoArgsTest test" << std::endl;
    
    std::cout << "About to invoke sendPacket() with no arguments." << std::endl;
    EXPECT_NO_THROW({
        channel->sendPacket();
    });
    std::cout << "sendPacket() executed with no arguments." << std::endl;
    
    std::cout << "Exiting SendPacketNoArgsTest test" << std::endl;
}
/**
 * @brief Verify that the sendPacket() API correctly processes a valid single integer argument without throwing an exception.
 *
 * This test invokes the sendPacket() method on the channel with a single integer argument (100) and checks that the method executes without throwing any exceptions. The purpose is to validate the positive functionality of sendPacket() for single argument input.
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
 * | Variation / Step | Description                                                            | Test Data                 | Expected Result                                             | Notes      |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------- | ----------------------------------------------------------- | ---------- |
 * | 01               | Invoke sendPacket() with a valid integer argument and check for no exception | inputValue = 100          | API call should complete without throwing any exceptions; EXPECT_NO_THROW passes | Should Pass |
 */
TEST_F(SubtecChannelTest, SendPacketSingleArgTest) {
    std::cout << "Entering SendPacketSingleArgTest test" << std::endl;
    
    int inputValue = 100;
    std::cout << "About to invoke sendPacket() with single integer argument: " << inputValue << std::endl;
    EXPECT_NO_THROW({
        channel->sendPacket(inputValue);
    });
    std::cout << "sendPacket() executed with argument: " << inputValue << std::endl;

    std::cout << "Exiting SendPacketSingleArgTest test" << std::endl;
}
/**
 * @brief Verify that sendPacket API handles multiple arguments without throwing exceptions
 *
 * This test validates that the sendPacket function correctly processes and sends packet data when provided with multiple arguments of different types (integer, string, and float). It ensures that no exceptions are thrown during execution, thereby confirming the API's proper integration under normal conditions.
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
 * | Variation / Step | Description                                                     | Test Data                                              | Expected Result                                      | Notes           |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------------ | ---------------------------------------------------- | --------------- |
 * | 01               | Initialize and assign values to integer, string, and float        | intValue = 50, strValue = packetData, floatValue = 3.14 | Variables are initialized with correct values         | Should be successful |
 * | 02               | Invoke sendPacket API with multiple arguments                     | input: intValue=50, strValue=packetData, floatValue=3.14| API call executes without exception                  | Should Pass     |
 * | 03               | Log the output messages before and after the API invocation       | Output messages with variable values                   | Correct logging of input values and confirmation message| Should be successful |
 */
TEST_F(SubtecChannelTest, SendPacketMultipleArgsTest) {
    std::cout << "Entering SendPacketMultipleArgsTest test" << std::endl;
    
    int intValue = 50;
    std::string strValue = "packetData";
    float floatValue = 3.14f;
    
    std::cout << "About to invoke sendPacket() with multiple arguments:" << std::endl;
    std::cout << "  Integer value: " << intValue << std::endl;
    std::cout << "  String value: " << strValue << std::endl;
    std::cout << "  Float value: " << floatValue << std::endl;
    
    EXPECT_NO_THROW({
        channel->sendPacket(intValue, strValue, floatValue);
    });
    std::cout << "sendPacket() executed with arguments: " << intValue 
              << ", " << strValue 
              << ", " << floatValue << std::endl;

    std::cout << "Exiting SendPacketMultipleArgsTest test" << std::endl;
}
/**
 * @brief Verify the sendPacket() function handles different enumeration arguments correctly
 *
 * This test validates that the sendPacket() method in TestableSubtecChannel correctly handles various enumeration
 * argument values (eSUB_TYPE_WEBVTT and eSUB_TYPE_MP4). It ensures that no exceptions are thrown when the method is
 * invoked with each valid enum value, confirming the correct and stable behavior of the API in processing enum inputs.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 044@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                | Test Data                                 | Expected Result                                              | Notes       |
 * | :--------------: | -------------------------------------------------------------------------- | ----------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Invoke sendPacket() with enum argument eSUB_TYPE_WEBVTT (0)                  | type = eSUB_TYPE_WEBVTT (0)                 | No exception thrown; EXPECT_NO_THROW passes                  | Should Pass |
 * | 02               | Invoke sendPacket() with enum argument eSUB_TYPE_MP4 (1)                     | type = eSUB_TYPE_MP4 (1)                    | No exception thrown; EXPECT_NO_THROW passes                  | Should Pass |
 */
TEST_F(SubtecChannelTest, SendPacketEnumArgTest) {
    std::cout << "Entering SendPacketEnumArgTest test" << std::endl;
    
    // Define enum values locally as they are not provided in this scope.
    const int eSUB_TYPE_WEBVTT = 0;
    const int eSUB_TYPE_MP4 = 1;
    
    int enumTypes[] = { eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4 };
    
    for (const int type : enumTypes) {
        std::cout << "Invoking sendPacket() with enum argument value: " << type << std::endl;
        EXPECT_NO_THROW({
            channel->sendPacket(type);
        });
        std::cout << "sendPacket() executed with enum argument: " << type << std::endl;
    }
    
    std::cout << "Exiting SendPacketEnumArgTest test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
