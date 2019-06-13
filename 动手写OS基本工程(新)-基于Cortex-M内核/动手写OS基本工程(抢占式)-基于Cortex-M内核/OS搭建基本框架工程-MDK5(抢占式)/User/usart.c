#include "usart.h"

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
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);
//PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                     

	GPIO_Init(GPIOA, &GPIO_InitStructure);
//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                    

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = baudRate;                 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
	USART_InitStructure.USART_StopBits = USART_StopBits_1;         
	USART_InitStructure.USART_Parity = USART_Parity_No;            
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

