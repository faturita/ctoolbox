#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
void main ()
{    
    int turno,i=0;    
    int a=0,b=0,c=0; 
    int pid;
    
    struct sembuf semaforo;
    

    if ((turno=semget(IPC_PRIVATE,2,IPC_CREAT|0600))==-1)
    {
	printf("ERROR AL CREAR SEMAFORO:\n");
	exit (-1);
    }
    

    semctl(turno,1,SETVAL,1);
    semctl(turno,0,SETVAL,0);
    
    switch (pid=fork())
    {
	case -1: printf ("Error en la llamada a fork()");
		 exit (1);
		 break;
	case 0:  while (i<3)		
		 {   i++;
		     /*******P(1)*******/
		     semaforo.sem_num=1;
		     semaforo.sem_op=-1;
		     semaforo.sem_flg=0;
		     semop (turno, &semaforo,1);
		     
		     printf ("Hola soy el hijo! \n");
		     a=getpid();
		     b=getppid();
	
	             /*******V(0)*******/  
		     semaforo.sem_num=0;
		     semaforo.sem_op=1;
		     semaforo.sem_flg=0;
		     semop (turno, &semaforo,1);
		     
		 }
		 break;
	default: while (i<3)
		 {
		     i++;
		     /*******P(0)*******/
		     semaforo.sem_num=0;
		     semaforo.sem_op=-1;
		     semaforo.sem_flg=0;
		     semop (turno, &semaforo,1);
		     
		     printf ("Hola soy el padre! \n");
		     b=getpid();
		     c=getppid();
		     
		     /*******V(1)*******/
		     semaforo.sem_num=1;
		     semaforo.sem_op=1;
		     semaforo.sem_flg=0;
		     semop (turno, &semaforo,1);
		 }
    }
printf("Hoy %d de %d del a¤o %d hace mucho frio !\n",a,b,c);
}