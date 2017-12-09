
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
volatile int16 xdata PhaseErrorAcVsHall;
volatile uint16 xdata RawPhaseErrorAcVsHall;
volatile int16 xdata FilteredPhaseErrorAcVsHall;
volatile uint8 xdata TriacTicker;
volatile uint8 xdata TriggerProcessed;
volatile uint8 xdata TriggerOn;
volatile uint16 xdata TriacPosAngle;

volatile uint16 xdata InitialAngle;
volatile uint16 xdata MaxAngleLimit;
volatile uint16 xdata MinAngleLimit;
volatile uint16 xdata InitialTimer1Value;
volatile uint8 xdata Startup_Delay_Count;


volatile uint8 xdata AcFirePos;
volatile uint8 xdata AcFireNeg;
volatile uint8 xdata FireZone;

volatile int16 xdata PID_Error;






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
					 
					 
		if((StartStopCtrl == OFF) || (RestartFlag == ON))
           {
                
                 Disable_Triac();           
                 Parameter_Reset(); 
                 
                 if(RestartFlag == ON)
                 {
                       
                       RestartFlag = OFF;
                 }
                 
                               
                 StartStopCtrl = ON;
           }
           else if(StartStopCtrl == ON)
           {
    //            Trigger_Angle_Handler();
					 }

					 
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
					 
					 
}







void Power_Assigned()
{
	if ((AcVoltagePhase<32767)&& ((AcVoltagePhase> (32767-FireAngle))))
	  AcFirePos=1;
					else AcFirePos=0;
	

	
	if ((AcVoltagePhase>=32767)&&((AcVoltagePhase> (65535-FireAngle)))) AcFireNeg=0;
					else AcFireNeg=1;
	
	
	if (AcVoltagePhase<32767) AcZeroSignal=1;else  AcZeroSignal=0;
	
	if (((AcVoltagePhase> No_Fire_Zone)&&(AcVoltagePhase<(32768-No_Fire_Zone)))||((AcVoltagePhase> (32767+No_Fire_Zone))&&(AcVoltagePhase<(65535-No_Fire_Zone))))
		
	
//	 || ((AcVoltagePhase>(32767+No_Fire_Zone)) &&)))
			FireZone=1;
		else FireZone=0;
			



}

/* Private variables declaration ---------------------------------------------*/

void main()
{  	
	
    IO_Init();				   //真对 IAP15W4K61S4  IO口初始化
		InitTime0();
		InitTime1();
		InitTime2();
		InitExtInterrupt();
	  Parameter_Reset();
		//UartInit();				   //串口初始化
    ENABLE_ALL_INTERRUPT();

		while(1)
		

		{ 
			
	if (FireZone==1) P55=1;else P55=0;
			
if ((((HALL1_PIN == 1)&&(AcFirePos==1))) ||((HALL1_PIN == 0)&&(AcFireNeg==0))) 

{
	P47=1;
	Enable_Triac1();

	}	
	else P47=0;

	

   //Run_Motor();
        
						Assign_Status_Flag();
            Power_Assigned();
         
              


          

			
		}			//防止程序跑飞
   	
}






