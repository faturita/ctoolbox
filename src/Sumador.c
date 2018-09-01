#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>

/* Cada proceso usa su parte de memoria aunque la direccion sea la misma.*/
/* 
Ejemplo:
Este es un claro ejemplo de que la memoria dentro de cada proceso es local a
el mismo, y que no puede ser compartida directamente.  Para poder compartir
memoria es necesario utilizar esta funcionalidad ipc y especificar que areas
seran compartidas.  A diferencia de los threads en donde la memoria si se
comparte, en los procesos no, y las variables son siempre locales a cada uno 
de los procesos.
*/


void main(void)
{
int pid;
int PiD=getpid();
long int i;
int whoami=0;
int p[3]={0,0,0};
pid=fork();
if (pid==0) whoami++;
if (PiD==getpid()) pid=fork();
if (pid==0) whoami++;
if (PiD==getpid()) for(i=0;i<1500000;i++) i=i+23+23-46;   /* Hace esperar al proceso padre */
p[whoami]=whoami;
printf ("Mi pid es:%d\n",getpid());
/*printf ("El puntero p:%x apunta al valor:%d\n",p,*p);*/
if (PiD==getpid()) for(i=0;i<1500000;i++) i=i+23+23-46;
for(i=0;i<3;i++) printf ("%d:",p[i]);printf ("\n");
}



