/*
packbox.c 

Ejemplo del libro de la pagina 42

Muestra los contenedores y los empaquetadores de widgets.

*/

#include <gtk/gtk.h>

GtkWidget *PackNewButton(GtkWidget *box, char *szLabel )
{
    GtkWidget *button;
    
    // Crea un nuevo boton conteniendo una etiqueta szLabel
    button = gtk_button_new_with_label (szLabel);
    
    // Coloca el boton dentro de del box box.
    gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);
    
    // Mostrar el boton
    gtk_widget_show(button);
    
    return (button);
}

void button_leave (GtkWidget *widget, gpointer data)
{
    g_print ("El mouse ha dejado la zona de seguridad...");
}


gint Delete (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit();
    
    // Cierre terminando el bucle de sucesos....
    
    return (FALSE);
}

void button_click(GtkWidget *widget, gpointer data)
{
  g_print ("Se ha presionado el boton correctamente......\n");
}


int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box;
    
    // Inicializacion normal de GTK
    gtk_init (&argc,&argv);
    
    // Crear la ventana principal ....
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title (GTK_WINDOW (window), "Empaquetado de Widgets verticalmente");
    
    // Asosiar a la ventana principal el evento delete_event con la funcion Delete..
    gtk_signal_connect (GTK_OBJECT (window), "delete_event", GTK_SIGNAL_FUNC (Delete), NULL);
        
    gtk_container_border_width(GTK_CONTAINER (window), 50);
    
    // Creacion del objeto para empaquetar.
    box = gtk_vbox_new (FALSE, 0);
    
    button= PackNewButton (box, "Boton 1");
    button= PackNewButton (box, "Boton 2");
    button= PackNewButton (box, "Boton 3");    
    button= PackNewButton (box, "Boton 4");
    button= PackNewButton (box, "Boton 5");
    
    gtk_signal_connect (GTK_OBJECT(button), "leave",GTK_SIGNAL_FUNC (button_leave), NULL);    
    
    gtk_signal_connect (GTK_OBJECT(button), "clicked",GTK_SIGNAL_FUNC (button_click), NULL);
    
    // Ahora se agrega este empaquetador a la ventana principal, y se hacen visibles
    gtk_container_add (GTK_CONTAINER (window), box);
    gtk_widget_show (box);
    gtk_widget_show (window);



    // Se arranca el bucle de sucesos.....
    gtk_main ();
    
    exit(0);
}


    
    
