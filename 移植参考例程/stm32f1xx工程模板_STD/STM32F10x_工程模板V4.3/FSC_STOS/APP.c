#include "fsc_stos.h"  //ʹ�ö������ں�
#include "STM32F10xPeripMode.h" //ʹ��STM32F1�����ط�װ��ģ��(�жϣ���ʱ��������,ADC��)
#include "STM32F10x_C51Mode.h"  //ʹ��C51��ʽIO����(λ������)
//#include "ds18b20.h"      
//#include "ds3231.h"
//#include "eeprom_24xx.h"

/* �����������⣬�˴�Ϊ����ֱ��ȡ��ΪTask1-5 */
/******************************�����������*************************/
void Task1(void); //����1����
void Task2(void); //����2����
void Task3(void); //����3����
void Task4(void); //����4����
void Task5(void); //����5����

#define Task1_StkSize 128  //����1��ջ��С
#define Task2_StkSize 128  //����2��ջ��С 
#define Task3_StkSize 128  //����3��ջ��С 
#define Task4_StkSize 128  //����4��ջ��С
#define Task5_StkSize 128  //����5��ջ��С

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
	 
	 SWON_JTAGOFF();  //��ʹ��SW���ܡ�ʾ������,ʹ����������ģʽʱɾ��
	 USART1_Config(9600); //����1��ʹ����ʾ������,��ʹ�ô���1ʱɾ��
	

	 /*-----------------------------------------------------------------------*/
	 OSInit(); //ϵͳ��ʹ��
	 /********************************��ϵͳ�д�������***********************************/
	 /*           ������ �������� ����ջ     ����ջ��С   ����״̬  �������ȼ�(Խ��Խ�ߣ�0-65535,=0ʱ������)*/
	 OSTaskCreate("Task1",Task1,Task1_Stk,Task1_StkSize,TASK_RUNNING,1); //OS��������1
	 OSTaskCreate("Task2",Task2,Task2_Stk,Task2_StkSize,TASK_RUNNING,2); //OS��������2
	 OSTaskCreate("Task3",Task3,Task3_Stk,Task3_StkSize,TASK_RUNNING,5); //OS��������3
	 OSTaskCreate("Task4",Task4,Task4_Stk,Task4_StkSize,TASK_RUNNING,4); //OS��������4
	 OSTaskCreate("Task5",Task5,Task5_Stk,Task5_StkSize,TASK_RUNNING,5); //OS��������5
	 /***********************************************************************************/
	 OSStartUp();//ϵͳ��ʼ����
}


/************************************************�û������������*************************************************/
/*----------------------ȫ�ֱ������궨����--------------------------*/



/*------------------------------------------------------------------*/


void Task1(void) //����1
{
	PC13_OUT;          //stm32f103c8t6��Сϵͳ�����LED  IO�ڳ�ʹ�����������ģʽ ��ʾ�����룬ʹ��ʱɾ��	
	while(1) 
	 {		
			PC13=~PC13;     //LED��˸��ʾ�����룬ʹ��ʱɾ��
			OSprintf("Task1 is running\r\n");//ʾ�����룬ʹ��ʱɾ��			 
			OS_delayMs(500);//��˸Ƶ��:1S��ʾ�����룬ʹ��ʱɾ��	
	 }
}
void Task2(void) //����2
{
	while(1) 
	 {
			OSprintf("Task2 is running\r\n");//ʾ�����룬ʹ��ʱɾ��			 
			OS_delayDHMS(0,0,0,1); 			//ʾ�����룬ʹ��ʱɾ��		
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
