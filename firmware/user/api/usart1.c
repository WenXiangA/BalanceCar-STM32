#include "usart1.h"
//TX--PA9  RX--PA10
void USART1_Config(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//USART1 1+8+0+1 9600
	USART_InitTypeDef USART_InitSturct = {0};
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_InitSturct.USART_BaudRate = brr;
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitSturct.USART_Parity = USART_Parity_No;
	USART_InitSturct.USART_StopBits = USART_StopBits_1;
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitSturct);
	
	USART_Cmd(USART1,ENABLE);
}


int fputc(int ch,FILE *fp)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1,ch);
	
	return ch;
}

