#include "sr04.h"
#include "delay.h"
#include "motor.h"
#include "stdio.h"

void Sr04_Init(void)
{
	Sr04_Config();
	Tim1Sr04_Config();
}

void Sr04_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	EXTI_InitTypeDef	EXTI_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(SR04_TRIG_CLK | SR04_ECHO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SR04_TRIG_PIN;
	GPIO_Init(SR04_TRIG_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SR04_ECHO_PIN;
	GPIO_Init(SR04_ECHO_PORT, &GPIO_InitStructure);	

	GPIO_ResetBits(SR04_TRIG_PORT, SR04_TRIG_PIN);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(SR04_ECHO_PinSourcePort, SR04_ECHO_PinSource);
  
  EXTI_InitStructure.EXTI_Line = SR04_ECHO_EXTI_Line;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = SR04_ECHO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

__SR04_TypeDef sr04 = {0};

void Sr04_SendTTL(void) 
{
	sr04.sendCount++; //123
	if(sr04.sendCount == 1 && GPIO_ReadOutputDataBit(SR04_TRIG_PORT, SR04_TRIG_PIN) == Bit_RESET) {
		GPIO_SetBits(SR04_TRIG_PORT, SR04_TRIG_PIN);
	}
	else if(sr04.sendCount == 3 && GPIO_ReadOutputDataBit(SR04_TRIG_PORT, SR04_TRIG_PIN) == Bit_SET) {
		GPIO_ResetBits(SR04_TRIG_PORT, SR04_TRIG_PIN);
	}
	else if(sr04.sendCount > 6000){
		sr04.sendCount = 0;
		GPIO_ResetBits(SR04_TRIG_PORT, SR04_TRIG_PIN);
	}
}

void SR04_ECHO_IRQHandler(void)
{
  if(EXTI_GetITStatus(SR04_ECHO_EXTI_Line) != RESET)
  {
    if(GPIO_ReadInputDataBit(SR04_ECHO_PORT, SR04_ECHO_PIN) == Bit_SET) {
			sr04.recvCount = 0;
			sr04.recvCountFlag = 1;
		}
		else {
			sr04.recvCountFlag = 0;
		}
    EXTI_ClearITPendingBit(SR04_ECHO_EXTI_Line);
  }
}


void Sr04_CountInc(void)
{
	if(sr04.recvCountFlag == 1)
		sr04.recvCount++;
}

float Sr04_GetLength(void)
{
	if(sr04.recvCountFlag == 0)
		sr04.leng = sr04.recvCount*34/200.0;
	if(sr04.leng > 400)	sr04.leng = 380;
	return sr04.leng;
}
#include "ble.h"
float Length = 0;
void Sr04_Mode(void)
{
	Length = Sr04_GetLength();
//	printf("Length==%.2f\r\n",Length);
	if(Length>10 && Length<30)
	{
		 CarDir = CarLeft;
	}
	else
	{
		CarDir = CarBalance;
	}

}
void Tim1Sr04_Config(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  TIM_TimeBaseStructure.TIM_Prescaler = 36-1;   //72M/36=2M 0.5us
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 20-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x05;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM5, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM5, DISABLE);
}
void TIM5_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) {
		Sr04_SendTTL();
		Sr04_CountInc();
		TIM_ClearFlag(TIM5, TIM_IT_Update);
	}
}

//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) {
//		TIM_ClearFlag(TIM3, TIM_IT_Update);
//	}
//}

//void Tim1Sr04_Config(void)
//{
//	NVIC_InitTypeDef   NVIC_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//  TIM_TimeBaseStructure.TIM_Prescaler = 36-1;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseStructure.TIM_Period = 20-1;//20-1;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	
//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	TIM_Cmd(TIM3, ENABLE);
//	
//	TIM_CtrlPWMOutputs(TIM3, DISABLE);
//}


