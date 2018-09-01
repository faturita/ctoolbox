#include <gtk/gtk.h>

/**
 * Crea un boton con el label szLabel, y luego lo agrega al empaquetador
 * box.
 * Las opciones de agregado son fijas.
 */
GtkWidget *PackNewButton (GtkWidget *box, char *szLabel)
{
    GtkWidget *button;
    
    button = gtk_button_new_with_label (szLabel);
    
    gtk_box_pack_start (GTK_BOX (box), button, FALSE,FALSE,0);
    
    gtk_widget_show(button);
    
    return (button);
}


gint destroy (GtkWidget *widget,gpointer gdata)
{
      g_print ("Quitting program\n");
      gtk_main_quit();

      return (FALSE);
}

void button_was_clicked (GtkWidget *widget, gpointer gdata)
{
      g_print ("Se ha presionado el boton....\n");
}

void button_enter (GtkWidget *widget, gpointer gdata)
{
    g_print ("Ingresando area boton.");
}

void button_leave (GtkWidget *widget, gpointer gdata)
{
    g_print (" Saliendo area boton.\n");
}

int main(int argc, char *argv[])
{
      GtkWidget *window;
      GtkWidget *button;
      GtkWidget *box;   		// Empaquetador
      GList *cbitems = NULL;

      gtk_init(&argc,&argv);
      
      /* Se crea una ventana pero no se la hace visible */
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (window), "Vertical box packing.");
    
      gtk_signal_connect (GTK_OBJECT (window),"delete_event",GTK_SIGNAL_FUNC (destroy), NULL);
      
      /* Deja espacio alrededor de los objetos en el container */
      gtk_container_border_width (GTK_CONTAINER (window), 50);


      /* Cuadro empaquetado vertical con los botones */
      box = gtk_vbox_new (FALSE, 0);
      
      button = PackNewButton (box, "Botonazo");
      gtk_signal_connect (GTK_OBJECT (button),"clicked",GTK_SIGNAL_FUNC(button_was_clicked), NULL);
      gtk_signal_connect (GTK_OBJECT (button),"enter",GTK_SIGNAL_FUNC(button_enter),NULL);
      gtk_signal_connect (GTK_OBJECT (button),"leave",GTK_SIGNAL_FUNC(button_leave),NULL);
      
      button = PackNewButton (box, "Boton 2");
      button = PackNewButton (box, "Boton 3");
      button = PackNewButton (box, "Boton 4");
      button = PackNewButton (box, "Boton 5");
      
      
      /* Agregar el boton a la ventana */
      gtk_container_add (GTK_CONTAINER (window), box);
      
      /* Hacer visible el boton */
      gtk_widget_show (box);

      gtk_widget_show (window);

      gtk_main();

      return 0;
}
