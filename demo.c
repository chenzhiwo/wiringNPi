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
	for(i = 0; i < 5; i++)
	{
		analogWrite(PWM0, 1000000, 200);
		delay(1000);
		analogWrite(PWM0, 1000000, 900);
		delay(1000);
	}

	analogStop(PWM0);
	wiringNPiExit();

	return EXIT_SUCCESS;

}
