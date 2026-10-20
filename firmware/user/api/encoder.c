#include "encoder.h"


void Encoder_Init(void)
{
	Encoder_TIM2Config();
	Encoder_TIM4Config();
}


void Encoder_TIM2Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
  TIM_ARRPreloadConfig(TIM2, DISABLE);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM2, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM2, 0);
	TIM_Cmd(TIM2, ENABLE);
}

void Encoder_TIM4Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_ICInitTypeDef TIM_ICInitStruct;
  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct); 
	TIM_ARRPreloadConfig(TIM4, DISABLE);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  
  
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInit(TIM4, &TIM_ICInitStruct);
  
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);
}

__IO int16_t EncoderA_Overflow_Count = 0;
__IO int16_t EncoderB_Overflow_Count = 0;
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if((TIM2->CR1 & (1<<4))) {
			EncoderA_Overflow_Count--;
		}
		else {
			EncoderA_Overflow_Count++;
		}
  }
}

void TIM4_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		if((TIM4->CR1 & (1<<4))) {
			EncoderB_Overflow_Count--;
		}
		else {
			EncoderB_Overflow_Count++;
		}
  }
}


__IO int8_t MotorA_Direction = 0;
__IO int32_t CaptureA_Count = 0;
__IO int32_t LastA_Count = 0;
__IO float ShaftA_Speed = 0.0f;

__IO int8_t MotorB_Direction = 0;
__IO int32_t CaptureB_Count = 0;
__IO int32_t LastB_Count = 0;
__IO float ShaftB_Speed = 0.0f;


#include "stdio.h"
void Encoder_ReadSpeedTest(void)
{
	MotorA_Direction = !!(TIM2->CR1 & (1<<4));
	MotorB_Direction = !!(TIM4->CR1 & (1<<4));
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	ShaftA_Speed = (float)(CaptureA_Count - LastA_Count) / (11*4) * 10 ;
	ShaftB_Speed = (float)(CaptureB_Count - LastB_Count) / (11*4) * 10 ;
	printf("方向：%d\t%d\r\n", MotorA_Direction, MotorB_Direction);
	printf("有效计数值：%d\t%d\r\n", CaptureA_Count - LastA_Count, CaptureB_Count - LastB_Count);/* 单位时间计数值 = 当前时刻总计数值 - 上一时刻总计数值 */
	printf("轴处转速：%.2f\t %.2f 转/秒 \r\n", ShaftA_Speed, ShaftB_Speed);
	printf("出轴转速：%.2f\t %.2f 转/秒 \r\n", ShaftA_Speed/34, ShaftB_Speed/34);/* 输出轴转速 = 转轴转速 / 减速比 */
	
	LastA_Count = CaptureA_Count;
	LastB_Count = CaptureB_Count;
}


int Read_MotorASpeed(void)
{
	int value = 0;
	CaptureA_Count = TIM_GetCounter(TIM2) + (EncoderA_Overflow_Count * 65535);
	value = CaptureA_Count - LastA_Count;
	LastA_Count = CaptureA_Count;
	return value;
}

int Read_MotorBSpeed(void)
{
	int value = 0;;
	CaptureB_Count = TIM_GetCounter(TIM4) + (EncoderB_Overflow_Count * 65535);
	value = CaptureB_Count - LastB_Count;
	LastB_Count = CaptureB_Count;
	return value;
}


int Read_Speed(int TIMx)
{
  int value_1;
  switch(TIMx)
  {
    case 2:
      value_1 = (short)TIM_GetCounter(TIM2);
      TIM_SetCounter(TIM2, 0);
      EncoderA_Overflow_Count = 0;
      break;
    case 4:
      value_1 = (short)TIM_GetCounter(TIM4);
      TIM_SetCounter(TIM4, 0);
      EncoderB_Overflow_Count = 0;
      break; 
    default: value_1 = 0;
  }
  return value_1;
}


