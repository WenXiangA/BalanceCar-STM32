#include "ble.h"
#include "led.h"
#include "stdio.h"
#include "motor.h"
#include "string.h"
#include "oled.h"
#include "encoder.h"

uint8_t CarDir = CarBalance;
uint8_t SteerDir = SteerStop;
volatile uint8_t SPEED_X = 10;


char Blue_buf[2] = {0};
int Blue_leng = 0;
u8 Blue_flag = 0;
void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
	{
		data = USART2->DR;
		USART1->DR = data;
		Blue_buf[Blue_leng++] = data;
//		switch(data)
//		{
//			case 0x05: LED_OFF(GPIOC,LED1_pin);break;
//			case 0x06: LED_ON(GPIOC,LED1_pin);break;
//			
////			default: CarDir = CarBalance; SteerDir = SteerStop; break;
//		}
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_IDLE) == SET)
	{
		data = USART2->DR;
		Blue_flag = 1;
	}
}
extern const unsigned char gImage_1[1024];
u8 Cut_mode_flag = 0;
void Blue_count(void)
{
    if(Blue_flag == 1)
		{
		    Blue_buf[1] = '\0';
			  switch(Blue_buf[0])
				{

					case 0x0D: 
						OLED_Clear();
						LED_OFF(GPIOC,LED1_pin);
					  Cut_mode_flag = 0;
						OLED_ShowString(0,80,(u8 *)"LYMODE",16,1);
						OLED_Refresh();
					break;
			    case 0x0E: 
						OLED_Clear();
						LED_ON(GPIOC,LED1_pin);
					  Cut_mode_flag = 1;
					  OLED_ShowString(0,80,(u8 *)"WXMODE",16,1);
						OLED_Refresh();
					break;
					case 0x11: 
//						LED_ON(GPIOC,LED1_pin);
						OLED_Clear();
					  Cut_mode_flag = 2;
					  OLED_ShowString(0,80,(u8 *)"BZMODE",16,1);
						OLED_Refresh();
					break;
					case 0x00:if(Cut_mode_flag == 0)CarDir = CarBalance; break;
					case 0x01:if(Cut_mode_flag == 0)CarDir = CarFore; break;
					case 0x02:if(Cut_mode_flag == 0)CarDir = CarBack; break;
					case 0x03:if(Cut_mode_flag == 0)CarDir = CarRight; break;
					case 0x04:if(Cut_mode_flag == 0)CarDir = CarLeft; break;
					case 0x05: SteerDir = SteerStop; break;
					case 0x06: SteerDir = SteerLeft; break;
					case 0x07: SteerDir = SteerRight; break;
					case 0x08: SteerDir = SteerUp; break;
					case 0x09: SteerDir = SteerDown; break;
					case 0x0A: SPEED_X = 10; break;
					case 0x0B: SPEED_X = 15; break;
					case 0x0C: SPEED_X = 20; break;
					case 0x0F:
						// display team info
						OLED_Clear();
						break;
					case 0x10:
						// display team info
						OLED_Clear();
						break;
					case 0x12:
						// display team info
						OLED_Clear();
						break;
					case 0x13:
						// display team info
						OLED_Clear();
						break;
					case 0x14:
					OLED_Clear();
					OLED_ShowPicture(0, 0, 128,64,(u8 *)gImage_1,0);
					break;
					case 0x15:
					OLED_Clear();
					break;
					default: CarDir = CarBalance; SteerDir = SteerStop; break;
//				  case 0x00:
//					Motor_PWMLoad(1000,1000);
//					break;
//					case 0x01:
//					Motor_PWMLoad(-1000,-1000);
//					break;
//					case 0x02:
//					Motor_PWMLoad(-1000,1000);
//					break;
//					case 0x03:
//					Motor_PWMLoad(1000,-1000);
//					break;
//					case 0x04:
//					Motor_PWMLoad(0,0);
//					break;
//					case 0x05:
//					LED_OFF(GPIOC,LED1_pin);
//					break;
//					case 0x06:
//					LED_ON(GPIOC,LED1_pin);
//					break;
//					case 0x07:
//					OLED_Clear();
//					break;
//					case 0x08:
//					{
//							int32_t deltaA = CaptureA_Count - LastA_Count;
//							int32_t deltaB = CaptureB_Count - LastB_Count;

//							int spdA = (int)(ShaftA_Speed * 10);
//							int spdB = (int)(ShaftB_Speed * 10);
//							int outA = (int)(ShaftA_Speed / 34.0f * 10);
//							int outB = (int)(ShaftB_Speed / 34.0f * 10);


//							OLED_ShowChineseStr(0, 0, (u8*)buf, 16, 1);

//							OLED_ShowChineseStr(0, 16, (u8*)buf, 16, 1);

//							OLED_ShowChineseStr(0, 32, (u8*)buf, 16, 1);

//							OLED_ShowChineseStr(0, 48, (u8*)buf, 16, 1);

//					}
//					case 0x09:
//					OLED_Clear();
//					break;
//					case 0x0A:
//					OLED_Clear();
//					break;

				}
		}
		Blue_flag = 0;
		Blue_leng = 0;
		memset(Blue_buf,0,sizeof(Blue_buf));
}


void Ble_Init(void)
{
	Usart2_Config(9600);
}


void Usart2_Config(uint32_t brr)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitTypeDef USART_InitStructuer;
	USART_InitStructuer.USART_BaudRate = brr;
	USART_InitStructuer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructuer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructuer.USART_Parity = USART_Parity_No;
	USART_InitStructuer.USART_StopBits = USART_StopBits_1;
	USART_InitStructuer.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructuer);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART2, ENABLE);
}


void Usart2_SendDate(uint8_t data)
{
	USART_SendData(USART2, data);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{}
}


void Usart2_SendString(char str[])
{
	uint16_t i = 0;
	while(str[i] != '\0') 
	{
		Usart2_SendDate(str[i]);
		i++;
	}
}


void Blue_SendCMD(char cmd[])
{
	Usart2_SendString(cmd);
}


void Blue_SetName(char name[])
{
	char buff[32] = {0};
	sprintf(buff,"AT+NAME%s\r\n", name);
	Usart2_SendString(buff);
}
