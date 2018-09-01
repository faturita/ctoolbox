/* PROGRAMA:  clientpac.c
OBJETIVO:  Es un cliente del juego Dpacman.c que brinda entrada de teclado 
remota (Getchremoto (C) Oel Amon Yag) y muestra los datos recibidos desde el 
servidor 
AUTOR: Oel Amon Yag
VERSION: 1
FECHA: 25/09/99
NOTAS:  Este proceso envia datos sobre las teclas que se presionaron al 
proceso maestro, que actua como server, y recibe datos acerca de las posiciones
de pantalla donde se deben imprimir los personajes.  Varios funciones aqui
implementadas son similares a las de Dpacman, con la diferencia que en este
programa no existe la comunicacion entre procesos, porque los procesos que
manejan a cada personaje se encuentran alojados en el servidor. 
Muchas funciones carecen de mecanismos IPC. */




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


/* Cantidad de ciclos (veces que imprime en pantalla el pacman) hasta que se 
suelta del modo COME FANTASMAS */
#define C_CICLOSMODO 100

/* Constantes para las posiciones iniciales de las figuras en la pantalla */
#define POSINIX_P 26
#define POSINIY_P 19
#define POSINIX_F 26
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
    int modalidad;
    int id;
    int direction;
    int aviso;
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
    static int Ciclos=0;
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
		if ( (est[i].x==est[j].x) && (est[i].y==est[j].y) && (i!=j) && (est[i].p_Modo==0) )
		    {
		    if (est[j].p_Modo==1)
			{
			move(13,56);
			printw("Muere maldito PACMAN!!!\n");

			/* Chequeo de las vidas */
			if (est[j].vidas==0)
			    {
			    move(14,1);
			    printw ("El pacman debe estar viejo como LEO porque lo hicieron de goma\n");
                            musicafunebre(0);
			    refresh();
			    return -1;
			    }
			else
			    {
			    move(13,56);
			    printw("Otra oportunidad !!!!!!");

			    /* Se actualiza la posicion en el vector de estado */
			    est[j].x=POSINIX_P;
			    est[j].y=POSINIY_P;
			    est[j].vidas--;
			    }
			}
		    else if (est[j].p_Modo==2)
			{
			/* Comer al fantasmita */
			
			/* Vacia el buffer del fantasma */
			//while ( (read(est[i].pip[0],&msg,sizeof(msg)))>=0 );
                        
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
	printw ("Uyyy!!!  Sos medio lelo como Martin....Como te gano el PACMAN !\n");
	musicagolpe(0);
	refresh();
	return -1;
	}

    if (est[id].p_Modo>0)
	{
	if (est[id].p_Modo==2)
	    {
	    Ciclos++;
            musicacomiendo1(1);
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
    
    move(1,60);printw("Pers Puntos Vidas");
    for (i=0;i<C_JUGADOR;i++)
	{
	move(2+i,60);printw ("%4c %5d %5d",est[i].SIMBOLO,est[i].puntos,est[i].vidas);
	}
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
    est[0].SIMBOLO='C';
    est[1].SIMBOLO='O';
    est[2].SIMBOLO='H';
    est[3].SIMBOLO='E';
    est[4].SIMBOLO='J';
    for (i=0;i<C_JUGADOR;i++)
	{

	/* Creacion de los pipes Monitor--Fantasmas (no bloqueante) */
	

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



    }


void getchremoto(int sock)
    {
    /* OBJETIVO:  Espera la entrada por teclado desde la terminal actual y la
    envia por un socket para ser leida por un proceso servidor. */
    /* NOTA:  El getch es bloqueante. */
    chtype c=32,cc=32;    
    while (c!='q')
	{
	while ((c=getch())==cc);
	    refresh();
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
char MiPanta[strlen(MiPantalla)];
struct TipoMsgMonitor msg;
struct TipoEstado est[C_JUGADOR];

if (argc!=3)
    {
    printf ("Copyright (C) Oel Amon Yag Sorware de Calidad.\n");
    printf ("Modo de uso: clientpac.out host nickname \n\n");
    exit(-1);
    }
    


sock=abrirSocketC(&hostsa,argv[1]);

/* Conectarse con el host */
/* El nombre del host es obtenido del archivo /etc/hosts */

if ( (connect(sock,(void *)&hostsa,sizeof(hostsa)))<0 )
    {
    printf ("Fallo en el connect ....\n");
    exit(1);
    }
    
printf ("Numero de socket asignado:%d\n",sock);
strcpy(msgg,argv[2]);

/* Envio del nick de usuario */
printf ("Nickname de usuario: %s\n",msgg);
write(sock,msgg,strlen(msgg));

/* Display de informacion de estado */
printf ("Conexion exitosa !!!!!!\n\n");
printf ("Datos del HOST REMOTO....\n");    
printf ("Puerto asignado:%d\n",hostsa.sin_port);    
printf ("Address:%ld\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d\n",hostsa.sin_family);


/* Espera de respuesta del servidor */
printf ("Esperando habilitacion del servidor.....\n");
read(sock,&msgg,20);
if (!strcmp(msgg,"sinlugar"))
    {
    printf ("El juego ya ha sido iniciado, y no hay posiciones libres para entrar en el.\n");
    printf ("Intente mas tarde, quizas se haya iniciado una nueva partida....\n\a");
    close(sock);
    exit(-1);
    }

/*  Esta habilitacion, es para que todos los jugadores comiencen al mismo
tiempo.  Quizas para una depuracion mejor del codigo, aqui se deberia enviar
para cada cliente, el id que se le ha asignado, para poder llevar una tabla
con puntos, o si se juega con vidas para cada fantasma, las vidas de cada fa
ntasma y si muere, se termina el proceso, y para este se hace cargo la comput
adora...... */


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
    

/* Aca se debe agreagar el codigo de impresion y cambiar el pipe por el sock.*/

/* Inicializacion de los datos de los personajes */
inicializar(est);


start_color();
inicializarcolores();
strcpy(MiPanta,MiPantalla);
muestrapantalla(MiPanta);

/* Inicializacion de las estructuras con los datos de fantasmas y de pacman */

curs_set(0);
//musicasole(0);


msg.x=1;msg.y=2;msg.SIMBOLO='a';msg.id=1;msg.modalidad=0;
	
while (1)
	{
	generarbox(est);
	refresh();
	
	/* Lectura del socket del server */
	read(sock,&msg,sizeof(msg));
	
	/* Si el proceso PACMAN mando una q */
	if (msg.aviso=='q')
	    break;

	/* Imprime el movimiento que corresponda */
	move(msg.y,msg.x);
	if (msg.id==0) attron(A_BOLD);
	addch(msg.SIMBOLO+est[msg.id].color*256);
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




printw ("\nCerrando el socket .....\n");
printw ("El juego ha finalizado...\n");

endwin();

/* Cierre del socket de comunicaciones con el servidor */
close(sock);

/* Eliminacion de los procesos hijos */
kill(0,SIGTERM);

}

