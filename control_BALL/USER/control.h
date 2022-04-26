#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "sys.h"
typedef struct
{
	int Set_X;
	int Set_Y;
		
	float Kp_x;
	float Ki_x;
	float Kd_x;
	
	float Kp_y;
	float Ki_y;
	float Kd_y;	
	
	float X_Ek;
	float X_EkD;	
	float X_Ek_pre;
	float Y_Ek;
	float Y_EkD;	
	float Y_Ek_pre;
	
	int PWM_X;
	int PWM_Y;
	
	int PWM_X_Middle; //舵机中值
	int PWM_Y_Middle; //舵机中值
	
}PIDTypedef;


//void PID_Init(PIDTypedef * PID_Struct);
void PID_Init();
void PID_realize(void);
extern PIDTypedef PID_Struct;




void Limit_PWM(int x,int y);
void SetPwm(int x, int y);
extern int init_pwm1,init_pwm2;


#endif
