/**
 * Ejemplo de utilizacion de combos con GTK+
 * Ejemplo simple que genera un combo con elementos y presta atencion a la señal de cambio
 * "change"
 */
 
#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();

    return (FALSE);
}


void entry_change( GtkWidget *widget, gpointer *data)
{
    char *buffer;

    g_print ("El cuadro de texto ha sido modificado\n");
    buffer = gtk_entry_get_text(GTK_ENTRY(widget));

    if (!(strcmp(buffer,"Erase me")))
        {
        g_print ("Element erased !!");
        }


}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *combo;
    GList *cbitems;

    gtk_init(&argc,&argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Entry");

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    gtk_container_border_width (GTK_CONTAINER(window), 50);

    /** Creacion de la lista de elementos */
    cbitems=NULL;
    cbitems = g_list_append (cbitems, "Dog");
    cbitems = g_list_append (cbitems, "Mouse");
    cbitems = g_list_append (cbitems, "Wolf");
    cbitems = g_list_append (cbitems, "Sheep");
    cbitems = g_list_append (cbitems, "Erase me");

    /* Combo */
    combo = gtk_combo_new();

    gtk_combo_set_popdown_strings (GTK_COMBO(combo), cbitems);

    /* Hace que el cuadro no sea editable */
    gtk_entry_set_editable (GTK_ENTRY(GTK_COMBO(combo)->entry), FALSE);

    /* Vincula una señal al cuadro editable */
    gtk_signal_connect (GTK_OBJECT (GTK_COMBO(combo)->entry), "changed", GTK_SIGNAL_FUNC (entry_change), NULL);

    gtk_widget_show(combo);

    /* Las senales le pasan el/los ultimos  parametro/s a la funcion que luego imprime ese mensaje */
    //gtk_signal_connect (GTK_OBJECT (entry), "changed", GTK_SIGNAL_FUNC (entry_change), NULL);

    gtk_container_add(GTK_CONTAINER(window), combo);

    gtk_widget_show(window);

    gtk_main();
    return (0);
}
