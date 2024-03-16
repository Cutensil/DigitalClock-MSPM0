#ifndef __OLED_H
#define __OLED_H			  	
#include "ti_msp_dl_config.h"				  				   

void OLED_WriteByte(uint8_t data,bool isCMD);
void OLED_Init();
void OLED_SetPos(uint8_t column, uint8_t page);
uint8_t OLED_ShowChar(uint8_t column, uint8_t page, unsigned char ch, bool border);
uint8_t OLED_ShowLargeChar(uint8_t column, uint8_t page, unsigned char ch);
void OLED_ShowString(uint8_t column, uint8_t page, char* str, bool border);
void OLED_ShowLargeString(uint8_t column, uint8_t page, char* str);
void OLED_ClearAll();
#endif