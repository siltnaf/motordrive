

#include "stc15f2k60s2.h"
#include "init.h"





void IO_Init(void)
{
  P0M0 = 0X00;
  P0M1 = 0X00;

  P1M0 = 0X00;
  P1M1 = 0X00;

  P2M0 = 0X00;
  P2M1 = 0X00;

  P3M0 = 0X00;
  P3M1 = 0X00;

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
	TMOD |= 0x01;              //set timer0 as timer mode 1
	
	TH0 =(65536-Timer0_Reload)/256;
  TL0 = (65536-Timer0_Reload)%256;
	
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
	  TMOD |= 0x10;              //set timer1 as timer mode 1
	  TH1 = 0x00;
	  TL1 = 0x00;
		TR1 =1;			 //定时器开始工作
	  ET1 =1;			 //使能定时器中断
	 
	
	  //mode 2: reload mode
//	  tmod &= 0x0f;
//	  tmod |= 0xa0;              //set timer1 as timer mode 2
//	  th1 = 0x32;//0x80;//
}


void InitTime2(void)
{

	T2L=0;			 //设置定时器2初始值
	T2H=0;      

  AUXR|=0X10;		 //允许定时器2运行	   12分频
	IE2|=0X04;       //开启定时器T2中断                                                               
  
}


void UartInit(void)		
{
       SCON=0x50;        
	   TMOD= 0x00;                      
	   AUXR=0X40;		 
       TL1=(65535-(11059200/4/9600));    
	   TH1=(65535-(11059200/4/9600))>>8;
	   TR1  = 1;        
	   ES   = 1;                                                                            
	   EA   = 1;        	
	
}

//void external interrupt init

