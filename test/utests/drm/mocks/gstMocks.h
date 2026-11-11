/*
 * If not stated otherwise in this file or this component's license file the
 * following copyright and licenses apply:
 *
 * Copyright 2026 RDK Management
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

#ifndef GST_MOCKS_H
#define GST_MOCKS_H

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Make the gst_buffer_map() mock return the given data/size for a buffer.
 *        Buffers that have not been registered are left unmapped (legacy behaviour).
 */
void MockGstBufferSetMapData(GstBuffer *buffer, guint8 *data, gsize size);

/**
 * @brief Remove all buffer data registered with MockGstBufferSetMapData().
 */
void MockGstBufferClearMapData(void);

#ifdef __cplusplus
}
#endif

#endif /* GST_MOCKS_H */
