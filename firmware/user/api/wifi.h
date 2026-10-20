#ifndef __WIFI_H
#define __WIFI_H

#include "stm32f10x.h"


typedef struct {
	uint8_t recvData[256];
	uint8_t recvCount;
	uint8_t recvOver;
	uint8_t connectFlag;
}__WIFI_TYPE;

extern __WIFI_TYPE wifi;

void Usart3_Config(uint32_t brr);
void WIFI_Analysis(void);



#endif
