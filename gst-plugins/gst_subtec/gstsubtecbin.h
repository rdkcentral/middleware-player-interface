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
 * @def GST_TYPE_SUBTECBIN
 * @brief Macro to retrieve the GType for GstSubtecBin.
 *
 * This macro returns the unique GType identifier for the GstSubtecBin
 * element type, used for runtime type checking and registration within
 * the GStreamer framework.
 */
#define GST_TYPE_SUBTECBIN   (gst_subtecbin_get_type())
/**
 * @def GST_SUBTECBIN
 * @brief Casts a generic GObject instance to a GstSubtecBin instance.
 *
 * This macro safely casts an object instance to a #GstSubtecBin pointer
 * after verifying its type at runtime.
 *
 * @param obj The object instance to cast.
 */
#define GST_SUBTECBIN(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SUBTECBIN,GstSubtecBin))
/**
 * @def GST_SUBTECBIN_CLASS
 * @brief Casts a generic GObject class to a GstSubtecBinClass.
 *
 * This macro safely casts a class instance to a #GstSubtecBinClass pointer
 * after verifying its type at runtime.
 *
 * @param klass The class instance to cast.
 */
#define GST_SUBTECBIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SUBTECBIN,GstSubtecBinClass))
/**
 * @def GST_IS_SUBTECBIN
 * @brief Checks whether an object is a GstSubtecBin instance.
 *
 * This macro returns TRUE if the given object is an instance of
 * #GstSubtecBin, or FALSE otherwise.
 *
 * @param obj The object instance to check.
 */
#define GST_IS_SUBTECBIN(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SUBTECBIN))
/**
 * @def GST_IS_SUBTECBIN_CLASS
 * @brief Checks whether a class is a GstSubtecBinClass type.
 *
 * This macro returns TRUE if the given class corresponds to
 * #GstSubtecBinClass, or FALSE otherwise.
 *
 * @param klass The class instance to check.
 */
#define GST_IS_SUBTECBIN_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SUBTECBIN))

typedef struct _GstSubtecBin GstSubtecBin;
typedef struct _GstSubtecBinClass GstSubtecBinClass;

/**
 * @struct _GstSubtecBin
 * @brief Instance structure for the GstSubtecBin element.
 *
 * This structure represents an instance of the SubTecBin element, which
 * extends #GstBin. The element manages a sub-pipeline for subtitle or
 * text-based processing, including typefinding, demuxing, formatting,
 * and rendering.
 *
 * It contains references to internal GStreamer elements and control flags
 * for runtime behavior such as mute state, synchronization, and EOS handling.
 *
 * @extends GstBin
 */
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
 * @struct _GstSubtecBinClass
 * @brief Class structure for the GstSubtecBin element.
 *
 * This structure defines the class metadata for the SubTecBin element,
 * which extends the base #GstBinClass. It contains class-level
 * information and function pointers used by the GStreamer type system.
 *
 * @extends GstBinClass
 */
struct _GstSubtecBinClass
{
  GstBinClass parent_class;
};

/**
 * @brief Retrieves the GType for the SubTecBin element.
 *
 * This function returns the unique GType identifier associated with
 * the SubTecBin GStreamer element. It is used internally by the GObject
 * type system for type registration and introspection.
 *
 * @return GType The registered type identifier for the SubTecBin element.
 */
GType gst_subtecbin_get_type (void);

G_END_DECLS

#endif
