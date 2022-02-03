/** 
 * un des exemples les plus simples dans le package gst-rtsp-server (examples/test-readme.c)
 * 
 * Compilation (une fois le package installé) en dehors de meson avec:
 * 
 * gcc rtsp_hello_world.c -o rtsp_hello_world `pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
 * 
 * X-compil:
 * export PATH PKG_CONFIG_LIBDIR PKG_CONFIG_SYSROOT_DIR
 * arm-linux-gnueabihf-gcc rtsp_hello_world.c -o rtsp_hello_world `pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0`
 * 
 */

#include <gst/gst.h>

#include <gst/rtsp-server/rtsp-server.h>

int
main (int argc, char *argv[])
{
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;

  gst_init (&argc, &argv);

  loop = g_main_loop_new (NULL, FALSE);

  /* create a server instance */
  server = gst_rtsp_server_new ();

  /* get the mount points for this server, every server has a default object
   * that be used to map uri mount points to media factories */
  mounts = gst_rtsp_server_get_mount_points (server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines. 
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, "( videotestsrc is-live=1 ! v4l2h264enc ! video/x-h264,level=(string)3 ! rtph264pay name=pay0 pt=96 )");

  gst_rtsp_media_factory_set_shared (factory, TRUE);

  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory (mounts, "/test", factory);

  /* don't need the ref to the mapper anymore */
  g_object_unref (mounts);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);

  /* start serving */
  g_print ("stream ready at rtsp://127.0.0.1:8554/test\n");
  g_main_loop_run (loop);

  return 0;
}
