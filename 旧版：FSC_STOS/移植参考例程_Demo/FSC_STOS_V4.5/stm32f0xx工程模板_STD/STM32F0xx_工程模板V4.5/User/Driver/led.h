#ifndef __led_H
#define __led_H 
#include "stm32f0xx.h"

#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA 
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_4


void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);

#endif



