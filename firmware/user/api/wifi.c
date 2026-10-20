#include "wifi.h"
#include "ble.h"
#include "control.h"
#include "string.h"
#include "steer.h"

__WIFI_TYPE wifi = {0};

void USART3_IRQHandler(void)
{
	uint8_t data = 0;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
	{
		data = USART3->DR;
		if(wifi.recvOver == 0) {
			if(wifi.recvCount < sizeof(wifi.recvData))
				wifi.recvData[wifi.recvCount++] = data;
		}
	}
	if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) == SET)
	{
		data = USART3->DR;
		wifi.recvOver = 1;
	}	
}


void Usart3_Config(uint32_t brr)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitTypeDef USART_InitStructuer;
	USART_InitStructuer.USART_BaudRate = brr;
	USART_InitStructuer.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructuer.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructuer.USART_Parity = USART_Parity_No;
	USART_InitStructuer.USART_StopBits = USART_StopBits_1;
	USART_InitStructuer.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructuer);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(USART3, ENABLE);
}


void WIFI_Analysis(void)
{
	uint8_t check = 0;
	if(wifi.recvOver == 0)	return;
	if(wifi.recvCount < 6)	goto err;
	if(wifi.recvData[0] != 0xAA || wifi.recvData[1] != 0x55)	goto err;
	check = wifi.recvData[0] + wifi.recvData[1] + wifi.recvData[2] + wifi.recvData[3] + wifi.recvData[4];
	if(check != wifi.recvData[5])	goto err;
//	for(uint8_t i=0; i<wifi.recvCount; i++)
//	{
//		printf("%02X\t", wifi.recvData[i]);
//	}
//	printf("\r\n");
	wifi.connectFlag = 1;
	switch(wifi.recvData[2])
	{
		case 0x00: CarDir = CarBalance; break;
		case 0x01: CarDir = CarFore; break;
		case 0x02: CarDir = CarBack; break;
		case 0x03: CarDir = CarRight; break;
		case 0x04: CarDir = CarLeft; break;
		default:CarDir = CarBalance;break;
	}
	switch(wifi.recvData[3])
	{
		case 0x05: SteerDir = SteerStop; break;
		case 0x06: SteerDir = SteerRight; break;
		case 0x07: SteerDir = SteerLeft; break;
		case 0x08: SteerDir = SteerUp; break;
		case 0x09: SteerDir = SteerDown; break;
		default:SteerDir = SteerStop;break;
	}
	switch(wifi.recvData[4])
	{
		case 0x0A: SPEED_X = 10; break;
		case 0x0B: SPEED_X = 15; break;
		case 0x0C: SPEED_X = 20; break;
		default: SPEED_X = 10; break;		
	}
	
	err:
		memset(wifi.recvData, 0, 256);
		wifi.recvCount = 0;
		wifi.recvOver = 0;
}


