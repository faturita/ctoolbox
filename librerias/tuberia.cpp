#include <stdio.h>


class clasefifo {
    /**
    Nombre: clasefifo
    Objetivo: Ofrecer una interface de facil acceso para el manejo de ficheros
	      fifo para comunicacion entre procesos.
    Autores: Oel Amon Yag.
    Fecha: 31/08/1999
    **/
    
    }
      







class clasetuberia {
    /**
    Nombre: clasetuberia
    Objetivo: brindar una interface para manejar tuberias.
	      Esta clase permitira manejar una sola tuberia bidireccional
	      entre dos procesos. Uno padre y uno hijo. Para el caso en el 
	      que se deseen tener mas se debe crear un array de este objeto
	      para mantener una tuberia por cada comunicacion bidireccional
	      entre dos procesos.
    Autores: Oel Amon Yag.
    Fecha: 31/08/1999
    **/
    
    /* Manejar dos tuberias para una transferencia de datos bidireccional **/
    private:
        int pipeto[2]; /* Tuberia de padre a hijo */
	int pipefr[2]; /* Tuberia de hijo a padre */
	int Ppid;      /* Alberga el pid del proceso padre (invoca) */
	
    public:
	clasetuberia ()
	    {
	    /* Creacion de ambas tuberias */
	    if ((pipe (pipeto))==-1)
		{
		perror ("Error al crear tuberia\n");
		exit(-1);
		}
	    if ((pipe (pipefr))==-1)
		{
		perror ("Error al crear tuberia\n");
		exit(-1);
		}
	    Ppid = getpid();
	    }
	
	void enviar(char *msj)
	    {
	    int tube[2];
	    /* No devuelve nada porque en caso de error directamente termina */
	    if (getpid()==Ppid)
		{
		/* Ejecucion para el proceso padre */
		tube[1]=pipeto[1];
		}
	    else
		{
		/* Ejecucion para el proceso hijo */
		tube[1]=pipefr[1];
		}
	    if ( (write(tube[1],msj,strlen(msj)))==-1 )
		{
		perror ("Error al escribir en la tuberia.\n");
		exit(-1);
		}
	    }
	    
	void cerrar()
	    {
	    /* Cada proceso elimina toda la tuberia bidireccional. */
	    /* Se debera accionar que se detecta la desconexion de alguno */
	    close(pipefr[0]);
	    close(pipeto[0]);
	    close(pipefr[1]);
	    close(pipeto[1]);
	    }
	    			
	void recibir(char *msj)
	    {
	    int tube[2];
	    int tam;
	    if (getpid()==Ppid)
		{
		/* Ejecucion del proceso Padre */
		tube[0]=pipefr[0];
		}
	    else
		{
		/* Ejecucion del proceso hijo */
		tube[0]=pipeto[0];
		}
	    if ( (tam=read(tube[0],msj,BUFSIZ)) <=0)
		{
		perror ("Se ha producido un error al leer.\n");
		exit(-1);
		}
	    msj[tam]='\0';
	    }
	    
    };
    

void main()
{
clasetuberia tuberia;
int pid;
char c;
char n[BUFSIZ];
pid=getpid();
fork();
if (pid!=getppid())
    {
    tuberia.enviar("Esta es por vos Mishu");
    tuberia.recibir(n);
    printf ("Recibido:%s\n",n);
    }
if (pid==getppid())
    {
    tuberia.recibir(n);
    printf ("Mensaje recibido por hijo:%s\n",n);
    tuberia.enviar("Esta es de yo hijo por vos Mishu");
    }
tuberia.cerrar();
exit(0);
}
	    
		    
	     
	    
	    
		
		
		
    
    
    
    
    
