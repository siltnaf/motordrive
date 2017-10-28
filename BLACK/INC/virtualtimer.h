#ifndef VIRTUALTIMER_H
#define VIRTUALTIMER_H

#include "typedef.h"

//=============== private typedef ===================================//
//---- virtual timer variables -----
extern volatile uint8 xdata  Tick_Pwm;

extern volatile uint8  xdata Startup_Delay_Count;
extern volatile uint8  xdata Tick1ms_Pwm_Num;
extern volatile uint8 xdata SynFlag;
extern volatile uint8  xdata NewStdeadyCtrlFlag;


extern void Virtual_Timer(void);
extern void User_Parameter_Init(void);

#endif