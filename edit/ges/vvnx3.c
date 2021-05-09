/* 
 * preview un clip video gesuriclip
 * 
 * 
export PKG_CONFIG_PATH=/usr/X11R7/lib/pkgconfig
gcc vvnx3.c -o vvnx3 `pkg-config --cflags --libs gst-editing-services-1.0`
*/


#include <stdlib.h>
#include <ges/ges.h>


int
main (int argc, gchar ** argv)
{
	GError *err = NULL;
	GOptionContext *ctx;
	GESPipeline *pipeline;
	GESTimeline *timeline;
	GESTrack *ma_track;
	GESLayer *ma_layer;
	GESUriClip *src;
	GMainLoop *mainloop;
	
	gint inpoint = 0, duration = 4;
	gboolean mute = TRUE;
	
	
	gst_init (&argc, &argv);
	
	ges_init ();
	
	/* Create an Audio/Video pipeline with two layers */
	pipeline = ges_pipeline_new ();
	
	timeline = ges_timeline_new ();
	
	
	ma_track = GES_TRACK (ges_video_track_new ());
	
	ma_layer = ges_layer_new ();
	
	
	if (!ges_timeline_add_layer (timeline, ma_layer) ||
	  !ges_timeline_add_track (timeline, ma_track) ||
	  !ges_pipeline_set_timeline (pipeline, timeline))
	return -1;


	gchar *uri = gst_filename_to_uri (argv[1], NULL);
	
	/* Add the file */
	src = ges_uri_clip_new (uri);
	g_free (uri);
	g_object_set (src, "start", 0, "in-point", inpoint * GST_SECOND,
		"duration", duration * GST_SECOND, "mute", mute, NULL);
	ges_layer_add_clip (ma_layer, GES_CLIP (src));
 

	/* Play the pipeline */
	gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);
	mainloop = g_main_loop_new (NULL, FALSE);
	g_timeout_add_seconds (duration + 2, (GSourceFunc) g_main_loop_quit, mainloop);
	g_main_loop_run (mainloop);

	return 0;
}
