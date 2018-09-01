/*
*******************************************************************************
Programa:  Dpacman.c
*******************************************************************************
Objetivo:  PACMAN DISTRIBUIDO.   PRIMERA PARTE
    El objetivo de este programa es plantear el juego pacman con la posibilidad
    de jugar en una red donde uno hace de pacman, y los demas actuan de fantasmas
    
Version: 10

Autor: Oel Amon Yag.

Fechas:
    Creacion:9/9/99
    Actualizacion: 29/9/99

Parametros: Debe recibir la cantidad de usuarios que se van a conectar

NOTA:  Compilar con -lncurses


*******************************************************************************
*/



#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <fcntl.h>
#include "laberinto.h"
#include "comunicaciones.h"
#include "Sonido.h"
#include <signal.h>
#include <sys/times.h>
#include "Colores.h"

/* Constantes para las posiciones iniciales de las figuras en la pantalla */
#define POSINIX_P 26
#define POSINIY_P 19
#define POSINIX_F 26
#define POSINIY_F 10

/* Cantidad de ciclos (veces que imprime en pantalla el pacman) hasta que se 
suelta del modo COME FANTASMAS */
#define C_CICLOSMODO 100

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
    int SIMBOLOB;
    int CONTROL;
    int p_Modo;    // Modalidad 0 fantasma, 1 pacman, 2 pacman mata fantasma
    char nick[30];
    int id;
    int sock;
    int pip[2];
    int color;
    int direction;
};

struct TipoMsgMonitor
{
    /* Estructura de los datos que son enviados al Monitor por cada proceso */
    int x;
    int y;
    char SIMBOLO;
    int modalidad;
    int id;	   // Id del proceso emisor del mensaje
    int direction; // Nuevas direcciones tomadas
    int aviso;     // Avisos que se mandan los procesos
};

void muestrapantalla(char MiPanta[])
/* Imprime el vector que representa la pantalla. */
    {
    int i=0;
    for (i=0;i<strlen(MiPantalla);i++)
   	{
	if (MiPantalla[i]==42)
	    {
	    addch(MiPantalla[i]+20*256);
	    }
	else if(MiPantalla[i]==177)
		{
		attron(A_BOLD | A_BLINK);
		addch (MiPantalla[i]+21*256);
		attroff(A_BOLD | A_BLINK);
		}
	    else
	    {
	    attron(A_DIM);
	    addch (MiPantalla[i]+22*256);
	    attroff(A_DIM);
	    }
	}
    move(1,1);
    }


void chequeapos(MiPanta,xx,yy,direction)
/* OBJETIVO: Toma una direccion y una coordenada, realiza el movimiento logico
y chequea si es posible.  En caso afirmativo deja todo como esta */
char MiPanta[];
int *xx;
int *yy;
int direction;
    {
    int x=*xx;
    int y=*yy;
    
    /* Ajuste da la nueva direccion y posicion */
    switch (direction)
	{
	case 0:y--;break;
	case 1:x++;break;
	case 2:y++;break;
	case 3:x--;break;
	}
	
	
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
	if (!( (MiPanta[(80*y)+x]==' ') || (MiPanta[(80*y)+x]=='*') || (MiPanta[(80*y)+x]=='±') ))
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
    int i,j;
    struct TipoMsgMonitor msg;
    /* Se tuvo que implementar asi porque no me deja definir como valor inicial
    en una estatica una variable */
    static int Ciclos=0;
    static long int MAXPUNTOS=0;
    if (MAXPUNTOS==0)
	MAXPUNTOS=contarpuntos(MiPanta);



    for (j=0;j<C_JUGADOR;j++)
	{
	/* Chequeo de colisiones. */
	if (est[j].p_Modo>0)
	    {
	    for (i=0;i<C_JUGADOR;i++)
		{
		if ( (est[i].x==est[j].x) && (est[i].y==est[j].y) && (i!=j) && (est[i].p_Modo==0) )
		    {
		    if (est[j].p_Modo==1)
			{
			move(13,56);
			printw("Te agarro Tripode Leo !");
			
			/* Incremento los puntos de ese fantasma */
			est[i].puntos++;

			/* Chequeo de las vidas */
			if (est[j].vidas==0)
			    {
                            musicafunebre(0);
			    refresh();
			    return -1;
			    }
			else
			    {
			    move(13,56);
			    printw("Otra oportunidad !!!!!!");
			    /* Actualiza el vector de estado de este pacman */
			    est[j].x=POSINIX_P;
			    est[j].y=POSINIY_P;
			    est[j].vidas--;
			    }
			}
		    else if (est[j].p_Modo==2)
			{
			/* Comer al fantasmita */
			est[i].x=POSINIX_F;
			est[i].y=POSINIY_F;
			}
		    }
		}
	    }
	}

    if (est[id].puntos==MAXPUNTOS)
	{
	move(25,1);
	printw ("Muy buena Guacho PACMAN!!!! No digo nada de nadie porque sino despues se ofenden...\n");
	musicagolpe(0);
	refresh();
	return -1;
	}

    if (est[id].p_Modo>0)
	{
	if (est[id].p_Modo==2)
	    {
	    Ciclos++;
	    if (Ciclos==C_CICLOSMODO)
		{
		est[id].p_Modo=1;
		Ciclos=0;
		}
	    }
	if (MiPanta[(80*y)+x]=='*')
	    {
            musicacomiendo(1);
	    refresh();

	    /* Actualizacion del vector modelo de la pantalla */
	    MiPanta[(80*y)+x]=' ';
	    est[id].puntos++;
	    }
	else if (MiPanta[(80*y)+x]=='±')
   	    {
    	    musicacomiendo1(1);
    	    refresh();
    	    MiPanta[(80*y)+x]=' ';
	    est[id].p_Modo=2;
	    Ciclos=0;
    	    }
	}
	
    return 1;

    }


void generarbox(struct TipoEstado est[])
/* OBJETIVO:  Imprime datos en el cuadro de dialogo */
    {
    int i;
    move(1,60);printw ("Pers Puntos Vidas");
    for (i=0;i<C_JUGADOR;i++)
	{
	move(2+i,60);
	printw ("%4c %5d %5d",est[i].SIMBOLO,est[i].puntos,est[i].vidas);
	}
	
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


int randomdirection(int direction)
/*  OBJETIVO: Devuelve un int que representa la direccion en modo SINCONTROL
    METODOLOGIA: Genera un numero aleatorio */
    {
    int r=( (int) (10.0*rand()/(RAND_MAX+1.0)));
    if (r==1) direction=0;
    if (r==2) direction=1;
    if (r==3) direction=2;
    if (r==4) direction=3;
    return direction;
    }

int mod(int x) 
/*  OBJETIVO: Fncion matematica valor absoluto */
    {
    if (x>0) return x;
    if (x<0) return ( (-1)*x);
    return 0;
    }
    
int getpacid(struct TipoEstado est[],int fid)
/*  OBJETIVO: Devuelve el numero de id de un pacman que este jugando 
    METODOLOGIA: Se fija cual es el pacman que esta mas cerca de este
    fantasma y devuelve su id */
    {
    int pacid=0;
    int i,minX=56,minY=25;
    int minaX,minaY;
    
    for (i=0;i<C_JUGADOR;i++)
	{
	if (est[i].p_Modo>0)
	    {
	    if (pacid==0) pacid=i;
	    minaX=mod(est[i].x-est[fid].x);
	    minaY=mod(est[i].y-est[fid].y);
	    if ( (minaX<minX) && (minaY<minY) )
		{
		minX=minaX;
		minY=minaY;
		pacid=i;
		}
	    }
	}
    return pacid;
    }
    	    
int pacmandirection(int direction,int pacXpos,int pacYpos,int fanXpos,int fanYpos,char MiPanta[],int modalidad)
/*OBJETIVO: dar inteligencia a los perseguidores controlados por la computadora.*/
    {
    int x,y,counter;
    int difX=pacXpos-fanXpos;
    int difY=pacYpos-fanYpos;
    int XoY;
    int directionX,directionY;

    if (modalidad==1)
	{
	if (difX>0) directionX=1;
        if (difX<0) directionX=3;
        if (difY>0) directionY=2;
        if (difY<0) directionY=0;
	}
    else
	{
	if (difX>0) directionX=3;
        if (difX<0) directionX=1;
        if (difY>0) directionY=0;
        if (difY<0) directionY=2;
	}

    do
    {
    x=fanXpos;
    y=fanYpos;
    XoY=(int) (3.0*rand()/(RAND_MAX+1.0))+1;
    if (XoY==1) direction=directionX;
    if (XoY==2) direction=directionY;
    if (XoY==3) direction=randomdirection(direction);
    switch (direction)
	{
        case 1:x++;break;
        case 2:y++;break;
        case 3:x--;break;
        case 0:y--;break;
        }

    } while ((MiPanta[(80*y)+x]!=' ') && (MiPanta[(80*y)+x]!='*') && (MiPanta[(80*y)+x]!='±'));

    return (direction);
    }




void controltty(char SIMBOLOA,char SIMBOLOB,int CONTROL,int id,int p_Modo,char MiPanta[],int pip[],int mon_fan[],int sock)
{
/*  OBJETIVO:  Obtiene datos de alguna fuente y genera nuevas direcciones de 
posicionamiento.
	    La codificacion actual es:
		CONTROL = 0    Generada al azar...
		CONTROL = 1    Obtiene el char por teclado del terminal....
		CONTROL = 2    Obtiene el char mediante un socket usando getchremoto..
	*/
int Ppid=getpid();
int m,i=0;
int direction=2;
int selSimbol=0;    /* Controla movimiento individual de cada personaje */
chtype c;
struct TipoMsgMonitor msg,pacMsg,monMsg;
cbreak();noecho();  /* Cancela el echo de las teclas */
timeout(90);  /* Establece el tiempo de espera del teclado hasta liberar */
keypad(stdscr,TRUE);  /* Habilita los caracteres extendidos. */

/* Inicializacion del id para identificar los mensajes. */
msg.id=id;

while (1)
    {
    /* Se imprime en pantalla utilizando el proceso monitor */
    msg.direction=direction;
    
    /* Da la sensacion de movimiento */
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

    /* Envio de mensaje al padre de la nueva direccion */
    
    write(pip[1],&msg,sizeof(msg));

    /* Segun CONTROL, captura datos de alguna fuente */
    
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
		/* En caso de que se aprete la q se le envia el dato como en
		el campo aviso de TipoMsgMonitor */
		
		msg.aviso='q';
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
		
		/* Le avisa al padre que el usuario remoto se ha ido */
		msg.aviso='c';
		write(pip[1],&msg,sizeof(msg));
		}

	    usleep(70000);
	    }
	break;
	}
	
	
    /* Calcula la direccion a tomar */
    
    if ( (CONTROL==1) || (CONTROL==2) )
        {
       /* Ejecuta si es pacman o fantasma remoto */
	direction=setdirection(c,direction);
	}
    else
	{
        /* Ejecuta si es fantasma */
	/* Genera una direccion */
	direction=randomdirection(direction);
	}

    }

printf ("Abandonaste !!! Sos un maricon, valla a saber como quien....??....\n");
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

	est[i].direction=0;
	if ( (i==0) )   // Para que por ahora sea el 0 el pacman.
	    {
	    est[i].p_Modo=1;
	    est[i].CONTROL=1;
	    }
	else
	    {
	    est[i].p_Modo=0;
	    est[i].CONTROL=0;
	    }

	est[i].sock=0;
	est[i].id=i;
	est[i].SIMBOLO=41;est[i].SIMBOLOB=41+30;
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
	    est[i].x=POSINIX_P+i;
	    est[i].puntos=0;
	    est[i].vidas=MAXVIDAS_P;
	    }
       //seteo de asignacion de colores.
       est[i].color=i+1;
	}
    
    /* Inicializacion de simbolos de cada personaje */
    est[0].SIMBOLO='C';est[0].SIMBOLOB='c';
    est[1].SIMBOLO='O';est[1].SIMBOLOB='o';
    est[2].SIMBOLO='H';est[2].SIMBOLOB='h';
    est[3].SIMBOLO='E';est[3].SIMBOLOB='e';
    est[4].SIMBOLO='J';est[4].SIMBOLOB='j';
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



main(int argc,char *argv[])
{
int cantjugadores;
int Pid;
int Ppid=getpid();
char difi;
char MiPanta[strlen(MiPantalla)];
int pip[2],sock;
char msgg[20];
struct TipoMsgMonitor msg;
struct TipoEstado est[C_JUGADOR];
int ciclo;

/* Control de cantidad de jugadores */

if ( (argc!=2)||(argv[1][0]<48)||((argv[1][0]-48)>C_JUGADOR) )
    {
    printf ("Copyright (C) Oel Amon Yag Software 1999.\n");
    printf ("\a\nNecesita poner la cantidad de jugadores remotos (valor Maximo:%d)\n",C_JUGADOR-1);
    exit(-1);
    }

cantjugadores=argv[1][0]-48;

/* Inicializacion de los datos de los personajes */
inicializar(est);


/* Apertura del socket de comunicaciones */
sock=abrirSocketS();

/* Esta implementacion funciona una vez que se conectan todos se larga el juego */
/* Espera a que se conecten todos los que se especifican.... */

conexiones(sock,est,cantjugadores);

printf ("\n\n\aListo para iniciar la partida\n");
printf ("Pulse ENTER por favor.....\n");
getc(stdin);


initscr();
start_color();
inicializarcolores();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);

/* Aperturas de canales para IPC */
/* Creacion del pipe de comunicaciones Fantasmas--Monitor */
pipe(pip);

/* Se setea para no visualizar el eco de los caracteres ni el cursor */
curs_set(0);
noecho();getch();

/* Musica inicial */
musicasole(0);

/* Creacion de hijos para oficiar de personajes.... */

for (ciclo=0;ciclo<C_JUGADOR;ciclo++)
    {
    fork();
    if (Ppid==getppid())
	{
	srand(time(NULL)*(ciclo+1));
	controltty(est[ciclo].SIMBOLO,est[ciclo].SIMBOLOB,est[ciclo].CONTROL,ciclo,est[ciclo].p_Modo,MiPanta,pip,est[ciclo].pip,est[ciclo].sock);
	close(est[ciclo].sock);
	endwin();
	exit(0);
	}
    }
    


/* Bucle de habilitacion de las demas terminales */
/* Si se desea descartar alguno de los remotos hay que mandarle "sinlugar" */
for (ciclo=0;ciclo<C_JUGADOR;ciclo++)
    {
    strcpy(msgg,"Habilitado");
    if (est[ciclo].sock>0)
	write(est[ciclo].sock,&msgg,sizeof(msgg));
    }
    


/* Aca llega solo el padre */
while (1)
	{
	generarbox(est);
	refresh();
	
	/* Lectura del pipe de los procesos hijos */
	if (read(pip[0],&msg,sizeof(msg))<0 )
	    {exit(0);}

	
	/* establece modo 1 persecucion, 2 huida */
	if (est[msg.id].p_Modo==1)
	    msg.modalidad=1;
	else
	    msg.modalidad=2;
	    
	
	
	/* Si es un mensaje de fantasma se le asigna una direccion para perse
	guir al pacman que este mas cercano a el */
	if (est[msg.id].CONTROL==0)
	    {
	    ciclo=getpacid(est,msg.id);
	    msg.direction=pacmandirection(est[msg.id].direction,est[ciclo].x,est[ciclo].y,est[msg.id].x,est[msg.id].y,MiPanta,est[ciclo].p_Modo);
	    }
	est[msg.id].direction=msg.direction;

	
	/* Calculo de la posicion x y en base a la direccion recibida */
	msg.y=est[msg.id].y;msg.x=est[msg.id].x;
	chequeapos (MiPanta,&msg.x,&msg.y,msg.direction);
	
	
	/* Envio de los datos a todos los sockets */
	for (ciclo=0;ciclo<C_JUGADOR;ciclo++)
	    {
	    if (est[ciclo].sock>0)
		write(est[ciclo].sock,&msg,sizeof(msg));
	    }
	
	
	/* Si el proceso PACMAN principal mando una q */
	if (msg.aviso=='q')
	    break;    
	if (msg.aviso=='c')
	    {
	    est[msg.id].CONTROL=0;
	    }
	
	
	/* Imprime el movimiento que corresponda */
	move(msg.y,msg.x);
	if (msg.id==0) attron(A_BOLD);
	if (est[msg.id].p_Modo==2) est[msg.id].color++;
	addch(msg.SIMBOLO+est[msg.id].color*256);
	if (est[msg.id].p_Modo==2) est[msg.id].color--;
	if (msg.id==0) attroff(A_BOLD);

	
	/* Se actualiza la pantalla si hubo algun cambio en las coordenadas */
	if ( (msg.y!=est[msg.id].y) || (msg.x!=est[msg.id].x) )
	    {
	    move(est[msg.id].y,est[msg.id].x);
	    if (MiPanta[(80*est[msg.id].y)+est[msg.id].x]=='±')
		{	
		attron(A_BLINK|A_BOLD);
		addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]+5*256);
	    	attroff(A_BLINK|A_BOLD);
		}
	    else
		addch(MiPanta[(80*est[msg.id].y)+est[msg.id].x]+20*256);
	    }

	/* Actualizacion del vector de estado para esta modificacion */
	est[msg.id].x=msg.x;
	est[msg.id].y=msg.y;

	if ( (accion(MiPanta,msg.x,msg.y,est,msg.id))<0 )
	    /* Salidas para cerrar las estructuras abiertas en caso de algun suceso */
	    break;
	}


/* Cierre de todos los sockets abiertos */
for (ciclo=0;ciclo<C_JUGADOR;ciclo++)
    {
    if (est[ciclo].sock>0) close(est[ciclo].sock);
    }

close(sock);

endwin();

/* Eliminacion de todos los procesos del grupo */
kill(0,SIGTERM);

exit(1);
}


