#include "wiringNPi.h"

typedef volatile uint8_t	reg8;
typedef volatile uint32_t	reg32;
/*-----------------------------------------------------------------------------
 *  GPIO_PHY_BASE :Only can be used by mmap. This is the first reg of GPIO
 *  (GPIOAOUT)
 *-----------------------------------------------------------------------------*/
#define GPIO_PHY_BASE	(0xC001A000)
#define GPIO_BASE_OFFSET (0x1000)
#define GPIO_MMAP_SIZE	(0xC001E064 - 0xC001A000)
#define GPIO_NUM_PAD	(5)
#define GPIO_NUM_PER_PAD (32)

struct  gpio_registers
{
	reg32 GPIOxOUT;
	reg32 GPIOxOUTENB;
	reg32 GPIOxDETMODE[2];
	reg32 GPIOxINTENB;
	reg32 GPIOxDET;
	reg32 GPIOxPAD;
	reg32 GPIOxPUENB;
	reg32 GPIOxALTFN[2];
	reg32 GPIOxDETMODEEX;
	reg32 Reserved0[4];
	reg32 GPIOxDETENB;
	reg32 GPIOx_SLEW;
	reg32 GPIOx_SLEW_DISABLE_DEFAULT;
	reg32 GPIOx_DRV1;
	reg32 GPIOx_DRV1_DISABLE_DEFAULT;
	reg32 GPIOx_DRV0;
	reg32 GPIOx_DRV0_DISABLE_DEFAULT;
	reg32 GPIOx_PULLSEL;
	reg32 GPIOx_PULLSEL_DISABLE_DEFAULT;
	reg32 GPIOx_PULLENB;
	reg32 GPIOx_PULLENB_DISABLE_DEFAULT;
	reg32 GPIOx_InputMuxSelect0;
	reg32 GPIOx_InputMuxSelect1;
	reg8  Reserved1[0x1000-0x70];
};

//wiringPi pins to phys pins
static const int wiringpi_to_gpio[] = {
	61, 58, 62, 63, 78, 59,
	97, 60, -1, -1, 94, 77,
	95, 96, 93, 117, 113, -1,
	-1, -1, -1, 72, 73, 74,
	76, 75, 92, 71, -1, -1, -1
};

static const int pin_to_gpio[] = {
	-1, -1, -1, 99, -1, 98, -1,  60, 117, -1, 113,
	61, 58, 62, -1, 63, 78,  -1,  59, 95, -1,
	96, 97, 93, 94, -1, 77, 103, 102, 72, -1,
	73, 92, 74, -1, 76, 71,  75, 162, -1, 163,
};

enum GPIO_ALTFNS {
	GPIO_ALTFN_0 = 0, GPIO_ALTFN_1 = 1, GPIO_ALTFN_2 = 2, GPIO_ALTFN_3 = 3,
};

const enum GPIO_ALTFNS GPIO_ALTFN[GPIO_NUM_PAD][GPIO_NUM_PER_PAD] = {
	//GPIOA
	{
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0
	},

	//GPIOB
	{
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_2 , GPIO_ALTFN_2 , GPIO_ALTFN_1 , GPIO_ALTFN_2 , GPIO_ALTFN_1 ,
		GPIO_ALTFN_2 , GPIO_ALTFN_1 , GPIO_ALTFN_2 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 ,
		GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1
	},
	//GPIOC

	{
		GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 ,
		GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 ,
		GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 ,
		GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1,  GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0
	},
	//GPIOD

	{
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0
	},
	//GPIOE

	{
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 , GPIO_ALTFN_0 ,
		GPIO_ALTFN_0 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1 , GPIO_ALTFN_1
	}
};

#define read_reg8(addr)           (*( (reg8*)  (addr) ))
#define read_reg32(addr)          (*( (reg32*) (addr) ))

#define write_reg8(addr,data)     (*( (reg8*)  (addr) ))  =  ( (reg8)  (data))
#define write_reg32(addr,data)    (*( (reg32*) (addr) ))  =  ( (reg32) (data))

#define gpio_pad(gpio)			( ((gpio) >> 5) & 0B111)
#define gpio_bit(gpio)			( (gpio) & 0B11111)

int gpio_dev_fd = -1;
int pwm_dev_fd = -1;
struct gpio_registers *gpio_MAPPED_ADDR = NULL;
struct gpio_registers *GPIO[GPIO_NUM_PAD] = {};
char logbuf[1024] = "";

static void logger(char msg[])
{
	fprintf(stderr, "LOG:%s\n", msg);
}

static void error(char msg[])
{
	fprintf(stderr, "ERR:%s errno:%s\n", msg, strerror(errno));
	exit(1);
}

// static functions
static inline void gpio_dev_open(void)
{
	int offset = 0;
	gpio_dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);
	if(-1 == gpio_dev_fd)
	{
		error("Can't open /dev/mem.");
	}

	gpio_MAPPED_ADDR = mmap(0 , GPIO_MMAP_SIZE,
			PROT_READ | PROT_WRITE, MAP_SHARED, gpio_dev_fd, GPIO_PHY_BASE);
	sprintf(logbuf, "gpio_MAPPED_ADDR:%p",  gpio_MAPPED_ADDR);
	logger(logbuf);

	for(offset = 0; offset < GPIO_NUM_PAD; offset++)
	{
		GPIO[offset] = (struct gpio_registers*) ( (uint32_t) gpio_MAPPED_ADDR + (offset * GPIO_BASE_OFFSET));
	}

}

static inline void gpio_dev_close(void)
{
	if(-1 != gpio_dev_fd)
	{
		munmap( (void *) gpio_MAPPED_ADDR, GPIO_MMAP_SIZE);
		close(gpio_dev_fd);
	}
	else
	{
		logger("can't munmap a unmapped memory");
	}
}


static inline void gpio_reg_set_bit(reg32 *addr, uint bit, uint val)
{
	reg32 new_val = 0;
	new_val = read_reg32(addr);
	new_val &= ~(1UL << bit);
	new_val |= (val & 0x01) << bit;
	write_reg32(addr, new_val);
}

static inline uint gpio_reg_get_bit(reg32 *addr, uint bit)
{
	return ( ( read_reg32(addr) >> bit ) & ( 1UL ) );
}

static inline void gpio_reg2_set_bit2(reg32 *addr, uint bit, uint val)
{
	reg32 new_val = 0;
	addr = (reg32 *) ( (uint)addr + ((bit / 16) * 0x04) );
	bit = (bit % 16) * 2;

	new_val = read_reg32(addr);
	new_val &= ~(3UL << bit);
	new_val |= ((val & 0B11) << bit);
	write_reg32(addr, new_val);
}

static inline uint gpio_reg2_get_bit2(reg32 *addr, uint bit)
{
	addr = (reg32 *) ( (uint)addr + ((bit / 16) * 0x04) );
	bit = (bit % 16) * 2;
	return ( (read_reg32(addr) >> bit) & 3UL );
}

static inline void gpio_set_altfn_gpio(uint pin)
{
	int pad = gpio_pad(pin);
	int bit = gpio_bit(pin);
	gpio_reg2_set_bit2(&GPIO[pad]->GPIOxALTFN[0], bit, GPIO_ALTFN[pad][bit]);
}

static inline void pwm_dev_open(void)
{
	pwm_dev_fd = open("/dev/pwm", O_RDONLY);
	if(-1 == pwm_dev_fd)
	{
		error("Can't open /dev/pwm.");
	}
}

static inline void pwm_dev_close(void)
{
	if(-1 != pwm_dev_fd)
	{
		close(pwm_dev_fd);
	}
	else
	{
		logger("can't close /dev/pwm");
	}
}

//public function


void delay (unsigned int howLong)
{
	struct timespec sleeper, dummy ;

	sleeper.tv_sec  = (time_t)(howLong / 1000) ;
	sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

	nanosleep (&sleeper, &dummy) ;
}

void delayMicrosecondsHard (unsigned int howLong)
{
	struct timeval tNow, tLong, tEnd ;

	gettimeofday (&tNow, NULL) ;
	tLong.tv_sec  = howLong / 1000000 ;
	tLong.tv_usec = howLong % 1000000 ;
	timeradd (&tNow, &tLong, &tEnd) ;

	while (timercmp (&tNow, &tEnd, <))
		gettimeofday (&tNow, NULL) ;
}

void delayMicroseconds (unsigned int howLong)
{
	struct timespec sleeper ;
	unsigned int uSecs = howLong % 1000000 ;
	unsigned int wSecs = howLong / 1000000 ;

	/* */ if (howLong ==   0)
		return ;
	else if (howLong  < 100)
		delayMicrosecondsHard (howLong) ;
	else
	{
		sleeper.tv_sec  = wSecs ;
		sleeper.tv_nsec = (long)(uSecs * 1000L) ;
		nanosleep (&sleeper, NULL) ;
	}
}

void wiringNPiSetup(void)
{
	gpio_dev_open();
	pwm_dev_open();
}

void wiringNPiExit(void)
{
	gpio_dev_close();
	pwm_dev_close();
}

void pinMode(uint pin, enum DIRECTION dire)
{
	int pad = gpio_pad(pin);
	int bit = gpio_bit(pin);

	gpio_set_altfn_gpio(pin);
	dire &= 0x01;

	if(OUTPUT == dire)
	{
		gpio_reg_set_bit(&GPIO[pad]->GPIOxOUTENB, bit, TRUE);
		gpio_reg_set_bit(&GPIO[pad]->GPIOxINTENB, bit, FALSE);
	}
	else
	{
		gpio_reg_set_bit(&GPIO[pad]->GPIOxOUTENB, bit, FALSE);
		gpio_reg_set_bit(&GPIO[pad]->GPIOxINTENB, bit, TRUE);
	}
}

void digitalWrite(uint pin, enum LEVEL level)
{
	gpio_reg_set_bit(&GPIO[gpio_pad(pin)]->GPIOxOUT, gpio_bit(pin), level);
}

uint digitalRead(uint pin)
{
	return gpio_reg_get_bit(&GPIO[gpio_pad(pin)]->GPIOxPAD, gpio_bit(pin)) ;
}

void analogWrite(uint pin, uint freq, uint duty)
{
	uint pwm_args[3] = {};
	pwm_args[0] = pin;
	pwm_args[1] = freq;
	pwm_args[2] = duty;
	if(-1 == ioctl(pwm_dev_fd, PWM_IOCTL_SET_FREQ, pwm_args))
	{
		logger("fail to set pwm");
	}
}

void analogStop(uint pin)
{
	if(-1 == ioctl(pwm_dev_fd, PWM_IOCTL_STOP, &pin))
	{
		logger("fail to stop pwm");
	}
}
