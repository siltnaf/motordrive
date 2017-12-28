
#include	"app.h"


/*******************************************************************************
 Funcion Name:  Trigger_Angle_Handler
 Description: 
*******************************************************************************/
void Trigger_Angle_Handler(void)
{   
          if((IocFlag == ON) && (SynFlag == ON) && (NewSteadyCtrlFlag == ON))  // PhaseAngle StartUp
          { 
             if(Startup_Delay_Count == 0)
             {
                  Startup_Delay_Count = 5;        // 5* 0.2 ms = 1ms

/*
                  if(PhaseErrorAcVsHall != AIM_PHASE_ERROR)
                  {
                       if(PhaseErrorAcVsHall > AIM_PHASE_ERROR)     // 9102 = 50degree
                             TriacPosAngle += 3;
                       else
                             TriacPosAngle -= 3;
                  } 
*/

                  PID_Error = AIM_PHASE_ERROR - PhaseErrorAcVsHall; // PhaseErrorAcVsHall;  // FilteredPhaseErrorAcVsHall;
                  TriacPosAngle += (PID_Error >> 8);               // 12(132V OK) 13(96V OK)

                  MaxAngleLimit = (InitialTimer1Value + 0x14FA);    // +29  (96V OK, 132V OK 1.4A)  33(96V NG, 132V OK) 
                  MinAngleLimit = (InitialTimer1Value);

                  if(TriacPosAngle >= MaxAngleLimit)                // min current limit
                       TriacPosAngle = MaxAngleLimit;               // 
                  else if(TriacPosAngle <= MinAngleLimit)           // min current limit
                       TriacPosAngle = MinAngleLimit;               //
             } 
          } 
}

//==============================================================================
// The end of the file
//==============================================================================