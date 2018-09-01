#include "./lib/unp.h"
#include <netinet/tcp.h>

/**
Basically this program was taken from unix network programming book and 
it could be userful to test default socket options, and to see and examine
the available other options for other kinds of protocols. 


Notes:

IPPROTO_IP options:

    IP_OPTIONS
    IP_HDRINCL
    IP_RECVDSTADDR
    IP_RECVIF
    IP_TOS
    IP_TTL

IPPROTO_ICMPV6

    ICMP6_FILTER

IPPROTO_IPV6

    IPV6_ADDRFORM
    IPV6_CHECKSUM
    IPV6_DSTOPTS
    IPV6_HOPLIMIT
    IPV6_HOPOPTS
    IPV6_NEXTHOP
    IPV6_PKTINFO
    IPV6_PKTOPTIONS
    IPV6_RTHDR
    IPV6_UNICAST_HOPS

IPPROTO_TCP

    TCP_KEEPALIVE
    TCP_MAXRT	(time for retransmittion, -1 forever)
    TCP_MAXSEG	(MSS)
    TCP_NODELAY
    TCP_STDURG (urgent pointer)
*/


union val {
    int			i_val;
    long		l_val;
    char		c_val[10];
    struct linger	linger_val;
    struct timeval	timeval_val;
} val;

static char *sock_str_flag(union val *, int);
static char *sock_str_int(union val *, int);
static char *sock_str_linger(union val *, int);
static char *sock_str_timeval(union val *, int);

struct sock_opts {
    char	*opt_str;
    int		opt_level;
    int		opt_name;
    char	*(*opt_val_str)(union val *, int);
} sock_opts[] = {
    "SO_BROADCAST",		SOL_SOCKET, SO_BROADCAST,	sock_str_flag,
    "SO_DEBUG",			SOL_SOCKET, SO_DEBUG	,	sock_str_flag,
    "SO_DONTROUTE",		SOL_SOCKET, SO_DONTROUTE,	sock_str_flag,
    "SO_ERROR",			SOL_SOCKET, SO_ERROR,		sock_str_int,
    "SO_KEEPALIVE",		SOL_SOCKET, SO_KEEPALIVE,	sock_str_flag,
    "SO_LINGER",		SOL_SOCKET, SO_LINGER,		sock_str_linger,
    "SO_OOBINLINE",		SOL_SOCKET, SO_OOBINLINE,	sock_str_flag,
    "SO_RCVBUF",		SOL_SOCKET, SO_RCVBUF,		sock_str_int,
    "SO_SNDBUF",		SOL_SOCKET, SO_SNDBUF,		sock_str_int,
    "SO_RCVLOWAT",		SOL_SOCKET, SO_RCVLOWAT,	sock_str_int,
    "SO_SNDLOWAT",		SOL_SOCKET, SO_SNDLOWAT,	sock_str_int,
    "SO_RCVTIMEO",		SOL_SOCKET, SO_RCVTIMEO,	sock_str_timeval,
    "SO_SNDTIMEO",		SOL_SOCKET, SO_SNDTIMEO,	sock_str_timeval,
    "SO_REUSEADDR",		SOL_SOCKET, SO_REUSEADDR,	sock_str_flag,
#ifdef SO_REUSEPORT
    "SO_REUSEPORT",		SOL_SOCKET, SO_REUSEPORT,	sock_str_flag,
#else
    "SO_REUSEPORT",		0,	0,			NULL,
#endif
    "SO_TYPE",			SOL_SOCKET, SO_TYPE,		sock_str_int,
#ifdef SO_USELOOPBACK
    "SO_USELOOPBACK",		SOL_SOCKET, SO_USELOOPBACK,	sock_str_flag,
#else
    "SO_USELOOPBACK",		0,	0,			NULL,
#endif
    "IP_TOS",			IPPROTO_IP, IP_TOS,		sock_str_int,
    "IP_TTL",			IPPROTO_IP, IP_TTL,		sock_str_int,
    "TCP_MAXSEG",		IPPROTO_TCP, TCP_MAXSEG,	sock_str_int,
    "TCP_NODELAY",		IPPROTO_TCP, TCP_NODELAY,	sock_str_flag,
    NULL,			0,	0,			NULL
};

int main(int argc, char **argv) {
    int fd,len;
    struct sock_opts *ptr;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    for (ptr = sock_opts; ptr->opt_str != NULL; ptr++) {
	printf ("%s:", ptr->opt_str);
	if (ptr->opt_val_str == NULL)
	    printf ("(undefined)\n");
	else {
	    len = sizeof(val);
	    if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1) {
		printf ("getsockopt error!\n");
		exit (-1);
	    } else {
		printf ("default = %s\n", (*ptr->opt_val_str) (&val, len));
	    }
	}
    }
}

static char strres[128];
static char *sock_str_flag(union val *ptr, int len) {
    if (len != sizeof(int))
	snprintf (strres, sizeof(strres), "size (%d) not sizeof(int)",len);
    else
	snprintf(strres, sizeof(strres), "%s", (ptr->i_val ==0) ? "off" : "on");
    return (strres);
}

static char *sock_str_linger(union val *ptr, int len) {
    snprintf (strres, sizeof(strres),"l_onoff=%s and l_linger=%d",((ptr->linger_val.l_onoff) == 0 ? "off" : "on"),(ptr->linger_val.l_linger), len);
    return (strres);
}

static char *sock_str_timeval(union val *ptr, int len) {
    snprintf (strres, sizeof(strres),"timeval",len);
    return (strres);
}

static char *sock_str_int(union val *ptr, int len) {
    snprintf (strres, sizeof(strres), "%d", (ptr->l_val) );
    return (strres);
}