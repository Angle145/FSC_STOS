#include "fsc_stos.h"  
#include "stm32f4xx.h"
#include "led.h"  
#include "usart.h"

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
/*******************************************************************/
void OS_MAIN(void)
{
	  /*------------------------------ȫ�ֳ�ʹ����-----------------------------*/
	 /*�Ƽ�����������ʹ�õ��ĳ�ʹ�����ڴ˴���Task�����õ��ĳ�ʹ������Task��*/
    
	  Usart_Config(); //���ڳ�ʼ��:9600
	
	
	 /*-----------------------------------------------------------------------*/	
	  /************************************************************************************/	
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

/****************************************�û�ȫ�ֱ������궨����*****************************************/



/*******************************************************************************************************/
/*********************************�û�����ʵ�������************************************/
void Task1(void) //����1
{
	LED_Init();//����F429���İ�LED
	while(1) 
	 {
		 	LED1_ON;	OS_delayMs(500);  //����LED1  //ʾ�����룬ʹ��ʱɾ��		
		  LED1_OFF; OS_delayMs(500);  //�ر�LED1
		  OSprintf("Task1 is runng \r\n");	//ʾ�����룬ʹ��ʱɾ��			 
	 }			
}
void Task2(void)  //����2
{ 	
	u16 a = 128;   //���Ա���
	float b = 9.123456; //���Ա���
	
	OSprintf("   ����������  \r\n\r\n");//ʾ�����룬ʹ��ʱɾ��		
	while (1)
	{		
		OSprintf("ʮ���Ƹ�ʽ��  %d\r\n",a); //ʾ�����룬ʹ��ʱɾ��		
		OSprintf("ʮ�����Ƹ�ʽ��0x%x\r\n",a);
		OSprintf("С����ʽ��    %f\r\n",b);	
    OSprintf("Task2 is runng \r\n");	//ʾ�����룬ʹ��ʱɾ��		
		OS_delayMs(1000);
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

