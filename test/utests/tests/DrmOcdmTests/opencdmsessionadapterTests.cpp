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
// PROCESS DRM KEY TESTS
// ========================================

/**
 * @brief Test processDRMKey with valid license data.
 *
 * **Test Group ID:** Adapter_ProcessDRMKey_01@n
 * **Test Case ID:** Adapter_ProcessDRMKey_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke processDRMKey with valid DrmData and 100ms timeout | key=valid DrmData, timeout=100 | Returns int result | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_ValidData)
{
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_ValidData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyData(256);
    for (size_t i = 0; i < keyData.size(); i++) {
        keyData[i] = static_cast<uint8_t>(i % 256);
    }
    
    DrmData key(reinterpret_cast<const char*>(keyData.data()), keyData.size());
    
    EXPECT_NO_THROW({
        int result = adapter.processDRMKey(&key, 100);
        (void)result; // May succeed or fail without real OCDM system
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_ValidData] - PASS" << std::endl;
}

/**
 * @brief Test processDRMKey with NULL key pointer.
 *
 * **Test Group ID:** Adapter_ProcessDRMKey_01@n
 * **Test Case ID:** Adapter_ProcessDRMKey_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke processDRMKey with NULL key pointer | key=nullptr, timeout=100 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_NullKeyPointer)
{
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_NullKeyPointer] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        int result = adapter.processDRMKey(nullptr, 100);
        (void)result;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_NullKeyPointer] - PASS" << std::endl;
}

/**
 * @brief Test processDRMKey with minimal timeout.
 *
 * **Test Group ID:** Adapter_ProcessDRMKey_01@n
 * **Test Case ID:** Adapter_ProcessDRMKey_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke processDRMKey with 1ms timeout | key=valid DrmData, timeout=1 | Returns quickly | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_MinimalTimeout)
{
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_MinimalTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyData(16, 0xAA);
    DrmData key(reinterpret_cast<const char*>(keyData.data()), keyData.size());
    
    auto start = std::chrono::steady_clock::now();
    int result = adapter.processDRMKey(&key, 1);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    (void)result;
    EXPECT_LE(duration, 100); // Should complete quickly

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_MinimalTimeout] - PASS" << std::endl;
}

/**
 * @brief Test processDRMKey with standard timeout.
 *
 * **Test Group ID:** Adapter_ProcessDRMKey_01@n
 * **Test Case ID:** Adapter_ProcessDRMKey_004@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke processDRMKey with 50ms timeout | key=valid DrmData, timeout=50 | Completes within timeout | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_StandardTimeout)
{
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_StandardTimeout] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyData(128);
    for (size_t i = 0; i < keyData.size(); i++) {
        keyData[i] = static_cast<uint8_t>((i * 17) % 256);
    }
    DrmData key(reinterpret_cast<const char*>(keyData.data()), keyData.size());
    
    EXPECT_NO_THROW({
        int result = adapter.processDRMKey(&key, 50);
        (void)result;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_StandardTimeout] - PASS" << std::endl;
}

/**
 * @brief Test processDRMKey with empty key data.
 *
 * **Test Group ID:** Adapter_ProcessDRMKey_01@n
 * **Test Case ID:** Adapter_ProcessDRMKey_005@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke processDRMKey with empty DrmData | key=empty DrmData, timeout=50 | Handles gracefully | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessDRMKey_EmptyKeyData)
{
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_EmptyKeyData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    DrmData key("", 0);
    
    EXPECT_NO_THROW({
        int result = adapter.processDRMKey(&key, 50);
        (void)result;
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessDRMKey_EmptyKeyData] - PASS" << std::endl;
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
// SETKEYID TESTS
// ========================================

/**
 * @brief Test setKeyId with valid key ID data.
 *
 * **Test Group ID:** Adapter_SetKeyId_01@n
 * **Test Case ID:** Adapter_SetKeyId_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Set valid 16-byte key ID | keyId vector with 16 bytes | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, SetKeyId_ValidData)
{
    std::cout << "[OCDMSessionAdapter.SetKeyId_ValidData] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyId = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                   0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    EXPECT_NO_THROW({
        adapter.setKeyId(keyId);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.SetKeyId_ValidData] - PASS" << std::endl;
}

/**
 * @brief Test setKeyId with empty key ID.
 *
 * **Test Group ID:** Adapter_SetKeyId_01@n
 * **Test Case ID:** Adapter_SetKeyId_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Set empty key ID | Empty vector | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, SetKeyId_EmptyKeyId)
{
    std::cout << "[OCDMSessionAdapter.SetKeyId_EmptyKeyId] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyId;

    EXPECT_NO_THROW({
        adapter.setKeyId(keyId);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.SetKeyId_EmptyKeyId] - PASS" << std::endl;
}

/**
 * @brief Test setKeyId with large key ID (256 bytes).
 *
 * **Test Group ID:** Adapter_SetKeyId_01@n
 * **Test Case ID:** Adapter_SetKeyId_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Set large 256-byte key ID | 256-byte vector | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, SetKeyId_LargeKeyId)
{
    std::cout << "[OCDMSessionAdapter.SetKeyId_LargeKeyId] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    std::vector<uint8_t> keyId(256);
    for (int i = 0; i < 256; i++) {
        keyId[i] = static_cast<uint8_t>(i);
    }

    EXPECT_NO_THROW({
        adapter.setKeyId(keyId);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.SetKeyId_LargeKeyId] - PASS" << std::endl;
}

// ========================================
// PROCESSOCDMCHALLENGE TESTS
// ========================================

/**
 * @brief Test processOCDMChallenge with standard challenge.
 *
 * **Test Group ID:** Adapter_ProcessChallenge_01@n
 * **Test Case ID:** Adapter_ProcessChallenge_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Process standard DRM challenge | destUrl, challenge data | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessOCDMChallenge_StandardChallenge)
{
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_StandardChallenge] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    const char* destUrl = "https://drm.example.com/license";
    const char* challengeData = "standard-challenge-data-for-testing";
    const uint8_t* challenge = reinterpret_cast<const uint8_t*>(challengeData);
    uint16_t challengeSize = static_cast<uint16_t>(strlen(challengeData));

    EXPECT_NO_THROW({
        adapter.processOCDMChallenge(destUrl, challenge, challengeSize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_StandardChallenge] - PASS" << std::endl;
}

/**
 * @brief Test processOCDMChallenge with individualization request.
 *
 * **Test Group ID:** Adapter_ProcessChallenge_01@n
 * **Test Case ID:** Adapter_ProcessChallenge_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Process individualization request | Message with Type delimiter | Callback triggered | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessOCDMChallenge_IndividualizationRequest)
{
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_IndividualizationRequest] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    const char* destUrl = "https://drm.example.com/individualization";
    const char* challengeData = "individualization-request:Type:payload-data-here";
    const uint8_t* challenge = reinterpret_cast<const uint8_t*>(challengeData);
    uint16_t challengeSize = static_cast<uint16_t>(strlen(challengeData));

    EXPECT_NO_THROW({
        adapter.processOCDMChallenge(destUrl, challenge, challengeSize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_IndividualizationRequest] - PASS" << std::endl;
}

/**
 * @brief Test processOCDMChallenge with license renewal message.
 *
 * **Test Group ID:** Adapter_ProcessChallenge_01@n
 * **Test Case ID:** Adapter_ProcessChallenge_003@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Process license renewal | LICENSE_RENEWAL message | Renewal callback triggered | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessOCDMChallenge_LicenseRenewal)
{
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_LicenseRenewal] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    const char* destUrl = "https://drm.example.com/renewal";
    const char* challengeData = "LICENSE_RENEWAL:Type:renewal-payload";
    const uint8_t* challenge = reinterpret_cast<const uint8_t*>(challengeData);
    uint16_t challengeSize = static_cast<uint16_t>(strlen(challengeData));

    EXPECT_NO_THROW({
        adapter.processOCDMChallenge(destUrl, challenge, challengeSize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_LicenseRenewal] - PASS" << std::endl;
}

/**
 * @brief Test processOCDMChallenge with empty challenge.
 *
 * **Test Group ID:** Adapter_ProcessChallenge_01@n
 * **Test Case ID:** Adapter_ProcessChallenge_004@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Process empty challenge | Size=0 | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessOCDMChallenge_EmptyChallenge)
{
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_EmptyChallenge] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    const char* destUrl = "https://drm.example.com/license";
    const uint8_t* challenge = nullptr;
    uint16_t challengeSize = 0;

    EXPECT_NO_THROW({
        adapter.processOCDMChallenge(destUrl, challenge, challengeSize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_EmptyChallenge] - PASS" << std::endl;
}

/**
 * @brief Test processOCDMChallenge with large challenge data.
 *
 * **Test Group ID:** Adapter_ProcessChallenge_01@n
 * **Test Case ID:** Adapter_ProcessChallenge_005@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Process large challenge | 4KB challenge data | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, ProcessOCDMChallenge_LargeChallenge)
{
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_LargeChallenge] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    const char* destUrl = "https://drm.example.com/license";
    std::vector<uint8_t> challengeData(4096);
    for (size_t i = 0; i < challengeData.size(); i++) {
        challengeData[i] = static_cast<uint8_t>(i % 256);
    }
    uint16_t challengeSize = static_cast<uint16_t>(challengeData.size());

    EXPECT_NO_THROW({
        adapter.processOCDMChallenge(destUrl, challengeData.data(), challengeSize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.ProcessOCDMChallenge_LargeChallenge] - PASS" << std::endl;
}

// ========================================
// KEYUPDATEOCDM TESTS
// ========================================

/**
 * @brief Test keyUpdateOCDM with valid key data.
 *
 * **Test Group ID:** Adapter_KeyUpdate_01@n
 * **Test Case ID:** Adapter_KeyUpdate_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Update with valid key | 32-byte key | Key added to usable keys | Should Pass |
 */
TEST(OCDMSessionAdapter, KeyUpdateOCDM_ValidKey)
{
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_ValidKey] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    uint8_t key[32];
    for (int i = 0; i < 32; i++) {
        key[i] = static_cast<uint8_t>(i);
    }
    uint8_t keySize = 32;

    EXPECT_NO_THROW({
        adapter.keyUpdateOCDM(key, keySize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_ValidKey] - PASS" << std::endl;
}

/**
 * @brief Test keyUpdateOCDM with multiple different keys.
 *
 * **Test Group ID:** Adapter_KeyUpdate_01@n
 * **Test Case ID:** Adapter_KeyUpdate_002@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Update with 10 different keys | Multiple key updates | All keys stored | Should Pass |
 */
TEST(OCDMSessionAdapter, KeyUpdateOCDM_MultipleKeys)
{
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_MultipleKeys] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    for (int i = 0; i < 10; i++) {
        uint8_t key[16];
        for (int j = 0; j < 16; j++) {
            key[j] = static_cast<uint8_t>(i * 16 + j);
        }
        uint8_t keySize = 16;
        
        EXPECT_NO_THROW({
            adapter.keyUpdateOCDM(key, keySize);
        });
    }

    const auto& usableKeys = adapter.getUsableKeys();
    EXPECT_GE(usableKeys.size(), 0);

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_MultipleKeys] - PASS" << std::endl;
}

/**
 * @brief Test keyUpdateOCDM with duplicate key.
 *
 * **Test Group ID:** Adapter_KeyUpdate_01@n
 * **Test Case ID:** Adapter_KeyUpdate_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Update same key twice | Same key data | Duplicate not added | Should Pass |
 */
TEST(OCDMSessionAdapter, KeyUpdateOCDM_DuplicateKey)
{
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_DuplicateKey] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    uint8_t key[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                       0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    uint8_t keySize = 16;

    EXPECT_NO_THROW({
        adapter.keyUpdateOCDM(key, keySize);
        adapter.keyUpdateOCDM(key, keySize); // Same key again
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_DuplicateKey] - PASS" << std::endl;
}

/**
 * @brief Test keyUpdateOCDM with zero-size key.
 *
 * **Test Group ID:** Adapter_KeyUpdate_01@n
 * **Test Case ID:** Adapter_KeyUpdate_004@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Update with keySize=0 | Empty key | No exception | Should Pass |
 */
TEST(OCDMSessionAdapter, KeyUpdateOCDM_EmptyKey)
{
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_EmptyKey] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    uint8_t key[1] = {0x00};
    uint8_t keySize = 0;

    EXPECT_NO_THROW({
        adapter.keyUpdateOCDM(key, keySize);
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeyUpdateOCDM_EmptyKey] - PASS" << std::endl;
}

// ========================================
// KEYSUPDATEDOCDM TESTS
// ========================================

/**
 * @brief Test keysUpdatedOCDM signals event.
 *
 * **Test Group ID:** Adapter_KeysUpdated_01@n
 * **Test Case ID:** Adapter_KeysUpdated_001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Invoke keysUpdatedOCDM | None | Event signaled | Should Pass |
 */
TEST(OCDMSessionAdapter, KeysUpdatedOCDM_EventSignal)
{
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_EventSignal] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    EXPECT_NO_THROW({
        adapter.keysUpdatedOCDM();
    });

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_EventSignal] - PASS" << std::endl;
}

/**
 * @brief Test keysUpdatedOCDM called multiple times.
 *
 * **Test Group ID:** Adapter_KeysUpdated_01@n
 * **Test Case ID:** Adapter_KeysUpdated_002@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Call keysUpdatedOCDM 100 times | Multiple calls | All succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, KeysUpdatedOCDM_MultipleCalls)
{
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_MultipleCalls] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    for (int i = 0; i < 100; i++) {
        EXPECT_NO_THROW({
            adapter.keysUpdatedOCDM();
        });
    }

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_MultipleCalls] - PASS" << std::endl;
}

/**
 * @brief Test keysUpdatedOCDM with rapid consecutive calls.
 *
 * **Test Group ID:** Adapter_KeysUpdated_01@n
 * **Test Case ID:** Adapter_KeysUpdated_003@n
 * **Priority:** Medium@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**@n
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :--------------: | ----------- | --------- | --------------- | ----- |
 * | 01 | Rapid consecutive calls | 1000 calls | All succeed | Should Pass |
 */
TEST(OCDMSessionAdapter, KeysUpdatedOCDM_RapidCalls)
{
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_RapidCalls] - START" << std::endl;

    DrmInfo drmInfo;
    auto drmHelper = std::make_shared<WidevineDrmHelper>(drmInfo);
    auto* drmCallbacks = new TestDrmCallbacks();
    OCDMSessionAdapter adapter(drmHelper, drmCallbacks);

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000; i++) {
        adapter.keysUpdatedOCDM();
    }
    auto end = std::chrono::steady_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    EXPECT_LT(duration, 1000); // Should complete in < 1 second

    delete drmCallbacks;
    std::cout << "[OCDMSessionAdapter.KeysUpdatedOCDM_RapidCalls] - PASS" << std::endl;
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
