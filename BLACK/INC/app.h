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

#define ENABLE_TIME3_OVERFLOW_INTERRUPT()   T3CON |= 0x20
#define DISABLE_TIME3_OVERFLOW_INTERRUPT()   T3CON &= 0xDF

#define CLR_TIME3_INTERRUPT_FLG()    T3CON &= 0x7F
////timer 1 overflow interrupt
#define ENABLE_TIME1_OVERFLOW_INTERRUPT()   ET1 = 1
#define DISABLE_TIME1_OVERFLOW_INTERRUPT()   ET1 = 0

				////UART0 interrupt
#define ENABLE_UART0_INTERRUPT()   ES = 1
#define DISABLE_UART0_INTERRUPT()   ES = 0

				////ADC interrupt
#define ENABLE_ADC_INTERRUPT()   ADCTL |= 0x01;eadc = 1
#define DISABLE_ADC_INTERRUPT()   ADCTL &= 0xfe;eadc = 0

				////external interrupt 2, including PWM1, PWM2, PWM3
#define ENABLE_PWM1_2_3_INTERRUPT()   i2fr = 1;ex2 = 1
#define DISABLE_PWM1_2_3_INTERRUPT()   i2fr = 1;ex2 = 0

				////external interrupt 3, including PWM4
#define ENABLE_PWM4_INTERRUPT()   i3fr = 1;ex3 = 1
#define DISABLE_PWM4_INTERRUPT()   i3fr = 1;ex3 = 0

				////external interrupt 4, including comp2, comp3, comp4
#define ENABLE_EX4_INTERRUPT()   ex4 = 1
#define DISABLE_EX4_INTERRUPT()   ex4 = 0

				////external interrupt 5, including I2C, SPI, SPI_eflash
#define ENABLE_EX5_INTERRUPT()   ex5 = 1
#define DISABLE_EX5_INTERRUPT()   ex5 = 0

				////external interrupt 6, including RTC, LVD18, LVD33, timer4, hwbp(hardware breakpoint)
#define ENABLE_EX6_INTERRUPT()   ex6 = 1
#define DISABLE_EX6_INTERRUPT()   ex6 = 0

				////UART2 interrupt
#define ENABLE_UART2_INTERRUPT()   ien2 = 0x01
#define DISABLE_UART2_INTERRUPT()   ien2 = 0x00


#define ENABLE_TIMER4()  T4CON |= 0x04               //enable timer4
#define DISABLE_TIME4()  T4CON &= 0XFB               //disable timer4

#define TIME01_CLKDIV_SET(LOW_8BIT, HIGH_8BIT)  TIM01_CLK_DIV_L = LOW_8BIT;TIM01_CLK_DIV_H = HIGH_8BIT
#define ENABLE_TIME0()   tr0 = 1
#define DISABLE_TIME0()  tr0 = 0
#define ENABLE_TIME1()   tr1 = 1
#define DISABLE_TIME1()  tr1 = 0

#define ADC_CALIBRATE_REFP()  ADCTL |= 0x06
#define ADC_CALIBRATE_REFN()  ADCTL &= 0xfd;ADCTL |= 0x04

#define ADC_START()  ADCTL |= 0x20
#define ADC_BUSY     (ADCTL & 0x40)




		

//***************variable rpm and direction***************//


extern volatile uint16 xdata DelayCount;
extern volatile uint8 xdata direction;
extern volatile uint16 xdata rpm;
extern volatile uint16 xdata new_rpm;
extern volatile uint8 xdata current_state;
extern volatile uint8 xdata next_state;

extern volatile uint16 xdata temp;	


//***********construct AC waveform***********************//

extern volatile uint8 xdata AcActual;
extern volatile uint8  xdata AcEdgeDetect;
extern volatile uint8 	xdata AcRebuild;
extern volatile uint8 	xdata AcIncFlag;
extern volatile uint8 	xdata AcRisingEdgeDetect;
extern volatile uint8 	xdata AcPeriodCount;
extern volatile uint8 	xdata AcDuration;
extern volatile uint16 xdata AcPhaseInc; 
extern volatile uint16 xdata AcHalfPhase;
extern volatile uint16 xdata AcFullPhase;
extern volatile uint16 xdata AcPhase;
extern volatile uint16 xdata AcPhasePrecise;
extern volatile uint8 	xdata AcFallingEdgeDetect;

//***********construct H1 ,H2 waveform**********************//

extern volatile uint8 	xdata H1Rebuild;
extern volatile uint8 	xdata H1PhaseIncFlag;
extern volatile uint8 	xdata H1RisingEdgeDetect;
extern volatile uint16 xdata H1PeriodCount;
extern volatile uint16 xdata H1Duration;
extern volatile uint16 xdata H1HalfPhase;
extern volatile uint16 xdata H1FullPhase;
extern volatile uint16 xdata H1PhaseInc;
extern volatile uint16 xdata H1Phase;
extern volatile uint16 xdata H1FireAngle;


extern volatile uint8 xdata H2Rebuild;
extern volatile uint16 xdata H2FireAngle;
 
//************trigger Triac***********************// 

extern volatile uint8 xdata AcFirePos;
extern volatile uint8 xdata AcFireNeg;
extern volatile uint8 xdata FireZone;
extern volatile uint8 xdata FirePower1;
extern volatile uint8 xdata FirePower2;
extern volatile uint8 xdata Trigger1On;
extern volatile uint8 xdata Trigger2On;
extern volatile uint8 xdata Triac1Ticker;
extern volatile uint8 xdata Triac2Ticker;

extern volatile uint16 xdata TargetFireAngle;
extern volatile uint16 xdata TargetPeriodCount;

extern volatile uint8 xdata Fire1Reg;
extern volatile uint8 xdata Fire2Reg;
extern volatile uint8 xdata FireSeq;
extern volatile uint8 xdata PrioritySwitch;


//**********max speed steady control************//


extern volatile int16 xdata H1PhaseFallEdge;
extern volatile uint16 xdata TargetAcH1;
extern volatile uint16 xdata SteadyAcH1;
extern volatile uint8 xdata SynFlag;

extern volatile uint8 xdata UpdateAcH1;
extern volatile uint8 xdata MaxSpeedFlag;
extern volatile int16 xdata PID_Error; 

extern volatile uint8 xdata UartData[];
extern volatile uint8 xdata UartArrayPtr;
extern volatile uint8 xdata UartRecFlag;
extern volatile uint8 xdata UartRecIntFlag;
extern volatile uint16 xdata UartRecInt;
 
//extern volatile uint16 xdata Ac[400];
//extern volatile uint8 xdata AcPtr;

 

extern void Run_Motor(void);
extern void Check_Speed(void);
extern void Find_TargetFireAngle(void);
extern void Rebuild_Waveform(void);
extern void Check_Error(void);
extern void UartSendByte(uint8 dat);
extern void UartSendStr(uint8 *s);
extern int StrComp(uint8 *s,uint8 *t);
extern uint16 CharToInt(uint8 *s);
extern void Check_Uart(void);
#endif