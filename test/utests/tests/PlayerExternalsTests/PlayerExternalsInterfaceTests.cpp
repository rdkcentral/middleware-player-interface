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
 * @file PlayerExternalsInterfaceTests.cpp
 * @brief Comprehensive unit tests for FakePlayerExternalsInterface and
 *        PlayerExternalsInterface singleton.
 *
 * In test builds IARM_MGR is NOT defined, so PlayerExternalsInterface uses
 * FakePlayerExternalsInterface internally for all operations.
 * UBUNTU is defined, so createInstance() static method is available.
 *
 * APIs covered (FakePlayerExternalsInterface):
 *   Constructor, Initialize, SetHDMIStatus, isHDCPConnection2_2,
 *   GetActiveInterface, GetTR181Config, GetDisplayResolution, setGstElement,
 *   getGstElement, setSourceWidth/Height, setDisplayWidth/Height,
 *   setHDCPEnabled, getSourceWidth/Height, getDisplayWidth/Height,
 *   isHDCPEnabled, setGstElementFake, IsSourceUHD, SetUseFireBoltSDK.
 *
 * APIs covered (PlayerExternalsInterface singleton):
 *   GetPlayerExternalsInterfaceInstance, IsPlayerExternalsInterfaceInstanceActive,
 *   createInstance, Initialize, isHDCPConnection2_2, IsSourceUHD,
 *   GetDisplayResolution, setGstElement, GetActiveInterface,
 *   IsConfigWifiCurlHeader, GetTR181PlayerConfig, SetUseFireBoltSDK.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include "PlayerExternalsInterface.h"

// ===========================================================================
// FakePlayerExternalsInterface tests
// ===========================================================================

class FakeExternalsInterfaceTest : public ::testing::Test
{
protected:
    FakePlayerExternalsInterface fake;
};

/**
 * @brief Constructor initialises HDCP to version 1x (enabled).
 *
 * **Test Group ID:** FakeExternals_Construction_001@n
 */
TEST_F(FakeExternalsInterfaceTest, ConstructorSetHDMIStatus)
{
    std::cout << "[FakeExternalsInterfaceTest.ConstructorSetHDMIStatus] - START" << std::endl;
    // SetHDMIStatus() is called from constructor: sets protocol to 1X, enabled=true
    EXPECT_TRUE(fake.isHDCPEnabled());
    std::cout << "  Constructor called SetHDMIStatus: HDCP enabled=true" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.ConstructorSetHDMIStatus] - PASS" << std::endl;
}

/**
 * @brief Initialize() does not throw.
 *
 * **Test Group ID:** FakeExternals_Initialize_001@n
 */
TEST_F(FakeExternalsInterfaceTest, InitializeDoesNotThrow)
{
    std::cout << "[FakeExternalsInterfaceTest.InitializeDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(fake.Initialize());
    std::cout << "  Initialize() completed without exception" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.InitializeDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief SetHDMIStatus() sets HDCP enabled=true and protocol=1X.
 *
 * **Test Group ID:** FakeExternals_SetHDMIStatus_001@n
 */
TEST_F(FakeExternalsInterfaceTest, SetHDMIStatusSetsHDCPEnabled)
{
    std::cout << "[FakeExternalsInterfaceTest.SetHDMIStatusSetsHDCPEnabled] - START" << std::endl;
    fake.setHDCPEnabled(false);
    EXPECT_FALSE(fake.isHDCPEnabled());
    fake.SetHDMIStatus();
    EXPECT_TRUE(fake.isHDCPEnabled());
    std::cout << "  SetHDMIStatus() restored HDCP enabled=true" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetHDMIStatusSetsHDCPEnabled] - PASS" << std::endl;
}

/**
 * @brief isHDCPConnection2_2() returns false when protocol is 1X (set by constructor).
 *
 * **Test Group ID:** FakeExternals_isHDCPConnection2_2_001@n
 */
TEST_F(FakeExternalsInterfaceTest, IsHDCPConnection22FalseWithVersion1X)
{
    std::cout << "[FakeExternalsInterfaceTest.IsHDCPConnection22FalseWithVersion1X] - START" << std::endl;
    // After constructor, protocol is PLAYER_dsHDCP_VERSION_1X (14), not 2X (22)
    EXPECT_FALSE(fake.isHDCPConnection2_2());
    std::cout << "  isHDCPConnection2_2() = false with protocol 1X" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.IsHDCPConnection22FalseWithVersion1X] - PASS" << std::endl;
}

/**
 * @brief GetActiveInterface() always returns false (fake).
 *
 * **Test Group ID:** FakeExternals_GetActiveInterface_001@n
 */
TEST_F(FakeExternalsInterfaceTest, GetActiveInterfaceFalse)
{
    std::cout << "[FakeExternalsInterfaceTest.GetActiveInterfaceFalse] - START" << std::endl;
    EXPECT_FALSE(fake.GetActiveInterface());
    std::cout << "  GetActiveInterface() = false" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.GetActiveInterfaceFalse] - PASS" << std::endl;
}

/**
 * @brief GetTR181Config() always returns nullptr (fake).
 *
 * **Test Group ID:** FakeExternals_GetTR181Config_001@n
 */
TEST_F(FakeExternalsInterfaceTest, GetTR181ConfigReturnsNullptr)
{
    std::cout << "[FakeExternalsInterfaceTest.GetTR181ConfigReturnsNullptr] - START" << std::endl;
    size_t len = 999;
    char* result = fake.GetTR181Config("Device.X_RDKCENTRAL", len);
    EXPECT_EQ(result, nullptr);
    std::cout << "  GetTR181Config() = nullptr" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.GetTR181ConfigReturnsNullptr] - PASS" << std::endl;
}

/**
 * @brief GetDisplayResolution() override is a no-op (does not modify params).
 *
 * **Test Group ID:** FakeExternals_GetDisplayResolution_001@n
 */
TEST_F(FakeExternalsInterfaceTest, GetDisplayResolutionNoOp)
{
    std::cout << "[FakeExternalsInterfaceTest.GetDisplayResolutionNoOp] - START" << std::endl;
    int w = 1920, h = 1080;
    fake.GetDisplayResolution(w, h);
    // No-op override: values unchanged
    EXPECT_EQ(w, 1920);
    EXPECT_EQ(h, 1080);
    std::cout << "  GetDisplayResolution() is a no-op on FakePlayerExternalsInterface" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.GetDisplayResolutionNoOp] - PASS" << std::endl;
}

/**
 * @brief setGstElement / getgstElement round-trip works via base class.
 *
 * **Test Group ID:** FakeExternals_GstElement_001@n
 */
TEST_F(FakeExternalsInterfaceTest, SetGetGstElementBaseRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.SetGetGstElementBaseRoundTrip] - START" << std::endl;
    GstElement* ptr = reinterpret_cast<GstElement*>(0xABCD);
    fake.setGstElement(ptr);
    EXPECT_EQ(fake.getGstElement(), ptr);
    std::cout << "  setGstElement/getGstElement base round-trip OK" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetGetGstElementBaseRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setGstElementFake / getGstElement round-trip works.
 *
 * **Test Group ID:** FakeExternals_GstElement_002@n
 */
TEST_F(FakeExternalsInterfaceTest, SetGetGstElementFakeRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.SetGetGstElementFakeRoundTrip] - START" << std::endl;
    GstElement* ptr = reinterpret_cast<GstElement*>(0x5678);
    fake.setGstElementFake(ptr);
    EXPECT_EQ(fake.getGstElement(), ptr);
    std::cout << "  setGstElementFake/getGstElement round-trip OK" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetGetGstElementFakeRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setSourceWidth / getSourceWidth round-trip.
 *
 * **Test Group ID:** FakeExternals_SourceDims_001@n
 */
TEST_F(FakeExternalsInterfaceTest, SourceWidthRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.SourceWidthRoundTrip] - START" << std::endl;
    fake.setSourceWidth(3840);
    EXPECT_EQ(fake.getSourceWidth(), 3840);
    std::cout << "  setSourceWidth(3840) / getSourceWidth() == 3840" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SourceWidthRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setSourceHeight / getSourceHeight round-trip.
 *
 * **Test Group ID:** FakeExternals_SourceDims_002@n
 */
TEST_F(FakeExternalsInterfaceTest, SourceHeightRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.SourceHeightRoundTrip] - START" << std::endl;
    fake.setSourceHeight(2160);
    EXPECT_EQ(fake.getSourceHeight(), 2160);
    std::cout << "  setSourceHeight(2160) / getSourceHeight() == 2160" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SourceHeightRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setDisplayWidth / getDisplayWidth round-trip.
 *
 * **Test Group ID:** FakeExternals_DisplayDims_001@n
 */
TEST_F(FakeExternalsInterfaceTest, DisplayWidthRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.DisplayWidthRoundTrip] - START" << std::endl;
    fake.setDisplayWidth(1920);
    EXPECT_EQ(fake.getDisplayWidth(), 1920);
    std::cout << "  setDisplayWidth(1920) / getDisplayWidth() == 1920" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.DisplayWidthRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setDisplayHeight / getDisplayHeight round-trip.
 *
 * **Test Group ID:** FakeExternals_DisplayDims_002@n
 */
TEST_F(FakeExternalsInterfaceTest, DisplayHeightRoundTrip)
{
    std::cout << "[FakeExternalsInterfaceTest.DisplayHeightRoundTrip] - START" << std::endl;
    fake.setDisplayHeight(1080);
    EXPECT_EQ(fake.getDisplayHeight(), 1080);
    std::cout << "  setDisplayHeight(1080) / getDisplayHeight() == 1080" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.DisplayHeightRoundTrip] - PASS" << std::endl;
}

/**
 * @brief setHDCPEnabled(true) / isHDCPEnabled() round-trip.
 *
 * **Test Group ID:** FakeExternals_HDCPEnabled_001@n
 */
TEST_F(FakeExternalsInterfaceTest, SetHDCPEnabledTrue)
{
    std::cout << "[FakeExternalsInterfaceTest.SetHDCPEnabledTrue] - START" << std::endl;
    fake.setHDCPEnabled(true);
    EXPECT_TRUE(fake.isHDCPEnabled());
    std::cout << "  setHDCPEnabled(true) / isHDCPEnabled() == true" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetHDCPEnabledTrue] - PASS" << std::endl;
}

/**
 * @brief setHDCPEnabled(false) / isHDCPEnabled() round-trip.
 *
 * **Test Group ID:** FakeExternals_HDCPEnabled_002@n
 */
TEST_F(FakeExternalsInterfaceTest, SetHDCPEnabledFalse)
{
    std::cout << "[FakeExternalsInterfaceTest.SetHDCPEnabledFalse] - START" << std::endl;
    fake.setHDCPEnabled(false);
    EXPECT_FALSE(fake.isHDCPEnabled());
    std::cout << "  setHDCPEnabled(false) / isHDCPEnabled() == false" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetHDCPEnabledFalse] - PASS" << std::endl;
}

/**
 * @brief IsSourceUHD() false when no gstElement set.
 *
 * **Test Group ID:** FakeExternals_IsSourceUHD_001@n
 */
TEST_F(FakeExternalsInterfaceTest, IsSourceUHDFalseNoElement)
{
    std::cout << "[FakeExternalsInterfaceTest.IsSourceUHDFalseNoElement] - START" << std::endl;
    fake.setGstElement(nullptr);
    EXPECT_FALSE(fake.IsSourceUHD());
    std::cout << "  IsSourceUHD() = false (no gstElement)" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.IsSourceUHDFalseNoElement] - PASS" << std::endl;
}

/**
 * @brief SetUseFireBoltSDK() does not throw (no-op override).
 *
 * **Test Group ID:** FakeExternals_SetUseFireBoltSDK_001@n
 */
TEST_F(FakeExternalsInterfaceTest, SetUseFireBoltSDKDoesNotThrow)
{
    std::cout << "[FakeExternalsInterfaceTest.SetUseFireBoltSDKDoesNotThrow] - START" << std::endl;
    EXPECT_NO_THROW(fake.SetUseFireBoltSDK(true));
    EXPECT_NO_THROW(fake.SetUseFireBoltSDK(false));
    std::cout << "  SetUseFireBoltSDK(true/false) completed without exception" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SetUseFireBoltSDKDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Source dims set to UHD values and IsSourceUHD still false (no element).
 *
 * **Test Group ID:** FakeExternals_SourceDims_003@n
 */
TEST_F(FakeExternalsInterfaceTest, SourceDimsUHDButNoElement)
{
    std::cout << "[FakeExternalsInterfaceTest.SourceDimsUHDButNoElement] - START" << std::endl;
    fake.setSourceWidth(3840);
    fake.setSourceHeight(2160);
    fake.setGstElement(nullptr);
    EXPECT_FALSE(fake.IsSourceUHD()); // element is null, so always false
    std::cout << "  IsSourceUHD() = false even with UHD dims set, because gstElement == nullptr" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SourceDimsUHDButNoElement] - PASS" << std::endl;
}

/**
 * @brief Source dims set to zero and IsSourceUHD is false.
 *
 * **Test Group ID:** FakeExternals_SourceDims_004@n
 */
TEST_F(FakeExternalsInterfaceTest, SourceDimsZero)
{
    std::cout << "[FakeExternalsInterfaceTest.SourceDimsZero] - START" << std::endl;
    fake.setSourceWidth(0);
    fake.setSourceHeight(0);
    EXPECT_EQ(fake.getSourceWidth(), 0);
    EXPECT_EQ(fake.getSourceHeight(), 0);
    std::cout << "  Source width=0, height=0 stored correctly" << std::endl;
    std::cout << "[FakeExternalsInterfaceTest.SourceDimsZero] - PASS" << std::endl;
}

// ===========================================================================
// PlayerExternalsInterface singleton tests
// ===========================================================================

class PlayerExternalsInterfaceTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @brief GetPlayerExternalsInterfaceInstance() returns non-null.
 *
 * **Test Group ID:** PlayerExternals_Singleton_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetInstanceNonNull)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetInstanceNonNull] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_NE(inst, nullptr);
    std::cout << "  GetPlayerExternalsInterfaceInstance() != nullptr" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetInstanceNonNull] - PASS" << std::endl;
}

/**
 * @brief GetPlayerExternalsInterfaceInstance() is a singleton (same pointer).
 *
 * **Test Group ID:** PlayerExternals_Singleton_002@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetInstanceIsSingleton)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetInstanceIsSingleton] - START" << std::endl;
    auto a = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    auto b = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_EQ(a.get(), b.get());
    std::cout << "  Two calls return the same singleton pointer" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetInstanceIsSingleton] - PASS" << std::endl;
}

/**
 * @brief IsPlayerExternalsInterfaceInstanceActive() returns true after GetInstance.
 *
 * **Test Group ID:** PlayerExternals_Singleton_003@n
 */
TEST_F(PlayerExternalsInterfaceTest, IsInstanceActiveAfterGet)
{
    std::cout << "[PlayerExternalsInterfaceTest.IsInstanceActiveAfterGet] - START" << std::endl;
    PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_TRUE(PlayerExternalsInterface::IsPlayerExternalsInterfaceInstanceActive());
    std::cout << "  IsPlayerExternalsInterfaceInstanceActive() = true after GetInstance" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.IsInstanceActiveAfterGet] - PASS" << std::endl;
}

/**
 * @brief createInstance() (UBUNTU-only) returns a non-null independent instance.
 *
 * **Test Group ID:** PlayerExternals_CreateInstance_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, CreateInstanceNonNull)
{
    std::cout << "[PlayerExternalsInterfaceTest.CreateInstanceNonNull] - START" << std::endl;
    auto inst = PlayerExternalsInterface::createInstance();
    EXPECT_NE(inst, nullptr);
    std::cout << "  createInstance() != nullptr" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.CreateInstanceNonNull] - PASS" << std::endl;
}

/**
 * @brief isHDCPConnection2_2() does not throw on singleton (fake uses 1X).
 *
 * **Test Group ID:** PlayerExternals_isHDCPConnection2_2_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, IsHDCPConnection22DoesNotThrow)
{
    std::cout << "[PlayerExternalsInterfaceTest.IsHDCPConnection22DoesNotThrow] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    bool result = inst->isHDCPConnection2_2();
    EXPECT_FALSE(result); // FakePlayerExternalsInterface uses 1X
    std::cout << "  isHDCPConnection2_2() = " << result << " (false, protocol 1X)" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.IsHDCPConnection22DoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief IsSourceUHD() returns false (fake has no valid gst element).
 *
 * **Test Group ID:** PlayerExternals_IsSourceUHD_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, IsSourceUHDReturnsFalse)
{
    std::cout << "[PlayerExternalsInterfaceTest.IsSourceUHDReturnsFalse] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_FALSE(inst->IsSourceUHD());
    std::cout << "  IsSourceUHD() = false" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.IsSourceUHDReturnsFalse] - PASS" << std::endl;
}

/**
 * @brief GetDisplayResolution() does not throw.
 *
 * **Test Group ID:** PlayerExternals_GetDisplayResolution_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetDisplayResolutionDoesNotThrow)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetDisplayResolutionDoesNotThrow] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    int w = -1, h = -1;
    inst->GetDisplayResolution(w, h);
    std::cout << "  GetDisplayResolution() completed, w=" << w << " h=" << h << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetDisplayResolutionDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief setGstElement() does not throw on singleton.
 *
 * **Test Group ID:** PlayerExternals_setGstElement_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, SetGstElementDoesNotThrow)
{
    std::cout << "[PlayerExternalsInterfaceTest.SetGstElementDoesNotThrow] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_NO_THROW(inst->setGstElement(nullptr));
    std::cout << "  setGstElement(nullptr) completed without exception" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.SetGstElementDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief GetActiveInterface() returns false (fake).
 *
 * **Test Group ID:** PlayerExternals_GetActiveInterface_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetActiveInterfaceFalse)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetActiveInterfaceFalse] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_FALSE(inst->GetActiveInterface());
    std::cout << "  GetActiveInterface() = false" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetActiveInterfaceFalse] - PASS" << std::endl;
}

/**
 * @brief IsConfigWifiCurlHeader() returns false (UBUNTU build, no IARM).
 *
 * **Test Group ID:** PlayerExternals_IsConfigWifiCurlHeader_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, IsConfigWifiCurlHeaderFalse)
{
    std::cout << "[PlayerExternalsInterfaceTest.IsConfigWifiCurlHeaderFalse] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_FALSE(inst->IsConfigWifiCurlHeader());
    std::cout << "  IsConfigWifiCurlHeader() = false (no IARM in UBUNTU build)" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.IsConfigWifiCurlHeaderFalse] - PASS" << std::endl;
}

/**
 * @brief GetTR181PlayerConfig() returns nullptr (fake impl).
 *
 * **Test Group ID:** PlayerExternals_GetTR181PlayerConfig_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetTR181PlayerConfigReturnsNullptr)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetTR181PlayerConfigReturnsNullptr] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    size_t len = 0;
    char* result = inst->GetTR181PlayerConfig("Device.DeviceInfo.SomeParam", len);
    EXPECT_EQ(result, nullptr);
    std::cout << "  GetTR181PlayerConfig() = nullptr" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetTR181PlayerConfigReturnsNullptr] - PASS" << std::endl;
}

/**
 * @brief GetTR181PlayerConfig() with empty param returns nullptr.
 *
 * **Test Group ID:** PlayerExternals_GetTR181PlayerConfig_002@n
 */
TEST_F(PlayerExternalsInterfaceTest, GetTR181PlayerConfigEmptyParamNullptr)
{
    std::cout << "[PlayerExternalsInterfaceTest.GetTR181PlayerConfigEmptyParamNullptr] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    size_t len = 0;
    char* result = inst->GetTR181PlayerConfig("", len);
    EXPECT_EQ(result, nullptr);
    std::cout << "  GetTR181PlayerConfig(\"\") = nullptr" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.GetTR181PlayerConfigEmptyParamNullptr] - PASS" << std::endl;
}

/**
 * @brief SetUseFireBoltSDK() does not throw.
 *
 * **Test Group ID:** PlayerExternals_SetUseFireBoltSDK_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, SetUseFireBoltSDKDoesNotThrow)
{
    std::cout << "[PlayerExternalsInterfaceTest.SetUseFireBoltSDKDoesNotThrow] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_NO_THROW(inst->SetUseFireBoltSDK(true));
    EXPECT_NO_THROW(inst->SetUseFireBoltSDK(false));
    std::cout << "  SetUseFireBoltSDK(true/false) completed without exception" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.SetUseFireBoltSDKDoesNotThrow] - PASS" << std::endl;
}

/**
 * @brief Initialize() does not throw on singleton.
 *
 * **Test Group ID:** PlayerExternals_Initialize_001@n
 */
TEST_F(PlayerExternalsInterfaceTest, InitializeDoesNotThrow)
{
    std::cout << "[PlayerExternalsInterfaceTest.InitializeDoesNotThrow] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    EXPECT_NO_THROW(inst->Initialize());
    std::cout << "  Initialize() completed without exception" << std::endl;
    std::cout << "[PlayerExternalsInterfaceTest.InitializeDoesNotThrow] - PASS" << std::endl;
}

// ===========================================================================
// Integration
// ===========================================================================

/**
 * @brief Full singleton API sequence without throwing.
 *
 * **Test Group ID:** PlayerExternals_Integration_001@n
 */
TEST(PlayerExternalsIntegration, FullAPISequence)
{
    std::cout << "[PlayerExternalsIntegration.FullAPISequence] - START" << std::endl;
    auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    ASSERT_NE(inst, nullptr);
    inst->Initialize();
    inst->setGstElement(nullptr);
    EXPECT_FALSE(inst->IsSourceUHD());
    EXPECT_FALSE(inst->isHDCPConnection2_2());
    EXPECT_FALSE(inst->GetActiveInterface());
    EXPECT_FALSE(inst->IsConfigWifiCurlHeader());
    inst->SetUseFireBoltSDK(true);
    inst->SetUseFireBoltSDK(false);
    int w = 0, h = 0;
    inst->GetDisplayResolution(w, h);
    size_t len = 0;
    char* cfg = inst->GetTR181PlayerConfig("Param", len);
    EXPECT_EQ(cfg, nullptr);
    std::cout << "  Full singleton API sequence completed" << std::endl;
    std::cout << "[PlayerExternalsIntegration.FullAPISequence] - PASS" << std::endl;
}

/**
 * @brief Stress: 20 GetInstance calls all return same pointer.
 *
 * **Test Group ID:** PlayerExternals_Integration_002@n
 */
TEST(PlayerExternalsIntegration, StressGetInstance20Calls)
{
    std::cout << "[PlayerExternalsIntegration.StressGetInstance20Calls] - START" << std::endl;
    auto first = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
    for (int i = 0; i < 20; ++i)
    {
        auto inst = PlayerExternalsInterface::GetPlayerExternalsInterfaceInstance();
        EXPECT_EQ(inst.get(), first.get());
    }
    std::cout << "  20 GetInstance calls all returned same singleton" << std::endl;
    std::cout << "[PlayerExternalsIntegration.StressGetInstance20Calls] - PASS" << std::endl;
}

/**
 * @brief FakePlayerExternalsInterface full lifecycle test.
 *
 * **Test Group ID:** PlayerExternals_Integration_003@n
 */
TEST(PlayerExternalsIntegration, FakeFullLifecycle)
{
    std::cout << "[PlayerExternalsIntegration.FakeFullLifecycle] - START" << std::endl;
    FakePlayerExternalsInterface fake;
    fake.Initialize();
    fake.setSourceWidth(1920);
    fake.setSourceHeight(1080);
    fake.setDisplayWidth(1920);
    fake.setDisplayHeight(1080);
    fake.setHDCPEnabled(true);
    EXPECT_TRUE(fake.isHDCPEnabled());
    fake.SetHDMIStatus();
    EXPECT_FALSE(fake.isHDCPConnection2_2());
    fake.SetUseFireBoltSDK(true);
    size_t l = 0;
    EXPECT_EQ(fake.GetTR181Config("param", l), nullptr);
    EXPECT_FALSE(fake.GetActiveInterface());
    int w = 0, h = 0;
    fake.GetDisplayResolution(w, h);
    std::cout << "  FakePlayerExternalsInterface full lifecycle completed" << std::endl;
    std::cout << "[PlayerExternalsIntegration.FakeFullLifecycle] - PASS" << std::endl;
}
