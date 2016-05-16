#include <stdio.h>
#include <stdlib.h>
#include <wiringNPi.h>
#include <wiringNPiSPI.h>


int main(int argc, char * argv[])
{
	int i = 0;

	wiringNPiSPISetup(SPI0_CHANNEL0, 100000);

	unsigned char spi_buf[100] = {};

	for(i = 0; i < 30; i++)
	{
		spi_buf[i] = i + '0';
	}

	wiringNPiSPIDataRW(SPI0_CHANNEL0, spi_buf, 30);

	for(i = 0; i < 30; i++)
	{
		printf("%c\n", spi_buf[i]);
	}

	return EXIT_SUCCESS;

}
