/* Este programa muestra como ejemplo, como utilizar parametros en linea de
comando en los programas en C en linux.... */

#include<stdio.h>
#include<sys/types.h>

/* En argc se recibe la cantidad de parametros incluido el comando. 
   argv es un puntero a diferentes vectores de caracteres.
   argv[0] representa el nombre del archivo que se ejecuto.
   argv[1] es el primer parametro (vector de caracteres.)
   y asi sucesivamente.....
  */
  
void main(argc,argv)		

int argc;       		// El numero de parametros
char *argv[];
{
printf ("\nLa cantidad de parametros es:%d",(argc-1));
printf ("\nParametro numero 1:");puts(argv[1]);
}
