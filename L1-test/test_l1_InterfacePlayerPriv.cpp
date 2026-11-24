
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
#include "InterfacePlayerPriv.h"


// Test Case: Move construction with typical valid data
/**
 * @brief Test move construction of CallbackData with typical valid data
 *
 * This test case verifies that the move constructor for GstPlayerPriv::CallbackData successfully transfers the state from a source object to a new object. The source object is constructed with typical valid inputs, and upon moving, the new object retains the expected member values.
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
 * | 01 | Create original CallbackData object using custom constructor | instance = 0x1234, id = 123, name = TestName | Object is constructed without any exceptions | Should Pass |
 * | 02 | Invoke move constructor to create moved CallbackData object | original object state | Moved object is created successfully with no exceptions | Should Pass |
 * | 03 | Verify the moved object's member values | moved.instance = 0x1234, moved.id = 123, moved.name = TestName | EXPECT_EQ validations pass confirming correct move semantics | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, MoveConstructionWithTypicalValidData) {
    std::cout << "Entering MoveConstructionWithTypicalValidData test" << std::endl;

    // Create original object using custom constructor
    gpointer validPtr = (gpointer)0x1234;
    gulong id = 123;
    char nameArr[50];
    std::strncpy(nameArr, "TestName", sizeof(nameArr) - 1);
    nameArr[sizeof(nameArr) - 1] = '\0';
    
    // Log the creation of the original
    std::cout << "Creating original CallbackData object with instance = " << validPtr
              << ", id = " << id 
              << ", name = " << nameArr << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData original(validPtr, id, std::string(nameArr));
        
        std::cout << "Original object created with instance = " << original.instance
                  << ", id = " << original.id
                  << ", name = " << original.name << std::endl;
        
        // Invoke move constructor
        std::cout << "Invoking move constructor for CallbackData using original object." << std::endl;
        EXPECT_NO_THROW({
            GstPlayerPriv::CallbackData moved(std::move(original));
            
            // Log moved object state
            std::cout << "Moved object created with instance = " << moved.instance
                      << ", id = " << moved.id
                      << ", name = " << moved.name << std::endl;
            
            // Verify the moved object's member values
            EXPECT_EQ(moved.instance, validPtr);
            EXPECT_EQ(moved.id, id);
            EXPECT_EQ(moved.name, std::string(nameArr));
        });
    });

    std::cout << "Exiting MoveConstructionWithTypicalValidData test" << std::endl;
}
/**
 * @brief Test move construction of CallbackData with null pointer and default values
 *
 * This test verifies that the move constructor of GstPlayerPriv::CallbackData correctly handles a nullptr instance along with default numeric and string values. The test ensures that after moving, the new object retains the expected values while no exceptions are thrown.
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
 * | Variation / Step | Description                                                                         | Test Data                                                    | Expected Result                                                                    | Notes       |
 * | :----:           | ----------------------------------------------------------------------------------- | ------------------------------------------------------------ | ----------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create original CallbackData object using nullptr for instance and default values     | input: instance = nullptr, id = 0, name = "TestName"         | Object is created without throwing exception                                        | Should Pass |
 * | 02               | Invoke move constructor to move the original object to the new object                | input: moving original object with instance = nullptr, id = 0, name = "TestName" | Moved object correctly has instance = nullptr, id = 0, and name = "TestName", and no exception is thrown | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, MoveConstructionWithNullPointerAndDefaultValues) {
    std::cout << "Entering MoveConstructionWithNullPointerAndDefaultValues test" << std::endl;

    // Create original object with nullptr and default numeric/string values
    gpointer nullPtr = nullptr;
    gulong id = 0;
    char nameArr[50];
    std::strncpy(nameArr, "TestName", sizeof(nameArr) - 1);
    nameArr[sizeof(nameArr) - 1] = '\0';

    // Log the creation of the original object
    std::cout << "Creating original CallbackData object with instance = nullptr"
              << ", id = " << id 
              << ", name = " << nameArr << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData original(nullPtr, id, std::string(nameArr));
        
        std::cout << "Original object created with instance = " << original.instance
                  << ", id = " << original.id
                  << ", name = " << original.name << std::endl;
        
        // Invoke move constructor
        std::cout << "Invoking move constructor for CallbackData using original object." << std::endl;
        EXPECT_NO_THROW({
            GstPlayerPriv::CallbackData moved(std::move(original));
            
            // Log moved object state
            std::cout << "Moved object created with instance = " << moved.instance
                      << ", id = " << moved.id
                      << ", name = " << moved.name << std::endl;
            
            // Verify the moved object's member values
            EXPECT_EQ(moved.instance, nullptr);
            EXPECT_EQ(moved.id, id);
            EXPECT_EQ(moved.name, std::string(nameArr));
        });
    });

    std::cout << "Exiting MoveConstructionWithNullPointerAndDefaultValues test" << std::endl;
}
/**
 * @brief Test move construction of CallbackData with maximum id value.
 *
 * This test verifies that the move constructor of the CallbackData class correctly transfers
 * the data from the original object when the object is constructed with a maximum id value.
 * It checks that the moved object's members (instance, id, and name) match those of the original
 * object created with a custom constructor.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                          | Expected Result                                                          | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | -------------------------------------------------- | ------------------------------------------------------------------------- | ------------- |
 * | 01               | Create the original CallbackData object using a custom constructor with maximum id value | validPtr = 0xABCD, id = ULONG_MAX, name = MaxIdTest | Original object is created without throwing exceptions                    | Should Pass   |
 * | 02               | Invoke the move constructor to create a new CallbackData object from the original | original object with validPtr = 0xABCD, id = ULONG_MAX, name = MaxIdTest | Moved object has the same member values as the original object without exception | Should Pass   |
 */
TEST(GstPlayerPriv_CallbackData, MoveConstructionWithMaximumIdValue) {
    std::cout << "Entering MoveConstructionWithMaximumIdValue test" << std::endl;

    // Create original object using custom constructor with maximum id value
    gpointer validPtr = (gpointer)0xABCD;
    gulong id = ULONG_MAX;
    char nameArr[50];
    std::strncpy(nameArr, "MaxIdTest", sizeof(nameArr) - 1);
    nameArr[sizeof(nameArr) - 1] = '\0';

    // Log the creation of the original object
    std::cout << "Creating original CallbackData object with instance = " << validPtr
              << ", id = " << id 
              << ", name = " << nameArr << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData original(validPtr, id, std::string(nameArr));
        
        std::cout << "Original object created with instance = " << original.instance
                  << ", id = " << original.id
                  << ", name = " << original.name << std::endl;

        // Invoke move constructor
        std::cout << "Invoking move constructor for CallbackData using original object." << std::endl;
        EXPECT_NO_THROW({
            GstPlayerPriv::CallbackData moved(std::move(original));

            // Log moved object state
            std::cout << "Moved object created with instance = " << moved.instance
                      << ", id = " << moved.id
                      << ", name = " << moved.name << std::endl;
            
            // Verify the moved object's member values
            EXPECT_EQ(moved.instance, validPtr);
            EXPECT_EQ(moved.id, id);
            EXPECT_EQ(moved.name, std::string(nameArr));
        });
    });

    std::cout << "Exiting MoveConstructionWithMaximumIdValue test" << std::endl;
}
/**
 * @brief Test to verify that the copy constructor correctly copies an object with a valid pointer, non-empty name, and non-zero id.
 *
 * This test verifies that when a GstPlayerPriv::CallbackData object is created with a valid non-null instance pointer, a non-zero id, and a non-empty name, the copy constructor correctly duplicates the object's values. The test ensures that no exceptions are thrown during copying and that all member variables of the copied object match those of the original object.
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
 * | 01 | Create an original CallbackData object using a valid pointer, non-zero id, and non-empty name. | input: orig_instance = 0x1234, orig_id = 12345, orig_name = "TestName" | Original object is created successfully without throwing exceptions. | Should be successful |
 * | 02 | Invoke the copy constructor on the original object and validate that the copied object’s members match the original. | input: original object with instance = 0x1234, id = 12345, name = "TestName" | The copy object's instance, id, and name match those of the original and no exception is thrown. | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, Copy_object_with_valid_pointer_non_empty_name_and_non_zero_id) {
    std::cout << "Entering Copy object with valid pointer, non-empty name, and non-zero id test" << std::endl;
    
    // Prepare original object values
    gpointer orig_instance = reinterpret_cast<void*>(0x1234);
    unsigned long orig_id = 12345;
    char nameBuffer[50] = {0};
    const char* initName = "TestName";
    std::strncpy(nameBuffer, initName, sizeof(nameBuffer)-1);
    std::string orig_name(nameBuffer);
    
    std::cout << "Creating original CallbackData with instance: " << orig_instance 
              << ", id: " << orig_id 
              << ", name: " << orig_name << std::endl;
    
    // Create original CallbackData object using the parameterized constructor
    GstPlayerPriv::CallbackData original(orig_instance, orig_id, orig_name);
    
    std::cout << "Invoking copy constructor on original CallbackData" << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData copy(original);
        std::cout << "Copy object created with instance: " << copy.instance 
                  << ", id: " << copy.id
                  << ", name: " << copy.name << std::endl;
        EXPECT_EQ(copy.instance, original.instance);
        EXPECT_EQ(copy.id, original.id);
        EXPECT_EQ(copy.name, original.name);
    });
    
    std::cout << "Exiting Copy object with valid pointer, non-empty name, and non-zero id test" << std::endl;
}
/**
 * @brief Test copying CallbackData object with maximum id value
 *
 * This test verifies that the copy constructor of the CallbackData object correctly duplicates an object when initialized with the maximum possible id value. It ensures that the instance, id, and name properties are properly copied from the original object to the new one.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Initialize original CallbackData object with instance = 0x4321, id = ULONG_MAX, and name = NormalName | orig_instance = 0x4321, orig_id = ULONG_MAX, orig_name = NormalName | Original CallbackData object is initialized with the correct values | Should be successful |
 * | 02 | Create a copy of CallbackData using the copy constructor and verify that the new object has matching instance, id, and name | Input: original object with instance = 0x4321, id = ULONG_MAX, name = NormalName; Output: Copy object with same field values | Copy object is created without exception and all fields match the original object | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, Copy_object_with_maximum_id_value) {
    std::cout << "Entering Copy object with maximum id value test" << std::endl;
    
    // Prepare original object values
    gpointer orig_instance = reinterpret_cast<void*>(0x4321);
    unsigned long orig_id = ULONG_MAX;
    char nameBuffer[50] = {0};
    const char* initName = "NormalName";
    std::strncpy(nameBuffer, initName, sizeof(nameBuffer)-1);
    std::string orig_name(nameBuffer);
    
    std::cout << "Creating original CallbackData with instance: " << orig_instance 
              << ", id: " << orig_id 
              << ", name: " << orig_name << std::endl;
    
    // Create original object using parameterized constructor
    GstPlayerPriv::CallbackData original(orig_instance, orig_id, orig_name);
    
    std::cout << "Invoking copy constructor on original CallbackData" << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData copy(original);
        std::cout << "Copy object created with instance: " << copy.instance 
                  << ", id: " << copy.id 
                  << ", name: " << copy.name << std::endl;
        EXPECT_EQ(copy.instance, original.instance);
        EXPECT_EQ(copy.id, original.id);
        EXPECT_EQ(copy.name, original.name);
    });
    
    std::cout << "Exiting Copy object with maximum id value test" << std::endl;
}
/**
 * @brief Verify that the copy constructor accurately duplicates a CallbackData object with special characters in its name
 *
 * This test creates an original CallbackData object with an instance pointer, an identifier, and a name containing special characters. It then uses the copy constructor to duplicate the object. The test validates that the copy's instance, id, and name exactly match those of the original object, ensuring that the handling of special characters in string fields is correct.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Setup original CallbackData with special characters in its name | orig_instance = 0x5678, orig_id = 123, orig_name = "EdgeCase!@#" | Original CallbackData object is created successfully with the specified values | Should be successful |
 * | 02 | Invoke copy constructor to create and validate copy of original object | Original object with instance = 0x5678, id = 123, name = "EdgeCase!@#" | Copy object's instance, id, and name match those of the original; no exception is thrown | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, Copy_object_with_special_characters_in_name) {
    std::cout << "Entering Copy object with special characters in name test" << std::endl;
    
    // Prepare original object values
    gpointer orig_instance = reinterpret_cast<void*>(0x5678);
    unsigned long orig_id = 123;
    char nameBuffer[50] = {0};
    const char* initName = "EdgeCase!@#";
    std::strncpy(nameBuffer, initName, sizeof(nameBuffer)-1);
    std::string orig_name(nameBuffer);
    
    std::cout << "Creating original CallbackData with instance: " << orig_instance 
              << ", id: " << orig_id 
              << ", name: " << orig_name << std::endl;
    
    // Create original object using parameterized constructor
    GstPlayerPriv::CallbackData original(orig_instance, orig_id, orig_name);
    
    std::cout << "Invoking copy constructor on original CallbackData" << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData copy(original);
        std::cout << "Copy object created with instance: " << copy.instance 
                  << ", id: " << copy.id 
                  << ", name: " << copy.name << std::endl;
        EXPECT_EQ(copy.instance, original.instance);
        EXPECT_EQ(copy.id, original.id);
        EXPECT_EQ(copy.name, original.name);
    });
    
    std::cout << "Exiting Copy object with special characters in name test" << std::endl;
}
/**
 * @brief Test the copy constructor for CallbackData with a nullptr instance
 *
 * This test verifies that the GstPlayerPriv::CallbackData copy constructor correctly duplicates an object created with a nullptr instance, a valid id, and a non-empty name. It ensures that the copied object has identical member values and that the original object remains unchanged.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create an original CallbackData object with orig_instance=nullptr, orig_id=12345, and orig_name="TestName". Invoke the copy constructor to create a new object. Verify that the copy retains the same member values as the original and that the original object's state remains unchanged. | orig_instance = nullptr, orig_id = 12345, orig_name = TestName, copy.instance = nullptr, copy.id = 12345, copy.name = TestName | The copy object should have identical member values to the original, and the original object should remain unchanged. All assertions must pass without exceptions. | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, Copy_object_with_nullptr_instance) {
    std::cout << "Entering Copy object with nullptr instance test" << std::endl;
    
    // Prepare original object values
    gpointer orig_instance = nullptr;
    unsigned long orig_id = 12345;
    char nameBuffer[50] = {0};
    const char* initName = "TestName";
    std::strncpy(nameBuffer, initName, sizeof(nameBuffer)-1);
    std::string orig_name(nameBuffer);
    
    std::cout << "Creating original CallbackData with instance: " << orig_instance 
              << ", id: " << orig_id 
              << ", name: " << orig_name << std::endl;
    
    // Create original object using parameterized constructor
    GstPlayerPriv::CallbackData original(orig_instance, orig_id, orig_name);
    
    std::cout << "Invoking copy constructor on original CallbackData" << std::endl;
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData copy(original);
        std::cout << "Copy object created with instance: " << copy.instance 
                  << ", id: " << copy.id 
                  << ", name: " << copy.name << std::endl;
        EXPECT_EQ(copy.instance, original.instance);
        EXPECT_EQ(copy.id, original.id);
        EXPECT_EQ(copy.name, original.name);
        std::cout << "Verifying that original object remains unchanged" << std::endl;
        EXPECT_EQ(original.instance, nullptr);
        EXPECT_EQ(original.id, orig_id);
        EXPECT_EQ(original.name, orig_name);
    });
    
    std::cout << "Exiting Copy object with nullptr instance test" << std::endl;
}
/**
 * @brief Validate that CallbackData is constructed successfully with a valid non-null pointer, a positive id, and a non-empty name.
 *
 * This test verifies that when CallbackData is instantiated with a valid non-null pointer along with a positive id and a non-empty name, 
 * the constructor executes without throwing any exceptions and all internal member variables (instance, id, and name) are properly assigned.
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
 * | Variation / Step | Description                                                                 | Test Data                                                        | Expected Result                                                                                 | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize a valid pointer, a positive id, and a non-empty name.             | validPointer = address of dummyValue, id = 123, name = "TestName"  | Test variables are initialized correctly.                                                     | Should be successful |
 * | 02               | Invoke the CallbackData constructor within EXPECT_NO_THROW to catch exceptions.| input: validPointer, id, name                                    | Constructor invocation does not throw any exception.                                           | Should Pass   |
 * | 03               | Assert that the object's internal state (instance, id, and name) is as expected.| instance = validPointer, id = 123, name = "TestName"               | EXPECT_EQ validations pass verifying that instance, id, and name match the input values.          | Should Pass   |
 */
TEST(GstPlayerPriv_CallbackData, ValidNonNullPointerPositiveIdNonEmptyName) {
    std::cout << "Entering ValidNonNullPointerPositiveIdNonEmptyName test" << std::endl;
    
    int dummyValue = 100;
    gpointer validPointer = static_cast<void*>(&dummyValue);
    gulong id = 123;
    std::string name = "TestName";
    std::cout << "Invoking CallbackData constructor with instance: " << validPointer 
              << ", id: " << id 
              << ", name: " << name << std::endl;
    
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData obj(validPointer, id, name);
        std::cout << "Constructor invoked successfully." << std::endl;
        std::cout << "Internal state - instance: " << obj.instance 
                  << ", id: " << obj.id 
                  << ", name: " << obj.name << std::endl;
        EXPECT_EQ(obj.instance, validPointer);
        EXPECT_EQ(obj.id, id);
        EXPECT_EQ(obj.name, name);
    });
    
    std::cout << "Exiting ValidNonNullPointerPositiveIdNonEmptyName test" << std::endl;
}
/**
 * @brief Verify CallbackData constructor accepts a null pointer with valid id and non-empty name
 *
 * This test verifies that the CallbackData constructor of GstPlayerPriv correctly initializes its members when passed a null pointer for the instance, a valid non-zero id, and a non-empty name. It ensures no exception is thrown and that the internal state matches the input parameters.
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
 * | Variation / Step | Description                                                            | Test Data                                                   | Expected Result                                                        | Notes         |
 * | :--------------: | ---------------------------------------------------------------------- | ----------------------------------------------------------- | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize test parameters with null pointer, valid id, and non-empty name  | instance = nullptr, id = 123, name = TestName                | Parameters are set as defined                                          | Should be successful |
 * | 02               | Invoke the CallbackData constructor with the input parameters          | Invoke API with instance = nullptr, id = 123, name = TestName  | Constructor executes without throwing an exception                   | Should Pass   |
 * | 03               | Validate the internal state of the created CallbackData object           | Object state: instance = nullptr, id = 123, name = TestName    | Object members exactly match the provided input values and assertions pass | Should Pass   |
 */
TEST(GstPlayerPriv_CallbackData, NullPointerPositiveIdNonEmptyName) {
    std::cout << "Entering NullPointerPositiveIdNonEmptyName test" << std::endl;
    
    gpointer nullPointer = nullptr;
    gulong id = 123;
    std::string name = "TestName";
    std::cout << "Invoking CallbackData constructor with instance: " << nullPointer 
              << ", id: " << id 
              << ", name: " << name << std::endl;
    
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData obj(nullPointer, id, name);
        std::cout << "Constructor invoked successfully." << std::endl;
        std::cout << "Internal state - instance: " << obj.instance 
                  << ", id: " << obj.id 
                  << ", name: " << obj.name << std::endl;
        EXPECT_EQ(obj.instance, nullPointer);
        EXPECT_EQ(obj.id, id);
        EXPECT_EQ(obj.name, name);
    });
    
    std::cout << "Exiting NullPointerPositiveIdNonEmptyName test" << std::endl;
}
/**
 * @brief Tests that constructing CallbackData with a non-null pointer, zero id, and non-empty name operates correctly.
 *
 * This test ensures that when the CallbackData constructor is invoked with a valid non-null pointer, an id of 0, and a non-empty name ("TestName"), the object's internal state (instance, id, name) matches the input parameters and no exceptions are thrown.
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
 * | 01 | Invoke CallbackData constructor with valid pointer, id = 0, and a non-empty name | instance = validPointer (address of dummyValue), id = 0, name = "TestName" | Object's instance equals validPointer, id equals 0, name equals "TestName"; No exception is thrown | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, NonNullPointerZeroIdNonEmptyName) {
    std::cout << "Entering NonNullPointerZeroIdNonEmptyName test" << std::endl;
    
    int dummyValue = 200;
    gpointer validPointer = static_cast<void*>(&dummyValue);
    gulong id = 0;
    std::string name = "TestName";
    std::cout << "Invoking CallbackData constructor with instance: " << validPointer 
              << ", id: " << id 
              << ", name: " << name << std::endl;
    
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData obj(validPointer, id, name);
        std::cout << "Constructor invoked successfully." << std::endl;
        std::cout << "Internal state - instance: " << obj.instance 
                  << ", id: " << obj.id 
                  << ", name: " << obj.name << std::endl;
        EXPECT_EQ(obj.instance, validPointer);
        EXPECT_EQ(obj.id, id);
        EXPECT_EQ(obj.name, name);
    });
    
    std::cout << "Exiting NonNullPointerZeroIdNonEmptyName test" << std::endl;
}
/**
 * @brief Verify that CallbackData constructor initializes correctly with a non-null pointer, maximum id, and non-empty name.
 *
 * This test verifies that the GstPlayerPriv::CallbackData constructor does not throw an exception when provided with a valid instance pointer, the maximum possible value for id (ULONG_MAX), and a non-empty string ("TestName"). It further checks that the internal state of the created object matches the input parameters.
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
 * | Variation / Step | Description                                                                                                       | Test Data                                                                                                                      | Expected Result                                                                                                                                | Notes       |
 * | :--------------: | ----------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the CallbackData constructor with valid instance pointer, ULONG_MAX as id, and non-empty name "TestName".   | input validPointer = address of dummyValue, input id = ULONG_MAX, input name = "TestName", output instance = validPointer, output id = ULONG_MAX, output name = "TestName" | The constructor should not throw; the object’s internal state (instance, id, name) should match the provided input values and pass the EXPECT_EQ assertions. | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, NonNullPointerMaxIdNonEmptyName) {
    std::cout << "Entering NonNullPointerMaxIdNonEmptyName test" << std::endl;
    
    int dummyValue = 300;
    gpointer validPointer = static_cast<void*>(&dummyValue);
    gulong id = ULONG_MAX;
    std::string name = "TestName";
    std::cout << "Invoking CallbackData constructor with instance: " << validPointer 
              << ", id: " << id 
              << ", name: " << name << std::endl;
    
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData obj(validPointer, id, name);
        std::cout << "Constructor invoked successfully." << std::endl;
        std::cout << "Internal state - instance: " << obj.instance 
                  << ", id: " << obj.id 
                  << ", name: " << obj.name << std::endl;
        EXPECT_EQ(obj.instance, validPointer);
        EXPECT_EQ(obj.id, id);
        EXPECT_EQ(obj.name, name);
    });
    
    std::cout << "Exiting NonNullPointerMaxIdNonEmptyName test" << std::endl;
}
/**
 * @brief Verify that the CallbackData constructor correctly handles a valid non-null pointer, a positive id, and an empty string as name.
 *
 * This test validates that the CallbackData constructor initializes an object using a valid pointer, a given non-zero id, and an empty name without throwing any exceptions. It ensures that the object's internal state accurately reflects the input parameters.
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
 * | Variation / Step | Description                                                                      | Test Data                                                        | Expected Result                                                                              | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------- | ---------------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke CallbackData constructor with valid pointer, id, and empty string         | validPointer = address of dummyValue, id = 456, name = ""          | Constructor does not throw and object fields match the input parameters (validPointer, 456, "") | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, NonNullPointerPositiveIdEmptyName) {
    std::cout << "Entering NonNullPointerPositiveIdEmptyName test" << std::endl;
    
    int dummyValue = 400;
    gpointer validPointer = static_cast<void*>(&dummyValue);
    gulong id = 456;
    std::string name = "";
    std::cout << "Invoking CallbackData constructor with instance: " << validPointer 
              << ", id: " << id 
              << ", name: [empty string]" << std::endl;
    
    EXPECT_NO_THROW({
        GstPlayerPriv::CallbackData obj(validPointer, id, name);
        std::cout << "Constructor invoked successfully." << std::endl;
        std::cout << "Internal state - instance: " << obj.instance 
                  << ", id: " << obj.id 
                  << ", name: \"" << obj.name << "\"" << std::endl;
        EXPECT_EQ(obj.instance, validPointer);
        EXPECT_EQ(obj.id, id);
        EXPECT_EQ(obj.name, name);
    });
    
    std::cout << "Exiting NonNullPointerPositiveIdEmptyName test" << std::endl;
}
/**
 * @brief Verify that the move assignment operator properly transfers the data from a valid non-null original CallbackData object to a destination CallbackData object.
 *
 * This test verifies that invoking the move assignment operator using a valid original object with non-null instance, non-zero id, and a non-empty name results in the destination object correctly acquiring the original object's data. It ensures that no exception is thrown and the post-move state of the destination object reflects the original object's state.
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
 * | 01 | Initialize destination CallbackData object with arbitrary valid non-null pointer, id and name | destInstance = 0x9999, destId = 0, destName = "InitialDest" | Destination object initialized with given data | Should be successful |
 * | 02 | Initialize original CallbackData object with valid non-null pointer, id 100 and name "TestName" | origInstance = 0x1, origId = 100, origName = "TestName" | Original object initialized with given data | Should be successful |
 * | 03 | Invoke move assignment operator to assign original object to destination | operator=(std::move(original)) | No exception thrown and move semantics applied | Should Pass |
 * | 04 | Validate that destination object holds original object's values | dest.instance = origInstance, dest.id = origId, dest.name = origName | Destination object updated correctly with original's values | Should be successful |
 */
TEST(GstPlayerPriv_CallbackData, MoveAssignmentValidNonNull) {
    std::cout << "Entering MoveAssignmentValidNonNull test" << std::endl;

    // Create destination CallbackData object with arbitrary initial values
    // Using arbitrary non-null pointer for destination instance
    void* destInstance = reinterpret_cast<void*>(0x9999);
    unsigned long destId = 0;
    std::string destName = "InitialDest";
    GstPlayerPriv::CallbackData dest(destInstance, destId, destName);
    std::cout << "Destination before move assignment: instance = " << dest.instance
              << ", id = " << dest.id << ", name = " << dest.name << std::endl;

    // Create original CallbackData object with valid non-null pointer, id 100 and name "TestName"
    void* origInstance = reinterpret_cast<void*>(0x1);
    unsigned long origId = 100;
    std::string origName = "TestName";
    GstPlayerPriv::CallbackData original(origInstance, origId, origName);
    std::cout << "Original before move assignment: instance = " << original.instance
              << ", id = " << original.id << ", name = " << original.name << std::endl;

    // Invoke move assignment operator
    std::cout << "Invoking operator=(std::move(original))" << std::endl;
    EXPECT_NO_THROW(dest = std::move(original));

    // Log the updated state of destination object
    std::cout << "Destination after move assignment: instance = " << dest.instance
              << ", id = " << dest.id << ", name = " << dest.name << std::endl;

    // Check expected values
    EXPECT_EQ(dest.instance, origInstance);
    EXPECT_EQ(dest.id, origId);
    EXPECT_EQ(dest.name, origName);

    std::cout << "Exiting MoveAssignmentValidNonNull test" << std::endl;
}
/**
 * @brief Test the move assignment operator for CallbackData when the original object has an empty name
 *
 * This test verifies that the move assignment operator for the CallbackData class correctly transfers
 * the instance pointer, id, and empty name from an original object to a destination object.
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
 * | 01 | Create destination CallbackData object with arbitrary initial values | destInstance = 0xAAAA, destId = 0, destName = "InitialDestEmpty" | Destination object is created successfully with the provided initial values | Should be successful |
 * | 02 | Create original CallbackData object with a valid non-null pointer, id 200 and an empty name | origInstance = 0x2, origId = 200, origName = "" | Original object is created successfully with the specified values | Should be successful |
 * | 03 | Invoke move assignment operator using std::move on original object | Operation: dest = std::move(original) | Move assignment operation completes without throwing; destination object's data reflects original object's values | Should Pass |
 * | 04 | Validate that the destination object's properties match the original object's properties | EXPECT_EQ(dest.instance, origInstance), EXPECT_EQ(dest.id, origId), EXPECT_EQ(dest.name, origName) | All assertions pass, confirming that move assignment transferred the correct values | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, MoveAssignmentEmptyName) {
    std::cout << "Entering MoveAssignmentEmptyName test" << std::endl;

    // Create destination CallbackData object with arbitrary initial values
    void* destInstance = reinterpret_cast<void*>(0xAAAA);
    unsigned long destId = 0;
    std::string destName = "InitialDestEmpty";
    GstPlayerPriv::CallbackData dest(destInstance, destId, destName);
    std::cout << "Destination before move assignment: instance = " << dest.instance
              << ", id = " << dest.id << ", name = " << dest.name << std::endl;

    // Create original CallbackData object with valid non-null pointer, id 200 and empty name
    void* origInstance = reinterpret_cast<void*>(0x2);
    unsigned long origId = 200;
    std::string origName = "";
    GstPlayerPriv::CallbackData original(origInstance, origId, origName);
    std::cout << "Original before move assignment: instance = " << original.instance
              << ", id = " << original.id << ", name = \"" << original.name << "\"" << std::endl;

    // Invoke move assignment operator
    std::cout << "Invoking operator=(std::move(original))" << std::endl;
    EXPECT_NO_THROW(dest = std::move(original));

    // Log the updated state of destination object
    std::cout << "Destination after move assignment: instance = " << dest.instance
              << ", id = " << dest.id << ", name = \"" << dest.name << "\"" << std::endl;

    // Check expected values
    EXPECT_EQ(dest.instance, origInstance);
    EXPECT_EQ(dest.id, origId);
    EXPECT_EQ(dest.name, origName);

    std::cout << "Exiting MoveAssignmentEmptyName test" << std::endl;
}
/**
 * @brief Tests move assignment operator with a null instance in the original object.
 *
 * Verifies that moving a CallbackData object with a null instance correctly transfers the null instance,
 * along with its id and name, to the destination object. Ensures that the object's state updates appropriately
 * without throwing exceptions.
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
 * | 01 | Create destination CallbackData object with non-null instance | destInstance = 0xBBBB, destId = 0, destName = "InitialDestNull" | Destination object initialized with provided non-null values | Should be successful |
 * | 02 | Create original CallbackData object with null instance | origInstance = nullptr, origId = 300, origName = "NullInstanceTest" | Original object initialized with null instance and provided id and name | Should be successful |
 * | 03 | Invoke move assignment operator to move original into destination | original: instance = nullptr, id = 300, name = "NullInstanceTest"; destination: pre-move state as above | Move assignment does not throw; destination object updated with original's values | Should Pass |
 * | 04 | Validate destination CallbackData object's values post move assignment | destination: instance = nullptr, id = 300, name = "NullInstanceTest" | Assertions verify that destination values match the original's expected values | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, MoveAssignmentNullInstance) {
    std::cout << "Entering MoveAssignmentNullInstance test" << std::endl;

    // Create destination CallbackData object with arbitrary initial values
    void* destInstance = reinterpret_cast<void*>(0xBBBB);
    unsigned long destId = 0;
    std::string destName = "InitialDestNull";
    GstPlayerPriv::CallbackData dest(destInstance, destId, destName);
    std::cout << "Destination before move assignment: instance = " << dest.instance
              << ", id = " << dest.id << ", name = " << dest.name << std::endl;

    // Create original CallbackData object with null instance, id 300 and name "NullInstanceTest"
    void* origInstance = nullptr;
    unsigned long origId = 300;
    std::string origName = "NullInstanceTest";
    GstPlayerPriv::CallbackData original(origInstance, origId, origName);
    std::cout << "Original before move assignment: instance = " 
              << (original.instance == nullptr ? "nullptr" : "non-null")
              << ", id = " << original.id << ", name = " << original.name << std::endl;

    // Invoke move assignment operator
    std::cout << "Invoking operator=(std::move(original))" << std::endl;
    EXPECT_NO_THROW(dest = std::move(original));

    // Log the updated state of destination object
    std::cout << "Destination after move assignment: instance = " 
              << (dest.instance == nullptr ? "nullptr" : "non-null")
              << ", id = " << dest.id << ", name = " << dest.name << std::endl;

    // Check expected values
    EXPECT_EQ(dest.instance, origInstance);
    EXPECT_EQ(dest.id, origId);
    EXPECT_EQ(dest.name, origName);

    std::cout << "Exiting MoveAssignmentNullInstance test" << std::endl;
}
/**
 * @brief Test the destruction of a default constructed CallbackData object
 *
 * This test verifies that a default constructed object of CallbackData is properly destroyed when it goes out of scope. It ensures that the object does not throw any exceptions during construction and destruction, and its internal state remains as expected for a default initialization.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                  | Expected Result                                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke default constructor for CallbackData, observe internal state and destruction process.     | No input arguments; output: instance not explicitly set, id uninitialized, name empty.                     | No exception thrown; object is correctly destroyed without errors.   | Should Pass |
 */
TEST(GstPlayerPriv_CallbackData, DestructionOfDefaultConstructedObject) {
    std::cout << "Entering DestructionOfDefaultConstructedObject test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor for CallbackData" << std::endl;
        {
            GstPlayerPriv::CallbackData obj;
            std::cout << "Created CallbackData object with default initialization" << std::endl;
            // Log internal state assuming default members: instance, id, name.
            std::cout << "Object internal state (default): instance is not set explicitly, id is uninitialized, name is empty (if default constructed)" << std::endl;
        }
        std::cout << "CallbackData object goes out of scope, triggering destructor" << std::endl;
    });
    
    std::cout << "Exiting DestructionOfDefaultConstructedObject test" << std::endl;
}
/**
 * @brief Validate that the default constructor of GstPlayerPriv does not throw exceptions
 *
 * This test verifies that a GstPlayerPriv instance can be created using its default constructor without any exceptions being thrown. It ensures that the constructor initializes the object properly, making it safe for subsequent operations.
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
 * | Variation / Step | Description                                                                                   | Test Data                                       | Expected Result                                                                                      | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------- | ---------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke default constructor to create GstPlayerPriv instance and verify no exceptions are thrown | input: none, output: GstPlayerPriv instance created | No exceptions are thrown; GstPlayerPriv instance is created successfully and assertion EXPECT_NO_THROW passes | Should Pass |
 */
TEST(GstPlayerPriv, GstPlayerPriv_Constructor_Start) {
    std::cout << "Entering GstPlayerPriv::GstPlayerPriv()_start test" << std::endl;

    EXPECT_NO_THROW({
        // Creating GstPlayerPriv object using default constructor
        GstPlayerPriv instance;
        std::cout << "Invoked default constructor: GstPlayerPriv instance created successfully." << std::endl;
    });

    std::cout << "Exiting GstPlayerPriv::GstPlayerPriv()_start test" << std::endl;
}
/**
 * @brief Test the destructor cleans up the GstPlayerPriv instance without throwing exceptions
 *
 * This test case verifies that the destructor of the GstPlayerPriv class safely cleans up an instance created 
 * using the default constructor without any exceptions. It ensures that deleting an instance does not result in 
 * any unexpected behavior.
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
 * | Variation / Step | Description                                           | Test Data                                               | Expected Result                                              | Notes        |
 * | :--------------: | ----------------------------------------------------- | ------------------------------------------------------- | ------------------------------------------------------------ | ------------ |
 * | 01               | Create an instance using the default constructor      | output1 = new GstPlayerPriv() (instance pointer created)| Instance is successfully created with a valid pointer         | Should Pass  |
 * | 02               | Delete the created instance to invoke the destructor    | input1 = instance pointer                               | Destructor completes without throwing an exception and cleans up the instance | Should Pass  |
 */
TEST(GstPlayerPriv, DestructorCleansUpInstance)
{
    std::cout << "Entering DestructorCleansUpInstance test" << std::endl;
    EXPECT_NO_THROW({
        // Create instance using default constructor
        GstPlayerPriv* instance = new GstPlayerPriv();
        std::cout << "Invoked GstPlayerPriv() constructor, created instance at " << instance << std::endl;
        
        // Invoke destructor by deleting the object
        delete instance;
        std::cout << "Destructor invoked safely; instance deleted." << std::endl;
    });
    std::cout << "Exiting DestructorCleansUpInstance test" << std::endl;
}
/**
 * @brief Validates that ForwardBuffersToAuxPipeline handles a valid dummy buffer pointer, false pause flag, and non-null user data without throwing exceptions.
 *
 * This test ensures that the InterfacePlayerPriv::ForwardBuffersToAuxPipeline function processes valid inputs correctly by not throwing any exceptions when a valid dummy GstBuffer pointer, a false pause flag, and a non-null user data pointer are provided.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** (High)@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                        | Test Data                                                                                         | Expected Result                                               | Notes      |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------- | ------------------------------------------------------------- | ---------- |
 * | 01               | Instantiate InterfacePlayerPriv and invoke ForwardBuffersToAuxPipeline with a valid dummy buffer, false pause flag, and non-null user data. | buffer = valid dummy GstBuffer pointer, pauseInjector = false, user_data = pointer to validData (12345) | No exception thrown and method executes successfully; assertion EXPECT_NO_THROW passes. | Should Pass |
 */
TEST(InterfacePlayerPriv, ValidBufferPauseFalseNonNullUserData) {
    std::cout << "Entering ValidBufferPauseFalseNonNullUserData test" << std::endl;

    // Create a valid dummy GstBuffer pointer (using new int as dummy storage)
    GstBuffer* dummyBuffer = reinterpret_cast<GstBuffer*>(new int(42));
    std::cout << "Created dummy GstBuffer pointer: " << dummyBuffer << std::endl;

    // Create a valid non-null user data pointer
    int validData = 12345;
    void* userData = static_cast<void*>(&validData);
    std::cout << "User data pointer is valid and points to value: " << validData << std::endl;

    // Instantiate object of InterfacePlayerPriv using default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Instantiated InterfacePlayerPriv object with default constructor." << std::endl;
        
        // Log method invocation and input values
        std::cout << "Invoking ForwardBuffersToAuxPipeline with parameters:" << std::endl;
        std::cout << "  buffer: " << dummyBuffer << std::endl;
        std::cout << "  pauseInjector: false" << std::endl;
        std::cout << "  user_data: " << userData << std::endl;
        
        EXPECT_NO_THROW(player.ForwardBuffersToAuxPipeline(dummyBuffer, false, userData));
        std::cout << "Method ForwardBuffersToAuxPipeline executed successfully." << std::endl;
    });

    // Clean up dummy buffer memory
    delete reinterpret_cast<int*>(dummyBuffer);
    std::cout << "Cleaned up dummy GstBuffer memory." << std::endl;
    
    std::cout << "Exiting ValidBufferPauseFalseNonNullUserData test" << std::endl;
}
/**
 * @brief Verifies that ForwardBuffersToAuxPipeline executes without throwing exceptions when provided with a valid GstBuffer, a true pause flag, and non-null user data.
 *
 * This test creates a dummy GstBuffer pointer by reinterpreting a dynamically allocated integer to simulate a valid memory buffer. It also sets up a valid non-null user data pointer. The InterfacePlayerPriv object is instantiated using the default constructor, and then the ForwardBuffersToAuxPipeline method is invoked with the provided parameters. The objective is to ensure that the method handles valid inputs correctly by executing without exceptions.
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
 * | 01 | Instantiate InterfacePlayerPriv and invoke ForwardBuffersToAuxPipeline with valid dummy buffer, pauseInjector set to true, and valid non-null user data. | dummyBuffer = valid GstBuffer (reinterpret_cast from new int(84)), pauseInjector = true, user_data = pointer to int (54321) | Method executes successfully without throwing any exceptions. | Should Pass |
 */
TEST(InterfacePlayerPriv, ValidBufferPauseTrueNonNullUserData) {
    std::cout << "Entering ValidBufferPauseTrueNonNullUserData test" << std::endl;

    // Create a valid dummy GstBuffer pointer (using new int as dummy storage)
    GstBuffer* dummyBuffer = reinterpret_cast<GstBuffer*>(new int(84));
    std::cout << "Created dummy GstBuffer pointer: " << dummyBuffer << std::endl;

    // Create a valid non-null user data pointer
    int validData = 54321;
    void* userData = static_cast<void*>(&validData);
    std::cout << "User data pointer is valid and points to value: " << validData << std::endl;

    // Instantiate object of InterfacePlayerPriv using default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Instantiated InterfacePlayerPriv object with default constructor." << std::endl;

        // Log method invocation and input values
        std::cout << "Invoking ForwardBuffersToAuxPipeline with parameters:" << std::endl;
        std::cout << "  buffer: " << dummyBuffer << std::endl;
        std::cout << "  pauseInjector: true" << std::endl;
        std::cout << "  user_data: " << userData << std::endl;

        EXPECT_NO_THROW(player.ForwardBuffersToAuxPipeline(dummyBuffer, true, userData));
        std::cout << "Method ForwardBuffersToAuxPipeline executed successfully." << std::endl;
    });

    // Clean up dummy buffer memory
    delete reinterpret_cast<int*>(dummyBuffer);
    std::cout << "Cleaned up dummy GstBuffer memory." << std::endl;

    std::cout << "Exiting ValidBufferPauseTrueNonNullUserData test" << std::endl;
}
/**
 * @brief Validate the execution of ForwardBuffersToAuxPipeline with a valid buffer, pause flag false, and null user data
 *
 * This test verifies that the InterfacePlayerPriv::ForwardBuffersToAuxPipeline API functions correctly when a valid GstBuffer pointer is passed,
 * along with a pauseInjector flag set to false and a null user data pointer. The test ensures that no exceptions are thrown during execution
 * and that the method behaves as expected with these inputs.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                       | Expected Result                                                              | Notes          |@n
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ---------------------------------------------------------------------------- | -------------- |@n
 * | 01               | Create a dummy GstBuffer pointer using new int(21)                                           | dummyBuffer = valid non-null GstBuffer pointer                  | Dummy GstBuffer pointer is successfully created                              | Should be successful |@n
 * | 02               | Set the user data pointer to null                                                            | userData = nullptr                                                | userData is set to null                                                       | Should be successful |@n
 * | 03               | Instantiate an object of InterfacePlayerPriv using the default constructor                   | player instantiated using default constructor                   | Object is instantiated without throwing exceptions                          | Should be successful |@n
 * | 04               | Invoke ForwardBuffersToAuxPipeline with the dummy buffer, pauseInjector as false, and null user data | buffer = dummyBuffer, pauseInjector = false, user_data = nullptr    | Method executes without throwing exceptions and behaves as expected (passes assertion) | Should Pass    |@n
 * | 05               | Clean up the allocated dummy GstBuffer memory by deletion                                    | deletion of dummyBuffer (delete reinterpret_cast<int*>(dummyBuffer)) | Memory is released without issues                                             | Should be successful |
 */
TEST(InterfacePlayerPriv, ValidBufferPauseFalseNullUserData) {
    std::cout << "Entering ValidBufferPauseFalseNullUserData test" << std::endl;

    // Create a valid dummy GstBuffer pointer (using new int as dummy storage)
    GstBuffer* dummyBuffer = reinterpret_cast<GstBuffer*>(new int(21));
    std::cout << "Created dummy GstBuffer pointer: " << dummyBuffer << std::endl;

    // Set user_data as null
    void* userData = nullptr;
    std::cout << "User data pointer is null." << std::endl;

    // Instantiate object of InterfacePlayerPriv using default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Instantiated InterfacePlayerPriv object with default constructor." << std::endl;

        // Log method invocation and input values
        std::cout << "Invoking ForwardBuffersToAuxPipeline with parameters:" << std::endl;
        std::cout << "  buffer: " << dummyBuffer << std::endl;
        std::cout << "  pauseInjector: false" << std::endl;
        std::cout << "  user_data: " << userData << std::endl;

        EXPECT_NO_THROW(player.ForwardBuffersToAuxPipeline(dummyBuffer, false, userData));
        std::cout << "Method ForwardBuffersToAuxPipeline executed successfully." << std::endl;
    });

    // Clean up dummy buffer memory
    delete reinterpret_cast<int*>(dummyBuffer);
    std::cout << "Cleaned up dummy GstBuffer memory." << std::endl;

    std::cout << "Exiting ValidBufferPauseFalseNullUserData test" << std::endl;
}
/**
 * @brief Validate that ForwardBuffersToAuxPipeline executes correctly with valid inputs.
 *
 * This test verifies that the InterfacePlayerPriv's ForwardBuffersToAuxPipeline method properly handles a valid GstBuffer pointer, a true pauseInjector flag, and a null user_data pointer. It ensures that no exceptions are thrown during the method invocation, confirming that the method interacts correctly with its parameters under these conditions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a dummy GstBuffer, set user_data to null, instantiate InterfacePlayerPriv and invoke ForwardBuffersToAuxPipeline with the dummy buffer, pauseInjector = true, and null user_data | dummyBuffer = valid GstBuffer pointer (using new int(168)), pauseInjector = true, userData = nullptr | The API call executes without throwing any exception, confirming successful execution with valid inputs | Should Pass |
 */
TEST(InterfacePlayerPriv, ValidBufferPauseTrueNullUserData) {
    std::cout << "Entering ValidBufferPauseTrueNullUserData test" << std::endl;

    // Create a valid dummy GstBuffer pointer (using new int as dummy storage)
    GstBuffer* dummyBuffer = reinterpret_cast<GstBuffer*>(new int(168));
    std::cout << "Created dummy GstBuffer pointer: " << dummyBuffer << std::endl;

    // Set user_data as null
    void* userData = nullptr;
    std::cout << "User data pointer is null." << std::endl;

    // Instantiate object of InterfacePlayerPriv using default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Instantiated InterfacePlayerPriv object with default constructor." << std::endl;
        
        // Log method invocation and input values
        std::cout << "Invoking ForwardBuffersToAuxPipeline with parameters:" << std::endl;
        std::cout << "  buffer: " << dummyBuffer << std::endl;
        std::cout << "  pauseInjector: true" << std::endl;
        std::cout << "  user_data: " << userData << std::endl;
        
        EXPECT_NO_THROW(player.ForwardBuffersToAuxPipeline(dummyBuffer, true, userData));
        std::cout << "Method ForwardBuffersToAuxPipeline executed successfully." << std::endl;
    });

    // Clean up dummy buffer memory
    delete reinterpret_cast<int*>(dummyBuffer);
    std::cout << "Cleaned up dummy GstBuffer memory." << std::endl;

    std::cout << "Exiting ValidBufferPauseTrueNullUserData test" << std::endl;
}
/**
 * @brief Test ForwardBuffersToAuxPipeline with a null GstBuffer and valid user data.
 *
 * This test verifies that the API, when invoked with a null GstBuffer pointer, a false pauseInjector flag, and a valid non-null user data pointer, does not throw an exception. It ensures that the method handles the null buffer scenario gracefully while still operating correctly with valid user data.
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
 * | Variation / Step | Description                                                                           | Test Data                                                                                          | Expected Result                                                                                   | Notes           |
 * | :--------------: | ------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Instantiate an InterfacePlayerPriv object using the default constructor.              | None                                                                                               | Object is created successfully without throwing an exception.                                   | Should be successful |
 * | 02               | Invoke ForwardBuffersToAuxPipeline with parameters: dummyBuffer = nullptr, pauseInjector = false, user_data points to validData (98765). | input: dummyBuffer = nullptr, pauseInjector = false, user_data = validData (98765), output: void | Method executes successfully without throwing any exception. Assertion check via EXPECT_NO_THROW. | Should Pass     |
 */
TEST(InterfacePlayerPriv, NullBufferPauseFalseNonNullUserData) {
    std::cout << "Entering NullBufferPauseFalseNonNullUserData test" << std::endl;

    // Set GstBuffer pointer as null
    GstBuffer* dummyBuffer = nullptr;
    std::cout << "GstBuffer pointer is null." << std::endl;

    // Create a valid non-null user data pointer
    int validData = 98765;
    void* userData = static_cast<void*>(&validData);
    std::cout << "User data pointer is valid and points to value: " << validData << std::endl;

    // Instantiate object of InterfacePlayerPriv using default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Instantiated InterfacePlayerPriv object with default constructor." << std::endl;

        // Log method invocation and input values
        std::cout << "Invoking ForwardBuffersToAuxPipeline with parameters:" << std::endl;
        std::cout << "  buffer: " << dummyBuffer << " (null)" << std::endl;
        std::cout << "  pauseInjector: false" << std::endl;
        std::cout << "  user_data: " << userData << std::endl;

        EXPECT_NO_THROW(player.ForwardBuffersToAuxPipeline(dummyBuffer, false, userData));
        std::cout << "Method ForwardBuffersToAuxPipeline executed successfully without crashing." << std::endl;
    });

    std::cout << "Exiting NullBufferPauseFalseNonNullUserData test" << std::endl;
}
/**
 * @brief Test to verify the successful instantiation of InterfacePlayerPriv using its default constructor
 *
 * This test ensures that invoking the non-parameterized constructor of InterfacePlayerPriv does not throw any exceptions.
 * It verifies the basic object creation functionality and confirms that the constructor initializes the object to a valid state.
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
 * | Variation / Step | Description                                                                           | Test Data                                             | Expected Result                 | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------- | ----------- |
 * | 01               | Invoke the non-parameterized constructor of InterfacePlayerPriv and check for exceptions| input = none, output = InterfacePlayerPriv instance     | No exception is thrown by the constructor | Should Pass |
 */
TEST(InterfacePlayerPriv, InterfacePlayerPriv__start)
{
    std::cout << "Entering InterfacePlayerPriv::InterfacePlayerPriv()_start test" << std::endl;
    
    // Invoke the non-parameterized constructor and ensure no exception is thrown
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Invoked InterfacePlayerPriv() constructor." << std::endl;
        
    });
    
    std::cout << "Exiting InterfacePlayerPriv::InterfacePlayerPriv()_start test" << std::endl;
}
/**
 * @brief Verify that the SendGstEvents API successfully processes valid input parameters without throwing exceptions
 *
 * This test validates the behavior of the InterfacePlayerPriv class when supplied with a set of valid input arguments.
 * It creates an object using the default constructor and invokes the SendGstEvents method with valid parameters.
 * The test is designed to ensure the API call does not throw any exceptions and functions as expected.
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
 * | Variation / Step | Description                                                            | Test Data                                                                                                      | Expected Result                                                | Notes        |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------- | ------------ |
 * | 01               | Construct InterfacePlayerPriv instance using default constructor       | No input parameters                                                                                            | Object is constructed without throwing exceptions              | Should be successful |
 * | 02               | Invoke SendGstEvents with valid parameters                             | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30                | Method call does not throw any exception                         | Should Pass  |
 */
TEST(InterfacePlayerPriv, PositiveValidInput) {
    std::cout << "Entering PositiveValidInput test" << std::endl;
    // Create an object of InterfacePlayerPriv using its default constructor
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Define parameters for valid input
        int mediaType = 1;
        guint64 pts = 100000;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        // Invoke method and expect no throw
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents invoked successfully." << std::endl;
    });
    std::cout << "Exiting PositiveValidInput test" << std::endl;
}
/**
 * @brief Validate that SendGstEvents handles an invalid media type gracefully.
 *
 * This test constructs an instance of InterfacePlayerPriv and invokes the SendGstEvents method with an invalid media type (-1). The test verifies that the method handles invalid input without throwing exceptions, ensuring robust error handling.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 026@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                    | Test Data                                                                                          | Expected Result                                                        | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------- |
 * | 01               | Construct an InterfacePlayerPriv object                                        | N/A                                                                                                | Object is constructed successfully                                     | Should be successful |
 * | 02               | Invoke SendGstEvents with invalid media type and valid parameters              | mediaType = -1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30 | SendGstEvents should process invalid mediaType without throwing exception | Should Pass      |
 */
TEST(InterfacePlayerPriv, InvalidMediaType) {
    std::cout << "Entering InvalidMediaType test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with an invalid mediaType
        int mediaType = -1;
        guint64 pts = 100000;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with invalid mediaType:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents handled invalid mediaType gracefully." << std::endl;
    });
    std::cout << "Exiting InvalidMediaType test" << std::endl;
}
/**
 * @brief Test that verifies the InterfacePlayerPriv's SendGstEvents function processes a pts value of zero without throwing exceptions.
 *
 * This test verifies that when the pts parameter is set to zero, the SendGstEvents method is invoked properly and completes its execution without errors. The test validates that the API can handle a pts value of 0, ensuring robustness in the case of boundary values.
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
 * | Variation / Step | Description                                                          | Test Data                                                                                   | Expected Result                                                                 | Notes           |
 * | :--------------: | -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------- | --------------- |
 * | 01               | Construct the InterfacePlayerPriv object.                            | No input parameters.                                                                        | Object is successfully constructed without throwing exceptions.               | Should be successful |
 * | 02               | Set parameters with pts set to zero.                                 | mediaType = 1, pts = 0, enableGstPosQuery = 1, enablePTSReStamp = false, vodTrickModeFPS = 30 | Parameters are set as specified with pts equal to zero.                         | Should be successful |
 * | 03               | Invoke SendGstEvents with the given parameters.                      | mediaType = 1, pts = 0, enableGstPosQuery = 1, enablePTSReStamp = false, vodTrickModeFPS = 30 | API call completes without throwing an exception and processes pts=0 correctly. | Should Pass     |
 */
TEST(InterfacePlayerPriv, ZeroPts) {
    std::cout << "Entering ZeroPts test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with pts as zero
        int mediaType = 1;
        guint64 pts = 0;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = false;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with pts set to zero:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents processed pts=0 without errors." << std::endl;
    });
    std::cout << "Exiting ZeroPts test" << std::endl;
}
/**
 * @brief Validate SendGstEvents API with maximum presentation timestamp value to ensure no overflow.
 *
 * This test verifies that when SendGstEvents is called with pts set to UINT64_MAX, the API does not throw any exceptions.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct InterfacePlayerPriv object and initialize test parameters with pts as UINT64_MAX | mediaType = 1, pts = UINT64_MAX, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30 | Object is successfully constructed and parameters are set correctly | Should be successful |
 * | 02 | Invoke SendGstEvents API with maximum pts value | mediaType = 1, pts = UINT64_MAX, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30 | API call completes without throwing exceptions | Should Pass |
 */
TEST(InterfacePlayerPriv, MaxPts) {
    std::cout << "Entering MaxPts test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with pts set to maximum (UINT64_MAX)
        int mediaType = 1;
        guint64 pts = std::numeric_limits<guint64>::max();
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with maximum pts:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents processed maximum pts without overflow errors." << std::endl;
    });
    std::cout << "Exiting MaxPts test" << std::endl;
}
/**
 * @brief Test to validate that SendGstEvents handles an invalid negative enableGstPosQuery value gracefully
 *
 * This test constructs an instance of InterfacePlayerPriv and sets the parameters for the SendGstEvents method with
 * a negative enableGstPosQuery value. The test ensures that even with an invalid parameter, the method does not throw
 * any exceptions and handles the input gracefully.
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
 * | Variation / Step | Description                                                     | Test Data                                                                                                   | Expected Result                                              | Notes            |
 * | :--------------: | --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------- |
 * | 01               | Construct InterfacePlayerPriv object                            | None                                                                                                        | Object constructed without exceptions                      | Should be successful |
 * | 02               | Invoke SendGstEvents with a negative enableGstPosQuery parameter    | mediaType = 1, pts = 100000, enableGstPosQuery = -1, enablePTSReStamp = true, vodTrickModeFPS = 30           | SendGstEvents invoked without throwing exceptions            | Should Pass      |
 */
TEST(InterfacePlayerPriv, InvalidEnableGstPosQuery) {
    std::cout << "Entering InvalidEnableGstPosQuery test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with a negative enableGstPosQuery value
        int mediaType = 1;
        guint64 pts = 100000;
        int enableGstPosQuery = -1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with invalid enableGstPosQuery:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents handled invalid enableGstPosQuery gracefully." << std::endl;
    });
    std::cout << "Exiting InvalidEnableGstPosQuery test" << std::endl;
}
/**
 * @brief Tests negative vodTrickModeFPS input in SendGstEvents API to ensure graceful handling of invalid parameters.
 *
 * This test verifies that the SendGstEvents method in the InterfacePlayerPriv class handles a negative value for vodTrickModeFPS appropriately by not throwing exceptions when invoked.
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
 * | 01 | Construct the InterfacePlayerPriv object | None | Object is created successfully | Should be successful |
 * | 02 | Set parameters for SendGstEvents including negative vodTrickModeFPS | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = -30 | Parameters are set correctly | Should be successful |
 * | 03 | Invoke SendGstEvents with the specified parameters | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = -30 | API call does not throw any exceptions | Should Pass |
 */
TEST(InterfacePlayerPriv, NegativeVodTrickModeFPS) {
    std::cout << "Entering NegativeVodTrickModeFPS test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with negative vodTrickModeFPS
        int mediaType = 1;
        guint64 pts = 100000;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = -30;
        
        std::cout << "Invoking SendGstEvents with negative vodTrickModeFPS:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents handled negative vodTrickModeFPS appropriately." << std::endl;
    });
    std::cout << "Exiting NegativeVodTrickModeFPS test" << std::endl;
}
/**
 * @brief Validate that SendGstEvents processes event without exception when enablePTSReStamp is true
 *
 * This test verifies that the InterfacePlayerPriv API function SendGstEvents correctly processes events when the enablePTSReStamp flag is set to true. It ensures that no exceptions are thrown during the invocation, indicating proper handling of PTS re-stamping.
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
 * | 01 | Construct the InterfacePlayerPriv object. | None | Object is constructed successfully. | Should be successful |
 * | 02 | Initialize parameters: mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS. | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30 | Parameters are set correctly. | Should be successful |
 * | 03 | Invoke SendGstEvents API with enablePTSReStamp true. | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = true, vodTrickModeFPS = 30 | No exception is thrown; API processes event correctly. | Should Pass |
 */
TEST(InterfacePlayerPriv, EnablePTSReStampTrue) {
    std::cout << "Entering EnablePTSReStampTrue test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with enablePTSReStamp true
        int mediaType = 1;
        guint64 pts = 100000;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with enablePTSReStamp = true:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents processed event correctly when enablePTSReStamp is true." << std::endl;
    });
    std::cout << "Exiting EnablePTSReStampTrue test" << std::endl;
}
/**
 * @brief Tests the SendGstEvents function when enablePTSReStamp is false
 *
 * This test verifies that invoking the SendGstEvents method with the enablePTSReStamp parameter set to false results in the event being processed without throwing an exception. It ensures that the InterfacePlayerPriv object correctly handles the API call under these conditions.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                   | Expected Result                                                                  | Notes        |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------- | ------------ |
 * | 01               | Construct InterfacePlayerPriv object and invoke SendGstEvents with enablePTSReStamp set to false.     | mediaType = 1, pts = 100000, enableGstPosQuery = 1, enablePTSReStamp = false, vodTrickModeFPS = 30           | No exception thrown; SendGstEvents processes the event correctly and as expected | Should Pass  |
 */
TEST(InterfacePlayerPriv, EnablePTSReStampFalse) {
    std::cout << "Entering EnablePTSReStampFalse test" << std::endl;
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Constructed InterfacePlayerPriv object." << std::endl;
        
        // Set parameters with enablePTSReStamp false
        int mediaType = 1;
        guint64 pts = 100000;
        int enableGstPosQuery = 1;
        bool enablePTSReStamp = false;
        int vodTrickModeFPS = 30;
        
        std::cout << "Invoking SendGstEvents with enablePTSReStamp = false:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enableGstPosQuery: " << enableGstPosQuery << std::endl;
        std::cout << " enablePTSReStamp: " << std::boolalpha << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        
        EXPECT_NO_THROW(player.SendGstEvents(mediaType, pts, enableGstPosQuery, enablePTSReStamp, vodTrickModeFPS));
        std::cout << "SendGstEvents processed event correctly when enablePTSReStamp is false." << std::endl;
    });
    std::cout << "Exiting EnablePTSReStampFalse test" << std::endl;
}
/**
 * @brief Verify that SendNewSegmentEvent successfully handles valid segment events for supported media types
 *
 * This test verifies that the InterfacePlayerPriv object can be created successfully and that it correctly processes segment events for valid media types (eSUB_TYPE_WEBVTT and eSUB_TYPE_MP4) without throwing exceptions.
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
 * | 01 | Create instance of InterfacePlayerPriv using default constructor | N/A | Instance is created without throwing exceptions | Should be successful |
 * | 02 | Invoke SendNewSegmentEvent for each valid media type with startPts = 1000 and stopPts = 2000 | For first iteration: mediaType = eSUB_TYPE_WEBVTT, startPts = 1000, stopPts = 2000; For second iteration: mediaType = eSUB_TYPE_MP4, startPts = 1000, stopPts = 2000 | API executes without throwing exceptions for each valid media type | Should Pass |
 */
TEST(InterfacePlayerPriv, PositiveValidSegmentEvent)
{
    std::cout << "Entering PositiveValidSegmentEvent test" << std::endl;

    // Create an object of InterfacePlayerPriv using default constructor.
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        // Loop through valid media types
        int validMediaTypes[2] = { eSUB_TYPE_WEBVTT, eSUB_TYPE_MP4 };
        for (int i = 0; i < 2; ++i)
        {
            int mediaType = validMediaTypes[i];
            GstClockTime startPts = 1000;
            GstClockTime stopPts  = 2000;
            std::cout << "Invoking SendNewSegmentEvent with mediaType: " << mediaType
                      << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
            EXPECT_NO_THROW({
                player.SendNewSegmentEvent(mediaType, startPts, stopPts);
            });
            std::cout << "Sent new segment event successfully for mediaType: " << mediaType << std::endl;
        }
    });

    std::cout << "Exiting PositiveValidSegmentEvent test" << std::endl;
}
/**
 * @brief Verify that a zero-duration segment event is processed without throwing exceptions.
 *
 * This test verifies that the SendNewSegmentEvent function can handle a scenario where the segment's start and stop PTS are identical, i.e., a zero-duration segment event, without throwing any exceptions.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                  | Expected Result                                     | Notes              |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | --------------------------------------------------- | ------------------ |
 * | 01               | Create an instance of InterfacePlayerPriv and initialize test variables                          | None                                                       | Object instance is created successfully             | Should be successful |
 * | 02               | Invoke SendNewSegmentEvent with mediaType = eSUB_TYPE_WEBVTT, startPts = 1000, stopPts = 1000       | mediaType = eSUB_TYPE_WEBVTT, startPts = 1000, stopPts = 1000 | API call does not throw an exception                | Should Pass        |
 * | 03               | Log the successful handling of the zero-duration segment event                                  | None                                                       | Log message confirms proper handling of event         | Should be successful |
 */
TEST(InterfacePlayerPriv, ZeroDurationSegmentEvent)
{
    std::cout << "Entering ZeroDurationSegmentEvent test" << std::endl;

    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        int mediaType = eSUB_TYPE_WEBVTT;
        GstClockTime startPts = 1000;
        GstClockTime stopPts  = 1000;
        std::cout << "Invoking SendNewSegmentEvent with mediaType: " << mediaType
                  << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
        EXPECT_NO_THROW({
            player.SendNewSegmentEvent(mediaType, startPts, stopPts);
        });
        std::cout << "Handled zero-duration segment event with startPts == stopPts" << std::endl;
    });

    std::cout << "Exiting ZeroDurationSegmentEvent test" << std::endl;
}
/**
 * @brief Test that the API handles startPts greater than stopPts without crashing.
 *
 * Tests the condition where the startPts value is higher than the stopPts value. This test verifies that the SendNewSegmentEvent method handles this edge case gracefully by not throwing exceptions, thereby ensuring robustness against invalid timestamp ordering.
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
 * | Variation / Step | Description                                                         | Test Data                                                | Expected Result                                   | Notes             |
 * | :--------------: | ------------------------------------------------------------------- | -------------------------------------------------------- | -------------------------------------------------- | ----------------- |
 * | 01               | Initialize InterfacePlayerPriv object and prepare input values.     | player instance, mediaType = eSUB_TYPE_MP4, startPts = 3000, stopPts = 2000 | Object initialization successful.               | Should be successful |
 * | 02               | Invoke SendNewSegmentEvent with startPts > stopPts.                  | mediaType = eSUB_TYPE_MP4, startPts = 3000, stopPts = 2000 | API executes without throwing exceptions.       | Should Pass       |
 */
TEST(InterfacePlayerPriv, StartPtsGreaterThanStopPts)
{
    std::cout << "Entering StartPtsGreaterThanStopPts test" << std::endl;

    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        int mediaType = eSUB_TYPE_MP4;
        GstClockTime startPts = 3000;
        GstClockTime stopPts  = 2000;
        std::cout << "Invoking SendNewSegmentEvent with mediaType: " << mediaType
                  << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
        EXPECT_NO_THROW({
            player.SendNewSegmentEvent(mediaType, startPts, stopPts);
        });
        std::cout << "Method handled startPts greater than stopPts without crashing" << std::endl;
    });

    std::cout << "Exiting StartPtsGreaterThanStopPts test" << std::endl;
}
/**
 * @brief Validate that SendNewSegmentEvent properly handles an invalid media type input.
 *
 * This test verifies that InterfacePlayerPriv::SendNewSegmentEvent does not throw an exception when invoked with an invalid media type (-1), ensuring graceful error handling.
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
 * | Variation / Step | Description                                                                                             | Test Data                                | Expected Result                                                 | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------- | ---------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate InterfacePlayerPriv, set mediaType to -1 with startPts = 1000 and stopPts = 2000, then invoke SendNewSegmentEvent | mediaType = -1, startPts = 1000, stopPts = 2000 | API call completes without throwing an exception and assertion passes | Should Pass |
 */
TEST(InterfacePlayerPriv, InvalidMediaType)
{
    std::cout << "Entering InvalidMediaType test" << std::endl;

    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        int mediaType = -1;
        GstClockTime startPts = 1000;
        GstClockTime stopPts  = 2000;
        std::cout << "Invoking SendNewSegmentEvent with invalid mediaType: " << mediaType
                  << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
        EXPECT_NO_THROW({
            player.SendNewSegmentEvent(mediaType, startPts, stopPts);
        });
        std::cout << "Method handled invalid media type (-1) appropriately" << std::endl;
    });

    std::cout << "Exiting InvalidMediaType test" << std::endl;
}
/**
 * @brief Verify that SendNewSegmentEvent handles maximum stopPts boundary condition without throwing an exception
 *
 * This test validates that the InterfacePlayerPriv::SendNewSegmentEvent method correctly processes a segment event when provided with the maximum possible value of GstClockTime for the stopPts parameter. The test confirms that no exception is thrown during the operation.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                                              | Expected Result                              | Notes      |
 * | :----:           | ----------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------- | -------------------------------------------- | ---------- |
 * | 01               | Create an instance of InterfacePlayerPriv and invoke SendNewSegmentEvent with maximum stopPts value   | mediaType = eSUB_TYPE_WEBVTT, startPts = 0, stopPts = std::numeric_limits<GstClockTime>::max() | No exception is thrown during the API invocation | Should Pass |
 */
TEST(InterfacePlayerPriv, BoundaryConditionMaxStopPts)
{
    std::cout << "Entering BoundaryConditionMaxStopPts test" << std::endl;

    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        int mediaType = eSUB_TYPE_WEBVTT;
        GstClockTime startPts = 0;
        // Maximum value for GstClockTime (unsigned 64-bit integer)
        GstClockTime stopPts = std::numeric_limits<GstClockTime>::max();
        std::cout << "Invoking SendNewSegmentEvent with mediaType: " << mediaType
                  << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
        EXPECT_NO_THROW({
            player.SendNewSegmentEvent(mediaType, startPts, stopPts);
        });
        std::cout << "Sent segment event with maximum GstClockTime stopPts: " << stopPts << std::endl;
    });

    std::cout << "Exiting BoundaryConditionMaxStopPts test" << std::endl;
}
/**
 * @brief Verify that SendNewSegmentEvent correctly handles a zero startPts and a valid stopPts.
 *
 * This test ensures that invoking the SendNewSegmentEvent API with a startPts value of 0 and a valid stopPts value
 * (2000) does not throw any exceptions. The test confirms that the InterfacePlayerPriv instance correctly processes
 * the event with these specific input parameters.
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
 * | Variation / Step | Description                                                          | Test Data                                            | Expected Result                                                                           | Notes             |
 * | :--------------: | -------------------------------------------------------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------------------------- | ----------------- |
 * |       01         | Initialize the InterfacePlayerPriv object                            | N/A                                                  | Object should be created successfully without throwing exceptions                        | Should be successful |
 * |       02         | Define test parameters: mediaType, startPts, and stopPts               | mediaType = eSUB_TYPE_MP4, startPts = 0, stopPts = 2000 | Test variables are set correctly                                                         | Should be successful |
 * |       03         | Invoke SendNewSegmentEvent with the defined parameters                 | mediaType = eSUB_TYPE_MP4, startPts = 0, stopPts = 2000 | API call should execute without throwing exceptions                                       | Should Pass       |
 */
TEST(InterfacePlayerPriv, ZeroStartPtsValidStopPts)
{
    std::cout << "Entering ZeroStartPtsValidStopPts test" << std::endl;

    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        int mediaType = eSUB_TYPE_MP4;
        GstClockTime startPts = 0;
        GstClockTime stopPts  = 2000;
        std::cout << "Invoking SendNewSegmentEvent with mediaType: " << mediaType
                  << ", startPts: " << startPts << ", stopPts: " << stopPts << std::endl;
        EXPECT_NO_THROW({
            player.SendNewSegmentEvent(mediaType, startPts, stopPts);
        });
        std::cout << "Sent segment event with startPts = 0 and valid stopPts" << std::endl;
    });

    std::cout << "Exiting ZeroStartPtsValidStopPts test" << std::endl;
}
/**
 * @brief Verify constructor and SendQtDemuxOverrideEvent behavior using valid default pointer parameters
 *
 * This test confirms that an object of InterfacePlayerPriv can be successfully created using its default constructor and that the SendQtDemuxOverrideEvent method, when invoked with valid parameters including a null pointer and zero length, returns the expected TRUE value (non-zero). This ensures the API handles default pointer values properly.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                                                                   | Expected Result                                                                      | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------ | ---------- |
 * | 01               | Create an instance of InterfacePlayerPriv using the default constructor and invoke SendQtDemuxOverrideEvent with valid parameters including a null pointer for buffer. | mediaType = 1, pts = 1000, enablePTSReStamp = true, vodTrickModeFPS = 30, ptr = nullptr, len = 0, ret = TRUE | The function does not throw, and SendQtDemuxOverrideEvent returns TRUE (1) as expected. | Should Pass |
 */
TEST(InterfacePlayerPriv, ValidParametersDefaultPointer) {
    std::cout << "Entering ValidParametersDefaultPointer test" << std::endl;
    
    // Create an object using default constructor, expecting no throw.
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 1000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        const void* ptr = nullptr;
        size_t len = 0;
        
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << ptr << std::endl;
        std::cout << " len: " << len << std::endl;
        
        // Invoke the method and capture the return value
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, ptr, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        // Verify expected output TRUE (non-zero)
        EXPECT_EQ(1, ret);
    });
    
    std::cout << "Exiting ValidParametersDefaultPointer test" << std::endl;
}
/**
 * @brief Verify that SendQtDemuxOverrideEvent returns success when invoked with valid parameters and a non-null buffer.
 *
 * This test passes valid parameters to SendQtDemuxOverrideEvent including a valid media type, timestamp, trick mode FPS, and a properly initialized non-null buffer. The API call is expected to complete without throwing any exceptions and return a success indicator (1), which is then verified via an assertion. This ensures that the API behaves as expected with proper inputs.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create InterfacePlayerPriv object, prepare a valid non-null buffer and associated parameters, then invoke SendQtDemuxOverrideEvent | mediaType = 1, pts = 5000, enablePTSReStamp = false, vodTrickModeFPS = 24, buffer = "Sample buffer data for testing.", len = 128, output ret = 1 | API returns success (1) without throwing exceptions; assertion EXPECT_EQ(1, ret) passes | Should Pass |
 */
TEST(InterfacePlayerPriv, ValidParametersWithBuffer) {
    std::cout << "Entering ValidParametersWithBuffer test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 5000;
        bool enablePTSReStamp = false;
        int vodTrickModeFPS = 24;
        
        // Prepare a valid non-null buffer
        char buffer[256];
        const char* sampleStr = "Sample buffer data for testing.";
        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, sampleStr, sizeof(buffer)-1);
        size_t len = 128; // As per test plan, use 128 as length
        
        std::cout << "Prepared buffer with content: " << buffer << std::endl;
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << static_cast<const void*>(buffer) << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, buffer, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(1, ret);
    });
    
    std::cout << "Exiting ValidParametersWithBuffer test" << std::endl;
}
/**
 * @brief Verify that the SendQtDemuxOverrideEvent API handles an invalid media type correctly.
 *
 * This test verifies that when an invalid media type (i.e., -1) is passed along with valid parameters to the 
 * SendQtDemuxOverrideEvent method of the InterfacePlayerPriv class, the function returns 0 as expected. 
 * This ensures that the API can gracefully handle invalid input for the media type.
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
 * | Variation / Step | Description                                                                    | Test Data                                                                                                 | Expected Result                              | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------- | -------------------------------------------- | ------------- |
 * | 01               | Create an instance of InterfacePlayerPriv using the default constructor.       | None                                                                                                      | Object is created without errors.            | Should be successful |
 * | 02               | Set up parameters with an invalid media type and valid other values.           | mediaType = -1, pts = 8000, enablePTSReStamp = true, vodTrickModeFPS = 30, ptr = nullptr, len = 0            | Parameters are initialized as expected.      | Should be successful |
 * | 03               | Invoke the SendQtDemuxOverrideEvent API and validate the return value.           | mediaType = -1, pts = 8000, enablePTSReStamp = true, vodTrickModeFPS = 30, ptr = nullptr, len = 0            | API returns 0 as expected.                     | Should Pass   |
 */
TEST(InterfacePlayerPriv, InvalidMediaType) {
    std::cout << "Entering InvalidMediaType test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = -1;
        GstClockTime pts = 8000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        const void* ptr = nullptr;
        size_t len = 0;
        
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << ptr << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, ptr, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(0, ret);
    });
    
    std::cout << "Exiting InvalidMediaType test" << std::endl;
}
/**
 * @brief Validate handling of invalid vodTrickModeFPS in SendQtDemuxOverrideEvent API
 *
 * This test verifies that when an invalid value (-10) is provided for vodTrickModeFPS, the
 * SendQtDemuxOverrideEvent API call does not throw an exception and returns 0 as expected.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                   | Expected Result                                          | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ---------- |
 * | 01               | Create InterfacePlayerPriv object and set parameters with an invalid vodTrickModeFPS value, then invoke the API. | mediaType = 1, pts = 2000, enablePTSReStamp = true, vodTrickModeFPS = -10, ptr = nullptr, len = 0          | API returns 0 and the EXPECT_EQ(0, ret) assertion is met   | Should Pass|
 */
TEST(InterfacePlayerPriv, InvalidVodTrickModeFPS) {
    std::cout << "Entering InvalidVodTrickModeFPS test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 2000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = -10;
        const void* ptr = nullptr;
        size_t len = 0;
        
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << ptr << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, ptr, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(0, ret);
    });
    
    std::cout << "Exiting InvalidVodTrickModeFPS test" << std::endl;
}
/**
 * @brief Verify that SendQtDemuxOverrideEvent handles a null buffer pointer with a non-zero length.
 *
 * This test verifies that invoking the SendQtDemuxOverrideEvent API with a nullptr for the buffer and a non-zero length does not throw an exception and returns a value of 0, indicating correct handling of the invalid input scenario.
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
 * | Variation / Step | Description                                                                                                  | Test Data                                                                                                                               | Expected Result                                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------- | ----------- |
 * | 01               | Create InterfacePlayerPriv object and invoke SendQtDemuxOverrideEvent with a null pointer and non-zero length  | mediaType = 1, pts = 3000, enablePTSReStamp = true, vodTrickModeFPS = 30, ptr = nullptr, len = 64                                    | API returns 0 and no exception is thrown                    | Should Pass |
 */
TEST(InterfacePlayerPriv, NonNullBufferPtrNullWithNonZeroLen) {
    std::cout << "Entering NonNullBufferPtrNullWithNonZeroLen test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 3000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        const void* ptr = nullptr;
        size_t len = 64;
        
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << ptr << " (nullptr with non-zero length)" << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, ptr, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(0, ret);
    });
    
    std::cout << "Exiting NonNullBufferPtrNullWithNonZeroLen test" << std::endl;
}
/**
 * @brief Test the SendQtDemuxOverrideEvent API with a non-null buffer pointer and zero length.
 *
 * This test verifies that when a valid non-null buffer is provided along with a zero length parameter,
 * the SendQtDemuxOverrideEvent API behaves as expected by returning 0 without processing any data.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                                                                      | Expected Result                                                      | Notes          |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------- | -------------- |
 * | 01               | Create an InterfacePlayerPriv object using the default constructor.                              | None                                                                                                                           | Object is instantiated successfully without throwing exceptions.     | Should be successful |
 * | 02               | Prepare a valid non-null buffer with sample content and set its length to zero.                    | buffer = "Another sample buffer data.", len = 0                                                                                 | Buffer is prepared correctly with a valid pointer and zero length.     | Should be successful |
 * | 03               | Invoke the SendQtDemuxOverrideEvent API with parameters: mediaType=1, pts=4000, enablePTSReStamp=true, vodTrickModeFPS=30, non-null buffer pointer, and len=0. | mediaType = 1, pts = 4000, enablePTSReStamp = true, vodTrickModeFPS = 30, ptr = buffer, len = 0                                   | API returns 0 as expected and no exceptions are thrown.                | Should Pass    |
 */
TEST(InterfacePlayerPriv, NonNullBufferPtrProvidedWithZeroLen) {
    std::cout << "Entering NonNullBufferPtrProvidedWithZeroLen test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 4000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 30;
        // Prepare a valid non-null buffer
        char buffer[256];
        const char* sampleStr = "Another sample buffer data.";
        std::memset(buffer, 0, sizeof(buffer));
        std::strncpy(buffer, sampleStr, sizeof(buffer)-1);
        size_t len = 0;
        
        std::cout << "Prepared buffer with content: " << buffer << std::endl;
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << std::endl;
        std::cout << " ptr: " << static_cast<const void*>(buffer) << " (valid pointer but zero length)" << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, buffer, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(0, ret);
    });
    
    std::cout << "Exiting NonNullBufferPtrProvidedWithZeroLen test" << std::endl;
}
/**
 * @brief Test the SendQtDemuxOverrideEvent API behavior when vodTrickModeFPS is set to zero.
 *
 * This test verifies that the SendQtDemuxOverrideEvent method in InterfacePlayerPriv properly handles a vodTrickModeFPS value of zero without throwing any exceptions and returns the expected result. It ensures that the function performs correctly under this edge case scenario.
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
 * | 01 | Invoke SendQtDemuxOverrideEvent with vodTrickModeFPS equal to zero using valid parameters. | mediaType = 1, pts = 1000, enablePTSReStamp = true, vodTrickModeFPS = 0, ptr = nullptr, len = 0 | API returns 0 and no exceptions are thrown. | Should Pass |
 */
TEST(InterfacePlayerPriv, VodTrickModeFPSZero) {
    std::cout << "Entering VodTrickModeFPSZero test" << std::endl;
    
    EXPECT_NO_THROW({
        InterfacePlayerPriv player;
        std::cout << "Created InterfacePlayerPriv object using default constructor." << std::endl;
        
        int mediaType = 1;
        GstClockTime pts = 1000;
        bool enablePTSReStamp = true;
        int vodTrickModeFPS = 0;
        const void* ptr = nullptr;
        size_t len = 0;
        
        std::cout << "Invoking SendQtDemuxOverrideEvent with parameters:" << std::endl;
        std::cout << " mediaType: " << mediaType << std::endl;
        std::cout << " pts: " << pts << std::endl;
        std::cout << " enablePTSReStamp: " << enablePTSReStamp << std::endl;
        std::cout << " vodTrickModeFPS: " << vodTrickModeFPS << " (zero specified)" << std::endl;
        std::cout << " ptr: " << ptr << std::endl;
        std::cout << " len: " << len << std::endl;
        
        gboolean ret = player.SendQtDemuxOverrideEvent(mediaType, pts, enablePTSReStamp, vodTrickModeFPS, ptr, len);
        std::cout << "Method returned: " << ret << std::endl;
        
        EXPECT_EQ(0, ret);
    });
    
    std::cout << "Exiting VodTrickModeFPSZero test" << std::endl;
}
/**
 * @brief Validate that InterfacePlayerPriv::SignalConnect handles valid parameters correctly
 *
 * This test verifies that when InterfacePlayerPriv::SignalConnect is invoked with valid parameters,
 * the method executes successfully without throwing any exceptions. It tests the creation of the object,
 * the preparation of valid inputs, and the proper invocation of the API.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                  | Expected Result                                            | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------- |
 * | 01               | Create InterfacePlayerPriv object using default constructor                                     | N/A                                                                                                                      | Object is created successfully                             | Should be successful |
 * | 02               | Invoke SignalConnect with valid parameters (instance pointer, detailed_signal, callback, and data) | instanceValue = 42, instance = valid pointer, detailed_signal = "signalName", callback = DummyCallback, dataValue = 84, data = valid pointer | SignalConnect executes without throwing any exception | Should Pass    |
 */
TEST(InterfacePlayerPriv, ValidParameters) {
    std::cout << "Entering ValidParameters test" << std::endl;
    
    // Create object using default constructor
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare valid parameters
    int instanceValue = 42;
    gpointer instance = reinterpret_cast<gpointer>(&instanceValue);
    std::cout << "Prepared valid instance pointer: " << instance << std::endl;
    
    char signalName[100];
    std::strncpy(signalName, "signalName", sizeof(signalName) - 1);
    signalName[sizeof(signalName) - 1] = '\0';
    std::cout << "Prepared valid detailed_signal: " << signalName << std::endl;
    
    GCallback callback = DummyCallback;
    std::cout << "Prepared valid callback handler pointer: " << (void*)callback << std::endl;
    
    int dataValue = 84;
    gpointer data = reinterpret_cast<gpointer>(&dataValue);
    std::cout << "Prepared valid user data pointer: " << data << std::endl;
    
    // Invoke method and check no exception is thrown
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with valid parameters" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked successfully" << std::endl;
    });
    
    std::cout << "Exiting ValidParameters test" << std::endl;
}
/**
 * @brief Verify that SignalConnect does not throw an exception when provided with a null instance pointer.
 *
 * This test checks whether the SignalConnect method of InterfacePlayerPriv handles a null instance pointer gracefully.
 * It ensures that even when the instance pointer is null, the API does not throw an exception, thereby validating its robustness.
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
 * | Variation / Step | Description                                                       | Test Data                                                                                      | Expected Result                                    | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- | -------------------------------------------------- | ------------- |
 * | 01               | Create InterfacePlayerPriv object using the default constructor.  | No input parameters.                                                                           | Object created successfully.                       | Should be successful |
 * | 02               | Prepare parameters with a null instance pointer.                  | instance = nullptr                                                                              | Parameter set to null.                             | Should be successful |
 * | 03               | Initialize detailed_signal string with "signalName".              | signalName = "signalName"                                                                       | String copied correctly into signalName.           | Should be successful |
 * | 04               | Prepare a valid callback handler pointer.                          | callback = DummyCallback                                                                        | Callback pointer valid.                            | Should be successful |
 * | 05               | Prepare valid user data pointer from dataValue.                    | dataValue = 84, data = address of dataValue                                                      | Data pointer valid.                                | Should be successful |
 * | 06               | Invoke SignalConnect using null instance pointer and validate no exception is thrown. | instance = nullptr, signalName = "signalName", callback = DummyCallback, data = address of dataValue | No exception is thrown by SignalConnect.           | Should Pass   |
 */
TEST(InterfacePlayerPriv, NullInstancePointer) {
    std::cout << "Entering NullInstancePointer test" << std::endl;
    
    // Create object
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare parameters with null instance pointer
    gpointer instance = nullptr;
    std::cout << "Using NULL for instance pointer" << std::endl;
    
    char signalName[100];
    std::strncpy(signalName, "signalName", sizeof(signalName) - 1);
    signalName[sizeof(signalName) - 1] = '\0';
    std::cout << "Prepared detailed_signal: " << signalName << std::endl;
    
    GCallback callback = DummyCallback;
    std::cout << "Prepared valid callback handler pointer: " << (void*)callback << std::endl;
    
    int dataValue = 84;
    gpointer data = reinterpret_cast<gpointer>(&dataValue);
    std::cout << "Prepared valid user data pointer: " << data << std::endl;
    
    // Invoke method and check no exception is thrown even with null instance pointer
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with NULL instance pointer" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked with NULL instance pointer" << std::endl;
    });
    
    std::cout << "Exiting NullInstancePointer test" << std::endl;
}
/**
 * @brief Verify that SignalConnect handles a null detailed_signal parameter without throwing an exception
 *
 * This test validates that when a null signal detail (signalName) is provided to the SignalConnect method, 
 * the API does not throw an exception. It ensures that the method can gracefully handle a null pointer for the signal name.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create InterfacePlayerPriv object using default constructor | None | Object is created successfully | Should be successful |
 * | 02 | Prepare valid instance pointer and set detailed_signal to nullptr | instanceValue = 42, instance = valid pointer, signalName = nullptr | Parameters prepared correctly | Should be successful |
 * | 03 | Prepare valid callback handler pointer and user data pointer | callback = DummyCallback, dataValue = 84, data = valid pointer | Parameters prepared correctly | Should be successful |
 * | 04 | Invoke SignalConnect with null detailed_signal to ensure no exception is thrown | instance = valid pointer, signalName = nullptr, callback = DummyCallback, data = valid pointer | API returns without throwing an exception and EXPECT_NO_THROW assertion passes | Should Pass |
 */
TEST(InterfacePlayerPriv, NullDetailedSignalString) {
    std::cout << "Entering NullDetailedSignalString test" << std::endl;
    
    // Create object
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare parameters with null detailed_signal
    int instanceValue = 42;
    gpointer instance = reinterpret_cast<gpointer>(&instanceValue);
    std::cout << "Prepared valid instance pointer: " << instance << std::endl;
    
    const gchar* signalName = nullptr;
    std::cout << "Using NULL for detailed_signal" << std::endl;
    
    GCallback callback = DummyCallback;
    std::cout << "Prepared valid callback handler pointer: " << (void*)callback << std::endl;
    
    int dataValue = 84;
    gpointer data = reinterpret_cast<gpointer>(&dataValue);
    std::cout << "Prepared valid user data pointer: " << data << std::endl;
    
    // Invoke method and check no exception is thrown even with null detailed_signal
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with NULL detailed_signal" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked with NULL detailed_signal" << std::endl;
    });
    
    std::cout << "Exiting NullDetailedSignalString test" << std::endl;
}
/**
 * @brief Validate the API's behavior when a null callback handler is supplied
 *
 * Validate that the SignalConnect method of the InterfacePlayerPriv object handles a null callback handler without throwing an exception@n
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
 * | Variation / Step | Description                                               | Test Data                                                       | Expected Result                                                   | Notes            |
 * | :--------------: | --------------------------------------------------------- | ---------------------------------------------------------------- | ----------------------------------------------------------------- | ---------------- |
 * | 01               | Create an object of InterfacePlayerPriv                   | N/A                                                             | Object created successfully                                         | Should be successful |
 * | 02               | Prepare a valid instance pointer pointing to instanceValue  | instanceValue = 42, instance = pointer to instanceValue          | Pointer prepared correctly                                           | Should be successful |
 * | 03               | Prepare signalName character array                        | signalName = "signalName"                                         | Array initialized properly                                           | Should be successful |
 * | 04               | Set callback handler to null                              | callback = nullptr                                               | Null callback handler is set                                         | Should Pass      |
 * | 05               | Prepare a valid user data pointer                         | dataValue = 84, data = pointer to dataValue                        | User data pointer prepared correctly                                 | Should be successful |
 * | 06               | Invoke SignalConnect with the null callback handler       | instance, signalName, callback (null), data                         | EXPECT_NO_THROW ensures the method executes without throwing exception | Should Pass      |
 */
TEST(InterfacePlayerPriv, NullCallbackHandler) {
    std::cout << "Entering NullCallbackHandler test" << std::endl;
    
    // Create object
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare parameters with null callback handler
    int instanceValue = 42;
    gpointer instance = reinterpret_cast<gpointer>(&instanceValue);
    std::cout << "Prepared valid instance pointer: " << instance << std::endl;
    
    char signalName[100];
    std::strncpy(signalName, "signalName", sizeof(signalName) - 1);
    signalName[sizeof(signalName) - 1] = '\0';
    std::cout << "Prepared detailed_signal: " << signalName << std::endl;
    
    GCallback callback = nullptr;
    std::cout << "Using NULL for callback handler" << std::endl;
    
    int dataValue = 84;
    gpointer data = reinterpret_cast<gpointer>(&dataValue);
    std::cout << "Prepared valid user data pointer: " << data << std::endl;
    
    // Invoke method and check no exception is thrown even with null callback handler
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with NULL callback handler" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked with NULL callback handler" << std::endl;
    });
    
    std::cout << "Exiting NullCallbackHandler test" << std::endl;
}
/**
 * @brief Verify that SignalConnect handles a null user data pointer without throwing an exception.
 *
 * This test validates that the SignalConnect method of the InterfacePlayerPriv class
 * correctly accepts a null user data pointer while the other parameters are valid.
 * It ensures that no exceptions are thrown during the method invocation, preserving
 * stability in scenarios where the user data pointer is intentionally set to null.
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
 * | Variation / Step | Description                                                                                 | Test Data                                                                                                                              | Expected Result                                                                                          | Notes           |
 * | :----:           | ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------- | --------------- |
 * | 01               | Create an InterfacePlayerPriv object using the default constructor.                         | Constructor call: InterfacePlayerPriv()                                                                                                | Object is created successfully with no exceptions.                                                     | Should be successful |
 * | 02               | Prepare a valid instance pointer by setting an integer value and converting its address.      | instanceValue = 42, instance = &instanceValue                                                                                            | The instance pointer is prepared correctly.                                                            | Should be successful |
 * | 03               | Prepare the detailed signal name by copying "signalName" into a character array.              | signalName = "signalName"                                                                                                                | The detailed signal name is prepared safely and null-terminated.                                         | Should be successful |
 * | 04               | Set up a valid callback handler pointer using DummyCallback.                                | callback = DummyCallback                                                                                                                 | The callback pointer is valid and ready for use.                                                       | Should be successful |
 * | 05               | Set the user data pointer to null.                                                          | data = nullptr                                                                                                                           | The user data pointer is set to null.                                                                   | Should be successful |
 * | 06               | Invoke the SignalConnect method with the prepared parameters including the null user data pointer. | instance = &instanceValue, signalName = "signalName", callback = DummyCallback, data = nullptr                                            | The API call completes without throwing an exception (checked via EXPECT_NO_THROW).                      | Should Pass     |
 */
TEST(InterfacePlayerPriv, NullUserDataPointer) {
    std::cout << "Entering NullUserDataPointer test" << std::endl;
    
    // Create object
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare parameters with null user data pointer
    int instanceValue = 42;
    gpointer instance = reinterpret_cast<gpointer>(&instanceValue);
    std::cout << "Prepared valid instance pointer: " << instance << std::endl;
    
    char signalName[100];
    std::strncpy(signalName, "signalName", sizeof(signalName) - 1);
    signalName[sizeof(signalName) - 1] = '\0';
    std::cout << "Prepared detailed_signal: " << signalName << std::endl;
    
    GCallback callback = DummyCallback;
    std::cout << "Prepared valid callback handler pointer: " << (void*)callback << std::endl;
    
    gpointer data = nullptr;
    std::cout << "Using NULL for user data pointer" << std::endl;
    
    // Invoke method and check no exception is thrown even with null user data pointer
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with NULL user data pointer" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked with NULL user data pointer" << std::endl;
    });
    
    std::cout << "Exiting NullUserDataPointer test" << std::endl;
}
/**
 * @brief Verify that SignalConnect accepts an empty detailed_signal string without error
 *
 * This test verifies that the SignalConnect method of the InterfacePlayerPriv class can be invoked using an empty detailed_signal string while providing valid instance, callback, and user data pointers. The test ensures that passing an empty string does not cause any exceptions.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 051
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create InterfacePlayerPriv object using default constructor | None | Object is created successfully | Should be successful |
 * | 02 | Prepare valid instance pointer | instanceValue = 42, instance pointer = valid address | Instance pointer is valid | Should be successful |
 * | 03 | Prepare empty detailed_signal string | signalName = "" | Detailed signal string is empty but valid | Should be successful |
 * | 04 | Prepare valid callback handler pointer | callback = DummyCallback | Callback pointer is valid | Should be successful |
 * | 05 | Prepare valid user data pointer | dataValue = 84, data pointer = valid address | User data pointer is valid | Should be successful |
 * | 06 | Invoke SignalConnect with empty detailed_signal string | instance, signalName (empty), callback, data | No exception thrown; API returns expected behavior | Should Pass |
 */
TEST(InterfacePlayerPriv, EmptyDetailedSignalString) {
    std::cout << "Entering EmptyDetailedSignalString test" << std::endl;
    
    // Create object
    InterfacePlayerPriv obj;
    std::cout << "Created InterfacePlayerPriv object using default constructor" << std::endl;
    
    // Prepare parameters with empty detailed_signal string
    int instanceValue = 42;
    gpointer instance = reinterpret_cast<gpointer>(&instanceValue);
    std::cout << "Prepared valid instance pointer: " << instance << std::endl;
    
    char signalName[100];
    std::strncpy(signalName, "", sizeof(signalName) - 1);
    signalName[sizeof(signalName) - 1] = '\0';
    std::cout << "Prepared empty detailed_signal string: \"" << signalName << "\"" << std::endl;
    
    GCallback callback = DummyCallback;
    std::cout << "Prepared valid callback handler pointer: " << (void*)callback << std::endl;
    
    int dataValue = 84;
    gpointer data = reinterpret_cast<gpointer>(&dataValue);
    std::cout << "Prepared valid user data pointer: " << data << std::endl;
    
    // Invoke method and check no exception is thrown even with empty detailed_signal
    EXPECT_NO_THROW({
        std::cout << "Invoking SignalConnect with empty detailed_signal string" << std::endl;
        obj.SignalConnect(instance, signalName, callback, data);
        std::cout << "SignalConnect invoked with empty detailed_signal string" << std::endl;
    });
    
    std::cout << "Exiting EmptyDetailedSignalString test" << std::endl;
}
/**
 * @brief Verify that the destructor is correctly invoked for a stack-allocated object.
 *
 * This test ensures that creating an instance of InterfacePlayerPriv on the stack
 * does not throw any exceptions and that the destructor is automatically invoked
 * upon exiting the scope. This behavior is crucial to avoid memory leaks and maintain
 * proper object lifecycle management.
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
 * | Variation / Step | Description                                                                                  | Test Data                                   | Expected Result                                         | Notes       |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------- | ----------- |
 * | 01               | Create a local InterfacePlayerPriv object on the stack; verify constructor and destructor calls | Object creation: InterfacePlayerPriv instance | No exceptions thrown; destructor invoked on scope exit | Should Pass |
 */
TEST(InterfacePlayerPriv, DestructorCalledOnStackObject_start) {
    std::cout << "Entering DestructorCalledOnStackObject_start test" << std::endl;
  
    EXPECT_NO_THROW({
        {
            // Create a local object on the stack.
            InterfacePlayerPriv obj;
            std::cout << "Invoked default constructor for InterfacePlayerPriv." << std::endl;    
            std::cout << "About to exit inner scope, destructor will be invoked." << std::endl;
        } // Destructor automatically invoked here.
    });
  
    std::cout << "Exiting DestructorCalledOnStackObject_start test" << std::endl;
}
/**
 * @brief Test the default construction of gst_media_stream to ensure it initializes to default values.
 *
 * This test validates that when a gst_media_stream object is default-constructed, all its members are initialized with the expected default values. It verifies that pointer members (sinkbin, source, demuxPad) are set to nullptr, the format is set to GST_FORMAT_INVALID, boolean flags (pendingSeek, resetPosition, bufferUnderrun, eosReached, sourceConfigured, firstBufferProcessed) are false, mutex operations on sourceLock succeed, timeScale is set to 1u, trackId is -1, and demuxProbeId is 0ul. This ensures that the object is in a valid initial state.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke default constructor of gst_media_stream. | No input required. | Object is constructed without throwing exceptions. | Should Pass |
 * | 02 | Verify sinkbin is initialized to nullptr. | stream.sinkbin = uninitialized | stream.sinkbin equals nullptr. | Should Pass |
 * | 03 | Verify source is initialized to nullptr. | stream.source = uninitialized | stream.source equals nullptr. | Should Pass |
 * | 04 | Verify format is initialized to GST_FORMAT_INVALID. | stream.format = uninitialized | stream.format equals GST_FORMAT_INVALID. | Should Pass |
 * | 05 | Verify pendingSeek is initialized to false. | stream.pendingSeek = uninitialized | stream.pendingSeek equals false. | Should Pass |
 * | 06 | Verify resetPosition is initialized to false. | stream.resetPosition = uninitialized | stream.resetPosition equals false. | Should Pass |
 * | 07 | Verify bufferUnderrun is initialized to false. | stream.bufferUnderrun = uninitialized | stream.bufferUnderrun equals false. | Should Pass |
 * | 08 | Verify eosReached is initialized to false. | stream.eosReached = uninitialized | stream.eosReached equals false. | Should Pass |
 * | 09 | Verify sourceConfigured is initialized to false. | stream.sourceConfigured = uninitialized | stream.sourceConfigured equals false. | Should Pass |
 * | 10 | Verify locking and unlocking sourceLock succeed. | pthread_mutex_lock(&stream.sourceLock), pthread_mutex_unlock(&stream.sourceLock) | Both lock and unlock return 0. | Should Pass |
 * | 11 | Verify timeScale is initialized to 1u. | stream.timeScale = uninitialized | stream.timeScale equals 1u. | Should Pass |
 * | 12 | Verify trackId is initialized to -1. | stream.trackId = uninitialized | stream.trackId equals -1. | Should Pass |
 * | 13 | Verify firstBufferProcessed is initialized to false. | stream.firstBufferProcessed = uninitialized | stream.firstBufferProcessed equals false. | Should Pass |
 * | 14 | Verify demuxPad is initialized to nullptr. | stream.demuxPad = uninitialized | stream.demuxPad equals nullptr. | Should Pass |
 * | 15 | Verify demuxProbeId is initialized to 0ul. | stream.demuxProbeId = uninitialized | stream.demuxProbeId equals 0ul. | Should Pass |
 */
TEST(gst_media_stream, DefaultConstructionPositiveTest) {
    std::cout << "Entering DefaultConstructionPositiveTest test" << std::endl;

    EXPECT_NO_THROW({
        gst_media_stream stream;
        std::cout << "Invoked gst_media_stream default constructor." << std::endl;

        // Check sinkbin
        std::cout << "sinkbin value: " << stream.sinkbin << std::endl;
        EXPECT_EQ(stream.sinkbin, nullptr);

        // Check source
        std::cout << "source value: " << stream.source << std::endl;
        EXPECT_EQ(stream.source, nullptr);

        // Check format; expecting GST_FORMAT_INVALID
        std::cout << "format value: " << stream.format << std::endl;
        EXPECT_EQ(stream.format, GST_FORMAT_INVALID);

        // Check pendingSeek
        std::cout << "pendingSeek value: " << std::boolalpha << stream.pendingSeek << std::endl;
        EXPECT_FALSE(stream.pendingSeek);

        // Check resetPosition
        std::cout << "resetPosition value: " << std::boolalpha << stream.resetPosition << std::endl;
        EXPECT_FALSE(stream.resetPosition);

        // Check bufferUnderrun
        std::cout << "bufferUnderrun value: " << std::boolalpha << stream.bufferUnderrun << std::endl;
        EXPECT_FALSE(stream.bufferUnderrun);

        // Check eosReached
        std::cout << "eosReached value: " << std::boolalpha << stream.eosReached << std::endl;
        EXPECT_FALSE(stream.eosReached);

        // Check sourceConfigured
        std::cout << "sourceConfigured value: " << std::boolalpha << stream.sourceConfigured << std::endl;
        EXPECT_FALSE(stream.sourceConfigured);

        // Check sourceLock by attempting to lock and unlock
        int lockResult = pthread_mutex_lock(&stream.sourceLock);
        std::cout << "Locking sourceLock returned: " << lockResult << std::endl;
        EXPECT_EQ(lockResult, 0);
        int unlockResult = pthread_mutex_unlock(&stream.sourceLock);
        std::cout << "Unlocking sourceLock returned: " << unlockResult << std::endl;
        EXPECT_EQ(unlockResult, 0);

        // Check timeScale
        std::cout << "timeScale value: " << stream.timeScale << std::endl;
        EXPECT_EQ(stream.timeScale, 1u);

        // Check trackId
        std::cout << "trackId value: " << stream.trackId << std::endl;
        EXPECT_EQ(stream.trackId, -1);

        // Check firstBufferProcessed
        std::cout << "firstBufferProcessed value: " << std::boolalpha << stream.firstBufferProcessed << std::endl;
        EXPECT_FALSE(stream.firstBufferProcessed);

        // Check demuxPad
        std::cout << "demuxPad value: " << stream.demuxPad << std::endl;
        EXPECT_EQ(stream.demuxPad, nullptr);

        // Check demuxProbeId
        std::cout << "demuxProbeId value: " << stream.demuxProbeId << std::endl;
        EXPECT_EQ(stream.demuxProbeId, 0ul);
    });

    std::cout << "Exiting DefaultConstructionPositiveTest test" << std::endl;
}
/**
 * @brief To test safe deletion of a default constructed gst_media_stream object by invoking its destructor, ensuring no exceptions are thrown.
 *
 * This test case verifies that a gst_media_stream object, when created using the default constructor and subsequently deleted, does not throw any exceptions. It confirms that the destructor ~gst_media_stream() is implemented correctly and releases resources without errors.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Invoke default constructor to create a gst_media_stream object. | constructor = default | Object created successfully without throwing exceptions | Should Pass |
 * | 02 | Invoke delete operator to call the destructor for the gst_media_stream object. | input: mediaStream pointer = valid object | Destructor executed successfully without throwing exceptions | Should Pass |
 */
TEST(gst_media_stream, Destructor_on_default_constructed_object) {
    std::cout << "Entering Destructor_on_default_constructed_object test" << std::endl;
    
    EXPECT_NO_THROW({
        // Create a gst_media_stream object using the default constructor
        gst_media_stream* mediaStream = new gst_media_stream();
        std::cout << "Invoked gst_media_stream default constructor. Object created successfully." << std::endl;
                
        // Delete the object which invokes the destructor ~gst_media_stream()
        delete mediaStream;
        std::cout << "Invoked destructor ~gst_media_stream() via delete and no errors occurred." << std::endl;
    });
    
    std::cout << "Exiting Destructor_on_default_constructed_object test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
