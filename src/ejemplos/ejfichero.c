/* PROGRAMA: Ejemplo Fichero
*/

#include <stdio.h>
#include <fcntl.h>

void main(argc,argv)
int argc;
char *argv[];
{
    char buffer[BUFSIZ];
    int fd;    /* File descriptor */
    int nbytes;
    
    if ((fd=open(argv[1],O_RDONLY))==-1)
    {
	perror(argv[1]);
	exit(-1);
    }
    printf ("Comenzando la lectura del archivo especificado...\n");
    while ((nbytes = read(fd,buffer,sizeof(buffer))) > 0)
	printf ("%s",buffer);
	
    printf ("\n");
    printf ("Cerrando el archivo %s...",argv[1]);
    close(fd);
    exit (0);
}

    
