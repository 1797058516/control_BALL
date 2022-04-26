#include "control.h"

int init_pwm1,init_pwm2;

void SetPwm(int x, int y)
{
	 init_pwm1 = 750 + x;
	 init_pwm2 = 750 + y;
	 Limit_PWM(init_pwm1,init_pwm2);
	 TIM_SetCompare1(TIM3, init_pwm1);  //250Ϊ0.5ms���ǵ�Ҫ�л���ģʽ1 PB4 ��
	 TIM_SetCompare2(TIM3, init_pwm2);  //��
}

void Limit_PWM(int x,int y)
{
	if(x<=400) x=400;
	if(y<=400) y=400;	
	if(x>=1100) x=1100;
	if(y>=1100) y=1100;	
}