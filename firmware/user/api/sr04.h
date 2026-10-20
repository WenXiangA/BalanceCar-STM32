#ifndef __SR04_H
#define __SR04_H

#include "stm32f10x.h"

#define SR04_TRIG_CLK		RCC_APB2Periph_GPIOB
#define SR04_TRIG_PORT	GPIOB
#define SR04_TRIG_PIN		GPIO_Pin_4

#define SR04_ECHO_CLK		RCC_APB2Periph_GPIOB
#define SR04_ECHO_PORT	GPIOB
#define SR04_ECHO_PIN		GPIO_Pin_5

#define SR04_ECHO_PinSourcePort		GPIO_PortSourceGPIOB
#define SR04_ECHO_PinSource				GPIO_PinSource5
#define SR04_ECHO_EXTI_Line				EXTI_Line5

#define SR04_ECHO_IRQn						EXTI9_5_IRQn
#define SR04_ECHO_IRQHandler			EXTI9_5_IRQHandler




typedef struct{
	uint32_t sendCount;
	uint8_t recvCountFlag;
	uint32_t recvCount;
	float leng;	
}__SR04_TypeDef;

extern __SR04_TypeDef sr04;
extern uint8_t SteerDir;

void Sr04_Init(void);
void Sr04_Config(void);
void Sr04_SendTTL(void) ;
void Sr04_CountInc(void);
float Sr04_GetLength(void);
void Tim1Sr04_Config(void);
void Sr04_Mode(void);


#endif
