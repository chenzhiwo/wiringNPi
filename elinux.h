#ifndef _ELINUX_H_
#define _ELINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define logger(msg)	do{\
	fprintf(stderr, "LOG: %s\n", msg);\
}while(0);

#define error(msg) do{\
	fprintf(stderr, "ERR: %s ERRON: %s\n", msg, strerror(errno));\
	exit(1);\
}while(0);

#ifdef DEBUG
#define debug(msg) do{\
	fprintf(stderr, "DEBUG: %s\n", msg);\
}while(0);

#else
#define debug(msg)
#endif


#endif

