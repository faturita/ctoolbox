/*
Utilizacion de popen, el pipe comun del shell...

El segundo argumento puede ser
    r	Para crear un pipe de entrada/lectura
    w	Para crear un pipe de salida/escritura
    

*/

#include "unpipc.h"

int main(int argc, char **argv)
{
    size_t n;
    char buff[256];
    
    FILE *fp;
    
    printf ("Escriba un comando a ejecutar....:");
    fgets(buff,256,stdin);
    n = strlen(buff);
    if ( buff[n-1] = '\n' )
	buff[n-1]='\0';

    fp = popen(buff,"r");
    
    while (fgets(buff,256,fp) != NULL)
	fputs(buff,stdout);
    
    pclose(fp);
    exit(0);
}
    
    
