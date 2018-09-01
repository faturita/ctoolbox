/*
Programa:  pantallaconcu.c
Objetivo:  Visualizacion y control de una unidad en la pantalla.
Version: 8
Autor: Oel Amon Yag.
Fecha:9/9/99
Parametros: ninguno por ahora.
NOTA:  Compilar con -lncurses

*/



#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <fcntl.h>
#include "laberinto.h"
#include "comunicaciones.h"
#include <sys/ioctl.h>
#include </usr/include/linux/kd.h>
#include <signal.h>


#define POSINIX_P 23
#define POSINIY_P 19
#define POSINIX_F 27
#define POSINIY_F 11

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
    int color;
};

struct TipoMsgMonitor
{
    int x;
    int y;
    char SIMBOLO;
    int id;
        
};

/*FUNCIONES PARA MANEJO DE SONIDO*/
void sonido(int frec,int dura)
    {
    ioctl(0,KIOCSOUND,(int)(1190000/frec));
    usleep(dura*500);
    ioctl(0,KIOCSOUND,40); //deberia ir 0 aca pero no funciona!!!
    }

void musicagolpe()
    {
    sonido(293, 300);
    sonido(311, 300);
    sonido(329, 300);
    sonido(522, 500);
    sonido(329, 300);
    sonido(522, 400);
    sonido(329, 300);
    sonido(522, 900);
    usleep(300*500);
    sonido(522, 300);
    sonido(586, 300);
    sonido(622, 250);
    sonido(658, 250);
    sonido(522, 250);
    sonido(586, 300);
    sonido(658, 550);
    usleep(30*500);
    sonido(522, 300);
    sonido(586, 500);
    usleep(50*500);
    sonido(522, 400);
    usleep(150*500);
    sonido(392, 500);
    usleep(100*500);
    sonido(261, 500);
    }


void musicafunebre()
    {
    sonido(293,800);
    usleep(100*500);
    sonido(293,500);
    usleep(100*500);
    sonido(293,300);
    usleep(100*500);
    sonido(293,500); 
    usleep(100*500);
    sonido(349,500);
    sonido(329,300);
    sonido(329,500);
    sonido(293,300);
    sonido(293,500);
    sonido(277,300);
    sonido(293,800);
    }

/*FIN DE FUNCIONES DE MANEJO DE SONIDO*/    

void muestrapantalla(char MiPanta[])
/* Imprime el vector que representa la pantalla. */
    {
    int i=0;
    initscr();
    start_color();
        for (i=0;i<strlen(MiPantalla);i++)
   	{if (MiPantalla[i]==42)
	    {init_pair(8,COLOR_RED,COLOR_BLACK);
	    addch(MiPantalla[i]+8*256);
	    }	        
	else if(MiPantalla[i]==177)
		{attron(A_BLINK);
		attron(A_BOLD);
		init_pair(1,COLOR_GREEN,COLOR_BLACK);
		addch (MiPantalla[i]+1*256);
		attroff(A_BLINK);
		attroff(A_BOLD);
		}
	else 
	{init_pair(9,COLOR_BLUE,COLOR_BLACK);
	addch (MiPantalla[i]+9*256);}
	}
    move(1,1);
    }

void agregacomida(char MiPanta[])
/* Agrega comida.*/
    {
    int columna,linea,valor;
    int total=0;
    while(total<60) //Variando este numero calculo la densidad de comida.
	{
        columna=((int) (55.0*rand()/(RAND_MAX+1.0)));
        linea=((int) (25.0*rand()/(RAND_MAX+1.0)));
        valor=(linea*80)+columna;
        if (MiPanta[valor]==' ')
	    {
	    move(linea,columna);
	    addch(42);
	    MiPanta[valor]='*';
	    total++;
	    }
        }
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
        if (x==-1 && y==12) //SI SE VA POR UN COSTADO LO POSICIONA EN EL OTRO
        {x=53;y=12;}
    else if (x==54 && y==12)
            {x=0;y=12;}
	 else if ((x==25||x==27||x==28||x==26) && (y==-1))
		{x=27; y=24;}
	      else if ((x==25||x==27||x==28||x==26) && (y==25))
		    {x=27;y=0;}

    
    
    else{
    if ( (MiPanta[(80*y)+x]==' ') || (MiPanta[(80*y)+x]=='*') || (MiPanta[(80*y)+x]=='±') )
	return;
    else
	
	switch (direction)
	    {
	    case 1:x--;break;
	    case 2:y--;break;
	    case 3:x++;break;
	    case 0:y++;break;
	    }
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
		printw("Te comio !!!! El fantasma Tripode Leo !!\n");
		musicafunebre();
		refresh();
		return -1;
		}
	    }
	}
    
    if ( (id!=0) && (est[id].x==est[0].x) && (est[id].y==est[0].y) )
	{
	move(23,1);
	printw("Te comio !!!! El fantasma Tripode Leo !!\n");
	musicafunebre();
	refresh();
	return -1;
	}
	
    if (est[0].puntos==258)
	{
	move(23,1);
	printw ("Muy buena Guacho PACMAN!!!!  O tuviste culo como Marcelo ?? O como la hermana ??\n");
	musicagolpe();
	refresh();
	return -1;
	}
    if (est[id].CONTROL==1)
	if (MiPanta[(80*y)+x]=='*') 
	    {
	    sonido(640,25);
	    //beep();
	    refresh();
	    /* Actualizacion del vector modelo de la pantalla */
	    MiPanta[(80*y)+x]=' ';
	    est[id].puntos++;
	    }

//BORRA EL CARACTER ± 
	else if (MiPanta[(80*y)+x]=='±')
	    {sonido(522, 100);
             sonido(586, 100);
             MiPanta[(80*y)+x]=' ';
	    
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

int pacmandirection(int direction,int pacXpos,int pacYpos,int fanXpos,int fanYpos,char MiPanta[])
/*OBJETIVO: dar inteligencia a los perseguidores controlados por la computadora.*/
    {
    int x,y,counter;
    int difX=pacXpos-fanXpos;
    int difY=pacYpos-fanYpos;
    int XoY;
    int directionX,directionY;

    if (difX>0) directionX=1;
    if (difX<0) directionX=3;
    if (difY>0) directionY=2;
    if (difY<0) directionY=0;
for(counter=0;counter<3;counter++)
    {
    x=fanXpos;
    y=fanYpos;
    XoY=(int) (2.0*rand()/(RAND_MAX+1.0))+1;
    if (XoY==1) direction=directionX;
    if (XoY==2) direction=directionY;
    switch (direction)
        {
        case 1:x++;break;
        case 2:y++;break;
        case 3:x--;break;
        case 0:y--;break;
        }

    if ((MiPanta[(80*y)+x]==' ') || (MiPanta[(80*y)+x]=='*')) break;
    }
    return (direction);
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
	    		
    
    
    
            
    
    

void controltty(char SIMBOLOA,char SIMBOLOB,int CONTROL,int id,char MiPanta[],int pip[],int pac_fan[],int sock)
{
/*  OBJETIVO:  Esta funcion obtiene bytes de alguna fuente y genera una direccion
	de coordenadas nuevas correspondiente, enviandolas a traves de un pipe (pip)
	al proceso Monitor, que es el que se encargara de imprimirlas en pantalla y 
	gestionarlas. 
	    La codificacion actual es:
		CONTROL = 0    Generada al azar...
		CONTROL = 1    Obtiene el char por teclado del terminal....
		CONTROL = 2    Obtiene el char mediante un socket usando getchremoto..
	*/
int Ppid=getpid();
int m;
int i=0;
int x;
int y;
int direction=1;
int selSimbol=0;
chtype c;
struct TipoMsgMonitor msg,pacMsg;
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
    msg.x=POSINIX_F; //28
    msg.y=POSINIY_F; //12
    }
x=msg.x;
y=msg.y;

while (1)
    {
    /* Se imprime en pantalla utilizando el proceso monitor */
    msg.x=x;
    msg.y=y;
    if (selSimbol)
	{
	msg.SIMBOLO=SIMBOLOA;
	selSimbol--;
	}
    else
	{
	msg.SIMBOLO=SIMBOLOB;
	selSimbol++;
	}

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
		return;
		};
	    }
	break;
	case 2:
	    {
	    /* GetchRemoto a traves del socket */
	    c='a';
	    read(sock,&c,sizeof(chtype));
	    if (c=='q')
		{
		close(sock);
		CONTROL=0;
		}

	    usleep(70000);
	    }
	break;
	}

    if ( (CONTROL==1) || (CONTROL==2) )
   {
	direction=setdirection(c,direction);
        if (id==0)
	    for (m=0;m<2;m++)
		write(pac_fan[1],&msg,sizeof(msg)); /*Envia la posicion vieja del PacMan*/
	}
    else
	{
        if (read(pac_fan[0],&pacMsg,sizeof(pacMsg))<0)
	    direction=pacmandirection(direction,pacMsg.x,pacMsg.y,x,y,MiPanta);
	else
	    direction=randomdirection(direction,x,y,MiPanta);
	}

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



main()
{
int Pid;
int Ppid=getpid();
char difi;
int tams,i;
char MiPanta[strlen(MiPantalla)];
int pip[2],pac_fan[2],sock;
struct TipoMsgMonitor msg;
struct TipoEstado est[C_JUGADOR];
struct sockaddr_in remtsa;

srand(time(NULL)); //Agrega semilla para azar.

/* Apertura del socket de comunicaciones */
sock=abrirSocketS();

/* Aca deberia ir el hijo que se encargara de la rutina del accept */

for (i=1;i<=3;i++) 
{
printf ("Esperando conexion de algun cliente que oficiara de fantasma...\n");
tams=sizeof(remtsa);
if ( (est[i].sock=accept(sock,(void *)&remtsa,&tams))<0 )
    {
    printf ("Se ha producido un error al intentar conectarse al cliente...\n");
    //printf ("Error devuelto por el kernel:%s",strerror(errno));
    exit(-1);
    }

    

printf ("Conexion exitosa !!!\n");
/* Lectura del nick..... */
tams=read(est[i].sock,est[i].nick,30);
est[i].nick[tams]='\0';

printf ("Conectado con %s.\n",est[1].nick);

/* Desbloqueo del read */
fcntl (est[i].sock,F_SETFL,O_NONBLOCK);
}
printf ("Listo para iniciar la partida \n");
printf ("Pulse una tecla por favor.....\n");
getc(stdin);


initscr();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);
pipe(pip);
pipe(pac_fan);
fcntl(pac_fan[0],F_SETFL,O_NONBLOCK);
est[0].puntos=0;


noecho();getch();
fork();
/* Si se agrega algun hijo se deben inicializar los datos para su estructura... */

if (Ppid==getppid())
    {
    controltty('O','o',2,1,MiPanta,pip,pac_fan,est[1].sock);close(est[1].sock);endwin();exit(0);
    }
else
    {
    fork();
    if (Ppid==getppid())
	{
	controltty('C','c',1,0,MiPanta,pip,pac_fan,0);endwin();exit(0);
	}
    else
	{
	fork();
	if (Ppid==getppid())
	    {
	    controltty('H','h',2,2,MiPanta,pip,pac_fan,est[2].sock);endwin();exit(0);
	    }
	else
	    {
	    fork();
	    if (Ppid==getppid())
		{
		controltty('E','e',2,3,MiPanta,pip,pac_fan,est[3].sock);endwin();exit(0);
		}
	    else
		{
		fork();
		if (Ppid==getppid())
		    {
		    controltty('J','j',0,4,MiPanta,pip,pac_fan,0);endwin();exit(0);
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
est[2].CONTROL=2;
est[3].CONTROL=2;
est[4].CONTROL=0;
est[0].color=2;
est[1].color=0; 
est[2].color=4; //h
est[3].color=5; //j
est[4].color=3;

/* Aca llega solo el padre */
//agregacomida(MiPanta);
while (1)
	{
	generarbox(est[0]);
	refresh();
	if (read(pip[0],&msg,sizeof(msg))<0 )
	    {exit(0);}
	move(msg.y,msg.x);

//AGREGADO  (MIO-MARCELO) PARA QUE LOS PACMAN Y FANTASMAS CAMBIEN DE COLOR

init_pair(est[msg.id].color,est[msg.id].color,COLOR_BLACK);
	addch(msg.SIMBOLO+est[msg.id].color*256);
	
	if ( (msg.y!=est[msg.id].y) || (msg.x!=est[msg.id].x) )
	    {
	    move(est[msg.id].y,est[msg.id].x);

//ESTO ES PARA QUE CUANDO LOS FANTASMAS PASEN POR ENCIMA DE UN * o de ± LE DEJEN EL
//COLOR QUE TENIAN
	    
//NO MODIFICAR LOS VALORES DE LOS COLORES!!!!!!!!!!!!!!!!


       if (MiPanta[(80*est[msg.id].y)+est[msg.id].x]=='±')
	    {
	    attron(A_BLINK);
	    attron(A_BOLD);
	    init_pair(2,0,COLOR_BLUE);
	    addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]+2*256);
	    attroff(A_BLINK);
	    attroff(A_BOLD);
	    }
	    
	else 
	    {
	    init_pair(8,COLOR_RED,COLOR_BLACK);
	    addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]+8*256);
	    }
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
    
