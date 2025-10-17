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

#ifndef _GST_SUBTECMP4TRANSFORM_H_
#define _GST_SUBTECMP4TRANSFORM_H_

#include <gst/base/gstbasetransform.h>

// Declared static here because this string exists in player and gstplugin .so 
// library files  This string needs to match the start
// of the gsteamer plugin name as created by the macros.
static const char* GstPluginNameSubtecMp4Transform = "subtecmp4transform";

G_BEGIN_DECLS

/**
* @def GST_TYPE_SUBTECMP4TRANSFORM
* @brief Macro to retrieve the GType for GstSubtecMp4Transform.
*
* This macro returns the unique GType identifier for the GstSubtecMp4Transform
* element type, used for runtime type checking and registration within GStreamer.
*/
#define GST_TYPE_SUBTECMP4TRANSFORM   (gst_subtecmp4transform_get_type())
/**
 * @def GST_SUBTECMP4TRANSFORM
 * @brief Casts a generic GObject instance to a GstSubtecMp4Transform instance.
 *
 * This macro safely casts an object instance to a #GstSubtecMp4Transform pointer
 * after verifying its type at runtime.
 *
 * @param obj The object instance to cast.
 */
#define GST_SUBTECMP4TRANSFORM(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_SUBTECMP4TRANSFORM,GstSubtecMp4Transform))
/**
 * @def GST_SUBTECMP4TRANSFORM_CLASS
 * @brief Casts a generic GObject class to a GstSubtecMp4TransformClass.
 *
 * This macro safely casts a class instance to a #GstSubtecMp4TransformClass pointer
 * after verifying its type at runtime.
 *
 * @param klass The class instance to cast.
 */
#define GST_SUBTECMP4TRANSFORM_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_SUBTECMP4TRANSFORM,GstSubtecMp4TransformClass))
/**
 * @def GST_IS_SUBTECMP4TRANSFORM
 * @brief Checks whether an object is a GstSubtecMp4Transform instance.
 *
 * This macro returns TRUE if the given object is an instance of
 * #GstSubtecMp4Transform, or FALSE otherwise.
 *
 * @param obj The object instance to check.
 */
#define GST_IS_SUBTECMP4TRANSFORM(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_SUBTECMP4TRANSFORM))
/**
 * @def GST_IS_SUBTECMP4TRANSFORM_CLASS
 * @brief Checks whether a class is a GstSubtecMp4TransformClass type.
 *
 * This macro returns TRUE if the given class corresponds to
 * #GstSubtecMp4TransformClass, or FALSE otherwise.
 *
 * @param klass The class instance to check.
 */
#define GST_IS_SUBTECMP4TRANSFORM_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_SUBTECMP4TRANSFORM))

typedef struct _GstSubtecMp4Transform GstSubtecMp4Transform;
typedef struct _GstSubtecMp4TransformClass GstSubtecMp4TransformClass;

/**
* @struct _GstSubtecMp4Transform
* @brief Instance structure for the GstSubtecMp4Transform element.
*
* This structure represents an instance of the SubtecMp4Transform element,
* which extends #GstBaseTransform. The element performs subtitle or text
* transformation within an MP4 stream, typically handling format conversion
* or timestamp adjustment.
*
* @extends GstBaseTransform
*/
struct _GstSubtecMp4Transform
{
  GstBaseTransform base_subtecmp4transform;

};

/**
* @struct _GstSubtecMp4TransformClass
* @brief Class structure for the GstSubtecMp4Transform element.
*
* This structure defines the class metadata for the SubtecMp4Transform element,
* which extends #GstBaseTransformClass. It contains class-level information
* and virtual function pointers used by the GStreamer type system.
*
* @extends GstBaseTransformClass
*/
struct _GstSubtecMp4TransformClass
{
  GstBaseTransformClass base_subtecmp4transform_class;
};

/**
* @brief Retrieves the GType for the SubtecMp4Transform element.
*
* This function returns the unique GType identifier associated with the
* SubtecMp4Transform GStreamer element. It is used internally by the
* GObject type system for type registration, type checking, and class
* introspection.
*
* @return GType The registered type identifier for the SubtecMp4Transform element.
*/
GType gst_subtecmp4transform_get_type (void);

G_END_DECLS

#endif
