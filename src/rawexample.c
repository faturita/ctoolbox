/**
 * Servidor UDP sobre puerto 4500 de echo reply.
 *
 * A diferencia del cliente realiza un bind para especificarle al kernel que
 * acepte paquetes udp con puerto 4500 y que los derive a este proceso.
 */
 
#include "./lib/unp.h"
#include <linux/if_ether.h>
#include <net/if_packet.h>

void dg_echo(int sockfd) {
    int n;
    socklen_t len;
    char mesg[256];
    struct sockaddr_in servaddr, cliaddr;
    int i=0;
    
    while (1) {
        len = sizeof(servaddr);
        n=recvfrom(sockfd,mesg,sizeof(mesg),0,&servaddr,&len);
        for (i=0;i<n;i++) {
	    printf (" %c ",mesg[i]);
	}
    }
}

int main(int argc,char **argv) {
    int sockfd;
    int  n,len;
    char mesg[256];
    const int on=256;

    struct sockaddr_in servaddr, cliaddr;

    printf ("Starting sniffer....");
    

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP))==-1) {
	printf ("Error en la apertura del socket especificado...");
	exit (-1);
    }
    
    if (setsockopt(sockfd, IPPROTO_IP,IP_HDRINCL, &on, sizeof(on)) <0) {
	printf ("Error no se puede especificar el tipo de socket.");
	exit(-1);
	}
    
    printf ("Socket initializzed.");
    
    bzero(&servaddr, sizeof(servaddr));
    //servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //servaddr.sin_port = htons(ETH_P_IP);
    
//    bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
    
    dg_echo(sockfd);
}
