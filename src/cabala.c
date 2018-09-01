/*
Programa CABALA

Fecha:   08/07/2000

Autor:   Rodrigo Ramele 

Equipo:  Oel Amon Yag Quality Software

*/

#include <stdio.h>
#include <stdlib.h>

void getNumber(int a,...)
    {
    printf ("The value sending is:%d",(int)*(&a+1));
    return;
    }
    
main()
{
int a=3;
int b=7;
char s[]="hola que tal";
getNumber(a,b);
srandom(b);
printf ("The value returned by the random function is:%d",random());
exit(0);
}
