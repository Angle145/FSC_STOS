/***
	***************************************************************************
	*	@file  	stm32f4xx_it.c
	*	@brief   �жϷ�����
   ***************************************************************************
   *  @description
	*
	*  ���ļ������STM32���е��쳣�������жϷ�����
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


//extern void TimingDelay_Decrement(void);	//����������������delay.c

	//������SysTick�жϷ�����

//void SysTick_Handler(void)
//{
//	TimingDelay_Decrement();
//}

/******************************************************************************/
/*                				                      									*/
/*  								STM32F4xx �����жϷ����� 									*/
/*  																									*/
/******************************************************************************/

// ����������1�жϺ���
// ˵���������յ������ݴ�� USART1_RecData ���飬���յ������ַ�0X0D��0X0A��
//			�����������ݡ��ڸ�����𴮿������У�0X0D��0X0A��Ӧ���ַ�����\r��\n
//

void	USART1_IRQHandler (void)
{
	uint8_t rxdata;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)	
	{	
    USART_ClearFlag(USART1, USART_FLAG_RXNE);    //��������жϱ�־
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);	
		
		rxdata=USART_ReceiveData(USART1);
		rxdata=rxdata;//��ֹ����
		
    #if (OS_CMD_ALL_ENABLE == 1)		
    OSCmdUsartReceiveByte( rxdata );	
    #endif		
	}	 	
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
