#include "control.h"
#include "encoder.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "motor.h"
#include "ble.h"


int MOTO1, MOTO2;
float Pitch, Roll, Yaw;
short gyrox, gyroy, gyroz;
short aacx, aacy, aacz;
int Encoder_Left, Encoder_Right;

float Med_Angle = 2;
float Target_Speed = 0;
float Turn_Speed = 0;

//380				0.9     
float Vertical_Kp = (230), 
			Vertical_Kd = 0.6; 

float Velocity_Kp = -0.5, 
			Velocity_Ki = -0.5/200;   

float Turn_Kd = -0.7, Turn_Kp = -20;
int Vertical_out, Velocity_out, Turn_out;

#define SPEED_Z 80

void Control_Task(void)
{
  int PWM_out = 0;
	Encoder_Left  = -Read_Speed(ENCODER_A);
	Encoder_Right = Read_Speed(ENCODER_B);
	mpu_dmp_get_data(&Pitch, &Roll, &Yaw);
	MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);
	
	if(Pitch > 50 || Pitch < -50) {
		MOTO1 = 0;
		MOTO2 = 0;
	}
	else{
		switch(CarDir) {
			case CarBalance: Target_Speed = 0; Turn_Speed = 0; Turn_Kd = -0.7; break;
			case CarFore: Target_Speed++; Turn_Kd = -0.7; break;
			case CarBack: Target_Speed--; Turn_Kd = -0.7; break;
			case CarLeft: Turn_Speed++; Turn_Kd=0; break;
			case CarRight: Turn_Speed--; Turn_Kd=0; break;
		}
		Target_Speed = Target_Speed>SPEED_X?SPEED_X:(Target_Speed<-SPEED_X?(-SPEED_X):Target_Speed);
		Turn_Speed = Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);
	
		Velocity_out = Velocity(Target_Speed, Encoder_Left, Encoder_Right);
		
		Vertical_out = Vertical(Velocity_out+Med_Angle, Pitch, gyroy);
		
		Turn_out = Turn(gyroz, Turn_Speed);
		
		PWM_out = Vertical_out;
		
		MOTO1 = PWM_out-Turn_out;
		MOTO2 = PWM_out+Turn_out;
		Limit(&MOTO1, &MOTO2);
	}
	Motor_PWMLoad(MOTO1, MOTO2);
}


int Vertical(float Med, float Angle, float gyro_Y) 
{
  int PWM_out;
  PWM_out = Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
  return PWM_out;
} 

int Velocity(int Target,int encoder_left,int encoder_right)
{
  static int PWM_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
  float a=0.7;
  
  Encoder_Err = ((encoder_left+encoder_right)-Target);
  // low_out = (1-a)*Ek+a*low_out_last
  EnC_Err_Lowout = (1-a)*Encoder_Err + a*EnC_Err_Lowout_last;
  EnC_Err_Lowout_last = EnC_Err_Lowout;
  Encoder_S+=EnC_Err_Lowout;
  Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
  PWM_out = Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
  
  return PWM_out;
}

int Turn(int gyro_Z, int RC)
{
  int PWM_out;
  PWM_out = Turn_Kd*gyro_Z+Turn_Kp*RC;
  return PWM_out;
}
