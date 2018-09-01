#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct tVector {
    int	entero;
    int terminado;
    int resultado;
    } vec[4];


void main()
{
int Hpid;
int Ppid;
int i;
char c;
struct msgbuf *msgp;
int msgid;
int msgsz;
int msgflg;
int msgtyp;
int status;

int whoiam=0;
msgp->mtype=0;

msgsz=8;
msgtyp=0;
msgflg=IPC_NOWAIT;
printf ("Iniciando el programa....\n");
/* IPC_PRIVATE establece la nueva llave activa que provee el sistema */
msgid=msgget(IPC_PRIVATE,msgflg);
printf ("Numero de cola:%d",msgid);    

if (msgid<0)
    {
    printf ("\nSe ha producido un error al crear el servicio de mensajes.");
    exit (1);
    }
Ppid=getpid();
Hpid=fork();
if (Ppid!=getpid())
    	{
	strcpy(msgp->mtext,"HOLA\0");
	printf ("\nEjecutando el hijo....");
	if (msgsnd(msgid,msgp,msgsz,msgflg)<0)
	    {
	    printf ("\nError en el envio del mensaje....");
	    
	    }
	exit(1);
	    
	}
else
    {
    while (1)
	{
	wait(&status);
	printf ("\nEjecutando el padre.....");
	if (msgrcv(msgid,msgp,msgsz,msgtyp,msgflg)<0) 
	    {
	    printf ("\nError en la recepcion del mensaje....");
	    
	    }
	printf ("%s",msgp->mtext);exit(1);
	}
	
    }
    
}

    
	
	
