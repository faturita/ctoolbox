/*
Programa:  slave.c
Este programa funciona con el programa master.c en otra terminal.

Parametros:Debe recibir el nombre del host y el nombre de usuario para el chat.
*/

	    
	

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<sys/ptrace.h>

void main(argc,argv)
int argc;
char *argv[];
{
int Ppid,Hpid;
int i=0;
int s;
int len;
struct sockaddr_in sa;     // socket de internet.
struct hostent *hp;
struct servent *sp;     
char *host;		// puntero al host
char *user;		// puntero al usuario.
char buf[BUFSIZ+1];
char *myname;
char msg[100];
char adc[100]; 
char destuser[15]="server";

myname = argv[0];

if (argc != 3)
    {
    fprintf(stderr,"Son necesarios dos parametros.\n");
    exit(1);
    }
host=argv[1];
user=argv[2];


if ((hp = gethostbyname(host))==NULL)
    {
    printf ("Host:%s no existe....\n",host);
    exit(1);
    }
/* Copia las direcciones y las familias en la estructura socket. */

bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
sa.sin_family = hp->h_addrtype;

if ((sp=getservbyname("conference","tcp"))==NULL)
    {
    printf ("El servidor no existe...\n");
    exit(1);
    }

sa.sin_port = sp->s_port;

/* Localizar un socket. */

if ((s = socket(hp->h_addrtype, SOCK_STREAM,0)) < 0) 
    {
    printf ("Error al intentar abrir el socket.\n");
    exit(1);
    }

/* Conectarse a ese socket. */

if (connect(s,&sa,sizeof sa ) < 0)
    {
    printf ("Error al intentar conectarse al socket.\n");
    exit(1);
    }
/* Envio de nombre de usuario conectado al servidor. */
    
printf ("Conexion exitosa.\nConectado a %s bajo el nombre de %s.\n",host,user);
printf ("El socket en uso es:%d\n",s);
printf ("El destinatario por defecto es \"server\". Se cambia escribiendo $DESTINO.\n");


Ppid=getpid();

/* Se crea un hijo para que atienda la recuperacion de los mensajes */
Hpid=fork();

if (Ppid!=getpid())
    {
    /*Ejecuta solo el hijo la parte de recepcion de mensajes */
    while (1)
	{
	i=read(s,buf,BUFSIZ);
	if (i<=0) 
	    {
	    printf ("Se produjo un error o el servidor cancelo la operacion.\n");
	    exit(1);
	    }
	buf[i]='\0';
	
	printf ("Mensaje recibido:%s\n",buf);
	}
    /* Sale solamente por el exit */
    }
    
write(s,user,strlen(user));
gets(msg);
while ((strcmp(msg,"FIN")))
    {/* Se atiende la emision de mensajes al servidor */
    if (strcmp(msg,"$DESTINO")==0)
	{/* Controla que se cambie el usuario destino y que este exista. */
	printf ("Escriba el nombre del destinatario:\n");
	gets(destuser);
	printf ("Todos los mensajes nuevos tipeados se dirijiran a:%s\n",destuser);
	}
    else
	{
	// Formato: destinatario:emisor > mensaje.
	
	sprintf (adc,"%s:%s > %s\0",destuser,user,msg);
	strcpy (msg,adc);
        if (write(s,msg,strlen(msg)) != strlen(msg))
    	    {
	    printf ("\nSe produjo un error al intentar escribir en el socket.");
	    exit(1);
	    }

    /*printf ("%s\n",buf);*/
    /*while ( (len=read(s,buf,BUFSIZ)) > 0 )*/
	/*write (1, buf,len)*/;
    /*printf ("%s",buf);*/
	}
    gets(msg);
    }
printf ("Finalizada la comunicacion...\n");
printf ("%d",Hpid);
close(s);
}
    

    
    
    

    
    


