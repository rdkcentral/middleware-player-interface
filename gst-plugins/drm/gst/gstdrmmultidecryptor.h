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

#ifndef _GST_DRMMULTIDECRYPTOR_H_
#define _GST_DRMMULTIDECRYPTOR_H_

#include <gst/gst.h>
#include "DrmSessionManager.h"
#include "DrmCallbacks.h"
#include <vector>

static const char* GstPluginNameMULTI = "drmmultidecryptor";

G_BEGIN_DECLS

#define GST_TYPE_DRM_MULTI_DECRYPTOR (gst_drm_multi_decryptor_get_type())
#define GST_DRMMULTIDECRYPTOR(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_DRM_MULTI_DECRYPTOR, GstDrmMultiDecryptor))
#define GST_DRMMULTIDECRYPTOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), GST_TYPE_DRM_MULTI_DECRYPTOR, GstDrmMultiDecryptorClass))
#define GST_IS_DRMMULTIDECRYPTOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), GST_TYPE_DRM_MULTI_DECRYPTOR))
#define GST_IS_DRMMULTIDECRYPTOR_CLASS(obj)    (G_TYPE_CHECK_CLASS_TYPE((klass), GST_TYPE_DRM_MULTI_DECRYPTOR))

typedef struct _GstDrmMultiDecryptor GstDrmMultiDecryptor;
typedef struct _GstDrmMultiDecryptorClass GstDrmMultiDecryptorClass;

/**
 * @struct _GstDrmMultiDecryptor 
 * @brief GstElement structure override for multi-decryptor
 */
struct _GstDrmMultiDecryptor {
  GstElement element;

  GstPad *sinkpad;
  GstPad *srcpad;

  std::vector<GstBuffer*> bufferVec;
  guint batchSize;

  class DrmSessionManager*    sessionManager;
  class DrmSession*           drmSession;
  class DrmCallbacks *        player;
  GstMediaType                mediaType;

  gboolean                    keyReceived;

  GMutex                      mutex;
  GCond                       condition;
  GstCaps*                    sinkCaps;
  const gchar*                selectedProtection;

};

struct _GstDrmMultiDecryptorClass {
  GstElementClass parent_class;
};

GType gst_drm_multi_decryptor_get_type (void);

G_END_DECLS


#endif
