/*
Programa:  Dpacman.c
Objetivo:  Visualizacion y control de una unidad en la pantalla.
Version: 8.1
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
#include "sonidos.h"
#include <signal.h>


/* Constantes para las posiciones iniciales de las figuras en la pantalla */
#define POSINIX_P 23
#define POSINIY_P 19
#define POSINIX_F 23
#define POSINIY_F 10


/* Cantidad de vidas al empezar */
#define MAXVIDAS_P 3


/* Cantidad de jugadores (figuras) en la pantalla */
#define C_JUGADOR 5

struct TipoEstado
{
    /* Estructura de los datos de cada elemento en movimiento en la pantalla. */
    long int puntos;
    int vidas;
    int x;
    int y;
    int SIMBOLO;
    int CONTROL;
    int p_Modo;    // Modalidad 0 fantasma, 1 pacman, 2 pacman mata fantasma
    char nick[30];
    int id;
    int sock;
    int pip[2];
    int color;
    
};

struct TipoMsgMonitor
{
    /* Estructura de los datos que son enviados al Monitor por cada proceso */
    int x;
    int y;
    char SIMBOLO;
    int id;
};

void muestrapantalla(char MiPanta[])
/* Imprime el vector que representa la pantalla. */
    {
    int i=0;
    initscr();
    //start_color();
    //init_pair(1,COLOR_RED,COLOR_CYAN);refresh();
    for (i=0;i<strlen(MiPanta);i++)
	//addch((unsigned char)(MiPanta[i]+512));
    addch((unsigned char)MiPanta[i]);
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
    
    /* Control del paso de una punta a la otra */
    if (x==-1 && y==12) 
        {
	x=53;
	y=12;
	}
    else if (x==54 && y==12)
        {
        x=0;
        y=12;
	}
    else if ((x==25||x==27||x==28||x==26) && (y==-1))
	{
	x=27;
	y=24;
	}
    else if ((x==25||x==27||x==28||x==26) && (y==25))
	{
	x=27;
	y=0;
	}
    
    else
	{
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
	}
	
    
    (*xx)=x;
    (*yy)=y;
	
    return;
    }
    




long int contarpuntos(char MiPanta[])
    {
    /* OBJETIVO: Cuenta los * (pastillitas) que hay en la pantalla. */
    int i;
    long int cuenta=0;
    for (i=0;i<strlen(MiPanta);i++)
	if (MiPanta[i]=='*') cuenta++;
    return (cuenta);
    }
    
	
int accion(char MiPanta[],int x,int y,struct TipoEstado est[],int id)
    {
    /* Se puede analizar en la estructura para ver que tipo de control tiene y
    ver que hacer en el caso de que sea un pacman remoto */
    int i,j;
    struct TipoMsgMonitor msg;
    /* Se tuvo que implementar asi porque no me deja definir como valor inicial
    en una estatica una variable */
    static long int MAXPUNTOS=0;
    if (MAXPUNTOS==0) 
	MAXPUNTOS=contarpuntos(MiPanta);
    
    
    //if (est[id].p_Modo>0)
    
    for (j=0;j<C_JUGADOR;j++)
	{
	/* Chequeo de colisiones. */
	if (est[j].p_Modo>0)
	    {
	    for (i=0;i<C_JUGADOR;i++)
		{
		if ( (est[i].x==est[j].x) && (est[i].y==est[j].y) && (i!=j) )
		    {
		    if (est[j].p_Modo==1)
			{
			move(20,56);
			printw("Te agarro Tripode Leo !!");
	
			/* Chequeo de las vidas */
			if (est[j].vidas==0)
			    {
			    musicafunebre();
			    refresh();
			    return -1;
			    }
			else    
			    {
			    move(20,56);
			    printw("Otra oportunidad !!!!!!!");
			    /* Reenvia por el pipe privado de recepcion de cada
			    proceso, las nuevas posiciones de x e y */
			    msg.x=POSINIX_P;
			    msg.y=POSINIY_P;
			    
			    /* Se actualiza la posicion en el vector de estado */
			    est[j].x=POSINIX_P;
			    est[j].y=POSINIY_P;
			    write(est[j].pip[1],&msg,sizeof(msg));
			    est[j].vidas--;
			    }
			}
		    else
			{
			/* Comer al fantasmita */
			msg.x=POSINIY_F;
			msg.y=POSINIY_F;
			write(est[i].pip[1],&msg,sizeof(msg));
			
			}
		    }
		}
	    }
	}
	
    /*if ( (est[id].p_Modo==0) && (est[id].x==est[0].x) && (est[id].y==est[0].y) )
	{
	move(20,56);
	printw("Te agarro Tripode Leo !!");
	
	/* Chequeo de las vidas *//*
	if (est[0].vidas==0)
	    {
	    musicafunebre();
	    refresh();
	    return -1;
	    }
	else    
	    {
	    move(20,56);
	    printw("Otra oportunidad !");
	    // Reenviar por el pipe las nuevas posiciones.
	    est[0].vidas--;
	    }
	    
	}*/
	
    if (est[id].puntos==MAXPUNTOS)
	{
	move(23,1);
	printw ("Muy buena Guacho PACMAN!!!!  O tuviste culo como Marcelo ?? O como la hermana ??\n");
	musicagolpe();
	refresh();
	return -1;
	}
	
    if (est[id].p_Modo>0)
	if (MiPanta[(80*y)+x]=='*') 
	    {
	    //beep();refresh();
	    
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
    move(2,60);printw ("Vidas : %d",est.vidas);
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


    

void controltty(char SIMBOLOA,char SIMBOLOB,int CONTROL,int id,int p_Modo,char MiPanta[],int pip[],int pac_fan[],int mon_fan[],int sock)
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
int selSimbol=0;    /* Controla movimiento individual de cada personaje */
chtype c;
struct TipoMsgMonitor msg,pacMsg,monMsg;
cbreak();noecho();  /* Cancela el echo de las teclas */
timeout(90);  /* Establece el tiempo de espera del teclado hasta liberar */
keypad(stdscr,TRUE);  /* Habilita los caracteres extendidos. */

/* Inicializacion segun si es fantasma o pacman */
msg.id=id;

if (p_Modo>0)
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
    
    if ( (read(mon_fan[0],&monMsg,sizeof(monMsg)))>0 )
	{
	refresh();
	msg.x=monMsg.x;
	msg.y=monMsg.y;
	x=msg.x;
	y=msg.y;
	}	

    write(pip[1],&msg,sizeof(msg));
    	
    
    switch (CONTROL)
	{
	case 0:
	    {
	    /* Delay para los fantasmas */
	    usleep(130000);  // 8000 es un buen numero
	    }
	break;
	case 1:
	    {
	    /* Control desde la terminal actual */
	    c=getch();
	    if (c=='q')
		{
		msg.x='q';
		
		/* En caso de que se aprete la q se le envia el dato como
		coordenada x al Monitor, que lo chequea y si es q termina */
		write(pip[1],&msg,sizeof(msg));
		return;
		};
	    }
	break;
	case 2:
	    {
	    /* GetchRemoto a traves del socket */
	    read(sock,&c,sizeof(chtype));
	    if (c=='q')
		{
		/* Si la letra que se recibe es una q, la computadora toma el
		lugar del getch remoto */
		close(sock);
		CONTROL=0;
		}

	    usleep(100000);  // 70000
	    }
	break;
	}

    if ( (CONTROL==1) || (CONTROL==2) )
       {
       /* Ejecuta si es pacman o fantasma remoto */
	direction=setdirection(c,direction);
        if ( (p_Modo>0) && (direction==1) ) 
	    /* Envia la posicion vieja del pacman 2 veces. */
	    /* Este write debe ser chequeado porque se puede bloquear
	    si se llena el buffer, lo que provoca un retardo realmente 
	    importante para el pacman */
	    {
		write(pac_fan[1],&msg,sizeof(msg));
	    }
	}
    else
	{
        /* Ejecuta si es fantasma */
	if (read(pac_fan[0],&pacMsg,sizeof(pacMsg))>=0)
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



void inicializar(struct TipoEstado est[])
    {
    /*OBJETIVO:  Esta rutina se encarga de la inicializacion del vector
    de estados. */
    /* Nota:  Es importante que esta rutina tenga concordancia con los
    demas valores con los que se crean los hijos y se crean los sockets. */
    /* Esta funcion coloca los valores de sockets y control en cero, si 
    se desean conectar sockets, se deben especificar luego de poner esta funcion */
    int i;
    
    for (i=0;i<C_JUGADOR;i++)
	{
	
	/* Creacion de los pipes Monitor--Fantasmas (no bloqueante) */
	pipe(est[i].pip);
	fcntl(est[i].pip[0],F_SETFL,O_NONBLOCK);
	
	
	if (i==0)   // Para que por ahora sea el 0 el pacman.
	    {
	    est[i].p_Modo=1;
	    est[i].CONTROL=1;
	    }
	else
	    {
	    est[i].p_Modo=0;
	    est[i].CONTROL=0;
	    }
	est[i].color=0;
	est[i].sock=0;
	est[i].id=i;
	if (est[i].p_Modo==0)
	    {
	    est[i].y=POSINIY_F;
	    est[i].x=POSINIX_F;
	    est[i].puntos=0;
	    est[i].vidas=0;
	    }
	else
	    {
	    est[i].y=POSINIY_P;
	    est[i].x=POSINIX_P;
	    est[i].puntos=0;
	    est[i].vidas=MAXVIDAS_P;
	    }
	}
	
    
    }



void conexiones(int sock,struct TipoEstado est[],int cant)
    {
    /* OBJETIVO:  Conecta a sockets tantos usuarios como se especifiquen en
    cant */
    
    int tams,i;
    struct sockaddr_in remtsa;
    for (i=1;i<=cant;i++)
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
	est[i].CONTROL=2;

	printf ("Conectado con %s.\n",est[i].nick);

	/* Desbloqueo del read */
	fcntl (est[i].sock,F_SETFL,O_NONBLOCK);

	}
    }






main(char *argv[],int argc)
{
int Pid;
int Ppid=getpid();
char difi;
char MiPanta[strlen(MiPantalla)];
int pip[2],pac_fan[2],sock;
struct TipoMsgMonitor msg;
struct TipoEstado est[C_JUGADOR];

/*if ( (argv[1][0]-48)>C_JUGADOR )
    {
    printf ("Necesita poner la cantidad de jugadores remotos (valor Maximo:%d)\n",C_JUGADOR);
    exit(-1);
    }*/
    

srand(time(NULL)); //Agrega semilla para azar.


/*  Inicializacion de los datos de los personajes */
inicializar(est);


/* Apertura del socket de comunicaciones */
sock=abrirSocketS();

/* Esta implementacion funciona una vez que se conectan todos se larga el juego */
/* Espera a que se conecten todos los que se especifican.... */

conexiones(sock,est,3);

printf ("Listo para iniciar la partida \n");
printf ("Pulse una tecla por favor.....\n");
getc(stdin);




initscr();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);

/* Aperturas de canales para IPC */
/* Creacion del pipe de comunicaciones Fantasmas--Monitor */
pipe(pip);

/* Creacion del pipe de comunicaciones Fantasmas--Pacman (no bloqueante) */
pipe(pac_fan);
fcntl(pac_fan[0],F_SETFL,O_NONBLOCK);



/* Inicializacion de las estructuras con los datos de fantasmas y de pacman */


noecho();getch();
fork();
/* Si se agrega algun hijo se deben inicializar los datos para su estructura... */
/* Creacion de hijos para oficiar de personajes.... */

    
if (Ppid==getppid())
    {
    controltty('O','o',est[1].CONTROL,1,est[1].p_Modo,MiPanta,pip,pac_fan,est[1].pip,est[1].sock);close(est[1].sock);endwin();exit(0);
    }
else
    {
    fork();
    if (Ppid==getppid())
	{
	controltty('C','c',est[0].CONTROL,0,est[0].p_Modo,MiPanta,pip,pac_fan,est[0].pip,est[0].sock);endwin();exit(0);
	}
    else
	{
	fork();
	if (Ppid==getppid())
	    {
	    controltty('H','h',est[2].CONTROL,2,est[2].p_Modo,MiPanta,pip,pac_fan,est[2].pip,est[2].sock);close(est[2].sock);endwin();exit(0);
	    }
	else
	    {
	    fork();
	    if (Ppid==getppid())
		{
		controltty('E','e',est[3].CONTROL,3,est[3].p_Modo,MiPanta,pip,pac_fan,est[3].pip,est[3].sock);endwin();exit(0);
		}
	    else
		{
		fork();
		if (Ppid==getppid())
		    {
		    controltty('J','j',est[4].CONTROL,4,est[4].p_Modo,MiPanta,pip,pac_fan,est[4].pip,est[4].sock);endwin();exit(0);
		    }
		}
	    }
	    
	}
    }






/* Aca llega solo el padre */
while (1)
	{
	generarbox(est[0]);
	refresh();
	/* Lectura del pipe de los procesos hijos */
	read(pip[0],&msg,sizeof(msg));
	
	
	/* Si el proceso PACMAN mando una q */
	if (msg.x=='q')
	    break;
	
	est[msg.id].color++;
	move(5,56);printw ("%d %d %d",est[0].color,est[1].color,est[4].color);
	move(msg.y,msg.x);
	addch(msg.SIMBOLO);
	
	/* Se actualiza la pantalla si hubo algun cambio en las coordenadas */	
	if ( (msg.y!=est[msg.id].y) || (msg.x!=est[msg.id].x) )
	    {
	    move(est[msg.id].y,est[msg.id].x);
	    addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]);
	    }
	    
	/* Actualizacion del vector de estado para esta modificacion */
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

/* Eliminacion de todos los procesos del grupo */
kill(0,SIGTERM);

exit(1);

}

    
