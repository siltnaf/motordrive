

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
//	//mode 0: 13-bit timer
////	  tmod &= 0xf0;              //clear timer0 mode 
////	  tmod |= 0x08;              //set timer0 as timer mode 0
////	  tl0 = 0x00;
////	  th0 = 0x00;

//	  //mode 1: 16-bit timer
	TMOD &= 0xf0;              //clear timer0 mode 
	//TMOD |= 0x01;              //set timer0 as timer mode 1
	
	AUXR =0x00 ;           //timer 0 as 12T mode
	
	TH0 =(65536-ACCounterWidth)/256;
  TL0 = (65536-ACCounterWidth)%256;
	
	TR0 =1;			 //定时器开始工作
	ET0 =1;			 //使能定时器中断

//	

//    //mode 2: reload mode
////    tmod &= 0xf0;              //clear timer0 mode 
////	  tmod |= 0x0a;              //set timer0 as timer mode 2
////	  th0 = 0x50;//0x00;//
}

//void InitTime1(void)
//{
//	
//	  //mode 1: 16-bit timer
//	  TMOD &= 0x0f;              //clear timer1 mode 
//	  AUXR |=0x40 ;           //timer 1 as 1T mode
//		TH1 =(65536-ACCounterWidth)/256;
//		TL1 = (65536-ACCounterWidth)%256;
//		TR1 =1;			 //定时器开始工作
//	  ET1 =1;			 //使能定时器中断
//	 
//	
//}


//void InitTime2(void)
//{

//	

//  AUXR|=0X14;		 //允许定时器2运行	   12分频
//	IE2|=0X04;       //开启定时器T2中断                                                               
//  
//}

       
void InitUart(void)   
 
 {
     SCON=0x50;        
     TMOD= 0x00;                      
     AUXR|=0X40;    
	   AUXR&=0xC0;    //Timer2 set as 1T mode
     TL1=(65535-(main_clk/4/9600));    
     TH1=(65535-(main_clk/4/9600))>>8;
   	 TR1  = 1;        
     ENABLE_UART0_INTERRUPT() ;                                                                          

  }


void InitExtInterrupt (void)
{

	IT0=0;  	//detect both falling and rising edge of signal (AC signal)
	EX0=1;
	PX0=1;
	IT1=0;  	//detect both falling and rising edge of signal  (H1 signal) only rising edge 
	EX1=1;	  //enable INT1
	PX1=1;
	INT_CLKO |= 0x10;  //enable INT2, falling edge 
	
}


/*******************************************************************************
 Funcion Name:  Parameter_Reset
 Description: 
*******************************************************************************/
void Triac1_Reset(void)
{
 
	FirePower1=0;
	Triac1Ticker=0;
	TRIAC1_PIN=0;
	 


}

void Triac2_Reset(void)
{
 
	 
	FirePower2=0;
	Triac2Ticker=0;
	TRIAC2_PIN=0;


}




void InitParameter(void)
{
	static volatile xdata i;
	for( i = 0; i < UartDataLen;  ++i )
   UartData[i] = (char)0;
	UartArrayPtr=0;
	AcDuration=0;
	H1Duration=0;
	AcPeriodCount=200;            //assume 50Hz
	AcPhaseInc=327;
	AcHalfPhase=32767;
	AcFullPhase=65535;
	new_rpm=500;
	H1Phase=0;
  DelayCount=0;
	H1FireAngle=InitFireAngle;
	H2FireAngle=InitFireAngle;
	Triac1_Reset();
	Triac2_Reset();
	AcIncFlag=0;
  current_state=Standby;
	direction=cw;               //write direction =ccw or cw, control the rotation direction
	AcPhase = 0;
	PID_Error = 0;
	TargetAcH1=InitAcH1Phase;
	UpdateAcH1=0;


}
