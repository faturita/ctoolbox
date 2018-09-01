#include<stdio.h>
#include<net/linuxnet/arp.h>

void main(argc,argv)
int argc;
char *argv[];
{
//arp_init();

printf ("Welcome to our arp control system.");


if (argc != 3)
    {
    fprintf(stderr,"Son necesarios dos parametros.\n");
    exit(1);
    }
    

exit(0);
}

    

    
    
    

    
    


