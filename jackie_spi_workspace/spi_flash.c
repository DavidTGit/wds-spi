#include "s3c24xx.h"
#include "s3c2440_spi.h"

static void SPIFlashSetCS(char val)
{
	// GPG2
	if (val)
		GPGDAT |=  (1 << 2);
	else
		GPGDAT &=  ~(1 << 2);

}

static void SPIFlashSendAddr(int addr)
{
	SPISendByte(addr >> 16);
	SPISendByte(addr >> 8);
	SPISendByte(addr & 0xFF);

}

void SPIFlashReadID(int *ManufactureId, int *DeviceId)
{
	SPIFlashSetCS(0); // SPI Flash Chip Select

	SPISendByte(0x90);
	SPIFlashSendAddr(0x00);
	*ManufactureId = SPIReceiveByte();
	*DeviceId      = SPIReceiveByte();
	
	SPIFlashSetCS(1); // 
}

void SPIFlashWriteEnable(int enable)
{
	if (enable)
	{
		SPIFlashSetCS(0);  // SPI Flash Chip Select
		SPISendByte(0x06); // Write Enable
		SPIFlashSetCS(1); 
	}else
	{
		SPIFlashSetCS(0);  // SPI Flash Chip Select
		SPISendByte(0x04); // Write Disable
		SPIFlashSetCS(1); 
	}
}

static unsigned char SPIFlashReadStatusReg1(void)
{
	unsigned char val;

	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x05); // 
	val = SPIReceiveByte();
	SPIFlashSetCS(1); 
	return val;
}

static unsigned char SPIFlashReadStatusReg2(void)
{
	unsigned char val;

	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x35); // 
	val = SPIReceiveByte();
	SPIFlashSetCS(1); 
	return val;
}

static void SPIFlashWaitWhenBusy(void)
{
	while(SPIFlashReadStatusReg1() & 0x01);
}

static void SPIFlashWriteStatusReg(unsigned char reg1, unsigned char reg2)
{
	SPIFlashWriteEnable(1);
	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x01); // 
	SPISendByte(reg1); // 
	SPISendByte(reg2); // 
	SPIFlashSetCS(1); 
	
	SPIFlashWaitWhenBusy();
}

static void SPIFlashClearProtectForStatusReg(void)
{
	unsigned char reg1, reg2;

	reg1 = SPIFlashReadStatusReg1();
	reg2 = SPIFlashReadStatusReg2();

	reg1 &= ~(1 < 7);
	reg2 &= ~(1 < 0);

	SPIFlashWriteStatusReg(reg1, reg2);

}

static void SPIFlashClearProtectForData(void)
{
	unsigned char reg1, reg2;

	reg1 = SPIFlashReadStatusReg1();
	reg2 = SPIFlashReadStatusReg2();

	reg1 &= ~(7 << 2);
	reg1 &= ~(1 << 6);
	SPIFlashWriteStatusReg(reg1, reg2);

}

void SPIFlashEraseSector(unsigned int addr)
{
	SPIFlashWriteEnable(1);
	
	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x20); // 
	SPIFlashSendAddr(addr);
	SPIFlashSetCS(1); 

	SPIFlashWaitWhenBusy();
}

void SPIFlashProgram(unsigned int addr, unsigned char *buf, int len)
{
	int i;
	
	SPIFlashWriteEnable(1);
	
	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x02); // 
	SPIFlashSendAddr(addr);

	for(i = 0; i < len; i++)
		SPISendByte(buf[i]);
		
	SPIFlashSetCS(1); 

	SPIFlashWaitWhenBusy();

}

void SPIFlashRead(unsigned int addr, unsigned char *buf, int len)
{
	int i;
	
	SPIFlashSetCS(0);  // SPI Flash Chip Select
	SPISendByte(0x03); // 
	SPIFlashSendAddr(addr);

	for(i = 0; i < len; i++)
		buf[i] = SPIReceiveByte();

	SPIFlashSetCS(1); 
}

void SPIFlashInit(void)
{
	SPIFlashClearProtectForStatusReg();
	SPIFlashClearProtectForData();
}

