
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		main.c
;Function:main
**************************************************************************************************/


#include  "stc15f2k60s2.h"		  //STC15 

#include  "init.h"
#include  "app.h"



//***************variable rpm and direction***************//


volatile uint16 xdata DelayCount;
volatile uint8 xdata direction;
volatile uint16 xdata rpm;
volatile uint16 xdata new_rpm;
volatile uint8 xdata current_state;
volatile uint8 xdata next_state;

volatile uint16 xdata temp;	


//***********construct AC waveform***********************//

volatile uint8 xdata AcActual;
volatile uint8  xdata AcEdgeDetect;
volatile uint8 	xdata AcRebuild;
volatile uint8 	xdata AcIncFlag;
volatile uint8 	xdata AcRisingEdgeDetect;
volatile uint8 	xdata AcPeriodCount;
volatile uint8 	xdata AcDuration;
volatile uint16 xdata AcPhaseInc; 
volatile uint16 xdata AcHalfPhase;
volatile uint16 xdata AcFullPhase;
volatile uint16 xdata AcPhase;
volatile uint16 xdata AcPhasePrecise;


//***********construct H1 ,H2 waveform**********************//

volatile uint8 	xdata H1Rebuild;
volatile uint8 	xdata H1PhaseIncFlag;
volatile uint8 	xdata H1RisingEdgeDetect;
volatile uint16 xdata H1PeriodCount;
volatile uint16 xdata H1Duration;
volatile uint16 xdata H1HalfPhase;
volatile uint16 xdata H1FullPhase;
volatile uint16 xdata H1PhaseInc;
volatile uint16 xdata H1Phase;
volatile uint16 xdata H1FireAngle;


volatile uint8 xdata H2Rebuild;
volatile uint16 xdata H2FireAngle;
 
//************trigger Triac***********************// 

volatile uint8 xdata AcFirePos;
volatile uint8 xdata AcFireNeg;
volatile uint8 xdata FireZone;
volatile uint8 xdata FirePower1;
volatile uint8 xdata FirePower2;
volatile uint8 xdata Trigger1On;
volatile uint8 xdata Trigger2On;
volatile uint8 xdata Triac1Ticker;
volatile uint8 xdata Triac2Ticker;

volatile uint16 xdata TargetFireAngle;
volatile uint16 xdata TargetPeriodCount;

volatile uint8 xdata Fire1Reg;
volatile uint8 xdata Fire2Reg;
volatile uint8 xdata FireSeq;
volatile uint8 xdata PrioritySwitch;


//**********max speed steady control************//

volatile int16 xdata AcH1PhaseDiff;
volatile int16 xdata H1PhaseFallEdge;
volatile uint16 xdata TargetAcH1Phase;

volatile uint8 xdata MaxSpeedFlag;
volatile int16 xdata PID_Error; 









 
void Rebuild_Waveform(void);
void State_Assign(void);
void Check_Error(void);










void State_Assign()
	
{

switch (current_state)
{
	
	case SystemOff:    
											
											Triac1_Reset();
											Triac2_Reset();										
											break;
	
	case SystemOn: 			
											if(DelayCount >Time0)		next_state = KickStart;                                // Start Open Loop with Timer2 Trigger  	
											H1FireAngle=InitFireAngle;
											break;
	
	case KickStart:			
											if(DelayCount >=Time1)                             
											{
											DelayCount = Time1;                        
											next_state = NormalRun;
											}
											H1FireAngle=StartFireAngle;
											break;
	
	case NormalRun:			
											if (MaxSpeedFlag==1)      //reach max speed 
											{
											 if (AcH1PhaseDiff<0)      //phase difference between Ac rising edge and H1 falling edge should be less than half AC cycle
												 TargetFireAngle+=100;				
												else
												next_state=SynMax;
											}
											DelayCount = Time2; 
											Check_Speed();											
										  if (AcEdgeDetect==1)	Find_TargetFireAngle();
											H1FireAngle=TargetFireAngle;											
											break;
	
	case SynMax:			  
											if (MaxSpeedFlag==0)
												{		 
													if (DelayCount>Time3)            //if loss speed in 5 sec, jump to normal run            
														next_state=NormalRun;       
												}
												else
													DelayCount=0;          //Clear DelayCount if run at max speed											
										  Check_Speed();
											if (AcEdgeDetect==1)	Find_TargetFireAngle();	
											H1FireAngle=TargetFireAngle;
											break;
}

current_state=next_state;

current_state=SynMax;



}






void main()
{  	
	
    IO_Init();				   //
		InitTime1();					//divide 20ms into 200 time interval
		InitExtInterrupt();

		InitParameter(); 
    ENABLE_ALL_INTERRUPT();
		
	
		while(1){

	

		Rebuild_Waveform();			
		
		State_Assign();
		Check_Error();
		if (current_state!=SystemOff) Run_Motor();
		if (TRIAC1_PIN==1) P55=1; else P55=0;                                      			
		}

   	
}






