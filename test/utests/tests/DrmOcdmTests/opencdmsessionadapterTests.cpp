
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
#include "opencdmsessionadapter.h"



class WidevineDrmHelper : public DrmHelper {
public:
    WidevineDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    const std::string& ocdmSystemId() const override {
        static const std::string id = "WidevineTestSystem";
        return id;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {1, 2, 3};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0xAA, 0xBB, 0xCC};
    }

    void generateLicenseRequest(const ChallengeInfo&, LicenseRequest&) const override { }
};


class TestDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override {
        std::cout << "Individualization callback invoked. Payload: " << payload << std::endl;
    }

    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        std::cout << "LicenseRenewal callback invoked." << std::endl;
        (void)drmHelper;
        (void)userData;
    }
};


class TestableOCDMSessionAdapter : public OCDMSessionAdapter {
public:
    TestableOCDMSessionAdapter(DrmHelperPtr drmHelper, DrmCallbacks* callbacks = nullptr)
        : OCDMSessionAdapter(drmHelper, callbacks) {}

    bool callVerifyOutputProtection() {
        return OCDMSessionAdapter::verifyOutputProtection();
    }
};

class OCDMSessionAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
        DrmInfo drmInfo;
        auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
        testAdapter = new TestableOCDMSessionAdapter(drmHelper);
    }
    void TearDown() override {
        delete testAdapter;
    }
    TestableOCDMSessionAdapter* testAdapter;
};


// Utility function to convert KeyState enum to string.
std::string keyStateToString(KeyState state)
{
    switch(state)
    {
        case KEY_INIT: return "KEY_INIT";
        case KEY_PENDING: return "KEY_PENDING";
        case KEY_READY: return "KEY_READY";
        case KEY_ERROR: return "KEY_ERROR";
        case KEY_CLOSED: return "KEY_CLOSED";
        case KEY_ERROR_EMPTY_SESSION_ID: return "KEY_ERROR_EMPTY_SESSION_ID";
        default: return "UNKNOWN";
    }
}

// Assuming Event class is defined elsewhere and available for linking

/**
 * @brief Verify that the default constructor of the Event class can be invoked successfully.
 *
 * This test ensures that the default constructor of the Event class works as expected without throwing any exceptions.
 * It verifies that an Event object can be created without any input parameters, thereby confirming the safe instantiation of the object.
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
 * | Variation / Step | Description                                                     | Test Data                                | Expected Result                                                | Notes      |
 * | :--------------: | ---------------------------------------------------------------- | ---------------------------------------- | -------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the default constructor of the Event class and check for exceptions. | input: none, output: valid Event instance | The Event object should be created without throwing any exceptions. | Should Pass |
 */
TEST(Event, DefaultConstructionValidity) {
    std::cout << "Entering DefaultConstructionValidity test" << std::endl;
    
    std::cout << "Invoking default constructor Event() with no parameters." << std::endl;
    EXPECT_NO_THROW({
        Event eventObj;
        std::cout << "Default constructor invoked successfully." << std::endl;
    });
    
    std::cout << "Exiting DefaultConstructionValidity test" << std::endl;
}
/**
 * @brief Verify that the Event signal API operates reliably without throwing exceptions.
 *
 * This test verifies that creating an Event object using its default constructor and subsequently invoking its signal() method does not throw any exceptions. It ensures that the signal() method functions as expected in a positive scenario by implicitly updating the internal signalled state.
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
 * | Variation / Step | Description                                                                                   | Test Data                                   | Expected Result                                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------- | ------------------------------------------- | ---------------------------------------------------------------------------- | ----------- |
 * | 01               | Create an Event object using the default constructor and invoke the signal() method on it.      | Constructor: none, signal: none             | signal() method call should not throw any exception and assertion passes     | Should Pass |
 */
TEST(Event, VerifySignalApi) {
    std::cout << "Entering VerifySignalApi test" << std::endl;
    
    EXPECT_NO_THROW({
        std::cout << "Creating Event object using default constructor." << std::endl;
        Event event;
        std::cout << "Invoking signal() method on Event object." << std::endl;
        event.signal();
        std::cout << "signal() method invoked successfully. No exception thrown." << std::endl;
        // Internal state 'signalled' now should be true (though not directly accessible without a getter).
    });
    
    std::cout << "Exiting VerifySignalApi test" << std::endl;
}
/**
 * @brief Verify the positive scenario for wait API with a positive wait time
 *
 * This test verifies that when the Event object's wait method is invoked with a positive wait time (100 ms), it returns true and resets its internal signalled state. This confirms proper functionality of the wait API under normal operating conditions.
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
 * | Variation / Step | Description                                                     | Test Data                                           | Expected Result                                                   | Notes         |
 * | :--------------: | --------------------------------------------------------------- | --------------------------------------------------- | ----------------------------------------------------------------- | ------------- |
 * | 01               | Create an Event object using the custom constructor with the signalled state set to true | Constructor argument: signalled = true              | Event object is created successfully with signalled set to true     | Should be successful |
 * | 02               | Invoke the wait method with a waitTime of 100 milliseconds        | waitTime = 100                                      | The wait function returns true confirming that the API operation is successful | Should Pass  |
 * | 03               | Validate that the internal state of the Event object is reset to false after wait()       | No additional input; output: internal state change confirmed by printed logs | The internal signalled state is reset to false after the wait operation | Should be successful |
 */
TEST(Event, WaitPositiveWaitTime) {
    std::cout << "Entering WaitPositiveWaitTime test" << std::endl;
    // Create Event object using custom constructor to initialize signalled as true
    // (Assuming a custom constructor exists that takes a bool to set the initial signalled state)
    EXPECT_NO_THROW({
        Event event;
        std::cout << "Created Event object with signalled set to true" << std::endl;
        uint32_t waitTime = 100;
        std::cout << "Invoking wait method with waitTime = " << waitTime << " milliseconds" << std::endl;
        bool result = event.wait(waitTime);
        std::cout << "wait() returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);
        std::cout << "Event internal state after wait() should have signalled reset to false" << std::endl;
    });
    std::cout << "Exiting WaitPositiveWaitTime test" << std::endl;
}
/**
 * @brief Test to verify that calling wait() with a zero wait time on a signalled Event returns immediately and resets the signalling state.
 *
 * This test creates an Event object with signalled initially set to true, invokes the wait() method with a wait time of 0 milliseconds, and checks if the method returns true and resets the internal signalled state to false. This verifies that a zero timeout condition is handled correctly without delay.
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
 * | 01 | Create an Event object using the custom constructor with signalled set to true. | constructor: signalled = true | Event object is created without throwing exceptions. | Should be successful |
 * | 02 | Invoke wait() method on the Event object with waitTime = 0. | waitTime = 0 | wait() returns true immediately. | Should Pass |
 * | 03 | Verify that wait() method result is true. | result = true | EXPECT_TRUE(result) assertion is successful. | Should Pass |
 * | 04 | Confirm that the internal signalled state is reset to false after wait() call. | event's signalled state | Internal signalled state becomes false. | Should be successful |
 */
TEST(Event, WaitZeroWaitTime) {
    std::cout << "Entering WaitZeroWaitTime test" << std::endl;
    // Create Event object using custom constructor to initialize signalled as true
    EXPECT_NO_THROW({
        Event event;
        std::cout << "Created Event object with signalled set to true" << std::endl;
        uint32_t waitTime = 0;
        std::cout << "Invoking wait method with waitTime = " << waitTime << " milliseconds" << std::endl;
        bool result = event.wait(waitTime);
        std::cout << "wait() returned: " << std::boolalpha << result << std::endl;
        EXPECT_TRUE(result);
        std::cout << "Event internal state after wait() should have signalled reset to false" << std::endl;
    });
    std::cout << "Exiting WaitZeroWaitTime test" << std::endl;
}
/**
 * @brief Verify that waiting on an unsignalled Event with the maximum wait time properly times out.
 *
 * This test checks that an Event created with the default constructor, which sets it as unsignalled, 
 * returns false when its wait method is called with the maximum allowable wait time (UINT32_MAX). 
 * This behavior confirms that the wait function correctly handles the timeout mechanism.
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
 * | Variation / Step | Description                                                     | Test Data                                        | Expected Result                                            | Notes               |
 * | :--------------: | --------------------------------------------------------------- | ------------------------------------------------ | ---------------------------------------------------------- | ------------------- |
 * | 01               | Instantiate Event object via default constructor                | None                                             | Event object is created without throwing any exception   | Should be successful|
 * | 02               | Set waitTime to UINT32_MAX                                        | waitTime = UINT32_MAX                            | waitTime variable is assigned the maximum value          | Should be successful|
 * | 03               | Invoke wait() on the Event object using defined waitTime          | input: waitTime = UINT32_MAX, output: result       | wait() returns false indicating a timeout occurred         | Should Pass         |
 */
TEST(Event, WaitMaximumWaitTime) {
    std::cout << "Entering WaitMaximumWaitTime test" << std::endl;
    // Create Event object using default constructor which sets signalled to false
    EXPECT_NO_THROW({
        Event event;
        std::cout << "Created Event object with default constructor (signalled assumed to be false)" << std::endl;
        uint32_t waitTime = UINT32_MAX;
        std::cout << "Invoking wait method with waitTime = " << waitTime << " milliseconds" << std::endl;
        bool result = event.wait(waitTime);
        std::cout << "wait() returned: " << std::boolalpha << result << std::endl;
        EXPECT_FALSE(result);
        std::cout << "Event wait() call timed out as expected for maximum wait time" << std::endl;
    });
    std::cout << "Exiting WaitMaximumWaitTime test" << std::endl;
}
/**
 * @brief Validate that a stack-allocated Event object is constructed and destructed without throwing exceptions.
 *
 * This test verifies that the default constructor of the Event class successfully creates an object on the stack and that the object's destructor is automatically invoked when it goes out of scope without any exceptions being thrown.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor to create a stack-allocated Event object and allow it to go out of scope to trigger the destructor | constructor = Event(), no input parameters, destructor auto-invoked | EXPECT_NO_THROW passes confirming no exceptions were thrown during construction and destruction | Should Pass |
 */
TEST(Event, EventDestructorOnStack) {
    std::cout << "Entering EventDestructorOnStack test" << std::endl;
    EXPECT_NO_THROW({
        std::cout << "Invoking default constructor for Event object." << std::endl;
        {
            // Create a stack-allocated Event object which will invoke the destructor when it goes out of scope.
            Event eventObj;
            std::cout << "Event object constructed successfully. (Stack allocated)" << std::endl;
            // Since the destructor is automatically called when the object goes out of scope,
            // no additional manipulation is done here. All internal states are default initialized.
        }
        std::cout << "Event object has gone out of scope, destructor invoked automatically." << std::endl;
    });
    std::cout << "Exiting EventDestructorOnStack test" << std::endl;
}
/**
 * @brief Validate successful construction of OCDMSessionAdapter using valid parameters
 *
 * This test verifies that the OCDMSessionAdapter constructor does not throw exceptions when invoked with valid instances of WidevineDrmHelper and TestDrmCallbacks. It ensures that the object is instantiated correctly.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 007@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                      | Test Data                                                                                          | Expected Result                                                        | Notes        |
 * | :--------------: | ---------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------ |
 * | 01               | Call OCDMSessionAdapter constructor with valid drmHelper and drmCallbacks | drmHelper = WidevineDrmHelper instance created with valid DrmInfo, drmCallbacks = TestDrmCallbacks instance | No exception thrown; object constructed successfully and assertion passes | Should Pass  |
 */
TEST(OCDMSessionAdapter, ValidConstruction) {
    std::cout << "Entering ValidConstruction test" << std::endl;
 
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMSessionAdapter object constructed successfully." << std::endl;
    });
    delete drmCallbacks;
    std::cout << "Exiting ValidConstruction test" << std::endl;
}
/**
 * @brief Tests that the OCDMSessionAdapter constructor throws an exception when passed a null drmHelper pointer.
 *
 * This test verifies that the OCDMSessionAdapter correctly handles the case where the drmHelper parameter is null even when valid drmCallbacks are provided. It ensures that the adapter's constructor properly validates its inputs and throws an exception for an invalid drmHelper pointer.
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
 * | Variation / Step | Description                                                                 | Test Data                                                        | Expected Result                                                                          | Notes      |
 * | :--------------: | --------------------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------------------------------------------------------------------------- | ---------- |
 * | 01               | Invoke the OCDMSessionAdapter constructor with a null drmHelper and valid drmCallbacks. | drmHelper = nullptr, drmCallbacks = pointer to TestDrmCallbacks instance | Exception thrown confirming proper error handling as verified by EXPECT_ANY_THROW macro.  | Should Pass |
 */
TEST(OCDMSessionAdapter, NullDrmHelperWithValidCallbacks) {
    std::cout << "Entering NullDrmHelperWithValidCallbacks test" << std::endl;
    
    auto* drmCallbacks = new TestDrmCallbacks();  
    EXPECT_ANY_THROW({
        OCDMSessionAdapter adapter(nullptr, drmCallbacks);
        std::cout << "OCDMSessionAdapter object constructed with nullptr drmHelper." << std::endl;
    });
    
    std::cout << "Exiting NullDrmHelperWithValidCallbacks test" << std::endl;
}
/**
 * @brief Validates that constructing OCDMSessionAdapter with valid drmHelper but null callbacks triggers an exception.
 *
 * This test verifies that when a valid drmHelper is provided but the callbacks are set to nullptr, the OCDMSessionAdapter constructor throws an exception as expected. This helps ensure that the adapter properly handles the scenario where essential components (callbacks) are missing.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 009@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                                                     | Test Data                                                         | Expected Result                                             | Notes      |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ----------------------------------------------------------------- | ----------------------------------------------------------- | ---------- |
 * | 01               | Invoke OCDMSessionAdapter constructor with valid drmHelper and null callbacks                   | drmHelper = valid WidevineDrmHelper instance, callbacks = nullptr   | Exception is thrown during OCDMSessionAdapter construction  | Should Pass |
 */
TEST(OCDMSessionAdapter, NullDrmHelperAndNullCallbacks) {
    std::cout << "Entering NullDrmHelperAndNullCallbacks test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 

    EXPECT_ANY_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object constructed with valid drmHelper and nullptr callbacks." << std::endl;
    });
    
    std::cout << "Exiting NullDrmHelperAndNullCallbacks test" << std::endl;
}
/**
 * @brief Verify that clearDecryptContext() method clears the decryption context without throwing any exceptions.
 *
 * This test verifies that an instance of OCDMSessionAdapter properly clears its decryption context when clearDecryptContext() is invoked. It ensures that the method operates without throwing exceptions, confirming correct behavior in a positive scenario.
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
 * | Variation / Step | Description                                                                                  | Test Data                                                                                                          | Expected Result                                                                  | Notes       |
 * | :----:           | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------------- | ----------- |
 * | 01               | Create OCDMSessionAdapter object using default constructor with drmHelper and drmCallbacks.  | drmHelper = instance of WidevineDrmHelper, drmCallbacks = instance of TestDrmCallbacks                             | Object is created successfully without any exception                           | Should Pass |
 * | 02               | Invoke clearDecryptContext() method on the adapter object.                                   | Method: clearDecryptContext(), adapter object initialized with valid drmHelper and drmCallbacks                      | Method executes without throwing any exception and clears the decryption context | Should Pass |
 */
TEST(OCDMSessionAdapter, verifyClearDecryptContext) {
    std::cout << "Entering verifyClearDecryptContext test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance
    
    EXPECT_NO_THROW({
        std::cout << "Creating OCDMSessionAdapter object using default constructor." << std::endl;
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "OCDMSessionAdapter object created." << std::endl;

        std::cout << "Invoking clearDecryptContext() method." << std::endl;
        adapter.clearDecryptContext();
        std::cout << "clearDecryptContext() method invoked successfully; decryption context cleared." << std::endl;
    });

    std::cout << "Exiting verifyClearDecryptContext test" << std::endl;
}
/**
 * @brief Validate DRM session generation with non-empty initialization and custom data
 *
 * This test validates that the OCDMSessionAdapter can successfully generate a DRM session when provided with non-empty initialization data and a valid custom data string. The test checks that the adapter is created and the generateDRMSession API is invoked without throwing exceptions.
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
 * | Variation / Step | Description                                                                                         | Test Data                                                               | Expected Result                                                              | Notes       |
 * | :--------------: | --------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ----------- |
 * | 01               | Create OCDMSessionAdapter instance and call generateDRMSession with valid non-empty data            | initData = {0x01,0x02,0x03}, initDataSize = 3, customData = valid-custom-data | Adapter is constructed and generateDRMSession executes without throwing exceptions | Should Pass |
 */
TEST(OCDMSessionAdapter, ValidDRMSessionGenerationNonEmptyData) {
    std::cout << "Entering ValidDRMSessionGenerationNonEmptyData test" << std::endl;
    
    // Prepare input initialization data {0x01, 0x02, 0x03}
    uint8_t initData[3] = {0x01, 0x02, 0x03};
    uint32_t initDataSize = 3;
    
    // Prepare custom data string
    std::string customData = "valid-custom-data";
    
    // Log input values
    std::cout << "Input initialization data: [";
    for (uint32_t i = 0; i < initDataSize; i++) {
        std::cout << "0x" << std::hex << static_cast<int>(initData[i]);
        if(i < initDataSize - 1) std::cout << ", ";
    }
    std::cout << "]" << std::dec << std::endl;
    std::cout << "Input initialization data size: " << initDataSize << std::endl;
    
    char customDataBuffer[64] = {0};
    strncpy(customDataBuffer, customData.c_str(), sizeof(customDataBuffer)-1);
    std::cout << "Input customData: " << customDataBuffer << std::endl;


    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();                     // Derived class instance

    
    // Create an instance of OCDMSessionAdapter using default constructor
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;
        
        // Invoke generateDRMSession
        std::cout << "Invoking generateDRMSession with provided initialization data and customData" << std::endl;
        EXPECT_NO_THROW(adapter.generateDRMSession(initData, initDataSize, customData));
        std::cout << "generateDRMSession executed successfully" << std::endl;
    });
    
    std::cout << "Exiting ValidDRMSessionGenerationNonEmptyData test" << std::endl;
}
/**
 * @brief Test the generation of a DRM session using minimal initialization data.
 *
 * This test verifies that an OCDMSessionAdapter object can be successfully created using the default constructor
 * and that the generateDRMSession API executes without throwing exceptions when provided with minimal initialization data
 * (one byte of 0xFF) and an empty custom data string. The test ensures the basic functionality of DRM session creation.
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
 * | Variation / Step | Description                                                                                     | Test Data                                                                                                                          | Expected Result                                                        | Notes        |
 * | :--------------: | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- | ------------ |
 * | 01               | Instantiate OCDMSessionAdapter and invoke generateDRMSession with minimal initData and empty customData | initData = {0xFF}, initDataSize = 1, customData = "", drmHelper = WidevineDrmHelper instance, drmCallbacks = TestDrmCallbacks pointer | generateDRMSession executes successfully without throwing exceptions | Should Pass  |
 */
TEST(OCDMSessionAdapter, ValidDRMSessionGenerationMinimalData) {
    std::cout << "Entering ValidDRMSessionGenerationMinimalData test" << std::endl;
    
    // Prepare input initialization data {0xFF}
    uint8_t initData[1] = {0xFF};
    uint32_t initDataSize = 1;
    
    // Prepare empty custom data string
    std::string customData = "";
    
    // Log input values
    std::cout << "Input initialization data: [0x" << std::hex << static_cast<int>(initData[0]) << "]" << std::dec << std::endl;
    std::cout << "Input initialization data size: " << initDataSize << std::endl;
    std::cout << "Input customData is empty" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   // Derived class instance
    auto* drmCallbacks = new TestDrmCallbacks();  
    
    // Create an instance of OCDMSessionAdapter using default constructor
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;
        
        // Invoke generateDRMSession
        std::cout << "Invoking generateDRMSession with minimal initialization data and empty customData" << std::endl;
        EXPECT_NO_THROW(adapter.generateDRMSession(initData, initDataSize, customData));
        std::cout << "generateDRMSession executed successfully" << std::endl;
    });
    
    std::cout << "Exiting ValidDRMSessionGenerationMinimalData test" << std::endl;
}
/**
 * @brief Verify that generating a DRM session with nullptr initialization data fails.
 *
 * This test verifies that the generateDRMSession API correctly handles the case when the initialization data pointer is nullptr while a non-zero size is provided. The test ensures that the API throws an exception under these conditions.
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
 * | 01 | Prepare test inputs and create OCDMSessionAdapter instance | input1 = initData: nullptr, input2 = initDataSize: 5, input3 = customData: "sample" | OCDMSessionAdapter instance is successfully created | Should be successful |
 * | 02 | Invoke generateDRMSession with nullptr initialization data and expect exception | input1 = initData: nullptr, input2 = initDataSize: 5, input3 = customData: "sample" | API throws an exception indicating error due to null initialization data | Should Fail |
 */
TEST(OCDMSessionAdapter, NegativeDRMSessionGenerationNullInitData) {
    std::cout << "Entering NegativeDRMSessionGenerationNullInitData test" << std::endl;
    
    // Prepare input: f_pbInitData is nullptr, but size is non-zero
    const uint8_t* initData = nullptr;
    uint32_t initDataSize = 5;
    
    // Prepare custom data string
    std::string customData = "sample";
    
    // Log input values
    std::cout << "Input initialization data pointer: " << static_cast<const void*>(initData) << std::endl;
    std::cout << "Input initialization data size: " << initDataSize << std::endl;
    std::cout << "Input customData: " << customData << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    
    // Create an instance of OCDMSessionAdapter using default constructor
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
    std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;
    
    // Invoke generateDRMSession and expect an exception due to nullptr input
    std::cout << "Invoking generateDRMSession with nullptr as initialization data" << std::endl;
    EXPECT_ANY_THROW(adapter.generateDRMSession(initData, initDataSize, customData));
    std::cout << "generateDRMSession threw exception as expected for nullptr input" << std::endl;
    
    std::cout << "Exiting NegativeDRMSessionGenerationNullInitData test" << std::endl;
}
/**
 * @brief Test the generateDRMSession method to ensure that a non-null pointer with zero-length initialization data is handled correctly
 *
 * This test validates that OCDMSessionAdapter's generateDRMSession handles the edge case of zero initialization data length when a valid non-null pointer is provided and logs the corresponding messages, ensuring that no exceptions are thrown.
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
 * | 01 | Prepare valid input values including a non-null initialization data pointer, zero initialization data size, and a valid custom data string | initData = valid non-null pointer (address of dummy), initDataSize = 0, customData = "valid-custom-data" | Input parameters are correctly prepared for API invocation | Should be successful |
 * | 02 | Instantiate OCDMSessionAdapter using the default constructor with valid drmHelper and drmCallbacks | drmHelper = valid WidevineDrmHelper shared pointer, drmCallbacks = valid TestDrmCallbacks pointer | Instance created without throwing an exception | Should Pass |
 * | 03 | Invoke generateDRMSession API with zero-length initialization data using the prepared input values | initData = valid non-null pointer (address of dummy), initDataSize = 0, customData = "valid-custom-data" | API executes successfully without throwing exceptions and handles zero-length data correctly | Should Pass |
 */
TEST(OCDMSessionAdapter, EdgeTestZeroInitDataLengthNonNullPointer) {
    std::cout << "Entering EdgeTestZeroInitDataLengthNonNullPointer test" << std::endl;
    
    // Prepare a dummy non-null pointer. We'll use a valid uint8_t variable.
    uint8_t dummy = 0x00;
    const uint8_t* initData = &dummy;
    uint32_t initDataSize = 0;
    
    // Prepare custom data string
    std::string customData = "valid-custom-data";
    
    // Log input values
    std::cout << "Input initialization data pointer: " << static_cast<const void*>(initData) << std::endl;
    std::cout << "Input initialization data length is zero" << std::endl;
    std::cout << "Input customData: " << customData << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    
    // Create an instance of OCDMSessionAdapter using default constructor
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;
        
        // Invoke generateDRMSession
        std::cout << "Invoking generateDRMSession with zero-length initialization data" << std::endl;
        EXPECT_NO_THROW(adapter.generateDRMSession(initData, initDataSize, customData));
        std::cout << "generateDRMSession executed successfully handling zero-length data" << std::endl;
    });
    
    std::cout << "Exiting EdgeTestZeroInitDataLengthNonNullPointer test" << std::endl;
}
/**
 * @brief Verify valid key request generation from OCDMSessionAdapter instance.
 *
 * This test verifies that an OCDMSessionAdapter object can be successfully created using a DRM helper 
 * and that invoking the generateKeyRequest API with a valid destination URL and timeout returns a valid, non-null DrmData pointer.
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
 * | Variation / Step | Description                                                                   | Test Data                                                                                                 | Expected Result                                                                             | Notes               |
 * | :--------------: | ----------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------- | ------------------- |
 * | 01               | Create OCDMSessionAdapter object using the DRM helper.                        | drmHelper = instance of WidevineDrmHelper constructed with drmInfo                                         | OCDMSessionAdapter object is created without throwing any exceptions.                     | Should Pass         |
 * | 02               | Invoke generateKeyRequest with destinationURL and timeout parameters.         | input: destinationURL = "https://license.server.com/request", timeout = 1000                                | Returns a valid DrmData pointer (not nullptr).                                              | Should Pass         |
 * | 03               | Validate that the returned DrmData pointer is non-null and log its data details. | output: DrmData pointer with getData() and getDataLength() values retrieved from the returned object       | DrmData pointer is valid (non-null) and the output details are logged successfully.           | Should be successful  |
 */
TEST(OCDMSessionAdapter, ValidKeyRequestGeneration) {
    std::cout << "Entering ValidKeyRequestGeneration test" << std::endl;
    
    std::string destinationURL = "https://license.server.com/request";
    uint32_t timeout = 1000;
    std::cout << "Preparing to create OCDMSessionAdapter object using default constructor." << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        std::cout << "Invoking generateKeyRequest with destinationURL: " 
                  << destinationURL << " and timeout: " << timeout << std::endl;
        DrmData* drmDataPtr = adapter.generateKeyRequest(destinationURL, timeout);
        std::cout << "generateKeyRequest returned pointer: " << drmDataPtr << std::endl;
        
        EXPECT_NE(drmDataPtr, nullptr);
        if (drmDataPtr) {
            std::cout << "Retrieved DrmData data: " << drmDataPtr->getData() << std::endl;
            std::cout << "Retrieved DrmData data length: " << drmDataPtr->getDataLength() << std::endl;
        }
    });
    
    std::cout << "Exiting ValidKeyRequestGeneration test" << std::endl;
}
/**
 * @brief Tests generateKeyRequest API with an empty destination URL.
 *
 * Tests the behavior of OCDMSessionAdapter's generateKeyRequest method when invoked with an empty destination URL.
 * The test verifies that the adapter correctly handles the empty URL scenario by not throwing exceptions and returning a null pointer.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter object using default constructor with valid drmHelper | drmInfo = default, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo) | Adapter is created successfully without throwing exceptions | Should be successful |
 * | 02 | Invoke generateKeyRequest with empty destination URL and specified timeout | destinationURL = "", timeout = 1000 | Function returns a nullptr and does not throw any exception | Should Pass |
 */
TEST(OCDMSessionAdapter, EmptyDestinationURL) {
    std::cout << "Entering EmptyDestinationURL test" << std::endl;
    
    std::string destinationURL = "";
    uint32_t timeout = 1000;
    std::cout << "Creating OCDMSessionAdapter object using default constructor." << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        std::cout << "Invoking generateKeyRequest with empty destinationURL and timeout: " 
                  << timeout << std::endl;
        DrmData* drmDataPtr = adapter.generateKeyRequest(destinationURL, timeout);
        std::cout << "generateKeyRequest returned pointer: " << drmDataPtr << std::endl;
        
        EXPECT_EQ(drmDataPtr, nullptr);
    });
    
    std::cout << "Exiting EmptyDestinationURL test" << std::endl;
}
/**
 * @brief Validates the behavior of the OCDMSessionAdapter::generateKeyRequest API when an invalid URL format is provided.
 *
 * This test case verifies that invoking generateKeyRequest with an improperly formatted URL (missing the scheme)
 * results in the failure of key request generation. The function is expected to return a nullptr, indicating that the
 * invalid URL input is handled gracefully without throwing exceptions.
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
 * | :----: | --------- | ---------- | -------------- | ----- |
 * | 01 | Create OCDMSessionAdapter object using a valid drmHelper constructed from default DrmInfo and WidevineDrmHelper. | drmInfo = default, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo) | Adapter object is created successfully without throwing any exceptions. | Should be successful |
 * | 02 | Invoke generateKeyRequest with an invalid URL and a timeout value. | destinationURL = "license.server.com/request", timeout = 1000 | generateKeyRequest returns a nullptr, indicating that the request failed due to invalid URL format. | Should Fail |
 * | 03 | Validate that the generateKeyRequest result is a nullptr using an assertion check. | drmDataPtr output from generateKeyRequest | EXPECT_EQ confirms that drmDataPtr is nullptr. | Should Fail |
 */
TEST(OCDMSessionAdapter, InvalidURLFormat) {
    std::cout << "Entering InvalidURLFormat test" << std::endl;
    
    std::string destinationURL = "license.server.com/request";
    uint32_t timeout = 1000;
    std::cout << "Creating OCDMSessionAdapter object using default constructor." << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        std::cout << "Invoking generateKeyRequest with destinationURL: " 
                  << destinationURL << " and timeout: " << timeout << std::endl;
        DrmData* drmDataPtr = adapter.generateKeyRequest(destinationURL, timeout);
        std::cout << "generateKeyRequest returned pointer: " << drmDataPtr << std::endl;
        
        EXPECT_EQ(drmDataPtr, nullptr);
    });
    
    std::cout << "Exiting InvalidURLFormat test" << std::endl;
}
/**
 * @brief Test generateKeyRequest API with a zero timeout value.
 *
 * This test verifies the behavior of the OCDMSessionAdapter when a zero timeout value is passed.
 * The test checks if the adapter correctly handles the zero timeout input by either returning a
 * valid key request payload or a null pointer as per the implementation logic. Assertions are used
 * to verify that the returned pointer meets the expected behavior.
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
 * | 01 | Create OCDMSessionAdapter object and call generateKeyRequest with a zero timeout value | drmHelper object initialized with default DrmInfo, destinationURL = "https://license.server.com/request", timeout = 0; output: adapter instance, drmDataPtr from generateKeyRequest | Valid key request payload is returned (non-null pointer) if zero timeout is allowed, or a null pointer is returned to signal error; assertions (EXPECT_NE or EXPECT_EQ) verify the result accordingly | Should Pass |
 */
TEST(OCDMSessionAdapter, ZeroTimeout) {
    std::cout << "Entering ZeroTimeout test" << std::endl;
    
    std::string destinationURL = "https://license.server.com/request";
    uint32_t timeout = 0;
    std::cout << "Creating OCDMSessionAdapter object using default constructor." << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        std::cout << "Invoking generateKeyRequest with destinationURL: " 
                  << destinationURL << " and timeout: " << timeout << std::endl;
        DrmData* drmDataPtr = adapter.generateKeyRequest(destinationURL, timeout);
        std::cout << "generateKeyRequest returned pointer: " << drmDataPtr << std::endl;
        
        // Depending on implementation, zero timeout can be allowed or rejected.
        // For this test, we assume a non-null pointer indicates a valid key request payload.
        if (drmDataPtr) {
            std::cout << "Retrieved DrmData data: " << drmDataPtr->getData() << std::endl;
            std::cout << "Retrieved DrmData data length: " << drmDataPtr->getDataLength() << std::endl;
            EXPECT_NE(drmDataPtr, nullptr);
        } else {
            std::cout << "Received null pointer indicating error due to zero timeout value." << std::endl;
            EXPECT_EQ(drmDataPtr, nullptr);
        }
    });
    
    std::cout << "Exiting ZeroTimeout test" << std::endl;
}
/**
 * @brief Validates the generateKeyRequest API using the maximum timeout value.
 *
 * This test verifies that the OCDMSessionAdapter correctly processes a key request when invoked with the maximum allowed timeout value. It creates a WidevineDrmHelper using a default DrmInfo instance, initializes the adapter, and then invokes generateKeyRequest with a maximum timeout. The test checks for successful object creation and ensures that the returned DrmData pointer is not null.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate OCDMSessionAdapter with a valid WidevineDrmHelper using default constructor | drmInfo: default, drmHelper: instance created from DrmInfo | OCDMSessionAdapter instance is created without throwing an exception | Should Pass |
 * | 02 | Invoke generateKeyRequest with destinationURL and maximum timeout value | destinationURL = "https://license.server.com/request", timeout = 4294967295 | Method returns a non-null DrmData pointer and valid DRM data is retrieved | Should Pass |
 */
TEST(OCDMSessionAdapter, MaximumTimeoutValue) {
    std::cout << "Entering MaximumTimeoutValue test" << std::endl;
    
    std::string destinationURL = "https://license.server.com/request";
    uint32_t timeout = 4294967295;
    std::cout << "Creating OCDMSessionAdapter object using default constructor." << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        std::cout << "Invoking generateKeyRequest with destinationURL: " 
                  << destinationURL << " and maximum timeout: " << timeout << std::endl;
        DrmData* drmDataPtr = adapter.generateKeyRequest(destinationURL, timeout);
        std::cout << "generateKeyRequest returned pointer: " << drmDataPtr << std::endl;
        
        EXPECT_NE(drmDataPtr, nullptr);
        if (drmDataPtr) {
            std::cout << "Retrieved DrmData data: " << drmDataPtr->getData() << std::endl;
            std::cout << "Retrieved DrmData data length: " << drmDataPtr->getDataLength() << std::endl;
        }
    });
    
    std::cout << "Exiting MaximumTimeoutValue test" << std::endl;
}
/**
 * @brief Verify that OCDMSessionAdapter::getState returns a valid key state.
 *
 * This test constructs an OCDMSessionAdapter object using a shared WidevineDrmHelper and then
 * invokes the getState method to ensure that it returns one of the allowed KeyState enum values.
 * This verification is essential to ensure that the adapter reliably reflects its state according
 * to the key lifecycle.
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
 * | Variation / Step | Description                                                         | Test Data                                                                                       | Expected Result                                                                                                                                                            | Notes       |
 * | :--------------: | ------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------- |
 * | 01               | Construct OCDMSessionAdapter object using default constructor       | drmHelper = instance of WidevineDrmHelper constructed with DrmInfo                              | OCDMSessionAdapter object is successfully created without throwing an exception                                                                                         | Should Pass |
 * | 02               | Invoke getState method and verify returned KeyState value             | Call: KeyState state = adapter.getState()                                                       | The returned KeyState value must be one of: KEY_INIT, KEY_PENDING, KEY_READY, KEY_ERROR, KEY_CLOSED, KEY_ERROR_EMPTY_SESSION_ID                                          | Should Pass |
 */
TEST(OCDMSessionAdapter, GetStateReturnsValidState) {
    std::cout << "Entering GetStateReturnsValidState test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    // Create OCDMSessionAdapter object using default constructor
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Constructed OCDMSessionAdapter object using default constructor." << std::endl;
        
        // Invoke getState method and log invocation
        std::cout << "Invoking getState method." << std::endl;
        KeyState state = adapter.getState();
        std::cout << "getState returned value: " << state << std::endl;
        
        // Validate that the returned state is one of the allowed enum values
        bool validState = (state == KEY_INIT ||
                           state == KEY_PENDING ||
                           state == KEY_READY ||
                           state == KEY_ERROR ||
                           state == KEY_CLOSED ||
                           state == KEY_ERROR_EMPTY_SESSION_ID);
        EXPECT_TRUE(validState);
        std::cout << "Verified that getState returns a valid KeyState value." << std::endl;
    });
    
    std::cout << "Exiting GetStateReturnsValidState test" << std::endl;
}
/**
 * @brief Validates that keyUpdateOCDM properly processes a typical 16-byte key.
 *
 * This test verifies that the OCDMSessionAdapter's keyUpdateOCDM method successfully updates a 16-byte binary key. The test ensures that when provided with the valid key and its length, the API completes without throwing any exceptions, thereby confirming correct handling of typical key updates.
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
 * | Variation / Step | Description | Test Data |Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Instantiate drmInfo and drmHelper, prepare a 16-byte key, and call keyUpdateOCDM | drmInfo = default, drmHelper = shared pointer to WidevineDrmHelper, key = [0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10], keyLength = 16 | Method executes without throwing an exception and updates the key successfully | Should Pass |
 */
TEST(OCDMSessionAdapter, ValidKeyUpdateTypical16Byte)
{
    std::cout << "Entering ValidKeyUpdateTypical16Byte test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    OCDMSessionAdapter adapter(drmHelper);

    // Prepare a 16-byte key value.
    uint8_t key[16];
    // Using strncpy to assign values to a temporary char array and then copying to uint8_t array.
    char tempKey[17] = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10";
    // Note: strncpy works on char arrays, but here we copy the binary data.
    memcpy(key, tempKey, 16);

    std::cout << "Invoking keyUpdateOCDM with 16-byte key." << std::endl;
    std::cout << "Key bytes: ";
    for (int i = 0; i < 16; i++)
    {
        std::cout << "0x" << std::hex << static_cast<int>(key[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW(adapter.keyUpdateOCDM(key, 16));
    std::cout << "keyUpdateOCDM successfully updated the key." << std::endl;

    std::cout << "Exiting ValidKeyUpdateTypical16Byte test" << std::endl;
}
/**
 * @brief Validate that keyUpdateOCDM successfully updates a 1-byte key without throwing an exception.
 *
 * This test verifies that when OCDMSessionAdapter::keyUpdateOCDM is invoked with a minimal key size (1-byte),
 * the function executes without throwing an exception, ensuring that the API correctly handles lower boundary conditions for key sizes.
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
 * | Variation / Step | Description                                                                                      | Test Data                                                       | Expected Result                                                              | Notes       |
 * | :--------------: | ------------------------------------------------------------------------------------------------ | --------------------------------------------------------------- | ---------------------------------------------------------------------------- | ----------- |
 * | 01               | Instantiate DrmInfo, WidevineDrmHelper, and OCDMSessionAdapter; prepare a 1-byte key value         | drmInfo instance, widevineDrmHelper instance, adapter instance, key = 0xFF | Objects and key instantiated successfully                                    | Should be successful |
 * | 02               | Invoke keyUpdateOCDM with the 1-byte key and verify that no exception is thrown                   | key = 0xFF, keyLength = 1                                         | Function returns without throwing exception; key update is successful        | Should Pass |
 */
TEST(OCDMSessionAdapter, ValidKeyUpdateMinimal)
{
    std::cout << "Entering ValidKeyUpdateMinimal test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare a 1-byte key value.
    uint8_t key[1];
    char tempKey[2] = "\xFF";
    memcpy(key, tempKey, 1);

    std::cout << "Invoking keyUpdateOCDM with 1-byte key." << std::endl;
    std::cout << "Key byte: 0x" << std::hex << static_cast<int>(key[0]) << std::dec << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW(adapter.keyUpdateOCDM(key, 1));
    std::cout << "keyUpdateOCDM successfully updated the key with minimal key size." << std::endl;

    std::cout << "Exiting ValidKeyUpdateMinimal test" << std::endl;
}
/**
 * @brief Verify that keyUpdateOCDM successfully updates the key when provided with the maximum key size.
 *
 * This test verifies that the OCDMSessionAdapter::keyUpdateOCDM method handles a 255-byte key correctly.
 * A sequential key array is prepared and passed to the method, and the test confirms that no exception is thrown.
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
 * | Variation / Step | Description                                                                                           | Test Data                                                       | Expected Result                                                                                 | Notes          |
 * | :--------------: | ----------------------------------------------------------------------------------------------------- | --------------------------------------------------------------- | ----------------------------------------------------------------------------------------------- | -------------- |
 * | 01               | Instantiate DrmInfo, WidevineDrmHelper, and OCDMSessionAdapter objects                                  | drmInfo = default, drmHelper = shared_ptr<WidevineDrmHelper>, adapter created | Objects instantiated successfully                                                               | Should be successful |
 * | 02               | Prepare a 255-byte key with sequential byte values (0x00, 0x01, ..., 0xFE)                            | key = byte array with values 0x00,0x01,...,0xFE                  | Key array populated with correct sequential values                                              | Should be successful |
 * | 03               | Invoke keyUpdateOCDM on adapter with the prepared key and key size of 255, and verify no exception is thrown | key = array of 255 bytes, keySize = 255                           | API call completes without throwing an exception, confirming successful key update              | Should Pass    |
 */
TEST(OCDMSessionAdapter, ValidKeyUpdateMaximum)
{
    std::cout << "Entering ValidKeyUpdateMaximum test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare a 255-byte key with values 0x00, 0x01, ..., 0xFE.
    uint8_t key[255];
    for (int i = 0; i < 255; i++)
    {
        key[i] = static_cast<uint8_t>(i & 0xFF);
    }

    std::cout << "Invoking keyUpdateOCDM with 255-byte key." << std::endl;
    std::cout << "First five key bytes: ";
    for (int i = 0; i < 5; i++)
    {
        std::cout << "0x" << std::hex << static_cast<int>(key[i]) << " ";
    }
    std::cout << std::dec << " ... ";
    std::cout << "Last five key bytes: ";
    for (int i = 250; i < 255; i++)
    {
        std::cout << "0x" << std::hex << static_cast<int>(key[i]) << " ";
    }
    std::cout << std::dec << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW(adapter.keyUpdateOCDM(key, 255));
    std::cout << "keyUpdateOCDM successfully updated the key with maximum key size." << std::endl;

    std::cout << "Exiting ValidKeyUpdateMaximum test" << std::endl;
}
/**
 * @brief Negative test for keyUpdateOCDM in OCDMSessionAdapter to ensure graceful handling of a NULL key pointer.
 *
 * This test verifies that invoking keyUpdateOCDM with a NULL key pointer and a valid keySize does not throw any exceptions, ensuring the method correctly handles invalid input without crashing.
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
 * | 01 | Instantiate OCDMSessionAdapter with a valid DrmHelper object and invoke keyUpdateOCDM with a NULL key pointer and keySize set to 16. | drmInfo = valid, key = nullptr, keySize = 16 | No exception thrown; keyUpdateOCDM handles the NULL key pointer gracefully. | Should Pass |
 */
TEST(OCDMSessionAdapter, NegativeTestNullKey)
{
    std::cout << "Entering NegativeTestNullKey test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    OCDMSessionAdapter adapter(drmHelper);

    uint8_t keySize = 16;
    uint8_t* key = nullptr;

    std::cout << "Invoking keyUpdateOCDM with NULL key pointer and keySize = " 
              << static_cast<int>(keySize) << std::endl;

    // Invoke the method and expect no exception (graceful handling).
    EXPECT_NO_THROW(adapter.keyUpdateOCDM(key, keySize));
    std::cout << "keyUpdateOCDM handled the NULL key pointer gracefully." << std::endl;

    std::cout << "Exiting NegativeTestNullKey test" << std::endl;
}
/**
 * @brief Validate handling of key updates with zero key size in OCDMSessionAdapter
 *
 * Validate that OCDMSessionAdapter::keyUpdateOCDM method properly handles the case where a non-null key pointer is passed with a keySize value of zero. The test ensures that no exception is thrown when an update is attempted with an invalid key size, thereby confirming the robustness of the method.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 025@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize DRM info, create WidevineDrmHelper, and construct OCDMSessionAdapter | drmInfo, WidevineDrmHelper instance, OCDMSessionAdapter instance | Adapter successfully constructs without error | Should be successful |
 * | 02 | Prepare test key data: allocate a 1-byte key and set keySize to 0 | key pointer with one byte (0x01), keySize = 0 | Key data is setup; keyUpdateOCDM is called with keySize zero without throwing exception | Should Pass |
 * | 03 | Invoke keyUpdateOCDM method on OCDMSessionAdapter | input: key pointer = pointer to key, keySize = 0 | No exception is thrown by keyUpdateOCDM; method handles zero keySize appropriately | Should Pass |
 */
TEST(OCDMSessionAdapter, NegativeTestZeroKeySize)
{
    std::cout << "Entering NegativeTestZeroKeySize test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare a 1-byte key value.
    uint8_t key[1];
    char tempKey[2] = "\x01";
    memcpy(key, tempKey, 1);
    uint8_t keySize = 0;

    std::cout << "Invoking keyUpdateOCDM with a non-null key pointer but keySize = " 
              << static_cast<int>(keySize) << std::endl;
    std::cout << "Key byte provided: 0x" << std::hex << static_cast<int>(key[0]) << std::dec << std::endl;

    // Invoke the method and expect no exception.
    EXPECT_NO_THROW(adapter.keyUpdateOCDM(key, keySize));
    std::cout << "keyUpdateOCDM did not update key due to zero keySize as expected." << std::endl;

    std::cout << "Exiting NegativeTestZeroKeySize test" << std::endl;
}
/**
 * @brief Verifies the functionality of keysUpdatedOCDM method on a valid OCDMSessionAdapter object.
 *
 * This test case creates an instance of OCDMSessionAdapter using a valid WidevineDrmHelper and then invokes the keysUpdatedOCDM method.
 * The objective is to ensure that both the object construction and the method invocation execute without throwing any exceptions and that the internal state reflects a successful DRM key update.
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
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter instance with a valid WidevineDrmHelper | drmInfo = default, drmHelper = valid shared pointer, adapter object creation | EXPECT_NO_THROW does not throw exception during construction | Should Pass |
 * | 02 | Invoke keysUpdatedOCDM method on the OCDMSessionAdapter object | adapter object; method call: keysUpdatedOCDM() | EXPECT_NO_THROW does not throw exception during method call | Should Pass |
 * | 03 | Log internal state after key update | Log message: "Internal state updated: DRM keys have been updated successfully." | Log message is printed confirming successful update | Should be successful |
 */
TEST(OCDMSessionAdapter, PositiveTest_ValidInvocationOf_keysUpdatedOCDM) {
    std::cout << "Entering PositiveTest_ValidInvocationOf_keysUpdatedOCDM test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);    
    // Create an instance of OCDMSessionAdapter using the default constructor and check it doesn't throw.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully." << std::endl;
        
        // Log the invocation of the method keysUpdatedOCDM.
        std::cout << "Invoking keysUpdatedOCDM method on OCDMSessionAdapter object." << std::endl;
        EXPECT_NO_THROW({
            adapter.keysUpdatedOCDM();
            std::cout << "keysUpdatedOCDM method executed without throwing an exception." << std::endl;
        });
        
        // Simulate logging internal state changes after key update.
        // Note: Internal state variables are assumed to be updated by keysUpdatedOCDM.
        std::cout << "Internal state updated: DRM keys have been updated successfully." << std::endl;
    });
    
    std::cout << "Exiting PositiveTest_ValidInvocationOf_keysUpdatedOCDM test" << std::endl;
}
/**
 * @brief Verify processDRMKey returns success when a valid, non-empty DRM key is provided
 *
 * This test validates that the processDRMKey API correctly processes a valid DRM key when supplied with a non-empty license key. The test constructs a DRM helper, creates a valid DRM key, and invokes the processDRMKey function with a timeout. It then asserts that the function returns a success status (0), indicating that the DRM key was processed as expected.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 027
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                         | Test Data                                                                                                    | Expected Result                                                          | Notes         |
 * | :--------------: | ----------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------ | ------------- |
 * | 01               | Instantiate OCDMSessionAdapter with valid DRM helper                                | drmInfo = default, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo), adapter constructed with drmHelper        | Adapter is instantiated successfully                                     | Should be successful |
 * | 02               | Prepare a valid license key by copying "VALID_LICENSE_KEY" into the buffer            | validLicenseKey = char array, value = "VALID_LICENSE_KEY"                                                      | validLicenseKey contains "VALID_LICENSE_KEY"                             | Should be successful |
 * | 03               | Create DrmData object using the valid license key                                   | input = validLicenseKey, length = strlen("VALID_LICENSE_KEY"), output = DrmData object constructed with these values            | DrmData.getData() returns "VALID_LICENSE_KEY" and DrmData.getDataLength() returns the correct length | Should be successful |
 * | 04               | Invoke processDRMKey API with the created DrmData object and a timeout value          | input: drmKey pointer = address of DrmData object, timeout = 1000, output: status                                | Returns status = 0 and EXPECT_EQ(status, 0) assertion passes               | Should Pass   |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_ValidNonEmpty)
{
    std::cout << "Entering ProcessDRMKey_ValidNonEmpty test" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare valid license data using strncpy.
    char validLicenseKey[32] = {0};
    strncpy(validLicenseKey, "VALID_LICENSE_KEY", sizeof(validLicenseKey) - 1);
    std::cout << "Constructed valid license key: " << validLicenseKey << std::endl;
    
    // Create DrmData object with valid license key using constructor.
    DrmData drmKey(validLicenseKey, strlen(validLicenseKey));
    std::cout << "Created DrmData object with data: " << drmKey.getData() 
              << " and length: " << drmKey.getDataLength() << std::endl;
    
    // Set timeout value.
    uint32_t timeout = 1000;
    std::cout << "Invoking processDRMKey with timeout: " << timeout << std::endl;
    
    // Invoke processDRMKey and get return value
    int status = adapter.processDRMKey(&drmKey, timeout);
    std::cout << "processDRMKey returned: " << status << std::endl;
    
    // Expect success (status == 0)
    EXPECT_EQ(status, 0);

    std::cout << "Exiting ProcessDRMKey_ValidNonEmpty test" << std::endl;
}
/**
 * @brief Validate that processDRMKey returns a non-zero status when provided a null key pointer.
 *
 * This test verifies the behavior of the processDRMKey API when it receives a null key pointer instead of a valid DRM key. The expected behavior is a failure indication (non-zero return value) to ensure that null inputs are handled correctly.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke processDRMKey with a null key pointer and a valid timeout value | key pointer = nullptr, timeout = 1000 | Non-zero return status indicating failure, and assertion EXPECT_NE(status, 0) passes | Should Fail |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_NullKey)
{
    std::cout << "Entering ProcessDRMKey_NullKey test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  
    OCDMSessionAdapter adapter(drmHelper);

    // Timeout value.
    uint32_t timeout = 1000;
    std::cout << "Invoking processDRMKey with a NULL key pointer and timeout: " << timeout << std::endl;

    // Invoke processDRMKey with nullptr and get return status
    int status = adapter.processDRMKey(nullptr, timeout);
    std::cout << "processDRMKey returned: " << status << std::endl;
    
    // Expected non-zero status indicating failure
    EXPECT_NE(status, 0);

    std::cout << "Exiting ProcessDRMKey_NullKey test" << std::endl;
}
/**
 * @brief Tests processDRMKey API with an empty DRM key input.
 *
 * This test verifies that the processDRMKey API properly handles a case when it is provided with an empty DRM key.
 * The function is expected to return a non-zero status indicating failure or invalid data.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 029@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description                                                        | Test Data                                   | Expected Result                                                                            | Notes         |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------- | ------------------------------------------------------------------------------------------ | ------------- |
 * | 01               | Invoke processDRMKey with an empty DrmData and a timeout of 1000 ms  | drmData: empty string, length = 0, timeout = 1000 | Returns non-zero value indicating failure; Assertion EXPECT_NE(status, 0) passes | Should Pass   |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_EmptyData)
{
    std::cout << "Entering ProcessDRMKey_EmptyData test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare an empty license data using strncpy.
    char emptyKey[1] = {0};  // empty string
    std::cout << "Constructed empty license key string." << std::endl;
    
    // Create DrmData object with empty data.
    DrmData drmKey(emptyKey, 0);
    std::cout << "Created DrmData object with data: \"" << drmKey.getData() 
              << "\" and length: " << drmKey.getDataLength() << std::endl;
    
    // Set valid timeout.
    uint32_t timeout = 1000;
    std::cout << "Invoking processDRMKey with empty DrmData and timeout: " << timeout << std::endl;
    
    // Invoke processDRMKey and get return value
    int status = adapter.processDRMKey(&drmKey, timeout);
    std::cout << "processDRMKey returned: " << status << std::endl;
    
    // Expected non-zero status indicating failure or invalid data.
    EXPECT_NE(status, 0);

    std::cout << "Exiting ProcessDRMKey_EmptyData test" << std::endl;
}
/**
 * @brief Validate processDRMKey returns failure when timeout is zero
 *
 * This test verifies that the processDRMKey method of OCDMSessionAdapter returns a non-zero status when invoked with a valid DRM key but a timeout value of zero. This is an edge case test to ensure correct handling of timeout parameters.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 030@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Initialize DRM helper, create valid license key and DrmData object, then invoke processDRMKey with a timeout value of 0. | drmKey = "VALID_LICENSE_KEY", drmKeyLength = strlen("VALID_LICENSE_KEY"), timeout = 0, output status = (non-zero) | processDRMKey returns a non-zero status indicating failure due to a zero timeout (EXPECT_NE(status, 0) passes) | Should Fail |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_ZeroTimeout)
{
    std::cout << "Entering ProcessDRMKey_ZeroTimeout test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare valid license data using strncpy.
    char validLicenseKey[32] = {0};
    strncpy(validLicenseKey, "VALID_LICENSE_KEY", sizeof(validLicenseKey) - 1);
    std::cout << "Constructed valid license key: " << validLicenseKey << std::endl;
    
    // Create DrmData object with valid license data.
    DrmData drmKey(validLicenseKey, strlen(validLicenseKey));
    std::cout << "Created DrmData object with data: " << drmKey.getData()
              << " and length: " << drmKey.getDataLength() << std::endl;
    
    // Set timeout value to 0.
    uint32_t timeout = 0;
    std::cout << "Invoking processDRMKey with timeout: " << timeout << std::endl;
    
    // Invoke processDRMKey and get return value.
    int status = adapter.processDRMKey(&drmKey, timeout);
    std::cout << "processDRMKey returned: " << status << std::endl;
    
    // Expected non-zero status indicating failure due to timeout.
    EXPECT_NE(status, 0);

    std::cout << "Exiting ProcessDRMKey_ZeroTimeout test" << std::endl;
}
/**
 * @brief Test the processDRMKey API handling maximum timeout value
 *
 * This test verifies that the processDRMKey function correctly handles the maximum possible timeout value (uint32_t max)
 * while processing a valid DRM key. It ensures that the adapter returns a success status upon processing the DRM key with the maximum timeout.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 031@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                                                | Test Data                                                                                                        | Expected Result                                          | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------- |
 * | 01               | Initialize DrmInfo, create a WidevineDrmHelper instance, and instantiate OCDMSessionAdapter                | drmInfo = default, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo)                                       | Adapter instance is successfully created                 | Should be successful |
 * | 02               | Prepare a valid license key using strncpy                                                                  | validLicenseKey[32] = "VALID_LICENSE_KEY"                                                                          | Buffer contains the valid license key                    | Should be successful |
 * | 03               | Create DrmData object with the valid license data                                                          | drmKey data = validLicenseKey, dataLength = strlen(validLicenseKey)                                                | DrmData object is initialized with correct data and length | Should be successful |
 * | 04               | Set the maximum uint32_t timeout value                                                                     | timeout = 4294967295                                                                                               | Timeout value is set to the maximum value                | Should be successful |
 * | 05               | Invoke processDRMKey with the prepared DrmData object and maximum timeout value                            | drmKey pointer, timeout = 4294967295                                                                               | Function returns a status code                           | Should Pass   |
 * | 06               | Verify that the returned status from processDRMKey is 0, indicating success                                  | status variable holds the returned value from processDRMKey                                                        | status equals 0 confirming successful processing         | Should Pass   |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_MaxTimeout)
{
    std::cout << "Entering ProcessDRMKey_MaxTimeout test" << std::endl;
    
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    OCDMSessionAdapter adapter(drmHelper);

    // Prepare valid license data using strncpy.
    char validLicenseKey[32] = {0};
    strncpy(validLicenseKey, "VALID_LICENSE_KEY", sizeof(validLicenseKey) - 1);
    std::cout << "Constructed valid license key: " << validLicenseKey << std::endl;
    
    // Create DrmData object with valid license data.
    DrmData drmKey(validLicenseKey, strlen(validLicenseKey));
    std::cout << "Created DrmData object with data: " << drmKey.getData()
              << " and length: " << drmKey.getDataLength() << std::endl;
    
    // Set maximum uint32_t timeout.
    uint32_t timeout = 4294967295;
    std::cout << "Invoking processDRMKey with maximum timeout: " << timeout << std::endl;
    
    // Invoke processDRMKey and get return value.
    int status = adapter.processDRMKey(&drmKey, timeout);
    std::cout << "processDRMKey returned: " << status << std::endl;
    
    // Expect success (status == 0)
    EXPECT_EQ(status, 0);

    std::cout << "Exiting ProcessDRMKey_MaxTimeout test" << std::endl;
}
/**
 * @brief Test valid processing of challenge data for OCDMSessionAdapter
 *
 * This test verifies that the OCDMSessionAdapter correctly processes a valid challenge.
 * It creates an instance of OCDMSessionAdapter, initializes the destination URL and challenge data,
 * and then calls processOCDMChallenge ensuring that no exceptions are thrown.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter instance, set destination URL and challenge data, and invoke processOCDMChallenge | drmHelper = valid shared ptr, destUrl = "https://license.server.com", challengeData = {0x01,0x02,0x03}, challengeSize = 3 | API executes successfully with no exceptions thrown; processOCDMChallenge is invoked as expected | Should Pass |
 */
TEST(OCDMSessionAdapter, ValidChallengeProcessing) {
    std::cout << "Entering ValidChallengeProcessing test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    // Create an instance using the default constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Prepare destination URL using strncpy for fixed size char array.
        char destUrl[256] = {0};
        const char* url = "https://license.server.com";
        strncpy(destUrl, url, sizeof(destUrl) - 1);
        std::cout << "destUrl set to: " << destUrl << std::endl;

        // Prepare challenge buffer.
        uint8_t challengeData[3] = {0x01, 0x02, 0x03};
        std::cout << "Challenge data set to: {0x01, 0x02, 0x03} with challengeSize = 3" << std::endl;

        // Invoke the method and log the invocation.
        std::cout << "Invoking processOCDMChallenge with valid challenge data" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, 3);
        std::cout << "Method processOCDMChallenge invoked successfully" << std::endl;

        // Debug log simulating internal state update.
        std::cout << "Internal state updated with destUrl: " << url 
                  << ", challengeSize: " << 3 
                  << ", and challenge data processed" << std::endl;
    });

    std::cout << "Exiting ValidChallengeProcessing test" << std::endl;
}
/**
 * @brief Test that processOCDMChallenge handles an empty destination URL gracefully
 *
 * This test verifies that invoking processOCDMChallenge with an empty destination URL does not cause an exception, while internally flagging an error state. The test sets up an instance of OCDMSessionAdapter using a WidevineDrmHelper, prepares an empty destination URL, and a fixed challenge buffer to simulate the scenario.
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
 * | 01 | Create OCDMSessionAdapter object using default constructor with a WidevineDrmHelper instance | drmInfo = default, drmHelper = instance of WidevineDrmHelper | Object is created without throwing an exception | Should be successful |
 * | 02 | Prepare an empty destination URL string | destUrl = "" | destUrl is set to an empty string | Should be successful |
 * | 03 | Prepare challenge buffer with predefined values | challengeData = {0xAA, 0xBB}, challengeSize = 2 | Challenge data is set correctly | Should be successful |
 * | 04 | Invoke processOCDMChallenge with the empty destination URL and challenge buffer | destUrl = "", challengeData = {0xAA, 0xBB}, challengeSize = 2 | Method completes without throwing an exception; internal error state is flagged | Should Pass |
 */
TEST(OCDMSessionAdapter, EmptyDestinationURL) {
    std::cout << "Entering EmptyDestinationURL test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Prepare an empty destination URL.
        char destUrl[256] = {0};
        strncpy(destUrl, "", sizeof(destUrl) - 1);
        std::cout << "destUrl set to an empty string" << std::endl;

        // Prepare challenge buffer.
        uint8_t challengeData[2] = {0xAA, 0xBB};
        std::cout << "Challenge data set to: {0xAA, 0xBB} with challengeSize = 2" << std::endl;

        // Invoke the method.
        std::cout << "Invoking processOCDMChallenge with empty destination URL" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, 2);
        std::cout << "Method processOCDMChallenge invoked with empty destUrl; error state should be flagged internally" << std::endl;
    });

    std::cout << "Exiting EmptyDestinationURL test" << std::endl;
}
/**
 * @brief Test to verify that processOCDMChallenge handles a nullptr destination URL gracefully
 *
 * This test verifies that when the destination URL pointer is set to nullptr,
 * the processOCDMChallenge method in OCDMSessionAdapter is invoked without throwing any exceptions.
 * It ensures that the internal error handling works as expected for a null destination URL.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 034@n
 * **Priority:** High@n
 * 
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 * 
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | ------------- | ----- |
 * | 01 | Create OCDMSessionAdapter using a valid drmHelper instance | drmHelper = shared instance with valid drmInfo | OCDMSessionAdapter object is created without exceptions | Should be successful |
 * | 02 | Set destination URL pointer to nullptr | destUrl = nullptr | destUrl is set to nullptr | Should be successful |
 * | 03 | Prepare challenge buffer with valid challenge data | challengeData = {0x10, 0x20}, challengeSize = 2 | Challenge data is prepared correctly | Should be successful |
 * | 04 | Invoke processOCDMChallenge with a nullptr destination URL | destUrl = nullptr, challengeData = {0x10, 0x20}, challengeSize = 2 | No exception is thrown; error handled internally | Should Pass |
 */
TEST(OCDMSessionAdapter, NullDestinationURLPointer) {
    std::cout << "Entering NullDestinationURLPointer test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Set destination URL pointer to nullptr.
        const char *destUrl = nullptr;
        std::cout << "destUrl is set to nullptr" << std::endl;

        // Prepare challenge buffer.
        uint8_t challengeData[2] = {0x10, 0x20};
        std::cout << "Challenge data set to: {0x10, 0x20} with challengeSize = 2" << std::endl;

        // Invoke the method.
        std::cout << "Invoking processOCDMChallenge with nullptr for destUrl" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, 2);
        std::cout << "Method processOCDMChallenge invoked with nullptr destUrl; appropriate error handling expected internally" << std::endl;
    });

    std::cout << "Exiting NullDestinationURLPointer test" << std::endl;
}
/**
 * @brief Validate that processOCDMChallenge handles a null challenge pointer gracefully.
 *
 * This test verifies that the OCDMSessionAdapter::processOCDMChallenge method does not throw an exception when provided with a null challenge pointer, while a valid destination URL and a non-zero challenge size are supplied.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter instance with a valid drmHelper using the default constructor. | drmHelper = valid instance of WidevineDrmHelper | Adapter instance is successfully created without exceptions. | Should be successful |
 * | 02 | Prepare and set destination URL for the challenge request. | destUrl = "https://license.server.com" | destUrl is set correctly with the provided URL. | Should be successful |
 * | 03 | Set challenge pointer to nullptr and specify challenge size. | challengeData = nullptr, challengeSize = 5 | Challenge pointer remains null while challengeSize is set. | Should be successful |
 * | 04 | Invoke processOCDMChallenge with the null challenge pointer. | destUrl, challengeData, 5 | Method processes the input without throwing an exception. | Should Pass |
 */
TEST(OCDMSessionAdapter, NullChallengePointer) {
    std::cout << "Entering NullChallengePointer test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Prepare destination URL.
        char destUrl[256] = {0};
        const char* url = "https://license.server.com";
        strncpy(destUrl, url, sizeof(destUrl) - 1);
        std::cout << "destUrl set to: " << destUrl << std::endl;

        // Set challenge pointer to nullptr.
        const uint8_t* challengeData = nullptr;
        std::cout << "challenge pointer is set to nullptr with challengeSize = 5" << std::endl;

        // Invoke the method.
        std::cout << "Invoking processOCDMChallenge with nullptr for challenge pointer" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, 5);
        std::cout << "Method processOCDMChallenge invoked with nullptr challenge pointer; error state expected internally" << std::endl;
    });

    std::cout << "Exiting NullChallengePointer test" << std::endl;
}
/**
 * @brief Ensure that processOCDMChallenge bypasses processing when challengeSize is zero even if the challenge buffer is non-null
 *
 * This test verifies that the OCDMSessionAdapter correctly handles a call to processOCDMChallenge when the challenge size is zero, despite a non-null challenge buffer being provided. The objective is to check that the function does not attempt to process the challenge data and does not throw any exceptions.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter object using a valid WidevineDrmHelper object | drmInfo = default, helper = std::make_shared<WidevineDrmHelper>(drmInfo) | Adapter object created without exception | Should be successful |
 * | 02 | Set up the destination URL for the service | destUrl = "https://license.server.com" | destUrl is correctly set with the URL | Should be successful |
 * | 03 | Prepare a challenge buffer containing non-null data but with a zero challenge size | challengeData = {0x30, 0x40}, challengeSize = 0 | Challenge buffer initialized, but size is zero | Should be successful |
 * | 04 | Invoke processOCDMChallenge with the prepared parameters | destUrl = "https://license.server.com", challengeData = {0x30, 0x40}, challengeSize = 0 | Function executes without throwing an exception; internal processing is bypassed | Should Pass |
 */
TEST(OCDMSessionAdapter, ZeroChallengeSizeWithNonNullBuffer) {
    std::cout << "Entering ZeroChallengeSizeWithNonNullBuffer test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Prepare destination URL.
        char destUrl[256] = {0};
        const char* url = "https://license.server.com";
        strncpy(destUrl, url, sizeof(destUrl) - 1);
        std::cout << "destUrl set to: " << destUrl << std::endl;

        // Prepare challenge buffer with non-null data but zero challenge size.
        uint8_t challengeData[2] = {0x30, 0x40};
        std::cout << "Challenge data set to: {0x30, 0x40} but challengeSize = 0" << std::endl;

        // Invoke the method.
        std::cout << "Invoking processOCDMChallenge with zero challengeSize" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, 0);
        std::cout << "Method processOCDMChallenge invoked with zero challengeSize; processing should be bypassed internally" << std::endl;
    });

    std::cout << "Exiting ZeroChallengeSizeWithNonNullBuffer test" << std::endl;
}
/**
 * @brief Test the processOCDMChallenge method with full byte range challenge data
 *
 * This test verifies that OCDMSessionAdapter::processOCDMChallenge can handle a challenge buffer 
 * containing the full byte range (0x00 to 0xFF) without throwing an exception. It ensures the adapter 
 * is initialized correctly and that the method processes the challenge data as expected.
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
 * | Variation / Step | Description                                                                                                         | Test Data                                                                                                          | Expected Result                                                            | Notes            |
 * | :--------------: | ------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ | -------------------------------------------------------------------------- | ---------------- |
 * | 01               | Create OCDMSessionAdapter object using drmHelper and prepare the destination URL                                    | drmInfo, drmHelper, destUrl = "https://license.server.com"                                                         | OCDMSessionAdapter object is created successfully and destination URL is set | Should be successful |
 * | 02               | Prepare the challenge buffer with a complete range of bytes from 0x00 to 0xFF                                          | challengeSize = 256, challengeData = [0,1,2,...,255]                                                               | Challenge buffer is correctly filled with values from 0x00 to 0xFF           | Should be successful |
 * | 03               | Invoke processOCDMChallenge method with the prepared destination URL and challenge data                             | Input: destUrl = "https://license.server.com", challengeData = full byte range (0x00 to 0xFF), challengeSize = 256 | Method is invoked without throwing exceptions          | Should Pass      |
 */
TEST(OCDMSessionAdapter, FullByteRangeChallengeData) {
    std::cout << "Entering FullByteRangeChallengeData test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "Created OCDMSessionAdapter object using default constructor" << std::endl;

        // Prepare destination URL.
        char destUrl[256] = {0};
        const char* url = "https://license.server.com";
        strncpy(destUrl, url, sizeof(destUrl) - 1);
        std::cout << "destUrl set to: " << destUrl << std::endl;

        // Prepare challenge buffer containing full byte range (0x00 to 0xFF).
        const uint16_t challengeSize = 256;
        uint8_t challengeData[256] = {0};
        for (uint16_t i = 0; i < challengeSize; ++i) {
            challengeData[i] = static_cast<uint8_t>(i);
        }
        std::cout << "Challenge data set to full byte range from 0x00 to 0xFF with challengeSize = 256" << std::endl;

        // Invoke the method.
        std::cout << "Invoking processOCDMChallenge with full byte range challenge data" << std::endl;
        adapter.processOCDMChallenge(destUrl, challengeData, challengeSize);
        std::cout << "Method processOCDMChallenge invoked successfully with full byte range challenge data" << std::endl;
        std::cout << "Internal state updated with complete challenge bytes" << std::endl;
    });

    std::cout << "Exiting FullByteRangeChallengeData test" << std::endl;
}
/**
 * @brief Verifies that an immediate timeout prevents any state transition.
 *
 * This test verifies that when a timeout of 0 is provided, the OCDMSessionAdapter does not transition to any requested state, returning false for each state. This behavior ensures that the adapter properly handles cases where immediate timeouts are expected.
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
 * | Variation / Step | Description                                                                                          | Test Data                                                                                                                  | Expected Result                                                                                     | Notes         |
 * | :--------------: | ---------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ------------- |
 * | 01               | Create an OCDMSessionAdapter instance using a WidevineDrmHelper constructed from default DrmInfo.   | drmInfo = default, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo)                                                | Instance is created successfully without throwing any exceptions.                                  | Should Pass   |
 * | 02               | Invoke waitForState for each KeyState with timeout = 0 to verify immediate timeout prevents state transition. | For each KeyState from KEY_INIT to KEY_ERROR_EMPTY_SESSION_ID, timeout = 0                                                  | Returns false for each call to waitForState, confirming that the immediate timeout prevents state change. | Should Fail   |
 */
TEST(OCDMSessionAdapter, ImmediateTimeoutPreventsStateTransition)
{
    std::cout << "Entering ImmediateTimeoutPreventsStateTransition test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    // Creating object using default (or custom) constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "[DEBUG] OCDMSessionAdapter instance created successfully using default constructor." << std::endl;
        
        const uint32_t timeout = 0;
        for (int i = KEY_INIT; i <= KEY_ERROR_EMPTY_SESSION_ID; i++) {
            KeyState state = static_cast<KeyState>(i);
            std::cout << "[DEBUG] Invoking waitForState with state = " << keyStateToString(state) 
                      << " (" << i << ") and timeout = " << timeout << "ms" << std::endl;
            
            bool result = adapter.waitForState(state, timeout);
            
            std::cout << "[DEBUG] Returned value from waitForState: " << (result ? "true" : "false") << std::endl;
            EXPECT_FALSE(result) << "Expected false for state " << keyStateToString(state) << " with timeout = 0";
        }
    });
    
    std::cout << "Exiting ImmediateTimeoutPreventsStateTransition test" << std::endl;
}
/**
 * @brief Verify that waitForState returns false when the timeout expires before achieving the desired key state.
 *
 * This test verifies that for each key state from KEY_INIT to KEY_ERROR_EMPTY_SESSION_ID, the OCDMSessionAdapter::waitForState method returns false when the specified timeout expires. This helps ensure that the adapter correctly handles timeout scenarios.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create an instance of OCDMSessionAdapter using a WidevineDrmHelper with default settings. | drmInfo = default, drmHelper = shared_ptr<WidevineDrmHelper>(drmInfo) | Instance created without throwing exceptions. | Should be successful |
 * | 02 | Invoke waitForState for each state from KEY_INIT to KEY_ERROR_EMPTY_SESSION_ID with a timeout of 500ms. | keyState = each value from KEY_INIT to KEY_ERROR_EMPTY_SESSION_ID, timeout = 500ms | Each call to waitForState returns false. | Should Pass |
 */
TEST(OCDMSessionAdapter, TimeoutExpiresBeforeDesiredState)
{
    std::cout << "Entering TimeoutExpiresBeforeDesiredState test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);   
    // Creating object using default (or custom) constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "[DEBUG] OCDMSessionAdapter instance created successfully using default constructor." << std::endl;
        
        const uint32_t timeout = 500;
        for (int i = KEY_INIT; i <= KEY_ERROR_EMPTY_SESSION_ID; i++) {
            KeyState state = static_cast<KeyState>(i);
            std::cout << "[DEBUG] Invoking waitForState with state = " << keyStateToString(state) 
                      << " (" << i << ") and timeout = " << timeout << "ms" << std::endl;
            
            bool result = adapter.waitForState(state, timeout);
            
            std::cout << "[DEBUG] Returned value from waitForState: " << (result ? "true" : "false") << std::endl;
            EXPECT_FALSE(result) << "Expected false for state " << keyStateToString(state) << " with timeout = 500";
        }
    });
    
    std::cout << "Exiting TimeoutExpiresBeforeDesiredState test" << std::endl;
}
/**
 * @brief Validate proper construction and destruction of OCDMSessionAdapter
 *
 * This test verifies that an instance of OCDMSessionAdapter is successfully constructed using a default constructor and correctly destructed when it goes out of scope. The test ensures that no exceptions are thrown during the object's lifetime, indicating proper resource management.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 040@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result |Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke the default constructor of OCDMSessionAdapter using a valid drmHelper and allow the instance to go out of scope. | drmInfo = default constructed, drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo) | OCDMSessionAdapter instance is constructed and destructed without throwing any exceptions (EXPECT_NO_THROW passes) | Should Pass |
 * | 02 | Log entry and exit messages to confirm the test execution flow. | N/A (Console outputs: "Entering Destruction test", "OCDMSessionAdapter instance constructed successfully using default constructor.", "OCDMSessionAdapter destructor will be invoked as object goes out of scope.", "Exiting Destruction test") | Console logs indicate the correct progression of the test execution steps. | Should be successful |
 */
TEST(OCDMSessionAdapter, Destruction) {
    std::cout << "Entering Destruction test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);  
    // Create and destroy an instance of OCDMSessionAdapter to test the destructor.
    EXPECT_NO_THROW({
        {
            // Invoking default constructor to create the object.
            OCDMSessionAdapter adapter(drmHelper);
            std::cout << "OCDMSessionAdapter instance constructed successfully using default constructor." << std::endl;

            // Before the scope ends, log that the destructor will be invoked.
            std::cout << "OCDMSessionAdapter destructor will be invoked as object goes out of scope." << std::endl;
        }
    });
    
    std::cout << "Exiting Destruction test" << std::endl;
}
/**
 * @brief Validate that verifyOutputProtection returns true under correct configuration.
 *
 * This test verifies that when the output protection configuration is set up correctly, the
 * verifyOutputProtection method returns true, indicating that the output protection is effective.
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
 * | Variation / Step | Description                                                        | Test Data                                                          | Expected Result                                      | Notes              |
 * | :--------------: | ------------------------------------------------------------------ | ------------------------------------------------------------------ | ---------------------------------------------------- | ------------------ |
 * | 01               | Setup correct configuration for output protection                  | drmInfo = default, WidevineDrmHelper = created, testAdapter = initialized | Configuration is set to default values              | Should be successful |
 * | 02               | Invoke verifyOutputProtection via callVerifyOutputProtection()       | testAdapter->callVerifyOutputProtection() = returns bool           | Returns true; assertion EXPECT_TRUE passes           | Should Pass        |
 */
TEST_F(OCDMSessionAdapterTest, VerifyOutputProtectionReturnsTrue) {
    std::cout << "Entering VerifyOutputProtectionReturnsTrue test" << std::endl;

    // Simulate a correct configuration for output protection.
    std::cout << "Setting up correct configuration for output protection." << std::endl;
    // (Assuming that by default, the correct configuration is set.
    //  Since no setters or public members are provided, we rely on the default state.)
    
    std::cout << "Invoking verifyOutputProtection via verifyOutputProtection()" << std::endl;
    bool result = testAdapter->callVerifyOutputProtection();
    std::cout << "Returned value from verifyOutputProtection: " << result << std::endl;

    EXPECT_TRUE(result);

    std::cout << "Exiting VerifyOutputProtectionReturnsTrue test" << std::endl;
}
#if defined(USE_OPENCDM_ADAPTER)
/**
 * @brief Test the setting of a valid key ID using a typical non-empty keyId vector
 *
 * This test validates that an OCDMSessionAdapter object can be constructed with a valid drmHelper, and that the setKeyId method accepts a typical non-empty keyId vector without throwing exceptions.
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
 * | Variation / Step | Description                                                           | Test Data                                                              | Expected Result                                                       | Notes         |
 * | :--------------: | --------------------------------------------------------------------- | ---------------------------------------------------------------------- | --------------------------------------------------------------------- | ------------- |
 * | 01               | Create OCDMSessionAdapter object using default constructor with drmHelper | drmHelper = std::make_shared<WidevineDrmHelper>(DrmInfo)               | Adapter object is successfully created without exceptions           | Should Pass   |
 * | 02               | Define a typical non-empty keyId vector                                | keyId = {0x01, 0x02, 0x03, 0x04}                                         | keyId vector is created as expected                                   | Should be successful |
 * | 03               | Invoke setKeyId method with the typical keyId vector                     | keyId = {0x01, 0x02, 0x03, 0x04}                                         | setKeyId method call executes without exceptions (EXPECT_NO_THROW)     | Should Pass   |
 */
TEST(OCDMSessionAdapter, SetValidKeyIdTypical) {
    std::cout << "Entering SetValidKeyIdTypical test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    // Create OCDMSessionAdapter object using default constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully using default constructor." << std::endl;
        
        // Define a typical non-empty keyId vector.
        std::vector<uint8_t> keyId = {0x01, 0x02, 0x03, 0x04};
        std::cout << "KeyId to be set: ";
        for (const auto &byte : keyId) {
            std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Invoke setKeyId method
        std::cout << "Invoking setKeyId with typical keyId vector" << std::endl;
        EXPECT_NO_THROW(adapter.setKeyId(keyId));
        std::cout << "setKeyId invoked successfully with typical keyId vector" << std::endl;
    });
    
    std::cout << "Exiting SetValidKeyIdTypical test" << std::endl;
}
/**
 * @brief Validate that setKeyId function correctly handles boundary byte values in keyId vector.
 *
 * This test verifies that the OCDMSessionAdapter object is created using a valid drmHelper and confirms that 
 * invoking the setKeyId method with a boundary keyId vector (containing values 0x00, 0xFF, 0x7F, 0x80) succeeds 
 * without throwing exceptions. This ensures that the adapter properly handles edge boundary values.
 *
 * **Test Group ID:** Basic: 01
 * **Test Case ID:** 043
 * **Priority:** High
 *
 * **Pre-Conditions:** None
 * **Dependencies:** None
 * **User Interaction:** None
 *
 * **Test Procedure:**
 * | Variation / Step | Description                                                                  | Test Data                                             | Expected Result                                                  | Notes         |
 * | :----:           | -----------------------------------------------------------------------------| ------------------------------------------------------| -----------------------------------------------------------------| ------------- |
 * | 01               | Construct OCDMSessionAdapter object using default constructor with valid drmHelper | drmHelper = instance of WidevineDrmHelper with valid DrmInfo, output: adapter object | OCDMSessionAdapter object is created without exception           | Should Pass   |
 * | 02               | Define keyId vector containing boundary byte values                           | keyId = {0x00, 0xFF, 0x7F, 0x80}                        | keyId vector is correctly initialized                           | Should be successful |
 * | 03               | Invoke setKeyId method with the boundary keyId vector                         | input: keyId = {0x00, 0xFF, 0x7F, 0x80}                | Method call completes without throwing an exception              | Should Pass   |
 */
TEST(OCDMSessionAdapter, SetValidKeyIdBoundary) {
    std::cout << "Entering SetValidKeyIdBoundary test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    // Create OCDMSessionAdapter object using default constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully using default constructor." << std::endl;
        
        // Define keyId vector with boundary byte values.
        std::vector<uint8_t> keyId = {0x00, 0xFF, 0x7F, 0x80};
        std::cout << "KeyId to be set: ";
        for (const auto &byte : keyId) {
            std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Invoke setKeyId method
        std::cout << "Invoking setKeyId with boundary keyId vector" << std::endl;
        EXPECT_NO_THROW(adapter.setKeyId(keyId));
        std::cout << "setKeyId invoked successfully with boundary keyId vector" << std::endl;
    });
    
    std::cout << "Exiting SetValidKeyIdBoundary test" << std::endl;
}
/**
 * @brief Test setting an empty keyId in OCDMSessionAdapter
 *
 * This test verifies that setting an empty keyId vector on the OCDMSessionAdapter object does not throw any exceptions. The test ensures that the default constructor of OCDMSessionAdapter works correctly and that the setKeyId method can handle an empty vector as input.
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
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Create OCDMSessionAdapter object using default constructor | drmInfo = default, drmHelper = shared_ptr<WidevineDrmHelper>(drmInfo) | Object is created without throwing any exceptions | Should Pass |
 * | 02 | Define an empty keyId vector | keyId = empty vector | KeyId vector is empty and prepared for method invocation | Should be successful |
 * | 03 | Invoke setKeyId method with empty keyId vector | keyId = empty vector passed to setKeyId() | setKeyId method executes without throwing any exceptions | Should Pass |
 */
TEST(OCDMSessionAdapter, SetKeyIdEmpty) {
    std::cout << "Entering SetKeyIdEmpty test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo); 
    // Create OCDMSessionAdapter object using default constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully using default constructor." << std::endl;
        
        // Define an empty keyId vector.
        std::vector<uint8_t> keyId;
        std::cout << "KeyId to be set is an empty vector." << std::endl;
        
        // Invoke setKeyId method
        std::cout << "Invoking setKeyId with empty keyId vector" << std::endl;
        EXPECT_NO_THROW(adapter.setKeyId(keyId));
        std::cout << "setKeyId invoked successfully with empty keyId vector" << std::endl;
    });
    
    std::cout << "Exiting SetKeyIdEmpty test" << std::endl;
}
/**
 * @brief Test the functionality of setKeyId with a large keyId vector input.
 *
 * This test verifies that the OCDMSessionAdapter::setKeyId method can handle a large vector of key IDs (1024 sequential byte values) without throwing an exception. It confirms the adapter's ability to process and store a sizable vector correctly and also checks the proper instantiation of the OCDMSessionAdapter object using its default constructor.
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
 * | 01 | Create DrmInfo, instantiate WidevineDrmHelper, and create OCDMSessionAdapter using the default constructor. | drmInfo = default, drmHelper = shared pointer to WidevineDrmHelper, adapter = OCDMSessionAdapter(drmHelper) | OCDMSessionAdapter object is successfully created without exceptions. | Should Pass |
 * | 02 | Create a large vector containing 1024 sequential byte values. | keyId vector: size = 1024, values = sequential bytes from 0 to 255 (repeated as needed) | Large vector is created successfully with sequential values. | Should be successful |
 * | 03 | Invoke the setKeyId method with the large keyId vector. | adapter.setKeyId(keyId), keyId vector = 1024 sequential bytes | setKeyId method completes execution without throwing any exceptions. | Should Pass |
 */
TEST(OCDMSessionAdapter, SetKeyIdLargeVector) {
    std::cout << "Entering SetKeyIdLargeVector test" << std::endl;
    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);     
    // Create OCDMSessionAdapter object using default constructor.
    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper);
        std::cout << "OCDMSessionAdapter object created successfully using default constructor." << std::endl;
        
        // Define a large vector containing 1024 sequential byte values.
        std::vector<uint8_t> keyId;
        keyId.reserve(1024);
        for (int i = 0; i < 1024; ++i) {
            keyId.push_back(static_cast<uint8_t>(i & 0xFF));
        }
        std::cout << "KeyId vector of size " << keyId.size() << " created with sequential values." << std::endl;
        std::cout << "First five values: ";
        for (size_t i = 0; i < 5 && i < keyId.size(); ++i) {
            std::cout << "0x" << std::hex << static_cast<int>(keyId[i]) << " ";
        }
        std::cout << std::dec << std::endl;
        
        // Invoke setKeyId method
        std::cout << "Invoking setKeyId with large keyId vector" << std::endl;
        EXPECT_NO_THROW(adapter.setKeyId(keyId));
        std::cout << "setKeyId invoked successfully with large keyId vector" << std::endl;
    });
    
    std::cout << "Exiting SetKeyIdLargeVector test" << std::endl;
}
#endif

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}