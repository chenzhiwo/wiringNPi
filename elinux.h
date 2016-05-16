#ifndef _ELINUX_H_
#define _ELINUX_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

extern char logbuf[1024]; 

void logger(char msg[]);
void error(char msg[]);

#endif
