

#include "stc15f2k60s2.h"
#include "init.h"
#include "app.h"




void IO_Init(void)
{
  P0M0 = 0X00;
  P0M1 = 0X00;

  P1M0 = 0X00;
  P1M1 = 0X00;

  P2M0 = 0Xff;
  P2M1 = 0X00;    //P2 is output 
	
	P2=0;

  P3M0 = 0X00;     //P3 is input
  P3M1 = 0Xfc;
	
  P4M0 = 0X00;
  P4M1 = 0X00;  
	
	
	
//	IO for triac is push pull
	
}

void InitTime0(void)
{
	//mode 0: 13-bit timer
//	  tmod &= 0xf0;              //clear timer0 mode 
//	  tmod |= 0x08;              //set timer0 as timer mode 0
//	  tl0 = 0x00;
//	  th0 = 0x00;

//	  //mode 1: 16-bit timer
	TMOD &= 0xf0;              //clear timer0 mode 
	//TMOD |= 0x01;              //set timer0 as timer mode 1
	
	TH0 =(65536-TriggerPulseWidth)/256;
  TL0 = (65536-TriggerPulseWidth)%256;
	
	TR0 =1;			 //定时器开始工作
	ET0 =1;			 //使能定时器中断

	

    //mode 2: reload mode
//    tmod &= 0xf0;              //clear timer0 mode 
//	  tmod |= 0x0a;              //set timer0 as timer mode 2
//	  th0 = 0x50;//0x00;//
}

void InitTime1(void)
{
	  //mode 0: 13-bit timer
//	  tmod &= 0x0f;              //clear timer1 mode 
//	  tmod |= 0x80;              //set timer1 as timer mode 0
//	  tl1 = 0x00;
//	  th1 = 0x00;

//	  //mode 1: 16-bit timer
	  TMOD &= 0x0f;              //clear timer1 mode 
	
		TH1 =(65536-ACCounterWidth)/256;
		TL1 = (65536-ACCounterWidth)%256;
		TR1 =1;			 //定时器开始工作
	  ET1 =1;			 //使能定时器中断
	 
	
	  //mode 2: reload mode
//	  tmod &= 0x0f;
//	  tmod |= 0xa0;              //set timer1 as timer mode 2
//	  th1 = 0x32;//0x80;//
}


void InitTime2(void)
{

		T2H =(65536-InitialTriacPosAngle)/256;
		T2L = (65536-InitialTriacPosAngle)%256;

  AUXR|=0X10;		 //允许定时器2运行	   12分频
	IE2|=0X04;       //开启定时器T2中断                                                               
  
}




void InitExtInterrupt (void)
{

	IT0=0;  	//detect both falling and rising edge of signal (AC signal)
	EX0=1;
	PX0=1;
	IT1=0;  	//detect both falling and rising edge of signal  (Hall1 signal) only rising edge 
	EX1=1;	  //enable INT1
	PX1=1;
	INT_CLKO |= 0x10;  //enable INT2, falling edge 
	
}


/*******************************************************************************
 Funcion Name:  Parameter_Reset
 Description: 
*******************************************************************************/
void Triac_Reset(void)
{
 
	Trigger1On=0;
	Trigger2On=0;
	Triac1Ticker=0;
	Triac2Ticker=0;


	TRIAC1_PIN=0;
	TRIAC2_PIN=0;

}


void InitParameter(void)
{
	
	Hall1MaxDuration=100;
	Hall1Duration=0;
	
	
	
	
	
}



