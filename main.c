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


//所有GPIO寄存器在物理内存的起始地址，注意这个地址只供mmap使用
#define GPIO_PHY_BASE	0xC001A000
#define MMAP_SIZE_GPIO	2058000

//GPIO方向
#define INPUT	0
#define OUTPUT	1

//GPIO电平
#define HIGH	1
#define LOW		0

/*-----------------------------------------------------------------------------
 *  具体到GPIO端口的寄存器偏移值，GPIOA的地址就为GPIO + 0 * GPIO_X_OFFSET
 *  同理GPIOD就为GPIO + 3 * GPIO_X_OFFSET
 *-----------------------------------------------------------------------------*/
#define GPIO_X_OFFSET 0x1000

#define	GPIOA_OFFSET	(0 * GPIO_X_OFFSET)
#define	GPIOB_OFFSET	(1 * GPIO_X_OFFSET)
#define	GPIOC_OFFSET	(2 * GPIO_X_OFFSET)
#define	GPIOD_OFFSET	(3 * GPIO_X_OFFSET)
#define	GPIOE_OFFSET	(4 * GPIO_X_OFFSET)

/*-----------------------------------------------------------------------------
 *  具体寄存器的偏移地址，如果要使用GPIOC的GPIOxALTFN0，那么就应该使用
 *-----------------------------------------------------------------------------*/
#define GPIOxOUT		0x00
#define GPIOxOUTENB		0x04
#define GPIOxDETMODE0	0x08
#define GPIOxDETMODE1	0x0C
#define GPIOxINTENB		0x10
#define GPIOxDET		0x14
#define GPIOxPAD		0x18
#define GPIOxALTFN0		0x20
#define GPIOxALTFN1		0x24
#define GPIOxDETMODEEX	0x28
#define GPIOxDETENB		0x3c
#define GPIOx_SLEW		0x40
#define GPIOx_DRV1		0x48
#define GPIOx_DRV0		0x50
#define GPIOx_PULLSEL	0x58
#define GPIOx_PULLENB	0x60
#define GPIOx_PULLENB_DISABLE_DEFAULT	0x64
#define GPIOx_SLEW_DISABLE_DEFAULT		0x44
#define GPIOx_DRV1_DISABLE_DEFAULT		0x4c
#define GPIOx_DRV0_DISABLE_DEFAULT		0x54
#define GPIOx_PULLSEL_DISABLE_DEFAULT	0x5C


/*-----------------------------------------------------------------------------
 *  由于寄存器都是32位的，所以把指针定义为32位长度的无符号整数，那么根据一定的
 *  偏移值就可以读写某个寄存器的所有内容
 *-----------------------------------------------------------------------------*/
static volatile uint32_t GPIO = -1;

//根据wiringPi管脚编号取得对应的物理管脚的编号
const int wiringPitoNpi[] = {
	61, 58, 62, 63, 78, 59, 
	97, 60, -1, -1, 94, 77,
	95, 96, 93, 117, 113, -1,
	-1, -1, -1, 72, 73, 74, 
	76, 75, 92, 71, -1, -1, -1
};

//每个管脚的bit的位置
const int wiring_pin_bit[] = {
	0x20000000, 0x4000000, 0x40000000, 0x80000000,
	0x4000, 0x8000000, 0x2, 0x10000000, -1,
	-1, 0x40000000, 0x2000, 0x80000000, 0x1,
	0x20000000, 0x200000, 0x20000, -1, -1,
	-1, -1, 0x100, 0x200, 0x400,
	0x1000, 0x800, 0x10000000, 0x80, -1,
	-1, -1 
};


//根据wiringPi管脚编号取得对应的GPIO端口的第一个寄存器的偏移地址
const int wiring_reg_addr[] = {
	GPIOB_OFFSET,
	GPIOB_OFFSET,
	GPIOB_OFFSET,
	GPIOB_OFFSET,
	GPIOC_OFFSET,
	GPIOB_OFFSET,
	GPIOD_OFFSET,
	GPIOB_OFFSET,
	-1,
	-1,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOD_OFFSET,
	GPIOC_OFFSET,
	GPIOD_OFFSET,
	GPIOD_OFFSET,
	-1,
	-1,
	-1,
	-1,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	GPIOC_OFFSET,
	-1,
	-1,
	-1
};

void logger(char msg[])
{
	printf("LOG:%s\n", msg);
}

void error(char msg[])
{
	printf("ERR:%s errno:%s\n", msg, strerror(errno));
	exit(1);
}

int main(int argc, char * argv[])
{

	int fd_mem = 0;

	fd_mem = open("/dev/mem", O_RDWR | O_NDELAY);
	if(-1 == fd_mem)
	{
		error("Can't open /dev/mem.");
	}

	GPIO = (unsigned int ) mmap(0 , 
			MMAP_SIZE_GPIO, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, GPIO_PHY_BASE);
	if(-1 == (int) GPIO)
	{
		error("Can't mmap.");
	}

	char *abcde[] = {"A", "B", "C", "D", "E"};

	int i = 0;
	for(i = 0; i <= 30; i++)
	{
		printf("wiring:%2d num:%3d GPIO:%s%2d %f\n", i, wiringPitoNpi[i], abcde[wiringPitoNpi[i] / 32], wiringPitoNpi[i] % 32, log2(wiring_pin_bit[i]));
	}

	munmap( (void *) GPIO, MMAP_SIZE_GPIO);
	close(fd_mem);

	return EXIT_SUCCESS;
}
