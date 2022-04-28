#include "openmv.h"
#include "usart.h"	
#include "control.h"
u16 Openmv[5];
u16 Openmv_x = 253, Openmv_y = 253;

extern PIDTypedef PID_Struct;

void uart_init2(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 

}

void USART2_IRQHandler(void) //�жϷ�����
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res = USART_ReceiveData(USART2);	//��ȡ���յ�������
		Openmv_Receive_Data(Res);
		//USART_SendData(USART3, Res); 
				
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 


void Openmv_Receive_Data(u16 data)
{
	static u8 state = 0;	
	static u8 bit_number=0;		
	if(state==0&&data==0xFF)	//��һλУ��λ0xAD
	{
		state=1;
		Openmv[bit_number++]=data;
	}
	else if(state==1&&data==0xFE)		//�ڶ�λУ��λ18
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
	else if(state==3)		//����Ƿ���ܵ�������־
	{
		if(data == 0x5B)	//���һλ��ֹλ0x5B
		{
					state = 0;
					bit_number=0;	
					Openmv_x=Openmv[2];
					Openmv_y=Openmv[3];
					//Openmv[bit_number++]=data;					
					//PID_realize();
					
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



