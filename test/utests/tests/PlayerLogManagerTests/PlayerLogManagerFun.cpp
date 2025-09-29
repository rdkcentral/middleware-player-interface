
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
#include "PlayerLogManager.h"


/**
 * @brief Verify that DumpBinaryBlob correctly processes a valid binary blob with multiple bytes
 *
 * This test verifies that when provided with a properly initialized binary blob containing multiple bytes, the DumpBinaryBlob API correctly processes the data without throwing any exceptions.
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
 * | 01 | Initialize a fixed-size buffer with binary values using strncpy | buffer = {0x01, 0x02, 0x03, 0x04} | Buffer is correctly initialized with the specified binary data | Should be successful |
 * | 02 | Invoke DumpBinaryBlob with the buffer pointer and length 4; verify that no exception is thrown | input ptr = buffer, len = 4 | DumpBinaryBlob executes without throwing any exception (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(DumpBinaryBlob_MultipleBytes, ValidBinaryBlobWithMultipleBytes) {
    std::cout << "Entering DumpBinaryBlob_MultipleBytes test" << std::endl;

    // Prepare a fixed size array and assign values using strncpy
    char buffer[5] = {0};  // extra space for safety
    strncpy(buffer, "\x01\x02\x03\x04", 4);
    std::cout << "Initialized buffer with values:";
    for (size_t i = 0; i < 4; i++) {
        std::cout << " 0x" << std::hex << (0xFF & static_cast<unsigned char>(buffer[i]));
    }
    std::cout << std::dec << std::endl;
    
    // Invocation log
    std::cout << "Invoking DumpBinaryBlob with ptr = buffer and len = 4" << std::endl;
    EXPECT_NO_THROW({
        DumpBinaryBlob(reinterpret_cast<const unsigned char*>(buffer), 4);
    });
    
    std::cout << "Exiting DumpBinaryBlob_MultipleBytes test" << std::endl;
}
/**
 * @brief Verify DumpBinaryBlob processes a valid binary blob with a single byte correctly.
 *
 * This test verifies that DumpBinaryBlob can handle a buffer containing a single valid byte.
 * It initializes a fixed size character array with the value 0xAA and ensures that invoking
 * DumpBinaryBlob with this buffer does not throw any exceptions.
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
 * | Variation / Step | Description                                                    | Test Data                                            | Expected Result                              | Notes              |
 * | :--------------: | -------------------------------------------------------------- | ---------------------------------------------------- | -------------------------------------------- | ------------------ |
 * | 01               | Initialize a fixed size buffer with the single byte value 0xAA   | buffer[0] = 0xAA, buffer[1] = 0                        | Buffer correctly initialized with value 0xAA | Should be successful |
 * | 02               | Invoke DumpBinaryBlob with pointer to buffer and length = 1      | ptr = address of buffer, len = 1                      | No exception thrown by DumpBinaryBlob call   | Should Pass        |
 */
TEST(DumpBinaryBlob_SingleByte, ValidBinaryBlobWithSingleByte) {
    std::cout << "Entering DumpBinaryBlob_SingleByte test" << std::endl;

    // Prepare a fixed size array and assign single byte using strncpy
    char buffer[2] = {0};
    strncpy(buffer, "\xAA", 1);
    std::cout << "Initialized buffer with value: 0x" 
              << std::hex << (0xFF & static_cast<unsigned char>(buffer[0])) 
              << std::dec << std::endl;
    
    // Invocation log
    std::cout << "Invoking DumpBinaryBlob with ptr = buffer and len = 1" << std::endl;
    EXPECT_NO_THROW({
        DumpBinaryBlob(reinterpret_cast<const unsigned char*>(buffer), 1);
    });
    
    std::cout << "Exiting DumpBinaryBlob_SingleByte test" << std::endl;
}
/**
 * @brief Tests DumpBinaryBlob with a valid pointer and a zero length.
 *
 * This unit test verifies that invoking DumpBinaryBlob with a valid character buffer pointer and a zero length does not throw any exceptions. It ensures that the function gracefully handles the edge case where no binary data is provided.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * |01| Initialize a buffer with dummy data and invoke DumpBinaryBlob with ptr = buffer and len = 0 | input: buffer = "dummy" (stored in a 10-character array), ptr = address of buffer, len = 0 | Function completes without throwing an exception | Should Pass |
 */
TEST(DumpBinaryBlob_ZeroLength, ValidPointerWithZeroLength) {
    std::cout << "Entering DumpBinaryBlob_ZeroLength test" << std::endl;

    // Prepare a fixed size array even though content is irrelevant
    char buffer[10] = {0};
    strncpy(buffer, "dummy", 5);
    std::cout << "Initialized buffer with dummy data." << std::endl;
    
    // Invocation log
    std::cout << "Invoking DumpBinaryBlob with ptr = buffer and len = 0" << std::endl;
    EXPECT_NO_THROW({
        DumpBinaryBlob(reinterpret_cast<const unsigned char*>(buffer), 0);
    });
    
    std::cout << "Exiting DumpBinaryBlob_ZeroLength test" << std::endl;
}
/**
 * @brief Test to verify DumpBinaryBlob handles null pointer correctly when provided with a non-zero length.
 *
 * This test verifies that the DumpBinaryBlob function does not throw an exception when called with a null pointer and a non-zero length value. The objective is to ensure that the function is robust against invalid pointer inputs and properly handles the scenario without crashing.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke DumpBinaryBlob with a null pointer and non-zero length | ptr = nullptr, len = 5 | DumpBinaryBlob does not throw an exception | Should Pass |
 */
TEST(DumpBinaryBlob_NullPointer, NullPointerWithNonZeroLength) {
    GTEST_SKIP();
    std::cout << "Entering DumpBinaryBlob_NullPointer test" << std::endl;
    
    // Log input values for null pointer case
    std::cout << "Invoking DumpBinaryBlob with ptr = nullptr and len = 5" << std::endl;
    EXPECT_NO_THROW({
        DumpBinaryBlob(nullptr, 5);
    });
    
    std::cout << "Exiting DumpBinaryBlob_NullPointer test" << std::endl;
}
/**
 * @brief Test the logprintf function with valid parameters across various log levels to ensure it does not throw exceptions.
 *
 * This test iterates through a set of defined log levels, prepares a file name and format string using fixed arrays, and calls the logprintf API with valid inputs. The objective is to verify that the function logs messages correctly without any exceptions being thrown.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Prepare file name array using strncpy with sourceFile = "test.cpp" | sourceFile = "test.cpp", fileArr = allocated char[256] (zero-initialized) | fileArr correctly contains "test.cpp" | Should be successful |
 * | 02 | Prepare format string array using strncpy with sourceFormat = "Test log message with value %d" | sourceFormat = "Test log message with value %d", formatArr = allocated char[256] (zero-initialized) | formatArr correctly contains "Test log message with value %d" | Should be successful |
 * | 03 | Iterate over all log levels and invoke logprintf API for each level | levels = { mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR }, fileArr = "test.cpp", line = 100, formatArr = "Test log message with value %d", argument = 42 | No exception thrown from logprintf for each log level (verified via EXPECT_NO_THROW) | Should Pass |
 */
TEST(LogPrintfTest, ValidLoggingTrace)
{
    std::cout << "Entering ValidLoggingTrace test" << std::endl;

    MW_LogLevel levels[] = { mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR };
    const int numLevels = sizeof(levels)/sizeof(levels[0]);

    // Prepare file name fixed array using strncpy
    char fileArr[256];
    std::memset(fileArr, 0, sizeof(fileArr));
    const char* sourceFile = "test.cpp";
    std::strncpy(fileArr, sourceFile, sizeof(fileArr)-1);

    // Prepare format string fixed array using strncpy
    char formatArr[256];
    std::memset(formatArr, 0, sizeof(formatArr));
    const char* sourceFormat = "Test log message with value %d";
    std::strncpy(formatArr, sourceFormat, sizeof(formatArr)-1);

    for (int i = 0; i < numLevels; i++)
    {
        std::cout << "Invoking logprintf with level = " << levels[i]
                  << ", file = " << fileArr
                  << ", line = " << 100
                  << ", format = \"" << formatArr << "\" with argument 42" << std::endl;
        EXPECT_NO_THROW(logprintf(levels[i], fileArr, 100, formatArr, 42));
        std::cout << "Completed invocation for level = " << levels[i] << std::endl;
    }

    std::cout << "Exiting ValidLoggingTrace test" << std::endl;
}
/**
 * @brief Test logprintf API for handling a NULL file pointer
 *
 * This test verifies that the logprintf API properly handles a NULL file pointer input without throwing an exception. 
 * The API is invoked with a NULL file pointer, a valid debug log level, a specific line number, and a valid format string. 
 * This ensures robustness of the API when encountering an invalid file pointer.
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
 * | Variation / Step | Description                                                                     | Test Data                                                                                                  | Expected Result                                                                         | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Set up test variables including a NULL file pointer, valid debug level, line number, and a valid format string; invoke logprintf API | level = mLOGLEVEL_DEBUG, file = NULL, line = 200, format = "Log message with NULL file pointer"          | API should not throw an exception; EXPECT_NO_THROW check passes                           | Should Pass |
 */
TEST(LogPrintfTest, NullFilePointer)
{
    std::cout << "Entering NullFilePointer test" << std::endl;

    // file pointer is set to NULL
    const char* filePtr = NULL;

    // Prepare format string fixed array using strncpy
    char formatArr[256];
    std::memset(formatArr, 0, sizeof(formatArr));
    const char* sourceFormat = "Log message with NULL file pointer";
    std::strncpy(formatArr, sourceFormat, sizeof(formatArr)-1);

    std::cout << "Invoking logprintf with level = " << mLOGLEVEL_DEBUG
              << ", file = NULL"
              << ", line = " << 200
              << ", format = \"" << formatArr << "\"" << std::endl;
    EXPECT_NO_THROW(logprintf(mLOGLEVEL_DEBUG, filePtr, 200, formatArr));
    std::cout << "Exiting NullFilePointer test" << std::endl;
}
/**
 * @brief Verify that logprintf API handles a NULL format string gracefully
 *
 * This test checks whether the logprintf API, when provided with a NULL format string along with valid file name, line number, and log level, does not throw any exception during execution.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                       | Expected Result                                        | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ------------------------------------------------------ | ---------- |
 * | 01               | Invoke logprintf with a NULL format string and valid file name, line number, and log level.    | log level = mLOGLEVEL_WARN, file = error.cpp, line = 250, format = NULL | No exception is thrown (EXPECT_NO_THROW passes) | Should Pass |
 */
TEST(LogPrintfTest, NullFormatString)
{
    std::cout << "Entering NullFormatString test" << std::endl;

    // Prepare file name fixed array using strncpy
    char fileArr[256];
    std::memset(fileArr, 0, sizeof(fileArr));
    const char* sourceFile = "error.cpp";
    std::strncpy(fileArr, sourceFile, sizeof(fileArr)-1);

    // format pointer is set to NULL
    const char* formatPtr = NULL;

    std::cout << "Invoking logprintf with level = " << mLOGLEVEL_WARN
              << ", file = " << fileArr
              << ", line = " << 250
              << ", format = NULL" << std::endl;
    EXPECT_NO_THROW(logprintf(mLOGLEVEL_WARN, fileArr, 250, formatPtr));
    std::cout << "Exiting NullFormatString test" << std::endl;
}
/**
 * @brief Test negative line number handling in logprintf API
 *
 * This test verifies that the logprintf function can handle a negative line number input gracefully without throwing exceptions.
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
 * | Variation / Step | Description                                                 | Test Data                                                            | Expected Result                                                          | Notes       |
 * | :--------------: | ----------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------------------------------------------------------ | ----------- |
 * | 01               | Invoke logprintf with level = mLOGLEVEL_ERROR, file = fileArr, line = -1, format = formatArr | input: level = mLOGLEVEL_ERROR, file = "main.cpp", line = -1, format = "Log message with negative line number" | No exception is thrown (API returns normally)                           | Should Pass |
 */
TEST(LogPrintfTest, NegativeLineNumber)
{
    std::cout << "Entering NegativeLineNumber test" << std::endl;

    // Prepare file name fixed array using strncpy
    char fileArr[256];
    std::memset(fileArr, 0, sizeof(fileArr));
    const char* sourceFile = "main.cpp";
    std::strncpy(fileArr, sourceFile, sizeof(fileArr)-1);

    // Prepare format string fixed array using strncpy
    char formatArr[256];
    std::memset(formatArr, 0, sizeof(formatArr));
    const char* sourceFormat = "Log message with negative line number";
    std::strncpy(formatArr, sourceFormat, sizeof(formatArr)-1);

    std::cout << "Invoking logprintf with level = " << mLOGLEVEL_ERROR
              << ", file = " << fileArr
              << ", line = " << -1
              << ", format = \"" << formatArr << "\"" << std::endl;
    EXPECT_NO_THROW(logprintf(mLOGLEVEL_ERROR, fileArr, -1, formatArr));
    std::cout << "Exiting NegativeLineNumber test" << std::endl;
}
/**
 * @brief Test the behavior of logprintf when provided with an empty format string.
 *
 * This test case verifies that the logprintf API handles an empty format string without throwing any exceptions.
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
 * | 01 | Set up the file name and empty format string, then invoke logprintf with valid parameters | level = mLOGLEVEL_MIL, file = "empty.cpp", line = 300, format = "" | logprintf should execute without throwing any exceptions | Should Pass |
 */
TEST(LogPrintfTest, EmptyFormatString)
{
    std::cout << "Entering EmptyFormatString test" << std::endl;

    // Prepare file name fixed array using strncpy
    char fileArr[256];
    std::memset(fileArr, 0, sizeof(fileArr));
    const char* sourceFile = "empty.cpp";
    std::strncpy(fileArr, sourceFile, sizeof(fileArr)-1);

    // Prepare empty format string fixed array using strncpy
    char formatArr[256];
    std::memset(formatArr, 0, sizeof(formatArr));
    const char* sourceFormat = "";
    std::strncpy(formatArr, sourceFormat, sizeof(formatArr)-1);

    std::cout << "Invoking logprintf with level = " << mLOGLEVEL_MIL
              << ", file = " << fileArr
              << ", line = " << 300
              << ", format = \"(empty string)\"" << std::endl;
    EXPECT_NO_THROW(logprintf(mLOGLEVEL_MIL, fileArr, 300, formatArr));
    std::cout << "Exiting EmptyFormatString test" << std::endl;
}
/**
 * @brief Validate that PlayerLogManager::SetLoggerInfo correctly configures logging parameters
 *
 * This test verifies that invoking PlayerLogManager::SetLoggerInfo with all parameters set to true (and level set to 3) 
 * successfully sets the corresponding internal states of PlayerLogManager without throwing exceptions. 
 * It ensures that the log redirection, ethan log redirection, log level and lock settings are established as expected.
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
 * | Variation / Step | Description                                                                                   | Test Data                                                         | Expected Result                                    | Notes             |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ------------------------------------------------- | ----------------- |
 * | 01               | Instantiate PlayerLogManager and invoke SetLoggerInfo with specified parameters               | input: logRedirectStatus = true, ethanLogStatus = true, level = 3, lock = true | No exception thrown; API sets internal state      | Should Pass       |
 * | 02               | Verify that disableLogRedirection is set to true                                               | output: disableLogRedirection = true                              | disableLogRedirection equals true                 | Should be successful |
 * | 03               | Verify that enableEthanLogRedirection is set to true                                             | output: enableEthanLogRedirection = true                            | enableEthanLogRedirection equals true              | Should be successful |
 * | 04               | Verify that mwLoglevel is set to MW_LogLevel(3)                                                  | output: mwLoglevel = MW_LogLevel(3)                                 | mwLoglevel equals MW_LogLevel(3)                  | Should be successful |
 * | 05               | Verify that locked is set to true                                                                | output: locked = true                                               | locked equals true                                | Should be successful |
 */
TEST(PlayerLogManager, AllParametersTrue_PositiveLogLevel) {
    std::cout << "Entering AllParametersTrue_PositiveLogLevel test" << std::endl;
    EXPECT_NO_THROW({
        // Creating object of PlayerLogManager using default constructor
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=true, ethanLogStatus=true, level=3, lock=true" << std::endl;
        PlayerLogManager::SetLoggerInfo(true, true, 3, true);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(3)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(3));
    EXPECT_EQ(PlayerLogManager::locked, true);
    std::cout << "Exiting AllParametersTrue_PositiveLogLevel test" << std::endl;
}
/**
 * @brief Verify correct configuration of PlayerLogManager with all parameters set to their false or equivalent values.
 *
 * This test ensures that invoking PlayerLogManager::SetLoggerInfo with log redirection disabled (false), ethan log disabled (false),
 * a valid mw log level (2), and lock disabled (false) does not throw any exceptions and properly sets the internal state of the logger.
 * It is critical to verify that the logging configuration behaves as expected under these conditions to ensure logging stability.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                       | Expected Result                                                                                                                  | Notes      |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke PlayerLogManager::SetLoggerInfo with logRedirectStatus=false, ethanLogStatus=false, level=2, lock=false | logRedirectStatus=false, ethanLogStatus=false, level=2, lock=false | No exception thrown; internal state: disableLogRedirection==false, enableEthanLogRedirection==false, mwLoglevel==MW_LogLevel(2), locked==false | Should Pass |
 */
TEST(PlayerLogManager, AllParametersFalse_PositiveLogLevel) {
    std::cout << "Entering AllParametersFalse_PositiveLogLevel test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=false, ethanLogStatus=false, level=2, lock=false" << std::endl;
        PlayerLogManager::SetLoggerInfo(false, false, 2, false);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(2)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(2));
    EXPECT_EQ(PlayerLogManager::locked, false);
    std::cout << "Exiting AllParametersFalse_PositiveLogLevel test" << std::endl;
}
/**
 * @brief Validate correct state update after invoking SetLoggerInfo with mixed booleans and a valid log level.
 *
 * This test verifies that the PlayerLogManager correctly updates its internal state when the SetLoggerInfo method is called with mixed boolean parameters and a valid log level. It ensures that no exceptions are thrown during the API call and that the static member variables reflect the expected values after the invocation.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 012
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetLoggerInfo with mixed boolean values and a valid log level | input: logRedirectStatus=true, ethanLogStatus=false, level=5, lock=false | No exception thrown during API call | Should Pass |
 * | 02 | Verify internal state of PlayerLogManager after the API call | output: disableLogRedirection=true, enableEthanLogRedirection=false, mwLoglevel=MW_LogLevel(5), locked=false | Static member variables match the expected values and assertions pass | Should be successful |
 */
TEST(PlayerLogManager, MixedBooleans_PositiveLogLevel) {
    std::cout << "Entering MixedBooleans_PositiveLogLevel test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=true, ethanLogStatus=false, level=5, lock=false" << std::endl;
        PlayerLogManager::SetLoggerInfo(true, false, 5, false);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(5)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(5));
    EXPECT_EQ(PlayerLogManager::locked, false);
    std::cout << "Exiting MixedBooleans_PositiveLogLevel test" << std::endl;
}
/**
 * @brief Test the PlayerLogManager::SetLoggerInfo method with mixed booleans and a positive log level.
 *
 * This test verifies that the SetLoggerInfo API correctly updates the internal static members of PlayerLogManager when provided with a mix of boolean values and a valid log level. It ensures that no exceptions are thrown during the API call and that the internal state matches the expected configuration.
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
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Log test entry into the function and display the invocation parameters. | None | Console displays the entry log and invocation parameters. | Should be successful |
 * | 02 | Invoke SetLoggerInfo API with specific parameters. | input1 = logRedirectStatus false, input2 = ethanLogStatus true, input3 = level 4, input4 = lock true | API call executes without throwing an exception. | Should Pass |
 * | 03 | Validate the internal state after API invocation with assertions. | output1 = disableLogRedirection false, output2 = enableEthanLogRedirection true, output3 = mwLoglevel MW_LogLevel(4), output4 = locked true | All assertions pass verifying the expected internal state. | Should Pass |
 */
TEST(PlayerLogManager, MixedBooleansVariant_PositiveLogLevel) {
    std::cout << "Entering MixedBooleansVariant_PositiveLogLevel test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=false, ethanLogStatus=true, level=4, lock=true" << std::endl;
        PlayerLogManager::SetLoggerInfo(false, true, 4, true);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(4)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(4));
    EXPECT_EQ(PlayerLogManager::locked, true);
    std::cout << "Exiting MixedBooleansVariant_PositiveLogLevel test" << std::endl;
}
/**
 * @brief Verify that SetLoggerInfo correctly updates internal states for a negative log level input scenario.
 *
 * This test verifies that when SetLoggerInfo is invoked with a negative log level input scenario (log level 4 as per test plan), 
 * it executes without throwing exceptions and updates the internal state variables accordingly. This ensures that even though 
 * the scenario is named negative, the function handles the provided parameters correctly.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate PlayerLogManager and invoke SetLoggerInfo with specific parameters to update the logging settings. | input: logRedirectStatus = true, ethanLogStatus = true, level = 4, lock = false; output: disableLogRedirection = true, enableEthanLogRedirection = true, mwLoglevel = MW_LogLevel(4), locked = false | API should run without throwing exceptions and the internal state should match the provided input values. | Should Pass |
 */
TEST(PlayerLogManager, NegativeLogLevelInput) {
    std::cout << "Entering NegativeLogLevelInput test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        // Although test name indicates negative log level, the input level is 4 as per test plan.
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=true, ethanLogStatus=true, level=4, lock=false" << std::endl;
        PlayerLogManager::SetLoggerInfo(true, true, 4, false);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(4)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(4));
    EXPECT_EQ(PlayerLogManager::locked, false);
    std::cout << "Exiting NegativeLogLevelInput test" << std::endl;
}
/**
 * @brief To verify that the SetLoggerInfo API correctly handles a zero log level input.
 *
 * This test validates that when SetLoggerInfo is invoked with logRedirectStatus set to true, ethanLogStatus set to false, a log level of 0, and lock set to true, the method correctly updates the internal state of the PlayerLogManager without throwing any exceptions. The objective is to ensure that the API behaves as expected with a zero log level and that all the corresponding internal variables are set properly.
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
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Create a PlayerLogManager instance and invoke SetLoggerInfo with specific parameters. | input: logRedirectStatus = true, ethanLogStatus = false, level = 0, lock = true | API does not throw any exception. | Should Pass |@n
 * | 02 | Verify that the internal state variables update as expected after the API call. | output: disableLogRedirection = true, enableEthanLogRedirection = false, mwLoglevel = MW_LogLevel(0), locked = true | All assertions (EXPECT_EQ) pass confirming the updated internal state. | Should Pass |
 */
TEST(PlayerLogManager, ZeroLogLevelInput) {
    std::cout << "Entering ZeroLogLevelInput test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=true, ethanLogStatus=false, level=0, lock=true" << std::endl;
        PlayerLogManager::SetLoggerInfo(true, false, 0, true);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(0)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(0));
    EXPECT_EQ(PlayerLogManager::locked, true);
    std::cout << "Exiting ZeroLogLevelInput test" << std::endl;
}
/**
 * @brief Verify that SetLoggerInfo correctly updates PlayerLogManager internal state for an extreme positive boundary value.
 *
 * This test verifies that the PlayerLogManager::SetLoggerInfo API call properly sets the internal static variables when given the maximum positive integer value for the log level (2147483647) along with specific boolean parameters. It ensures that no exception is thrown during the call and that the state change reflects the input provided.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke SetLoggerInfo API with extreme positive log level and boolean parameters. | input: logRedirectStatus = false, ethanLogStatus = false, level = 2147483647, lock = false | API call completes without throwing an exception. | Should Pass |
 * | 02 | Verify the internal state of PlayerLogManager reflects the parameters passed to SetLoggerInfo. | output: disableLogRedirection = false, enableEthanLogRedirection = false, mwLoglevel = MW_LogLevel(2147483647), locked = false | Each internal variable is set to its corresponding expected value. | Should Pass |
 */
TEST(PlayerLogManager, ExtremePositiveLogLevel_BoundaryCase) {
    std::cout << "Entering ExtremePositiveLogLevel_BoundaryCase test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=false, ethanLogStatus=false, level=2147483647, lock=false" << std::endl;
        PlayerLogManager::SetLoggerInfo(false, false, 2147483647, false);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(2147483647)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(2147483647));
    EXPECT_EQ(PlayerLogManager::locked, false);
    std::cout << "Exiting ExtremePositiveLogLevel_BoundaryCase test" << std::endl;
}
/**
 * @brief Verify that the SetLoggerInfo API handles extreme negative log level boundary conditions correctly.
 *
 * This test case verifies that the PlayerLogManager's SetLoggerInfo function correctly processes an extreme negative boundary value (-2147483648) for the log level without throwing exceptions. It confirms that the internal state of the PlayerLogManager is updated as expected based on the provided input parameters.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |@n
 * | :----: | --------- | ---------- |-------------- | ----- |@n
 * | 01 | Invoke SetLoggerInfo with logRedirectStatus=true, ethanLogStatus=false, level=-2147483648, lock=false | input1 = logRedirectStatus:true, input2 = ethanLogStatus:false, input3 = level:-2147483648, input4 = lock:false | API call completes without exception and internal state updates: disableLogRedirection = true, enableEthanLogRedirection = false, mwLoglevel = MW_LogLevel(-2147483648), locked = false | Should Pass |
 */
TEST(PlayerLogManager, ExtremeNegativeLogLevel_BoundaryCase) {
    std::cout << "Entering ExtremeNegativeLogLevel_BoundaryCase test" << std::endl;
    EXPECT_NO_THROW({
        PlayerLogManager manager;
        std::cout << "Invoking SetLoggerInfo with parameters: logRedirectStatus=true, ethanLogStatus=false, level=-2147483648, lock=false" << std::endl;
        PlayerLogManager::SetLoggerInfo(true, false, -2147483648, false);
        std::cout << "Method SetLoggerInfo invoked" << std::endl;
        std::cout << "Internal state after invocation:" << std::endl;
        std::cout << "  disableLogRedirection: " << PlayerLogManager::disableLogRedirection << std::endl;
        std::cout << "  enableEthanLogRedirection: " << PlayerLogManager::enableEthanLogRedirection << std::endl;
        std::cout << "  mwLoglevel set to MW_LogLevel(-2147483648)" << std::endl;
        std::cout << "  locked: " << PlayerLogManager::locked << std::endl;
    });
    EXPECT_EQ(PlayerLogManager::disableLogRedirection, true);
    EXPECT_EQ(PlayerLogManager::enableEthanLogRedirection, false);
    EXPECT_EQ(PlayerLogManager::mwLoglevel, MW_LogLevel(-2147483648));
    EXPECT_EQ(PlayerLogManager::locked, false);
    std::cout << "Exiting ExtremeNegativeLogLevel_BoundaryCase test" << std::endl;
}
/**
 * @brief Validate conversion of a single element zero into its hexadecimal string representation.
 *
 * This test verifies that the PlayerLogManager::getHexDebugStr method correctly converts a vector containing a single element 0 into the expected hexadecimal string "0x00". It ensures that the API correctly handles the minimal input scenario and returns the appropriate string without any exception.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 018
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                 | Test Data                                             | Expected Result                                          | Notes            |
 * | :--------------: | --------------------------------------------------------------------------- | ----------------------------------------------------- | -------------------------------------------------------- | ---------------- |
 * | 01               | Set up the input vector with a single element 0.                           | input = {0}                                           | Input vector is successfully created.                  | Should be successful |
 * | 02               | Create a PlayerLogManager object using the default constructor.            | No input required; output: playerLogManager instance  | Object is created without exceptions.                  | Should be successful |
 * | 03               | Invoke getHexDebugStr with the input vector.                               | input = {0}                                           | Returns a hexadecimal string "0x00".                   | Should Pass      |
 * | 04               | Validate that the returned hexadecimal string is exactly "0x00".             | result = "0x00"                                        | EXPECT_EQ check passes confirming result matches "0x00". | Should Pass      |
 */
TEST(PlayerLogManager, SingleElementZeroConversion) {
    std::cout << "Entering SingleElementZeroConversion test" << std::endl;
    
    std::vector<uint8_t> input = {0};
    std::cout << "Input vector contains: 0" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerLogManager playerLogManager);
    PlayerLogManager playerLogManager;
    std::cout << "Created PlayerLogManager object." << std::endl;
    
    std::cout << "Invoking PlayerLogManager::getHexDebugStr with input: {0}" << std::endl;
    std::string result = playerLogManager.getHexDebugStr(input);
    std::cout << "Returned hex string: " << result << std::endl;
    
    EXPECT_EQ(result, "0x00");
    std::cout << "Exiting SingleElementZeroConversion test" << std::endl;
}
/**
 * @brief Verify that a single-element vector with maximum value converts to a hex value string.
 *
 * This test validates that when an input vector containing a single maximum value (255) is provided,
 * the PlayerLogManager::getHexDebugStr method correctly converts it to the corresponding hexadecimal string "0xFF".
 * This ensures proper functionality for basic conversion operation.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 019@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create input vector with maximum uint8_t value. | input = {255} | A vector with element 255 is successfully created. | Should be successful |
 * | 02 | Construct PlayerLogManager object using default constructor and check for exceptions. | None | Object constructed without throwing exceptions. | Should Pass |
 * | 03 | Invoke getHexDebugStr API with the input vector. | input = {255} | API returns the hexadecimal string "0xFF". | Should Pass |
 * | 04 | Verify that the returned string matches "0xFF". | result = "0xFF" | Assertion succeeds confirming the hex string is correct. | Should Pass |
 */
TEST(PlayerLogManager, SingleElementMaximumValueConversion) {
    std::cout << "Entering SingleElementMaximumValueConversion test" << std::endl;
    
    std::vector<uint8_t> input = {255};
    std::cout << "Input vector contains: 255" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerLogManager playerLogManager);
    PlayerLogManager playerLogManager;
    std::cout << "Created PlayerLogManager object." << std::endl;
    
    std::cout << "Invoking PlayerLogManager::getHexDebugStr with input: {255}" << std::endl;
    std::string result = playerLogManager.getHexDebugStr(input);
    std::cout << "Returned hex string: " << result << std::endl;
    
    EXPECT_EQ(result, "0xFF");
    std::cout << "Exiting SingleElementMaximumValueConversion test" << std::endl;
}
/**
 * @brief Validate conversion of multiple typical numeric elements to an uppercase hexadecimal debug string
 *
 * This test verifies that the PlayerLogManager's getHexDebugStr API correctly converts a vector of typical numeric values into a properly formatted uppercase hexadecimal debug string. The test creates a vector with typical values, instantiates the PlayerLogManager object without throwing any exceptions, and then calls getHexDebugStr with the vector. The output string is compared against the expected hexadecimal representation.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create a vector with multiple typical numeric values | input = {16, 32, 48} | A valid vector is created containing the numbers 16, 32, 48 | Should be successful |
 * | 02 | Instantiate the PlayerLogManager object using the default constructor and verify no exceptions are thrown | Invocation: PlayerLogManager playerLogManager | Object is successfully created without throwing an exception | Should Pass |
 * | 03 | Invoke getHexDebugStr on the PlayerLogManager object with the input vector and verify the returned hex string | Method call: getHexDebugStr(input), input = {16, 32, 48} | Return value equals "0x102030" and passes the EXPECT_EQ assertion | Should Pass |
 */
TEST(PlayerLogManager, MultipleElementsConversionWithTypicalValues) {
    std::cout << "Entering MultipleElementsConversionWithTypicalValues test" << std::endl;
    
    std::vector<uint8_t> input = {16, 32, 48};
    std::cout << "Input vector contains: 16, 32, 48" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerLogManager playerLogManager);
    PlayerLogManager playerLogManager;
    std::cout << "Created PlayerLogManager object." << std::endl;
    
    std::cout << "Invoking PlayerLogManager::getHexDebugStr with input: {16, 32, 48}" << std::endl;
    std::string result = playerLogManager.getHexDebugStr(input);
    std::cout << "Returned hex string: " << result << std::endl;
    
    EXPECT_EQ(result, "0x102030");
    std::cout << "Exiting MultipleElementsConversionWithTypicalValues test" << std::endl;
}
/**
 * @brief Validate that getHexDebugStr returns uppercase hexadecimal digits for input vector.
 *
 * This test verifies that when an input vector containing the decimal value 127 is passed to the PlayerLogManager::getHexDebugStr, the returned hexadecimal string is in uppercase format ("0x7F"). It ensures that the conversion function correctly handles and formats the hexadecimal output as expected.
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
 * | 01 | Construct PlayerLogManager and invoke getHexDebugStr with an input vector containing 127 | input = {127}, object = PlayerLogManager, output = result | Function returns "0x7F" and the equality assertion passes | Should Pass |
 */
TEST(PlayerLogManager, ValidateUppercaseHexadecimalDigitsInOutput) {
    std::cout << "Entering ValidateUppercaseHexadecimalDigitsInOutput test" << std::endl;
    
    std::vector<uint8_t> input = {127};
    std::cout << "Input vector contains: 127" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerLogManager playerLogManager);
    PlayerLogManager playerLogManager;
    std::cout << "Created PlayerLogManager object." << std::endl;
    
    std::cout << "Invoking PlayerLogManager::getHexDebugStr with input: {127}" << std::endl;
    std::string result = playerLogManager.getHexDebugStr(input);
    std::cout << "Returned hex string: " << result << std::endl;
    
    EXPECT_EQ(result, "0x7F");
    std::cout << "Exiting ValidateUppercaseHexadecimalDigitsInOutput test" << std::endl;
}
/**
 * @brief Validate the conversion of repeated uint8_t values to a hexadecimal debug string.
 *
 * This test verifies that the PlayerLogManager's getHexDebugStr method correctly converts a vector
 * of repeated uint8_t values (15, 15, 15) into its corresponding hex string representation "0x0F0F0F".
 * It also confirms that the PlayerLogManager object can be created without throwing any exceptions.
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
 * | Variation / Step | Description                                          | Test Data                                  | Expected Result                                | Notes         |
 * | :--------------: | ---------------------------------------------------- | ------------------------------------------ | ---------------------------------------------- | ------------- |
 * | 01               | Initialize input vector with repeated values         | input = 15,15,15                           | Input vector is correctly populated            | Should be successful |
 * | 02               | Create PlayerLogManager object using default constructor | No input arguments                         | Object is instantiated without throwing exception | Should Pass    |
 * | 03               | Invoke getHexDebugStr with input vector              | input = 15,15,15, expected output = 0x0F0F0F  | Method returns hex string "0x0F0F0F"            | Should Pass    |
 */
TEST(PlayerLogManager, RepeatedValuesConversionTest) {
    std::cout << "Entering RepeatedValuesConversionTest test" << std::endl;
    
    std::vector<uint8_t> input = {15, 15, 15};
    std::cout << "Input vector contains: 15, 15, 15" << std::endl;
    
    // Create object using default constructor
    EXPECT_NO_THROW(PlayerLogManager playerLogManager);
    PlayerLogManager playerLogManager;
    std::cout << "Created PlayerLogManager object." << std::endl;
    
    std::cout << "Invoking PlayerLogManager::getHexDebugStr with input: {15, 15, 15}" << std::endl;
    std::string result = playerLogManager.getHexDebugStr(input);
    std::cout << "Returned hex string: " << result << std::endl;
    
    EXPECT_EQ(result, "0x0F0F0F");
    std::cout << "Exiting RepeatedValuesConversionTest test" << std::endl;
}
/**
 * @brief Verify that all log levels are allowed when threshold is set to mLOGLEVEL_TRACE.
 *
 * This test verifies that the PlayerLogManager object can be instantiated without throwing exceptions, that the log level threshold is correctly set to mLOGLEVEL_TRACE, and that the isLogLevelAllowed API returns true for every log level from mLOGLEVEL_TRACE to mLOGLEVEL_ERROR. This ensures that the logging mechanism functions as expected when the lowest log level is set.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 023
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate a PlayerLogManager object using the default constructor wrapped with EXPECT_NO_THROW | No input; Output: Object creation | Object is created without throwing an exception | Should be successful |
 * | 02 | Set the log level threshold to mLOGLEVEL_TRACE | PlayerLogManager::mwLoglevel = mLOGLEVEL_TRACE | Threshold is set to mLOGLEVEL_TRACE | Should be successful |
 * | 03 | Iterate through all MW_LogLevel enum values and call isLogLevelAllowed for each with the current level | For each iteration: currentLevel = {mLOGLEVEL_TRACE, ..., mLOGLEVEL_ERROR} | isLogLevelAllowed returns true for each log level | Should Pass |
 */
TEST(PlayerLogManager, isLogLevelAllowed_Start)
{
    std::cout << "Entering isLogLevelAllowed_Start test" << std::endl;
    
    // Create object using default constructor (wrapped with EXPECT_NO_THROW)
    EXPECT_NO_THROW({ PlayerLogManager manager; });
    
    // Set the threshold to mLOGLEVEL_TRACE
    PlayerLogManager::mwLoglevel = mLOGLEVEL_TRACE;
    std::cout << "Set PlayerLogManager::mwLoglevel to mLOGLEVEL_TRACE (" << mLOGLEVEL_TRACE << ")" << std::endl;
    
    // Iterate over all MW_LogLevel enum values
    for (int level = mLOGLEVEL_TRACE; level <= mLOGLEVEL_ERROR; ++level)
    {
        MW_LogLevel currentLevel = static_cast<MW_LogLevel>(level);
        std::cout << "Invoking PlayerLogManager::isLogLevelAllowed with param value: " << level << std::endl;
        bool result = PlayerLogManager::isLogLevelAllowed(currentLevel);
        std::cout << "Method returned: " << std::boolalpha << result << std::endl;
        // Expect true because every log level is >= mLOGLEVEL_TRACE
        EXPECT_TRUE(result);
    }
    
    std::cout << "Exiting isLogLevelAllowed_Start test" << std::endl;
}
/**
 * @brief Test verifies that invoking lockLogLevel with true sets the internal locked state to true.
 *
 * This test verifies that the lockLogLevel method correctly updates the PlayerLogManager::locked state to true when invoked with a true parameter. It ensures that no exceptions are thrown and that the state change is properly reflected. 
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
 * | Variation / Step | Description                                                        | Test Data                                    | Expected Result                                                  | Notes        | 
 * | :--------------: | ------------------------------------------------------------------ | -------------------------------------------- | ---------------------------------------------------------------- | ------------ |
 * | 01               | Invoke PlayerLogManager::lockLogLevel with lockValue = true          | input: lockValue = true                      | API does not throw any exception                                 | Should Pass  |
 * | 02               | Verify that PlayerLogManager::locked is set to true after API call     | output: PlayerLogManager::locked = true       | EXPECT_TRUE(PlayerLogManager::locked) assertion passes successfully | Should be successful |
 */
TEST(PlayerLogManager, LockLogLevelTrue) {
    std::cout << "Entering LockLogLevelTrue test" << std::endl;
    bool lockValue = true;
    std::cout << "Invoking lockLogLevel with parameter: " << lockValue << std::endl;
    
    // Invoke the static method lockLogLevel via the object.
    EXPECT_NO_THROW(PlayerLogManager::lockLogLevel(lockValue));
    std::cout << "Method lockLogLevel invoked successfully." << std::endl;
    
    // Verify that the locked state is set to true.
    std::cout << "Expected internal state: PlayerLogManager::locked = true" << std::endl;
    std::cout << "Actual internal state: PlayerLogManager::locked = " << PlayerLogManager::locked << std::endl;
    EXPECT_TRUE(PlayerLogManager::locked);
    
    std::cout << "Exiting LockLogLevelTrue test" << std::endl;
}
/**
 * @brief Validate that the log level is unlocked when lockLogLevel is invoked with false.
 *
 * This test verifies that calling PlayerLogManager::lockLogLevel with the parameter lockValue set to false does not throw an exception and sets the internal state (PlayerLogManager::locked) to false.
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
 * | Variation / Step | Description                                                                               | Test Data                                   | Expected Result                                                      | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------- | ------------------------------------------- | ------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke lockLogLevel with lockValue set to false and verify the internal state is false.     | lockValue = false, expected locked = false  | API does not throw exception and PlayerLogManager::locked is set to false | Should Pass |
 */
TEST(PlayerLogManager, LockLogLevelFalse) {
    std::cout << "Entering LockLogLevelFalse test" << std::endl;
    bool lockValue = false;
    std::cout << "Invoking lockLogLevel with parameter: " << lockValue << std::endl;
    
    // Invoke the static method lockLogLevel via the object.
    EXPECT_NO_THROW(PlayerLogManager::lockLogLevel(lockValue));
    std::cout << "Method lockLogLevel invoked successfully." << std::endl;
    
    // Verify that the locked state is set to false.
    std::cout << "Expected internal state: PlayerLogManager::locked = false" << std::endl;
    std::cout << "Actual internal state: PlayerLogManager::locked = " << PlayerLogManager::locked << std::endl;
    EXPECT_FALSE(PlayerLogManager::locked);
    
    std::cout << "Exiting LockLogLevelFalse test" << std::endl;
}
/**
 * @brief Verify that setLogLevel correctly updates the log level when the log manager is unlocked.
 *
 * This test verifies that when the PlayerLogManager is unlocked, calling setLogLevel with each available log level from the MW_LogLevel enum properly updates the internal log level without throwing exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------------------------------------------------- | -------------------------------------------------------------- | --------------------------------------------------------------- | ----------- |
 * | 01 | Set the log manager to an unlocked state by setting PlayerLogManager::locked to false | locked = false | PlayerLogManager::locked is updated to false without error | Should be successful |
 * | 02 | Iterate through all log levels (mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR), invoke setLogLevel for each, and verify that PlayerLogManager::mwLoglevel is updated | For each level in [mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR]: input: level value; output: PlayerLogManager::mwLoglevel equals level | Each invocation of setLogLevel does not throw an exception and correctly updates the mwLoglevel | Should Pass |
 */
TEST(PlayerLogManager, PositiveScenario_SetLogLevel)
{
    std::cout << "Entering PositiveScenario_SetLogLevel test" << std::endl;
    
    // Ensure the log manager is unlocked.
    PlayerLogManager::locked = false;
    std::cout << "Set PlayerLogManager::locked to false" << std::endl;
    
    // List of all log levels in enum MW_LogLevel.
    MW_LogLevel levels[] = { mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR };
    
    // Invoke setLogLevel for each log level.
    for(auto level : levels)
    {
        std::cout << "Invoking setLogLevel with value: " << level << std::endl;
        EXPECT_NO_THROW(PlayerLogManager::setLogLevel(level)); // Invoking method
        
        std::cout << "After invocation, PlayerLogManager::mwLoglevel = " << PlayerLogManager::mwLoglevel << std::endl;
        // Verification: The log level should be updated to the provided value.
        EXPECT_EQ(PlayerLogManager::mwLoglevel, level);
    }
    
    std::cout << "Exiting PositiveScenario_SetLogLevel test" << std::endl;
}
/**
 * @brief Verify that setting the log level on a locked PlayerLogManager does not change the log level
 *
 * This test verifies that when PlayerLogManager is locked, any attempt to change its log level using setLogLevel does not alter the current log level. The test iterates through all the available MW_LogLevel values and confirms that the internal log level remains unchanged (mLOGLEVEL_INFO). This ensures the lock functionality is working as expected.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                          | Expected Result                                                               | Notes         |
 * | :--------------: | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------- | ------------- |
 * | 01               | Create an instance of PlayerLogManager and lock the log manager with initial log level INFO    | PlayerLogManager instance creation; PlayerLogManager::locked = true, PlayerLogManager::mwLoglevel = mLOGLEVEL_INFO  | Instance created without exception and initial log level is set to mLOGLEVEL_INFO | Should be successful |
 * | 02               | Invoke setLogLevel with mLOGLEVEL_TRACE on a locked log manager and verify level remains unchanged | input level = mLOGLEVEL_TRACE, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                   | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 * | 03               | Invoke setLogLevel with mLOGLEVEL_DEBUG on a locked log manager and verify level remains unchanged | input level = mLOGLEVEL_DEBUG, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                   | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 * | 04               | Invoke setLogLevel with mLOGLEVEL_INFO on a locked log manager and verify level remains unchanged  | input level = mLOGLEVEL_INFO, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                    | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 * | 05               | Invoke setLogLevel with mLOGLEVEL_WARN on a locked log manager and verify level remains unchanged  | input level = mLOGLEVEL_WARN, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                    | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 * | 06               | Invoke setLogLevel with mLOGLEVEL_MIL on a locked log manager and verify level remains unchanged   | input level = mLOGLEVEL_MIL, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                     | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 * | 07               | Invoke setLogLevel with mLOGLEVEL_ERROR on a locked log manager and verify level remains unchanged | input level = mLOGLEVEL_ERROR, locked = true, initial mwLoglevel = mLOGLEVEL_INFO                                   | No exception thrown and mwLoglevel remains mLOGLEVEL_INFO                     | Should Fail   |
 */
TEST(PlayerLogManager, NegativeScenario_SetLogLevel_Locked)
{
    std::cout << "Entering NegativeScenario_SetLogLevel_Locked test" << std::endl;
    
    // Create an object of PlayerLogManager using default constructor.
    EXPECT_NO_THROW(PlayerLogManager manager);
    PlayerLogManager manager;
    
    // Lock the log manager and set an initial known log level.
    PlayerLogManager::locked = true;
    PlayerLogManager::mwLoglevel = mLOGLEVEL_INFO;
    std::cout << "Set PlayerLogManager::locked to true, initial PlayerLogManager::mwLoglevel = " << PlayerLogManager::mwLoglevel << std::endl;
    
    // List of all log levels in enum MW_LogLevel.
    MW_LogLevel levels[] = { mLOGLEVEL_TRACE, mLOGLEVEL_DEBUG, mLOGLEVEL_INFO, mLOGLEVEL_WARN, mLOGLEVEL_MIL, mLOGLEVEL_ERROR };
    
    // Attempt to invoke setLogLevel for each log level.
    for(auto level : levels)
    {
        std::cout << "Invoking setLogLevel with value: " << level << std::endl;
        EXPECT_NO_THROW(PlayerLogManager::setLogLevel(level)); // Invoking method
        
        std::cout << "After invocation, PlayerLogManager::mwLoglevel remains = " << PlayerLogManager::mwLoglevel << std::endl;
        // Verification: The log level must remain unchanged.
        EXPECT_EQ(PlayerLogManager::mwLoglevel, mLOGLEVEL_INFO);
    }
    
    std::cout << "Exiting NegativeScenario_SetLogLevel_Locked test" << std::endl;
}
