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
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#define GPIOA(pin)	(0   + (pin))
#define GPIOB(pin)	(32  + (pin))
#define GPIOC(pin)	(64  + (pin))
#define GPIOD(pin)	(96  + (pin))
#define GPIOE(pin)	(128 + (pin))

#define PWM_IOCTL_SET_FREQ      (0x1)
#define PWM_IOCTL_STOP          (0x0)
#define PWM_IOCTL_CONFIG        (0x4)
#define PWM_IOCTL_RELEASE       (0x8)

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

enum PWMS {
	PWM0 = GPIOD(1),
	PWM1 = GPIOC(13),
	PWM2 = GPIOC(14)
};

void delay (unsigned int howLong);
void delayMicrosecondsHard (unsigned int howLong);
void delayMicroseconds (unsigned int howLong);
void wiringNPiSetup(void);
void wiringNPiExit(void);
void pinMode(uint pin, enum DIRECTION dire);
void digitalWrite(uint pin, enum LEVEL level);
uint digitalRead(uint pin);
void analogWrite(uint pin, uint freq, uint duty);
void analogStop(uint pin);

#endif
