/**
 * 
 * Récupérer des valeurs de quantification d'image dans un bus message
 * 
 * Basic tutorial 1: Hello world! mais pour la gestion des messages: la partie "Parse message" de Basic tutorial 3: Dynamic pipelines
 * 
 * gcc gst_msg.c -o gst_msg `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0`
 * 
 **/



#include <gst/gst.h>

int
main (int argc, char *argv[])
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  gboolean terminate = FALSE;
  
  g_print ("Hello.\n");

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline 
   * gst-launch-1.0 -m v4l2src ! videoconvert ! videoanalyse  ! fakesink
   * */
  pipeline = gst_parse_launch("v4l2src ! videoconvert ! videoanalyse  ! fakesink", NULL);

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  




  /*msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  if (msg != NULL) gst_message_unref (msg);*/
  
/**greffe fécale de basic tuto 3 : Dynamic Pipelines*/  
   do {
    msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
        GST_MESSAGE_ERROR | GST_MESSAGE_EOS | GST_MESSAGE_ELEMENT);

    /* Parse message */
    if (msg != NULL) {
      GError *err;
      gchar *debug_info;
      const GstStructure *s;
      const gchar *name;
      const GValue *value;

      switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_ERROR:
          gst_message_parse_error (msg, &err, &debug_info);
          g_printerr ("Error received from element %s: %s\n", GST_OBJECT_NAME (msg->src), err->message);
          g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
          g_clear_error (&err);
          g_free (debug_info);
          terminate = TRUE;
          break;
        case GST_MESSAGE_EOS:
          g_print ("End-Of-Stream reached.\n");
          terminate = TRUE;
          break;
        case GST_MESSAGE_ELEMENT:
          s = gst_message_get_structure (msg);
          name = gst_structure_get_name (s);
          value = gst_structure_get_value (s, "luma-average"); // G_VALUE_TYPE_NAME(value) en %s = gdouble          
          //value = gst_structure_get_value (s, "duration"); // G_VALUE_TYPE_NAME(value) en %s = guint64          
          g_print ("Rx element msg, name struct=%s, valeur=%f\n", name, g_value_get_double(value));          
		  break;
        default:
          /* We should not reach here */
          g_printerr ("Unexpected message received.\n");
          break;
      }
      gst_message_unref (msg);
    }
  } while (!terminate);
  
  

    
    
    
    
    
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
