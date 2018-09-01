/*
Programa:maestro.c y exclavo.c
Utilidad:Permiten establecer la rutina de conferencia (chat).
Parametros que recibe:  cantidad de usuarios.
Linea de comandos>   $DESTINO
			    Cambia el destinatario del mensaje.  Escriba server
			    para especificar que el mensaje es para todos.
		     FIN
		            Termina la conexion con el socket.
			    
*/


#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<pwd.h>
#include<unistd.h>
#include<fcntl.h>
#define MAXHOSTNAME 32
#define BACKLOG 2
#define MAXTAMUSER 10
#define MAXTOTALUSER 9






struct TipoUsuario {
    char nombre[MAXTAMUSER];
    int socket;
    } usuario[MAXTOTALUSER];

int userid=0;

int MAXUSER=MAXTOTALUSER;
    
void main(argc,argv)
int argc;
char *argv[];
{
int Ppid;
int s;
int t=0;   
int i;
int cnt;   
char res[BUFSIZ+1];
char rs[BUFSIZ+1];
char aux[MAXTAMUSER];
struct sockaddr_in sa,isa;     // socket de internet.
struct hostent *hp;
struct servent *sp;     
char localhost[MAXHOSTNAME+1];


if (argc!=2)
    {
    fprintf (stderr,"Use: 'maestro.out --help'     para ver ayuda.\n");
    exit(1);
    }
    
if (strcmp(argv[1],"--help")==0)
    {
    ayuda();
    exit(1);
    }

if ( (argv[1][0]<49) || (argv[1][0]>58) )
    {
    fprintf (stderr,"Parametro no valido.\n");
    exit(1);
    }
    
MAXUSER=argv[1][0]-48;


if ((sp = getservbyname("conference","tcp")) == NULL)
    {
    printf ("No existe la rutina conference.\n");
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


while (userid<MAXUSER)
    {
	/* El servidor espera por cada una de las conexiones. */
    
	printf ("Esperando por %d clientes mas....\n",(MAXUSER-userid));
	if ((t=accept(s,&isa,&i)) < 0)
	    {
	    printf ("Error en accept\n");
	    exit (1);
	    }
	printf ("Se ha abierto nuevo socket %d.\n",t);
    
	/* Leo el nombre del usuario */
	i=read(t,aux,BUFSIZ);
	aux[i]='\0';
	printf ("Nuevo cliente en el sistema:%s\n",aux);
	
	/* Se actualiza la tabla de usuarios */
	strcpy(usuario[userid].nombre,aux);
	usuario[userid].socket=t;
        
        userid++;
    }
userid--;

for (cnt=0;cnt<=userid;cnt++)
    {
    atencion(usuario[cnt].socket,s,isa,i);
    }


t=usuario[0].socket;
strcpy(aux,usuario[0].nombre);
while (strcmp(res,"FIN"))
	{
	printf ("Destinatario de mensajes: %s.\n",aux);
	gets(res);
	
	if (strcmp(res,"$DESTINO")==0)
	    {
	    printf ("Destino de los mensajes:");
	    gets(aux);
	    if ((t=ubicarSocketDestino(aux))<0) 
		{
		printf ("No existe. Se deja usuario por defecto (%s).\n",usuario[0].nombre);
		t=usuario[0].socket;
		strcpy(aux,usuario[0].nombre);
		}
	    }
	else    
	    if (strcmp(res,"FIN")==0)
		{
		printf ("Se ha cancelado el programa servidor....\n");
		}
	    else
		{
		strcpy(rs,res);
	    
		if (strcmp(aux,"server")==0)
		    {
		    /* Redireccionar a todos */
		    sprintf(res,"$server > %s",rs);
		    for (cnt=0;cnt<=userid;cnt++)
			write(usuario[cnt].socket,res,strlen(res));
		    }
		else
		    {
		    sprintf(res,"server > %s",rs);
		    write(t,res,strlen(res));
		    }
		}
        }


printf ("Cerrando sockets...\n");
for (cnt=0;cnt<=userid;cnt++)
    close(usuario[cnt].socket);
}


    
int ubicarSocketDestino (char nom[MAXTAMUSER])
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
    i++;
    while (no[i]!='\0')
	{
	no[ii++]=no[i];
	i++;
	}
    no[ii]='\0';
    }
    
int servicio(int socket,char *no,char *destuser)
    {
    int cnt;
    char buf[BUFSIZ+1];
    /* Requiere que no(mensaje en si) y destuser esten previamente calculados. */
    if (strcmp(no,"$$LISTA")==0)
	{
	strcpy(buf,"\nLista de usuarios habilitados.\n");
	/* Devolver al usuario la lista de usuarios */
	for (cnt=0;cnt<=userid;cnt++)
	    {
	    strcat(buf,usuario[cnt].nombre);
	    strcat(buf,"\n");
	    }
	write(socket,buf,strlen(buf));
	return 1;
	}
	
    /* Aca se pueden agregar otros servicios que se deseen brindar */
    return 0;
    }
    
    
	    


ayuda()
    {
    printf ("Programa de chateo version 1.1.1.1 .\n");
    printf ("Universidad Nacional de La Matanza.\n");
    printf ("Oel Amon Yag 1999.\n\n");
    printf ("Uso:\n\tmaestro.out [cant_usuarios]\n");
    printf ("\n\nParametros:\n\t\t[cant_usuarios]  Cantidad de usuarios a conectarse en el chat.\n");
    printf ("\n\nComandos en el programa:\n");
    printf ("\t$DESTINO\tCambia el destinatario de los mensajes.Especifique \n");
    printf ("\t        \tserver para que todos vean el mensaje.\n");
    printf ("\tFIN     \tTermina la conexion.\n\n");
    }
    



atencion(sock)
int sock;
    {
    char buf[BUFSIZ+1];
    char aux[BUFSIZ+1];
    int Ppid,Hpid;	
    int i;
    char destuser[MAXTAMUSER];
    int ss;
    Ppid=getpid();
    Hpid=fork();
    if (getppid()==Ppid)
	{/* Ejecuta solo el hijo la parte de recepcion y el padre sale de la f.*/
	while (1)
	{
	i=read(sock,buf,BUFSIZ) ;
	
	if (i <=0)
	    {
	    printf ("Se ha producido un error....\n");
	    kill (Ppid);
	    close(sock);
	    exit(0);  /* Se produjo un error o se termino la conexion */
	    }
	    
	buf[i]='\0';
	decodif(buf,destuser);
	
       if (servicio(sock,buf,destuser)==0)
	    
	/* Analisis de donde va a ir a para el mensaje */
	if ((ss=ubicarSocketDestino(destuser))==0)
	    {
	    /* Direccionar el mensaje a todos */
	    /* Esto se produce cuando el destinatario es server */
	    printf("Mensaje recibido:%s\n",buf);
	    /* Se le agrega un mensaje para identificar que es para todos */
	    sprintf(aux,"Para todos:%s",buf);
	    strcpy(buf,aux);
	    for (i=0;i<=userid;i++)
		write(usuario[i].socket,buf,strlen(buf));
	    }
	else
	    {
	    if (ss<0)  /* No existe el destino */
		{sprintf(buf,"El destinatario %s no existe.\0",destuser);
		 ss=sock;
		}
		 
	    /* Redireccionar el mensaje al socket correcto */
	    write(ss,buf,BUFSIZ);
	    }
	}
    }
}