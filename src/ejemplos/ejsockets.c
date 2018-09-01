/* Este es un ejemplo de un socket SERVIDOR */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

void main()
{
/* Estructura donde se almacenan los datos del host y es una estructura
que devuelve gethostname() leyendo el archivo /etc/hosts  */
struct hostent *hp;
/* Lo mismo que servent, leyendo el archivo /etc/services.  Esto es si se quiere
implementar algun servicio, sino no es necesario */
struct servent *sp;

/* Estructura que corresponde al protocolo de TCP/IP.
    Hay dos, porque en una se van a albergar los datos correspondientes al host,
    en tanto que en la otra, se guardan los datos que corresponden al cliente */
struct sockaddr_in hostsa;
struct sockaddr_in remtsa;

/* Socket generico y socket especifico devuelto por accept o connect */
int sock,socki;

int tams;
char hst[16];


/* Lee el nombre del host donde estamos y lo guarda en hst */
gethostname(hst,32);
printf ("Nombre del host:%s\n",hst);


/* Lee el archivo /etc/hosts y devuelve los datos que encuentra alli en una
estructura, segun el nombre que se le especifique en hst */
hp=gethostbyname(hst);
printf ("Nombre host completo:%s\n",hp->h_name);
printf ("Alias :\n");
printf ("Familia :%d\n",hp->h_addrtype);
printf ("Tamanio :%d\n",hp->h_length);
printf ("Direccion :%ld\n",hp->h_addr_list[0]);


/* A partir de 5000 se pueden usar puertos libres que el sistema no tiene
asignado y que tampoco va a asignar.  de 0 a 1024 son solo reservados para
super usuario, y de 1024 hasta 5000 los puede gestionar automaticamente */

hostsa.sin_port=5001;
hostsa.sin_family=AF_INET;

/* AF_INET especifica el protocolo de internet */

bcopy(hp->h_addr,(char *)&hostsa.sin_addr,hp->h_length);

/* bcopy es una funcion que copia cadenas...  origen  destino  cant a copiar.
se guarda en la estructura correspondiente a este host el dato de la direccion
que tiene asignado este host que leimos antes con gethostbyname */


sock=socket(hostsa.sin_family,SOCK_STREAM,0);

/* Se crea el socket.  El 0 es porque en ese campo va el protocolo especifico
que se va a usar, y poniendo 0 se daja para que se encargue el sistema */


/* bind hace la publicacion en la red de los datos que se fueron guardando 
en la estructura hostsa.  Con esto se habilita la posibilidad de que los
demas se puedan conectar */

if (bind(sock,(void *)&hostsa,sizeof (hostsa))<0)
    {
    printf ("Se produjo un error en el BIND...\n");
    exit(0);
    }
    
/* bind modifica la estructura y coloca una direccion que es la que se le 
asigna a este host dentro de la red  */


printf ("Datos de HOST....\n");
printf ("Puerto asignado:%d\n",hostsa.sin_port);    
printf ("Address:%ld\n",hostsa.sin_addr);
printf ("Familia de protocolos:%d\n",hostsa.sin_family);


/* listen marca la cantidad de elementos que va a tener la cola de espera para
la conexion */

listen(sock,2);

/* accept acepta una conexion connect (para el caso de SOCK_STREAM). 
lo que se le manda es una estructura del mismo tipo que la del hostsa, donde 
se van a guardar los datos del cliente, como direccion, puerto del proceso 
que esta ejecutando como cliente en el otro nodo, etc. */
/* NOTA:  Este puerto puede no coincidir con el anterior, porque corresponde
al proceso que oficia de cliente en la otra maquina */

tams=sizeof(remtsa);
if ( (socki=accept(sock,(void *)&remtsa,&tams))<0 )
    {
    printf ("Se produjo un error en el accept....\n");
    exit(1);
    }

printf ("Conexion exitosa !!!!!!\n\n");
    
printf ("Datos del CLIENTE...\n");
printf ("Puerto asignado:%d\n",remtsa.sin_port);    
printf ("Address:%ld\n",remtsa.sin_addr);
printf ("Familia de protocolos:%d\n",remtsa.sin_family);


printf ("Cerrando el socket .....\n");


close(sock);
getc(stdin);
}
