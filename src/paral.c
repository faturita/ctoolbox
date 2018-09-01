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
    
for (i=0;i<240;i++)
    {
    int a;
    //printf ("Writing data to 0x378\n");
    //scanf("%d",&a);    
    //outb(a,0x378);
    //scanf("%d",&a);
    //outb(a,0x378+1);
    //scanf("%d",&a);
    //outb(a,0x378+2);
    printf ("Data read 0:%d\n",inb(0x378));
    printf ("Data read 1:%d\n",inb(0x378+1));
    printf ("Data read 2:%d\n",inb(0x378+2));
    
    //if (inb(0x378)!=120) exit(-1);
    system("clear");
    if (i==239) i=0;
    }
}


