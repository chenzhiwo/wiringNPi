#include "wiringNPiSPI.h"
#include "elinux.h"

const uint8_t SPI_BITS_PER_WORD	= 8;

const static char *spi_channel_to_dev[4] = {
	"/dev/spidev0.0",
	"/dev/spidev0.1",
	"/dev/spidev1.0",
	"/dev/spidev1.1",
};

static int spi_fds[4] = {
	-1, -1, -1, -1
};

static int spi_speeds[4] = {
	-1, -1, -1, -1
};

static int spi_bits_per_word[4] = {
	-1, -1, -1, -1
};


int wiringNPiSPIDataRW (int channel, unsigned char *data, int len)
{
	struct spi_ioc_transfer spi_data = {};

	channel &= 0B11;

	spi_data.tx_buf = (unsigned long) data;
	spi_data.rx_buf = (unsigned long) data;
	spi_data.len = len;
	spi_data.delay_usecs = 0;
	spi_data.speed_hz = spi_speeds[channel];
	spi_data.bits_per_word = spi_bits_per_word[channel];

	return ioctl(spi_fds[channel], SPI_IOC_MESSAGE(1), &spi_data);
}

int wiringNPiSPISetupMode (int channel, int speed, int rw_mode)
{
	channel &= 0B11;
	rw_mode &= 0B11;

	spi_speeds[channel] = speed;
	spi_bits_per_word[channel] = SPI_BITS_PER_WORD;

	if(ioctl(spi_fds[channel], SPI_IOC_WR_MODE, &rw_mode) == -1)
	{
		error("fail to set spi r/w mode.");
		return -1;
	}

	if(ioctl(spi_fds[channel], SPI_IOC_RD_MODE, &rw_mode) == -1)
	{
		error("fail to set spi r/w mode.");
		return -1;
	}

	if(ioctl(spi_fds[channel], SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
	{
		error("fail to set spi speed.");
		return -1;
	}

	if(ioctl(spi_fds[channel], SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
	{
		error("fail to set spi speed.");
		return -1;
	}

	if(ioctl(spi_fds[channel], SPI_IOC_WR_BITS_PER_WORD, &spi_bits_per_word[channel]) == -1)
	{
		error("fail to set spi bits per word.");
		return -1;
	}

	if(ioctl(spi_fds[channel], SPI_IOC_RD_BITS_PER_WORD, &spi_bits_per_word[channel]) == -1)
	{
		error("fail to set spi bits per word.");
		return -1;
	}

	return 0;
}

int wiringNPiSPISetup (int channel, int speed)
{
	int fd = -1;
	channel &= 0B11;

	fd = open(spi_channel_to_dev[channel], O_RDWR);
	if(fd == -1)
	{
		error("fail to open spi dev.");
		return -1;
	}
	spi_fds[channel] = fd;

	//Set default SPI Mode to Mode0.
	wiringNPiSPISetupMode(channel, speed, SPI_MODE_0);

	return 0;
}

