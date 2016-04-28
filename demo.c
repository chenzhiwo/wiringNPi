#include <stdio.h>
#include <stdlib.h>
#include <wiringNPi.h>


int main(int argc, char * argv[])
{
#define PIN GPIOC(12)

	int i = 0;
	wiringNPiSetup();

	pinMode(PIN, OUTPUT);
	for(i = 0; i < 100000; i++)
	{
		digitalWrite(PIN, HIGH);
		digitalWrite(PIN, LOW);
	}

	wiringNPiExit();

	return EXIT_SUCCESS;

}
