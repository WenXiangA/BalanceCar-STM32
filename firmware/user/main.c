#include "main.h"
#include "delay.h"
#include "LED.h"
#include "usart1.h"
#include "oled.h"
#include "motor.h"
#include "encoder.h"
#include "ble.h"
#include "inv_mpu.h"
#include "mpu6050.h"
#include "mpuiic.h"
#include "control.h"
#include "steer.h"
#include "wifi.h"
#include "sr04.h"

int LED_Flag = 0;

void LED_Task(void);

void ENC_Task(void);

extern const unsigned char gImage_1[1024];
float x,y,z;
int main(void)
{
	JTAG_SWDConfig();
	SysTick_Init(72000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Config();
	USART1_Config(115200);
	Motor_Init();
	Encoder_Init();
	OLED_Init();
	Ble_Init();
	MPU_Init();
	mpu_dmp_init();
	Steer_Init();
	Usart3_Config(9600);
	Sr04_Init();
//	Motor_PWMLoad(-7000,7000);
	

//	Usart2_SendString("AT\r\n");
	
	
	OLED_ShowPicture(0, 0, 128,64,(u8 *)gImage_1,0);


	OLED_Refresh();
	Delay_ms(3000);
	while(1)
	{
		
		
		  LED_Task();
		 if(BLUE_TIME > 30)
		 {
		   BLUE_TIME = 0;
			 Blue_count();
		 }
		 if(Control_TIME > 5)
		 {
		     Control_TIME = 0;
			   Control_Task();
		 }
		 if(Steer_TIME>50)
		 {
				Steer_TIME = 0;
			 Steer_Control();
			 if(Cut_mode_flag==1)
			 {
					WIFI_Analysis();
			 }
		 }
		 if(Sr04_TIME>50)
		 {
			 Sr04_TIME = 0;
			 if(Cut_mode_flag == 2)
				{
					Sr04_Mode();
				}
		 }
//		 if(MPU6050_TIME > 100)
//		 {
//				MPU6050_TIME = 0;
//			 mpu_dmp_get_data(&x,&y,&z);
//			 printf("x==%.1f y==%.1f z==%.1f\r\n",x,y,z);
//		 }
//		 ENC_Task();
//		 LED_ON(GPIOC,LED1_pin);
//		 Delay_ms(300);
//		 LED_OFF(GPIOC,LED1_pin);
//		 Delay_ms(300);n2
		
		
		
//		LED_ON(GPIOC,LED1_pin);
//		Delay_ms(500);
//		LED_OFF(GPIOC,LED1_pin);
//		Delay_ms(500);
//		LED_ON(GPIOC,LED2_pin);
//		Delay_ms(1000);
//		LED_OFF(GPIOC,LED2_pin);
// 		Delay_ms(1000);
	}
}	
void LED_Task(void)
{
    if(LED_Time[0] > LED_Time[1])
		{
		    LED_Time[0] = 0;
			  LED_Flag = !LED_Flag;
			  
		
		 if(LED_Flag == 1)
		{
		    //LED_ON(GPIOC,LED1_pin);
			  LED_ON(GPIOC,LED2_pin);
		}
		else if(LED_Flag == 0)
		{
		    //LED_OFF(GPIOC,LED1_pin);
			  LED_OFF(GPIOC,LED2_pin);
		}
	}
}


void ENC_Task(void)
{
    if (ENC_Time[0] > ENC_Time[1])
		{
		    ENC_Time[0] = 0;
			  Encoder_ReadSpeedTest();
		}
}
