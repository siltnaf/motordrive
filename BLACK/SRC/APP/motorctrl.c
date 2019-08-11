
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
		

	//****************Trigger Triac*********************//
  if  (FirePower2==0) 
	{
		if (((H1SignalRebuild == 1)&&((AcFirePos&FireZone)==1)&&(AcRebuild==1)) ||  ((H1SignalRebuild == 0)&&((AcFireNeg&FireZone)==1)&&(AcRebuild==0)))
			FirePower1=1;
		else FirePower1=0;
	}
	if (FirePower1==0)  
	{
		if (((H2SignalRebuild == 1)&&((AcFireNeg&FireZone)==1)&&(AcRebuild==0)) ||  ((H1SignalRebuild == 0)&&((AcFirePos&FireZone)==1)&&(AcRebuild==1)))
			FirePower2=1;
		else FirePower2=0;
	}

 
  


		
}



 
