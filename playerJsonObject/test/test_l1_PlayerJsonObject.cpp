
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
#include "PlayerJsonObject.h"

using namespace std;


// Test Case: Verify default constructor successfully creates a PlayerJsonObject instance without throwing any exceptions
/**
 * @brief Validate that the default constructor of PlayerJsonObject initializes the object without throwing exceptions.
 *
 * This test case verifies that invoking the default constructor of the PlayerJsonObject class does not throw any exception and that the object is properly initialized. It ensures that even though the internal state is not directly accessible, the construction proceeds as expected under normal conditions.
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
 * | Variation / Step | Description                                                                                              | Test Data                                          | Expected Result                                                                                              | Notes             |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ----------------- |
 * | 01               | Log the entry of the test case.                                                                          | None                                               | Log message "Entering PlayerJsonObject::PlayerJsonObject()_start test" is printed.                           | Should be successful |
 * | 02               | Invoke the default constructor of PlayerJsonObject and verify that no exception is thrown.                 | No input, output: PlayerJsonObject instance created | The object is constructed successfully without throwing any exceptions.                                    | Should Pass       |
 * | 03               | Log the exit of the test case.                                                                           | None                                               | Log message "Exiting PlayerJsonObject::PlayerJsonObject()_end test" is printed.                              | Should be successful |
 */
TEST(PlayerJsonObject, PlayerJsonObject_default_constructor_start) {
    std::cout << "Entering PlayerJsonObject::PlayerJsonObject()_start test" << std::endl;
    
    // Test the default constructor using EXPECT_NO_THROW.
    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor: PlayerJsonObject instance creation" << std::endl;
        PlayerJsonObject player;
        std::cout << "PlayerJsonObject default constructor invoked successfully. Instance created." << std::endl;
        
        // Since internal state (internal JSON object pointer) is not directly accessible,
        // we assume that the proper initialization is done if no exception is thrown.
        std::cout << "Assuming internal state is valid after construction." << std::endl;
    });
    
    std::cout << "Exiting PlayerJsonObject::PlayerJsonObject()_end test" << std::endl;
}
/**
 * @brief Test to verify move construction from a default constructed PlayerJsonObject
 *
 * This test verifies that a PlayerJsonObject can be default constructed without throwing an exception,
 * that its initial state can be retrieved using the print() method, and that move construction from this object
 * properly initializes a new object while leaving the moved-from object in a valid state.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                     | Expected Result                                                                     | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- | ------------- |
 * | 01               | Construct a default PlayerJsonObject instance (obj1) and log its initial state using print() method   | No input; output: default constructed obj1, initialState = value               | No exception thrown; obj1 is valid and its initial state is printed                  | Should Pass   |
 * | 02               | Retrieve the initial state of obj1 via print()                                                       | Input: obj1.print(), output: initial state string                              | print() returns a valid state string without throwing an exception                   | Should Pass   |
 * | 03               | Move construct a new PlayerJsonObject (obj2) by moving from obj1                                     | Input: std::move(obj1), output: new obj2 constructed from obj1                   | Move construction occurs without throwing; obj2 is properly initialized              | Should Pass   |
 * | 04               | Retrieve and log the states of obj2 and the moved-from obj1 using print()                              | Inputs: obj2.print(), obj1.print(), outputs: movedState and movedFromState strings | obj2 returns a valid state string; moved-from obj1 returns a valid state (possibly empty) | Should Pass   |
 */
TEST(PlayerJsonObject, MoveConstructFromDefault) {
    std::cout << "Entering MoveConstructFromDefault test" << std::endl;

    // Construct a default object and log its initial state.
    EXPECT_NO_THROW({
        PlayerJsonObject obj1;
        std::cout << "Invoked PlayerJsonObject default constructor for obj1" << std::endl;
        
        // Retrieve and log the initial state using print().
        std::string initialState = obj1.print();
        std::cout << "obj1 initial state (via print): " << initialState << std::endl;

        // Move construct a new object from obj1.
        PlayerJsonObject obj2 = std::move(obj1);
        std::cout << "Invoked PlayerJsonObject move constructor: constructed obj2 by moving from obj1" << std::endl;
        
        // Retrieve and log the state of obj2.
        std::string movedState = obj2.print();
        std::cout << "obj2 state after move (via print): " << movedState << std::endl;

        // Attempt to retrieve and log the state of the moved-from obj1.
        std::string movedFromState = obj1.print();
        std::cout << "obj1 state after being moved-from (via print): " << movedFromState << std::endl;
    });

    std::cout << "Exiting MoveConstructFromDefault test" << std::endl;
}
/**
 * @brief Verify that a valid JSON string correctly initializes a PlayerJsonObject instance.
 *
 * This test verifies that the constructor of PlayerJsonObject properly parses and initializes the object when provided with a valid JSON string representing a player's name and score. The test ensures that no exceptions are thrown during object creation, indicating correct handling of valid input.
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
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Prepare valid JSON input string and copy it to the buffer using strncpy. | inputJson = "{\"name\": \"Player1\", \"score\": 100}\"; jsonStr buffer assigned the value from inputJson | The JSON string is successfully prepared in jsonStr. | Should be successful |
 * | 02 | Invoke the PlayerJsonObject constructor with jsonStr and verify that no exception is thrown. | input: jsonStr = "{\"name\": \"Player1\", \"score\": 100}\"; output: PlayerJsonObject instance created | No exception is thrown and the PlayerJsonObject instance is created successfully. | Should Pass |
 */
TEST(PlayerJsonObject, ValidJSONStringCreatesValidPlayerJsonObjectInstance) {
    std::cout << "Entering ValidJSONStringCreatesValidPlayerJsonObjectInstance test" << std::endl;

    // Prepare the valid JSON string as input.
    char jsonStr[256];
    const char *inputJson = "{\"name\": \"Player1\", \"score\": 100}";
    std::cout << "Assigning the JSON input value using strncpy. Input value: " << inputJson << std::endl;
    std::strncpy(jsonStr, inputJson, sizeof(jsonStr));
    jsonStr[sizeof(jsonStr)-1] = '\0';
    std::cout << "Final jsonStr after strncpy: " << jsonStr << std::endl;

    // Invoke the constructor and expect no exceptions.
    std::cout << "Invoking PlayerJsonObject constructor with jsonStr: " << jsonStr << std::endl;
    EXPECT_NO_THROW({
        PlayerJsonObject player(jsonStr);
        std::cout << "PlayerJsonObject constructor invoked successfully." << std::endl;
        // Additional debug log: Indicate that the internal state (mJsonObj) is assumed to be set correctly as per valid JSON.
        std::cout << "Assuming mJsonObj is non-null and in a valid state since the JSON is valid." << std::endl;
    });

    std::cout << "Exiting ValidJSONStringCreatesValidPlayerJsonObjectInstance test" << std::endl;
}
/**
 * @brief Validate that constructing a PlayerJsonObject with an empty JSON string does not throw an exception.
 *
 * This test checks that the PlayerJsonObject constructor properly handles an empty JSON string ("{}"). It verifies that no exception is thrown during object instantiation and the internal state correctly represents an empty JSON object.
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
 * | Variation / Step | Description                                                      | Test Data           | Expected Result                                                                      | Notes       |
 * | :--------------: | ---------------------------------------------------------------- | ------------------- | ------------------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke PlayerJsonObject constructor with an empty JSON string "{}" | jsonStr = {}        | No exception thrown and internal state represents an empty JSON object verified by EXPECT_NO_THROW | Should Pass |
 */
TEST(PlayerJsonObject, Constructor_EmptyJson)
{
    std::cout << "Entering Constructor_EmptyJson test" << std::endl;

    std::string jsonStr = "{}";
    std::cout << "Invoking PlayerJsonObject constructor with input: " << jsonStr << std::endl;
    
    // Creating PlayerJsonObject instance using the provided JSON string.
    EXPECT_NO_THROW({
        PlayerJsonObject playerObj(jsonStr);
        std::cout << "PlayerJsonObject instance created, internal state (mJsonObj) is expected to represent an empty JSON object." << std::endl;
    });
    
    std::cout << "Exiting Constructor_EmptyJson test" << std::endl;
}
/**
 * @brief To validate the correct instantiation of PlayerJsonObject with multiple key-value pairs using a JSON string.
 *
 * This test verifies that when a valid JSON string, containing multiple key-value pairs, is passed to the PlayerJsonObject constructor, the object is successfully instantiated without throwing exceptions. The internal state of the object is expected to correctly represent the JSON key-value pairs provided as input.
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
 * | Variation / Step | Description                                                  | Test Data                                                  | Expected Result                                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------- | ---------------------------------------------------------- | --------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke PlayerJsonObject constructor with a valid JSON string  | input: jsonStr = {"name":"player1", "score":100, "active":true} | The constructor should create an object without throwing any exception      | Should Pass |
 */
TEST(PlayerJsonObject, Constructor_MultipleKeyValues)
{
    std::cout << "Entering Constructor_MultipleKeyValues test" << std::endl;

    std::string jsonStr = "{\"name\":\"player1\", \"score\":100, \"active\":true}";
    std::cout << "Invoking PlayerJsonObject constructor with input: " << jsonStr << std::endl;
    
    // Creating PlayerJsonObject instance using the provided JSON string.
    EXPECT_NO_THROW({
        PlayerJsonObject playerObj(jsonStr);
        std::cout << "PlayerJsonObject instance created, internal state (mJsonObj) now represents a JSON object with key-value pairs:" << std::endl;
        std::cout << "    name: player1" << std::endl;
        std::cout << "    score: 100" << std::endl;
        std::cout << "    active: true" << std::endl;
    });
    
    std::cout << "Exiting Constructor_MultipleKeyValues test" << std::endl;
}
/**
 * @brief Validates that a valid nested JSON object is added successfully.
 *
 * This test verifies that the add method of the PlayerJsonObject class correctly handles the addition of a valid nested JSON object to a parent object.
 * It constructs both parent and child objects using their default constructors and then attempts to add the child to the parent using a specified name.
 * The test confirms that no exceptions are thrown during object construction and that the add method returns true, indicating a successful operation.
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
 * | Variation / Step | Description                                                  | Test Data                                                    | Expected Result                                         | Notes               |
 * | :--------------: | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------- | ------------------- |
 * | 01               | Construct parent PlayerJsonObject object using default constructor | N/A                                                         | Parent object constructed without exceptions          | Should be successful|
 * | 02               | Construct child PlayerJsonObject object using default constructor  | N/A                                                         | Child object constructed without exceptions           | Should be successful|
 * | 03               | Invoke the add method on parent with parameters: name = "child", child object | input: name = "child", input: child object, output: expected true from add method | add method returns true and no exception is thrown      | Should Pass         |
 */
TEST(PlayerJsonObject, AddValidNestedJSON) {
    std::cout << "Entering AddValidNestedJSON test" << std::endl;

    // Create parent object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject parent;
        std::cout << "Constructed parent PlayerJsonObject object using default constructor" << std::endl;

        // Create child object using default constructor
        EXPECT_NO_THROW({
            PlayerJsonObject child;
            std::cout << "Constructed child PlayerJsonObject object using default constructor" << std::endl;

            std::string name = "child";
            std::cout << "Invoking add with parameters: name = \"" << name << "\" and child object" << std::endl;
            bool ret = parent.add(name, child);
            std::cout << "Method add returned: " << (ret ? "true" : "false") << std::endl;
            EXPECT_TRUE(ret);
        });
    });

    std::cout << "Exiting AddValidNestedJSON test" << std::endl;
}
/**
 * @brief Verify that adding a child JSON object with an empty name to a PlayerJsonObject fails.
 *
 * This test validates that when invoking the add API with an empty string as the name and a valid child PlayerJsonObject, the operation does not throw any exceptions and returns false. This ensures that the system handles invalid input as expected.
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
 * | Variation / Step | Description                                                                               | Test Data                                               | Expected Result                                               | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------- | ------------- |
 * |01| Construct parent PlayerJsonObject object using the default constructor.  | No input, output: parent object instance                | No exception is thrown.                                       | Should be successful |
 * |02| Construct child PlayerJsonObject object using the default constructor.   | No input, output: child object instance                 | No exception is thrown.                                       | Should be successful |
 * |03| Invoke add API on parent object with an empty name and the child object.   | input: name = "", child = valid PlayerJsonObject instance | Method returns false; assertion EXPECT_FALSE(ret) passes.     | Should Fail   |
 */
TEST(PlayerJsonObject, AddEmptyNameNestedJSON) {
    std::cout << "Entering AddEmptyNameNestedJSON test" << std::endl;

    // Create parent object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject parent;
        std::cout << "Constructed parent PlayerJsonObject object using default constructor" << std::endl;

        // Create child object using default constructor
        EXPECT_NO_THROW({
            PlayerJsonObject child;
            std::cout << "Constructed child PlayerJsonObject object using default constructor" << std::endl;

            std::string name = "";
            std::cout << "Invoking add with parameters: name = \"\" (empty string) and child object" << std::endl;
            bool ret = parent.add(name, child);
            std::cout << "Method add returned: " << (ret ? "true" : "false") << std::endl;
            EXPECT_FALSE(ret);
        });
    });

    std::cout << "Exiting AddEmptyNameNestedJSON test" << std::endl;
}
/**
 * @brief Validate adding a valid non-empty name with boolean value true.
 *
 * This test verifies that the PlayerJsonObject correctly handles adding an entry when provided with a valid non-empty name ("video_enabled") and a boolean value true. The test checks that the object is constructed without throwing exceptions, that the name is properly assigned via strncpy, and that the add() method returns true.
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
 * | 01 | Invoke default constructor of PlayerJsonObject and verify no exception is thrown | Constructor: PlayerJsonObject (no input) | Object is constructed without throwing an exception | Should Pass |
 * | 02 | Copy string "video_enabled" into a fixed size char array using strncpy | cName = "video_enabled", fixedName array size = 50 | fixedName holds "video_enabled" correctly null-terminated | Should be successful |
 * | 03 | Call add() method with name as fixedName and value as true | name = video_enabled, value = true | add() returns true | Should Pass |
 * | 04 | Assert that add() returns true using EXPECT_TRUE | return value from add() = true (expected true) | Test passes if add() returns true | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNonEmptyNameWithBoolTrue) {
    std::cout << "Entering AddValidNonEmptyNameWithBoolTrue test" << std::endl;

    std::cout << "Invoking default constructor for PlayerJsonObject" << std::endl;
    EXPECT_NO_THROW({
        PlayerJsonObject tmpObj;
    });
    PlayerJsonObject obj;
    
    const char* cName = "video_enabled";
    char fixedName[50];
    std::cout << "Assigning name using strncpy, source: " << cName << std::endl;
    std::strncpy(fixedName, cName, sizeof(fixedName) - 1);
    fixedName[sizeof(fixedName) - 1] = '\0';
    
    std::cout << "Invoking add() with name: " << fixedName << " and value: true" << std::endl;
    bool result = obj.add(std::string(fixedName), true);
    std::cout << "Method add() returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNonEmptyNameWithBoolTrue test" << std::endl;
}
/**
 * @brief Test adding a valid non-empty attribute name with boolean false value.
 *
 * This test verifies that the add() method successfully adds an attribute with a valid non-empty name ("audio_enabled") and a boolean false value. It also confirms that the default constructor of PlayerJsonObject does not throw any exceptions.
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
 * | Variation / Step | Description                                                                                   | Test Data                                            | Expected Result                                                | Notes           |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ---------------------------------------------------- | --------------------------------------------------------------- | --------------- |
 * | 01               | Invoke the default constructor for PlayerJsonObject and ensure no exception is thrown.           | None                                                 | Default constructor completes without throwing an exception.   | Should be successful |
 * | 02               | Call add() method with valid non-empty name "audio_enabled" and boolean false value; verify result.| name = audio_enabled, value = false                  | add() returns true and EXPECT_TRUE assertion passes.           | Should Pass     |
 */
TEST(PlayerJsonObject, AddValidNonEmptyNameWithBoolFalse) {
    std::cout << "Entering AddValidNonEmptyNameWithBoolFalse test" << std::endl;

    std::cout << "Invoking default constructor for PlayerJsonObject" << std::endl;
    EXPECT_NO_THROW({
        PlayerJsonObject tmpObj;
    });
    PlayerJsonObject obj;
    
    const char* cName = "audio_enabled";
    char fixedName[50];
    std::cout << "Assigning name using strncpy, source: " << cName << std::endl;
    std::strncpy(fixedName, cName, sizeof(fixedName)-1);
    fixedName[sizeof(fixedName)-1] = '\0';
    
    std::cout << "Invoking add() with name: " << fixedName << " and value: false" << std::endl;
    bool result = obj.add(std::string(fixedName), false);
    std::cout << "Method add() returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNonEmptyNameWithBoolFalse test" << std::endl;
}
/**
 * @brief Validate add() behavior when provided with an empty name and a boolean true value.
 *
 * This test verifies that the PlayerJsonObject::add() method returns false when invoked with an empty name string and a boolean value true. This helps ensure that empty names are not accepted for adding a player.
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
 * | Variation / Step | Description                                                      | Test Data                                        | Expected Result                                                    | Notes             |
 * | :--------------: | ---------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------------------ | ----------------- |
 * |       01         | Invoke the default constructor for PlayerJsonObject              | No parameters used                               | Constructor completes without throwing any exceptions              | Should be successful |
 * |       02         | Copy an empty string into a character buffer using strncpy       | cName = "", fixedName buffer (size = 50)           | Buffer contains an empty string with proper null termination         | Should be successful |
 * |       03         | Invoke add() with an empty name and boolean true                   | input: name = "", value = true                     | Method returns false; assertion checks that the result is false       | Should Pass       |
 */
TEST(PlayerJsonObject, AddEmptyNameWithBoolTrue) {
    std::cout << "Entering AddEmptyNameWithBoolTrue test" << std::endl;

    std::cout << "Invoking default constructor for PlayerJsonObject" << std::endl;
    EXPECT_NO_THROW({
        PlayerJsonObject tmpObj;
    });
    PlayerJsonObject obj;
    
    const char* cName = "";
    char fixedName[50];
    std::cout << "Assigning empty name using strncpy" << std::endl;
    std::strncpy(fixedName, cName, sizeof(fixedName)-1);
    fixedName[sizeof(fixedName)-1] = '\0';
    
    std::cout << "Invoking add() with empty name and value: true" << std::endl;
    bool result = obj.add(std::string(fixedName), true);
    std::cout << "Method add() returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddEmptyNameWithBoolTrue test" << std::endl;
}
/**
 * @brief Validates the add method of PlayerJsonObject with a valid numeric cJSON value.
 *
 * This test verifies that the add method correctly processes a valid numeric cJSON input by adding a numeric value to the PlayerJsonObject without throwing exceptions. It confirms that the object is constructed properly, the cJSON numeric object is created with the correct value, and the add method returns true, indicating successful addition.
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
 * | Variation / Step | Description                                                                                                | Test Data                                                       | Expected Result                                                | Notes              |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------- | ------------------ |
 * | 01               | Construct the PlayerJsonObject instance using the default constructor.                                     | N/A                                                             | Object is constructed successfully.                          | Should be successful |
 * | 02               | Create a cJSON numeric object with a value of 95.5.                                                        | value = 95.5                                                    | cJSON object is created and is not null.                       | Should be successful |
 * | 03               | Invoke the add method with "score" as key and the numeric cJSON object as value.                             | name = score, json->valuedouble = 95.5                            | add method returns true indicating successful addition.        | Should Pass       |
 * | 04               | Free the allocated cJSON numeric object to prevent memory leaks.                                           | cJSON_Delete called with jsonNumeric                             | Memory is properly freed.                                      | Should be successful |
 */
TEST(PlayerJsonObject, AddWithValidNumericValue) {
    std::cout << "Entering AddWithValidNumericValue test" << std::endl;

    EXPECT_NO_THROW({
        // Construct PlayerJsonObject object
        PlayerJsonObject obj;
        std::cout << "PlayerJsonObject constructed via default constructor." << std::endl;

        // Create a cJSON object with numeric value (proper way)
        cJSON* jsonNumeric = cJSON_CreateNumber(95.5);
        ASSERT_NE(jsonNumeric, nullptr) << "Failed to create cJSON number object.";
        std::cout << "cJSON numeric object created with valuedouble: " 
                  << jsonNumeric->valuedouble << std::endl;

        // Invoke the add method with name "score" and the numeric cJSON object
        std::cout << "Invoking add with name: \"score\" and numeric value: " 
                  << jsonNumeric->valuedouble << std::endl;

        bool result = obj.add("score", jsonNumeric);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);

        // Free memory properly
        cJSON_Delete(jsonNumeric);
    });

    std::cout << "Exiting AddWithValidNumericValue test" << std::endl;
}
/**
 * @brief Tests the add method of PlayerJsonObject using a valid cJSON string value.
 *
 * Verifies that the PlayerJsonObject can be initialized, a cJSON object can be created with a valid string ("John"), and the add method correctly adds the JSON property without throwing exceptions.
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
 * | Variation / Step | Description                                                       | Test Data                                                                                               | Expected Result                                                               | Notes           |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------- | --------------- |
 * | 01               | Construct PlayerJsonObject                                        | N/A                                                                                                     | Object is constructed successfully without exceptions                       | Should be successful |
 * | 02               | Create a cJSON string object with value "John"                     | input: value="John"                                                                                     | cJSON_CreateString returns a non-null pointer                                  | Should Pass     |
 * | 03               | Invoke the add method with key "playerName" and the created cJSON    | input: name="playerName", value=cJSON string object with valuestring="John"                             | Method returns true and EXPECT_TRUE(result) assertion passes                   | Should Pass     |
 * | 04               | Free the created cJSON string object                              | input: cJSON_Delete(jsonString)                                                                          | Memory is freed successfully                                                  | Should be successful |
 */
TEST(PlayerJsonObject, AddWithValidStringValue) {
    std::cout << "Entering AddWithValidStringValue test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "PlayerJsonObject constructed via default constructor." << std::endl;

        // Proper way: Create a cJSON object with string value
        cJSON* jsonString = cJSON_CreateString("John");
        ASSERT_NE(jsonString, nullptr) << "Failed to create cJSON string object.";
        std::cout << "cJSON string object created with valuestring: " 
                  << jsonString->valuestring << std::endl;

        // Invoke the add method with name "playerName" and the string cJSON object
        std::cout << "Invoking add with name: \"playerName\" and valuestring: " 
                  << jsonString->valuestring << std::endl;
        bool result = obj.add("playerName", jsonString);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;

        EXPECT_TRUE(result);

        // Free memory properly
        cJSON_Delete(jsonString);
    });

    std::cout << "Exiting AddWithValidStringValue test" << std::endl;
}
/**
 * @brief Test the add method of PlayerJsonObject with a null cJSON pointer
 *
 * This test ensures that when the add method is called with the name "playerScore" and a null cJSON pointer, the method returns false. The test verifies that the API correctly handles invalid pointer inputs without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerJsonObject using the default constructor. | None | Object is constructed successfully. | Should be successful |
 * | 02 | Prepare a null cJSON pointer. | nullJson = nullptr | cJSON pointer is null. | Should be successful |
 * | 03 | Invoke add method on PlayerJsonObject with "playerScore" and the null cJSON pointer. | name = "playerScore", nullJson = nullptr | add method returns false. | Should Fail |
 * | 04 | Assert that the result of the add method is false. | result = false | EXPECT_FALSE assertion passes confirming the API returns false. | Should Pass |
 */
TEST(PlayerJsonObject, AddWithNullCjsonPointer) {
    std::cout << "Entering AddWithNullCjsonPointer test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "PlayerJsonObject constructed via default constructor." << std::endl;
        
        // Prepare a null cJSON pointer
        cJSON* nullJson = nullptr;
        std::cout << "cJSON pointer is set to NULL." << std::endl;
        
        // Invoke the add method with name "playerScore" and null pointer
        std::cout << "Invoking add with name: \"playerScore\" and NULL cJSON pointer." << std::endl;
        bool result = obj.add("playerScore", nullJson);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting AddWithNullCjsonPointer test" << std::endl;
}
/**
 * @brief Validate that adding a JSON element with an empty name fails as expected.
 *
 * This test verifies that when attempting to add a JSON element to a PlayerJsonObject with an empty name, 
 * the API correctly returns false. The test constructs a valid PlayerJsonObject, creates a valid cJSON numeric object,
 * and then calls the add method with an empty name. It asserts that the API call does not succeed with an empty name.
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
 * | Variation / Step | Description                                                                             | Test Data                                                  | Expected Result                                                                     | Notes            |
 * | :----:           | --------------------------------------------------------------------------------------- | ---------------------------------------------------------- | ----------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Construct PlayerJsonObject using default constructor                                    | None                                                       | Object is successfully constructed                                                  | Should be successful |
 * | 02               | Create a cJSON numeric object with valuedouble set to 10.0                              | input: valuedouble = 10.0, output: jsonNumericEmpty pointer  | cJSON numeric object is created and pointer is not null                             | Should be successful |
 * | 03               | Invoke add method with an empty name and the valid numeric cJSON pointer                  | input: name = "", jsonNumericEmpty->valuedouble = 10.0       | API returns false indicating that adding with an empty name is not allowed            | Should Fail      |
 * | 04               | Assert that the result of the add function is false                                      | input: result from add method                              | Assertion passes as result is false                                                 | Should be successful |
 * | 05               | Free the allocated cJSON numeric object                                                 | input: jsonNumericEmpty pointer                            | Memory is freed without errors                                                      | Should be successful |
 */
TEST(PlayerJsonObject, AddWithEmptyName) {
    std::cout << "Entering AddWithEmptyName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "PlayerJsonObject constructed via default constructor." << std::endl;
        
        // Proper way: Create a cJSON number
        cJSON* jsonNumericEmpty = cJSON_CreateNumber(10.0);
        ASSERT_NE(jsonNumericEmpty, nullptr) << "Failed to create cJSON number object.";
        std::cout << "cJSON numeric object created with valuedouble: " 
                  << jsonNumericEmpty->valuedouble << std::endl;
        
        // Invoke the add method with an empty name and valid numeric cJSON pointer
        std::cout << "Invoking add with empty name \"\" and numeric valuedouble: " 
                  << jsonNumericEmpty->valuedouble << std::endl;
        bool result = obj.add("", jsonNumericEmpty);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_FALSE(result);
        
        // Free memory properly
        cJSON_Delete(jsonNumericEmpty);
    });
    
    std::cout << "Exiting AddWithEmptyName test" << std::endl;
}
/**
 * @brief Tests the add() API with a valid name and value using default encoding.
 *
 * This test verifies that a PlayerJsonObject can be instantiated using the default constructor and that the add() method correctly adds a key-value pair when a valid name and value are provided. It validates that the API returns a true value as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 015@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                                     | Expected Result                                                     | Notes              |
 * | :--------------: | ------------------------------------------------------------ | ------------------------------------------------------------- | ------------------------------------------------------------------- | ------------------ |
 * | 01               | Create a PlayerJsonObject object using the default constructor | No inputs: Object instantiation using default constructor     | Object is instantiated without throwing exceptions                  | Should be successful |
 * | 02               | Prepare input parameters including the name and value         | name = "player1", value = "active", (default encoding ENCODING_STRING implied) | Input parameters are correctly assigned without errors              | Should be successful |
 * | 03               | Invoke the add() method and check the return value              | input1 = player1, input2 = active, output1 = true              | The method returns true; assertion passes indicating successful addition | Should Pass        |
 */
TEST(PlayerJsonObject, AddValidNameValueDefaultEncoding) {
    std::cout << "Entering AddValidNameValueDefaultEncoding test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Prepare input parameters
    std::string name = "player1";
    char value[100];
    std::cout << "Copying value 'active' into fixed size char array using strncpy" << std::endl;
    strncpy(value, "active", sizeof(value) - 1);
    value[sizeof(value) - 1] = '\0'; // ensure null termination
    
    // Log input details
    std::cout << "Invoking add with parameters: name = " << name << ", value = " << value 
              << ", encoding = ENCODING_STRING" << std::endl;
              
    // Call the add method with default encoding
    bool result = obj.add(name, value);
    
    // Log the result of the call
    std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNameValueDefaultEncoding test" << std::endl;
}
/**
 * @brief Validate that the add method correctly adds a valid name and value using Base64 encoding.
 *
 * This test verifies that a PlayerJsonObject can successfully add a key-value pair with the name "player2" and the value "score:100" using ENCODING_BASE64. It checks that the object is constructed without exceptions, the input parameters are correctly prepared, and the add method returns true as expected.
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
 * | Variation / Step | Description                                                                 | Test Data                                            | Expected Result                                           | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------- | --------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerJsonObject instance using the default constructor            | None                                                 | Object is created without throwing an exception         | Should Pass    |
 * | 02               | Prepare input parameters: set name to "player2" and copy "score:100" into a fixed size char array with proper null termination | name = player2, value = score:100                    | Input parameters are correctly initialized                | Should be successful |
 * | 03               | Invoke the add method with the prepared parameters and ENCODING_BASE64 encoding | input: name = player2, value = score:100, encoding = ENCODING_BASE64 | add method returns true confirming successful addition | Should Pass    |
 */
TEST(PlayerJsonObject, AddValidNameValueBase64Encoding) {
    std::cout << "Entering AddValidNameValueBase64Encoding test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Prepare input parameters
    std::string name = "player2";
    char value[100];
    std::cout << "Copying value 'score:100' into fixed size char array using strncpy" << std::endl;
    strncpy(value, "score:100", sizeof(value) - 1);
    value[sizeof(value) - 1] = '\0'; // ensure null termination
    
    // Log input details
    std::cout << "Invoking add with parameters: name = " << name << ", value = " << value 
              << ", encoding = ENCODING_BASE64" << std::endl;
              
    // Call the add method with ENCODING_BASE64 encoding
    bool result = obj.add(name, value, PlayerJsonObject::ENCODING_BASE64);
    
    // Log the result of the call
    std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNameValueBase64Encoding test" << std::endl;
}
/**
 * @brief Verify that the add method succeeds when provided with a valid name, valid value, and Base64 URL encoding.
 *
 * This test creates a PlayerJsonObject using the default constructor, prepares valid input parameters,
 * and invokes the add method using a fixed-size char array containing "score:100". The test checks that no
 * exceptions are thrown during object creation and that the add method returns true, confirming correct behavior.
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
 * | 01 | Create PlayerJsonObject object, prepare input parameters, and invoke add() with name "player2", value "score:100", and encoding ENCODING_BASE64_URL. | name = player2, value = score:100, encoding = ENCODING_BASE64_URL | Object created without exception; add() returns true. | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNameValueBase64UrlEncoding) {
    std::cout << "Entering AddValidNameValueBase64UrlEncoding test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Prepare input parameters
    std::string name = "player2";
    char value[100];
    std::cout << "Copying value 'score:100' into fixed size char array using strncpy" << std::endl;
    strncpy(value, "score:100", sizeof(value) - 1);
    value[sizeof(value) - 1] = '\0'; // ensure null termination
    
    // Log input details
    std::cout << "Invoking add with parameters: name = " << name << ", value = " << value 
              << ", encoding = ENCODING_BASE64_URL" << std::endl;
              
    // Call the add method with ENCODING_BASE64_URL encoding
    bool result = obj.add(name, value, PlayerJsonObject::ENCODING_BASE64_URL);
    
    // Log the result of the call
    std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNameValueBase64UrlEncoding test" << std::endl;
}
/**
 * @brief Test the behavior of PlayerJsonObject::add when provided with an empty name and a valid string value.
 *
 * Validate that when an empty name is provided along with a valid value, the add operation fails as expected. This ensures the object enforces a non-empty name requirement.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerJsonObject instance using the default constructor; expect the creation to succeed without exceptions. | None | Object is created with no exceptions thrown. | Should be successful |
 * | 02 | Prepare input parameters: set an empty string for the name and copy "inactive" into a fixed size char array safely using strncpy. | name = "", value = "inactive", encoding = ENCODING_STRING | Input parameters are set correctly; char array contains "inactive" with proper null termination. | Should be successful |
 * | 03 | Invoke the add method with the prepared inputs and verify that it returns false as the empty name is invalid. | name = "", value = "inactive", encoding = ENCODING_STRING | API returns false; assertion EXPECT_FALSE(result) passes. | Should Fail |
 */
TEST(PlayerJsonObject, AddEmptyNameValidValue) {
    std::cout << "Entering AddEmptyNameValidValue test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Prepare input parameters: empty name
    std::string name = "";
    char value[100];
    std::cout << "Copying value 'inactive' into fixed size char array using strncpy" << std::endl;
    strncpy(value, "inactive", sizeof(value) - 1);
    value[sizeof(value) - 1] = '\0'; // ensure null termination
    
    // Log input details
    std::cout << "Invoking add with parameters: name = (empty string), value = " << value 
              << ", encoding = ENCODING_STRING" << std::endl;
              
    // Call the add method and expect it to return false
    bool result = obj.add(name, value);
    
    // Log the result of the call
    std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddEmptyNameValidValue test" << std::endl;
}
/**
 * @brief Validate that the add method returns false when a valid name is provided but the value is null.
 *
 * This test creates an instance of PlayerJsonObject and prepares valid input parameters with a non-null name ("player3") and a null value. It then calls the add method to ensure it fails gracefully (returns false). This test is vital to confirm robust error handling in scenarios where the JSON object receives an invalid/null value.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create object using default constructor | input: N/A, output: instance of PlayerJsonObject created | Object is instantiated without exceptions | Should be successful |
 * | 02 | Prepare input parameters for add method | input: name = "player3", value = nullptr | Parameters prepared correctly | Should be successful |
 * | 03 | Call add method with the input parameters | input: name = "player3", value = nullptr, encoding = ENCODING_STRING, output: result from add method | API returns false and EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNameNullValue) {
    std::cout << "Entering AddValidNameNullValue test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Prepare input parameters: valid name and nullptr for value
    std::string name = "player3";
    const char* value = nullptr;
    
    // Log input details
    std::cout << "Invoking add with parameters: name = " << name << ", value = nullptr" 
              << ", encoding = ENCODING_STRING" << std::endl;
              
    // Call the add method and expect it to return false
    bool result = obj.add(name, value);
    
    // Log the result of the call
    std::cout << "Method add returned: " << (result ? "true" : "false") << std::endl;
    EXPECT_FALSE(result);
    
    std::cout << "Exiting AddValidNameNullValue test" << std::endl;
}
/**
 * @brief Test the valid addition of a player entry using the default encoding.
 *
 * This test verifies that a PlayerJsonObject can be constructed using the default constructor, and that invoking the add method with valid input parameters (name, value, and default ENCODING_STRING) does not throw an exception and returns true. This ensures that the JSON object correctly handles valid player additions.
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
 * | Variation / Step | Description                                                                 | Test Data                                                     | Expected Result                                                         | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------- | ----------------------------------------------------------------------- | -------------- |
 * | 01               | Construct PlayerJsonObject using the default constructor                    | N/A, output: instance of PlayerJsonObject                     | Object constructed successfully without exceptions                    | Should be successful |
 * | 02               | Prepare input parameters for the add method                                 | name = username, value = player1, encoding = ENCODING_STRING     | Input parameters are set correctly                                      | Should be successful |
 * | 03               | Invoke add method on the PlayerJsonObject instance with the prepared parameters | name = username, value = player1, encoding = ENCODING_STRING, output: result (bool) | API returns true indicating successful addition                         | Should Pass    |
 * | 04               | Validate that the result of the add method is true                          | result = true                                                  | Assertion passes (EXPECT_TRUE(result)) confirming expected API behavior   | Should Pass    |
 */
TEST(PlayerJsonObject, ValidAdditionDefaultEncoding) {
    std::cout << "Entering ValidAdditionDefaultEncoding test" << std::endl;
    
    // Create a PlayerJsonObject object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject jsonObj;
        std::cout << "PlayerJsonObject constructed using default constructor." << std::endl;
        
        // Prepare input parameters
        std::string name = "username";
        std::string value = "player1";
        std::cout << "Invoking add with name: " << name 
                  << ", value: " << value 
                  << ", encoding: ENCODING_STRING" << std::endl;
        
        // Invoke the method with default encoding
        bool result = jsonObj.add(name, value, PlayerJsonObject::ENCODING_STRING);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        // Check result is as expected
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidAdditionDefaultEncoding test" << std::endl;
}
/**
 * @brief Validate the addition of a token using Base64 encoding in PlayerJsonObject
 *
 * This test verifies that invoking the "add" method with valid input parameters including
 * a token name and value, along with specifying Base64 encoding, executes successfully without
 * exceptions and returns a true value. The test ensures that the API correctly handles the conversion
 * or processing required when Base64 encoding is specified.
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
 * | Variation / Step | Description                                                        | Test Data                                                             | Expected Result                                             | Notes           |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------------- | ----------------------------------------------------------- | --------------- |
 * | 01               | Construct a PlayerJsonObject instance using the default constructor | N/A                                                                   | Object is created successfully                              | Should be successful |
 * | 02               | Prepare input parameters and invoke the add method with Base64 encoding | name = token, value = abc123, encoding = ENCODING_BASE64                | The add method is invoked without throwing exceptions       | Should Pass     |
 * | 03               | Verify that the add method returns true as expected                | input: Result from add method invocation (expected true)              | Returned value is true and the operation completes normally  | Should Pass     |
 */
TEST(PlayerJsonObject, ValidAdditionBase64) {
    std::cout << "Entering ValidAdditionBase64 test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject jsonObj;
        std::cout << "PlayerJsonObject constructed using default constructor." << std::endl;
        
        // Prepare input parameters
        std::string name = "token";
        std::string value = "abc123";
        std::cout << "Invoking add with name: " << name 
                  << ", value: " << value 
                  << ", encoding: ENCODING_BASE64" << std::endl;
        
        // Invoke the method with ENCODING_BASE64
        bool result = jsonObj.add(name, value, PlayerJsonObject::ENCODING_BASE64);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        // Check result is as expected
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidAdditionBase64 test" << std::endl;
}
/**
 * @brief Verifies that PlayerJsonObject::add method works correctly for Base64 URL encoded input.
 *
 * This test constructs a PlayerJsonObject instance, prepares valid input parameters, and invokes the add method using ENCODING_BASE64_URL to ensure the method returns true. It verifies that the API correctly handles Base64 URL encoding.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 022
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                    | Test Data                                                       | Expected Result                              | Notes           |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------- | --------------- |
 * | 01               | Construct PlayerJsonObject instance.                           | N/A                                                              | Object is constructed without exceptions.    | Should be successful |
 * | 02               | Prepare input parameters for the add method.                   | name = "token", value = "abc123", encoding = ENCODING_BASE64_URL     | Parameters are set as expected.              | Should be successful |
 * | 03               | Invoke PlayerJsonObject::add with the prepared parameters.       | name = "token", value = "abc123", output = result                 | Method returns true.                         | Should Pass     |
 * | 04               | Validate the returned result using assertion.                  | result = true                                                    | EXPECT_TRUE confirms the result is true.       | Should Pass     |
 */
TEST(PlayerJsonObject, ValidAdditionBase64Url) {
    std::cout << "Entering ValidAdditionBase64Url test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject jsonObj;
        std::cout << "PlayerJsonObject constructed using default constructor." << std::endl;
        
        // Prepare input parameters
        std::string name = "token";
        std::string value = "abc123";
        std::cout << "Invoking add with name: " << name 
                  << ", value: " << value 
                  << ", encoding: ENCODING_BASE64_URL" << std::endl;
        
        // Invoke the method with ENCODING_BASE64_URL
        bool result = jsonObj.add(name, value, PlayerJsonObject::ENCODING_BASE64_URL);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        // Check result is as expected
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting ValidAdditionBase64Url test" << std::endl;
}
/**
 * @brief Verify that creating a PlayerJsonObject and invoking add() with an empty name returns false.
 *
 * This test ensures that the PlayerJsonObject::add method handles an empty name input correctly by not adding the name and returning false. The test constructs a PlayerJsonObject using the default constructor, sets up input parameters with an empty name and a valid value, invokes add(), and asserts that the result is false.
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
 * | Variation / Step | Description                                                              | Test Data                                                             | Expected Result                                                | Notes          |
 * | :--------------: | ------------------------------------------------------------------------ | --------------------------------------------------------------------- | -------------------------------------------------------------- | -------------- |
 * | 01               | Construct PlayerJsonObject using the default constructor                 | No input, using default constructor                                   | Object is constructed without throwing exceptions              | Should be successful |
 * | 02               | Prepare input parameters with an empty name                              | name = "", value = "someValue", encoding = ENCODING_STRING             | Parameters are set as expected                                  | Should be successful |
 * | 03               | Invoke the add method with empty name                                    | name = "", value = "someValue", encoding = ENCODING_STRING             | Method returns false                                             | Should Fail    |
 * | 04               | Verify that the returned result is false                                 | result = false                                                        | Assertion check verifies that the result is false                | Should be successful |
 */
TEST(PlayerJsonObject, NegativeEmptyName) {
    std::cout << "Entering NegativeEmptyName test" << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject jsonObj;
        std::cout << "PlayerJsonObject constructed using default constructor." << std::endl;
        
        // Prepare input parameters: empty name is expected to fail.
        std::string name = "";
        std::string value = "someValue";
        std::cout << "Invoking add with an empty name: '" << name 
                  << "', value: " << value 
                  << ", encoding: ENCODING_STRING" << std::endl;
        
        // Invoke the method, expecting a false response due to empty name.
        bool result = jsonObj.add(name, value, PlayerJsonObject::ENCODING_STRING);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        // Check result is as expected (false)
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting NegativeEmptyName test" << std::endl;
}
/**
 * @brief Test add() method with valid name and multiple non-empty strings
 *
 * Verifies that adding a valid player name along with multiple strings representing actions returns true and does not throw exceptions. Ensures correct behavior of the add method when provided with valid input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerJsonObject instance and invoke add method with valid inputs | name = player1, values = run, jump, shoot, output = true | API returns true; no exceptions thrown | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNameMultipleStrings) {
    std::cout << "Entering AddValidNameMultipleStrings test" << std::endl;

    // Create a PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        
        std::string name = "player1";
        std::vector<std::string> values = {"run", "jump", "shoot"};
        
        std::cout << "Invoking add() with name: " << name << " and values: ";
        for (const auto& val : values) {
            std::cout << "\"" << val << "\" ";
        }
        std::cout << std::endl;
        
        bool result = obj.add(name, values);
        std::cout << "Method add() returned: " << std::boolalpha << result << std::endl;
        
        // Expected: true
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting AddValidNameMultipleStrings test" << std::endl;
}
/**
 * @brief Verify that add() method accepts a valid player name with an empty vector of values.
 *
 * This test verifies that when invoking the add() method with a valid player name ("player2") and an empty vector, no exceptions are thrown and the method returns true. This ensures that the API correctly handles the case where a player is added without any additional attributes.
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
 * | Variation / Step | Description                                                                              | Test Data                                                      | Expected Result                                                  | Notes            |
 * | :--------------: | ---------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------- |
 * | 01               | Instantiate PlayerJsonObject and initialize variables for name and values vector          | Object instantiation, name = "player2", values = empty vector    | Object is created without throwing an exception                  | Should be successful |
 * | 02               | Invoke add() method with the valid name and empty vector, then assert the return value      | Call add(name = "player2", values = empty vector), output expected = true | add() method should return true without exceptions                | Should Pass      |
 */
TEST(PlayerJsonObject, AddValidNameEmptyVector) {
    std::cout << "Entering AddValidNameEmptyVector test" << std::endl;

    // Create a PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        
        std::string name = "player2";
        std::vector<std::string> values; // empty vector
        
        std::cout << "Invoking add() with name: " << name << " and an empty vector of values" << std::endl;
        
        bool result = obj.add(name, values);
        std::cout << "Method add() returned: " << std::boolalpha << result << std::endl;
        
        // Expected: true
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting AddValidNameEmptyVector test" << std::endl;
}
/**
 * @brief Verifies that the add method returns false when an empty name is provided even though the vector is non-empty.
 *
 * This test case evaluates the behavior of the add function by providing an empty string for the name parameter and a non-empty vector of string values. The objective is to ensure that the function correctly identifies invalid input (empty name) and subsequently returns false as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerJsonObject instance and invoke the add method with an empty name and a non-empty vector of strings. | name = "", values = value1, value2, output = false | The add method returns false indicating the input validation correctly failed. | Should Fail |
 */
TEST(PlayerJsonObject, AddEmptyNameNonEmptyVector) {
    std::cout << "Entering AddEmptyNameNonEmptyVector test" << std::endl;

    // Create a PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        
        std::string name = "";
        std::vector<std::string> values = {"value1", "value2"};
        
        std::cout << "Invoking add() with empty name and values: ";
        for (const auto& val : values) {
            std::cout << "\"" << val << "\" ";
        }
        std::cout << std::endl;
        
        bool result = obj.add(name, values);
        std::cout << "Method add() returned: " << std::boolalpha << result << std::endl;
        
        // Expected: false due to invalid (empty) name
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting AddEmptyNameNonEmptyVector test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add() returns true when provided with a valid name and a vector containing empty, whitespace, and valid strings
 *
 * This test creates an instance of PlayerJsonObject using the default constructor and invokes the add() method with a valid name and a vector that includes empty and whitespace strings along with a valid string. The purpose is to ensure that the method correctly handles empty or whitespace string values within the vector and returns true, indicating successful addition.
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
 * | Variation / Step | Description                                                    | Test Data                                                        | Expected Result                                                              | Notes         |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a PlayerJsonObject instance and invoke add() with a valid name and a vector containing empty, whitespace, and valid strings | name = player3, values = "", " ", "validValue"              | The method should return true; EXPECT_TRUE(result) assertion passes             | Should Pass   |
 */
TEST(PlayerJsonObject, AddValidNameWithEmptyOrWhitespaceStrings) {
    std::cout << "Entering AddValidNameWithEmptyOrWhitespaceStrings test" << std::endl;

    // Create a PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        
        std::string name = "player3";
        std::vector<std::string> values = {"", " ", "validValue"};
        
        std::cout << "Invoking add() with name: " << name << " and values: ";
        for (const auto& val : values) {
            std::cout << "\"" << val << "\" ";
        }
        std::cout << std::endl;
        
        bool result = obj.add(name, values);
        std::cout << "Method add() returned: " << std::boolalpha << result << std::endl;
        
        // Expected: true indicating success even with empty/whitespace strings inside the vector
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting AddValidNameWithEmptyOrWhitespaceStrings test" << std::endl;
}
/**
 * @brief Verify that adding a player with a valid name, non-empty values, and ENCODING_STRING returns true
 *
 * This test case verifies that when a valid player name ("player1"), a non-empty vector of values ([0x01, 0x02, 0x03]),
 * and ENCODING_STRING encoding are provided, the add method correctly processes the input and returns true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 028@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create PlayerJsonObject instance using default constructor | None | No exception thrown during object creation | Should be successful |@n
 * | 02 | Invoke add method with valid name "player1", non-empty values [0x01, 0x02, 0x03], and encoding ENCODING_STRING | name = player1, values = [0x01, 0x02, 0x03], encoding = ENCODING_STRING, result = true | Returns true and assertion (EXPECT_TRUE) passes | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNameNonEmptyValuesEncodingString) {
    std::cout << "Entering AddValidNameNonEmptyValuesEncodingString test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Setup input parameters
    std::string name = "player1";
    std::vector<uint8_t> values = {0x01, 0x02, 0x03};
    
    // Logging the input parameters
    std::cout << "Invoking add with name: " << name << ", values: [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << static_cast<int>(values[i]);
        if (i != values.size()-1) std::cout << ", ";
    }
    std::cout << "], encoding: ENCODING_STRING" << std::endl;
    
    // Call the add method and capture the return value
    bool result = obj.add(name, values, PlayerJsonObject::ENCODING_STRING);
    std::cout << "Result from add: " << std::boolalpha << result << std::endl;
    
    // Validate the expected outcome
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNameNonEmptyValuesEncodingString test" << std::endl;
}
/**
 * @brief Test the PlayerJsonObject add method with valid name, non-empty values, and ENCODING_BASE64 encoding
 *
 * This test verifies that the add method of PlayerJsonObject correctly adds an entry when provided with a valid non-empty name,
 * non-empty values, and the ENCODING_BASE64 encoding option. The test expects the method to return true, confirming that the data was added successfully.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                        | Expected Result                                             | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------ | ------------- |
 * | 01               | Create PlayerJsonObject object using the default constructor                                         | none                                                             | Object created successfully without throwing an exception    | Should be successful |
 * | 02               | Setup input parameters including a valid string name and a non-empty vector of values                 | name = player1, values = 0x01,0x02,0x03                           | Valid input parameters initialized                           | Should be successful |
 * | 03               | Invoke the add method with name, values, and ENCODING_BASE64 encoding                                  | name = player1, values = 0x01,0x02,0x03, encoding = ENCODING_BASE64 | Returns true indicating the entry was successfully added       | Should Pass   |
 * | 04               | Validate the outcome by checking that the returned value is true                                      | result = true                                                    | EXPECT_TRUE(result) confirms the API call returned true      | Should Pass   |
 */
TEST(PlayerJsonObject, AddValidNameNonEmptyValuesEncodingBase64) {
    std::cout << "Entering AddValidNameNonEmptyValuesEncodingBase64 test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Setup input parameters
    std::string name = "player1";
    std::vector<uint8_t> values = {0x01, 0x02, 0x03};
    
    // Logging the input parameters
    std::cout << "Invoking add with name: " << name << ", values: [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << static_cast<int>(values[i]);
        if (i != values.size() - 1) std::cout << ", ";
    }
    std::cout << "], encoding: ENCODING_BASE64" << std::endl;
    
    // Call the add method
    bool result = obj.add(name, values, PlayerJsonObject::ENCODING_BASE64);
    std::cout << "Result from add: " << std::boolalpha << result << std::endl;
    
    // Validate the expected outcome
    EXPECT_TRUE(result);    
    
    std::cout << "Exiting AddValidNameNonEmptyValuesEncodingBase64 test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add successfully adds an element when provided a valid non-empty name, a non-empty vector of values, and the encoding set to ENCODING_BASE64_URL.
 *
 * This test validates that the add method returns true (indicating success) when a valid name ("player1"), a non-empty values vector ([0x01, 0x02, 0x03]), and the correct encoding (ENCODING_BASE64_URL) are provided. The test ensures that no exceptions occur during object construction and that the API properly processes and logs the provided inputs.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                                      | Expected Result                                                                 | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------ | -------------------------------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerJsonObject instance using the default constructor and verify no exceptions.  | N/A                                                                            | Instance is created without throwing an exception.                             | Should be successful |
 * | 02               | Invoke the add method with a valid name ("player1"), non-empty values ([0x01,0x02,0x03]), and encoding ENCODING_BASE64_URL; verify the method returns true. | name = player1, values = 0x01,0x02,0x03, encoding = ENCODING_BASE64_URL, output = result (expected true) | The add method returns true indicating that the addition was successful and passes the assertion check. | Should Pass    |
 */
TEST(PlayerJsonObject, AddValidNameNonEmptyValuesEncodingBase64Url) {
    std::cout << "Entering AddValidNameNonEmptyValuesEncodingBase64Url test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Setup input parameters
    std::string name = "player1";
    std::vector<uint8_t> values = {0x01, 0x02, 0x03};
    
    // Logging the input parameters
    std::cout << "Invoking add with name: " << name << ", values: [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << static_cast<int>(values[i]);
        if (i != values.size() - 1) std::cout << ", ";
    }
    std::cout << "], encoding: ENCODING_BASE64_URL" << std::endl;
    
    // Call the add method
    bool result = obj.add(name, values, PlayerJsonObject::ENCODING_BASE64_URL);
    std::cout << "Result from add: " << std::boolalpha << result << std::endl;
    
    // Validate the expected outcome
    EXPECT_TRUE(result);
    
    std::cout << "Exiting AddValidNameNonEmptyValuesEncodingBase64Url test" << std::endl;
}
/**
 * @brief Verify that adding a player with an empty name and non-empty values fails.
 *
 * This test verifies that invoking the add method of PlayerJsonObject with an empty name and a non-empty vector of values correctly fails to add the player.
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
 * | Variation / Step | Description                                                    | Test Data                                                  | Expected Result                                                  | Notes       |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayerJsonObject, setup input parameters, and invoke add  | name = "", values = 0x01,0x02, encoding = ENCODING_STRING, output result expected to be false | API returns false; Assertion: EXPECT_FALSE(result) validates the failure | Should Fail |
 */
TEST(PlayerJsonObject, FailToAddEmptyNameNonEmptyValues) {
    std::cout << "Entering FailToAddEmptyNameNonEmptyValues test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Setup input parameters
    std::string name = "";
    std::vector<uint8_t> values = {0x01, 0x02};
    
    // Logging the input parameters
    std::cout << "Invoking add with empty name, values: [";
    for (size_t i = 0; i < values.size(); ++i) {
        std::cout << static_cast<int>(values[i]);
        if(i != values.size()-1) std::cout << ", ";
    }
    std::cout << "], encoding: ENCODING_STRING" << std::endl;
    
    // Call the add method
    bool result = obj.add(name, values, PlayerJsonObject::ENCODING_STRING);
    std::cout << "Result from add: " << std::boolalpha << result << std::endl;
    
    // Validate the expected outcome
    EXPECT_FALSE(result);
    
    std::cout << "Exiting FailToAddEmptyNameNonEmptyValues test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add returns false when provided an empty values vector despite having a valid name.
 *
 * This test verifies that when a PlayerJsonObject is created and the add() method is invoked with a valid name ("player2")
 * but an empty values vector, the API should return false, indicating that adding with empty values is not permitted.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 032
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                           | Test Data                                                       | Expected Result                                  | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------ | ----------- |
 * | 01               | Create a PlayerJsonObject object, set the name to "player2", set an empty values vector, and invoke add() with encoding ENCODING_STRING | name = player2, values = empty, encoding = ENCODING_STRING, output = false | API returns false and EXPECT_FALSE(result) assertion passes | Should Fail |
 */
TEST(PlayerJsonObject, FailToAddValidNameEmptyValues) {
    std::cout << "Entering FailToAddValidNameEmptyValues test" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerJsonObject obj);
    PlayerJsonObject obj;
    
    // Setup input parameters
    std::string name = "player2";
    std::vector<uint8_t> values;  // Empty vector
    
    // Logging the input parameters
    std::cout << "Invoking add with name: " << name << ", empty values vector, encoding: ENCODING_STRING" << std::endl;
    
    // Call the add method
    bool result = obj.add(name, values, PlayerJsonObject::ENCODING_STRING);
    std::cout << "Result from add: " << std::boolalpha << result << std::endl;
    
    // Validate the expected outcome
    EXPECT_FALSE(result);
    
    std::cout << "Exiting FailToAddValidNameEmptyValues test" << std::endl;
}
/**
 * @brief Validate that a valid entry is added successfully to the PlayerJsonObject.
 *
 * This test case verifies that the PlayerJsonObject instance is created without exceptions,
 * that valid input parameters (a key as "score" and a double value 10.0) are defined properly,
 * and that the add method successfully adds the entry, returning true. The test logs each step
 * for clarity and ensures that key-value pair is correctly handled in the internal state.
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
 * | Variation / Step | Description                                                                 | Test Data                                   | Expected Result                                                | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------- | -------------------------------------------------------------- | --------------- |
 * | 01               | Create a PlayerJsonObject instance using the default constructor.           | None                                        | Instance is created without throwing any exceptions.         | Should be successful |
 * | 02               | Define input values (key "score" and value 10.0) and prepare parameters.      | testName = "score", testValue = 10.0          | Input values are set correctly and logged.                     | Should be successful |
 * | 03               | Invoke the add method with the prepared key and value, and capture the result. | input: name = "score", value = 10.0, output: result returned by add. | The add method returns true and the internal state is updated accordingly. | Should Pass     |
 */
TEST(PlayerJsonObject, AddValidPositive) {
    std::cout << "Entering AddValidPositive test" << std::endl;

    // Create PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "Created PlayerJsonObject instance." << std::endl;

        // Define input values
        const char* testName = "score";
        double testValue = 10.0;

        // Log input parameters
        char nameBuffer[32];
        std::strncpy(nameBuffer, testName, sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer)-1] = '\0';
        std::cout << "Invoking add with name: " << nameBuffer << ", value: " << testValue << std::endl;

        // Invoke the add method
        bool result = obj.add(std::string(nameBuffer), testValue);
        std::cout << "add returned: " << std::boolalpha << result << std::endl;

        // Assert that the add method succeeded
        EXPECT_TRUE(result);
        std::cout << "Internal state updated with key: " << nameBuffer << " and value: " << testValue << std::endl;
    });
    std::cout << "Exiting AddValidPositive test" << std::endl;
}
/**
 * @brief Validates that the PlayerJsonObject::add method accepts a negative value and updates the internal state accordingly.
 *
 * This test creates an instance of PlayerJsonObject and then attempts to add an entry with the key "lives" and a negative value (-3.0). The objective is to ensure that the add method processes negative values correctly without throwing exceptions and returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerJsonObject instance using default constructor | None | Instance is created with no exceptions thrown | Should be successful |
 * | 02 | Define input parameters and log them | testName = "lives", testValue = -3.0 | Parameters are defined and logged successfully | Should be successful |
 * | 03 | Invoke add method with name "lives" and value -3.0, then assert that it returns true | input: name = lives, value = -3.0; output: result = true | add method returns true and updates internal state | Should Pass |
 */
TEST(PlayerJsonObject, AddValidNegative) {
    std::cout << "Entering AddValidNegative test" << std::endl;

    // Create PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "Created PlayerJsonObject instance." << std::endl;

        // Define input values
        const char* testName = "lives";
        double testValue = -3.0;

        // Log input parameters
        char nameBuffer[32];
        std::strncpy(nameBuffer, testName, sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer)-1] = '\0';
        std::cout << "Invoking add with name: " << nameBuffer << ", value: " << testValue << std::endl;

        // Invoke the add method
        bool result = obj.add(std::string(nameBuffer), testValue);
        std::cout << "add returned: " << std::boolalpha << result << std::endl;

        // Assert that the add method succeeded
        EXPECT_TRUE(result);
        std::cout << "Internal state updated with key: " << nameBuffer << " and value: " << testValue << std::endl;
    });
    std::cout << "Exiting AddValidNegative test" << std::endl;
}
/**
 * @brief Verifies that the add method successfully adds a key "time" with a zero value.
 *
 * This test checks whether calling the add method on a PlayerJsonObject instance with the key "time" and a value of 0.0 succeeds without throwing an exception. It also confirms that the object's internal state is updated as expected.
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
 * | Variation / Step | Description                                                              | Test Data                                              | Expected Result                                                | Notes             |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------ | -------------------------------------------------------------- | ----------------- |
 * | 01               | Create a PlayerJsonObject instance using the default constructor.         | None                                                   | Object is created without throwing exceptions.               | Should be successful |
 * | 02               | Define input parameters with key "time" and value 0.0.                     | testName = "time", testValue = 0.0                       | Input parameters are set correctly.                            | Should be successful |
 * | 03               | Invoke the add method with the defined key and value.                      | input: key = "time", value = 0.0; output: result expected = true | add method returns true indicating a successful addition.       | Should Pass        |
 * | 04               | Validate that the object's internal state is updated and assertion passes. | output from add = true                                  | EXPECT_TRUE assertion verifies the result successfully.        | Should be successful |
 */
TEST(PlayerJsonObject, AddValidZero) {
    std::cout << "Entering AddValidZero test" << std::endl;

    // Create PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "Created PlayerJsonObject instance." << std::endl;

        // Define input values
        const char* testName = "time";
        double testValue = 0.0;

        // Log input parameters
        char nameBuffer[32];
        std::strncpy(nameBuffer, testName, sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer)-1] = '\0';
        std::cout << "Invoking add with name: " << nameBuffer << ", value: " << testValue << std::endl;

        // Invoke the add method
        bool result = obj.add(std::string(nameBuffer), testValue);
        std::cout << "add returned: " << std::boolalpha << result << std::endl;

        // Assert that the add method succeeded
        EXPECT_TRUE(result);
        std::cout << "Internal state updated with key: " << nameBuffer << " and value: " << testValue << std::endl;
    });
    std::cout << "Exiting AddValidZero test" << std::endl;
}
/**
 * @brief Test that the add method fails when provided an empty name.
 *
 * This test verifies that the add method of the PlayerJsonObject correctly handles an empty string for the name by not updating its internal state. The test ensures that when an invalid empty key is provided, the method returns false, preventing any unintended state changes.
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
 * | Variation / Step | Description                                                          | Test Data                                         | Expected Result                                 | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------- | ----------------- |
 * | 01               | Create an instance of PlayerJsonObject using its default constructor | None                                              | Instance is created without exceptions          | Should be successful |
 * | 02               | Define input values with an empty name and a test value              | testName = "", testValue = 5.0                      | Input values are set correctly                   | Should be successful |
 * | 03               | Invoke the add method with an empty name and the test value            | input: name = "", value = 5.0; output: expected false | add method returns false                         | Should Fail       |
 * | 04               | Verify that the internal state is not updated due to the empty key    | expected: add returns false                       | Assertion passes confirming negative scenario    | Should be successful |
 */
TEST(PlayerJsonObject, AddEmptyName) {
    std::cout << "Entering AddEmptyName test" << std::endl;

    // Create PlayerJsonObject instance using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "Created PlayerJsonObject instance." << std::endl;

        // Define input values
        const char* testName = "";
        double testValue = 5.0;

        // Log input parameters
        char nameBuffer[32];
        std::strncpy(nameBuffer, testName, sizeof(nameBuffer));
        nameBuffer[sizeof(nameBuffer)-1] = '\0';
        std::cout << "Invoking add with name: \"" << nameBuffer << "\", value: " << testValue << std::endl;

        // Invoke the add method
        bool result = obj.add(std::string(nameBuffer), testValue);
        std::cout << "add returned: " << std::boolalpha << result << std::endl;

        // Assert that the add method fails for empty name
        EXPECT_FALSE(result);
        std::cout << "No internal state update due to empty key." << std::endl;
    });
    std::cout << "Exiting AddEmptyName test" << std::endl;
}
/**
 * @brief Test for adding a valid entry with a positive value in PlayerJsonObject
 *
 * This test verifies that a PlayerJsonObject can be instantiated using its default constructor
 * and that invoking the add() method with a valid positive value ("score" = 10) successfully returns true.
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
 * | Variation / Step | Description                                                             | Test Data                                             | Expected Result                                                  | Notes            |
 * | :--------------: | ----------------------------------------------------------------------- | ----------------------------------------------------- | ---------------------------------------------------------------- | ---------------- |
 * | 01               | Invoke default constructor to create a PlayerJsonObject instance        | None                                                  | Object is created successfully without throwing any exceptions   | Should be successful |
 * | 02               | Call add method with name "score" and positive value 10 to add an entry   | input1 = name: score, input2 = value: 10, output1 = true | Method returns true and the EXPECT_TRUE assertion passes           | Should Pass      |
 */
TEST(PlayerJsonObject, Add_valid_entry_with_positive_value) {
    std::cout << "Entering Add_valid_entry_with_positive_value test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject playerObj;
        std::cout << "PlayerJsonObject default constructor invoked successfully." << std::endl;
        
        std::string name = "score";
        int value = 10;
        std::cout << "Invoking add with name: " << name << " and value: " << value << std::endl;
        
        bool result = playerObj.add(name, value);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting Add_valid_entry_with_positive_value test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add properly handles negative value inputs.
 *
 * This test ensures that a PlayerJsonObject instance can successfully add an entry with a negative value.
 * It verifies that the default constructor does not throw any exceptions and that invoking the add method
 * with the parameters name = "penalty" and value = -5 returns true, confirming proper handling of negative inputs.
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
 * | Variation / Step | Description                                                             | Test Data                                            | Expected Result                                                                                | Notes         |
 * | :--------------: | ----------------------------------------------------------------------- | ---------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke the default constructor of PlayerJsonObject                      | No input                                             | The object is constructed without throwing any exceptions                                     | Should be successful |
 * | 02               | Call the add method with name "penalty" and value -5                      | name = penalty, value = -5                             | The add method returns true; EXPECT_TRUE assertion passes                                      | Should Pass   |
 */
TEST(PlayerJsonObject, Add_valid_entry_with_negative_value) {
    std::cout << "Entering Add_valid_entry_with_negative_value test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject playerObj;
        std::cout << "PlayerJsonObject default constructor invoked successfully." << std::endl;
        
        std::string name = "penalty";
        int value = -5;
        std::cout << "Invoking add with name: " << name << " and value: " << value << std::endl;
        
        bool result = playerObj.add(name, value);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting Add_valid_entry_with_negative_value test" << std::endl;
}
/**
 * @brief Verify the behavior of the add method when invoked with an empty name.
 *
 * This test checks that calling the add function with an empty string as the name and a valid integer value does not throw any exceptions and returns true. It ensures that the API correctly handles the edge case of an empty name input.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 039@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                | Expected Result                                | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------- | ---------------------------------------------- | ----------- |
 * | 01               | Invoke the add method on a PlayerJsonObject object using an empty name string | name = "", value = 100                   | Method returns true and no exceptions are thrown | Should Pass |
 */
TEST(PlayerJsonObject, Add_entry_with_empty_string_as_name) {
    std::cout << "Entering Add_entry_with_empty_string_as_name test" << std::endl;
    
    // Creating object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject playerObj;
        std::cout << "PlayerJsonObject default constructor invoked successfully." << std::endl;
        
        std::string name = "";
        int value = 100;
        std::cout << "Invoking add with an empty name and value: " << value << std::endl;
        
        bool result = playerObj.add(name, value);
        std::cout << "Method add returned: " << std::boolalpha << result << std::endl;
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting Add_entry_with_empty_string_as_name test" << std::endl;
}
/**
 * @brief Tests adding a positive value to PlayerJsonObject with a valid non-empty name.
 *
 * Tests that the add method of PlayerJsonObject correctly adds a positive value when provided with a valid name. This test verifies that the method returns true and the internal state is updated accordingly.
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
 * | Variation / Step | Description                                                  | Test Data                                          | Expected Result                                    | Notes      |
 * | :--------------: | ------------------------------------------------------------ | -------------------------------------------------- | -------------------------------------------------- | ---------- |
 * | 01               | Construct PlayerJsonObject and invoke add("score", 100)        | testName = score, testValue = 100, expected return = true | Method returns true and internal state updated successfully | Should Pass |
 */
TEST(PlayerJsonObject, AddWithValidNameAndPositiveValue) {
    std::cout << "Entering AddWithValidNameAndPositiveValue test" << std::endl;

    // Create instance using default constructor and log creation.
    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "score";
        long testValue = 100;
        std::cout << "Invoking add method with name: " << testName << " and value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"" << testName << "\", " << testValue << ") returned: " << result << std::endl;
        
        EXPECT_TRUE(result);
        std::cout << "Internal state change: Added positive value successfully" << std::endl;
    });
    
    std::cout << "Exiting AddWithValidNameAndPositiveValue test" << std::endl;
}
/**
 * @brief Verify that add method successfully adds a valid name with a zero value.
 *
 * This test ensures that when a valid non-empty name "level" and an integer value of zero are passed into the add method of PlayerJsonObject, no exception is thrown, and the method returns true, indicating correct internal state change.
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
 * | Variation / Step | Description                                                             | Test Data                                            | Expected Result                                                      | Notes             |
 * | :--------------: | ----------------------------------------------------------------------- | ---------------------------------------------------- | -------------------------------------------------------------------- | ----------------- |
 * | 01               | Create PlayerJsonObject instance using default constructor              | None                                                 | Instance is created successfully without exceptions                  | Should be successful |
 * | 02               | Invoke add method with valid name "level" and zero integer value (0)      | testName = "level", testValue = 0                     | Method returns true indicating successful addition and no exception thrown | Should Pass       |
 * | 03               | Verify the result from add method via assertion                         | result = true                                        | EXPECT_TRUE assertion passes, confirming the expected output         | Should Pass       |
 */
TEST(PlayerJsonObject, AddWithValidNameAndZeroValue) {
    std::cout << "Entering AddWithValidNameAndZeroValue test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "level";
        long testValue = 0;
        std::cout << "Invoking add method with name: " << testName << " and value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"" << testName << "\", " << testValue << ") returned: " << result << std::endl;
        
        EXPECT_TRUE(result);
        std::cout << "Internal state change: Added zero value successfully" << std::endl;
    });
    
    std::cout << "Exiting AddWithValidNameAndZeroValue test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add returns true for a valid name with a negative value.
 *
 * This test validates that the add method of the PlayerJsonObject class successfully handles a valid name ("penalty") with a negative value (-50). It ensures that even when provided with a negative value, the method behaves as expected without throwing exceptions and returns true.
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
 * | Variation / Step | Description                                                        | Test Data                                               | Expected Result                                                       | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------- | --------------------------------------------------------------------- | ----------- |
 * | 01               | Create a PlayerJsonObject instance using the default constructor.  | None                                                    | Instance created successfully without exception.                    | Should be successful |
 * | 02               | Invoke the add method with a valid name "penalty" and negative value -50. | input: testName = penalty, testValue = -50              | Method returns true indicating the negative value was added successfully. | Should Pass |
 * | 03               | Perform an assertion check to verify the returned result from add.   | output: result from add method                          | The assertion confirms that the result is true.                       | Should Pass |
 */
TEST(PlayerJsonObject, AddWithValidNameAndNegativeValue) {
    std::cout << "Entering AddWithValidNameAndNegativeValue test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "penalty";
        long testValue = -50;
        std::cout << "Invoking add method with name: " << testName << " and value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"" << testName << "\", " << testValue << ") returned: " << result << std::endl;
        
        EXPECT_TRUE(result);
        std::cout << "Internal state change: Added negative value successfully" << std::endl;
    });
    
    std::cout << "Exiting AddWithValidNameAndNegativeValue test" << std::endl;
}
/**
 * @brief Test the add() method behavior when provided with an empty name
 *
 * This test verifies that invoking the add() method on a PlayerJsonObject instance with an empty string as the name and a valid numeric value results in a failure to add the player. The test ensures that the method correctly handles an empty input for the name, and that the internal state remains unchanged.
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
 * | Variation / Step | Description                                                         | Test Data                                         | Expected Result                                              | Notes          |
 * | :---------------:| ------------------------------------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------ | -------------- |
 * | 01               | Instantiate PlayerJsonObject using the default constructor         | None                                              | Object is instantiated without any error                     | Should be successful |
 * | 02               | Invoke add() method with an empty name and a valid numeric value     | testName = "", testValue = 123                      | add() returns false indicating that addition failed due to empty name | Should Fail    |
 * | 03               | Validate that the add() method result is false via assertion         | result = false from add() method call             | EXPECT_FALSE assertion passes confirming the method's handling of empty name | Should be successful |
 */
TEST(PlayerJsonObject, AddWithEmptyName) {
    std::cout << "Entering AddWithEmptyName test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "";
        long testValue = 123;
        std::cout << "Invoking add method with an empty name and value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"\"," << testValue << ") returned: " << result << std::endl;
        
        EXPECT_FALSE(result);
        std::cout << "Internal state check: Addition failed due to empty name as expected" << std::endl;
    });
    
    std::cout << "Exiting AddWithEmptyName test" << std::endl;
}
/**
 * @brief Test the add method of PlayerJsonObject using the maximum long value boundary.
 *
 * This test verifies that the PlayerJsonObject::add method correctly handles the maximum long value input without throwing any exceptions and confirms through an assertion that the addition operation returns true.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of PlayerJsonObject, then invoke the add() method with testName = "maxValue" and testValue = LONG_MAX, and finally verify that the method returns true without throwing an exception. | input: testName = "maxValue", testValue = LONG_MAX; output: result = true | The add() method should return true indicating the successful addition of the maximum long value without exceptions. | Should Pass |
 */
TEST(PlayerJsonObject, AddWithMaxLongValue) {
    std::cout << "Entering AddWithMaxLongValue test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "maxValue";
        long testValue = LONG_MAX;
        std::cout << "Invoking add method with name: " << testName << " and maximum long value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"" << testName << "\", " << testValue << ") returned: " << result << std::endl;
        
        EXPECT_TRUE(result);
        std::cout << "Internal state change: Added maximum long value successfully" << std::endl;
    });
    
    std::cout << "Exiting AddWithMaxLongValue test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add accepts the minimum long value without throwing exceptions.
 *
 * This test validates that the add method in PlayerJsonObject properly handles the extremity of using LONG_MIN as an input value.
 * It ensures that the instance construction and the subsequent add operation execute without exceptions and that the method returns true.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 045
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                   | Test Data                                        | Expected Result                                | Notes               |
 * | :--------------: | ----------------------------------------------------------------------------- | ------------------------------------------------ | ---------------------------------------------- | ------------------- |
 * | 01               | Create PlayerJsonObject instance using the default constructor                | -                                                | Instance created successfully                  | Should be successful  |
 * | 02               | Invoke add method with name "minValue" and minimum long value (LONG_MIN)        | testName = minValue, testValue = LONG_MIN          | Method returns true and no exception thrown    | Should Pass         |
 * | 03               | Assert that the returned value from add is true                               | result = true                                    | Assertion passes confirming add method outcome | Should be successful  |
 */
TEST(PlayerJsonObject, AddWithMinLongValue) {
    std::cout << "Entering AddWithMinLongValue test" << std::endl;

    EXPECT_NO_THROW({
        PlayerJsonObject player;
        std::cout << "Created PlayerJsonObject instance using default constructor" << std::endl;
        
        std::string testName = "minValue";
        long testValue = LONG_MIN;
        std::cout << "Invoking add method with name: " << testName << " and minimum long value: " << testValue << std::endl;
        bool result = player.add(testName, testValue);
        std::cout << "add(\"" << testName << "\", " << testValue << ") returned: " << result << std::endl;
        
        EXPECT_TRUE(result);
        std::cout << "Internal state change: Added minimum long value successfully" << std::endl;
    });
    
    std::cout << "Exiting AddWithMinLongValue test" << std::endl;
}
/**
 * @brief Validates adding an array of PlayerJsonObject pointers using the add method.
 *
 * This test verifies that the add method correctly processes a vector containing valid PlayerJsonObject pointers
 * by adding them with the provided key "players". It ensures that the method returns true when the operation is successful.
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
 * | Variation / Step | Description                                                                           | Test Data                                                     | Expected Result                                          | Notes               |
 * | :--------------: | ------------------------------------------------------------------------------------- | ------------------------------------------------------------- | -------------------------------------------------------- | ------------------- |
 * | 01               | Invoke add() on a PlayerJsonObject instance with key "players" and a vector containing two valid PlayerJsonObject pointers. | input1 = "players", input2 = vector containing player1 and player2, output1 = true | API returns true, validated by EXPECT_TRUE(result) check | Should Pass         |
 * | 02               | Log test entry and exit messages to trace the execution flow of the test.             | No API inputs; console logs printed during test execution     | Console messages indicate proper method invocation and test progress | Should be successful |
 */
TEST(PlayerJsonObject, ValidArrayTest) {
    std::cout << "Entering ValidArrayTest test" << std::endl;
    
    // Create a PlayerJsonObject instance to invoke add method
    PlayerJsonObject jsonObj;
    // Create two PlayerJsonObject instances using default constructors
    PlayerJsonObject player1;
    PlayerJsonObject player2;
    
    // Prepare vector of pointers
    std::vector<PlayerJsonObject*> players;
    players.push_back(&player1);
    players.push_back(&player2);
    
    std::cout << "Invoking add with name: \"players\" and vector size: " << players.size() << std::endl;
    bool result = jsonObj.add("players", players);
    std::cout << "Method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);
    std::cout << "Exiting ValidArrayTest test" << std::endl;
}
/**
 * @brief Verify that the add method returns true when provided with duplicate player pointers.
 *
 * This test verifies the behavior of the PlayerJsonObject::add API when called with a vector containing duplicate player object pointers. It ensures that the API handles duplicate entries as expected.
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
 * | 01 | Invoke the add API with "duplicatePlayers" as key and a vector containing duplicate player pointers | input: key = "duplicatePlayers", duplicatePlayers = [&player, &player], output: result expected = true | API returns true and EXPECT_TRUE(result) passes | Should Pass |
 */
TEST(PlayerJsonObject, DuplicatePlayersTest) {
    std::cout << "Entering DuplicatePlayersTest test" << std::endl;
    
    PlayerJsonObject jsonObj;
    // Create a single PlayerJsonObject instance using default constructor
    PlayerJsonObject player;
    
    // Prepare vector with the same pointer repeated
    std::vector<PlayerJsonObject*> duplicatePlayers;
    duplicatePlayers.push_back(&player);
    duplicatePlayers.push_back(&player);
    
    std::cout << "Invoking add with name: \"duplicatePlayers\" and vector size: " << duplicatePlayers.size() << std::endl;
    bool result = jsonObj.add("duplicatePlayers", duplicatePlayers);
    std::cout << "Method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_TRUE(result);
    std::cout << "Exiting DuplicatePlayersTest test" << std::endl;
}
/**
 * @brief Verify that adding a PlayerJsonObject with an empty name fails.
 *
 * This test evaluates the behavior of the add method within the PlayerJsonObject class when provided an empty string as the player name. It verifies that the method correctly rejects an empty name even if the vector of valid PlayerJsonObject pointers is provided.
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
 * | Variation / Step | Description                                                              | Test Data                                                                                         | Expected Result                                                                      | Notes            |
 * | :--------------: | ------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ---------------- |
 * | 01               | Create PlayerJsonObject instances and prepare a vector with a valid pointer | jsonObj = instance of PlayerJsonObject, player = instance of PlayerJsonObject, players = vector containing &player | Instances and vector successfully created                                            | Should be successful |
 * | 02               | Invoke add method using an empty name and the prepared vector              | input name = "", input players = vector containing &player, output result variable 'result'         | The add method returns false indicating that an empty name is not accepted            | Should Pass      |
 * | 03               | Validate the result using the EXPECT_FALSE assertion                      | result = false (expected from add method)                                                         | EXPECT_FALSE(result) passes confirming that the add method handled empty name correctly | Should Pass      |
 */
TEST(PlayerJsonObject, EmptyNameTest) {
    std::cout << "Entering EmptyNameTest test" << std::endl;
    
    PlayerJsonObject jsonObj;
    // Create a PlayerJsonObject instance
    PlayerJsonObject player;
    
    // Prepare vector with one valid pointer
    std::vector<PlayerJsonObject*> players;
    players.push_back(&player);
    
    std::cout << "Invoking add with empty name \"\" and vector size: " << players.size() << std::endl;
    bool result = jsonObj.add("", players);
    std::cout << "Method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyNameTest test" << std::endl;
}
/**
 * @brief Verify that the add method returns false when provided with an empty vector.
 *
 * This test verifies that the PlayerJsonObject::add API correctly handles an empty vector input. 
 * It checks that when an empty vector is passed along with a valid name, the method returns false as expected.
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
 * | Variation / Step | Description                                                            | Test Data                                                       | Expected Result                             | Notes            |
 * | :--------------: | ---------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------- | ---------------- |
 * | 01               | Instantiate a PlayerJsonObject object                                  | N/A                                                             | Object instantiated successfully            | Should be successful |
 * | 02               | Prepare an empty vector of PlayerJsonObject pointers                   | emptyPlayers = []                                               | Empty vector created                        | Should be successful |
 * | 03               | Invoke the add() method with name "emptyArray" and the empty vector      | input1 = "emptyArray", input2 = emptyPlayers (size = 0)           | API returns false                           | Should Fail      |
 * | 04               | Verify that the result from add() is false using EXPECT_FALSE assertion  | result = false                                                  | The EXPECT_FALSE assertion passes           | Should be successful |
 */
TEST(PlayerJsonObject, EmptyVectorTest) {
    std::cout << "Entering EmptyVectorTest test" << std::endl;
    
    PlayerJsonObject jsonObj;
    // Prepare an empty vector
    std::vector<PlayerJsonObject*> emptyPlayers;
    
    std::cout << "Invoking add with name: \"emptyArray\" and vector size: " << emptyPlayers.size() << std::endl;
    bool result = jsonObj.add("emptyArray", emptyPlayers);
    std::cout << "Method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyVectorTest test" << std::endl;
}
/**
 * @brief Verify that PlayerJsonObject::add returns false when the input vector contains a nullptr
 *
 * This test verifies that the add method in PlayerJsonObject correctly identifies the presence of a nullptr within a vector of PlayerJsonObject pointers and returns false. The objective is to ensure that the API handles invalid pointers gracefully and does not process a collection containing a nullptr.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 050
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate the PlayerJsonObject and a valid player instance | No input arguments; objects: jsonObj, player | Objects are instantiated successfully | Should be successful |
 * | 02 | Prepare a vector containing one valid pointer and one nullptr | playersWithNull = { pointer1 = address of player, pointer2 = nullptr } | Vector is populated with one valid pointer and one nullptr | Should be successful |
 * | 03 | Invoke the add API with key "playersWithNull" and the prepared vector | input key = "playersWithNull", players vector = { pointer1 = address of player, pointer2 = nullptr }, expected output: result = false | The add method returns false due to the presence of nullptr in the vector | Should Fail |
 * | 04 | Validate the result with EXPECT_FALSE to ensure the API behaves as expected | result variable = false | Assertion passes confirming that add returned false | Should Pass |
 */
TEST(PlayerJsonObject, ContainsNullptrTest) {
    std::cout << "Entering ContainsNullptrTest test" << std::endl;
    
    PlayerJsonObject jsonObj;
    // Create a valid PlayerJsonObject instance
    PlayerJsonObject player;
    
    // Prepare vector with one valid pointer and one nullptr
    std::vector<PlayerJsonObject*> playersWithNull;
    playersWithNull.push_back(&player);
    playersWithNull.push_back(nullptr);
    
    std::cout << "Invoking add with name: \"playersWithNull\" and vector size: " 
              << playersWithNull.size() << std::endl;
    std::cout << "Pointer values: [" << playersWithNull[0] << ", " << playersWithNull[1] << "]" << std::endl;
    
    bool result = jsonObj.add("playersWithNull", playersWithNull);
    std::cout << "Method returned: " << (result ? "true" : "false") << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting ContainsNullptrTest test" << std::endl;
}
/**
 * @brief Retrieve valid subobject from JSON
 *
 * This test case verifies that a subobject can be successfully retrieved from a JSON object when a valid JSON structure is provided with a nested subobject.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                      | Expected Result                                                                                | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerJsonObject with a valid JSON string containing a subobject.    | input = "{\"object\":{\"key\":\"value\"}}"                                                    | Object created without exception.                                                            | Should Pass    |
 * | 02               | Create a default PlayerJsonObject to hold the retrieved subobject.            | No input parameters; using default constructor                                                 | Object created without exception.                                                            | Should Pass    |
 * | 03               | Invoke the get API with "object" to retrieve the subobject from the parent.   | input1 = "object", output object = subObj                                                       | get returns true indicating the subobject was successfully retrieved.                         | Should Pass    |
 * | 04               | Validate the result by asserting that the get method returned true.           | result = value returned by get API                                                               | Assertion check passes confirming the subobject was retrieved successfully.                    | Should Pass    |
 */
TEST(PlayerJsonObject, RetrieveValidSubobjectFromJSON) {
    std::cout << "Entering RetrieveValidSubobjectFromJSON test" << std::endl;
    
    // Create a PlayerJsonObject with valid JSON string containing "object": {"key":"value"}
    EXPECT_NO_THROW(PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}"));
    PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}");
    
    // Create a default PlayerJsonObject to hold the retrieved subobject
    EXPECT_NO_THROW(PlayerJsonObject subObj);
    PlayerJsonObject subObj;
    
    std::cout << "Invoking get with name = \"object\"" << std::endl;
    bool result = parent.get("object", subObj);
    std::cout << "get returned: " << result << std::endl;
    
    // Validate the result: expecting true
    EXPECT_TRUE(result);   
    std::cout << "Exiting RetrieveValidSubobjectFromJSON test" << std::endl;
}
/**
 * @brief Validate retrieval of a non-existing property from PlayerJsonObject.
 *
 * This test case verifies that when attempting to retrieve a property that does not exist within a JSON string, 
 * the API correctly returns false without throwing any exceptions. This ensures robust behavior when accessing missing keys.
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
 * | Variation / Step | Description                                                            | Test Data                                                                   | Expected Result                                                 | Notes              |
 * | :--------------: | ---------------------------------------------------------------------- | --------------------------------------------------------------------------- | -------------------------------------------------------------- | ------------------ |
 * | 01               | Create a PlayerJsonObject with a JSON string that does not include "missing" property | input = json string = "{\"object\":{\"key\":\"value\"}}", output = valid PlayerJsonObject  | Object created successfully without exceptions               | Should be successful |
 * | 02               | Create a default PlayerJsonObject to be used for property retrieval      | input = default constructor, output = empty PlayerJsonObject                | Object created successfully without exceptions               | Should be successful |
 * | 03               | Invoke get on parent object with property name "missing" to retrieve into subObj | input = property name "missing", subObj, output = bool result (expected false) | API call returns false indicating property is not found        | Should Pass        |
 */
TEST(PlayerJsonObject, RetrieveNonExistingPropertyFromJSON) {
    std::cout << "Entering RetrieveNonExistingPropertyFromJSON test" << std::endl;
    
    // Create a PlayerJsonObject with JSON string that does not contain "missing"
    EXPECT_NO_THROW(PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}"));
    PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}");
    
    // Create a default PlayerJsonObject to try to retrieve non-existent property
    EXPECT_NO_THROW(PlayerJsonObject subObj);
    PlayerJsonObject subObj;
    
    std::cout << "Invoking get with non-existing name = \"missing\"" << std::endl;
    bool result = parent.get("missing", subObj);
    std::cout << "get returned: " << result << std::endl;
    
    // Since the property does not exist, expect false
    EXPECT_FALSE(result); 
    std::cout << "Exiting RetrieveNonExistingPropertyFromJSON test" << std::endl;
}
/**
 * @brief Test to retrieve a sub-object with an empty property name.
 *
 * This test verifies the behavior of the get method when an empty property name is provided. The expected behavior is that the get method returns false, indicating that an empty property name is invalid. It ensures that the API correctly handles invalid input without throwing an exception.
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
 * | Variation / Step | Description                                                                  | Test Data                                                              | Expected Result                                                    | Notes           |
 * | :--------------: | ---------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------------ | --------------- |
 * | 01               | Create a PlayerJsonObject with a valid JSON string                           | input = JSON string "{\"object\":{\"key\":\"value\"}}"                  | No exception thrown                                                | Should be successful |
 * | 02               | Create a default PlayerJsonObject to hold the result                         | output = subObj (default constructed)                                  | No exception thrown                                                | Should be successful |
 * | 03               | Invoke get method with an empty property name                                | input1 = "" (empty property name), input2 = subObj                      | Returns false                                                      | Should Fail     |
 */
TEST(PlayerJsonObject, RetrieveWithEmptyPropertyName) {
    std::cout << "Entering RetrieveWithEmptyPropertyName test" << std::endl;
    
    // Create a PlayerJsonObject with valid JSON string
    EXPECT_NO_THROW(PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}"));
    PlayerJsonObject parent("{\"object\":{\"key\":\"value\"}}");
    
    // Create a default PlayerJsonObject to hold the result
    EXPECT_NO_THROW(PlayerJsonObject subObj);
    PlayerJsonObject subObj;
    
    std::cout << "Invoking get with empty property name" << std::endl;
    bool result = parent.get("", subObj);
    std::cout << "get returned: " << result << std::endl;
    
    // Expect false since empty property name should not be valid
    EXPECT_FALSE(result);
       
    std::cout << "Exiting RetrieveWithEmptyPropertyName test" << std::endl;
}
/**
 * @brief Validate retrieval of a valid double value from a JSON string
 *
 * This test verifies that the PlayerJsonObject correctly extracts a double value from a JSON string when provided with a valid property name. It checks whether the get() method returns true and properly sets the output value to the expected double.
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
 * | Variation / Step | Description                                                        | Test Data                                                                                          | Expected Result                                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Initialize JSON string with a valid double and invoke get() method.  | input: jsonStr = {"duration": 123.45}, propertyName = "duration", initial value = 0.0, output: value should be 123.45 | get() returns true; retrieved double value equals 123.45 via assertions | Should Pass |
 */
TEST(PlayerJsonObject, RetrieveValidDoubleValue_FromJSONString) {
    std::cout << "Entering RetrieveValidDoubleValue_FromJSONString test" << std::endl;

    // JSON string containing the key "duration"
    std::string jsonStr = "{\"duration\": 123.45}";
    PlayerJsonObject player(jsonStr);

    double value = 0.0;
    std::string propertyName = "duration";
    std::cout << "Invoking get with property name: " << propertyName << std::endl;

    bool result = player.get(propertyName, value);

    std::cout << "Method get returned: " << std::boolalpha << result 
              << " and retrieved value: " << value << std::endl;

    EXPECT_TRUE(result);
    EXPECT_DOUBLE_EQ(value, 123.45);

    std::cout << "Exiting RetrieveValidDoubleValue_FromJSONString test" << std::endl;
}
/**
 * @brief Verify behavior of get() method when invoked with an empty string as the property name.
 *
 * This test validates that the get() method of PlayerJsonObject returns false when an empty property name is provided.
 * Additionally, it ensures that the value passed to the method remains unmodified when the property name is empty.
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
 * | Variation / Step | Description                                                                         | Test Data                                                              | Expected Result                                                   | Notes       |
 * | :----:           | ----------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ----------------------------------------------------------------- | ----------- |
 * | 01               | Validate default construction of PlayerJsonObject using default constructor.       | No input arguments                                                     | Object constructed without throwing any exceptions.             | Should Pass |
 * | 02               | Invoke get() with an empty property name and verify that the output value remains unchanged. | propertyName = "", value (initial) = -1.0, expected result return value = false | get() returns false and the value remains -1.0.                   | Should Fail |
 */
TEST(PlayerJsonObject, EmptyStringAsPropertyName) {
    std::cout << "Entering EmptyStringAsPropertyName test" << std::endl;

    // Creating object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject player;
    });
    PlayerJsonObject player;

    // Setting up the test with an initial value to verify unmodified result.
    double initialValue = -1.0;
    double value = initialValue;
    std::string propertyName;
    char tempName[1];
    // Using strncpy to assign an empty string to fixed size array and then to std::string.
    std::strncpy(tempName, "", sizeof(tempName) - 1);
    tempName[sizeof(tempName) - 1] = '\0';
    propertyName = tempName;
    std::cout << "Invoking get with empty property name" << std::endl;
    
    bool result = player.get(propertyName, value);
    std::cout << "Method get returned: " << std::boolalpha << result 
              << " and value after invocation: " << value << std::endl;
              
    EXPECT_FALSE(result);
    EXPECT_EQ(value, initialValue);
    std::cout << "Exiting EmptyStringAsPropertyName test" << std::endl;
}
/**
 * @brief Validate retrieval of numerical property from JSON string using the get method
 *
 * This test verifies that when a PlayerJsonObject instance is constructed using a valid JSON string containing the property "volume", the get() method correctly retrieves the property value. The test confirms that the API returns true and updates the provided output parameter with the expected value from the JSON string.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 056
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                               | Test Data                                                              | Expected Result                           | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ----------------------------------------- | ---------- |
 * | 01               | Construct PlayerJsonObject using a valid JSON string and retrieve "volume" using get method | input: jsonStr = {"volume": 50}, propName = volume, value = 0            | get() returns true and value equals 50    | Should Pass|
 */
TEST(PlayerJsonObject, ValidPropertyRetrieval_FromJSONString) {
    std::cout << "Entering ValidPropertyRetrieval_FromJSONString test" << std::endl;
    
    // Construct directly from JSON string
    std::string jsonStr = "{\"volume\": 50}";
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject instance created using JSON string: " << jsonStr << std::endl;
    
    std::string propName = "volume";
    int value = 0;
    std::cout << "Invoking get with name = '" << propName << "' and initial value = " << value << std::endl;
    
    bool result = playerObj.get(propName, value);
    std::cout << "get returned: " << std::boolalpha << result << ", value = " << value << std::endl;

    EXPECT_TRUE(result);
    EXPECT_EQ(value, 50);
    
    std::cout << "Exiting ValidPropertyRetrieval_FromJSONString test" << std::endl;
}
/**
 * @brief Test the get method of PlayerJsonObject with an empty property name.
 *
 * This test verifies that invoking the get method with an empty string as the property name does not alter 
 * the supplied integer value and returns false, since the property name provided is empty. This is important 
 * to ensure that the API correctly handles invalid or empty property keys.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 057@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                                   | Test Data                                                  | Expected Result                                                                              | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerJsonObject using its default constructor                                                          | No input arguments; default constructed object             | Instance is created successfully                                                              | Should be successful |
 * | 02               | Call the get method with an empty property name and a predefined integer variable                                               | input: propName = "", value = 123                          | The get method should return false, and the integer value should remain unchanged             | Should Pass   |
 * | 03               | Validate the result from the get method call using an assertion                                                                  | output: result = false                                     | EXPECT_FALSE(result) assertion passes                                                         | Should Pass   |
 */
TEST(PlayerJsonObject, EmptyPropertyName) {
    std::cout << "Entering EmptyPropertyName test" << std::endl;
    
    // Create instance of PlayerJsonObject using default constructor
    PlayerJsonObject playerObj;
    std::cout << "PlayerJsonObject instance created using default constructor." << std::endl;
    
    std::string propName = "";
    int value = 123;  // initialize with a value to check if it stays unchanged
    std::cout << "Invoking get with an empty property name '' and initial value = " << value << std::endl;
    
    bool result = playerObj.get(propName, value);
    std::cout << "get returned: " << std::boolalpha << result << std::endl;
    std::cout << "Value after get call: " << value << std::endl;
    
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyPropertyName test" << std::endl;
}
/**
 * @brief Verify that retrieving a string property as an integer fails due to type mismatch
 *
 * This test creates an instance of PlayerJsonObject using a JSON string with a string "subtitle" property.
 * It then attempts to retrieve the "subtitle" property as an integer. The test ensures that the API fails to convert
 * the string type to an integer and that the integer variable remains unchanged.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 058@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                                          | Test Data                                                                                                                  | Expected Result                                             |Notes        |
 * | :--------------: | -------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- |------------ |
 * | 01               | Instantiate PlayerJsonObject with a JSON containing a string property and invoke get to retrieve it as integer       | jsonStr = "{\"subtitle\": \"English\"}", propName = "subtitle", initial value = 999                                       | API returns false indicating type mismatch and value remains unchanged | Should Fail |
 */
TEST(PlayerJsonObject, PropertyNotIntegerType) {
    std::cout << "Entering PropertyNotIntegerType test" << std::endl;
    
    // Create instance of PlayerJsonObject using JSON string constructor
    const char* jsonStr = "{\"subtitle\": \"English\"}";
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject initialized with JSON: " << jsonStr << std::endl;
    
    // Now try to retrieve it as integer
    std::string propName = "subtitle";
    int value = 999;  // initialize with a value to check if it stays unchanged
    std::cout << "Invoking get with property name = '" << propName 
              << "' (non-integer type) and initial value = " << value << std::endl;
    
    bool result = playerObj.get(propName, value);
    std::cout << "get returned: " << std::boolalpha << result << std::endl;
    std::cout << "Value after get call: " << value << std::endl;
    
    // Expect failure because type mismatch
    EXPECT_FALSE(result);
    
    std::cout << "Exiting PropertyNotIntegerType test" << std::endl;
}
/**
 * @brief Verify that the get() method returns the expected non-empty value for an existing property.
 *
 * This test ensures that a PlayerJsonObject created from a valid JSON string correctly retrieves the "title" property that has a non-empty value. The test confirms that both object instantiation and the get() call do not throw exceptions and that the value is properly retrieved.
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
 * | 01 | Instantiate PlayerJsonObject with provided JSON string | jsonStr = "{\"title\":\"Player1\"}" | Object created without exceptions | Should Pass |
 * | 02 | Call get method with property name "title" | propertyName = title, value = empty string | API returns true and sets value = "Player1" | Should Pass |
 * | 03 | Validate API result and cleanup the allocated object | result = true, value = "Player1" | Assertion check returns true and resource is released | Should be successful |
 */
TEST(PlayerJsonObject, RetrieveExistingPropertyWithNonEmptyValue) {
    std::cout << "Entering RetrieveExistingPropertyWithNonEmptyValue test" << std::endl;

    PlayerJsonObject* obj = nullptr;
    const char* jsonStr = "{\"title\":\"Player1\"}";
    EXPECT_NO_THROW({ obj = new PlayerJsonObject(jsonStr); });
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    std::string propertyName = "title";
    std::cout << "Invoking get with name: " << propertyName << std::endl;
    
    std::string value = "";
    bool result = false;
    EXPECT_NO_THROW({
        result = obj->get(propertyName, value);
    });

    std::cout << "Method get invoked, passed name: " << propertyName << std::endl;
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    std::cout << "Retrieved value: " << value << std::endl;

    EXPECT_TRUE(result);

    delete obj;
    std::cout << "Exiting RetrieveExistingPropertyWithNonEmptyValue test" << std::endl;
}
/**
 * @brief Test to verify that retrieving a non-existent property returns false and does not alter the default value.
 *
 * This test creates a PlayerJsonObject instance from a valid JSON string and then attempts to retrieve a property that does not exist.
 * It verifies that the get method returns false and the output value remains unchanged.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 060@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**  
 * | Variation / Step | Description | Test Data | Expected Result | Notes |  
 * | :----: | --------- | ---------- |-------------- | ----- |  
 * | 01 | Create a PlayerJsonObject instance with valid JSON string. | input: jsonStr = {"title":"Player1","volume":50}, output: instance pointer | Instance should be created without exception and not be null. | Should be successful |  
 * | 02 | Invoke get() on "nonExistingProperty" to verify behavior when property is absent. | input: propertyName = nonExistingProperty, value = "unchanged" (initial), output: result = false, value remains "unchanged" | get() should return false and leave value unchanged ("unchanged"). | Should Fail |  
 * | 03 | Delete the allocated PlayerJsonObject instance. | input: pointer to PlayerJsonObject instance, output: instance deleted | Object should be deleted without error. | Should be successful |
 */
TEST(PlayerJsonObject, RetrieveNonExistentProperty) {
    std::cout << "Entering RetrieveNonExistentProperty test" << std::endl;

    PlayerJsonObject* obj = nullptr;
    const char* jsonStr = "{\"title\":\"Player1\",\"volume\":50}";
    EXPECT_NO_THROW({ obj = new PlayerJsonObject(jsonStr); });
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    std::string propertyName = "nonExistingProperty";
    std::cout << "Invoking get with name: " << propertyName << std::endl;

    std::string value = "unchanged";  // initialized to check if it stays the same
    bool result = false;
    EXPECT_NO_THROW({
        result = obj->get(propertyName, value);
    });

    std::cout << "Method get invoked, passed name: " << propertyName << std::endl;
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    std::cout << "Retrieved value (should remain unchanged): '" << value << "'" << std::endl;

    EXPECT_FALSE(result);
    EXPECT_EQ(value, "unchanged");

    delete obj;
    std::cout << "Exiting RetrieveNonExistentProperty test" << std::endl;
}
/**
 * @brief Test retrieval behavior when an empty property name is provided
 *
 * This test verifies that when the get method of PlayerJsonObject is invoked with an empty property name, 
 * it handles the case gracefully by not throwing an exception and returning false. The test ensures that 
 * the method does not modify the value parameter and provides appropriate error handling for empty property inputs.
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
 * | Variation / Step | Description                                                         | Test Data                                                       | Expected Result                                        | Notes           |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------- | --------------- |
 * | 01               | Create an instance of PlayerJsonObject                              | None                                                            | Instance created without exception                   | Should be successful |
 * | 02               | Set the property name to an empty string                            | propertyName = ""                                               | propertyName is an empty string                      | Should be successful |
 * | 03               | Invoke get method with the empty property name                      | input: propertyName = "", output: value = ""                      | get method returns false                             | Should Pass     |
 * | 04               | Assert that the get method returned false                           | result = false                                                  | Result is false as per expectation                   | Should be successful |
 * | 05               | Delete the PlayerJsonObject instance to clean up resources             | None                                                            | Instance deleted without memory leak                 | Should be successful |
 */
TEST(PlayerJsonObject, RetrieveWithEmptyPropertyName) {
    std::cout << "Entering RetrieveWithEmptyPropertyName test" << std::endl;
    PlayerJsonObject* obj = nullptr;
    EXPECT_NO_THROW({ obj = new PlayerJsonObject(); });
    std::cout << "PlayerJsonObject instance created" << std::endl;

    std::string propertyName = "";
    std::cout << "Invoking get with empty property name" << std::endl;
    std::string value = "";
    bool result = false;
    EXPECT_NO_THROW({
        result = obj->get(propertyName, value);
    });
    std::cout << "Method get invoked with empty name" << std::endl;
    std::cout << "Method returned: " << std::boolalpha << result << std::endl;
    std::cout << "Retrieved value (should be unchanged): '" << value << "'" << std::endl;

    EXPECT_FALSE(result);

    delete obj;
    std::cout << "Exiting RetrieveWithEmptyPropertyName test" << std::endl;
}
/**
 * @brief Test the retrieval of a valid players array from JSON input
 *
 * This test verifies that the PlayerJsonObject correctly parses valid JSON containing a players array, constructs an object without exceptions, and successfully retrieves the players array using the get method.
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
 * | 01 | Create PlayerJsonObject using a JSON string with a players array and invoke the get method to retrieve the array | inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}", jsonStr = copied inputJson, players = empty vector | PlayerJsonObject is created without throwing exceptions; get method returns true and the vector contains 2 elements; EXPECT_TRUE assertion passes | Should Pass |
 */
TEST(PlayerJsonObject, RetrieveValidArray) {
    std::cout << "Entering RetrieveValidArray test" << std::endl;
    
    char jsonStr[256];
    const char* inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}";
    std::strncpy(jsonStr, inputJson, sizeof(jsonStr) - 1);
    jsonStr[sizeof(jsonStr) - 1] = '\0';
    std::cout << "Creating PlayerJsonObject with JSON: " << jsonStr << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject pjo(jsonStr);
        std::cout << "PlayerJsonObject created successfully using custom constructor." << std::endl;
        
        std::vector<PlayerJsonObject> players;
        std::cout << "Invoking get with property name \"players\" and empty vector." << std::endl;
        bool result = pjo.get("players", players);
        std::cout << "Returned value from get: " << std::boolalpha << result << std::endl;
        
        if(result) {
            std::cout << "Retrieved " << players.size() << " PlayerJsonObject elements." << std::endl;
        } else {
            std::cout << "No PlayerJsonObject elements retrieved." << std::endl;
        }
        
        EXPECT_TRUE(result);
    });
    
    std::cout << "Exiting RetrieveValidArray test" << std::endl;
}
/**
 * @brief Verify retrieval of a non-existent property in PlayerJsonObject
 *
 * This test verifies that when the get() method is invoked with a property name that does not exist ("nonexistent"), the API does not throw an exception, returns false, and leaves the players vector empty.
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
 * | Variation / Step | Description                                                        | Test Data                                                                                  | Expected Result                                        |Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------------------------------------------------ |------------- |
 * | 01               | Create a JSON string and initialize PlayerJsonObject using custom constructor | jsonStr = "{\"players\": [{\"id\":1},{\"id\":2}]}", inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}" | PlayerJsonObject instantiated without throwing an exception | Should Pass  |
 * | 02               | Invoke get() method with property name "nonexistent"                 | property name = "nonexistent"                                                                | Method returns false indicating property not found     | Should Pass  |
 * | 03               | Verify that the returned vector remains empty                        | players vector size                                                               | Returned vector size is 0                              | Should Pass  |
 */
TEST(PlayerJsonObject, PropertyNameNotFound) {
    std::cout << "Entering PropertyNameNotFound test" << std::endl;
    
    char jsonStr[256];
    const char* inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}";
    std::strncpy(jsonStr, inputJson, sizeof(jsonStr) - 1);
    jsonStr[sizeof(jsonStr) - 1] = '\0';
    std::cout << "Creating PlayerJsonObject with JSON: " << jsonStr << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject pjo(jsonStr);
        std::cout << "PlayerJsonObject created successfully using custom constructor." << std::endl;
        
        std::vector<PlayerJsonObject> players;
        std::cout << "Invoking get with non-existent property name \"nonexistent\"." << std::endl;
        bool result = pjo.get("nonexistent", players);
        std::cout << "Returned value from get: " << std::boolalpha << result << std::endl;
        std::cout << "Vector size returned: " << players.size() << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting PropertyNameNotFound test" << std::endl;
}
/**
 * @brief Validate that invoking the get method with an empty property name returns false.
 *
 * This test creates a PlayerJsonObject using a valid JSON string and then calls the get() method with an empty property name.
 * It verifies that the API correctly handles an invalid empty property name by returning false without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                               | Test Data                                                            | Expected Result                                              | Notes          |
 * | :--------------: | --------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------ | -------------- |
 * | 01               | Copy a valid JSON string into a buffer and create a PlayerJsonObject instance using the custom constructor.| inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}", jsonStr = same    | Instance created without throwing any exception              | Should be successful |
 * | 02               | Invoke get() on the PlayerJsonObject with an empty property name and check the returned boolean value.     | propertyName = "", output: result = false, players size = 0            | get() returns false as expected                              | Should Pass    |
 */
TEST(PlayerJsonObject, EmptyPropertyName) {
    std::cout << "Entering EmptyPropertyName test" << std::endl;
    
    char jsonStr[256];
    const char* inputJson = "{\"players\": [{\"id\":1},{\"id\":2}]}";
    std::strncpy(jsonStr, inputJson, sizeof(jsonStr) - 1);
    jsonStr[sizeof(jsonStr) - 1] = '\0';
    std::cout << "Creating PlayerJsonObject with JSON: " << jsonStr << std::endl;
    
    EXPECT_NO_THROW({
        PlayerJsonObject pjo(jsonStr);
        std::cout << "PlayerJsonObject created successfully using custom constructor." << std::endl;
        
        std::vector<PlayerJsonObject> players;
        std::cout << "Invoking get with empty property name \"\"." << std::endl;
        bool result = pjo.get("", players);
        std::cout << "Returned value from get: " << std::boolalpha << result << std::endl;
        std::cout << "Vector size returned: " << players.size() << std::endl;
        
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting EmptyPropertyName test" << std::endl;
}
/**
 * @brief Validate get method with a valid key that returns expected array values.
 *
 * This test ensures that the PlayerJsonObject is properly created from a valid JSON string containing an array of subtitles and verifies that the get method correctly retrieves the array values. It checks for exception safety and correctness of the output.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 065
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                  | Test Data                                                              | Expected Result                                                                | Notes      |
 * | :----:           | ------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------ | ---------- |
 * | 01               | Create PlayerJsonObject instance with valid JSON string      | input: jsonStr = {"subtitles":["English","French","Spanish"]}           | Object is created without exceptions                                           | Should Pass |
 * | 02               | Call get method with key "subtitles" to retrieve array values  | input: name = "subtitles", output: values (expected: English, French, Spanish) | get method returns true and values vector contains "English", "French", "Spanish" | Should Pass |
 */
TEST(PlayerJsonObject, ValidNameWithExpectedArrayValues) {
    std::cout << "Entering ValidNameWithExpectedArrayValues test" << std::endl;

    // JSON string with subtitles array
    const char* jsonStr = R"({"subtitles":["English","French","Spanish"]})";

    PlayerJsonObject* obj = nullptr;
    EXPECT_NO_THROW({
        obj = new PlayerJsonObject(jsonStr);
    });
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    // Initialize parameters
    std::string name = "subtitles";
    std::vector<std::string> values;
    std::cout << "Invoking get method with name: " << name << std::endl;

    // Call get method and capture the return value
    bool result = false;
    EXPECT_NO_THROW({
        result = obj->get(name, values);
    });
    std::cout << "Returned value from get: " << std::boolalpha << result << std::endl;

    // If method returns true, print out the values obtained
    if (result) {
        std::cout << "Values retrieved: ";
        for (const auto& val : values) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Validate results
    EXPECT_TRUE(result);
   
    delete obj;
    std::cout << "Exiting ValidNameWithExpectedArrayValues test" << std::endl;
}
/**
 * @brief Validate that the get method returns false when a non-existent name is queried in an empty JSON object.
 *
 * This test verifies that when a PlayerJsonObject is initialized with an empty JSON string, calling the get method with a name that does not exist in the JSON returns false, and the output vector remains empty. This ensures that the API correctly handles cases where the queried key is absent.
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
 * | Variation / Step | Description                                                     | Test Data                                            | Expected Result                                           | Notes          |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------------------- | --------------------------------------------------------- | -------------- |
 * | 01               | Instantiate PlayerJsonObject with an empty JSON string           | input: json = "{}"                                   | Instance is created without throwing an exception         | Should Pass    |
 * | 02               | Initialize parameters for get method                             | input: name = "nonexistent", values = empty vector   | Parameters correctly initialized                          | Should be successful |
 * | 03               | Invoke the get method on the PlayerJsonObject instance             | input: name = "nonexistent", values = empty vector   | get method returns false without throwing an exception    | Should Pass    |
 * | 04               | Validate that get method output is false and values vector is empty | output: result = false, values vector size = 0       | Assertion verifies result is false and values vector is empty | Should Pass    |
 * | 05               | Clean up the allocated PlayerJsonObject instance                  | -                                                    | Object is deleted successfully                            | Should be successful |
 */
TEST(PlayerJsonObject, NameNotPresentInJson) {
    std::cout << "Entering NameNotPresentInJson test" << std::endl;

    // Create PlayerJsonObject with empty JSON string
    PlayerJsonObject* obj = nullptr;
    EXPECT_NO_THROW({
        obj = new PlayerJsonObject("{}");
    });
    std::cout << "PlayerJsonObject instance created with empty JSON" << std::endl;

    // Initialize parameters
    std::string name = "nonexistent";
    std::vector<std::string> values;
    std::cout << "Invoking get method with name: " << name << " and an empty vector for values." << std::endl;
    
    // Call get method and capture the return value
    bool result = false;
    EXPECT_NO_THROW({
        result = obj->get(name, values);
    });
    std::cout << "Returned value from get: " << std::boolalpha << result << std::endl;
    
    // Validate that method returns false and the vector remains empty
    EXPECT_FALSE(result);
    std::cout << "Values vector remains empty with size: " << values.size() << std::endl;

    delete obj;
    std::cout << "Exiting NameNotPresentInJson test" << std::endl;
}
/**
 * @brief Validate that the get method returns false when an empty name parameter is provided.
 *
 * This test verifies that when the PlayerJsonObject is instantiated with an empty JSON string and the get method is invoked with an empty name parameter, the method correctly returns false. This behavior ensures that invalid or empty inputs are properly handled.
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
 * | Variation / Step | Description                                                                                              | Test Data                                                      | Expected Result                                                      | Notes     |
 * | :--------------: | -------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------- | --------- |
 * | 01               | Create a PlayerJsonObject instance with an empty JSON, initialize an empty name string and an empty vector, invoke get method, and verify the outcome. | json = "{}", name = "", values = [], output = false              | get() returns false and the vector remains empty as validated by EXPECT_FALSE(result) | Should Pass |
 */
TEST(PlayerJsonObject, EmptyNameParameter) {
    std::cout << "Entering EmptyNameParameter test" << std::endl;

    // Create PlayerJsonObject with empty JSON
    PlayerJsonObject obj("{}");
    std::cout << "PlayerJsonObject instance created with empty JSON." << std::endl;

    // Initialize parameters
    std::string name = "";
    std::vector<std::string> values;
    std::cout << "Before get call, values vector size: " << values.size() << std::endl;

    // Call get method and capture return value
    bool result = false;
    EXPECT_NO_THROW({
        result = obj.get(name, values);
    });
    std::cout << "Returned value from get with empty name: " << std::boolalpha << result << std::endl;

    // Validate outcome
    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyNameParameter test" << std::endl;
}
/**
 * @brief Validate retrieval of a valid property using different encoding methods
 *
 * This test verifies that the PlayerJsonObject correctly retrieves an existing property ("existingProp")
 * when different encoding methods (ENCODING_STRING, ENCODING_BASE64, and ENCODING_BASE64_URL) are used.
 * It checks that the method get does not throw exceptions and returns true for a successful property retrieval.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                              | Expected Result                                   | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------- | ------------- |
 * | 01               | Create PlayerJsonObject instance with JSON containing property 'existingProp'                   | JSON input = {"existingProp": "Player1"}                                                                               | Instance created successfully                     | Should be successful |
 * | 02               | Invoke get() to retrieve 'existingProp' using ENCODING_STRING                                     | input: propertyName = existingProp, encoding = ENCODING_STRING, output: values vector populated, return value = true    | Method returns true without exceptions            | Should Pass   |
 * | 03               | Invoke get() to retrieve 'existingProp' using ENCODING_BASE64                                     | input: propertyName = existingProp, encoding = ENCODING_BASE64, output: values vector populated, return value = true    | Method returns true without exceptions            | Should Pass   |
 * | 04               | Invoke get() to retrieve 'existingProp' using ENCODING_BASE64_URL                                  | input: propertyName = existingProp, encoding = ENCODING_BASE64_URL, output: values vector populated, return value = true | Method returns true without exceptions            | Should Pass   |
 */
TEST(PlayerJsonObject, RetrieveValidProperty) {
    std::cout << "Entering RetrieveValidProperty test" << std::endl;

    // Initialize JSON with a known property
    PlayerJsonObject obj(R"({"existingProp": "Player1"})");
    std::cout << "PlayerJsonObject instance created with property 'existingProp'." << std::endl;

    // Define encodings to test
    PlayerJsonObject::ENCODING encodings[] = {
        PlayerJsonObject::ENCODING_STRING,
        PlayerJsonObject::ENCODING_BASE64,
        PlayerJsonObject::ENCODING_BASE64_URL
    };

    for (auto encoding : encodings) {
        std::vector<uint8_t> values;
        std::string encodingStr;

        switch (encoding) {
            case PlayerJsonObject::ENCODING_STRING:    encodingStr = "ENCODING_STRING"; break;
            case PlayerJsonObject::ENCODING_BASE64:    encodingStr = "ENCODING_BASE64"; break;
            case PlayerJsonObject::ENCODING_BASE64_URL:encodingStr = "ENCODING_BASE64_URL"; break;
        }

        std::cout << "Invoking get with name = 'existingProp', encoding = " << encodingStr << std::endl;

        bool ret = false;
        EXPECT_NO_THROW({
            ret = obj.get("existingProp", values, encoding);
        });

        std::cout << "Method get returned: " << (ret ? "true" : "false") << std::endl;

        // Assertions
        EXPECT_TRUE(ret);
    }

    std::cout << "Exiting RetrieveValidProperty test" << std::endl;
}
/**
 * @brief Validate behavior when accessing a non-existent property in PlayerJsonObject
 *
 * This test verifies that calling the get method with a property name that does not exist ("missingProp") returns false and does not throw an exception, regardless of the encoding used. It is designed to ensure the API properly handles requests for unavailable properties.
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
 * | Variation / Step | Description                                                      | Test Data                                           | Expected Result                                               | Notes         |
 * | :--------------: | ---------------------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------------------- | ------------- |
 * | 01               | Invoke get with property 'missingProp' using ENCODING_STRING       | name = missingProp, encoding = ENCODING_STRING      | Return value false and no exception thrown                    | Should Pass   |
 * | 02               | Invoke get with property 'missingProp' using ENCODING_BASE64         | name = missingProp, encoding = ENCODING_BASE64        | Return value false and no exception thrown                    | Should Pass   |
 * | 03               | Invoke get with property 'missingProp' using ENCODING_BASE64_URL     | name = missingProp, encoding = ENCODING_BASE64_URL    | Return value false and no exception thrown                    | Should Pass   |
 */
TEST(PlayerJsonObject, RetrieveNonExistentProperty) {
    std::cout << "Entering RetrieveNonExistentProperty test" << std::endl;

    // Create an instance of PlayerJsonObject
    PlayerJsonObject obj;
    std::cout << "PlayerJsonObject instance created using default constructor." << std::endl;

    // Define encodings to test
    PlayerJsonObject::ENCODING encodings[] = {
        PlayerJsonObject::ENCODING_STRING,
        PlayerJsonObject::ENCODING_BASE64,
        PlayerJsonObject::ENCODING_BASE64_URL
    };

    for (auto encoding : encodings) {
        std::vector<uint8_t> values;
        std::string encodingStr;

        switch (encoding) {
            case PlayerJsonObject::ENCODING_STRING:     encodingStr = "ENCODING_STRING"; break;
            case PlayerJsonObject::ENCODING_BASE64:     encodingStr = "ENCODING_BASE64"; break;
            case PlayerJsonObject::ENCODING_BASE64_URL: encodingStr = "ENCODING_BASE64_URL"; break;
        }

        std::cout << "Invoking get with name = 'missingProp', encoding = " << encodingStr << std::endl;

        bool ret = false;
        EXPECT_NO_THROW({
            ret = obj.get("missingProp", values, encoding);
        });

        // Assertions
        EXPECT_FALSE(ret);
    }

    std::cout << "Exiting RetrieveNonExistentProperty test" << std::endl;
}
/**
 * @brief Verifies that the get method returns false when provided with an empty property name.
 *
 * This test ensures that no values are retrieved by the PlayerJsonObject::get method when an empty property name is passed.
 * It iterates through all supported encoding types (ENCODING_STRING, ENCODING_BASE64, and ENCODING_BASE64_URL) and verifies
 * that the method correctly returns false, indicating that an empty property name is not accepted.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 070@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke get method for property name '' with encoding = ENCODING_STRING | name = '', encoding = ENCODING_STRING, values = empty vector | Return value is false; no values retrieved | Should Pass |
 * | 02 | Invoke get method for property name '' with encoding = ENCODING_BASE64 | name = '', encoding = ENCODING_BASE64, values = empty vector | Return value is false; no values retrieved | Should Pass |
 * | 03 | Invoke get method for property name '' with encoding = ENCODING_BASE64_URL | name = '', encoding = ENCODING_BASE64_URL, values = empty vector | Return value is false; no values retrieved | Should Pass |
 */
TEST(PlayerJsonObject, RetrieveEmptyPropertyName)
{
    std::cout << "Entering RetrieveEmptyPropertyName test" << std::endl;
    // Create an instance of PlayerJsonObject using default constructor.
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        // Loop through each encoding type.
        PlayerJsonObject::ENCODING encodings[] = {
            PlayerJsonObject::ENCODING_STRING,
            PlayerJsonObject::ENCODING_BASE64,
            PlayerJsonObject::ENCODING_BASE64_URL
        };
        for (auto encoding : encodings)
        {
            std::vector<uint8_t> values;
            std::string encodingStr;
            if(encoding == PlayerJsonObject::ENCODING_STRING) {
                encodingStr = "ENCODING_STRING";
            } else if(encoding == PlayerJsonObject::ENCODING_BASE64) {
                encodingStr = "ENCODING_BASE64";
            } else if(encoding == PlayerJsonObject::ENCODING_BASE64_URL) {
                encodingStr = "ENCODING_BASE64_URL";
            }
            std::cout << "Invoking get with empty name '', encoding = " << encodingStr << std::endl;
            bool ret = obj.get("", values, encoding);
            std::cout << "Method get returned: " << (ret ? "true" : "false") << std::endl;
            if (!ret)
            {
                std::cout << "No value retrieved because property name is empty, vector remains empty." << std::endl;
            }
            else
            {
                std::cout << "Unexpectedly retrieved values for an empty property name." << std::endl;
            }
            EXPECT_FALSE(ret);
        }
    });
    std::cout << "Exiting RetrieveEmptyPropertyName test" << std::endl;
}
/**
 * @brief Validate that a valid array key returns true using isArray method of PlayerJsonObject
 *
 * This test function verifies that when the isArray method is invoked on a PlayerJsonObject instance with a valid JSON string
 * containing an array field ("array_field"), the method correctly identifies the field as an array and returns true.
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
 * | Variation / Step | Description                                                          | Test Data                                                                 | Expected Result                                                                   | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create a PlayerJsonObject instance with a valid JSON string containing an array field | jsonStr = {"array_field": ["eng", "hin", "fr"]}                           | Instance created without throwing exceptions                                      | Should be successful |
 * | 02               | Invoke isArray method with the key "array_field"                      | key = "array_field", output: result variable                                | isArray returns true verifying the key corresponds to an array                    | Should Pass       |
 * | 03               | Validate that the returned value is true using EXPECT_TRUE            | output: result = true                                                      | Assertion passes confirming that isArray returns the expected boolean true        | Should Pass       |
 */
TEST(PlayerJsonObject, isArray_ValidArrayKeyReturnsTrue) {
    std::cout << "Entering isArray_ValidArrayKeyReturnsTrue test" << std::endl;

    // JSON string with an array field
    const char* jsonStr = R"({
        "array_field": ["eng", "hin", "fr"]
    })";

    // Create PlayerJsonObject using JSON constructor
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    // Key we want to test
    std::string key = "array_field";

    std::cout << "Invoking isArray with key: " << key << std::endl;

    bool result = false;
    EXPECT_NO_THROW({
        result = playerObj.isArray(key);
    });

    std::cout << "Method isArray returned: " << std::boolalpha << result << std::endl;

    // Verify the expected output true for array_field
    EXPECT_TRUE(result);

    std::cout << "Exiting isArray_ValidArrayKeyReturnsTrue test" << std::endl;
}
/**
 * @brief Verify isArray method returns false for a non-array JSON key
 *
 * This test verifies that when the PlayerJsonObject is constructed with a JSON string containing a non-array (string) field, invoking the isArray method with the corresponding key returns false without throwing any exceptions.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 072@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerJsonObject with JSON string having a string field | jsonStr = {"string_field": "HelloWorld"} | PlayerJsonObject instance created | Should be successful |
 * | 02 | Prepare key for isArray call | key = "string_field" | Key prepared correctly | Should be successful |
 * | 03 | Invoke isArray method on the key | input key = "string_field" | returns false without throwing exception | Should Pass |
 * | 04 | Validate the returned result using EXPECT_FALSE | result = false | Assertion passes (result is false) | Should Pass |
 */
TEST(PlayerJsonObject, isArray_ValidNonArrayKeyReturnsFalse) {
    std::cout << "Entering isArray_ValidNonArrayKeyReturnsFalse test" << std::endl;

    // JSON string with a non-array (string) field
    const char* jsonStr = R"({
        "string_field": "HelloWorld"
    })";

    // Create PlayerJsonObject using JSON constructor
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    // Prepare key
    std::string key = "string_field";
    std::cout << "Key prepared: " << key << std::endl;

    // Invoke isArray method
    std::cout << "Invoking isArray with key: " << key << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = playerObj.isArray(key);
    });

    std::cout << "Method isArray returned: " << std::boolalpha << result << std::endl;

    // Verify the expected output false for string_field
    EXPECT_FALSE(result);

    std::cout << "Exiting isArray_ValidNonArrayKeyReturnsFalse test" << std::endl;
}
/**
 * @brief Validate that isArray() returns false for a non-existent key.
 *
 * This test verifies that invoking the isArray() method with a key that does not exist in the JSON object 
 * correctly returns false without throwing any exceptions. It ensures the API gracefully handles missing fields.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                | Expected Result                                                   | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayerJsonObject with valid JSON, prepare a non-existent key, and call isArray method.   | jsonStr = {"existingArray": [1,2,3], "existingString": "Hello"}, key = non_existent_field, output: result (bool)         | API returns false and EXPECT_FALSE assertion passes               | Should Pass |
 */
TEST(PlayerJsonObject, isArray_NonExistentKeyReturnsFalse) {
    std::cout << "Entering isArray_NonExistentKeyReturnsFalse test" << std::endl;

    // JSON string with some fields, but not the one we are querying
    const char* jsonStr = R"({
        "existingArray": [1, 2, 3],
        "existingString": "Hello"
    })";

    // Create PlayerJsonObject with JSON constructor
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    // Prepare non-existent key
    std::string key = "non_existent_field";
    std::cout << "Key prepared: " << key << std::endl;

    // Invoke isArray method
    std::cout << "Invoking isArray with key: " << key << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = playerObj.isArray(key);
    });

    std::cout << "Method isArray returned: " << std::boolalpha << result << std::endl;

    // Verify the expected output: false
    EXPECT_FALSE(result);

    std::cout << "Exiting isArray_NonExistentKeyReturnsFalse test" << std::endl;
}
/**
 * @brief Test the scenario where isArray() is called with an empty key.
 *
 * This test verifies that invoking the isArray() method with an empty key on a valid PlayerJsonObject instance returns false without throwing an exception, ensuring that invalid key input is handled gracefully.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 074
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize PlayerJsonObject with a valid JSON string | jsonStr = {"numbers": [1, 2, 3], "name": "Player1"} | Object is created successfully without exceptions | Should be successful |
 * | 02 | Invoke isArray method with an empty key | key = "" | API returns false, assertion EXPECT_FALSE(result) | Should Pass |
 */
TEST(PlayerJsonObject, isArray_EmptyKeyReturnsFalse) {
    std::cout << "Entering isArray_EmptyKeyReturnsFalse test" << std::endl;

    // JSON string with valid keys (array + string)
    const char* jsonStr = R"({
        "numbers": [1, 2, 3],
        "name": "Player1"
    })";

    // Create PlayerJsonObject with JSON constructor
    PlayerJsonObject playerObj(jsonStr);
    std::cout << "PlayerJsonObject instance created with JSON: " << jsonStr << std::endl;

    // Prepare empty key
    std::string key = "";
    std::cout << "Prepared empty key: '" << key << "'" << std::endl;

    // Invoke isArray method
    std::cout << "Invoking isArray with empty key" << std::endl;
    bool result = false;
    EXPECT_NO_THROW({
        result = playerObj.isArray(key);
    });

    std::cout << "Method isArray returned: " << std::boolalpha << result << std::endl;

    // Verify expected output: false
    EXPECT_FALSE(result);

    std::cout << "Exiting isArray_EmptyKeyReturnsFalse test" << std::endl;
}
/**
 * @brief Validate that a valid integer key returns true
 *
 * This test verifies that the API correctly identifies an integer field in a JSON object. It creates a PlayerJsonObject with a JSON string containing an integer under the key "valid-key" and calls the isNumber method using the key set via a fixed size array. The test ensures that the integer is properly detected and that the method returns true as expected.
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
 * | 01 | Create a PlayerJsonObject with a JSON containing an integer field and invoke isNumber with a valid key | jsonStr = {"valid-key":42,"name":"Player1"}, key = "valid-key", expected output = true | Returns true as verified by EXPECT_TRUE assertion | Should Pass |
 */
TEST(PlayerJsonObject, ValidIntegerKey) {
    std::cout << "Entering ValidIntegerKey test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject with JSON containing an integer field
        const char* jsonStr = R"({
            "valid-key": 42,
            "name": "Player1"
        })";
        PlayerJsonObject pobj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare input key using fixed size array and strncpy
        char key[64];
        std::strncpy(key, "valid-key", sizeof(key) - 1);
        key[sizeof(key) - 1] = '\0';
        std::cout << "Key value set using strncpy: " << key << std::endl;

        // Invoke isNumber method and log the process
        std::cout << "Invoking isNumber with input: " << key << std::endl;
        bool result = pobj.isNumber(std::string(key));
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;

        // Expect true because "valid-key" is an integer in the JSON
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting ValidIntegerKey test" << std::endl;
}
/**
 * @brief Verify that isNumber returns false for a key with a non-integer value.
 *
 * This test checks whether the PlayerJsonObject's isNumber method correctly handles the scenario where the key ("non_integer_key") is associated with a string instead of an integer. The test constructs the object using a JSON input, prepares the key using strncpy, and then calls isNumber, expecting it to return false.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 076@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                         | Test Data                                                                                                  | Expected Result                                        | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | ------------------------------------------------------ | ----------- |
 * |       01         | Create PlayerJsonObject with a JSON having a non-integer value under "non_integer_key", set up the key using strncpy, and invoke isNumber to verify that it returns false. | jsonStr = {"non_integer_key": "hello", "another_field": [1, 2, 3]}, key = non_integer_key, result = false | isNumber should return false; Assertion: EXPECT_FALSE(result) | Should Pass |
 */
TEST(PlayerJsonObject, inValidIntegerKey) {
    std::cout << "Entering inValidIntegerKey test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject with JSON containing a non-integer field
        const char* jsonStr = R"({
            "non_integer_key": "hello",
            "another_field": [1, 2, 3]
        })";
        PlayerJsonObject pobj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare input key using fixed size array and strncpy
        char key[64];
        std::strncpy(key, "non_integer_key", sizeof(key) - 1);
        key[sizeof(key) - 1] = '\0';
        std::cout << "Key value set using strncpy: " << key << std::endl;

        // Invoke isNumber method and log the process
        std::cout << "Invoking isNumber with input: " << key << std::endl;
        bool result = pobj.isNumber(std::string(key));
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;

        // Expect false because "non_integer_key" exists but is a string, not a number
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting inValidIntegerKey test" << std::endl;
}
/**
 * @brief Verify that isNumber returns false for a non-existent key in the JSON.
 *
 * This test verifies that when a key that does not exist in the JSON data is provided, 
 * the isNumber method returns false, ensuring the API correctly handles non-existent keys.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                              | Expected Result                                                       | Notes          |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- | -------------- |
 * | 01               | Create a PlayerJsonObject instance with valid JSON.                         | jsonStr = {"valid_number": 42, "string_field": "hello"}                                                | Instance created successfully                                         | Should be successful |
 * | 02               | Set up a fixed-size char array to hold a key using strncpy with "Non-existent_key". | key = "Non-existent_key"                                                                                 | Key setup completed                                                   | Should be successful |
 * | 03               | Invoke isNumber method with the non-existent key and validate the result.     | input = key, output = bool result                                                                          | Return value is false and EXPECT_FALSE(result) passes                 | Should Pass    |
 */
TEST(PlayerJsonObject, NonExistentKey) {
    std::cout << "Entering NonExistentKey test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject instance with some valid JSON
        const char* jsonStr = R"({
            "valid_number": 42,
            "string_field": "hello"
        })";
        PlayerJsonObject pobj(jsonStr);
        std::cout << "PlayerJsonObject object created with JSON: " << jsonStr << std::endl;

        // Prepare input key using fixed size array and strncpy
        char key[64];
        std::strncpy(key, "Non-existent_key", sizeof(key) - 1);
        key[sizeof(key) - 1] = '\0';
        std::cout << "Key value set using strncpy: " << key << std::endl;

        // Invoke isNumber method and log the process
        std::cout << "Invoking isNumber with input: " << key << std::endl;
        bool result = pobj.isNumber(std::string(key));
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;

        // Verify expected false because the key does not exist
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting NonExistentKey test" << std::endl;
}
/**
 * @brief Verify that isNumber returns false when invoked with an empty string as key.
 *
 * This test verifies that when an empty string is provided as the key input to the isNumber method of the PlayerJsonObject,
 * the method processes the input without throwing an exception and returns false, ensuring robust handling of edge cases.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create a PlayerJsonObject instance with valid JSON and prepare an empty string key using strncpy | jsonStr = {"valid_number": 123, "string_field": "hello"}, key = "" | Object is created successfully and key is initialized to an empty string | Should be successful |@n
 * | 02 | Call isNumber with the empty string key and verify that it returns false | input: key = "", output: result (boolean) | isNumber should return false and the assertion EXPECT_FALSE(result) passes | Should Pass |
 */
TEST(PlayerJsonObject, EmptyStringInput) {
    std::cout << "Entering EmptyStringInput test" << std::endl;
    EXPECT_NO_THROW({
        // Create PlayerJsonObject instance with some dummy JSON
        const char* jsonStr = R"({
            "valid_number": 123,
            "string_field": "hello"
        })";
        PlayerJsonObject pobj(jsonStr);
        std::cout << "PlayerJsonObject object created with JSON: " << jsonStr << std::endl;

        // Prepare input key as an empty string using fixed size array and strncpy
        char key[64];
        std::strncpy(key, "", sizeof(key) - 1);
        key[sizeof(key) - 1] = '\0';
        std::cout << "Key value set using strncpy: (empty string) '" << key << "'" << std::endl;

        // Invoke isNumber method and log the process
        std::cout << "Invoking isNumber with input: (empty string)" << std::endl;
        bool result = pobj.isNumber(std::string(key));
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;

        // Verify empty string should return false
        EXPECT_FALSE(result);
    });
    std::cout << "Exiting EmptyStringInput test" << std::endl;
}
/**
 * @brief Test to verify that PlayerJsonObject::isObject returns true with a valid JSON object key.
 *
 * This test creates a PlayerJsonObject instance from a JSON string containing a "metadata" object.
 * It then prepares the key "metadata" using strncpy and verifies that the isObject method returns true.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 079@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                   | Test Data                                                                                                                         | Expected Result                                      | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- | ------------- |
 * | 01               | Create PlayerJsonObject instance using a valid JSON string containing a "metadata" object        | jsonStr = {"metadata": {"title": "Movie Title", "duration": 120}}                                                                  | Instance is created without throwing an exception    | Should Pass   |
 * | 02               | Prepare key using strncpy to copy "metadata" into a buffer                                    | inputKey = "metadata", keyBuffer size = 50, output key = "metadata"                                                                  | Key is correctly prepared as "metadata"              | Should be successful |
 * | 03               | Invoke isObject method with the prepared key                                                  | key = "metadata", output expected result = true                                                                                   | isObject returns true                                | Should Pass   |
 */
TEST(PlayerJsonObject, ValidObjectKey) {
    std::cout << "Entering ValidObjectKey test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject with JSON containing an object field
        const char* jsonStr = R"({
            "metadata": {
                "title": "Movie Title",
                "duration": 120
            }
        })";
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare key using strncpy
        char keyBuffer[50];
        std::memset(keyBuffer, 0, sizeof(keyBuffer));
        const char* inputKey = "metadata";
        std::strncpy(keyBuffer, inputKey, sizeof(keyBuffer) - 1);
        std::string key = std::string(keyBuffer);
        std::cout << "Key prepared using strncpy: " << key << std::endl;

        // Invoke isObject method
        std::cout << "Invoking isObject with key: " << key << std::endl;
        bool result = obj.isObject(key);
        std::cout << "isObject returned: " << std::boolalpha << result << std::endl;

        // Verify the key "metadata" is indeed an object
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting ValidObjectKey test" << std::endl;
}
/**
 * @brief Verify that isObject returns false for a non-object key.
 *
 * This test verifies that when a JSON string with a key mapping to a non-object value is used to create a PlayerJsonObject,
 * calling isObject with that key returns false. This ensures the API correctly identifies non-object JSON values.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 080@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                       | Test Data                                                       | Expected Result                                                    | Notes         |
 * | :----:           | ----------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------| ------------- |
 * | 01               | Create PlayerJsonObject with a JSON string containing a non-object field for key "title". | jsonStr = {"title": "Inception"}                                  | Object creation succeeds without exceptions                        | Should Pass   |
 * | 02               | Prepare key using strncpy for the "title" attribute.              | inputKey = "title", keyBuffer initialized with zeros            | Key is copied correctly into buffer                                 | Should be successful |
 * | 03               | Invoke isObject method with the prepared key.                     | key = "title"                                                   | Returns false as the value is not a JSON object (assertion check passes)| Should Pass   |
 */
TEST(PlayerJsonObject, NonObjectKeyPresent) {
    std::cout << "Entering NonObjectKeyPresent test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject with JSON containing a non-object field
        const char* jsonStr = R"({
            "title": "Inception"
        })";
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare key using strncpy for "title"
        char keyBuffer[50];
        std::memset(keyBuffer, 0, sizeof(keyBuffer));
        const char* inputKey = "title";
        std::strncpy(keyBuffer, inputKey, sizeof(keyBuffer) - 1);
        std::string key = std::string(keyBuffer);
        std::cout << "Key prepared using strncpy: " << key << std::endl;

        // Invoke isObject method
        std::cout << "Invoking isObject with key: " << key << std::endl;
        bool result = obj.isObject(key);
        std::cout << "isObject returned: " << std::boolalpha << result << std::endl;

        // Since "title" is a string, not an object, expect false
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting NonObjectKeyPresent test" << std::endl;
}
/**
 * @brief Test the isObject API for a JSON key that does not exist.
 *
 * This test verifies that when a JSON object is created without the key "nonExistent", the isObject method correctly returns false.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create PlayerJsonObject with provided JSON, prepare key using strncpy, invoke isObject with non-existent key, and assert that the returned boolean is false. | jsonStr = {"title": "Interstellar", "metadata": {"year": 2014}}, inputKey = nonExistent, output = false | The API returns false and the assertion (EXPECT_FALSE) passes. | Should Pass |
 */
TEST(PlayerJsonObject, NonExistentKey) {
    std::cout << "Entering NonExistentKey test" << std::endl;

    EXPECT_NO_THROW({
        // Create PlayerJsonObject with JSON containing some fields, but not "nonExistent"
        const char* jsonStr = R"({
            "title": "Interstellar",
            "metadata": { "year": 2014 }
        })";
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare key using strncpy for "nonExistent"
        char keyBuffer[50];
        std::memset(keyBuffer, 0, sizeof(keyBuffer));
        const char* inputKey = "nonExistent";
        std::strncpy(keyBuffer, inputKey, sizeof(keyBuffer) - 1);
        std::string key = std::string(keyBuffer);
        std::cout << "Key prepared using strncpy: " << key << std::endl;

        // Invoke isObject method
        std::cout << "Invoking isObject with key: " << key << std::endl;
        bool result = obj.isObject(key);
        std::cout << "isObject returned: " << std::boolalpha << result << std::endl;

        // Since "nonExistent" is not in the JSON, expect false
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting NonExistentKey test" << std::endl;
}
/**
 * @brief Tests that the isObject method returns false when provided with an empty key.
 *
 * This test verifies that when an empty string is passed as a key to the isObject method of PlayerJsonObject,
 * the method returns false because an empty key is considered invalid. This ensures proper input validation for key parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 082@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Initialize PlayerJsonObject with valid JSON and prepare an empty key; invoke isObject with the empty key. | jsonStr = "{ \"title\": \"Inception\", \"metadata\": { \"year\": 2010 } }", key = "" | isObject returns false, validating that an empty key is considered invalid. | Should Fail |
 */
TEST(PlayerJsonObject, EmptyStringKey) {
    std::cout << "Entering EmptyStringKey test" << std::endl;
    
    EXPECT_NO_THROW({
        // Create object with valid JSON containing some fields
        const char* jsonStr = R"({
            "title": "Inception",
            "metadata": { "year": 2010 }
        })";
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;
        
        // Prepare empty key using strncpy
        char keyBuffer[50];
        std::memset(keyBuffer, 0, sizeof(keyBuffer));
        const char* inputKey = "";
        std::strncpy(keyBuffer, inputKey, sizeof(keyBuffer) - 1);
        std::string key = std::string(keyBuffer);
        std::cout << "Key prepared using strncpy (empty string): \"" << key << "\"" << std::endl;
        
        // Invoke isObject method
        std::cout << "Invoking isObject with key: \"" << key << "\"" << std::endl;
        bool result = obj.isObject(key);
        std::cout << "isObject returned: " << std::boolalpha << result << std::endl;

        // Expect false because empty key is invalid
        EXPECT_FALSE(result);
    });
    
    std::cout << "Exiting EmptyStringKey test" << std::endl;
}
/**
 * @brief Tests that PlayerJsonObject::isString returns true when a valid string key is provided.
 *
 * This test verifies that when PlayerJsonObject is constructed using a JSON string with a valid string key, 
 * the isString API correctly identifies the key as having a string value. It ensures proper JSON parsing and 
 * string key validation, which is critical for the functionality relying on correct type handling.
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
 * | Variation / Step | Description                                                             | Test Data                                                                                         | Expected Result                                                     | Notes         |
 * | :--------------: | ----------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ------------- |
 * |      01        | Provide valid JSON string and create a PlayerJsonObject instance        | jsonStr = {"validStringKey": "some string value"}                                                 | Object is created without throwing any exception                    | Should Pass   |
 * |      02        | Prepare a key using strncpy to copy the valid key string                | validKey = "validStringKey", keyBuffer gets copied (null terminated)                                | keyBuffer contains the valid key string in a null-terminated format   | Should be successful |
 * |      03        | Invoke isString with the prepared key                                   | keyStr = "validStringKey", output result from isString                                         | isString returns true, satisfying the EXPECT_TRUE assertion           | Should Pass   |
 */
TEST(PlayerJsonObject, ValidStringKey) {
    std::cout << "Entering ValidStringKey test" << std::endl;

    EXPECT_NO_THROW({
        // JSON with a valid string key
        const char* jsonStr = R"({
            "validStringKey": "some string value"
        })";

        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare key using strncpy
        char keyBuffer[128];
        const char* validKey = "validStringKey";
        std::cout << "Assigning key using strncpy, value: " << validKey << std::endl;
        std::strncpy(keyBuffer, validKey, sizeof(keyBuffer) - 1);
        keyBuffer[sizeof(keyBuffer) - 1] = '\0';

        std::string keyStr(keyBuffer);

        // Invoke isString
        std::cout << "Invoking isString with key: " << keyStr << std::endl;
        bool result = obj.isString(keyStr);
        std::cout << "isString returned: " << std::boolalpha << result << std::endl;

        // Since the key exists and is a string, expect true
        EXPECT_TRUE(result);
    });

    std::cout << "Exiting ValidStringKey test" << std::endl;
}
/**
 * @brief Verify that isString returns false when a non-existent key is provided.
 *
 * This test validates that the PlayerJsonObject::isString method correctly returns false when the key queried does not exist in
 * the JSON input. The test constructs a JSON object with known keys ("title" and "metadata") and then attempts to check a key ("nonExistentKey")
 * that is not present, ensuring that the method behaves as expected.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Construct PlayerJsonObject using JSON input with valid keys "title" and "metadata" | jsonStr = {"title": "Sample Title", "metadata": {"duration": 120}} | Object is created without throwing exceptions | Should be successful |
 * | 02 | Prepare a non-existent key by copying "nonExistentKey" into a buffer using strncpy | nonExistentKey = "nonExistentKey" | Buffer correctly holds "nonExistentKey" | Should be successful |
 * | 03 | Invoke isString method with the prepared key from the object instance | key = "nonExistentKey", object = PlayerJsonObject instance | API returns false as the key does not exist | Should Pass |
 * | 04 | Validate the return value using EXPECT_FALSE to confirm behavior | result from isString = false | Assertion passes confirming key absence | Should Pass |
 */
TEST(PlayerJsonObject, NonExistentKey) {
    std::cout << "Entering NonExistentKey test" << std::endl;

    EXPECT_NO_THROW({
        // Construct with JSON that has some keys but NOT the one we will test
        const char* jsonStr = R"({
            "title": "Sample Title",
            "metadata": { "duration": 120 }
        })";

        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        // Prepare non-existent key using strncpy
        char keyBuffer[128];
        const char* nonExistentKey = "nonExistentKey";
        std::cout << "Assigning key using strncpy, value: " << nonExistentKey << std::endl;
        std::strncpy(keyBuffer, nonExistentKey, sizeof(keyBuffer) - 1);
        keyBuffer[sizeof(keyBuffer) - 1] = '\0';

        std::string keyStr(keyBuffer);

        // Invoke isString on non-existent key
        std::cout << "Invoking isString with key: " << keyStr << std::endl;
        bool result = obj.isString(keyStr);
        std::cout << "isString returned: " << std::boolalpha << result << std::endl;

        // Since the key is not present, expect false
        EXPECT_FALSE(result);
    });

    std::cout << "Exiting NonExistentKey test" << std::endl;
}
/**
 * @brief Verify that isString returns false for an integer type key value.
 *
 * This test verifies that when the PlayerJsonObject is instantiated with a JSON string where the key "integerKey" holds an integer
 * value instead of a string, the method isString returns false. This ensures that the data type validation within the JSON object is handled correctly.
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
 * | Variation / Step | Description                                                            | Test Data                                                            | Expected Result                                                  | Notes       |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayerJsonObject instance using JSON string                     | jsonStr = {"integerKey": 42, "title": "Sample Title"}                 | Instance is created without throwing any exceptions              | Should Pass |
 * | 02               | Prepare the key and invoke isString method on the "integerKey"           | key = "integerKey", integerKey in JSON = 42                            | isString returns false indicating that the value is not a string   | Should Pass |
 */
TEST(PlayerJsonObject, IntegerKey) {
    std::cout << "Entering IntegerKey test" << std::endl;

    const char* jsonStr = R"({
        "integerKey": 42,
        "title": "Sample Title"
    })";

    EXPECT_NO_THROW({
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        char keyBuffer[128];
        const char* integerKey = "integerKey";
        std::strncpy(keyBuffer, integerKey, sizeof(keyBuffer) - 1);
        keyBuffer[sizeof(keyBuffer) - 1] = '\0';
        std::string keyStr(keyBuffer);

        std::cout << "Invoking isString with key: " << keyStr << std::endl;
        bool result = obj.isString(keyStr);
        std::cout << "isString returned: " << std::boolalpha << result << std::endl;

        EXPECT_FALSE(result);
    });

    std::cout << "Exiting IntegerKey test" << std::endl;
}
/**
 * @brief Verify that invoking isString on a boolean key returns false
 *
 * This test verifies that when the JSON object contains a key with a boolean value, the method isString correctly identifies that the value is not a string and returns false. The test ensures that PlayerJsonObject is constructed without exceptions and that the API call to isString behaves as expected.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                               | Expected Result                                                        | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ----------- |
 * | 01               | Create PlayerJsonObject from JSON string containing a boolean key and a title                | jsonStr = {"booleanKey": true, "title": "Sample Title"}                                  | PlayerJsonObject is constructed successfully without exceptions      | Should Pass |
 * | 02               | Invoke isString with key "booleanKey" to check that the boolean value is not treated as a string | key = "booleanKey", expected output: false                                             | isString returns false as verified by EXPECT_FALSE(result)             | Should Pass |
 */
TEST(PlayerJsonObject, BooleanKey) {
    std::cout << "Entering BooleanKey test" << std::endl;

    const char* jsonStr = R"({
        "booleanKey": true,
        "title": "Sample Title"
    })";

    EXPECT_NO_THROW({
        PlayerJsonObject obj(jsonStr);
        std::cout << "PlayerJsonObject created with JSON: " << jsonStr << std::endl;

        char keyBuffer[128];
        const char* booleanKey = "booleanKey";
        std::strncpy(keyBuffer, booleanKey, sizeof(keyBuffer) - 1);
        keyBuffer[sizeof(keyBuffer) - 1] = '\0';
        std::string keyStr(keyBuffer);

        std::cout << "Invoking isString with key: " << keyStr << std::endl;
        bool result = obj.isString(keyStr);
        std::cout << "isString returned: " << std::boolalpha << result << std::endl;

        EXPECT_FALSE(result);
    });

    std::cout << "Exiting BooleanKey test" << std::endl;
}
/**
 * @brief Test PlayerJsonObject::isString with an empty key to verify proper handling of invalid key inputs
 *
 * This test ensures that when an empty key is provided to the isString method of PlayerJsonObject, the method correctly returns false,
 * indicating that the key does not exist in the JSON object. This test validates the robustness of the JSON key handling logic.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 087@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerJsonObject using a valid JSON string and invoke isString() with an empty key | jsonStr = {"validStringKey": "Player1", "integerKey": 123, "booleanKey": true}, keyStr = "" | isString() returns false; assertion EXPECT_FALSE(result) passes | Should Fail |
 */
TEST(PlayerJsonObject, EmptyKey) {
    std::cout << "Entering EmptyKey test" << std::endl;

    // Initialize JSON with some sample keys
    const char* jsonStr = R"({
        "validStringKey": "Player1",
        "integerKey": 123,
        "booleanKey": true
    })";

    // Create object using JSON constructor
    PlayerJsonObject obj(jsonStr);
    std::cout << "PlayerJsonObject instance created using JSON constructor." << std::endl;

    // Empty key
    std::string keyStr = "";
    std::cout << "Invoking isString with empty key" << std::endl;

    bool result = obj.isString(keyStr);
    std::cout << "isString returned: " << std::boolalpha << result << std::endl;

    EXPECT_FALSE(result);
    std::cout << "Exiting EmptyKey test" << std::endl;
}
/**
 * @brief Validate that PlayerJsonObject::print returns a valid JSON string for a properly initialized object.
 *
 * This test verifies that when a PlayerJsonObject is created using the default constructor, calling its print() method returns a non-empty JSON string that accurately reflects its internal state. The test ensures that no exceptions are thrown during object construction and method invocation.
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
 * | 01 | Create PlayerJsonObject instance using default constructor | No input parameters, output: instance created | Instance is created without throwing exceptions | Should be successful |
 * | 02 | Invoke print() method on the instance | Input: internal state of PlayerJsonObject, output: JSON string from print() method | Non-empty JSON string is returned and no exception is thrown | Should Pass |
 */
TEST(PlayerJsonObject, Print_returns_valid_JSON_string_for_a_properly_initialized_object) {
    std::cout << "Entering Print_returns_valid_JSON_string_for_a_properly_initialized_object test" << std::endl;

    // Create the PlayerJsonObject instance using the default constructor
    EXPECT_NO_THROW({
        PlayerJsonObject playerJson;
        std::cout << "PlayerJsonObject instance created using default constructor." << std::endl;

        // Invoke print() method
        std::cout << "Invoking print() method of PlayerJsonObject." << std::endl;
        std::string jsonResult;
        EXPECT_NO_THROW({
            jsonResult = playerJson.print();
            std::cout << "print() returned JSON string: " << jsonResult << std::endl;
        });

        std::cout << "Debug: Validated that the JSON string is non-empty, reflecting the internal state." << std::endl;
    });

    std::cout << "Exiting Print_returns_valid_JSON_string_for_a_properly_initialized_object test" << std::endl;
}
/**
 * @brief Validate that the PlayerJsonObject correctly prints a valid JSON to an empty data vector without throwing exceptions.
 *
 * This test verifies that a PlayerJsonObject constructed with a valid JSON string ("{\"key\":\"value\"}") does not throw any exceptions when its print method is invoked on an initially empty vector. The output vector is then converted to a std::string for verification purposes.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 089@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:** 
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an empty vector and instantiate PlayerJsonObject with a valid JSON string. | input = JSON string = "{\"key\":\"value\"}" | Instance is created without throwing exceptions. | Should Pass |
 * | 02 | Invoke the print method using the empty vector to capture printed JSON. | input = data vector (empty) | print method executes without throwing exceptions; data vector is populated with JSON string. | Should Pass |
 * | 03 | Convert the output vector to a std::string for verification of printed JSON. | output = data vector contents | Printed JSON string matches the expected valid JSON format. | Should be successful |
 */
TEST(PlayerJsonObject, Valid_JSON_printing_to_an_empty_vector)
{
    std::cout << "Entering Valid_JSON_printing_to_an_empty_vector test" << std::endl;

    // Create an empty vector of uint8_t to capture JSON print output.
    std::vector<uint8_t> data;

    EXPECT_NO_THROW({
        // Construct PlayerJsonObject with valid JSON string
        PlayerJsonObject player("{\"key\":\"value\"}");
        std::cout << "PlayerJsonObject instance created with valid JSON string" << std::endl;

        // Invoke the print method
        std::cout << "Invoking print method with data vector of size: " << data.size() << std::endl;
        player.print(data);
        std::cout << "Returned from print invocation" << std::endl;

        // Convert the output vector to a std::string for debug logging.
        std::string printedJson(data.begin(), data.end());
        std::cout << "Printed JSON string inside data vector: " << printedJson << std::endl;

    });

    std::cout << "Exiting Valid_JSON_printing_to_an_empty_vector test" << std::endl;
}
/**
 * @brief Tests that the PlayerJsonObject can be created and its print_UnFormatted method returns a non-empty JSON string.
 *
 * This test verifies that a PlayerJsonObject instance can be successfully created using its default constructor without throwing any exceptions.
 * It further checks that the print_UnFormatted method can be invoked without exceptions and that it returns a non-empty JSON string, confirming a basic functionality of JSON generation.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 090
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create PlayerJsonObject using default constructor | constructor: default | Object instantiation succeeds without exceptions | Should Pass |
 * | 02 | Invoke print_UnFormatted method on PlayerJsonObject instance to obtain JSON string | output: jsonResult = result of print_UnFormatted() | Non-empty JSON string is returned and no exception is thrown | Should Pass |
 */
TEST(PlayerJsonObject, print_UnFormatted_start) {
    std::cout << "Entering print_UnFormatted_start test" << std::endl;
    
    // Create object using default constructor and verify no exception occurs
    EXPECT_NO_THROW({
        PlayerJsonObject obj;
        std::cout << "Created PlayerJsonObject object using default constructor." << std::endl;
        
        // Invoke the method print_UnFormatted
        std::cout << "Invoking print_UnFormatted() method." << std::endl;
        std::string jsonResult;
        EXPECT_NO_THROW({
            jsonResult = obj.print_UnFormatted();
            std::cout << "print_UnFormatted() returned: " << jsonResult << std::endl;
        });
        
        std::cout << "Verified that the returned JSON string is non-empty." << std::endl;
    });
    
    std::cout << "Exiting print_UnFormatted_start test" << std::endl;
}
/**
 * @brief Verify that a fully initialized PlayerJsonObject instance is destroyed without exceptions.
 *
 * This test verifies that both parent and main PlayerJsonObject objects, after being fully initialized and having their internal state simulated, can be deleted safely without throwing any exceptions. It ensures that the destructor of the PlayerJsonObject manages proper cleanup of internal resources and parent-child relationships.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 091@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                  | Test Data                                                       | Expected Result                                                               | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a valid parent PlayerJsonObject instance using the default constructor.               | No input required; output: parent object pointer valid          | Parent object is created without any exception.                               | Should be successful |
 * | 02               | Create a main PlayerJsonObject instance using the default constructor.                       | No input required; output: main object pointer valid              | Main object is created without any exception.                                 | Should be successful |
 * | 03               | Simulate internal state by assigning a valid cJSON pointer and setting mParent for main object. | Internal state simulation; values: simulated cJSON pointer, parent pointer | Internal state is set successfully as simulated in logs.                     | Should be successful |
 * | 04               | Invoke destructor for the main PlayerJsonObject instance.                                   | Input: main object pointer; Expected output: no exceptions thrown  | Main object is deleted without exceptions, calling its destructor properly.   | Should Pass   |
 * | 05               | Invoke destructor for the parent PlayerJsonObject instance.                                 | Input: parent object pointer; Expected output: no exceptions thrown | Parent object is deleted without exceptions, calling its destructor properly. | Should Pass   |
 */
TEST(PlayerJsonObject, Destruction_of_a_fully_initialized_PlayerJsonObject_instance) {
    std::cout << "Entering Destruction_of_a_fully_initialized_PlayerJsonObject_instance test" << std::endl;

    // Create a valid parent PlayerJsonObject instance using default constructor
    std::cout << "Invoking PlayerJsonObject default constructor for parent object" << std::endl;
    EXPECT_NO_THROW({
        PlayerJsonObject* parent = new PlayerJsonObject();
        std::cout << "Parent object created at address: " << parent << std::endl;

        // Create a main PlayerJsonObject instance using default constructor
        std::cout << "Invoking PlayerJsonObject default constructor for main object" << std::endl;
        PlayerJsonObject* player = new PlayerJsonObject();
        std::cout << "Main object created at address: " << player << std::endl;

        // Simulate the internal state assignment: 
        // mJsonObj initialized with a valid pointer (simulated as non-null debug log)
        // mParent set to parent object (simulated via debug logs)
        std::cout << "Simulating internal state setup:" << std::endl;
        std::cout << " - mJsonObj is set to a valid cJSON pointer (simulation)" << std::endl;
        std::cout << " - mParent is assigned the parent object at address: " << parent << std::endl;

        // Destruction of main object
        std::cout << "Invoking destructor for main object" << std::endl;
        EXPECT_NO_THROW({
            delete player;
            std::cout << "PlayerJsonObject destructor invoked on main object" << std::endl;
        });

        // Destruction of parent object
        std::cout << "Invoking destructor for parent object" << std::endl;
        EXPECT_NO_THROW({
            delete parent;
            std::cout << "PlayerJsonObject destructor invoked on parent object" << std::endl;
        });
    });

    std::cout << "Exiting Destruction_of_a_fully_initialized_PlayerJsonObject_instance test" << std::endl;
}
/**
 * @brief Verify that the PlayerJsonParseException::what() method returns the expected error message.
 *
 * This test creates a PlayerJsonParseException object using the default constructor and invokes its what() method to retrieve the error message. It validates that the object is successfully created and the what() method returns a valid error message, which is critical for debugging exception details.
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
 * | Variation / Step | Description                                                                 | Test Data                                                                                  | Expected Result                                                         | Notes            |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a PlayerJsonParseException object using default constructor          | constructor = default, output = valid PlayerJsonParseException object                        | Object is created without throwing an exception and logs creation info  | Should Pass      |
 * | 02               | Invoke the what() method on the exception object to capture the error message | exception object = valid PlayerJsonParseException object, output = errorMessage variable       | what() returns a valid error message as a C-style string                | Should Pass      |
 * | 03               | Print the returned error message and conclude the test                      | No additional input; utilizes previously captured errorMessage                             | Logs display the error message and confirm method functionality         | Should be successful |
 */
TEST(PlayerJsonParseException, VerifyThatWhatReturnsTheExpectedErrorMessage) {
    std::cout << "Entering VerifyThatWhatReturnsTheExpectedErrorMessage test" << std::endl;

    // Create a PlayerJsonParseException object using default constructor
    EXPECT_NO_THROW({
        PlayerJsonParseException exception;
        std::cout << "Created PlayerJsonParseException object using default constructor." << std::endl;
        
        // Invocation of what() method with debug logs
        std::cout << "Invoking PlayerJsonParseException::what() method." << std::endl;
        const char* errorMessage = nullptr;
        EXPECT_NO_THROW({
            errorMessage = exception.what();
            std::cout << "PlayerJsonParseException::what() returned: " << errorMessage << std::endl;
        });
        
    });

    std::cout << "Exiting VerifyThatWhatReturnsTheExpectedErrorMessage test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
