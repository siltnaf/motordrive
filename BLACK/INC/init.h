
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define TriggerPulseWidth	  (100)                   //count 100us trigger pulse width
#define ACCounterWidth	  (100)                     //AC  duration divided into 200 step each step increment by 327; totall value is 65535 


#define TargetFireAngle        	(15767)                  //range is 0 to 32767
#define	InitFireAngle				 		(5000)
#define StartFireAngle				 	(15000)
#define No_Fire_Zone           	(2000)
#define No_Fire_Zone1            (No_Fire_Zone)
#define No_Fire_Zone2            (AcHalfPhase-No_Fire_Zone)
#define No_Fire_Zone3            (AcHalfPhase+No_Fire_Zone)
#define No_Fire_Zone4            (AcFullPhase-No_Fire_Zone)
#define FireAngleMargin          (500)
#define SynSpeed									(10)





void IO_Init(void);
void InitTime0(void);
void InitTime1(void);
void InitTime2(void);
void InitExtInterrupt(void);
void InitParameter(void);
void Parameter_Reset(void);
void Triac1_Reset(void);
void Triac2_Reset(void);

#endif

