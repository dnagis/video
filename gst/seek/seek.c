/**
 * Basé sur: 
 * https://gstreamer.freedesktop.org/documentation/tutorials/basic/hello-world.html?gi-language=c
 * 
 * gcc seek.c -o seek `pkg-config --cflags --libs gstreamer-1.0`
 * 
 **/



#include <gst/gst.h>

int
main (int argc, char *argv[])
{
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  GstEvent *seek_event;
  gboolean terminated;
  
  terminated=FALSE;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! vaapih264dec ! vaapih264enc ! h264parse ! mp4mux ! filesink location=out.mp4", NULL);
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! vaapih264dec ! vaapisink", NULL);
  pipeline = gst_parse_launch("playbin uri=file:///root/in.mp4", NULL);

  seek_event =
        gst_event_new_seek (1.0, GST_FORMAT_TIME, 
        GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE, GST_SEEK_TYPE_SET,
        1 * GST_SECOND , GST_SEEK_TYPE_SET, 2 * GST_SECOND);
        

        
  /* Start playing */
  //gst_element_set_state (pipeline, GST_STATE_READY);  
  //gst_element_set_state (pipeline, GST_STATE_PAUSED);
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  


  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  //msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  
  
  
  
  
  //Espoir: de placer un seek dans la boucle ci dessou
  //gst_element_send_event (pipeline, seek_event);  
  //gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 2 * GST_SECOND);
  
  
  
    do {
    msg = gst_bus_pop (bus);

    
    if (msg != NULL) {
      
      
      g_print(gst_message_type_get_name(msg->type));
      
      
    } 
  } while (!terminated);
  
  
   


  /* Free resources */
  gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
