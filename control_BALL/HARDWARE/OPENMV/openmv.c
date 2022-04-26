#include "openmv.h"
#include "usart.h"	
#include "control.h"
u16 Openmv[5];
u16 Openmv_x = 0, Openmv_y = 0;

extern PIDTypedef PID_Struct;

void uart_init2(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口1 

}

void USART2_IRQHandler(void) //中断服务函数
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res = USART_ReceiveData(USART2);	//读取接收到的数据
		Openmv_Receive_Data(Res);
		//USART_SendData(USART3, Res); 
			
			
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 


void Openmv_Receive_Data(u16 data)
{
	static u8 state = 0;	
	static u8 bit_number=0;		
	if(state==0&&data==0xFF)	//第一位校验位0xAD
	{
		state=1;
		Openmv[bit_number++]=data;
	}
	else if(state==1&&data==0xFE)		//第二位校验位18
	{
		state=2;
		Openmv[bit_number++]=data;
	}
	else if(state==2)		
	{
		Openmv[bit_number++]=data;
		if(bit_number>=4)
		{
			state=3;
		}
	}
	else if(state==3)		//检测是否接受到结束标志
	{
		if(data == 0x5B)	//最后一位终止位0x5B
		{
					state = 0;
					bit_number=0;	
					Openmv_x=Openmv[2];
					Openmv_y=Openmv[3];
					//Openmv[bit_number++]=data;					
					PID_realize();
					
		}
		else if(data != 0x5B)
		{
				 state = 0;
				 bit_number=0;						
		}
	}    
	else
	{
		state = 0;
		bit_number=0;
	}

	/*
	if((flag == 0) && (date == 0xFF))
	{
		flag = 1;
		Openmv[0] = date;
	}
	else if((flag == 1) && (date == 0xFE))
	{
		flag = 2;
		Openmv[1] = date;
	}
	else if(flag == 2)
	{
		flag = 3;
		Openmv[2] = date;
	}
	else if(flag == 3)
	{
			flag = 0;
			Openmv[3] = date;
			Openmv_Date();
			//printf(" PID_Struct;%d",PID_Struct.Set_X);
			//printf(" Openmv_x;%d",Openmv_x);
			PID_realize();
	}
	else if()
	else
	{
		int j;
		flag = 0;
		for(j=0;j<4;j++)
		{
			Openmv[j] = 0;
		}
		
	}
*/	
}

void Openmv_Date(void)
{
	Openmv_x = Openmv[2];
	Openmv_y = Openmv[3];
}



