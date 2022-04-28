#include "control.h"

#include "usart.h"

#include "openmv.h"
#include "math.h"
int init_pwm1,init_pwm2;

void SetPwm(int x, int y)
{
	 init_pwm1 = 750 + x;
	 init_pwm2 = 750 + y;
	 Limit_PWM(init_pwm1,init_pwm2);
	 TIM_SetCompare1(TIM3, init_pwm1);  //250为0.5ms，记得要切换成模式1 PB4 左
	 TIM_SetCompare2(TIM3, init_pwm2);  //右
}

void Limit_PWM(int x,int y)
{
	if(x<=400) x=400;
	if(y<=400) y=400;	
	if(x>=1100) x=1100;
	if(y>=1100) y=1100;	
}


PIDTypedef PID_Struct;
PIDTypedef PID_V;//速度环

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

	PID_Struct.Kp_y = 2.15;
	PID_Struct.Kd_y = 1.4;//22
	PID_Struct.Ki_y = 0.02;	//0.027
	//位置环
	PID_Struct.Kp_x = 2.15;//  1.95 2.2 
	PID_Struct.Kd_x = 1.4;//  1.1 1.4
	PID_Struct.Ki_x = 0.2;	//0.5 0.2
	//速度环
	PID_V.Kp_x=1.5;
	PID_V.Kd_x=0.0;
	PID_V.Ki_x=0.0;	
	//pid 2.2 29 0,03 
	PID_Struct.X_Ek	= 0;
	PID_Struct.X_Ek_pre = 0;
	PID_Struct.X_EkD=0;		
	PID_Struct.Y_Ek = 0;
	PID_Struct.Y_Ek_pre = 0;
	PID_Struct.Y_EkD=0;		
	PID_Struct.PWM_X_Middle=750;
	PID_Struct.PWM_Y_Middle=750;
	PID_Struct.PWM_X = PID_Struct.PWM_X_Middle;
	PID_Struct.PWM_Y = PID_Struct.PWM_Y_Middle;
}


void PID_realize(void)
{
	static float Sum_x,Sum_y;
	static u16 count=0;
//	static int speed_x=0,speed_y;
//	static u16 last_x;

	if(Openmv_x==253 && Openmv_y==253) //	if(Openmv[2]==253 && Openmv[3]==253) 
	{	
		PID_Struct.PWM_X = PID_Struct.PWM_X_Middle;
		PID_Struct.PWM_Y = PID_Struct.PWM_Y_Middle;	
	}
	
	else
	{	
//位置环
		PID_Struct.X_Ek = PID_Struct.Set_X-Openmv[2];	//小球x坐标
		PID_Struct.Y_Ek = Openmv[3]-PID_Struct.Set_Y;	//小球y坐标
//速度环
//		if(count>2)		speed_x=(Openmv[2]-last_x)/0.1;

//		last_x=Openmv[2];

//		PID_V.X_Ek=0-speed_x;
		
		//printf("speed_x:%d\r\n",speed_x);
		Sum_x=Sum_x+(PID_Struct.X_Ek_pre+PID_Struct.X_Ek)/2;
	  Sum_y=Sum_y+(PID_Struct.Y_Ek_pre+PID_Struct.Y_Ek)/2;
		//printf("Openmvx:%d\r\n",Openmv[2]);
		//PID_Struct.PWM_X = PID_Struct.Kp*PID_Struct.X_Ek +PID_Struct.Kd*(PID_Struct.X_Ek-PID_Struct.X_Ek_pre) + PID_Struct.PWM_X_Middle+PID_Struct.Ki*Sum;
		PID_Struct.PWM_Y = PID_Struct.Kp_y*PID_Struct.Y_Ek +PID_Struct.Kd_y*(PID_Struct.Y_Ek-PID_Struct.Y_Ek_pre)*10 + PID_Struct.PWM_Y_Middle+PID_Struct.Ki_y*Sum_y*0.1;
		PID_Struct.PWM_X = PID_Struct.Kp_x*PID_Struct.X_Ek +PID_Struct.Kd_x*(PID_Struct.X_Ek-PID_Struct.X_Ek_pre)*10 + PID_Struct.PWM_X_Middle+PID_Struct.Ki_x*Sum_x*0.1;		
		
		//PID_V.PWM_X = PID_V.Kp_x*PID_Struct.X_Ek +PID_V.Kd_x*(PID_V.X_Ek-PID_V.X_Ek_pre)+PID_Struct.PWM_X_Middle;		
		//位置环
		PID_Struct.X_Ek_pre = PID_Struct.X_Ek;
		PID_Struct.Y_Ek_pre = PID_Struct.Y_Ek;
		//速度环
		//PID_V.X_Ek_pre=PID_V.X_Ek;
		
		//printf("PWM_X:%d\r\n",PID_Struct.PWM_X);
	//	printf("Sum:%f\r\n",PID_Struct.Ki_x*Sum_x*0.1);		
//		printf("PID_Struct.X_Ek:%f\r\n",PID_Struct.X_Ek);				
		//printf("PID_Struct.X_EkD:%f\r\n",PID_Struct.X_EkD*PID_Struct.Kd);			
//		printf("PID_Struct.X_Ek_pre:%f\r\n",PID_Struct.X_Ek_pre);		
	count++;
	if(count>=1000||Sum_x>25000||Sum_y>25000)//对i进行发清零1200  2000
	{
		printf("count=%d\r\n",count);
		count=0;
		if(Sum_x>2500)		Sum_x=0;
		if(Sum_y>2500)		Sum_y=Sum_y/2;

	}		
	
		if(fabs(PID_Struct.X_Ek)<5) PID_Struct.PWM_X=PID_Struct.PWM_X_Middle;

		if(fabs(PID_Struct.Y_Ek)<5) PID_Struct.PWM_Y=PID_Struct.PWM_Y_Middle;	
		
		if(PID_Struct.PWM_X>1100)		PID_Struct.PWM_X=1100;
		if(PID_Struct.PWM_X<400)		PID_Struct.PWM_X=400;
		
		if(PID_Struct.PWM_Y>1100)		PID_Struct.PWM_Y=1100;
		if(PID_Struct.PWM_Y<400)		PID_Struct.PWM_Y=400;
	}
	
		TIM_SetCompare1(TIM3,PID_Struct.PWM_X);	 //更新PWM参数，改变输出占空比
		//TIM_SetCompare1(TIM3,PID_V.PWM_X);	 //更新PWM参数，改变输出占空比

		//printf("PID_Struct.PWM_X:%d\r\n",PID_Struct.PWM_X);
		TIM_SetCompare2(TIM3,PID_Struct.PWM_Y);
	

}
