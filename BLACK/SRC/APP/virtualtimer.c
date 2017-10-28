
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		virtualtimer.c
;Function: timer
**************************************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "typedef.h"         // user difine
#include "virtualtimer.h"


/*******************************************************************************
 Funcion Name:  Virtual_Timer
 Description: 
*******************************************************************************/
void Virtual_Timer(void)
{	  
          Tick_Pwm++;
  	  if(Tick_Pwm >= Tick1ms_Pwm_Num)     // Unit = MiliSecond
	  {
		 Tick_Pwm = 0;
		
		 if(Startup_Delay_Count != 0) 
			Startup_Delay_Count --;	

	 	 //if(Adc_Start_Count != 0) 
		//    Adc_Start_Count --;	
/*
       if(NewStdeadyCtrlFlag == 1)
       {
		 if(Lose_Step_Delay_Count != 0) 
			Lose_Step_Delay_Count --;	
	   }
*/
	 	// if(SynchronizeDalyCount != 0) 
		//    SynchronizeDalyCount --;	
	  }
}


/*******************************************************************************
 Funcion Name:  User_Parameter_Init
 Description: 
*******************************************************************************/
void User_Parameter_Init(void)
{
     Tick1ms_Pwm_Num = 2;   // 2*100us = 0.2ms
     Startup_Delay_Count = 1;
     //SynchronizeDalyCount = 500;
     // Lose_Step_Delay_Count = 500;
}


/*******************************************************************************
  The end of the file
*******************************************************************************/
