#ifndef _OS_H_
#define _OS_H_
#include "stm32f10x.h"
#include <stdio.h>
typedef unsigned char  INT8U;            
typedef unsigned short INT16U;           
typedef unsigned int   INT32U;           
typedef unsigned int   OS_STK;            

/*******************************OS参数定义********************************************/
#define OS_MAX_TASKS        16         //最大任务数(1-255)
#define TASK_TIME_SLICE     10         //任务时间切片,单位：1ms   
#define STK_IDLE_SIZE       18         //空闲任务栈长度
/**************************************************************************************/

//重封装printf函数
#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS提供的打印输出函数

//任务状态
#define TASK_STATE_CREATING   0
#define TASK_STATE_RUNNING    1
#define TASK_STATE_PAUSING    2

//系统布尔值
#define OS_FALSE 0 
#define OS_TRUE  1 

//任务控制块类型定义(深入学习后可修改为双向链表)
typedef struct 
{
	OS_STK    *StkPtr;  //任务栈指针 (必须是第一个)  
	INT8U     State;    //任务状态
  //INT32U  DelayTime;//任务延时时间	
	//INT8U   Prio;     //任务优先级
	//INT32U  TimeSlice;//任务时间片
	//struct OS_TCB *last;//上个任务
	//struct OS_TCB *next;//下个任务
} OS_TCB; 

extern OS_TCB OSTCBTbl[OS_MAX_TASKS]; //各个任务的TCB，数组方式(标号0-空闲任务，标号1-Task1--标号n-Taskn)
extern OS_TCB *OSTCBCur;  //当前正在运行的任务TCB
extern OS_TCB *OSTCBNext; //下个即将运行的任务TCB

//系统变量类型定义
typedef struct 
{
	INT8U OSRunning;    //运行标志变量
	INT8U OSLockNesting;//任务切换锁定层数统计变量 
	INT8U  OSTaskNext;  //下个运行任务的TCB组数标号
	INT32U TimeMS;      //系统时钟节拍累计变量
	INT32U TaskTimeSlice;//任务时间切片(为系统时钟节拍倍数,任务持续运行的时间)
}SYSTEM;
extern SYSTEM System;//系统变量

void OS_CORE_ENTER(void);                            //关闭全局中断函数(在汇编中)            
void OS_CORE_EXIT(void);                             //打开全局中断函数(在汇编中) 
void OSInit(void);                                   //系统初使化函数
void OSStartUp(void);                                //系统启动函数(在汇编中) 
void OSScheduler(void);                              //任务调度器
void OSPendSVPulse(void);                            //任务环境切换函数 
void OSSchedSwitch(void);                            //任务调度并切换函数        
OS_STK* OSTaskStkInit(void* task,OS_STK *topstkptr); //任务堆栈初使化函数
void OSTaskCreate(void* task,OS_STK *topstkptr);     //任务创建函数
void OSInitTaskIdle(void);                           //空闲任务创建函数
void OS_TaskIdle(void);                              //空闲任务函数体
void OSScheduler(void);                              //任务调度器
void OSPendSVPulse(void);                            //任务环境切换函数
void OSSchedSwitch(void);                            //任务调度并切换函数                            
void OSSchedLock (void);                             //任务切换锁定函数                         
void OSSchedUnlock (void);                           //任务切换解锁函数
INT32U GetTime(void);                                //获取系统时钟节拍数函数
void delay_us(INT32U us);                            //微秒延时函数
void delay_ms(INT32U ms);	                           //毫秒延时函数
void OS_delayMs(INT32U ms);                          //任务延时函数
#endif
