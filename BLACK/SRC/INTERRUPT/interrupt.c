

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"



//AC _zero Handler (detect both rising and falling edge)
void extint0()   interrupt 0 using 1
{

					
           if(AC_ZERO_PIN == 1)
                      
               {							 
                AcRisingEdgeDetect=1;                     // Synchronize phase angle with AC rising edge  
								AcActual=1;			
								AcPhase=0;								 
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
			H1Phase=0;
			
//-------------------------------------------------------------// Get phase difference between AC zero and Hall
             RawPhaseErrorAcVsHall = AcPhase;
             if(RawPhaseErrorAcVsHall > 32767 )             // Phase advance
             {
                   PhaseErrorAcVsHall = -(65535 - RawPhaseErrorAcVsHall);
             }
             else
             {
                   PhaseErrorAcVsHall =  RawPhaseErrorAcVsHall;
             }      
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




//Hall2 signal Handler (detect falling edge only)
void extint2()   interrupt 10 using 1
{
		 

	
}











//Timer 0 for TRIAC pulse width  100us
void tm0() interrupt 1 using 1
{

	TR0=0;
	TF0=0;
	

	

}


//Timer 1 for count phase angle of AC 100us

void tm1() interrupt 3 using 1
	

{
	
		AcIncFlag=1;
		H1PhaseIncFlag=1;
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
	
	if (AcRisingEdgeDetect==1) 
		{
			
			AcRisingEdgeDetect=0;
			
			AcPeriodCount=(AcDuration+AcPeriodCount)>>1 ;    
			AcDuration=0;
			
		} else  
		

			AcDuration++;

			
			if (H1RisingEdgeDetect==1) 
		{
			
			H1RisingEdgeDetect=0;
			
			H1PeriodCount=(H1PeriodCount+H1Duration)>>1  ;
			H1PhaseInc=65535/H1PeriodCount;
			H1FullPhase=(H1PhaseInc*H1PeriodCount);
			H1HalfPhase=H1FullPhase>>1;
			H1Duration=0;
			
		} else  
		

			H1Duration++;
		
	
	
            
  
//	if(StartLoseStepTickerFlag == ON)
//    {   
//      LoseStepTicker++; 
//     }   
	  
	
}




//Timer 2 for TRIAC post angle delay

void tm2() interrupt 12 using 1
{



}

