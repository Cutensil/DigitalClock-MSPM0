#include "oled.h"
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define PAGE10_INDEX 3
#define PAGE20_INDEX 3
#define PAGE22_INDEX 3
#define PAGE220_INDEX 3
#define PAGE222_INDEX 3
#define DATA_BUFFER_SIZE 12
#define CONFIG_ADDRESS (0x9000)
#define TIME_ADDRESS (0x7500)
#define ALARM_ADDRESS (0x9100)

uint8_t current_page = 0;
uint8_t mode = 0;
uint8_t previous_page = 0;
struct tm* local_time;
time_t current_time = 0;
time_t received_time = 0;
struct tm time_temp;
uint8_t cursor = 0;
uint8_t alarm_target = 0;
uint8_t page_before_alarm = 0;
uint8_t page_before_data = 0;
uint8_t countdown_h_temp = 0;
uint8_t countdown_m_temp = 0;
uint8_t countdown_s_temp = 0;
bool ring_flag = 0;
uint16_t ring_delay = 0;
char data_temp[12] = {0};
char data_receive = 0;
bool data_flag = 0;
char flash_temp[8] = {0};

struct Alarm
{
	bool enable;
	bool null;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t mode; //0: Once; 1: Daily; 2: Custom
	uint8_t month;
	uint8_t day;
}alarm[3], alarm_temp;

struct Config
{
	bool mode_12h;
	uint8_t countdown_init_h;
	uint8_t countdown_init_m;
	uint8_t countdown_init_s;
}config;

struct Timer
{
	bool enable;
	uint16_t s;
	uint8_t ms;
}timer;

struct Countdown
{
	bool enable;
	uint16_t h;
	uint16_t m;
	uint16_t s;
}countdown;

void SaveConfig()
{
	struct Config config_temp[2];
	memcpy(config_temp, &config, sizeof(struct Config));
	DL_FlashCTL_unprotectSector(FLASHCTL, CONFIG_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
	DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, CONFIG_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
	DL_FlashCTL_programMemoryFromRAM(FLASHCTL, CONFIG_ADDRESS, (uint32_t *)config_temp, sizeof(struct Config), DL_FLASHCTL_REGION_SELECT_MAIN);
	DL_FlashCTL_programMemoryFromRAM(FLASHCTL, ALARM_ADDRESS, (uint32_t *)alarm, 3 * sizeof(struct Alarm), DL_FLASHCTL_REGION_SELECT_MAIN);
}

void LoadConfig()
{
	memcpy(&config, (struct Config *)CONFIG_ADDRESS, sizeof(struct Config));
	memcpy(alarm, (struct Alarm *)ALARM_ADDRESS, 3 * sizeof(struct Alarm));
}

void SaveTime()
{
	DL_FlashCTL_unprotectSector(FLASHCTL, TIME_ADDRESS, DL_FLASHCTL_REGION_SELECT_MAIN);
	DL_FlashCTL_eraseMemoryFromRAM(FLASHCTL, TIME_ADDRESS, DL_FLASHCTL_COMMAND_SIZE_SECTOR);
	DL_FlashCTL_programMemoryFromRAM(FLASHCTL, TIME_ADDRESS, &current_time, sizeof(current_time), DL_FLASHCTL_REGION_SELECT_MAIN);
}

void LoadTime()
{
	current_time = *((time_t *)TIME_ADDRESS);
}

uint8_t GetMaxDay(uint8_t year, uint8_t month)
{
	if (month + 1 == 1 || month + 1 == 3 ||
			month + 1 == 5 || month + 1 == 7 ||
			month + 1 == 8 || month + 1 == 10 ||
			month + 1 == 12)
	{
		return 31;
	}
	else if (month + 1 == 2)
	{
		if ((year + 1900) % 4 == 0)
		{
			if ((year + 1900) % 100 == 0)
			{
				if ((year + 1900) % 400 == 0)
				{
					return 29;
				}
				else
				{
					return 28;
				}
			}
			else
			{
				return 29;
			}
		}
		else
		{
			return 28;
		}
	}
	else
	{
		return 30;
	}
}

char GetInput(bool* flag)
{
	const uint16_t delay_time = 10000;
	uint8_t input = 0;
	DL_GPIO_setPins(PAD_PORT, PAD_V1_PIN);
	DL_GPIO_setPins(PAD_PORT, PAD_V2_PIN);
	DL_GPIO_setPins(PAD_PORT, PAD_V3_PIN);
	DL_GPIO_setPins(PAD_PORT, PAD_V4_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);

	//S1-S4
	DL_GPIO_clearPins(PAD_PORT, PAD_V1_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
	if(!DL_GPIO_readPins(PAD_PORT, PAD_H1_PIN))
	{
		input = 1;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H2_PIN))
	{
		input = 2;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H3_PIN))
	{
		input = 3;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H4_PIN))
	{
		input = 4;
	}
	DL_GPIO_setPins(PAD_PORT, PAD_V1_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
			
	//S5-S8
	DL_GPIO_clearPins(PAD_PORT, PAD_V2_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
	if(!DL_GPIO_readPins(PAD_PORT, PAD_H1_PIN))
	{
		input = 5;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H2_PIN))
	{
		input = 6;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H3_PIN))
	{
		input = 7;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H4_PIN))
	{
		input = 8;
	}
	DL_GPIO_setPins(PAD_PORT, PAD_V2_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);

	//S9-S12
	DL_GPIO_clearPins(PAD_PORT, PAD_V3_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
	if(!DL_GPIO_readPins(PAD_PORT, PAD_H1_PIN))
	{
		input = 9;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H2_PIN))
	{
		input = 10;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H3_PIN))
	{
		input = 11;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H4_PIN))
	{
		input = 12;
	}
	DL_GPIO_setPins(PAD_PORT, PAD_V3_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
			
	//S13-S16
	DL_GPIO_clearPins(PAD_PORT, PAD_V4_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
	if(!DL_GPIO_readPins(PAD_PORT, PAD_H1_PIN))
	{
		input = 13;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H2_PIN))
	{
		input = 14;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H3_PIN))
	{
		input = 15;
	}
	else if(!DL_GPIO_readPins(PAD_PORT, PAD_H4_PIN))
	{
		input = 16;
	}
	DL_GPIO_setPins(PAD_PORT, PAD_V4_PIN);
	delay_cycles(CPUCLK_FREQ / delay_time);
	if(!*flag)
	{
		if(input != 0)
		{
			*flag = 1;
		}
		return input;
	}
	else if(input == 0)
	{
		*flag = 0;
		return 0;
	}
	return 0;
}

void Refresh()
{
	char time_str[15] = {0};
	if (current_page != previous_page)
	{
		OLED_ClearAll();
		previous_page = current_page;
	}
	switch (current_page)
	{
		case 0:
			if (config.mode_12h)
			{
				if (local_time->tm_hour > 12)
				{
					sprintf(time_str, "%02d:%02d", local_time->tm_hour - 12, local_time->tm_min);
					OLED_ShowString(86, 0, "PM", 0);
				}
				else
				{
					sprintf(time_str, "%02d:%02d", local_time->tm_hour, local_time->tm_min);
					if (local_time->tm_hour == 12)
					{
						OLED_ShowString(86, 0, "PM", 0);
					}
					else
					{
						OLED_ShowString(86, 0, "AM", 0);
					}
				}
			}
			else
			{
				sprintf(time_str, "%02d:%02d", local_time->tm_hour, local_time->tm_min);
			}
			OLED_ShowLargeString(10, 0, time_str);
			sprintf(time_str, "%02d", local_time->tm_sec);
			OLED_ShowString(86, 2, time_str, 0);
			for (uint8_t i = 0; i < 3; i++)
			{
				if (alarm[i].enable)
				{
					OLED_ShowLargeChar(104, 0, 'A');
					break;
				}
			}
			if (timer.enable || countdown.enable)
			{
				OLED_ShowLargeChar(104, 2, 'T');
			}
			switch (local_time->tm_wday)
			{
				case 0:
					sprintf(time_str, "%04d/%02d/%02d SUN", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 1:
					sprintf(time_str, "%04d/%02d/%02d MON", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 2:
					sprintf(time_str, "%04d/%02d/%02d TUE", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 3:
					sprintf(time_str, "%04d/%02d/%02d WED", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 4:
					sprintf(time_str, "%04d/%02d/%02d THU", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 5:
					sprintf(time_str, "%04d/%02d/%02d FRI", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
				case 6:
					sprintf(time_str, "%04d/%02d/%02d SAT", local_time->tm_year + 1900, local_time->tm_mon + 1, local_time->tm_mday);
					break;
			}
			OLED_ShowString(10, 4, time_str, 0);
			OLED_ShowString(10, 6, "S1:Mode S9:Menu", 0);
			break;
		case 1:
			sprintf(time_str, "%-5d", timer.s);
			OLED_ShowLargeString(10, 0, time_str);
			sprintf(time_str, ".%d", timer.ms);
			OLED_ShowString(102, 2, time_str, 0);
			OLED_ShowString(24, 4, "S5:S/P S6:R", 0);
			OLED_ShowString(10, 6, "S1:Mode S9:Menu", 0);
			break;
		case 2:
			sprintf(time_str, "%02d:%02d", countdown.h - 1, countdown.m - 1);
			OLED_ShowLargeString(18, 0, time_str);
			sprintf(time_str, "%02d", countdown.s - 1);
			OLED_ShowString(94, 2, time_str, 0);
			OLED_ShowString(2, 4, "S5:S/P S6:R S7:Set", 0);
			OLED_ShowString(10, 6, "S1:Mode S9:Menu", 0);
			break;
		case 3:
			OLED_ShowLargeChar(104, 4, 'A');
			sprintf(time_str, "%02d:%02d:%02d", local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
			OLED_ShowLargeString(8, 0, time_str);
			OLED_ShowString(2, 4, "S1:Close", 0);
			OLED_ShowString(2, 6, "S9:Ring 5m later", 0);
			break;
		case 4:
			OLED_ShowString(2, 0, "Received Data", 0);
			sprintf(time_str, "%04d/%02d/%02d", time_temp.tm_year + 1900, time_temp.tm_mon + 1, time_temp.tm_mday);
			OLED_ShowString(2, 2, time_str, 0);
			sprintf(time_str, "%02d:%02d:%02d", time_temp.tm_hour, time_temp.tm_min, time_temp.tm_sec);
			OLED_ShowString(2, 4, time_str, 0);
			OLED_ShowString(2, 6, "S1:! S9:@", 0);
			break;
		case 29:
			sprintf(time_str, "%02d", countdown_h_temp);
			OLED_ShowString(2, 0, time_str, cursor == 0);
			OLED_ShowChar(18, 0, ':', 0);
		
			sprintf(time_str, "%02d", countdown_m_temp);
			OLED_ShowString(22, 0, time_str, cursor == 1);
			OLED_ShowChar(38, 0, ':', 0);
		
			sprintf(time_str, "%02d", countdown_s_temp);
			OLED_ShowString(42, 0, time_str, cursor == 2);
		
			OLED_ShowString(2, 4, "S5:+ S6:- S1:! S9:@", 0);
			OLED_ShowString(2, 6, "S4:% S12:^", 0);
			break;
		case 10:
			OLED_ShowString(2, 0, "Clock", cursor == 0);
			OLED_ShowString(2, 2, "Stopwatch", cursor == 1);
			OLED_ShowString(2, 4, "Countdown", cursor == 2);
			OLED_ShowString(2, 6, "S7:# S8:$ S1:! S9:@", 0);
			break;
		case 20:
			OLED_ShowString(2, 0, "Time Setting", cursor == 0);
			OLED_ShowString(2, 2, "Alarm Setting", cursor == 1);
			OLED_ShowString(2, 4, "12/24h", cursor == 2);
			if (config.mode_12h)
			{
				OLED_ShowString(58, 4, "12", 0);
			}
			else
			{
				OLED_ShowString(58, 4, "24", 0);
			}
			OLED_ShowString(2, 6, "S7:# S8:$ S1:! S9:@", 0);
			break;
		case 22:
			for (uint8_t i = 0; i < 3; i++)
			{
				if (alarm[i].null)
				{
					OLED_ShowString(2, i * 2, "Not Set", cursor == i);
				}
				else
				{
					sprintf(time_str, "%02d:%02d:%02d", alarm[i].hour, alarm[i].minute, alarm[i].second);
					OLED_ShowString(2, i * 2, time_str, cursor == i);
					if (alarm[i].mode == 0)
					{
						sprintf(time_str, "%02d/%02d", alarm[i].month + 1, alarm[i].day);
						OLED_ShowString(62, i * 2, time_str, 0);
					}
					else if (alarm[i].mode == 1)
					{
						OLED_ShowString(62, i * 2, "Daily", 0);
					}
					else
					{
						OLED_ShowString(62, i * 2, "Custom", 0);
					}
				}
				if (alarm[i].enable)
				{
					OLED_ShowChar(114, i * 2, '!', 0);
				}
				else
				{
					OLED_ShowChar(114, i * 2, '@', 0);
				}
			}
			OLED_ShowString(2, 6, "S7:# S8:$ S1:! S9:@", 0);
			break;
		case 21:
			if (config.mode_12h)
			{
				if (time_temp.tm_hour > 12)
				{
					sprintf(time_str, "%02d", time_temp.tm_hour - 12);
					OLED_ShowString(64, 0, "PM", 0);
				}
				else
				{
					sprintf(time_str, "%02d", time_temp.tm_hour);
					if (time_temp.tm_hour == 12)
					{
						OLED_ShowString(64, 0, "PM", 0);
					}
					else
					{
						OLED_ShowString(64, 0, "AM", 0);
					}
				}
			}
			else
			{
				sprintf(time_str, "%02d", time_temp.tm_hour);
			}
			OLED_ShowString(2, 0, time_str, cursor == 0);
			OLED_ShowChar(18, 0, ':', 0);
		
			sprintf(time_str, "%02d", time_temp.tm_min);
			OLED_ShowString(22, 0, time_str, cursor == 1);
			OLED_ShowChar(38, 0, ':', 0);
		
			sprintf(time_str, "%02d", time_temp.tm_sec);
			OLED_ShowString(42, 0, time_str, cursor == 2);
		
			sprintf(time_str, "%02d", time_temp.tm_year + 1900);
			OLED_ShowString(2, 2, time_str, cursor == 3);
			OLED_ShowChar(34, 2, '/', 0);
		
			sprintf(time_str, "%02d", time_temp.tm_mon + 1);
			OLED_ShowString(42, 2, time_str, cursor == 4);
			OLED_ShowChar(58, 2, '/', 0);
		
			sprintf(time_str, "%02d", time_temp.tm_mday);
			OLED_ShowString(64, 2, time_str, cursor == 5);
			
			OLED_ShowString(2, 4, "S5:+ S6:- S1:! S9:@", 0);
			OLED_ShowString(2, 6, "S7/8:#$ S4/12:%^", 0);
			break;
		case 221:
			if (config.mode_12h)
			{
				if (alarm_temp.hour > 12)
				{
					sprintf(time_str, "%02d", alarm_temp.hour - 12);
					OLED_ShowString(62, 0, "P", 0);
				}
				else
				{
					sprintf(time_str, "%02d", alarm_temp.hour);
					if (alarm_temp.hour == 12)
					{
						OLED_ShowString(62, 0, "P", 0);
					}
					else
					{
						OLED_ShowString(62, 0, "A", 0);
					}
				}
			}
			else
			{
				sprintf(time_str, "%02d", alarm_temp.hour);
			}
			OLED_ShowString(2, 0, time_str, cursor == 0);
			OLED_ShowChar(18, 0, ':', 0);
		
			sprintf(time_str, "%02d", alarm_temp.minute);
			OLED_ShowString(22, 0, time_str, cursor == 1);
			OLED_ShowChar(38, 0, ':', 0);
		
			sprintf(time_str, "%02d", alarm_temp.second);
			OLED_ShowString(42, 0, time_str, cursor == 2);
		 	if (alarm_temp.mode == 0)
			{
				sprintf(time_str, "%02d", alarm_temp.month + 1);
				OLED_ShowString(77, 0, time_str, cursor == 3);
				OLED_ShowChar(93, 0, '/', 0);
		
				sprintf(time_str, "%02d", alarm_temp.day);
				OLED_ShowString(101, 0, time_str, cursor == 4);	
			}
			else if (alarm_temp.mode == 1);
			else
			{
				OLED_ShowString(77, 0, "Mo", cursor == 3);
				OLED_ShowString(102, 0, "Tu", cursor == 4);
				OLED_ShowString(2, 2, "We", cursor == 5);
				OLED_ShowString(27, 2, "Th", cursor == 6);
				OLED_ShowString(52, 2, "Fr", cursor == 7);
				OLED_ShowString(77, 2, "Sa", cursor == 8);
				OLED_ShowString(102, 2, "Su", cursor == 9);
				for (uint8_t i = 0; i < 7; i++)
				{
					if ((1 << i) & alarm_temp.mode)
					{
						if (i < 2)
						{
							OLED_ShowChar(93 + i * 25, 0, '!', 0);
						}
						else
						{
							OLED_ShowChar(18 + (i - 2) * 25, 2, '!', 0);
						}
					}
					else
					{
						if (i < 2)
						{
							OLED_ShowChar(93 + i * 25, 0, '@', 0);
						}
						else
						{
							OLED_ShowChar(18 + (i - 2) * 25, 2, '@', 0);
						}
					}
				}
			}
			OLED_ShowString(2, 4, "S5:+ S6:- S1:! S9:@", 0);
			OLED_ShowString(2, 6, "S7/8:#$ S4/12:%^", 0);
			break;
		case 220:
			OLED_ShowString(2, 0, "Set Time", cursor == 0);
			OLED_ShowString(2, 2, "Choose Mode", cursor == 1);
			OLED_ShowString(2, 4, "Enable/Disable", cursor == 2);
			if (alarm[alarm_target].enable)
			{
				OLED_ShowChar(118, 4, '!', 0);
			}
			else
			{
				OLED_ShowChar(118, 4, '@', 0);
			}
			OLED_ShowString(2, 6, "S7:# S8:$ S1:! S9:@", 0);
			break;
		case 222:
			OLED_ShowString(2, 0, "Once", cursor == 0);
			OLED_ShowString(2, 2, "Daily", cursor == 1);
			OLED_ShowString(2, 4, "Custom", cursor == 2);
			OLED_ShowString(2, 6, "S7:# S8:$ S1:! S9:@", 0);
			break;
	}
}

void Ring()
{
	if (current_page == 3)
	{
		DL_TimerG_stopCounter(TIMG4);
		NVIC_ClearPendingIRQ(TIMG4_INT_IRQn);
		DL_TimerG_startCounter(TIMG4);
	}
	else
	{
		ring_flag = 1;
		ring_delay = 0;
		NVIC_ClearPendingIRQ(TIMG4_INT_IRQn);
		NVIC_EnableIRQ(TIMG4_INT_IRQn);
		DL_TimerG_startCounter(TIMG4);
		page_before_alarm = current_page;
		current_page = 3;
	}
	Refresh();
}

void StopRing()
{
	DL_GPIO_clearPins(BUZZER_PORT, BUZZER_RING_PIN);
	NVIC_DisableIRQ(TIMG4_INT_IRQn);
	DL_TimerG_stopCounter(TIMG4);
	current_page = page_before_alarm;
	Refresh();
}

int main()
{
	bool flag = 0;
	SYSCFG_DL_init();
	OLED_Init();
	NVIC_ClearPendingIRQ(TIMG0_INT_IRQn);
	NVIC_EnableIRQ(TIMG0_INT_IRQn);
	DL_TimerG_startCounter(TIMG0);
	NVIC_ClearPendingIRQ(UART0_INT_IRQn);
	NVIC_EnableIRQ(UART0_INT_IRQn);
	for (uint8_t i = 0; i < 3; i++)
	{
		alarm[i].null = 1;
	}
	config.mode_12h = 0;
	config.countdown_init_h = 0;
	config.countdown_init_m = 3;
	config.countdown_init_s = 0;
	
	LoadTime();
	
	if (!DL_GPIO_readPins(SWITCH_PORT, SWITCH_RESET_PIN))
	{
		current_page = 40;
		OLED_ShowString(2, 0, "Reset Settings", 0);
		OLED_ShowString(2, 6, "S1:! S9:@", 0);
		bool input_flag = 0;
		while (!input_flag)
		{
			switch (GetInput(&flag))
			{
				case 1:
					SaveConfig();
					current_time = 0;
					SaveTime();
					input_flag = 1;
					break;
				case 9:
					LoadConfig();
					input_flag = 1;
					break;
				default:
					break;
			}
		}
		OLED_ClearAll();
		current_page = 0;
	}
	else
	{
		LoadConfig();
		LoadTime();
	}
	
	countdown.h = config.countdown_init_h + 1;
	countdown.m = config.countdown_init_m + 1;
	countdown.s = config.countdown_init_s + 1;
	local_time = localtime(&current_time);
	Refresh();
	while(1)
	{
		if (current_page == 3)
		{
			ring_delay++;
			if (ring_delay == 200)
			{
				ring_flag = !ring_flag;
				ring_delay = 0;
			}
			if (ring_flag)
			{
				DL_GPIO_togglePins(BUZZER_PORT, BUZZER_RING_PIN);
			}
		}
		switch (GetInput(&flag))
		{
			case 1:
				switch (current_page)
				{
					case 0: case 1: case 2:
						current_page = 10;
						cursor = mode;
						Refresh();
						break;
					case 3:
						StopRing();
						break;
					case 4:
						current_time = received_time;
						local_time = localtime(&current_time);
						current_page = page_before_data;
						Refresh();
						break;
					case 29:
						current_page = 2;
						config.countdown_init_h = countdown_h_temp;
						config.countdown_init_m = countdown_m_temp;
						config.countdown_init_s = countdown_s_temp;
						SaveConfig();
					  if (!countdown.enable)
						{
							countdown.h = config.countdown_init_h + 1;
							countdown.m = config.countdown_init_m + 1;
							countdown.s = config.countdown_init_s + 1;
						}
						Refresh();
						break;
					case 10:
						mode = cursor;
						current_page = cursor;
						Refresh();
						break;
					case 20:
						switch (cursor)
						{
							case 0:
								current_page = 21;
								memcpy(&time_temp, local_time, sizeof(struct tm));
								cursor = 0;
								break;
							case 1:
								current_page = 22;
								cursor = 0;
								break;
							case 2:
								config.mode_12h = !config.mode_12h;
								SaveConfig();
								break;
						}
						Refresh();
						break;
					case 22:
						current_page = 220;
						alarm_target = cursor;
						cursor = 0;
						Refresh();
						break;
					case 21:
						current_time = mktime(&time_temp);
						current_page = 20;
						cursor = 0;
						Refresh();
						break;
					case 220:
						switch (cursor)
						{
							case 0:
								current_page = 221;
								cursor = 0;
								if (alarm[alarm_target].null)
								{
									alarm_temp.hour = local_time->tm_hour;
									alarm_temp.minute = local_time->tm_min;
									alarm_temp.second = local_time->tm_sec;
									alarm_temp.month = local_time->tm_mon;
									alarm_temp.day = local_time->tm_mday;
									alarm_temp.enable = 0;
									alarm_temp.mode = alarm[alarm_target].mode;
									alarm_temp.null = 0;
								}
								else
								{
									memcpy(&alarm_temp, &alarm[alarm_target], sizeof(struct Alarm));
								}
								if (alarm_temp.mode == 0b10000000)
								{
									alarm_temp.mode |= (1 << (local_time->tm_wday + 6) % 7);
								}
								break;
							case 1:
								current_page = 222;
								cursor = 0;
								break;
							case 2:
								if (!alarm[alarm_target].null)
								{
									alarm[alarm_target].enable = !alarm[alarm_target].enable;
									SaveConfig();
								}
								break;
						}
						Refresh();
						break;
					case 221:
						memcpy(&alarm[alarm_target], &alarm_temp, sizeof(struct Alarm));
						cursor = 0;
						current_page = 220;
						SaveConfig();
						Refresh();
						break;
					case 222:
						switch (cursor)
						{
							case 0:
								alarm[alarm_target].mode = 0;
								break;
							case 1:
								alarm[alarm_target].mode = 1;
								break;
							case 2:
								alarm[alarm_target].mode = 0b10000000;
								break;
						}
						cursor = 1;
						current_page = 220;
						SaveConfig();
						Refresh();
						break;
				}
				break;
			case 4:
				switch (current_page)
				{
					case 21:
						cursor = (cursor + 5) % 6;
						Refresh();
						break;
					case 29:
						cursor = (cursor + 2) % 3;
						Refresh();
						break;
					case 221:
						switch (alarm_temp.mode)
						{
							case 0:
								cursor = (cursor + 4) % 5;
								break;
							case 1:
								cursor = (cursor + 2) % 3;
								break;
							default:
								cursor = (cursor + 9) % 10;
								break;
						}
						Refresh();
						break;
				}
				break;
			case 5:
				switch (current_page)
				{
					case 1:
						if (timer.enable)
						{
							NVIC_DisableIRQ(TIMG1_INT_IRQn);
							DL_TimerG_stopCounter(TIMG1);
							timer.enable = 0;
						}
						else
						{
							NVIC_ClearPendingIRQ(TIMG1_INT_IRQn);
							NVIC_EnableIRQ(TIMG1_INT_IRQn);
							DL_TimerG_startCounter(TIMG1);
							timer.enable = 1;
						}
						Refresh();
						break;
					case 2:
						if (countdown.enable)
						{
							NVIC_DisableIRQ(TIMG2_INT_IRQn);
							DL_TimerG_stopCounter(TIMG2);
							countdown.enable = 0;
						}
						else
						{
							NVIC_ClearPendingIRQ(TIMG2_INT_IRQn);
							NVIC_EnableIRQ(TIMG2_INT_IRQn);
							DL_TimerG_startCounter(TIMG2);
							countdown.enable = 1;
						}
						Refresh();
						break;
					case 29:
						switch (cursor)
						{
							case 0:
								countdown_h_temp = (countdown_h_temp + 1) % 100;
								break;
							case 1:
								countdown_m_temp = (countdown_m_temp + 1) % 60;
								break;
							case 2:
								countdown_s_temp = (countdown_s_temp + 1) % 60;
								break;
						}
						Refresh();
						break;
					case 21:
						switch (cursor)
						{
							case 0:
								time_temp.tm_hour = (time_temp.tm_hour + 1) % 24;
								break;
							case 1:
								time_temp.tm_min = (time_temp.tm_min + 1) % 60;
								break;
							case 2:
								time_temp.tm_sec = (time_temp.tm_sec + 1) % 60;
								break;
							case 3:
								if (time_temp.tm_year < 137)
								{
									time_temp.tm_year++;
									if (time_temp.tm_mday > GetMaxDay(time_temp.tm_year, time_temp.tm_mon))
									{
										time_temp.tm_mday = GetMaxDay(time_temp.tm_year, time_temp.tm_mon);
									}
								}
								break;
							case 4:
								time_temp.tm_mon = (time_temp.tm_mon + 1) % 12;
								if (time_temp.tm_mday > GetMaxDay(time_temp.tm_year, time_temp.tm_mon))
								{
									time_temp.tm_mday = GetMaxDay(time_temp.tm_year, time_temp.tm_mon);
								}
								break;
							case 5:
								time_temp.tm_mday = time_temp.tm_mday % GetMaxDay(time_temp.tm_year, time_temp.tm_mon) + 1;
								break;
						}
						Refresh();
						break;
					case 221:
						switch (cursor)
						{
							case 0:
								alarm_temp.hour = (alarm_temp.hour + 1) % 24;
								break;
							case 1:
								alarm_temp.minute = (alarm_temp.minute + 1) % 60;
								break;
							case 2:
								alarm_temp.second = (alarm_temp.second + 1) % 60;
								break;
							default:
								if (alarm_temp.mode == 0)
								{
									if (cursor == 3)
									{
										alarm_temp.month = (alarm_temp.month + 1) % 12;
										if (alarm_temp.day > GetMaxDay(100, alarm_temp.month))
										{
											alarm_temp.day = GetMaxDay(100, alarm_temp.month);
										}
									}
									else
									{
										alarm_temp.day = alarm_temp.day % GetMaxDay(100, alarm_temp.month) + 1;
									}
								}
								else
								{
									alarm_temp.mode |= (1 << (cursor - 3));
								}
								break;
						}
						Refresh();
						break;
				}
				break;
			case 6:
				switch (current_page)
				{
					case 1:
						if (timer.enable)
						{
							timer.enable = 0;
							NVIC_DisableIRQ(TIMG1_INT_IRQn);
							DL_TimerG_stopCounter(TIMG1);
						}
						timer.ms = 0;
						timer.s = 0;
						OLED_ClearAll();
						Refresh();
						break;
					case 2:
						if (countdown.enable)
						{
							countdown.enable = 0;
							NVIC_DisableIRQ(TIMG2_INT_IRQn);
							DL_TimerG_stopCounter(TIMG2);
						}
						countdown.h = config.countdown_init_h + 1;
						countdown.m = config.countdown_init_m + 1;
						countdown.s = config.countdown_init_s + 1;
						Refresh();
						break;
					case 29:
						switch (cursor)
						{
							case 0:
								countdown_h_temp = (countdown_h_temp + 99) % 100;
								break;
							case 1:
								countdown_m_temp = (countdown_m_temp + 59) % 60;
								break;
							case 2:
								countdown_s_temp = (countdown_s_temp + 59) % 60;
								break;
						}
						Refresh();
						break;
					case 21:
						switch (cursor)
						{
							case 0:
								time_temp.tm_hour = (time_temp.tm_hour + 23) % 24;
								break;
							case 1:
								time_temp.tm_min = (time_temp.tm_min + 59) % 60;
								break;
							case 2:
								time_temp.tm_sec = (time_temp.tm_sec + 59) % 60;
								break;
							case 3:
								if (time_temp.tm_year > 70)
								{
									time_temp.tm_year--;
									if (time_temp.tm_mday > GetMaxDay(time_temp.tm_year, time_temp.tm_mon))
									{
										time_temp.tm_mday = GetMaxDay(time_temp.tm_year, time_temp.tm_mon);
									}
								} 
								break;
							case 4:
								time_temp.tm_mon = (time_temp.tm_mon + 11) % 12;
								if (time_temp.tm_mday > GetMaxDay(time_temp.tm_year, time_temp.tm_mon))
								{
									time_temp.tm_mday = GetMaxDay(time_temp.tm_year, time_temp.tm_mon);
								}
								break;
							case 5:
								time_temp.tm_mday = (time_temp.tm_mday + GetMaxDay(time_temp.tm_year, time_temp.tm_mon) - 2) % GetMaxDay(time_temp.tm_year, time_temp.tm_mon) + 1;
								break;
						}
						Refresh();
						break;
					case 221:
						switch (cursor)
						{
							case 0:
								alarm_temp.hour = (alarm_temp.hour + 23) % 24;
								break;
							case 1:
								alarm_temp.minute = (alarm_temp.minute + 59) % 60;
								break;
							case 2:
								alarm_temp.second = (alarm_temp.second + 59) % 60;
								break;
							default:
								if (alarm_temp.mode == 0)
								{
									if (cursor == 3)
									{
										alarm_temp.month = (alarm_temp.month + 11) % 12;
										if (alarm_temp.day > GetMaxDay(100, alarm_temp.month))
										{
											alarm_temp.day = GetMaxDay(100, alarm_temp.month);
										}
									}
									else
									{
										alarm_temp.day = (alarm_temp.day + GetMaxDay(100, alarm_temp.month) - 2) % GetMaxDay(100, alarm_temp.month) + 1;
									}
								}
								else
								{
									if ((~(1 << (cursor - 3)) & alarm_temp.mode) != 0b10000000)
									{
										alarm_temp.mode &= ~(1 << (cursor - 3));
									}
								}
								break;
						}
						Refresh();
						break;
				}
				break;
			case 7:
				switch (current_page)
				{
					case 2:
						current_page = 29;
						cursor = 0;
						countdown_h_temp = config.countdown_init_h;
						countdown_m_temp = config.countdown_init_m;
						countdown_s_temp = config.countdown_init_s;
						Refresh();
						break;
					case 10:
						cursor = (cursor + PAGE10_INDEX - 1) % PAGE10_INDEX;
						Refresh();
						break;
					case 20:
						cursor = (cursor + PAGE20_INDEX - 1) % PAGE20_INDEX;
						Refresh();
						break;
					case 22:
						cursor = (cursor + PAGE22_INDEX - 1) % PAGE22_INDEX;
						Refresh();
						break;
					case 21:
						cursor = (cursor + 3) % 6;
						Refresh();
						break;
					case 220:
						cursor = (cursor + PAGE220_INDEX - 1) % PAGE220_INDEX;
						Refresh();
						break;
					case 221:
						switch (alarm_temp.mode)
						{
							case 0:
								break;
							case 1:
								break;
							default:
								cursor = (cursor + 5) % 10;
								Refresh();
								break;
						}
						break;
					case 222:
						cursor = (cursor + PAGE222_INDEX - 1) % PAGE222_INDEX;
						Refresh();
						break;
				}
				break;
			case 8:
				switch (current_page)
				{
					case 10:
						cursor = (cursor + 1) % PAGE10_INDEX;
						Refresh();
						break;
					case 20:
						cursor = (cursor + 1) % PAGE20_INDEX;
						Refresh();
						break;
					case 22:
						cursor = (cursor + 1) % PAGE22_INDEX;
						Refresh();
						break;
					case 21:
						cursor = (cursor + 3) % 6;
						Refresh();
						break;
					case 220:
						cursor = (cursor + 1) % PAGE220_INDEX;
						Refresh();
						break;
					case 221:
						switch (alarm_temp.mode)
						{
							case 0:
								break;
							case 1:
								break;
							default:
								cursor = (cursor + 5) % 10;
								Refresh();
								break;
						}
						break;
					case 222:
						cursor = (cursor + 1) % PAGE222_INDEX;
						Refresh();
						break;
				}
				break;
			case 9:
				switch (current_page)
				{
					case 0: case 1: case 2:
						current_page = 20;
						cursor = 0;
						Refresh();
						break;
					case 3:
						StopRing();
						if (countdown.enable)
						{
							DL_TimerG_stopCounter(TIMG2);
						}
						else
						{
							countdown.enable = 1;
						}
						NVIC_EnableIRQ(TIMG2_INT_IRQn);
						DL_TimerG_startCounter(TIMG2);
						countdown.h = 1;
						countdown.m = 6;
						countdown.s = 1;
						Refresh();
						break;
					case 4:
						current_page = page_before_data;
						local_time = localtime(&current_time);
						Refresh();
						break;
					case 29:
						current_page = 2;
						Refresh();
						break;
					case 10:
						current_page = mode;
						Refresh();
						break;
					case 20:
						current_page = mode;
						Refresh();
						break;
					case 22:
						current_page = 20;
						cursor = 1;
						Refresh();
						break;
					case 21:
						current_page = 20;
						cursor = 0;
						Refresh();
						break;
					case 220:
						current_page = 22;
						cursor = alarm_target;
						Refresh();
						break;
					case 221:
						current_page = 220;
						cursor = 0;
						Refresh();
						break;
					case 222:
						current_page = 220;
						cursor = 1;
						Refresh();
						break;
				}
				break;
			case 12:
				switch (current_page)
				{
					case 21:
						cursor = (cursor + 1) % 6;
						Refresh();
						break;
					case 29:
						cursor = (cursor + 1) % 3;
						Refresh();
						break;
					case 221:
						switch (alarm_temp.mode)
						{
							case 0:
								cursor = (cursor + 1) % 5;
								break;
							case 1:
								cursor = (cursor + 1) % 3;
								break;
							default:
								cursor = (cursor + 1) % 10;
								break;
						}
						Refresh();
						break;
				}
				break;
		}
	}
}

void TIMER_SEC_INST_IRQHandler()
{
	switch (DL_TimerG_getPendingInterrupt(TIMG0))
	{
		case DL_TIMER_IIDX_ZERO:
			current_time++;
			local_time = localtime(&current_time);
			if (!DL_GPIO_readPins(SWITCH_PORT, SWITCH_AUTOSAVE_PIN))
			{
				SaveTime();
			}
			if (current_page == 4)
			{
				received_time++;
			}
			for (uint8_t i = 0; i < 3; i++)
			{
				if (alarm[i].enable)
				{
					if (alarm[i].second == local_time->tm_sec &&
							alarm[i].minute == local_time->tm_min &&
							alarm[i].hour == local_time->tm_hour)
					{
						if (alarm[i].mode == 0)
						{
							if (alarm[i].month == local_time->tm_mon && alarm[i].day == local_time->tm_mday)
							{
								alarm[i].enable = 0;
								Ring();
							}
						}
						else if (alarm[i].mode == 1)
						{
							Ring();
						}
						else
						{
							if ((1 << (local_time->tm_wday + 6) % 7) & alarm[i].mode)
							{
								Ring();
							}
						}
					}
				}
			}
			if (current_page == 0)
			{
				Refresh();
			}
			break;
		default:
			break;
	}
}

void TIMER_100MS_INST_IRQHandler()
{
	switch (DL_TimerG_getPendingInterrupt(TIMG1))
	{
		case DL_TIMER_IIDX_ZERO:
			timer.ms++;
			if (timer.ms == 10)
			{
				timer.ms = 0;
				timer.s++;
			}
			if (current_page == 1)
			{
				Refresh();
			}
			break;
		default:
			break;
	}
}

void TIMER_COUNTDOWN_INST_IRQHandler()
{
	switch (DL_TimerG_getPendingInterrupt(TIMG2))
	{
		case DL_TIMER_IIDX_ZERO:
			countdown.s--;
			if (countdown.s == 0)
			{
				countdown.s = 60;
				countdown.m--;
				if (countdown.m == 0)
				{
					countdown.m = 60;
					countdown.h--;
					if (countdown.h == 0)
					{
						countdown.h = config.countdown_init_h + 1;
						countdown.m = config.countdown_init_m + 1;
						countdown.s = config.countdown_init_s + 1;
						NVIC_DisableIRQ(TIMG2_INT_IRQn);
						DL_TimerG_stopCounter(TIMG2);
						countdown.enable = 0;
						Ring();
					}
				}
			}
			if (current_page == 2)
			{
				Refresh();
			}
			break;
		default:
			break;
	}
}

void TIMER_ALARM_INST_IRQHandler()
{
	switch (DL_TimerG_getPendingInterrupt(TIMG4))
	{
		case DL_TIMER_IIDX_ZERO:
			StopRing();
			break;
		default:
			break;
	}
}

void UART0_IRQHandler()
{
  switch(DL_UART_Main_getPendingInterrupt(UART0))
	{
		case DL_UART_MAIN_IIDX_RX:
			data_receive = DL_UART_Main_receiveData(UART0);
			if (data_receive == '(')
			{
				for (uint8_t i = 0; i < DATA_BUFFER_SIZE; i++)
				{
					data_temp[i] = 0;
				}
				data_flag = 1;
			}
			else if (data_receive == ')')
			{
				if (data_flag)
				{
					data_flag = 0;
					received_time = atoi(data_temp) + 28800; //GMT+8
					local_time = localtime(&received_time);
					memcpy(&time_temp, local_time, sizeof(struct tm));
					page_before_data = current_page;
					current_page = 4;
					Refresh();
				}
			}
			else
			{
				data_flag = 1;
				if (data_flag)
				{
					for (uint8_t i = 0; i < DATA_BUFFER_SIZE; i++)
					{
						if (data_temp[i] == 0)
						{
							data_temp[i] = data_receive;
							break;
						}
					}
				}
			}
      break;
		default:
			break;
	}
}