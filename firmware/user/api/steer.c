#include "steer.h"


void Tim1_Config(u16 psc, u16 arr, u16 ccr)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = ccr;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

int steer1Angle = 0, steer2Angle = -90;
void Steer_Init(void)
{
	Tim1_Config(72,20000,1500);
	Steer_SetPWM(1, steer1Angle);
	Steer_SetPWM(2, steer2Angle);
}

void Steer_SetPWM(uint8_t steer, uint16_t pwm) 
{
	switch(steer)
	{
		case 1: TIM_SetCompare1(TIM1, pwm); break;
		case 2: TIM_SetCompare4(TIM1, pwm); break;
	}
}

void Steer_SetAngle(uint8_t steer, int angle) 
{
	switch(steer)
	{
		case 1: 
			if(angle < -90 || angle > 90)	break;
			angle = angle*(2000.0/180)+1500;
			TIM_SetCompare1(TIM1, angle); 
		break;
		case 2: 
			if(angle < -180 || angle > 0)	break;
			angle = angle*(-2000.0/180)+500;
			TIM_SetCompare4(TIM1, angle); 
		break;
	}
}


#include "ble.h"
void Steer_Control(void)
{
	switch(SteerDir) {
		case SteerStop:  break;
		case SteerLeft: steer1Angle-=2; break;
		case SteerRight: steer1Angle+=2; break;
		case SteerUp: steer2Angle+=2; break;
		case SteerDown: steer2Angle-=2; break;
		default:break;
	}
	steer1Angle = steer1Angle>90?90:(steer1Angle<-90?(-90):steer1Angle);
	steer2Angle = steer2Angle>0?0:(steer2Angle<-180?(-180):steer2Angle);
	Steer_SetPWM(1, steer1Angle);
	Steer_SetPWM(2, steer2Angle);
}


