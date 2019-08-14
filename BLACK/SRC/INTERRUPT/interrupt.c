

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
								AcActual=0;
							
						 
							 
           
			
//------------------------------------------------------------//  
           DelayCount++;
//------------------------------------------------------------// time flow for control            
//						if (SynUpdate==0)
//						{
//							SynTicker++;
//							if (SynTicker>SynSpeed)
//							{
//								SynTicker=0;
//								SynUpdate=1;
//							}
//						}

//           
//						if (current_state==MaxSteady)
//						{
//						
//						TH0 =(65536-TriacPosTime)/256;
//						TL0 = (65536-TriacPosTime)%256;
//						TR0=1;
//						TF0=0;
//						}
}




//H1 signal Handler (detect both rising and falling edge )
void extint1()   interrupt 2 using 1
{

	
 
		 
 if  (H1_PIN==1)
		{
			H1RisingEdgeDetect=1;  

			
//-------------------------------------------------------------// Get phase difference between AC zero and Hall
//             RawPhaseErrorAcVsHall = AcPhase;
//             if(RawPhaseErrorAcVsHall > 32767 )             // Phase advance
//             {
//                   PhaseErrorAcVsHall = -(65535 - RawPhaseErrorAcVsHall);
//             }
//             else
//             {
//                   PhaseErrorAcVsHall =  RawPhaseErrorAcVsHall;
//             }      
//------------------------------------------------------------// Motor lose step judgement algorithm
//             if(NewSteadyCtrlFlag == ON)
//             {      
//                   StartLoseStepTickerFlag = ON;           // Enable LooseStepTicker starts to count 

//							                    if(LoseStepTicker > 100)                // Check LoseStepTicker 100@5kHz, 200@10kHz (50Hz,20ms)
//                   {
//                      RestartFlag = ON;
//                   }

//                   LoseStepTicker = 0;                     // Clear LoseStepTicker
//             }  


					 }
}



/**************************************************************************************************
;Hall2 signal Handler (detect falling edge only)
;
;increase H2FireAngle if edge drop before H2Rebuild 
;
;decrease H2FireAngle if edge drop after H2Rebuild
;
;
**************************************************************************************************/





void extint2()   interrupt 10 using 1
{


	
		if (H2Rebuild==1)
		 H2FireAngle+=((H1HalfPhase+(H1HalfPhase>>1)-H1Phase)>>4);
		else H2FireAngle-=((H1Phase-(H1HalfPhase+(H1HalfPhase>>1)))>>4);
		if (H2FireAngle>TargetFireAngle) H2FireAngle=TargetFireAngle;
		if (H2FireAngle<InitFireAngle) H2FireAngle=InitFireAngle;
	
}











//Timer 0 for TRIAC pulse width  100us
void tm0() interrupt 1 using 1
{


	

}


//Timer 1 for count phase angle of AC 100us

void tm1() interrupt 3 using 1
	

{
	
		AcIncFlag=1;
		H1PhaseIncFlag=1;
	
	
	
/**************************************************************************************************
;both Triac1 and Triac2 trigger at 100us pulse interval 
;
;totaly no of pulse = MaxTriggerPulse/2
;
;
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
	
	
            
  
//	if(StartLoseStepTickerFlag == ON)
//    {   
//      LoseStepTicker++; 
//     }   
	  
	
}




//Timer 2 for TRIAC post angle delay

void tm2() interrupt 12 using 1
{



}

