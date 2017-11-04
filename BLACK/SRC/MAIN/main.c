
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		main.c
;Function:main
**************************************************************************************************/


#include  "stc15f2k60s2.h"		  //STC15头文件
#include  "typedef.h"	    		  //宏定义 常用函数
#include  "delay.h"				  //延时函数
#include  "uart.h"
#include  "init.h"
#include  "app.h"
#include  "virtualtimer.h"






/* Private variables definition ----------------------------------------------*/
//--------------------------------- // External Interrupt 
volatile uint8 xdata Exti2IsrTicker;  
volatile uint8 xdata Exti3IsrTicker;  
volatile uint8 xdata Timer3OverFlowTicker;
volatile uint8 xdata TimerOneIsrTicker; 

volatile uint8 xdata MainTicker; 
volatile uint8 xdata IocIsrTicker;
volatile uint8 xdata LoseStepTicker;
volatile uint8 xdata PosAcFlag;

volatile uint8 xdata IocFlag;
volatile uint8 xdata RestartFlag;
volatile uint8 xdata StartStopCtrl;
volatile uint8 xdata SynFlag;
volatile uint8 xdata HallEdgeFlag;
volatile uint8 xdata MotorStallFlag;
volatile uint8 xdata AdcTrigger;
volatile uint8 xdata NewSteadyCtrlFlag;
volatile uint8 xdata StartLoseStepTickerFlag;
volatile uint8 xdata Delay200usFlag;
volatile uint8 xdata Delay200usCounter;

volatile uint16 xdata AdcResult;
volatile uint16 xdata Ac_Voltage;
volatile uint16 xdata AcVoltagePhase;
volatile int16 xdata PhaseErrorAcVsHall;
volatile uint16 xdata RawPhaseErrorAcVsHall;
volatile int16 xdata FilteredPhaseErrorAcVsHall;

/* Private variables declaration ---------------------------------------------*/

void main()
{  	
	
    IO_Init();				   //真对 IAP15W4K61S4  IO口初始化
		InitTime0();
		InitTime1();
		InitTime2();
		UartInit();				   //串口初始化
    ENABLE_ALL_INTERRUPT();

		while(1)
		

		{ 
	
		}			//防止程序跑飞
   	
}






