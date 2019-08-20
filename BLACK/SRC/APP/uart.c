  
#
#include	"app.h"
#include "init.h"

     
            

void UartSendByte(uint8 dat)
  {
			SBUF = dat;
			while(TI == 0);
			TI = 0;
   }
 
	
void UartSendStr(uint8 *s)
	{
		
	
	while (*s != '\0')
	{
		UartSendByte(*s);
		s++;
	}
	
	
}
	

int StrComp(uint8 *s, uint8 *t)
	{
		static uint8 match;
		
		match=1;
	while ((*s != '\0')||(*t !='\0'))
	{
		if (*s!=*t) match=0;
		s++;
		t++;
	}
	return match;
	
	
}
	
	
uint16 CharToInt(uint8 *s)
	{
	static int value;
		
		value=0;
	while (*s != '\0')
	{
		 value+=(*s-48);
			s++;
		 if (*s != '\0') value*=10;
	} 
	s=0;
	return value;
	
	
}
	
	
	

   
           

	 
 
