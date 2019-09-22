
/**************************************************************************************************
;Project:	Two Phase BLACK Controller
;MCU:			STC15F2K60
;Com7iler:Keil C Compiler
;Author:	ken
;Date:		2017.10.28
;Files:		motorctrl.c
;Function: motor on/off
**************************************************************************************************/
#include	"app.h"
#include "init.h"







/*******************************************************************************
 Funcion Name:	Run_Motor
 Description: 
*******************************************************************************/
void Run_Motor(void)
{
 

	//****************Trigger Triac*********************//
   	switch (FireSeq)
		{
			case refresh:
				
			//************find FirePower1 and FirePower2 start position**************//
								
			
											if (direction==cw)
												{
														if (((H1_PIN== 0)&&(AcFirePos==1)&&(AcRebuild==1)) ||  ((H1_PIN == 1)&&(AcFireNeg==1)&&(AcRebuild==0)))
																	Fire2Reg=1;
															else Fire2Reg=0;
														
														if (((H2_PIN == 1)&&(AcFirePos==1)&&(AcRebuild==1)) ||  ((H2_PIN  == 0)&&(AcFireNeg==1)&&(AcRebuild==0)))
																	Fire1Reg=1;
															else Fire1Reg=0;
												}
											if (direction==ccw)
												{
													
														if (((H2_PIN  == 0)&&(AcFirePos==1)&&(AcRebuild==1)) ||  ((H2_PIN  == 1)&&(AcFireNeg==1)&&(AcRebuild==0)))
																	Fire2Reg=1;
															else Fire2Reg=0;
															if (((H1_PIN == 0)&&(AcFirePos==1)&&(AcRebuild==1)) ||  ((H1_PIN == 1)&&(AcFireNeg==1)&&(AcRebuild==0)))
																	Fire1Reg=1;
															else Fire1Reg=0;
												}
			
											if ((Fire1Reg==1)||(Fire2Reg==1)) FireSeq=start;

											break;
			//***************handle case either FirePower1 or FirePower2 is triggered*************//
			case start:			
											if ((Fire1Reg==1)&&(Fire2Reg==0)){FirePower1=1;FirePower2=0;FireSeq=complete;}
											if ((Fire2Reg==1)&&(Fire1Reg==0)){FirePower2=1;FirePower1=0;FireSeq=complete;}
											if ((Fire1Reg==1)&&(Fire2Reg==1)){FireSeq=sort;}
											break;
			//************if both FirePower1 and FirePower2 are triggered, swap the trigger sequence********//
			case sort:			
											if (PrioritySwitch==0)
												{
													FirePower1=1;
													FirePower2=0;
													PrioritySwitch=1;
													FireSeq=complete;
												}
												else 
												{	
													FirePower2=1;
													FirePower1=0;
													PrioritySwitch=0;
													FireSeq=complete;
												}
												break;
			case complete:		break;								
												
	}
		
		
		

 
	
		
 
		
	}
 
	


		




 
