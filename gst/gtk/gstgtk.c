/**
 * Basé sur le tuto 5 de gstreamer: Basic tutorial 5: GUI toolkit integration
 * mais avec une pipeline maison, pour pouvoir ajouter un timeoverlay (voir basic2.c)
 * 
 * export PKG_CONFIG_PATH=/usr/X11R7/lib/pkgconfig
 * gcc gstgtk.c -o gstgtk `pkg-config --cflags --libs gstreamer-video-1.0 gtk+-3.0 gstreamer-1.0`
 * 
 * 
 */

#include <string.h>
#include <libgen.h>

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/video/videooverlay.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h>

/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData
{
  //GstElement *playbin;          /* Our one and only pipeline */
  
  //je crée ma pipeline à moi style gst-launch-1.0 filesrc location=174.mp4 ! decodebin ! vaapisink
  GstElement *pipeline;
  GstElement *source;
  GstElement *decode;  
  GstElement *convert;
  GstElement *time;
  GstElement *sink;
  
  
  

  GtkWidget *slider;            /* Slider widget to keep track of current position */
  GtkWidget *streams_list;      /* Text widget to display info about the streams */
  gulong slider_update_signal_id;       /* Signal ID for the slider update signal */

  GstState state;               /* Current state of the pipeline */
  gint64 duration;              /* Duration of the clip, in nanoseconds */
} CustomData;

/* Handler for the pad-added signal  */
//static void pad_added_handler (GstElement * src, GstPad * pad, CustomData * data);    
/* This function will be called by the pad-added signal sur le decodebin (voir basic2.c)*/
static void
pad_added_handler (GstElement * src, GstPad * new_pad, CustomData * data)
{
  GstPad *sink_pad = gst_element_get_static_pad (data->time, "video_sink"); //voir gst-inspect pour le nom du sink

  g_print ("Received new pad '%s' from '%s':\n", GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (src));
  
  gst_pad_link (new_pad, sink_pad);

 
}

/* This function is called when the GUI toolkit creates the physical window that will hold the video.
 * At this point we can retrieve its handler and pass it to GStreamer through the XOverlay interface. */
static void
realize_cb (GtkWidget * widget, CustomData * data)
{
  GdkWindow *window = gtk_widget_get_window (widget);
  guintptr window_handle;

  if (!gdk_window_ensure_native (window))
    g_error ("Couldn't create native window needed for GstXOverlay!");

  /* Retrieve window handler from GDK */
  window_handle = GDK_WINDOW_XID (window);

  /* Pass it to playbin, which implements XOverlay and will forward it to the video sink 
   * sauf que moi j'ai plus de playbin.*/
  gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (data->sink), window_handle);
}

/* This function is called when the PLAY button is clicked */
static void
play_cb (GtkButton * button, CustomData * data)
{
  gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
}

/* This function is called when the PAUSE button is clicked */
static void
pause_cb (GtkButton * button, CustomData * data)
{
  gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
static void
stop_cb (GtkButton * button, CustomData * data)
{
  gst_element_set_state (data->pipeline, GST_STATE_READY);
}

/* This function is called when the main window is closed */
static void
delete_event_cb (GtkWidget * widget, GdkEvent * event, CustomData * data)
{
  stop_cb (NULL, data);
  gtk_main_quit ();
}

/* This function is called everytime the video window needs to be redrawn (due to damage/exposure,
 * rescaling, etc). GStreamer takes care of this in the PAUSED and PLAYING states, otherwise,
 * we simply draw a black rectangle to avoid garbage showing up. */
static gboolean
draw_cb (GtkWidget * widget, cairo_t * cr, CustomData * data)
{
  if (data->state < GST_STATE_PAUSED) {
    GtkAllocation allocation;

    /* Cairo is a 2D graphics library which we use here to clean the video window.
     * It is used by GStreamer for other reasons, so it will always be available to us. */
    gtk_widget_get_allocation (widget, &allocation);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_rectangle (cr, 0, 0, allocation.width, allocation.height);
    cairo_fill (cr);
  }

  return FALSE;
}

/* This function is called when the slider changes its position. We perform a seek to the
 * new position here. */
static void
slider_cb (GtkRange * range, CustomData * data)
{
  gdouble value = gtk_range_get_value (GTK_RANGE (data->slider));
  gst_element_seek_simple (data->pipeline, GST_FORMAT_TIME,
      GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT,
      (gint64) (value * GST_SECOND));
}

/* This creates all the GTK+ widgets that compose our application, and registers the callbacks */
static void
create_ui (CustomData * data)
{
  GtkWidget *main_window;       /* The uppermost window, containing all other windows */
  GtkWidget *video_window;      /* The drawing area where the video will be shown */
  GtkWidget *main_box;          /* VBox to hold main_hbox and the controls */
  GtkWidget *main_hbox;         /* HBox to hold the video_window and the stream info text widget */
  GtkWidget *controls;          /* HBox to hold the buttons and the slider */
  GtkWidget *play_button, *pause_button, *stop_button;  /* Buttons */

  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (main_window), "delete-event",
      G_CALLBACK (delete_event_cb), data);

  video_window = gtk_drawing_area_new ();
  gtk_widget_set_double_buffered (video_window, FALSE);
  g_signal_connect (video_window, "realize", G_CALLBACK (realize_cb), data);
  g_signal_connect (video_window, "draw", G_CALLBACK (draw_cb), data);

  play_button =
      gtk_button_new_from_icon_name ("media-playback-start",
      GTK_ICON_SIZE_SMALL_TOOLBAR);
  g_signal_connect (G_OBJECT (play_button), "clicked", G_CALLBACK (play_cb),
      data);

  pause_button =
      gtk_button_new_from_icon_name ("media-playback-pause",
      GTK_ICON_SIZE_SMALL_TOOLBAR);
  g_signal_connect (G_OBJECT (pause_button), "clicked", G_CALLBACK (pause_cb),
      data);

  stop_button =
      gtk_button_new_from_icon_name ("media-playback-stop",
      GTK_ICON_SIZE_SMALL_TOOLBAR);
  g_signal_connect (G_OBJECT (stop_button), "clicked", G_CALLBACK (stop_cb),
      data);

  data->slider =
      gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
  gtk_scale_set_draw_value (GTK_SCALE (data->slider), 0);
  data->slider_update_signal_id =
      g_signal_connect (G_OBJECT (data->slider), "value-changed",
      G_CALLBACK (slider_cb), data);

  data->streams_list = gtk_text_view_new ();
  gtk_text_view_set_editable (GTK_TEXT_VIEW (data->streams_list), FALSE);

  controls = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (controls), play_button, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), pause_button, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), stop_button, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), data->slider, TRUE, TRUE, 2);

  main_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), video_window, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), data->streams_list, FALSE, FALSE, 2);

  main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start (GTK_BOX (main_box), main_hbox, TRUE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (main_box), controls, FALSE, FALSE, 0);
  gtk_container_add (GTK_CONTAINER (main_window), main_box);
  gtk_window_set_default_size (GTK_WINDOW (main_window), 1920, 1080);

  gtk_widget_show_all (main_window);
}

/* This function is called periodically to refresh the GUI */
static gboolean
refresh_ui (CustomData * data)
{
  gint64 current = -1;

  /* We do not want to update anything unless we are in the PAUSED or PLAYING states */
  if (data->state < GST_STATE_PAUSED)
    return TRUE;

  /* If we didn't know it yet, query the stream duration */
  if (!GST_CLOCK_TIME_IS_VALID (data->duration)) {
    if (!gst_element_query_duration (data->pipeline, GST_FORMAT_TIME,
            &data->duration)) {
      g_printerr ("Could not query current duration.\n");
    } else {
      /* Set the range of the slider to the clip duration, in SECONDS */
      gtk_range_set_range (GTK_RANGE (data->slider), 0,
          (gdouble) data->duration / GST_SECOND);
    }
  }

  if (gst_element_query_position (data->pipeline, GST_FORMAT_TIME, &current)) {
    /* Block the "value-changed" signal, so the slider_cb function is not called
     * (which would trigger a seek the user has not requested) */
    g_signal_handler_block (data->slider, data->slider_update_signal_id);
    /* Set the position of the slider to the current pipeline positoin, in SECONDS */
    gtk_range_set_value (GTK_RANGE (data->slider),
        (gdouble) current / GST_SECOND);
    /* Re-enable the signal */
    g_signal_handler_unblock (data->slider, data->slider_update_signal_id);
  }
  return TRUE;
}




/* This function is called when an error message is posted on the bus */
static void
error_cb (GstBus * bus, GstMessage * msg, CustomData * data)
{
  GError *err;
  gchar *debug_info;

  /* Print error details on the screen */
  gst_message_parse_error (msg, &err, &debug_info);
  g_printerr ("Error received from element %s: %s\n",
      GST_OBJECT_NAME (msg->src), err->message);
  g_printerr ("Debugging information: %s\n", debug_info ? debug_info : "none");
  g_clear_error (&err);
  g_free (debug_info);

  /* Set the pipeline to READY (which stops playback) */
  gst_element_set_state (data->pipeline, GST_STATE_READY);
}

/* This function is called when an End-Of-Stream message is posted on the bus.
 * We just set the pipeline to READY (which stops playback) */
static void
eos_cb (GstBus * bus, GstMessage * msg, CustomData * data)
{
  g_print ("End-Of-Stream reached.\n");
  gst_element_set_state (data->pipeline, GST_STATE_READY);
}

/* This function is called when the pipeline changes states. We use it to
 * keep track of the current state. */
static void
state_changed_cb (GstBus * bus, GstMessage * msg, CustomData * data)
{
  GstState old_state, new_state, pending_state;
  gst_message_parse_state_changed (msg, &old_state, &new_state, &pending_state);
  if (GST_MESSAGE_SRC (msg) == GST_OBJECT (data->pipeline)) {
    data->state = new_state;
    g_print ("State set to %s\n", gst_element_state_get_name (new_state));
    if (old_state == GST_STATE_READY && new_state == GST_STATE_PAUSED) {
      /* For extra responsiveness, we refresh the GUI as soon as we reach the PAUSED state */
      refresh_ui (data);
    }
  }
}

/* Extract metadata from all the streams and write it to the text widget in the GUI */
static void
analyze_streams (CustomData * data)
{
  gint i;
  GstTagList *tags;
  gchar *str, *total_str;
  guint rate;
  gint n_video, n_audio, n_text;
  GtkTextBuffer *text;

  /* Clean current contents of the widget */
  text = gtk_text_view_get_buffer (GTK_TEXT_VIEW (data->streams_list));
  gtk_text_buffer_set_text (text, "", -1);

  /* Read some properties */
  g_object_get (data->pipeline, "n-video", &n_video, NULL);
  g_object_get (data->pipeline, "n-audio", &n_audio, NULL);
  g_object_get (data->pipeline, "n-text", &n_text, NULL);

  for (i = 0; i < n_video; i++) {
    tags = NULL;
    /* Retrieve the stream's video tags */
    g_signal_emit_by_name (data->pipeline, "get-video-tags", i, &tags);
    if (tags) {
      total_str = g_strdup_printf ("video stream %d:\n", i);
      gtk_text_buffer_insert_at_cursor (text, total_str, -1);
      g_free (total_str);
      gst_tag_list_get_string (tags, GST_TAG_VIDEO_CODEC, &str);
      total_str = g_strdup_printf ("  codec: %s\n", str ? str : "unknown");
      gtk_text_buffer_insert_at_cursor (text, total_str, -1);
      g_free (total_str);
      g_free (str);
      gst_tag_list_free (tags);
    }
  }

  for (i = 0; i < n_audio; i++) {
    tags = NULL;
    /* Retrieve the stream's audio tags */
    g_signal_emit_by_name (data->pipeline, "get-audio-tags", i, &tags);
    if (tags) {
      total_str = g_strdup_printf ("\naudio stream %d:\n", i);
      gtk_text_buffer_insert_at_cursor (text, total_str, -1);
      g_free (total_str);
      if (gst_tag_list_get_string (tags, GST_TAG_AUDIO_CODEC, &str)) {
        total_str = g_strdup_printf ("  codec: %s\n", str);
        gtk_text_buffer_insert_at_cursor (text, total_str, -1);
        g_free (total_str);
        g_free (str);
      }
      if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str)) {
        total_str = g_strdup_printf ("  language: %s\n", str);
        gtk_text_buffer_insert_at_cursor (text, total_str, -1);
        g_free (total_str);
        g_free (str);
      }
      if (gst_tag_list_get_uint (tags, GST_TAG_BITRATE, &rate)) {
        total_str = g_strdup_printf ("  bitrate: %d\n", rate);
        gtk_text_buffer_insert_at_cursor (text, total_str, -1);
        g_free (total_str);
      }
      gst_tag_list_free (tags);
    }
  }

  for (i = 0; i < n_text; i++) {
    tags = NULL;
    /* Retrieve the stream's subtitle tags */
    g_signal_emit_by_name (data->pipeline, "get-text-tags", i, &tags);
    if (tags) {
      total_str = g_strdup_printf ("\nsubtitle stream %d:\n", i);
      gtk_text_buffer_insert_at_cursor (text, total_str, -1);
      g_free (total_str);
      if (gst_tag_list_get_string (tags, GST_TAG_LANGUAGE_CODE, &str)) {
        total_str = g_strdup_printf ("  language: %s\n", str);
        gtk_text_buffer_insert_at_cursor (text, total_str, -1);
        g_free (total_str);
        g_free (str);
      }
      gst_tag_list_free (tags);
    }
  }
}

/* This function is called when an "application" message is posted on the bus.
 * Here we retrieve the message posted by the tags_cb callback */
static void
application_cb (GstBus * bus, GstMessage * msg, CustomData * data)
{
  if (g_strcmp0 (gst_structure_get_name (gst_message_get_structure (msg)),
          "tags-changed") == 0) {
    /* If the message is the "tags-changed" (only one we are currently issuing), update
     * the stream info GUI */
    analyze_streams (data);
  }
}

int
main (int argc, char *argv[])
{
  CustomData data;
  GstStateChangeReturn ret;
  GstBus *bus;

  /* Initialize GTK */
  gtk_init (&argc, &argv);

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));
  data.duration = GST_CLOCK_TIME_NONE;

  /* Create the elements */
  //data.playbin = gst_element_factory_make ("playbin", "playbin");
//je crée ma pipeline à moi style gst-launch-1.0 filesrc location=174.mp4 ! decodebin ! vaapisink
  data.source = gst_element_factory_make ("filesrc", "source");
  data.decode = gst_element_factory_make ("decodebin", "decodebin");
  
  data.convert = gst_element_factory_make ("videoconvert", "convert");
  data.time = gst_element_factory_make ("timeoverlay", "time");  
  data.sink = gst_element_factory_make ("xvimagesink", "sink");
  
  data.pipeline = gst_pipeline_new ("ma-pipeline");  
  
  
  if (!data.pipeline || !data.source || !data.decode || !data.convert || !data.time || !data.sink) {
    g_printerr ("Not all elements could be created.\n");
    return -1;
  }

  //g_print ("fichier = %s et son basename = %s\n", argv[1], basename(argv[1]));  

  /*réglages filesrc et  timeoverlay*/  
  g_object_set (data.source, "location", argv[1], NULL);
  g_object_set (data.time, "text", basename(argv[1]), NULL);
  
  
  /* Build the pipeline */
  gst_bin_add_many (GST_BIN (data.pipeline), data.source, data.decode, data.convert, data.time, data.sink, NULL);
  
  // on linke ceux qui ont des pads linkables: source et decode. decode a pas de src au démarrage
  if (gst_element_link (data.source, data.decode) != TRUE) {
    g_printerr ("Elements 1 could not be linked.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }
  
  
  if (gst_element_link_many (data.time, data.convert, data.sink, NULL) != TRUE) {
    g_printerr ("Elements 2 could not be linked.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }
  
  

      
  /* Connect to the pad-added signal car decode a pas de src initialement */
  g_signal_connect (data.decode, "pad-added", G_CALLBACK (pad_added_handler), &data);

  /* Create the GUI */
  create_ui (&data);

  /* Instruct the bus to emit signals for each received message, and connect to the interesting signals */
  bus = gst_element_get_bus (data.pipeline);
  gst_bus_add_signal_watch (bus);
  g_signal_connect (G_OBJECT (bus), "message::error", (GCallback) error_cb,
      &data);
  g_signal_connect (G_OBJECT (bus), "message::eos", (GCallback) eos_cb, &data);
  g_signal_connect (G_OBJECT (bus), "message::state-changed",
      (GCallback) state_changed_cb, &data);
  g_signal_connect (G_OBJECT (bus), "message::application",
      (GCallback) application_cb, &data);
  gst_object_unref (bus);

  /* Start playing */
  ret = gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr ("Unable to set the pipeline to the playing state.\n");
    gst_object_unref (data.pipeline);
    return -1;
  }

  /* Register a function that GLib will call every second */
  g_timeout_add_seconds (1, (GSourceFunc) refresh_ui, &data);

  /* Start the GTK main loop. We will not regain control until gtk_main_quit is called. */
  gtk_main ();

  /* Free resources */
  gst_element_set_state (data.pipeline, GST_STATE_NULL);
  gst_object_unref (data.pipeline);
  return 0;
}
