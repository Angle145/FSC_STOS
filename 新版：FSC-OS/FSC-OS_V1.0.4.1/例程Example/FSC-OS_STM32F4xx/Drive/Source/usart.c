#include "usart.h"  

// �������ض���fputc����
#pragma import(__use_no_semihosting)    //����Ҫ��ѡ micro lib                        
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;          
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int c, FILE *fp) //printf����ʹ�õ�USARTx���޸Ĵ˴���USARTx����Ҫʹ�õĴ���
{
	USART_SendData( USART1,(u8)c );	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (c); 
}


// ������usart IO�ڳ�ʼ��
//
void  USART1_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART1_TX_CLK|USART1_RX_CLK, ENABLE); 	//IO��ʱ������

	//IO����
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   	 //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶȵȼ�

	//��ʼ�� TX	����
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;	 
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);	
	//��ʼ�� RX ����													   
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;	
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);		
	
	//IO���ã����õ�USART1
	GPIO_PinAFConfig(USART1_TX_PORT,USART1_TX_PinSource,GPIO_AF_USART1); 
	GPIO_PinAFConfig(USART1_RX_PORT,USART1_RX_PinSource,GPIO_AF_USART1);	
}

// ������USART �ڳ�ʼ��
//
void Usart1_Config(u32 BaudRate)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// IO�ڳ�ʼ��
	USART1_GPIO_Config();
		 
	// ���ô��ڸ������
	USART_InitStructure.USART_BaudRate 	 = BaudRate; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //ֹͣλ1λ
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //��У��
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��ʹ��Ӳ��������
	
	USART_Init(USART1,&USART_InitStructure); //��ʼ������1
	USART_Cmd(USART1,ENABLE);	//ʹ�ܴ���1
}

/*-----------------------------------------------------------------------------------------*/

// ������usart IO�ڳ�ʼ��
//
void  USART2_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART2_TX_CLK|USART2_RX_CLK, ENABLE); 	//IO��ʱ������

	//IO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	 //����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶȵȼ�

	//��ʼ�� TX	����
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;	 
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);	
	//��ʼ�� RX ����													   
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;	
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);		
	
	//IO���ã����õ�USART2
	GPIO_PinAFConfig(USART2_TX_PORT,USART2_TX_PinSource,GPIO_AF_USART2); 
	GPIO_PinAFConfig(USART2_RX_PORT,USART2_RX_PinSource,GPIO_AF_USART2);	
}

// ������USART �ڳ�ʼ��
//
void Usart2_Config(u32 BaudRate)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //��������ʱ��
	
	USART2_GPIO_Config();	// IO�ڳ�ʼ��

	USART_InitStructure.USART_BaudRate 	 = BaudRate; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //����λ8λ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //ֹͣλ1λ
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //��У��
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��ʹ��Ӳ��������
	
	USART_Init(USART2,&USART_InitStructure); //��ʼ������2	
	USART_Cmd(USART2,ENABLE);	//ʹ�ܴ���2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//ʹ�ܴ����жϽ���
}






