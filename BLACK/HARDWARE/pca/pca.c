
#include "stc15f2k60s2.h"
#include "delay.h"
#include "def.h"
#include "pca.h"



  u8 time=0x20;					 //��ʱ����

//PCA ��������
void pca(void)
{				 	  	

  P_SW1|=0X10;				 //��������ڵ�P3.5�� P3.5Ҳ�Ǻ���Ŀ��Ƶƿ�	 �ֲ�790
                             //P_SW1��AUXR1��һ���Ĵ��� ���ֲ�790ҳ���Կ���
							 //����� SPI���ߵ���  MOSI-P4^0  MISO-P4^1  SCLK-P4^3	����������AUXR1
							 //��P_SW1.4 P_SW1.5���ǵ��� PCA���������IO����� ��������һ��Ҫע�� �ǻ���� 

  CCON=0;					 //PCA���ƼĴ���				   �ֲ�785
  CL=0;						 //PCA ��������λ 	  ���ü�������ʼֵ 	 ����ͼ796ҳ
							 //ͼ�п��Կ��� PWMֻ�õ���CL���ۼӼ����� ����CCAPnL���бȽ�
  CH=0;						 //PCA ��������λ
  CMOD=0x02;				 //����PCAʱ��Դ   ��Ƶ2��Ƶ	   �ֲ�784

  PCA_PWM0=0X00;			 //8λPWM���
  CCAP0H=CCAP0L=time;		 //ռ�ձ�	 CL�ۼ�ֵ��CCAP0L�Ƚ� CL���ڵ���CCAP0L���1  ����CCAP0L���0
  CCAPM0=0x42;				 //PCAģ��0Ϊ8λpwmģʽ			   �ֲ�785

//  PCA_PWM1=0X40;			 //7λPWM���    ��P_SW1|=0X10���� �����P3.6
//  CCAP1H=CCAP1L=0x20;		 //ռ�ձ�
//  CCAPM1=0x42;				 //PCAģ��1Ϊ7λpwmģʽ
//
//  PCA_PWM2=0X80;			 //8λPWM���	 ��P_SW1|=0X10���� �����P3.7
//  CCAP2H=CCAP2L=0x20;		 //ռ�ձ�
//  CCAPM2=0x42;				 //PCAģ��2Ϊ6λpwmģʽ
							 
  CR=1;						 //ʹ��PCA

//  while(1)					 //�Զ�����ռ�ձ�  ʹP3.5�ڵĵ����ȷ����仯
//  {
//    delay1ms(200);			 //��ʱ
//	t+=10;
//	if(t>200)t=0;
//  	CCAP0H=CCAP0L=t;		 //��������ֵ��ռ�ձȴ洢�ռ�   
//  }
}




			 
