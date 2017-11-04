
#ifndef INIT_H
#define INIT_H

#define MAIN_Fosc   12000000UL
#define Timer0_Reload	  (100)
#define Timer1_Reload	  (200)
#define Timer2_Reload	  (250)


void IO_Init(void);
void InitTime0(void);
void InitTime1(void);
void InitTime2(void);
void UartInit(void);

#endif

