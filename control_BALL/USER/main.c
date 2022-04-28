
//#include "stm32f10x.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "openmv.h"
#include "ustart_blue.h"
#include "control.h"
#include "ustart_blue.h"
//#include "pid.h"

extern u16 Openmv_x;
extern u16 Openmv_y;
extern u8 blue_data[];
extern PIDTypedef PID_Struct;
/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */
//������
void start_task(void *pvParameters);
void led0_task(void *pvParameters);
//PIDTypedef *PID_struct;

int main(void)
{	
	//int temp=750;
	BSP_Init();
	PID_Init();
//	PID_Init(PID_struct);

  while(1)
	{
	//delay_ms(2);
	//printf("Openmv_x:%d,Openmv_y:%d\r\n",Openmv_x,Openmv_y);
		//PID_realize();
		
//		TIM_SetCompare1(TIM3, 1000);
//		delay_ms(1000);
//		TIM_SetCompare1(TIM3, 750);
		delay_ms(500);
//		LED0=!LED0;
		
//		temp++;
//		if (temp>=1150)
//		{temp=750;}
//		TIM_SetCompare1(TIM3, temp);
		//delay_ms(1000);
		//TIM_SetCompare1(TIM3, 1100);	
	}
}


/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����ϵͳ�ж����ȼ�����4
	delay_init();									//��ʱ������ʼ��
	uart_init(115200);								//��ʼ������
	uart_init2(115200);						//openmv
	uart_init3(115200);						//����
	LED_Init();										//��ʼ��LED
	/*
	//�������Ĭ��Ƶ��50Hz
	//��׼50Hzʱ��ռ�ձȷ�Χ3%~13%
	//��׼����ߵ�ƽ��Χ0.5ms~2.5ms
	//�˴�ȡlimLowMs=0.6ms~limHighMs=2.4ms����һЩ����
//�������
//֧�ֶ˿ڣ�    
//��ʱ�����Ŷ�Ӧ�б�
//ͨ�� CH1  CH2  CH3  CH4
//TIM1 PA8FT  PA9FT  PA10FT PA11FT
//TIM2 PA0  PA1  PA2  PA3
//TIM3 PA6  PA7  PB0  PB1
//TIM4 PB6FT  PB7FT  PB8FT  PB9FT	
*/
	TIM3_PWM_Init(9999,143);	 //144��Ƶ��PWMƵ��=72000000/��10000*144��=50hz
	//TIM4_Int_Init(4999,7199);    //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	TIM4_Int_Init(4999,1439);//100ms
	TIM_SetCompare1(TIM3, 730);//250Ϊ0.5ms���ǵ�Ҫ�л���ģʽ1 PB4 ��
	TIM_SetCompare2(TIM3, 750);//750Ϊ1.5ms   1250Ϊ2.5ms  300 0.6ms--
	
}

