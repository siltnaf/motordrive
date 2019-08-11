
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
 
 
 


volatile uint8 xdata DelayCount;

volatile uint8 xdata RestartFlag;






volatile uint8 	xdata AcRebuild;
volatile uint8 	xdata AcIncFlag;
volatile uint8 	xdata AcRisingEdgeDetect;
volatile uint8 	xdata AcPeriodCount;
volatile uint8 	xdata AcDuration;
volatile uint16 xdata AcPhaseInc; 
volatile uint16 xdata AcHalfPhase;
volatile uint16 xdata AcFullPhase;
volatile uint16 xdata AcPhase;

volatile uint8 	xdata H1SignalRebuild;
volatile uint8 	xdata H1PhaseIncFlag;
volatile uint8 	xdata H1RisingEdgeDetect;
volatile uint16 xdata H1PeriodCount;
volatile uint16 xdata H1Duration;
volatile uint16 xdata H1HalfPhase;
volatile uint16 xdata H1FullPhase;
volatile uint16 xdata H1PhaseInc;
volatile uint16 xdata H1Phase;

volatile uint8 xdata H2SignalRebuild;

 
volatile uint8 xdata AcFirePos;
volatile uint8 xdata AcFireNeg;
volatile uint8 xdata FireZone;
volatile uint8 xdata FirePower1;
volatile uint8 xdata FirePower2;
volatile uint8 xdata Trigger1On;
volatile uint8 xdata Trigger2On;
volatile uint8 xdata Triac1Ticker;
volatile uint8 xdata Triac2Ticker;





volatile uint8 xdata SynFlag;
volatile uint8 xdata NewSteadyCtrlFlag;
volatile uint8 xdata StartLoseStepTickerFlag;
volatile uint8 xdata LoseStepTicker;
volatile int16 xdata PhaseErrorAcVsHall;
volatile uint16 xdata RawPhaseErrorAcVsHall;
volatile int16 xdata FilteredPhaseErrorAcVsHall;
volatile uint16 xdata TriacPosTime;
volatile uint16 xdata InitialAngle;
volatile uint16 xdata MaxAngleLimit;
volatile uint16 xdata MinAngleLimit;
volatile uint16 xdata TriacMinPosTime;
volatile uint16 xdata AIM_PHASE_DIFF;
volatile uint16 xdata MaxAngle;
volatile uint16 xdata MinAngle;
volatile uint16 xdata MiddleAngle;
volatile uint16 xdata SteadyAngle;
volatile uint8 xdata SynUpdate;
volatile uint8 xdata SynTicker;







volatile uint8 xdata current_state;
volatile uint8 xdata next_state;
volatile uint16 xdata FireAngle;
volatile uint8 xdata MaxSpeedFlag;
volatile int16 xdata PID_Error; 
volatile uint16 xdata temp;	




 
void Rebuild_Waveform(void);
void State_Assign(void);
void Check_Error(void);









void Rebuild_Waveform()
{


static unsigned char Zone1_Flag;
static unsigned char Zone2_Flag;
	
	//************rebuild AC signal*****************// 
	
	if (AcIncFlag==1)
	{
		AcIncFlag=0;
		 AcPhase+=AcPhaseInc;
	}	if ( AcPhase>=AcFullPhase) AcPhase=0;
			
	//*************rebuild H1 signal*************//
	
	if (H1PhaseIncFlag==1)
	{
		H1PhaseIncFlag=0;
		H1Phase+=H1PhaseInc;
	}	if (H1Phase>=H1FullPhase) H1Phase=0;
	if (H1Phase<H1HalfPhase) H1SignalRebuild=1;else  H1SignalRebuild=0;
 			
	//************rebuild H2 signal*************//
	
	if (H1Phase<(H1HalfPhase>>1)||(H1Phase>(H1HalfPhase+(H1HalfPhase>>1)))) H2SignalRebuild=0;
  else H2SignalRebuild=1;
		
 
	
	//****************create  dead zone ****************//

	if ( AcPhase<AcHalfPhase) AcRebuild=1;else  AcRebuild=0;
	if (( AcPhase> No_Fire_Zone1)&&( AcPhase<No_Fire_Zone2))	Zone1_Flag=1;	else Zone1_Flag=0;
	if (( AcPhase> No_Fire_Zone3)&&( AcPhase<No_Fire_Zone4))	Zone2_Flag=1;	else Zone2_Flag=0;
	if ((Zone1_Flag==1)||(Zone2_Flag==1)) FireZone=1;else FireZone=0;
			
	//********define positive wave fire region ***********//
	if (FireZone==1)
		{
		if  ( AcPhase> (AcHalfPhase-FireAngle)) 
				AcFirePos=1; 
		
  //**********define negative wave fire region*************//
	
		if ( AcPhase> (AcFullPhase-FireAngle)) 
				AcFireNeg=1;
			}
	else 
			{
				AcFirePos=0;
				AcFireNeg=0;
			}

	
}





void State_Assign()
	
{

switch (current_state)
{
	
	case SystemOff:    
											if((DelayCount > 36)  &&    (DelayCount<60))  next_state=SystemOn;
											Triac1_Reset();
											Triac2_Reset();										
											break;
	
	case SystemOn: 			
											if(DelayCount >= 60)		next_state = KickStart;                                // Start Open Loop with Timer2 Trigger  	
											FireAngle=InitFireAngle;
											break;
	
	case KickStart:			
											if(DelayCount >= 90)                             
											{
											DelayCount = 90;                        
											next_state = NormalRun;
											}
											FireAngle=StartFireAngle;
											break;
	
	case NormalRun:			
											if (MaxSpeedFlag==1) 
											{
												next_state=SynMax;
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
												next_state=MaxSteady;
												AIM_PHASE_DIFF=PhaseErrorAcVsHall;
												TriacPosTime=(32768-FireAngle)>>2;  //convert 10ms=32768  Fire angle  to time t= (32768-Fireangle) * 10ms/32768 
												TriacPosTime+=(TriacPosTime>>2);
												TriacMinPosTime=TriacPosTime;
											}
										
											break;
	
	case MaxSteady:			Trigger_Angle_Handler();
											if (MaxSpeedFlag==0)
											{
												next_state=LoseStep;
												
											}
											
											break;
											
	case LoseStep:			break;
	
	
}

current_state=next_state;

if (current_state!=SystemOff) Run_Motor();



}


void Check_Error()
{
 
	//**********stop trigger after defined number of pulse*******//
			if (Triac1Ticker>=MaxTriggerPulse) Triac1_Reset();
			if (Triac2Ticker>=MaxTriggerPulse) Triac2_Reset();
	
	
	//**************no trigger beyond Dead zone*************//
			if (FireZone==0)	
					{
					Triac1_Reset();
					Triac2_Reset();
					}
					
	//**************retart if needed**************************//				
			if (RestartFlag == ON)
            {
              	Triac1_Reset();
								Triac2_Reset();
                Parameter_Reset();
                RestartFlag = OFF;
            }
		
	//*************only accept AC frequency of 50 or 60 Hz********//
						
			if ((AcPeriodCount>165)&&(AcPeriodCount<202))    //allow AC frequency either 50 (20ms) or 60 Hz (16.6ms)
				{
					if (AcPeriodCount<169) 
					{
						AcPhaseInc=394;			//60Hz-->16.7ms period=394*166, half period=32702
					  AcHalfPhase=32702;  //half period time count=394*166/2
						AcFullPhase=65404;  //full period time count=394*166
					}
						if (AcPeriodCount>198)
						{
							AcPhaseInc=327;			//50Hz-->20ms period=327*200, half period=32700    
							AcHalfPhase=32700;  //half period time count=327*200/2
							AcFullPhase=65400;  //full period time count=327*200
						}
					}
				else current_state=SystemOff;
				
				if (AcPeriodCount==H1PeriodCount) MaxSpeedFlag=1;

		}


void main()
{  	
	
    IO_Init();				   //真对 IAP15W4K61S4  IO口初始化
		InitTime0();					//single 100us count
		InitTime1();					//divide 20ms into 200 time interval
		InitTime2();					//calculate post angle time
		InitExtInterrupt();
	  Triac1_Reset();
		Triac2_Reset();
		InitParameter();
	  Parameter_Reset();
    ENABLE_ALL_INTERRUPT();
		
	
	
		while(1){

		Check_Error();
		Rebuild_Waveform();
		State_Assign();
			if (( TRIAC2_PIN==1)) P55=1; else P55=0;
                                          			
		}

   	
}






