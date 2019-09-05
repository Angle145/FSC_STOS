#include "usart.h"
#include "os.h"

#pragma import(__use_no_semihosting)                             
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;    
void _sys_exit(int x) 
{ 
	x = x; 
}
int fputc(int ch, FILE *f)
{	
	USART_GetFlagStatus(USART1, USART_FLAG_TC); 
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	return ch;	
}
void USART1_Config(u32 baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                     
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b��8λ���ݣ�USART_WordLength_9b��9λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5��0.5��ֹͣλ��USART_StopBits_1��1����USART_StopBits_1_5��1.5����USART_StopBits_2��2��
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No������żУ�飬USART_Parity_Even��żУ�飬USART_Parity_Odd����У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ���˴��շ���ʹ��
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	//���������ж�
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//���������ж�
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void USART1_IRQHandler(void)  //����1�ж�  PA9(TX) PA10(RX) //���յ����ֽ�ʱ�����ж�
{
	uint8_t Rxdata;
	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //���ָ����USART�жϷ������
	{
    Rxdata= USART_ReceiveData(USART1);	         //���ڽ���1�ֽ�����
		USART_ClearFlag(USART1, USART_FLAG_RXNE);    //��������жϱ�־
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); //����Ϊ�Դ��Ľ��ջ���,��ʾ��˵��TXT����ʹ��˵��	
		//ϵͳָ���//	
		os_shell_input(Rxdata); //ʹ�ô���1��Ϊϵͳָ��(���ڴ���1�ж����ʹ�ô���1�����������ľ�ʹ�������Ĵ���)   	
	}
}
