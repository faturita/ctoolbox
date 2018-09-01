/**
Example of name bindings.
*/

#include "./lib/unp.h"
#include <netinet/in.h>

int main(int argc, char **argv) {
    char **pptr;
    char str[16];
    struct hostent *hs;
    struct in_addr *ad;
    
    /* Check that i am using the entire in_addr structure to allocate the address
    and i after use the structure as argument for gethostbyaddr. */
    inet_pton(AF_INET, argv[1], ad);

    if ( (hs=gethostbyaddr((char *)ad, sizeof(struct in_addr), AF_INET)) == NULL ) {
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