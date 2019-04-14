#include "fsc_stos.h"  //使用多任务内核
#include "led.h"
#include "USART1.h"

/******************************创建任务参数*************************/
void Task1(void); //任务1声明
void Task2(void); //任务2声明
void Task3(void); //任务3声明
void Task4(void); //任务4声明
void Task5(void); //任务5声明

#define Task1_StkSize 64       //任务1堆栈大小
#define Task2_StkSize 64       //任务2堆栈大小 
#define Task3_StkSize 64       //任务3堆栈大小 
#define Task4_StkSize 32       //任务4堆栈大小
#define Task5_StkSize 32       //任务5堆栈大小

OSSTK Task1_Stk[Task1_StkSize]; //任务1堆栈
OSSTK Task2_Stk[Task2_StkSize]; //任务2堆栈
OSSTK Task3_Stk[Task3_StkSize]; //任务3堆栈
OSSTK Task4_Stk[Task4_StkSize]; //任务4堆栈
OSSTK Task5_Stk[Task5_StkSize]; //任务5堆栈

void OS_MAIN(void)
{
	 /*------------------------全局初使化区--------------------------*/
    USART1_Init(9600);


   /*-------------------------------------------------------------*/
	 /************************************************************************************/	
	 OSInit(); //系统初使化
	 /********************************在系统中创建任务***********************************/
	 OSTaskCreate("Task1",Task1,Task1_Stk,Task1_StkSize,TASK_RUNNING,1); //OS创建任务1
	 OSTaskCreate("Task2",Task2,Task2_Stk,Task2_StkSize,TASK_RUNNING,2); //OS创建任务2
	 OSTaskCreate("Task3",Task3,Task3_Stk,Task3_StkSize,TASK_RUNNING,3); //OS创建任务3
	 OSTaskCreate("Task4",Task4,Task4_Stk,Task4_StkSize,TASK_RUNNING,4); //OS创建任务4
	 OSTaskCreate("Task5",Task5,Task5_Stk,Task5_StkSize,TASK_RUNNING,5); //OS创建任务5
	 /***********************************************************************************/
	 OSStartUp();//OS开始运行
}
/********************************************************************************************************************/
/*-------------------全局变量及宏定义区------------------------*/


/*-----------------------------------------------------------*/

void Task1(void)  //任务1
{ 	
  LED_Init();
  while (1)
  {	
		LED_ON();
		OS_delayMs(500);
		LED_OFF();
		OS_delayMs(500);
		OSprintf("Task1 is running  \r\n");
  } 	
}

void Task2(void) //任务2
{
	while(1) 
	 {
		 OSprintf("Task2 is running  \r\n");
		 OS_delayMs(2000);
	 }			
}

void Task3(void) //任务3
{	
	while(1) 
	 {
	 
		 OS_delayMs(0);
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
/********************************************************************************************************************/

