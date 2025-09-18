
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
* @file test_l1_DrmJsonObject.cpp
* @page DrmJsonObject Level 1 Tests
*
* ## Module's Role
* This module includes Level 1 functional tests (success and failure scenarios).
* This is to ensure that the DrmJsonObject methods meet the requirements.
*
* **Pre-Conditions:**  None @n
* **Dependencies:** None @n
*
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <cstring>
#include "DrmJsonObject.h"



class DrmJsonObjectTests : public ::testing::Test 
{
    protected:
        void SetUp() override 
        {
        }

        void TearDown() override 
        {
        }   
};   


// Test case for the default constructor of DrmJsonObject.
/**
 * @brief Validate that DrmJsonObject's default constructor functions correctly
 *
 * This test verifies that invoking the default constructor of the DrmJsonObject class does not throw any exceptions and successfully creates an instance. The objective is to ensure that basic object instantiation works as expected, providing a stable basis for further functionality tests.
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
 * | Variation / Step | Description                                                      | Test Data                                      | Expected Result                                                                    | Notes      |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------- | ---------------------------------------------------------------------------------- | ---------- |
 * |       01       | Invoke the default constructor of DrmJsonObject.                | No input parameters, output: valid instance    | DrmJsonObject instance is created without throwing exceptions.                   | Should Pass |
 */
TEST_F(DrmJsonObjectTests, DrmJsonObject_DefaultConstructor) {
    std::cout << "Entering DrmJsonObject::DrmJsonObject()_start test" << std::endl;
    std::cout << "Invoking DrmJsonObject default constructor with no input parameters." << std::endl;
    
    // Create an object using the default constructor.
    EXPECT_NO_THROW(
        {
            DrmJsonObject drmJsonObj;
        }
    );
    std::cout << "DrmJsonObject default constructor invoked. DrmJsonObject instance created successfully." << std::endl;
        
    std::cout << "Exiting DrmJsonObject::DrmJsonObject()_start test" << std::endl;
}
/**
 * @brief Verifies that the move constructor correctly transfers JSON data from a non-empty source object.
 *
 * This test ensures that after invoking the move constructor on a valid DrmJsonObject initialized with non-empty JSON data, 
 * the new (moved) object retains the expected JSON content, and the source object becomes empty. 
 * This behavior confirms the proper implementation of move semantics for the DrmJsonObject class.
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
 * | Variation / Step | Description                                                                                       | Test Data                                                                                         | Expected Result                                                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create a valid JSON string and instantiate a source DrmJsonObject using the custom constructor.   | validJson = {"key":"value"}                                                                         | Source object is initialized with the valid JSON data.                                               | Should Pass |
 * | 02               | Call the print() function on the source object to fetch its JSON content before moving.           | API: print() on source, returns sourceJsonBefore                                                  | The source object's JSON content matches the initialized JSON string.                                  | Should Pass |
 * | 03               | Invoke the move constructor using std::move() to create a new DrmJsonObject from the source.      | API: move constructor, input: std::move(source)                                                   | A new object (moved) is created with the JSON data; the source object is left in a valid but unspecified state. | Should Pass |
 * | 04               | Retrieve the JSON content from the moved object by calling print() to validate the move operation. | API: print() on moved, returns movedJson                                                          | The moved object's JSON string contains "\"key\":\"value\"" as a substring.                             | Should Pass |
 * | 05               | Retrieve the JSON content from the source object after the move operation to confirm it's empty.  | API: print() on source after move, returns sourceJsonAfter                                          | The source object's JSON string is empty.                                                             | Should Pass |
 */
TEST_F(DrmJsonObjectTests, MoveConstructorValidNonEmpty) {
    std::cout << "Entering MoveConstructorValidNonEmpty test" << std::endl;

    // Create a valid JSON string and instantiate source object using the custom constructor.
    std::string validJson = "{\"key\":\"value\"}";
    std::cout << "Invoking DrmJsonObject constructor with valid JSON string: " << validJson << std::endl;
    DrmJsonObject source(validJson);

    // Call print() to fetch JSON content from source before moving.
    std::string sourceJsonBefore = source.print();
    std::cout << "Source object JSON before move: " << sourceJsonBefore << std::endl;

    // Invoke move constructor using std::move(source)
    std::cout << "Invoking move constructor: DrmJsonObject moved(std::move(source))" << std::endl;
    DrmJsonObject moved(std::move(source));

    // Retrieve JSON content from the moved object.
    std::string movedJson = moved.print();
    std::cout << "Moved object JSON after move: " << movedJson << std::endl;

    // Check that the moved object contains the expected JSON data.
    EXPECT_NE(movedJson.find("\"key\":\"value\""), std::string::npos);

    // Retrieve and print the state of source after move.
    std::string sourceJsonAfter = source.print();
    std::cout << "Source object JSON after move (expected to be empty): " << sourceJsonAfter << std::endl;
    EXPECT_TRUE(sourceJsonAfter.empty());

    std::cout << "Exiting MoveConstructorValidNonEmpty test" << std::endl;
}
/**
 * @brief Verify that the move constructor correctly handles an empty DrmJsonObject.
 *
 * This test verifies that when a DrmJsonObject is created using the default constructor (resulting in an empty object)
 * and then move constructed, both the moved object and the original source object remain empty. This ensures that the
 * move semantics are correctly implemented even for an already empty object.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                           | Expected Result                                          | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Invoke the default constructor to create an empty DrmJsonObject.                                 | output: source = DrmJsonObject(empty)                               | source object is instantiated as empty                 | Should Pass   |
 * | 02               | Retrieve JSON content from the source object using print() method before move operation.           | input: source.print() = ?                                           | Expected to return an empty string                       | Should Pass   |
 * | 03               | Invoke the move constructor to create a new DrmJsonObject by moving the source object.             | input: source = std::move(source), output: moved = DrmJsonObject(moved)| Moved object's JSON content should be empty             | Should Pass   |
 * | 04               | Retrieve JSON content from both the moved object and the source object after the move operation.    | input: moved.print() = ?, source.print() = ?                          | Both moved object and source object should return empty  | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, MoveConstructorEmpty) {
    std::cout << "Entering MoveConstructorEmpty test" << std::endl;

    // Create an empty DrmJsonObject using the default constructor.
    std::cout << "Invoking default constructor to create an empty DrmJsonObject" << std::endl;
    DrmJsonObject source;

    // Retrieve JSON content from the source object before move.
    std::string sourceJsonBefore = source.print();
    std::cout << "Source object JSON before move (expected empty): " << sourceJsonBefore << std::endl;

    // Invoke move constructor using std::move(source)
    std::cout << "Invoking move constructor on empty object: DrmJsonObject moved(std::move(source))" << std::endl;
    DrmJsonObject moved(std::move(source));

    // Retrieve JSON content from the moved object.
    std::string movedJson = moved.print();
    std::cout << "Moved object JSON after move (expected empty): " << movedJson << std::endl;
    EXPECT_TRUE(movedJson.empty());

    // Retrieve and print the state of source after move.
    std::string sourceJsonAfter = source.print();
    std::cout << "Source object JSON after move (expected empty): " << sourceJsonAfter << std::endl;
    EXPECT_TRUE(sourceJsonAfter.empty());

    std::cout << "Exiting MoveConstructorEmpty test" << std::endl;
}
/**
 * @brief Verify that creating a DrmJsonObject with a valid simple JSON does not throw any exceptions.
 *
 * This test validates that the DrmJsonObject constructor correctly handles a well-formed JSON string by not throwing any exceptions. The test confirms that the simple JSON input provided is processed without errors, ensuring the proper functionality of the constructor in a positive scenario.
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
 * | 01 | Invoke the DrmJsonObject constructor with a valid JSON string to verify it processes the input correctly without throwing exceptions. | inputJson = "{\"key\": \"value\"}" | API returns a valid object and does not throw any exception; EXPECT_NO_THROW assertion passes. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, Construct_ValidSimpleJson) {
    std::cout << "Entering Construct_ValidSimpleJson test" << std::endl;
    
    const char inputJson[] = "{\"key\": \"value\"}";
    std::cout << "Invoking DrmJsonObject constructor with JSON input: " << inputJson << std::endl;
    EXPECT_NO_THROW(
        {
            DrmJsonObject obj(inputJson);
        }
    );
        
    std::cout << "Exiting Construct_ValidSimpleJson test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject correctly constructs an object from valid nested JSON input.
 *
 * This test validates that the DrmJsonObject constructor processes a nested JSON input containing both an inner JSON object and an array, ensuring that the object is constructed without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Define valid nested JSON string input. | inputJson = "{\"outerKey\": {\"innerKey1\": \"innerValue1\", \"innerArr\": [1, 2, 3]}}" | JSON string is defined correctly. | Should be successful |
 * | 02 | Invoke DrmJsonObject constructor with JSON input. | inputJson = "{\"outerKey\": {\"innerKey1\": \"innerValue1\", \"innerArr\": [1, 2, 3]}}" | DrmJsonObject constructed without throwing an exception. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, Construct_ValidNestedJson) {
    std::cout << "Entering Construct_ValidNestedJson test" << std::endl;
    
    const char inputJson[] = "{\"outerKey\": {\"innerKey1\": \"innerValue1\", \"innerArr\": [1, 2, 3]}}";
    std::cout << "Invoking DrmJsonObject constructor with JSON input: " << inputJson << std::endl;
    EXPECT_NO_THROW(
        {
            DrmJsonObject obj(inputJson);
        }
    );
    
    std::cout << "Exiting Construct_ValidNestedJson test" << std::endl;
}
/**
 * @brief Verify that constructing a DrmJsonObject with an empty JSON string throws an exception.
 *
 * Tests that passing an empty JSON string to the DrmJsonObject constructor results in an exception being thrown. This verifies the API's robustness in handling invalid input.
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
 * | Variation / Step | Description                                                        | Test Data                | Expected Result                                                    | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------ | ------------------------------------------------------------------ | ---------- |
 * | 01               | Initialize an empty JSON string and invoke DrmJsonObject constructor | inputJson = ""           | The DrmJsonObject constructor throws an exception upon receiving empty input | Should Pass |
 */
TEST_F(DrmJsonObjectTests, Construct_EmptyJson) {
    std::cout << "Entering Construct_EmptyJson test" << std::endl;
    
    const char inputJson[] = "";
    std::cout << "Invoking DrmJsonObject constructor with empty JSON input." << std::endl;
    EXPECT_ANY_THROW(
        {
            DrmJsonObject obj(inputJson);
        }
    );
        
    std::cout << "Exiting Construct_EmptyJson test" << std::endl;
}
/**
 * @brief Test the DrmJsonObject constructor with a null JSON pointer input.
 *
 * This test verifies that the DrmJsonObject constructor properly handles an invalid (null) JSON input pointer by throwing an exception. This is crucial for ensuring robust error handling in scenarios where the input data is missing or corrupted.
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
 * | Variation / Step | Description                                                  | Test Data                           | Expected Result                                          | Notes                |
 * | :--------------: | ------------------------------------------------------------ | ----------------------------------- | -------------------------------------------------------- | -------------------- |
 * | 01               | Initialize the null JSON pointer variable                    | inputJson = nullptr                 | Variable inputJson is set to a null pointer              | Should be successful |
 * | 02               | Invoke DrmJsonObject constructor with the null pointer input   | inputJson = nullptr                 | Constructor throws an exception as expected              | Should Fail          |
 */
TEST_F(DrmJsonObjectTests, Construct_NullPointerJson) {
    std::cout << "Entering Construct_NullPointerJson test" << std::endl;
    
    const char* inputJson = nullptr;
    std::cout << "Invoking DrmJsonObject constructor with null JSON input pointer." << std::endl;
    EXPECT_ANY_THROW(
        {
            DrmJsonObject obj(inputJson);
        }
    );

    std::cout << "Exiting Construct_NullPointerJson test" << std::endl;
}
/**
 * @brief Verify that the DrmJsonObject constructor handles a minimal empty JSON string correctly.
 *
 * This test verifies that passing an empty JSON object string "{}" to the DrmJsonObject constructor does not throw exceptions and that the object is correctly initialized by ensuring the internal pointer "mJsonObj" is not NULL. This ensures that the API behaves as expected with minimal valid JSON input.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 008@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                            | Expected Result                                       | Notes               |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------ | ----------------------------------------------------- | ------------------- |
 * | 01               | Invoke DrmJsonObject constructor with minimal empty JSON input   | input = "{}"                         | No exception is thrown                                | Should Pass         |
 * | 02               | Verify internal state after construction                         | output: mJsonObj should not be NULL  | mJsonObj is not NULL                                  | Should Pass         |
 */
TEST_F(DrmJsonObjectTests, ValidMinimalJsonString) {
    std::cout << "Entering ValidMinimalJsonString test" << std::endl;
    std::string input = "{}";
    bool ret = false;

    std::cout << "Invoking DrmJsonObject constructor with input: " << input << std::endl;
    EXPECT_NO_THROW(
        {
            DrmJsonObject obj("{}");
            ret = obj.get("dummy", input); // Dummy call to use obj and avoid unused variable warning
        }
    );
    std::cout << "After construction, checking internal state: mJsonObj should not be NULL." << std::endl;  
    EXPECT_TRUE(ret);

    std::cout << "Exiting ValidMinimalJsonString test" << std::endl;
}
/**
 * @brief Validates that a complex JSON string is correctly parsed without exceptions.
 *
 * This test verifies that the DrmJsonObject constructor can correctly parse a well-formed complex JSON string containing various elements such as key-value pairs, arrays, and nested objects. It checks that no exceptions are thrown during the object creation and that the internal state (mJsonObj) is set properly (i.e., not NULL).
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
 * | 01 | Prepare valid complex JSON string input. | input = {"key": "value", "array": [1, 2, 3], "object": {"nested": true}} | Variable 'input' contains the valid JSON string. | Should be successful |
 * | 02 | Invoke the DrmJsonObject constructor with the valid input. | input = {"key": "value", "array": [1, 2, 3], "object": {"nested": true}}, output1 = DrmJsonObject instance | No exception is thrown during object construction. | Should Pass |
 * | 03 | Validate that the object's internal state is set. | output1 = mJsonObj pointer of the created object | mJsonObj is not NULL, indicating successful parsing of nested JSON structure. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidComplexJsonString) {
    std::cout << "Entering ValidComplexJsonString test" << std::endl;

    // Using escaped quotes for valid complex JSON string
    DrmJsonObject obj;
    std::string input = "{\"key\": \"value\", \"array\": [1, 2, 3], \"object\": {\"nested\": true}}";
    std::cout << "Invoking DrmJsonObject constructor with input: " << input << std::endl;
    EXPECT_NO_THROW(
        {
            DrmJsonObject obj(input);
            bool ret = obj.get("test",input);
            EXPECT_TRUE(ret);
        }
    );

    // Debug log: Internal state check assumed for mJsonObj (should be non-null and properly represent nested structures)
    std::cout << "After construction, expecting internal state: mJsonObj is not NULL for nested JSON structure." << std::endl;
    //EXPECT_NE(obj.mJsonObj, static_cast<cJSON*>(nullptr));
    
    std::cout << "Exiting ValidComplexJsonString test" << std::endl;
}
/**
 * @brief Verify addition of an empty nested DrmJsonObject to a parent object.
 *
 * This test checks that an empty nested DrmJsonObject can be successfully added to a main DrmJsonObject using the 'add' method.
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
 * | Variation / Step | Description                                                      | Test Data                                               | Expected Result                    | Notes             |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------- | ----------------- |
 * |       01         | Create mainObj using default constructor                         | None                                                    | mainObj created successfully       | Should be successful |
 * |       02         | Create nestedObj (empty) using default constructor               | None                                                    | nestedObj created successfully     | Should be successful |
 * |       03         | Invoke add method on mainObj with name "nested" and nestedObj      | name = "nested", nestedObj = empty DrmJsonObject          | Returns true                       | Should Pass       |
 * |       04         | Assert that the result from add method is true                   | result = output of add method                           | Expect result to be true           | Should Pass       |
 */
TEST_F(DrmJsonObjectTests, ValidNestedAdditionEmpty) {
    std::cout << "Entering ValidNestedAdditionEmpty test" << std::endl;

    // Creating main DrmJsonObject using default constructor
    DrmJsonObject mainObj;
    std::cout << "Created mainObj using default constructor" << std::endl;

    // Creating nested DrmJsonObject (empty)
    DrmJsonObject nestedObj;
    std::cout << "Created nestedObj (empty) using default constructor" << std::endl;

    // Invoking add method with name = "nested" and nestedObj
    std::cout << "Invoking add with name: \"nested\" and nestedObj (empty)" << std::endl;
    bool result = mainObj.add("nested", nestedObj);
    std::cout << "add returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidNestedAdditionEmpty test" << std::endl;
}
/**
 * @brief Validate the addition of a non-empty nested DrmJsonObject into a main DrmJsonObject instance
 *
 * This test creates a main DrmJsonObject using the default constructor and a nested DrmJsonObject initialized with a valid JSON string. It then invokes the add method on the main object with the nested object and verifies that the operation is successful by expecting the return value to be true. This test is essential to confirm that non-empty nested JSON data is correctly handled.
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
 * | Variation / Step | Description                                                        | Test Data                                                              | Expected Result                                              | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------------------------------------------------------ | ------------- |
 * | 01               | Create main DrmJsonObject using the default constructor.           | mainObj created via default constructor                                | mainObj is successfully created.                             | Should be successful |
 * | 02               | Create nested DrmJsonObject with a valid JSON string.               | jsonStr = "{\"key\":\"value\"}", configObj constructed with jsonStr     | configObj contains valid JSON data.                          | Should be successful |
 * | 03               | Invoke add method on mainObj with name "config" and configObj.       | name = config, configObj = non-empty JSON object                         | add method returns true; EXPECT_TRUE(result) assertion passes. | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, ValidNestedAdditionNonEmpty) {
    std::cout << "Entering ValidNestedAdditionNonEmpty test" << std::endl;

    // Creating main DrmJsonObject using default constructor
    DrmJsonObject mainObj;
    std::cout << "Created mainObj using default constructor" << std::endl;

    // Creating nested DrmJsonObject with valid JSON string data
    std::string jsonStr = "{\"key\":\"value\"}";
    DrmJsonObject configObj(jsonStr);
    std::cout << "Created configObj with JSON string: " << jsonStr << std::endl;

    // Invoking add method with name = "config" and configObj
    std::cout << "Invoking add with name: \"config\" and configObj (non-empty)" << std::endl;
    bool result = mainObj.add("config", configObj);
    std::cout << "add returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidNestedAdditionNonEmpty test" << std::endl;
}
/**
 * @brief Validate the behavior of the add method when provided with an empty name string.
 *
 * This test verifies that invoking the add method with an empty string as the name results in a false return value, indicating that the object is not added.
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
 * | Variation / Step | Description                                                           | Test Data                                                       | Expected Result                                                        | Notes           |
 * | :--------------: | --------------------------------------------------------------------- | --------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------- |
 * | 01               | Create main DrmJsonObject using default constructor                   | No inputs                                                       | mainObj is initialized successfully                                    | Should be successful |
 * | 02               | Create nested DrmJsonObject using default constructor                 | No inputs                                                       | nestedObj is initialized successfully                                  | Should be successful |
 * | 03               | Invoke add method with empty name and the nested object               | input = "", nestedObj = default constructed object                | API returns false                                                      | Should Pass     |
 * | 04               | Verify that the result of the add invocation is false using assertion  | output = result, expected value = false                          | EXPECT_FALSE(result) assertion passes                                  | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, AddWithEmptyName) {
    std::cout << "Entering AddWithEmptyName test" << std::endl;

    // Creating main DrmJsonObject using default constructor
    DrmJsonObject mainObj;
    std::cout << "Created mainObj using default constructor" << std::endl;

    // Creating nested DrmJsonObject (could be empty or with data; using empty for this test)
    DrmJsonObject nestedObj;
    std::cout << "Created nestedObj using default constructor" << std::endl;

    // Invoking add method with empty name and nestedObj
    std::cout << "Invoking add with empty name \"\" and nestedObj" << std::endl;
    bool result = mainObj.add("", nestedObj);
    std::cout << "add returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);

    std::cout << "Exiting AddWithEmptyName test" << std::endl;
}
/**
 * @brief Validate that self-referencing addition in a DrmJsonObject is prevented
 *
 * This test checks that invoking the add method on a DrmJsonObject instance with itself as the value (self-referencing) does not allow the addition and returns false. This ensures the integrity of the object by preventing cyclic references.
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
 * | Variation / Step | Description                                                                             | Test Data                                                     | Expected Result                                               | Notes          |
 * | :----:           | :-------------------------------------------------------------------------------------- | :------------------------------------------------------------ | :------------------------------------------------------------ | :------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor                           | (No input arguments)                                          | A new instance of DrmJsonObject is created                    | Should be successful |
 * | 02               | Invoke add method with name "self" passing the same instance (self-reference)           | name = "self", reference = mainObj, output = result           | The add method returns false indicating rejection of self-reference | Should Fail    |
 * | 03               | Verify the return value using EXPECT_FALSE to ensure self-referencing addition was blocked | result = false                                                 | The test assertion passes confirming the API's behavior       | Should be successful |
 */
TEST_F(DrmJsonObjectTests, SelfReferencingAddition) {
    std::cout << "Entering SelfReferencingAddition test" << std::endl;

    // Creating a DrmJsonObject using default constructor
    DrmJsonObject mainObj;
    std::cout << "Created mainObj using default constructor" << std::endl;

    // Invoking add method with name "self" with the same instance (self-referencing)
    std::cout << "Invoking add with name: \"self\" and mainObj itself (self-referencing)" << std::endl;
    bool result = mainObj.add("self", mainObj);
    std::cout << "add returned value: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);

    std::cout << "Exiting SelfReferencingAddition test" << std::endl;
}
/**
 * @brief Verify that adding a valid non-empty key paired with a true value to DrmJsonObject is successful
 *
 * This test evaluates the functionality of the add method in DrmJsonObject when provided with a valid non-empty key and a boolean true value. It checks that the object is correctly instantiated, that the input values are properly passed to the API, and that the API returns a successful result as expected.
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
 * | Variation / Step | Description                                                      | Test Data                                                  | Expected Result                                                               | Notes           |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate a DrmJsonObject using the default constructor        | N/A                                                        | DrmJsonObject instance is created successfully                                | Should be successful |
 * | 02               | Prepare input values for the add method                          | key = validKey, value = true                               | Input values are set correctly                                                | Should be successful |
 * | 03               | Invoke the add method with the prepared key and value            | input key = validKey, input value = true, output result = capture | The add method returns true                                          | Should Pass |
 * | 04               | Assert that the returned result is true using EXPECT_TRUE         | result = returned value from add()                         | Assertion passes confirming that the returned result is true                  | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddingValidNonEmptyKeyWithTrue) {
    std::cout << "Entering AddingValidNonEmptyKeyWithTrue test" << std::endl;

    // Create a DrmJsonObject instance using the default constructor
    DrmJsonObject obj;
    
    // Prepare input values
    std::string key = "validKey";
    bool value = true;
    std::cout << "Invoking add with key: " << key << " and value: " << std::boolalpha << value << std::endl;
    
    // Invoke add method
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return true
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddingValidNonEmptyKeyWithTrue test" << std::endl;
}
/**
 * @brief Verify that the add() method successfully adds a valid non-empty key with a false boolean value.
 *
 * This test verifies that the DrmJsonObject::add() method correctly handles the insertion of a valid, non-empty key ("validKey") with a boolean value of false. The test ensures that the method returns true when the addition is successful.
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
 * | 01 | Create a DrmJsonObject instance using the default constructor | None | Instance of DrmJsonObject is created | Should be successful |
 * | 02 | Invoke the add() method with a valid key and a false boolean value | key = validKey, value = false | The add() method returns true | Should Pass |
 * | 03 | Perform assertion check on the return value of add() | result = true | EXPECT_TRUE assertion passes confirming the method returns true | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddingValidNonEmptyKeyWithFalse) {
    std::cout << "Entering AddingValidNonEmptyKeyWithFalse test" << std::endl;

    // Create a DrmJsonObject instance using the default constructor
    DrmJsonObject obj;
    
    // Prepare input values
    std::string key = "validKey";
    bool value = false;
    std::cout << "Invoking add with key: " << key << " and value: " << std::boolalpha << value << std::endl;
    
    // Invoke add method
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return true
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddingValidNonEmptyKeyWithFalse test" << std::endl;
}
/**
 * @brief Test for adding an entry with an empty key and a true value, expecting failure.
 *
 * This test verifies that when the add method of DrmJsonObject is invoked with an empty key and a boolean value true, it returns false indicating that the operation is invalid.
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
 * | Variation / Step | Description                                                         | Test Data                | Expected Result                                                      | Notes       |
 * | :----:           | ------------------------------------------------------------------- | ------------------------ | -------------------------------------------------------------------- | ----------- |
 * | 01               | Create a DrmJsonObject, prepare an empty key and true value, and invoke the add method | key = "", value = true   | The add method returns false, validated by EXPECT_FALSE             | Should Fail |
 */
TEST_F(DrmJsonObjectTests, AddingEmptyKeyWithTrue) {
    std::cout << "Entering AddingEmptyKeyWithTrue test" << std::endl;

    // Create a DrmJsonObject instance using the default constructor
    DrmJsonObject obj;
    
    // Prepare input values with an empty key
    std::string key = "";
    bool value = true;
    std::cout << "Invoking add with key: \"" << key << "\" (empty key) and value: " 
              << std::boolalpha << value << std::endl;
    
    // Invoke add method
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false due to invalid empty key
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddingEmptyKeyWithTrue test" << std::endl;
}
/**
 * @brief Validate that add() returns false when invoked with an empty key.
 *
 * This test verifies that the DrmJsonObject::add() method correctly handles an attempt to add a new entry using an empty key and boolean false value. The method is expected to detect the empty key as invalid and return false, thus preventing the addition of an invalid entry.
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
 * | Variation / Step | Description                                                        | Test Data                             | Expected Result                                              | Notes               |
 * | :--------------: | -------------------------------------------------------------------| --------------------------------------| -------------------------------------------------------------| ------------------- |
 * | 01               | Instantiate a DrmJsonObject using the default constructor.         | None                                  | Object is created successfully.                              | Should be successful|
 * | 02               | Prepare input values with an empty key and a boolean false value.    | key = "", value = false                 | Inputs are initialized as specified.                         | Should be successful|
 * | 03               | Invoke the add() method with the provided inputs.                    | input key = "", input value = false     | Method returns false due to invalid empty key.               | Should Pass         |
 * | 04               | Validate the output using EXPECT_FALSE to check the API behavior.    | output result = false                  | The assertion passes confirming the API correctly handles the condition. | Should be successful|
 */
TEST_F(DrmJsonObjectTests, AddingEmptyKeyWithFalse) {
    std::cout << "Entering AddingEmptyKeyWithFalse test" << std::endl;

    // Create a DrmJsonObject instance using the default constructor
    DrmJsonObject obj;
    
    // Prepare input values with an empty key
    std::string key = "";
    bool value = false;
    std::cout << "Invoking add with key: \"" << key << "\" (empty key) and value: " 
              << std::boolalpha << value << std::endl;
    
    // Invoke add method
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false due to invalid empty key
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddingEmptyKeyWithFalse test" << std::endl;
}
/**
 * @brief Verify that adding a key with special characters is correctly rejected
 *
 * This test verifies that the DrmJsonObject::add method correctly handles keys containing special characters by rejecting them. The function should return false to indicate that the key was not added due to invalid characters.
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
 * | Variation / Step | Description                                                      | Test Data                                          | Expected Result                         | Notes         |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------- | --------------------------------------- | ------------- |
 * | 01               | Instantiate a DrmJsonObject object                               | None                                               | Object created successfully             | Should be successful |
 * | 02               | Invoke add method with key containing special characters         | key = !@#$%^&*(), value = true                       | Return value should be false            | Should Fail   |
 * | 03               | Validate that the add method returned false using EXPECT_FALSE   | result = false                                     | EXPECT_FALSE assertion passes           | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddingKeyWithSpecialCharacters) {
    std::cout << "Entering AddingKeyWithSpecialCharacters test" << std::endl;

    // Create a DrmJsonObject instance using the default constructor
    DrmJsonObject obj;
    
    // Prepare input values with special characters in the key
    std::string key = "!@#$%^&*()";
    bool value = true;
    std::cout << "Invoking add with key: " << key << " and value: " 
              << std::boolalpha << value << std::endl;
    
    // Invoke add method
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false based on test plan for special characters
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddingKeyWithSpecialCharacters test" << std::endl;
}
/**
 * @brief Verify that adding a valid number to DrmJsonObject returns success.
 *
 * This test exercises the add method of DrmJsonObject with a valid cJSON-formatted number input.
 * It ensures that the DrmJsonObject instance correctly accepts and processes a valid numeric cJSON object.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                   | Test Data                                                              | Expected Result                                              | Notes                 |
 * | :--------------: | --------------------------------------------------------------| -----------------------------------------------------------------------| ----------------------------------------------------------- | --------------------- |
 * | 01               | Create a DrmJsonObject instance                                 | None                                                                   | DrmJsonObject instance created successfully                  | Should be successful  |
 * | 02               | Create and initialize a valid cJSON object for a number         | jsonNumber->type = cJSON_Number, jsonNumber->valueint = 42, jsonNumber->valuedouble = 42.0, jsonNumber->valuestring = nullptr, jsonNumber->string = nullptr | Valid cJSON object created with number details              | Should be successful  |
 * | 03               | Invoke DrmJsonObject::add with name "numberField" and the valid cJSON pointer | name = numberField, cJSON pointer = address of jsonNumber             | API invoked and returns true                                | Should Pass           |
 * | 04               | Check the return value using EXPECT_TRUE                        | result = true                                                          | EXPECT_TRUE condition satisfied (result is true)            | Should Pass           |
 * | 05               | Clean up allocated memory for the cJSON object                  | delete jsonNumber                                                      | Memory successfully released                                | Should be successful  |
 */
TEST_F(DrmJsonObjectTests, AddValidNumber) {
    std::cout << "Entering AddValidNumber test" << std::endl;

    // Create DrmJsonObject instance using default constructor.
    DrmJsonObject obj;
    std::cout << "Created DrmJsonObject instance." << std::endl;

    // Allocate and initialize a valid cJSON object for a number.
    cJSON* jsonNumber = new cJSON;
    jsonNumber->next = nullptr;
    jsonNumber->prev = nullptr;
    jsonNumber->child = nullptr;
    jsonNumber->type = cJSON_Number;
    jsonNumber->valueint = 42;
    jsonNumber->valuedouble = 42.0;
    jsonNumber->valuestring = nullptr;
    jsonNumber->string = nullptr;
    std::cout << "Created cJSON object for number:" << std::endl;
    std::cout << "  type: " << jsonNumber->type << std::endl;
    std::cout << "  valueint: " << jsonNumber->valueint << std::endl;
    std::cout << "  valuedouble: " << jsonNumber->valuedouble << std::endl;

    // Log invocation details.
    std::string name = "numberField";
    std::cout << "Invoking DrmJsonObject::add with name: " << name << " and cJSON pointer: " << jsonNumber << std::endl;
    
    // Invoke add method.
    bool result = obj.add(name, jsonNumber);
    std::cout << "DrmJsonObject::add returned: " << std::boolalpha << result << std::endl;

    // Check expected output.
    EXPECT_TRUE(result);

    // Clean up allocated memory.
    delete jsonNumber;

    std::cout << "Exiting AddValidNumber test" << std::endl;
}
/**
 * @brief Validate DrmJsonObject::add with valid string input.
 *
 * This test verifies that the add method of DrmJsonObject correctly adds a valid cJSON object representing a string when provided with a valid field name. It ensures that the initialization, memory operations, and successful return value are correctly handled.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 020@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                             | Test Data                                                                                                                      | Expected Result                                                             | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------- | ----------------- |
 * | 01 | Create a DrmJsonObject instance.                                                   | No input arguments.                                                                                                            | Instance created successfully.                                              | Should be successful |
 * | 02 | Allocate and initialize a valid cJSON object for a string.                         | jsonString->type = cJSON_String, jsonString->valueint = 0, jsonString->valuedouble = 0.                                         | cJSON object is properly initialized.                                       | Should be successful |
 * | 03 | Prepare and assign a valid example string to jsonString->valuestring using strncpy.  | exampleStr = "example", tempBuffer contains "example", jsonString->valuestring allocated with correct size.                     | cJSON object contains a valid string value.                                 | Should be successful |
 * | 04 | Invoke DrmJsonObject::add with valid name and cJSON pointer.                         | name = "stringField", jsonString pointer.                                                                                      | API returns true indicating successful addition.                          | Should Pass       |
 * | 05 | Validate the API output using an assertion.                                          | Return value from add() expected to be true, compared using EXPECT_TRUE(result).                                               | Assertion passes with true value.                                           | Should Pass       |
 * | 06 | Clean up allocated memory for jsonString and its members.                          | De-allocation of jsonString->valuestring and jsonString.                                                                       | Memory freed without errors.                                                | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddValidString) {
    std::cout << "Entering AddValidString test" << std::endl;

    // Create DrmJsonObject instance.
    DrmJsonObject obj;
    std::cout << "Created DrmJsonObject instance." << std::endl;

    // Allocate and initialize a valid cJSON object for a string.
    cJSON* jsonString = new cJSON;
    jsonString->next = nullptr;
    jsonString->prev = nullptr;
    jsonString->child = nullptr;
    jsonString->type = cJSON_String;
    jsonString->valueint = 0;
    jsonString->valuedouble = 0.0;

    // Prepare valuestring using strncpy.
    char tempBuffer[100];
    std::memset(tempBuffer, 0, sizeof(tempBuffer));
    const char* exampleStr = "example";
    std::strncpy(tempBuffer, exampleStr, sizeof(tempBuffer) - 1);
    jsonString->valuestring = new char[std::strlen(tempBuffer) + 1];
    std::strncpy(jsonString->valuestring, tempBuffer, std::strlen(tempBuffer) + 1);
    jsonString->string = nullptr;
    std::cout << "Created cJSON object for string:" << std::endl;
    std::cout << "  type: " << jsonString->type << std::endl;
    std::cout << "  valuestring: " << jsonString->valuestring << std::endl;

    // Log invocation details.
    std::string name = "stringField";
    std::cout << "Invoking DrmJsonObject::add with name: " << name << " and cJSON pointer: " << jsonString << std::endl;
    
    // Invoke add method.
    bool result = obj.add(name, jsonString);
    std::cout << "DrmJsonObject::add returned: " << std::boolalpha << result << std::endl;

    // Check expected output.
    EXPECT_TRUE(result);

    // Clean up allocated memory.
    delete [] jsonString->valuestring;
    delete jsonString;

    std::cout << "Exiting AddValidString test" << std::endl;
}
/**
 * @brief Test the behavior of adding an element with an empty name to a DrmJsonObject.
 *
 * This test verifies that when an empty name string is passed along with a valid cJSON object to the DrmJsonObject::add method, 
 * the method correctly returns false, indicating a failure to add the element. The test also ensures proper memory allocation and cleanup.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                    | Expected Result                                   | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------------------------------------------- | ------------- |
 * | 01               | Create an instance of DrmJsonObject.                                        | instance: new DrmJsonObject()                                                | Instance is created successfully.                 | Should be successful  |
 * | 02               | Allocate and initialize a valid cJSON object for a string type.             | cJSON->type = cJSON_String, valueint = 0, valuedouble = 0.0                    | cJSON object is created with string type correctly.| Should be successful  |
 * | 03               | Prepare and set the valuestring property of the cJSON object using strncpy. | dataStr = "data", tempBuffer initialized, valuestring allocated accordingly    | valuestring is assigned correctly.                | Should be successful  |
 * | 04               | Log invocation details and prepare the empty name string.                   | name = "", jsonString pointer                                                  | Invocation details are logged.                    | Should be successful  |
 * | 05               | Invoke DrmJsonObject::add with an empty name and valid cJSON pointer.         | input: name = "", jsonString pointer; output: bool result                      | The add method returns false.                     | Should Fail   |
 * | 06               | Validate the result using an EXPECT_FALSE assertion.                        | result expected to be false                                                    | Test passes if add returns false.                 | Should Pass   |
 * | 07               | Clean up allocated memory.                                                  | delete valuestring and cJSON object                                            | Memory is freed successfully.                     | Should be successful  |
 */
TEST_F(DrmJsonObjectTests, AddEmptyName) {
    std::cout << "Entering AddEmptyName test" << std::endl;

    // Create DrmJsonObject instance.
    DrmJsonObject obj;
    std::cout << "Created DrmJsonObject instance." << std::endl;

    // Allocate and initialize a valid cJSON object for a string.
    cJSON* jsonString = new cJSON;
    jsonString->next = nullptr;
    jsonString->prev = nullptr;
    jsonString->child = nullptr;
    jsonString->type = cJSON_String;
    jsonString->valueint = 0;
    jsonString->valuedouble = 0.0;

    // Prepare valuestring using strncpy.
    char tempBuffer[100];
    std::memset(tempBuffer, 0, sizeof(tempBuffer));
    const char* dataStr = "data";
    std::strncpy(tempBuffer, dataStr, sizeof(tempBuffer) - 1);
    jsonString->valuestring = new char[std::strlen(tempBuffer) + 1];
    std::strncpy(jsonString->valuestring, tempBuffer, std::strlen(tempBuffer) + 1);
    jsonString->string = nullptr;
    std::cout << "Created cJSON object for string with empty name test:" << std::endl;
    std::cout << "  type: " << jsonString->type << std::endl;
    std::cout << "  valuestring: " << jsonString->valuestring << std::endl;

    // Log invocation details.
    std::string name = "";
    std::cout << "Invoking DrmJsonObject::add with empty name and cJSON pointer: " << jsonString << std::endl;
    
    // Invoke add method.
    bool result = obj.add(name, jsonString);
    std::cout << "DrmJsonObject::add returned: " << std::boolalpha << result << std::endl;

    // Check expected output.
    EXPECT_FALSE(result);

    // Clean up allocated memory.
    delete [] jsonString->valuestring;
    delete jsonString;

    std::cout << "Exiting AddEmptyName test" << std::endl;
}
/**
 * @brief Verifies that DrmJsonObject::add returns false when a NULL cJSON pointer is provided
 *
 * This test case creates an instance of DrmJsonObject, calls the add method with a valid non-empty name "nullField" and a NULL cJSON pointer, and verifies that the method returns false.
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
 * | 01 | Instantiate a DrmJsonObject object | none | DrmJsonObject instance is created successfully | Should be successful |
 * | 02 | Invoke add method with name = "nullField" and NULL cJSON pointer | name = nullField, cJSON pointer = nullptr | API returns false | Should Fail |
 * | 03 | Assert that the API output is false | result = false | EXPECT_FALSE(result) passes | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddNullValue) {
    std::cout << "Entering AddNullValue test" << std::endl;

    // Create DrmJsonObject instance.
    DrmJsonObject obj;
    std::cout << "Created DrmJsonObject instance." << std::endl;

    // Log invocation details.
    std::string name = "nullField";
    std::vector<std::string> values;
    std::cout << "Invoking DrmJsonObject::add with name: " << name << " and NULL cJSON pointer." << std::endl;
    
    // Invoke add method with NULL pointer.
    bool result = obj.add("name", &values);
    std::cout << "DrmJsonObject::add returned: " << std::boolalpha << result << std::endl;

    // Check expected output.
    EXPECT_FALSE(result);

    std::cout << "Exiting AddNullValue test" << std::endl;
}/**
 * @brief Verify that DrmJsonObject correctly adds a cJSON object containing special characters in its string field.
 *
 * This test examines the functionality of the DrmJsonObject::add method when invoked with a cJSON object whose string field contains special characters.
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
 * | 01 | Create DrmJsonObject instance. | None | Instance of DrmJsonObject is created. | Should be successful |
 * | 02 | Allocate and initialize a valid cJSON object for a string. | cJSON: type = cJSON_String, valueint = 0, valuedouble = 0.0 | cJSON object is allocated and all fields are initialized properly. | Should be successful |
 * | 03 | Prepare valuestring using strncpy with a special string. | tempBuffer = "special_data", valuestring allocated with copied content. | Buffer is correctly copied and cJSON->valuestring holds "special_data". | Should be successful |
 * | 04 | Invoke the add method with a non-empty name containing special characters and the prepared cJSON pointer. | name = "special!@#Field", jsonString pointer = allocated cJSON pointer | API returns true; EXPECT_TRUE(result) assertion passes. | Should Pass |
 * | 05 | Clean up the allocated memory after validation. | Deletes for jsonString->valuestring and jsonString. | All allocated memory is cleaned up without error. | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddSpecialCharacters) {
    std::cout << "Entering AddSpecialCharacters test" << std::endl;

    // Create DrmJsonObject instance.
    DrmJsonObject obj;
    std::cout << "Created DrmJsonObject instance." << std::endl;

    // Allocate and initialize a valid cJSON object for a string.
    cJSON* jsonString = new cJSON;
    jsonString->next = nullptr;
    jsonString->prev = nullptr;
    jsonString->child = nullptr;
    jsonString->type = cJSON_String;
    jsonString->valueint = 0;
    jsonString->valuedouble = 0.0;
    
    // Prepare valuestring using strncpy.
    char tempBuffer[100];
    std::memset(tempBuffer, 0, sizeof(tempBuffer));
    const char* specialStr = "special_data";
    std::strncpy(tempBuffer, specialStr, sizeof(tempBuffer) - 1);
    jsonString->valuestring = new char[std::strlen(tempBuffer) + 1];
    std::strncpy(jsonString->valuestring, tempBuffer, std::strlen(tempBuffer) + 1);
    jsonString->string = nullptr;
    std::cout << "Created cJSON object for string with special characters:" << std::endl;
    std::cout << "  type: " << jsonString->type << std::endl;
    std::cout << "  valuestring: " << jsonString->valuestring << std::endl;

    // Log invocation details.
    std::string name = "special!@#Field";
    std::cout << "Invoking DrmJsonObject::add with name: " << name << " and cJSON pointer: " << jsonString << std::endl;
    
    // Invoke add method.
    bool result = obj.add(name, jsonString);
    std::cout << "DrmJsonObject::add returned: " << std::boolalpha << result << std::endl;

    // Check expected output.
    EXPECT_TRUE(result);

    // Clean up allocated memory.
    delete [] jsonString->valuestring;
    delete jsonString;

    std::cout << "Exiting AddSpecialCharacters test" << std::endl;
}
/**
 * @brief Verify that adding a valid non-empty array to a DrmJsonObject returns a successful result
 *
 * This test validates that when a valid key ("videoFormats") and a non-empty vector of strings 
 * ({"mp4", "webm", "avi"}) are provided to the add method of the DrmJsonObject, the method returns true.
 * The test confirms that the API correctly processes the input and stores/handles the given values as expected.
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
 * | Variation / Step | Description                                                    | Test Data                                                        | Expected Result                                                    | Notes         |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize the DrmJsonObject using the default constructor.    | None                                                             | DrmJsonObject instance created successfully.                       | Should be successful |
 * | 02               | Define valid test inputs: set the name and non-empty vector.     | name = "videoFormats", values = "mp4, webm, avi"                   | Variables correctly assigned the provided values.                  | Should be successful |
 * | 03               | Invoke the add method with valid name and non-empty vector.      | input: name = "videoFormats", values = "mp4, webm, avi"; output: result | API returns true indicating the array was added successfully.        | Should Pass   |
 * | 04               | Validate the returned result using EXPECT_TRUE assertion.      | result = true                                                     | EXPECT_TRUE(result) passes confirming the successful addition.       | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, AddingValidArray) {
    std::cout << "Entering AddingValidArray test" << std::endl;
    
    // Creating a DrmJsonObject object using the default constructor.
    DrmJsonObject obj;
    
    // Define test input: valid name and non-empty vector of strings.
    std::string name = "videoFormats";
    std::vector<std::string> values = {"mp4", "webm", "avi"};
    
    // Log invocation details.
    std::cout << "Invoking add with name: " << name << " and values: ";
    for (const auto &val : values)
        std::cout << "[" << val << "] ";
    std::cout << std::endl;
    
    bool result = obj.add(name, values);
    
    // Log the returned result.
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Check the expected output.
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddingValidArray test" << std::endl;
}
/**
 * @brief Tests the add function of DrmJsonObject with a valid name and an empty vector.
 *
 * This test verifies that when a valid name and an empty vector are provided to the add method of DrmJsonObject, the function returns true. The purpose is to ensure that the add function can handle an empty vector correctly.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a DrmJsonObject instance | None | DrmJsonObject instance is created successfully | Should be successful |
 * | 02 | Define test inputs with a valid name and an empty vector | name = "emptyArray", values = {} | Variables are initialized with the correct test data | Should be successful |
 * | 03 | Invoke the add method with the provided name and empty vector | input: name = "emptyArray", input: values = {} | Method returns true indicating successful addition | Should Pass |
 * | 04 | Assert the return value is true using EXPECT_TRUE | output: result = true | EXPECT_TRUE confirms the method returned true | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddingEmptyVector) {
    std::cout << "Entering AddingEmptyVector test" << std::endl;
    
    // Creating a DrmJsonObject object.
    DrmJsonObject obj;
    
    // Define test input: valid name and empty vector.
    std::string name = "emptyArray";
    std::vector<std::string> values;
    
    // Log invocation details.
    std::cout << "Invoking add with name: " << name << " and an empty vector of values" << std::endl;
    
    bool result = obj.add(name, values);
    
    // Log the returned result.
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Check the expected output.
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddingEmptyVector test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject::add rejects an empty name with non-empty values.
 *
 * This test evaluates whether the DrmJsonObject::add API correctly handles the case where an empty string is provided as the name along with a non-empty vector of values, ensuring that the function returns false as expected.
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
 * | Variation / Step | Description                                                                                                    | Test Data                                                      | Expected Result                                             | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ----------------------------------------------------------- | ------------- |
 * | 01               | Create a DrmJsonObject instance and initialize test inputs with an empty name and a valid vector of values.    | name = "", values = "value1, value2"                             | Instance created and inputs initialized                     | Should be successful |
 * | 02               | Invoke the add API of DrmJsonObject with the empty name and the non-empty vector.                                | name = "", values = "value1, value2" output: result = (to be returned) | add() returns false                                          | Should Fail   |
 * | 03               | Assert that the add function call returns false using the EXPECT_FALSE check.                                  | result = false                                                 | Assertion passes confirming add() returned false             | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddingEmptyName) {
    std::cout << "Entering AddingEmptyName test" << std::endl;
    
    // Creating a DrmJsonObject object.
    DrmJsonObject obj;
    
    // Define test input: empty name and non-empty vector.
    std::string name = "";
    std::vector<std::string> values = {"value1", "value2"};
    
    // Log invocation details.
    std::cout << "Invoking add with an empty name and values: ";
    for (const auto &val : values)
        std::cout << "[" << val << "] ";
    std::cout << std::endl;
    
    bool result = obj.add(name, values);
    
    // Log the returned result.
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Check the expected output.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddingEmptyName test" << std::endl;
}
/**
 * @brief Verify that the API correctly rejects a mix of empty and valid string values.
 *
 * This test validates that the DrmJsonObject::add method correctly handles a scenario where a valid name is provided along with a vector of values containing an empty string and a valid string. It ensures that the method enforces input validations by rejecting the mixed values and returning false.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of DrmJsonObject | No input parameters | Instance of DrmJsonObject is created | Should be successful |
 * | 02 | Define test input data with a valid name and a vector containing an empty string and a valid string | name = "mixedValues", values = ["", "validValue"] | Test input is configured correctly | Should be successful |
 * | 03 | Log invocation details showing name and values | name = "mixedValues", values = ["", "validValue"] | Log displays invocation details with an "empty" placeholder for empty string | Should be successful |
 * | 04 | Invoke the add method with the defined test inputs and capture the result | name = "mixedValues", values = ["", "validValue"] | Method returns false | Should Fail |
 * | 05 | Log the result and assert that the returned value is false using EXPECT_FALSE | output = false | Assertion passes confirming the API returns false for mixed values | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddingMixedValues) {
    std::cout << "Entering AddingMixedValues test" << std::endl;
    
    // Creating a DrmJsonObject object.
    DrmJsonObject obj;
    
    // Define test input: valid name and vector with an empty string element.
    std::string name = "mixedValues";
    std::vector<std::string> values = {"", "validValue"};
    
    // Log invocation details.
    std::cout << "Invoking add with name: " << name << " and values: ";
    for (const auto &val : values)
        std::cout << "[" << (val.empty() ? "empty" : val) << "] ";
    std::cout << std::endl;
    
    bool result = obj.add(name, values);
    
    // Log the returned result.
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Check the expected output.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddingMixedValues test" << std::endl;
}
/**
 * @brief Validate the DrmJsonObject add() API with a large vector input.
 *
 * This test verifies that the DrmJsonObject API correctly processes the addition of a key associated with a large vector containing 1000 valid entries. The objective is to ensure that the function can handle large datasets and returns a successful result when provided with valid input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create an instance of DrmJsonObject. | N/A | Object is created successfully. | Should be successful |@n
 * | 02 | Define valid name "largeArray" and prepare a large vector with 1000 entries where each entry is a string "entryX" (X from 1 to 1000). | name = "largeArray", vector values = ["entry1", "entry2", ..., "entry1000"] | The test input is set up correctly with 1000 valid entries. | Should be successful |@n
 * | 03 | Invoke the add API on the DrmJsonObject instance with the defined name and large vector. | input: name = "largeArray", values = vector with 1000 entries; output: bool result from add() | API returns true and the assertion EXPECT_TRUE(result) passes. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddingLargeVector) {
    std::cout << "Entering AddingLargeVector test" << std::endl;
    
    // Creating a DrmJsonObject object.
    DrmJsonObject obj;
    
    // Define test input: valid name and a large vector with 1000 valid entries.
    std::string name = "largeArray";
    std::vector<std::string> values;
    values.reserve(1000);
    for (int i = 1; i <= 1000; i++) {
        // Using snprintf like approach with strncpy equivalent behavior.
        char buffer[50];
        std::string entry = "entry" + std::to_string(i);
        // Simulate fixed array assignment using strncpy style.
        strncpy(buffer, entry.c_str(), sizeof(buffer)-1);
        buffer[sizeof(buffer)-1] = '\0';
        values.push_back(std::string(buffer));
    }
    
    // Log invocation details.
    std::cout << "Invoking add with name: " << name << " and a large vector of size: " << values.size() << std::endl;
    
    bool result = obj.add(name, values);
    
    // Log the returned result.
    std::cout << "Returned value: " << std::boolalpha << result << std::endl;
    
    // Check the expected output.
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddingLargeVector test" << std::endl;
}
/**
 * @brief Test adding a valid key with a positive double value.
 *
 * This test verifies that the DrmJsonObject::add method correctly adds a key with a valid positive double value.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 029
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a DrmJsonObject object using the default constructor | No input parameters | Object is created successfully | Should be successful |
 * | 02 | Invoke add method with valid key and positive double value | key = validKey, value = 42.0 | API returns true for a successful addition | Should Pass |
 * | 03 | Verify that the add method returns true using assertion | result = true (from add method call) | EXPECT_TRUE assertion passes confirming valid key addition | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddValidKeyPositiveDoubleValue) {
    std::cout << "Entering AddValidKeyPositiveDoubleValue test" << std::endl;
    
    std::string key = "validKey";
    double value = 42.0;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add with name: " << key << " and value: " << value << std::endl;
    bool result = jsonObj.add(key, value);
    
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidKeyPositiveDoubleValue test" << std::endl;
}
/**
 * @brief Test to verify that adding a valid key with a negative double value returns true.
 *
 * Verifies that the DrmJsonObject add function successfully handles the insertion of a negative double value with a valid key. This test checks the correct processing of negative values within a JSON object.
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
 * | Variation / Step | Description                                                    | Test Data                                                  | Expected Result                                                 | Notes             |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------- | --------------------------------------------------------------- | ----------------- |
 * |       01         | Initialize test data with key and negative double value.       | key = negativeKey, value = -3.14                            | Variables are initialized for test execution.                   | Should be successful |
 * |       02         | Instantiate DrmJsonObject using default constructor.           | output: jsonObj object creation                             | DrmJsonObject object is created successfully.                   | Should be successful |
 * |       03         | Invoke the add API with key and negative double value.           | input1 = key = negativeKey, input2 = value = -3.14, output1 = result | API returns true and the assertion EXPECT_TRUE(result) passes.    | Should Pass        |
 */
TEST_F(DrmJsonObjectTests, AddValidKeyNegativeDoubleValue) {
    std::cout << "Entering AddValidKeyNegativeDoubleValue test" << std::endl;
    
    std::string key = "negativeKey";
    double value = -3.14;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add with name: " << key << " and value: " << value << std::endl;
    bool result = jsonObj.add(key, value);
    
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidKeyNegativeDoubleValue test" << std::endl;
}
/**
 * @brief Verify that a valid key with a zero value can be added successfully to the DrmJsonObject.
 *
 * This test checks if the DrmJsonObject's add method correctly allows insertion of a key associated with a value of zero. It ensures that the API accepts zero as a valid numerical input and returns a success indicator, thus avoiding misinterpretation of zero as an invalid value.
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
 * | Variation / Step | Description                                               | Test Data                                    | Expected Result                                                         | Notes          |
 * | :--------------: | --------------------------------------------------------- | -------------------------------------------- | ----------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize the valid key and zero value.                  | key = "zeroKey", value = 0.0                   | Variables are initialized with the provided values.                   | Should be successful |
 * | 02               | Create DrmJsonObject object using the default constructor. | No input arguments                           | Object is instantiated successfully.                                   | Should be successful |
 * | 03               | Invoke the add() method with the key and zero value.        | key = "zeroKey", value = 0.0                   | The add() method returns true indicating successful addition.          | Should Pass    |
 * | 04               | Verify the result using EXPECT_TRUE assertion.            | result = value returned from add()           | The EXPECT_TRUE assertion passes as result is true.                     | Should Pass    |
 */
TEST_F(DrmJsonObjectTests, AddValidKeyZeroValue) {
    std::cout << "Entering AddValidKeyZeroValue test" << std::endl;
    
    std::string key = "zeroKey";
    double value = 0.0;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add with name: " << key << " and value: " << value << std::endl;
    bool result = jsonObj.add(key, value);
    
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidKeyZeroValue test" << std::endl;
}
/**
 * @brief Validate that the add function successfully adds a key with the maximum double value.
 *
 * This test verifies that the DrmJsonObject::add method correctly handles the addition of a key ("maxDoubleKey")
 * with the maximum possible double value (DBL_MAX). This ensures that the method can process extreme numeric values
 * without causing errors or overflow issues.
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
 * | Variation / Step | Description                                                            | Test Data                                             | Expected Result                                                  | Notes           |
 * | :--------------: | ---------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------------------------------------------- | --------------- |
 * | 01               | Log test entry and initialize key and value                           | key = "maxDoubleKey", value = DBL_MAX                   | Variables initialized correctly                                  | Should be successful |
 * | 02               | Create DrmJsonObject instance using the default constructor            | No input parameters                                    | Object created successfully                                       | Should be successful |
 * | 03               | Invoke add method with the key and maximum double value                 | key = "maxDoubleKey", value = DBL_MAX                   | add returns true indicating that the key-value pair is added        | Should Pass     |
 * | 04               | Assert that the add method returned true                                | result from add = true                                 | EXPECT_TRUE assertion passes                                     | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddValidKeyMaxDoubleValue) {
    std::cout << "Entering AddValidKeyMaxDoubleValue test" << std::endl;
    
    std::string key = "maxDoubleKey";
    double value = DBL_MAX;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add with name: " << key << " and value: " << value << std::endl;
    bool result = jsonObj.add(key, value);
    
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidKeyMaxDoubleValue test" << std::endl;
}
/**
 * @brief Verify that adding a key-value pair with an empty string as the key returns false.
 *
 * This test validates that the DrmJsonObject::add() method correctly handles an attempt to add a key-value pair when the key is an empty string. The expected behavior is that the method returns false, ensuring that invalid keys are not accepted.
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
 * | Variation / Step | Description                                               | Test Data                                       | Expected Result                                                       | Notes         |
 * | :--------------: | --------------------------------------------------------- | ----------------------------------------------- | --------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test variables with an empty key and a value.  | key = "", value = 1.23                           | Test variables are defined correctly.                                | Should be successful |
 * | 02               | Create a DrmJsonObject instance using default constructor.| No input                                        | Instance of DrmJsonObject is created successfully.                   | Should be successful |
 * | 03               | Invoke the add() method with the empty key and the value.   | input: key = "", value = 1.23, output: result      | API returns false, and EXPECT_FALSE(result) assertion passes.        | Should Fail   |
 */
TEST_F(DrmJsonObjectTests, AddEmptyStringKey) {
    std::cout << "Entering AddEmptyStringKey test" << std::endl;
    
    std::string key = "";
    double value = 1.23;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add with an empty key and value: " << value << std::endl;
    bool result = jsonObj.add(key, value);
    
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddEmptyStringKey test" << std::endl;
}
/**
 * @brief Validates that the DrmJsonObject can successfully add a positive integer parameter.
 *
 * This test verifies that using the default constructor to create a DrmJsonObject, the add() method returns true when invoked with a valid parameter name and a positive integer value. This confirms the correct functionality of the add() method with standard positive integer input.
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
 * | Variation / Step | Description                                                             | Test Data                                           | Expected Result                                   | Notes             |
 * | :---------------:| ----------------------------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------- | ----------------- |
 * | 01               | Create DrmJsonObject object using the default constructor               | none                                                | Object is created without errors                  | Should be successful |
 * | 02               | Invoke add() method with a valid parameter name and positive integer value| name = param1, value = 123, output (result) = true    | add() returns true and passes the assertion       | Should Pass       |
 */
TEST_F(DrmJsonObjectTests, ValidAddPositiveInt) {
    std::cout << "Entering ValidAddPositiveInt test" << std::endl;
    
    std::string name = "param1";
    int value = 123;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;

    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidAddPositiveInt test" << std::endl;
}
/**
 * @brief Validate that adding a negative integer value to a DrmJsonObject is handled correctly.
 *
 * This test verifies that the add() method of the DrmJsonObject class accepts and correctly processes a negative integer value. It ensures that negative values are supported and the API returns a successful result.
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
 * | Variation / Step | Description                                                                 | Test Data                                          | Expected Result                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------- | ---------------------------------------- | ------------- |
 * | 01               | Initialize test parameters with name and negative value                     | name = param2, value = -456                         | Variables initialized                    | Should be successful |
 * | 02               | Create a DrmJsonObject instance using the default constructor               | (none)                                             | Object created successfully              | Should be successful |
 * | 03               | Invoke the add() method with the initialized name and negative value        | input1 = name (param2), input2 = value (-456)         | API returns true                         | Should Pass   |
 * | 04               | Validate the result of the add() method with an assertion check             | output1 = result (true)                             | EXPECT_TRUE(result) passes               | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, ValidAddNegativeInt) {
    std::cout << "Entering ValidAddNegativeInt test" << std::endl;
    
    std::string name = "param2";
    int value = -456;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidAddNegativeInt test" << std::endl;
}
/**
 * @brief Verify that adding a parameter with a zero value is handled correctly by DrmJsonObject
 *
 * This test ensures that the add() method correctly processes a valid parameter where the value is zero. 
 * The intent is to validate that the API does not misinterpret a zero value as an error and returns a successful result.
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
 * | Variation / Step | Description                                                           | Test Data                                      | Expected Result                                               | Notes      |
 * | :--------------: | --------------------------------------------------------------------- | ---------------------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the add() method on a DrmJsonObject object with name "param_zero" and value 0 | name = param_zero, value = 0, expected_return = true | The add() API returns true and the assertion passes            | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidAddZeroValue) {
    std::cout << "Entering ValidAddZeroValue test" << std::endl;
    
    std::string name = "param_zero";
    int value = 0;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidAddZeroValue test" << std::endl;
}
/**
 * @brief Test to validate that add() fails when provided with an empty name.
 *
 * This test verifies that invoking the add() method on a DrmJsonObject with an empty string as the name and a valid integer value correctly returns false. The test confirms that the API does not accept an empty string as a valid key, ensuring input validation enforcement.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 037
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                         | Test Data                   | Expected Result                                                                 | Notes      |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------- | ------------------------------------------------------------------------------- | ---------- |
 * | 01               | Create a default DrmJsonObject and invoke add() with empty name and value 10 | name = "", value = 10         | add() returns false as empty name is invalid, confirmed by EXPECT_FALSE assertion | Should Fail|
 */
TEST_F(DrmJsonObjectTests, InvalidAddEmptyName) {
    std::cout << "Entering InvalidAddEmptyName test" << std::endl;
    
    std::string name = "";
    int value = 10;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with empty name and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InvalidAddEmptyName test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject::add() returns true when adding INT_MAX as the integer value
 *
 * This test verifies that the add() method of DrmJsonObject correctly accepts and processes the maximum integer value (INT_MAX). The objective is to ensure that the API handles edge integer values without errors.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize variables with name and value. | name = max_value, value = INT_MAX | Variables hold intended values | Should be successful |
 * | 02 | Create DrmJsonObject object using default constructor. | None | Object is successfully created | Should be successful |
 * | 03 | Invoke add() method with given parameters. | input: name = max_value, value = INT_MAX; output: expects true | Method returns true | Should Pass |
 * | 04 | Validate the return value using EXPECT_TRUE. | result = outcome of add() | Assertion passes confirming the API returns true for valid inputs | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidAddIntMax) {
    std::cout << "Entering ValidAddIntMax test" << std::endl;
    
    std::string name = "max_value";
    int value = INT_MAX;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
        
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidAddIntMax test" << std::endl;
}
/**
 * @brief Validate that the add() API correctly adds an INT_MIN value with a valid key.
 *
 * This test verifies that when a valid key ("min_value") and the minimum integer value (INT_MIN) are provided to the DrmJsonObject::add() method, the operation succeeds and returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Initialize test variables (name and value) | name = "min_value", value = INT_MIN | Variables correctly initialized | Should be successful |@n
 * | 02 | Create a DrmJsonObject instance using the default constructor | No input, output: DrmJsonObject instance created | Instance is successfully created | Should be successful |@n
 * | 03 | Invoke add() method with the initialized name and INT_MIN value | name = "min_value", value = INT_MIN, output: result | add() method returns true | Should Pass |@n
 * | 04 | Verify that the API call returns a true value using EXPECT_TRUE | result = true | EXPECT_TRUE assertion passes confirming valid behavior | Should be successful |
 */
TEST_F(DrmJsonObjectTests, ValidAddIntMin) {
    std::cout << "Entering ValidAddIntMin test" << std::endl;
    
    std::string name = "min_value";
    int value = INT_MIN;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
        
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidAddIntMin test" << std::endl;
}
/**
 * @brief Verify that the add() method handles special characters in the key correctly.
 *
 * This test verifies that the DrmJsonObject::add() API correctly processes a key containing special characters 
 * along with a valid integer value. The objective is to ensure the method adds the key-value pair successfully and 
 * returns true, indicating that the handling of special characters in the key does not affect the functionality.
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
 * | Variation / Step | Description                                                                 | Test Data                                                      | Expected Result                                               | Notes               |
 * | :----:           | --------------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------- | ------------------- |
 * | 01               | Initialize test parameters with a key containing special characters and an integer value | name = "special!@#$%^&*()", value = 77                           | Variables initialized with expected values                    | Should be successful|
 * | 02               | Create a DrmJsonObject object using the default constructor                  | None                                                           | Object created successfully                                     | Should be successful|
 * | 03               | Invoke the add() method with the initialized key and value                   | name = "special!@#$%^&*()", value = 77                           | Return value true from add() indicating successful addition, EXPECT_TRUE assertion passes | Should Pass         |
 */
TEST_F(DrmJsonObjectTests, ValidAddSpecialCharacters) {
    std::cout << "Entering ValidAddSpecialCharacters test" << std::endl;
    
    std::string name = "special!@#$%^&*()";
    int value = 77;
    
    std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
    DrmJsonObject jsonObj;
    
    std::cout << "Invoking add() with name: " << name << " and value: " << value << std::endl;
    bool result = jsonObj.add(name, value);
    std::cout << "add() method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidAddSpecialCharacters test" << std::endl;
}
/**
 * @brief Validate that a valid positive long value is successfully added to a JSON object.
 *
 * This test verifies that the DrmJsonObject API correctly adds a valid key associated with a positive long value.
 * The test ensures that the add method returns true when provided with proper inputs.
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
 * | Variation / Step | Description                                                                       | Test Data                                                        | Expected Result                                             | Notes              |
 * | :--------------: | --------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------- | ------------------ |
 * | 01               | Initialize DrmJsonObject instance and define the test inputs                      | key = "positiveKey", value = 123                                   | DrmJsonObject instance created successfully                 | Should be successful |
 * | 02               | Invoke the add() API with the valid key and positive long value                    | object = DrmJsonObject, key = "positiveKey", value = 123, output: result = true | API returns true, and EXPECT_TRUE(result) passes            | Should Pass        |
 */
TEST_F(DrmJsonObjectTests, AddValidPositiveLongValue) {
    std::cout << "Entering AddValidPositiveLongValue test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "positiveKey";
    long value = 123;
    std::cout << "Invoking add with name: \"" << key << "\", value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidPositiveLongValue test" << std::endl;
}
/**
 * @brief Test the addition of a valid negative long value with a valid key.
 *
 * This test verifies that the DrmJsonObject::add method correctly accepts and handles a negative long value when provided with a valid key. It checks that the API returns true and the value is added as expected.
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
 * | Variation / Step | Description                                                       | Test Data                                  | Expected Result            | Notes              |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------ | -------------------------- | ------------------ |
 * | 01               | Instantiate DrmJsonObject                                         | None                                       | Object created             | Should be successful |
 * | 02               | Initialize key ("negativeKey") and negative long value (-456)       | key = negativeKey, value = -456              | Values initialized correctly | Should be successful |
 * | 03               | Invoke add API with key and negative value                        | key = negativeKey, value = -456              | API returns true           | Should Pass        |
 * | 04               | Assert that add method returns true                               | result = true                              | Assertion passes           | Should be successful |
 */
TEST_F(DrmJsonObjectTests, AddValidNegativeLongValue) {
    std::cout << "Entering AddValidNegativeLongValue test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "negativeKey";
    long value = -456;
    std::cout << "Invoking add with name: \"" << key << "\", value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNegativeLongValue test" << std::endl;
}
/**
 * @brief Validate that adding a key with a zero long value returns true.
 *
 * This test verifies that the DrmJsonObject correctly handles the insertion of a zero long value. It adds an entry with the key "zeroKey" and value 0, ensuring that the add method returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DrmJsonObject object | None | Object instantiated successfully | Should be successful |
 * | 02 | Initialize key and value variables | key = "zeroKey", value = 0 | Variables set correctly | Should be successful |
 * | 03 | Call add method with key and value | input: key = "zeroKey", value = 0; output: result | Method returns true and EXPECT_TRUE assertion passes | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddValidZeroLongValue) {
    std::cout << "Entering AddValidZeroLongValue test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "zeroKey";
    long value = 0;
    std::cout << "Invoking add with name: \"" << key << "\", value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidZeroLongValue test" << std::endl;
}
/**
 * @brief Validate adding an entry with maximum long integer value.
 *
 * Tests that the DrmJsonObject.add() method can correctly add an entry with the key "maxValueKey" and the edge-case value LONG_MAX, ensuring that the JSON object handles maximum integer values properly.
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
 * | Variation / Step | Description                                               | Test Data                                        | Expected Result                                   | Notes             |
 * | :--------------: | --------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------- | ----------------- |
 * | 01               | Initialize a DrmJsonObject instance                       | None                                             | Instance should be created                        | Should be successful |
 * | 02               | Define key "maxValueKey" and assign LONG_MAX to value       | key = "maxValueKey", value = LONG_MAX              | Variables key and value set correctly             | Should be successful |
 * | 03               | Call the add() method with the defined key and value        | input key = "maxValueKey", input value = LONG_MAX   | add() should return true                          | Should Pass       |
 * | 04               | Assert that the returned value is true using EXPECT_TRUE    | output result = true                               | EXPECT_TRUE assertion should pass                 | Should Pass       |
 */
TEST_F(DrmJsonObjectTests, AddEntryWithMaxLongValue) {
    std::cout << "Entering AddEntryWithMaxLongValue test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "maxValueKey";
    long value = LONG_MAX;
    std::cout << "Invoking add with name: \"" << key << "\", value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddEntryWithMaxLongValue test" << std::endl;
}
/**
 * @brief Verifies that adding an entry with the minimum long value returns true.
 *
 * This test instantiates a DrmJsonObject and attempts to add an entry where the key is "minValueKey" 
 * and the value is LONG_MIN. The test is used to confirm that the API can handle extreme negative 
 * numerical values correctly without any overflow or underflow issues.
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
 * | Variation / Step | Description                                                          | Test Data                                      | Expected Result                                      | Notes          |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------- | -----------------------------------------------------| -------------- |
 * | 01               | Initialize an instance of DrmJsonObject                               | None                                           | DrmJsonObject instance is created successfully       | Should be successful |
 * | 02               | Set the key to "minValueKey" and value to LONG_MIN                     | key = "minValueKey", value = LONG_MIN          | Input parameters are set correctly                   | Should be successful |
 * | 03               | Invoke the add method with the prepared inputs                         | input1 = key "minValueKey", input2 = LONG_MIN, output1 = result | API returns true; EXPECT_TRUE(result) assertion passes | Should Pass    |
 */
TEST_F(DrmJsonObjectTests, AddEntryWithMinLongValue) {
    std::cout << "Entering AddEntryWithMinLongValue test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "minValueKey";
    long value = LONG_MIN;
    std::cout << "Invoking add with name: \"" << key << "\", value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddEntryWithMinLongValue test" << std::endl;
}
/**
 * @brief Test to verify that adding an entry with an empty key returns false.
 *
 * This test validates that the DrmJsonObject::add API method properly rejects an entry when provided with an empty key. 
 * It checks that the method returns false and that the negative scenario is correctly handled.
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
 * | Variation / Step | Description                                                    | Test Data                         | Expected Result                                               | Notes          |
 * | :--------------: | -------------------------------------------------------------- | --------------------------------- | ------------------------------------------------------------- | -------------- |
 * | 01               | Initialize a DrmJsonObject instance and prepare test data      | N/A                               | DrmJsonObject instance created successfully                   | Should be successful |
 * | 02               | Invoke API method add with an empty key and a valid value        | key = "", value = 100             | Return value should be false and the assertion EXPECT_FALSE passes | Should Fail    |
 */
TEST_F(DrmJsonObjectTests, AddEntryWithEmptyName) {
    std::cout << "Entering AddEntryWithEmptyName test" << std::endl;
    
    DrmJsonObject obj;
    
    std::string key = "";
    long value = 100;
    std::cout << "Invoking add with an empty name, value: " << value << std::endl;
    
    bool result = obj.add(key, value);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddEntryWithEmptyName test" << std::endl;
}
/**
 * @brief Test the add() method of DrmJsonObject with a valid name and a non-empty vector of pointers.
 *
 * This test verifies that the add() method correctly handles a valid input, where the main JSON object is
 * provided with a non-empty vector containing two valid pointers to DrmJsonObject instances. The test ensures 
 * that the method returns true as expected when valid data is provided.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create main DrmJsonObject instance and allocate two valid DrmJsonObject pointers; add them to a vector. | json = default constructor, obj1 = new DrmJsonObject, obj2 = new DrmJsonObject, vector = {obj1, obj2} | Main object and vector are properly initialized with two valid pointer entries. | Should be successful |
 * | 02 | Invoke the add() method on the main DrmJsonObject with a valid name and the non-empty vector. | name = "validArray", vector size = 2 | Method returns true and EXPECT_TRUE assertion passes. | Should Pass |
 * | 03 | Clean up the allocated memory by deleting the two DrmJsonObject instances. | delete obj1, delete obj2 | Memory is successfully freed without errors. | Should be successful |
 */
TEST_F(DrmJsonObjectTests, ValidInputWithNonEmptyVector) {
    std::cout << "Entering ValidInputWithNonEmptyVector test" << std::endl;

    // Create main json object using default constructor
    DrmJsonObject json;
    
    // Create vector with two valid pointers to DrmJsonObject instances
    std::vector<DrmJsonObject*> values;
    DrmJsonObject* obj1 = new DrmJsonObject();
    DrmJsonObject* obj2 = new DrmJsonObject();
    values.push_back(obj1);
    values.push_back(obj2);
    
    std::string name = "validArray";
    std::cout << "Invoking add with name: \"" << name << "\" and vector size: " << values.size() << std::endl;
    
    // Call the add method and capture the result
    bool result = json.add(name, values);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect true based on test plan
    EXPECT_TRUE(result);
    
    // Clean up the allocated memory
    delete obj1;
    delete obj2;
    
    std::cout << "Exiting ValidInputWithNonEmptyVector test" << std::endl;
}
/**
 * @brief Validate that the add method returns true when an empty vector is provided.
 *
 * This test verifies that the DrmJsonObject::add method behaves correctly when passed a valid name ("emptyArray") along with an empty vector of DrmJsonObject pointers. It ensures that the method handles this edge case without error and returns true as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create a DrmJsonObject using the default constructor. | No inputs. | Object is instantiated successfully. | Should be successful |
 * | 02 | Create an empty vector of DrmJsonObject pointers. | No elements. | Empty vector is created. | Should be successful |
 * | 03 | Set the test name to "emptyArray" and log the step. | name = "emptyArray" | Name is set correctly. | Should be successful |
 * | 04 | Call the add method with "emptyArray" and the empty vector, then capture the result. | input: name = "emptyArray", values = empty vector; output: result | add returns true. | Should Pass |
 * | 05 | Validate the result using EXPECT_TRUE. | result = true | The assertion passes confirming the API works as expected. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidInputWithEmptyVector) {
    std::cout << "Entering ValidInputWithEmptyVector test" << std::endl;
    
    // Create main json object using default constructor
    DrmJsonObject json;
    
    // Create an empty vector of DrmJsonObject pointers
    std::vector<DrmJsonObject*> values;
    
    std::string name = "emptyArray";
    std::cout << "Invoking add with name: \"" << name << "\" and empty vector" << std::endl;
    
    // Call the add method and capture the result
    bool result = json.add(name, values);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect true for an empty vector
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidInputWithEmptyVector test" << std::endl;
}
/**
 * @brief Validate that the add method returns false when given an empty name input.
 *
 * This test verifies that the add API of DrmJsonObject correctly handles invalid input,
 * specifically when an empty string is provided as the name while the object vector contains
 * a valid DrmJsonObject pointer. The expected behavior is that the API should return false,
 * indicating the failure to add due to an invalid name.
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
 * | Variation / Step | Description                                                           | Test Data                                         | Expected Result                                                            | Notes           |
 * | :--------------: | --------------------------------------------------------------------- | ------------------------------------------------- | -------------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate a DrmJsonObject using the default constructor.            | None                                              | A json object is created.                                                  | Should be successful |
 * | 02               | Create a vector and add one valid DrmJsonObject pointer to it.          | vector = {DrmJsonObject* obj, allocated}          | Vector contains one valid pointer.                                         | Should be successful |
 * | 03               | Invoke the add method with an empty name and the vector.                | name = "", values = {DrmJsonObject* obj}           | API returns false; EXPECT_FALSE(result) assertion passes.                  | Should Pass     |
 * | 04               | Clean up the dynamically allocated DrmJsonObject pointer.             | delete obj                                        | Memory is freed without leaks.                                           | Should be successful |
 */
TEST_F(DrmJsonObjectTests, InvalidInputEmptyName) {
    std::cout << "Entering InvalidInputEmptyName test" << std::endl;
    
    // Create main json object using default constructor
    DrmJsonObject json;
    
    // Create vector with one valid DrmJsonObject pointer
    std::vector<DrmJsonObject*> values;
    DrmJsonObject* obj = new DrmJsonObject();
    values.push_back(obj);

    std::string name = "";
    std::cout << "Invoking add with empty name and vector size: " << values.size() << std::endl;
    
    // Call the add method and capture the result
    bool result = json.add(name, values);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect false due to empty name
    EXPECT_FALSE(result);
    
    // Clean up allocated memory
    delete obj;
    
    std::cout << "Exiting InvalidInputEmptyName test" << std::endl;
}
/**
 * @brief Verify that adding an element array containing a nullptr fails.
 *
 * This test verifies that adding a vector containing a nullptr element to a DrmJsonObject correctly fails.
 * It ensures that the API robustly checks for invalid inputs by returning false when encountering a nullptr.
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
 * | Variation / Step | Description                                                        | Test Data                                                      | Expected Result                                                          | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------------------- | ------------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize a DrmJsonObject instance using the default constructor. | No inputs; output: json object constructed.                    | DrmJsonObject instance is successfully created.                          | Should be successful |
 * | 02               | Create a vector and add a nullptr element to it.                   | values = vector containing nullptr.                            | Vector is created with a nullptr element.                                | Should be successful |
 * | 03               | Invoke the add method with a valid name and the vector containing nullptr. | name = "arrayWithNull", values = [nullptr].                     | API returns false indicating the operation failed due to invalid input.  | Should Fail   |
 * | 04               | Validate the return value using EXPECT_FALSE.                      | result = false.                                                | The assertion confirms that the API returned false.                      | Should Fail   |
 */
TEST_F(DrmJsonObjectTests, InvalidInputWithNullptrElements) {
    std::cout << "Entering InvalidInputWithNullptrElements test" << std::endl;
    
    // Create main json object using default constructor
    DrmJsonObject json;
    
    // Create vector containing a nullptr element
    std::vector<DrmJsonObject*> values;
    values.push_back(nullptr);
    
    std::string name = "arrayWithNull";
    std::cout << "Invoking add with name: \"" << name << "\" and vector containing nullptr" << std::endl;
    
    // Call the add method and capture the result
    bool result = json.add(name, values);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    
    // Expect false since vector contains a nullptr
    EXPECT_FALSE(result);

    std::cout << "Exiting InvalidInputWithNullptrElements test" << std::endl;
}
/**
 * @brief Verify the retrieval of an existing nested JSON object using the get() method.
 *
 * This test checks that a primary JSON object correctly retrieves a valid nested JSON object under the key "settings". 
 * It validates that the nested JSON contains the expected properties "volume" and "mode", ensuring that the API behaves as expected.
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
 * | 01 | Create primary DrmJsonObject with a nested "settings" JSON object | jsonInput = "{\"settings\": {\"volume\": 50, \"mode\": \"auto\"}}" | Primary object is instantiated successfully with valid nested JSON | Should be successful |
 * | 02 | Invoke get() method to retrieve the nested object into nestedObj | input1 = "settings", output1 = nestedObj | get() returns true indicating successful retrieval | Should Pass |
 * | 03 | Validate the retrieved nested object's contents contain "volume" and "mode" | nestedContent = result of nestedObj.print() | Assert that "volume" and "mode" keys are present in the output string | Should Pass |
 */
TEST_F(DrmJsonObjectTests, RetrieveExistingNestedJSONObject) {
    std::cout << "Entering RetrieveExistingNestedJSONObject test" << std::endl;
    
    // Create primary JSON object with a valid nested JSON object for "settings"
    std::string jsonInput = "{\"settings\": {\"volume\": 50, \"mode\": \"auto\"}}";
    std::cout << "Creating main DrmJsonObject with JSON: " << jsonInput << std::endl;
    DrmJsonObject mainObj(jsonInput);
    
    // Create an empty DrmJsonObject that will be populated by get()
    DrmJsonObject nestedObj;
    
    std::cout << "Invoking get(\"settings\", nestedObj)" << std::endl;
    bool result = mainObj.get("settings", nestedObj);
    std::cout << "Returned value from get(): " << std::boolalpha << result << std::endl;
    
    // Check that the retrieval is successful
    ASSERT_TRUE(result);
    
    // Debug: print the retrieved nested JSON object's content
    std::string nestedContent = nestedObj.print();
    std::cout << "Retrieved nested object content: " << nestedContent << std::endl;
    ASSERT_NE(nestedContent.find("volume"), std::string::npos);
    ASSERT_NE(nestedContent.find("mode"), std::string::npos);
    
    std::cout << "Exiting RetrieveExistingNestedJSONObject test" << std::endl;
}
/**
 * @brief Validate retrieval of a non-existent property from DrmJsonObject
 *
 * This test verifies that when attempting to retrieve a property ("missing") that does not exist in the JSON input, 
 * the get() method returns false and the output object remains unchanged.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                  | Expected Result                                                                                                      | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create main DrmJsonObject with JSON that does not include the "missing" property and an empty outObj | input jsonInput = {"settings": {"brightness": 75}}, outObj = default constructed DrmJsonObject                              | mainObj is constructed correctly, outObj is empty                                                                 | Should be successful |
 * | 02               | Invoke get("missing", outObj) on mainObj to attempt retrieval of the non-existent property      | mainObj, property = missing, outObj; get() is called                                                                     | get() returns false; ASSERT_FALSE(result) passes                                                                    | Should Fail   |
 * | 03               | Verify that outObj remains unchanged after the failed retrieval                                | outObj printed content after get() call                                                                                    | outObj content remains unchanged (empty JSON)                                                                       | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrieveNonExistentProperty) {
    std::cout << "Entering RetrieveNonExistentProperty test" << std::endl;
    
    // Create JSON object without the 'missing' property
    std::string jsonInput = "{\"settings\": {\"brightness\": 75}}";
    std::cout << "Creating main DrmJsonObject with JSON: " << jsonInput << std::endl;
    DrmJsonObject mainObj(jsonInput);
    
    // Create an empty DrmJsonObject which is expected to remain unchanged
    DrmJsonObject outObj;
    
    std::cout << "Invoking get(\"missing\", outObj)" << std::endl;
    bool result = mainObj.get("missing", outObj);
    std::cout << "Returned value from get(): " << std::boolalpha << result << std::endl;
    
    // Check that the retrieval fails
    ASSERT_FALSE(result);
    
    // Optionally check that outObj remains unchanged (printing its content)
    std::string outContent = outObj.print();
    std::cout << "outObj content after failed get: " << outContent << std::endl;
    
    std::cout << "Exiting RetrieveNonExistentProperty test" << std::endl;
}
/**
 * @brief Validate that retrieving a non-object property using get() fails as expected
 *
 * This test checks that when attempting to retrieve a JSON property that exists as a string rather than a nested JSON object, 
 * the get() method returns false and does not modify the output object. This behavior ensures that only properties of the expected type are retrieved.
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
 * | Variation / Step | Description                                                                               | Test Data                                                   | Expected Result                                                                       | Notes           |
 * | :----:           | ----------------------------------------------------------------------------------------- | ----------------------------------------------------------- | -------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a JSON object with a string property "name".                                        | jsonInput = {"name": "TestUser"}                            | DrmJsonObject instance is created with the given JSON string                           | Should be successful |
 * | 02               | Initialize an empty DrmJsonObject to capture the output.                                   | outObj is uninitialized                                     | An empty DrmJsonObject is available                                                    | Should be successful |
 * | 03               | Invoke get() to retrieve the "name" property as a nested JSON object into outObj.           | property key = "name", outObj = empty object                | get() returns false indicating failure as the "name" property is not a nested JSON object | Should Fail      |
 * | 04               | Print the content of outObj to verify that it remains unchanged (empty or invalid).           | Call print() on outObj                                      | Output remains unchanged and indicates that outObj is empty or invalid                   | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrieveNonObjectProperty) {
    std::cout << "Entering RetrieveNonObjectProperty test" << std::endl;
    
    // Create JSON object where "name" exists as a string property
    std::string jsonInput = "{\"name\": \"TestUser\"}";
    std::cout << "Creating main DrmJsonObject with JSON: " << jsonInput << std::endl;
    DrmJsonObject mainObj(jsonInput);
    
    // Create an empty DrmJsonObject to attempt to populate
    DrmJsonObject outObj;
    
    std::cout << "Invoking get(\"name\", outObj)" << std::endl;
    bool result = mainObj.get("name", outObj);
    std::cout << "Returned value from get(): " << std::boolalpha << result << std::endl;
    
    // Expected to fail because "name" is a string, not a nested JSON object
    ASSERT_FALSE(result);
    
    // Print the content of outObj to show it remains unchanged (likely empty or invalid)
    std::string outContent = outObj.print();
    std::cout << "outObj content after get call: " << outContent << std::endl;
    
    std::cout << "Exiting RetrieveNonObjectProperty test" << std::endl;
}
/**
 * @brief Verify that retrieving a value with an empty key fails.
 *
 * This test validates that when an empty key ("") is passed to the get() method of DrmJsonObject, 
 * the API correctly returns false and does not modify the output DrmJsonObject. This scenario is crucial 
 * to ensure that invalid key access is properly handled.
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
 * | Variation / Step | Description                                                            | Test Data                                                                                  | Expected Result                                                  | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ---------------------------------------------------------------- | ------------- |
 * | 01               | Initialize main DrmJsonObject with valid JSON string                   | jsonInput = {"config": {"enabled": true}}                                                  | Object created successfully                                      | Should be successful |
 * | 02               | Create an empty DrmJsonObject for potential population                 | No input arguments                                                                         | Empty object remains unchanged                                   | Should be successful |
 * | 03               | Invoke get() with an empty key and capture the return result           | input key = "", outObj = empty DrmJsonObject                                                 | get() returns false indicating empty key not found                | Should Fail   |
 * | 04               | Verify that outObj was not modified by printing its contents           | No input, output captured from outObj.print()                                               | outObj remains unpopulated (empty or default state)               | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrieveWithEmptyKey) {
    std::cout << "Entering RetrieveWithEmptyKey test" << std::endl;
    
    // Create JSON object that does not define an empty key property
    std::string jsonInput = "{\"config\": {\"enabled\": true}}";
    std::cout << "Creating main DrmJsonObject with JSON: " << jsonInput << std::endl;
    DrmJsonObject mainObj(jsonInput);
    
    // Create an empty DrmJsonObject that is expected to remain unchanged
    DrmJsonObject outObj;
    
    std::cout << "Invoking get(\"\"," " outObj)" << std::endl;
    bool result = mainObj.get("", outObj);
    std::cout << "Returned value from get() with empty key: " << std::boolalpha << result << std::endl;
    
    // Expected to fail because an empty key property doesn't exist
    ASSERT_FALSE(result);
    
    // Print the content of outObj to demonstrate it was not populated
    std::string outContent = outObj.print();
    std::cout << "outObj content after get with empty key: " << outContent << std::endl;
    
    std::cout << "Exiting RetrieveWithEmptyKey test" << std::endl;
}
/**
 * @brief Verify that the DrmJsonObject API retrieves a valid double value for an existing JSON property.
 *
 * This test case creates an instance of DrmJsonObject and attempts to retrieve a double value 
 * from the JSON property named "price". It verifies that the get() method returns true and that the retrieved
 * value is different from its initialized default of 0.0, confirming proper modification.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Instantiate DrmJsonObject object | None | Object instance created | Should be successful |
 * | 02 | Prepare input variables with property name "price" and initial retrievedValue 0.0 | propertyName = price, retrievedValue = 0.0 | Variables are initialized properly | Should be successful |
 * | 03 | Invoke get API to retrieve double value from JSON property | propertyName = price, retrievedValue = 0.0 | get() returns true indicating successful retrieval | Should Pass |
 * | 04 | Validate that the retrieved value is modified (not 0.0) | retrievedValue != 0.0 | EXPECT_NE passes confirming the value was updated | Should be successful |
 */
TEST_F(DrmJsonObjectTests, ValidDoubleValueRetrievalFromJSONProperty) {
    std::cout << "Entering ValidDoubleValueRetrievalFromJSONProperty test" << std::endl;
    
    // Create object of DrmJsonObject using default constructor.
    DrmJsonObject jsonObj;
    std::cout << "Created DrmJsonObject instance." << std::endl;
    
    // Prepare input values.
    std::string propertyName = "price";
    double retrievedValue = 0.0;
    std::cout << "Invoking get with name: " << propertyName << " and initial value: " << retrievedValue << std::endl;
    
    // Invoke the method.
    bool result = jsonObj.get(propertyName, retrievedValue);
    std::cout << "Method get returned: " << std::boolalpha << result << std::endl;
    
    // If retrieval was successful, log the retrieved value.
    if(result) {
        std::cout << "Retrieved double value: " << retrievedValue << std::endl;
    }
    
    // Validate expected outcome.
    EXPECT_TRUE(result);
    // Validate that the retrieved value is not left unmodified.
    EXPECT_NE(retrievedValue, 0.0);
    
    std::cout << "Exiting ValidDoubleValueRetrievalFromJSONProperty test" << std::endl;
}
/**
 * @brief Test retrieval of a non-existent property from DrmJsonObject to ensure proper error handling
 *
 * This test verifies that attempting to retrieve a value using a non-existent property key from a DrmJsonObject instance returns false and leaves the initial value unchanged. The test ensures that the API handles missing keys correctly without causing side effects.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                       | Expected Result                                                     | Notes          |
 * | :----:           | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------------------- | -------------- |
 * | 01               | Instantiate DrmJsonObject using default constructor                                          | None                                                            | Valid instance created                                              | Should be successful |
 * | 02               | Set non-existent property name to "nonexistent" and initialize retrieved value to 0.0         | propertyName = nonexistent, retrievedValue = 0.0                | Variables initialized correctly                                     | Should be successful |
 * | 03               | Invoke get method on jsonObj with the non-existent property and verify the returned value      | input: propertyName = nonexistent, retrievedValue = 0.0, output: expected false | API returns false and retrievedValue remains unchanged             | Should Pass    |
 */
TEST_F(DrmJsonObjectTests, RetrievalOfNonexistentProperty) {
    std::cout << "Entering RetrievalOfNonexistentProperty test" << std::endl;
    
    // Create object of DrmJsonObject using default constructor.
    DrmJsonObject jsonObj;
    std::cout << "Created DrmJsonObject instance." << std::endl;
    
    // Prepare non-existent property key.
    std::string propertyName = "nonexistent";
    double retrievedValue = 0.0;
    std::cout << "Invoking get with name: " << propertyName << " and initial value: " << retrievedValue << std::endl;
    
    // Invoke the method.
    bool result = jsonObj.get(propertyName, retrievedValue);
    std::cout << "Method get returned: " << std::boolalpha << result << std::endl;
    
    // Validate expected outcome.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting RetrievalOfNonexistentProperty test" << std::endl;
}
/**
 * @brief Test invalid retrieval when an empty property name is provided using DrmJsonObject.
 *
 * This test verifies that when an empty property name is used to retrieve a value from a DrmJsonObject instance, the get method returns false, ensuring that the API correctly handles invalid input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DrmJsonObject instance using default constructor. | None | Instance is created successfully. | Should be successful |
 * | 02 | Initialize empty property name and default retrieval value. | propertyName = "", retrievedValue = 0.0 | Values initialized correctly. | Should be successful |
 * | 03 | Invoke the get method with an empty property name to attempt retrieval. | propertyName = "", retrievedValue = 0.0 | API returns false indicating invalid retrieval. | Should Fail |
 * | 04 | Validate that the get method returned false. | result = false expected | EXPECT_FALSE assertion passes. | Should be successful |
 */
TEST_F(DrmJsonObjectTests, InvalidRetrievalWithEmptyPropertyName) {
    std::cout << "Entering InvalidRetrievalWithEmptyPropertyName test" << std::endl;
    
    // Create object of DrmJsonObject using default constructor.
    DrmJsonObject jsonObj;
    std::cout << "Created DrmJsonObject instance." << std::endl;
    
    // Use an empty string for property name.
    std::string propertyName = "";
    double retrievedValue = 0.0;
    std::cout << "Invoking get with an empty property name and initial value: " << retrievedValue << std::endl;
    
    // Invoke the method.
    bool result = jsonObj.get(propertyName, retrievedValue);
    std::cout << "Method get returned: " << std::boolalpha << result << std::endl;
    
    // Validate expected outcome.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InvalidRetrievalWithEmptyPropertyName test" << std::endl;
}
/**
 * @brief Validate that a valid integer property exists and is correctly retrieved from DrmJsonObject.
 *
 * This test verifies that the DrmJsonObject correctly updates an integer property from its default sentinel value
 * after retrieving a valid property ("validInt"). The purpose is to ensure that the get method works as expected for integer values.
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
 * | Variation / Step | Description                                                        | Test Data                                          | Expected Result                                           | Notes            |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------- | --------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate DrmJsonObject using default constructor                | N/A                                                | Object is instantiated                                    | Should be successful |
 * | 02               | Initialize variable 'value' with sentinel value                    | value = -999                                       | 'value' is set to -999                                     | Should be successful |
 * | 03               | Invoke get method with property "validInt" and initial value -999    | property = validInt, input value = -999            | API returns true and updates 'value'                      | Should Pass       |
 * | 04               | Validate that the retrieved value is different from the sentinel     | result from get = true, value != -999               | Assertions pass confirming valid property retrieval       | Should Pass       |
 */
TEST_F(DrmJsonObjectTests, ValidIntegerPropertyExists) {
    std::cout << "Entering ValidIntegerPropertyExists test" << std::endl;
    
    // Create DrmJsonObject using the default constructor.
    DrmJsonObject obj;
    
    // Prepare the value variable with a sentinel value.
    int value = -999;
    std::cout << "Initial value set to: " << value << std::endl;
    
    // Log the method invocation.
    std::cout << "Invoking get with name: \"validInt\" and initial value: " << value << std::endl;
    bool result = obj.get("validInt", value);
    
    // Log the returned result and the updated value.
    std::cout << "Method get returned: " << std::boolalpha << result << " with value updated to: " << value << std::endl;
    
    // Validate that the property exists and its integer value was retrieved.
    EXPECT_TRUE(result);
    // Ensure that the value is updated from the sentinel.
    EXPECT_NE(value, -999);
    
    std::cout << "Exiting ValidIntegerPropertyExists test" << std::endl;
}
/**
 * @brief Validate that retrieving a non-existent property does not modify the initial provided value and returns false.
 *
 * Test verifies that calling get on a non-existent property correctly returns false and leaves the initial value unchanged.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DrmJsonObject using the default constructor | N/A | Object instantiated successfully | Should be successful |
 * | 02 | Set an initial arbitrary value and log the value | initialValue = 1234, value = 1234 | Value is set and logged correctly | Should be successful |
 * | 03 | Invoke get with name "nonexistent" and provided initial value | property = "nonexistent", value = 1234 | get returns false; initial value remains unchanged | Should Pass |
 * | 04 | Validate the returned result and unchanged value using assertions | result = false, value = 1234, initialValue = 1234 | EXPECT_FALSE(result) and EXPECT_EQ(value, initialValue) pass | Should Pass |
 */
TEST_F(DrmJsonObjectTests, PropertyDoesNotExist) {
    std::cout << "Entering PropertyDoesNotExist test" << std::endl;
    
    // Create DrmJsonObject using the default constructor.
    DrmJsonObject obj;
    
    // Set the value variable to an arbitrary initial value.
    int initialValue = 1234;
    int value = initialValue;
    std::cout << "Initial value set to: " << value << std::endl;
    
    // Log the method invocation.
    std::cout << "Invoking get with name: \"nonexistent\" and initial value: " << value << std::endl;
    bool result = obj.get("nonexistent", value);
    
    // Log the returned result and check if value remains unchanged.
    std::cout << "Method get returned: " << std::boolalpha << result << " with value remaining: " << value << std::endl;
    
    // Validate that get returns false and value remains unchanged.
    EXPECT_FALSE(result);
    EXPECT_EQ(value, initialValue);
    
    std::cout << "Exiting PropertyDoesNotExist test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject::get does not modify an integer when the underlying property is not an integer
 *
 * This test checks that when attempting to retrieve a property "stringValue" that is not an integer, the get method returns false and leaves the passed integer value unchanged. This behavior is crucial to ensure type safety and correct error handling within the DrmJsonObject API.
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
 * | Variation / Step | Description                                                                                           | Test Data                                               | Expected Result                                                     | Notes           |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of DrmJsonObject using the default constructor.                                    | None                                                    | DrmJsonObject instance should be created successfully.              | Should be successful |
 * | 02               | Set an arbitrary initial integer value and log it.                                                   | initialValue = 5678, value = 5678                         | Variable is set with the correct initial value.                     | Should be successful |
 * | 03               | Invoke the get method with the name "stringValue" to attempt conversion, expecting failure.           | name = stringValue, value = 5678                          | get method returns false indicating failure to convert non-integer.   | Should Fail     |
 * | 04               | Verify that the returned boolean is false and that the integer value remains unchanged after the call. | result = false, value = 5678                              | Assertions pass confirming false return and unchanged integer value.  | Should be successful |
 */
TEST_F(DrmJsonObjectTests, PropertyIsNotInteger) {
    std::cout << "Entering PropertyIsNotInteger test" << std::endl;
    
    // Create DrmJsonObject using the default constructor.
    DrmJsonObject obj;
    
    // Set the value variable to an arbitrary initial value.
    int initialValue = 5678;
    int value = initialValue;
    std::cout << "Initial value set to: " << value << std::endl;
    
    // Log the method invocation.
    std::cout << "Invoking get with name: \"stringValue\" (expected value is not an integer) and initial value: " << value << std::endl;
    bool result = obj.get("stringValue", value);
    
    // Log the returned result and check if value remains unchanged.
    std::cout << "Method get returned: " << std::boolalpha << result << " with value remaining: " << value << std::endl;
    
    // Validate that get returns false and the integer value remains unchanged.
    EXPECT_FALSE(result);
    EXPECT_EQ(value, initialValue);
    
    std::cout << "Exiting PropertyIsNotInteger test" << std::endl;
}
/**
 * @brief Verifies that providing an empty property name to the get API does not alter the value.
 *
 * This test confirms that when an empty property name is passed to the get method of a DrmJsonObject along with an initial value, the method returns false and the value remains unchanged. This check is vital to ensure the object's behavior is correct in handling invalid or empty input strings.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 061@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create a DrmJsonObject instance using the default constructor | None | Object is created successfully | Should be successful |@n
 * | 02 | Set initial value for testing by assigning 91011 to both initialValue and value | initialValue = 91011, value = 91011 | The value is set correctly | Should be successful |@n
 * | 03 | Invoke the get method with an empty property name and the initial value | property name = "", value = 91011 | get returns false and value remains unchanged (91011) | Should Pass |@n
 * | 04 | Validate the output by asserting that get returns false and the value stays equal to the initial value | result = false, value = 91011 | EXPECT_FALSE(result) and EXPECT_EQ(value, initialValue) pass | Should Pass |
 */
TEST_F(DrmJsonObjectTests, EmptyPropertyNameProvided) {
    std::cout << "Entering EmptyPropertyNameProvided test" << std::endl;
    
    // Create DrmJsonObject using the default constructor.
    DrmJsonObject obj;
    
    // Set the value variable to an arbitrary initial value.
    int initialValue = 91011;
    int value = initialValue;
    std::cout << "Initial value set to: " << value << std::endl;
    
    // Log the method invocation.
    std::cout << "Invoking get with an empty property name and initial value: " << value << std::endl;
    bool result = obj.get("", value);
    
    // Log the returned result and check if value remains unchanged.
    std::cout << "Method get returned: " << std::boolalpha << result << " with value remaining: " << value << std::endl;
    
    // Validate that get returns false and the value remains unchanged.
    EXPECT_FALSE(result);
    EXPECT_EQ(value, initialValue);
    
    std::cout << "Exiting EmptyPropertyNameProvided test" << std::endl;
}
/**
 * @brief Verify that the get method successfully retrieves the expected value for an existing valid property.
 *
 * This test validates that when a valid property name is provided to the get method, it returns true and updates the value correctly. It ensures proper functioning of the property retrieval mechanism.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create DrmJsonObject instance using the default constructor. | Object instantiated | Object created successfully | Should be successful |
 * | 02 | Call get method with propertyName 'validProperty' and initial value "". | propertyName = validProperty, value (initial) = "" | Returns true and updates value to expectedValue | Should Pass |
 * | 03 | Assert that the get method returns true and value equals "expectedValue". | result = true, value = expectedValue | Assertions pass | Should Pass |
 */
TEST_F(DrmJsonObjectTests, RetrieveExistingPropertyTest) {
    std::cout << "Entering RetrieveExistingPropertyTest test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    std::string value = "";
    std::string propertyName = "validProperty";
    std::cout << "Invoking get with propertyName: '" << propertyName << "' and initial value: '" << value << "'" << std::endl;
    
    bool result = obj.get(propertyName, value);
    std::cout << "Method get returned: " << (result ? "true" : "false") << std::endl;
    std::cout << "Retrieved value: '" << value << "'" << std::endl;
    
    EXPECT_TRUE(result);
    EXPECT_EQ(value, "expectedValue");
    
    std::cout << "Exiting RetrieveExistingPropertyTest test" << std::endl;
}
/**
 * @brief Verify retrieving a non-existing property returns false.
 *
 * The test verifies that attempting to retrieve a property that does not exist in a DrmJsonObject instance using the get method returns false and leaves the provided value unchanged. This behavior confirms that the API correctly handles missing keys.
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
 * | Variation / Step | Description                                             | Test Data                                                            | Expected Result                                                 | Notes            |
 * | :--------------: | ------------------------------------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------- | ---------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor | No input parameters, output: default constructed DrmJsonObject         | Instance is successfully created                                  | Should be successful |
 * | 02               | Invoke get() method with a non-existing property and an initial empty string | propertyName = nonExistingProperty, value = ""                         | Returns false and value remains ""                               | Should Fail      |
 * | 03               | Verify the API response using assertions               | result = false, value = ""                                             | EXPECT_FALSE(result) passes, EXPECT_EQ(value, "") passes           | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrieveNonExistingPropertyTest) {
    std::cout << "Entering RetrieveNonExistingPropertyTest test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    std::string value = "";
    std::string propertyName = "nonExistingProperty";
    std::cout << "Invoking get with propertyName: '" << propertyName << "' and initial value: '" << value << "'" << std::endl;
    
    bool result = obj.get(propertyName, value);
    std::cout << "Method get returned: " << (result ? "true" : "false") << std::endl;
    std::cout << "Retrieved value: '" << value << "'" << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_EQ(value, "");
    
    std::cout << "Exiting RetrieveNonExistingPropertyTest test" << std::endl;
}
/**
 * @brief Validate behavior when retrieving a property with an empty property name.
 *
 * This test verifies that calling the get method with an empty property name
 * does not retrieve any value and correctly returns false. It ensures that the API
 * handles empty input parameters as expected.
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
 * | Variation / Step | Description                                                           | Test Data                                 | Expected Result                                            | Notes         |
 * | :--------------: | --------------------------------------------------------------------- | ----------------------------------------- | ---------------------------------------------------------- | ------------- |
 * | 01               | Initialize the DrmJsonObject instance and define input parameters     | propertyName = "", value = ""               | Instance created and parameters set successfully           | Should be successful |
 * | 02               | Invoke the get method with an empty property name and validate results  | propertyName = "", value = ""               | Return value is false and value remains ""                 | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, RetrieveEmptyNamePropertyTest) {
    std::cout << "Entering RetrieveEmptyNamePropertyTest test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    std::string value = "";
    std::string propertyName = "";
    std::cout << "Invoking get with empty propertyName: '" << propertyName << "' and initial value: '" << value << "'" << std::endl;
    
    bool result = obj.get(propertyName, value);
    std::cout << "Method get returned: " << (result ? "true" : "false") << std::endl;
    std::cout << "Retrieved value: '" << value << "'" << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_EQ(value, "");
    
    std::cout << "Exiting RetrieveEmptyNamePropertyTest test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject correctly retrieves an array of JSON objects from a valid JSON string.
 *
 * This test ensures that when a valid JSON string containing an "items" array with two JSON objects is provided,
 * the DrmJsonObject instance can successfully parse the JSON string and extract the two JSON objects using the get() method.
 * This is essential to confirm that the API correctly handles valid JSON arrays and returns the expected objects.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                   | Expected Result                                                           | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | ------------- |
 * | 01               | Construct a valid JSON string and create a DrmJsonObject instance from it.                   | jsonStr = "{\"items\":[{\"key\":\"value1\"}, {\"key\":\"value2\"}]}"                                        | DrmJsonObject is successfully constructed.                                | Should be successful |
 * | 02               | Invoke the get() method with the key "items" to extract the array of JSON objects.           | inputKey = "items", outputVector = empty vector                                                             | get() returns true indicating the key exists and extraction is successful. | Should Pass   |
 * | 03               | Check that the retrieved outputVector contains exactly two JSON object elements.             | expectedResult = true, vectorSize = 2                                                                       | get() returns true and outputVector.size() equals 2.                      | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrieveValidArrayOfJSONObjects) {
    std::cout << "Entering RetrieveValidArrayOfJSONObjects test" << std::endl;
    
    // Create a JSON object with an "items" array containing two valid JSON objects.
    std::string jsonStr = "{\"items\":[{\"key\":\"value1\"}, {\"key\":\"value2\"}]}";
    std::cout << "Constructing DrmJsonObject with json string: " << jsonStr << std::endl;
    DrmJsonObject drmObject(jsonStr);
    
    std::vector<DrmJsonObject> outputVector;
    std::cout << "Invoking get(\"items\", outputVector)" << std::endl;
    bool result = drmObject.get("items", outputVector);
    std::cout << "get() returned: " << std::boolalpha << result << std::endl;
    
    // Verify that get() succeeded and the output vector contains two JSON object elements.
    EXPECT_TRUE(result);
    std::cout << "Retrieved outputVector size: " << outputVector.size() << std::endl;
    EXPECT_EQ(outputVector.size(), 2);
    
    std::cout << "Exiting RetrieveValidArrayOfJSONObjects test" << std::endl;
}
/**
 * @brief Verify that retrieving a non-existent property from a JSON object fails.
 *
 * This test checks that when the get() method is called with a property name ("nonexistent")
 * that does not exist in the JSON object, the method returns false and the output vector remains empty.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                         | Expected Result                                         | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------------- | --------------- |
 * | 01               | Construct DrmJsonObject instance with a JSON string that does not contain the key "nonexistent" | jsonStr = {"items":[{"key":"value1"}]}                             | DrmJsonObject is successfully instantiated              | Should be successful |
 * | 02               | Invoke get() method on the DrmJsonObject with key "nonexistent" and an empty vector             | property = "nonexistent", outputVector = empty                     | get() returns false                                     | Should Fail     |
 * | 03               | Verify that outputVector remains empty after the get() call                                   | No additional inputs                                              | outputVector.size() equals 0                              | Should be successful |
 */
TEST_F(DrmJsonObjectTests, RetrievePropertyDoesNotExist) {
    std::cout << "Entering RetrievePropertyDoesNotExist test" << std::endl;
    
    // Create a JSON object that does not contain the key "nonexistent".
    std::string jsonStr = "{\"items\":[{\"key\":\"value1\"}]}";
    std::cout << "Constructing DrmJsonObject with json string: " << jsonStr << std::endl;
    DrmJsonObject drmObject(jsonStr);
    
    std::vector<DrmJsonObject> outputVector;
    std::cout << "Invoking get(\"nonexistent\", outputVector)" << std::endl;
    bool result = drmObject.get("nonexistent", outputVector);
    std::cout << "get() returned: " << std::boolalpha << result << std::endl;
    
    // Verify that get() returns false and outputVector remains empty.
    EXPECT_FALSE(result);
    std::cout << "Retrieved outputVector size: " << outputVector.size() << std::endl;
    EXPECT_EQ(outputVector.size(), 0);
    
    std::cout << "Exiting RetrievePropertyDoesNotExist test" << std::endl;
}
/**
 * @brief Test the behavior of the get() method when retrieving a property that is not an array.
 *
 * This test verifies that calling the get() function on a DrmJsonObject with a property whose value is a string returns false and that the output vector remains empty. This is important to ensure that the method correctly handles non-array data types when an array is expected.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                           | Expected Result                                                                      | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Construct JSON string with a property "item" set to "stringValue".                           | jsonStr = "{\"item\":\"stringValue\"}"                              | JSON string is constructed correctly                                                  | Should be successful |
 * | 02               | Instantiate DrmJsonObject using the constructed JSON string.                                | Input: jsonStr = "{\"item\":\"stringValue\"}"                        | Instance of DrmJsonObject is created without errors                                  | Should be successful |
 * | 03               | Invoke the get() method with key "item" and an empty output vector.                           | key = "item", outputVector = empty vector                           | Method returns false and output vector remains empty                                  | Should Fail   |
 * | 04               | Validate that the get() method returned false using EXPECT_FALSE.                           | result from get() = false                                           | EXPECT_FALSE(result) check passes                                                     | Should Pass   |
 * | 05               | Verify that the output vector remains empty by checking its size using EXPECT_EQ.             | outputVector.size() = 0                                               | EXPECT_EQ(outputVector.size(), 0) check passes                                          | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, RetrievePropertyNotArray) {
    std::cout << "Entering RetrievePropertyNotArray test" << std::endl;
    
    // Create a JSON object with a property "item" that is a single JSON object/primitive (here, a string).
    std::string jsonStr = "{\"item\":\"stringValue\"}";
    std::cout << "Constructing DrmJsonObject with json string: " << jsonStr << std::endl;
    DrmJsonObject drmObject(jsonStr);
    
    std::vector<DrmJsonObject> outputVector;
    std::cout << "Invoking get(\"item\", outputVector)" << std::endl;
    bool result = drmObject.get("item", outputVector);
    std::cout << "get() returned: " << std::boolalpha << result << std::endl;
    
    // Verify that get() returns false and the outputVector remains empty.
    EXPECT_FALSE(result);
    std::cout << "Retrieved outputVector size: " << outputVector.size() << std::endl;
    EXPECT_EQ(outputVector.size(), 0);
    
    std::cout << "Exiting RetrievePropertyNotArray test" << std::endl;
}
/**
 * @brief Verify that the API returns false when an empty property name is used
 *
 * This test verifies that invoking the get() method on a DrmJsonObject with an empty property name correctly returns false. The test ensures the API handles JSON objects with empty keys appropriately without causing unexpected behavior.
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
 * | Variation / Step | Description                                                                                  | Test Data                                         | Expected Result                                                         | Notes           |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------------------- | --------------- |
 * | 01               | Construct DrmJsonObject using a JSON string with an empty property name and null value         | jsonStr = "{\"\":null}"                           | DrmJsonObject is successfully constructed                               | Should be successful |
 * | 02               | Invoke get() method with an empty property name to retrieve value into outputVector            | input property = "", outputVector = empty vector  | get() returns false and the EXPECT_FALSE assertion passes               | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, RetrieveEmptyPropertyName) {
    std::cout << "Entering RetrieveEmptyPropertyName test" << std::endl;
    
    // Create a JSON object that contains an empty key. In this example, the value is null.
    std::string jsonStr = "{\"\":null}";
    std::cout << "Constructing DrmJsonObject with json string: " << jsonStr << std::endl;
    DrmJsonObject drmObject(jsonStr);
    
    std::vector<DrmJsonObject> outputVector;
    std::cout << "Invoking get(\"\", outputVector)" << std::endl;
    bool result = drmObject.get("", outputVector);
    std::cout << "get() returned: " << std::boolalpha << result << std::endl;
    
    // Verify that get() returns false.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting RetrieveEmptyPropertyName test" << std::endl;
}
/**
 * @brief Verify retrieval of an existing JSON array with multiple elements.
 *
 * This test checks the behavior of DrmJsonObject::get() when an existing key associated with a JSON array is provided.
 * It validates that the function returns true and that the provided array vector is populated with the expected elements.@n
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
 * | Variation / Step | Description                                                        | Test Data                                                   | Expected Result                                                                                                      | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | ----------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Instantiate DrmJsonObject and initialize an empty vector.          | DrmJsonObject: default constructor, values: empty vector    | Object successfully created and vector initialized as empty.                                                       | Should be successful |
 * | 02               | Define the array key and invoke get() to retrieve the JSON array.    | name = "myArray", values = empty vector                     | get() method returns true indicating that the array key exists and retrieval is successful.                          | Should Pass        |
 * | 03               | Validate the result and verify the vector contents.                | result = true, values expected: "value1", "value2", "value3"  | EXPECT_TRUE(result) is satisfied and vector contains exactly three expected elements.                              | Should Pass        |
 */
TEST_F(DrmJsonObjectTests, GetExistingArrayMultiple) {
    std::cout << "Entering GetExistingArrayMultiple test" << std::endl;

    // Create an instance of DrmJsonObject using its default constructor
    DrmJsonObject obj;
    std::vector<std::string> values;
    std::cout << "Created DrmJsonObject object. Initial values vector size: " << values.size() << std::endl;

    // Define the name and expected values
    std::string name = "myArray";
    std::cout << "Calling get() with name: " << name << std::endl;
    bool result = obj.get(name, values);
    std::cout << "Method get() returned: " << std::boolalpha << result << std::endl;
    std::cout << "Values vector size after get(): " << values.size() << std::endl;

    // Print all values retrieved
    for (size_t i = 0; i < values.size(); i++) {
        std::cout << "Retrieved value[" << i << "]: " << values[i] << std::endl;
    }

    // Validate the results: Expected to return true and populate with three elements: "value1", "value2", "value3"
    EXPECT_TRUE(result);

    std::cout << "Exiting GetExistingArrayMultiple test" << std::endl;
}
/**
 * @brief Verify that retrieving a non-existent element returns false and leaves the output vector empty.
 *
 * This test verifies that calling the get() method with a key that does not exist in the DrmJsonObject correctly returns false and does not modify the provided output vector. This ensures that the API behaves as expected in negative scenarios.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 070
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate a DrmJsonObject object. | API call: DrmJsonObject() | Object instantiated successfully | Should be successful |
 * | 02 | Invoke the get() method using a non-existent key "nonExistentKey" with an empty output vector. | input: name = nonExistentKey, values = empty vector; output: result variable to capture return value | get() returns false indicating key not found | Should Fail |
 * | 03 | Validate that the returned result is false and the output vector remains empty. | result = false, values.size() = 0 | EXPECT_FALSE(result) passes and EXPECT_EQ(values.size(), 0u) passes | Should be successful |
 */
TEST_F(DrmJsonObjectTests, GetNonExistentElement) {
    std::cout << "Entering GetNonExistentElement test" << std::endl;

    // Create an instance of DrmJsonObject using its default constructor
    DrmJsonObject obj;
    std::vector<std::string> values;
    std::cout << "Created DrmJsonObject object. Initial values vector size: " << values.size() << std::endl;

    // Define the non-existent key name
    std::string name = "nonExistentKey";
    std::cout << "Calling get() with name: " << name << std::endl;
    bool result = obj.get(name, values);
    std::cout << "Method get() returned: " << std::boolalpha << result << std::endl;
    std::cout << "Values vector size after get(): " << values.size() << std::endl;

    // Validate the results: Expected to return false and leave the vector empty
    EXPECT_FALSE(result);
    EXPECT_EQ(values.size(), 0u);

    std::cout << "Exiting GetNonExistentElement test" << std::endl;
}
/**
 * @brief Verify that calling get() with an empty name returns false and leaves the values vector empty
 *
 * This test verifies that when the API DrmJsonObject::get is invoked with an empty name string, it correctly
 * returns false and does not modify the values vector. This behavior is crucial for ensuring the robustness
 * of the API against invalid inputs.@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 071@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                                             | Expected Result                                                       | Notes           |
 * | :----:           | ---------------------------------------------------------------- | --------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------- |
 * | 01               | Create a DrmJsonObject instance and initialize a values vector     | input: none, output: instance created, values vector = empty          | Instance is created with an empty values vector                        | Should be successful |
 * | 02               | Define an empty name                                             | input: name = ""                                                      | Empty name string is defined                                          | Should be successful |
 * | 03               | Call get() method with the empty name                            | input: name = "", output: result, values vector remains empty           | get() returns false and the values vector remains empty                | Should Pass     |
 * | 04               | Validate the output using assertions                             | input: result, values vector size                                      | EXPECT_FALSE(result) and EXPECT_EQ(values.size(), 0u) are true          | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, GetWithEmptyName) {
    std::cout << "Entering GetWithEmptyName test" << std::endl;

    // Create an instance of DrmJsonObject using its default constructor
    DrmJsonObject obj;
    std::vector<std::string> values;
    std::cout << "Created DrmJsonObject object. Initial values vector size: " << values.size() << std::endl;

    // Define an empty name
    std::string name = "";
    std::cout << "Calling get() with an empty name" << std::endl;
    bool result = obj.get(name, values);
    std::cout << "Method get() returned: " << std::boolalpha << result << std::endl;
    std::cout << "Values vector size after get(): " << values.size() << std::endl;

    // Validate the results: Expected to return false and leave the vector empty
    EXPECT_FALSE(result);
    EXPECT_EQ(values.size(), 0u);

    std::cout << "Exiting GetWithEmptyName test" << std::endl;
}
/**
 * @brief Verify that isArray returns true for a valid array key.
 *
 * This test verifies that the DrmJsonObject::isArray method correctly identifies the key "arrayKey" as an array. It validates that the API returns true for an array key, ensuring correct behavior for valid JSON array retrieval.
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
 * | Variation / Step | Description                                                              | Test Data                                         | Expected Result                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------- | ----------------------------------------------------------- | --------------- |
 * | 01               | Instantiate DrmJsonObject and initialize key variable                    | N/A, key = "arrayKey"                             | Object is created successfully and key variable is set      | Should be successful |
 * | 02               | Invoke isArray method with the provided key                              | input: key = "arrayKey", output: result (bool)     | isArray returns true                                        | Should Pass     |
 * | 03               | Verify the result using EXPECT_TRUE                                      | expected: result = true                           | The assertion passes as the result is true                  | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, IsArray_ReturnTrue_ForArrayKey) {
    std::cout << "Entering IsArray_ReturnTrue_ForArrayKey test" << std::endl;
    // Create object using default constructor
    DrmJsonObject obj;

    std::string key = "arrayKey";

    std::cout << "Invoking isArray with key: " << key << std::endl;
    bool result = obj.isArray(key);
    std::cout << "Method isArray returned: " << (result ? "true" : "false")
              << " for key: " << key << std::endl;

    EXPECT_TRUE(result);

    std::cout << "Exiting IsArray_ReturnTrue_ForArrayKey test" << std::endl;
}
/**
 * @brief Verifies that isArray returns false for a non-array key.
 *
 * This test checks that when a key not associated with an array ("nonArrayKey") is provided to the isArray method of DrmJsonObject, the method correctly returns false. The objective is to validate that the API correctly identifies keys that do not correspond to JSON arrays.
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
 * | Variation / Step | Description                                                         | Test Data                                      | Expected Result                                                               | Notes           |
 * | :--------------: | --------------------------------------------------------------------| -----------------------------------------------| ----------------------------------------------------------------------------- | --------------- |
 * |      01        | Instantiate a DrmJsonObject using its default constructor            | No input (default constructor)                 | Object is created successfully                                                | Should be successful |
 * |      02        | Define a non-array key                                               | key = nonArrayKey                              | key holds the value "nonArrayKey"                                             | Should be successful |
 * |      03        | Invoke the isArray method with the non-array key                     | input: key = nonArrayKey, output: result expected = false | isArray returns false; assertion EXPECT_FALSE passes                           | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, IsArray_ReturnFalse_ForNonArrayKey) {
    std::cout << "Entering IsArray_ReturnFalse_ForNonArrayKey test" << std::endl;
    // Create object using default constructor
    DrmJsonObject obj;

    std::string key = "nonArrayKey";

    std::cout << "Invoking isArray with key: " << key << std::endl;
    bool result = obj.isArray(key);
    std::cout << "Method isArray returned: " << (result ? "true" : "false") 
              << " for key: " << key << std::endl;

    EXPECT_FALSE(result);

    std::cout << "Exiting IsArray_ReturnFalse_ForNonArrayKey test" << std::endl;
}
/**
 * @brief Verify that isArray returns false when called with an empty key.
 *
 * This test verifies that the isArray method of the DrmJsonObject class behaves correctly by returning false when provided with an empty key. This ensures that the method handles empty input parameters appropriately.
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
 * | Variation / Step | Description                                                       | Test Data         | Expected Result                                              | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ----------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Create a DrmJsonObject instance and invoke isArray with empty key. | key = ""          | Method returns false and EXPECT_FALSE assertion passes.    | Should Pass |
 */
TEST_F(DrmJsonObjectTests, IsArray_ReturnFalse_ForEmptyKey) {
    std::cout << "Entering IsArray_ReturnFalse_ForEmptyKey test" << std::endl;
    // Create object using default constructor
    DrmJsonObject obj;

    std::string key = "";
    std::cout << "Invoking isArray with an empty key" << std::endl;
    bool result = obj.isArray(key);
    std::cout << "Method isArray returned: " << (result ? "true" : "false") 
              << " for an empty key" << std::endl;

    // Expectation: empty key should return false
    EXPECT_FALSE(result);

    std::cout << "Exiting IsArray_ReturnFalse_ForEmptyKey test" << std::endl;
}
/**
 * @brief Validate that the isNumber API returns true when passed a valid integer field name.
 *
 * This test verifies that when a valid numeric field (represented by an integer value) is provided to the isNumber API,
 * the function correctly identifies it as a numeric field and returns true. This is important to ensure proper type validation.
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
 * | Variation / Step | Description                                                         | Test Data                                   | Expected Result                                                  | Notes         |
 * | :----:           | ------------------------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------------------- | ------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor.      | N/A                                         | Object is created successfully.                                  | Should be successful |
 * | 02               | Define the field name for testing.                                  | fieldName = "numberField"                   | Field name variable is set with the value "numberField".           | Should be successful |
 * | 03               | Call the isNumber API with the defined field name and capture result. | input1 = fieldName: "numberField"           | API returns true indicating the field is numeric.                | Should Pass   |
 * | 04               | Validate the returned value using EXPECT_TRUE assertion.            | result = true                                | EXPECT_TRUE passes confirming the API behaved as expected.         | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, FieldWithIntegerNumericValue) {
    std::cout << "Entering FieldWithIntegerNumericValue test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    
    // Define the field name
    std::string fieldName = "numberField";
    std::cout << "Invoking isNumber with input: " << fieldName << std::endl;
    
    // Call method isNumber and capture result
    bool result = obj.isNumber(fieldName);
    std::cout << "Returned value from isNumber: " << std::boolalpha << result << std::endl;
    
    // Validate the returned value; expecting true
    EXPECT_TRUE(result);
    
    std::cout << "Exiting FieldWithIntegerNumericValue test" << std::endl;
}
/**
 * @brief Verify that calling isNumber with a non-existent field returns false.
 *
 * This test verifies that the DrmJsonObject::isNumber method correctly identifies that a non-existent field 
 * is not a number by returning false. It ensures the robustness of the method when dealing with uninitialized or missing fields.
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
 * | Variation / Step | Description                                                                 | Test Data                                           | Expected Result                                           | Notes           |
 * | :----:           | --------------------------------------------------------------------------- | --------------------------------------------------- | --------------------------------------------------------- | --------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor               | No input, output: obj instance                      | Object is created successfully                            | Should be successful |
 * | 02               | Invoke isNumber with a non-existent field "nonExistentField"                | input: fieldName = nonExistentField, output: result   | isNumber returns false                                    | Should Pass     |
 * | 03               | Validate the returned result using EXPECT_FALSE                             | result = false                                      | Assertion passes confirming that result is false          | Should be successful |
 */
TEST_F(DrmJsonObjectTests, NonExistentField) {
    std::cout << "Entering NonExistentField test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    
    // Define a non-existent field name
    std::string fieldName = "nonExistentField";
    std::cout << "Invoking isNumber with input: " << fieldName << std::endl;
    
    // Call method isNumber and capture result
    bool result = obj.isNumber(fieldName);
    std::cout << "Returned value from isNumber: " << std::boolalpha << result << std::endl;
    
    // Validate the returned value; expecting false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting NonExistentField test" << std::endl;
}
/**
 * @brief Validate that isNumber method returns false for an empty field name.
 *
 * This test verifies that when the isNumber method is invoked with an empty string as the field name,
 * it correctly returns false. It ensures the API handles empty field inputs by returning a boolean false,
 * thus preventing further validation on invalid field names.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 077@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                              | Test Data                                                           | Expected Result                                                                              | Notes           |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an instance of DrmJsonObject using the default constructor        | No input, output: obj created                                         | Object created successfully                                                                  | Should be successful |
 * | 02               | Define an empty field name                                               | fieldName = ""                                                       | fieldName is assigned an empty string                                                        | Should be successful |
 * | 03               | Invoke isNumber function with an empty field name                        | input: fieldName = "", output: bool result                           | result is false; EXPECT_FALSE(result) assertion passes                                       | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, EmptyFieldName) {
    std::cout << "Entering EmptyFieldName test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    
    // Define an empty field name
    std::string fieldName = "";
    std::cout << "Invoking isNumber with input: <empty string>" << std::endl;
    
    // Call method isNumber and capture result
    bool result = obj.isNumber(fieldName);
    std::cout << "Returned value from isNumber: " << std::boolalpha << result << std::endl;
    
    // Validate the returned value; expecting false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting EmptyFieldName test" << std::endl;
}
/**
 * @brief Test to verify that isNumber returns false for field names with special characters
 *
 * This test checks that an instance of DrmJsonObject correctly identifies that a field name containing special characters (e.g., "@#$%") is not a valid numerical value. The isNumber method is invoked with such input and the result is verified against the expected output.
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
 * | Variation / Step | Description                                                                     | Test Data                                   | Expected Result                                                            | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------- | ------------------------------------------- | -------------------------------------------------------------------------- | --------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor.                | None                                        | Object is instantiated successfully.                                     | Should be successful |
 * | 02               | Define a field name with special characters.                                  | fieldName = "@#$%"                          | Field name is set to "@#$%".                                               | Should be successful |
 * | 03               | Invoke the isNumber API with the special character field name.                  | input: fieldName = "@#$%"                    | API returns false indicating the field is not a number.                    | Should Pass     |
 * | 04               | Validate the returned value using an assertion.                               | output: result = false                       | EXPECT_FALSE assertion passes confirming the method works as expected.     | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, FieldNameWithSpecialCharacters) {
    std::cout << "Entering FieldNameWithSpecialCharacters test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject obj;
    
    // Define a field name with special characters
    std::string fieldName = "@#$%";
    std::cout << "Invoking isNumber with input: " << fieldName << std::endl;
    
    // Call method isNumber and capture result
    bool result = obj.isNumber(fieldName);
    std::cout << "Returned value from isNumber: " << std::boolalpha << result << std::endl;
    
    // Validate the returned value; expecting false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting FieldNameWithSpecialCharacters test" << std::endl;
}
/**
 * @brief Test that verifies a valid JSON object name returns true.
 *
 * This test creates an instance of DrmJsonObject and verifies that calling the isObject method
 * with a key "ValidObjectKey" returns true. It ensures that a valid JSON object key is correctly recognized.
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
 * | 01 | Create DrmJsonObject instance using default constructor | None | Instance created successfully | Should be successful |
 * | 02 | Initialize key variable with value "ValidObjectKey" | key = "ValidObjectKey" | Key variable set correctly | Should be successful |
 * | 03 | Invoke isObject method on instance with the key | input: key = ValidObjectKey, output: result (boolean) | Method returns true indicating valid JSON object name | Should Pass |
 * | 04 | Assert that the result is true using EXPECT_TRUE | result = true | EXPECT_TRUE passes as result is true | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidJsonObjectName_ReturnsTrue) {
    std::cout << "Entering ValidJsonObjectName_ReturnsTrue test" << std::endl;
    
    // Create an instance of DrmJsonObject using default constructor
    DrmJsonObject jsonObj;
    std::string key = "ValidObjectKey";
    std::cout << "Invoking isObject with key: " << key << std::endl;
    
    // Invoke the method
    bool result = jsonObj.isObject(key);
    std::cout << "isObject returned: " << std::boolalpha << result << std::endl;
    
    // Debug internal state (if available, here we log the assumed state)
    std::cout << "Internal state: DrmJsonObject instance created with default parameters." << std::endl;
    
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidJsonObjectName_ReturnsTrue test" << std::endl;
}
/**
 * @brief Validate that isObject returns false for a key associated with a non-object value.
 *
 * This test verifies that when the isObject method of the DrmJsonObject class is invoked with a key that exists but does not map to an object, the method correctly returns false. This ensures that the API properly distinguishes between object and non-object JSON values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 080@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                                     | Expected Result                                                     | Notes         |
 * | :----:           | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of DrmJsonObject and initialize the key variable with "NonObjectValueKey".  | jsonObj = default, key = NonObjectValueKey                      | Instance of DrmJsonObject created and key initialized successfully    | Should be successful |
 * | 02               | Invoke the isObject method using the key "NonObjectValueKey".                                 | input key = NonObjectValueKey                                  | API returns false indicating that the key is associated with a non-object value | Should Pass   |
 * | 03               | Verify that EXPECT_FALSE assertion confirms that the returned value is false.                | result expected = false                                       | Assertion passes verifying the API returns false                      | Should be successful |
 */
TEST_F(DrmJsonObjectTests, NonObjectValueKey_ReturnsFalse) {
    std::cout << "Entering NonObjectValueKey_ReturnsFalse test" << std::endl;
    
    // Create an instance of DrmJsonObject using default constructor
    DrmJsonObject jsonObj;
    std::string key = "NonObjectValueKey";
    std::cout << "Invoking isObject with key: " << key << std::endl;
    
    // Invoke the method
    bool result = jsonObj.isObject(key);
    std::cout << "isObject returned: " << std::boolalpha << result << std::endl;
    
    // Debug internal state (assuming state change due to key association)
    std::cout << "Internal state: JSON key '" << key << "' exists but is not an object." << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting NonObjectValueKey_ReturnsFalse test" << std::endl;
}
/**
 * @brief Verify that isObject returns false for a non-existing key
 *
 * This test is designed to ensure that when a key that does not exist in the DrmJsonObject is provided to the isObject method, the method returns false as expected. This confirms the correct handling of invalid key queries.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 081@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                      | Test Data: input arguments and their value                                  | Expected Result: Return value and Assertion Outcome                      | Notes             |
 * | :--------------: | ---------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------------------------------------------------------------------- | ----------------- |
 * | 01               | Call isObject with non-existing key "NonExistingKey"             | jsonObj = instance of DrmJsonObject, key = "NonExistingKey"                   | isObject returns false and EXPECT_FALSE(result) assertion passes            | Should Pass       |
 */
TEST_F(DrmJsonObjectTests, NonExistingKey_ReturnsFalse) {
    std::cout << "Entering NonExistingKey_ReturnsFalse test" << std::endl;
    
    // Create an instance of DrmJsonObject using default constructor
    DrmJsonObject jsonObj;
    std::string key = "NonExistingKey";
    std::cout << "Invoking isObject with key: " << key << std::endl;
    
    // Invoke the method
    bool result = jsonObj.isObject(key);
    std::cout << "isObject returned: " << std::boolalpha << result << std::endl;
    
    // Debug internal state
    std::cout << "Internal state: JSON key '" << key << "' does not exist in the object." << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting NonExistingKey_ReturnsFalse test" << std::endl;
}
/**
 * @brief Verify that passing an empty string key to DrmJsonObject::isObject returns false.
 *
 * This test creates an instance of DrmJsonObject and invokes the isObject method using an empty string as the key.
 * It validates that the API correctly handles an empty input by returning false since an empty key is not a valid JSON object identifier.
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
 * | 01 | Create a DrmJsonObject instance, set key as an empty string, and invoke isObject. | jsonObj = instance from default constructor, key = "" | isObject returns false and EXPECT_FALSE verifies the result. | Should Fail |
 */
TEST_F(DrmJsonObjectTests, EmptyStringKey_ReturnsFalse) {
    std::cout << "Entering EmptyStringKey_ReturnsFalse test" << std::endl;
    
    // Create an instance of DrmJsonObject using default constructor
    DrmJsonObject jsonObj;
    std::string key = "";
    std::cout << "Invoking isObject with an empty string key" << std::endl;
    
    // Invoke the method
    bool result = jsonObj.isObject(key);
    std::cout << "isObject returned: " << std::boolalpha << result << std::endl;
    
    // Debug internal state
    std::cout << "Internal state: Empty key provided, no valid JSON object identifier." << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyStringKey_ReturnsFalse test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject::isObject returns false for keys with special characters
 *
 * This test creates an instance of DrmJsonObject and calls the isObject method using a key comprised of special characters ("!@#$%^&*()"). 
 * It ensures that the method correctly identifies the key as invalid and returns false, with the corresponding assertion passing.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 083
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate DrmJsonObject and invoke isObject with a key containing special characters | key = "!@#$%^&*()" | API returns false and assertion EXPECT_FALSE(result) passes | Should Pass |
 */
TEST_F(DrmJsonObjectTests, SpecialCharactersKey_ReturnsFalse) {
    std::cout << "Entering SpecialCharactersKey_ReturnsFalse test" << std::endl;
    
    // Create an instance of DrmJsonObject using default constructor
    DrmJsonObject jsonObj;
    std::string key = "!@#$%^&*()";
    std::cout << "Invoking isObject with key: " << key << std::endl;
    
    // Invoke the method
    bool result = jsonObj.isObject(key);
    std::cout << "isObject returned: " << std::boolalpha << result << std::endl;
    
    // Debug internal state
    std::cout << "Internal state: Special character key provided, no associated JSON object found." << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting SpecialCharactersKey_ReturnsFalse test" << std::endl;
}
/**
 * @brief Verify that isString returns true when the JSON attribute exists and its value is a string
 *
 * This test verifies that when a valid key with a string value exists in the DrmJsonObject, the method isString returns true. It ensures that the API correctly identifies a string type for the provided attribute key.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 084@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                                          | Expected Result                                                         | Notes           |
 * | :----:           | ---------------------------------------------------------------  | ------------------------------------------------------------------ | ----------------------------------------------------------------------- | --------------- |
 * | 01               | Create DrmJsonObject instance using default constructor          | None                                                               | DrmJsonObject instance is created successfully                          | Should be successful |
 * | 02               | Define a valid key with a string value                             | key = validStringKey                                               | The key is assigned the string value "validStringKey"                    | Should be successful |
 * | 03               | Invoke isString API with the defined key                           | input: key = validStringKey, output: result                            | The API should return true indicating the key's value is a string         | Should Pass     |
 * | 04               | Verify that the result from isString is true using EXPECT_TRUE check | result = API returned value                                         | EXPECT_TRUE assertion passes confirming result is true                  | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, VerifyIsStringReturnsTrueWhenAttributeIsAString) {
    std::cout << "Entering Verify isString returns true when the JSON attribute exists and its value is a string test" << std::endl;
    
    // Creating DrmJsonObject object using default constructor.
    DrmJsonObject jsonObject;
    std::cout << "Created DrmJsonObject object using default constructor." << std::endl;
    
    // Define the key that exists and its value is a string.
    std::string key = "validStringKey";
    std::cout << "Invoking isString with argument: " << key << std::endl;
    
    // Invoke the method isString
    bool result = jsonObject.isString(key);
    std::cout << "Method isString returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return true
    EXPECT_TRUE(result);
    
    std::cout << "Exiting Verify isString returns true when the JSON attribute exists and its value is a string test" << std::endl;
}
/**
 * @brief Verifies that the isString method returns false when the JSON attribute exists but its value is not a string.
 *
 * This test creates an instance of the DrmJsonObject using its default constructor, sets up a key that exists
 * with a non-string value, invokes the isString API, and asserts that the method rightly returns false.
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
 * | Variation / Step | Description                                                        | Test Data                                      | Expected Result                                                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ---------------------------------------------- | ------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Instantiate DrmJsonObject using the default constructor            | None                                           | DrmJsonObject instance is created successfully                                                   | Should be successful |
 * | 02               | Define key having a non-string attribute value                     | key = nonStringKey                             | The key is assigned the value "nonStringKey"                                                      | Should be successful |
 * | 03               | Invoke isString with the non-string key and validate the return value | input key = nonStringKey, output result = false | The method returns false and the EXPECT_FALSE assertion passes                                     | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, VerifyIsStringReturnsFalseWhenAttributeIsNotAString) {
    std::cout << "Entering Verify isString returns false when the JSON attribute exists but its value is not a string test" << std::endl;
    
    // Creating DrmJsonObject object using default constructor.
    DrmJsonObject jsonObject;
    std::cout << "Created DrmJsonObject object using default constructor." << std::endl;
    
    // Define the key that exists but its value is not a string (e.g., number or object).
    std::string key = "nonStringKey";
    std::cout << "Invoking isString with argument: " << key << std::endl;
    
    // Invoke the method isString
    bool result = jsonObject.isString(key);
    std::cout << "Method isString returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting Verify isString returns false when the JSON attribute exists but its value is not a string test" << std::endl;
}
/**
 * @brief Verify that isString returns false when the JSON attribute does not exist.
 *
 * This test creates an instance of DrmJsonObject and verifies that calling the isString method with a key that does not exist returns false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 086@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                     | Test Data                                         | Expected Result                                     | Notes             |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------- | --------------------------------------------------- | ----------------- |
 * | 01               | Create DrmJsonObject object using default constructor           | No input, output: new DrmJsonObject object        | DrmJsonObject is instantiated successfully          | Should be successful |
 * | 02               | Define a key that does not exist in the JSON object               | key = "missingKey"                                | key variable is initialized with "missingKey"       | Should be successful |
 * | 03               | Invoke isString method with the non-existent key                  | input: key = "missingKey", output: result variable  | API returns false                                   | Should Pass       |
 * | 04               | Assert that the result from isString is false using EXPECT_FALSE   | output: result from API call                      | EXPECT_FALSE confirms that result is false          | Should be successful |
 */
TEST_F(DrmJsonObjectTests, VerifyIsStringReturnsFalseWhenAttributeDoesNotExist) {
    std::cout << "Entering Verify isString returns false when the JSON attribute does not exist in the JSON object test" << std::endl;
    
    // Creating DrmJsonObject object using default constructor.
    DrmJsonObject jsonObject;
    std::cout << "Created DrmJsonObject object using default constructor." << std::endl;
    
    // Define the key that does not exist in the JSON object.
    std::string key = "missingKey";
    std::cout << "Invoking isString with argument: " << key << std::endl;
    
    // Invoke the method isString
    bool result = jsonObject.isString(key);
    std::cout << "Method isString returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false
    EXPECT_FALSE(result);
    
    std::cout << "Exiting Verify isString returns false when the JSON attribute does not exist in the JSON object test" << std::endl;
}
/**
 * @brief Verify that DrmJsonObject::isString returns false when provided an empty attribute name.
 *
 * This test case verifies that when an empty attribute name is passed to the isString method of a DrmJsonObject object, the method returns false.
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
 * | Variation / Step | Description                                                         | Test Data                                              | Expected Result                         | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------ | --------------------------------------- | --------------- |
 * | 01               | Create a DrmJsonObject object using the default constructor.        | No input parameters                                    | Instance of DrmJsonObject is created.   | Should be successful |
 * | 02               | Define an empty attribute name for testing.                         | key = ""                                               | key variable is set to an empty string. | Should be successful |
 * | 03               | Invoke the isString method with the empty attribute name.           | input: key = "", output: result not yet defined        | Method returns false.                   | Should Pass     |
 * | 04               | Verify the return value using an assertion.                         | Expected: result = false                               | EXPECT_FALSE(result) assertion passes.  | Should Pass     |
 */
TEST_F(DrmJsonObjectTests, VerifyIsStringReturnsFalseWhenEmptyAttributeNameIsProvided) {
    std::cout << "Entering Verify isString returns false when provided with an empty string as the attribute name test" << std::endl;
    
    // Creating DrmJsonObject object using default constructor.
    DrmJsonObject jsonObject;
    std::cout << "Created DrmJsonObject object using default constructor." << std::endl;
    
    // Define an empty key.
    std::string key = "";
    std::cout << "Invoking isString with an empty argument." << std::endl;
    
    // Invoke the method isString
    bool result = jsonObject.isString(key);
    std::cout << "Method isString returned: " << std::boolalpha << result << std::endl;
    
    // Expect the method to return false for an empty key.
    EXPECT_FALSE(result);
    
    std::cout << "Exiting Verify isString returns false when provided with an empty string as the attribute name test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject prints a non-empty JSON string.
 *
 * This test verifies that a DrmJsonObject constructed using the default constructor
 * can successfully generate a non-empty JSON string via its print() method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 088@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                        | Test Data                                                           | Expected Result                                                    | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------- | ------------------------------------------------------------------ | ------------- |
 * | 01               | Construct DrmJsonObject using default constructor                  | input: None, output: drmJsonObject instance                         | DrmJsonObject is constructed successfully                          | Should Pass   |
 * | 02               | Invoke print() method on DrmJsonObject                               | input: N/A, output: jsonStr = string returned by print()              | print() returns a non-empty JSON string                             | Should Pass   |
 * | 03               | Validate that the JSON string is non-empty                           | jsonStr = value returned from print()                                 | ASSERT check confirms that jsonStr is not empty                     | Should be successful |
 */
TEST_F(DrmJsonObjectTests, printTest) {
    std::cout << "Entering printTest test" << std::endl;
    // Create and populate DrmJsonObject using the default constructor
    DrmJsonObject drmJsonObject;
    std::cout << "Constructed DrmJsonObject object using default constructor" << std::endl;

    // Invoke the print() method on the populated object
    std::cout << "Invoking print() method on DrmJsonObject" << std::endl;
    std::string jsonStr = drmJsonObject.print();
    std::cout << "print() returned: " << jsonStr << std::endl;

    // Check that the JSON string is non-empty
    if (!jsonStr.empty()) {
        std::cout << "Internal JSON object structure is populated successfully" << std::endl;
    } else {
        std::cout << "Internal JSON object structure is empty. Test will fail." << std::endl;
    }
    EXPECT_FALSE(jsonStr.empty());

    std::cout << "Exiting printTest test" << std::endl;
}
/**
 * @brief Checks if DrmJsonObject prints correctly when provided an empty output vector.
 *
 * Verifies that invoking print() on a DrmJsonObject instance with an empty output vector results in no JSON data being output, ensuring that the print function handles empty data gracefully without errors.
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
 * | 01 | Create a default instance of DrmJsonObject using the default constructor. | drmJsonObj (instance created with default constructor) | Instance is created successfully. | Should be successful |
 * | 02 | Initialize an empty vector for output. | data vector = empty vector | Vector is empty with size 0. | Should be successful |
 * | 03 | Invoke print() method on DrmJsonObject with empty vector. | input: drmJsonObj, input: data = empty vector | Method executes without error; output vector remains unchanged or empty. | Should Pass |
 * | 04 | Convert the output vector to a string to inspect the printed JSON data. | data vector converted to string (printedJson) | Printed JSON string is empty, indicating no output due to empty vector. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidJSONPrintOnEmptyOutputVector)
{
    std::cout << "Entering ValidJSONPrintOnEmptyOutputVector test" << std::endl;

    // Create instance of DrmJsonObject using default constructor
    DrmJsonObject drmJsonObj;
    std::cout << "DrmJsonObject instance created using default constructor." << std::endl;

    // Construct an empty vector for output
    std::vector<uint8_t> data;
    std::cout << "Initial output vector (data) created. Size: " << data.size() << std::endl;

    // Log invocation of the print method
    std::cout << "Invoking print() method on DrmJsonObject instance with empty data vector." << std::endl;
    drmJsonObj.print(data);
    std::cout << "print() method invoked." << std::endl;

    // Convert the output vector to a string to inspect the printed JSON
    std::string printedJson(data.begin(), data.end());
    std::cout << "Data vector size after print: " << data.size() << std::endl;
    std::cout << "Printed JSON output: " << printedJson << std::endl;

    std::cout << "Exiting ValidJSONPrintOnEmptyOutputVector test" << std::endl;
}
/**
 * @brief Test the print_UnFormatted method of DrmJsonObject to ensure it returns the expected unformatted JSON string.
 *
 * This test verifies that the print_UnFormatted method of DrmJsonObject returns a string representing
 * the JSON object in an unformatted manner, ensuring the object is constructed properly and the method behaves as expected.
 * The test records console output to trace the execution steps.
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
 * | Variation / Step | Description                                                  | Test Data                                                        | Expected Result                                                                               | Notes              |
 * | :--------------: | ------------------------------------------------------------ | ---------------------------------------------------------------- | --------------------------------------------------------------------------------------------- | ------------------ |
 * | 01               | Create a DrmJsonObject object using its default constructor    | drmJsonObj: default constructed object                             | Object is successfully constructed                                                            | Should be successful |
 * | 02               | Invoke the print_UnFormatted() method                          | drmJsonObj.print_UnFormatted()                                     | Returns a string representing unformatted JSON; assertion check on return value               | Should Pass        |
 * | 03               | Log the returned value from print_UnFormatted()                | retStr: output of print_UnFormatted()                              | Console outputs the returned string                                                           | Should be successful |
 */
TEST_F(DrmJsonObjectTests, print_UnFormatted) {
    std::cout << "Entering print_UnFormatted test" << std::endl;

    // Create an object of DrmJsonObject using its default constructor.
    std::cout << "Creating DrmJsonObject object using default constructor." << std::endl;
    DrmJsonObject drmJsonObj;

    // Invoke the method print_UnFormatted and log its invocation.
    std::cout << "Invoking print_UnFormatted() method." << std::endl;
    std::string retStr = drmJsonObj.print_UnFormatted();
    std::cout << "Method print_UnFormatted() returned: " << retStr << std::endl;

    std::cout << "Exiting print_UnFormatted test" << std::endl;
}
/**
 * @brief Verify that the destructor of DrmJsonObject properly frees allocated resources when the object goes out of scope
 *
 * This test validates that when a DrmJsonObject is instantiated using its default constructor, no exceptions are thrown and the destructor is invoked automatically when the object goes out of scope. It ensures that the destructor correctly handles the clean up of any allocated resources.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 091
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                    | Test Data                                 | Expected Result                                              | Notes        |
 * | :--------------: | ------------------------------------------------------------------------------ | ----------------------------------------- | ------------------------------------------------------------ | ------------ |
 * | 01               | Invoke default constructor of DrmJsonObject to allocate resources              | None                                      | The constructor does not throw any exception (EXPECT_NO_THROW) | Should Pass  |
 * | 02               | Allow the object to go out of scope to automatically invoke the destructor     | None                                      | Destructor is invoked and resources are freed without error   | Should be successful |
 */
TEST_F(DrmJsonObjectTests, Destructor_With_Valid_Allocated_Resources)
{
    std::cout << "Entering Destructor_With_Valid_Allocated_Resources test" << std::endl;
    
    {
        std::cout << "Creating DrmJsonObject object using default constructor" << std::endl;
        EXPECT_NO_THROW(
            {
                DrmJsonObject obj;
            }
        );
        std::cout << "Invoking destructor automatically when object goes out of scope" << std::endl;
    }
    
    std::cout << "Exiting Destructor_With_Valid_Allocated_Resources test" << std::endl;
}
/**
 * @brief Verify that DrmJsonParseException::what returns the expected error message.
 *
 * This test verifies that the default constructed DrmJsonParseException object, when its what() method is invoked,
 * returns the error message "Failed to parse JSON string". The test prepares the expected error message using strncpy,
 * invokes the what() method, and compares the returned message with the expected one using EXPECT_STREQ. This ensures
 * that the exception class provides the correct diagnostic message as intended.
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
 * | 01 | Construct DrmJsonParseException object using default constructor | No input parameters | Object is created successfully | Should Pass |
 * | 02 | Invoke what() method on DrmJsonParseException object to retrieve the error message | drmException.what() | Returns error message string ("Failed to parse JSON string") | Should Pass |
 * | 03 | Prepare expected error message using strncpy | expectedMessage = "Failed to parse JSON string" | expectedMessage array correctly holds "Failed to parse JSON string" | Should be successful |
 * | 04 | Compare the returned message with the expected message using EXPECT_STREQ | Input: expectedMessage, returnedMessage | EXPECT_STREQ passes as both strings are equal | Should Pass |
 */
TEST(DrmJsonParseException, PositiveTest_VerifyExpectedErrorMessageReturned) {
    std::cout << "Entering PositiveTest_VerifyExpectedErrorMessageReturned test" << std::endl;
    
    // Create object of DrmJsonParseException using default constructor
    DrmJsonParseException drmException;
    std::cout << "Constructed DrmJsonParseException object." << std::endl;
    
    // Invoke what() method
    std::cout << "Invoking what() method on DrmJsonParseException object." << std::endl;
    const char* returnedMessage = drmException.what();
    std::cout << "what() returned: " << returnedMessage << std::endl;
    
    // Prepare expected message using strncpy for fixed size array
    char expectedMessage[50] = {0};
    std::strncpy(expectedMessage, "Failed to parse JSON string", sizeof(expectedMessage) - 1);
    std::cout << "Expected message prepared using strncpy: " << expectedMessage << std::endl;
    
    // Compare the returned message with the expected message
    std::cout << "Comparing the returned message with the expected message." << std::endl;
    EXPECT_STREQ(expectedMessage, returnedMessage);
    
    std::cout << "Exiting PositiveTest_VerifyExpectedErrorMessageReturned test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject::add correctly handles valid inputs with various encoding types
 *
 * This test verifies that when valid key ("validKey") and valid value ("validValue") are used with each supported encoding type 
 * (ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL), the DrmJsonObject::add method returns true. This ensures that the API 
 * functions as expected for typical valid inputs.
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
 * | Variation / Step | Description                                                          | Test Data                                                      | Expected Result                                                                      | Notes      |
 * | :----:           | --------------------------------------------------------------       | -------------------------------------------------------------- | ------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Call add() with encoding set to ENCODING_STRING                        | name = validKey, value = validValue, encoding = ENCODING_STRING   | Return value true and EXPECT_TRUE(result) passes                                      | Should Pass|
 * | 02               | Call add() with encoding set to ENCODING_BASE64                        | name = validKey, value = validValue, encoding = ENCODING_BASE64   | Return value true and EXPECT_TRUE(result) passes                                      | Should Pass|
 * | 03               | Call add() with encoding set to ENCODING_BASE64_URL                      | name = validKey, value = validValue, encoding = ENCODING_BASE64_URL | Return value true and EXPECT_TRUE(result) passes                                      | Should Pass|
 */
TEST_F(DrmJsonObjectTests, ValidInputWithTypicalNameAndValue) {
    std::cout << "Entering ValidInputWithTypicalNameAndValue test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject jsonObj;
    
    std::string name = "validKey";
    const char* value = "validValue";

    // Define encoding types with their string representations for logging
    struct EncodingMap {
        DrmJsonObject::ENCODING enc;
        std::string encName;
    } encodings[] = {
        { DrmJsonObject::ENCODING_STRING, "ENCODING_STRING" },
        { DrmJsonObject::ENCODING_BASE64, "ENCODING_BASE64" },
        { DrmJsonObject::ENCODING_BASE64_URL, "ENCODING_BASE64_URL" }
    };

    bool result = false;
    for (const auto &encMap : encodings) {
        std::cout << "Invoking add with name: " << name 
                  << ", value: " << value 
                  << ", encoding: " << encMap.encName << std::endl;
        result = jsonObj.add(name, value, encMap.enc);
        std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_TRUE(result);
    }
    
    std::cout << "Exiting ValidInputWithTypicalNameAndValue test" << std::endl;
}
/**
 * @brief Test the addition of a valid key with an empty string value across different encoding types
 *
 * This test verifies that adding a valid key with an empty string value performs successfully for each supported encoding type.
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
 * | Variation / Step | Description                                                                 | Test Data                                                   | Expected Result                                | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------------- | ---------------------------------------------- | ------------- |
 * | 01               | Create a DrmJsonObject instance using the default constructor               | None                                                        | Object is created successfully                 | Should be successful |
 * | 02               | Call add API with key = "validKey", value = "" and encoding = ENCODING_STRING  | name = validKey, value = , encoding = ENCODING_STRING         | Return true and EXPECT_TRUE assertion passes   | Should Pass   |
 * | 03               | Call add API with key = "validKey", value = "" and encoding = ENCODING_BASE64    | name = validKey, value = , encoding = ENCODING_BASE64           | Return true and EXPECT_TRUE assertion passes   | Should Pass   |
 * | 04               | Call add API with key = "validKey", value = "" and encoding = ENCODING_BASE64_URL| name = validKey, value = , encoding = ENCODING_BASE64_URL       | Return true and EXPECT_TRUE assertion passes   | Should Pass   |
 */
TEST_F(DrmJsonObjectTests, ValidInputWithEmptyValue) {
    std::cout << "Entering ValidInputWithEmptyValue test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject jsonObj;
    
    std::string name = "validKey";
    const char* value = "";

    // Prepare encoding types
    struct EncodingMap {
        DrmJsonObject::ENCODING enc;
        std::string encName;
    } encodings[] = {
        { DrmJsonObject::ENCODING_STRING, "ENCODING_STRING" },
        { DrmJsonObject::ENCODING_BASE64, "ENCODING_BASE64" },
        { DrmJsonObject::ENCODING_BASE64_URL, "ENCODING_BASE64_URL" }
    };

    bool result = false;
    for (const auto &encMap : encodings) {
        std::cout << "Invoking add with name: " << name 
                  << ", value: \"" << value << "\"" 
                  << ", encoding: " << encMap.encName << std::endl;
        result = jsonObj.add(name, value, encMap.enc);
        std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_TRUE(result);
    }
    
    std::cout << "Exiting ValidInputWithEmptyValue test" << std::endl;
}
/**
 * @brief Test the behavior of DrmJsonObject::add with a NULL value input.
 *
 * This test verifies that the DrmJsonObject::add function correctly handles a NULL pointer for the value parameter.
 * The function is expected to fail the add operation when provided with a NULL value across multiple encoding types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 095@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                          | Expected Result                                     | Notes       |
 * | :----:           | :------------------------------------------------------------------------------------------- | :------------------------------------------------- | :-------------------------------------------------- | :---------: |
 * | 01               | Call add with valid key "validKey", NULL value, and encoding ENCODING_STRING.                 | input1 = validKey, input2 = NULL, input3 = ENCODING_STRING | API returns false and EXPECT_FALSE assertion passes  | Should Fail |
 * | 02               | Call add with valid key "validKey", NULL value, and encoding ENCODING_BASE64.                   | input1 = validKey, input2 = NULL, input3 = ENCODING_BASE64   | API returns false and EXPECT_FALSE assertion passes  | Should Fail |
 * | 03               | Call add with valid key "validKey", NULL value, and encoding ENCODING_BASE64_URL.               | input1 = validKey, input2 = NULL, input3 = ENCODING_BASE64_URL | API returns false and EXPECT_FALSE assertion passes  | Should Fail |
 */
TEST_F(DrmJsonObjectTests, InvalidInputWithNullValue) {
    std::cout << "Entering InvalidInputWithNullValue test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject jsonObj;
    
    std::string name = "validKey";
    const char* value = nullptr;

    // Define encoding types for iteration
    struct EncodingMap {
        DrmJsonObject::ENCODING enc;
        std::string encName;
    } encodings[] = {
        { DrmJsonObject::ENCODING_STRING, "ENCODING_STRING" },
        { DrmJsonObject::ENCODING_BASE64, "ENCODING_BASE64" },
        { DrmJsonObject::ENCODING_BASE64_URL, "ENCODING_BASE64_URL" }
    };

    bool result = true;
    for (const auto &encMap : encodings) {
        std::cout << "Invoking add with name: " << name 
                  << ", value: NULL" 
                  << ", encoding: " << encMap.encName << std::endl;
        result = jsonObj.add(name, value, encMap.enc);
        std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_FALSE(result);
    }
    
    std::cout << "Exiting InvalidInputWithNullValue test" << std::endl;
}
/**
 * @brief Test that the API correctly handles invalid input when an empty name is provided.
 *
 * This test case verifies that the DrmJsonObject::add method returns false when an empty string is used as the name parameter. The test iterates over multiple encoding types to ensure that the method consistently rejects invalid input across different encodings.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Instantiate DrmJsonObject using the default constructor | none | Object is created successfully | Should be successful |
 * | 02 | Initialize test data with an empty name and a valid value | name = "", value = validValue | Variables are set with the correct values | Should be successful |
 * | 03 | Iterate over defined encoding types and invoke the add method for each iteration | encoding = ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL | The add method returns false for each encoding type | Should Fail |
 * | 04 | Verify that the EXPECT_FALSE assertion check passes for each API invocation | result = false | The test assertion passes confirming the method returns false | Should be successful |
 */
TEST_F(DrmJsonObjectTests, PotentiallyInvalidInputWithEmptyName) {
    std::cout << "Entering PotentiallyInvalidInputWithEmptyName test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject jsonObj;
    
    std::string name = "";
    const char* value = "validValue";

    // Prepare encoding type iterations
    struct EncodingMap {
        DrmJsonObject::ENCODING enc;
        std::string encName;
    } encodings[] = {
        { DrmJsonObject::ENCODING_STRING, "ENCODING_STRING" },
        { DrmJsonObject::ENCODING_BASE64, "ENCODING_BASE64" },
        { DrmJsonObject::ENCODING_BASE64_URL, "ENCODING_BASE64_URL" }
    };

    bool result = true;
    for (const auto &encMap : encodings) {
        std::cout << "Invoking add with name: (empty string)" 
                  << ", value: " << value 
                  << ", encoding: " << encMap.encName << std::endl;
        result = jsonObj.add(name, value, encMap.enc);
        std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
        EXPECT_FALSE(result);
    }
    
    std::cout << "Exiting PotentiallyInvalidInputWithEmptyName test" << std::endl;
}
/**
 * @brief Validate valid addition for all supported encoding types
 *
 * This test verifies that the add method of the DrmJsonObject class returns true when invoked with a valid key and value across all supported encoding types (ENCODING_STRING, ENCODING_BASE64, and ENCODING_BASE64_URL). It ensures the API consistently handles different encoding formats for addition.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create a DrmJsonObject instance using the default constructor | None | Instance is successfully created | Should be successful |@n
 * | 02 | Define test inputs for key and value | testName = key, testValue = someData | Inputs are set correctly | Should be successful |@n
 * | 03 | Invoke add API with encoding type ENCODING_STRING | testName = key, testValue = someData, encoding = ENCODING_STRING | API returns true; assertion passes | Should Pass |@n
 * | 04 | Invoke add API with encoding type ENCODING_BASE64 | testName = key, testValue = someData, encoding = ENCODING_BASE64 | API returns true; assertion passes | Should Pass |@n
 * | 05 | Invoke add API with encoding type ENCODING_BASE64_URL | testName = key, testValue = someData, encoding = ENCODING_BASE64_URL | API returns true; assertion passes | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidAdditionAllEncoding) {
    std::cout << "Entering ValidAdditionAllEncoding test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject drmObj;
    
    // Define test inputs
    std::string testName = "key";
    std::string testValue = "someData";
    
    // Loop through the encoding types
    DrmJsonObject::ENCODING encodings[] = {DrmJsonObject::ENCODING_STRING, 
                                             DrmJsonObject::ENCODING_BASE64, 
                                             DrmJsonObject::ENCODING_BASE64_URL};
    
    for (auto encoding : encodings) {
        std::cout << "Invoking add with name: " << testName 
                  << ", value: " << testValue 
                  << ", encoding: " << encoding << std::endl;
        bool result = drmObj.add(testName, testValue, encoding);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        // Expect true for successful addition for each encoding type.
        ASSERT_TRUE(result);
    }
    
    std::cout << "Exiting ValidAdditionAllEncoding test" << std::endl;
}
/**
 * @brief Test verifies that adding an empty value to a DrmJsonObject fails across all supported encoding types.
 *
 * This test ensures that the add method of DrmJsonObject does not allow inserting an empty value. It iterates through different encoding types (ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL) and verifies that the method returns false for each case, using ASSERT_FALSE to validate the function's behavior.
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
 * | Variation / Step | Description                                                    | Test Data                                                          | Expected Result                                    | Notes      |
 * | :--------------: | -------------------------------------------------------------- | ------------------------------------------------------------------ | -------------------------------------------------- | ---------- |
 * | 01               | Invoke add with empty value using ENCODING_STRING              | testName = emptyValue, testValue = , encoding = ENCODING_STRING      | API returns false; ASSERT_FALSE validates return   | Should Fail |
 * | 02               | Invoke add with empty value using ENCODING_BASE64               | testName = emptyValue, testValue = , encoding = ENCODING_BASE64       | API returns false; ASSERT_FALSE validates return   | Should Fail |
 * | 03               | Invoke add with empty value using ENCODING_BASE64_URL           | testName = emptyValue, testValue = , encoding = ENCODING_BASE64_URL    | API returns false; ASSERT_FALSE validates return   | Should Fail |
 */
TEST_F(DrmJsonObjectTests, ValidAdditionEmptyValue) {
    std::cout << "Entering ValidAdditionEmptyValue test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject drmObj;
    
    // Define test inputs
    std::string testName = "emptyValue";
    std::string testValue = "";
    
    // Loop through the encoding types
    DrmJsonObject::ENCODING encodings[] = {DrmJsonObject::ENCODING_STRING, 
                                             DrmJsonObject::ENCODING_BASE64, 
                                             DrmJsonObject::ENCODING_BASE64_URL};
    
    for (auto encoding : encodings) {
        std::cout << "Invoking add with name: " << testName 
                  << ", empty value, encoding: " << encoding << std::endl;
        bool result = drmObj.add(testName, testValue, encoding);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        // Expect false as empty value is not allowed.
        ASSERT_FALSE(result);
    }
    
    std::cout << "Exiting ValidAdditionEmptyValue test" << std::endl;
}
/**
 * @brief Verify that the add API correctly handles large input strings.
 *
 * This test validates that the DrmJsonObject::add API can successfully add a key-value pair when both key and value are large strings (10,000 characters each). The test constructs the large input, simulates a fixed-size buffer assignment similar to strncpy, and then verifies that the add function returns true. This ensures that the API handles large inputs as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of DrmJsonObject using the default constructor | None | DrmJsonObject instance is created successfully | Should be successful |
 * | 02 | Prepare large input strings by creating a 10,000-character string and copying it to fixed-size buffers | largeStr = "a" (repeated 10000 times), nameBuffer & valueBuffer derive from largeStr | Large strings are created and buffers are populated correctly | Should be successful |
 * | 03 | Convert the buffers into std::string variables (testName and testValue) | testName, testValue initialized from nameBuffer and valueBuffer respectively | Strings contain 10,000 'a' characters each | Should be successful |
 * | 04 | Invoke the add API with testName, testValue, and ENCODING_STRING | input: testName = <10000 chars>, testValue = <10000 chars>, encoding = ENCODING_STRING; output: expected true | API returns true indicating successful addition | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidAdditionLargeInput) {
    std::cout << "Entering ValidAdditionLargeInput test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject drmObj;
    
    // Prepare large strings (10,000 characters each)
    std::string largeStr(10000, 'a');
    // Use a buffer with strncpy style assignment (simulate fixed-size array assignment)
    char nameBuffer[10001] = {0};
    char valueBuffer[10001] = {0};
    strncpy(nameBuffer, largeStr.c_str(), sizeof(nameBuffer) - 1);
    strncpy(valueBuffer, largeStr.c_str(), sizeof(nameBuffer) - 1);
    std::string testName(nameBuffer);
    std::string testValue(valueBuffer);
    
    std::cout << "Invoking add with name: <10000 chars>, value: <10000 chars>, encoding: ENCODING_STRING" << std::endl;
    bool result = drmObj.add(testName, testValue, DrmJsonObject::ENCODING_STRING);
    std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
    // Expect true indicating the value was successfully added.
    ASSERT_TRUE(result);
    
    std::cout << "Exiting ValidAdditionLargeInput test" << std::endl;
}
/**
 * @brief Test the negative scenario for DrmJsonObject add method when an empty name is provided
 *
 * This test verifies that the DrmJsonObject::add method returns false when an empty string is used as the name.
 * The test iterates over multiple encoding types (ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL) to ensure consistent behavior.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 100@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                 | Test Data                                                                                         | Expected Result                                     | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ---------------------------------------------------- | ----------- |
 * | 01               | Invoke add with empty name and encoding DrmJsonObject::ENCODING_STRING                        | testName = "", testValue = value, encoding = DrmJsonObject::ENCODING_STRING                         | Method returns false and ASSERT_FALSE(result) passes | Should Fail |
 * | 02               | Invoke add with empty name and encoding DrmJsonObject::ENCODING_BASE64                          | testName = "", testValue = value, encoding = DrmJsonObject::ENCODING_BASE64                           | Method returns false and ASSERT_FALSE(result) passes | Should Fail |
 * | 03               | Invoke add with empty name and encoding DrmJsonObject::ENCODING_BASE64_URL                        | testName = "", testValue = value, encoding = DrmJsonObject::ENCODING_BASE64_URL                         | Method returns false and ASSERT_FALSE(result) passes | Should Fail |
 */
TEST_F(DrmJsonObjectTests, NegativeAdditionEmptyName) {
    std::cout << "Entering NegativeAdditionEmptyName test" << std::endl;
    
    // Create object using default constructor
    DrmJsonObject drmObj;
    
    // Define test inputs
    std::string testName = "";
    std::string testValue = "value";
    
    // Loop through the encoding types
    DrmJsonObject::ENCODING encodings[] = {DrmJsonObject::ENCODING_STRING, 
                                             DrmJsonObject::ENCODING_BASE64, 
                                             DrmJsonObject::ENCODING_BASE64_URL};
    
    for (auto encoding : encodings) {
        std::cout << "Invoking add with empty name, value: " << testValue 
                  << ", encoding: " << encoding << std::endl;
        bool result = drmObj.add(testName, testValue, encoding);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        // Expect false as empty name is not allowed.
        ASSERT_FALSE(result);
    }
    
    std::cout << "Exiting NegativeAdditionEmptyName test" << std::endl;
}
/**
 * @brief Verifies that the add method returns true when provided with a valid name and a non-empty byte vector
 *
 * This test creates an instance of DrmJsonObject and invokes its add method using a valid string name "LoopTest",
 * a non-empty byte vector {0x10, 0x20, 0x30}, and three different encoding types (ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL).
 * The objective is to ensure that the add method successfully processes valid inputs and returns true consistently across the supported encoding types.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 101@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Call add method with encoding ENCODING_STRING | name = LoopTest, values = 0x10, 0x20, 0x30, encoding = ENCODING_STRING | API returns true and EXPECT_TRUE assertion passes | Should Pass |
 * | 02 | Call add method with encoding ENCODING_BASE64 | name = LoopTest, values = 0x10, 0x20, 0x30, encoding = ENCODING_BASE64 | API returns true and EXPECT_TRUE assertion passes | Should Pass |
 * | 03 | Call add method with encoding ENCODING_BASE64_URL | name = LoopTest, values = 0x10, 0x20, 0x30, encoding = ENCODING_BASE64_URL | API returns true and EXPECT_TRUE assertion passes | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddWithValidNameNonEmptyByteVector)
{
    std::cout << "Entering AddWithValidNameNonEmptyByteVector test" << std::endl;
    
    // Create an object of DrmJsonObject using its default constructor
    DrmJsonObject drmObj;
    
    // Prepare test inputs
    std::string name = "LoopTest";
    std::vector<uint8_t> values = { 0x10, 0x20, 0x30 };

    // Array of encoding types to test
    DrmJsonObject::ENCODING encodings[] = { DrmJsonObject::ENCODING_STRING, DrmJsonObject::ENCODING_BASE64, DrmJsonObject::ENCODING_BASE64_URL };

    // Loop through the encoding types and invoke the add method
    for (auto encoding : encodings)
    {
        std::cout << "Invoking add with name: " << name 
                  << ", values: {0x" << std::hex << static_cast<int>(values[0]) << ", 0x" << static_cast<int>(values[1])
                  << ", 0x" << static_cast<int>(values[2]) << "}" << std::dec 
                  << ", encoding: " << encoding << std::endl;
        
        bool result = drmObj.add(name, values, encoding);
        std::cout << "Returned value from add: " << std::boolalpha << result << std::endl;
        
        // Assert that the return value is true for valid inputs
        EXPECT_TRUE(result);
    }
    
    std::cout << "Exiting AddWithValidNameNonEmptyByteVector test" << std::endl;
}
/**
 * @brief Verify that adding an entry with an empty name and non-empty byte vector fails.
 *
 * This test verifies that when invoking the add method of DrmJsonObject with an empty string as the name and a non-empty byte vector, the method returns false. It iterates through different encoding types to ensure consistent behavior across various encoding options.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 102@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke add with empty name and non-empty byte vector using ENCODING_STRING | name = "", values = {0xAA,0xBB,0xCC}, encoding = ENCODING_STRING | API returns false and EXPECT_FALSE assertion passes | Should Fail |
 * | 02 | Invoke add with empty name and non-empty byte vector using ENCODING_BASE64 | name = "", values = {0xAA,0xBB,0xCC}, encoding = ENCODING_BASE64 | API returns false and EXPECT_FALSE assertion passes | Should Fail |
 * | 03 | Invoke add with empty name and non-empty byte vector using ENCODING_BASE64_URL | name = "", values = {0xAA,0xBB,0xCC}, encoding = ENCODING_BASE64_URL | API returns false and EXPECT_FALSE assertion passes | Should Fail |
 */
TEST_F(DrmJsonObjectTests, AddWithEmptyNameNonEmptyByteVector)
{
    std::cout << "Entering AddWithEmptyNameNonEmptyByteVector test" << std::endl;
    
    // Create an object of DrmJsonObject using its default constructor
    DrmJsonObject drmObj;
    
    // Prepare test inputs
    std::string name = "";
    std::vector<uint8_t> values = { 0xAA, 0xBB, 0xCC };

    // Array of encoding types to test
    DrmJsonObject::ENCODING encodings[] = { DrmJsonObject::ENCODING_STRING, DrmJsonObject::ENCODING_BASE64, DrmJsonObject::ENCODING_BASE64_URL };

    // Loop through the encoding types and invoke the add method
    for (auto encoding : encodings)
    {
        std::cout << "Invoking add with empty name, values: {0x" << std::hex << static_cast<int>(values[0]) << ", 0x" << static_cast<int>(values[1])
                  << ", 0x" << static_cast<int>(values[2]) << "}" << std::dec 
                  << ", encoding: " << encoding << std::endl;
        
        bool result = drmObj.add(name, values, encoding);
        std::cout << "Returned value from add: " << std::boolalpha << result << std::endl;
        
        // Assert that the return value is false when name is empty
        EXPECT_FALSE(result);
    }
    
    std::cout << "Exiting AddWithEmptyNameNonEmptyByteVector test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject::add returns false when provided with a valid name and an empty byte vector.
 *
 * This test verifies that the DrmJsonObject::add method correctly handles an empty byte vector by returning false. The test uses different encoding types to ensure consistent behavior when no data is supplied in the byte vector.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 103@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of DrmJsonObject using the default constructor | None | Instance is successfully created | Should be successful |
 * | 02 | Initialize test inputs with a valid name and an empty byte vector | name = "EmptyVectorTest", values = (empty vector) | Test inputs are correctly initialized | Should be successful |
 * | 03 | Iterate over each encoding type and call the add method | encoding = ENCODING_STRING, ENCODING_BASE64, ENCODING_BASE64_URL; name = "EmptyVectorTest", values = (empty vector) | add() returns false for each encoding type | Should Pass |
 * | 04 | Validate the return value of the add method using assertion check | output: result = false | EXPECT_FALSE(result) passes for each encoding type | Should Pass |
 */
TEST_F(DrmJsonObjectTests, AddWithValidNameEmptyByteVector)
{
    std::cout << "Entering AddWithValidNameEmptyByteVector test" << std::endl;
    
    // Create an object of DrmJsonObject using its default constructor
    DrmJsonObject drmObj;
    
    // Prepare test inputs
    std::string name = "EmptyVectorTest";
    std::vector<uint8_t> values;  // empty vector

    // Array of encoding types to test
    DrmJsonObject::ENCODING encodings[] = { DrmJsonObject::ENCODING_STRING, DrmJsonObject::ENCODING_BASE64, DrmJsonObject::ENCODING_BASE64_URL };

    // Loop through the encoding types and invoke the add method
    for (auto encoding : encodings)
    {
        std::cout << "Invoking add with name: " << name 
                  << ", empty values vector" 
                  << ", encoding: " << encoding << std::endl;
        
        bool result = drmObj.add(name, values, encoding);
        std::cout << "Returned value from add: " << std::boolalpha << result << std::endl;
        
        // Assert that the return value is false when values vector is empty
        EXPECT_FALSE(result);
    }
    
    std::cout << "Exiting AddWithValidNameEmptyByteVector test" << std::endl;
}
/**
 * @brief Verifies that retrieving a valid JSON property using string encoding returns the correct result.
 *
 * This test ensures that the DrmJsonObject correctly retrieves the property "validProperty" when using the ENCODING_STRING mode. 
 * It checks that the retrieval operation returns true and that the output vector is populated as expected.
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
 * | 01 | Instantiate DrmJsonObject and initialize test parameters. | property = validProperty, values = empty vector, encoding = ENCODING_STRING | Object is created and test parameters are initialized successfully. | Should be successful |
 * | 02 | Invoke get() API with the provided valid property and string encoding, and verify the returned result. | property = validProperty, values = output vector, encoding = ENCODING_STRING | API returns true and the output vector is populated as expected. | Should Pass |
 */
TEST_F(DrmJsonObjectTests, ValidRetrieval_StringEncoding) {
    std::cout << "Entering ValidRetrieval_StringEncoding test" << std::endl;
    
    // Create the object (assumes default constructor populates necessary JSON property "validProperty")
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "validProperty";
    
    std::cout << "Invoking get with property: " << property 
              << ", encoding: ENCODING_STRING" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_STRING);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    
    if (result) {
        std::cout << "Retrieved bytes: ";
        for (auto byte : values) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }
        
    EXPECT_TRUE(result);

    std::cout << "Exiting ValidRetrieval_StringEncoding test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject correctly retrieves property data using Base64 encoding.
 *
 * This test verifies that when a valid property ("validProperty") is requested with the 
 * Base64 encoding option, the API call correctly returns true and populates the output 
 * vector with the expected bytes. The objective is to ensure the proper functionality 
 * of the get method for valid Base64 retrieval scenarios.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 105@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize test objects and set up test data. | N/A | Objects created and variables initialized. | Should be successful |
 * | 02 | Invoke DrmJsonObject::get with property "validProperty" and encoding ENCODING_BASE64. | property = validProperty, encoding = ENCODING_BASE64, vector values = empty | API returns true after populating vector if data exists. | Should Pass |
 * | 03 | Verify that the EXPECT_TRUE assertion confirms the API call returned true. | result = true | Assertion passes confirming valid retrieval. | Should be successful |
 */
TEST_F(DrmJsonObjectTests, ValidRetrieval_Base64) {
    std::cout << "Entering ValidRetrieval_Base64 test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "validProperty";
    
    std::cout << "Invoking get with property: " << property 
              << ", encoding: ENCODING_BASE64" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_BASE64);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    
    if (result) {
        std::cout << "Retrieved bytes: ";
        for (auto byte : values) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }
        
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidRetrieval_Base64 test" << std::endl;
}
/**
 * @brief Tests valid retrieval of a property using Base64Url encoding.
 *
 * This test verifies that the DrmJsonObject::get method successfully retrieves the value of an existing property while using the Base64Url encoding. It ensures that the function returns true when the property "validProperty" exists and that the output is correctly populated.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 106
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                     | Test Data                                                           | Expected Result                                                                    | Notes             |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ---------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Initialize the test environment and create DrmJsonObject instance along with variables.         | property = validProperty, values = empty                            | Objects are instantiated successfully.                                             | Should be successful |
 * | 02               | Invoke the get() method with property "validProperty" and encoding ENCODING_BASE64_URL.           | property = validProperty, encoding = ENCODING_BASE64_URL, values = empty| Returns true indicating successful retrieval and populates the values vector.         | Should Pass       |
 * | 03               | Validate the result and print the retrieved bytes when the result is true.                      | result = true, values = retrieved bytes                             | EXPECT_TRUE(result) passes confirming the valid retrieval of property value.        | Should be successful |
 */
TEST_F(DrmJsonObjectTests, ValidRetrieval_Base64Url) {
    std::cout << "Entering ValidRetrieval_Base64Url test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "validProperty";
    
    std::cout << "Invoking get with property: " << property 
              << ", encoding: ENCODING_BASE64_URL" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_BASE64_URL);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    
    if (result) {
        std::cout << "Retrieved bytes: ";
        for (auto byte : values) {
            std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
    }

    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidRetrieval_Base64Url test" << std::endl;
}
/**
 * @brief Test behavior of DrmJsonObject::get when the requested property is missing
 *
 * This test verifies that calling the get method with a non-existent property ("missingProperty")
 * using the ENCODING_STRING encoding returns false and leaves the output vector empty. This is important
 * to ensure that the API properly handles missing properties without altering the output data structures.
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
 * | Variation / Step | Description                                                                                                  | Test Data                                                              | Expected Result                                                                                 | Notes         |
 * | :----:           | ------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke DrmJsonObject::get with a property name "missingProperty" using encoding ENCODING_STRING to retrieve data | property = missingProperty, encoding = ENCODING_STRING, output vector = empty | API returns false; assertion checks: EXPECT_FALSE(result) and EXPECT_TRUE(values.empty()) | Should Fail   |
 */
TEST_F(DrmJsonObjectTests, MissingProperty) {
    std::cout << "Entering MissingProperty test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "missingProperty";
    
    std::cout << "Invoking get with property: " << property 
              << ", encoding: ENCODING_STRING" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_STRING);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    std::cout << "Size of retrieved vector: " << values.size() << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(values.empty());
    
    std::cout << "Exiting MissingProperty test" << std::endl;
}
/**
 * @brief This test verifies that calling the get function with an empty property name using ENCODING_STRING returns false and leaves the output vector empty.
 *
 * This test is designed to check that the DrmJsonObject::get function correctly handles an empty property name when the encoding type is set to ENCODING_STRING. It ensures that the function returns false and that the output vector remains empty, confirming proper handling of an edge case input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 108@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                             | Test Data                                                      | Expected Result                                                                        | Notes       |
 * | :--------------: | ------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke get with an empty property name using ENCODING_STRING | property = "", values = empty vector, encoding = ENCODING_STRING   | API returns false; values vector remains empty (assert: EXPECT_FALSE(result), EXPECT_TRUE(values.empty())) | Should Pass |
 */
TEST_F(DrmJsonObjectTests, EmptyPropertyName_StringEncoding) {
    std::cout << "Entering EmptyPropertyName_StringEncoding test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "";
    
    std::cout << "Invoking get with empty property name"
              << ", encoding: ENCODING_STRING" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_STRING);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    std::cout << "Size of retrieved vector: " << values.size() << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(values.empty());
    
    std::cout << "Exiting EmptyPropertyName_StringEncoding test" << std::endl;
}
/**
 * @brief Verify that calling get() with an empty property name using ENCODING_BASE64 properly fails.
 *
 * This test checks that when get() is invoked with an empty property name and the encoding set to ENCODING_BASE64,
 * it returns false and leaves the output vector empty. This ensures that the API does not process invalid empty property names.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 109
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize a DrmJsonObject, set property as an empty string, and invoke the get() method with ENCODING_BASE64. | property = "", values = empty vector, encoding = DrmJsonObject::ENCODING_BASE64 | get() returns false and the values vector remains empty. | Should Fail |
 */
TEST_F(DrmJsonObjectTests, EmptyPropertyName_Base64) {
    std::cout << "Entering EmptyPropertyName_Base64 test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "";
    
    std::cout << "Invoking get with empty property name"
              << ", encoding: ENCODING_BASE64" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_BASE64);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    std::cout << "Size of retrieved vector: " << values.size() << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(values.empty());
    
    std::cout << "Exiting EmptyPropertyName_Base64 test" << std::endl;
}
/**
 * @brief Validate that DrmJsonObject::get returns false and an empty vector when provided with an empty property name using ENCODING_BASE64_URL.
 *
 * This test verifies that the API correctly handles the scenario where an empty property name is passed. It ensures that the function returns false and does not populate the output vector when the input is invalid.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 110@n
 * **Priority:** (High) This test is crucial as it confirms proper handling of invalid input for overall functionality@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke get method on DrmJsonObject with an empty property name and ENCODING_BASE64_URL encoding | input: property = "", encoding = DrmJsonObject::ENCODING_BASE64_URL; output: values vector is empty | API returns false and the values vector remains empty (EXPECT_FALSE(result) and EXPECT_TRUE(values.empty())) | Should Fail |
 */
TEST_F(DrmJsonObjectTests, EmptyPropertyName_Base64Url) {
    std::cout << "Entering EmptyPropertyName_Base64Url test" << std::endl;
    
    DrmJsonObject obj;
    std::vector<uint8_t> values;
    std::string property = "";
    
    std::cout << "Invoking get with empty property name"
              << ", encoding: ENCODING_BASE64_URL" << std::endl;
    bool result = obj.get(property, values, DrmJsonObject::ENCODING_BASE64_URL);
    std::cout << "Returned from get: " << std::boolalpha << result << std::endl;
    std::cout << "Size of retrieved vector: " << values.size() << std::endl;
    
    EXPECT_FALSE(result);
    EXPECT_TRUE(values.empty());
    
    std::cout << "Exiting EmptyPropertyName_Base64Url test" << std::endl;
}
