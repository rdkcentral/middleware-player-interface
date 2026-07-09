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
 * @file PlayerExternalsRdkInterface.cpp
 * @brief player interface with IARM specific to RDK
 */
#include "PlayerExternalsRdkInterface.h"
#include "PlayerExternalUtils.h"
#include "DeviceInterfaceBase.h"
#include "DeviceIARMInterface.h"
#include "DeviceFireboltInterface.h"
#include "PlayerExternalsInterface.h"
#include <utility>

#ifdef USE_DS_THUNDER_PLUGIN
#include "PlayerThunderAccess.h"
#endif

#include <cstdio>

#define DISPLAY_WIDTH_UNKNOWN       -1  /**< Parsing failed for getResolution().getName(); */
#define DISPLAY_HEIGHT_UNKNOWN      -1  /**< Parsing failed for getResolution().getName(); */
#define DISPLAY_RESOLUTION_NA        0  /**< Resolution not available yet or not connected to HDMI */

std::shared_ptr<PlayerExternalsRdkInterface> s_pPlayerIarmRdkOP = nullptr;

static bool isInterfaceWifi = false;

/**
 * @brief Singleton for object creation
 */
std::shared_ptr<PlayerExternalsRdkInterface> PlayerExternalsRdkInterface::GetPlayerExternalsRdkInterfaceInstance()
{
    if(s_pPlayerIarmRdkOP == nullptr) {
        s_pPlayerIarmRdkOP = std::shared_ptr<PlayerExternalsRdkInterface>(new PlayerExternalsRdkInterface());
    }

    return s_pPlayerIarmRdkOP;
}

PlayerExternalsRdkInterface::PlayerExternalsRdkInterface()
{
    
}

void PlayerExternalsRdkInterface::Initialize()
{

    MW_PRE_LOGGER_LOG("Initializing started \n");

    /*
    IARM Deprecation Note:
    IARM is to be deprecated in favor of DeviceSettings and Firebolt Device API.
    */
    /*
    Remove the section between the comment section remove-start and remove-end when deprecating IARM
    */
    
    //remove-start
    //initialize only if needed
    if(m_initialized != InitState::NOT_INITIALIZED)
    {
        if(m_initialized == InitState::FIREBOLT && (m_use_firebolt_sdk || IsContainerEnvironment()))
        {
            MW_PRE_LOGGER_LOG("Firebolt already Inited \n");
            //firebolt already inited
            return;
        }
        else if(m_initialized == InitState::IARM && (!m_use_firebolt_sdk) && (!IsContainerEnvironment()))
        {
            MW_PRE_LOGGER_LOG("IARM already Inited \n");
            //IARM already inited
            return;
        }
        else
        {
            MW_PRE_LOGGER_LOG("m_use_firebolt_sdk or IsContainerEnvironment() has changed, init again \n");
            //m_use_firebolt_sdk has changed init again
        }
    }
    else
    {
        MW_PRE_LOGGER_LOG("Initializing \n");
    }
    //remove-end
    
    if(m_pDeviceInterfaceBase)
    {
        m_pDeviceInterfaceBase = nullptr;
    }

    MW_PRE_LOGGER_LOG("m_use_firebolt_sdk : %d, IsContainerEnvironment() : %d \n", m_use_firebolt_sdk, IsContainerEnvironment());

    //remove-start
    if(m_use_firebolt_sdk || IsContainerEnvironment()) //if explicitly config'd to or if in container go for firebolt
    {
    //remove-end
        MW_PRE_LOGGER_LOG("Using Firebolt \n");
        m_pDeviceInterfaceBase = DeviceFireboltInterface::GetInstance();
        DeviceFireboltInterface::Initialize();
    //remove-start
        m_initialized = PlayerExternalsRdkInterface::InitState::FIREBOLT;
    }
    else
    {
        MW_PRE_LOGGER_LOG("Using IARM \n");
        m_pDeviceInterfaceBase = DeviceIARMInterface::GetInstance();
        DeviceIARMInterface::Initialize();
        m_initialized = PlayerExternalsRdkInterface::InitState::IARM;
    }
    //remove-end

    MW_PRE_LOGGER_LOG("Done getting interface \n");

#ifdef USE_DS_THUNDER_PLUGIN
    /* Register event handlers first so Thunder objects exist before SetHDMIStatus() is called */
    RegisterThunderEventHandlers();
    /* Post the initial HDMI status query through the worker thread */
    PostHDMIStatusUpdate();
#else
    SetHDMIStatus();
#  ifdef USE_DS_EVENT_SUPPORTED
    RegisterDsClientEventHandler();
#  endif
#endif

    MW_PRE_LOGGER_LOG("Initializing completed \n");
}

#ifdef USE_DS_EVENT_SUPPORTED
void PlayerExternalsRdkInterface::RegisterDsClientEventHandler()
{
	try {
		device::Manager::Initialize();
		device::Host::getInstance().Register(baseInterface<device::Host::IVideoOutputPortEvents>(),"PI::DisplayInfo");
		device::Host::getInstance().Register(baseInterface<device::Host::IDisplayDeviceEvents>(), "PI::DisplaySettings");
	}
	catch (const std::exception& e) {
		MW_LOG_WARN("DeviceSettings exception caught: %s\n", e.what());
	}
	catch (...) {
		MW_LOG_WARN("DeviceSettings unknown exception caught\n");
	}
}

void PlayerExternalsRdkInterface::RemoveDsClientEventHandlers()
{
	try
	{
		device::Host::getInstance().UnRegister(baseInterface<device::Host::IVideoOutputPortEvents>());
		device::Host::getInstance().UnRegister(baseInterface<device::Host::IDisplayDeviceEvents>());
		device::Manager::DeInitialize();
	}
	catch (const std::exception& e)
	{
		MW_LOG_WARN("DeviceSettings exception caught: %s\n", e.what());
	}
	catch(...)
	{
		MW_LOG_WARN("DeviceSettings unknown exception caught\n");
	}
}

void PlayerExternalsRdkInterface::OnDisplayHDMIHotPlug(dsDisplayEvent_t displayEvent)
{
	const char *hdmihotplug = (displayEvent == dsDISPLAY_EVENT_CONNECTED) ? "connected" : "disconnected";
	MW_LOG_WARN(" Received Display HDMI HotPlug event data:%d status: %s\n",
			   (int)displayEvent, hdmihotplug);

	SetHDMIStatus();
}

void PlayerExternalsRdkInterface::OnHDCPStatusChange(dsHdcpStatus_t hdcpStatus)
{
	const char *hdcpStatusStr = (hdcpStatus == dsHDCP_STATUS_AUTHENTICATED) ? "authenticated" : "authentication failure";
	MW_LOG_WARN(" Received HDCP Status Change event data:%d status:%s\n",
			  hdcpStatus, hdcpStatusStr);

	SetHDMIStatus();
}

/**
 * @brief event handler for resolution changes
 */
void PlayerExternalsRdkInterface::OnResolutionPostChange(int width, int height)
{

	MW_LOG_WARN(" Received Resolution Post Change event width : %d height : %d\n", width, height);
	SetResolution(width, height);
}

void PlayerExternalsRdkInterface::OnResolutionPreChange(int width, int height)
{
	MW_LOG_WARN(" Received Resolution PreChange event \n");
}
#endif

PlayerExternalsRdkInterface::~PlayerExternalsRdkInterface()
{
#ifdef USE_DS_THUNDER_PLUGIN
    RemoveThunderEventHandlers();
#elif defined(USE_DS_EVENT_SUPPORTED)
	RemoveDsClientEventHandlers();
#endif
    m_pDeviceInterfaceBase = nullptr;
    s_pPlayerIarmRdkOP = nullptr;
}

void PlayerExternalsRdkInterface::GetDisplayResolution(int &width, int &height)
{
    width   = m_displayWidth;
    height  = m_displayHeight;
}

void PlayerExternalsRdkInterface::SetResolution(int width, int height)
{
    MW_LOG_WARN(" Resolution : width %d height:%d\n",width,height);
    m_displayWidth   = width;
    m_displayHeight  = height;
}

/**
 * @brief Set the HDCP status using data from DeviceSettings
 */
void PlayerExternalsRdkInterface::SetHDMIStatus()
{
#ifdef USE_DS_THUNDER_PLUGIN
    /*
     * Thunder path: replaces all libds (device::Manager / VideoOutputPort) calls.
     * CSV mapping:
     *   getHDCPCurrentProtocol  -> HdcpProfile.1  getHDCPStatus -> currentHDCPVersion
     *   getHDCPReceiverProtocol -> HdcpProfile.1  getHDCPStatus -> receiverHDCPVersion
     *   isContentProtected      -> HdcpProfile.1  getHDCPStatus -> isHDCPEnabled
     *   isDisplayConnected      -> HdcpProfile.1  getHDCPStatus -> isConnected
     *   getPixelResolution      -> DisplaySettings.1  getCurrentResolution -> w / h
     */
    bool isConnected  = false;
    bool isHDCPEnabled = false;
    dsHdcpProtocolVersion_t hdcpCurrentProtocol = dsHDCP_VERSION_1X;

    MW_LOG_WARN("[DS-Thunder] SetHDMIStatus() called\n");

    /* --- Query HDCP status via HdcpProfile.1 --- */
    if (m_hdcpProfileThunder) {
        JsonObject param, result;
        std::string rawResponse;
        MW_LOG_WARN("[DS-Thunder] Calling HdcpProfile.1::getHDCPStatus\n");
        bool rpcRet = m_hdcpProfileThunder->InvokeJSONRPC("getHDCPStatus", param, result);
        result.ToString(rawResponse);
        MW_LOG_WARN("[DS-Thunder] getHDCPStatus rpcRet=%d rawResponse=%s\n", rpcRet, rawResponse.c_str());
        if (rpcRet) {
            /* Response is nested: { "HDCPStatus": { "isConnected":..., ... }, "success":true } */
            if (result.HasLabel("HDCPStatus")) {
                JsonObject hdcpStatus = result["HDCPStatus"].Object();
                std::string hdcpStatusStr;
                hdcpStatus.ToString(hdcpStatusStr);
                MW_LOG_WARN("[DS-Thunder] HDCPStatus object=%s\n", hdcpStatusStr.c_str());
                isConnected   = hdcpStatus["isConnected"].Boolean();
                isHDCPEnabled = hdcpStatus["isHDCPEnabled"].Boolean();
                std::string currentVer = hdcpStatus["currentHDCPVersion"].String();
                hdcpCurrentProtocol = (currentVer == "2.2") ? dsHDCP_VERSION_2X : dsHDCP_VERSION_1X;
                MW_LOG_WARN("[DS-Thunder] getHDCPStatus: isConnected=%d isHDCPEnabled=%d currentHDCPVersion=%s\n",
                            isConnected, isHDCPEnabled, currentVer.c_str());
            } else {
                MW_LOG_WARN("[DS-Thunder] getHDCPStatus: 'HDCPStatus' key missing in response — falling back to flat read\n");
                /* Fallback: try flat fields (old API) */
                isConnected   = result["isConnected"].Boolean();
                isHDCPEnabled = result["isHDCPEnabled"].Boolean();
                std::string currentVer = result["currentHDCPVersion"].String();
                hdcpCurrentProtocol = (currentVer == "2.2") ? dsHDCP_VERSION_2X : dsHDCP_VERSION_1X;
                MW_LOG_WARN("[DS-Thunder] fallback flat: isConnected=%d isHDCPEnabled=%d currentHDCPVersion=%s\n",
                            isConnected, isHDCPEnabled, currentVer.c_str());
            }
        } else {
            MW_LOG_WARN("[DS-Thunder] getHDCPStatus JSONRPC call failed (rpcRet=false)\n");
        }
    } else {
        MW_LOG_WARN("[DS-Thunder] m_hdcpProfileThunder is NULL — was RegisterThunderEventHandlers() called?\n");
    }

    /* --- Query pixel resolution via DisplaySettings.1::getCurrentResolution --- */
    if (isConnected && m_dsThunder) {
        JsonObject param, result;
        std::string rawResponse;
        MW_LOG_WARN("[DS-Thunder] Calling DisplaySettings.1::getCurrentResolution\n");
        bool rpcRet = m_dsThunder->InvokeJSONRPC("getCurrentResolution", param, result);
        result.ToString(rawResponse);
        MW_LOG_WARN("[DS-Thunder] getCurrentResolution rpcRet=%d rawResponse=%s\n", rpcRet, rawResponse.c_str());
        if (rpcRet && result["success"].Boolean()) {
            int w = static_cast<int>(result["w"].Number());
            int h = static_cast<int>(result["h"].Number());
            MW_LOG_WARN("[DS-Thunder] getCurrentResolution: resolution=%s w=%d h=%d\n",
                        result["resolution"].String().c_str(), w, h);
            if (w > 0 && h > 0) {
                SetResolution(w, h);
            } else {
                MW_LOG_WARN("[DS-Thunder] getCurrentResolution: invalid resolution w=%d h=%d — keeping current\n", w, h);
            }
        } else {
            MW_LOG_WARN("[DS-Thunder] getCurrentResolution failed rpcRet=%d success=%d\n",
                        rpcRet, result["success"].Boolean());
        }
    } else if (!isConnected) {
        MW_LOG_WARN("[DS-Thunder] Display not connected — setting resolution to NA\n");
        SetResolution(DISPLAY_RESOLUTION_NA, DISPLAY_RESOLUTION_NA);
    } else {
        MW_LOG_WARN("[DS-Thunder] m_dsThunder is NULL\n");
    }

    m_isHDCPEnabled = isHDCPEnabled;
    if (m_isHDCPEnabled) {
        m_hdcpCurrentProtocol = hdcpCurrentProtocol;
        MW_LOG_WARN("[DS-Thunder] HDCP version detected: %s\n", m_hdcpCurrentProtocol == dsHDCP_VERSION_2X ? "2.x" : "1.4");
    } else {
        MW_LOG_WARN("[DS-Thunder] HDCP is not enabled\n");
    }
    if (!isConnected) {
        m_hdcpCurrentProtocol = dsHDCP_VERSION_1X;
        MW_LOG_WARN("[DS-Thunder] GetHDCPVersion: Display not connected — defaulting to HDCP 1.4 (%d)\n", m_hdcpCurrentProtocol);
    }

#else
    bool                    isConnected              = false;
    bool                    isHDCPCompliant          = false;
    bool                    isHDCPEnabled            = true;
    dsHdcpProtocolVersion_t hdcpProtocol             = dsHDCP_VERSION_MAX;
    dsHdcpProtocolVersion_t hdcpReceiverProtocol     = dsHDCP_VERSION_MAX;
    dsHdcpProtocolVersion_t hdcpCurrentProtocol      = dsHDCP_VERSION_MAX;

    try {
        //Get the HDMI port
        device::Manager::Initialize();
        std::string strVideoPort = device::Host::getInstance().getDefaultVideoPortName();
        ::device::VideoOutputPort &vPort = ::device::Host::getInstance().getVideoOutputPort(strVideoPort);
        isConnected        = vPort.isDisplayConnected();
        hdcpProtocol       = (dsHdcpProtocolVersion_t)vPort.getHDCPProtocol();
        if(isConnected) {
            isHDCPCompliant          = (vPort.getHDCPStatus() == dsHDCP_STATUS_AUTHENTICATED);
            isHDCPEnabled            = vPort.isContentProtected();
            hdcpReceiverProtocol     = (dsHdcpProtocolVersion_t)vPort.getHDCPReceiverProtocol();
            hdcpCurrentProtocol      = (dsHdcpProtocolVersion_t)vPort.getHDCPCurrentProtocol();
            //get the resolution of the TV
            int width,height;
            int iResID = vPort.getResolution().getPixelResolution().getId();
            if( device::PixelResolution::k720x480 == iResID )
            {
                width =  720;
                height = 480;
            }
            else if(  device::PixelResolution::k720x576 == iResID )
            {
                width = 720;
                height = 576;
            }
            else if(  device::PixelResolution::k1280x720 == iResID )
            {
                width =  1280;
                height = 720;
            }
            else if(  device::PixelResolution::k1920x1080 == iResID )
            {
                width =  1920;
                height = 1080;
            }
            else if(  device::PixelResolution::k3840x2160 == iResID )
            {
                width =  3840;
                height = 2160;
            }
            else if(  device::PixelResolution::k4096x2160 == iResID )
            {
                width =  4096;
                height = 2160;
            }
            else
            {
                width =  DISPLAY_WIDTH_UNKNOWN;
                height = DISPLAY_HEIGHT_UNKNOWN;
                std::string _res = vPort.getResolution().getName();
                MW_LOG_ERR(" ERR parse failed for getResolution().getName():%s id:%d\n",(_res.empty() ? "NULL" : _res.c_str()),iResID);
            }

            SetResolution(width, height);
        }
        else {
            isHDCPCompliant = false;
            isHDCPEnabled = false;
            SetResolution(DISPLAY_RESOLUTION_NA,DISPLAY_RESOLUTION_NA);
        }

        device::Manager::DeInitialize();
    }
    catch (const std::exception& e) {
        MW_LOG_WARN("DeviceSettings exception caught: %s\n", e.what());
    }
    catch (...) {
        MW_LOG_WARN("DeviceSettings unknown exception caught\n");
    }

    m_isHDCPEnabled = isHDCPEnabled;

    if(m_isHDCPEnabled) {
        if(hdcpCurrentProtocol == dsHDCP_VERSION_2X) {
            m_hdcpCurrentProtocol = hdcpCurrentProtocol;
        }
        else {
            m_hdcpCurrentProtocol = dsHDCP_VERSION_1X;
        }
        MW_LOG_WARN(" detected HDCP version %s\n", m_hdcpCurrentProtocol == dsHDCP_VERSION_2X ? "2.x" : "1.4");
    }
    else {
        MW_LOG_WARN("DeviceSettings HDCP is not enabled\n");
    }

    if(!isConnected) {
        m_hdcpCurrentProtocol = dsHDCP_VERSION_1X;
        MW_LOG_WARN(" GetHDCPVersion: Did not detect HDCP version defaulting to 1.4 (%d)\n", m_hdcpCurrentProtocol);
    }
#endif

    return;
}

#ifdef USE_DS_THUNDER_PLUGIN
/**
 * @brief Signal the worker thread that an HDMI status update is needed.
 *        Safe to call from Thunder event callbacks (returns immediately).
 *        Multiple concurrent calls are coalesced — the worker runs SetHDMIStatus()
 *        exactly once per burst of events.
 */
void PlayerExternalsRdkInterface::PostHDMIStatusUpdate()
{
    {
        std::lock_guard<std::mutex> lk(m_eventMutex);
        m_eventPending = true;
    }
    m_eventCv.notify_one();
    MW_LOG_WARN("[DS-Thunder] PostHDMIStatusUpdate: event queued\n");
}

/**
 * @brief Worker thread body.
 *        Waits for a pending update, then calls SetHDMIStatus() once.
 *        Coalesces rapid back-to-back events into a single call.
 */
void PlayerExternalsRdkInterface::EventWorkerLoop()
{
    MW_LOG_WARN("[DS-Thunder] EventWorkerLoop: thread started\n");
    while (true) {
        std::unique_lock<std::mutex> lk(m_eventMutex);
        m_eventCv.wait(lk, [this] {
            return m_eventPending.load() || m_eventWorkerStop.load();
        });
        if (m_eventWorkerStop.load()) {
            MW_LOG_WARN("[DS-Thunder] EventWorkerLoop: stop requested — exiting\n");
            break;
        }
        m_eventPending = false;
        lk.unlock(); /* unlock before JSONRPC calls */
        MW_LOG_WARN("[DS-Thunder] EventWorkerLoop: calling SetHDMIStatus()\n");
        SetHDMIStatus();
    }
}

/**
 * @brief Create Thunder plugin objects and subscribe to HDMI/HDCP/resolution events.
 * CSV mapping:
 *   IARM_BUS_DSMGR_EVENT_HDCP_STATUS    -> HdcpProfile.1  onDisplayConnectionChanged
 *   IARM_BUS_DSMGR_EVENT_HDMI_HOTPLUG   -> DisplaySettings.1 connectedVideoDisplaysUpdated
 *   IARM_BUS_DSMGR_EVENT_RES_POSTCHANGE -> DisplaySettings.1 resolutionChanged
 *   IARM_BUS_DSMGR_EVENT_RES_PRECHANGE  -> DisplaySettings.1 resolutionPreChange
 */
void PlayerExternalsRdkInterface::RegisterThunderEventHandlers()
{
    MW_LOG_WARN("[DS-Thunder] RegisterThunderEventHandlers() start\n");

    /* Start the worker thread that serialises SetHDMIStatus() calls */
    m_eventWorkerStop = false;
    m_eventPending    = false;
    m_eventWorkerThread = std::thread(&PlayerExternalsRdkInterface::EventWorkerLoop, this);
    MW_LOG_WARN("[DS-Thunder] EventWorkerLoop thread started\n");

    /* ---- HdcpProfile.1 ---- */
    MW_LOG_WARN("[DS-Thunder] Creating PlayerThunderAccess for HDCPPROFILE (org.rdk.HdcpProfile.1)\n");
    m_hdcpProfileThunder = std::make_unique<PlayerThunderAccess>(PlayerThunderAccessPlugin::HDCPPROFILE);
    bool activateRet = m_hdcpProfileThunder->ActivatePlugin();
    MW_LOG_WARN("[DS-Thunder] HdcpProfile.1 ActivatePlugin() ret=%d\n", activateRet);

    /* onDisplayConnectionChanged: replaces IARM_BUS_DSMGR_EVENT_HDCP_STATUS */
    /* NOTE: Thunder does not allow InvokeJSONRPC from within a callback.
     * Spawn a detached thread so the callback returns immediately. */
    bool subRet = m_hdcpProfileThunder->SubscribeEvent(
        "onDisplayConnectionChanged",
        [this](const WPEFramework::Core::JSON::VariantContainer& params) {
            std::string paramsStr;
            params.ToString(paramsStr);
            MW_LOG_WARN("[DS-Thunder] onDisplayConnectionChanged received params=%s\n", paramsStr.c_str());
            PostHDMIStatusUpdate();
        });
    MW_LOG_WARN("[DS-Thunder] HdcpProfile.1 SubscribeEvent(onDisplayConnectionChanged) ret=%d\n", subRet);

    /* ---- DisplaySettings.1 ---- */
    MW_LOG_WARN("[DS-Thunder] Creating PlayerThunderAccess for DS (org.rdk.DisplaySettings.1)\n");
    m_dsThunder = std::make_unique<PlayerThunderAccess>(PlayerThunderAccessPlugin::DS);
    activateRet = m_dsThunder->ActivatePlugin();
    MW_LOG_WARN("[DS-Thunder] DisplaySettings.1 ActivatePlugin() ret=%d\n", activateRet);

    /* connectedVideoDisplaysUpdated: replaces IARM_BUS_DSMGR_EVENT_HDMI_HOTPLUG */
    subRet = m_dsThunder->SubscribeEvent(
        "connectedVideoDisplaysUpdated",
        [this](const WPEFramework::Core::JSON::VariantContainer& params) {
            std::string paramsStr;
            params.ToString(paramsStr);
            MW_LOG_WARN("[DS-Thunder] connectedVideoDisplaysUpdated received params=%s\n", paramsStr.c_str());
            PostHDMIStatusUpdate();
        });
    MW_LOG_WARN("[DS-Thunder] DisplaySettings.1 SubscribeEvent(connectedVideoDisplaysUpdated) ret=%d\n", subRet);

    /* resolutionChanged: replaces IARM_BUS_DSMGR_EVENT_RES_POSTCHANGE */
    subRet = m_dsThunder->SubscribeEvent(
        "resolutionChanged",
        [this](const WPEFramework::Core::JSON::VariantContainer& params) {
            std::string paramsStr;
            params.ToString(paramsStr);
            MW_LOG_WARN("[DS-Thunder] resolutionChanged received params=%s\n", paramsStr.c_str());
            PostHDMIStatusUpdate();
        });
    MW_LOG_WARN("[DS-Thunder] DisplaySettings.1 SubscribeEvent(resolutionChanged) ret=%d\n", subRet);

    /* resolutionPreChange: replaces IARM_BUS_DSMGR_EVENT_RES_PRECHANGE */
    subRet = m_dsThunder->SubscribeEvent(
        "resolutionPreChange",
        [](const WPEFramework::Core::JSON::VariantContainer& params) {
            MW_LOG_WARN("[DS-Thunder] resolutionPreChange event received\n");
        });
    MW_LOG_WARN("[DS-Thunder] DisplaySettings.1 SubscribeEvent(resolutionPreChange) ret=%d\n", subRet);

    MW_LOG_WARN("[DS-Thunder] RegisterThunderEventHandlers() done\n");
}

/**
 * @brief Unsubscribe Thunder events and release plugin objects.
 */
void PlayerExternalsRdkInterface::RemoveThunderEventHandlers()
{
    /* Stop worker thread before unsubscribing */
    {
        std::lock_guard<std::mutex> lk(m_eventMutex);
        m_eventWorkerStop = true;
    }
    m_eventCv.notify_one();
    if (m_eventWorkerThread.joinable()) {
        m_eventWorkerThread.join();
        MW_LOG_WARN("[DS-Thunder] EventWorkerLoop thread joined\n");
    }

    if (m_hdcpProfileThunder) {
        m_hdcpProfileThunder->UnSubscribeEvent("onDisplayConnectionChanged");
        m_hdcpProfileThunder.reset();
    }
    if (m_dsThunder) {
        m_dsThunder->UnSubscribeEvent("connectedVideoDisplaysUpdated");
        m_dsThunder->UnSubscribeEvent("resolutionChanged");
        m_dsThunder->UnSubscribeEvent("resolutionPreChange");
        m_dsThunder.reset();
    }
}
#endif /* USE_DS_THUNDER_PLUGIN */

void PlayerExternalsRdkInterface::setHdcpProtocol(dsHdcpProtocolVersion_t t_protocol)
{
    m_hdcpCurrentProtocol = t_protocol;
    MW_LOG_WARN(" detected HDCP version %s\n", m_hdcpCurrentProtocol == dsHDCP_VERSION_2X ? "2.x" : "1.4");
}

std::shared_ptr<DeviceInterfaceBase> PlayerExternalsRdkInterface::GetDeviceInterface()
{
    return m_pDeviceInterfaceBase;
}

bool PlayerExternalsRdkInterface::GetActiveInterface()
{
    return isInterfaceWifi;
}

void PlayerExternalsRdkInterface::SetActiveInterface(bool isWifi)
{
    isInterfaceWifi = isWifi;
}

char * PlayerExternalsRdkInterface::GetTR181Config(const char * paramName, size_t & iConfigLen)
{
    return m_pDeviceInterfaceBase->GetTR181Config(paramName, iConfigLen);
}

void PlayerExternalsRdkInterface::SetUseFireBoltSDK(bool t_use_firebolt_sdk)
{
    MW_PRE_LOGGER_LOG("old : %d, new : %d \n", m_use_firebolt_sdk, t_use_firebolt_sdk);
    if(m_use_firebolt_sdk != t_use_firebolt_sdk)
    {
        m_use_firebolt_sdk = t_use_firebolt_sdk;
        //reinitialize
        m_initialized = InitState::NOT_INITIALIZED;
        Initialize();

    }
    
}

void PlayerExternalsRdkInterface::SetPowerEvent(bool powerEvt)
{
    mPowerEvt = powerEvt;	
}

bool PlayerExternalsRdkInterface::GetPowerEvent()
{
    return mPowerEvt;	
}

void PlayerExternalsRdkInterface::SetDoFakeTuneCallBack(const std::function<void()>& t_doFakeTuneCallback)
{
    m_doFakeTuneCallback = t_doFakeTuneCallback;
}

std::function<void()> PlayerExternalsRdkInterface::GetDoFakeTuneCallBack()
{
    return m_doFakeTuneCallback;
}
