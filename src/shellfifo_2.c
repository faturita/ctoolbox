/*
Utilizacion de popen (ver shellpipe.c) con fifos con nombre    

*/

#include "unpipc.h"

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int main(int argc, char **argv)
{
    size_t n;
    char buff[256];
    int writefd,readfd;
    
    FILE *fp;
    
    printf ("Este programa esta disenado para ser ejecutado en background.\n");
    
    /* Creacion de los fifos si es que no estan crados.... */
    
    if ((mkfifo(argv[2], FILE_MODE) < 0) && (errno != EEXIST) )
	err_quit ("No se puede crear el fifo");
    if ((mkfifo(argv[1], FILE_MODE) < 0) && (errno != EEXIST) )
	{
	//unlink(argv[2]);
	err_quit ("NO Se puede crear el segundo fifo.");
	}

    
    readfd=open(argv[2],O_RDONLY,0);
    writefd=open(argv[1],O_WRONLY,0);
        
        
    // Lee el comando a transmitir por el fifo de la linea de entrada.
    
    fgets(buff,256,stdin);
    n = strlen(buff);
    if ( buff[n-1] = '\n' )
	buff[n-1]='\0';
    //fputs(buff,writefd);
    write(writefd,buff,strlen(buff));
            
    while ( (n = read(readfd,buff,256) ) > 0)
	fputs(buff,stdout);
    
    close(readfd);
    close(writefd);
    
    //unlink(argv[1]);
    //unlink(argv[2]);
    
    exit(0);
}
    
    
