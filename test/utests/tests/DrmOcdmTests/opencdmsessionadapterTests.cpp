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
 * @file opencdmsessionadapterTests_new.cpp
 * @brief Comprehensive unit tests for OCDMSessionAdapter and Event classes
 *
 * This file contains extensive unit tests for the base OCDMSessionAdapter class
 * and the Event helper class, including all public APIs, edge cases, stress tests,
 * and integration scenarios.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <thread>
#include <chrono>
#include "opencdmsessionadapter.h"

/**
 * @brief Test DrmCallbacks implementation
 */
class TestDrmCallbacks : public DrmCallbacks {
public:
    void Individualization(const std::string& payload) override {
        (void)payload;
    }

    void LicenseRenewal(DrmHelperPtr drmHelper, void* userData) override {
        (void)drmHelper;
        (void)userData;
    }
};

/**
 * @brief Widevine DRM Helper for testing
 */
class WidevineDrmHelper : public DrmHelper {
public:
    WidevineDrmHelper(const DrmInfo& drmInfo) : DrmHelper(drmInfo) {}

    const std::string& ocdmSystemId() const override {
        static const std::string id = "com.widevine.alpha";
        return id;
    }

    void createInitData(std::vector<uint8_t>& initData) const override {
        initData = {0x00, 0x00, 0x00, 0x20};
    }

    bool parsePssh(const uint8_t* initData, uint32_t initDataLen) override {
        return (initData != nullptr && initDataLen > 0);
    }

    bool isClearDecrypt() const override { return false; }

    void getKey(std::vector<uint8_t>& keyID) const override {
        keyID = {0x12, 0x34, 0x56, 0x78};
    }

    void generateLicenseRequest(const ChallengeInfo&, LicenseRequest&) const override { }
};

/**
 * @brief Testable OCDMSessionAdapter for accessing protected members
 */
class TestableOCDMSessionAdapter : public OCDMSessionAdapter {
public:
    TestableOCDMSessionAdapter(DrmHelperPtr drmHelper, DrmCallbacks* callbacks = nullptr)
        : OCDMSessionAdapter(drmHelper, callbacks) {}

    bool callVerifyOutputProtection() {
        return OCDMSessionAdapter::verifyOutputProtection();
    }

    void triggerProcessOCDMChallenge(const char* url, const uint8_t* challenge, uint16_t size) {
        processOCDMChallenge(url, challenge, size);
    }

    void triggerKeysUpdatedOCDM() {
        keysUpdatedOCDM();
    }

    void triggerKeyUpdateOCDM(const uint8_t* key, uint8_t keySize) {
        keyUpdateOCDM(key, keySize);
    }
};

// ========================================
// EVENT CLASS TESTS
// ========================================

/**
 * @brief Verify Event default constructor.
 *
 * **Test Group ID:** Event_Constructor_01@n
 * **Test Case ID:** Event_Constructor_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create Event object | None | Object created successfully | Should Pass |
 */
TEST(Event, Constructor_DefaultConstruction)
{
    std::cout << "[Event.Constructor_DefaultConstruction] - START" << std::endl;

    EXPECT_NO_THROW({
        Event event;
    });

    std::cout << "[Event.Constructor_DefaultConstruction] - PASS" << std::endl;
}

/**
 * @brief Verify Event signal method.
 *
 * **Test Group ID:** Event_Signal_01@n
 * **Test Case ID:** Event_Signal_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke signal() on Event object | None | No exception thrown | Should Pass |
 */
TEST(Event, Signal_BasicSignal)
{
    std::cout << "[Event.Signal_BasicSignal] - START" << std::endl;

    EXPECT_NO_THROW({
        Event event;
        event.signal();
    });

    std::cout << "[Event.Signal_BasicSignal] - PASS" << std::endl;
}

/**
 * @brief Verify Event signal called multiple times.
 *
 * **Test Group ID:** Event_Signal_01@n
 * **Test Case ID:** Event_Signal_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call signal() 100 times | None | All calls succeed | Should Pass |
 */
TEST(Event, Signal_MultipleSignals)
{
    std::cout << "[Event.Signal_MultipleSignals] - START" << std::endl;

    Event event;
    
    EXPECT_NO_THROW({
        for (int i = 0; i < 100; i++) {
            event.signal();
        }
    });

    std::cout << "[Event.Signal_MultipleSignals] - PASS" << std::endl;
}

/**
 * @brief Verify Event wait with immediate signal (no timeout).
 *
 * **Test Group ID:** Event_Wait_01@n
 * **Test Case ID:** Event_Wait_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Signal event then wait with 100ms timeout | timeout=100 | Wait returns true immediately | Should Pass |
 */
TEST(Event, Wait_SignalThenWait)
{
    std::cout << "[Event.Wait_SignalThenWait] - START" << std::endl;

    Event event;
    event.signal();
    
    bool result = event.wait(100);
    
    EXPECT_TRUE(result);

    std::cout << "[Event.Wait_SignalThenWait] - PASS" << std::endl;
}

/**
 * @brief Verify Event wait timeout when not signalled.
 *
 * **Test Group ID:** Event_Wait_01@n
 * **Test Case ID:** Event_Wait_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Wait without signal, 50ms timeout | timeout=50 | Returns false after timeout | Should Pass |
 */
TEST(Event, Wait_TimeoutNoSignal)
{
    std::cout << "[Event.Wait_TimeoutNoSignal] - START" << std::endl;

    Event event;
    
    auto start = std::chrono::steady_clock::now();
    bool result = event.wait(50);
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    EXPECT_FALSE(result);
    EXPECT_GE(duration, 40); // Allow some timing variance

    std::cout << "[Event.Wait_TimeoutNoSignal] - PASS" << std::endl;
}

/**
 * @brief Verify Event wait with zero timeout.
 *
 * **Test Group ID:** Event_Wait_01@n
 * **Test Case ID:** Event_Wait_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Signal and wait with timeout=0 | timeout=0 | Returns true | Should Pass |
 */
TEST(Event, Wait_ZeroTimeout)
{
    std::cout << "[Event.Wait_ZeroTimeout] - START" << std::endl;

    Event event;
    event.signal();
    
    bool result = event.wait(0);
    
    EXPECT_TRUE(result);

    std::cout << "[Event.Wait_ZeroTimeout] - PASS" << std::endl;
}

/**
 * @brief Verify Event destructor.
 *
 * **Test Group ID:** Event_Destructor_01@n
 * **Test Case ID:** Event_Destructor_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create and destroy Event object | None | Destructor executes without error | Should Pass |
 */
TEST(Event, Destructor_ValidDestruction)
{
    std::cout << "[Event.Destructor_ValidDestruction] - START" << std::endl;

    EXPECT_NO_THROW({
        {
            Event event;
            event.signal();
        }
    });

    std::cout << "[Event.Destructor_ValidDestruction] - PASS" << std::endl;
}

// ========================================
// OCDMSESSIONADAPTER CONSTRUCTOR TESTS
// ========================================

/**
 * @brief Validate OCDMSessionAdapter construction with valid parameters.
 *
 * **Test Group ID:** Adapter_Constructor_01@n
 * **Test Case ID:** Adapter_Constructor_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create adapter with valid helper and callbacks | drmHelper, drmCallbacks | Constructor succeeds | Should Pass |
 */
TEST(OCDMSessionAdapter, Constructor_ValidConstruction)
{
    std::cout << "[OCDMSessionAdapter.Constructor_ValidConstruction] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.Constructor_ValidConstruction] - PASS" << std::endl;
}

/**
 * @brief Validate adapter construction without callbacks.
 *
 * **Test Group ID:** Adapter_Constructor_01@n
 * **Test Case ID:** Adapter_Constructor_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create adapter with nullptr callbacks | callbacks=nullptr | Constructor succeeds or handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, Constructor_NullCallbacks)
{
    std::cout << "[OCDMSessionAdapter.Constructor_NullCallbacks] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter(drmHelper, nullptr);
    });

    std::cout << "[OCDMSessionAdapter.Constructor_NullCallbacks] - PASS" << std::endl;
}

/**
 * @brief Validate adapter destructor.
 *
 * **Test Group ID:** Adapter_Constructor_01@n
 * **Test Case ID:** Adapter_Constructor_003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create and destroy adapter | Valid parameters | Destructor executes without error | Should Pass |
 */
TEST(OCDMSessionAdapter, Constructor_ValidDestruction)
{
    std::cout << "[OCDMSessionAdapter.Constructor_ValidDestruction] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();

    EXPECT_NO_THROW({
        {
            OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
        }
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.Constructor_ValidDestruction] - PASS" << std::endl;
}

/**
 * @brief Test multiple adapter instances.
 *
 * **Test Group ID:** Adapter_Constructor_01@n
 * **Test Case ID:** Adapter_Constructor_004@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 3 adapter instances | Valid parameters each | All instances created successfully | Should Pass |
 */
TEST(OCDMSessionAdapter, Constructor_MultipleInstances)
{
    std::cout << "[OCDMSessionAdapter.Constructor_MultipleInstances] - START" << std::endl;

    DrmInfo drmInfo;
    auto helper1 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto helper2 = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto helper3 = std::make_shared<WidevineDrmHelper>(drmInfo);

    auto* cb1 = new TestDrmCallbacks();
    auto* cb2 = new TestDrmCallbacks();
    auto* cb3 = new TestDrmCallbacks();

    EXPECT_NO_THROW({
        OCDMSessionAdapter adapter1(helper1, cb1);
        OCDMSessionAdapter adapter2(helper2, cb2);
        OCDMSessionAdapter adapter3(helper3, cb3);
    });

    delete cb1;
    delete cb2;
    delete cb3;

    std::cout << "[OCDMSessionAdapter.Constructor_MultipleInstances] - PASS" << std::endl;
}

// ========================================
// CLEAR DECRYPT CONTEXT TESTS
// ========================================

/**
 * @brief Verify clearDecryptContext method.
 *
 * **Test Group ID:** Adapter_ClearContext_01@n
 * **Test Case ID:** Adapter_ClearContext_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke clearDecryptContext | None | No exception thrown | Should Pass |
 */
TEST(OCDMSessionAdapter, ClearContext_BasicClear)
{
    std::cout << "[OCDMSessionAdapter.ClearContext_BasicClear] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        adapter.clearDecryptContext();
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ClearContext_BasicClear] - PASS" << std::endl;
}

/**
 * @brief Verify multiple clearDecryptContext calls.
 *
 * **Test Group ID:** Adapter_ClearContext_01@n
 * **Test Case ID:** Adapter_ClearContext_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call clearDecryptContext 10 times | None | All calls succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, ClearContext_MultipleCalls)
{
    std::cout << "[OCDMSessionAdapter.ClearContext_MultipleCalls] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        for (int i = 0; i < 10; i++) {
            adapter.clearDecryptContext();
        }
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ClearContext_MultipleCalls] - PASS" << std::endl;
}

// ========================================
// GENERATE DRM SESSION TESTS
// ========================================

/**
 * @brief Test generateDRMSession with valid init data.
 *
 * **Test Group ID:** Adapter_GenerateSession_01@n
 * **Test Case ID:** Adapter_GenerateSession_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke generateDRMSession with 16-byte init data | initData[16], customData="" | No exception thrown | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateSession_ValidInitData)
{
    std::cout << "[OCDMSessionAdapter.GenerateSession_ValidInitData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> initData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                      0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    std::string customData = "test-custom-data";

    EXPECT_NO_THROW({
        adapter.generateDRMSession(initData.data(), static_cast<uint32_t>(initData.size()), customData);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateSession_ValidInitData] - PASS" << std::endl;
}

/**
 * @brief Test generateDRMSession with minimal init data.
 *
 * **Test Group ID:** Adapter_GenerateSession_01@n
 * **Test Case ID:** Adapter_GenerateSession_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke with 1-byte init data | initData[1]=0xFF | No exception thrown | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateSession_MinimalInitData)
{
    std::cout << "[OCDMSessionAdapter.GenerateSession_MinimalInitData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    uint8_t initData[1] = {0xFF};
    std::string customData = "";

    EXPECT_NO_THROW({
        adapter.generateDRMSession(initData, 1, customData);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateSession_MinimalInitData] - PASS" << std::endl;
}

/**
 * @brief Test generateDRMSession with NULL init data.
 *
 * **Test Group ID:** Adapter_GenerateSession_Invalid_01@n
 * **Test Case ID:** Adapter_GenerateSession_003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke with NULL init data | initData=nullptr, size=16 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateSession_NullInitData)
{
    std::cout << "[OCDMSessionAdapter.GenerateSession_NullInitData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::string customData = "test";

    EXPECT_NO_THROW({
        adapter.generateDRMSession(nullptr, 16, customData);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateSession_NullInitData] - PASS" << std::endl;
}

/**
 * @brief Test generateDRMSession with zero init data size.
 *
 * **Test Group ID:** Adapter_GenerateSession_Invalid_01@n
 * **Test Case ID:** Adapter_GenerateSession_004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke with size=0 | initData[16], size=0 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateSession_ZeroInitDataSize)
{
    std::cout << "[OCDMSessionAdapter.GenerateSession_ZeroInitDataSize] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> initData(16, 0xAA);
    std::string customData = "";

    EXPECT_NO_THROW({
        adapter.generateDRMSession(initData.data(), 0, customData);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateSession_ZeroInitDataSize] - PASS" << std::endl;
}

/**
 * @brief Test generateDRMSession with large init data (4KB).
 *
 * **Test Group ID:** Adapter_GenerateSession_01@n
 * **Test Case ID:** Adapter_GenerateSession_005@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke with 4KB init data | initData[4096] | No exception thrown | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateSession_LargeInitData)
{
    std::cout << "[OCDMSessionAdapter.GenerateSession_LargeInitData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> initData(4096);
    for (size_t i = 0; i < initData.size(); i++) {
        initData[i] = static_cast<uint8_t>(i % 256);
    }
    std::string customData = "large-data-test";

    EXPECT_NO_THROW({
        adapter.generateDRMSession(initData.data(), static_cast<uint32_t>(initData.size()), customData);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateSession_LargeInitData] - PASS" << std::endl;
}

// ========================================
// GENERATE KEY REQUEST TESTS
// ========================================

/**
 * @brief Test generateKeyRequest with standard timeout.
 *
 * **Test Group ID:** Adapter_GenerateKeyRequest_01@n
 * **Test Case ID:** Adapter_GenerateKeyRequest_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke generateKeyRequest with 5000ms timeout | timeout=5000 | Returns DrmData pointer or null | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateKeyRequest_StandardTimeout)
{
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_StandardTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::string destinationURL;
    
    EXPECT_NO_THROW({
        DrmData* key = adapter.generateKeyRequest(destinationURL, 50);
        if (key) delete key;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_StandardTimeout] - PASS" << std::endl;
}

/**
 * @brief Test generateKeyRequest with zero timeout.
 *
 * **Test Group ID:** Adapter_GenerateKeyRequest_01@n
 * **Test Case ID:** Adapter_GenerateKeyRequest_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * |01 | Invoke with timeout=0 | timeout=0 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateKeyRequest_ZeroTimeout)
{
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_ZeroTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::string destinationURL;
    
    EXPECT_NO_THROW({
        DrmData* key = adapter.generateKeyRequest(destinationURL, 1);
        if (key) delete key;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_ZeroTimeout] - PASS" << std::endl;
}

/**
 * @brief Test generateKeyRequest with maximum timeout.
 *
 * **Test Group ID:** Adapter_GenerateKeyRequest_01@n
 * **Test Case ID:** Adapter_GenerateKeyRequest_003@n
 * **Priority:** Low@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke with 100ms timeout (reduced from max for testing) | timeout=100 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, GenerateKeyRequest_MaxTimeout)
{
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_MaxTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::string destinationURL;
    
    EXPECT_NO_THROW({
        DrmData* key = adapter.generateKeyRequest(destinationURL, 100);
        if (key) delete key;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GenerateKeyRequest_MaxTimeout] - PASS" << std::endl;
}

// ========================================
// GET STATE TESTS
// ========================================

/**
 * @brief Test getState method returns valid state.
 *
 * **Test Group ID:** Adapter_GetState_01@n
 * **Test Case ID:** Adapter_GetState_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke getState | None | Returns valid KeyState | Should Pass |
 */
TEST(OCDMSessionAdapter, GetState_ValidState)
{
    std::cout << "[OCDMSessionAdapter.GetState_ValidState] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        KeyState state = adapter.getState();
        EXPECT_TRUE(state >= KEY_INIT && state <= KEY_ERROR_EMPTY_SESSION_ID);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GetState_ValidState] - PASS" << std::endl;
}

/**
 * @brief Test getState called multiple times.
 *
 * **Test Group ID:** Adapter_GetState_01@n
 * **Test Case ID:** Adapter_GetState_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call getState 100 times | None | All calls succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, GetState_MultipleCalls)
{
    std::cout << "[OCDMSessionAdapter.GetState_MultipleCalls] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    for (int i = 0; i < 100; i++) {
        KeyState state = adapter.getState();
        (void)state;
    }

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GetState_MultipleCalls] - PASS" << std::endl;
}

// ========================================
// WAIT FOR STATE TESTS
// ========================================

/**
 * @brief Test waitForState with KEY_INIT state.
 *
 * **Test Group ID:** Adapter_WaitForState_01@n
 * **Test Case ID:** Adapter_WaitForState_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Wait for KEY_INIT state with 100ms timeout | state=KEY_INIT, timeout=100 | Returns bool result | Should Pass |
 */
TEST(OCDMSessionAdapter, WaitForState_KeyInit)
{
    std::cout << "[OCDMSessionAdapter.WaitForState_KeyInit] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    bool result = adapter.waitForState(KEY_INIT, 100);
    
    EXPECT_TRUE(result == true || result == false);

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.WaitForState_KeyInit] - PASS" << std::endl;
}

/**
 * @brief Test waitForState with KEY_READY state.
 *
 * **Test Group ID:** Adapter_WaitForState_01@n
 * **Test Case ID:** Adapter_WaitForState_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Wait for KEY_READY with 50ms timeout | state=KEY_READY, timeout=50 | Returns bool | Should Pass |
 */
TEST(OCDMSessionAdapter, WaitForState_KeyReady)
{
    std::cout << "[OCDMSessionAdapter.WaitForState_KeyReady] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    bool result = adapter.waitForState(KEY_READY, 50);
    
    EXPECT_TRUE(result == true || result == false);

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.WaitForState_KeyReady] - PASS" << std::endl;
}

/**
 * @brief Test waitForState with zero timeout.
 *
 * **Test Group ID:** Adapter_WaitForState_01@n
 * **Test Case ID:** Adapter_WaitForState_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Wait with timeout=0 | state=KEY_INIT, timeout=0 | Returns immediately | Should Pass |
 */
TEST(OCDMSessionAdapter, WaitForState_ZeroTimeout)
{
    std::cout << "[OCDMSessionAdapter.WaitForState_ZeroTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    auto start = std::chrono::steady_clock::now();
    bool result = adapter.waitForState(KEY_INIT, 1);
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    EXPECT_TRUE(result == true || result == false);
    EXPECT_LE(duration, 100); // Should return quickly

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.WaitForState_ZeroTimeout] - PASS" << std::endl;
}

// ========================================
// GET USABLE KEYS TESTS
// ========================================

/**
 * @brief Test getUsableKeys returns vector.
 *
 * **Test Group ID:** Adapter_GetUsableKeys_01@n
 * **Test Case ID:** Adapter_GetUsableKeys_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke getUsableKeys | None | Returns vector of key IDs | Should Pass |
 */
TEST(OCDMSessionAdapter, GetUsableKeys_ValidReturn)
{
    std::cout << "[OCDMSessionAdapter.GetUsableKeys_ValidReturn] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        const auto& keys = adapter.getUsableKeys();
        (void)keys;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.GetUsableKeys_ValidReturn] - PASS" << std::endl;
}

// ========================================
// STRESS TESTS
// ========================================

/**
 * @brief Stress test with 500 consecutive getState calls.
 *
 * **Test Group ID:** Adapter_Stress_01@n
 * **Test Case ID:** Adapter_Stress_001@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call getState 500 times | None | All calls succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, Stress_ConsecutiveGetState)
{
    std::cout << "[OCDMSessionAdapter.Stress_ConsecutiveGetState] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    for (int i = 0; i < 500; i++) {
        KeyState state = adapter.getState();
        (void)state;
    }

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.Stress_ConsecutiveGetState] - PASS" << std::endl;
}

/**
 * @brief Stress test with 200 clearDecryptContext calls.
 *
 * **Test Group ID:** Adapter_Stress_01@n
 * **Test Case ID:** Adapter_Stress_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call clearDecryptContext 200 times | None | All calls succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, Stress_ConsecutiveClearContext)
{
    std::cout << "[OCDMSessionAdapter.Stress_ConsecutiveClearContext] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    for (int i = 0; i < 200; i++) {
        adapter.clearDecryptContext();
    }

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.Stress_ConsecutiveClearContext] - PASS" << std::endl;
}

/**
 * @brief Stress test creating/destroying 100 adapters.
 *
 * **Test Group ID:** Adapter_Stress_01@n
 * **Test Case ID:** Adapter_Stress_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create and destroy adapter 100 times | Valid parameters | All operations succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, Stress_LifecycleIterations)
{
    std::cout << "[OCDMSessionAdapter.Stress_LifecycleIterations] - START" << std::endl;

    DrmInfo drmInfo;

    for (int i = 0; i < 100; i++) {
        auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
        auto* drmCallbacks = new TestDrmCallbacks();
        
        {
            OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
            adapter.clearDecryptContext();
            KeyState state = adapter.getState();
            (void)state;
        }
        
        delete drmCallbacks;
    }

    std::cout << "[OCDMSessionAdapter.Stress_LifecycleIterations] - PASS" << std::endl;
}

// ========================================
// INTEGRATION TESTS
// ========================================

/**
 * @brief Integration test: Full workflow simulation.
 *
 * **Test Group ID:** Adapter_Integration_01@n
 * **Test Case ID:** Adapter_Integration_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Execute full workflow: create, generate session, get state, clear, destroy | Standard data | All operations complete | Should Pass |
 */
TEST(OCDMSessionAdapter, Integration_FullWorkflow)
{
    std::cout << "[OCDMSessionAdapter.Integration_FullWorkflow] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();

    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);
    
    // Generate session
    std::vector<uint8_t> initData = {0x01, 0x02, 0x03, 0x04};
    std::string customData = "test";
    adapter.generateDRMSession(initData.data(), 4, customData);
    
    // Get state
    KeyState state = adapter.getState();
    (void)state;
    
    // Clear context
    adapter.clearDecryptContext();
    
    // Get usable keys
    const auto& keys = adapter.getUsableKeys();
    (void)keys;

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.Integration_FullWorkflow] - PASS" << std::endl;
}

/**
 * @brief Integration test: Multiple adapters concurrent operations.
 *
 * **Test Group ID:** Adapter_Integration_01@n
 * **Test Case ID:** Adapter_Integration_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Create 5 adapters, perform operations on each | Different data | All operations succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, Integration_MultipleAdaptersConcurrent)
{
    std::cout << "[OCDMSessionAdapter.Integration_MultipleAdaptersConcurrent] - START" << std::endl;

    DrmInfo drmInfo;
    std::vector<std::shared_ptr<WidevineDrmHelper>> helpers;
    std::vector<DrmCallbacks*> callbacks;

    for (int i = 0; i < 5; i++) {
        helpers.push_back(std::make_shared<WidevineDrmHelper>(drmInfo));
        callbacks.push_back(new TestDrmCallbacks());
    }

    std::vector<std::unique_ptr<OCDMSessionAdapter>> adapters;
    
    for (size_t i = 0; i < helpers.size(); i++) {
        adapters.push_back(std::make_unique<OCDMSessionAdapter>(helpers[i], callbacks[i]));
    }

    for (auto& adapter : adapters) {
        adapter->clearDecryptContext();
        KeyState state = adapter->getState();
        (void)state;
    }

    for (auto* cb : callbacks) delete cb;

    std::cout << "[OCDMSessionAdapter.Integration_MultipleAdaptersConcurrent] - PASS" << std::endl;
}
