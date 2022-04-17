
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

extern u8 Openmv_x;
extern u8 Openmv_y;
extern u8 blue_data[];

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
	BSP_Init();

  while(1)
	{
	delay_ms(2000);
	//USART_SendData( USART3,0x30);		
	}
}




//LED0任务函数
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
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置系统中断优先级分组4
	delay_init();									//延时函数初始化
	uart_init(115200);								//初始化串口
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

//	TIM_SetCompare1(TIM3, 750);//250为0.5ms，记得要切换成模式1 PB4 左
//	TIM_SetCompare2(TIM3, 750);//750为1.5ms   1250为2.5ms  300 0.6ms--
	
}

