/* Este es un ejemplo de un socket CLIENTE */


#include <stdio.h>
#include <string.h>
#include "comunicaciones.h"
#include <ncurses.h>

#define DP_PORT 5504


struct TipoMsgMonitor
{
    int x;
    int y;
    char SIMBOLO;
    int id;
};


void getchremoto(int sock)
    {
    /* OBJETIVO:  Espera la entrada por teclado desde la terminal actual y la
    envia por un socket para ser leida por un proceso servidor. */
    /* NOTA:  El getch es bloqueante. */
    chtype c=32;    
    while (c!='q')
	{
	c=getch();refresh();
	write(sock,&c,sizeof(c));
	}
    }
    
    
    
void main(int argc,char *argv[])
{
int Ppid=getpid();
struct sockaddr_in hostsa;
int sock;
int tams;
char hst[32];
char msgg[20];
struct TipoMsgMonitor msg;

if (argc!=2)
    {
    printf ("Copyright (C) Oel Amon Yag Software de Calidad.\n");
    printf ("Modo de uso: clientpac.out nickname \n\n");
    exit(-1);
    }
    


sock=abrirSocketC(&hostsa);

/* Conectarse con el host */
/* El nombre del host es obtenido del archivo /etc/hosts */

if ( (connect(sock,(void *)&hostsa,sizeof(hostsa)))<0 )
    {
    printf ("Fallo en el connect ....\n");
    exit(1);
    }
    
printf ("Numero de socket asignado:%d\n",sock);
bcopy(argv[1],msgg,20);

/* Envio del nick de usuario */
printf ("Nickname de usuario: %s",msgg);
write(sock,msgg,strlen(msgg));

/* Display de informacion de estado */
printf ("Conexion exitosa !!!!!!\n\n");
printf ("Datos del HOST REMOTO....\n");    
printf ("Puerto asignado:%d\n",hostsa.sin_port);    
printf ("Address:%ld\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d\n",hostsa.sin_family);


/* Espera de respuesta del servidor */
printf ("Esperando habilitacion del servidor.....\n");
read(sock,msgg,strlen(msgg));


/* Inicializacion de ncurses */
initscr();
cbreak();
noecho();
keypad(stdscr,TRUE);
refresh();



/* Creacion de un hijo para el envio de datos al server */

fork();
if (Ppid==getppid())
    {
    getchremoto(sock);
    endwin();
    exit(1);
    }
    
/* Ejecuta el padre la impresion de datos en pantalla */


/* Aca se debe agreagar el codigo de impresion y cambiar el pipe por el sock.*/



    
    




while (1);

printf ("Cerrando el socket .....\n");

endwin();
close(sock);

getc(stdin);
}
