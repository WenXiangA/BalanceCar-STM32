#ifndef __STEER_H
#define __STEER_H

#include "stm32f10x.h"

void Steer_Init(void);
void Tim1_Config(u16 psc, u16 arr, u16 ccr);
void Steer_SetPWM(uint8_t steer, uint16_t pwm);
void Steer_SetAngle(uint8_t steer, int angle);
void Steer_Control(void);
#endif
