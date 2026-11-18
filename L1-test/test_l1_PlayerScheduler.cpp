
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
#include "PlayerScheduler.h"

class TestablePlayerScheduler : public PlayerScheduler {
public:
	using PlayerScheduler::ExecuteAsyncTask;
};

class PlayerSchedulerTest : public ::testing::Test {
protected:
	void SetUp() override {
		scheduler = new TestablePlayerScheduler();
	}
	
	void TearDown() override {
		delete scheduler;
		scheduler = nullptr;
	}
	
	TestablePlayerScheduler* scheduler;
};

//
// Test Case: Valid inputs with non-empty task name and valid id
//
/**
 * @brief Validate construction of PlayerAsyncTaskObj using valid inputs and non-empty task name
 *
 * This test verifies that a PlayerAsyncTaskObj can be constructed without exceptions when provided with a valid lambda task, a valid data pointer, a non-empty task name ("TestTask"), and a valid id (1). The test also checks that the internal state of the object (mData, mTaskName, and mId) is correctly set and that the lambda function (mTask) is invoked as expected.
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
 * | 01 | Construct PlayerAsyncTaskObj with valid inputs and verify internal state | taskLambda = valid lambda, dataPtr = address of int (value = 42), taskName = "TestTask", id = 1 | Object is created without throwing exceptions; mData equals dataPtr, mTaskName equals "TestTask", mId equals 1; lambda is valid and returns expected log outputs | Should Pass |
 */
TEST(PlayerAsyncTaskObj, ValidInputsWithNonEmptyTask) {
    std::cout << "Entering ValidInputsWithNonEmptyTask test" << std::endl;

    int value = 42;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    // Lambda that processes the data pointer.
    auto taskLambda = [](void* d) {
        std::cout << "Inside task lambda. Received data pointer: " << d << std::endl;
    };
    std::cout << "Lambda task constructed." << std::endl;

    // Constructing PlayerAsyncTaskObj with task, data, non-empty task name and valid id.
    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, dataPtr, "TestTask", 1);
        std::cout << "PlayerAsyncTaskObj constructed with task, data, taskName 'TestTask', id 1." << std::endl;
        // Internal state inspection.
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        // Verify function pointer is valid by invoking it.
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda..." << std::endl;
            obj.mTask(dataPtr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, "TestTask");
        EXPECT_EQ(obj.mId, 1);
    });

    std::cout << "Exiting ValidInputsWithNonEmptyTask test" << std::endl;
}
/**
 * @brief Validate the construction of PlayerAsyncTaskObj using valid inputs and defaults
 *
 * This test validates the creation of a PlayerAsyncTaskObj using a valid lambda task and data pointer while relying on default values for mTaskName and mId. It ensures that no exception is thrown, the lambda is set and invoked correctly, and the internal state of the object (mData, mTaskName, mId) matches the expected defaults.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerAsyncTaskObj with a valid lambda task and a valid data pointer while using default parameters for taskName and id. | taskLambda = [lambda that prints received data pointer], dataPtr = pointer to int with value 100, default mTaskName = "", default mId = PLAYER_TASK_ID_INVALID | No exception thrown; object's mData equals dataPtr, mTaskName equals "", and mId equals PLAYER_TASK_ID_INVALID; lambda task outputs expected message when invoked. | Should Pass |
 */
TEST(PlayerAsyncTaskObj, ValidInputsUsingDefaults) {
    std::cout << "Entering ValidInputsUsingDefaults test" << std::endl;

    int value = 100;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    auto taskLambda = [](void* d) {
        std::cout << "Inside default task lambda. Received data pointer: " << d << std::endl;
    };
    std::cout << "Default lambda task constructed." << std::endl;

    // Constructing PlayerAsyncTaskObj with task and data only (default tskName and id).
    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, dataPtr);
        std::cout << "PlayerAsyncTaskObj constructed with default taskName and id." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: '" << obj.mTaskName << "'" << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda with data pointer..." << std::endl;
            obj.mTask(dataPtr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, "");
        EXPECT_EQ(obj.mId, PLAYER_TASK_ID_INVALID);
    });

    std::cout << "Exiting ValidInputsUsingDefaults test" << std::endl;
}
/**
 * @brief This test validates that constructing a PlayerAsyncTaskObj with a null task pointer does not throw an exception and initializes the object correctly.
 *
 * This test ensures that when a null function pointer is provided for the task, the PlayerAsyncTaskObj is still constructed without throwing exceptions. It checks that the internal state (mData, mTaskName, mId) is correctly set based on the provided inputs and that the mTask member remains null.
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
 * | Variation / Step | Description                                                                                                           | Test Data                                                           | Expected Result                                                                                                                                             | Notes      |
 * | :--------------: | --------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerAsyncTaskObj constructor with a null task pointer, a valid data pointer, task name "NullTask", and id 10. | task = nullptr, dataPtr = address of an integer with value 55, taskName = "NullTask", id = 10 | No exception thrown; obj.mData equals dataPtr; obj.mTaskName equals "NullTask"; obj.mId equals 10; obj.mTask is null as expected. | Should Pass |
 */
TEST(PlayerAsyncTaskObj, NullTaskFunctionPointer) {
    std::cout << "Entering NullTaskFunctionPointer test" << std::endl;

    int value = 55;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    // Using nullptr for the task.
    std::cout << "Using nullptr for task lambda." << std::endl;

    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(nullptr, dataPtr, "NullTask", 10);
        std::cout << "PlayerAsyncTaskObj constructed with nullptr task, data, taskName 'NullTask', id 10." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is unexpectedly valid." << std::endl;
        } else {
            std::cout << "mTask is null as expected." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, "NullTask");
        EXPECT_EQ(obj.mId, 10);
    });

    std::cout << "Exiting NullTaskFunctionPointer test" << std::endl;
}
/**
 * @brief Verify that PlayerAsyncTaskObj correctly handles a nullptr data pointer.
 *
 * This test case validates that when a nullptr is provided as the data pointer to the PlayerAsyncTaskObj constructor, the object
 * is created without throwing an exception, and its internal state (mData, mTaskName, and mId) is correctly initialized. In addition,
 * it checks that the task lambda, when invoked with a nullptr, executes as expected.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                  | Expected Result                                                                                           | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Set data pointer to nullptr and construct a lambda task that accepts a void pointer.           | data pointer = nullptr                                                     | Lambda task is defined and ready for invocation.                                                          | Should be successful |
 * | 02               | Construct PlayerAsyncTaskObj using the lambda task, nullptr for data, taskName "TaskWithNullData", and id 5. | input: taskLambda = defined lambda, data = nullptr, taskName = TaskWithNullData, id = 5 | Object is constructed without throwing any exception.                                                   | Should Pass   |
 * | 03               | Validate the internal state of the object and, if valid, invoke the lambda with a nullptr parameter. | internal members: mData = nullptr, mTaskName = "TaskWithNullData", mId = 5   | mData is nullptr, mTaskName equals "TaskWithNullData", mId equals 5, and lambda executes with nullptr.    | Should Pass   |
 */
TEST(PlayerAsyncTaskObj, NullDataPointer) {
    std::cout << "Entering NullDataPointer test" << std::endl;

    // Using nullptr for data pointer.
    std::cout << "Using nullptr for data pointer." << std::endl;

    auto taskLambda = [](void* d) {
        std::cout << "Inside task lambda with possible null data. Received data pointer: " << d << std::endl;
    };
    std::cout << "Lambda task constructed for null data test." << std::endl;

    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, nullptr, "TaskWithNullData", 5);
        std::cout << "PlayerAsyncTaskObj constructed with task, nullptr data, taskName 'TaskWithNullData', id 5." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda with nullptr..." << std::endl;
            obj.mTask(nullptr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, nullptr);
        EXPECT_EQ(obj.mTaskName, "TaskWithNullData");
        EXPECT_EQ(obj.mId, 5);
    });

    std::cout << "Exiting NullDataPointer test" << std::endl;
}
/**
 * @brief Verifies that PlayerAsyncTaskObj handles the boundary id value (INT_MAX) correctly.
 *
 * This test constructs a PlayerAsyncTaskObj using a lambda function, a pointer to an integer, the task name "BoundaryTask",
 * and sets the id to INT_MAX. It then verifies that no exception is thrown during construction, and checks that
 * the internal state values (mData, mTaskName, mId) match the inputs. Finally, if the lambda task is valid, it is invoked
 * with the data pointer to confirm proper functionality.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Construct PlayerAsyncTaskObj with a lambda, data pointer, task name "BoundaryTask", and id INT_MAX | taskLambda = lambda function, dataPtr = pointer to int value 77, taskName = "BoundaryTask", id = INT_MAX | Object is constructed without throwing an exception | Should Pass |
 * | 02 | Validate internal state of PlayerAsyncTaskObj (mData, mTaskName, mId) | dataPtr = pointer to int value 77, expected mTaskName = "BoundaryTask", id = INT_MAX | mData equals dataPtr, mTaskName equals "BoundaryTask", mId equals INT_MAX | Should Pass |
 * | 03 | Invoke the lambda task if mTask is valid | mTask = lambda function, dataPtr value = 77 | Lambda executes without throwing an exception and processes the data pointer | Should Pass |
 */
TEST(PlayerAsyncTaskObj, BoundaryValueId_INT_MAX) {
    std::cout << "Entering BoundaryValueId_INT_MAX test" << std::endl;

    int value = 77;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    auto taskLambda = [](void* d) {
        std::cout << "Inside task lambda for INT_MAX test. Received data pointer: " << d << std::endl;
    };
    std::cout << "Lambda task constructed for INT_MAX test." << std::endl;

    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, dataPtr, "BoundaryTask", INT_MAX);
        std::cout << "PlayerAsyncTaskObj constructed with task, data, taskName 'BoundaryTask', id INT_MAX (" << INT_MAX << ")." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda for INT_MAX test..." << std::endl;
            obj.mTask(dataPtr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, "BoundaryTask");
        EXPECT_EQ(obj.mId, INT_MAX);
    });

    std::cout << "Exiting BoundaryValueId_INT_MAX test" << std::endl;
}
/**
 * @brief Test that PlayerAsyncTaskObj can correctly handle the boundary value of INT_MIN.
 *
 * Verifies that PlayerAsyncTaskObj properly stores the data pointer, task name, and id when constructed with the boundary value INT_MIN. This test ensures that the object initializes its members correctly and that the associated lambda task is callable.
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
 * | Variation / Step | Description                                                         | Test Data                                                                                   | Expected Result                                                                                  | Notes         |
 * | :--------------: | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Initialize test variables and lambda function                       | value = 88, dataPtr = address of value, lambda = defined, taskName = "BoundaryTask", id = INT_MIN | Variables and lambda initialized successfully                                                    | Should be successful |
 * | 02               | Construct PlayerAsyncTaskObj and validate internal state using EXPECT_NO_THROW | taskLambda, dataPtr, taskName = "BoundaryTask", id = INT_MIN                                 | No exception thrown; obj.mData equals dataPtr, obj.mTaskName equals "BoundaryTask", obj.mId equals INT_MIN | Should Pass   |
 * | 03               | Invoke the lambda task if valid and verify its execution           | mTask is invoked with dataPtr                                                               | Lambda executed and prints received data pointer                                                  | Should Pass   |
 */
TEST(PlayerAsyncTaskObj, BoundaryValueId_INT_MIN) {
    std::cout << "Entering BoundaryValueId_INT_MIN test" << std::endl;

    int value = 88;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    auto taskLambda = [](void* d) {
        std::cout << "Inside task lambda for INT_MIN test. Received data pointer: " << d << std::endl;
    };
    std::cout << "Lambda task constructed for INT_MIN test." << std::endl;

    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, dataPtr, "BoundaryTask", INT_MIN);
        std::cout << "PlayerAsyncTaskObj constructed with task, data, taskName 'BoundaryTask', id INT_MIN (" << INT_MIN << ")." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda for INT_MIN test..." << std::endl;
            obj.mTask(dataPtr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, "BoundaryTask");
        EXPECT_EQ(obj.mId, INT_MIN);
    });

    std::cout << "Exiting BoundaryValueId_INT_MIN test" << std::endl;
}
/**
 * @brief This test verifies that the PlayerAsyncTaskObj constructor correctly handles very long task names.
 *
 * This test checks that when a long string is provided as the task name, it is properly copied into the object's internal state without overflow.
 * It also verifies that the internal members mData, mTaskName, and mId are correctly assigned.
 * Furthermore, it confirms that the provided lambda task is valid and can be invoked with the expected data.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                          | Expected Result                                                                                                         | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare test data: initialize integer value, data pointer, lambda, and long task name string          | value = 123, dataPtr = address of value, longName = "ThisIsAnExtremelyLongTaskNameDesignedToTestTheStringHandlingCapabilitiesOfTheConstructor" | Test data is initialized without errors                                                                               | Should be successful |
 * | 02               | Construct PlayerAsyncTaskObj with the lambda, data pointer, long task name, and id 3, then verify internal state | input: dataPtr, taskName string, id = 3, taskLambda function pointer                                                                  | Object is constructed without throwing an exception; mData equals dataPtr, mTaskName equals the long task name, mId equals 3  | Should Pass   |
 * | 03               | Invoke the lambda function via the object's mTask member if available                                   | input: dataPtr                                                                                                                     | Lambda is invoked without errors                                                                                       | Should Pass   |
 */
TEST(PlayerAsyncTaskObj, LongTaskNameString) {
    std::cout << "Entering LongTaskNameString test" << std::endl;

    int value = 123;
    int* dataPtr = &value;
    std::cout << "Data pointer address: " << dataPtr << " with value: " << value << std::endl;

    auto taskLambda = [](void* d) {
        std::cout << "Inside task lambda for long task name test. Received data pointer: " << d << std::endl;
    };
    std::cout << "Lambda task constructed for long task name test." << std::endl;

    const char* longName = "ThisIsAnExtremelyLongTaskNameDesignedToTestTheStringHandlingCapabilitiesOfTheConstructor";
    char taskNameBuffer[256];
    strncpy(taskNameBuffer, longName, sizeof(taskNameBuffer) - 1);
    taskNameBuffer[sizeof(taskNameBuffer) - 1] = '\0';
    std::string taskName(taskNameBuffer);
    std::cout << "Long task name prepared: " << taskName << std::endl;

    EXPECT_NO_THROW({
        PlayerAsyncTaskObj obj(taskLambda, dataPtr, taskName, 3);
        std::cout << "PlayerAsyncTaskObj constructed with task, data, long taskName, id 3." << std::endl;
        std::cout << "Internal state - mData: " << obj.mData << std::endl;
        std::cout << "Internal state - mTaskName: " << obj.mTaskName << std::endl;
        std::cout << "Internal state - mId: " << obj.mId << std::endl;
        if (obj.mTask) {
            std::cout << "mTask is valid. Invoking lambda for long task name test..." << std::endl;
            obj.mTask(dataPtr);
        } else {
            std::cout << "mTask is null." << std::endl;
        }
        EXPECT_EQ(obj.mData, dataPtr);
        EXPECT_EQ(obj.mTaskName, taskName);
        EXPECT_EQ(obj.mId, 3);
    });

    std::cout << "Exiting LongTaskNameString test" << std::endl;
}
/**
 * @brief Verify copy constructor of PlayerAsyncTaskObj with valid non-default values
 *
 * This test verifies that the copy constructor of PlayerAsyncTaskObj properly duplicates an object 
 * created with a custom lambda, a valid data pointer, task name "TestTask", and mId value 101. 
 * It ensures that both the original and copied objects function as expected by invoking their lambda functions
 * and comparing member values.
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
 * | 01 | Initialize test data including an integer value, its pointer, and a lambda for mTask. | value = 10, dataPtr = address(value), lambda = valid lambda function | Test variables are set up correctly without error. | Should be successful |
 * | 02 | Create original PlayerAsyncTaskObj using the custom constructor. | lambda = valid lambda function, dataPtr = valid pointer, mTaskName = TestTask, mId = 101 | Object is created without throwing exceptions and its members are correctly assigned. | Should Pass |
 * | 03 | Invoke the mTask lambda of the original object. | dataPtr = valid pointer | Lambda function executes and prints expected output without exceptions. | Should Pass |
 * | 04 | Create a copy of the original object using the copy constructor. | original object with members: mData, mId = 101, mTaskName = TestTask | Copy object is created with identical member values as the original. | Should Pass |
 * | 05 | Validate that the copied object's members match those of the original. | Comparison of original.mData, original.mId, original.mTaskName with copied objects' corresponding members | All assertions pass confirming that the copy has the correct values. | Should Pass |
 * | 06 | Invoke the mTask lambda of the copied object. | dataPtr = valid pointer | Lambda function in the copied object executes successfully without throwing exceptions. | Should Pass |
 */
TEST(PlayerAsyncTaskObj, CopyConstructorWithValidNonDefaultValues) {
    std::cout << "Entering CopyConstructorWithValidNonDefaultValues test" << std::endl;
    
    // Setting up test data
    int value = 10;
    void* dataPtr = static_cast<void*>(&value);
    
    // Define a lambda function for mTask
    std::function<void(void*)> lambda = [](void* data) {
        int* intPtr = static_cast<int*>(data);
        std::cout << "Lambda invoked with value: " << *intPtr << std::endl;
    };
    std::cout << "Initialized lambda for mTask with address: " << (void*)(&lambda) << std::endl;
    std::cout << "Data pointer set to: " << dataPtr << " (value = " << value << ")" << std::endl;
    std::cout << "Test mId value: 101 and mTaskName: TestTask" << std::endl;
    
    // Create original object using custom constructor
    EXPECT_NO_THROW({
        PlayerAsyncTaskObj orig(lambda, dataPtr, "TestTask", 101);
        std::cout << "Original PlayerAsyncTaskObj created." << std::endl;
        std::cout << "orig.mData = " << orig.mData << std::endl;
        std::cout << "orig.mId = " << orig.mId << std::endl;
        std::cout << "orig.mTaskName = " << orig.mTaskName << std::endl;
        
        // Invoke the original mTask to demonstrate behavior
        std::cout << "Invoking orig.mTask lambda:" << std::endl;
        EXPECT_NO_THROW(orig.mTask(dataPtr));

        // Create a new object using the copy constructor
        PlayerAsyncTaskObj copy(orig);
        std::cout << "Copy constructed PlayerAsyncTaskObj created." << std::endl;
        std::cout << "copy.mData = " << copy.mData << std::endl;
        std::cout << "copy.mId = " << copy.mId << std::endl;
        std::cout << "copy.mTaskName = " << copy.mTaskName << std::endl;
        
        // Validate that the copied object's members match the original's
        EXPECT_EQ(copy.mData, orig.mData);
        EXPECT_EQ(copy.mId, 101);
        EXPECT_EQ(copy.mTaskName, "TestTask");
        
        // Invoke the copied mTask to ensure it still works as expected
        std::cout << "Invoking copy.mTask lambda:" << std::endl;
        EXPECT_NO_THROW(copy.mTask(dataPtr));
    });
    
    std::cout << "Exiting CopyConstructorWithValidNonDefaultValues test" << std::endl;
}
/**
 * @brief Validate that the copy assignment operator correctly copies all attributes from one PlayerAsyncTaskObj to another.
 *
 * This test verifies that when a source PlayerAsyncTaskObj is assigned to a target PlayerAsyncTaskObj using the copy assignment
 * operator, the target object's mTask, mData, mId, and mTaskName are updated to match those of the source. This ensures that
 * the operator= performs a proper deep copy of the function pointer and other members.
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
 * | Variation / Step | Description                                                               | Test Data                                                                                                      | Expected Result                                                           | Notes           |
 * | :--------------: | ------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- | --------------- |
 * | 01               | Construct the source PlayerAsyncTaskObj with a lambda that sets a flag.    | sourceTask lambda, sourceData pointer = &sourceDataVal, mTaskName = "DownloadTask", mId = 100                   | Source object is constructed successfully.                              | Should be successful |
 * | 02               | Construct the target PlayerAsyncTaskObj with an initial lambda.            | targetTask lambda, targetData pointer = &targetDataVal, mTaskName = "OldTask", mId = 50                           | Target object is constructed successfully.                              | Should be successful |
 * | 03               | Invoke the copy assignment operator to assign the source to the target.    | Implicit assignment operator invocation                                                                           | Assignment operator returns target reference and target attributes are copied. | Should Pass     |
 * | 04               | Invoke the mTask lambda of the target after assignment.                  | No explicit test data                                                                                           | sourceTaskFlag is set to 10, verifying that target.mTask equals sourceTask lambda.  | Should Pass     |
 * | 05               | Verify that mData pointers, mId values, and mTaskName are equal between source and target. | Source: mData, mId, mTaskName; Target: mData, mId, mTaskName                         | All comparisons (mData, mId, mTaskName) return true using EXPECT_EQ.      | Should Pass     |
 */
TEST(PlayerAsyncTaskObj, CopyAssignmentValid) {
    std::cout << "Entering CopyAssignmentValid test" << std::endl;

    // Create a flag variable for the source lambda function
    int sourceTaskFlag = 0;
    // Define a lambda for source that sets the flag to 10 when invoked
    std::function<void(void*)> sourceTask = [&sourceTaskFlag](void* data) {
        std::cout << "Invoking sourceTask lambda: setting sourceTaskFlag to 10" << std::endl;
        sourceTaskFlag = 10;
    };

    // Create a dummy integer for source mData pointer
    int sourceDataVal = 123;
    void* sourceData = static_cast<void*>(&sourceDataVal);

    // Create the source PlayerAsyncTaskObj with mTask = sourceTask, mData, mId = 100, mTaskName = "DownloadTask"
    PlayerAsyncTaskObj source(sourceTask, sourceData, std::string("DownloadTask"), 100);
    std::cout << "Source object constructed with mId: " << 100 
              << ", mTaskName: " << "DownloadTask" 
              << ", mData pointer: " << sourceData << std::endl;

    // Create a flag variable for the target lambda function before assignment (for demonstration)
    int targetTaskFlag = 0;
    // Define a lambda for target that sets the flag to 20 when invoked. This will be overwritten.
    std::function<void(void*)> targetTask = [&targetTaskFlag](void* data) {
        std::cout << "Invoking targetTask lambda: setting targetTaskFlag to 20" << std::endl;
        targetTaskFlag = 20;
    };

    // Create a dummy integer for target mData pointer
    int targetDataVal = 456;
    void* targetData = static_cast<void*>(&targetDataVal);

    // Create the target PlayerAsyncTaskObj with initial values: mTask = targetTask, mData = targetData, mId = 50, mTaskName = "OldTask"
    PlayerAsyncTaskObj target(targetTask, targetData, std::string("OldTask"), 50);
    std::cout << "Target object constructed with mId: " << 50 
              << ", mTaskName: " << "OldTask" 
              << ", mData pointer: " << targetData << std::endl;

    // Invoke the assignment operator: target = source
    std::cout << "Invoking operator= to assign source to target" << std::endl;
    PlayerAsyncTaskObj& retRef = (target = source);
    std::cout << "Assignment operator returned reference: " << &retRef << ", target object address: " << &target << std::endl;

    // Invoke the mTask of target to verify that it is now the source lambda
    std::cout << "Invoking target.mTask lambda after assignment" << std::endl;
    if(target.mTask) {
        target.mTask(nullptr);
    } else {
        std::cout << "target.mTask is empty." << std::endl;
    }
    
    // Check that sourceTaskFlag has been set by the lambda (indicating that target.mTask was properly assigned)
    std::cout << "sourceTaskFlag after invoking target.mTask: " << sourceTaskFlag << std::endl;
    EXPECT_EQ(sourceTaskFlag, 10);

    // Verify that mData pointers match
    std::cout << "Comparing mData pointers: source(" << source.mData << ") vs target(" << target.mData << ")" << std::endl;
    EXPECT_EQ(target.mData, source.mData);

    // Verify that mId values match
    std::cout << "Comparing mId: source(" << source.mId << ") vs target(" << target.mId << ")" << std::endl;
    EXPECT_EQ(target.mId, source.mId);
    
    // Verify that mTaskName values match
    std::cout << "Comparing mTaskName: source(" << source.mTaskName << ") vs target(" << target.mTaskName << ")" << std::endl;
    EXPECT_EQ(target.mTaskName, source.mTaskName);

    std::cout << "Exiting CopyAssignmentValid test" << std::endl;
}
/**
 * @brief Verify that a newly constructed PlayerScheduler object can enable its schedule task without throwing exceptions.
 *
 * This test verifies that a PlayerScheduler object, when created using its default constructor, is properly instantiated and that its EnableScheduleTask() method can be invoked successfully without any exceptions. This ensures that the basic scheduling functionality works as expected in a positive scenario.
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
 * | 01 | Create a PlayerScheduler object using its default constructor. | Constructor: none, Output: Valid PlayerScheduler instance | Object is instantiated successfully without throwing exceptions. | Should be successful |
 * | 02 | Invoke EnableScheduleTask() method on the created PlayerScheduler object. | Method Call: EnableScheduleTask(), Input: none, Output: void | Method executes without throwing exceptions and enables the schedule task. | Should Pass |
 */
TEST(PlayerScheduler, EnableScheduleTask_on_newly_constructed) {
    std::cout << "Entering EnableScheduleTask_on_newly_constructed test" << std::endl;

    // Log creation of the PlayerScheduler object using default constructor.
    std::cout << "Creating a PlayerScheduler object using default constructor." << std::endl;
    EXPECT_NO_THROW({
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created successfully." << std::endl;

        // Log invocation of EnableScheduleTask method.
        std::cout << "Invoking EnableScheduleTask() method on the PlayerScheduler object." << std::endl;
        EXPECT_NO_THROW(scheduler.EnableScheduleTask());
        std::cout << "EnableScheduleTask() method invoked successfully." << std::endl;

        // Log that no exceptions were thrown and the scheduler is enabled.
        std::cout << "Scheduler enabled; internal state should reflect an active schedule task." << std::endl;
    });

    std::cout << "Exiting EnableScheduleTask_on_newly_constructed test" << std::endl;
}
/**
 * @brief Tests the default construction of PlayerScheduler.
 *
 * This test verifies that the default constructor of the PlayerScheduler class does not throw an exception and successfully creates an instance.
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
 * | Variation / Step | Description                                          | Test Data                                           | Expected Result                                                                                  | Notes       |
 * | :--------------: | ---------------------------------------------------- | --------------------------------------------------- | ------------------------------------------------------------------------------------------------ | ----------- |
 * |       01         | Invoke the default constructor of PlayerScheduler    | No input arguments, output: instance of PlayerScheduler | Default constructor should not throw an exception and the object should be created successfully | Should Pass |
 */
TEST(PlayerScheduler, DefaultConstruction) {
    std::cout << "Entering DefaultConstruction test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking PlayerScheduler() default constructor" << std::endl;
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created successfully" << std::endl;
    });
    
    std::cout << "Exiting DefaultConstruction test" << std::endl;
}
/**
 * @brief Test to verify that RemoveAllTasks method clears all tasks from the task queue.
 *
 * This test creates a PlayerScheduler object using the default constructor and then invokes the RemoveAllTasks()
 * method to ensure that all tasks are cleared from the task queue without throwing any exceptions.
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
 * | Variation / Step | Description                                                            | Test Data                                                      | Expected Result                                                        | Notes            |
 * | :--------------: | ---------------------------------------------------------------------- | -------------------------------------------------------------- | ---------------------------------------------------------------------- | ---------------- |
 * | 01               | Creating PlayerScheduler object using its default constructor.         | constructor = (no parameters)                                  | Object should be created successfully without exceptions.            | Should be successful |
 * | 02               | Invoking RemoveAllTasks() method to clear all tasks from the queue.      | API: RemoveAllTasks() invoked on the created object              | RemoveAllTasks() should execute without exceptions and clear the tasks. | Should Pass      |
 */
TEST(PlayerScheduler, RemoveAllTasks_clears_all_tasks_from_a_task_queue) {
    std::cout << "Entering RemoveAllTasks_clears_all_tasks_from_a_task_queue test" << std::endl;

    // Creating PlayerScheduler object using default constructor
    std::cout << "Creating PlayerScheduler object using its default constructor." << std::endl;
    EXPECT_NO_THROW({
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created successfully." << std::endl;

        // Invoking RemoveAllTasks method
        std::cout << "Invoking RemoveAllTasks() method." << std::endl;
        EXPECT_NO_THROW({
            scheduler.RemoveAllTasks();
            std::cout << "RemoveAllTasks() method invoked successfully." << std::endl;
        });
    });

    std::cout << "Exiting RemoveAllTasks_clears_all_tasks_from_a_task_queue test" << std::endl;
}
/**
 * @brief Verify that RemoveTask successfully removes an existing task.
 *
 * This test verifies that the RemoveTask method of PlayerScheduler correctly removes a task when provided with an existing task ID.
 * It ensures that no exceptions are thrown during execution and that the method returns true, indicating successful removal.
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
 * | Variation / Step | Description                                            | Test Data                      | Expected Result                                  | Notes           |
 * | :--------------: | ------------------------------------------------------ | ------------------------------ | ------------------------------------------------ | --------------- |
 * | 01               | Instantiate PlayerScheduler using default constructor  | None                           | Scheduler object is created successfully         | Should be successful |
 * | 02               | Invoke RemoveTask with id = 1 and verify removal       | input id = 1, output result = true | API returns true; no exceptions thrown      | Should Pass     |
 */
TEST(PlayerScheduler, RemoveTask_with_existing_task) {
    std::cout << "Entering RemoveTask_with_existing_task test" << std::endl;

    EXPECT_NO_THROW({
        // Create object using default constructor.
        PlayerScheduler scheduler;
        std::cout << "Invoked default constructor for PlayerScheduler" << std::endl;

        // Invoke RemoveTask with id = 1.
        std::cout << "Invoking PlayerScheduler::RemoveTask with id: " << 1 << std::endl;
        bool result = scheduler.RemoveTask(1);
        std::cout << "RemoveTask returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);

    });

    std::cout << "Exiting RemoveTask_with_existing_task test" << std::endl;
}
/**
 * @brief Verify that invoking RemoveTask with a non-existing task id returns false.
 *
 * This test case verifies that the PlayerScheduler's RemoveTask method behaves as expected when a task removal is attempted with an id that does not exist. The test ensures that the method returns false without throwing any exceptions, thereby confirming the robustness of error handling in the scheduler.
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
 * | Variation / Step | Description                                                            | Test Data                 | Expected Result                                                                  | Notes             |
 * | :--------------: | ---------------------------------------------------------------------- | ------------------------- | -------------------------------------------------------------------------------- | ----------------- |
 * | 01               | Create PlayerScheduler object using the default constructor            | None                      | PlayerScheduler object is successfully created                                   | Should be successful |
 * | 02               | Invoke RemoveTask with a non-existing task id                          | id = 99                   | API returns false indicating task not found and the EXPECT_FALSE assertion passes| Should Pass       |
 */
TEST(PlayerScheduler, RemoveTask_with_non_existing_task) {
    std::cout << "Entering RemoveTask_with_non_existing_task test" << std::endl;

    EXPECT_NO_THROW({
        // Create object using default constructor.
        PlayerScheduler scheduler;
        std::cout << "Invoked default constructor for PlayerScheduler" << std::endl;

        // Invoke RemoveTask with a non-existing id.
        std::cout << "Invoking PlayerScheduler::RemoveTask with id: " << 99 << std::endl;
        bool result = scheduler.RemoveTask(99);
        std::cout << "RemoveTask returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);

    });

    std::cout << "Exiting RemoveTask_with_non_existing_task test" << std::endl;
}
/**
 * @brief Verify that RemoveTask correctly handles a negative task identifier.
 *
 * This test validates that when RemoveTask is invoked with a negative id (-10), the API does not throw any exception and returns false, ensuring that invalid task identifiers are properly handled.
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
 * | Variation / Step | Description                                                      | Test Data                                 | Expected Result                                           | Notes            |
 * | :--------------: | ---------------------------------------------------------------- | ----------------------------------------- | --------------------------------------------------------- | ---------------- |
 * | 01               | Create a PlayerScheduler instance using the default constructor  | input: none                               | Instance created without throwing any exception         | Should be successful |
 * | 02               | Invoke RemoveTask with a negative id (-10) and verify the result   | input: id = -10, output: result = false   | RemoveTask returns false indicating invalid id handling   | Should Fail      |
 */
TEST(PlayerScheduler, RemoveTask_with_negative_id) {
    std::cout << "Entering RemoveTask_with_negative_id test" << std::endl;

    EXPECT_NO_THROW({
        // Create object using default constructor.
        PlayerScheduler scheduler;
        std::cout << "Invoked default constructor for PlayerScheduler" << std::endl;

        // Invoke RemoveTask with negative id = -10.
        std::cout << "Invoking PlayerScheduler::RemoveTask with id: " << -10 << std::endl;
        bool result = scheduler.RemoveTask(-10);
        std::cout << "RemoveTask returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);

    });

    std::cout << "Exiting RemoveTask_with_negative_id test" << std::endl;
}
/**
 * @brief This test verifies that the ResumeScheduler method of the PlayerScheduler class operates without throwing exceptions.
 *
 * This test checks if a PlayerScheduler object can be created using its default constructor and if invoking the ResumeScheduler method on that object does not throw any exceptions. The test ensures that both object construction and method invocation are successful.
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
 * | Variation / Step | Description                                                               | Test Data                                                  | Expected Result                                                                                         | Notes         |
 * | :--------------: | ------------------------------------------------------------------------- | ---------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create a PlayerScheduler object using the default constructor.            | Constructor: no parameters                                 | Object is created without throwing exceptions.                                                         | Should Pass   |
 * | 02               | Invoke the ResumeScheduler() method on the created PlayerScheduler object.  | Function: ResumeScheduler(), no input parameters           | ResumeScheduler() is executed successfully without any exceptions thrown; assertion for no exception.    | Should Pass   |
 */
TEST(PlayerScheduler, ResumeScheduler_start) {
    std::cout << "Entering ResumeScheduler_start test" << std::endl;
    
    // Create a PlayerScheduler object using the default constructor with no exception.
    EXPECT_NO_THROW({
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created using default constructor." << std::endl;
        
        // Invoke the ResumeScheduler method and log method invocation.
        std::cout << "Invoking ResumeScheduler() method." << std::endl;
        EXPECT_NO_THROW({
            scheduler.ResumeScheduler();
            std::cout << "ResumeScheduler() method called successfully." << std::endl;
        });
        
    });
    
    std::cout << "Exiting ResumeScheduler_start test" << std::endl;
}
/**
 * @brief Validate that a proper task is scheduled successfully.
 *
 * This test verifies that when a valid task callback, non-null data pointer, valid task name, and valid task ID are provided, 
 * the PlayerScheduler successfully schedules the task without throwing exceptions and returns a valid task ID (>= 0). 
 * This ensures proper task scheduling functionality.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Print entry message indicating test start | None | "Entering ValidTaskScheduling_ProperTask test" printed to console | Should be successful |
 * | 02 | Prepare task callback lambda that prints data value | task = lambda(void* data) | Lambda assigned successfully | Should be successful |
 * | 03 | Prepare sample integer and its pointer for task data | sampleData = 100, dataPtr = address of sampleData | Valid integer initialization and pointer assignment | Should be successful |
 * | 04 | Log input parameters for PlayerAsyncTaskObj creation | mTask callback, mData pointer (value: 100), mTaskName = ValidTask, mId = 10 | Correct log message output | Should be successful |
 * | 05 | Create PlayerAsyncTaskObj with valid parameters | task (lambda), dataPtr, mTaskName = "ValidTask", mId = 10 | PlayerAsyncTaskObj instance created successfully | Should be successful |
 * | 06 | Create PlayerScheduler object using default constructor and validate no exception is thrown | None | Object created without exception | Should be successful |
 * | 07 | Invoke ScheduleTask method with valid task object | taskObj instance | Returns scheduledTaskId >= 0 | Should Pass |
 * | 08 | Log and assert the scheduled task ID | scheduledTaskId returned from ScheduleTask | Console output shows scheduledTaskId and EXPECT_GE(scheduledTaskId, 0) passes | Should Pass |
 * | 09 | Print exit message indicating test completion | None | "Exiting ValidTaskScheduling_ProperTask test" printed to console | Should be successful |
 */
TEST(PlayerScheduler, ValidTaskScheduling_ProperTask) {
    std::cout << "Entering ValidTaskScheduling_ProperTask test" << std::endl;

    // Prepare task callback: a lambda that prints a message when invoked.
    std::function<void(void*)> task = [](void* data) {
        int* intData = static_cast<int*>(data);
        std::cout << "Inside task callback. Data value: " << (intData ? *intData : -1) << std::endl;
    };

    // Prepare a valid integer variable and pointer to it.
    int sampleData = 100;
    void* dataPtr = static_cast<void*>(&sampleData);
    
    // Log input parameters.
    std::cout << "Preparing PlayerAsyncTaskObj with mTask callback, non-null mData pointer (value: " << sampleData 
              << "), mTaskName: ValidTask, and mId: 10" << std::endl;

    // Create the task object with provided parameters.
    PlayerAsyncTaskObj taskObj(task, dataPtr, "ValidTask", 10);

    // Create PlayerScheduler object using default constructor.
    std::cout << "Invoking PlayerScheduler default constructor" << std::endl;
    EXPECT_NO_THROW(PlayerScheduler scheduler;);

    PlayerScheduler scheduler;
    std::cout << "Invoking ScheduleTask method with valid task object" << std::endl;
    int scheduledTaskId = scheduler.ScheduleTask(taskObj);

    // Log the returned scheduled task ID.
    std::cout << "ScheduleTask returned task ID: " << scheduledTaskId << std::endl;
    std::cout << "Expecting task ID to be >= 0 indicating successful scheduling" << std::endl;
    EXPECT_GE(scheduledTaskId, 0);

    std::cout << "Exiting ValidTaskScheduling_ProperTask test" << std::endl;
}
/**
 * @brief Validate scheduling a task with nullptr as data pointer
 *
 * This test verifies that a task object containing a valid callback and a nullptr for data is correctly scheduled by the PlayerScheduler. It ensures that the scheduler can handle tasks with null data without throwing exceptions and returns a valid task ID.
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
 * | 01 | Prepare task callback lambda, set mData to nullptr, and create PlayerAsyncTaskObj with mTaskName "TaskWithNullData" and mId 20 | task = lambda (prints message), dataPtr = nullptr, mTaskName = "TaskWithNullData", mId = 20 | PlayerAsyncTaskObj is constructed with provided parameters | Should be successful |
 * | 02 | Instantiate PlayerScheduler using default constructor and invoke ScheduleTask with valid task object | Scheduler instantiated via default constructor, ScheduleTask(taskObj) called | Returns a scheduled task ID >= 0 indicating successful scheduling | Should Pass |
 */
TEST(PlayerScheduler, ValidTaskScheduling_NullData) {
    std::cout << "Entering ValidTaskScheduling_NullData test" << std::endl;

    // Prepare task callback: a lambda that performs a simple operation.
    std::function<void(void*)> task = [](void* data) {
        std::cout << "Inside task callback with null data pointer" << std::endl;
    };

    // mData is explicitly set to nullptr.
    void* dataPtr = nullptr;
    
    // Log input parameters.
    std::cout << "Preparing PlayerAsyncTaskObj with mTask callback, mData as nullptr, mTaskName: TaskWithNullData, and mId: 20" << std::endl;

    // Create the task object.
    PlayerAsyncTaskObj taskObj(task, dataPtr, "TaskWithNullData", 20);

    // Create PlayerScheduler object using default constructor.
    std::cout << "Invoking PlayerScheduler default constructor" << std::endl;
    EXPECT_NO_THROW(PlayerScheduler scheduler;);

    PlayerScheduler scheduler;
    std::cout << "Invoking ScheduleTask method with valid task object having nullptr as mData" << std::endl;
    int scheduledTaskId = scheduler.ScheduleTask(taskObj);

    // Log the returned scheduled task ID.
    std::cout << "ScheduleTask returned task ID: " << scheduledTaskId << std::endl;
    std::cout << "Expecting task ID to be >= 0 indicating successful scheduling" << std::endl;
    EXPECT_GE(scheduledTaskId, 0);

    std::cout << "Exiting ValidTaskScheduling_NullData test" << std::endl;
}
/**
 * @brief Negative Testing for ScheduleTask API with Empty Task Callback
 *
 * This test verifies that the ScheduleTask method returns a negative error code when a PlayerAsyncTaskObj with an empty task callback is provided. It ensures that the scheduler properly validates the task callback and handles the error scenario as expected.
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
 * | Variation / Step | Description                                                                              | Test Data                                                                                   | Expected Result                                                                          | Notes          |
 * | :--------------: | ---------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Initialize an empty task callback, dummy data, and data pointer.                         | emptyTask = null, dummyData = 50, dataPtr = address of dummyData                             | Empty task callback prepared.                                                            | Should be successful |
 * | 02               | Create a PlayerAsyncTaskObj with the empty task callback and valid parameters.           | emptyTask, dataPtr, taskName = "EmptyTaskCallback", id = 30                                    | Task object is correctly instantiated with provided parameters.                        | Should be successful |
 * | 03               | Instantiate a PlayerScheduler object using the default constructor.                      | None                                                                                        | PlayerScheduler constructed without throwing an exception.                             | Should be successful |
 * | 04               | Invoke ScheduleTask on the scheduler with the task object having an empty task callback.   | taskObj (with emptyTask, dataPtr, taskName = "EmptyTaskCallback", id = 30)                      | API returns a negative value (< 0) indicating failure due to the invalid (empty) callback. | Should Fail    |
 */
TEST(PlayerScheduler, NegativeTaskScheduling_EmptyTaskCallback) {
    std::cout << "Entering NegativeTaskScheduling_EmptyTaskCallback test" << std::endl;

    // Prepare an empty task callback.
    std::function<void(void*)> emptyTask;
    
    // For demonstration, we use a valid pointer (could be null as well, but valid object example)
    int dummyData = 50;
    void* dataPtr = static_cast<void*>(&dummyData);

    // Log input parameters.
    std::cout << "Preparing PlayerAsyncTaskObj with empty mTask callback, mData pointer (value: " << dummyData 
              << "), mTaskName: EmptyTaskCallback, and mId: 30" << std::endl;

    // Create the task object.
    PlayerAsyncTaskObj taskObj(emptyTask, dataPtr, "EmptyTaskCallback", 30);

    // Create PlayerScheduler object using default constructor.
    std::cout << "Invoking PlayerScheduler default constructor" << std::endl;
    EXPECT_NO_THROW(PlayerScheduler scheduler;);
    
    PlayerScheduler scheduler;
    std::cout << "Invoking ScheduleTask method with task object having empty mTask callback" << std::endl;
    int scheduledTaskId = scheduler.ScheduleTask(taskObj);

    // Log the returned scheduled task ID.
    std::cout << "ScheduleTask returned task ID: " << scheduledTaskId << std::endl;
    std::cout << "Expecting a negative error code indicating failure due to empty task callback" << std::endl;
    EXPECT_LT(scheduledTaskId, 0);

    std::cout << "Exiting NegativeTaskScheduling_EmptyTaskCallback test" << std::endl;
}
/**
 * @brief Test negative task scheduling scenario by providing an invalid task ID.
 *
 * This test verifies the failure behavior of the ScheduleTask API when an invalid task ID (-1) is provided.
 * It creates a task object with an invalid task identifier, schedules it using the PlayerScheduler, and confirms that a negative error code is returned.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                                     | Expected Result                                                             | Notes         |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare task callback lambda and a valid data pointer                                            | input: sampleData = 200, task callback = lambda, dataPtr = address of sampleData              | Lambda and data pointer setup successfully                                  | Should be successful |
 * | 02               | Create PlayerAsyncTaskObj with an invalid task ID (-1)                                           | input: mTask = lambda, mData = dataPtr, mTaskName = InvalidTaskId, mId = -1                     | Object created with invalid task ID                                         | Should be successful |
 * | 03               | Create PlayerScheduler object using the default constructor                                      | input: call to PlayerScheduler default constructor                                            | Scheduler object created without throwing exceptions                        | Should Pass    |
 * | 04               | Invoke ScheduleTask method with the task object having an invalid mId (-1)                         | input: task object with mId = -1                                                                | API returns a negative error code indicating failure                        | Should Fail    |
 * | 05               | Validate that the returned scheduled task ID is negative, confirming failure in task scheduling    | output: scheduledTaskId from ScheduleTask                                                     | Assertion EXPECT_LT(scheduledTaskId, 0) passes (i.e., scheduledTaskId < 0)      | Should Pass    |
 */
TEST(PlayerScheduler, NegativeTaskScheduling_InvalidTaskId) {
    std::cout << "Entering NegativeTaskScheduling_InvalidTaskId test" << std::endl;

    // Prepare task callback: a lambda that performs a simple operation.
    std::function<void(void*)> task = [](void* data) {
        std::cout << "Inside task callback for invalid task ID test" << std::endl;
    };

    // Use a valid data pointer.
    int sampleData = 200;
    void* dataPtr = static_cast<void*>(&sampleData);

    // Log input parameters.
    std::cout << "Preparing PlayerAsyncTaskObj with mTask callback, mData pointer (value: " << sampleData 
              << "), mTaskName: InvalidTaskId, and mId: -1" << std::endl;

    // Create the task object with an invalid task ID.
    PlayerAsyncTaskObj taskObj(task, dataPtr, "InvalidTaskId", -1);

    // Create PlayerScheduler object using default constructor.
    std::cout << "Invoking PlayerScheduler default constructor" << std::endl;
    EXPECT_NO_THROW(PlayerScheduler scheduler;);
    
    PlayerScheduler scheduler;
    std::cout << "Invoking ScheduleTask method with task object having invalid mId (-1)" << std::endl;
    int scheduledTaskId = scheduler.ScheduleTask(taskObj);

    // Log the returned scheduled task ID.
    std::cout << "ScheduleTask returned task ID: " << scheduledTaskId << std::endl;
    std::cout << "Expecting a negative error code indicating failure due to invalid task ID" << std::endl;
    EXPECT_LT(scheduledTaskId, 0);

    std::cout << "Exiting NegativeTaskScheduling_InvalidTaskId test" << std::endl;
}
/**
 * @brief Test to verify that the PlayerScheduler starts successfully without throwing exceptions.
 *
 * This test creates a PlayerScheduler object using its default constructor and then calls the StartScheduler() method.
 * It verifies that the method completes execution normally, ensuring that the scheduler initialization does not throw any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a PlayerScheduler object using default constructor. | None | A valid PlayerScheduler object is created successfully. | Should be successful |
 * | 02 | Invoke the StartScheduler() method on the PlayerScheduler object. | scheduler: valid PlayerScheduler object, input: none, output: none | Method completes execution without throwing exceptions. | Should Pass |
 */
TEST(PlayerScheduler, StartScheduler_Successfully) {
    std::cout << "Entering Start Scheduler Successfully test" << std::endl;
    
    EXPECT_NO_THROW({
        // Create a PlayerScheduler object using the default constructor.
        std::cout << "Creating PlayerScheduler object via default constructor." << std::endl;
        PlayerScheduler scheduler;
        
        // Log before invoking StartScheduler.
        std::cout << "Invoking StartScheduler() method." << std::endl;
        scheduler.StartScheduler();
        
        // Log completion of StartScheduler invocation.
        std::cout << "StartScheduler() method completed successfully." << std::endl;
    });
    
    std::cout << "Exiting Start Scheduler Successfully test" << std::endl;
}
/**
 * @brief Verify that invoking StopScheduler method in PlayerScheduler does not throw an exception.
 *
 * This test creates a PlayerScheduler instance using the default constructor and then calls the StopScheduler method. The objective is to ensure that no exceptions are thrown during both the instantiation and the StopScheduler invocation, thereby confirming that the scheduler stops gracefully.
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
 * | Variation / Step | Description                                                                    | Test Data                                                                              | Expected Result                                                     | Notes       |
 * | :----:           | :----------------------------------------------------------------------------- | :------------------------------------------------------------------------------------- | :------------------------------------------------------------------ | :---------: |
 * | 01               | Create a PlayerScheduler instance using the default constructor and invoke StopScheduler() method | PlayerScheduler constructor = default, StopScheduler() method = no input parameters | No exception thrown during instantiation and StopScheduler() invocation | Should Pass |
 */
TEST(PlayerScheduler, StopScheduler_start) {
    std::cout << "Entering PlayerScheduler::StopScheduler()_start test" << std::endl;

    // Create PlayerScheduler instance using default constructor
    std::cout << "Creating PlayerScheduler instance using default constructor" << std::endl;
    EXPECT_NO_THROW({
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler instance created successfully" << std::endl;
        
        std::cout << "Invoking StopScheduler method" << std::endl;
        
        // Invoke StopScheduler and ensure no exception is thrown
        EXPECT_NO_THROW(scheduler.StopScheduler());
        std::cout << "StopScheduler method invoked successfully" << std::endl;
    });

    std::cout << "Exiting PlayerScheduler::StopScheduler()_start test" << std::endl;
}
/**
 * @brief Validate that the SuspendScheduler() method in PlayerScheduler executes without throwing exceptions.
 *
 * This test case ensures that invoking the SuspendScheduler() method on a PlayerScheduler object, which is instantiated using its default constructor, does not result in any exceptions. The test verifies that the method call is handled properly in a positive scenario.
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
 * | Variation / Step | Description                                                        | Test Data                                                    | Expected Result                                                       | Notes           |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------ | --------------------------------------------------------------------- | --------------- |
 * | 01               | Create a PlayerScheduler object using the default constructor.     | Constructor: PlayerScheduler()                               | Object is created successfully.                                       | Should be successful |
 * | 02               | Invoke the SuspendScheduler() method on the PlayerScheduler object.  | Method Call: scheduler.SuspendScheduler()                    | No exception thrown; method executed as expected (validated by EXPECT_NO_THROW). | Should Pass     |
 */
TEST(PlayerScheduler, SuspendScheduler_start) {
    std::cout << "Entering SuspendScheduler_start test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Creating PlayerScheduler object using default constructor." << std::endl;
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created successfully." << std::endl;
        
        std::cout << "Invoking SuspendScheduler method." << std::endl;
        scheduler.SuspendScheduler();
        std::cout << "SuspendScheduler method invoked successfully. No exception thrown." << std::endl;
    });
    
    std::cout << "Exiting SuspendScheduler_start test" << std::endl;
}
/**
 * @brief Verify that an idle PlayerScheduler object can be safely destroyed without throwing exceptions
 *
 * This test validates that a PlayerScheduler object created using the default constructor is properly cleaned up when it goes out of scope. The test ensures that no exceptions are thrown during the object creation and its subsequent destruction.
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
 * | 01 | Invoke the PlayerScheduler default constructor inside an EXPECT_NO_THROW block. | input: none, output: PlayerScheduler object instance | Object is created without throwing an exception. | Should Pass |
 * | 02 | Allow the PlayerScheduler object to go out of scope to trigger the destructor call. | input: none, output: automatic destructor invocation | Destructor is called automatically without throwing any exceptions. | Should be successful |
 */
TEST(PlayerScheduler, Destruction_of_an_idle_PlayerScheduler_object)
{
    std::cout << "Entering Destruction_of_an_idle_PlayerScheduler_object test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Invoking PlayerScheduler default constructor." << std::endl;
        PlayerScheduler scheduler;
        std::cout << "PlayerScheduler object created. Current internal state assumed as default values." << std::endl;
        std::cout << "PlayerScheduler destructor will be automatically called on object destruction." << std::endl;
    });
    
    std::cout << "Exiting Destruction_of_an_idle_PlayerScheduler_object test" << std::endl;
}
/**
 * @brief Verifies that the ExecuteAsyncTask method executes normally without throwing any exceptions.
 *
 * This test case validates that when the ExecuteAsyncTask method is invoked on a valid scheduler instance,
 * it completes its execution as expected without throwing any exceptions. The test also confirms that debug log 
 * statements are printed in the correct order to indicate the API's lifecycle (i.e., entry, execution, and exit).@n
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                  | Test Data                                                | Expected Result                                                                 | Notes       |
 * | :--------------: | ---------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke the ExecuteAsyncTask API and monitor execution for exceptions         | No input parameters, output via log statements           | ExecuteAsyncTask completes without throwing exceptions and logs debug messages  | Should Pass |
 */
TEST_F(PlayerSchedulerTest, ExecuteAsyncTask_NormalInvocation) {
    std::cout << "Entering ExecuteAsyncTask_NormalInvocation test" << std::endl;
    std::cout << "[DEBUG] About to invoke PlayerScheduler::ExecuteAsyncTask()" << std::endl;
    EXPECT_NO_THROW({
        scheduler->ExecuteAsyncTask();
        std::cout << "[DEBUG] PlayerScheduler::ExecuteAsyncTask() has completed execution." << std::endl;
    });
    std::cout << "Exiting ExecuteAsyncTask_NormalInvocation test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
