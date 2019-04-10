/***
	***************************************************************************
	*	@file  	stm32f4xx_it.c
	*	@brief   中断服务函数
   ***************************************************************************
   *  @description
	*
	*  此文件存放了STM32所有的异常操作和中断服务函数
	* 	
	***************************************************************************
***/
#include "stm32f4xx_it.h"
#include "fsc_stos.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}


//extern void TimingDelay_Decrement(void);	//函数声明，定义在delay.c

	//函数：SysTick中断服务处理

//void SysTick_Handler(void)
//{
//	TimingDelay_Decrement();
//}

/******************************************************************************/
/*                				                      									*/
/*  								STM32F4xx 外设中断服务函数 									*/
/*  																									*/
/******************************************************************************/

// 函数：串口1中断函数
// 说明：将接收到的数据存进 USART1_RecData 数组，若收到结束字符0X0D和0X0A则
//			结束接收数据。在格西烽火串口助手中，0X0D和0X0A对应的字符串是\r和\n
//

void	USART1_IRQHandler (void)
{
	uint8_t rxdata;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	
	{	
    USART_ClearFlag(USART1, USART_FLAG_RXNE);    //清除接收中断标志
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		
		rxdata=USART_ReceiveData(USART1);
		rxdata=rxdata;//仿止警告
		
    #if (OS_CMD_ALL_ENABLE == 1)		
    OSCmdUsartReceiveByte( rxdata );	
    #endif		
	}	 	
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
