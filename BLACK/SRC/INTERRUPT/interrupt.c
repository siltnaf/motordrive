

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"



//AC _zero Handler (detect both rising and falling edge)
void extint0()   interrupt 0 using 1
{

	
 if(StartStopCtrl == ON)
       {
           if(AC_ZERO_PIN == 1)
           {                
               {
                   AcVoltagePhase = 0;                      // Synchronize phase angle with AC rising edge  
								 
               }
           }
//------------------------------------------------------------//    
          Disable_Triac();             
					 TriggerProcessed=0;
//------------------------------------------------------------//  
           IocIsrTicker++;
//------------------------------------------------------------// time flow for control            
           if(IocIsrTicker > 36)                              // 300ms 
           {
                 IocFlag = ON;                                // Start Open Loop with phase angle trigger 
                                                              // used for Power On
                Enable_Triac1();
                 
                
           }
//------------------------------------------------------------// 
           if(IocIsrTicker >= 60)                              // 500ms 
           {
                 SynFlag = ON;                                // Start Open Loop with Timer2 Trigger  
           }
           if(IocIsrTicker >= 90)                              // 750ms
           {
                 IocIsrTicker = 90;                           // Start Phase Closed Loop with Timer2 Trigger 
                 NewSteadyCtrlFlag = ON;
           }
//------------------------------------------------------------//  
           Get_Init_Angle();
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
}




//Hall1 signal Handler (detect rising edge only)
void extint1()   interrupt 2 using 1
{
	

 if  (HALL1_PIN==1)
		{
		Exti2IsrTicker++;
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
             if(NewSteadyCtrlFlag == ON)
             {      
                   StartLoseStepTickerFlag = ON;           // Enable LooseStepTicker starts to count 

							                    if(LoseStepTicker > 100)                // Check LoseStepTicker 100@5kHz, 200@10kHz (50Hz,20ms)
                   {
                      RestartFlag = ON;
                   }

                   LoseStepTicker = 0;                     // Clear LoseStepTicker
             }  


					 }
}

//Timer 0 for TRIAC pulse width  100us
void tm0() interrupt 1 using 1
{


	
	if ((TriggerOn==1)	&&(TriggerProcessed==0))
		
	{

TriacTicker++;
	
	}
	
	
	
	if (TriacTicker>MaxTriggerPulse) 
	{
	
			TriacTicker=0;
			TriggerProcessed=1;
			
	}

}


//Timer 1 for count phase angle of AC 

void tm1() interrupt 3 using 1
{

							TimerOneIsrTicker++;
               AcVoltagePhase += AC_PHASE_INC;
              (AcVoltagePhase < 32767)? (AcZeroSignal = 1):(AcZeroSignal = 0);
               
               //TEST_PA2_PIN6 = TRIAC_PIN;              //AcZeroSingal;
               //PB_ODR_bit.ODR4 = TRIAC_PIN;         //AcZeroSingal;//TimerOneIsrTicker & 0x01;
//------------------------------------------------------------//    
                      
//------------------------------------------------------------//  

               if(StartStopCtrl == ON)
               { 
                    if((IocFlag == ON) && (SynFlag == OFF) && (NewSteadyCtrlFlag == OFF))  // PhaseAngle StartUp
                    {   
                        Run_Motor(); 
                    }
                    
                  
               }
               else
               {
                         Disable_Triac();
               }  

//------------------------------------------------------------//    
               Virtual_Timer();                                   // virtual timer application
//------------------------------------------------------------//     Check Hall rising and falling edge to just motor wheather is run
              // FilteredPhaseErrorAcVsHall += (PhaseErrorAcVsHall - FilteredPhaseErrorAcVsHall) >> K_LOW_PASS_FILTER;
              // AdcResult = 1020;
              // Filtered_Ac_AdResult += (AdcResult - Filtered_Ac_AdResult) >> 3;

              // TEST02_GPIO = AcZeroSingal;
//------------------------------------------------------------//    
               if(StartLoseStepTickerFlag == ON)
               {   
                     LoseStepTicker++; 
               }   
							  Disable_Triac();
	
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
                         Enable_Triac1();
                    }
                    else if((AC_ZERO_PIN == 0) && (HALL1_PIN == 0))  //AcRisingEdgeFlag
                    {
                       Enable_Triac1();
                    }
                    else
                    {
                        Disable_Triac();
                    }
              }
          }
          else
          {
                     Disable_Triac();
          }


}

