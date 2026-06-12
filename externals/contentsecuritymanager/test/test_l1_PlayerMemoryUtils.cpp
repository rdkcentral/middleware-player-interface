
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
#include "PlayerMemoryUtils.h"


/**
 * @brief Test the functionality of player_CleanUpSharedMem when provided with valid shared memory parameters
 *
 * This test case verifies that when a valid memory pointer along with correct shared memory key and length are provided,
 * the player_CleanUpSharedMem function executes without throwing an exception. This ensures that the cleanup operation
 * for shared memory parameters is carried out correctly.
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
 * | Variation / Step | Description                                                                       | Test Data                                                      | Expected Result                                                      | Notes         |
 * | :--------------: | --------------------------------------------------------------------------------- | -------------------------------------------------------------- | -------------------------------------------------------------------- | ------------- |
 * | 01               | Allocate memory and prepare valid shared memory parameters                        | shmPointer allocated with size = 1024, shmKey = 0x1234, shmLen = 1024 | Memory allocation successful; parameters set correctly               | Should be successful |
 * | 02               | Invoke player_CleanUpSharedMem using the valid shared memory parameters             | Input: shmPointer, shmKey = 0x1234, shmLen = 1024               | Function executes without throwing exceptions                        | Should Pass   |
 */
TEST(CleanupWithValidSharedMemoryParameters, TestCleanupWithValidSharedMemoryParameters) {
    std::cout << "Entering CleanupWithValidSharedMemoryParameters test" << std::endl;
    void* shmPointer = malloc(1024);
    if (shmPointer == NULL) {
        perror("malloc failed");
        exit(1);
    }
    std::cout << "Allocated memory at pointer: " << shmPointer << std::endl;
    
    key_t shmKey = 0x1234;
    size_t shmLen = 1024;
    std::cout << "Invoking player_CleanUpSharedMem with shmKey: " << shmKey << ", shmLen: " << shmLen << std::endl;
    EXPECT_NO_THROW({
        player_CleanUpSharedMem(shmPointer, shmKey, shmLen);
        std::cout << "player_CleanUpSharedMem executed without error" << std::endl;
    });
    
    free(shmPointer);
    std::cout << "Exiting CleanupWithValidSharedMemoryParameters test" << std::endl;
}
/**
 * @brief Verify that player_CleanUpSharedMem handles a NULL pointer gracefully.
 *
 * This test verifies that calling player_CleanUpSharedMem with a NULL pointer does not throw any exceptions and handles the scenario properly without causing a crash. It ensures that the function is robust when provided with invalid shared memory pointers.
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
 * | Variation / Step | Description                                             | Test Data                                                        | Expected Result                                                        | Notes       |
 * | :----:           | ------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke player_CleanUpSharedMem using a NULL pointer.    | shmPointer = NULL, shmKey = 0x1234, shmLen = 1024                 | Function call does not throw any exceptions, and returns a safe cleanup. | Should Pass |
 */
TEST(CleanupWithNullPointer, TestCleanupWithNullPointer) {
    std::cout << "Entering CleanupWithNullPointer test" << std::endl;
    void* shmPointer = NULL;
    std::cout << "Using NULL pointer for shmPointer" << std::endl;
    key_t shmKey = 0x1234;
    size_t shmLen = 1024;
    std::cout << "Invoking player_CleanUpSharedMem with shmKey: " << shmKey << ", shmLen: " << shmLen << std::endl;
    EXPECT_NO_THROW({
        player_CleanUpSharedMem(shmPointer, shmKey, shmLen);
        std::cout << "player_CleanUpSharedMem handled NULL pointer gracefully" << std::endl;
    });
    std::cout << "Exiting CleanupWithNullPointer test" << std::endl;
}
/**
 * @brief Test to ensure cleanup function correctly handles a scenario with a zero shared memory length.
 *
 * This test verifies that the player_CleanUpSharedMem function can gracefully handle a situation where the shared memory length parameter is zero.
 * It ensures that even when the shared memory length is zero, the function does not throw any exceptions or exhibit erroneous behavior.
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
 * | Variation / Step | Description                                                         | Test Data                                                             | Expected Result                                                       | Notes        |
 * | :--------------: | ------------------------------------------------------------------- | --------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------ |
 * | 01               | Allocate memory using malloc, and invoke player_CleanUpSharedMem with shmKey = 0x1234 and shmLen = 0 to test cleanup behavior with zero shared memory length | shmPointer = pointer from malloc(1024), shmKey = 0x1234, shmLen = 0 | API should complete without throwing any exception and handle the zero shared memory length gracefully | Should Pass |
 */
TEST(CleanupWithZeroSharedMemoryLength, TestCleanupWithZeroSharedMemoryLength) {
    std::cout << "Entering CleanupWithZeroSharedMemoryLength test" << std::endl;
    void* shmPointer = malloc(1024);
    if (shmPointer == NULL) {
        perror("malloc failed");
        exit(1);
    }
    std::cout << "Allocated memory at pointer: " << shmPointer << std::endl;
    
    key_t shmKey = 0x1234;
    size_t shmLen = 0;
    std::cout << "Invoking player_CleanUpSharedMem with shmKey: " << shmKey << ", shmLen: " << shmLen << " (zero length)" << std::endl;
    EXPECT_NO_THROW({
        player_CleanUpSharedMem(shmPointer, shmKey, shmLen);
        std::cout << "player_CleanUpSharedMem handled zero length parameter gracefully" << std::endl;
    });
    
    free(shmPointer);
    std::cout << "Exiting CleanupWithZeroSharedMemoryLength test" << std::endl;
}
/**
 * @brief Validate that player_CleanUpSharedMem handles an invalid shared memory key gracefully.
 *
 * This test verifies that the API player_CleanUpSharedMem does not throw an exception when invoked
 * with an invalid shared memory key. The test ensures proper memory allocation, API invocation with the
 * invalid key, and memory cleanup to confirm that even invalid inputs are managed without causing runtime errors.
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
 * | Variation / Step | Description                                                              | Test Data                                      | Expected Result                                                        | Notes           |
 * | :--------------: | ------------------------------------------------------------------------ | ---------------------------------------------- | ---------------------------------------------------------------------- | --------------- |
 * | 01               | Allocate 1024 bytes of memory using malloc                               | malloc size = 1024                             | Memory is allocated successfully and pointer is not NULL               | Should be successful |
 * | 02               | Configure invalid shared memory key and valid shared memory length         | shmKey = -1, shmLen = 1024                       | Invalid key is set while shared memory length is defined                 | Should be successful |
 * | 03               | Invoke player_CleanUpSharedMem API with the invalid shared memory key      | shmPointer = <pointer>, shmKey = -1, shmLen = 1024 | API call does not throw any exception                                   | Should Pass     |
 * | 04               | Free the allocated memory                                                | free(shmPointer)                               | Allocated memory is freed successfully                                 | Should be successful |
 */
TEST(CleanupWithInvalidSharedMemoryKey, TestCleanupWithInvalidSharedMemoryKey) {
    std::cout << "Entering CleanupWithInvalidSharedMemoryKey test" << std::endl;
    void* shmPointer = malloc(1024);
    if (shmPointer == NULL) {
        perror("malloc failed");
        exit(1);
    }
    std::cout << "Allocated memory at pointer: " << shmPointer << std::endl;
    
    key_t shmKey = -1;
    size_t shmLen = 1024;
    std::cout << "Invoking player_CleanUpSharedMem with invalid shmKey: " << shmKey << ", shmLen: " << shmLen << std::endl;
    EXPECT_NO_THROW({
        player_CleanUpSharedMem(shmPointer, shmKey, shmLen);
        std::cout << "player_CleanUpSharedMem processed the invalid key input gracefully" << std::endl;
    });
    
    free(shmPointer);
    std::cout << "Exiting CleanupWithInvalidSharedMemoryKey test" << std::endl;
}
/**
 * @brief Verify that player_CleanUpSharedMem handles an extremely large shared memory length gracefully
 *
 * This test verifies that the player_CleanUpSharedMem API correctly processes an input scenario where the shared memory length is set to SIZE_MAX. The test ensures that the API does not throw any exceptions while cleaning up memory, even with an extremely large length. This scenario is critical to ensure system robustness when handling edge case memory parameters.
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
 * | Variation / Step | Description                                                    | Test Data                                             | Expected Result                                                    | Notes           |
 * | :--------------: | -------------------------------------------------------------- | ----------------------------------------------------- | ------------------------------------------------------------------ | --------------- |
 * | 01               | Allocate a block of memory using malloc                        | input: size = 1024, output: shmPointer = non-null value | Memory allocation successful; shmPointer is not NULL              | Should be successful |
 * | 02               | Invoke player_CleanUpSharedMem with allocated memory, shmKey=0x1234, and shmLen=SIZE_MAX | input: shmPointer = valid pointer, shmKey = 0x1234, shmLen = SIZE_MAX | API call executes without throwing exceptions or errors           | Should Pass     |
 * | 03               | Free the allocated memory                                        | input: shmPointer = valid allocated pointer           | Memory successfully freed                                          | Should be successful |
 */
TEST(CleanupWithExtremelyLargeSharedMemoryLength, TestCleanupWithExtremelyLargeSharedMemoryLength) {
    std::cout << "Entering CleanupWithExtremelyLargeSharedMemoryLength test" << std::endl;
    void* shmPointer = malloc(1024);
    if (shmPointer == NULL) {
        perror("malloc failed");
        exit(1);
    }
    std::cout << "Allocated memory at pointer: " << shmPointer << std::endl;
    
    key_t shmKey = 0x1234;
    size_t shmLen = SIZE_MAX;
    std::cout << "Invoking player_CleanUpSharedMem with shmKey: " << shmKey << ", extremely large shmLen: " << shmLen << std::endl;
    EXPECT_NO_THROW({
        player_CleanUpSharedMem(shmPointer, shmKey, shmLen);
        std::cout << "player_CleanUpSharedMem handled the extremely large length gracefully" << std::endl;
    });
    
    free(shmPointer);
    std::cout << "Exiting CleanupWithExtremelyLargeSharedMemoryLength test" << std::endl;
}
/**
 * @brief Validate that player_CreateSharedMem creates shared memory successfully.
 *
 * This test ensures that the API player_CreateSharedMem returns a non-null memory pointer and modifies the shared memory key, demonstrating successful shared memory creation. It verifies that no exception is thrown during the process.
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
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke player_CreateSharedMem with defined shmLen and shmKey values, then verify the returned pointer and the updated key. | shmLen = 1024, shmKey = 5000, originalKey = 5000 | API call does not throw exception; returned memPtr is not nullptr; updated shmKey is not equal to originalKey. | Should Pass |
 */
TEST(Player_CreateSharedMem_Test, ValidSharedMemoryCreation) {
    std::cout << "Entering Valid Shared Memory Creation test" << std::endl;
    size_t shmLen = 1024;
    key_t originalKey = 5000;
    key_t shmKey = originalKey;
    
    std::cout << "Invoking player_CreateSharedMem with shmLen = " << shmLen 
              << " and shmKey = " << shmKey << std::endl;
    void* memPtr = nullptr;
    EXPECT_NO_THROW(memPtr = player_CreateSharedMem(shmLen, shmKey));
    
    std::cout << "Returned pointer address: " << memPtr << std::endl;
    std::cout << "Updated shmKey value: " << shmKey << std::endl;
    EXPECT_NE(memPtr, nullptr);
    EXPECT_NE(shmKey, originalKey);
    
    std::cout << "Exiting Valid Shared Memory Creation test" << std::endl;
}
/**
 * @brief Test player_CreateSharedMem API with a zero-length buffer request.
 *
 * This test verifies that when a zero-length buffer request is made using the player_CreateSharedMem API,
 * the function does not allocate any memory and returns a nullptr. It also ensures that the shared memory key remains unchanged.
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
 * | Variation / Step | Description                                                            | Test Data                                            | Expected Result                                                                                       | Notes       |
 * | :--------------: | ---------------------------------------------------------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Invoke player_CreateSharedMem API with shmLen as 0 and a valid shmKey   | input: shmLen = 0, shmKey = 1234, output: pointer = nullptr | API returns nullptr without throwing an exception and the shmKey remains as 1234 after call | Should Pass |
 */
TEST(Player_CreateSharedMem_Test, ZeroLengthBufferRequest) {
    std::cout << "Entering Zero Length Buffer Request test" << std::endl;
    size_t shmLen = 0;
    key_t originalKey = 1234;
    key_t shmKey = originalKey;
    
    std::cout << "Invoking player_CreateSharedMem with shmLen = " << shmLen 
              << " and shmKey = " << shmKey << std::endl;
    void* memPtr = nullptr;
    EXPECT_NO_THROW(memPtr = player_CreateSharedMem(shmLen, shmKey));
    
    std::cout << "Returned pointer address: " << memPtr << std::endl;
    std::cout << "shmKey value after call: " << shmKey << std::endl;
    EXPECT_EQ(memPtr, nullptr);
    EXPECT_EQ(shmKey, originalKey);
    
    std::cout << "Exiting Zero Length Buffer Request test" << std::endl;
}
/**
 * @brief Test minimal buffer allocation and key update using player_CreateSharedMem API
 *
 * This test verifies that the player_CreateSharedMem function correctly allocates memory when provided the minimal buffer size and updates the shared memory key. It checks that no exception is thrown during allocation, the returned memory pointer is non-null, and that the key is updated from its original value.
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
 * | Variation / Step | Description                                                                 | Test Data                                         | Expected Result                                                                                  | Notes         |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Initialize shared memory length and key values                              | shmLen = 1, originalKey = 2000, shmKey = 2000       | Variables are set with initial values                                                            | Should be successful |
 * | 02               | Invoke player_CreateSharedMem API to allocate shared memory with minimal size | Input: shmLen = 1, shmKey = 2000; Output: memPtr, updated shmKey                       | API call does not throw an exception, returns a non-null pointer, and updates shmKey           | Should Pass   |
 * | 03               | Validate that the allocated memory pointer is not null and shmKey has changed  | Validation: memPtr != nullptr, shmKey != originalKey | Assertions pass confirming pointer allocation and key update (shmKey != originalKey)             | Should Pass   |
 */
TEST(Player_CreateSharedMem_Test, MinimalBufferSizeAllocation) {
    std::cout << "Entering Minimal Buffer Size Allocation test" << std::endl;
    size_t shmLen = 1;
    key_t originalKey = 2000;
    key_t shmKey = originalKey;
    
    std::cout << "Invoking player_CreateSharedMem with shmLen = " << shmLen 
              << " and shmKey = " << shmKey << std::endl;
    void* memPtr = nullptr;
    EXPECT_NO_THROW(memPtr = player_CreateSharedMem(shmLen, shmKey));
    
    std::cout << "Returned pointer address: " << memPtr << std::endl;
    std::cout << "Updated shmKey value: " << shmKey << std::endl;
    EXPECT_NE(memPtr, nullptr);
    EXPECT_NE(shmKey, originalKey);
    
    std::cout << "Exiting Minimal Buffer Size Allocation test" << std::endl;
}
/**
 * @brief Test the negative scenario for creating shared memory with an invalid key value.
 *
 * This test verifies that the API player_CreateSharedMem returns a nullptr when invoked with a negative shared memory key, 
 * ensuring that the invalid input is correctly handled by the function. The shared memory length and key are provided as inputs, 
 * and the returned pointer is validated along with ensuring that the shared memory key value remains unchanged.
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
 * | Variation / Step | Description                                                       | Test Data                                  | Expected Result                                                                 | Notes         |
 * | :--------------: | ----------------------------------------------------------------- | ------------------------------------------ | ------------------------------------------------------------------------------- | ------------- |
 * | 01               | Invoke player_CreateSharedMem with shmLen = 1024 and shmKey = -10    | shmLen = 1024, shmKey = -10, output memPtr = nullptr | The function should return a nullptr and the shmKey should remain unchanged at -10 | Should Fail   |
 */
TEST(Player_CreateSharedMem_Test, NegativeSharedMemoryKeyValue) {
    std::cout << "Entering Negative Shared Memory Key Value test" << std::endl;
    size_t shmLen = 1024;
    key_t originalKey = -10;
    key_t shmKey = originalKey;
    
    std::cout << "Invoking player_CreateSharedMem with shmLen = " << shmLen 
              << " and shmKey = " << shmKey << std::endl;
    void* memPtr = nullptr;
    EXPECT_NO_THROW(memPtr = player_CreateSharedMem(shmLen, shmKey));
    
    std::cout << "Returned pointer address: " << memPtr << std::endl;
    std::cout << "shmKey value after call: " << shmKey << std::endl;
    EXPECT_EQ(memPtr, nullptr);
    EXPECT_EQ(shmKey, originalKey);
    
    std::cout << "Exiting Negative Shared Memory Key Value test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
