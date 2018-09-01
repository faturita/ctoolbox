#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

main(argc,argv)
int argc;
char *argv[];           /*declaracion de argumentos*/
{int s;                 /*descriptor de socket*/
 int len;               /*longitud de los datos recibidos*/
 struct sockaddr_in sa; /*estructura del tipo socketaddr de Internet*/
 struct hostent *hp;    /*resultado de la busqueda del nombre del host*/
 struct servent *sp;	/*resultado de la busqueda de servicio*/
/* char buf[BUFSIZ+1];	/*buffer para leer la informacion de whois*/
 char *myname;		/*puntero al nombre de este programa*/
 char *host;		/*puntero al nombre del host remoto*/
 char *user;		/*puntero al nombre de usuario remoto*/
 char cadena[BUFSIZ+1];
 int i;
 
 myname=argv[0];	
 /*chequea que haya tres argumentos de linea de comando*/
 if(argc!=3) 
     {fprintf(stderr,"Usar:%shost nombre-de-usuario\n",myname);
      exit(1);
     }
 host=argv[1];
 user=argv[2];

 /*busca el nombre del host especificado*/
 if((hp=gethostbyname(host))==NULL)
     {fprintf(stderr,"%s:no existe tal host!\n",myname,host);
      exit(1);
     }
/*pone la direccion del host y el tipo de direccion 
  dentro de una estructura de socket*/
bcopy((char *)hp->h_addr,(char *)&sa.sin_addr,hp->h_length);
sa.sin_family=hp->h_addrtype;
/*busca el numero de socket para el servicio whois*/
if((sp=getservbyname("conference","tcp"))==NULL)
    {fprintf(stderr,"%s:El servicio conference no esta en este host",myname);
     exit(1);
     }
/*pone el numero de socket de whois dentro de la estructura de socket*/
sa.sin_port=sp->s_port;
/*ubica un socket abierto*/
if((s=socket(hp->h_addrtype,SOCK_STREAM,0))<0)
    {perror("socket");
     exit(1);
     }
/*se conecta con el servidor remoto*/
if(connect(s,&sa,sizeof sa)<0)
    {perror("connect");
    exit(1);
    }
/*envia la solicitud*/
while(1)
    {
    printf("Para salir #FIN#: ");
    gets(cadena);
    i=strlen(cadena)+1;
    cadena[i]='\0';
    if(write(s,user,strlen(user))!=strlen(user))
	{
	fprintf(stderr,"%s:Error de escritura\n",myname);
	exit(1);
	}
    if(write(s,cadena,strlen(cadena))!=strlen(cadena))
        {
	fprintf(stderr,"%s:Error de escritura\n",myname);
	exit(1);
	}
    if(strcmp(cadena,"#FIN#")==0)
	exit(1);
    }
/*lee la respuesta y pone la salida del usuario*/
/*while((len=read(s,buf,BUFSIZ))>0)
    write(1,buf,len);*/
close(s);
exit(0);
}     	            


