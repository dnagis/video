/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstmydropframes
 *
 * The mydropframes element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mydropframes ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideofilter.h>
#include "gstmydropframes.h"

GST_DEBUG_CATEGORY_STATIC (gst_my_drop_frames_debug_category);
#define GST_CAT_DEFAULT gst_my_drop_frames_debug_category

/* vvnx */
gint16 buffCount;

/* prototypes */


static void gst_my_drop_frames_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_my_drop_frames_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_my_drop_frames_dispose (GObject * object);
static void gst_my_drop_frames_finalize (GObject * object);

static gboolean gst_my_drop_frames_start (GstBaseTransform * trans);
static gboolean gst_my_drop_frames_stop (GstBaseTransform * trans);
static gboolean gst_my_drop_frames_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info);
static GstFlowReturn gst_my_drop_frames_transform_frame (GstVideoFilter * filter,
    GstVideoFrame * inframe, GstVideoFrame * outframe);
static GstFlowReturn gst_my_drop_frames_transform_frame_ip (GstVideoFilter * filter,
    GstVideoFrame * frame);

enum
{
  PROP_0,
  PROP_START_FRAME, /* vvnx */
  PROP_END_FRAME /* vvnx */
};

/* pad templates */

/* FIXME: add/remove formats you can handle */
#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")

/* FIXME: add/remove formats you can handle */
#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMyDropFrames, gst_my_drop_frames, GST_TYPE_VIDEO_FILTER,
  GST_DEBUG_CATEGORY_INIT (gst_my_drop_frames_debug_category, "mydropframes", 0,
  "debug category for mydropframes element"));

static void
gst_my_drop_frames_class_init (GstMyDropFramesClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstBaseTransformClass *base_transform_class = GST_BASE_TRANSFORM_CLASS (klass);
  GstVideoFilterClass *video_filter_class = GST_VIDEO_FILTER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SRC_CAPS)));
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS(klass),
      gst_pad_template_new ("sink", GST_PAD_SINK, GST_PAD_ALWAYS,
        gst_caps_from_string (VIDEO_SINK_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS(klass),
      "FIXME Long name", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");
  


  gobject_class->set_property = gst_my_drop_frames_set_property;
  gobject_class->get_property = gst_my_drop_frames_get_property;
  gobject_class->dispose = gst_my_drop_frames_dispose;
  gobject_class->finalize = gst_my_drop_frames_finalize;
  base_transform_class->start = GST_DEBUG_FUNCPTR (gst_my_drop_frames_start);
  base_transform_class->stop = GST_DEBUG_FUNCPTR (gst_my_drop_frames_stop);
  video_filter_class->set_info = GST_DEBUG_FUNCPTR (gst_my_drop_frames_set_info);
  video_filter_class->transform_frame = GST_DEBUG_FUNCPTR (gst_my_drop_frames_transform_frame);
  video_filter_class->transform_frame_ip = GST_DEBUG_FUNCPTR (gst_my_drop_frames_transform_frame_ip);

  //vvnx    
  g_object_class_install_property (gobject_class, PROP_START_FRAME, g_param_spec_int ("start-frame", "Du texte 1", "Du texte 2", 0, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (gobject_class, PROP_END_FRAME, g_param_spec_int ("end-frame", "Du texte 1", "Du texte 2", 0, G_MAXINT, 0, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

}

static void
gst_my_drop_frames_init (GstMyDropFrames *mydropframes)
{
}

void
gst_my_drop_frames_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (object);

  GST_DEBUG_OBJECT (mydropframes, "set_property");

  switch (property_id) {
	case PROP_START_FRAME:
		mydropframes->start_frame = g_value_get_int (value);
		g_print ("start-frame argument was changed to %i\n", mydropframes->start_frame);
		break;
	case PROP_END_FRAME:
		mydropframes->end_frame = g_value_get_int (value);
		g_print ("start-frame argument was changed to %i\n", mydropframes->end_frame);
		break;	  
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_my_drop_frames_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (object);

  GST_DEBUG_OBJECT (mydropframes, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_my_drop_frames_dispose (GObject * object)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (object);

  GST_DEBUG_OBJECT (mydropframes, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_my_drop_frames_parent_class)->dispose (object);
}

void
gst_my_drop_frames_finalize (GObject * object)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (object);

  GST_DEBUG_OBJECT (mydropframes, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_my_drop_frames_parent_class)->finalize (object);
}

static gboolean
gst_my_drop_frames_start (GstBaseTransform * trans)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (trans);

  GST_DEBUG_OBJECT (mydropframes, "start");

  return TRUE;
}

static gboolean
gst_my_drop_frames_stop (GstBaseTransform * trans)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (trans);

  GST_DEBUG_OBJECT (mydropframes, "stop");

  return TRUE;
}

static gboolean
gst_my_drop_frames_set_info (GstVideoFilter * filter, GstCaps * incaps,
    GstVideoInfo * in_info, GstCaps * outcaps, GstVideoInfo * out_info)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (filter);

  GST_DEBUG_OBJECT (mydropframes, "set_info");

  return TRUE;
}

/* transform */
static GstFlowReturn
gst_my_drop_frames_transform_frame (GstVideoFilter * filter, GstVideoFrame * inframe,
    GstVideoFrame * outframe)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (filter);

  GST_DEBUG_OBJECT (mydropframes, "transform_frame et la valeur de start frame = %i", mydropframes->start_frame);
  GST_DEBUG_OBJECT (mydropframes, "transform_frame et la valeur de end frame = %i", mydropframes->end_frame);

  //return GST_FLOW_OK;
 
  buffCount ++;
    
  // gint16 start = 30;
  // gint16 end = 70;
  
  if ((buffCount < mydropframes->start_frame) || (buffCount > mydropframes->end_frame)) {
	  GST_DEBUG_OBJECT (mydropframes, "Condition OFF = block drop frame");
	  //return GST_FLOW_OK;
	  return GST_BASE_TRANSFORM_FLOW_DROPPED;
  } else { 
	  GST_DEBUG_OBJECT (mydropframes, "Condition ON = copy frame");
	  gst_video_frame_copy (outframe, inframe);
	  return GST_FLOW_OK;
  };
  
  
  
  
  
}

static GstFlowReturn
gst_my_drop_frames_transform_frame_ip (GstVideoFilter * filter, GstVideoFrame * frame)
{
  GstMyDropFrames *mydropframes = GST_MY_DROP_FRAMES (filter);

  GST_DEBUG_OBJECT (mydropframes, "transform_frame_ip");

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{

  /* FIXME Remember to set the rank if it's an element that is meant
     to be autoplugged by decodebin. */
  return gst_element_register (plugin, "mydropframes", GST_RANK_NONE,
      GST_TYPE_MY_DROP_FRAMES);
}

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    mydropframes,
    "FIXME plugin description",
    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)

