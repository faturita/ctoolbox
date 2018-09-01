/* Nombre: Archivo de cabecera protocolo.h
   Objetivo:  Provee las estructuras necesarias para utilizar un protocolo
   
*/


/* Constantes para el protocolo */

#include "comunica.h"

/* Tamanio de los mensajes escritor entre los jugadores */
#define MAXMSG 80

#define MAXARB 5

struct ArbType
    {
    char host[MAXHOSTNAME];
    int sock;
    int iD;
    int port;
    };
    

    
        
void netconnectS(int cantarb,struct ArbType arb[])
    {
    int sock;
    int i,j;
    int tams,done;
    char hst[MAXHOSTNAME];
    struct sockaddr_in remsa;
    /* Apertura de socket general */
    arb[0].sock=0;
    arb[0].iD=0;
    gethostname(arb[0].host,MAXHOSTNAME);

    sock=abrirSocketS(0);
    
    printf ("Realizando conexiones con arbitros como ARBITRO MAESTRO.\n");
    
    /* Espera por conexion de todos los arbitros al sistema */
    for (i=1;i<cantarb;i++)
	{
	/* Conectar a arbitros */
	tams=sizeof(remsa);
	printf ("Esperando conexion de algun arbitro...\n");
	arb[i].sock=accept(sock,(void *)&remsa,&tams);
	
	tams=read(arb[i].sock,hst,MAXHOSTNAME);
	hst[tams]='\0';
	write(arb[i].sock,&cantarb,sizeof(cantarb));
	write(arb[i].sock,&i,sizeof(i));
	strcpy(arb[i].host,hst);
	printf ("Host destino:%s\n",arb[i].host);
	}
    
    /* Envio de la HOST TABLE */	
    for (i=1;i<cantarb;i++)
	{
	for (j=0;j<cantarb;j++)
	    {
	    write(arb[i].sock,arb[i].host,MAXHOSTNAME);
	    }
	}
    
    printf ("Armando la red malla......\n");
    
    for (i=1;i<cantarb;i++)
	{
	/* Envio de habilitacion como SERVIDORES */
	done=0;
	write(arb[i].sock,&done,sizeof(int));
	
	/* Recepcion de finalizacion de conexiones por arbitro */
	read(arb[i].sock,&done,sizeof(int));
	}
	
		
    printf ("Conexiones realizadas correctamente.....\n");
    
    printf ("Lista de host y socket asignados\n");    
    
    for (i=0;i<cantarb;i++)
	{
	printf (" ID         HOST         PUERTO\n");
	printf ("%4d %40s\n",i,arb[i].host);
	}
    close(sock);	
    }
    


void main(int argc,char *argv[])
{
int cantarb;
int i;
char msg[MAXMSG];
struct ArbType arb[MAXARB];

cantarb=argv[1][0]-48;

if ( (cantarb>MAXARB) || (cantarb<0) )
    {
    printf ("Maxima cantidad de arbitros:%d\n\a",MAXARB);
    exit(-1);
    }
    

netconnectS(cantarb,arb);


/* Cierre de los sockets */
for (i=0;i<cantarb;i++)
    if (arb[i].sock>0)
	close(arb[i].sock);
    

while (1) ;
}

