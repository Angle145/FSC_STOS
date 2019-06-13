#include "os.h"

//TCB变量
OS_TCB OSTCBTbl[OS_MAX_TASKS]; 
OS_TCB *OSTCBCur; 
OS_TCB *OSTCBNext;

//系统变量
SYSTEM System;

//空闲任务栈(由数组实现)
OS_STK TASK_IDLE_STK[STK_IDLE_SIZE];

/*定时器初使化
*CTRL寄存器：bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位*/
void OSSysTickInit(void)//用作OS的计时脉冲
{ 
	//Systick定时器初使化(使用其他定时器时，请修改为其他定时器)
	char *Systick_priority = (char *)0xe000ed23;       //Systick中断优先级寄存器
	SysTick->LOAD  = (SystemCoreClock/8/1000000)* 1000; //Systick定时器重装载计数值:1ms(固定值)
	*Systick_priority = 0x00;                           //Systick定时器中断优先级
	SysTick->VAL   = 0;                                 //Systick定时器计数器清0
	SysTick->CTRL = 0x3;//Systick打开并使能中断且使用外部晶振时钟,8分频  72MHz/8=9MHz  计数9000次=1ms  计数9次=1us
}
void SysTick_Handler(void) //定时器中断函数(移植时需要注释掉原工程中的SysTick_Handler中断函数)
{
	INT8U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
	  if(OSTCBTbl[i].DelayTime>1) OSTCBTbl[i].DelayTime--;
	}
	 System.TimeMS++;  //系统时钟节拍累加
	 if((--System.TaskTimeSlice) == 0)  
		 {
			 System.TaskTimeSlice = TASK_TIME_SLICE;//重置时间片初值
			 OSSchedSwitch();	//调度并切换任务					
		 }  
}

/*PendSV初使化及触发中断函数(中断里有任务环境切换代码)
*移植时需要注释原工程中的PendSV_Handler中断函数*/
void OSPendSVInit(void)//PendSV初使化
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //优先级寄存器地址
	*NVIC_SYSPRI14=0xFF;//设置PendSV中断优先级最低 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //中断控制寄存器地址
void OSPendSVPulse(void)//触发PendSV中断
{
	OS_CORE_ENTER();
  *NVIC_INT_CTRL=0x10000000; 
	OS_CORE_EXIT();	
}

/*
*此调度器是最简单的调度器，实现了让任务从任务0-任务n循环运行。
*此方式较傻瓜，失去了OS有序高效组织任务特性，开发者重新设计。
 建议1：在TCB数据类型中添加任务延时累计变量，只要该变量值
			 不为0(被延时函数赋初值)，该变量在定时器中断中不断递减，同时检
			 测该计时是否到时(=0)，未到时则调度器暂停该任务的运行并忽略检
			 测该任务，把运行权交给延时短或没有延时的任务，如到时则通知调度
			 器立即切换到该任务运行。
 建议2：在TCB数据类型中添加任务优先级变量，调度器中优先把运行权交
       给最高优先级任务运行。
 建议3：调度器实现高优先级任务优先运行，相同优先级并同属最高优先级轮
       流运行。
 建议4：在TCB数据类型中添加任务时间片变量，调度器中检测任务运行频繁程
       度，作出智能时间片调整。
*调度器是OS的核心，不同的调度方式造就OS不同的特色。
*调度器要求：1.能快速找出下个要运行的任务(可能需要优秀算法支持);
            2.能稳定组织所有任务有序高效运行，尽量避免CPU出现
              空运转情况。
            3.能判断任务是否出现延时共振现象(任务同步延时)，智能错开任
              务延时，让cpu高效运行。
*控制任务运行办法：OSTCBNext指针指向哪个任务的TCB数组地址则在切换环境后
        就会运行哪个任务。
*/
void OSScheduler(void)  //任务调度器(OS核心)
{
/*-----------------------------------顺序运行模式---------------------------------------------*/
//	if(System.OSTaskNext<OS_MAX_TASKS-1) System.OSTaskNext++; //下一任务，直接轮到下个任务
//	else System.OSTaskNext = 0;//从空闲任务开始循环
//	while((OSTCBTbl[System.OSTaskNext].StkPtr == (OS_STK*)0)&&     //空任务检测
//		    (OSTCBTbl[System.OSTaskNext].State!=TASK_STATE_RUNNING)) //非运行状态检测
//	{ 
//		System.OSTaskNext++;//跳过任务
//		if(System.OSTaskNext >= OS_MAX_TASKS) //任务循环运行
//			{
//				System.OSTaskNext = 0;//从空闲任务开始循环
//			}
//	}
//  OSTCBNext = &OSTCBTbl[System.OSTaskNext];//更新OSTCBNext	
/*-----------------------------------------------------------------------------------------*/		
	
/*-----------------------------------优先级运行模式------------------------------------------*/	
	INT8U i,HighestPrio;
	for(HighestPrio=0,i=1;i<OS_MAX_TASKS;i++)//查找最高优先级任务
	{
		if((OSTCBTbl[i].StkPtr != (OS_STK*)0)&&     //非空任务检测
			 (OSTCBTbl[i].DelayTime==1)&&             //延时完成检测
			 (OSTCBTbl[i].State==TASK_STATE_RUNNING)) //运行状态检测
		{
			if(HighestPrio<OSTCBTbl[i].Prio)
			{
				HighestPrio=OSTCBTbl[i].Prio;
			}
		}
	}
	if(OSTCBTbl[HighestPrio].DelayTime==1) OSTCBTbl[HighestPrio].DelayTime=0;
	OSTCBNext = &OSTCBTbl[HighestPrio];//更新OSTCBNext	
/*-----------------------------------------------------------------------------------------*/	
}
void OSSchedSwitch(void)//任务调度并切换函数
{
	if(System.OSLockNesting == 0) //任务切换锁定检测，>0说明有锁，==0无锁
	{
		OSScheduler();   //先调度，OSTCBNext获取到将要运行的任务
		if(OSTCBNext!=OSTCBCur) OSPendSVPulse(); //任务环境切换，将OSTCBNext切换到OSTCBCur运行	
	}
}

//任务堆栈初使化
//只要产生PendSV中断,跳转中断处理函数前 xPSR,PC,LR,R12,R3-R0被自动保存到系统或任务栈中(此步聚是PendSV中断硬件机制)，
//保存在哪个栈取决于当前的SP类型，如是MSP则保存到系统栈，如是PSP则保存到任务栈。本OS是保存于任务栈。而R11-R4需要手动保存到任务栈中
//入栈顺序：栈顶->栈尾 xPSR,PC,LR,R12,R3-R0,R4-R11共16个(SP(R13)保存在TCB首个成员变量中)。
OS_STK* OSTaskStkInit(void* task,OS_STK *topstkptr)
{
    OS_STK *stk;
    stk = topstkptr;
    *(stk)    = (INT32U)0x01000000L;//xPSR 程序状态寄存器，xPSR T位(第24位)必须置1,否则第一次执行任务时进入Fault中断                                                     
    *(--stk)  = (INT32U)task;       //PC   初使化时指向任务函数首地址（任务切换时，可能指向任务函数中间某地址）            
    *(--stk)  = (INT32U)0xFFFFFFFEL;//LR   保存着各种跳转的返回连接地址,初使化为最低4位为E，是一个非法值，主要目的是不让使用R14，即任务是不能返回的             
    *(--stk)  = (INT32U)0x12121212L;//R12  通用寄存器(12121212为无效值，只起到标记为R12的作用，可修改为0,以下同)                                                       
    *(--stk)  = (INT32U)0x03030303L;//R3   通用寄存器                                                       
    *(--stk)  = (INT32U)0x02020202L;//R2   通用寄存器                                                        
    *(--stk)  = (INT32U)0x01010101L;//R1   通用寄存器                                                        
    *(--stk)  = (INT32U)0;          //R0   通用寄存器            
    *(--stk)  = (INT32U)0x04040404L;//R4   通用寄存器             
    *(--stk)  = (INT32U)0x05050505L;//R5   通用寄存器             
    *(--stk)  = (INT32U)0x06060606L;//R6   通用寄存器             
    *(--stk)  = (INT32U)0x07070707L;//R7   通用寄存器             
    *(--stk)  = (INT32U)0x08080808L;//R8   通用寄存器             
    *(--stk)  = (INT32U)0x09090909L;//R9   通用寄存器             
    *(--stk)  = (INT32U)0x10101010L;//R10  通用寄存器             
    *(--stk)  = (INT32U)0x11111111L;//R11  通用寄存器
    //以上R12-R11的过程可以简化为：  
    //stk-=13;	
    return stk;
}

void OSInit(void) //系统初使化
{
    INT8U i;
    for(i = 0; i < OS_MAX_TASKS; i++) {
        OSTCBTbl[i].StkPtr = (OS_STK*)0;
			  OSTCBTbl[i].DelayTime=1;//必须为1 (设计：为0时为不运行)
        OSTCBTbl[i].State = TASK_STATE_CREATING;
    }
		System.TimeMS=0;
		System.TaskTimeSlice = TASK_TIME_SLICE;
		System.OSRunning=OS_TRUE;
    OSInitTaskIdle();
    OSTCBCur = &OSTCBTbl[0];
    OSTCBNext = &OSTCBTbl[0];		
}
void OSTaskCreate(void* task,OS_STK *topstkptr,INT8U prio)//任务创建函数
{
    OS_STK * stkptr;
    INT8U i = 1;
    while(OSTCBTbl[i].StkPtr != (OS_STK*)0) { //查找空任务
        i++;
    }
    stkptr = OSTaskStkInit(task,topstkptr);
		OSTCBTbl[i].StkPtr=stkptr;
		OSTCBTbl[i].Prio=prio;
    OSTCBTbl[i].State = TASK_STATE_RUNNING;//创建时为运行态(也可为创建态TASK_STATE_CREATING,调度器检测到就绪条件时再转为运行态)
}
void OSInitTaskIdle(void)//空闲任务创建函数
{
    OSTCBTbl[0].StkPtr = OSTaskStkInit(OS_TaskIdle,(OS_STK*)&TASK_IDLE_STK[STK_IDLE_SIZE - 1]);
	  OSTCBTbl[0].Prio=0;
    OSTCBTbl[0].State = TASK_STATE_RUNNING;//必须是运行态TASK_STATE_RUNNING
}
void OS_TaskIdle(void) //空闲任务，用于防止用户任务都处于暂停或为数量为0时出错
{
	while(1) 
	{
		
	}
}

void OSSchedLock(void) //任务切换锁定  
{
	OS_CORE_ENTER();
	if(System.OSRunning == OS_TRUE)  
		{                                                                       
			if (System.OSLockNesting < 255u) System.OSLockNesting++;	     
	  }
	OS_CORE_EXIT();
}


void OSSchedUnlock(void) //任务切换解锁 
{
	OS_CORE_ENTER();
	if(System.OSRunning == OS_TRUE)
		{                               
			if (System.OSLockNesting > 0) System.OSLockNesting--;  
		}
	OS_CORE_EXIT();
} 


INT32U GetTime(void)
{
    return System.TimeMS;  //系统时钟节拍累计变量
}


//uS延时

void delay_us(INT32U us)
{       
    INT32U ticks;
    INT32U told,tnow,tcnt=0;
    INT32U reload=SysTick->LOAD;       
    ticks=us*(SystemCoreClock/8/1000000);      
    tcnt=0;
    told=SysTick->VAL;
    while(1)
    {
			tnow=SysTick->VAL;  
			if(tnow!=told)
			{       
				if(tnow<told) tcnt += told-tnow;
				else tcnt += reload-tnow+told;      
				told = tnow;
				if(tcnt>=ticks) break;
			} 					
    }
}
//mS延时
void delay_ms(INT32U ms)
{   
  delay_us(ms*1000);			
}

void OS_delayMs(INT32U ms)  //主动放弃CPU运行权,ms=0 永久放弃，ms>0放弃一段时间后恢复(延时)
{
	 if(ms>0) ms=ms+1;
   OSTCBCur->DelayTime=ms;
	 OSSchedSwitch();
	 while(OSTCBCur->DelayTime);
}
