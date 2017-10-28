
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		motorctrl.c
;Function: motor on/off
**************************************************************************************************/
#include  "app.h"


 volatile uint8 xdata AcZeroSignal;




/*******************************************************************************
 Funcion Name:  Run_Motor
 Description: 
*******************************************************************************/
void Run_Motor(void)
{
     if((AcZeroSignal == 1) && (HALL_PIN == 1))  //AcRisingEdgeFlag
     {
           if((AcVoltagePhase >= TriacPosAngle) && (AcVoltagePhase <= (32767 - TRIAC_ON_RANGE)))  // [Alpha,170]
           {
                  //Global_Triac_Enable();
                  Enable_Triac();
           }
           else
           {
                  //Global_Triac_Disable();
                  Disable_Triac();
           }
     }
     else if((AcZeroSignal == 0) && (HALL_PIN == 0))
     {
           if((AcVoltagePhase >= (32767 + TriacPosAngle)) && (AcVoltagePhase <= (65535 - TRIAC_ON_RANGE))) // [180+Alpha, 350]
           {
                  //Global_Triac_Enable();
                  Enable_Triac();
           }
           else
           {
                  //Global_Triac_Disable();
                  Disable_Triac();
           }
     }
     else
     {
                  //Global_Triac_Disable();
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
void Enable_Triac(void)
{
     TRIAC_PIN = 0;
}
/*******************************************************************************
 Funcion Name: Disable_Triac
 Description: 
*******************************************************************************/
void Disable_Triac(void)
{
     TRIAC_PIN = 1;
}

/*******************************************************************************
 Funcion Name: Enable_Triac
 Description: 
*******************************************************************************/
void Comp_Enable_Triac(void)
{
     TRIAC_PIN = 1;
}
/*******************************************************************************
 Funcion Name: Comp_Disable_Triac
 Description: 
*******************************************************************************/
void Comp_Disable_Triac(void)
{
     TRIAC_PIN = 0;
}
/*******************************************************************************
 Funcion Name: Global_Triac_Enable
 Description: 
*******************************************************************************/
void Global_Triac_Enable(void)
{
      if(AC_ZERO_PIN == 1)
           Comp_Enable_Triac();
      else
           Enable_Triac();
}
/*******************************************************************************
 Funcion Name: Global_Triac_Disable
 Description: 
*******************************************************************************/
void Global_Triac_Disable(void)
{
      if(AC_ZERO_PIN == 1)
           Comp_Disable_Triac();
      else
           Disable_Triac();

}
//==============================================================================
// The end of the file
//==============================================================================