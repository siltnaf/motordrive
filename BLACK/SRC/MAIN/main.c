
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


volatile uint16 xdata DelayTimer;
volatile uint16 xdata DelayTimer2;
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
volatile uint8 xdata H1Prev;
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
volatile uint16 xdata SynMaxFireAngle;


volatile uint16 xdata TargetPeriodCount;

volatile uint8 xdata Fire1Reg;
volatile uint8 xdata Fire2Reg;
volatile uint8 xdata FireSeq;
volatile uint8 xdata PrioritySwitch;


//**********max speed steady control************//

volatile int16 xdata H1PhaseFallEdge;
volatile int16 xdata H1PhaseRiseEdge;
volatile int16 xdata H2PhaseFallEdge;

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
											 
											if (new_rpm!=0) 
												{
												DelayTimer =0;
												next_state=SystemOn;
												}
											break;
	
	case SystemOn: 			
											if(DelayTimer >Time0)		next_state = KickStart;                                // Start Open Loop with Timer2 Trigger  	
											TargetFireAngle=InitFireAngle;
											H2FireAngle=TargetFireAngle;	
											H1FireAngle=TargetFireAngle;
											break;
	
	case KickStart:			
											if(DelayTimer >=Time1)                             
											{
									     DelayTimer=0;                 
											next_state=NormalRun;
											}
											TargetFireAngle=StartFireAngle;
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;	
											break;
	
	case NormalRun:			if(DelayTimer >=Time2)
											{	    
												if ((MaxSpeedFlag==1) && (new_rpm==max_rpm))   //reach max speed 
													{
										 
														PID_Error=RESET;
														if (direction==cw)      //phase difference between Ac rising edge and H1 falling edge should be less than half AC cycle
															{
																if ((UpdateAcH1_cw==0) && (H1PhaseRiseEdge<AcHalfPhase))
																		{	
																
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
																				TargetAcH1=InitAcH1Phase+(AcHalfPhase>>1);
																				next_state=FindSteadyPoint;
																		}
																	else
																		{
																			SynFlag=0;
																			next_state=SynMax;
																		}
												
																}
														}
														else DelayTimer=0;
													}
																				
										  Find_TargetFireAngle();
											H1FireAngle=TargetFireAngle;	
											H2FireAngle=TargetFireAngle;	
											break;
	
	case FindSteadyPoint:								
										
											
														
													if ((Fire2Reg==1)&&(direction==cw))
																	{
																		
																		
																		UpdateAcH1_cw=1;	
																		DelayTimer=0;		//start timer count													
																		SynFlag=0;			
																		TargetFireAngle+=MarginAcH1 ;
																		SteadyAcH1_cw=0;
																		next_state=FindAcHallPhase;
																	}
																	else 
																	{
																		if(MaxSpeedFlag==1)
																		{
																				
  		 
																			TargetFireAngle-=ReduceSpeed;
																		}
																	}
								 
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
												
											break;
					
case FindAcHallPhase:					
											
										
											if ((DelayTimer<Time10)&&(DelayTimer>Time5))
													{
													  if (H1PhaseRiseEdge>SteadyAcH1_cw)
														SteadyAcH1_cw=(H1PhaseRiseEdge+SteadyAcH1_cw)>>1;
//													
													}
													else 
														if (DelayTimer>Time10)
														{
//													
															DelayTimer=0;
															DelayTimer2=0;
															next_state=SynMax;
														}
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
											break;
	case SynMax:			  
											  
											if (direction==cw)	TargetAcH1=SteadyAcH1_cw;else TargetAcH1=SteadyAcH1_ccw;  
											
//											if ((MaxSpeedFlag==0)&&(SynFlag==1))
//												{		 
//													
//													if (DelayTimer2>Time3)  
//													{				
//														if (direction==cw) UpdateAcH1_cw=0;		
//																else UpdateAcH1_ccw=0;								//if loss speed in 3 sec, jump to normal run            
//														next_state=NormalRun; 
//													}
//												}
//												else
//												{
//													Find_TargetFireAngle();
//													DelayTimer=0;
//													DelayTimer2=0;          //reset timer2  
//													if (MaxSpeedFlag==1) SynFlag=1;
//												
//												}													
										  
										
									
//											
													if ((Fire2Reg==1)&&(direction==cw))
													{
														SteadyAcH1_cw--;
																DelayTimer=0;		
													}											//reset timer and start lock phase
													if (DelayTimer<Time5)
															lock_TargetAcH1();
													else 
													{
														SynFlag=1;
														DelayTimer2=0;									//stop timer keep current TargetFireAngle
														DelayTimer=Time5;
													}
										
											
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
											break;
												

		
	
												
}

current_state=next_state;

//current_state= KickStart;



}




void main()
{  	
	
    IO_Init();				   //
		InitTime0();					//divide 20ms into 200 time interval
//		InitUart() ;
		InitExtInterrupt();
		InitParameter(); 
    ENABLE_ALL_INTERRUPT();
	

		while(1)
		{	
//		Check_Uart();
			
		Rebuild_Waveform();		
		Check_Speed();	
	if (AcEdgeDetect==1){	State_Assign();AcEdgeDetect=0;}
		Check_Error();
		if ((current_state!=SystemOff)&&(FireZone==1)) Run_Motor();
		if (H2_PIN==1) P55=1; else P55=0;                                      			
		}

   	
}






