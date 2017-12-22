
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		motorctrl.c
;Function: motor on/off
**************************************************************************************************/
#include	"app.h"








/*******************************************************************************
 Funcion Name:	Run_Motor
 Description: 
*******************************************************************************/
void Run_Motor(void)
{
		



if ((FirePower1==1)&&(FireZone==1)&&(Trigger2On==0)&&(FireSet==0))
{

	 Trigger1On=1;
	FireSet=1;
 
}


	if ((FirePower2==1)&&(FireZone==1)&&(Trigger1On==0)&&(FireSet==0))
{
	//	P55=1;
		Trigger2On=1;
	FireSet=1;
	
 
	}	


//			 if((AcVoltagePhase >= TriacPosAngle) && (AcVoltagePhase <= (32767 - TRIAC_ON_RANGE)))	// [Alpha,170]
//					 {
//									
//							Enable_Triac1();
//						
//					 }
//					 else
//					 {
//								 
//									Disable_Triac();
//					 }
		
						

//			 if((AcVoltagePhase >= (32767 + TriacPosAngle)) && (AcVoltagePhase <= (65535 - TRIAC_ON_RANGE))) // [180+Alpha, 350]
//					 {
//									
//						Enable_Triac1();
//						
////					 }
//					 else//					 {
//								 
//									Disable_Triac();
//					 }
		
}

void Enable_Triac1(void) 			
	
{
//	if (Trigger2On==0)
//					{	
//			
						Trigger1On=1;		
//						}			else Trigger1On=0;
																	
}



void Enable_Triac2(void) 	
{
				if (Trigger1On==0)
					{	
				
						Trigger2On=1;		
						}		else Trigger2On=0;							
													

}
/*******************************************************************************
 Funcion Name: Disable_Triac
 Description: 
*******************************************************************************/
	void Disable_Triac(void)
	{
		
		TRIAC1_PIN = 0;
		TRIAC2_PIN=0;
	}


