/*
Programa:  fifo.c
Objetivo: Implementa ficheros del tipo fifo para la comunicacion entre procesos
que no esten enparentados.
Este proceso grabara en un fifo.  Funciona conjuntamente con fifo2.c y este debe
ejecutarse primero ya que crea los fifos.
Nota:  El tratamiento de los ficheros es exactamente igual a ficheros comunes, 
esto quiere decir que en el caso de lectura del fifo todo dependera si se esta-
blece o no la lectura por parte de un proceso en modo posesivo o no.

*/



#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utmp.h>


#define TOPE 256


void main()
{
/* Descriptores de los ficheros */
int fifoto,fifofr;
/* El tamanio corresponde al maximo permitido para un nombre de fichero */
char nomfifoto[TOPE],nomfifofr[TOPE];

char msj[TOPE];

/* Creacion de dos fifos, uno para la ida y el otro para la vuelta. */
strcpy (nomfifoto,"fifoto");
strcpy (nomfifofr,"fifofr");  

/* Se les elimina el enlace que pudieran haber tenido */
unlink(nomfifoto);
unlink(nomfifofr);

/* Se crea solamente un fifo que se usara para escribir */
if  (mknod(nomfifoto,S_IFIFO | 0666,0)==-1)  {
    perror ("Error al crear el fichero de comunicaciones.\n");
    exit(-1);
    }

printf ("La tuberia fifo ha sido creada.....\n");

/* Necesitamos abrir el fichero para establecer la comunicacion */
/* Se quedara esperando hasta que alguien abra el fifo */
if ( (fifoto=open(nomfifoto,O_WRONLY))==-1 )    
    {
    perror ("Error en la apertura de los ficheros fifo.\n");
    exit(-1);
    }
    
/* En este punto los ficheros estan abierto y listos para ser utilizados. */
printf ("Enviando mensajes a la fifo comun....\n");

sprintf (msj,"PID (%d) ->Este es un mensaje para que algun proceso lo capture.\n",getpid());
write(fifoto,msj,strlen (msj));

close(fifoto);
exit(0);
}




