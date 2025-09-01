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
 * GST_TYPE_SUBTECSINK:
 *
 * Macro to get the GType for GstSubtecSink.
 */
#define GST_TYPE_SUBTECSINK   (gst_subtecsink_get_type())

/**
 * GST_SUBTECSINK:
 * @obj: Object to cast.
 *
 * Macro to cast an object to GstSubtecSink.
 */
#define GST_SUBTECSINK(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SUBTECSINK,GstSubtecSink))

/**
 * GST_SUBTECSINK_CLASS:
 * @klass: Class to cast.
 *
 * Macro to cast a class to GstSubtecSinkClass.
 */
#define GST_SUBTECSINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SUBTECSINK,GstSubtecSinkClass))

/**
 * GST_IS_SUBTECSINK:
 * @obj: Object to check.
 *
 * Macro to check if an object is of type GstSubtecSink.
 */
#define GST_IS_SUBTECSINK(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SUBTECSINK))

/**
 * GST_IS_SUBTECSINK_CLASS:
 * @klass: Class to check.
 *
 * Macro to check if a class is of type GstSubtecSinkClass.
 */
#define GST_IS_SUBTECSINK_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SUBTECSINK))

/**
 * GstSubtecSink:
 * @base_subtecsink: Parent GstBaseSink structure.
 * @m_channel: Unique pointer to SubtecChannel.
 * @m_mute: Mute flag.
 * @m_no_eos: No end-of-stream flag.
 * @m_send_timestamp: Flag to indicate if timestamp should be sent.
 * @m_segmentstart: Segment start value.
 * @m_pts_offset: Presentation timestamp offset.
 * @m_subtec_socket: Subtec socket string.
 * @m_attribute_values: Attribute values.
 * @m_attribute_mask: Attribute mask.
 *
 * Structure representing the GstSubtecSink element.
 */
typedef struct _GstSubtecSink GstSubtecSink;
typedef struct _GstSubtecSinkClass GstSubtecSinkClass;

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
 * GstSubtecSinkClass:
 * @base_subtecsink_class: Parent GstBaseSinkClass structure.
 *
 * Class structure for GstSubtecSink.
 */
struct _GstSubtecSinkClass
{
  GstBaseSinkClass base_subtecsink_class;
};

/**
 * gst_subtecsink_get_type:
 *
 * Returns the GType for GstSubtecSink.
 *
 * Returns: The GType of GstSubtecSink.
 */
GType gst_subtecsink_get_type (void);

G_END_DECLS

#endif
