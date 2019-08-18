#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED���ú� ------------------------*/

#define LED1_PIN             GPIO_Pin_7        		 // LED1 ����      
#define LED1_PORT            GPIOG                  // LED1 GPIO�˿�     
#define LED1_CLK             RCC_AHB1Periph_GPIOG	 // LED1 GPIO�˿�ʱ��

#define LED2_PIN             GPIO_Pin_12            // LED2 ����             
#define LED2_PORT            GPIOD                  // LED2 GPIO�˿�          
#define LED2_CLK             RCC_AHB1Periph_GPIOD   // LED2 GPIO�˿�ʱ��

/*---------------------- LED���ƺ� ------------------------*/
					
#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// ����͵�ƽ������LED1	
#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// ����ߵ�ƽ���ر�LED1	

#define LED2_ON 	  GPIO_ResetBits(LED2_PORT,LED2_PIN);	// ����͵�ƽ������LED2
#define LED2_OFF 	  GPIO_SetBits(LED2_PORT,LED2_PIN);		// ����ߵ�ƽ���ر�LED2

/*---------------------- �������� ----------------------------*/

void LED_Init(void);	//LED��ʼ������


#endif //__LED_H

