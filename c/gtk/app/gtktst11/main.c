
#include <gtk/gtk.h>
#include <gtk/app/gtktst11/cb.h>

int main(int argc, char *argv[]) {

  gtk_init(&argc, &argv);

  GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_show(window);
  
  g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),0);  

  gtk_main();

  return 0;

}