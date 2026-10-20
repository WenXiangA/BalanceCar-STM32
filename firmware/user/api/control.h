#ifndef  _CONTROL_H
#define  _CONTROL_H

#include "stm32f10x.h"
#include "stdio.h"
#include "IO_BIT.h" 

extern int MOTO1,MOTO2;
extern float Pitch,Roll,Yaw;		//角度
extern short gyrox,gyroy,gyroz;	//陀螺仪--角速度
extern short aacx,aacy,aacz;		//加速度
extern int Encoder_Left,Encoder_Right;
extern volatile uint8_t SPEED_X;


void Control_Task(void);
int Vertical(float Med,float Angle,float gyro_Y);
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z, int RC);
#endif


