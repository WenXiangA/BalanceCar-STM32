#include "LED.h"
#include "delay.h"
//LED2 PC8    LED1 PC9
void LED_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct = {0};  
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	LED_OFF(GPIOC,LED1_pin);
	LED_OFF(GPIOC,LED2_pin);
}

void LED_Breatha(void)
{
    for(int t = 0;t < 1000;t++)
	{
	   	LED_ON(GPIOC,LED1_pin);
	    LED_ON(GPIOC,LED2_pin); 
		  Delay_us(t);
		  LED_OFF(GPIOC,LED1_pin);
	    LED_OFF(GPIOC,LED2_pin);
		  Delay_us(1000-t);
	}
	for(int t = 0;t < 1000;t++)
	{
	   	LED_ON(GPIOC,LED1_pin);
	    LED_ON(GPIOC,LED2_pin); 
		  Delay_us(1000-t);
		  LED_OFF(GPIOC,LED1_pin);
	    LED_OFF(GPIOC,LED2_pin);
		  Delay_us(t);
	}
}

//void LED_Task(void)
//{
//    GPIO_SetBits(GPIOC,GPIO_Pin_8);
//}
