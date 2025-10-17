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

#ifndef _GST_SUBTECSINK_H_
#define _GST_SUBTECSINK_H_

#include <gst/base/gstbasesink.h>
#include <string>
#include <cstdint>
#include <array>
#include "SubtecChannel.hpp"

G_BEGIN_DECLS
/**
 * @def GST_TYPE_SUBTECSINK
 * @brief Macro to retrieve the GType for GstSubtecSink.
 *
 * This macro returns the unique GType identifier for the GstSubtecSink
 * element type, used for runtime type checking and registration within GStreamer.
 */
#define GST_TYPE_SUBTECSINK   (gst_subtecsink_get_type())
/**
 * @def GST_SUBTECSINK
 * @brief Casts a generic GObject instance to a GstSubtecSink instance.
 *
 * This macro safely casts an object instance to a #GstSubtecSink pointer
 * after verifying its type at runtime.
 *
 * @param obj The object instance to cast.
 */
#define GST_SUBTECSINK(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SUBTECSINK,GstSubtecSink))
/**
 * @def GST_SUBTECSINK_CLASS
 * @brief Casts a generic GObject class to a GstSubtecSinkClass.
 *
 * This macro safely casts a class instance to a #GstSubtecSinkClass pointer
 * after verifying its type at runtime.
 *
 * @param klass The class instance to cast.
 */
#define GST_SUBTECSINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SUBTECSINK,GstSubtecSinkClass))
/**
 * @def GST_IS_SUBTECSINK
 * @brief Checks whether an object is a GstSubtecSink instance.
 *
 * This macro returns TRUE if the given object is an instance of
 * #GstSubtecSink, or FALSE otherwise.
 *
 * @param obj The object instance to check.
 */
#define GST_IS_SUBTECSINK(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SUBTECSINK))
/**
 * @def GST_IS_SUBTECSINK_CLASS
 * @brief Checks whether a class is a GstSubtecSinkClass type.
 *
 * This macro returns TRUE if the given class corresponds to
 * #GstSubtecSinkClass, or FALSE otherwise.
 *
 * @param klass The class instance to check.
 */
#define GST_IS_SUBTECSINK_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SUBTECSINK))

typedef struct _GstSubtecSink GstSubtecSink;
typedef struct _GstSubtecSinkClass GstSubtecSinkClass;

/**
 * @struct _GstSubtecSink
 * @brief Instance structure for the GstSubtecSink element.
 *
 * This structure represents an instance of the SubtecSink element, which
 * extends #GstBaseSink. It manages subtitle or text output, including
 * channel handling, muting, timestamp management, and attribute control.
 *
 * @extends GstBaseSink
 */
struct _GstSubtecSink
{
  GstBaseSink base_subtecsink;

	std::unique_ptr<SubtecChannel> m_channel;
  gboolean m_mute;
  gboolean m_no_eos;
  gboolean m_send_timestamp{true};
  guint64  m_segmentstart{0};
  guint64  m_pts_offset{0};
  std::string   m_subtec_socket{};
  attributesType m_attribute_values{0};
  guint m_attribute_mask{0};
};
/**
 * @struct _GstSubtecSinkClass
 * @brief Class structure for the GstSubtecSink element.
 *
 * This structure defines the class-level metadata for the SubtecSink element,
 * which extends #GstBaseSinkClass. It contains class-specific information
 * and virtual function pointers used by the GStreamer type system.
 *
 * @extends GstBaseSinkClass
 */
struct _GstSubtecSinkClass
{
  GstBaseSinkClass base_subtecsink_class;
};

/**
 * @brief Retrieves the GType for the SubtecSink element.
 *
 * This function returns the unique GType identifier associated with the
 * SubtecSink GStreamer element. It is used by the GObject type system
 * for type registration, type checking, and class introspection.
 *
 * @return GType The registered type identifier for the SubtecSink element.
 */
GType gst_subtecsink_get_type (void);

G_END_DECLS

#endif
