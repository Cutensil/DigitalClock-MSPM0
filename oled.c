#include "oled.h"
#include "oledfont.h"

uint8_t OLED_ShowChar(uint8_t column, uint8_t page, unsigned char ch, bool border)
{
	OLED_SetPos(column, page);
	if (ch >= '0' && ch <= '9')
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (i == 8)
			{
				OLED_SetPos(column, page + 1);
			}
			if (border)
			{
				if (i < 8)
				{
					OLED_WriteByte(number[ch - '0'][i] | 0x01, 0);
				}
				else
				{
					OLED_WriteByte(number[ch - '0'][i] | 0x80, 0);
				}
			}
			else
			{
				OLED_WriteByte(number[ch - '0'][i], 0);
			}
		}
		return 8;
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (i == 8)
			{
				OLED_SetPos(column, page + 1);
			}
			if (border)
			{
				if (i < 8)
				{
					OLED_WriteByte(lowercase_letter[ch - 'a'][i] | 0x01, 0);
				}
				else
				{
					OLED_WriteByte(lowercase_letter[ch - 'a'][i] | 0x80, 0);
				}
			}
			else
			{
				OLED_WriteByte(lowercase_letter[ch - 'a'][i], 0);
			}
		}
		return 8;
	}
	else if (ch >= 'A' && ch <= 'Z')
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (i == 8)
			{
				OLED_SetPos(column, page + 1);
			}
			if (border)
			{
				if (i < 8)
				{
					OLED_WriteByte(uppercase_letter[ch - 'A'][i] | 0x01, 0);
				}
				else
				{
					OLED_WriteByte(uppercase_letter[ch - 'A'][i] | 0x80, 0);
				}
			}
			else
			{
				OLED_WriteByte(uppercase_letter[ch - 'A'][i], 0);
			}
		}
		return 8;
	}
	else if (ch == ':')
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			if (i == 4)
			{
				OLED_SetPos(column, page + 1);
			}
			if (border)
			{
				if (i < 4)
				{
					OLED_WriteByte(number[10][i] | 0x01, 0);
				}
				else
				{
					OLED_WriteByte(number[10][i] | 0x80, 0);
				}
			}
			else
			{
				OLED_WriteByte(number[10][i], 0);
			}
		}
		return 4;
	}
	else if (ch == '/')
	{
		for (uint8_t i = 0; i < 16; i++)
		{
			if (i == 8)
			{
				OLED_SetPos(column, page + 1);
			}
			if (border)
			{
				if (i < 8)
				{
					OLED_WriteByte(number[11][i] | 0x01, 0);
				}
				else
				{
					OLED_WriteByte(number[11][i] | 0x80, 0);
				}
			}
			else
			{
				OLED_WriteByte(number[11][i], 0);
			}
		}
		return 8;
	}
	else
	{
		char index = 0;
		switch (ch)
		{
			case '!':
				index = 1;
				break;
			case '@':
				index = 2;
				break;
			case '#':
				index = 3;
				break;
			case '$':
				index = 4;
				break;
			case '%':
				index = 5;
				break;
			case '^':
				index = 6;
				break;
			case '+':
				index = 7;
				break;
			case '-':
				index = 8;
				break;
			case '.':
				index = 9;
				break;
			default:
				index = 0;
				break;
		}
		if (index > 0)
		{
			for (uint8_t i = 0; i < 16; i++)
			{
				if (i == 8)
				{
					OLED_SetPos(column, page + 1);
				}
				if (border)
				{
					if (i < 8)
					{
						OLED_WriteByte(symbol[index - 1][i] | 0x01, 0);
					}
					else
					{
						OLED_WriteByte(symbol[index - 1][i] | 0x80, 0);
					}
				}
				else
				{
					OLED_WriteByte(symbol[index - 1][i], 0);
				}
			}
			return 8;
		}
		else
		{
			if (border)
			{
				for (uint8_t i = 0; i < 8; i++)
				{
					if (i == 4)
					{
						OLED_SetPos(column, page + 1);
					}
					if (i < 4)
					{
						OLED_WriteByte(0x01, 0);
					}
					else
					{
						OLED_WriteByte(0x80, 0);
					}
				}
			}
			else
			{
				for (uint8_t i = 0; i < 8; i++)
				{
					if (i == 4)
					{
						OLED_SetPos(column, page + 1);
					}
					OLED_WriteByte(0x00, 0);
				}
			}
			return 4;
		}
	}
	return 0;
}

uint8_t OLED_ShowLargeChar(uint8_t column, uint8_t page, unsigned char ch)
{
	if (ch >= '0' && ch <= '9')
	{
		for (uint8_t i = 0; i < 64; i++)
		{
			if (i % 16 == 0)
			{
				OLED_SetPos(column, page + i / 16);
			}
			OLED_WriteByte(large_number[ch - '0'][i], 0);
		}
		return 16;
	}
	else if (ch == ':')
	{
		for (uint8_t i = 0; i < 32; i++)
		{
			if (i % 8 == 0)
			{
				OLED_SetPos(column, page + i / 8);
			}
			OLED_WriteByte(large_number[10][i], 0);
		}
		return 8;
	}
	else if (ch == 'A')
	{
		for (uint8_t i = 0; i < 32; i++)
		{
			if (i % 16 == 0)
			{
				OLED_SetPos(column, page + i / 16);
			}
			OLED_WriteByte(large_number[11][i], 0);
		}
		return 16;
	}
	else if (ch == 'T')
	{
		for (uint8_t i = 0; i < 32; i++)
		{
			if (i % 16 == 0)
			{
				OLED_SetPos(column, page + i / 16);
			}
			OLED_WriteByte(large_number[12][i], 0);
		}
		return 16;
	}
	else
	{
		for (uint8_t i = 0; i < 32; i++)
		{
			if (i % 8 == 0)
			{
				OLED_SetPos(column, page + i / 8);
			}
			OLED_WriteByte(0x00, 0);
		}
		return 16;
	}
	return 16;
}

void OLED_ShowString(uint8_t column, uint8_t page, char* str, bool border)
{
	uint8_t i = 0;
	uint8_t pos = column;
	while (str[i] != 0)
	{
		pos += OLED_ShowChar(pos, page, str[i], border);
		i++;
	}
}

void OLED_ShowLargeString(uint8_t column, uint8_t page, char* str)
{
	uint8_t i = 0;
	uint8_t pos = column;
	while (str[i] != 0)
	{
		pos += OLED_ShowLargeChar(pos, page, str[i]);
		i++;
	}
}

void OLED_WriteByte(uint8_t data, bool isCMD)
{
	if (isCMD)
	{
		DL_GPIO_clearPins(OLED_PORT, OLED_CMD_PIN);
	}
	else
	{
		DL_GPIO_setPins(OLED_PORT, OLED_CMD_PIN);
	}
	DL_GPIO_clearPins(OLED_PORT, OLED_SELECT_PIN);
	for (uint8_t i = 0; i < 8; i++)
	{
		DL_GPIO_clearPins(OLED_PORT, OLED_CLK_PIN);
		if (data & 0x80)
		{
			DL_GPIO_setPins(OLED_PORT, OLED_INPUT_PIN);
		}
		else
		{
			DL_GPIO_clearPins(OLED_PORT, OLED_INPUT_PIN);
		}
		DL_GPIO_setPins(OLED_PORT, OLED_CLK_PIN);
		data <<= 1;
	}
	DL_GPIO_setPins(OLED_PORT, OLED_SELECT_PIN);
}

void OLED_SetPos(uint8_t column, uint8_t page) //column: 0-127; page: 0-7
{
	OLED_WriteByte(0x00 | (column & 0x0F), 1);
	OLED_WriteByte(0x10 | (column & 0xF0) >> 4, 1);
	OLED_WriteByte(0xB0 | page, 1);
}

void OLED_ClearAll()
{
	for (uint8_t y = 0; y < 8; y++)
	{
		OLED_SetPos(0, y);
		for (uint8_t x = 0; x < 128; x++)
		{
			OLED_WriteByte(0, 0);
		}
	}
}

void OLED_Init(void)
{
	delay_cycles(CPUCLK_FREQ / 10);
	
	OLED_WriteByte(0xAE, 1); //display off
	
	OLED_WriteByte(0x00, 1); //1. set lower column start address
	
	OLED_WriteByte(0x10, 1); //2. set higher column start address
	
	OLED_WriteByte(0x20, 1); //3. set memory addressing mode
	OLED_WriteByte(2, 1); //page addressing mode
	
	//4. set column address

	//5. set page address
	
	OLED_WriteByte(0x40, 1); //6. set display start line
	
	OLED_WriteByte(0x81, 1); //7. set contract control
	OLED_WriteByte(200, 1);
	
	OLED_WriteByte(0xA1, 1); //8. set segment re-map

	OLED_WriteByte(0xA4, 1); //9. set entire display off
	
	OLED_WriteByte(0xA6, 1); //10. set not reverse (0=off, 1=on)
	
	//11. set multiplex ratio
	//default: 63
	
	//12. set display on/off
	
	OLED_WriteByte(0xB0, 1); //13. set page start address
	
	OLED_WriteByte(0xC8, 1); //14. set COM output scan direction
	
	OLED_WriteByte(0xD3, 1); //15. set display offset
	OLED_WriteByte(0, 1);
	
	OLED_WriteByte(0xD5, 1); //16. set osc division
	OLED_WriteByte(128, 1);
	
	OLED_WriteByte(0xD9, 1); //17. set pre-charge period
	OLED_WriteByte(241, 1);
	
	OLED_WriteByte(0xDA, 1); //18. set COM pins hardware configuration
	OLED_WriteByte(18, 1);
	
	OLED_WriteByte(0xDB, 1); //19. set V_comh deselect level
	OLED_WriteByte(64, 1);

	OLED_WriteByte(0x8D, 1); //EX. set charge pump enable
	OLED_WriteByte(0x14, 1);

	OLED_WriteByte(0xAF, 1); //display on
	delay_cycles(CPUCLK_FREQ / 10);
	OLED_ClearAll();
}
