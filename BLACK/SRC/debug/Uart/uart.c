

#include "uart.h"



void UartSendByte(uint8 dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void UartSendStr(uint8 *str)
{
	while(*str)
	{
		UartSendByte(*str);
		str++;
	}
}

void UartSend(uint8 *str ,uint8 len)
{
	while(len--)
	{
		UartSendByte(*str);
		str++;
	}
}


//更新PWM占空比
void Setup_pwm(uint8 sbufp)
{

  CCAP0H=CCAP0L=sbufp;			 //更新PWM占空比

  UartSendStr("ok\r\n");

}


/*****************************************************************************/
		 


void UART0_Interrupt(void) interrupt 4 using 1
{
	uint8 SbufTemp;
	
	if(RI)
	{
		RI = 0;
		SbufTemp = SBUF;

	    Setup_pwm(SbufTemp);
	}
}

 


