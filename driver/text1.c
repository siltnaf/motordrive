


/* �������
   ��˸��ʵ�� 
   �˶�ָʾ��ֱ�ӽ���P55����
   ֻҪ����io����Ӧ��ƽ�仯����
   */



#include "stc15f2k60s2.h"


void delayms(unsigned int m)
     {
	  int  a, b;

	 for(a=0;a<5000;a++)
	 for(b=0;b<m;b++);
	   
	 }


//��� IAP15W4K61S4   STC15W4K56S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   
//ע��: STC15W4K32S4ϵ�е�оƬ,�ϵ��������PWM��ص�IO�ھ�Ϊ
//      ����̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ��
//���IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
void IO_init(void)
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
}



main()
{
IO_init();				   //��� IAP15W4K61S4  IO�ڳ�ʼ��
while(1)
{
  P55=0;
  delayms(50);
  P55=1;
  delayms(50);

}
}

