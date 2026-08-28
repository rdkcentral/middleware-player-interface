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
 * @file DeviceFireboltInterface.cpp
 * @brief Firebolt device api interface
 */

/*
IARM Deprecation Note:
IARM is to be deprecated in favor of DeviceSettings and Firebolt Device API.
*/

/*
Move the entire folder externals/rdk/IFirebolt 's contents to externals/rdk/
IFirebolt folder to be deleted, as IARM is no longer available as an alternative
*/

#include "DeviceFireboltInterface.h"
#include "fireboltaamp.h"
#include "PlayerLogManager.h"
#include "PlayerExternalsRdkInterface.h"
#include "PlayerExternalUtils.h"

#include <cstring>
#include <cstdio>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <cctype>

std::shared_ptr<DeviceFireboltInterface> s_pDeviceFireboltInterface = nullptr;

std::mutex mFireboltConnectionMutex;
std::condition_variable mFireboltConnectionCV;

static void HDCPEventHandlerFirebolt(const Firebolt::Device::HDCPVersionMap& t_HDCPVersionMap);
static void ResolutionHandlerFirebolt(const std::string& t_res);
static void getActiveInterfaceEventHandlerFirebolt (const Firebolt::Device::NetworkInfoResult& t_NetworkInfoResult);

static bool TryParseResolutionFromPayload(const std::string& payload, int& width, int& height)
{
	int first = -1;
	int second = -1;
	int current = -1;

	for (char ch : payload)
	{
		if (std::isdigit(static_cast<unsigned char>(ch)))
		{
			if (current < 0)
			{
				current = 0;
			}
			current = (current * 10) + (ch - '0');
		}
		else if (current >= 0)
		{
			if (first < 0)
			{
				first = current;
			}
			else
			{
				second = current;
				break;
			}
			current = -1;
		}
	}

	if (second < 0 && current >= 0)
	{
		if (first < 0)
		{
			first = current;
		}
		else
		{
			second = current;
		}
	}

	if (first > 0 && second > 0)
	{
		width = first;
		height = second;
		return true;
	}

	return false;
}

std::shared_ptr<DeviceFireboltInterface> DeviceFireboltInterface::GetInstance()
{
    if(nullptr == s_pDeviceFireboltInterface)
    {
        s_pDeviceFireboltInterface = std::shared_ptr<DeviceFireboltInterface>(new DeviceFireboltInterface());
    }

    return s_pDeviceFireboltInterface;
}

DeviceFireboltInterface::DeviceFireboltInterface()
{
	m_pFireboltInterface = FireboltInterface::GetInstance();
	MW_LOG_WARN("[FIREBOLT] Inside constructor for fireboltinterface");
}

DeviceFireboltInterface::~DeviceFireboltInterface()
{
	MW_LOG_WARN("[FIREBOLT] DeviceFireboltInterface destructor called \n");
	RemoveEventHandlers();
	m_pFireboltInterface = nullptr;
}

void DeviceFireboltInterface::Initialize()
{
	MW_LOG_WARN("[FIREBOLT] Initialize \n");
	if(s_pDeviceFireboltInterface)
	{
		MW_PRE_LOGGER_LOG("Registering events \n");
		s_pDeviceFireboltInterface->RegisterDsMgrEventHandler();
		s_pDeviceFireboltInterface->RegisterNtwMgrEventHandler();
	}
	else
	{
		MW_LOG_WARN("[FIREBOLT] Init called before instance \n");
	}

	MW_LOG_WARN("[FIREBOLT] Initialize completed \n");
	
}


void DeviceFireboltInterface::RegisterDsMgrEventHandler()
{
    MW_LOG_WARN("[FIREBOLT] This shouldnt be called!!!!!");   
	MW_PRE_LOGGER_LOG("Subscribing to Firebolt hdcp change event \n");

	auto result =  Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().subscribeOnHdcpChanged(
					[](const auto& hdcpProtocol) {
						MW_LOG_ERR("[Event] HDCP changed");
						HDCPEventHandlerFirebolt(hdcpProtocol);
					});

	if(result)
	{
		MW_PRE_LOGGER_LOG("HDCP changed event registered \n");
		mDsMgrSubscriptionId.push_back(result.value());
	}

	else
	{
		MW_PRE_LOGGER_LOG("Failed to subscribe to hdcp change events: %d \n", static_cast<int>(result.error()));
	}

	MW_PRE_LOGGER_LOG("Subscribing to Firebolt resolution change event  \n");

	result = Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().subscribeOnVideoResolutionChanged(
					[](const std::string& videoResolution) 
					{
						MW_LOG_WARN("[Event] Video resolution changed: %s" , videoResolution.c_str());
						ResolutionHandlerFirebolt(videoResolution);
					});
	if(result)
	{
		MW_PRE_LOGGER_LOG("Resolution changed event registered\n");
        mDsMgrSubscriptionId.push_back(result.value());
	}
	else
	{
		MW_PRE_LOGGER_LOG("Failed to get video resolution %d\n", static_cast<int>(result.error()) );
    }

}

void DeviceFireboltInterface::RemoveEventHandlers()
{
	MW_LOG_WARN("[FIREBOLT] This shouldnt be called!!!!!");
	//removes everything ...
    Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().unsubscribeAll();        
}

void DeviceFireboltInterface::RegisterNtwMgrEventHandler()
{
	MW_PRE_LOGGER_LOG("Subscribing to Firebolt Network change event\n");

	auto result =  Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().subscribeOnNetworkChanged(
					[](const auto& network) {
						MW_LOG_ERR("[Event] network changed");
					    getActiveInterfaceEventHandlerFirebolt(network);
					});
	
	if(result)
	{
		MW_PRE_LOGGER_LOG("Network changed event registered\n");
		mNtwMgrSubscriptionId.push_back(result.value());
	}
	else
	{
		MW_PRE_LOGGER_LOG("Failed to subscribe to network change events: %d\n", static_cast<int>(result.error()));
		MW_LOG_ERR("Failed to subscribe to network change events: %d", static_cast<int>(result.error()));
	}

	std::shared_ptr<PlayerExternalsRdkInterface> pInstance = PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance();

	auto network = Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().network();

	if(network)
	{
		if(network.value().type == Firebolt::Device::NetworkType::WIFI)
		{
			MW_PRE_LOGGER_LOG("Active interface wifi\n");
			pInstance->SetActiveInterface(true);
		}
		else
		{
			MW_PRE_LOGGER_LOG("Active interface eth\n");
			pInstance->SetActiveInterface(false);
		}
	}

}

char * DeviceFireboltInterface::GetTR181Config(const char * paramName, size_t & iConfigLen)
{
	MW_LOG_ERR("TR181 not supported for firebolt");
    return nullptr;
}

/**
 * @brief Queries Device.hdcp and Device.videoResolution via Firebolt and updates player HDCP/resolution state.
 * Implements xrn:firebolt:capability:device:info for SetHDMIStatus on RDK-E (USE_FIREBOLT) builds.
 */
void DeviceFireboltInterface::SetHDMIStatus()
{
	MW_LOG_WARN("[FIREBOLT] SetHDMIStatus: entering via Firebolt Device API path");
	std::unique_lock<std::mutex> lock(m_hdmiStatusMutex, std::try_to_lock);
    if (!lock.owns_lock()) {
        MW_LOG_WARN("DeviceFirebolt SetHDMIStatus: Already in progress on another thread, skipping");
        return;
    }
    std::shared_ptr<PlayerExternalsRdkInterface> pInstance = PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance();

    // Query current HDCP state via Firebolt Device.hdcp (xrn:firebolt:capability:device:info)
    auto hdcpResult = Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().hdcp();
    if (hdcpResult)
    {
        const auto& hdcpMap = hdcpResult.value();
        if (hdcpMap.hdcp2_2)
        {
            pInstance->SetHDCPEnabled(true);
            pInstance->setHdcpProtocol(dsHDCP_VERSION_2X);
			MW_LOG_WARN("[FIREBOLT] SetHDMIStatus: HDCP 2.2 detected");
        }
        else if (hdcpMap.hdcp1_4)
        {
            pInstance->SetHDCPEnabled(true);
            pInstance->setHdcpProtocol(dsHDCP_VERSION_1X);
			MW_LOG_WARN("[FIREBOLT] SetHDMIStatus: HDCP 1.4 detected");
        }
        else
        {
            pInstance->SetHDCPEnabled(false);
            pInstance->setHdcpProtocol(dsHDCP_VERSION_1X);
            MW_LOG_WARN("[FIREBOLT] DeviceFirebolt SetHDMIStatus: HDCP not supported, defaulting to 1.4");
        }
    }
    else
    {
        MW_LOG_ERR("[FIREBOLT] DeviceFirebolt SetHDMIStatus: Failed to query HDCP: %d", static_cast<int>(hdcpResult.error()));
    }

    // Query current resolution via Firebolt Device.videoResolution (xrn:firebolt:capability:device:info)
    auto resolutionResult = Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().screenResolution();
    if (resolutionResult)
    {
        int width  = resolutionResult.value()[0];
        int height = resolutionResult.value()[1];
        pInstance->SetResolution(width, height);
        MW_LOG_WARN("[FIREBOLT] DeviceFirebolt SetHDMIStatus: Resolution [%d][%d]", width, height);
    }
    else
    {
        MW_LOG_ERR("[FIREBOLT] DeviceFirebolt SetHDMIStatus: Failed to query resolution: %d", static_cast<int>(resolutionResult.error()));
    }
}

static void getActiveInterfaceEventHandlerFirebolt (const Firebolt::Device::NetworkInfoResult& t_NetworkInfoResult)
{
    std::shared_ptr<PlayerExternalsRdkInterface> pInstance = PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance();

	if(t_NetworkInfoResult.state == Firebolt::Device::NetworkState::CONNECTED)
	{
		std::string interface = "unknown";
		if(t_NetworkInfoResult.type == Firebolt::Device::NetworkType::WIFI)
		{
			interface = "wlan";
			pInstance->SetActiveInterface(true);
			MW_LOG_INFO("Network interface changed to wifi");
		}
		else if(t_NetworkInfoResult.type == Firebolt::Device::NetworkType::ETHERNET)
		{
			interface = "eth";
			pInstance->SetActiveInterface(false);
			MW_LOG_INFO("Network interface changed to ethernet");
		}
		else
		{
			MW_LOG_ERR("Unsupported Interface %d", (int)t_NetworkInfoResult.type);
		}
		MW_LOG_INFO("getActiveInterfaceEventHandler activeinterface changed to %s\n", interface.c_str());
	}
	else
	{
		MW_LOG_ERR("Disconnected interface type:%d state:%d\n", (int)t_NetworkInfoResult.type, (int)t_NetworkInfoResult.state);
	}
    
	
}

/**
 * @brief IARM event handler for HDCP and HDMI hot plug events
 */
static void HDCPEventHandlerFirebolt(const Firebolt::Device::HDCPVersionMap& t_HDCPVersionMap)
{
    std::shared_ptr<PlayerExternalsRdkInterface> pInstance = PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance();

    if(t_HDCPVersionMap.hdcp2_2)
	{
		pInstance->setHdcpProtocol(dsHDCP_VERSION_2X);
		MW_LOG_INFO("HDCP protocol updated 2_2");
	}
	else if(t_HDCPVersionMap.hdcp1_4)
	{
		pInstance->setHdcpProtocol(dsHDCP_VERSION_1X);
		MW_LOG_INFO("HDCP protocol updated 1_4");
	}
	else
	{
		MW_LOG_ERR("Unknown HDCP protocol");
	}
	MW_LOG_WARN("[FIREBOLT] This sethdmistatus call should use firebolt path");
	pInstance->SetHDMIStatus();
            
}

/**
 * @brief IARM event handler for resolution changes
 */
static void ResolutionHandlerFirebolt(const std::string& t_res)
{
    int width = 1280;
	int height = 720;

	MW_LOG_WARN("[FIREBOLT]Resolution: %s", t_res.c_str());
	std::shared_ptr<PlayerExternalsRdkInterface> pInstance = PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance();

	if (TryParseResolutionFromPayload(t_res, width, height))
	{
		pInstance->SetResolution(width, height);
		MW_LOG_INFO("[FIREBOLT] Updating resolution from event payload [%d][%d]", width, height);
		return;
	}

	MW_LOG_WARN("[FIREBOLT] Resolution payload parse failed, falling back to Device.videoResolution");

	auto currentResolution = Firebolt::IFireboltAampAccessor::Instance().DeviceInterface().screenResolution();

	if(currentResolution)
	{
		width = currentResolution.value()[0];
		height = currentResolution.value()[1];
		pInstance->SetResolution(width, height);
		MW_LOG_INFO("[FIREBOLT] Updating resolution from Device.screenResolution [%d][%d]", width, height);
	}
	else
	{
		MW_LOG_ERR("[FIREBOLT] Failed to get current resolution");
	}

}
