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
        MW_LOG_ERR("[M] Marker Name: %s value:%s", markerName.c_str(),jsonString);
    bool bRet = false;
    if(mInitializer.isInitialized()	)
    {
        cJSON *root = cJSON_CreateObject();

        cJSON_AddStringToObject(root, "app", appName.c_str());

        for (const auto& pair : intData) {
            std::string key = pair.first;
            int value = pair.second;
            cJSON_AddNumberToObject(root, key.c_str(), value);
        }

        for (const auto& pair : stringData) {
            std::string key = pair.first;
            std::string value = pair.second;
            cJSON_AddStringToObject(root, key.c_str(), value.c_str());
        }

        for (const auto& pair : floatData) {
            std::string key = pair.first;
            float value = pair.second;
            cJSON_AddNumberToObject(root, key.c_str(), value);
        }

        //lets use cJSON_PrintUnformatted , cJSON_Print is formated adds whitespace n hence takes more memory also eats up more logs if logged.
        char *jsonString = cJSON_PrintUnformatted(root);

        MW_LOG_INFO("[M] Marker Name: %s value:%s", markerName.c_str(),jsonString);

        T2ERROR t2Error = t2_event_s( (char *)markerName.c_str(),jsonString);

        if(T2ERROR_SUCCESS == t2Error)
        {
            bRet = true;
        }
        else
        {
            MW_LOG_ERR("t2_event_s map failed:%d ", t2Error);
        }
        cJSON_free(jsonString);
        cJSON_Delete(root);
    }

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
