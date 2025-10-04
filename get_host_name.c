#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for gethostname */
#include <netdb.h> /* for hints */

#include "get_host_name.h"

int get_host_name_IP4(char *host_name)
{
    /* easy way to get hostname */
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    printf("Hostname: %s\n", hostname);
#if 0                                   /* an other way to get the name */
    struct hostent *h;
    h = gethostbyname(hostname);
    printf("h_name: %s\n", h->h_name);
#endif
	int ret = snprintf(host_name, sizeof hostname, "%s", hostname);

	return ret; 
}

int get_host_name_IP6(char *host_name){
    /* more advanced version, working with IPv6 */
    struct addrinfo hints, *info, *p;
    int gai_result;
    char hostname[1024];

    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    printf("hostname: %s DISPLAY: %s\n", hostname, getenv("DISPLAY"));
#if 0 /* IPv6*/
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;        /*either IPV4 or IPV6*/
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0)
    {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai_result));
	exit(1);
    }
    freeaddrinfo(info);
    for (p = info; p != NULL; p = p->ai_next)
	printf("hostname: %s DISPLAY: %s\n", p->ai_canonname, getenv("DISPLAY"));
#endif
    int ret = snprintf(host_name, sizeof hostname, "%s", hostname);

    return ret;
}
