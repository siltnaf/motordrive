
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define Timer0_Reload	  (MAIN_Fosc/1200000)



void IO_Init(void);
void InitTime0(void);
void InitTime1(void);
void InitTime2(void);
void UartInit(void);

#endif

