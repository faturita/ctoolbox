#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();

    return (FALSE);
}


void entry_change( GtkWidget *widget, gpointer *data)
{
    g_print ("El cuadro de texto ha sido modificado\n");
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *entry;

    gtk_init(&argc,&argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Entry");

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    gtk_container_border_width (GTK_CONTAINER(window), 50);

    /* Creacion de etiqueta */
    entry = gtk_entry_new ();

    gtk_entry_set_text( GTK_ENTRY (entry), "12:00");

    gtk_entry_append_text (GTK_ENTRY (entry) ,"am");

    gtk_entry_set_editable (GTK_ENTRY (entry), TRUE);

    gtk_entry_set_visibility(GTK_ENTRY (entry), TRUE);

    gtk_widget_show(entry);

    /* Las senales le pasan el/los ultimos  parametro/s a la funcion que luego imprime ese mensaje */
    gtk_signal_connect (GTK_OBJECT (entry), "changed", GTK_SIGNAL_FUNC (entry_change), NULL);

    gtk_container_add(GTK_CONTAINER(window), entry);

    gtk_widget_show(window);

    gtk_main();
    return (0);
}
