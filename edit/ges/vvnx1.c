/* GStreamer Editing Services
basé sur ges.../examples/c/test4.c
* 
syntaxe:
./vvnx1 file:///root/out.ogg /root/1.avi
* 
/root/gstreamer-editing-services-1.16.0/examples/c/.libs/lt-vvnx1 file:///root/out.ogg /root/1.avi
 */

#include <ges/ges.h>
#include <gst/pbutils/encoding-profile.h>

GstEncodingProfile *make_encoding_profile (gchar * audio, gchar * container);

/* This example will take a series of files and create a audio-only timeline
 * containing the first second of each file and render it to the output uri 
 * using ogg/vorbis */

/* make_encoding_profile
 * simple method creating an encoding profile. This is here in
 * order not to clutter the main function. */
GstEncodingProfile *
make_encoding_profile (gchar * audio, gchar * container)
{
  GstEncodingContainerProfile *profile;
  GstEncodingProfile *stream;
  GstCaps *caps;

  caps = gst_caps_from_string (container);
  profile =
      gst_encoding_container_profile_new ((gchar *) "ges-test4", NULL, caps,
      NULL);
  gst_caps_unref (caps);

  caps = gst_caps_from_string (audio);
  stream = (GstEncodingProfile *)
      gst_encoding_video_profile_new (caps, NULL, NULL, 0);
  gst_encoding_container_profile_add_profile (profile, stream);
  gst_caps_unref (caps);

  return (GstEncodingProfile *) profile;
}

int
main (int argc, gchar ** argv)
{
  GESPipeline *pipeline;
  GESTimeline *timeline;
  GESTrack *trackv;
  GESLayer *layer;
  GMainLoop *mainloop;
  GstEncodingProfile *profile;
  gchar *container = (gchar *) "application/ogg";
  gchar *audio = (gchar *) "video/x-theora";
  gchar *output_uri;
  guint i;
  GError *err = NULL;
  GOptionEntry options[] = {
    {"format", 'f', 0, G_OPTION_ARG_STRING, &container,
        "Container format", "<GstCaps>"},
    {"aformat", 'a', 0, G_OPTION_ARG_STRING, &audio,
        "Audio format", "<GstCaps>"},
    {NULL}
  };
  GOptionContext *ctx;
  guint64 duration;
  
  g_print("Debut de main78\n");
  //printf("Debut de main\n"); 
  
  

  ctx = g_option_context_new ("- renders a sequence of video files.");
  //g_option_context_add_main_entries (ctx, options, NULL);
  g_option_context_add_group (ctx, gst_init_get_option_group ());

  if (!g_option_context_parse (ctx, &argc, &argv, &err)) {
    g_printerr ("Error initializing: %s\n", err->message);
    g_option_context_free (ctx);
    g_clear_error (&err);
    return -1;
  }
  g_option_context_free (ctx);

  if (argc < 2) {
    g_print ("VVNX Usage: %s <output uri> <list of video files>\n", argv[0]);
    return -1;
  }

  /* Initialize GStreamer (this will parse environment variables and commandline
   * arguments. */
  gst_init (&argc, &argv);

  /* Initialize the GStreamer Editing Services */
  ges_init ();

  /* Setup of an audio timeline */

  /* This is our main GESTimeline */
  timeline = ges_timeline_new ();
  
  //timeline = ges_timeline_new_audio_video ();

  //tracka = GES_TRACK (ges_audio_track_new ());
  trackv = GES_TRACK (ges_video_track_new ());

  /* We are only going to be doing one layer of clips */
  layer = ges_layer_new ();

  /* Add the tracks and the layer to the timeline */
  if (!ges_timeline_add_layer (timeline, layer))
    return -1;
  if (!ges_timeline_add_track (timeline, trackv))
    return -1;

  /* Here we've finished initializing our timeline, we're 
   * ready to start using it... by solely working with the layer ! */

  for (i = 2; i < argc; i++) {
    gchar *uri = gst_filename_to_uri (argv[i], NULL);
    GESUriClip *src = ges_uri_clip_new (uri);

    g_assert (src);
    g_free (uri);

    g_object_set (src, "start", ges_layer_get_duration (layer),
        "duration", GST_SECOND, NULL);
    // GESSimpleLayer -> objects automatically appended to the end of the layer 
    ges_layer_add_clip (layer, (GESClip *) src);
  }
  
  
  //ma version avec un seul clip
    /*gchar *uri = gst_filename_to_uri (argv[2], NULL);
    GESClip *src = GES_CLIP (ges_uri_clip_new (uri));
    g_assert (src);
    g_free (uri);
    g_object_set (src, "start", ges_layer_get_duration (layer),
        "duration", GST_SECOND, NULL);
    ges_layer_add_clip (layer, src);*/
    
    /*g_object_get (src, "duration", &duration, NULL);
    g_print("vvnx 2");
    g_print("vvnx properties de src: %f\n", ((gfloat) duration) / GST_SECOND);*/ 
    
    
  //ma version avec un test clip  
  //GESTestClip *src = ges_test_clip_new ();
  //g_assert (src);
  //g_object_set (src, "start", ges_layer_get_duration (layer), "duration", GST_SECOND, NULL);
  //ges_layer_add_clip (layer, (GESClip *) src);
  

  /* In order to view our timeline, let's grab a convenience pipeline to put
   * our timeline in. */
  pipeline = ges_pipeline_new ();

  /* Add the timeline to that pipeline */
  if (!ges_pipeline_set_timeline (pipeline, timeline))
    return -1;


  /* RENDER SETTINGS ! */
  /* We set our output URI and rendering setting on the pipeline */
  
  if (gst_uri_is_valid (argv[1])) { 
    output_uri = g_strdup (argv[1]);
  } else if (g_file_test (argv[1], G_FILE_TEST_EXISTS)) {
    output_uri = gst_filename_to_uri (argv[1], NULL);
  } else {
    g_printerr ("Unrecognised command line argument '%s'.\n"
        "Please pass an URI or file as argument!\n", argv[1]);
    return -1;
  }
  profile = make_encoding_profile (audio, container);
  if (!ges_pipeline_set_render_settings (pipeline, output_uri, profile))
    return -1;

  /* We want the pipeline to render (without any preview) */
  if (!ges_pipeline_set_mode (pipeline, GES_PIPELINE_MODE_RENDER))
    return -1;


  /* The following is standard usage of a GStreamer pipeline (note how you haven't
   * had to care about GStreamer so far ?).
   *
   * We set the pipeline to playing ... */
  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);

  /* ... and we start a GMainLoop. GES **REQUIRES** a GMainLoop to be running in
   * order to function properly ! */
  mainloop = g_main_loop_new (NULL, FALSE);

  /* Simple code to have the mainloop shutdown after 4s */
  /* FIXME : We should wait for EOS ! */
  g_timeout_add_seconds (argc - 1, (GSourceFunc) g_main_loop_quit, mainloop);
  g_main_loop_run (mainloop);

  return 0;
}
