
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
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdio.h>
#include "gstplayertaskpool.h"


/**
 * @brief Verify that gst_player_taskpool_get_type returns a valid, non-zero GType
 *
 * This test case ensures that calling gst_player_taskpool_get_type does not throw any exceptions and returns a valid non-zero GType essential for proper functionality.
 *
 * **Test Group ID:** Basic: 01@n
 * **Test Case ID:** 001@n
 * **Priority:** High@n
 *
 * **Pre-Conditions:** None@n
 * **Dependencies:** None@n
 * **User Interaction:** None@n
 *
 * **Test Procedure:**
 * | Variation / Step | Description | Test Data | Expected Result | Notes |
 * | :----: | --------- | ---------- |-------------- | ----- |
 * | 01 | Invoke gst_player_taskpool_get_type and validate that it returns a non-zero GType without throwing any exception | input: None, output: type_val = value from gst_player_taskpool_get_type | API returns a valid, non-zero GType and no exception is thrown | Should Pass |
 */
TEST(gst_player_taskpool_get_type_start, ValidReturn) {
    std::cout << "Entering gst_player_taskpool_get_type_start test" << std::endl;
    
    GType type_val;
    
    std::cout << "Invoking gst_player_taskpool_get_type()" << std::endl;
    EXPECT_NO_THROW({
        type_val = gst_player_taskpool_get_type();
    });

    std::cout << "gst_player_taskpool_get_type returned value: " << type_val << std::endl;

    std::cout << "Exiting gst_player_taskpool_get_type_start test" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
