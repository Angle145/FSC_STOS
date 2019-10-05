#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f4xx.h"

/*----------------------USART���ú� ------------------------*/
//����1
#define  USART1_TX_PIN				GPIO_Pin_9						// TX ����
#define	 USART1_TX_PORT				GPIOA							  	// TX ���Ŷ˿�
#define	 USART1_TX_CLK				RCC_AHB1Periph_GPIOA	// TX ����ʱ��
#define  USART1_TX_PinSource  GPIO_PinSource9				// ����Դ

#define  USART1_RX_PIN				GPIO_Pin_10          	// RX ����
#define	 USART1_RX_PORT				GPIOA                	// RX ���Ŷ˿�
#define	 USART1_RX_CLK				RCC_AHB1Periph_GPIOA  // RX ����ʱ��
#define  USART1_RX_PinSource  GPIO_PinSource10  		// ����Դ

//����2
#define  USART2_TX_PIN				GPIO_Pin_2					 	// TX ����
#define	 USART2_TX_PORT				GPIOA							   	// TX ���Ŷ˿�
#define	 USART2_TX_CLK				RCC_AHB1Periph_GPIOA	// TX ����ʱ��
#define  USART2_TX_PinSource  GPIO_PinSource2	 			// ����Դ

#define  USART2_RX_PIN				GPIO_Pin_3           	// RX ����
#define	 USART2_RX_PORT				GPIOA                	// RX ���Ŷ˿�
#define	 USART2_RX_CLK				RCC_AHB1Periph_GPIOA  // RX ����ʱ��
#define  USART2_RX_PinSource  GPIO_PinSource3   		// ����Դ

/*---------------------- �������� ----------------------------*/

void  Usart1_Config (u32 BaudRate);	// USART��ʼ������
void  Usart2_Config (u32 BaudRate);	// USART��ʼ������

#endif //__USART_H

