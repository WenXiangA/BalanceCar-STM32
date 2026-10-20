#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

void LED_Config(void);
void LED_Breatha(void);

#define LED1_pin GPIO_Pin_9
#define LED2_pin GPIO_Pin_8

#define LED_OFF(port,pin) GPIO_SetBits(port,pin)//宏定义，将后面函数封装为前面函数
#define LED_ON(port,pin) GPIO_ResetBits(port,pin)//宏定义，将后面函数封装为前面函数



#endif

