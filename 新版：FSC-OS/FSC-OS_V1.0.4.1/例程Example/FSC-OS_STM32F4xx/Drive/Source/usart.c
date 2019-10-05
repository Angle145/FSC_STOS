#include "usart.h"  

// 函数：重定义fputc函数
#pragma import(__use_no_semihosting)    //不需要勾选 micro lib                        
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;          
void _sys_exit(int x) 
{ 
	x = x; 
} 
int fputc(int c, FILE *fp) //printf函数使用的USARTx，修改此处的USARTx成想要使用的串口
{
	USART_SendData( USART1,(u8)c );	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return (c); 
}


// 函数：usart IO口初始化
//
void  USART1_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART1_TX_CLK|USART1_RX_CLK, ENABLE); 	//IO口时钟配置

	//IO配置
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;   	 //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		 //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度等级

	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;	 
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;	
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);		
	
	//IO复用，复用到USART1
	GPIO_PinAFConfig(USART1_TX_PORT,USART1_TX_PinSource,GPIO_AF_USART1); 
	GPIO_PinAFConfig(USART1_RX_PORT,USART1_RX_PinSource,GPIO_AF_USART1);	
}

// 函数：USART 口初始化
//
void Usart1_Config(u32 BaudRate)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	// IO口初始化
	USART1_GPIO_Config();
		 
	// 配置串口各项参数
	USART_InitStructure.USART_BaudRate 	 = BaudRate; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //停止位1位
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //无校验
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(USART1,&USART_InitStructure); //初始化串口1
	USART_Cmd(USART1,ENABLE);	//使能串口1
}

/*-----------------------------------------------------------------------------------------*/

// 函数：usart IO口初始化
//
void  USART2_GPIO_Config	(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd ( USART2_TX_CLK|USART2_RX_CLK, ENABLE); 	//IO口时钟配置

	//IO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   	 //复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		 //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度等级

	//初始化 TX	引脚
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;	 
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);	
	//初始化 RX 引脚													   
	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;	
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);		
	
	//IO复用，复用到USART2
	GPIO_PinAFConfig(USART2_TX_PORT,USART2_TX_PinSource,GPIO_AF_USART2); 
	GPIO_PinAFConfig(USART2_RX_PORT,USART2_RX_PinSource,GPIO_AF_USART2);	
}

// 函数：USART 口初始化
//
void Usart2_Config(u32 BaudRate)
{		
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //开启串口时钟
	
	USART2_GPIO_Config();	// IO口初始化

	USART_InitStructure.USART_BaudRate 	 = BaudRate; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //数据位8位
	USART_InitStructure.USART_StopBits   = USART_StopBits_1; //停止位1位
	USART_InitStructure.USART_Parity     = USART_Parity_No ; //无校验
	USART_InitStructure.USART_Mode 	    = USART_Mode_Rx | USART_Mode_Tx; //发送和接收模式
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 不使用硬件流控制
	
	USART_Init(USART2,&USART_InitStructure); //初始化串口2	
	USART_Cmd(USART2,ENABLE);	//使能串口2
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//使能串口中断接收
}






