#include "elinux.h"

char logbuf[1024] = "";

void logger(char msg[])
{
	fprintf(stderr, "LOG: %s\n", msg);
}

void error(char msg[])
{
	fprintf(stderr, "ERR: %s ERRON: %s\n", msg, strerror(errno));
	exit(1);
}
