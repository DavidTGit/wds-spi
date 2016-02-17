#include "s3c24xx.h"

void SPI_GPIO_Init(void)
{
	// GPF1 OLED_CS  output
	GPFCON &= ~( 3 << (1 * 2));
	GPFCON |= (1 << (1 * 2));
	GPFDAT |= (1 << 1);

	// GPG2 FLASH_CS output
	// GPG4 OLED_DC  output
	// GPG5 SPIMISO 
	// GPG6 SPIMOSI 
	// GPG7 SPICLK   
	GPGCON &= ~((3 << (2 * 2))|(3 << (4 * 2))|(3 << (5 * 2))|(3 << (6 * 2))|(3 << (7 * 2)));
	GPGCON |=  ((1 << (2 * 2))|(1 << (4 * 2))|(3 << (5 * 2))|(3 << (6 * 2))|(3 << (7 * 2)));
	GPGDAT |= (1 << 2);
}

static void SPIControllerInit(void)
{
	SPPRE0 = 2;
	SPPRE1 = 2;

	SPCON0 = (1 << 3) | ( 1 << 4);
	SPCON1 = (1 << 3) | ( 1 << 4);
	
}

void SPIInit(void)
{
	SPI_GPIO_Init();
	SPIControllerInit();
}

void SPISendByte(unsigned char val)
{
	while(!(SPSTA1 & 0x01));
	SPTDAT1 = val;
}

unsigned char SPIReceiveByte(void)
{
	SPTDAT1 = 0xFF;
	while(!(SPSTA1 & 0x01));
	return SPRDAT1;
}


