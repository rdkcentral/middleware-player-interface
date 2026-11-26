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
 * @file gst_svp_meta.h
 * @brief Minimal unit-test stub for the external SVP header.
 *
 * AmlogicSocInterface.cpp includes this header unconditionally but only uses
 * the gst_svp_ext_* entry points under #ifdef USE_SVP.  The unit-test build
 * does not define USE_SVP and does not link gst-svp-ext, so an empty header
 * is sufficient to satisfy the include.
 */

#ifndef GST_SVP_META_H
#define GST_SVP_META_H

#include <gst/gst.h>

#endif /* GST_SVP_META_H */
