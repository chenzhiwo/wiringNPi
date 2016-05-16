#ifndef _WIRINGNPISPI_H_
#define _WIRINGNPISPI_H_

#include <linux/spi/spidev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>

enum SPI_CHANNELS
{
	SPI0_CHANNEL0 = 0,
	SPI0_CHANNEL1 = 1,
	SPI1_CHANNEL0 = 2,
	SPI1_CHANNEL1 = 3
};

int wiringNPiSPIDataRW    (int channel, unsigned char *data, int len) ;
int wiringNPiSPISetupMode (int channel, int speed, int mode) ;
int wiringNPiSPISetup     (int channel, int speed) ;

#endif
