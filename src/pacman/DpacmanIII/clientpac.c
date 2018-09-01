/* Este es un ejemplo de un socket CLIENTE */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <ncurses.h>

#define DP_PORT 5504

void main()
{
struct hostent *hp;
struct servent *sp;
struct sockaddr_in hostsa;
struct sockaddr_in remtsa;
int sock,socki;
int tams;
char hst[32];
chtype c=32;
char msg[20];
/* El nombre del host como del puerto se deben especificar al intentar la conexion */

/* O al menos el nombre del host si se va a usar un servicio especifico del host */

/* La direccion del socket remoto la averiguamos del fichero /etc/host    */



gethostname(hst,32);


hp=gethostbyname(hst);
printf ("Informacion del host especificado.  Host remoto ......\n");
printf ("Nombre host completo:%s\n",hp->h_name);
printf ("Alias :\n");
printf ("Familia :%d\n",hp->h_addrtype);
printf ("Tamanio :%d\n",hp->h_length);
printf ("Direccion :%ld\n",hp->h_addr_list[0]);


hostsa.sin_port=DP_PORT;   // Definido
hostsa.sin_family=AF_INET;
bcopy(hp->h_addr,(char *)&hostsa.sin_addr,hp->h_length);

/* Cliente abre el socket */
if ( (sock=socket(hostsa.sin_family,SOCK_STREAM,0))<0 )
    {
    printf ("Fallo en el socket.....\n");
    exit(0);
    }
    
if ( (connect(sock,(void *)&hostsa,sizeof(hostsa)))<0 )
    {
    printf ("Fallo en el connect ....\n");
    exit(1);
    }
    
printf ("Nombre de el socket:%d",sock);
strcpy(msg,"fatu");
write(sock,msg,strlen(msg));


printf ("Conexion exitosa !!!!!!\n\n");

printf ("Datos del HOST REMOTO....\n");    
printf ("Puerto asignado:%d\n",hostsa.sin_port);    
printf ("Address:%ld\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d\n",hostsa.sin_family);

printf ("Presione una tecla por favor......");
getc(stdin);

initscr();
cbreak();
noecho();
keypad(stdscr,TRUE);
refresh();

while (c!='q')
    {
    c=getch();refresh();
    write(sock,&c,sizeof(c));
    }
    
    

printf ("Cerrando el socket .....\n");

endwin();
close(sock);

getc(stdin);
}
