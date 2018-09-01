#include <gtk/gtk.h>

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
    
    return (FALSE);
}

void button_event (GtkWidget *widget, gpointer *data)
{
    g_print ("Button event: %s\n",data);
}

int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    
    gtk_init(&argc,&argv);
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(window), "Push Button");
    
    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (CloseAppWindow), NULL);
    
    gtk_container_border_width (GTK_CONTAINER(window), 50);
    
    button = gtk_check_button_new_with_label("Check Button");
    
    gtk_widget_show(button);
    
    /* Las senales le pasan el/los ultimos  parametro/s a la funcion que luego imprime ese mensaje */
    gtk_signal_connect (GTK_OBJECT(button), "toggled", GTK_SIGNAL_FUNC (button_event), "toggled");
    gtk_signal_connect (GTK_OBJECT(button), "pressed", GTK_SIGNAL_FUNC (button_event), "pressed");
    gtk_signal_connect (GTK_OBJECT(button), "released", GTK_SIGNAL_FUNC (button_event), "released");
    gtk_signal_connect (GTK_OBJECT(button), "clicked", GTK_SIGNAL_FUNC (button_event), "clicked");
    gtk_signal_connect (GTK_OBJECT(button), "enter", GTK_SIGNAL_FUNC (button_event), "enter");
    gtk_signal_connect (GTK_OBJECT(button), "leave", GTK_SIGNAL_FUNC (button_event), "leave");

    gtk_container_add(GTK_CONTAINER(window), button);
    
    gtk_widget_show(window);
    
    gtk_main();
    return (0);
}
