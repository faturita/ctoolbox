#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

void main(argc,argv)
int argc;
char *argv[];
{
int i;
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
    printf ("\n%s no es un host existente....",host);
    exit(1);
    }
/* Copia las direcciones y las familias en la estructura socket. */

bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
sa.sin_family = hp->h_addrtype;

if ((sp=getservbyname("whois","tcp"))==NULL)
    {
    printf ("\nEl servidor no existe...");
    exit(1);
    }

sa.sin_port = sp->s_port;

/* Localiar un socket. */

if ((s = socket(hp->h_addrtype, SOCK_STREAM,0)) < 0) 
    {
    printf ("\nError al intentar abrir el socket.");
    exit(1);
    }

/* Conectarse a ese socket. */
/*
if (connect(s,&sa,sizeof sa ) < 0)
    {
    printf ("\nError al intentar conectarse al socket.");
    exit(1);
    }
*/    



gets(msg);

if (connect(s,&sa,sizeof sa ) < 0)
    {
    printf ("\nError al intentar conectarse al socket.");
    exit(1);
    }


while ((strcmp(msg,"FIN")))
    {/* El llamado al socket. Requerimiento */
    printf ("El socket en uso es:%d\n",s);
    sprintf (adc,"%s > %s\0",user,msg);
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
    
    printf ("Esperando por la contestacion....\n");
    while ((i=read(s,buf,BUFSIZ)) <= 0);
    buf[i]='\0';
    printf ("Respondio:	%s\n",buf);
    close(s);
    gets(msg);
    
    if ((s = socket(hp->h_addrtype, SOCK_STREAM,0)) < 0) 
	{
	printf ("\nError al intentar abrir el socket.");
	exit(1);
	}
    if (connect(s,&sa,sizeof sa ) < 0)
	{
	printf ("\nError al intentar conectarse al socket.");
	exit(1);
	}

    
    }
close(s);
}
    

    
    
    

    
    


