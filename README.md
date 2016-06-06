# wiringNPi
wiringNPi, a GPIO library for friendly arm nano pi 2, which featured samsung s5p4418 soc.



# 中文HOWTO

## 提示
理论上库适用于所有使用友善之臂s5p4418芯片的板子。
PWM依赖于``matrix_pwm``内核模块，首先开机加载该模块。
编辑``/etc/modules``，添加如下内容。
```
matrix_pwm
```

## 安装前
只需要安装基本的编译工具即可
```
# apt-get install build-essential
```

## 编译以及安装
进入源码目录
```
make
make install
```

## 开始使用wiringNPi
要使用wiringNPi库，只需要在你的代码中``#include <wiringNPi.h>``，编译的时候链接``wiringNPi``库，即编译的时候加上``-lwiringNPi``参数。如
```
gcc main.c -lwiringNPi -o main
```

## demo
```
#include <stdio.h>
#include <stdlib.h>
#include <wiringNPi.h>
#include <wiringNPiSPI.h>


int main(int argc, char * argv[])
{
	int i = 0;
	//You must setup first.
	wiringNPiSetup();

	//GPIO RW
	pinMode(GPIOC(12), OUTPUT);

	for(i = 0; i < 100000; i++)
	{
		digitalWrite(GPIOC(12), HIGH);
		digitalWrite(GPIOC(12), LOW);
	}

	//PWM R/W
	analogWrite(PWM0, 1000000, 200);
	delay(500);

	analogWrite(PWM1, 1000000, 900);

	//You can delay in ms
	delay(500);

	//You can also delay in us
	delayMicroseconds (1000);


	//Stop pwm.
	analogStop(PWM0);
	analogStop(PWM1);



	//Setup SPI first.
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
```
