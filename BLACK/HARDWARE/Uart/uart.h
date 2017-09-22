#ifndef  __UART_H__
#define  __UART_H__

#include <string.h>
#include <stdio.h>
#include  "STC15F2K60S2.h"
#include  "def.h"	    		  //宏定义 常用函数

#define T_1  1
#define T_12 12
#define Double_Baudrate	 1
#define Single_Baudrate	 2

void UartInit(void);
void UartSendStr(u8 *str);
void UartSendByte(u8 dat);
void UartSend(u8 *str ,u8 len);
#endif