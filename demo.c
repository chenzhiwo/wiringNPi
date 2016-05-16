#include <stdio.h>
#include <stdlib.h>
#include <wiringNPi.h>
#include <wiringNPiSPI.h>


int main(int argc, char * argv[])
{
#define PIN GPIOC(12)

	int i = 0;
	wiringNPiSetup();

	pinMode(PIN, OUTPUT);

//	for(i = 0; i < 100000; i++)
//	{
//		digitalWrite(PIN, HIGH);
//		digitalWrite(PIN, LOW);
//	}
//	for(i = 0; i < 5; i++)
//	{
//		analogWrite(PWM0, 1000000, 200);
//		delay(500);
//		analogWrite(PWM0, 1000000, 900);
//		delay(500);
//	}
//
//	analogStop(PWM0);
//
	unsigned char spi_buf[100] = {};
	for(i = 0; i < 10; i++)
	{
		spi_buf[i] = (i * 0xf) + i;
	}
	wiringNPiSPISetup(SPI0_CHANNEL0, 100000);
	wiringNPiSPISetupMode(SPI0_CHANNEL0, 100000, SPI_MODE_0);
	wiringNPiSPIDataRW(SPI0_CHANNEL0, spi_buf, 10);

	wiringNPiExit();

	return EXIT_SUCCESS;

}
