

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"



//AC _zero Handler (detect both rising and falling edge)
void extint0()   interrupt 0 using 1
{

  
           if(AC_ZERO_PIN == 1)
                         
               {
								 
								 
                  AcVoltagePhaseReset=1;                     // Synchronize phase angle with AC rising edge  
									AcFirePos=0;
								 InvertAcFireNeg=1;
								 AcFireNeg=1;
								 InvertAcFirePos=0;
								 
								 
               }
							 else 
							 {
								 AcFirePos=1;
								  InvertAcFirePos=1;
								 InvertAcFireNeg=0;
								 AcFireNeg=0;
							 }
          
//------------------------------------------------------------//    
                    
			
//------------------------------------------------------------//  
           IocIsrTicker++;
//------------------------------------------------------------// time flow for control            
        
//------------------------------------------------------------//  
   //      Get_Init_Angle();
//------------------------------------------------------------//  State Control
           if((IocFlag == ON) && (SynFlag == OFF) && (NewSteadyCtrlFlag == OFF))  // AcPhaseAngle StartUp
           {
                 TriacPosAngle = InitialAngle;                // After SynFlag is set, switch to constant Timer1 value
           }
           else if((IocFlag == ON) && (SynFlag == ON) && (NewSteadyCtrlFlag == OFF))  // Timer1 Constant Value Control
           {
                 TriacPosAngle =InitialTriacPosAngle;          // After SynFlag is set, switch to constant Timer1 value
                 
                T2H = (65536-InitialTriacPosAngle)/256;                       // Period =16666us;
                T2L = (65536-InitialTriacPosAngle)%256;                       //

              
           }
           else if((IocFlag == ON) && (SynFlag == ON) && (NewSteadyCtrlFlag == ON))  // Timer1 PhaseLoop Control
           {
                  T2H = (65536-TriacPosAngle)/256;                       // Period = 16666us;
               T2L = (65536-TriacPosAngle)%256;                      //

                   
           }

           
}




//Hall1 signal Handler (detect rising edge only)
void extint1()   interrupt 2 using 1
{
	
	
	if (HALL1_PIN==0) Hall1Duration=0;
		 
 if  (HALL1_PIN==1)
		{
			Hall1MaxDuration=Hall1Duration;
			Hall1Duration=0;

//-------------------------------------------------------------// Get phase difference between AC zero and Hall
             RawPhaseErrorAcVsHall = AcVoltagePhase;
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


	
	

}


//Timer 1 for count phase angle of AC 

void tm1() interrupt 3 using 1
	

{
		if (Triac1Ticker<MaxTriggerPulse)
		{
		
			if (Trigger1On==1)
					{	
						
					TRIAC1_PIN=~TRIAC1_PIN;
					Triac1Ticker++;
					}
					else TRIAC1_PIN=0;
			}else 
				{
				TRIAC1_PIN=0;
				
				}
	
	if (Triac2Ticker<MaxTriggerPulse)		
		{

			if (Trigger2On==1)
			{
				TRIAC2_PIN=~TRIAC2_PIN;		
				Triac2Ticker++;
	
			} else TRIAC2_PIN=0;
		}else TRIAC2_PIN=0;
	
	if (AcVoltagePhaseReset==1) 
		{
			
			AcVoltagePhaseReset=0;
			AcVoltagePhase=0;
			AcMaxDuration=AcDuration;
			AcDuration=0;
			
		} else  
		
		{
			AcVoltagePhase += AC_PHASE_INC;
			AcDuration++;
		}
			
		
		
	 Hall1Duration++;;
	
            
  
//	if(StartLoseStepTickerFlag == ON)
//    {   
//      LoseStepTicker++; 
//     }   
	  
	
}




//Timer 2 for TRIAC post angle delay

void tm2() interrupt 12 using 1
{
 Timer2OverFlowTicker++; 
//------------------------------------------------------------//    
          if(StartStopCtrl == ON)
          {
              if((IocFlag == ON) && (SynFlag == ON)) 
              {
                    if((AC_ZERO_PIN == 1) && (HALL1_PIN == 1))       //AcRisingEdgeFlag
                    {
            //            Enable_Triac1();
                    }
                    else if((AC_ZERO_PIN == 0) && (HALL1_PIN == 0))  //AcRisingEdgeFlag
                    {
//                       Enable_Triac1();
                    }
                    else
                    {
//                        Disable_Triac();
                    }
              }
          }
          else
          {
//                     Disable_Triac();
          }


}

