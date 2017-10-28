
#ifndef MOTORCTRL_H
#define MOTORCTRL_H
#include  "typedef.h"
#include "app.h"
#include  "stc15f2k60s2.h"


volatile uint8 xdata AcZeroSingal;

void Stop_Motor(void);
void Enable_Triac(void);
void Disable_Triac(void);
void Comp_Enable_Triac(void);
void Comp_Disable_Triac(void);
void Global_Triac_Enable(void);
void Global_Triac_Disable(void);
void Run_Motor(void);

#endif
