#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pwd.h>
#define MAXHOSTNAME 32
#define BACKLOG 2


    
void main()
{
int s;
int t=0;   // Guarda los numeros de sockets que devuelve accept
int i;
int cnt;
struct sockaddr_in sa,isa;     // socket de internet.
struct hostent *hp;
struct servent *sp;     
char *myname;
char localhost[MAXHOSTNAME+1];


if ((sp = getservbyname("whois","tcp")) == NULL)
    {
    printf ("\nNo existe whois.");
    exit(1);
    }


gethostname (localhost, 32);
if ((hp=gethostbyname(localhost)) == NULL)
    {
    fprintf (stderr,"\nSe produjo un error al intentar obtener informacion del host.");
    exit(1);
    }

    
sa.sin_port = sp->s_port;
bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
sa.sin_family = hp->h_addrtype;

if ((s=socket(hp->h_addrtype,SOCK_STREAM,0)) < 0)
    {
    printf ("\nError en socket");
    exit(1);
    }
    
if (bind(s,&sa,sizeof sa) < 0)
    {
    printf ("\nerror en bind");
    exit(1);
    }
    
    
listen(s,BACKLOG);

i = sizeof isa;
printf ("El servidor esta esperando puesto que el socket esta vacio.\n");
printf ("El servidor esta seteado para esperar hasta que se conecten %d clientes.\n",BACKLOG);
/*for (cnt = 0;cnt<BACKLOG;cnt++)
    {
    /* El servidor espera por cada una de las BACKLOG conexiones. 
    if ((t[cnt]=accept(s,&isa,&i)) < 0)
	{
	printf ("Error en accept");
	exit (1);
	}
    }*/


/* Este ciclo deja al servidor activo esperando cada mensaje de los clientes. */    
while (1) 
    {
    whois(t,s,isa,i);
    }

close(t);   /* Cierro los sockets */

}
    


whois(sock,s,isa,f)
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
    	
    int i;
    static int cnt=0;
    
    printf ("Esperando mensaje de cliente...\n");
    if ((sock=accept(s,&isa,&f)) < 0)
	{
	printf ("Error en accept");
	exit (1);
	}
    printf ("El socket en uso es:%d",sock);
    i=read(sock,buf,BUFSIZ) ;
	    /*{
	    if (cnt>=BACKLOG) cnt=0;
	    }*/
	      
	        
    if (i <=0)
	exit(1);  /* Se produjo un error o se termino la conexion */
	
    buf[i]='\0';
    
    
    /*if ((p=getpwnam(buf)) == NULL)
	strcpy(buf,"Usuario no encontrado\n");
    else
	sprintf(buf,"%s : %s\n", p->pw_name,p->pw_gecos);*/
	
		
    printf("%s\n",buf);	
    printf ("Responder al cliente:");gets(res);
    
    /*strcpy(buf,"RECIBIDO");*/
    write(sock,res,strlen(res));
    close(sock);
    return;
    }
    
