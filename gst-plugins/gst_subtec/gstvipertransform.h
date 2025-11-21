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

#ifndef _GST_VIPERTRANSFORM_H_
#define _GST_VIPERTRANSFORM_H_

#include <gst/base/gstbasetransform.h>

#include <cinttypes>

// Declared static here because this string exists in player and gstplugin .so
// library files  This string needs to match the start
// of the gsteamer plugin name as created by the macros.
static const char* GstPluginNameViperTransform = "vipertransform";

G_BEGIN_DECLS

/**
 * GST_TYPE_VIPERTRANSFORM:
 *
 * Macro to get the GType for GstViperTransform.
 */
#define GST_TYPE_VIPERTRANSFORM   (gst_vipertransform_get_type())

/**
 * GST_VIPERTRANSFORM:
 * @obj: Object to cast.
 *
 * Macro to cast an object to GstViperTransform.
 */
#define GST_VIPERTRANSFORM(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIPERTRANSFORM,GstViperTransform))

/**
 * GST_VIPERTRANSFORM_CLASS:
 * @klass: Class to cast.
 *
 * Macro to cast a class to GstViperTransformClass.
 */
#define GST_VIPERTRANSFORM_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIPERTRANSFORM,GstViperTransformClass))

/**
 * GST_IS_VIPERTRANSFORM:
 * @obj: Object to check.
 *
 * Macro to check if an object is of type GstViperTransform.
 */
#define GST_IS_VIPERTRANSFORM(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIPERTRANSFORM))

/**
 * GST_IS_VIPERTRANSFORM_CLASS:
 * @klass: Class to check.
 *
 * Macro to check if a class is of type GstViperTransformClass.
 */
#define GST_IS_VIPERTRANSFORM_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIPERTRANSFORM))

/**
 * GstViperTransform:
 * @base_vipertransform: Parent GstBaseTransform structure.
 * @m_content_type: Content type for Viper processing.
 * @m_linear_begin_offset: Linear begin offset value.
 * @m_search_first_begin: Flag indicating if first begin should be searched.
 *
 * Structure representing the GstViperTransform element.
 */
typedef struct _GstViperTransform GstViperTransform;
typedef struct _GstViperTransformClass GstViperTransformClass;

/**
 * ViperContentType:
 * @UNKNOWN: Unknown content type.
 * @PASSTHROUGH: Passthrough content type.
 * @LINEAR_OFFSET: Linear offset content type.
 * @LINEAR_OFFSET_PRELIM: Linear offset preliminary content type.
 * @HARMONIC_UHD: Harmonic UHD content type.
 *
 * Enum representing the content type for Viper processing.
 */
enum class ViperContentType
{
  UNKNOWN,
  PASSTHROUGH,
  LINEAR_OFFSET,
  LINEAR_OFFSET_PRELIM,
  HARMONIC_UHD
};

/**
 * GstViperTransform:
 * @base_vipertransform: Parent GstBaseTransform structure.
 * @m_content_type: Content type for Viper processing.
 * @m_linear_begin_offset: Linear begin offset value.
 * @m_search_first_begin: Flag indicating if first begin should be searched.
 *
 * Structure representing the GstViperTransform element.
 */
struct _GstViperTransform
{
  GstBaseTransform  base_vipertransform;
  ViperContentType  m_content_type        {ViperContentType::UNKNOWN};
  gint64            m_linear_begin_offset {0};
  gboolean          m_search_first_begin  {true};
};

/**
 * GstViperTransformClass:
 * @base_vipertransform_class: Parent GstBaseTransformClass structure.
 *
 * Class structure for GstViperTransform.
 */
struct _GstViperTransformClass
{
  GstBaseTransformClass base_vipertransform_class;
};

/**
 * gst_vipertransform_get_type:
 *
 * Returns the GType for GstViperTransform.
 *
 * Returns: The GType of GstViperTransform.
 */
GType gst_vipertransform_get_type (void);

G_END_DECLS

#endif
