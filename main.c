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

/*-----------------------------------------------------------------------------
 *  GPIO_PHY_BASE :Only can be used by mmap. This is the first reg of GPIO
 *  (GPIOAOUT)
 *-----------------------------------------------------------------------------*/
#define GPIO_PHY_BASE	(0xC001A000)
#define GPIO_BASE_OFFSET (0x1000)
#define MMAP_SIZE_GPIO	(2058000)
#define GPIO_PAD_NUM	(5)

#define ReadIO8(Addr)           (*( (reg8*)  (Addr) ))
#define ReadIO16(Addr)          (*( (reg16*) (Addr) ))
#define ReadIO32(Addr)          (*( (reg32*) (Addr) ))

#define WriteIO8(Addr,Data)     (*( (reg8*)  (Addr) ))  =  ( (reg8)  (Data))
#define WriteIO16(Addr,Data)    (*( (reg16*) (Addr) ))  =  ( (reg16) (Data))
#define WriteIO32(Addr,Data)    (*( (reg32*) (Addr) ))  =  ( (reg32) (Data))

#define SetIO8(Addr,Data)       (*( (reg8*)  (Addr) )) |=  ( (reg8)  (Data))
#define SetIO16(Addr,Data)      (*( (reg16*) (Addr) )) |=  ( (reg16) (Data))
#define SetIO32(Addr,Data)      (*( (reg32*) (Addr) )) |=  ( (reg32) (Data))

#define ClearIO8(Addr,Data)     (*( (reg8*)  (Addr) )) &= ~( (reg8)  (Data))
#define ClearIO16(Addr,Data)    (*( (reg16*) (Addr) )) &= ~( (reg16) (Data))
#define ClearIO32(Addr,Data)    (*( (reg32*) (Addr) )) &= ~( (reg32) (Data))

typedef volatile uint8_t	reg8;   
typedef volatile uint16_t	reg16;  
typedef volatile uint32_t	reg32;

struct  GPIO_registers
{
	reg32 GPIOxOUT;						  ///< 0x00   : Output Register
	reg32 GPIOxOUTENB;       			  ///< 0x04   : Output Enable Register
	reg32 GPIOxDETMODE[2];   			  ///< 0x08   : Event Detect Mode Register
	reg32 GPIOxINTENB;       			  ///< 0x10   : Interrupt Enable Register
	reg32 GPIOxDET;          			  ///< 0x14   : Event Detect Register
	reg32 GPIOxPAD;          			  ///< 0x18   : PAD Status Register
	reg32 GPIOxPUENB;        			  ///< 0x1C   : Pull Up Enable Register
	reg32 GPIOxALTFN[2];     			  ///< 0x20   : Alternate Function Select Register
	reg32 GPIOxDETMODEEX;    			  ///< 0x28   : Event Detect Mode extended Register
	reg32 Reserved0[4];     			  ///< 0x2B   :
	reg32 GPIOxDETENB;       			  ///< 0x3C   : IntPend Detect Enable Register
	reg32 GPIOx_SLEW;                     ///< 0x40   : Slew Register
	reg32 GPIOx_SLEW_DISABLE_DEFAULT;     ///< 0x44   : Slew set On/Off Register
	reg32 GPIOx_DRV1;                     ///< 0x48   : drive strength LSB Register
	reg32 GPIOx_DRV1_DISABLE_DEFAULT;     ///< 0x4C   : drive strength LSB set On/Off Register
	reg32 GPIOx_DRV0;                     ///< 0x50   : drive strength MSB Register
	reg32 GPIOx_DRV0_DISABLE_DEFAULT;     ///< 0x54   : drive strength MSB set On/Off Register
	reg32 GPIOx_PULLSEL;                  ///< 0x58   : Pull UP/DOWN Selection Register
	reg32 GPIOx_PULLSEL_DISABLE_DEFAULT;  ///< 0x5C   : Pull UP/DOWN Selection On/Off Register
	reg32 GPIOx_PULLENB;                  ///< 0x60   : Pull Enable/Disable Register
	reg32 GPIOx_PULLENB_DISABLE_DEFAULT;  ///< 0x64   : Pull Enable/Disable selection On/Off Register
	reg32 GPIOx_InputMuxSelect0;          ///< 0x68
	reg32 GPIOx_InputMuxSelect1;          ///< 0x6C
	reg8  Reserved1[0x1000-0x70];
};

enum GPIO_X 
{
	GPIOA = 0, GPIOB, GPIOC, GPIOD, GPIOE
};

int fd_mem = -1;
struct GPIO_registers *GPIO_MAPPED_ADDR = NULL;
struct GPIO_registers *GPIO[GPIO_PAD_NUM] = {};
char logbuf[1024] = "";

//根据wiringPi管脚编号取得对应的物理管脚的编号
const int wiringPitoGPIO[] = {
	61, 58, 62, 63, 78, 59, 
	97, 60, -1, -1, 94, 77,
	95, 96, 93, 117, 113, -1,
	-1, -1, -1, 72, 73, 74, 
	76, 75, 92, 71, -1, -1, -1
};


void logger(char msg[])
{
	fprintf(stderr, "LOG:%s\n", msg);
}

void error(char msg[])
{
	fprintf(stderr, "ERR:%s errno:%s\n", msg, strerror(errno));
	exit(1);
}

void mem_open(void)
{
	fd_mem = open("/dev/mem", O_RDWR | O_NDELAY);
	if(-1 == fd_mem)
	{
		error("Can't open /dev/mem.");
	}

	GPIO_MAPPED_ADDR = mmap(0 , MMAP_SIZE_GPIO,
		   	PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, GPIO_PHY_BASE);
	sprintf(logbuf, "GPIO_MAPPED_ADDR:%p",  GPIO_MAPPED_ADDR);
	logger(logbuf);

	int offset = 0;
	for(offset = 0; offset < GPIO_PAD_NUM; offset++)
	{
		GPIO[offset] = (struct GPIO_registers*) ( (uint32_t) GPIO_MAPPED_ADDR + (offset * GPIO_PHY_BASE));
	}

}

void mem_close(void)
{
	if(-1 != fd_mem)
	{
		munmap( (void *) GPIO_MAPPED_ADDR, MMAP_SIZE_GPIO);
		close(fd_mem);
	}
	else
	{
		logger("can't munmap a unmapped memory");
	}
}

int main(int argc, char * argv[])
{
	mem_open();

	WriteIO32(GPIO[GPIOA], 0xaaaa);
	printf("ADDR:%p -> %x\n", GPIO[GPIOA], GPIO[GPIOA]->GPIOxOUT);
	WriteIO32(GPIO[GPIOA], 0xaaaaaa);
	printf("ADDR:%p -> %x\n", GPIO[GPIOA], GPIO[GPIOA]->GPIOxOUT);

	mem_close();

	return EXIT_SUCCESS;
}
