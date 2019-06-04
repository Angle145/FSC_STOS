#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED配置宏 ------------------------*/

#define LED1_PIN             GPIO_Pin_7        		 // LED1 引脚      
#define LED1_PORT            GPIOG                  // LED1 GPIO端口     
#define LED1_CLK             RCC_AHB1Periph_GPIOG	 // LED1 GPIO端口时钟

#define LED2_PIN             GPIO_Pin_12            // LED2 引脚             
#define LED2_PORT            GPIOD                  // LED2 GPIO端口          
#define LED2_CLK             RCC_AHB1Periph_GPIOD   // LED2 GPIO端口时钟

/*---------------------- LED控制宏 ------------------------*/
					
#define LED1_ON 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// 输出低电平，点亮LED1	
#define LED1_OFF 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// 输出高电平，关闭LED1	

#define LED2_ON 	  GPIO_ResetBits(LED2_PORT,LED2_PIN);	// 输出低电平，点亮LED2
#define LED2_OFF 	  GPIO_SetBits(LED2_PORT,LED2_PIN);		// 输出高电平，关闭LED2

/*---------------------- 函数声明 ----------------------------*/

void LED_Init(void);	//LED初始化函数


#endif //__LED_H

