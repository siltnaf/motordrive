
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
		 if((AcZeroSignal == 1) && (HALL1_PIN == 1))	//AcRisingEdgeFlag
		 {
					 if((AcVoltagePhase >= TriacPosAngle) && (AcVoltagePhase <= (32767 - TRIAC_ON_RANGE)))	// [Alpha,170]
					 {
									
							Enable_Triac1();
						
					 }
					 else
					 {
								 
									Disable_Triac();
					 }
		 }
		 else if((AcZeroSignal == 0) && (HALL1_PIN == 0))
		 {
					 if((AcVoltagePhase >= (32767 + TriacPosAngle)) && (AcVoltagePhase <= (65535 - TRIAC_ON_RANGE))) // [180+Alpha, 350]
					 {
									
								 Enable_Triac1();
						
					 }
					 else
					 {
								 
									Disable_Triac();
					 }
		 }
		 else
		 {
									
									Disable_Triac();
		 }
}
/*******************************************************************************
 Funcion Name: Stop_Motor
 Description: 
*******************************************************************************/
void Stop_Motor(void)
{			
		 Disable_Triac();
}
/*******************************************************************************
 Funcion Name: Enable_Triac
 Description: 
*******************************************************************************/
void Enable_Triac1(void)
{
		 
	if (TriggerOn==0)		TriggerOn=1;
			
	 if (TriggerProcessed==0) 
		{
		switch (TriacTicker%2)
			{
			case 0:	TRIAC1_PIN=1; break;
			case 1: TRIAC1_PIN=0; break;
		
			}
		}
	else TRIAC1_PIN=0;		
	
	
 
}

/*******************************************************************************
 Funcion Name: Enable_Triac
 Description: 
*******************************************************************************/
void Enable_Triac2(void)
{
		 
	if (TriggerOn==0)		TriggerOn=1;
			
	 if (TriggerProcessed==0) 
		{
		switch (TriacTicker%2)
			{
			case 0:	TRIAC2_PIN=1; break;
			case 1: TRIAC2_PIN=0; break;
		
			}
		}
	else TRIAC2_PIN=0;		
	
	
 
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


