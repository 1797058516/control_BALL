#ifndef __PID_H
#define __PID_H

typedef struct
{
	int Set_X;
	int Set_Y;
		
	float Kp;
	float Ki;
	float Kd;
	
	float X_Ek;
	float X_Ek1;
	float Y_Ek;
	float Y_Ek1;
	
	int PWM_X;
	int PWM_Y;
	
	int PWM_X_Middle; //舵机中值
	int PWM_Y_Middle; //舵机中值
	
}PIDTypedef;

//void PID_Init(PIDTypedef * PID_Struct);
void PID_Init();
void PID_realize(void);
#endif
