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
#include "laberinto.h"


struct TipoEstado
{
    /* Estructura de los datos de cada elemento en movimiento en la pantalla. */
    long int puntos;
};

struct TipoMsgMonitor
{
    int x;
    int y;
    char SIMBOLO;
};


void monitor(int pip[],char MiPanta[])
    {
    /* Esta funcion es la unica que imprime en pantalla 
	Recibe datos de la tuberia pip y los coloca en pantalla */
	
    
    struct TipoMsgMonitor msg;
    //initscr();
    while (1)
	{
	refresh();
	if (read(pip[0],&msg,sizeof(msg))<0 )
	    {exit(0);}
	move(msg.y,msg.x);
	addch(msg.SIMBOLO);
	/* {move(1,53);printw ("Simbolo %c, x %d, y %d:",msg.SIMBOLO,msg.x,msg.y);}*/
	}
    }
    

void hijomonitor(int pip[],char MiPanta[])
    {
    int Ppid=getpid();
    noecho();
    getch();
    fork();
    if (Ppid==getppid())
	{monitor(pip,MiPanta);exit(0);}
    else
	return;
    }
    



void muestrapantalla(char MiPanta[])
/* Imprime el vector que representa la pantalla. */
    {
    int i=0;
    //initscr();
    
    for (i=0;i<strlen(MiPanta);i++)
	addch (MiPanta[i]);
    move(1,1);
    //endwin();
    }
    

/* Cheqeua si callo en un lugar que no puede pasar y corrije el valor. */
void chequeapos(MiPanta,xx,yy,direction)
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
	{
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
    }
    
	
void accion(char MiPanta[],int x,int y,struct TipoEstado *est,int CONTROL)
    {
    if (CONTROL==1)
	if (MiPanta[(80*y)+x]=='*') 
	    {
	    beep();refresh();
	    /* Actualizacion del vector modelo de la pantalla */
	    MiPanta[(80*y)+x]=' ';
	    est->puntos++;
	    }
	
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
/* OBJETIVO: Hace que el pacman busque un poco mejor las *. */
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
	    		
    
    
    
            
    
    

void controltty(char SIMBOLO,int CONTROL,char MiPanta[],int pip[])
{
int Ppid=getpid();

int i=0;
int x;
long j;
int y;
int direction=1;
chtype c;
struct TipoEstado est;
struct TipoMsgMonitor msg;
est.puntos=0;
srand(9);  /* Libreria stdlib.h semilla para numeros aleatorios */
cbreak();noecho();  /* Cancela el echo de las teclas */
timeout(130);  /* Establece el tiempo de espera del teclado hasta liberar */
keypad(stdscr,TRUE);  /* Habilita los caracteres extendidos. */

/* Inicializacion segun si es fantasma o pacman */
if (CONTROL==1)
    {
    msg.x=23;
    msg.y=19;
    }
else
    {
    msg.x=23;msg.y=10;
    }
x=msg.x;y=msg.y;    

while (1)
    {
    /* Se imprime en pantalla utilizando el proceso monitor */
    msg.SIMBOLO=MiPanta[(80*y)+x];
    write(pip[1],&msg,sizeof(msg));
    if(CONTROL==1)
	c=getch();
    else 
	for(j=0;j<100000;j++) fflush(stdin);
	
    msg.x=x;
    msg.y=y;
    msg.SIMBOLO=SIMBOLO;
    write(pip[1],&msg,sizeof(msg));
    
    /*msg.SIMBOLO=MiPanta[(80*y)+x];
    if(CONTROL==1)
	c=getch();
    else 
	for(j=0;j<100000;j++) fflush(stdin);

    write(pip[1],&msg,sizeof(msg));*/
        
    /*move(y,x);   
    /*addch(SIMBOLO);  /* Mete el PACMAN, jajaj */
    /*refresh();   /* Refresh para ver lo que se cambio en memoria. */
    /*c=getch();   /* Captura de tecla semibloqueante */
    /*move(y,x);
    /*addch(' ');  /* Borra el pacman en la posicion vieja */
    
    if (c=='q') break;
    
    if (CONTROL==1)
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
    accion(MiPanta,x,y,&est,CONTROL);	
    
    }
    
//endwin();    /* Cierra ncurses */
printf ("Abandonaste !!!  Sos como Leo, medio medio....\n");
}



void main()
{
int Pid;
int Ppid=getpid();
char MiPanta[strlen(MiPantalla)];
int pip[2];

initscr();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);
pipe(pip);

hijomonitor(pip,MiPanta);

fork();

if (Ppid==getppid())
    {
    controltty('!',0,MiPanta,pip);endwin();exit(0);
    }
else
    {
    fork();
    if (Ppid==getppid())
	{
	sleep(1);
	controltty('&',0,MiPanta,pip);endwin();exit(0);
	}
    else
	{
	fork();
        if (Ppid==getppid())
	    {
	    sleep(2);
	    controltty('$',0,MiPanta,pip);endwin();exit(0);
    	    }
    	else
    	    {
	    fork();
    	    if (Ppid==getppid())
		{
		sleep(3);
		controltty('#',0,MiPanta,pip);endwin();exit(0);
		}
    	    else
		{
		/* Este es el padre */
		controltty('@',1,MiPanta,pip);endwin();exit(0);
		}
	    }
        }
}
endwin();
exit(1);
}

