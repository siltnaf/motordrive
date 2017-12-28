#ifndef __APP_H
#define __APP_H

#include "typedef.h"
#include  "stc15f2k60s2.h"



#define mHighLevelEffective 1

#define mPeriod_L 0xD0     //0x07D0 = 2000   2000 * 12.5ns * 2 = 50us
#define mPeriod_H 0x07
#define mDeadTime_L  0xA0  //0x00A0 = 160  160 * 12.5ns = 2us
#define mDeadTime_H  0x00

//all interrupt
#define ENABLE_ALL_INTERRUPT()  EA = 1
#define DISABLE_ALL_INTERRUPT()  EA = 0

//external interrupt 0, including EXINT1, EXINT2, EXINT3, EXINT4
#define ENABLE_EX0_INTERRUPT()   EX0 = 1;IT0 = 1
#define DISABLE_EX0_INTERRUPT()   EX0 = 0;IT0 = 1

//timer 0 overflow interrupt
#define ENABLE_TIME0_OVERFLOW_INTERRUPT()   ET0 = 1
#define DISABLE_TIME0_OVERFLOW_INTERRUPT()   ET0 = 0

//external interrupt 1, including comp1(overcurrent protect), timer3(overflow & external flag)
#define ENABLE_EX1_INTERRUPT()   IT1 = 1;EX1 = 1
#define DISABLE_EX1_INTERRUPT()   IT1 = 1;EX1 = 0

				//#define ENABLE_TIME3_OVERFLOW_INTERRUPT()   T3CON |= 0x20
				//#define DISABLE_TIME3_OVERFLOW_INTERRUPT()   T3CON &= 0xDF

				//#define CLR_TIME3_INTERRUPT_FLG()    T3CON &= 0x7F
				////timer 1 overflow interrupt
				//#define ENABLE_TIME1_OVERFLOW_INTERRUPT()   ET1 = 1
				//#define DISABLE_TIME1_OVERFLOW_INTERRUPT()   ET1 = 0

				////UART1 interrupt
				//#define ENABLE_UART1_INTERRUPT()   ES0 = 1
				//#define DISABLE_UART1_INTERRUPT()   ES1 = 0

				////ADC interrupt
				//#define ENABLE_ADC_INTERRUPT()   ADCTL |= 0x01;eadc = 1
				//#define DISABLE_ADC_INTERRUPT()   ADCTL &= 0xfe;eadc = 0

				////external interrupt 2, including PWM1, PWM2, PWM3
				//#define ENABLE_PWM1_2_3_INTERRUPT()   i2fr = 1;ex2 = 1
				//#define DISABLE_PWM1_2_3_INTERRUPT()   i2fr = 1;ex2 = 0

				////external interrupt 3, including PWM4
				//#define ENABLE_PWM4_INTERRUPT()   i3fr = 1;ex3 = 1
				//#define DISABLE_PWM4_INTERRUPT()   i3fr = 1;ex3 = 0

				////external interrupt 4, including comp2, comp3, comp4
				//#define ENABLE_EX4_INTERRUPT()   ex4 = 1
				//#define DISABLE_EX4_INTERRUPT()   ex4 = 0

				////external interrupt 5, including I2C, SPI, SPI_eflash
				//#define ENABLE_EX5_INTERRUPT()   ex5 = 1
				//#define DISABLE_EX5_INTERRUPT()   ex5 = 0

				////external interrupt 6, including RTC, LVD18, LVD33, timer4, hwbp(hardware breakpoint)
				//#define ENABLE_EX6_INTERRUPT()   ex6 = 1
				//#define DISABLE_EX6_INTERRUPT()   ex6 = 0

				////UART2 interrupt
				//#define ENABLE_UART2_INTERRUPT()   ien2 = 0x01
				//#define DISABLE_UART2_INTERRUPT()   ien2 = 0x00

				//#define ENABLE_TIMER3()  T3CON |= 0x04               //enable timer3
				//#define DISABLE_TIME3()  T3CON &= 0XFB               //disable timer3

				//#define ENABLE_TIMER4()  T4CON |= 0x04               //enable timer4
				//#define DISABLE_TIME4()  T4CON &= 0XFB               //disable timer4

				//#define TIME01_CLKDIV_SET(LOW_8BIT, HIGH_8BIT)  TIM01_CLK_DIV_L = LOW_8BIT;TIM01_CLK_DIV_H = HIGH_8BIT
				//#define ENABLE_TIME0()   tr0 = 1
				//#define DISABLE_TIME0()  tr0 = 0
				//#define ENABLE_TIME1()   tr1 = 1
				//#define DISABLE_TIME1()  tr1 = 0

				//#define ADC_CALIBRATE_REFP()  ADCTL |= 0x06
				//#define ADC_CALIBRATE_REFN()  ADCTL &= 0xfd;ADCTL |= 0x04

				//#define ADC_START()  ADCTL |= 0x20
				//#define ADC_BUSY     (ADCTL & 0x40)



#define MaxTriggerPulse						(10)
#define ON                        (1)
#define OFF                       (0)

#define HALL2_PIN                 (P36)
#define HALL1_PIN                  (P33)
#define AC_ZERO_PIN               (P32)
#define TRIAC2_PIN                (P22)
#define TRIAC1_PIN                 (P21)
#define TEST_PA2_PIN6             (P55)



#define TRIAC_ON_RANGE            (9102)//10

#define STARTUP_ANGLE             (29120)  // 
#define AIM_PHASE_ERROR           (7282)  // 10012 <=> 55 degree between AC rising edge and HALL falling 




								

				
extern volatile uint8 xdata MaxSpeedFlag;
extern volatile uint8 xdata AboveHalfMaxSpeedFlag;													
																	
																	
extern volatile uint8 xdata Exti2IsrTicker;  
extern volatile uint8 xdata Exti3IsrTicker;  
extern volatile uint8 xdata Timer2OverFlowTicker;
extern volatile uint8 xdata TimerOneIsrTicker;


extern volatile uint8 xdata Trigger1On;
extern volatile uint8 xdata Trigger2On;
extern volatile uint8 xdata Triac1Ticker;
extern volatile uint8 xdata Triac2Ticker;
extern volatile uint8 xdata MainTicker; 
extern volatile uint8 xdata IocIsrTicker;
extern volatile uint8 xdata LoseStepTicker;
extern volatile uint8 xdata PosAcFlag;

extern volatile uint8 xdata AcVoltagePhaseReset;
extern volatile uint8 xdata TriacReset;
extern volatile uint8 xdata FireSet;
extern volatile uint8 xdata IocFlag;
extern volatile uint8 xdata RestartFlag;
extern volatile uint8 xdata StartStopCtrl;
extern volatile uint8 xdata SynFlag;
extern volatile uint8 xdata HallEdgeFlag;
extern volatile uint8 xdata MotorStallFlag;
extern volatile uint8 xdata AdcTrigger;
extern volatile uint8 xdata NewSteadyCtrlFlag;
extern volatile uint8 xdata StartLoseStepTickerFlag;
extern volatile uint8 xdata Delay200usFlag;
extern volatile uint8 xdata Delay200usCounter;

extern volatile uint16 xdata AdcResult;
extern volatile uint16 xdata Ac_Voltage;
extern volatile uint16 xdata AcVoltagePhase;
extern volatile int16 xdata PhaseErrorAcVsHall;
extern volatile uint16 xdata RawPhaseErrorAcVsHall;
extern volatile int16 xdata FilteredPhaseErrorAcVsHall;


extern volatile uint8 xdata AcDuration;

extern volatile uint8 xdata AcMaxDuration;

extern volatile uint8 xdata FireZone;
extern volatile uint16 xdata Hall1MaxDuration;

extern volatile uint16 xdata Hall1Duration;
extern volatile uint8 xdata Hall1HalfFlag;
extern volatile uint8 xdata VirtualHall2;

extern volatile uint8 xdata InvertedAcSignal;

extern volatile uint8 xdata FirePower1;
extern volatile uint8 xdata FirePower2;

extern volatile uint8 xdata AcZeroSignal;

extern volatile uint8 xdata AcFirePos;
extern volatile uint8 xdata AcFireNeg;

extern volatile uint8 xdata InvertAcFireNeg;

extern volatile uint8 xdata InvertAcFirePos;

extern volatile uint16 xdata TriacPosAngle;
extern volatile uint16 xdata Lose_Step_Delay_Count;
extern volatile uint16 xdata InitialAngle;
extern volatile uint16 xdata InitialTimer1Value;
extern volatile int16 xdata PID_Error;

extern volatile uint16 xdata MaxAngleLimit;
extern volatile uint16 xdata MinAngleLimit;

extern void Run_Motor(void);


extern void Get_Init_Angle(void);


extern void Enable_Triac1(void);
extern void Enable_Triac2(void);

extern void Disable_Triac(void);

extern volatile uint8 xdata Startup_Delay_Count;

extern volatile uint16 xdata SynchronizeDalyCount;

extern void User_Parameter_Init(void);
#endif