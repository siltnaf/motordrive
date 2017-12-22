
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define TriggerPulseWidth	  (100)                   //count 100us trigger pulse width
#define ACCounterWidth	  (100)                     //AC  duration divided into 200 step each step increment by 327; totall value is 65535 
#define AC_PHASE_INC      (327)

#define InitialTriacPosAngle	  (16666)
#define FireAngle        (15767)                  //range is 0 to 32767
#define No_Fire_Zone           (5000)


void IO_Init(void);
void InitTime0(void);
void InitTime1(void);
void InitTime2(void);
void InitExtInterrupt(void);
void InitParameter(void);
void Triac_Reset(void);

#endif

