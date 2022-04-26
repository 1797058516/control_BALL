//#include "usart.h"
//#include "pid.h"
//#include "openmv.h"
//#include "math.h"




//PIDTypedef PID_Struct;
//extern u16 Openmv_x,Openmv_y;
//extern u16 Openmv[];

///*
////void PID_Init(PIDTypedef * PID_Struct)
////{
////	PID_Struct->Set_X = 0;
////	PID_Struct->Set_Y = 0;
////	
////	PID_Struct->Kp = 0;
////	PID_Struct->Kd = 0;
////	PID_Struct->Ki = 0;
////	
////	PID_Struct->X_Ek	= 0;
////	PID_Struct->X_Ek1 = 0;
////	PID_Struct->Y_Ek = 0;
////	PID_Struct->Y_Ek1 = 0;
////	
////	PID_Struct->PWM_X_Middle=750;
////	PID_Struct->PWM_Y_Middle=750;
////	PID_Struct->PWM_X = PID_Struct->PWM_X_Middle;
////	PID_Struct->PWM_Y = PID_Struct->PWM_Y_Middle;
////}
//*/
//void PID_Init(void)
//{
//	PID_Struct.Set_X = 80;
//	PID_Struct.Set_Y = 60;
////XXX	
////	PID_Struct.Kp = 2.6;
////	PID_Struct.Kd = 20;
////	PID_Struct.Ki = 0;
////YYY	
//	PID_Struct.Kp_y = 2.9;
//	PID_Struct.Kd_y = 23;//22
//	PID_Struct.Ki_y = 0.003;	//0.027
//	
//	PID_Struct.Kp_x = 1.5;//1.6 2  2,2          
//	PID_Struct.Kd_x = 0;//20 18  12  28
//	PID_Struct.Ki_x = 0.008;	//0.036   0.03
//	//pid 2.2 29 0,03 
//	PID_Struct.X_Ek	= 0;
//	PID_Struct.X_Ek_pre = 0;
//	PID_Struct.X_EkD=0;		
//	PID_Struct.Y_Ek = 0;
//	PID_Struct.Y_Ek_pre = 0;
//	PID_Struct.Y_EkD=0;		
//	PID_Struct.PWM_X_Middle=770;
//	PID_Struct.PWM_Y_Middle=770;
//	PID_Struct.PWM_X = PID_Struct.PWM_X_Middle;
//	PID_Struct.PWM_Y = PID_Struct.PWM_Y_Middle;
//}


//void PID_realize(void)
//{
//	static float Sum_x,Sum_y;
//	static u16 count=0;
//  //float X_ek;
//	if(Openmv[2]==253 && Openmv[3]==253) 
//	{	
//		PID_Struct.PWM_X = PID_Struct.PWM_X_Middle;
//		PID_Struct.PWM_Y = PID_Struct.PWM_Y_Middle;	
//	}
//	
//	else
//	{	

//		PID_Struct.X_Ek = PID_Struct.Set_X-Openmv[2];	//小球x坐标
//		PID_Struct.Y_Ek = Openmv[3]-PID_Struct.Set_Y;	//小球y坐标

//		//X_ek=PID_Struct.X_Ek;
//		//PID_Struct.X_EkD=X_ek-PID_Struct.X_Ek_pre;//PID of D;
//		//PID_Struct.Y_EkD=PID_Struct.Y_Ek-PID_Struct.Y_Ek_pre;	
//		
//	
//		//Sum=(PID_Struct.X_Ek_pre+PID_Struct.X_Ek)/2;		
//		Sum_x=Sum_x+(PID_Struct.X_Ek_pre+PID_Struct.X_Ek)/2;
//	  Sum_y=Sum_y+(PID_Struct.Y_Ek_pre+PID_Struct.Y_Ek)/2;
//		//printf("Openmvx:%d\r\n",Openmv[2]);
//		//PID_Struct.PWM_X = PID_Struct.Kp*PID_Struct.X_Ek +PID_Struct.Kd*(PID_Struct.X_Ek-PID_Struct.X_Ek_pre) + PID_Struct.PWM_X_Middle+PID_Struct.Ki*Sum;
//		PID_Struct.PWM_Y = PID_Struct.Kp_y*PID_Struct.Y_Ek +PID_Struct.Kd_y*(PID_Struct.Y_Ek-PID_Struct.Y_Ek_pre) + PID_Struct.PWM_Y_Middle+PID_Struct.Ki_y*Sum_y;
//		//PID_Struct.PWM_Y = PID_Struct.Kp*PID_Struct.Y_Ek +PID_Struct.Kd*((PID_Struct.Y_Ek-PID_Struct.Y_Ek1)/时间) + PID_Struct.PWM_Y_Middle;
//		PID_Struct.PWM_X = PID_Struct.Kp_x*PID_Struct.X_Ek +PID_Struct.Kd_x*(PID_Struct.X_Ek-PID_Struct.X_Ek_pre) + PID_Struct.PWM_X_Middle+PID_Struct.Ki_x*Sum_x;		
//		
//		PID_Struct.X_Ek_pre = PID_Struct.X_Ek;
//		PID_Struct.Y_Ek_pre = PID_Struct.Y_Ek;
//		//printf("PWM_X:%d\r\n",PID_Struct.PWM_X);
//		//printf("Sum:%f\r\n",Sum);		
////		printf("PID_Struct.X_Ek:%f\r\n",PID_Struct.X_Ek);				
//		//printf("PID_Struct.X_EkD:%f\r\n",PID_Struct.X_EkD*PID_Struct.Kd);			
////		printf("PID_Struct.X_Ek_pre:%f\r\n",PID_Struct.X_Ek_pre);		
//	count++;
//	if(count>=1000||Sum_x>3000||Sum_y>3000)//对i进行发清零1200  2000
//	{
//		printf("count=%d",count);
//		count=0;
//		if(Sum_x>2500)		Sum_x=Sum_x/2;
//		if(Sum_y>2500)		Sum_y=Sum_y/2;

//	}		
//	
//		if(fabs(PID_Struct.X_Ek)<5) PID_Struct.PWM_X=PID_Struct.PWM_X_Middle;

//		if(fabs(PID_Struct.Y_Ek)<5) PID_Struct.PWM_Y=PID_Struct.PWM_Y_Middle;	
//		
//		if(PID_Struct.PWM_X>1100)		PID_Struct.PWM_X=1100;
//		if(PID_Struct.PWM_X<400)		PID_Struct.PWM_X=400;
//		
//		if(PID_Struct.PWM_Y>1100)		PID_Struct.PWM_Y=1100;
//		if(PID_Struct.PWM_Y<400)		PID_Struct.PWM_Y=400;
//	}
//	
//		TIM_SetCompare1(TIM3,PID_Struct.PWM_X);	 //更新PWM参数，改变输出占空比
//		//TIM_SetCompare2(TIM3,PID_Struct.PWM_Y);
//	

//}


