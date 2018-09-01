/**
 * Servidor UDP sobre puerto 4500 de echo reply.
 *
 * A diferencia del cliente realiza un bind para especificarle al kernel que
 * acepte paquetes udp con puerto 4500 y que los derive a este proceso.
 */
 
#include "./lib/unp.h"

static void sig_interrupt(int signo) {
    printf ("I am dying ... Signal number:%d",signo);
    
    exit(1);
}

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen) {
    int n;
    socklen_t len;
    char mesg[256];
    
    for (;;) {
	len = clilen;
	n = recvfrom(sockfd, mesg, 256, 0, pcliaddr, &len);
	
	sendto(sockfd, mesg, n, 0, pcliaddr, len);
    }
}

int main(int argc,char **argv) {
    int sockfd;
    
    struct sockaddr_in servaddr, cliaddr;
    void sig_interrupt(int);
    
    signal(SIGINT, sig_interrupt);    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(4500);
    
    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
    

    dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
