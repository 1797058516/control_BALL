
//#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"


//任务优先级
#define START_TASK_PRIO 1
//任务堆栈大小
#define START_STK_SIZE 150
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);


//任务优先级
#define LED0_TASK_PRIO 2
//任务堆栈大小
#define LED0_STK_SIZE 80
//任务句柄
TaskHandle_t LED0Task_Handler;




/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

static void BSP_Init(void);/* 用于初始化板载相关资源 */
//任务函数
void start_task(void *pvParameters);
void led0_task(void *pvParameters);


int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	BSP_Init();

	//创建开始任务
	xReturn =xTaskCreate((TaskFunction_t)start_task,			 //任务函数
				(const char *)"start_task",			 //任务名称
				(uint16_t)START_STK_SIZE,			 //任务堆栈大小
				(void *)NULL,						 //传递给任务函数的参数
				(UBaseType_t)START_TASK_PRIO,		 //任务优先级
				(TaskHandle_t *)&StartTask_Handler); //任务句柄
				
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
		
  while(1);   /* 正常不会执行到这里 */    	
}


//开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL(); //进入临界区
	//创建LED0任务
	xTaskCreate((TaskFunction_t)led0_task,
				(const char *)"led0_task",
				(uint16_t)LED0_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)LED0_TASK_PRIO,
				(TaskHandle_t *)&LED0Task_Handler);
	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();			//退出临界区
}

//LED0任务函数
void led0_task(void *pvParameters)
{
	while (1)
	{ 
		
		LED0 = ~LED0;
		delay_ms(500);
	}
}
/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
	delay_init();									//延时函数初始化
	uart_init(115200);								//初始化串口
	LED_Init();										//初始化LED
  
}

