
#include	"app.h"
#include "init.h"




/*******************************************************************************
 Funcion Name:  Find_Speed
 Description: 
*******************************************************************************/
void Check_Speed()
{
	
	
 	if (((AcPeriodCount+2)>=H1PeriodCount)||(H1PeriodCount<AcPeriodCount)) 
			{
				if (new_rpm==max_rpm)	MaxSpeedFlag=1;
			}
			else
			
				MaxSpeedFlag=0;
				
	if(H1PeriodCount<AcPeriodCount)
			{
				OverSpeedFlag=1;
			}
			else
			
				OverSpeedFlag=0;
				
	if ((AcPeriodCount+10)>=H1PeriodCount) 
					LooseSpeedFlag=0;
			
			else
			
				LooseSpeedFlag=1;
/************************************************************************
;   calculate TargetPeriod Count take time
;   this operation is performed once when new_rpm value is input
;
;    Target Period =  200 *(3000/rpm)
;    if rpm is 3000, it is 50Hz so period count is 20ms or 200 count of 100us pulse 
;
;************************************************************************/
				if (new_rpm!=rpm)
				{
					MaxSpeedFlag=0;
					rpm=new_rpm;
					if (rpm!=0)
						TargetPeriodCount=(((60000/(rpm>>3))*10)>>3);
					
					                               
				}
			

}


/*******************************************************************************
 Funcion Name:  Find_TargetFireAngle
 Description: 
*******************************************************************************/

//void lock_TargetAcH1()
//{
//	
//static uint16 xdata HallPhaseRiseEdge;
//			
//				
//				
//// cw direction								
///************************************************************************
//;   case 1.  
//;   decrement TargetFireAngle (reduce power) if delta is left of Tar
//;			
//;				
//;                            delta
//;                        -->	    <----			
//;                        --> Tar<---
//;                          : :
//;               ---         -------         -------
//;          AC      |       | :     |       |       |
//;                   -------         -------         -----------
//;                            :
//;                              ----x---          ----x
//;         H1                  |        |        |
//;                -------------          ----x--- 
//;			
//;                        --------          --------
//;         H2            |        |        |        |
//;               --------          --------          ------------
//;										
//;        Targetphase is between rising edge of AC and rising edge of H1 
//;	
//;************************************************************************/
//							if (direction ==cw) 
//								{
//									HallPhaseRiseEdge=H1PhaseRiseEdge;
//									 
//								}
//									else 
//									{
//										HallPhaseRiseEdge=H2PhaseRiseEdge;
//								 
//									}
//						
//						
//						
//							PID_Error= (HallPhaseRiseEdge-TargetAcHall);
//				
//					     
//								if (PID_Error>0)
//									{
//										PID_Error=PID_Error>>4;
//										TargetFireAngle+=PID_Error;
//									}
//									else 
//									{
//										PID_Error=-PID_Error;
//										PID_Error=PID_Error>>4;
//										TargetFireAngle-=PID_Error;
//									}
//								
//								 	
//			
//			
//	
//			
//			

//			
//			 
//		
//					}
//	 
/***********************************************************************
; If speed is below max speed
;	control TargetFireAngle so that H1PeriodCount approach the calculated TargetPeriodCount
;
**************************************************************************/
				
void Find_TargetFireAngle()	
{
	volatile uint16 xdata diff;
				
 
				if (H1PeriodCount<=TargetPeriodCount)
					{ 
						diff =TargetPeriodCount-H1PeriodCount;
						if (diff>50) diff=diff>>1;
							else 
							{
								 diff=diff>>4;
							}
 						 TargetFireAngle -=diff;
						
						  
					}	
					else
					{
						
						diff =H1PeriodCount-TargetPeriodCount;
					  if (diff>50) diff =diff>>1;
								else
								{
								 diff=diff>>4;
								}
 						 TargetFireAngle+=diff ;
						
						 		
					}
			 
					if ((new_rpm==max_rpm)&&(MaxSpeedFlag==0)) TargetFireAngle+=100;
			 
			
			
				if (TargetFireAngle>MaxFireAngle)
						TargetFireAngle=MaxFireAngle;
				if (TargetFireAngle<No_Fire_Zone)
						TargetFireAngle=No_Fire_Zone;
				
				 
			
				if (diff<5) LockSpeedFlag=1;
								else LockSpeedFlag=0;
			
				if (TargetFireAngle>MaxFireAngle)
						TargetFireAngle=MaxFireAngle;
				if (TargetFireAngle<No_Fire_Zone)
						TargetFireAngle=No_Fire_Zone;
				
			
			
}

			
/************************************************************************
;   
;   From AcPeriod and H1Period
;   reconstruct the AC,H1,H2 waveform
;   define also the positive and negative fire zone 
;
;************************************************************************/

void Rebuild_Waveform()
{


static unsigned char Zone1_Flag;
static unsigned char Zone2_Flag;
	

	
	if (AcIncFlag==1)
			{
			
				AcIncFlag=0;
				AcPhase+=AcPhaseInc;

			}	
		else
			AcPhasePrecise=AcPhase+(256-TL1)*3;    //each step of AcPhase increment is  ~320 , and there is 1200 count in T1 timer, so each count stand for 4/15 * counter_value
	
//	if ( AcPhase>=AcFullPhase) AcPhase=0;
	if ( AcPhase<AcHalfPhase) 
							AcRebuild=Ac_pos;else  AcRebuild=Ac_neg;
	if (AcPrev!=AcRebuild) 
			{
				H1PhaseInc=65535/H1PeriodCount;
				 H1FullPhase=(H1PhaseInc*H1PeriodCount);
				 H1HalfPhase=H1FullPhase>>1;
				
				AcEdgeDetect=1;
				AcPrev=AcRebuild;
				FireSeq=refresh;	                 //AC edge detect
			}
		
		
	//*************rebuild H1 signal*************//
	
	if (H1PhaseIncFlag==1)
	{
		H1PhaseIncFlag=0;
		H1Phase+=H1PhaseInc;
	}	if (H1Phase>=H1FullPhase) H1Phase=0;
		if (H1Phase<H1HalfPhase) H1Rebuild=1;else  H1Rebuild=0;
	//**********find H1 fall edge with respect to Ac //
  if (H1Prev!=H1Rebuild) 
		{
			H1Prev=H1Rebuild;
//			if (H1Rebuild==0) 	H1PhaseFallEdge=AcPhasePrecise;
		}
	
	
	//************rebuild H2 signal*************//
	
	if (H1Phase<(H1HalfPhase>>1)||(H1Phase>(H1HalfPhase+(H1HalfPhase>>1)))) 
	{
			if (direction ==cw) H2Rebuild=1; else H2Rebuild=0;
	}
  else 
	{
		if (direction==cw) H2Rebuild=0; else H2Rebuild=1;
	}
 
	


/************************************************************************
;    create   dead zone routine
;    AcPhase is increment every by 327 during a 100us pulse timer1 interrupt 
;
;    AcPhasePrecise is read from TL1 register and add the remainder value to AcPhase
;    So AcPhasePrecise is continuous value without step
;    It gives more precise value of triac firing angle 
;     
;************************************************************************/
	

	if (( AcPhase > No_Fire_Zone1)&&( AcPhase <No_Fire_Zone2))	Zone1_Flag=1;	else Zone1_Flag=0;
	if (( AcPhase > No_Fire_Zone3)&&( AcPhase <No_Fire_Zone4))	Zone2_Flag=1;	else Zone2_Flag=0;
	if ((Zone1_Flag==1)||(Zone2_Flag==1)) FireZone=1;else FireZone=0;
			
	//********define positive wave fire region ***********//
	if ((FireZone==1)&&(AcRebuild==1)&&(AcPhase > (AcHalfPhase-H1FireAngle))) 
				AcFirePos=1;
		else AcFirePos=0;
		
 //**********define negative wave fire region*************//

	if ((FireZone==1)&&(AcRebuild==0)&&(AcPhase > (AcFullPhase-H2FireAngle)))    
		{	
			AcFireNeg=1;
		
		}
		else 
		{
			
			AcFireNeg=0;
		}
	
}

		
/************************************************************************
;   
;   Error check routine
;   1. make sure no Triac trigger at Deadzone 
;   2. if AC frequency is beyond 50/60 Hz, the program should stop running
;
;************************************************************************/



void Check_Error()
{
 
	//**************no trigger beyond Dead zone*************//
			if ((FireZone==0))	
					{
					
					Triac1_Reset();
					Triac2_Reset();
					}
		
					
	//*************only accept AC frequency of 50 or 60 Hz********//
		
/************ AC signal********************************************* 
;        apply Kalman Filter to AC pulse width
;	
;				Kalman Gain=Es	/ (Es+Em)
;	Since Em0 find in scope is 100us, assume the first Es0=50us,
; x is the full period of ac	
;	K[0]=   Es[0]/(Es[0]+Em[0]) 
;	x[1]=x[0]+K[0](x[0]-m[0]),   Es[1]=(1-K[1])Es[0]	
;
;
;
;
;
;
;	
;	
;*****************************************************************************/	
	
					
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
				else 
					if (DelayTimer>Time3)
						current_state=SystemOff;


		}


//==============================================================================
// The end of the file
//==============================================================================