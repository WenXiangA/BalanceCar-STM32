#ifndef  _MOTOR_H
#define  _MOTOR_H

#include "stm32f10x.h"
#include "stdio.h"

#define PWM_MAX (7200)
#define PWM_MIN (-7200)	// PWM限幅变量

#define PWMTIM				TIM3
#define PWMTIM_CLK		RCC_APB1Periph_TIM3
#define PWMTIM_CLKCMD	RCC_APB1PeriphClockCmd

#define PWM1_INIT			TIM_OC1Init
#define PWM2_INIT			TIM_OC2Init
#define PWM3_INIT			TIM_OC3Init
#define PWM4_INIT			TIM_OC4Init

#define PWM1_SET			TIM_SetCompare1
#define PWM2_SET			TIM_SetCompare2
#define PWM3_SET			TIM_SetCompare3
#define PWM4_SET			TIM_SetCompare4

#define PWM1_CHANNEL	TIM_Channel_1
#define PWM2_CHANNEL	TIM_Channel_2
#define PWM3_CHANNEL	TIM_Channel_3
#define PWM4_CHANNEL	TIM_Channel_4

#define PWM1_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM1_CHANNEL, TIM_CCx_Disable), (GPIOA->CRL &= ~(0xF<<24), GPIOA->CRL |= (0x3<<24)), GPIO_SetBits(PWM1_PORT, PWM1_PIN))
#define PWM2_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM2_CHANNEL, TIM_CCx_Disable), (GPIOA->CRL &= ~(0xF<<28), GPIOA->CRL |= (0x3<<28)), GPIO_SetBits(PWM2_PORT, PWM2_PIN))
#define PWM3_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM3_CHANNEL, TIM_CCx_Disable), (GPIOB->CRL &= ~(0xF<<0), GPIOB->CRL |= (0x3<<0)), GPIO_SetBits(PWM3_PORT, PWM3_PIN))
#define PWM4_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM4_CHANNEL, TIM_CCx_Disable), (GPIOB->CRL &= ~(0xF<<4), GPIOB->CRL |= (0x3<<4)), GPIO_SetBits(PWM4_PORT, PWM4_PIN))

//#define PWM1_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM1_CHANNEL, TIM_CCx_Disable), (GPIOA->CRL &= ~(0xF<<24), GPIOA->CRL |= (0x3<<24)), GPIO_ResetBits(PWM1_PORT, PWM1_PIN))
//#define PWM2_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM2_CHANNEL, TIM_CCx_Disable), (GPIOA->CRL &= ~(0xF<<28), GPIOA->CRL |= (0x3<<28)), GPIO_ResetBits(PWM2_PORT, PWM2_PIN))
//#define PWM3_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM3_CHANNEL, TIM_CCx_Disable), (GPIOB->CRL &= ~(0xF<<0), GPIOB->CRL |= (0x3<<0)), GPIO_ResetBits(PWM3_PORT, PWM3_PIN))
//#define PWM4_DISABLE()	(TIM_CCxCmd(PWMTIM, PWM4_CHANNEL, TIM_CCx_Disable), (GPIOB->CRL &= ~(0xF<<4), GPIOB->CRL |= (0x3<<4)), GPIO_ResetBits(PWM4_PORT, PWM4_PIN))

#define PWM1_ENABLE()		((GPIOA->CRL &= ~(0xF<<24), GPIOA->CRL |= (0xB<<24)), TIM_CCxCmd(PWMTIM, PWM1_CHANNEL, TIM_CCx_Enable))
#define PWM2_ENABLE()		((GPIOA->CRL &= ~(0xF<<28), GPIOA->CRL |= (0xB<<28)), TIM_CCxCmd(PWMTIM, PWM2_CHANNEL, TIM_CCx_Enable))
#define PWM3_ENABLE()		((GPIOB->CRL &= ~(0xF<<0), GPIOB->CRL |= (0xB<<0)), TIM_CCxCmd(PWMTIM, PWM3_CHANNEL, TIM_CCx_Enable))
#define PWM4_ENABLE()		((GPIOB->CRL &= ~(0xF<<4), GPIOB->CRL |= (0xB<<4)), TIM_CCxCmd(PWMTIM, PWM4_CHANNEL, TIM_CCx_Enable))




#define PWM1_CLK			RCC_APB2Periph_GPIOA
#define PWM1_PORT			GPIOA
#define PWM1_PIN			GPIO_Pin_6

#define PWM2_CLK			RCC_APB2Periph_GPIOA
#define PWM2_PORT			GPIOA
#define PWM2_PIN			GPIO_Pin_7

#define PWM3_CLK			RCC_APB2Periph_GPIOB
#define PWM3_PORT			GPIOB
#define PWM3_PIN			GPIO_Pin_0

#define PWM4_CLK			RCC_APB2Periph_GPIOB
#define PWM4_PORT			GPIOB
#define PWM4_PIN			GPIO_Pin_1



void Motor_Init(void);
void Motor_PWMConfig(uint16_t psc, uint16_t per);
void Limit(int *pwmA,int *pwmB);
int GFP_abs(int p);
void Motor_PWMLoad(int motoA, int motoB);
#endif
