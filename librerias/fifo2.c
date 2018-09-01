/*
Programa:  fifo2.c
Objetivo: Implementa ficheros del tipo fifo para la comunicacion entre procesos
que no esten enparentados.
Nota:  El tratamiento de los ficheros es exactamente igual a ficheros comunes, 
esto quiere decir que en el caso de lectura del fifo todo dependera si se esta-
blece o no la lectura por parte de un proceso en modo posesivo o no.
Este programa trabaja en conjunto con fifo.c Y PRESUPONE LA EXISTENCIA DE LOS
FIFOS YA CREADOS......
Se puede implementar codigo para la verificacion si los fifos existen ya, pero 
como este programa es solo a nivel demostrativo, ACONSEJAMOS al lector que 
ejecute primero el programa fifo.c, y luego este.
Autores: Oel Amon Yag.
Fecha: 1/9/1999

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
int tam;
/* Creacion de dos fifos, uno para la ida y el otro para la vuelta. */
strcpy (nomfifoto,"fifofr");  /* Difiere del otro programa en que tiene diferentes nombres */
strcpy (nomfifofr,"fifoto");  

/* El recuadro siguiente se debe habilitar para el caso de que este sea el primero
en ejecutar.  Para esto se verifica si ya estan creados los ficheros y si no 
se crean */


/*if ( (mknod(nomfifoto,S_IFIFO | 0666,0)==-1) || (mknod(nomfifofr,S_IFIFO | 0666,0)) )
    {
    perror ("Error al crear el fichero de comunicaciones.\n");
    exit(-1);
    }*/
    

/* Necesitamos abrir el fichero para establecer la comunicacion */
/* Se debe agregar un controlador de existencia de los archivos, para
el caso en que se ejecuten en cualquier orden */
/* Asi como esta EXIJE que se ejecute primero fifo.out. */
if ( (fifofr=open(nomfifofr,O_RDONLY)) == -1 )    
    {
    perror ("Error en la apertura de los ficheros fifo.\n");
    exit(-1);
    }
    
/* En este punto los ficheros estan abierto y listos para ser utilizados. */
printf ("Esperando recibir mensajes de algun proceso....\n");

/* El read es bloqueante por defecto */
if ( (tam=read(fifofr,msj,TOPE))==-1)
    {
    perror("Se ha producido un error en la lectura del fichero....\n");
    exit(-1);
    }
    
msj[tam]='\0';    
printf ("Mensaje recibido:%s\n",msj);
close(fifofr);
exit(0);
}




