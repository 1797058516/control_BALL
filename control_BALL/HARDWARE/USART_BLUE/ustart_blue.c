# include "ustart_blue.h"
# include "usart.h"
#	include "string.h"
u8 blue[8]={0};
u8 blue_data[8]={0};
void uart_init3(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出  TX
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
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

  USART_Init(USART3, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

void USART3_IRQHandler(void) //中断服务函数
{
	static u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		recetive_blue_32(Res);
		

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
}
 
void recetive_blue_32(u8 data)//收1个字节数据
{
	static u8 flag = 0;
	static u8 bit_number=0;	
	u8 i;
	u32 l;
	static u8 b;	
	if(flag == 0 && data==0xA5)
	{		
		flag = 1;
		blue[bit_number++] = data;
	}
	else if(flag==1)		
	{
		blue[bit_number++]=data;
		if(bit_number>=6)
		{
			//printf("data:%d\r\n",bit_number);
			flag=2;
		}		
	}
	else if(flag==2)		
	{
		flag=3;
		//printf("data:%d\r\n",bit_number);
		blue[bit_number++]=data;
		for(i=1;i<=bit_number-2;i++) l+=blue[i];
		//printf("data:%d\r\n",blue[5]);
		b=l;			
	}	
	else if((flag==3)&&(b==blue[6]))		//检测是否接受到结束标志
	{
		if(data == 0x5A)	//最后一位终止位0x5B
		{		
					flag = 0;	
						
					memcpy(blue_data,blue,bit_number);
					printf("data:%d\r\n",blue_data[2]);
					bit_number =0;
					//PID_realize();
			
		}
		else if(blue[7] != 0x5A)
		{
				 memset( blue,0x00,bit_number);
				 flag = 0;
				 bit_number=0;						
		}
	}
	else
	{
		flag = 0;
		bit_number=0;
	} 	
}