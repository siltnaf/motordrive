
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

 Enable_Triac1();
 
}


	if ((FirePower2==1)&&(FireZone==1)&&(Trigger1On==0)&&(FireSet==0))
{


 Enable_Triac2();
	
 
	}	



		
}

void Enable_Triac1(void) 			
	
{
	 Trigger1On=1;
	FireSet=1;
																	
}



void Enable_Triac2(void) 	
{
					
				Trigger2On=1;
	FireSet=1;											

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


