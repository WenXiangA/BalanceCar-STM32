#ifndef __BLE_H
#define __BLE_H

#include "stm32f10x.h"

typedef enum {
	CarBalance = 0x00,
	CarFore = 0x01,
	CarBack = 0x02,
	CarLeft = 0x03,
	CarRight = 0x04,
}__CAR_DIR;

extern uint8_t CarDir;
extern u8 Cut_mode_flag;

typedef enum {
	SteerStop = 0x00,
	SteerLeft = 0x01,
	SteerRight = 0x02,
	SteerUp = 0x03,
	SteerDown = 0x04,
}__STEER_DIR;

extern uint8_t SteerDir;
extern uint8_t CarDir;
extern volatile uint8_t SPEED_X;

void Usart2_Config(uint32_t brr);
void Usart2_SendDate(uint8_t data);
void Usart2_SendString(char str[]);
void Blue_SendCMD(char cmd[]);
void Blue_SetName(char name[]);
void Ble_Init(void);

void Blue_count(void);


#endif
