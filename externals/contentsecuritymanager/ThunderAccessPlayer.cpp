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
 * @file ThunderAccess.cpp
 * @brief wrapper class for accessing thunder plugins
 */
#include "PlayerLogManager.h"
#include "ThunderAccessPlayer.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#ifdef USE_CPP_THUNDER_PLUGIN_ACCESS
#ifndef DISABLE_SECURITY_TOKEN
#include <securityagent/SecurityTokenUtil.h>
#endif
#pragma GCC diagnostic pop

using namespace std;
using namespace WPEFramework;
#endif
#define SERVER_DETAILS  "127.0.0.1:9998"

#define MAX_LENGTH 1024

//Delete non-array object
#define SAFE_DELETE(ptr) { delete(ptr); ptr = NULL; }

/**
 * @brief Structure to save the Thunder security token details
 **/
typedef struct ThunderSecurityPlayer
{
    std::string securityToken;
    int tokenStatus;
    bool tokenQueried;
    ThunderSecurityPlayer(): securityToken(), tokenStatus(0), tokenQueried(false) { };
}ThunderSecurityPlayerData;

ThunderSecurityPlayerData gSecurityPlayerData;


/**
 * @brief  ThunderAccessPlayer constructor
 */
ThunderAccessPlayer::ThunderAccessPlayer(std::string callsign)
                 : remoteObject(NULL)
                   ,controllerObject(NULL)
                   ,pluginCallsign(callsign)
{
    MW_LOG_WARN( "[GSK-TAP-CTOR] callsign=%s server=%s", callsign.c_str(), SERVER_DETAILS);
    uint32_t status = Core::ERROR_NONE;

    Core::SystemInfo::SetEnvironment(_T("THUNDER_ACCESS"), (_T(SERVER_DETAILS)));
    string sToken = "";
#ifdef DISABLE_SECURITY_TOKEN
     gSecurityPlayerData.securityToken = "token=" + sToken;
     gSecurityPlayerData.tokenQueried = true;
     MW_LOG_WARN( "[GSK-TAP-TOKEN] security token DISABLED");
#else
    if(!gSecurityPlayerData.tokenQueried)
    {
        unsigned char buffer[MAX_LENGTH] = {0};
        gSecurityPlayerData.tokenStatus = GetSecurityToken(MAX_LENGTH,buffer);
        if(gSecurityPlayerData.tokenStatus > 0){
            sToken = (char*)buffer;
            gSecurityPlayerData.securityToken = "token=" + sToken;
            MW_LOG_WARN( "[GSK-TAP-TOKEN] GetSecurityToken success len=%d", gSecurityPlayerData.tokenStatus);
        } else {
            MW_LOG_WARN( "[GSK-TAP-TOKEN] GetSecurityToken failed status=%d proceeding without token", gSecurityPlayerData.tokenStatus);
        }
        gSecurityPlayerData.tokenQueried = true;
    } else {
        MW_LOG_INFO( "[GSK-TAP-TOKEN] reusing cached token tokenStatus=%d", gSecurityPlayerData.tokenStatus);
    }
#endif
    if (NULL == controllerObject) {
        /*Passing empty string instead of Controller callsign.This is assumed as controller plugin.*/
        if(gSecurityPlayerData.tokenStatus > 0){
            controllerObject = new JSONRPC::LinkType<Core::JSON::IElement>(_T(""), _T(""), false, gSecurityPlayerData.securityToken);
        }
        else{
            controllerObject = new JSONRPC::LinkType<Core::JSON::IElement>(_T(""));
        }

        if (NULL == controllerObject) {
            MW_LOG_WARN( "[GSK-TAP-CTRL] controller object creation FAILED");
        } else {
            MW_LOG_WARN( "[GSK-TAP-CTRL] controller object created OK server=%s", SERVER_DETAILS);
        }
    }

    if(gSecurityPlayerData.tokenStatus > 0){
        remoteObject = new JSONRPC::LinkType<Core::JSON::IElement>(_T(pluginCallsign), _T(""), false, gSecurityPlayerData.securityToken);
    }
    else{
        remoteObject = new JSONRPC::LinkType<Core::JSON::IElement>(_T(pluginCallsign), _T(""));
    }
    if (NULL == remoteObject) {
        MW_LOG_WARN( "[GSK-TAP-REMOTE] %s remote object creation FAILED", pluginCallsign.c_str());
    } else {
        MW_LOG_WARN( "[GSK-TAP-REMOTE] %s remote object created OK server=%s", pluginCallsign.c_str(), SERVER_DETAILS);
    }
}

/**
 * @brief  ThunderAccessPlayer destructor
 */
ThunderAccessPlayer::~ThunderAccessPlayer()
{
    SAFE_DELETE(controllerObject);
    SAFE_DELETE(remoteObject);
}

/**
 * @brief  ActivatePlugin
 */
bool ThunderAccessPlayer::ActivatePlugin()
{
    bool ret = true;
    JsonObject result;
    JsonObject controlParam;
    std::string response;
    uint32_t status = Core::ERROR_NONE;

    if (NULL != controllerObject) {
        controlParam["callsign"] = pluginCallsign;
        MW_LOG_WARN( "[GSK-TAP-ACTIVATE] sending activate callsign=%s", pluginCallsign.c_str());
        status = controllerObject->Invoke<JsonObject, JsonObject>(THUNDER_RPC_TIMEOUT, _T("activate"), controlParam, result);
        if (Core::ERROR_NONE == status){
            result.ToString(response);
            MW_LOG_WARN( "[GSK-TAP-ACTIVATE] %s activated OK response=%s", pluginCallsign.c_str(), response.c_str());
        }
        else
        {
            MW_LOG_WARN( "[GSK-TAP-ACTIVATE] %s activation FAILED errorStatus=%u", pluginCallsign.c_str(), status);
            ret = false;
        }
    } else {
        MW_LOG_WARN( "[GSK-TAP-ACTIVATE] controller object is NULL cannot activate %s", pluginCallsign.c_str());
        ret = false;
    }
    return ret;
}

/*To Do: Only JSON Object can be used as parameter now*/
/**
 * @brief  subscribeEvent
 */
bool ThunderAccessPlayer::SubscribeEvent (string eventName, std::function<void(const WPEFramework::Core::JSON::VariantContainer&)> functionHandler)
{
    bool ret = true;
    uint32_t status = Core::ERROR_NONE;
    MW_LOG_WARN( "[GSK-TAP-SUBSCRIBE] callsign=%s event=%s", pluginCallsign.c_str(), eventName.c_str());
    if (NULL != remoteObject) {
        status = remoteObject->Subscribe<JsonObject>(THUNDER_RPC_TIMEOUT, _T(eventName), functionHandler);
        if (Core::ERROR_NONE == status) {
            MW_LOG_WARN( "[GSK-TAP-SUBSCRIBE] %s::%s subscribed OK", pluginCallsign.c_str(), eventName.c_str());
        } else {
            MW_LOG_WARN( "[GSK-TAP-SUBSCRIBE] %s::%s subscription FAILED errorStatus=%u", pluginCallsign.c_str(), eventName.c_str(), status);
            ret = false;
        }
    } else {
        MW_LOG_WARN( "[GSK-TAP-SUBSCRIBE] remoteObject is NULL for %s cannot subscribe to %s", pluginCallsign.c_str(), eventName.c_str());
        ret = false;
    }
    return ret;
}

/*To Do: Only JSON Object can be used as parameter now*/

/**
 * @brief  unSubscribeEvent
 */
bool ThunderAccessPlayer::UnSubscribeEvent (std::string eventName)
{
    bool ret = true;
    MW_LOG_WARN( "[GSK-TAP-UNSUBSCRIBE] callsign=%s event=%s", pluginCallsign.c_str(), eventName.c_str());
    if (NULL != remoteObject) {
        remoteObject->Unsubscribe(THUNDER_RPC_TIMEOUT, _T(eventName));
        MW_LOG_WARN( "[GSK-TAP-UNSUBSCRIBE] %s::%s unsubscribed OK", pluginCallsign.c_str(), eventName.c_str());
    } else {
        MW_LOG_WARN( "[GSK-TAP-UNSUBSCRIBE] remoteObject is NULL for %s cannot unsubscribe %s", pluginCallsign.c_str(), eventName.c_str());
        ret = false;
    }
    return ret;
}

/**
 *  @brief  invokeJSONRPC
 *  @note   Invoke JSONRPC call for the plugin
 */
bool ThunderAccessPlayer::InvokeJSONRPC(std::string method, const JsonObject &param, JsonObject &result, const uint32_t waitTime)
{
    bool ret = true;
    std::string response;
    uint32_t status = Core::ERROR_NONE;

    MW_LOG_WARN( "[GSK-TAP-INVOKE] callsign=%s method=%s", pluginCallsign.c_str(), method.c_str());
    if(NULL == remoteObject)
    {
        MW_LOG_WARN( "[GSK-TAP-INVOKE] remoteObject is NULL for %s cannot invoke %s", pluginCallsign.c_str(), method.c_str());
        return false;
    }

    JsonObject result_internal;
    status = remoteObject->Invoke<JsonObject, JsonObject>(waitTime, _T(method), param, result_internal);
    result_internal.ToString(response);
    if (Core::ERROR_NONE == status)
    {
        if (result_internal["success"].Boolean()) {
            MW_LOG_WARN( "[GSK-TAP-INVOKE] %s::%s SUCCESS response=%s", pluginCallsign.c_str(), method.c_str(), response.c_str());
        } else {
            MW_LOG_WARN( "[GSK-TAP-INVOKE] %s::%s FAILED (no success field) response=%s", pluginCallsign.c_str(), method.c_str(), response.c_str());
            ret = false;
        }
    } else {
        MW_LOG_WARN( "[GSK-TAP-INVOKE] %s::%s invoke FAILED errorStatus=%u response=%s", pluginCallsign.c_str(), method.c_str(), status, response.c_str());
        ret = false;
    }

    result = result_internal;
    return ret;
}
