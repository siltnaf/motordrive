
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define main_clk     12000000
#define TriggerPulseWidth	  (100)                   //count 100us trigger pulse width
#define ACCounterWidth	  (100)                     //AC  duration divided into 200 step each step increment by 327; totall value is 65535 
#define UartDataLen         40




//Fire angle is count from falling edge of AC and backwards

#define	InitFireAngle				 		(10000)
#define StartFireAngle				 	(15000)
#define MaxFireAngle            (30000)        //max value is 32700-deadzone


#define InitAcH1Phase       		(5000)    //10012 <=> 55 degree between AC falling edge and HALL falling 
#define MarginAcH1              (500)

#define No_Fire_Zone           	(5000)         //dead zone 
#define No_Fire_Zone1            (No_Fire_Zone)
#define No_Fire_Zone2            (AcHalfPhase-No_Fire_Zone)
#define No_Fire_Zone3            (AcHalfPhase+No_Fire_Zone)
#define No_Fire_Zone4            (AcFullPhase-No_Fire_Zone)


#define Time0                 (50)				//0.5s
#define Time1									(100)				//1s
#define Time2									(200)				//3s
#define Time3									(300)				//5s





#define MaxTriggerPulse						(10)
#define ON                        (1)
#define OFF                       (0)

#define H2_PIN                 		(P36)
#define H1_PIN                  	(P33)
#define AC_PIN               			(P32)
#define TRIAC2_PIN                (P22)
#define TRIAC1_PIN                (P21)
#define TEST_PA2_PIN6             (P55)


//rotation direction
#define cw												0
#define ccw												1



// state 
#define	SystemOn									0
#define SystemOff									1
#define Standby										2
#define NormalRun									3
#define	KickStart									4
#define	SynMax										5
#define FindSteadyPoint						6


//FireSeq

#define refresh										0
#define start											1
#define sort											2
#define complete									3


#define max_rpm                3000




void IO_Init(void);
void InitTime0(void);
void InitUart(void); 

void InitExtInterrupt(void);
void InitParameter(void);
void Parameter_Reset(void);
void Triac1_Reset(void);
void Triac2_Reset(void);

#endif

