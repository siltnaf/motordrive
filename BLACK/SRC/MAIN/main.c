
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

volatile uint8 xdata SynFlag;
volatile uint8 xdata HallEdgeFlag;


volatile uint8 xdata StartLoseStepTickerFlag;

volatile uint8 xdata AcZeroSignal;



volatile uint8 xdata AcDuration;

volatile uint8 xdata AcMaxDuration;

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
volatile uint16 xdata TriacPosTime;

volatile uint16 xdata InitialAngle;
volatile uint16 xdata MaxAngleLimit;
volatile uint16 xdata MinAngleLimit;
volatile uint16 xdata TriacMinPosTime;
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


volatile uint8 xdata MaxSpeedFlag;
volatile uint8 xdata AboveHalfMaxSpeedFlag;
volatile uint8 xdata state;
volatile uint16 xdata FireAngle;

volatile uint16 xdata AIM_PHASE_DIFF;

volatile uint16 xdata MaxAngle;
volatile uint16 xdata MinAngle;
volatile uint16 xdata MiddleAngle;
volatile uint16 xdata SteadyAngle;
volatile uint8 xdata SynUpdate;
volatile uint8 xdata SynTicker;





void Assign_Status_Flag()
{

		if (Hall1Duration<(Hall1MaxDuration>>1))	Hall1HalfFlag=1; else Hall1HalfFlag=0;			
	
					 
		if (((Hall1HalfFlag==0)&&(HALL1_PIN==1))||((Hall1HalfFlag==1)&&(HALL1_PIN==0))) VirtualHall2=0;else VirtualHall2=1;
					 
    if( Hall1MaxDuration<=AcMaxDuration>>1) MaxSpeedFlag=1;else MaxSpeedFlag=0;   //check if Halll signal is 1/2 of AC frequency
		if( Hall1MaxDuration<=AcMaxDuration) AboveHalfMaxSpeedFlag=1;else AboveHalfMaxSpeedFlag=0;   //check if Halll signal is 1/2 of AC frequency 
					 
		if (AcZeroSignal==0) InvertedAcSignal=1;else InvertedAcSignal=0;
					
					 
			
					 
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
	
	

	
	
	
	
	

}

/* Private variables declaration ---------------------------------------------*/





void Check_Fire_Angle()
	
{

switch (state)
{
	
	case SystemOff:    
											if((IocIsrTicker > 36)  &&    (IocIsrTicker<60))  state=SystemOn;
											Triac_Reset(); 
											break;
										
	
	case SystemOn: 			
											if(IocIsrTicker >= 60)		state = KickStart;                                // Start Open Loop with Timer2 Trigger  	
											FireAngle=InitFireAngle;
											
											break;
	
	case KickStart:			
											if(IocIsrTicker >= 90)                             
											{
											IocIsrTicker = 90;                        
											state = NormalRun;
											}
											FireAngle=StartFireAngle;
											
											break;
	
	case NormalRun:			
											if (MaxSpeedFlag==1) 
											{
												state=SynMax;
												MinAngle=(StartFireAngle+TargetFireAngle)>>1;
												MaxAngle=32765;
												SynUpdate=0;
												SynTicker=0;
												
											}
											FireAngle=TargetFireAngle;
											
											break;
	
	case SynMax:			  MiddleAngle=(MaxAngle+MinAngle)>>1;
											FireAngle=MiddleAngle;
											if ((MaxAngle>MinAngle)&&(SynUpdate==1))
												{		
													if (MaxSpeedFlag==1) MaxAngle=MiddleAngle+1;	
													else MinAngle=MiddleAngle-1;
													SynUpdate=0;
												}
											else 
											{
												state=MaxSteady;
												AIM_PHASE_DIFF=PhaseErrorAcVsHall;
												TriacPosTime=(32768-FireAngle)>>2;  //convert 10ms=32768  Fire angle  to time t= (32768-Fireangle) * 10ms/32768 
												TriacPosTime+=(TriacPosTime>>2);
												TriacMinPosTime=TriacPosTime;
											}
										
											break;
	
	case MaxSteady:			Trigger_Angle_Handler();
											if (MaxSpeedFlag==0)
											{
												state=LoseStep;
												
											}
											
											break;
											
	case LoseStep:			break;
	
	
}







}








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
		
			
				if ((state!=SystemOff)||(state!=MaxSteady)) Run_Motor();


	//		if (MaxSpeedFlag==1) P55=1;else P55=0;

//			 if (( PhaseErrorAcVsHall>0) &&(AcVoltagePhase< PhaseErrorAcVsHall)) P55=1; else P55=0;
		//	 if ((PhaseErrorAcVsHall<0)&& (AcVoltagePhase > (65535+PhaseErrorAcVsHall))) P47=1; else P47=0;
			
			P47=TRIAC1_PIN;
 
			Assign_Status_Flag();
			Power_Assigned();
			Check_Fire_Angle();
						
						
		}			
   	
}






