#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();

    return (FALSE);
}


int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *label;

    gtk_init(&argc,&argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Labels");

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    gtk_container_border_width (GTK_CONTAINER(window), 50);

    /* Creacion de etiqueta */
    label = gtk_label_new ("Pasadas: ");
    /* Alineacion en (x,y) */
    gtk_misc_set_alignment (GTK_MISC(label), 1.0, .5);

    gtk_widget_show(label);

    /* Las senales le pasan el/los ultimos  parametro/s a la funcion que luego imprime ese mensaje */

    gtk_container_add(GTK_CONTAINER(window), label);

    gtk_widget_show(window);

    gtk_main();
    return (0);
}
