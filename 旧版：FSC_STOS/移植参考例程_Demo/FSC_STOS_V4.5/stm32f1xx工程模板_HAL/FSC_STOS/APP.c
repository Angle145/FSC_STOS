#include "fsc_stos.h"  //ʹ�ö������ں�

void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);

/* �����������⣬�˴�Ϊ����ֱ��ȡ��ΪTask1-5 */
/******************************�����������*************************/
void Task1(void); //����1����
void Task2(void); //����2����
void Task3(void); //����3����
void Task4(void); //����4����
void Task5(void); //����5����

#define Task1_StkSize 128       //����1��ջ��С
#define Task2_StkSize 128       //����2��ջ��С 
#define Task3_StkSize 128       //����3��ջ��С 
#define Task4_StkSize 128       //����4��ջ��С
#define Task5_StkSize 128       //����5��ջ��С

OSSTK Task1_Stk[Task1_StkSize]; //����1��ջ
OSSTK Task2_Stk[Task2_StkSize]; //����2��ջ
OSSTK Task3_Stk[Task3_StkSize]; //����3��ջ
OSSTK Task4_Stk[Task4_StkSize]; //����4��ջ
OSSTK Task5_Stk[Task5_StkSize]; //����5��ջ

void OS_MAIN(void)
{
	 /*-----------------------------�û�ȫ�ֳ�ʹ����---------------------------*/
	 /*�Ƽ�����������ʹ�õ��ĳ�ʹ�����ڴ˴���Task�����õ��ĳ�ʹ������Task��*/
	 
	 USART_ITConfig(USART1, ((uint16_t)0x0525), ENABLE);//��ֲ�ı�׼�⴮��1�����ж�ʹ��(HAL�⴮�ڽ��ձȽ�����)
	
	
	 /*-----------------------------------------------------------------------*/
	 /***********************************************************************************/	
	 OSInit(); //ϵͳ��ʹ��
	 /********************************��ϵͳ�д�������***********************************/
	 /*           ������ �������� ����ջ     ����ջ��С   ����״̬  �������ȼ�(Խ��Խ�ߣ�0-65535,=0ʱ������)*/
	 OSTaskCreate("Task1",Task1,Task1_Stk,Task1_StkSize,TASK_RUNNING,1); //OS��������1
	 OSTaskCreate("Task2",Task2,Task2_Stk,Task2_StkSize,TASK_RUNNING,2); //OS��������2
	 OSTaskCreate("Task3",Task3,Task3_Stk,Task3_StkSize,TASK_RUNNING,3); //OS��������3
	 OSTaskCreate("Task4",Task4,Task4_Stk,Task4_StkSize,TASK_RUNNING,4); //OS��������4
	 OSTaskCreate("Task5",Task5,Task5_Stk,Task5_StkSize,TASK_RUNNING,5); //OS��������5
	 /***********************************************************************************/
	 OSStartUp();//OS��ʼ����
}
/*******************************************************************/

/***********************************�û������������**************************************/

extern UART_HandleTypeDef huart1;
int fputc(int ch,FILE *f) //printf�����ض���
{
  uint8_t temp[1]={ch};
	HAL_UART_Transmit(&huart1,temp,1,2);//ʹ�ô���1
	return ch;	
}

void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)//��ֲ�ı�׼�⴮��1�����ж�ʹ��
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

void Task1(void)  //����1 ��LED��˸��ʾ�����룩
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

void Task2(void) //����2
{
	while(1) 
	 {
   	 OSprintf("Task2 is running\r\n");	//ʾ�����룬ʹ��ʱɾ��			 
  	 OS_delayDHMS(0,0,0,2);			//ʾ�����룬ʹ��ʱɾ��		 
	 }			
}

void Task3(void) //����3
{	
	while(1) 
	 {
		 OS_delayMs(0);//Ϊ0ʱ������ʱ,��ռcpu��Դ
	 }			
}

void Task4(void) //����4
{
	while(1) 
	 {		
		 OS_delayMs(0);
	 }
}

void Task5(void) //����5
{
	while(1) 
	 {	
	   OS_delayMs(0);
	 }
}

/****************************************************������ˮ*************************************************************/

