#include "unp.h"
#include <syslog.h>

#define MAXFD	64

/**
* Basicamente genera un hijo del proceso original.  Luego lo setea como proceso
* lider de session.  Captura la senal de SIGHUP,y genera un segundo hijo. El
* primer hijo finaliza y se lleva consigo la posibilidad de quedar sessionado a una
* terminal especifica en caso de abrirla.  Luego el segundo hijo setea como
* su directorio de trabajo "/" y coloca en 0 la mascara de bits de seguridad
* por defecto para la creacion de archivos.  Luego cierra todos los descriptores
* que se suponen que pudieron quedar abiertos en el proceso original (best-effort)
* y luego inicializa las secuencias para loguear con el syslogd.
*
* Ref: UNIX NETWORK PROGRAMMING vol 1
*/

void daemon_init(const char *pname, int facility) {
    int	i;
    pid_t	pid;
    
    if ( (pid=fork()) != 0 )
	exit(0);		// Terminates parent
	
    setsid();			// become session leader.
    
    signal(SIGHUP, SIG_IGN);
    
    if ( (pid=fork()) != 0)
	exit(0);		// First child terminates
	
    chdir("/");
    
    umask(0);
    
    for (i=0;i<MAXFD;i++)
	close(i);
	
    openlog(pname, LOG_PID, facility);
}