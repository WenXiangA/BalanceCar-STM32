#ifndef _MAIN_H_
#define _MAIN_H_

#include "stdio.h"
#include "stm32f10x.h"
#include "IO_BIT.h"




//关闭JTAG功能，保留SWD功能
inline void JTAG_SWDConfig(void)
{
	//打开AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//关闭JTAG功能，打开SWD功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}
#endif
