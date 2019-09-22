
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
volatile uint8 	xdata AcFallingEdgeDetect;

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

volatile int16 xdata H1PhaseFallEdge;
volatile uint16 xdata TargetAcH1;

volatile uint8 xdata MaxSpeedFlag;
volatile int16 xdata PID_Error; 
volatile uint8 xdata UpdateAcH1_cw;
volatile uint8 xdata UpdateAcH1_ccw;
volatile uint16 xdata SteadyAcH1_cw;
volatile uint16 xdata SteadyAcH1_ccw;
volatile uint8 xdata SynFlag;






 
void Rebuild_Waveform(void);
void State_Assign(void);
void Check_Error(void);
void Check_Uart(void);

volatile uint8 xdata UartData[UartDataLen];
volatile uint8 xdata UartArrayPtr;
volatile uint8 xdata UartRecFlag;
volatile uint8 xdata UartRecIntFlag;
volatile uint16 xdata UartRecInt;


volatile uint16 xdata test[400]=0;
volatile uint8 xdata testPtr;


void State_Assign()
	
{
	test[testPtr]=current_state;
	testPtr++;
switch (current_state)
{
	
	case SystemOff:    
											
											Triac1_Reset();
											Triac2_Reset();	
											next_state=SystemOff;
											break;
	
	case Standby:				
											Triac1_Reset();
											Triac2_Reset();	
											Check_Speed();
											if (new_rpm!=0) 
												{
												
												next_state=SystemOn;
												}
											break;
	
	case SystemOn: 			
											if(DelayCount >Time0)		next_state = KickStart;                                // Start Open Loop with Timer2 Trigger  	
											TargetFireAngle=InitFireAngle;
											H2FireAngle=TargetFireAngle;	
											H1FireAngle=TargetFireAngle;
											break;
	
	case KickStart:			
											if(DelayCount >=Time1)                             
											{
											DelayCount = Time1;                        
											next_state = NormalRun;
											}
											TargetFireAngle=StartFireAngle;
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;	
											break;
	
	case NormalRun:			
											if ((MaxSpeedFlag==1) && (new_rpm==max_rpm))   //reach max speed 
											{
												DelayCount=0;
												PID_Error=RESET;
											 if (direction==cw)      //phase difference between Ac rising edge and H1 falling edge should be less than half AC cycle
													{
														if (UpdateAcH1_cw==0)
															{	
																	TargetAcH1=InitAcH1Phase;
																	next_state=FindSteadyPoint;
															}
															else
															{
																		SynFlag=0;	
																		next_state=SynMax;
															}
													}
												else
													{
														if ((UpdateAcH1_ccw==0))      //phase difference between Ac rising edge and H1 falling edge should be less than half AC cycle
																{	
																		TargetAcH1=InitAcH1Phase;
																		next_state=FindSteadyPoint;
																}
																else
																{
																			SynFlag=0;
																			next_state=SynMax;
																}
												
														}
											}
											DelayCount = Time2; 
											Check_Speed();											
										  Find_TargetFireAngle();
											H1FireAngle=TargetFireAngle;	
											H2FireAngle=TargetFireAngle;	
											break;
	
	case FindSteadyPoint:								
											Check_Speed();
											
										 
												 
													Find_TargetFireAngle();	
	
													if ((PID_Error<=1)&&(PID_Error>=0))
															{
															if  ((MaxSpeedFlag==1))
																	{
																		DelayCount=0;
																		TargetAcH1+=50;
																		PID_Error=RESET;     //escape the loop and check error again
																	}
																}
														if (DelayCount>Time5) 
																	{
																		if (direction==cw) {SteadyAcH1_cw=TargetAcH1-MarginAcH1;UpdateAcH1_cw=1;}
																				else 
																				{SteadyAcH1_ccw=TargetAcH1-MarginAcH1;UpdateAcH1_ccw=1;}
																		DelayCount=0;															
																		SynFlag=0;
																		next_state=SynMax;
																	}
														
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
												
											break;
												
	case SynMax:			  
											if (direction==cw)	TargetAcH1=SteadyAcH1_cw;
														else TargetAcH1=10000;         //test
											if ((MaxSpeedFlag==0)&&(SynFlag==1))
												{		 
													
													if (DelayCount>Time3)  
													{				
														if (direction==cw) UpdateAcH1_cw=0;		
																else UpdateAcH1_ccw=0;								//if loss speed in 3 sec, jump to normal run            
														next_state=NormalRun; 
													}
												}
												else
												{
													
													DelayCount=0;          //Clear DelayCount if run at max speed
													if (MaxSpeedFlag==1) SynFlag=1;
												}													
										  Check_Speed();
											 Find_TargetFireAngle(); 
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
											break;
												

		
	
												
}

current_state=next_state;





}




void main()
{  	
	
    IO_Init();				   //
		InitTime0();					//divide 20ms into 200 time interval
		InitUart() ;
		InitExtInterrupt();
		InitParameter(); 
    ENABLE_ALL_INTERRUPT();
	

		while(1)
		{	
		Check_Uart();
			
		Rebuild_Waveform();			
	if (AcEdgeDetect==1){	State_Assign();AcEdgeDetect=0;}
		Check_Error();
		if ((current_state!=SystemOff)&&(FireZone==1)) Run_Motor();
		if (H2Rebuild==1) P55=1; else P55=0;                                      			
		}

   	
}






