/* Nombre: Archivo de cabecera protocolo.h
   Objetivo:  Provee las estructuras necesarias para utilizar un protocolo
   
*/


/* Constantes para el protocolo */

/* Tamanio de los mensajes escritor entre los jugadores */
#define MAXMSG 80
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
    {int a;
    /* Fin de pedido de observacion */
    };
    
struct Data210
    {int b;
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
    struct TipoHeader header;
    struct Data100 data;
    int cambio;
    };
    

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
    int pacid=-1;
    int i,minX=56,minY=25;
    int minaX,minaY;
    
    for (i=0;i<C_JUGADOR;i++)
	{
	if (est[i].p_Modo>0)
	    {
	    if (pacid==-1) pacid=i;
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

void main()
{
struct TipoEstado est[5];
int i;
for (i=0;i<5;i++)
    {
    est[i].x=1;
    est[i].y=2;
    est[i].p_Modo=0;
    }
est[0].x=20;
est[0].y=10;
est[0].p_Modo=1;
est[1].x=2;
est[1].y=2;
est[1].p_Modo=1;
printf ("Id del seleccionado por el fantasma %d -- :%d\n",2,getpacid(est,2));
printf ("El modulo de %d - %d es :%d\n",est[2].x,est[1].x,mod(est[1].x-est[2].x));

}
    	    
