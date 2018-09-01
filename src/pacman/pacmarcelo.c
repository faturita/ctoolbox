/*
Programa:  pantallaconcu.c
Objetivo:  Visualizacion y control de una unidad en la pantalla.
Version: 1
Autor: Oel Amon Yag.
Fecha:9/9/99
Parametros: ninguno por ahora.
NOTA:  Compilar con -lncurses

*/


#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <fcntl.h>
#include <signal.h>
#include "laberinto.h"
#include "comunicaciones.h"

#define POSINIX_P 23
#define POSINIY_P 19
#define POSINIX_F 23
#define POSINIY_F 10

#define C_JUGADOR 5

struct TipoEstado
{
    /* Estructura de los datos de cada elemento en movimiento en la pantalla. */
    long int puntos;
    int x;
    int y;
    int SIMBOLO;
    int CONTROL;
    char nick[30];
    int id;
    int sock;
    
};

struct TipoMsgMonitor
{
    int x;
    int y;
    char SIMBOLO;
    int id;
};



void muestrapantalla(char MiPanta[])
/* Imprime el vector que representa la pantalla. */
    {
    int i=0;
    for (i=0;i<strlen(MiPanta);i++)
	addch((chtype)MiPanta[i]);
    move(1,1);
    }
    

void chequeapos(MiPanta,xx,yy,direction)
/* OBJETIVO: Cheque si callo en un lugar que no puede pasar y corrije el valor */
char MiPanta[];
int *xx;
int *yy;
int direction;
    {
    int x=*xx;
    int y=*yy;
    if ( (MiPanta[(80*y)+x]==' ') || (MiPanta[(80*y)+x]=='*') )
	return;
    else
	
	switch (direction)
	    {
	    case 1:x--;break;
	    case 2:y--;break;
	    case 3:x++;break;
	    case 0:y++;break;
	    }
	(*xx)=x;
	(*yy)=y;
	
	return;
	}
    
    
	
int accion(char MiPanta[],int x,int y,struct TipoEstado est[],int id)
    {
    /* Esta definido aca explicitamente que el id 0 corresponde al pacman
    que esta controlado */
    /* Se puede analizar en la estructura para ver que tipo de control tiene y
    ver que hacer en el caso de que sea un pacman remoto */
    int i;
    if (id==0)
	{
	for (i=1;i<C_JUGADOR;i++)
	    {
	    if ( (est[i].x==est[0].x) && (est[i].y==est[0].y) )
		{
		move(23,1);
		printw("Te comio !!!! Es fantasma Tripode Leo !!\n");
		refresh();
		return -1;
		}
	    }
	}
    
    if ( (id!=0) && (est[id].x==est[0].x) && (est[id].y==est[0].y) )
	{
	move(23,1);
	printw("Te comio !!!! Es fantasma Tripode Leo !!\n");
	refresh();
	return -1;
	}
	
    if (est[0].puntos==190)
	{
	move(23,1);
	printw ("Muy buena Guacho PACMAN!!!!  O tuviste culo como Marcelo ?? O como la hermana ??\n");
	refresh();
	return -1;
	}
    if (est[id].CONTROL==1)
	if (MiPanta[(80*y)+x]=='*') 
	    {
	    beep();refresh();
	    /* Actualizacion del vector modelo de la pantalla */
	    MiPanta[(80*y)+x]=' ';
	    est[id].puntos++;
	    }
    return 1;
	
    }
    
    
void generarbox(struct TipoEstado est)
/* OBJETIVO:  Imprime datos en el cuadro de dialogo */
    {
    move(1,60);printw ("Puntos: %d",est.puntos);
    }
    
int setdirection(chtype c,int direction)
/* OBJETIVO: Analiza c y permite cambiar la direccion en modo CONTROL */
    {
    if (c==KEY_LEFT) direction=3;
    if (c==KEY_RIGHT) direction=1;
    if (c==KEY_DOWN) direction=2;
    if (c==KEY_UP) direction=0;
    return direction;
    }
    
    
int randomdirection(int direction,int x,int y,char MiPanta[])
/*  OBJETIVO: Devuelve un int que representa la direccion en modo SINCONTROL
    METODOLOGIA: Genera un numero aleatorio, chequeando si es una posible
	direccion a tomar y lo elige devolviendolo si lo es. */
    {
    int fg=0;
    int x1=x;
    int y1=y;
    int r;
    while (fg==0)
	{
	x=x1;y=y1;
	r=( (int) (100.0*rand()/(RAND_MAX+1.0)));
	if (r==1) direction=0;
	if (r==2) direction=1;
	if (r==3) direction=2;
	if (r==4) direction=3;
	switch (direction)
	    {
	    case 1:x++;break;
	    case 2:y++;break;
	    case 3:x--;break;
	    case 0:y--;break;
	    }
	if ((MiPanta[(80*y)+x]==' ') || (MiPanta[(80*y)+x]=='*'))
	    fg=1;
	}
    return direction;
	    
    }
    

int inteldirection(int direction,int x,int y,char MiPanta[])
/* OBJETIVO: Genera direcciones de movimiento para que un pacman busque
	las pastillas en la pantalla 
    NOTA:  Utiliza la funcion randomdirectio() */
    {
    int x1=x;
    int y1=y;
    int i=0;
    int fl=0;
    int direct=direction;
    
    while ( (i<=3) && (fl==0) )
	{
	x=x1;y=y1;
	switch (i)
	    {
	    case 1:x++;break;
	    case 2:y++;break;
	    case 3:x--;break;
	    case 0:y--;break;
	    }
	if (MiPanta[(80*y)+x]=='*')
	    {
	    beep();
	    fl=1;
	    direct=i;
	    }
	i++;
	}
    if (fl==0) 
	return (randomdirection(direction,x1,y1,MiPanta));
    else
	return (direct);
    }
	    		
    
    
    
            
    
    

void controltty(char SIMBOLO,int CONTROL,int id,char MiPanta[],int pip[],int sock)
{
/*  OBJETIVO:  Esta funcion obtiene bytes de alguna fuente y genera una direccion 
	de coordenadas nuevas correspondiente, enviandolas a traves de un pipe (pip)
	al proceso Monitor, que es el que se encargara de imprimirlas en pantalla y 
	gestionarlas. 
	    La codificacion actual es:
		CONTROL = 0    Generada al azar...
		CONTROL = 1    Obtiene el char por teclado del termina....
		CONTROL = 2    Obtiene el char mediante un socket usando getchremoto..
	*/
int Ppid=getpid();
int i=0;
int x;
int y;
int direction=1;
chtype c;
struct TipoMsgMonitor msg;
srand(getpid());  /* Libreria stdlib.h semilla para numeros aleatorios */
cbreak();noecho();  /* Cancela el echo de las teclas */
timeout(90);  /* Establece el tiempo de espera del teclado hasta liberar */
keypad(stdscr,TRUE);  /* Habilita los caracteres extendidos. */
/* Inicializacion segun si es fantasma o pacman */
msg.id=id;

if (CONTROL==1)
    {
    msg.x=POSINIX_P;//23
    msg.y=POSINIY_P;//19;
    }
else
    {
    msg.x=POSINIX_F; //23
    msg.y=POSINIY_F; //10 
    }
x=msg.x;
y=msg.y;    

while (1)
    {
    /* Se imprime en pantalla utilizando el proceso monitor */
    msg.x=x;
    msg.y=y;
    msg.SIMBOLO=SIMBOLO;
    write(pip[1],&msg,sizeof(msg));
    switch (CONTROL)
	{
	case 0:
	    {
	    /* Delay para los fantasmas */
	    usleep(80000);  // 130000 es un buen numero
	    }
	break;
	case 1:
	    {
	    /* Control desde la terminal actual */
	    c=getch();
	    if (c=='q')
		{
		break;
		};
	    }
	break;
	case 2:
	    {
	    /* GetchRemoto a traves del socket */
	    read(sock,&c,sizeof(chtype));
	    if (c=='q')
		{
		close(sock);
		CONTROL==0;
		}
	    
	    usleep(70000);
	    }
	break;
	}	
    
    if ( (CONTROL==1) || (CONTROL==2) )
	direction=setdirection(c,direction);
    else
	direction=randomdirection(direction,x,y,MiPanta);
	
    
    
    switch (direction)
	{
	case 1:x++;break;
	case 2:y++;break;
	case 3:x--;break;
	case 0:y--;break;
	}
    
    chequeapos(MiPanta,&x,&y,direction);
    }
    
printf ("Abandonaste !!!  Sos como Leo, medio medio....\n");
}



void main()
{
int Pid;
int Ppid=getpid();
char difi;
int tams;
char MiPanta[strlen(MiPantalla)];
int pip[2],sock;
struct TipoMsgMonitor msg;
struct TipoEstado est[C_JUGADOR];
struct sockaddr_in remtsa;


/* Apertura del socket de comunicaciones */
sock=abrirSocketS();

/* Aca deberia ir el hijo que se encargara de la rutina del accept */

printf ("Esperando conexion de algun cliente que oficiara de fantasma...\n");
tams=sizeof(remtsa);
if ( (est[1].sock=accept(sock,(void *)&remtsa,&tams))<0 )
    {
    printf ("Se ha producido un error al intentar conectarse al cliente...\n");
    printf ("Error devuelto por el kernel:%s",strerror(errno));
    exit(-1);
    }

    

printf ("Conexion exitosa !!!\n");
/* Lectura del nick..... */
tams=read(est[1].sock,est[1].nick,30);
est[1].nick[tams]='\0';

printf ("Conectado con %s.\n",est[1].nick);

/* Desbloqueo del read */
fcntl (est[1].sock,F_SETFL,O_NONBLOCK);

printf ("Listo para iniciar la partida \n");
printf ("Pulse una tecla por favor.....\n");
getc(stdin);



initscr();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);
pipe(pip);
est[0].puntos=0;


noecho();getch();
fork();
/* Si se agrega algun hijo se deben inicializar los datos para su estructura... */

if (Ppid==getppid())
    {
    controltty('!',2,1,MiPanta,pip,est[1].sock);close(est[1].sock);endwin();exit(0);
    }
else
    {
    fork();
    if (Ppid==getppid())
	{
	controltty('@',1,0,MiPanta,pip,0);endwin();exit(0);
	}
    else
	{
	fork();
	if (Ppid==getppid())
	    {
	    controltty('&',0,2,MiPanta,pip,0);endwin();exit(0);
	    }
	else
	    {
	    fork();
	    if (Ppid==getppid())
		{
		controltty('#',0,3,MiPanta,pip,0);endwin();exit(0);
		}
	    else
		{
		fork();
		if (Ppid==getppid())
		    {
		    controltty('+',0,4,MiPanta,pip,0);endwin();exit(0);
		    }
		}
	    }
	    
	}
    }


/* Inicializacion de las estructuras con los datos de fantasmas y de pacman */
est[0].y=19;est[0].x=23;
est[1].y=10;est[2].y=10;
est[3].y=10;est[4].y=10;
est[1].x=23;est[2].x=23;
est[3].x=23;est[4].x=23;
est[0].CONTROL=1;
est[1].CONTROL=2;
est[2].CONTROL=0;
est[3].CONTROL=0;
est[4].CONTROL=0;

/* Aca llega solo el padre */
while (1)
	{
	generarbox(est[0]);
	refresh();
	if (read(pip[0],&msg,sizeof(msg))<0 )
	    {exit(0);}
	move(msg.y,msg.x);
	addch(msg.SIMBOLO);
	
	if ( (msg.y!=est[msg.id].y) || (msg.x!=est[msg.id].x) )
	    {
	    move(est[msg.id].y,est[msg.id].x);
	    addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]);
	    }
	    
	est[msg.id].x=msg.x;
	est[msg.id].y=msg.y;
	if ( (accion(MiPanta,msg.x,msg.y,est,msg.id))<0 )
	    /* Salidas para cerrar las estructuras abiertas en caso de algun suceso */
	    break;
	}
    

getch();
close(est[1].sock);
close(sock);
endwin();
kill(0,SIGTERM);
exit(1);
}

