

#include "stc15f2k60s2.h"
#include "delay.h"

/*
24MHz��Ƶ�� 
��ʾ�����¿�
һ������ʱ������ 0.8us
*/


//4΢��	��ʱ
//�˺�������4us�ı�������
//t  4us�ı���
void delay4us(uint16 us)
 {
  uint16 i;
  uint8 m;
  for(i=0;i<us;i++)
  for(m=0;m<5;m++);
 }  



//1���� ��ʱ
//ms 1ms�ı���
void delay1ms(uint16 ms)
{
 uint16 m;
 for(m=0;m<ms;m++)
 delay4us(250);
} 
  

