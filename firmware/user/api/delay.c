#include "delay.h"

vu32 System_Time = 0;
//uint16_t LED_Time = 0;
uint16_t  LED_Time[2] = {0,1000};
uint16_t  ENC_Time[2] = {0,500};
u32 BLUE_TIME = 0;
u32 MPU6050_TIME = 0;
u32 Control_TIME = 0;
u32 Steer_TIME = 0;
u32 Sr04_TIME = 0;

void SysTick_Handler(void)
{
	System_Time++;
//	LED_Time++;
		LED_Time[0]++;
	  ENC_Time[0]++;
	  BLUE_TIME++;
		MPU6050_TIME++;
		Control_TIME++;
		Steer_TIME++;
	  Sr04_TIME++;
}

void Delay_ms(uint32_t time)
{
	uint32_t tick = System_Time + time;
	while(System_Time < tick)
	{}
}


void SysTick_Init(uint32_t reload)
{
	if(SysTick_Config(reload))
	{
		while(1);
	}
}


void Delay_us(uint32_t time)
{
	while(time--)
		delay_1us();
}


