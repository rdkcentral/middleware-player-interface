/*
* Copyright 2026 RDK Management
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation, version 2.1
* of the license.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
* Boston, MA 02110-1301, USA.
*/

#include "gstdrmmultidecryptor.h"
#include "DrmConstants.h"

GST_DEBUG_CATEGORY_STATIC ( gst_drm_multi_decryptor_debug_category);
#define GST_CAT_DEFAULT  gst_drm_multi_decryptor_debug_category

enum
{
    PROP_0, PROP_PLAYER, PROP_DRM_SESSION_MANAGER
};

static GstStaticPadTemplate sink_factory =  GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
                GST_STATIC_CAPS(
                        //PLAYREADY
                        "application/x-cenc, original-media-type=(string)video/x-h264, protection-system=(string)" PLAYREADY_UUID "; "
                        "application/x-cenc, original-media-type=(string)video/x-h265, protection-system=(string)" PLAYREADY_UUID "; "
                        "application/x-cenc, original-media-type=(string)audio/x-eac3, protection-system=(string)" PLAYREADY_UUID "; "
                        "application/x-cenc, original-media-type=(string)audio/x-ac3, protection-system=(string)" PLAYREADY_UUID "; "
                        "application/x-cenc, original-media-type=(string)audio/x-gst-fourcc-ec_3, protection-system=(string)" PLAYREADY_UUID "; "
                        "application/x-cenc, original-media-type=(string)audio/mpeg, protection-system=(string)" PLAYREADY_UUID));

static GstStaticPadTemplate src_factory =         GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        GST_STATIC_CAPS("video/x-h264;video/x-h264(memory:SecMem);audio/mpeg;video/x-h265;video/x-h265(memory:SecMem);audio/x-eac3;audio/x-gst-fourcc-ec_3;audio/x-ac3"));

G_DEFINE_TYPE_WITH_CODE (GstDrmMultiDecryptor, gst_drm_multi_decryptor, GST_TYPE_ELEMENT,
        GST_DEBUG_CATEGORY_INIT (gst_drm_multi_decryptor_debug_category, "multidecryptor", 0,
                "debug category for multi decryptor element"));

static GstFlowReturn
gst_drm_multi_decryptor_process_buffers(GstObject * parent)
{
    GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(parent);
    GstFlowReturn ret = GST_FLOW_OK;
    g_mutex_lock(&self->mutex);

    std::vector<GstBuffer *> toSend;
    if (!self->bufferVec.empty()) {
        if (self->drmSession != NULL) {
             GST_WARNING_OBJECT(self, "calling decryption %d", self->bufferVec.size());
             int errorCode = self->drmSession->decrypt(self->bufferVec, self->sinkCaps);
             if (errorCode != 0 ) {
                 GST_ERROR_OBJECT(self, "decryption failed; error code %d",errorCode);
                 GError *error = g_error_new(GST_STREAM_ERROR , GST_STREAM_ERROR_FAILED, "Decrypt Error: code %d", errorCode);
                 gst_element_post_message(reinterpret_cast<GstElement*>(self), gst_message_new_error (GST_OBJECT (self), error, "Decrypt Failed"));
                 g_error_free(error);
                 ret = GST_FLOW_ERROR;
             }
        } else { /* If drmSession creation failed, then the call will be aborted here */
            ret = GST_FLOW_NOT_SUPPORTED;
            GST_ERROR_OBJECT(self, "drmSession is **** NULL ****, returning GST_FLOW_NOT_SUPPORTED");
        }
        if (ret == GstFlowReturn::GST_FLOW_OK) {
            for (auto &buff: self->bufferVec) {
                toSend.push_back(buff);
            }
        }
        self->bufferVec.resize(0);
    }
    g_mutex_unlock(&self->mutex);

    if (ret == GstFlowReturn::GST_FLOW_OK) {
        for (auto &buf: toSend) {
            GstProtectionMeta* protectionMeta = gst_buffer_get_protection_meta(buf);
            if (protectionMeta) {
                gst_buffer_remove_meta(buf, reinterpret_cast<GstMeta*>(protectionMeta));
            }

            ret = gst_pad_push (self->srcpad, buf);
            if (ret != GstFlowReturn::GST_FLOW_OK) {
                break;
            }
        }
    }
    return ret;
}

static GstFlowReturn
gst_drm_multi_decryptor_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(parent);
  GstFlowReturn ret = GST_FLOW_OK;
  GST_TRACE_OBJECT(self, "Received buffer ");

  std::shared_ptr<SocInterface> socInterface = SocInterface::CreateSocInterface();
  if ((gst_buffer_get_protection_meta(buf) == nullptr) && socInterface && socInterface->IsDecryptRequired()) {
      GstStructure *crypto_info = gst_structure_new ("protection_meta_info",
              "subsample_count", G_TYPE_UINT, 0,
              "subsamples", GST_TYPE_BUFFER, NULL,
              "iv", GST_TYPE_BUFFER, NULL,
              "kid", GST_TYPE_BUFFER, NULL,
              "initWithLast15", G_TYPE_UINT, 0,
              NULL);
      gst_buffer_add_protection_meta(buf, crypto_info);
  }

  if (gst_buffer_get_protection_meta(buf)) {
      int currentSize{};
      g_mutex_lock(&self->mutex);
      if (!self->keyReceived) {
          g_cond_wait(&self->condition, &self->mutex);
      }

      self->bufferVec.push_back(buf);
      currentSize = self->bufferVec.size();
      g_mutex_unlock(&self->mutex);

      // If queue is full - process the buffers
      if (currentSize >= self->batchSize) {
          ret = gst_drm_multi_decryptor_process_buffers(parent);
      }
  } else {
      ret = gst_pad_push(self->srcpad, buf);
  }

  return ret;
}

static void gst_drm_multi_decryptor_set_property(GObject * object,
        guint prop_id, const GValue * value, GParamSpec * pspec)
{
    GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(object);
    GST_INFO_OBJECT(self, "set property");
    switch (prop_id)
    {
    case PROP_DRM_SESSION_MANAGER:
        g_mutex_lock(&self->mutex);
        self->player = (DrmCallbacks*) g_value_get_pointer(value);
        GST_INFO_OBJECT(self, "Received DRM session manager");
        g_mutex_unlock(&self->mutex);
        break;
    case PROP_PLAYER:
        g_mutex_lock(&self->mutex);
        self->sessionManager = (DrmSessionManager*)g_value_get_pointer(value);
        GST_INFO_OBJECT(self, "Received player instancer");
        g_mutex_unlock(&self->mutex);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
        break;
    }
}
static gboolean
gst_drm_multi_decryptor_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
    GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(parent);
    gboolean ret = true;

    GST_INFO_OBJECT(self, "Received %s event: %" GST_PTR_FORMAT, GST_EVENT_TYPE_NAME (event), event);

    switch (GST_EVENT_TYPE (event)) {
        case GST_EVENT_CAPS:
        {
            gst_drm_multi_decryptor_process_buffers(parent);

            GstCaps* caps{};
            GstCaps* transformedCaps = gst_caps_new_empty();

            gst_event_parse_caps(event, &caps);
            g_mutex_lock(&self->mutex);

            if(!self->selectedProtection) {
                GstStructure *capstruct = gst_caps_get_structure(caps, 0);
                const gchar* capsinfo = gst_structure_get_string(capstruct, "protection-system");
                if(capsinfo != NULL) {
                    if(!g_strcmp0(capsinfo, PLAYREADY_UUID)) {
                        self->selectedProtection = PLAYREADY_UUID;
                    }
                    GST_INFO_OBJECT(self, "Selected protection system: %s", self->selectedProtection);
                }
            } else {
                GST_DEBUG_OBJECT(self, "can't find protection-system field from caps: %" GST_PTR_FORMAT, caps);
            }

            guint transformedCapsSize = 0;
            auto const size = gst_caps_get_size(caps);
            for (guint i = 0; i < size; ++i) {
                GstStructure* in = gst_caps_get_structure(caps, i);
                GstStructure* out{};

                auto name = gst_structure_get_name(in);
                if (!strcmp("application/x-cenc", name)) {
                    if (!gst_structure_has_field(in, "original-media-type")) {
                        continue;
                    }

                    out = gst_structure_copy(in);
                    gst_structure_set_name(out, gst_structure_get_string(out, "original-media-type"));
                    // remove protection related fields from the down-stream caps
                    gst_structure_remove_fields(out, "protection-system", "original-media-type", nullptr);
                } else {
                    // just pass input caps to output
                    out = gst_structure_copy(in);
                }

                for (guint j = 0; j < transformedCapsSize; ++j) {
                    GstStructure* s = gst_caps_get_structure(transformedCaps, j);
                    if (gst_structure_is_equal(out, s)) {
                        // skip duplicate structure
                        gst_structure_free(out);
                        out = nullptr;
                        break;
                    }
                }

                if (out) {
                    gst_caps_append_structure(transformedCaps, out);
                    ++transformedCapsSize;
                }
            }
            if (self->sinkCaps)
            {
                gst_caps_unref(self->sinkCaps);
                self->sinkCaps = NULL;
            }
            self->sinkCaps = gst_caps_copy(transformedCaps);
            g_mutex_unlock(&self->mutex);

            ret = gst_pad_set_caps(self->srcpad, transformedCaps);
            break;
        }
        case GST_EVENT_PROTECTION:
        {
            const gchar* systemId;
            const gchar* origin;
            GstBuffer* initdatabuffer;

            gst_event_parse_protection(event, &systemId, &initdatabuffer, &origin);
            GST_INFO_OBJECT(self, "Protection event for systemId: %s, origin %s", systemId, origin);
            if (!g_str_equal(systemId, self->selectedProtection))
            {
                GST_INFO_OBJECT(self, "Currently chosen systemId %s - ignoring event for %s", self->selectedProtection, systemId);
                gst_event_unref(event);
                ret = TRUE;
                break;
            }

            GstMapInfo mapInfo;
            if (!gst_buffer_map(initdatabuffer, &mapInfo, GST_MAP_READ)) {
                GST_INFO("Failed to map init data GstBuffer");
                break;
            }
            if (self->sessionManager == NULL) {
                GST_ERROR_OBJECT(self, "DRM Session manager is null!!!!");
                break;
            }

            if (eGST_MEDIATYPE_MANIFEST == self->mediaType)
            {
                GstCaps* caps = gst_pad_get_current_caps(self->sinkpad);
                GstStructure *capstruct = gst_caps_get_structure(caps, 0);
                const gchar* capsinfo = gst_structure_get_string(capstruct,
                        "original-media-type");

                if (!g_strcmp0(capsinfo, "audio/mpeg"))
                {
                    self->mediaType = eGST_MEDIATYPE_AUDIO;
                }
                else if (!g_strcmp0(capsinfo, "audio/x-opus"))
                {
                    self->mediaType = eGST_MEDIATYPE_AUDIO;
                }
                else if (!g_strcmp0(capsinfo, "audio/x-eac3") || !g_strcmp0(capsinfo, "audio/x-ac3"))
                {
                    self->mediaType = eGST_MEDIATYPE_AUDIO;
                }
                else if (!g_strcmp0(capsinfo, "audio/x-gst-fourcc-ec_3"))
                {
                    self->mediaType = eGST_MEDIATYPE_AUDIO;
                }
                else if (!g_strcmp0(capsinfo, "video/x-h264"))
                {
                    self->mediaType = eGST_MEDIATYPE_VIDEO;
                }
                else if (!g_strcmp0(capsinfo, "video/x-h265"))
                {
                    self->mediaType = eGST_MEDIATYPE_VIDEO;
                }
                else
                {
                    gst_caps_unref(caps);
                    ret = false;
                    break;
                }
                gst_caps_unref(caps);
            }

            g_mutex_lock(&self->mutex);
            std::shared_ptr<void> e = self->sessionManager->DrmMetaDataCb();
            int err = -1;
            int responseCode =-1;
            self->drmSession = self->sessionManager->createDrmSession(responseCode, err,
                        reinterpret_cast<const char *>(systemId), eMEDIAFORMAT_DASH,
                        reinterpret_cast<const unsigned char *>(mapInfo.data),
                        mapInfo.size, (int)self->mediaType, self->player, e.get(), nullptr, false);
            if(err != -1) {
                self->sessionManager->setfailureCb(e.get(),err);
            }
            if (self->drmSession == NULL) {
                if(SessionMgrState::eSESSIONMGR_ACTIVE == self->sessionManager->getSessionMgrState()) {
                    GST_ERROR_OBJECT(self, "Failed to create DRM Session");
                }
                ret = TRUE;
            } else {
                self->keyReceived = true;
                ret = TRUE;
                GST_INFO_OBJECT(self, "DRM session created");
            }
            g_cond_signal(&self->condition);
            g_mutex_unlock(&self->mutex);

            gst_buffer_unmap(initdatabuffer, &mapInfo);
            gst_event_unref(event);
            break;
        }

    case GST_EVENT_FLUSH_STOP:
        GST_INFO_OBJECT(self, "GST_EVENT_FLUSH_STOP");
        g_mutex_lock(&self->mutex);
        self->bufferVec.resize(0);
        g_mutex_unlock(&self->mutex);
        ret = gst_pad_event_default (pad, parent, event);
        break;

    case GST_EVENT_EOS:
    {
        GST_INFO_OBJECT(self, "GST_EVENT_EOS");

        gst_drm_multi_decryptor_process_buffers(parent);

        ret = gst_pad_event_default (pad, parent, event);
        break;
    }
    default:
        ret = gst_pad_event_default (pad, parent, event);
        break;
    }
    GST_INFO_OBJECT(self, "Done Received event");

    return ret;
}

static GstStateChangeReturn gst_drm_multi_decryptor_changestate(GstElement* element, GstStateChange transition)
{
    GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
    GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(element);

    ret = GST_ELEMENT_CLASS(gst_drm_multi_decryptor_parent_class)->change_state(element, transition);
    switch (transition)
    {
    case GST_STATE_CHANGE_PAUSED_TO_READY:
        GST_INFO_OBJECT(self, "PAUSED->READY");
        g_mutex_lock(&self->mutex);
        g_cond_signal(&self->condition);
        g_mutex_unlock(&self->mutex);
        break;
    default:
        break;
    }

    return ret;
}

void gst_drm_multi_decryptor_dispose(GObject * object)
{
    GstDrmMultiDecryptor *self = GST_DRMMULTIDECRYPTOR(object);
    GST_INFO_OBJECT(self, "dispose");

    g_mutex_clear(&self->mutex);
    g_cond_clear(&self->condition);
    self->bufferVec.resize(0);
    if (self->sinkCaps)
    {
        gst_caps_unref(self->sinkCaps);
        self->sinkCaps = NULL;
    }

    G_OBJECT_CLASS(gst_drm_multi_decryptor_parent_class)->dispose(object);
}

static void
gst_drm_multi_decryptor_init (GstDrmMultiDecryptor * self)
{
  self->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_event_function (self->sinkpad, GST_DEBUG_FUNCPTR(gst_drm_multi_decryptor_sink_event));
  gst_pad_set_chain_function (self->sinkpad, GST_DEBUG_FUNCPTR (gst_drm_multi_decryptor_chain));
  gst_element_add_pad (GST_ELEMENT (self), self->sinkpad);

  self->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  gst_element_add_pad (GST_ELEMENT (self), self->srcpad);

  self->batchSize = 20;
  self->sessionManager = NULL;
  self->drmSession = NULL;
  self->player = NULL;
  g_mutex_init(&self->mutex);
  g_cond_init(&self->condition);
  self->keyReceived = false;
  self->sinkCaps = NULL;
  self->selectedProtection = NULL;

  GST_INFO_OBJECT(self, "init batch size: %d", self->batchSize);

  if (self->bufferVec.capacity() < self->batchSize) {
      self->bufferVec.reserve(self->batchSize);
  }
}

static void
gst_drm_multi_decryptor_class_init (GstDrmMultiDecryptorClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *gstelement_class = GST_ELEMENT_CLASS (klass);

  gobject_class->set_property = gst_drm_multi_decryptor_set_property;
  gobject_class->dispose = gst_drm_multi_decryptor_dispose;

  GST_ELEMENT_CLASS(klass)->change_state = gst_drm_multi_decryptor_changestate;

  g_object_class_install_property(gobject_class, PROP_PLAYER,
          g_param_spec_pointer("aamp", "AAMP",
                  "DrmSessionManager instance for DrmCallback", G_PARAM_WRITABLE));

  g_object_class_install_property(gobject_class, PROP_DRM_SESSION_MANAGER,
          g_param_spec_pointer("drm-session-manager", "DRM Session Manager",
              "Pointer to DRM session manager", G_PARAM_WRITABLE));

  gst_element_class_add_static_pad_template (gstelement_class, &src_factory);
  gst_element_class_add_static_pad_template (gstelement_class, &sink_factory);

  gst_element_class_set_static_metadata(gstelement_class,
          "Custom Batched Decryptor",
          GST_ELEMENT_FACTORY_KLASS_DECRYPTOR,
          "Decrypts streams encrypted using DRM.",
          "Custom");

}
