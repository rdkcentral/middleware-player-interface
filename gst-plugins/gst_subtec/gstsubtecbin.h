/*
 * Copyright (C) 2022 RDK Management
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GST_SUBTECBIN_H_
#define _GST_SUBTECBIN_H_

#include <gst/gst.h>
#include <string>

// Declared static here because this string exists in player and gstplugin .so
// library files  This string needs to match the start
// of the gsteamer plugin name as created by the macros.
static const char* GstPluginNameSubtecBin = "subtecbin";

G_BEGIN_DECLS

/**
 * GST_TYPE_SUBTECBIN:
 *
 * Macro to get the GType for GstSubtecBin.
 */
#define GST_TYPE_SUBTECBIN   (gst_subtecbin_get_type())

/**
 * GST_SUBTECBIN:
 * @obj: Object to cast.
 *
 * Macro to cast an object to GstSubtecBin.
 */
#define GST_SUBTECBIN(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SUBTECBIN,GstSubtecBin))

/**
 * GST_SUBTECBIN_CLASS:
 * @klass: Class to cast.
 *
 * Macro to cast a class to GstSubtecBinClass.
 */
#define GST_SUBTECBIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SUBTECBIN,GstSubtecBinClass))

/**
 * GST_IS_SUBTECBIN:
 * @obj: Object to check.
 *
 * Macro to check if an object is of type GstSubtecBin.
 */
#define GST_IS_SUBTECBIN(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SUBTECBIN))

/**
 * GST_IS_SUBTECBIN_CLASS:
 * @klass: Class to check.
 *
 * Macro to check if a class is of type GstSubtecBinClass.
 */
#define GST_IS_SUBTECBIN_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SUBTECBIN))

/**
 * GstSubtecBin:
 * @parent: Parent GstBin structure.
 * @sinkpad: Sink pad for the bin.
 * @typefind: Typefind element.
 * @demux: Demux element.
 * @formatter: Formatter element.
 * @sink: Sink element.
 * @no_eos: Flag to indicate no end-of-stream.
 * @mute: Flag to indicate mute state.
 * @async: Flag to indicate asynchronous operation.
 * @sync: Flag to indicate synchronous operation.
 * @subtec_socket: Socket string for subtec communication.
 * @pts_offset: Presentation timestamp offset.
 *
 * Structure representing the GstSubtecBin element.
 */
typedef struct _GstSubtecBin GstSubtecBin;
typedef struct _GstSubtecBinClass GstSubtecBinClass;

struct _GstSubtecBin
{
  GstBin      parent;
  GstPad      *sinkpad;
  GstElement  *typefind;
  GstElement  *demux;
  GstElement  *formatter;
  GstElement  *sink;
  bool        no_eos = false;
  bool        mute = false;
  bool        async = true;
  bool        sync = true;
  std::string subtec_socket{};
  guint64     pts_offset{0};
};

/**
 * GstSubtecBinClass:
 * @parent_class: Parent GstBinClass structure.
 *
 * Class structure for GstSubtecBin.
 */
struct _GstSubtecBinClass
{
  GstBinClass parent_class;
};

/**
 * gst_subtecbin_get_type:
 *
 * Returns the GType for GstSubtecBin.
 *
 * Returns: The GType of GstSubtecBin.
 */
GType gst_subtecbin_get_type (void);

G_END_DECLS

#endif
