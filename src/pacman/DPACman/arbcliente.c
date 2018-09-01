/* Nombre: Archivo de cabecera protocolo.h
   Objetivo:  Provee las estructuras necesarias para utilizar un protocolo
   
*/


/* Constantes para el protocolo */
#include <sys/types.h>
#include <signal.h>
#include "comunica.h"

/* Tamanio de los mensajes escritor entre los jugadores */
#define MAXMSG 80

struct ArbType
    {
    char host[MAXHOSTNAME];
    int sock;
    int iD;
    };
    

void netconnectC(int *id,int *cantarbb,struct ArbType arb[],char arbhst[MAXHOSTNAME])
    {
    int sock,socks=0,sockc,tams;
    int i,j,k;
    struct sockaddr_in hostsa;
    char hst[MAXHOSTNAME];
    int cantarb;
    
    cantarb=(*cantarbb);
    
    sock=abrirSocketC(&hostsa,arbhst,0);
    
    printf ("Conectandose con el ARBITRO MAESTRO....\n");
    
    /* Conexion al ARBITRO MAESTRO */
    connect(sock,(void *)&hostsa,sizeof(hostsa));
    gethostname(hst,MAXHOSTNAME);
    write(sock,hst,strlen(hst));
    read(sock,&cantarb,sizeof(int));
    read(sock,&i,sizeof(i));
    printf ("Datos obtenidos: %d indice %d\n",cantarb,i);
    arb[0].sock=sock;
    arb[i].iD=i;
    arb[i].sock=0;
    
    /* Se bloquea hasta recibir la HOST TABLE */
    printf ("Host Table\n");
    for (k=0;k<cantarb;k++)
        {
	tams=read(sock,&hst,MAXHOSTNAME);
	hst[tams]='\0';
	strcpy(arb[k].host,hst);
	printf ("%s\n",arb[k].host);
	arb[k].iD=k;
	}

    printf ("HOST TABLE recibida satisfactoriamente......\n");    

    
    /* Conexiones como CLIENTE */
    for (j=1;j<i;j++)
	{
	/* Obtengo los datos del host remoto */
	printf ("Conectandose al puerto %d\n",DP_PORT+j);
	arb[j].sock=abrirSocketC(&hostsa,arb[j].host,j);
	
	if (arb[j].sock<0)
	    {
	    printf ("Se ha producido un error grave en la conexion\n\a");
	    kill(0,SIGTERM);
	    }
	
	printf ("Conectando a servidor.......\n");
	/* Conexion al host remoto */
	usleep(j*1000);
	while (  connect(arb[j].sock,(void *)&hostsa,sizeof(hostsa))<0 ) ;
	
	/* Conexion establecida con un nuevo arbitro */
	write(arb[j].sock,&i,sizeof(i));
	}

    /* Espera habilitacion de SERVIDOR */
    if (read(sock,&j,sizeof(j))<0) 
	{
	printf ("Se produjo un error en el read\n\a");
	kill(0,SIGTERM);
	}
	
	    
    /* Conexiones como SERVIDOR */
    if (i!=(cantarb-1))
        socks=abrirSocketS(i);
	
    
    
    for (k=i+1;k<cantarb;k++)
        {
	printf ("Accept esperando conexiones de clientes....\n");
        sockc=accept(socks,(void *)&hostsa,&tams);
	
	/* Se bloquea hasta recibir el id del arbitro cliente */
	read(sockc,&j,sizeof(int));
	arb[j].sock=sockc;
	}
    
    //exit(-1);    
    /* Envio de done al ARBITRO MAESTRO */
    write(sock,&j,sizeof(j));
    if (socks>0) 
	close(socks);
    (*cantarbb)=cantarb;
    (*id)=i;
    }
    
    
    
    
    
    
        

void main(int argc,char *argv[])
{
int cantarb=2;
int i;
int id;
char msg[MAXMSG];
struct ArbType arb[2];

netconnectC(&id,&cantarb,arb,argv[1]);

if (id==1)
    {
    strcpy(msg,"Hola!!!");
    write(arb[2].sock,msg,MAXMSG);
    }
else
    {
    i=read(arb[1].sock,msg,MAXMSG);
    msg[i]='\0';
    printf ("%s",msg);
    }
    

for (i=0;i<cantarb;i++)
    if (arb[i].sock>0)
	close(arb[i].sock);
    
//kill(0,SIGTERM);

}

