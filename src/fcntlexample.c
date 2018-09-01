#include "./lib/unp.h"

int main(int argc, char **argv) {
    int fd,len,flags;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if ( (flags = fcntl(fd, F_GETFL, 0)) < 0) {
	printf ("Error fcntl get");
    } else {
	flags |= O_NONBLOCK;
	if (fcntl(fd, F_SETFL, flags) < 0) {
	    printf ("Error fcntl set");
	}  else {
	    printf ("Setting NONBLOCKING IO");
	}
	    
    }
    close(fd);	
    
}