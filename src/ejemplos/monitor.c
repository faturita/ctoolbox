/* Programa ejemplo de dos procesos que imprimen en pantalla al mismo tiempo */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/* Implementacion de un programa que reciba datos y los ponga en pantalla */

/* Asi podria ser el programa que se encargara de recibir datos de la tuberia e
 imprimirlos en la pantalla.....
 Siendo uno solo, no hay problema y todo se imprime en su lugar....
 Fijense que si le sacan las dobles barras al getch, se hace paso a paso, 
 pero solamente aparecen los datos del proceso padre (un hermano manda datos y
 otro es el monitor).  Quizas sea porque se llena el tamanio de la cola de pipes
 y es por eso que los datos que lee son solo de uno de los proceso, pero en 
 ese caso capaz que se podria solucionar haciendo que le conteste el monitor por
 un pipe bidimensional, cuando leyo los datos... No se... 
 
 Otra cosa que es importante es que para que funcione que se puedan mandar y 
 recibir mensajes con una estructura hay que poner en el read como esta especifi
 cado abajo:
     read (s,&msg,sizeof(msg))  siendo msg la estructura....
     
 
  Para el pacman vamos a tener que hacer que le conteste cuando recibio los
  datos, porque si no va a pasar como pasa en este programa que uno imprime muchas
  veces y otro no.  O si no, capaz usando semaforos , en fin, tampoco se...
  
 */

struct TipoMsgMonitor
    {
    char SIMBOLO;
    int x;
    int y;
    };
    
    
void monitor(int pipe[])
    {
    struct TipoMsgMonitor msg;
    char SIMBOLO;
    int salir=0;
    int nbytes;
    initscr();	
    while (salir==0)
	{
	/* La lectura de la tuberia es bloqueante. Imprime si lee */
	if (nbytes=read (pipe[0],&msg,sizeof(msg))<=0)
	    {
	    addstr("Hay un error en el paso de mensajes !!!!!");
	    break;
	    }
	if ( (msg.SIMBOLO=='-') && (msg.x==msg.y) && (msg.x==0) )
	    salir=1;
	else
	    {
	    beep();
	    move(msg.x,msg.y);
	    addch(msg.SIMBOLO);
	    /*printw ("Cantidad bytes recibido %d:\n",nbytes);
	    printw ("Cantidad x: %d\n",msg.x);
	    printw ("Cantidad y: %d\n",msg.y);
	    printw ("Cantidad Simbolo: %c\n",msg.SIMBOLO);*/
	    refresh();
	    msg.SIMBOLO=' ';msg.x=0;msg.y=0;
	    }
	//getch();
	}
    endwin();
    }
    
	    
    

void crearhijomonitor(int pipe[])
/* Crea un hijo que sera el encargado de imprimir en pantalla */
/* Como parametro recibe una tuberia YA CREADA */
    {
    int Ppid=getpid();
    fork();
    if (Ppid==getppid())
	{monitor(pipe);exit(0);}
    else
	return;
    }
    
    
        
void main()
{
int Ppid=getpid();
int pid;
int pip[2];
char SIMBOLO;
struct TipoMsgMonitor msg;
pipe(pip);
crearhijomonitor(pip);

fork();
if (Ppid==getppid())
    {
    msg.x=10;
    while (1)
	{
	msg.x++;msg.y=10;
	msg.SIMBOLO='F';
	while ( write(pip[1],&msg,sizeof(msg))<0 );
	if (msg.x>20) msg.x=10;
	}
    }
else
    {
    msg.y=10;
    while (1)
	{
	msg.y++;msg.x=5;
	msg.SIMBOLO='S';
	if (write(pip[1],&msg,sizeof(msg))<0) 
	    {
	    printf ("ERRORFRAFSFSDF");
	    exit(1);
	    }
	    
	if (msg.y==20) msg.y=10;
	}
    }
exit(0);
}
    

	
