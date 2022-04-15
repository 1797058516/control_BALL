
//#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

//�������ȼ�
#define START_TASK_PRIO 1
//�����ջ��С
#define START_STK_SIZE 150
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);


//�������ȼ�
#define LED0_TASK_PRIO 2
//�����ջ��С
#define LED0_STK_SIZE 80
//������
TaskHandle_t LED0Task_Handler;




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
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

	BSP_Init();

	//������ʼ����
	xReturn =xTaskCreate((TaskFunction_t)start_task,			 //������
				(const char *)"start_task",			 //��������
				(uint16_t)START_STK_SIZE,			 //�����ջ��С
				(void *)NULL,						 //���ݸ��������Ĳ���
				(UBaseType_t)START_TASK_PRIO,		 //�������ȼ�
				(TaskHandle_t *)&StartTask_Handler); //������
				
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
		
  while(1);   /* ��������ִ�е����� */    	
}


//��ʼ����������
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); //�����ٽ���
	//����LED0����
	xTaskCreate((TaskFunction_t)led0_task,
				(const char *)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t *)&LED0Task_Handler);
	
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();			//�˳��ٽ���
		
}

//LED0������
void led0_task(void *pvParameters)
{
	int temp=750;

	while (1)
	{ 
		
//		LED0 = ~LED0;
//		delay_ms(500);

		
		delay_ms(300);
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

//	TIM_SetCompare1(TIM3, 750);//250Ϊ0.5ms���ǵ�Ҫ�л���ģʽ1 PB4 ��
//	TIM_SetCompare2(TIM3, 750);//750Ϊ1.5ms   1250Ϊ2.5ms  300 0.6ms--
	
}

