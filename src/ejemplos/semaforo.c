#include<sys/types.h>
#include<stdio.h>
#include<sys/ipc.h>
#include<sys/sem.h>

/* Cada proceso usa su parte de memoria aunque la direccion sea la misma.*/



void main(void)
{
// Pan y queso....
int sid;
int Ppid;
int i;
struct sembuf semaforo;
sid=semget(IPC_PRIVATE,2,IPC_CREAT | 0600);


semctl(sid,0,SETVAL,0);
semctl(sid,1,SETVAL,1);

fork();
if (Ppid==getppid())
    {
    /* Ejecuta el hijo */
    printf ("Ejecutando el hijo...\n");
    semaforo.sem_num=1;
    for (i=0;i<10;i++)
	{
	printf ("Pan...\n");
	semaforo.sem_op=1;
	semop(sid,&semaforo,1);   // V
	semaforo.sem_op=-1;
	semop(sid,&semaforo,1);   // P
	}
    semctl(sid,0,IPC_RMID,0);
    exit(0);
    }
else
    {
    /* Ejecuta el padre */
    semaforo.sem_num=0;
    printf ("ejecutando el padre....\n");
    for (i=0;i<10;i++)
	{
	semaforo.sem_op=-1;
	semop(sid,&semaforo,1);   // P
	printf ("Queso.\n");
	semaforo.sem_op=1;
	semop(sid,&semaforo,1);   // V
	}
    semctl(sid,0,IPC_RMID,0);
    exit(0);
    }
    
}
	
        
    
    



