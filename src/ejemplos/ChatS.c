#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pwd.h>
#define BACKLOG 5		/*numero maximo de solicitudes para la cola*/
#define MAXHOSTNAME 32		/*maxima longitud para el nombre del host*/

main(argc,argv)

int argc;			/*declaracion de argumentos estandar*/
char *argv[];
{ 
int s,t;			/*descriptores de sockets*/
int i;				/*entero de proposito general*/
struct sockaddr_in sa,isa;	/*estructura de direccion de socket de internet*/
struct hostent *hp;		/*resultado de busqueda de nombre de host*/
char *myname;			/*puntero al nombre de este programa*/
struct servent *sp;		/*resultado de busqueda de servicio*/
char localhost[MAXHOSTNAME+1];	/*nombre de host local como cadena de caracteres*/
int pid;
int tuberia_Pa_Hi[2];

if (pipe(tuberia_Pa_Hi)==-1)
    {
    perror ("PIPE");
    exit(-1);
    }
 
/*printf("Padre:%d ",getpid());*/


    myname=argv[0];
    /*busca la entrada de servicio whois*/
    if((sp=getservbyname("conference","tcp"))==NULL)
	{
	fprintf(stderr,"%s:el servicio conference no esta en este host\n",myname);
	exit(1);
	}
    /*obtiene informacion de nuestro propio host*/
    gethostname(localhost,MAXHOSTNAME);
    if((hp=gethostbyname(localhost))==NULL)
	{
	fprintf(stderr,"%s:no se puede obtener informacion del host local!\n",myname);
	exit(1);
	}
    /*pone el numero de socket de whois e informacion de nuestra direccion
    en la estructura de socket*/
    sa.sin_port=sp->s_port;
    bcopy((char *)hp->h_addr,(char *)&sa.sin_addr,hp->h_length);
    sa.sin_family=hp->h_addrtype;
    /*asigna un socket abierto para las conecciones entrantes*/
    if((s=socket(hp->h_addrtype,SOCK_STREAM,0))<0)
	{
	perror("Socket");
	exit(1);
	}
    /*liga el socket a un puesto de servicio*/
    /*tambien tenemos noticias de las conexiones estrantes*/
    if(bind(s,&sa,sizeof sa)<0)
	{
	perror("bind");
	exit(1);
	}
	/*configuramos numero maximo de conexiones que se crearan despues*/
    listen(s,BACKLOG);

    /*entra en un ciclo infinito esperando por nuevas conexiones*/
    while(1)
	{
	i=sizeof isa;
	/*colgamos en accept() mientras esperamos por nuevos clientes*/
/*	printf("ESPERANDO CONEXION...\n");*/
	if((t=accept(s,&isa,&i))<0)
	    {
	    perror("accept");
	    exit(1);
	    }
/*	printf("Valor de t: %d",t);*/
	if((pid=fork())==-1)
	    {    
	    perror("Error en la llamada a fork!");
	    exit(-1);
	    }
	else if (pid!=0)
	    {
	    /*printf("Esto indica el fork:%d",getpid());*/
	    write(tuberia_Pa_Hi[1],t,sizeof(t));
	    close(tuberia_Pa_Hi[1]);
	    }
	else
	    {
	    read(tuberia_Pa_Hi[0],t,sizeof(t));
	    close(tuberia_Pa_Hi[0]);
	    chat(t);
	    close(t);
	    exit();
	    }
	}
}

 /*obtiene la solicitud whois del host remoto y da formato a una respuesta*/
chat(sock)
int sock;
{
char buf[BUFSIZ+1];
int i;
/*obtiene una solicitud de linea*/
while(1)
    {
    if((i=read(sock,buf,BUFSIZ))<=0)
	return;
    buf[i]='\0';   	/*terminador nulo*/
    /*devuelve nombre del emisor*/
    write(1,buf,strlen(buf));
    printf(": ");
    if((i=read(sock,buf,BUFSIZ))<=0)
	return;
    buf[i]='\0';   	/*terminador nulo*/
    /*devuelve el mensaje del emisor*/
    write(1,buf,strlen(buf));
    printf("\n");    
    if (strcmp(buf,"#FIN#")==0)
	exit(1);
    }
return;
}
