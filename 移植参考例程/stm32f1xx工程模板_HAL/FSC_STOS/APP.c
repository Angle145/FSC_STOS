#include "fsc_stos.h"  //使用多任务内核

void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);

/* 任务名称任意，此处为方便直观取名为Task1-5 */
/******************************创建任务参数*************************/
void Task1(void); //任务1声明
void Task2(void); //任务2声明
void Task3(void); //任务3声明
void Task4(void); //任务4声明
void Task5(void); //任务5声明

#define Task1_StkSize 128       //任务1堆栈大小
#define Task2_StkSize 128       //任务2堆栈大小 
#define Task3_StkSize 128       //任务3堆栈大小 
#define Task4_StkSize 128       //任务4堆栈大小
#define Task5_StkSize 128       //任务5堆栈大小

OSSTK Task1_Stk[Task1_StkSize]; //任务1堆栈
OSSTK Task2_Stk[Task2_StkSize]; //任务2堆栈
OSSTK Task3_Stk[Task3_StkSize]; //任务3堆栈
OSSTK Task4_Stk[Task4_StkSize]; //任务4堆栈
OSSTK Task5_Stk[Task5_StkSize]; //任务5堆栈

void OS_MAIN(void)
{
	 /*-----------------------------用户全局初使化区---------------------------*/
	 /*推荐把所有任务都使用到的初使化放在此处，Task独立用到的初使化放在Task内*/
	 
	 USART_ITConfig(USART1, ((uint16_t)0x0525), ENABLE);//移植的标准库串口1接收中断使能(HAL库串口接收比较难用)
	
	
	 /*-----------------------------------------------------------------------*/
	 /***********************************************************************************/	
	 OSInit(); //系统初使化
	 /********************************在系统中创建任务***********************************/
	 /*           任务名 任务函数名 任务栈     任务栈大小   任务状态  任务优先级(越大越高，0-65535,=0时不运行)*/
	 OSTaskCreate("Task1",Task1,Task1_Stk,Task1_StkSize,TASK_RUNNING,1); //OS创建任务1
	 OSTaskCreate("Task2",Task2,Task2_Stk,Task2_StkSize,TASK_RUNNING,2); //OS创建任务2
	 OSTaskCreate("Task3",Task3,Task3_Stk,Task3_StkSize,TASK_RUNNING,3); //OS创建任务3
	 OSTaskCreate("Task4",Task4,Task4_Stk,Task4_StkSize,TASK_RUNNING,4); //OS创建任务4
	 OSTaskCreate("Task5",Task5,Task5_Stk,Task5_StkSize,TASK_RUNNING,5); //OS创建任务5
	 /***********************************************************************************/
	 OSStartUp();//OS开始运行
}
/*******************************************************************/

/***********************************用户任务代码编程区**************************************/

extern UART_HandleTypeDef huart1;
int fputc(int ch,FILE *f) //printf函数重定向
{
  uint8_t temp[1]={ch};
	HAL_UART_Transmit(&huart1,temp,1,2);//使用串口1
	return ch;	
}

void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)//移植的标准库串口1接收中断使能
{
  uint32_t usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  uint32_t usartxbase = 0x00;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  /* The CTS interrupt is not available for UART4 and UART5 */
  if (USART_IT == ((uint16_t)0x096A))
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  usartxbase = (uint32_t)USARTx;

  /* Get the USART register index */
  usartreg = (((uint8_t)USART_IT) >> 0x05);

  /* Get the interrupt position */
  itpos = USART_IT & ((uint16_t)0x001F);
  itmask = (((uint32_t)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register */
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register */
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register */
  {
    usartxbase += 0x14; 
  }
  if (NewState != DISABLE)
  {
    *(__IO uint32_t*)usartxbase  |= itmask;
  }
  else
  {
    *(__IO uint32_t*)usartxbase &= ~itmask;
  }
}

void Task1(void)  //任务1 （LED闪烁，示例代码）
{ 	
	while(1) 
	 {
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		 OS_delayMs(500);
		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		 OS_delayMs(500);	
     OSprintf("Task1 is running\r\n");		 
	 }	
}

void Task2(void) //任务2
{
	while(1) 
	 {
   	 OSprintf("Task2 is running\r\n");	//示例代码，使用时删除			 
  	 OS_delayDHMS(0,0,0,2);			//示例代码，使用时删除		 
	 }			
}

void Task3(void) //任务3
{	
	while(1) 
	 {
		 OS_delayMs(0);//为0时无限延时,不占cpu资源
	 }			
}

void Task4(void) //任务4
{
	while(1) 
	 {		
		 OS_delayMs(0);
	 }
}

void Task5(void) //任务5
{
	while(1) 
	 {	
	   OS_delayMs(0);
	 }
}

/****************************************************望穿秋水*************************************************************/

