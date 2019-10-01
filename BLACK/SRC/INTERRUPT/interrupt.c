

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"


/**************************************************************************************************
;Hall2 Hall Signal(detect rising edge only)
;
;increase H2FireAngle if edge drop before H2Rebuild 
;
;decrease H2FireAngle if edge drop after H2Rebuild
;
;
**************************************************************************************************/


void extint0()   interrupt 0 using 1
{					
			if  (H2_PIN==1)                  //rising edge of H1 signal
			{
			H2RisingEdgeDetect=1;  
				H2Phase=0;
			H2PhaseRiseEdge=AcPhasePrecise ;	
			}
		
				
}




//H1 Hall signal  (detect both rising and falling edge )
void extint1()   interrupt 2 using 1
{
		if  (H1_PIN==1)                  //rising edge of H1 signal
			{
			H1RisingEdgeDetect=1;  
					H1Phase=0;
			H1PhaseRiseEdge=AcPhasePrecise ;	
			}
			
					test[testPtr]=H2PhaseRiseEdge;
				testPtr++;
				
			
			
			
		
			
			

				
}





//AC Handler (detect  falling edge)

void extint2()   interrupt 10 using 1
{
	
//																	//At each start of AC cycle, refresh the trigger position
					TH0 =(65536-ACCounterWidth)/256;								//reset  timer0
					TL0 = (65536-ACCounterWidth)%256;
					TR0 =1;			
					ET0 =1;			 
					  AcFallingEdgeDetect=1;
					AcPhase=0;
       
           DelayTimer++;
				
}




//Timer 0 for TRIAC pulse width  100us
void tm0() interrupt 1 using 1
{
	 
		AcIncFlag=1;
		H1PhaseIncFlag=1;
	
	
	
/**************************************************************************************************
;both Triac1 and Triac2 trigger at 100us pulse interval 
;
;totaly no of pulse = MaxTriggerPulse/2
;
;
*********************************************************************************************/
	
		if (Triac1Ticker<MaxTriggerPulse)
			{
					if (FirePower1==1)
							{				
							TRIAC1_PIN=~TRIAC1_PIN;
							Triac1Ticker++;
							}
				} 
		
		if (Triac2Ticker<MaxTriggerPulse)		
			{
				if (FirePower2==1)
				{
				TRIAC2_PIN=~TRIAC2_PIN;		
				Triac2Ticker++;
				}  
			}
	
			
/**************************************************************************************************
;AcDuration and H1Duration is counted every 100us
;At each rising edge of Ac or H1, it will update the numnber of pulse counted
;For example, AC is 50Hz and 20ms each rising edge interval
;number of pulse count should be 200, so AcPeriodCount =200
;using 16bit to represent the full AC period, that is 65536/200=327
;each 100us interval should add 327 to the AcPhase, 327 *200 is 65400 which is the value of Full period
;
*********************************************************************************************/
			
			
		if (AcFallingEdgeDetect==1) 
			{
				AcFallingEdgeDetect=0;
				
				AcPeriodCount=(AcDuration+AcPeriodCount)>>1 ;    
				AcDuration=1;
			} else  AcDuration++;

			
		if (H1RisingEdgeDetect==1) 
			{
			
				H1RisingEdgeDetect=0;
				H1PeriodCount=(H1PeriodCount+H1Duration)>>1  ;
				
				H1Duration=1;
			} 
			else  
			{
						
/**************************************************************************************************
;Calculation of H1FullPhase takes time and will affect the trigger pulse
;
;The calcution is divided to 4 steps at H1Duration =0,10,20,30 
;
;
;
;
**************************************************************************************************/			
				H1Duration++;

			}
	
	
}


//Timer 1 for count phase angle of AC 100us

void tm1() interrupt 3 using 1
	

{
	

	
}








//Timer 2 for TRIAC post angle delay

void tm2() interrupt 12 using 1
{



}


void uart_int(void) interrupt 4 using 1
{ 
static unsigned char tmp,i;
static signed char int_tmp;    
	
	if (RI) 
    {

      tmp = SBUF;

	  if (tmp!=13) 
		{
			UartData[UartArrayPtr]=tmp;					//pack received character to UartData
			if (UartArrayPtr<UartDataLen) UartArrayPtr++;
			 else UartArrayPtr=0;
			 int_tmp=tmp-48;
			if ((int_tmp>=0)&&(int_tmp<=9))
			 UartRecIntFlag=1;
			
			
		}
		else
		{
	  for (i=UartArrayPtr;i<UartDataLen;i++)
				UartData[i] = (char)0;
		
		UartArrayPtr=0;
		UartRecFlag=1;
		}

      RI = 0;
    }
}  


   




