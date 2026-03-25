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

#pragma once

/**
 * @file TelemetryMarkers.h
 * @brief Telemetry event marker definitions for the middleware player interface.
 *
 * Each macro defines the string key used when emitting a telemetry event via
 * PlayerTelemetry::sendEvent().  Markers are grouped by category to match the
 * lifecycle of a GStreamer-based playback session.
 */

/* ── Playback events ──────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_PLAYBACK_STARTED   "PLAYBACK_STARTED"   /**< Pipeline transitions to PLAYING */
#define TELEMETRY_EVENT_PLAYBACK_PAUSED    "PLAYBACK_PAUSED"    /**< Pipeline transitions to PAUSED on user request */
#define TELEMETRY_EVENT_PLAYBACK_RESUMED   "PLAYBACK_RESUMED"   /**< Pipeline transitions back to PLAYING from PAUSED */
#define TELEMETRY_EVENT_PLAYBACK_STOPPED   "PLAYBACK_STOPPED"   /**< Stop() tears down the pipeline */
#define TELEMETRY_EVENT_PLAYBACK_COMPLETED "PLAYBACK_COMPLETED" /**< End-of-stream reached and EOS callback scheduled */

/* ── Media / buffering events ─────────────────────────────────────────────── */
#define TELEMETRY_EVENT_BUFFERING_STARTED  "BUFFERING_STARTED"  /**< Pre-roll buffering begins */
#define TELEMETRY_EVENT_BUFFERING_ENDED    "BUFFERING_ENDED"    /**< Sufficient frames buffered; pipeline unpaused */
#define TELEMETRY_EVENT_SEEK_STARTED       "SEEK_STARTED"       /**< Flush seek requested */
#define TELEMETRY_EVENT_SEEK_COMPLETED     "SEEK_COMPLETED"     /**< gst_element_seek() succeeded */

/* ── Error events ─────────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_ERROR              "ERROR"              /**< Generic GStreamer pipeline error (GST_MESSAGE_ERROR) */
#define TELEMETRY_EVENT_DECODE_ERROR       "DECODE_ERROR"       /**< Decoder reported a decode-error-callback */
#define TELEMETRY_EVENT_NETWORK_ERROR      "NETWORK_ERROR"      /**< Resource/stream error that indicates a network fault */

/* ── Pipeline state change failure ───────────────────────────────────────── */
#define TELEMETRY_EVENT_PIPELINE_STATE_CHANGE_FAILURE "MW_PIPELINE_STATE_CHANGE_FAILURE" /**< gst_element_set_state() returned GST_STATE_CHANGE_FAILURE */

/* ── Lifecycle events ─────────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_INITIALIZED        "INTERFACE_INITIALIZED" /**< InterfacePlayerRDK constructor completed */
#define TELEMETRY_EVENT_SHUTDOWN           "INTERFACE_SHUTDOWN"    /**< InterfacePlayerRDK destructor entered */

/* ── Miscellaneous events ─────────────────────────────────────────────────── */
#define TELEMETRY_EVENT_EOS_DETECTED       "END_OF_STREAM_DETECTED" /**< GST_MESSAGE_EOS received on the pipeline bus */
#define TELEMETRY_EVENT_TRACK_SWITCHED     "TRACK_SWITCHED"         /**< Mid-stream audio/video track change */
