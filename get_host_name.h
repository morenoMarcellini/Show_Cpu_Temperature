#ifndef GET_HOST_NAME
#define GET_HOST_NAME

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* for gethostname */
#include <netdb.h> /* for hints */

int get_host_name_IP4(char*);
int get_host_name_IP6(char*);

#endif
