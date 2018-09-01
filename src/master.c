/*
Programa:master.c y slave.c
Utilidad:Permiten establecer la rutina de conferencia (chat).
Parametros que recibe:  ninguno.
*/



#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pwd.h>
#include<unistd.h>
#define MAXHOSTNAME 32
#define BACKLOG 2


struct TipoUsuario {
    char nombre[10];
    int socket;
    } usuario[10];
int userid=0;
    
void main()
{
int Ppid;
int p;
int s;
int t=0;   
int i;
int cnt;
char aux[BUFSIZ+1];
struct sockaddr_in sa,isa;     // socket de internet.
struct hostent *hp;
struct servent *sp;     
char *myname;
char localhost[MAXHOSTNAME+1];



if ((sp = getservbyname("conference","tcp")) == NULL)
    {
    printf ("No existe conference la rutina.\n");
    exit(1);
    }


gethostname (localhost, MAXHOSTNAME);
if ((hp=gethostbyname(localhost)) == NULL)
    {
    fprintf (stderr,"Se produjo un error al intentar obtener informacion del host.\n");
    exit(1);
    }

    
sa.sin_port = sp->s_port;
bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
sa.sin_family = hp->h_addrtype;

if ((s=socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)
    {
    printf ("Se produjo un error en la apertura del socket.\n");
    exit(1);
    }
    
if (bind(s,&sa,sizeof sa) < 0)
    {
    printf ("Imposible establecer estructura de socket.\n");
    exit(1);
    }
    
    
listen(s,BACKLOG);

i = sizeof isa;
printf ("Inicializando socket....\n");


while (1)
    {
    /* El servidor espera por cada una de las conexiones. */
    /* El proceso padre se queda siempre esperando nuevas conexiones */
    
	printf ("Esperando por cliente....\n");
	if ((t=accept(s,&isa,&i)) < 0)
	    {
	    printf ("Error en accept\n");
	    exit (1);
	    }
	printf ("Se ha abierto nuevo socket %d.\n",t);
    
    /* Leo el nombre del usuario */


	Ppid=getpid();
    /* Se ejecuta un fork generando un hijo por cada conexion */

    /* Se permite la entrada al while solo al hijo */     
	read(t,aux,BUFSIZ);
	printf ("Nuevo cliente en el sistema:%s\n",aux);
	strcpy(usuario[userid].nombre,aux);
	usuario[userid++].socket=t;
	
        while (userid==2)
	    atencion(t,s,isa,i);
	    
        
    }


/* Este ciclo deja al servidor activo esperando cada mensaje de los clientes. */    

close(t);   /* Cierro los sockets */

}
    
int ubicarSocketDestino (char *nom)
    {
    int id=0;
    if (strcmp(nom,"server")==0) 
	return 0;
    for (id=0;id<=userid;id++)
        if (strcmp(nom,usuario[id].nombre)==0) 
		return (usuario[id].socket);
    return -1;
    }
    



void decodif(char *no,char *destuser)
    {
    int i=0,ii=0;
    while (no[i]!=':')
        {
	destuser[i]=no[i];
	i++;
	}
    destuser[i]='\0';

    while (no[i]!='\0')
	{
	no[ii++]=no[i];
	i++;
	}
    no[ii]='\0';
    }

atencion(sock,s,isa,f)
int sock;
int s;
struct sockaddr_in isa;
int f;
    {
    struct sockaddr_in isa;
    struct passwd *p;
    char buf[BUFSIZ+1];
    char res[BUFSIZ+1];
    char whoami[4];
    int Ppid,Hpid,Hpid2;	
    int i;
    int status;
    char destuser[15];
    int ss;
    int cnt=0;
    for (cnt=0;cnt<userid;cnt++)    
	{
    	Ppid=getpid();
	Hpid=fork();
	if (getppid()==Ppid)
	    {/* Ejecuta el hijo la recepcion de mensajes */
	    sock=usuario[cnt].socket;
	    printf ("Recibiendo de socket %d:\n",sock);
	    while ((i=read(sock,buf,BUFSIZ))<0) ;
	    if (i <=0)
		{
		printf ("Se ha producido un error....\n");
		/*kill (Ppid);
		close(sock);
		exit(0);  /* Se produjo un error o se termino la conexion */
		}
	    
	    buf[i]='\0';
	    decodif(buf,destuser);
	    printf ("Destinatario:%s.\n",destuser);
	    if ((ss=ubicarSocketDestino(destuser))==0)
		printf("\nMensaje recibido:%s\n",buf);
	    else
		{
		if (ss<0)  /* No existe el destino */
		    {sprintf(buf,"El destinatario %s no existe.\0",destuser);
		     ss=sock;
		    }
		 
		/* Redireccionar el mensaje al socket correcto */
		printf ("Paso de mensajes...a socket %d\n",ss);
		write(ss,buf,BUFSIZ);
		}
	    exit(0);  /* Termina el hijo su ejecucion */
	    }
	/*if ((p=getpwnam(buf)) == NULL)
	strcpy(buf,"Usuario no encontrado\n");
	else
	sprintf(buf,"%s : %s\n", p->pw_name,p->pw_gecos);*/
/*	else  /* Ejecuta el padre */
	    /*{
	    printf ("Responder al cliente en el socket %d.",sock);
	    gets(res);
	    if (strcmp(res,"NEW")==0)
		break;
	
	    if (strcmp(res,"FIN")==0)
		{
		printf ("Se ha cancelado el programa servidor....\n");
		printf ("Cerrando socket %d....",sock);
		close(sock);
		exit(1);
		}
	    write(sock,res,strlen(res));
	    }*/
    
    }
    wait(&status);
    wait(&status);
}