/* Este ejemplo demuestra el uso de templates en linux */
/* Este programa se debe compilar usando el compilador g++ */


#include <stdio.h>

template <class T> T valor(T a,T b)
    {
    printf ("El valor de a es :%d",a);
    return a;
    };
    
void main(int argc,char *argv[])
{
/* Programa ejemplo de implementacion de templates */
int a,b;
a=valor((a=2),(b=3));
printf ("El valor que devolvio la funcion es :%d",a);
}
 