#include "s3c24xx.h"
#include "gpio_spi.h"
#include "oledfont.h"

OLEDSetDC(char val)
{
   if(val)
   	{
		GPGDAT |= (1 << 4);	 // GPG4
   	}else
	{
		GPGDAT &= ~(1 << 4);
	}
}

OLEDSetCS(char val)
{
   if(val)
   	{
		GPFDAT |= (1 << 1);	 // GPF1
   	}else
	{
		GPFDAT &= ~(1 << 1);
	}
}

void OLEDWriteCmd(unsigned char cmd)
{
	OLEDSetDC(0); // 1 = Date, 0 = Command
	OLEDSetCS(0); // 1 = OLED Chip Select

	SPISendByte(cmd);

	OLEDSetCS(1); // 0 = Chip Select Disable
	OLEDSetDC(1); // 1 = Date, 0 = Command

}

void OLEDWriteData(unsigned char data)
{
	OLEDSetDC(1); // 1 = Date, 0 = Command
	OLEDSetCS(0); // 1 = OLED Chip Select

	SPISendByte(data);

	OLEDSetCS(1); // 0 = Chip Select Disable
	OLEDSetDC(0); // 1 = Date, 0 = Command

}

void OLEDSetPageAddrMode(void)
{
	OLEDWriteCmd(0x20);
	OLEDWriteCmd(0x02);
}

void OLEDSetPosition(int page, int col)
{
	OLEDWriteCmd(0xB0 + page);
	OLEDWriteCmd(col & 0x0F);
	OLEDWriteCmd(0x10 + (col >> 4));
}

void OLEDClear(void)
{
	int page, col;

	for (page = 0; page < 8; page++)
	{
		OLEDSetPosition(page, 0);
		for(col = 0; col < 128; col++)
			OLEDWriteData(0);
			
	}
}

void OLEDInit(void)
{
    /* 向OLED发命令以初始化 */
    OLEDWriteCmd(0xAE); /*display off*/ 
    OLEDWriteCmd(0x00); /*set lower column address*/ 
    OLEDWriteCmd(0x10); /*set higher column address*/ 
    OLEDWriteCmd(0x40); /*set display start line*/ 
    OLEDWriteCmd(0xB0); /*set page address*/ 
    OLEDWriteCmd(0x81); /*contract control*/ 
    OLEDWriteCmd(0x66); /*128*/ 
    OLEDWriteCmd(0xA1); /*set segment remap*/ 
    OLEDWriteCmd(0xA6); /*normal / reverse*/ 
    OLEDWriteCmd(0xA8); /*multiplex ratio*/ 
    OLEDWriteCmd(0x3F); /*duty = 1/64*/ 
    OLEDWriteCmd(0xC8); /*Com scan direction*/ 
    OLEDWriteCmd(0xD3); /*set display offset*/ 
    OLEDWriteCmd(0x00); 
    OLEDWriteCmd(0xD5); /*set osc division*/ 
    OLEDWriteCmd(0x80); 
    OLEDWriteCmd(0xD9); /*set pre-charge period*/ 
    OLEDWriteCmd(0x1f); 
    OLEDWriteCmd(0xDA); /*set COM pins*/ 
    OLEDWriteCmd(0x12); 
    OLEDWriteCmd(0xdb); /*set vcomh*/ 
    OLEDWriteCmd(0x30); 
    OLEDWriteCmd(0x8d); /*set charge pump enable*/ 
    OLEDWriteCmd(0x14); 

    OLEDSetPageAddrMode();

    OLEDClear();
    
    OLEDWriteCmd(0xAF); /*display ON*/    
}

void OLEDPutChar(int page, int col, char chr)
{
	int i;
	unsigned char *dots = oled_asc2_8x16[chr - ' '];

	OLEDSetPosition(page, col);
	for (i = 0; i < 8; i++)
	{
		OLEDWriteData(dots[i]);
	}

	
	OLEDSetPosition(page+1, col);
	for (i = 0; i < 8; i++)
	{
		OLEDWriteData(dots[i+8]);
	}

}

void OLEDPrint(int page, int col, char *str)
{
	int i = 0;
	while(str[i])
	{
		OLEDPutChar(page, col, str[i]);
		col  += 8;
		if(col > 127)
		{
			col = 0;
			page += 2;
		}
		
		i++;
	}
}


