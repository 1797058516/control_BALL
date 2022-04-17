
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
//#include "control.h"
#include "ustart_blue.h"
extern u8 Openmv_x;
extern u8 Openmv_y;
extern u8 blue_data[];

/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */
//������
void start_task(void *pvParameters);
void led0_task(void *pvParameters);


int main(void)
{	
 int temp=750;	
	BSP_Init();

  while(1)
	{
	delay_ms(2000);
	if(blue_data[2]==1)
	{
		//num += 100;
		printf("data:%d\r\n",blue_data[2]);
		TIM_SetCompare1(TIM3,900);
		//delays_ms();
		//SetPwm(num, 0);
		blue_data[2]=0;
	}	
	//USART_SendData( USART3,0x30);	
	}
}




//LED0������
void led0_task(void *pvParameters)
{
	int temp=750;
	//USART_SendData( USART3,0x30);
	while (1)
	{ 
		
//		LED0 = ~LED0;
//		delay_ms(500);

		//printf("Openmv_x:%d,Openmv_y:%d \r\n",blue_data[0],blue_data[1]);
		
		delay_ms(3000);
		if (300>=temp)
		{
			temp=750;
			
		}
		TIM_SetCompare1(TIM3, temp);
		TIM_SetCompare2(TIM3, temp);		
		temp--;
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

	TIM_SetCompare1(TIM3, 750);//250Ϊ0.5ms���ǵ�Ҫ�л���ģʽ1 PB4 ��
	TIM_SetCompare2(TIM3, 750);//750Ϊ1.5ms   1250Ϊ2.5ms  300 0.6ms--
	
}

