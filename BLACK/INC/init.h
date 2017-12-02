
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define TriggerPulseWidth	  (100)
#define Timer1_Reload	  (25000)
#define InitialTriacPosAngle	  (16666)


void IO_Init(void);
void InitTime0(void);
void InitTime1(void);
void InitTime2(void);
void InitExtInterrupt(void);
void UartInit(void);
void Parameter_Reset(void);

#endif

