/* Nombre: Archivo de cabecera protocolo.h
   Objetivo:  Provee las estructuras necesarias para utilizar un protocolo
   
*/


/* Constantes para el protocolo */

#include "comunicaciones.h"

/* Tamanio de los mensajes escritor entre los jugadores */
#define MAXMSG 80

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

struct Data100
    {
    /* Tipo 100 en la cabecera CAMBIO DE SECTOR */
    long personaje;
	/* 1   PAcman
	   2   Fantasma
	   */
    char modalidad;
	/*  1  MultiSector
	    2  Endosector ?
	    */
	    
    long iD;
	/* Identificacion arbitro origen */
    long puntaje;
    long vidas;
    int color;
    int direction;
    int sock;
	/* Valor de socket para ser un reenvio de pantalla a cliente. */
    };
    
struct Data110
    {
    long iD;
	/* arbitro donde esta el personaje del receptor */
    long posicion;
	/* posicion del personaje
	   segun FILA * 100 + COLUMNA 
	   Si es 1 es porque fue comido (???) */
    char mapa[25][50];
	/* La pantalla es m;as grande .... */
    };
    /* Se realiza un PEDIDO DE OBSERVACION DEL SECTOR (???) */
    
struct Data200
    {int b;/* Fin de pedido de observacion */
    };
    
struct Data210
    {
    int a;
    /* Envio de movimiento cuando un personaje esta en un sector foranio */
    };
    
struct Data300
    {
    char movimiento;
	/* Movimiento direccion segun 
	    1 arriba
	    2 abajo
	    3 derecha
	    4 izquierda
	    */
    int id;
	/* Id global del personaje */
    char SIMBOLO;
	/* Simbolo que se va a imprimir...Este simbolo va alternandose */
    };
    
struct Data310
    {
    char res;
	/*
	OK = 1
	ERROR = -1
	COMIDO = 2 */
	
    char mapa[25][50];
    };
    
struct Data400
    {
    long posicion;
	/* Fila * 100 + columna */
    char mapa[25][50];
    };
    
struct Data500
    {
    char msg[MAXMSG];
    };
    
struct Data600
    {
    char ping;
    };
    
struct Data610
    {
    char pong;
    };
    

struct TipoHeader
    {
    long tipo;
    long subTipo;
    long version;
    long msgID;
    long msgID_O;
    long iD;
    long long_data;
    };
    
struct TipoMsg
    {
    TipoHeader header;
    TipoData data;
    int cambio;
    };
    
struct ArbType
    {
    char host[MAXHOSTNAME];
    int sock;
    int iD;
    };
    
/*void netconnectC(int cantarb,struct ArbType arb[],char hst[])
    {
    int i,cont=0;
    int sock;
    int iD;
    struct sockaddr_in remsa
    
    /* Apertura del socket como cliente */
    sock=abrirSocketC(hst,&remsa);
    
    /* Se conecta al arbitro principal maestro */
    recibir(sock,&hst,&iD);
    
    /* Recibe el host al que se tiene que conectar */
    /* Recibe el iD que le da la identificacion de arbitro */
    arb[iD].iD=iD;
    arb[cont].sock=sock;
    
    /* Se conecta a los demas socket con cliente */
    for (i=0;i<(cantarb-1-iD);i++)
	{
	sock=abrirSocketC(hst,&remsa);
	arb[++cont].sock=sock;
	recibir(sock,&hst);
	}
	
    for (;i<cantarb;i++)
	{
	sock=abrirSocketS();
	arb[i].sock=sock;
	enviar(sock,hst
*/    


void netconnectC(int cantarb,struct ArbType arb[])
    {
    int sock;
    int i;
    struct sockaddr_in remsa;
    
    sock=abrirSocketC("darkstar",&remsa);
    
    connect(sock,
    
    
        
void netconnectS(int cantarb,struct ArbType arb[])
    {
    int sock;
    int i;
    char hst[MAXHOSTNAME];
    struct sockaddr_in remsa;
    /* Apertura de socket general */
    arb[0].sock=0;
    arb[0].iD=0;
    sock=abrirSocketS();
    
    printf ("Realizando conexiones con arbitros como ARBITRO MAESTRO.\n");
    
    /* Espera por conexion de todos los arbitros al sistema */
    for (i=1;i<cantarb;i++)
	{
	/* Conectar a arbitros */
	tams=sizeof(remsa);
	printf ("Esperando conexion de algun arbitro...\n");
	arb[i].sock=accept(sock,(void *)&remsa,&tmas);
	
	read(arb[i].sock,hst,strlen(hst));
	write(arb[i].sock,&cantarb,sizeof(cantarb));
	write(arb[i].sock,&i,sizeof(i));
	strcpy(arb[i].host,hst);
	}
    }
    


void main()
{
int cantarb=2;
struct ArbType arb[2];

netconnectC(cantarb,arb);
}

