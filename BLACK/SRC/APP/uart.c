  
#
#include	"app.h"
#include "init.h"

const char Str_off[4]={'o','f','f','\0'};                                                
const char Str_cw[3]={'c','w','\0'};
const char Str_ccw[4]={'c','c','w','\0'};
const char Str_on[3]={'o','n','\0'};
//const char Str_state[6]={'s','t','a','t','e','\0'};

volatile uint8 store_state;       
volatile uint16 store_rpm; 

void UartSendByte(uint8 dat)
  {
			SBUF = dat;
			while(TI == 0);
			TI = 0;
   }
 
	
void UartSendStr(uint8 *s)
	{
		
	
	while (*s != '\0')
	{
		UartSendByte(*s);
		s++;
	}
	
	
}
	

int StrComp(uint8 *s, uint8 *t)
	{
		static uint8 match;
		
		match=1;
	while ((*s != '\0')||(*t !='\0'))
	{
		if (*s!=*t) match=0;
		s++;
		t++;
	}
	return match;
	
	
}
	
	
uint16 CharToInt(uint8 *s)
	{
	static int value;
		
		value=0;
	while (*s != '\0')
	{
		 value+=(*s-48);
			s++;
		 if (*s != '\0') value*=10;
	} 
	s=0;
	return value;
	
	
}
	
	
	

void Check_Uart()
{
static uint16 tmp_rpm;
	
			if	(store_state!=current_state)
						{
						 
							UartSendStr("Current state = ");
							switch (current_state)
								{
	
								case SystemOff:    UartSendStr("system off\n\r");
																		break;
								case Standby:      UartSendStr("standby\n\r");
																		break;
								case KickStart:		 UartSendStr("kick start\n\r");
																		break;				
								case NormalRun:		 UartSendStr("normal run\n\r");
																		break;			
								case FindSteadyPoint:	UartSendStr("find steady point\n\r");
																		break;		
								case SynMax: 				UartSendStr("syn max\n\r");
																		break;	
								}
								store_state=current_state;
					
						}
	
	
		if (UartRecFlag==1)
			{
			
				UartRecFlag=0;	
				  
				if	(StrComp(UartData,Str_off)==1)
						{
							UartSendStr("Current Status is off\n\r");
							if (current_state!=Standby) store_rpm=new_rpm;
							new_rpm=0;
							MaxSpeedFlag=0;
							current_state=Standby;
						}
				if	(StrComp(UartData,Str_on)==1)
						{
							UartSendStr("Current Status is running\n\r");
							new_rpm=store_rpm;
							current_state=Standby;
						}
				if	(StrComp(UartData,Str_cw)==1)
						{
							UartSendStr("Current direction is clockwise\n\r");
							direction=cw;
						}
				if	(StrComp(UartData,Str_ccw)==1)
						{
							UartSendStr("Current direction is anti-clockwise\n\r");
							direction=ccw; 
						}
		
				if (UartRecIntFlag==1)
						{
									UartSendStr("Rpm is set to ");
									UartSendStr(UartData);
									UartSendStr("\n\r");
									tmp_rpm=CharToInt(UartData);
									if ((tmp_rpm>0)&&(tmp_rpm<=3000))
										{
											new_rpm=tmp_rpm;
											current_state=Standby;
										}
										else 
											{
											new_rpm=0;
											MaxSpeedFlag=0;
											current_state=Standby;
											}
									UartRecIntFlag=0;
						}
		
		
		
			}
}
	
   
           

	 
 
