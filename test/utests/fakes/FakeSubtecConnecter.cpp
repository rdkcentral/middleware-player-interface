/*
* If not stated otherwise in this file or this component's license file the
* following copyright and licenses apply:
*
* Copyright 2024 RDK Management
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
#include "../../closedcaptions/subtec/SubtecConnector.h"

namespace subtecConnector {

    mrcc_Error initHal(void* /*handle*/) {
        return CC_VL_OS_API_RESULT_SUCCESS; 
    }

    mrcc_Error initPacketSender() {
        return CC_VL_OS_API_RESULT_SUCCESS;
    }

    void resetChannel() 
    {
    }

    void close() 
    {
    }

    namespace ccMgrAPI {
    
        mrcc_Error ccSetDigitalChannel(unsigned int)
        { 
            return {};
        }
        
        mrcc_Error ccSetAnalogChannel(unsigned int)
        {
            return {}; 
        }
        
        mrcc_Error ccShow() 
        { 
            return CC_VL_OS_API_RESULT_SUCCESS; 
        }
        
        mrcc_Error ccHide() 
        { 
            return CC_VL_OS_API_RESULT_SUCCESS; 
        }
    };
}
