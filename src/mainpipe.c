/*
* Ejemplo de pipe (Viejo y conocido)....


*/
#include "unpipc.h"

void client (int,int), server (int, int);

int main(int argc, char **argv)
{
    int pipe1[2], pipe2[2];
    
    pid_t	childpid;
    
    
    /* Creacion comun de dos pipes...*/
    pipe(pipe1);
    pipe(pipe2);
    
    if ( (childpid=fork())==0)
	{
	/* Este es el hijo porque el fork le devolvio 0 */
	printf ("Running child with pid:%d",getpid());
	close(pipe1[1]);
	close(pipe2[0]);
	
	server(pipe1[0], pipe2[1]);
	exit(0);
	}
    
    /* Este es el padre puesto que el fork le devolvio un valor diferente de 0 */
    /* Los cierra al reves que el hijo para dejar un unico canal de comunicacion */
    printf ("Running parent with pid:%d",getpid());
    close(pipe1[0]);
    close(pipe2[1]);
    
    client(pipe2[0], pipe1[1]);
    
    /* Espera que el hijo termine */
    waitpid(childpid, NULL, 0);
    exit(0);
    }


void client(int readfd,int writefd)
{
    size_t len;
    ssize_t n;
    char buff[256];
    
    fgets(buff, 256, stdin);
    
    len = strlen(buff);
    if (buff[len-1] == '\n')
	len--;
	
    write(writefd, buff, len);
    
    while ( (n=read(readfd, buff, 256)) > 0)
	write(STDOUT_FILENO, buff, n);
}


void server(int readfd, int writefd)
{
    int fd;
    ssize_t n;
    char buff[256+1];
    
    if ( (n=read(readfd, buff, 256)) == 0)
	err_quit("end of file while reading pathname...");

    buff[n]='\0';
    
    if ( (fd=open(buff, O_RDONLY)) < 0)
	{
	snprintf (buff+n,sizeof(buff)-n,"Se produjo un error de la puta que los pario...");
	write(writefd, buff, strlen(buff));
	}
    else
	{
	while ( (n=read(fd, buff, 256)) > 0)
	    write(writefd, buff, n);
	close(fd);
	}
    }
    
	
	