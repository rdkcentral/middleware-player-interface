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
 * @file PlayerExternalsInterface.h
 * @brief Output protection management for Player
 */

#ifndef PlayerExternalsInterface_h
#define PlayerExternalsInterface_h

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <memory>

#include "PlayerExternalsInterfaceBase.h"


#undef __in
#undef __out
#undef __reserved


//used for FakePlayerExternalsInterface only, mimics dsmgr params
#define PLAYER_dsHDCP_VERSION_MAX 30
#define PLAYER_dsHDCP_VERSION_2X 22
#define PLAYER_dsHDCP_VERSION_1X 14
typedef int playerDsHdcpProtocolVersion_t;

class FakePlayerExternalsInterface : public PlayerExternalsInterfaceBase
{
        playerDsHdcpProtocolVersion_t m_hdcpCurrentProtocol;
    public:
        FakePlayerExternalsInterface(){SetHDMIStatus();}
                 /**
         * @brief Get the source video width.
         *
         * Returns the width of the source video stream.
         *
         * @return int Source width in pixels.
         */
        int getSourceWidth() const { return m_sourceWidth; }
    
        /**
         * @brief Get the source video height.
         *
         * Returns the height of the source video stream.
         *
         * @return int Source height in pixels.
         */
        int getSourceHeight() const { return m_sourceHeight; }
        
        /**
         * @brief Get the display width.
         *
         * Returns the width of the display.
         *
         * @return int Display width in pixels.
         */
        int getDisplayWidth() const { return m_displayWidth; }
        
        /**
         * @brief Get the display height.
         *
         * Returns the height of the display.
         *
         * @return int Display height in pixels.
         */
        int getDisplayHeight() const { return m_displayHeight; }
        
        /**
         * @brief Check if HDCP is enabled.
         *
         * Returns true if HDCP (High-bandwidth Digital Content Protection) is enabled.
         *
         * @return bool True if HDCP is enabled, false otherwise.
         */
        bool isHDCPEnabled() const { return m_isHDCPEnabled; }
        
        /**
         * @brief Get the associated GstElement pointer.
         *
         * Returns the internal GstElement pointer used by this player interface.
         *
         * @return GstElement* Pointer to the GstElement.
         */
        GstElement* getGstElement() const { return m_gstElement; }
    
        /**
         * @brief Set the source video width.
         *
         * Sets the width of the source video stream.
         *
         * @param width Source width in pixels.
         */
        void setSourceWidth(int width) { m_sourceWidth = width; }
    
        /**
         * @brief Set the source video height.
         *
         * Sets the height of the source video stream.
         *
         * @param height Source height in pixels.
         */
        void setSourceHeight(int height) { m_sourceHeight = height; }
    
        /**
         * @brief Set the display width.
         *
         * Sets the width of the display.
         *
         * @param width Display width in pixels.
         */
        void setDisplayWidth(int width) { m_displayWidth = width; }
    
        /**
         * @brief Set the display height.
         *
         * Sets the height of the display.
         *
         * @param height Display height in pixels.
         */
        void setDisplayHeight(int height) { m_displayHeight = height; }
    
        /**
         * @brief Enable or disable HDCP.
         *
         * Sets the HDCP (High-bandwidth Digital Content Protection) enabled state.
         *
         * @param enabled True to enable HDCP, false to disable.
         */
        void setHDCPEnabled(bool enabled) { m_isHDCPEnabled = enabled; }
    
        /**
         * @brief Set the GstElement pointer (for fake/test purposes).
         *
         * Sets the internal GstElement pointer used by this player interface.
         *
         * @param element Pointer to the GstElement.
         */
        void setGstElementFake(GstElement *element) { m_gstElement = element; }


        void Initialize() override {}

        /**
         * @fn GetDisplayResolution
         * @brief Get current resolution's display width and height
         * @param[out] width width of current resolution
         * @param[out] height height of current resolution
         */
        void GetDisplayResolution(int &width, int &height) override{}

        /**
         * @fn SetHDMIStatus
         * @brief Checks Display Settings and sets HDMI parameters like video output resolution, HDCP protocol
         */
        void SetHDMIStatus() override{
            m_hdcpCurrentProtocol = PLAYER_dsHDCP_VERSION_1X;
            m_isHDCPEnabled = true;
        }
        
        /**
         * @fn GetTR181Config
         * @brief Gets appropriate TR181 Config
         * @param[in] paramName String of name of the parameter to be retrieved
         * @param[out] iConfigLen Length of config retrieved
         * @return Parameter config retrieved
         */
        char * GetTR181Config(const char * paramName, size_t & iConfigLen) override{return nullptr;}

        /**
         * @fn isHDCPConnection2_2
         * @brief Is current HDCP protocol 2_2
         * @return True if current HDCP protocol is 2_2. False, if not.
         */
        bool isHDCPConnection2_2() override{ return m_hdcpCurrentProtocol == PLAYER_dsHDCP_VERSION_2X; }

        /**
         * @fn GetActiveInterface
         * @brief Is current active interface wifi?
         * @return True if wifi. False, if not.
         */
        bool GetActiveInterface()override{return false;}

        void SetUseFireBoltSDK(bool t_use_firebolt_sdk) override {}

	void SetPowerEvent(bool powerEvt) override {}

    bool GetPowerEvent() override { return false; }

    /**
     * @brief Set callback function for fake tune operations (stub implementation)
     * @param[in] t_doFakeTuneCallback Function to call for fake tune operations
     */
    void SetDoFakeTuneCallBack(const std::function<void()>& t_doFakeTuneCallback) override {}

    /**
     * @brief Get callback function for fake tune operations (stub implementation)
     * @return nullptr for fake implementation
     */
    std::function<void()> GetDoFakeTuneCallBack() override { return nullptr; }
        
        ~FakePlayerExternalsInterface(){}
};

/**
 * @class PlayerExternalsInterface
 * @brief Class to enforce HDCP authentication
 */
class PlayerExternalsInterface
{

private:


    std::shared_ptr<PlayerExternalsInterfaceBase> m_pIarmInterface;

    static std::shared_ptr<PlayerExternalsInterface> s_pPlayerOP;

    /**
     * @fn PlayerExternalsInterface
     */
    PlayerExternalsInterface();
    

    
public:

    /**
     * @fn ~PlayerExternalsInterface
     */
    virtual ~PlayerExternalsInterface();
    /**     
     * @brief Copy constructor disabled
     *
     */
    PlayerExternalsInterface(const PlayerExternalsInterface&) = delete;
    /**
     * @brief assignment operator disabled
     *
     */
    PlayerExternalsInterface& operator=(const PlayerExternalsInterface&) = delete;

    void Initialize();	 
	 



    char * GetTR181PlayerConfig(const char * paramName, size_t & iConfigLen);

    // State functions

    /**
     * @brief Check if HDCP is 2.2
     * @retval true if 2.2 false otherwise
     */
    bool isHDCPConnection2_2() { return m_pIarmInterface->isHDCPConnection2_2(); }
    /**
     * @fn IsSourceUHD
     * @retval true, if source is UHD, otherwise false
     */
    bool IsSourceUHD();

    /**
     * @fn GetDisplayResolution
     * @param[out] width : Display Width
     * @param[out] height : Display height
     */
    void GetDisplayResolution(int &width, int &height);

    /**
     * @brief Set GstElement
     * @param element Gst element to set
     */
    void setGstElement(GstElement *element) { m_pIarmInterface->setGstElement(element);  }

    // Singleton for object creation

    /**
     * @fn GetPlayerExternalsInterfaceInstance
     * @retval PlayerExternalsInterface object
     */
    static std::shared_ptr<PlayerExternalsInterface> GetPlayerExternalsInterfaceInstance();
    /**
     * @fn IsPlayerExternalsInterfaceInstanceActive
     * @retval true or false
     */
    static bool IsPlayerExternalsInterfaceInstanceActive();
    /**
     * @fn GetActiveInterface
     * @brief Is current active interface wifi?
     * @return True if wifi. False, if not.
     */
    bool GetActiveInterface();

    /**
     * @fn IsConfigWifiCurlHeader
     * @brief Routine to find if IARM is supported in platform
     */
    bool IsConfigWifiCurlHeader();


    void SetUseFireBoltSDK(bool t_use_firebolt_sdk);

    void SetPowerEvent(bool powerEvt);

    bool GetPowerEvent();

    /**
     * @brief Set callback function for fake tune operations
     * @param[in] t_doFakeTuneCallback Function to call for fake tune operations
     */
    void SetDoFakeTuneCallBack(const std::function<void()>& t_doFakeTuneCallback);

    /**
     * @brief Checks if platform device properties are accessible.
     *
     * Device properties are only exposed via platform services when running
     * with IARM manager in a native (non-container) environment. This helper
     * therefore returns true if and only if those platform services are
     * expected to be available.
     */
    static bool IsDevicePropertiesPresent();
};

#endif // PlayerExternalsInterface_h
