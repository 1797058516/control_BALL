
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
#include "openmv.h"
#include "ustart_blue.h"
//#include "control.h"
#include "ustart_blue.h"
#include "pid.h"

extern u16 Openmv_x,Openmv_y;
//extern u16 Openmv_y;
extern u8 blue_data[];
extern PIDTypedef PID_Struct;
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t PID_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LOCATION_Task_Handle = NULL;
/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */
//������
static void AppTaskCreate(void);/* ���ڴ������� */
static void PID_TASK(void *pvParameters);
static void LOCATION_TASK(void *pvParameters);
//PIDTypedef *PID_struct;

int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	BSP_Init();
	PID_Init();
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )2, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 	
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
//	PID_Init(PID_struct);
//  while(1)
//	{
//	//delay_ms(2);
//	//printf("Openmv_x:%d,Openmv_y:%d\r\n",Openmv_x,Openmv_y);
//		//PID_realize();
//	}
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

	TIM_SetCompare1(TIM3, 770);//250Ϊ0.5ms���ǵ�Ҫ�л���ģʽ1 PB4 ��
	TIM_SetCompare2(TIM3, 770);//750Ϊ1.5ms   1250Ϊ2.5ms  300 0.6ms--
	
}

static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����PID_TASK���� */
  xReturn = xTaskCreate((TaskFunction_t )PID_TASK, /* ������ں��� */
                        (const char*    )"PID_TASK",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&PID_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED1_Task����ɹ�!\r\n");
  
	/* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LOCATION_TASK, /* ������ں��� */
                        (const char*    )"LOCATION_TASK",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LOCATION_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED2_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}
static void PID_TASK(void *pvParameters)
{
    while (1)
    {
		   //delay_ms(2000);
			printf("PID_TASK\r\n");

    }
}

static void LOCATION_TASK(void *pvParameters)
{
    while (1)
    {
		   delay_ms(200);		
			printf("LOCATION_TASK\r\n");

    }
}

