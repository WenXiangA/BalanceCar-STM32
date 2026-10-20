#include "motor.h"
#include "delay.h"

void Motor_Init(void)
{
	Motor_PWMConfig(10, 7200);
}

void Motor_PWMConfig(uint16_t psc, uint16_t per)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
	RCC_APB2PeriphClockCmd(PWM1_CLK | PWM2_CLK | PWM3_CLK | PWM4_CLK, ENABLE);
	PWMTIM_CLKCMD(PWMTIM_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = PWM1_PIN;
	GPIO_Init(PWM1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM2_PIN;
	GPIO_Init(PWM2_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM3_PIN;
	GPIO_Init(PWM3_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PWM4_PIN;
	GPIO_Init(PWM4_PORT, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = per-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = psc-1;
	TIM_TimeBaseInit(PWMTIM, &TIM_TimeBaseInitStruct);
	
	
	TIM_OCInitTypeDef TIM_OCInitStructure = {0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//TIM_OCMode_PWM1;//
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	PWM1_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM2_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM3_INIT(PWMTIM, &TIM_OCInitStructure);
	PWM4_INIT(PWMTIM, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(PWMTIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(PWMTIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(PWMTIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(PWMTIM, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(PWMTIM, ENABLE);
	TIM_Cmd(PWMTIM, ENABLE);
}


void Limit(int *pwmA, int *pwmB)
{
	if(*pwmA>PWM_MAX)*pwmA=PWM_MAX;
	if(*pwmA<PWM_MIN)*pwmA=PWM_MIN;
	
	if(*pwmB>PWM_MAX)*pwmB=PWM_MAX;
	if(*pwmB<PWM_MIN)*pwmB=PWM_MIN;
}


int GFP_abs(int p)
{
	int q;
	q = p>0?p:(-p);
	return q;
}


void Motor_PWMLoad(int motoA, int motoB)
{
	if(motoA > 0)	{
		PWM1_DISABLE();
		__NOP();__NOP();
		PWM2_ENABLE();
		PWM2_SET(PWMTIM, GFP_abs(motoA));
	}
	else {
		PWM2_DISABLE();
		__NOP();__NOP();
		PWM1_ENABLE();
		PWM1_SET(PWMTIM, GFP_abs(motoA));
	}		
	
	if(motoB>0) {
		PWM4_DISABLE();
		__NOP();__NOP();
		PWM3_ENABLE();
		PWM3_SET(PWMTIM, GFP_abs(motoB));
	}
	else {
		PWM3_DISABLE();
		__NOP();__NOP();
		PWM4_ENABLE();
		PWM4_SET(PWMTIM, GFP_abs(motoB));
	}	
	if(motoA == 0 && motoB == 0)
	{
	    PWM1_DISABLE();
		  PWM2_DISABLE();
		  PWM3_DISABLE();
			PWM4_DISABLE();
	}
}


