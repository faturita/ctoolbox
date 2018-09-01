/***
    PROGRAMA: Tuberias.
    Descripcion:Transferencia de mensajes entre un programa emisor un un 
    programa receptor.  Principalmente dos procesos ejecutando en forma 
    concurrente.
    NOTA> Este programa fue copiado en forma casi textual del libro UNIX
    programacion avanzada.
    
***/

#include <stdio.h>
#define MAX 256

void main()
{
    int tuberia_em_re[2];
    int tuberia_re_em[2];
    int pid,Ppid=getpid();
    char mensaje[MAX];
    
    /* Creacion de la tuberia de comunicacion */
    if (pipe(tuberia_em_re) == -1 || pipe (tuberia_re_em) == -1)
    {
	perror("pipe");
	exit(-1);
    }
    printf ("Creacion de tuberias correcta\n");
    /* Creacion de hijo */
    if ((pid=fork())==-1)
    {
	perror("fork");
	exit(-1);
    }
    
    if (getpid()!=Ppid)
	{
	    /*Codigo del proceso hijo*/
	    printf ("\nEjecutando el codigo del hijo\n");
	    read(tuberia_em_re[0],mensaje,MAX);
	    while (strcmp(mensaje,"FIN")!=0)
		{
		printf ("Proceso receptor recibio %s\n",mensaje);
		read(tuberia_em_re[0],mensaje,MAX);
		}
	    close(tuberia_em_re[0]);
	    close(tuberia_em_re[1]);
	    close(tuberia_re_em[0]);
	    close(tuberia_re_em[1]);
	    printf ("El hijo esta terminando la ejecucion.\n");
	    exit(0);
	}
	
    if (getpid()==Ppid)
	{
	    /* Codigo del padre */
	    printf ("Ejecutando el codigo del padre.\n");
	    strcpy(mensaje,"INICIAL");
	    while (strcmp(mensaje,"FIN")!=0)
		{
		gets(mensaje);
		write (tuberia_em_re[1],mensaje,strlen(mensaje)+1);
		}
	    close(tuberia_em_re[0]);
	    close(tuberia_em_re[1]);
	    close(tuberia_re_em[0]);
	    close(tuberia_re_em[1]);
	    exit(0);
	}
    }
    
	
	            