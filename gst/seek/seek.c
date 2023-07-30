/**
 * Basé sur: 
 * https://gstreamer.freedesktop.org/documentation/tutorials/basic/hello-world.html?gi-language=c
 * 
 * gcc seek.c -o seek `pkg-config --cflags --libs gstreamer-1.0`
 * 
 * export PATH=$PATH:/initrd/mnt/dev_save/rpi/cross/bin/
 * export PKG_CONFIG_LIBDIR=/initrd/mnt/dev_save/rpi/cross/aarch64-linux-gnu/lib/pkgconfig
 * export PKG_CONFIG_SYSROOT_DIR=/initrd/mnt/dev_save/rpi/cross/aarch64-linux-gnu
 * aarch64-linux-gnu-gcc seek.c -o seek `pkg-config --cflags --libs gstreamer-1.0`
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
  gboolean terminate;
  gboolean seek_done;
  
  terminate=FALSE;
  seek_done=FALSE;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  
  //création des fichiers videos pour l'input cf au début de LOG_seek_main
  
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! vaapih264dec ! vaapih264enc ! h264parse ! mp4mux ! filesink location=out.mp4", NULL);
  
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! vaapih264dec ! vaapih264enc ! filesink location=out.h264", NULL);
  
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! mp4mux ! filesink location=out.mp4", NULL);
  
  pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! filesink location=out.h264", NULL);
  
  
  
  //pipeline = gst_parse_launch("filesrc location=video.mp4 ! qtdemux ! h264parse ! v4l2h264dec ! v4l2h264enc ! video/x-h264,level=(string)3 ! h264parse ! mp4mux ! filesink location=out.mp4", NULL);
 
  //pipeline = gst_parse_launch("filesrc location=in.mp4 ! qtdemux ! h264parse ! vaapih264dec ! vaapisink", NULL);
  //pipeline = gst_parse_launch("playbin uri=file:///root/in.mp4", NULL);

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
  
  
  
  
  
  //Espoir: de placer un seek dans la boucle ci dessous
  //gst_element_send_event (pipeline, seek_event);  
  //gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 2 * GST_SECOND);
  
  
  
    do {
    msg = gst_bus_pop (bus);

    
    if (msg != NULL) {
      
      
      g_print("msg=%s\n",gst_message_type_get_name(msg->type));
      
		switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:
          g_print ("End-Of-Stream reached.\n");
          terminate = TRUE;
          break;
        case GST_MESSAGE_STATE_CHANGED:
			          /* We are only interested in state-changed messages from the pipeline */
          if (GST_MESSAGE_SRC (msg) == GST_OBJECT (pipeline)) {
            GstState old_state, new_state, pending_state;
            gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
            g_print ("Pipeline state changed from %s to %s:\n",
                gst_element_state_get_name (old_state), gst_element_state_get_name (new_state));
                
           if ( old_state == GST_STATE_PAUSED && new_state == GST_STATE_PLAYING && seek_done == FALSE ) {
			   g_print ("On a detecte Pipeline state changed from PAUSED to PLAYING on lance un seek\n");
			   //gst_element_seek_simple (pipeline, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT, 2 * GST_SECOND);
			   gst_element_send_event (pipeline, seek_event); 
			   seek_done=TRUE;
			   }
          }
          break;
        default:
          /* We should not reach here */
          g_printerr ("Unexpected message received.\n");
          break;
          
		  
	  }
      
      
      
      
    } 
     
  } while (!terminate);
  
  
   


  /* Free resources */
  gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}
