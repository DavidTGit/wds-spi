
#ifndef _OLED_H
#define _OLED_H

void OLEDWriteCmd(unsigned char cmd);
void OLEDSetPageAddrMode(void);
void OLEDClear(void);
void OLEDInit(void);
void OLEDPrint(int page, int col, char *str);

#endif
