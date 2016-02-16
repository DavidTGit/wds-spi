#include <stdio.h>
#include "serial.h"
#include "i2c.h"
#include "gpio_spi.h"
#include "oled.h"
#include "spi_flash.h"

unsigned char at24cxx_read(unsigned char address);
void at24cxx_write(unsigned char address, unsigned char data);


int main()
{
    char c;
    char str[200];
    int i;
	int address;
	int data;
    unsigned int mid, pid;
	
    uart0_init();   // 波特率115200，8N1(8个数据位，无校验位，1个停止位)

	SPIInit();
	OLEDInit();
	OLEDPrint(0, 0, "Hello World SPI!");

	SPIFlashReadID(&mid, &pid);
	printf("SPI FLASH : Mid = 0x%2x, Pid = 0x%2x\n\r", mid, pid);

	sprintf(str, "SPI FLASH: %2x,%2x", mid, pid);
	OLEDPrint(2, 0, str);

	SPIFlashInit();
	SPIFlashEraseSector(4096);
	SPIFlashEraseSector(1024);
	SPIFlashProgram(4096, "Happy NEW YEAR !", 20);
	SPIFlashProgram(1024, "Chinese New Year", 20);

	SPIFlashRead(1024, str, 18);
	printf("SPI Flash read from 1024: %s\n\r", str);
//	OLEDPrint(4, 0, str);
	
	SPIFlashRead(4096, str, 18);
	printf("SPI Flash read from 4096: %s\n\r", str);
	//OLEDPrint(6, 0, str);

	Test_Adc();
	i2c_init();
	at24cxx_write(0,0x11);
	data = at24cxx_read(0);
	if (data == 0x11)
	    OLEDPrint(4, 0, "I2C OK!!!!");
	else
	    OLEDPrint(4, 0, "I2C ERROR!");	
	    
    return 0;
}
