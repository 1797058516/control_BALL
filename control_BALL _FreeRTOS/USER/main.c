
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
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t PID_Task_Handle = NULL;
/* LED2任务句柄 */
static TaskHandle_t LOCATION_Task_Handle = NULL;
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

static void BSP_Init(void);/* 用于初始化板载相关资源 */
//任务函数
static void AppTaskCreate(void);/* 用于创建任务 */
static void PID_TASK(void *pvParameters);
static void LOCATION_TASK(void *pvParameters);
//PIDTypedef *PID_struct;

int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	BSP_Init();
	PID_Init();
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )2, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 	
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
//	PID_Init(PID_struct);
//  while(1)
//	{
//	//delay_ms(2);
//	//printf("Openmv_x:%d,Openmv_y:%d\r\n",Openmv_x,Openmv_y);
//		//PID_realize();
//	}
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
	uart_init2(115200);						//openmv
	uart_init3(115200);						//蓝牙
	LED_Init();										//初始化LED
	/*
	//构建舵机默认频率50Hz
	//标准50Hz时，占空比范围3%~13%
	//标准舵机高电平范围0.5ms~2.5ms
	//此处取limLowMs=0.6ms~limHighMs=2.4ms，留一些余量
//舵机驱动
//支持端口：    
//定时器引脚对应列表
//通道 CH1  CH2  CH3  CH4
//TIM1 PA8FT  PA9FT  PA10FT PA11FT
//TIM2 PA0  PA1  PA2  PA3
//TIM3 PA6  PA7  PB0  PB1
//TIM4 PB6FT  PB7FT  PB8FT  PB9FT	
*/
	TIM3_PWM_Init(9999,143);	 //144分频。PWM频率=72000000/（10000*144）=50hz

	TIM_SetCompare1(TIM3, 770);//250为0.5ms，记得要切换成模式1 PB4 左
	TIM_SetCompare2(TIM3, 770);//750为1.5ms   1250为2.5ms  300 0.6ms--
	
}

static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建PID_TASK任务 */
  xReturn = xTaskCreate((TaskFunction_t )PID_TASK, /* 任务入口函数 */
                        (const char*    )"PID_TASK",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&PID_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED1_Task任务成功!\r\n");
  
	/* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )LOCATION_TASK, /* 任务入口函数 */
                        (const char*    )"LOCATION_TASK",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LOCATION_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED2_Task任务成功!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
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

