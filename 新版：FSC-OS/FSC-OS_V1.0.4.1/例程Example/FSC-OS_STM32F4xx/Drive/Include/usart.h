#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"

/*----------------------USART配置宏 ------------------------*/
//串口1
#define  USART1_TX_PIN				GPIO_Pin_9						// TX 引脚
#define	 USART1_TX_PORT				GPIOA							  	// TX 引脚端口
#define	 USART1_TX_CLK				RCC_AHB1Periph_GPIOA	// TX 引脚时钟
#define  USART1_TX_PinSource  GPIO_PinSource9				// 引脚源

#define  USART1_RX_PIN				GPIO_Pin_10          	// RX 引脚
#define	 USART1_RX_PORT				GPIOA                	// RX 引脚端口
#define	 USART1_RX_CLK				RCC_AHB1Periph_GPIOA  // RX 引脚时钟
#define  USART1_RX_PinSource  GPIO_PinSource10  		// 引脚源

//串口2
#define  USART2_TX_PIN				GPIO_Pin_2					 	// TX 引脚
#define	 USART2_TX_PORT				GPIOA							   	// TX 引脚端口
#define	 USART2_TX_CLK				RCC_AHB1Periph_GPIOA	// TX 引脚时钟
#define  USART2_TX_PinSource  GPIO_PinSource2	 			// 引脚源

#define  USART2_RX_PIN				GPIO_Pin_3           	// RX 引脚
#define	 USART2_RX_PORT				GPIOA                	// RX 引脚端口
#define	 USART2_RX_CLK				RCC_AHB1Periph_GPIOA  // RX 引脚时钟
#define  USART2_RX_PinSource  GPIO_PinSource3   		// 引脚源

/*---------------------- 函数声明 ----------------------------*/

void  Usart1_Config (u32 BaudRate);	// USART初始化函数
void  Usart2_Config (u32 BaudRate);	// USART初始化函数

#endif //__USART_H

