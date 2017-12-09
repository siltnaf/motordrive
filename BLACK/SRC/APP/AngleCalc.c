
#include	"app.h"


/*******************************************************************************
 Funcion Name:  Get_Init_Angle
 Description:   470kOhm + 300kOhm
AD Test Result 20160629
//----------
80  => 788
85  => 796
90  => 808
95  => 820
96  => 824
100 => 832
105 => 848
110 => 860
115 => 876
120 => 888
125 => 904
130 => 916
132 => 924
135 => 932
138 => 940
140 => 948
145 => 960
150 => 976
*******************************************************************************/
void Get_Init_Angle(void)
{
 if(Ac_Voltage <= 820)             // <=95V
                {
                     InitialTimer1Value = 0x2CD9; // 55 62 //55; 0xE81F
                     InitialAngle = 10012;        // startup angle = 55
                }
                else if(Ac_Voltage <= 832)        // (95,100V]
                {
                     InitialTimer1Value = 0x2CD9; // 62;  
                     InitialAngle = 10012;        // startup angle = 55
                }
                else if(Ac_Voltage <= 848)        // (100,105V]  980
                {
                     InitialTimer1Value = 0x3130; // 68;
                     InitialAngle = 12015;        // 66
                }
                else if(Ac_Voltage <= 876)        // (105,115V] 980
                {
                     InitialTimer1Value = 0x3415; // 72;   When motor is too hot @132V, the value is changed into this range.
                     InitialAngle = 12743;        // 70
                }
                else if(Ac_Voltage <= 904)        // (115,125V]
                {
                     InitialTimer1Value = 0x36FA; // 76; little load current
                     InitialAngle = 13653;        // 75  startup current
                }
                else if(Ac_Voltage <= 932)        // (125,135V]
                {
                     InitialTimer1Value = 0x3925; // 79; little load current
                     InitialAngle = 14017;        // 77  startup current
                }
                else                              // (135,]
                {
                     InitialTimer1Value = 0x3A98; // 81; little load current
                     InitialAngle = 14563;        // 80  startup current
                }

}

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