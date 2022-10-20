/**
 * Hello World GTK3 avec gestion keypress event
 * 
 * gcc -o hello-world-gtk hello-world-gtk.c $(pkg-config --libs --cflags gtk+-3.0)
 * 
 * 
 * 
 */


#include <gtk/gtk.h>

static void print_hello (GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	if (event->keyval == GDK_KEY_space){
        g_print ("Hello World\n");
    }  
}

static void
activate (GtkApplication *app, gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  
  //Action au keypress event
  //https://stackoverflow.com/questions/44098084/how-do-i-handle-keyboard-events-in-gtk3
  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (print_hello), NULL);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
