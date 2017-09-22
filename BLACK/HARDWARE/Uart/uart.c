

#include "uart.h"


void UartInit(void)		
{
       SCON=0x50;        
	   TMOD= 0x00;                      
	   AUXR=0X40;		 
       TL1=(65535-(11059200/4/9600));    
	   TH1=(65535-(11059200/4/9600))>>8;
	   TR1  = 1;        
	   ES   = 1;                                                                            
	   EA   = 1;        	
	
}

void UartSendByte(u8 dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void UartSendStr(u8 *str)
{
	while(*str)
	{
		UartSendByte(*str);
		str++;
	}
}

void UartSend(u8 *str ,u8 len)
{
	while(len--)
	{
		UartSendByte(*str);
		str++;
	}
}


//更新PWM占空比
void Setup_pwm(u8 sbufp)
{

  CCAP0H=CCAP0L=sbufp;			 //更新PWM占空比

  UartSendStr("ok\r\n");

}


/*****************************************************************************/
		 


void UART0_Interrupt(void) interrupt 4 using 1
{
	u8 SbufTemp;
	
	if(RI)
	{
		RI = 0;
		SbufTemp = SBUF;

	    Setup_pwm(SbufTemp);
	}
}

 


