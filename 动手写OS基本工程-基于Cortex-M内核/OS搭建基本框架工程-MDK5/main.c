#include "os.h"  //使用内核
#include "usart.h" 

/****************************用户任务函数声明*****************************/
void Task1(void);
void Task2(void);
void Task3(void);
/*******************************************************************/
/*****************************用户任务堆栈创建**************************/
//任务堆栈(一维数组)全局变量
OS_STK Task1Stk[128];
OS_STK Task2Stk[64];
OS_STK Task3Stk[64];
/*******************************************************************/
/******************************用户任务加入内核运行*********************************/
int main(void)
{   
	  USART1_Config(9600); //串口1初使化
	  printf("-@OS Inside\r\n");
    OSInit();
    OSTaskCreate(Task1,&Task1Stk[128-1]); //创建任务1 //必须-1,数组标号最大为N-1
    OSTaskCreate(Task2,&Task2Stk[64-1]); //创建任务2
    OSTaskCreate(Task3,&Task3Stk[64-1]); //创建任务3
    OSStartUp();
}
/***********************************************************************************/
void Task1(void)  //任务1
{   
	while(1) 
	 {	
		 OSprintf("Task1 is running\r\n");		  
		 OS_delayMs(1000);  	
	 }
}
void Task2(void)  //任务2
{
	while(1) 
	{
		OSprintf("Task2 is running\r\n");
		OS_delayMs(2000);		
	}
}
		
void Task3(void) //任务3
{
	while(1) 
	 {
		 OSprintf("Task3 is running\r\n");						
		 OS_delayMs(4000);							
	 }			
}

/*
*望穿秋水提示：
*想要更多任务的自己手动添加,添加任务时，任务堆栈、任务函数声明、任务创建参数名、任务函数名四者均要添加且名称要统一。
*把void Taskx(void)看成普通的int main(void)使用即可,while(1){}必须存在;
*任务名可以随意修改，Task1是默认的任务名，修改任务名时，任务堆栈、任务函数声明、任务创建参数名、任务函数名四者名称要统一。
*这里默认创建了3个用户任务，用户可自行添加或删除(最大255个任务(含空闲任务))
*/
