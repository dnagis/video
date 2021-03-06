//export PKG_CONFIG_PATH=/usr/X11R7/lib/pkgconfig:/usr/X11R7/share/pkgconfig/:/usr/share/pkgconfig/
//gcc basic2.c -o basic2 `pkg-config --cflags --libs gstreamer-1.0`


#include <gst/gst.h>

typedef struct _CustomData
{
  GstElement *pipeline;
  GstElement *source;
  GstElement *decode;
  GstElement *time;
  GstElement *sink;
} CustomData;


/* déclaration de l'Handler for the pad-added signal */
static void pad_added_handler (GstElement * src, GstPad * pad, CustomData * data);

int
main (int argc, char *argv[])
{
  CustomData data;	
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Create the elements */
  //source = gst_element_factory_make ("videotestsrc", "source");
  
  //gst-launch-1.0 filesrc location=174.mp4 ! decodebin ! vaapisink --> le graal
  //gst-launch-1.0 filesrc location=174.mp4 ! autovideosink -> marche pas (sink-vaapi: failed to validate source buffer)
  data.source = gst_element_factory_make ("filesrc", "source");
  data.decode = gst_element_factory_make ("decodebin", "decode");
  data.time = gst_element_factory_make ("timeoverlay", "time");
  data.sink = gst_element_factory_make ("autovideosink", "sink");

  /* Create the empty pipeline */
  data.pipeline = gst_pipeline_new ("test-pipeline");

  if (!data.pipeline || !data.source || !data.decode || !data.time || !data.sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  /* Build the pipeline mais on ne linke pas encore le sink (le decode a pas de src_pad initialement)*/
  gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.decode, data.time, data.sink, NULL);
  
  //Attention grosse astuce: gst_element_link_many plante si yen a que deux!
  if (gst_element_link (data.source, data.decode) != TRUE) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }
  
   if (gst_element_link (data.time, data.sink) != TRUE) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }
  
  /* Modify the source's properties */
  g_object_set (data.source, "location", "174.mp4", NULL);
  
  /* Connect to the pad-added signal */
  g_signal_connect (data.decode, "pad-added", G_CALLBACK (pad_added_handler), &data);

  /* Start playing */
  ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }

  /* Wait until error or EOS */
  bus = gst_element_get_bus (data.pipeline);
  msg =
      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Parse message */
  if (msg != NULL) {
    GError *err;
    gchar *debug_info;

    switch (GST_MESSAGE_TYPE (msg)) {
      case GST_MESSAGE_ERROR:
        gst_message_parse_error (msg, &err, &debug_info);
        g_printerr ("Error received from element %s: %s\n",
            GST_OBJECT_NAME (msg->src), err->message);
        g_printerr ("Debugging information: %s\n",
            debug_info ? debug_info : "none");
        g_clear_error (&err);
        g_free (debug_info);
        break;
      case GST_MESSAGE_EOS:
        g_print ("End-Of-Stream reached.\n");
        break;
      default:
        /* We should not reach here because we only asked for ERRORs and EOS */
        g_printerr ("Unexpected message received.\n");
        break;
    }
    gst_message_unref (msg);
  }

  /* Free resources */
  gst_object_unref (bus);
  gst_element_set_state (data.pipeline, GST_STATE_NULL);
  gst_object_unref (data.pipeline);
  return 0;
}

/* This function will be called by the pad-added signal */
static void
pad_added_handler (GstElement * src, GstPad * new_pad, CustomData * data)
{
  GstPad *sink_pad = gst_element_get_static_pad (data->time, "video_sink"); //l'arg 2 c'est le nom que tu vas trouver dans gst-inspect à pad-templates

  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));
  
  gst_pad_link (new_pad, sink_pad);

 
}
