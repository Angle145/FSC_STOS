/*FSC_STOS_V4.9*/
#include "fsc_stos.h"  //ʹ�ö������ں�


/* �����������⣬�˴�Ϊ����ֱ��ȡ��Task1-5                        */
/******************************�����������*************************/
void Task1(void); //����1����
void Task2(void); //����2����
void Task3(void); //����3����
void Task4(void); //����4����
void Task5(void); //����5����

//��̬������������Ҫ��ջ�ڴ�
//#define Task1_StkSize 128  //����1��ջ��С
//#define Task2_StkSize 128  //����2��ջ��С 
//#define Task3_StkSize 128  //����3��ջ��С 
//#define Task4_StkSize 128  //����4��ջ��С
//#define Task5_StkSize 128  //����5��ջ��С

//OSSTK Task1_Stk[Task1_StkSize]; //����1��ջ
//OSSTK Task2_Stk[Task2_StkSize]; //����2��ջ
//OSSTK Task3_Stk[Task3_StkSize]; //����3��ջ
//OSSTK Task4_Stk[Task4_StkSize]; //����4��ջ
//OSSTK Task5_Stk[Task5_StkSize]; //����5��ջ
//��̬������������Ҫ��ջ�ڴ�

void OS_MAIN(void)
{
	 /*-------------------ȫ�ֱ������궨����------------------------*/



   /*-------------------------------------------------------------*/
	 /************************************************************************************/	
	 OSInit(); //ϵͳ��ʹ��
	 /********************************��ϵͳ�д�������***********************************/
	 //�������ȼ�(Խ��Խ�ߣ���Χ��0-OS_MAX_TASKS-1)
	
	 //��̬����(��Ҫ�û��ṩ�ڴ�ռ�)
	 /*                  ������  ��������   ջ��ַ     ջ��С      ʱ��Ƭ  ���ȼ�     ����״̬*/
//	 OSTaskCreateStatic("Task1", Task1,  Task1_Stk, Task1_StkSize, 10,     1,     TASK_RUNNING); //OS��������1
//	 OSTaskCreateStatic("Task2", Task2,  Task2_Stk, Task2_StkSize, 10,     2,     TASK_RUNNING); //OS��������2
//	 OSTaskCreateStatic("Task3", Task3,  Task3_Stk, Task3_StkSize, 10,     5,     TASK_RUNNING); //OS��������3
//	 OSTaskCreateStatic("Task4", Task4,  Task4_Stk, Task4_StkSize, 10,     4,     TASK_RUNNING); //OS��������4
//	 OSTaskCreateStatic("Task5", Task5,  Task5_Stk, Task5_StkSize, 10,     5,     TASK_RUNNING); //OS��������5
	
	 
	 //��̬����(ϵͳ�Զ�����ջ�������ɿռ�,ע�����ú��ڴ�صĴ�С)
	 /*            ������  ��������  ջ��С  ʱ��Ƭ  �������ȼ�   ����״̬*/
	 OSTaskCreate("Task1",  Task1,    128,     10,       4,    TASK_RUNNING); //OS��������1
	 OSTaskCreate("Task2",  Task2,    128,     10,       6,    TASK_RUNNING); //OS��������2
	 OSTaskCreate("Task3",  Task3,    128,     10,       5,    TASK_RUNNING); //OS��������3
	 OSTaskCreate("Task4",  Task4,    128,     10,       5,    TASK_RUNNING); //OS��������4
	 OSTaskCreate("Task5",  Task5,    128,     10,       3,    TASK_RUNNING); //OS��������5
	 /***********************************************************************************/
	 OSStartUp();//OS��ʼ����
}


void Task1(void)  //����1
{ 	
	while(1) 
	 {
		 
		 OS_delayMs(1000);//������ʱ����,��λ��ms
	 }	
}

void Task2(void) //����2
{
	while(1) 
	 {
		 
		 OS_delayDHMS(0,0,0,1);//OS_delayMs()�ı��κ���������ʱ����Ϊ�������
	 }			
}

void Task3(void) //����3
{	
	while(1) 
	 {
	 
		 OS_delayMs(0);//=0Ϊ������ʱ����ռcpu��Դ
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
/********************************************************************************************************************/

