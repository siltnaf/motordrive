
#include	"app.h"


/*******************************************************************************
 Funcion Name:  Trigger_Angle_Handler
 Description: 
*******************************************************************************/
void Trigger_Angle_Handler(void)
{   
        
                  PID_Error = AIM_PHASE_DIFF - PhaseErrorAcVsHall; // PhaseErrorAcVsHall;  // FilteredPhaseErrorAcVsHall; PID error is +ve means motor is running too fast.
                  TriacPosTime += (PID_Error >> 8);               // 12(132V OK) 13(96V OK)

                

                  if(TriacPosTime >= TriacMaxPosTime	)                // min current limit
                       TriacPosTime = TriacMaxPosTime	;               // 
                  else if(TriacPosTime <= TriacMinPosTime)           // min current limit
												TriacPosTime = TriacMinPosTime; 
}

//==============================================================================
// The end of the file
//==============================================================================