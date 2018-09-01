#include <sys/types.h>
void main ()
{    
    int i=0;
    int pid;

    switch (pid=fork())
    {
	case -1: printf ("Error");
		 exit (1);
		 break;
	case 0:  while (i<10000)
printf ("Soy el hijo, mi pid es %d, el de mi padre %d, i=%d \n",getpid(),getppid(),i++);
			
		 break;
	default:               
		     while (i<10000)
printf ("Soy el padre, mi pid es %d, el de mi hijo %d, i=%d \n",getpid(),pid,i++);
    } 

}