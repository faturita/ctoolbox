/**
 * Ejemplo de conexi'on UDP
 * Extraido del libro de stevens, UNIX NETWORK PROGRAMMING.
 *
 * Inicializa las estructuras para trabajar con UDP.
 * Abre un socket para servir como interfaz, y luego utiliza recvfrom y sendto
 * para enviar informacion.  No controla errores de ning'un tipo.
 */

#include "./lib/unp.h"

void dg_cli(FILE *pf, int sockfd, const SA *pservaddr, socklen_t servlen) {
    int n;
    char sendline[256], recvline [257];
    
    /* Nueva direccion para comprobar si los paquetes UDP recibidos son del
    servidor esperado */
    socklen_t len;
    struct sockaddr *preply_addr;
    preply_addr = (struct sockaddr*)malloc(servlen);
        
    while (fgets(sendline, 256, pf) != NULL) {
	sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
	len=servlen;
	/* recvfrom (sockfd, char [], sizechar, flags, estrucutura server, tamanio) */
	/* n = recvfrom(sockfd, recvline, 256, 0, NULL, NULL); */
	n = recvfrom(sockfd, recvline, 256, 0, preply_addr, &len);
	// En linux memcmp devuelve -1 si son iguales.
	if (len != servlen || memcmp(pservaddr, preply_addr, len) != -1) {
	    printf ("Reply from: %s ignored...\n",Sock_ntop(preply_addr,len));
	    continue;
	}
	
	recvline[n] = '\0';
	fputs(recvline, stdout);
    }
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    
    if (argc!=2) {
	printf ("Hey man, this was a great mistake !\n");
	printf ("You may provide 1 arguments, which is the server.\n");
	exit(0);
    }
    /* Blanquea la estructura */	
    bzero(&servaddr, sizeof(servaddr));
    
    /* Inicializa los campos para que funcionen en el puerto 4500 */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(4500);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    
    /* Genera el socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));
    exit(0);
}