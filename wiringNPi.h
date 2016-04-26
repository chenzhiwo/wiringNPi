#ifndef _WIRINGNPI_H_
#define _WIRINGNPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <math.h>

#define GPIOA(pin)	(0   + (pin))
#define GPIOB(pin)	(32  + (pin))
#define GPIOC(pin)	(64  + (pin))
#define GPIOD(pin)	(96  + (pin))
#define GPIOE(pin)	(128 + (pin))

typedef uint32_t			uint;
typedef unsigned char uchar;

enum BOOL {
	FALSE = 0, TRUE = 1
};

enum LEVEL {
	LOW = 0, HIGH = 1
};

enum DIRECTION {
	INPUT = 0, OUTPUT = 1
};

void gpio_init(void);
void gpio_deinit(void);
void pinMode(uint pin, enum DIRECTION dire);
void digitalWrite(uint pin, enum LEVEL level);
uint digitalRead(uint pin);

#endif
