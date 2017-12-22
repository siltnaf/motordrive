
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		main.c
;Function:main
**************************************************************************************************/


#include  "stc15f2k60s2.h"		  //STC15头文件

#include  "init.h"
#include  "app.h"




/* Private variables definition ----------------------------------------------*/
//--------------------------------- // External Interrupt 
volatile uint8 xdata Exti2IsrTicker;  
volatile uint8 xdata Exti3IsrTicker;  
volatile uint8 xdata Timer2OverFlowTicker;
volatile uint8 xdata TimerOneIsrTicker; 

volatile uint8 xdata MainTicker; 
volatile uint8 xdata IocIsrTicker;
volatile uint8 xdata LoseStepTicker;
volatile uint8 xdata PosAcFlag;

volatile uint8 xdata IocFlag;
volatile uint8 xdata RestartFlag;
volatile uint8 xdata StartStopCtrl;
volatile uint8 xdata SynFlag;
volatile uint8 xdata HallEdgeFlag;
volatile uint8 xdata MotorStallFlag;
volatile uint8 xdata AdcTrigger;
volatile uint8 xdata NewSteadyCtrlFlag;
volatile uint8 xdata StartLoseStepTickerFlag;
volatile uint8 xdata Delay200usFlag;
volatile uint8 xdata Delay200usCounter;
volatile uint8 xdata AcZeroSignal;




volatile uint16 xdata AdcResult;
volatile uint16 xdata Ac_Voltage;

volatile uint16 xdata AcVoltagePhase;

volatile uint8 xdata AcVoltagePhaseReset;

volatile int16 xdata PhaseErrorAcVsHall;
volatile uint16 xdata RawPhaseErrorAcVsHall;
volatile int16 xdata FilteredPhaseErrorAcVsHall;
volatile uint8 xdata Triac1Ticker;
volatile uint8 xdata FireSet;
volatile uint8 xdata TriacReset;
volatile uint8 xdata Triac2Ticker;
volatile uint8 xdata TriggerProcessed;
volatile uint8 xdata Trigger1On;
volatile uint8 xdata Trigger2On;
volatile uint16 xdata TriacPosAngle;

volatile uint16 xdata InitialAngle;
volatile uint16 xdata MaxAngleLimit;
volatile uint16 xdata MinAngleLimit;
volatile uint16 xdata InitialTimer1Value;
volatile uint8 xdata Startup_Delay_Count;


volatile uint8 xdata AcFirePos;
volatile uint8 xdata AcFireNeg;
volatile uint8 xdata InvertAcFireNeg;

volatile uint8 xdata InvertAcFirePos;

volatile uint8 xdata FireZone;
volatile uint8 xdata FirePower1;
volatile uint8 xdata FirePower2;
volatile int16 xdata PID_Error;

volatile uint16 xdata Hall1MaxDuration;

volatile uint16 xdata Hall1Duration;

volatile uint8 xdata Hall1HalfFlag;
volatile uint8 xdata VirtualHall2;
volatile uint8 xdata InvertedAcSignal;



void Assign_Status_Flag()
{

   if((IocIsrTicker > 36)  &&    (IocIsrTicker<60))                   // 300ms 
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
					 
					 
		if (Hall1Duration<(Hall1MaxDuration>>1))	Hall1HalfFlag=1; else Hall1HalfFlag=0;			
	
					 
		if (((Hall1HalfFlag==0)&&(HALL1_PIN==1))||((Hall1HalfFlag==1)&&(HALL1_PIN==0))) VirtualHall2=0;else VirtualHall2=1;
					 
		if (AcZeroSignal==0) InvertedAcSignal=1;else InvertedAcSignal=0;
					
					 
					 
//		if((StartStopCtrl == OFF) || (RestartFlag == ON))
//           {
//                
//                 Disable_Triac();           
//         
//                 
//                 if(RestartFlag == ON)
//                 {
//                       
//                       RestartFlag = OFF;
//                 }
//                 
//                               
//                 StartStopCtrl = ON;
//           }
//           else if(StartStopCtrl == ON)
//           {
//               Trigger_Angle_Handler();
//					 }

//					 
//		   if(StartStopCtrl == ON)
//               { 
//                    if((IocFlag == ON) && (SynFlag == OFF) && (NewSteadyCtrlFlag == OFF))  // PhaseAngle StartUp
//                    {   
//                       
//											Run_Motor(); 
//                    }
//                    
//                  
//               }
//               else
//               {
//                         Disable_Triac();
//										
//               }  			 
//					 
					 
}







void Power_Assigned()
{
	
	
	
	if ((AcVoltagePhase<32767)&& ((AcVoltagePhase> (32767-FireAngle))))
		{
			InvertAcFireNeg=0;
	  AcFirePos=1;
		}
					else 
						{
						
						AcFirePos=0;
						InvertAcFireNeg=1;
						}

	
	if ((AcVoltagePhase>=32767)&&((AcVoltagePhase> (65535-FireAngle)))) 
	{
		AcFireNeg=0;
		InvertAcFirePos=1;
	}
					else 
					{
						AcFireNeg=1;
						InvertAcFirePos=0;
					}
	
	
	if (AcVoltagePhase<32767) AcZeroSignal=1;else  AcZeroSignal=0;
	
	if (((AcVoltagePhase> No_Fire_Zone)&&(AcVoltagePhase<(32768-No_Fire_Zone)))||((AcVoltagePhase> (32767+No_Fire_Zone))&&(AcVoltagePhase<(65535-No_Fire_Zone))))
			FireZone=1;
			else FireZone=0;
			

	if ((((HALL1_PIN == 1)&&(AcFirePos==1))) ||((HALL1_PIN == 0)&&(AcFireNeg==0))) 
			FirePower1=1;
		else FirePower1=0;
	
	
		if ((((VirtualHall2 == 1)&&(InvertAcFirePos==1))) ||((VirtualHall2 == 0)&&(InvertAcFireNeg==0))) 
			FirePower2=1;
		else FirePower2=0;
	
	
	
	
	if (FireZone==0)
	{
	
		Triac_Reset();
	}
	if 	((Triac1Ticker>=MaxTriggerPulse)||(Triac2Ticker>=MaxTriggerPulse))   Disable_Triac();
//	if (Trigger1On==0) TRIAC1_PIN=0;
//	if (Trigger2On==0) TRIAC2_PIN=0;

}

/* Private variables declaration ---------------------------------------------*/

void main()
{  	
	
    IO_Init();				   //真对 IAP15W4K61S4  IO口初始化
		InitTime0();
		InitTime1();
		InitTime2();
		InitExtInterrupt();
	  Triac_Reset();
		InitParameter();
    ENABLE_ALL_INTERRUPT();

		while(1)
		

		{ 
			
			Run_Motor();   

	
			
		 if (TRIAC1_PIN==1) P55=1;else P55=0;
		 if (TRIAC2_PIN==1) P47=1;else P47=0;
			Assign_Status_Flag();
			Power_Assigned();

						
						
		}			
   	
}






