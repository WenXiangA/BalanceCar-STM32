#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

#define ENCODER_A	2
#define ENCODER_B	4


extern __IO int16_t EncoderA_Overflow_Count;
extern __IO int16_t EncoderB_Overflow_Count;
extern __IO int32_t CaptureA_Count;
extern __IO int32_t LastA_Count;
extern __IO int32_t CaptureB_Count;
extern __IO int32_t LastB_Count;
extern __IO int8_t MotorA_Direction;
extern __IO int8_t MotorB_Direction;
extern __IO float ShaftA_Speed;
extern __IO float ShaftB_Speed;

void Encoder_Init(void);
void Encoder_TIM2Config(void);   // 编码器1初始化函数
void Encoder_TIM4Config(void);   // 编码器2初始化函数
void Encoder_ReadSpeedTest(void); 
int Read_MotorASpeed(void);
int Read_MotorBSpeed(void);
int Read_Speed(int TIMx);
#endif
