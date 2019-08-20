

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"



//AC Handler (detect both rising and falling edge)
void extint0()   interrupt 0 using 1
{					
	
					FireSeq=refresh;																//At each start of AC cycle, refresh the trigger position
			   
					 AcEdgeDetect=1;
           if(AC_PIN == 1)
                      
               {							 
                AcRisingEdgeDetect=1;                     // Synchronize phase angle with AC rising edge  
								AcActual=1;																// for test AcRebuild and actual AC waveform difference			
											 
               }
							 else 
							 {
								AcActual=0;
								temp=AcPhasePrecise;
							 }
           DelayCount++;
				
}




//H1 Hall signal  (detect both rising and falling edge )
void extint1()   interrupt 2 using 1
{
		if  (H1_PIN==1)                  //rising edge of H1 signal
			{
			H1RisingEdgeDetect=1;  
					
			}
			else													//falling edge of H1 signal
			{					
				
				
									
/************************************************************************
;                           -------
;          AC              |       |
;             -------------         ----------------------
;
;                               -------
;         H1                   |       |
;                 -------------         ----------------------
;
;                 Targetphase is between falling edge of AC and falling edge of H1 
;	
;************************************************************************/
				
				
// caputre falling edge of  H1
				
				H1PhaseFallEdge=AcPhasePrecise;
				
//					
//			 if (AcPhasePrecise > AcHalfPhase)
//			 {
//			PID_Error = TargetAcH1 - (AcPhasePrecise-AcHalfPhase);
			
//			 }
//			 else 
//			 {
//			PID_Error=-(AcPhasePrecise+(AcHalfPhase-TargetAcH1)); 
				
//			 }
				}
				
				
				
			
			
			
		
			
			

				
}



/**************************************************************************************************
;Hall2 Hall Signal(detect falling edge only)
;
;increase H2FireAngle if edge drop before H2Rebuild 
;
;decrease H2FireAngle if edge drop after H2Rebuild
;
;
**************************************************************************************************/
void extint2()   interrupt 10 using 1
{
	
		// apply seperate H2Fire angle if speed is below  max speed
	
	  if (MaxSpeedFlag==0)
		{
			if (H2Rebuild==1)
			H2FireAngle+=((H1HalfPhase+(H1HalfPhase>>1)-H1Phase)>>4);
			else H2FireAngle-=((H1Phase-(H1HalfPhase+(H1HalfPhase>>1)))>>4);
			if (H2FireAngle>TargetFireAngle) H2FireAngle=TargetFireAngle;
			if (H2FireAngle<InitFireAngle) H2FireAngle=InitFireAngle;
		}
		else H2FireAngle=TargetFireAngle;
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
			
			
		if (AcRisingEdgeDetect==1) 
			{
				AcRisingEdgeDetect=0;
				AcPhase=0;
				AcPeriodCount=(AcDuration+AcPeriodCount)>>1 ;    
				AcDuration=0;
			} else  AcDuration++;

			
		if (H1RisingEdgeDetect==1) 
			{
				H1Phase=0;
				H1RisingEdgeDetect=0;
				H1PeriodCount=(H1PeriodCount+H1Duration)>>1  ;
				H1Duration=0;
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
				if (H1Duration==10)  H1PhaseInc=65535/H1PeriodCount;
				if (H1Duration==20)  H1FullPhase=(H1PhaseInc*H1PeriodCount);
				if (H1Duration==30)  H1HalfPhase=H1FullPhase>>1;
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
			if ((int_tmp<0)||(int_tmp>9))
			 UartRecIntFlag=0;
			
			
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


   




