#include "s3c24xx.h"

void SPI_GPIO_Init(void)
{
	// GPF1 OLED_CS  output
	GPFCON &= ~( 3 << (1 * 2));
	GPFCON |= (1 << (1 * 2));
	GPFDAT |= (1 << 1);

	// GPG2 FLASH_CS output
	// GPG4 OLED_DC  output
	// GPG5 SPIMISO  input
	// GPG6 SPIMOSI  output
	// GPG7 SPICLK   output
	GPGCON &= ~((3 << (2 * 2))|(3 << (4 * 2))|(3 << (5 * 2))|(3 << (6 * 2))|(3 << (7 * 2)));
	GPGCON |=  ((1 << (2 * 2))|(1 << (4 * 2))|(0 << (5 * 2))|(1 << (6 * 2))|(1 << (7 * 2)));
	GPGDAT |= (1 << 2);
}

void SPIInit(void)
{
	SPI_GPIO_Init();
}

void SPISetCLK(char val)
{ 
	if (val)
	{
		GPGDAT |= (1 << 7);
	}else
	{
		GPGDAT &= ~(1 << 7);
	}
}

void SPISetDO(char val)
{
	if (val)
	{
		GPGDAT |= (1 << 6);
	}else
	{
		GPGDAT &= ~(1 << 6);
	}
	
}

void SPISendByte(unsigned char val)
{
	int i;

	for ( i = 0; i < 8; i++)
	{
		SPISetCLK(0);
        SPISetDO(val&0x80);
		SPISetCLK(1);
		val <<= 1;
	}
	
}



