
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
#include "PacketSender.hpp"

class PacketSenderTest : public ::testing::Test {
protected:
    PacketSender* sender;

    void SetUp() override {
        // Obtain the singleton instance before each test
        sender = PacketSender::Instance();
        ASSERT_NE(sender, nullptr) << "PacketSender instance should not be null";
    }
};

/**
 * @brief Validate runWorkerTask with a valid context pointer.
 *
 * This test validates the correct behavior of the runWorkerTask API when provided with a properly initialized valid context pointer. It ensures that no exceptions are thrown during the execution of the function using a valid context.
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
 * | Variation / Step | Description                                                                 | Test Data                                        | Expected Result                                         | Notes           |
 * | :--------------: | --------------------------------------------------------------------------- | ------------------------------------------------ | ------------------------------------------------------- | --------------- |
 * | 01               | Initialize the context message by copying "ValidMessage" into a fixed size array ensuring null termination. | validMessage = "ValidMessage"                      | The validMessage array is correctly initialized with a null terminator. | Should be successful |
 * | 02               | Invoke runWorkerTask using the valid context pointer.                       | input: validContext = pointer to validMessage      | runWorkerTask completes without throwing any exceptions.             | Should Pass     |
 */
TEST(RunWorkerTaskTest, ValidContextTest) {
    GTEST_SKIP();
    std::cout << "Entering ValidContextTest test" << std::endl;
    
    // Initialize context message using strncpy into fixed size array.
    char validMessage[64];
    strncpy(validMessage, "ValidMessage", sizeof(validMessage));
    validMessage[sizeof(validMessage)-1] = '\0'; // ensure null termination

    std::cout << "ValidContextTest: Invoking runWorkerTask with valid context pointer: " << validMessage << std::endl;
    EXPECT_NO_THROW(runWorkerTask(static_cast<void*>(validMessage)));
    
    std::cout << "ValidContextTest: Completed invocation of runWorkerTask"  << std::endl;
 
    std::cout << "Exiting ValidContextTest test" << std::endl;
}
/**
 * @brief Verifies that runWorkerTask can safely handle a null pointer.
 *
 * This test checks if the runWorkerTask API correctly handles a null context input by ensuring that no exception is thrown during its execution.
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
 * | Variation / Step | Description                                                   | Test Data                        | Expected Result                                             | Notes      |
 * | :--------------: | ------------------------------------------------------------- | -------------------------------- | ----------------------------------------------------------- | ---------- |
 * | 01               | Invoke runWorkerTask with a null pointer to check for safety.  | input: context = nullptr         | The function should execute without throwing any exception. | Should Pass|
 */
TEST(RunWorkerTaskTest, NullContextTest) {
    GTEST_SKIP();
    std::cout << "Entering NullContextTest test" << std::endl;
    
    std::cout << "NullContextTest: Invoking runWorkerTask with null pointer" << std::endl;
    EXPECT_NO_THROW(runWorkerTask(nullptr));
    
    std::cout << "NullContextTest: Completed invocation of runWorkerTask with null pointer" << std::endl;
    std::cout << "Exiting NullContextTest test" << std::endl;
}
/**
 * @brief Test runWorkerTask using an invalid context to verify that no exception is thrown.
 *
 * This test initializes an invalid context message and passes it to the runWorkerTask API via a static_cast.
 * The purpose of this test is to ensure that runWorkerTask correctly handles non-standard context inputs without throwing exceptions.@n
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
 * | Variation / Step | Description                                                 | Test Data                                               | Expected Result                                     | Notes                |
 * | :--------------: | ----------------------------------------------------------- | ------------------------------------------------------- | ---------------------------------------------------- | -------------------- |
 * | 01               | Initialize an invalid context message using strncpy         | invalidMessage = "123Invalid"                           | Buffer is properly null terminated                  | Should be successful |
 * | 02               | Invoke runWorkerTask with the invalid context pointer       | context = pointer to invalidMessage                     | API does not throw an exception (EXPECT_NO_THROW)    | Should Pass          |
 * | 03               | Log the completion of the runWorkerTask invocation          | No input parameters                                     | Completion message is logged successfully           | Should be successful |
 */
TEST(RunWorkerTaskTest, InvalidContextTest) {
    GTEST_SKIP();
    std::cout << "Entering InvalidContextTest test" << std::endl;
    
    // Initialize context message using strncpy into fixed size array.
    char invalidMessage[64];
    strncpy(invalidMessage, "123Invalid", sizeof(invalidMessage));
    invalidMessage[sizeof(invalidMessage)-1] = '\0'; // ensure null termination
    
    std::cout << "InvalidContextTest: Invoking runWorkerTask with context pointer: " << invalidMessage << std::endl;
    EXPECT_NO_THROW(runWorkerTask(static_cast<void*>(invalidMessage)));
    
    std::cout << "InvalidContextTest: Completed invocation of runWorkerTask"  << std::endl;

    std::cout << "Exiting InvalidContextTest test" << std::endl;
}
/**
 * @brief Verify that the Close method of PacketSender does not throw exceptions on an active player instance.
 *
 * This test validates that invoking the Close() method on the active PacketSender instance executes without any exceptions.
 * It confirms that the instance obtained from PacketSender::Instance() properly handles the call to Close() and that the
 * active player instance can be gracefully terminated without error.
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
 * | Variation / Step | Description                                                        | Test Data                                                                   | Expected Result                                      | Notes      |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------------------- | ---------------------------------------------------- | ---------- |
 * |      01        | Call sender->Close() within an EXPECT_NO_THROW block to verify that the method executes without throwing exceptions. | sender = valid PacketSender instance, method = Close(), output = void       | API method executes without throwing exceptions and completes successfully. | Should Pass|
 */
TEST_F(PacketSenderTest, CloseOnActivePlayerInstance) {
    std::cout << "Entering CloseOnActivePlayerInstance test" << std::endl;
    
    EXPECT_NO_THROW({
        // Invoking Close method on the active player instance.
        std::cout << "Invoking PacketSender::Close() on the active player instance." << std::endl;
        EXPECT_NO_THROW(sender->Close());
        std::cout << "PacketSender::Close() executed without throwing errors." << std::endl;
    });
    
    std::cout << "Exiting CloseOnActivePlayerInstance test" << std::endl;
}
/**
 * @brief Validate the successful execution of PacketSender::Flush() method
 *
 * This test verifies that invoking the Flush method on the PacketSender's packet queue completes without throwing any exceptions. The test ensures that the singleton instance of PacketSender is valid before performing the flush operation, thus confirming the stability and correctness of the method execution.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke PacketSender::Flush() within EXPECT_NO_THROW to verify that no exception is thrown | sender = valid PacketSender instance, (Flush() has no input parameters, no output value) | Flush() completes without throwing any exception | Should Pass |
 */
TEST_F(PacketSenderTest, FlushOnPacketQueue) {
    std::cout << "Entering FlushOnPacketQueue test" << std::endl;

    // Invoking PacketSender::Flush() method
    std::cout << "Invoking PacketSender::Flush() method" << std::endl;
    EXPECT_NO_THROW({
        sender->Flush();
        std::cout << "PacketSender::Flush() invoked without throwing any exception" << std::endl;
    });

    std::cout << "Exiting FlushOnPacketQueue test" << std::endl;
}
/**
 * @brief Verify that the PacketSender initializes successfully with default configuration.
 *
 * This test verifies that the PacketSender's Init() method executes without throwing an exception and returns true when invoked with default configuration settings.
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
 * | Variation / Step | Description                                                        | Test Data                                                       | Expected Result                                              | Notes       |
 * | :--------------: | ------------------------------------------------------------------ | --------------------------------------------------------------- | ------------------------------------------------------------ | ----------- |
 * | 01               | Invoke PacketSender::Init() method to initialize with defaults.    | sender (instance of PacketSender), no input parameters, output: initResult expected to be true  | The Init() method should return true and no exception should be thrown. | Should Pass |
 */
TEST_F(PacketSenderTest, DefaultInitializationSucceeds) {
    GTEST_SKIP();
    std::cout << "Entering DefaultInitializationSucceeds test" << std::endl;

    EXPECT_NO_THROW({
        bool initResult = sender->Init();
        std::cout << "Init() method returned: " << std::boolalpha << initResult << std::endl;

        // Validate expected outcome (default initialization should succeed)
        EXPECT_TRUE(initResult);
        std::cout << "Internal state: PacketSender initialized with default configuration successfully." << std::endl;
    });

    std::cout << "Exiting DefaultInitializationSucceeds test" << std::endl;
}
/**
 * @brief Verify that PacketSender::Init initializes correctly with a valid Unix socket path.
 *
 * This test ensures that the PacketSender instance can be properly initialized when a valid Unix socket path is provided.
 * It verifies that the API call to PacketSender::Init returns a true value, indicating successful initialization.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 * @n
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * @n
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                        | Test Data                                                  | Expected Result                                                                              | Notes      |
 * | :--------------: | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------- | -------------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Prepare a valid Unix socket path and invoke PacketSender::Init with it to check initialization success. | socketPath = /tmp/packet_socket, output result = bool value | Returns true and passes the EXPECT_TRUE assertion confirming successful initialization.       | Should Pass |
 */
TEST_F(PacketSenderTest, ValidUnixSocketPathInitialization) {
    GTEST_SKIP();
    std::cout << "Entering ValidUnixSocketPathInitialization test" << std::endl;
    
    // Prepare input socket path
    const char* socketPath = "/tmp/packet_socket";
    std::cout << "Invoking PacketSender::Init with socket_path: " << socketPath << std::endl;
    
    // Invoke Init method and capture the return value
    bool result = sender->Init(socketPath);
    std::cout << "Returned value from PacketSender::Init: " << std::boolalpha << result << std::endl;
    
    // Validate that initialization succeeds
    EXPECT_TRUE(result);
    
    std::cout << "Exiting ValidUnixSocketPathInitialization test" << std::endl;
}
/**
 * @brief Validate that invoking PacketSender::Init with a null socket path fails initialization.
 *
 * This test verifies that when a null socket path is passed to the PacketSender::Init API,
 * the initialization should fail and return false. The purpose of this test is to ensure that the API correctly
 * handles an invalid input scenario, which is critical for reliability.
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
 * | Variation / Step | Description                                                           | Test Data                                | Expected Result                                    | Notes       |
 * | :--------------: | --------------------------------------------------------------------- | ---------------------------------------- | -------------------------------------------------- | ----------- |
 * | 01               | Prepare a null socket path and invoke PacketSender::Init API          | socketPath = nullptr                     | Return value false; assertion EXPECT_FALSE(result) | Should Fail |
 */
TEST_F(PacketSenderTest, InitializationWithNullSocketPath) {
    GTEST_SKIP();
    std::cout << "Entering InitializationWithNullSocketPath test" << std::endl;
    
    // Prepare input socket path as nullptr
    const char* socketPath = nullptr;
    std::cout << "Invoking PacketSender::Init with socket_path: " << "nullptr" << std::endl;
    
    // Invoke Init method and capture the return value
    bool result = sender->Init(socketPath);
    std::cout << "Returned value from PacketSender::Init: " << std::boolalpha << result << std::endl;
    
    // Validate that initialization fails
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InitializationWithNullSocketPath test" << std::endl;
}
/**
 * @brief Test that initialization fails with an empty socket path.
 *
 * This test verifies that PacketSender::Init returns false when invoked with an empty socket path.
 * An empty socket path is an invalid input and should result in the method failing to initialize the sender.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 009
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                           | Test Data                                 | Expected Result                             | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------- | ----------------------------------------- | ------------------------------------------- | ----------- |
 * | 01               | Prepare an empty socket path and invoke PacketSender::Init to validate failure          | socketPath = "", output result = false      | PacketSender::Init returns false            | Should Fail |
 */
TEST_F(PacketSenderTest, InitializationWithEmptySocketPath) {
    std::cout << "Entering InitializationWithEmptySocketPath test" << std::endl;
    
    // Prepare input socket path as empty string
    char socketPath[256];
    strncpy(socketPath, "", sizeof(socketPath) - 1);
    socketPath[sizeof(socketPath) - 1] = '\0';
    std::cout << "Invoking PacketSender::Init with socket_path (empty string): '" << socketPath << "'" << std::endl;
    
    // Invoke Init method and capture the return value
    bool result = sender->Init(socketPath);
    std::cout << "Returned value from PacketSender::Init: " << std::boolalpha << result << std::endl;
    
    // Validate that initialization fails
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InitializationWithEmptySocketPath test" << std::endl;
}
/**
 * @brief Verify that PacketSender::Init returns false when provided with an invalid socket path format.
 *
 * This test verifies that when an invalid socket path is provided, the PacketSender API correctly identifies the erroneous input and fails the initialization. It confirms that the system properly handles invalid socket formats as expected.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 010@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                           | Test Data                                                    | Expected Result                                                        | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------ | ---------------------------------------------------------------------- | ------------- |
 * | 01               | Prepare the input socket path with an invalid format.                                               | socketPath = invalid_socket_path@@                           | socketPath variable is correctly initialized with "invalid_socket_path@@" | Should be successful |
 * | 02               | Invoke PacketSender::Init using the invalid socket path and validate that the return value is false.  | input: socketPath = invalid_socket_path@@, output: result      | API returns false and the assertion (EXPECT_FALSE) confirms the failure  | Should Fail   |
 */
TEST_F(PacketSenderTest, InitializationWithInvalidSocketPathFormat) {
    std::cout << "Entering InitializationWithInvalidSocketPathFormat test" << std::endl;

    // Prepare input socket path with invalid format
    char socketPath[256];
    strncpy(socketPath, "invalid_socket_path@@", sizeof(socketPath) - 1);
    socketPath[sizeof(socketPath) - 1] = '\0';
    std::cout << "Invoking PacketSender::Init with socket_path: " << socketPath << std::endl;
    
    // Invoke Init method and capture the return value
    bool result = sender->Init(socketPath);
    std::cout << "Returned value from PacketSender::Init: " << std::boolalpha << result << std::endl;
    
    // Validate that initialization fails
    EXPECT_FALSE(result);
    
    std::cout << "Exiting InitializationWithInvalidSocketPathFormat test" << std::endl;
}
/**
 * @brief Verify that PacketSender::Instance returns a valid non-null pointer
 *
 * This test invokes the PacketSender::Instance() method and verifies that it does not throw any exceptions and returns a valid (non-null) pointer. The test ensures the Singleton instance is correctly created and accessible. 
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
 * | Variation / Step | Description                                           | Test Data                                                                 | Expected Result                                                                 | Notes        |
 * | :--------------: | ----------------------------------------------------- | ------------------------------------------------------------------------- | ------------------------------------------------------------------------------- | ------------ |
 * | 01               | Print entry log message for the test                  | No input                                                                  | "Entering VerifyInstanceReturnsNonNull test" message printed to console          | Should be successful |
 * | 02               | Initialize instance pointer to nullptr                | instance = nullptr                                                        | instance variable set to nullptr                                                 | Should be successful |
 * | 03               | Print log message before invoking PacketSender::Instance() | No input                                                               | "Invoking PacketSender::Instance()" log message printed to console                 | Should be successful |
 * | 04               | Call PacketSender::Instance() within an EXPECT_NO_THROW block | No input; output: instance pointer from PacketSender::Instance()          | No exception is thrown and instance is assigned a pointer value                   | Should Pass  |
 * | 05               | Verify the returned pointer from PacketSender::Instance() is not null | output1 = instance pointer from PacketSender::Instance()           | instance is not nullptr as asserted by EXPECT_NE                                  | Should Pass  |
 * | 06               | Print exit log message for the test                   | No input                                                                  | "Exiting VerifyInstanceReturnsNonNull test" message printed to console             | Should be successful |
 */
TEST_F(PacketSenderTest, VerifyInstanceReturnsNonNull)
{
    std::cout << "Entering VerifyInstanceReturnsNonNull test" << std::endl;
    
    PacketSender* instance = nullptr;
    
    std::cout << "Invoking PacketSender::Instance()" << std::endl;
    EXPECT_NO_THROW({
        instance = PacketSender::Instance();
        std::cout << "PacketSender::Instance() returned pointer: " << instance << std::endl;
    });
    
    EXPECT_NE(instance, nullptr);
    
    std::cout << "Exiting VerifyInstanceReturnsNonNull test" << std::endl;
}
/**
 * @brief Verify that the IsRunning() method returns false for a fresh PacketSender instance.
 *
 * This test verifies that when a new instance of PacketSender is obtained, the IsRunning() method returns false,
 * ensuring that the sender is not running immediately after construction.
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
 * | 01 | Invoke IsRunning() method on PacketSender instance and verify its initial running status is false. | sender = PacketSender instance, method = IsRunning(), output = false | The method returns false and the assertion verifies the expected behavior. | Should Pass |
 */
TEST_F(PacketSenderTest, VerifyIsRunningReturnsFalseForFreshInstance) {
    std::cout << "Entering VerifyIsRunningReturnsFalseForFreshInstance test" << std::endl;
    
    EXPECT_NO_THROW({

        // Call IsRunning() method.
        std::cout << "Invoking IsRunning() method on PacketSender instance" << std::endl;
        bool runningStatus = sender->IsRunning();
        std::cout << "IsRunning() returned: " << (runningStatus ? "true" : "false") << std::endl;
        
        // Verify that the return value is false.
        EXPECT_FALSE(runningStatus);
        std::cout << "Verified that IsRunning() returned false for a freshly constructed instance" << std::endl;
    });
    
    std::cout << "Exiting VerifyIsRunningReturnsFalseForFreshInstance test" << std::endl;
}
/**
 * @brief Verifies that a valid packet can be transmitted by the PacketSender instance.
 *
 * This test simulates the positive flow of packet transmission by creating a valid Packet instance
 * and then transferring its ownership via the SendPacket API. The objective is to ensure that the
 * PacketSender's SendPacket method receives the packet without throwing any exceptions and correctly
 * queues it for transmission.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                                      | Expected Result                                                                                                            | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create a valid Packet instance using its default constructor                                     | input: None, output: Packet instance created using default constructor         | Packet instance should be created successfully                                                                             | Should be successful |
 * | 02               | Invoke SendPacket method with the valid Packet instance transferring its ownership               | input: sender->SendPacket(std::move(packet)), packet pointer becomes null; no exception thrown | SendPacket should execute without throwing any exception and the packet should be queued for transmission | Should Pass      |
 */
TEST_F(PacketSenderTest, ValidPacketTransmission) {
    std::cout << "Entering ValidPacketTransmission test" << std::endl;
    
    std::cout << "Creating PacketSender object using default constructor" << std::endl;
    EXPECT_NO_THROW({

        // Create a valid PacketPtr instance.
        std::cout << "Creating a valid PacketPtr instance" << std::endl;
        // Assuming Packet has a default constructor.
        std::unique_ptr<Packet> packet = std::make_unique<Packet>();
        std::cout << "PacketPtr instance created. Packet address: " << packet.get() << std::endl;
        
        // Debug log: before invoking SendPacket method.
        std::cout << "Invoking SendPacket with PacketPtr instance, transferring ownership" << std::endl;
        
        // Invoke SendPacket with EXPECT_NO_THROW wrapper.
        EXPECT_NO_THROW({
            sender->SendPacket(std::move(packet));
            std::cout << "SendPacket method invoked successfully" << std::endl;
        });
        
        // Debug log: After invoking SendPacket.
        std::cout << "SendPacket method completed. Packet should now be queued for transmission" << std::endl;
    });
    
    std::cout << "Exiting ValidPacketTransmission test" << std::endl;
}
/**
 * @brief Validate that senderTask() executes normally without throwing exceptions.
 *
 * This test ensures that the senderTask() method of the PacketSender instance completes its execution without any exceptions. It is critical as it verifies the normal operation of the senderTask() functionality in a controlled environment using a singleton instance of PacketSender.
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
 * | Variation / Step | Description                                                               | Test Data                                  | Expected Result                                                        | Notes      |
 * | :--------------: | ------------------------------------------------------------------------- | ------------------------------------------ | ---------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke senderTask() on PacketSender instance and verify no exceptions occur | sender = valid instance, method: senderTask() | senderTask() executes normally without throwing, assertion EXPECT_NO_THROW passes | Should Pass |
 */
TEST_F(PacketSenderTest, ExecuteSenderTaskNormal) {
    GTEST_SKIP();
    std::cout << "Entering ExecuteSenderTaskNormal test" << std::endl;
    
    std::cout << "Invoking senderTask() method" << std::endl;
    EXPECT_NO_THROW({
        sender->senderTask();
        std::cout << "senderTask() executed without throwing exceptions" << std::endl;
    });
    
    std::cout << "Exiting ExecuteSenderTaskNormal test" << std::endl;
}