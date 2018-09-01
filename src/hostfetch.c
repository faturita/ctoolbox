/**
Example of name bindings.
*/

#include "./lib/unp.h"

int main(int argc, char **argv) {
    char **pptr;
    char str[16];
    struct hostent *hs;
    
    if ( (hs=gethostbyname(argv[1])) == NULL ) {
	printf ("Error when trying to fetch the host...\n");
	exit(-1);
        }
    printf ("Oficcial hostname :%s\n", hs->h_name);
    for (pptr=hs->h_aliases; (*pptr != NULL); pptr++)
	printf ("\tAlias: %s\n", *pptr);
	
    switch (hs->h_addrtype) {
	case AF_INET:
#ifdef AF_INET6
	case AF_INET6:
#endif
	    pptr = hs->h_addr_list;
	    for ( ; *pptr != NULL; pptr++)
		printf ("\tAddress: %s\n",inet_ntop(hs->h_addrtype, *pptr, str, sizeof(str)));
	    break;
	default:
	    printf ("Unknow address type.\n");
	    exit(-1);
	    break;
    }
    exit(0);
}