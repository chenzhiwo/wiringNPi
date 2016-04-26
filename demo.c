#include <stdio.h>
#include <stdlib.h>
#include "wiringNPi.h"


int main(int argc, char * argv[])
{
#define PIN GPIOD(1)

	int i = 0;
	gpio_init();

	pinMode(PIN, OUTPUT);
	for(i = 0; i < 2000000; i++)
	{
		digitalWrite(PIN, HIGH);
		digitalWrite(PIN, LOW);
	}

	gpio_deinit();

	return EXIT_SUCCESS;

}
