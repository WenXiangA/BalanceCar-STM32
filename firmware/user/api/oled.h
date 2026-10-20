#ifndef __OLED_H
#define __OLED_H 

#include "stdlib.h"	 
#include "stm32f10x.h" 
#include "delay.h"
#include "string.h"

#define IIC   1U
#define SPI   0U

#define MODE  SPI      


//----------------OLED引脚定义-----------------
#define OLED_SCL_RCC     RCC_APB2Periph_GPIOB
#define OLED_SCL_Pin     GPIO_Pin_13
#define OLED_SCL_GPIO    GPIOB

#define OLED_SDA_RCC     RCC_APB2Periph_GPIOB
#define OLED_SDA_Pin     GPIO_Pin_15
#define OLED_SDA_GPIO    GPIOB

#define OLED_RES_RCC     RCC_APB2Periph_GPIOB
#define OLED_RES_Pin     GPIO_Pin_14
#define OLED_RES_GPIO    GPIOB

#define OLED_DC_RCC     RCC_APB2Periph_GPIOB
#define OLED_DC_Pin     GPIO_Pin_12
#define OLED_DC_GPIO    GPIOB

#define OLED_CS_RCC     RCC_APB2Periph_GPIOC
#define OLED_CS_Pin     GPIO_Pin_7
#define OLED_CS_GPIO    GPIOC


//-----------------OLED端口定义---------------- 

#define OLED_SCL_Clr() GPIO_ResetBits(OLED_SCL_GPIO,OLED_SCL_Pin)//SCL
#define OLED_SCL_Set() GPIO_SetBits(OLED_SCL_GPIO,OLED_SCL_Pin)

#define OLED_SDA_Clr() GPIO_ResetBits(OLED_SDA_GPIO,OLED_SDA_Pin)//DIN
#define OLED_SDA_Set() GPIO_SetBits(OLED_SDA_GPIO,OLED_SDA_Pin)

#define OLED_RES_Clr() GPIO_ResetBits(OLED_RES_GPIO,OLED_RES_Pin)//RES
#define OLED_RES_Set() GPIO_SetBits(OLED_RES_GPIO,OLED_RES_Pin)

#define OLED_DC_Clr()  GPIO_ResetBits(OLED_DC_GPIO,OLED_DC_Pin)//DC
#define OLED_DC_Set()  GPIO_SetBits(OLED_DC_GPIO,OLED_DC_Pin)

#define OLED_CS_Clr()  GPIO_ResetBits(OLED_CS_GPIO,OLED_CS_Pin)//CS
#define OLED_CS_Set()  GPIO_SetBits(OLED_CS_GPIO,OLED_CS_Pin)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


//正常显示，还是反转显示
#define NORMAL    1u
#define REVERSE   0u


void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawRectangle(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 *cha,u8 size,u8 mode);
void OLED_ShowChineseStr(u8 x,u8 y,u8 *buff,u8 size,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_GPIO_Config(void);
void OLED_Init(void);

#endif

