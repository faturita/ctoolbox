#include <gtk/gtk.h>

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

      gtk_init(&argc,&argv);
      
      /* Se crea una ventana pero no se la hace visible */
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

      gtk_signal_connect (GTK_OBJECT (window),"delete_event",GTK_SIGNAL_FUNC (destroy), NULL);
      
      /* Deja espacio alrededor de los objetos en el container */
      gtk_container_border_width (GTK_CONTAINER (window), 15);

      button = gtk_button_new_with_label ("Apretame !!!");

      gtk_signal_connect (GTK_OBJECT (button),"clicked",GTK_SIGNAL_FUNC(button_was_clicked), NULL);
      gtk_signal_connect (GTK_OBJECT (button),"enter",GTK_SIGNAL_FUNC(button_enter),NULL);
      gtk_signal_connect (GTK_OBJECT (button),"leave",GTK_SIGNAL_FUNC(button_leave),NULL);
      
      
      /* Agregar el boton a la ventana */
      gtk_container_add (GTK_CONTAINER (window), button);
      
      /* Hacer visible el boton */
      gtk_widget_show (button);

      gtk_widget_show (window);

      gtk_main();

      return 0;
}
