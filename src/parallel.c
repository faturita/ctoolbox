#include <stdio.h>
#include <stdlib.h>
#include <asm/io.h>

int main(int argc,char *argv[])
{
int i;
if (ioperm(0x378,3,1))
    {
    perror("Se ha producido un error en el puerto asignado.\n");
    exit(-1);
    }
    
for (i=0;i<10000;i++)
    {
    //outb(99,0x378+1);
    //outb(175,0x378+2);
    outb(111,0x378);
    printf ("Saving info...\n");
    //printf ("Reading data from port 0x378:%d\n",inb(0x378+1));
    }
}


