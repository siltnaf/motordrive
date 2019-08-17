
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define TriggerPulseWidth	  (100)                   //count 100us trigger pulse width
#define ACCounterWidth	  (100)                     //AC  duration divided into 200 step each step increment by 327; totall value is 65535 

//Fire angle is count from falling edge of AC and backwards

#define	InitFireAngle				 		(7000)
#define StartFireAngle				 	(15000)
#define MaxFireAngle            (30000)        //max value is 32700-deadzone


#define AcH1PhaseSteady       (10000)    //10012 <=> 55 degree between AC falling edge and HALL falling 

#define No_Fire_Zone           	(3000)         //dead zone 
#define No_Fire_Zone1            (No_Fire_Zone)
#define No_Fire_Zone2            (AcHalfPhase-No_Fire_Zone)
#define No_Fire_Zone3            (AcHalfPhase+No_Fire_Zone)
#define No_Fire_Zone4            (AcFullPhase-No_Fire_Zone)


#define Time0                 (50)				//0.5s
#define Time1									(100)				//1s
#define Time2									(200)				//3s
#define Time3									(500)				//5s



void IO_Init(void);

void InitTime1(void);

void InitExtInterrupt(void);
void InitParameter(void);
void Parameter_Reset(void);
void Triac1_Reset(void);
void Triac2_Reset(void);

#endif

