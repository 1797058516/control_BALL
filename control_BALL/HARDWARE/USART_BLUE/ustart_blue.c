# include "ustart_blue.h"
# include "usart.h"
#	include "string.h"
#include "control.h"
#include "pid.h"
u8 blue[8]={0};
u8 blue_data[8]={0};
int num=0;
int num2=0;
extern PIDTypedef PID_Struct;
void uart_init3(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������  TX
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
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

  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

}

void USART3_IRQHandler(void) //�жϷ�����
{
	static u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		recetive_blue_32(Res);
		//blue_control();

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
}
 
void recetive_blue_32(u8 data)//��1���ֽ�����
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
	else if((flag==3)&&(b==blue[6]))		//����Ƿ���ܵ�������־
	{
		if(data == 0x5A)	//���һλ��ֹλ0x5B
		{		
					flag = 0;	
						
					memcpy(blue_data,blue,bit_number);
					//printf("data:%d\r\n",blue_data[2]);
					bit_number =0;
					blue_control();
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



void blue_control()
{
	if(blue_data[1]==1)
	{
		//SetPwm(0, 0);
		PID_Struct.Set_X=80;
		PID_Struct.Set_Y=60;		
	}
	else if(blue_data[2]==1)
	{
//		num =num+10;
//		SetPwm(0, num);
		PID_Struct.Set_X=20;
		PID_Struct.Set_Y=60;
		blue_data[2]=0;
	}	
	else if(blue_data[3]==1)
	{
		num =num-10;
		SetPwm(0, num);
		blue_data[3]=0;
	}	
	else if(blue_data[4]==1)
	{
		num2 =num2-10;
		SetPwm(num2, 0);
		blue_data[4]=0;
	}		
	else if(blue_data[5]==1)
	{
		num2 =num2+10;
		SetPwm(num2, 0);
		blue_data[5]=0;
	}			
}

