/* 
 * Essayer de mimer ce qui se passe dans tools/ges-launcher.c
 * 
 * 
export PKG_CONFIG_PATH=/usr/X11R7/lib/pkgconfig
gcc vvnx2.c -o vvnx2 `pkg-config --cflags --libs gst-editing-services-1.0`
 */

#include <ges/ges.h>



int
main (int argc, gchar ** argv)
{
  GESAsset *src_asset;
  GESPipeline *pipeline;
  GESTimeline *timeline;
  GESClip *source;
  GESLayer *layer;
  GMainLoop *mainloop;
  
  GESProject *project;
  GError *error = NULL;
  


  g_print("debut main\n");
  
    
  //gchar *sanitized_timeline = " +clip 174.ogg"; //attention il faut au moins un espace au début!
  gchar *sanitized_timeline = " +clip DJI_0174.MOV";

  /* Initialize GStreamer (this will parse environment variables and commandline
   * arguments. */
  gst_init (&argc, &argv);
  


  /* Initialize the GStreamer Editing Services */
  ges_init ();



  /* Là je mets ce qui est dans tools/ges-launcher.c li 240 @ _create_timeline */
  g_print("sanitized_timeline = %s\n", sanitized_timeline);
  project = ges_project_new (sanitized_timeline);

  timeline = GES_TIMELINE (ges_asset_extract (GES_ASSET (project), &error));

  if (error) {
    g_printerr ("\nERROR: Could not create timeline because: %s\n\n",
        error->message);
    g_error_free (error);
    return FALSE;
  }

  //ges_timeline_commit (timeline);

  pipeline = ges_pipeline_new ();
  
  //ges_pipeline_set_mode (pipeline, GES_PIPELINE_MODE_PREVIEW); //de toutes façons c'est le défaut

  /* Add the timeline to that pipeline */
  if (!ges_pipeline_set_timeline (pipeline, timeline))
    return -1;

  /* The following is standard usage of a GStreamer pipeline (note how you haven't
   * had to care about GStreamer so far ?).
   *
   * We set the pipeline to playing ... */
  gst_element_set_state (GST_ELEMENT (pipeline), GST_STATE_PLAYING);

  /* .. and we start a GMainLoop. GES **REQUIRES** a GMainLoop to be running in
   * order to function properly ! */
  mainloop = g_main_loop_new (NULL, FALSE);

  /* Simple code to have the mainloop shutdown after 20s */
  g_timeout_add_seconds (4, (GSourceFunc) g_main_loop_quit, mainloop);
  g_main_loop_run (mainloop);

  return 0;
}
