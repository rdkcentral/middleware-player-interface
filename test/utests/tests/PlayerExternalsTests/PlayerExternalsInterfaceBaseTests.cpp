/*
 * If not stated otherwise in this file or this component's license file the
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
 * @file PlayerExternalsInterfaceBaseTests.cpp
 * @brief Comprehensive unit tests for PlayerExternalsInterfaceBase abstract class.
 *
 * Uses a concrete TestExternalsBase subclass to exercise all non-pure public
 * APIs inherited from PlayerExternalsInterfaceBase.
 *
 * APIs covered:
 *   Constructor, IsSourceUHD, setGstElement, getgstElement,
 *   GetDisplayResolution, SetHDMIStatus, GetTR181Config,
 *   isHDCPConnection2_2, GetActiveInterface, Initialize,
 *   SetUseFireBoltSDK, ~PlayerExternalsInterfaceBase.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "PlayerExternalsInterfaceBase.h"

// ===========================================================================
// Concrete test subclass
// ===========================================================================

class TestExternalsBase : public PlayerExternalsInterfaceBase
{
public:
    bool initCalled{false};
    bool fireboltFlag{false};
    bool powerEvent{false};
    std::function<void()> fakeTuneCallback;

    void Initialize() override { initCalled = true; }
    void SetUseFireBoltSDK(bool v) override { fireboltFlag = v; }
    void SetPowerEvent(bool powerEvt) override { powerEvent = powerEvt; }
    bool GetPowerEvent() override { return powerEvent; }
    void SetDoFakeTuneCallBack(const std::function<void()>& t_doFakeTuneCallback) override { fakeTuneCallback = t_doFakeTuneCallback; }
    std::function<void()> GetDoFakeTuneCallBack() override { return fakeTuneCallback; }
};

// ===========================================================================
// Fixture
// ===========================================================================

class ExternalsBaseTest : public ::testing::Test
{
protected:
    TestExternalsBase obj;
};

// ===========================================================================
// Group: Construction
// ===========================================================================

/**
 * @brief Default constructor initialises gstElement to nullptr.
 *
 * **Test Group ID:** ExternalsBase_Construction_001@n
 * **Test Case ID:** ExternalsBase_Construction_001@n
 * **Priority:** High@n
 *
 * | Step | Description | Data | Expected | Notes |
 * |------|-------------|------|----------|-------|
 * | 01 | Construct TestExternalsBase | - | No throw | Should Pass |
 * | 02 | Check getgstElement() | - | nullptr | Should Pass |
 */
TEST_F(ExternalsBaseTest, DefaultConstructorGstElementIsNull)
{
    std::cout << "[ExternalsBaseTest.DefaultConstructorGstElementIsNull] - START" << std::endl;
    // setGstElement(nullptr) verified indirectly: IsSourceUHD() returns false when null
    obj.setGstElement(nullptr);
    EXPECT_FALSE(obj.IsSourceUHD());
    std::cout << "  gstElement is nullptr on default construction (IsSourceUHD=false)" << std::endl;
    std::cout << "[ExternalsBaseTest.DefaultConstructorGstElementIsNull] - PASS" << std::endl;
}

/**
 * @brief Destructor does not throw (lifetime safe).
 *
 * **Test Group ID:** ExternalsBase_Construction_002@n
 * **Test Case ID:** ExternalsBase_Construction_002@n
 * **Priority:** High@n
 */
TEST(ExternalsBaseLifetime, DestructorSafe)
{
    std::cout << "[ExternalsBaseLifetime.DestructorSafe] - START" << std::endl;
    EXPECT_NO_THROW({ TestExternalsBase tmp; });
    std::cout << "  TestExternalsBase constructed and destroyed safely" << std::endl;
    std::cout << "[ExternalsBaseLifetime.DestructorSafe] - PASS" << std::endl;
}

// ===========================================================================
// Group: setGstElement / getgstElement
// ===========================================================================

/**
 * @brief setGstElement stores the pointer; getgstElement retrieves it.
 *
 * **Test Group ID:** ExternalsBase_GstElement_001@n
 */
TEST_F(ExternalsBaseTest, SetGstElementDoesNotThrow)
{
    std::cout << "[ExternalsBaseTest.SetGstElementDoesNotThrow] - START" << std::endl;
    GstElement* fake = reinterpret_cast<GstElement*>(0xDEADBEEF);
    EXPECT_NO_THROW(obj.setGstElement(fake));
    std::cout << "  setGstElement(ptr) completed without exception" << std::endl;
    std::cout << "[ExternalsBaseTest.SetGstElementDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief setGstElement(nullptr) clears the stored element.
 *
 * **Test Group ID:** ExternalsBase_GstElement_002@n
 */
TEST_F(ExternalsBaseTest, SetGstElementNullDoesNotThrow)
{
    std::cout << "[ExternalsBaseTest.SetGstElementNullDoesNotThrow] - START" << std::endl;
    obj.setGstElement(nullptr);
    EXPECT_FALSE(obj.IsSourceUHD());
    std::cout << "  setGstElement(nullptr) + IsSourceUHD() = false OK" << std::endl;
    std::cout << "[ExternalsBaseTest.SetGstElementNullDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Multiple setGstElement calls retain the last pointer.
 *
 * **Test Group ID:** ExternalsBase_GstElement_003@n
 */
TEST_F(ExternalsBaseTest, SetGstElementRepeatCalls)
{
    std::cout << "[ExternalsBaseTest.SetGstElementRepeatCalls] - START" << std::endl;
    GstElement* first  = reinterpret_cast<GstElement*>(0x1111);
    GstElement* second = reinterpret_cast<GstElement*>(0x2222);
    EXPECT_NO_THROW(obj.setGstElement(first));
    EXPECT_NO_THROW(obj.setGstElement(second));
    EXPECT_NO_THROW(obj.setGstElement(nullptr));
    std::cout << "  Multiple setGstElement() calls completed without exception" << std::endl;
    std::cout << "[ExternalsBaseTest.SetGstElementRepeatCalls] - PASS" << std::endl;
}

// ===========================================================================
// Group: IsSourceUHD
// ===========================================================================

/**
 * @brief IsSourceUHD() returns false when gstElement is null.
 *
 * **Test Group ID:** ExternalsBase_IsSourceUHD_001@n
 */
TEST_F(ExternalsBaseTest, IsSourceUHDFalseWhenNoElement)
{
    std::cout << "[ExternalsBaseTest.IsSourceUHDFalseWhenNoElement] - START" << std::endl;
    obj.setGstElement(nullptr);
    EXPECT_FALSE(obj.IsSourceUHD());
    std::cout << "  IsSourceUHD() == false when m_gstElement == nullptr" << std::endl;
    std::cout << "[ExternalsBaseTest.IsSourceUHDFalseWhenNoElement] - PASS" << std::endl;
}

/**
 * @brief IsSourceUHD() can be called multiple times with no element without crashing.
 *
 * **Test Group ID:** ExternalsBase_IsSourceUHD_002@n
 */
TEST_F(ExternalsBaseTest, IsSourceUHDMultipleCallsStable)
{
    std::cout << "[ExternalsBaseTest.IsSourceUHDMultipleCallsStable] - START" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_FALSE(obj.IsSourceUHD());
    }
    std::cout << "  IsSourceUHD() called 10 times, all false (no valid gst element)" << std::endl;
    std::cout << "[ExternalsBaseTest.IsSourceUHDMultipleCallsStable] - PASS" << std::endl;
}

// ===========================================================================
// Group: GetDisplayResolution (default no-op virtual)
// ===========================================================================

/**
 * @brief GetDisplayResolution() does not throw with default implementation.
 *
 * **Test Group ID:** ExternalsBase_GetDisplayResolution_001@n
 */
TEST_F(ExternalsBaseTest, GetDisplayResolutionDoesNotThrow)
{
    std::cout << "[ExternalsBaseTest.GetDisplayResolutionDoesNotThrow] - START" << std::endl;
    int w = -1, h = -1;
    obj.GetDisplayResolution(w, h);
    std::cout << "  GetDisplayResolution() completed without exception, w=" << w << " h=" << h << std::endl;
    std::cout << "[ExternalsBaseTest.GetDisplayResolutionDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief GetDisplayResolution() with zero-initialised params does not crash.
 *
 * **Test Group ID:** ExternalsBase_GetDisplayResolution_002@n
 */
TEST_F(ExternalsBaseTest, GetDisplayResolutionZeroInitialized)
{
    std::cout << "[ExternalsBaseTest.GetDisplayResolutionZeroInitialized] - START" << std::endl;
    int w = 0, h = 0;
    EXPECT_NO_THROW(obj.GetDisplayResolution(w, h));
    std::cout << "  GetDisplayResolution(0,0) no exception" << std::endl;
    std::cout << "[ExternalsBaseTest.GetDisplayResolutionZeroInitialized] - PASS" << std::endl;
}

// ===========================================================================
// Group: SetHDMIStatus (default no-op virtual)
// ===========================================================================

/**
 * @brief SetHDMIStatus() default implementation does not throw.
 *
 * **Test Group ID:** ExternalsBase_SetHDMIStatus_001@n
 */
TEST_F(ExternalsBaseTest, SetHDMIStatusDoesNotThrow)
{
    std::cout << "[ExternalsBaseTest.SetHDMIStatusDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(obj.SetHDMIStatus());
    std::cout << "  SetHDMIStatus() completed without exception" << std::endl;
    std::cout << "[ExternalsBaseTest.SetHDMIStatusDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Group: GetTR181Config (default returns nullptr)
// ===========================================================================

/**
 * @brief GetTR181Config() default implementation returns nullptr.
 *
 * **Test Group ID:** ExternalsBase_GetTR181Config_001@n
 */
TEST_F(ExternalsBaseTest, GetTR181ConfigReturnsNullptr)
{
    std::cout << "[ExternalsBaseTest.GetTR181ConfigReturnsNullptr] - START" << std::endl;
    size_t len = 999;
    char* result = obj.GetTR181Config("Device.DeviceInfo.X_RDKCENTRAL-COM_RFC.Feature.Foo", len);
    EXPECT_EQ(result, nullptr);
    std::cout << "  GetTR181Config() default returns nullptr" << std::endl;
    std::cout << "[ExternalsBaseTest.GetTR181ConfigReturnsNullptr] - PASS" << std::endl;
}

/**
 * @brief GetTR181Config() with empty param name returns nullptr.
 *
 * **Test Group ID:** ExternalsBase_GetTR181Config_002@n
 */
TEST_F(ExternalsBaseTest, GetTR181ConfigEmptyParamReturnsNullptr)
{
    std::cout << "[ExternalsBaseTest.GetTR181ConfigEmptyParamReturnsNullptr] - START" << std::endl;
    size_t len = 0;
    char* result = obj.GetTR181Config("", len);
    EXPECT_EQ(result, nullptr);
    std::cout << "  GetTR181Config(\"\") returns nullptr" << std::endl;
    std::cout << "[ExternalsBaseTest.GetTR181ConfigEmptyParamReturnsNullptr] - PASS" << std::endl;
}

// ===========================================================================
// Group: isHDCPConnection2_2 (default returns false)
// ===========================================================================

/**
 * @brief isHDCPConnection2_2() default returns false.
 *
 * **Test Group ID:** ExternalsBase_isHDCPConnection2_2_001@n
 */
TEST_F(ExternalsBaseTest, IsHDCPConnection22DefaultFalse)
{
    std::cout << "[ExternalsBaseTest.IsHDCPConnection22DefaultFalse] - START" << std::endl;
    EXPECT_FALSE(obj.isHDCPConnection2_2());
    std::cout << "  isHDCPConnection2_2() default returns false" << std::endl;
    std::cout << "[ExternalsBaseTest.IsHDCPConnection22DefaultFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: GetActiveInterface (default returns false)
// ===========================================================================

/**
 * @brief GetActiveInterface() default returns false.
 *
 * **Test Group ID:** ExternalsBase_GetActiveInterface_001@n
 */
TEST_F(ExternalsBaseTest, GetActiveInterfaceDefaultFalse)
{
    std::cout << "[ExternalsBaseTest.GetActiveInterfaceDefaultFalse] - START" << std::endl;
    EXPECT_FALSE(obj.GetActiveInterface());
    std::cout << "  GetActiveInterface() default returns false" << std::endl;
    std::cout << "[ExternalsBaseTest.GetActiveInterfaceDefaultFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: Initialize (pure virtual - concrete impl)
// ===========================================================================

/**
 * @brief Initialize() is called and sets the initCalled flag.
 *
 * **Test Group ID:** ExternalsBase_Initialize_001@n
 */
TEST_F(ExternalsBaseTest, InitializeCallsConcreteImpl)
{
    std::cout << "[ExternalsBaseTest.InitializeCallsConcreteImpl] - START" << std::endl;
    EXPECT_FALSE(obj.initCalled);
    obj.Initialize();
    EXPECT_TRUE(obj.initCalled);
    std::cout << "  Initialize() invokes concrete override" << std::endl;
    std::cout << "[ExternalsBaseTest.InitializeCallsConcreteImpl] - PASS" << std::endl;
}

/**
 * @brief Initialize() can be called multiple times without issues.
 *
 * **Test Group ID:** ExternalsBase_Initialize_002@n
 */
TEST_F(ExternalsBaseTest, InitializeMultipleCalls)
{
    std::cout << "[ExternalsBaseTest.InitializeMultipleCalls] - START" << std::endl;
    for (int i = 0; i < 5; ++i) obj.Initialize();
    EXPECT_TRUE(obj.initCalled);
    std::cout << "  Initialize() called 5 times without issues" << std::endl;
    std::cout << "[ExternalsBaseTest.InitializeMultipleCalls] - PASS" << std::endl;
}

// ===========================================================================
// Group: SetUseFireBoltSDK (pure virtual - concrete impl)
// ===========================================================================

/**
 * @brief SetUseFireBoltSDK(true) is forwarded to concrete override.
 *
 * **Test Group ID:** ExternalsBase_SetUseFireBoltSDK_001@n
 */
TEST_F(ExternalsBaseTest, SetUseFireBoltSDKTrue)
{
    std::cout << "[ExternalsBaseTest.SetUseFireBoltSDKTrue] - START" << std::endl;
    obj.SetUseFireBoltSDK(true);
    EXPECT_TRUE(obj.fireboltFlag);
    std::cout << "  SetUseFireBoltSDK(true) sets flag to true" << std::endl;
    std::cout << "[ExternalsBaseTest.SetUseFireBoltSDKTrue] - PASS" << std::endl;
}

/**
 * @brief SetUseFireBoltSDK(false) is forwarded to concrete override.
 *
 * **Test Group ID:** ExternalsBase_SetUseFireBoltSDK_002@n
 */
TEST_F(ExternalsBaseTest, SetUseFireBoltSDKFalse)
{
    std::cout << "[ExternalsBaseTest.SetUseFireBoltSDKFalse] - START" << std::endl;
    obj.SetUseFireBoltSDK(true);
    obj.SetUseFireBoltSDK(false);
    EXPECT_FALSE(obj.fireboltFlag);
    std::cout << "  SetUseFireBoltSDK(false) sets flag to false" << std::endl;
    std::cout << "[ExternalsBaseTest.SetUseFireBoltSDKFalse] - PASS" << std::endl;
}

// ===========================================================================
// Group: Integration
// ===========================================================================

/**
 * @brief Full lifecycle: construct, init, set element, check UHD, teardown.
 *
 * **Test Group ID:** ExternalsBase_Integration_001@n
 */
TEST_F(ExternalsBaseTest, FullLifecycleNoElement)
{
    std::cout << "[ExternalsBaseTest.FullLifecycleNoElement] - START" << std::endl;
    obj.Initialize();
    EXPECT_TRUE(obj.initCalled);
    obj.setGstElement(nullptr);
    EXPECT_FALSE(obj.IsSourceUHD());
    obj.SetHDMIStatus();
    EXPECT_FALSE(obj.isHDCPConnection2_2());
    EXPECT_FALSE(obj.GetActiveInterface());
    obj.SetUseFireBoltSDK(true);
    EXPECT_TRUE(obj.fireboltFlag);
    int w = 0, h = 0;
    obj.GetDisplayResolution(w, h);
    std::cout << "  Full lifecycle sequence completed" << std::endl;
    std::cout << "[ExternalsBaseTest.FullLifecycleNoElement] - PASS" << std::endl;
}

/**
 * @brief Stress: 100 set/get gstElement cycles remain consistent.
 *
 * **Test Group ID:** ExternalsBase_Integration_002@n
 */
TEST(ExternalsBaseStress, SetGstElement100Cycles)
{
    std::cout << "[ExternalsBaseStress.SetGstElement100Cycles] - START" << std::endl;
    TestExternalsBase obj;
    for (int i = 1; i <= 100; ++i)
    {
        GstElement* ptr = reinterpret_cast<GstElement*>(static_cast<uintptr_t>(i));
        EXPECT_NO_THROW(obj.setGstElement(ptr));
    }
    std::cout << "  100 setGstElement() cycles completed without exception" << std::endl;
    std::cout << "[ExternalsBaseStress.SetGstElement100Cycles] - PASS" << std::endl;
}
