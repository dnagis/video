/** 
 * 
 * gst-rtsp-server-1.18.1/examples/test-launch.c
 * 
 * -compil:
 * gcc test-launch.c -o test-launch `pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
 * ./test-launch "( videotestsrc is-live=1 ! vaapih264enc ! rtph264pay name=pay0 pt=96 )"
 * 
 * 
 * 
 * -X-compil:
 * export PATH PKG_CONFIG_LIBDIR PKG_CONFIG_SYSROOT_DIR
 * arm-linux-gnueabihf-gcc test-launch.c -o test-launch `pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
 * 
 * ./test-launch "( v4l2src ! video/x-raw,width=720,height=480,format=YUY2,framerate=30/1 ! v4l2h264enc ! video/x-h264,level=(string)3 ! rtph264pay name=pay0 pt=96 )"
 * 
 * -lecture:
 * gst-launch-1.0 rtspsrc location=rtsp://192.168.49.1:8554/test latency=100 ! queue ! rtph264depay ! h264parse ! vaapih264dec ! videoconvert ! videoscale ! ximagesink
 * 
 * 
 */

#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "8554"

static char *port = (char *) DEFAULT_RTSP_PORT;

static GOptionEntry entries[] = {
  {"port", 'p', 0, G_OPTION_ARG_STRING, &port,
      "Port to listen on (default: " DEFAULT_RTSP_PORT ")", "PORT"},
  {NULL}
};

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;
  GOptionContext *optctx;
  GError *error = NULL;

  optctx = g_option_context_new ("<launch line> - Test RTSP Server, Launch\n\n"
      "Example: \"( videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96 )\"");
  g_option_context_add_main_entries (optctx, entries, NULL);
  g_option_context_add_group (optctx, gst_init_get_option_group ());
  if (!g_option_context_parse (optctx, &argc, &argv, &error)) {
    g_printerr ("Error parsing options: %s\n", error->message);
    g_option_context_free (optctx);
    g_clear_error (&error);
    return -1;
  }
  g_option_context_free (optctx);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new ();
  g_object_set (server, "service", port, NULL);

  /* get the mount points for this server, every server has a default object
   * that be used to map uri mount points to media factories */
  mounts = gst_rtsp_server_get_mount_points (server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines.
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, argv[1]);
  gst_rtsp_media_factory_set_shared (factory, TRUE);

  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory (mounts, "/test", factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref (mounts);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);

  /* start serving */
  g_print ("stream ready at rtsp://127.0.0.1:%s/test\n", port);
  g_main_loop_run (loop);

  return 0;
}
