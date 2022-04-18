#include "usart.h"
#include "pid.h"
#include "openmv.h"
#include "math.h"

PIDTypedef PID_Struct;
extern u16 Openmv_x,Openmv_y;
extern u16 Openmv[];
/*
//void PID_Init(PIDTypedef * PID_Struct)
//{
//	PID_Struct->Set_X = 0;
//	PID_Struct->Set_Y = 0;
//	
//	PID_Struct->Kp = 0;
//	PID_Struct->Kd = 0;
//	PID_Struct->Ki = 0;
//	
//	PID_Struct->X_Ek	= 0;
//	PID_Struct->X_Ek1 = 0;
//	PID_Struct->Y_Ek = 0;
//	PID_Struct->Y_Ek1 = 0;
//	
//	PID_Struct->PWM_X_Middle=750;
//	PID_Struct->PWM_Y_Middle=750;
//	PID_Struct->PWM_X = PID_Struct->PWM_X_Middle;
//	PID_Struct->PWM_Y = PID_Struct->PWM_Y_Middle;
//}
*/
void PID_Init(void)
{
	PID_Struct.Set_X = 80;
	PID_Struct.Set_Y = 60;
//XXX	
//	PID_Struct.Kp = 2.6;
//	PID_Struct.Kd = 20;
//	PID_Struct.Ki = 0;
//YYY	
	PID_Struct.Kp = 2.6;
	PID_Struct.Kd = 20;
	PID_Struct.Ki = 0;	
	
	PID_Struct.X_Ek	= 0;
	PID_Struct.X_Ek1 = 0;
	PID_Struct.Y_Ek = 0;
	PID_Struct.Y_Ek1 = 0;
	PID_Struct.PWM_X_Middle=750;
	PID_Struct.PWM_Y_Middle=750;
	PID_Struct.PWM_X = PID_Struct.PWM_X_Middle;
	PID_Struct.PWM_Y = PID_Struct.PWM_Y_Middle;
}


void PID_realize(void)
{	  
	if(Openmv[2]==253 && Openmv[3]==253) 
	{	
		PID_Struct.PWM_X = PID_Struct.PWM_X;
		PID_Struct.PWM_Y = PID_Struct.PWM_Y;
		
	}
	
	else
	{	
		PID_Struct.X_Ek = PID_Struct.Set_X-Openmv[2];	//小球x坐标
		PID_Struct.Y_Ek = Openmv[3]-PID_Struct.Set_Y;	//小球y坐标
				
		//printf("Openmvx:%d\r\n",Openmv[2]);
		PID_Struct.PWM_X = PID_Struct.Kp*PID_Struct.X_Ek +PID_Struct.Kd*(PID_Struct.X_Ek-PID_Struct.X_Ek1) + PID_Struct.PWM_X_Middle;
		PID_Struct.PWM_Y = PID_Struct.Kp*PID_Struct.Y_Ek +PID_Struct.Kd*(PID_Struct.Y_Ek-PID_Struct.Y_Ek1) + PID_Struct.PWM_Y_Middle;
//		printf("PID_Struct.X_Ek:%f\r\n",PID_Struct.X_Ek);
		printf("PWM_X:%d\r\n",PID_Struct.PWM_X);
		PID_Struct.X_Ek1 = PID_Struct.X_Ek;
		PID_Struct.Y_Ek1 = PID_Struct.Y_Ek;
		
//		if(fabs(PID_Struct.X_Ek)<3) PID_Struct.PWM_X=PID_Struct.PWM_X_Middle;
//		if(fabs(PID_Struct.Y_Ek)<3) PID_Struct.PWM_Y=PID_Struct.PWM_Y_Middle;	
		
//		if(PID_Struct.PWM_X>1100)		PID_Struct.PWM_X=1100;
//		if(PID_Struct.PWM_X<400)		PID_Struct.PWM_X=400;
//		
//		if(PID_Struct.PWM_Y>1100)		PID_Struct.PWM_Y=1100;
//		if(PID_Struct.PWM_Y<400)		PID_Struct.PWM_Y=400;
	}
	
		TIM_SetCompare1(TIM3,PID_Struct.PWM_X);	 //更新PWM参数，改变输出占空比
		TIM_SetCompare2(TIM3,PID_Struct.PWM_Y);
	

}

