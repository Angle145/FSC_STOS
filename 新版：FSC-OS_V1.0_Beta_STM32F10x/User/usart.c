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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b：8位数据，USART_WordLength_9b：9位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5：0.5个停止位，USART_StopBits_1：1个，USART_StopBits_1_5：1.5个，USART_StopBits_2：2个
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No：无奇偶校验，USART_Parity_Even：偶校验，USART_Parity_Odd：奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式，此处收发均使能
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//开启发送中断
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void USART1_IRQHandler(void)  //串口1中断  PA9(TX) PA10(RX) //接收到新字节时产生中断
{
	uint8_t Rxdata;
	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //检查指定的USART中断发生与否
	{
    Rxdata= USART_ReceiveData(USART1);	         //串口接收1字节数据
		USART_ClearFlag(USART1, USART_FLAG_RXNE);    //清除接收中断标志
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); //以下为自带的接收缓存,在示例说明TXT里有使用说明	
		//系统指令函数//	
		os_shell_input(Rxdata); //使用串口1作为系统指令(放在串口1中断里就使用串口1，放在其他的就使用其他的串口)   	
	}
}
