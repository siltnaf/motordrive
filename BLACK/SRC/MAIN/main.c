
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

volatile uint8 xdata direction;
volatile uint16 xdata rpm;
volatile uint16 xdata new_rpm;
volatile uint8 xdata current_state;
volatile uint8 xdata next_state;
volatile uint8 xdata Count;
volatile uint16 xdata temp;	


//***********construct AC waveform***********************//

volatile uint8 xdata AcActual;
volatile uint8  xdata AcPrev;
volatile uint8 	xdata AcRebuild;
volatile uint8 	xdata AcIncFlag;
volatile uint8 	xdata AcEdgeDetect;
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
volatile uint8 	xdata H2RisingEdgeDetect;
volatile uint16 xdata H2Phase;
 
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
volatile uint16 xdata AverageFireAngle;
volatile uint16 xdata SynMaxFireAngle;
volatile uint16 xdata SteadyFireAngle_cw;
volatile uint16 xdata SteadyFireAngle_ccw;

volatile uint16 xdata TargetPeriodCount;

volatile uint8 xdata Fire1Reg;
volatile uint8 xdata Fire2Reg;
volatile uint8 xdata FireSeq;
volatile uint8 xdata PrioritySwitch;


//**********max speed steady control************//

volatile int16 xdata H1PhaseRiseEdge;
volatile int16 xdata H2PhaseRiseEdge;

volatile uint16 xdata TargetAcHall;
volatile uint16 xdata TargetAcH1;
volatile uint16 xdata TargetAcH2;


volatile int16 xdata test_sigma;
volatile int16 xdata sigma;

volatile uint8 xdata MaxSpeedFlag;
volatile uint8 xdata LooseSpeedFlag;
volatile uint8 xdata OverSpeedFlag;
volatile uint8 xdata UpdateSpeedFlag;
volatile uint8 xdata LockSpeedFlag;
volatile uint8 xdata SpeedMatchCount;

volatile uint8 xdata ConvergeSpeed;

volatile int16 xdata PID_Error; 
volatile uint8 xdata UpdateAcH1_cw;
volatile uint8 xdata UpdateAcH2_ccw;
volatile uint16 xdata SteadyAcH1;
volatile uint16 xdata SteadyAcH2;
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
	
	case NormalRun:			

										if(DelayTimer >=Time1)
											{	    
												if ((MaxSpeedFlag==1) && (new_rpm==max_rpm))   //reach max speed 
													{
  
														if ((direction==cw)&&(Fire2Reg==0))      //if UpdateAcH1_cw=0 or UpdateAcH2_ccw=0 ,it should go through the calibration process to find minimum power
															{
																if (UpdateAcH1_cw==0) 
																		{	
															        DelayTimer=0;								 
																			next_state= FindSteadyPoint;
																			 
																		}
																else
																		{
																			 DelayTimer=0;		
																				SynFlag=0;	
																				next_state=SynMax;
																		}
																}
															if ((direction==ccw)&&(Fire1Reg==0)) 
																{
																	if ((UpdateAcH2_ccw==0))      //phase difference between Ac rising edge and H1 falling edge should be less than half AC cycle
																		{	
																			
																		 DelayTimer=0;
																			next_state= FindSteadyPoint;
																		}
																	else
																	{				
																		 DelayTimer=0;		
																			SynFlag=0;	
																			next_state=SynMax;
																		}
												
																}
														}
														else DelayTimer=0;
													}

													if ((DelayTimer>Time0)&&(UpdateSpeedFlag==1))	
													{
														
														Find_TargetFireAngle();
														
														if (LockSpeedFlag==1) 
															{	
																	
																AverageFireAngle=(AverageFireAngle+TargetFireAngle)>>1;
																SpeedMatchCount++;
															}
														DelayTimer=0;
													}
											 
											  if (SpeedMatchCount>100)
												{
														UpdateSpeedFlag=0;
														SpeedMatchCount=0;
												}
												else 
												{		
													if ((MaxSpeedFlag==1) && (new_rpm==max_rpm)&&(SpeedMatchCount>5))
															UpdateSpeedFlag=0;
													
												}
													
												if (UpdateSpeedFlag==0)
														TargetFireAngle=AverageFireAngle;
												
													
											
														
											H1FireAngle=TargetFireAngle;	
											H2FireAngle=TargetFireAngle;			
														
														
														
														
											break;
	
	
												
	case FindSteadyPoint:		
													
															
												if (DelayTimer>Time0)
												{
													DelayTimer=0;
												 

										    if (direction==cw) sigma=(AcHalfPhase-H1PhaseRiseEdge-TargetFireAngle-buffer);
														else sigma=(AcFullPhase-H2PhaseRiseEdge-TargetFireAngle-buffer);
												
													if (sigma>0) 
													{
														sigma=sigma>>8;
														if (MaxSpeedFlag==1) TargetFireAngle-=sigma;
												
													 }
												 }

											
													if  ((sigma<0)&&(direction==cw))
																	
																		{
															
																	TargetFireAngle+=MarginFireAngle;
																			SteadyFireAngle_cw=TargetFireAngle;
																		UpdateAcH1_cw=1;
																		UpdateSpeedFlag=1;
																		DelayTimer=0;															
																		SynFlag=0;
																		next_state=NormalRun;
																			}
													if  ((sigma<0)&&(direction==ccw))
																	
																		{
																	 UpdateSpeedFlag=1;
																		TargetFireAngle+=MarginFireAngle;
																		SteadyFireAngle_ccw=TargetFireAngle  ;
																		UpdateAcH2_ccw=1;
																		DelayTimer=0;															
																		SynFlag=0;
																		next_state=NormalRun;
																			}				
																			
																			
																 
										  
																	
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
												
											break;
					
 
	case SynMax:			  
											  
										if (direction==cw) TargetFireAngle=SteadyFireAngle_cw;
														else TargetFireAngle=SteadyFireAngle_ccw;
									 
												
	
											if ((MaxSpeedFlag==0)&&(SynFlag==1))
												{		 
													
													if (DelayTimer>Time3)  
													{				
														if (direction==cw) UpdateAcH1_cw=0;		
																else UpdateAcH2_ccw=0;															//if loss speed in 3 sec, jump to normal run            
														DelayTimer=0;
														UpdateSpeedFlag=1;
														next_state=NormalRun; 
													}
												}
												else
												{
													
													DelayTimer=0;
																													//reset timer2  
													if (MaxSpeedFlag==1) SynFlag=1;
												
												}													
							
											sigma= (TargetFireAngle-SteadyFireAngle_cw);
											TargetFireAngle-=sigma>>8;
												
											
											H1FireAngle=TargetFireAngle;
											H2FireAngle=TargetFireAngle;
											break;
												

		
	
												
}

current_state=next_state;


if (current_state>=	KickStart) current_state=KickStart;

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
		Check_Speed();	
	if (AcEdgeDetect==1){	State_Assign();AcEdgeDetect=0;}
		Check_Error();
		if ((current_state!=SystemOff)&&(FireZone==1)) Run_Motor();
 		if (MaxSpeedFlag==1) P55=0; else P55=1;                                      			
		}

   	
}






