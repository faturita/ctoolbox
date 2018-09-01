/* Nombre: Archivo de cabecera protocolo.h
   Objetivo:  Provee las estructuras necesarias para utilizar un protocolo
   
*/


/* Constantes para el protocolo */

/* Tamanio de los mensajes escritor entre los jugadores */
#define MAXMSG 80

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
    };
    /* Debe devolver una respuesta de que va a recibirlo (???) */
    
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
    {;/* Fin de pedido de observacion */
    };
    
struct Data210
    {
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
    



typedef TipoData struct Data100;


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
    
    