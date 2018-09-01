#include <gtk/gtk.h>

gint destroyapp (GtkWidget *widget, gpointer gdata)
{
	g_print ("Quitting...\n");
	gtk_main_quit ();
	
	/* True provocaria que la ventana no se cerrara */
	return (FALSE);
}

int main(int argc, char *argv[])
{
	GtkWidget *window;
	
	gtk_init (&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (destroyapp), NULL);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}
