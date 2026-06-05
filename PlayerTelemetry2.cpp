#include "PlayerTelemetry2.hpp"
#include <fstream>

#include <telemetry_busmessage_sender.h>

Player_TelemetryInitializer::Player_TelemetryInitializer()
: m_Initialized(false) // Initialize 'initialized' to false
{
}

void Player_TelemetryInitializer::Init()
{
    if(false == m_Initialized)
    {
        m_Initialized = true;
        t2_init((char *)"mwplayer");
        MW_LOG_MIL("t2_init done ");
    }
}

bool Player_TelemetryInitializer::isInitialized() const
{
    return m_Initialized;
}

Player_TelemetryInitializer::~Player_TelemetryInitializer()
{
    t2_uninit();
    MW_LOG_MIL("t2_uninit done ");
}

Player_TelemetryInitializer PlayerTelemetry2::mInitializer;

PlayerTelemetry2::PlayerTelemetry2() {
    PlayerTelemetry2("");
}

PlayerTelemetry2::PlayerTelemetry2( const std::string &appName) : appName(appName) {
    mInitializer.Init();	// deinit is done in destructor of PlayerTelemetryInitializer.
}

bool PlayerTelemetry2::send( const std::string &markerName, const std::map<std::string, int>& intData, const std::map<std::string, std::string>& stringData, const std::map<std::string, float>& floatData ) {
        MW_LOG_ERR("[M] Marker Name: %s %d", markerName.c_str(), mInitializer.isInitialized());
    bool bRet = false;

    // Log entry and initializer status
    MW_LOG_ERR("[M] Entered send() | marker: %s | initializer: %d",
               markerName.c_str(), mInitializer.isInitialized());

    bool init = mInitializer.isInitialized();
    if(init)
    {
        MW_LOG_ERR("[M] Inside initializer block");

        cJSON *root = cJSON_CreateObject();
        if(!root)
        {
            MW_LOG_ERR("[M] cJSON_CreateObject failed");
            return false;
        }

        MW_LOG_ERR("[M] JSON object created");

        cJSON_AddStringToObject(root, "app", appName.c_str());
        MW_LOG_ERR("[M] appName added: %s", appName.c_str());

        for (const auto& pair : intData)
        {
            MW_LOG_ERR("[M] int key=%s value=%d", pair.first.c_str(), pair.second);
            cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
        }

        for (const auto& pair : stringData)
        {
            MW_LOG_ERR("[M] string key=%s value=%s", pair.first.c_str(), pair.second.c_str());
            cJSON_AddStringToObject(root, pair.first.c_str(), pair.second.c_str());
        }

        for (const auto& pair : floatData)
        {
            MW_LOG_ERR("[M] float key=%s value=%f", pair.first.c_str(), pair.second);
            cJSON_AddNumberToObject(root, pair.first.c_str(), pair.second);
        }

        char *jsonString = cJSON_PrintUnformatted(root);
        if(!jsonString)
        {
            MW_LOG_ERR("[M] cJSON_PrintUnformatted failed");
            cJSON_Delete(root);
            return false;
        }

        MW_LOG_ERR("[M] Marker Name: %s | JSON: %s", markerName.c_str(), jsonString);

        T2ERROR t2Error = t2_event_s((char *)markerName.c_str(), jsonString);
        MW_LOG_ERR("[M] t2_event_s returned: %d", t2Error);

        if(T2ERROR_SUCCESS == t2Error)
        {
            MW_LOG_ERR("[M] Telemetry event sent successfully");
            bRet = true;
        }
        else
        {
            MW_LOG_ERR("[M] Telemetry event failed: %d", t2Error);
        }

        cJSON_free(jsonString);
        cJSON_Delete(root);
    }
    else
    {
        MW_LOG_ERR("[M] Telemetry initializer not ready");
    }

    MW_LOG_ERR("[M] Exiting send() | marker: %s", markerName.c_str());
    return bRet;
}

bool PlayerTelemetry2::send( const std::string &markerName, const char *  data)
{
    bool bRet = false;
    if(mInitializer.isInitialized()	&& NULL != data)
    {
        MW_LOG_INFO("[S] Marker Name: %s value:%s", markerName.c_str(),data );
        T2ERROR t2Error =  t2_event_s( (char *)markerName.c_str(),(char*)data );

        if(T2ERROR_SUCCESS == t2Error)
        {
            bRet = true;
        }
        else
        {
            MW_LOG_ERR("t2_event_s string failed:%d ", t2Error);
        }
    }
    return bRet;
}
void PlayerTelemetry::sendEvent(const std::string& eventName)
{
    MW_LOG_MIL("[TELEMETRY] event=%s", eventName.c_str());
    t2_event_d(const_cast<char*>(eventName.c_str()), 1);
}
