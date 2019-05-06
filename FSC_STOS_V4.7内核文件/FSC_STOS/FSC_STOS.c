/*--------------------------------------------版本: V4.7--------------------------------------------*/
#include "fsc_stos.h" 

/**********************************************可修改部分*********************************************/
/*---------------其他文件变量或函数声明区--------------*/



#if (OS_SYSTIME_ENABLE== 1) //时间设置函数声明
extern u8 RTC_SetTimes(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//设置时间函数年月日时分秒(从外部引进)
#endif
/*--------------------------------------------------*/
/*---------------------------------自定义系统指令-----------------------------------*/
//如任务超过5个，请自行增加指令,并到OS_TaskManage()任务管理器任务中增加相应的代码(在本页约481行)。
#if (OS_CMD_ALL_ENABLE == 1)
#if (OS_CMD_NSY_ENABLE == 1)
/*---------------任务开关----------------*/
char cmd_task_open_01[]={"cmd/Task1/open//"};//任务1打开  只要保证指令以//结尾即可，如{"打开任务1//"}
char cmd_task_open_02[]={"cmd/Task2/open//"};//任务2打开 
char cmd_task_open_03[]={"cmd/Task3/open//"};//任务3打开 
char cmd_task_open_04[]={"cmd/Task4/open//"};//任务4打开 
char cmd_task_open_05[]={"cmd/Task5/open//"};//任务5打开 

char cmd_task_close_01[]={"cmd/Task1/close//"};//任务1关闭
char cmd_task_close_02[]={"cmd/Task2/close//"};//任务2关闭
char cmd_task_close_03[]={"cmd/Task3/close//"};//任务3关闭
char cmd_task_close_04[]={"cmd/Task4/close//"};//任务4关闭
char cmd_task_close_05[]={"cmd/Task5/close//"};//任务5关闭
/*-------------------------------------------*/
/*-----------------任务优先级-----------------*/
char cmd_task_prio_01[]={"cmd/Task1/prio=%d//"};//%d表示整型数字,例：cmd/Task1/prio=8//
char cmd_task_prio_02[]={"cmd/Task2/prio=%d//"};
char cmd_task_prio_03[]={"cmd/Task3/prio=%d//"};
char cmd_task_prio_04[]={"cmd/Task4/prio=%d//"};
char cmd_task_prio_05[]={"cmd/Task5/prio=%d//"};
/*-------------------------------------------*/
/*---------------任务时间切片-----------------*/
char cmd_task_timeslice_01[]={"cmd/Task1/timeslice=%d//"}; //设置任务1时间切片时间(一个任务连续运行的时间)
char cmd_task_timeslice_02[]={"cmd/Task2/timeslice=%d//"}; //设置任务2时间切片时间
char cmd_task_timeslice_03[]={"cmd/Task3/timeslice=%d//"}; //设置任务3时间切片时间
char cmd_task_timeslice_04[]={"cmd/Task4/timeslice=%d//"}; //设置任务4时间切片时间
char cmd_task_timeslice_05[]={"cmd/Task5/timeslice=%d//"}; //设置任务5时间切片时间
/*-------------------------------------------*/
/*----------------设置系统时间----------------*/
char cmd_setsystime[]={"cmd/setsystime=%d/%d/%d/%d.%d.%d//"};//年/月/日/时.分.秒
/*------------------------------------------*/
#endif
/*------------------系统有关-----------------*/
char cmd_osmanage[]={"cmd/osmanage//"};          //查看系统状态
char cmd_runmode_order[]={"cmd/runmode/order//"};//顺序运行模式
char cmd_runmode_prio[]={"cmd/runmode/prio//"};  //优先级运行模式
/*------------------------------------------*/
/*-----------------自定义指令区--------------*/


/*------------------------------------------*/
/*----------------------------------------------------------------------------------*/
#endif
/*---------------------------------------系统定时器----------------------------------*/
//默认使用SysTick定时器   bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位
#ifndef SystemCoreClock
#define SystemCoreClock 72000000  //如为其他主频，请设置好对应的主频
#endif
#define OSTIMER_OFF()      SysTick->CTRL&=~(1<<0)     //关闭系统定时器中断
#define OSTIMER_ON()       SysTick->CTRL|=(1<<0)      //打开系统定时器中断
#define OSTIMER_INT_OFF()  SysTick->CTRL&=~(1<<1)     //关闭系统定时器中断
#define OSTIMER_INT_ON()   SysTick->CTRL|=(1<<1)      //打开系统定时器中断
#define OSTIMER_CONT_1us   (SystemCoreClock/8/1000000)//系统定时器1us计数次数 
#define OSTIMER_LOAD       SysTick->LOAD              //系统定时器重装载值寄存器
#define OSTIMER_VAL        SysTick->VAL               //系统定时器当前值寄存器
void OSTimerInit(void)//用作OS的计时脉冲
{ 
	OS_INT_ENTER();
	
	//Systick定时器初使化(使用其他定时器时，请修改为其他定时器)
	char * Systick_priority = (char *)0xe000ed23;     //Systick中断优先级寄存器
	SysTick->LOAD  = OSTIMER_CONT_1us* 1000;           //Systick定时器重装载计数值:1ms(固定值)
	*Systick_priority = 0x00;                         //Systick定时器中断优先级
	SysTick->VAL   = 0;                               //Systick定时器计数器清0
	SysTick->CTRL = 0x3;//Systick打开并使能中断且使用外部晶振时钟,8分频  72MHz/8=9MHz  计数9000次=1ms  计数9次=1us
	
	OS_INT_EXIT();
}
void SysTick_Handler(void) //Systick定时器中断函数(使用其他定时器时,请修改为其他定时器的中断函数名)
{
  OS_Timer_Handler();
}
/*----------------------------------------------------------------------------------*/
/*--------------------------------------PendSV初使化--------------------------------*/
void OSPendSVInit(void)//PendSV初使化
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //优先级寄存器地址
	*NVIC_SYSPRI14=0xFF;//设置PendSV中断优先级最低 
}
void OSPendSVPulse(void)//触发PendSV中断
{
	int* NVIC_INT_CTRL= (int *)0xE000ED04;  //中断控制寄存器地址
  *NVIC_INT_CTRL=0x10000000;  
}
/*---------------------------------------------------------------------------------*/
/*--------------------------------------任务切换函数--------------------------------*/
void OSSupendCtxSw(void) //触发任务切换函数
{
	OS_INT_ENTER();
	OS_System.TaskCtxSw=OS_TRUE;
	OSPendSVPulse();
	OS_INT_EXIT();
  while(OSTimerValGet());
}
void OSForceSupendCtxSw(void) //强制触发任务切换函数(无视其他任务的任务切换锁定,可能产生不良后果)
{
  OS_INT_ENTER();
	OS_System.TaskCtxSw=OS_TRUE;
	OS_System.LockNesting=0;
	OSPendSVPulse();
	OS_INT_EXIT();
  while(OSTimerValGet());
}
/*---------------------------------------------------------------------------------*/
/*--------------------------------------系统使能函数--------------------------------*/
void OSStopRun(void)//系统停止运行(进入裸机模式，当前任务函数等同于main函数)
{
	OS_INT_ENTER();
  OS_System.Running=OS_FALSE;
	OS_INT_EXIT();
}
void OSStartRun(void)//系统开始运行(进入OS模式)
{
	OS_INT_ENTER();
	OS_System.Running=OS_TRUE;
  OSTIMER_ON();
	OS_INT_EXIT();
}
/*---------------------------------------------------------------------------------*/
/*--------------------------------------系统使能函数--------------------------------*/
/*******************************************************
*函数名  ：OSEnterDriver  、    OSExitDriver
*使用方法：在驱动函数开头调用 、  在驱动函数结尾调用
*作用    :屏蔽OS对驱动的影响
*注意    :驱动程序里只能使用delay_us()来延时，
         除此之外不能使用任何与OS有关的其他资源
*******************************************************/
void OSEnterDriver(void)//进入驱动程序 调用
{
	OS_INT_ENTER();
	OS_System.RunDriverCode=OS_TRUE;
  OSTIMER_INT_OFF();
	OS_INT_EXIT();
}
void OSExitDriver(void)//离开驱动程序 调用
{
	OS_INT_ENTER();
	OS_System.RunDriverCode=OS_FALSE;
  OSTIMER_INT_ON();
	OS_INT_EXIT();
}
/*---------------------------------------------------------------------------------*/

/*-------------------------------------FSC智能纠错系统函数（Page Up）------------------------------------------*/
#if (OS_CMD_ALL_ENABLE == 1)
void OSManage(void);//函数声明
#endif
#if (OS_FAULT_ENABLE == 1)
void OSCtxSwTo(INT16U tasknum)
{
	OS_INT_ENTER();
	OSTCBCur=&OSTCBTbl[tasknum];
	OSPendSVPulse();
	OS_INT_EXIT();
}
void OSTaskEndlessLoopsCheck(void) //任务陷入死循环检测,任务连续不间断运行N次则认为陷入死循环
{
  if(OSTCBCur->TaskNum>1)	//从用户任务开始检测
  {
	  if(OS_Fault.runLastTask==OSTCBCur->TaskNum)	
	  {
			OS_System.TaskContinuRunTimeCnt++;		
			if(OS_System.TaskContinuRunTimeCnt>OS_EndlessLoops_Times) 
			{
				OSTCBCur->TaskState=TASK_PAUSING;
				OS_System.LockNesting=0;			 
#if (OS_FAULT_DISP_ENABLE == 1)					 
				OSprintf("FSC提示：任务%s卡死，暂停运行！\r\n",OSTCBCur->TaskStr); 
#endif		 
			}
	  }
	  else
	  {				
	 	  OS_System.TaskContinuRunTimeCnt=0;					 
	 	  OS_Fault.runLastTask=OSTCBCur->TaskNum;
	  }
  }
}
void OS_RESET_DELAY(INT32U div,INT32U time)
{
	OS_System.Running=OS_FALSE;
	OS_Fault.osresetimecnt=time*div;
	while(OS_System.Running==OS_FALSE)	
	 {
		 if(OS_Fault.osresetimecnt%div==0) 
		 {
			 OSprintf("系统重启倒计时：   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------系统启动------------\r\n");
		 }
	 }
	__disable_fault_irq();
  NVIC_SystemReset();
}
void OS_SHUTDOWN_DELAY(INT32U div,INT32U time)
{
	OS_System.Running=OS_FALSE;
	OS_Fault.osresetimecnt=time*div;
	while(OS_System.Running==OS_FALSE)	
	 {
		 if(OS_Fault.osresetimecnt%div==0) 
		 {
			 OSprintf("系统关闭倒计时：   %dS\r\n",OS_Fault.osresetimecnt/div);
		 }
	   if(OS_Fault.osresetimecnt>0) OS_Fault.osresetimecnt--;
		 else 
		 {
		   OS_System.Running=OS_TRUE;
			 OSprintf("------------系统关闭------------\r\n");
		 }
	 }
   while(1);
}
void HardFault_Handler(void)
{
	OSprintf("FSC提示：系统奔溃,错误信息及系统状态：\r\n");
	OSprintf("\r\nHardFault_Handler:\r\n");
	OSprintf("{\r\n");
  OSprintf(" %s error! \r\n",OS_Fault.tcb->TaskStr);
	OSprintf(" %s running last! \r\n",OS_Fault.tcblast->TaskStr);
	OSprintf(" %s running next! \r\n",OS_Fault.tcbnext->TaskStr);
	OSprintf("}\r\n");
	OSManage();
	OSprintf("\r\n\r\n");
#if (OSFAULT_Reset_Enable	== 1)
	OSprintf("-----------系统将在%d秒后重新启动----------- \r\n\r\n",OS_Reset_Delay_Time);
  OS_RESET_DELAY(1000000,OS_Reset_Delay_Time);
#else
	OSprintf("-----------系统将在%d秒后关闭----------- \r\n\r\n",OS_Reset_Delay_Time);
	OS_SHUTDOWN_DELAY(1000000,OS_Reset_Delay_Time);
#endif
}

INT8U OS_FAULT_STK_CHECK(OS_TCB* tcb)//堆栈溢出检测
{
	if(tcb->StkPtr!=(STK32U*)0)
	{
		if(tcb->StkAdd[0]!=0)
		{
			tcb->TaskState=TASK_PAUSING;
#if (OS_FAULT_DISP_ENABLE	==1)		
			OSprintf("FSC提示：任务%s堆栈溢出，暂停运行！\r\n\r\n",tcb->TaskStr);
#endif			
			return OS_TRUE ;
		}
		else return OS_FALSE ;
	}
	else return OS_FALSE ;
}
void FSC_FAULT_SYSTEM(void)
{
	INT16U TaskCount;
	for(TaskCount=0;TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		OS_FAULT_STK_CHECK(&OSTCBTbl[TaskCount]);
	}
}	
void OSTaskPrioBuckup(OS_TCB* tcb) //任务优先级备份
{
  tcb->TaskPrioBackup=tcb->TaskPrio;
}
void OSTaskPrioRecover(OS_TCB* tcb)  //任务优先级从备份中恢复
{
  tcb->TaskPrio=tcb->TaskPrioBackup;
}
void FSC_FAULT_SYSTEM_OS_Timer_Handler(INT16U tasknum)
{
	INT16U result;
	if(((tasknum>=2)&&(OSTCBTbl[tasknum].TaskFlagBit&(1<<_BIT_TimeLess))==0)&&\
     (OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&(OSTCBTbl[tasknum].TaskState==TASK_RUNNING))//从任务2开始检测
	{		
		result= (OSTCBTbl[tasknum].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt)*10;//取一位小数
		if(result>900) //系统占用率(实测：当有任务卡死时占用率最高为98.1%)
		 {	
			 if(result<=980) 
			 { 
#if (OS_FAULT_DISP_ENABLE == 1)	
					OSprintf("FSC提示：任务%s CPU占用率过高，占用率:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif					 
			 }
			 else  
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;	 	 
				 if(OS_System.RuningMode==0)
				 {
					 /*----------------有待开发-------------------*/				 
				 }	
				 else if(OS_System.RuningMode==1)		
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==0)
					 {
			       /*----------------有待开发-------------------*/  
					 }
					 else
					 {
						 OSTCBTbl[tasknum].TaskPrio--;//逐级提高优先级
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC提示：任务%s卡死，逐级降低优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif						 
					 }
				 }					 
			 }		 
		 }
		 else if(result<1) //系统占用率<0.1%
		 {		
			 if(result>0)
			 {
#if (OS_FAULT_DISP_ENABLE == 1)				 
				 OSprintf("FSC提示：任务%s CPU占用率过低，占用率:%0.1f%%\r\n",OSTCBTbl[tasknum].TaskStr,result/10.0);
#endif			 
			 }
			 else
			 {
				 OS_Fault.task[tasknum]=OS_TRUE;			 
				 if(OS_System.RuningMode==0)
				 {
           /*----------------有待开发-------------------*/
				 }
				 else if(OS_System.RuningMode==1)
				 {
					 if(OSTCBTbl[tasknum].TaskPrio==OSRunStateHighestPrioTaskNumGet())
					 {
					   /*----------------有待开发-------------------*/
					 }
					 else
					 {
						 if(OSTCBTbl[tasknum].TaskPrio<OS_MAX_TASKS-1) OSTCBTbl[tasknum].TaskPrio++;//逐级提高优先级
#if (OS_FAULT_DISP_ENABLE == 1)					 
						 OSprintf("FSC提示：任务%s未运行，逐级增大优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif			 
					 }
				 }
			 }
		 }
		 else //正常范围
		 {
			 if(OS_Fault.task[tasknum]==OS_TRUE)
			 {
					OS_Fault.task[tasknum]=OS_FALSE;
				  OSTaskPrioRecover(&OSTCBTbl[tasknum]);
					OSTCBTbl[tasknum].TaskState=TASK_RUNNING;
#if (OS_FAULT_DISP_ENABLE == 1)	 
				 OSprintf("FSC提示：任务%s正常运行，已恢复优先级,当前优先级:%d\r\n",OSTCBTbl[tasknum].TaskStr,OSTCBTbl[tasknum].TaskPrio); 
#endif 
			 }
		 }
	 }
}

#endif
/*-----------------------------------FSC智能纠错系统函数（Page Down）------------------------------------------*/
/********************************************************************************************************/
 


OSSTK TASK_IDLE_STK[TaskIDLE_StkSize];    //空闲任务堆栈
OSSTK TASK_MANAGE_STK[TaskManage_StkSize];//任务管理器任务堆栈


#if (OS_CMD_ALL_ENABLE == 1) 
INT32 CmdNumBuff[OS_CMD_NUM_LEN]={0};
OS_CMD OS_Cmd;
#endif
#if (OS_FAULT_ENABLE == 1) 
OS_FAULT OS_Fault;
#endif

OS_SYSTEM OS_System;
OS_TCB OSTCBTbl[OS_MAX_TASKS];    
OS_TCB *OSTCBRun; 
OS_TCB *OSTCBCur; 
OS_TCB *OSTCBNext;

STK32U* OSTaskStkInit(void (*task),STK32U *p)
{
    STK32U *stk;
    stk = p;
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
        stk  -= 17;
#endif
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
    *(--stk)  = (INT32U)0x01000000L;   // xPSR   
#else
    *(  stk)  = (INT32U)0x01000000L;   // xPSR  
#endif
    *(--stk)  = (INT32U)task;          // R15 (PC)             
    *(--stk)  = (INT32U)0xFFFFFFFEL;   // R14 (LR)         
        stk  -= 5;
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1) 
        stk  -= 16;
#endif    
	    stk  -=  8;
    return stk;
}

void OSInit(void) //系统初使化，初使化各个任务的任务控制块变量的参数
{ 
	  OS_INT_ENTER();             //关闭总中断
    INT32U i;	
#if (OS_TIM_SYS_ENABLE == 1)	
	  for(i = 0; i < TIMER_SIZE; i++) {
	      OS_System.TIMER[i]=0;
			  OS_System.TIMER_RELOAD[i]=0;
		}
#endif	
#if (OS_SIGN_PP_ENABLE == 1)
		INT16U j;
		for(i = 0; i < FLAG_SIZE; i++) {
			  OS_System.POST_FLAG_COUNT[i]=0;	
			  for(j = 0; j < OS_MAX_TASKS; j++) {
					  OS_System.FLAG[i][j]=OS_FALSE;	   				
				}
		}
    for(i = 0; i < FLAG_GROUP_SIZE; i++) {
			  OS_System.FlagGroupNameInTask[i]=0;
			  OS_System.ReturnFlagGroupTable[i]=OS_FALSE;
			  OS_System.POST_FGROUP_COUNT[i]=0;
			  for(j = 0; j < FLAG_SIZE; j++) {		
		      OS_System.FlagGroupTable[i][j]=OS_FALSE;
				}
		}
		for(i = 0; i < MUTEX_SIZE; i++) {
			  OS_System.MUTEX[i]=OS_FALSE;
			  OS_System.MutexTaskNum[i]=0;  		 
		}		
		for(i = 0; i < MBOX_SIZE; i++) {
		    OS_System.MBOX[i]=(void*)0;
			  OS_System.POST_MBOX_COUNT[i]=0;
		}	
#endif			
    for(i = 0; i < OS_MAX_TASKS; i++) { 
        OSTCBTbl[i].StkPtr = (STK32U*)0; 
#if (OS_CMD_ALL_ENABLE == 1) 	
			  OSTCBTbl[i].StkSize=0;		
			  OSTCBTbl[i].StkUsed=0;
			  OSTCBTbl[i].StkFree=0;
			  OSTCBTbl[i].TaskCPUOccRateCnt=0; 
			  OSTCBTbl[i].TaskStr[0]='\0';
#endif		   			
			  OSTCBTbl[i].TaskAdd=0;
			  OSTCBTbl[i].TaskNum=0;
			  OSTCBTbl[i].TaskPrio=0;
			  OSTCBTbl[i].TaskDelayMs=0;
        OSTCBTbl[i].TaskState = TASK_CREATING;
    }
		OS_System.RuningMode=OS_RINNING_MODE;
		OS_System.Error=OS_FALSE;
#if (OS_CMD_ALL_ENABLE== 1)		
		OS_Cmd.RXOK_Flag=OS_FALSE;
		OS_System.TaskCPUOccRateMax=0;
		OS_System.TaskTimeSliceCnt=1; //从1开始计数(0不能作除数)
		OS_Cmd.RX_COUNT=0;
#endif
#if (OS_GET_SET_SW_ENABLE == 1)			
		OS_System.TaskSwitchBackNum=0;
#endif		
#if (OS_FAULT_ENABLE == 1)			
		OS_Fault.runLastTask=0;
#endif	
#if (OS_TIMECOUNTSYS_ENABLE == 1)		
		OS_ShutDown_DelayTime_Set_YMDHMS(0,0,0,0,0,0);
		OS_System.RunTime_second=0;
		OS_System.RunTime_minute=0;
		OS_System.RunTime_hour=0;
		OS_System.RunTime_day=0;
		OS_System.RunTime_mouth=0;
		OS_System.RunTime_year=0;
#endif		
		OS_System.TaskCtxSw=OS_FALSE;
    OS_System.RunDriverCode=OS_FALSE;		
		OS_System.RunOneRound=OS_FALSE;
		OS_System.FirstEnter=OS_TRUE;
		OS_System.TaskNextRunFlag=OS_TRUE;
		OS_System.TaskContinuRunTimeCnt=0;
		OS_System.RunTime_ms=0;
		OS_System.SysTimerCnt=0;
		OS_System.TimeSliceCnt=0;
		OS_System.RunLastTask=0;//初使化为任务0
		OS_System.TaskHighestPrioLast=0;//初使化最高任务优级为任务0的优先级
		OS_System.TaskNext=0;  //Next任务计数初使化
		OS_CreatTaskIdle();      //创建任务0  -空闲任务
    OSCreatTaskManage();     //创建任务1  -任务管理器任务		
		OSTCBRun = OSTCBCur;     //运行TCB指向Cur
		OSTCBCur = &OSTCBTbl[0]; //从任务0开始运行
		OSTCBNext = &OSTCBTbl[1];//下一任务赋给Next
		OS_System.Running=OS_TRUE;//OS打开运行
			
		OSPendSVInit();//PendSV初使化
		OSTimerInit(); //系统定时器初使化(1ms)
		
		OS_INT_EXIT();      //打开总中断
}

void OS_CreatTaskIdle(void)//创建空闲任务(独立创建)
{
    OS_INT_ENTER(); 
#if (OS_CMD_ALL_ENABLE== 1)
    INT8U i;	
	  char taskstr[]={"Task_Idle"};
#endif		
    OSTCBTbl[0].StkPtr = OSTaskStkInit(OS_TaskIdle,(STK32U*)&TASK_IDLE_STK[TaskIDLE_StkSize - 1]);
		OSTCBTbl[0].StkAdd = TASK_IDLE_STK;
    OSTCBTbl[0].TaskState = TASK_RUNNING;
	  OSTCBTbl[0].TaskAdd=(INT32U)OS_TaskIdle;
	  OSTCBTbl[0].TaskPrio=0;
		OSTCBTbl[0].TaskPrioBackup=OSTCBTbl[0].TaskPrio;
		OSTCBTbl[0].TaskTimeSlice=OS_TIMESLICE_IDLE;
#if (OS_CMD_ALL_ENABLE== 1)		
    OSTCBTbl[0].StkSize=TaskIDLE_StkSize;	
	  for(i=0;i<TASK_NAME_LEN;i++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[0].TaskStr[i]=taskstr[i];
		}
#endif			
    OS_INT_EXIT();
}
void OSCreatTaskManage(void)//创建任务管理器任务(独立创建)
{
    OS_INT_ENTER(); 
#if (OS_CMD_ALL_ENABLE== 1)	
    INT8U i;	
		char taskstr[]={"Task_Manage"};
#endif			
    OSTCBTbl[1].StkPtr = OSTaskStkInit(OS_TaskManage, (STK32U*)&TASK_MANAGE_STK[TaskManage_StkSize - 1]);
		OSTCBTbl[1].StkAdd = TASK_MANAGE_STK;
    OSTCBTbl[1].TaskState = TASK_RUNNING;
	  OSTCBTbl[1].TaskAdd=(INT32U)OS_TaskManage;
   	OSTCBTbl[1].TaskPrio=1;
		OSTCBTbl[1].TaskPrioBackup=OSTCBTbl[1].TaskPrio;
		OSTCBTbl[1].TaskTimeSlice=OS_TIMESLICE_MANAGE;
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[1].StkSize=TaskManage_StkSize;	
		for(i=0;i<TASK_NAME_LEN;i++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[1].TaskStr[i]=taskstr[i];
		}
#endif		
    OS_INT_EXIT();
}

void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT32U tasktimeslice,INT16U taskprio)//任务创建函数(创建存放内存区域(堆栈)等)
{
	  OS_INT_ENTER();
    INT16U i;
#if (OS_CMD_ALL_ENABLE== 1)		
	  INT16U j; 
#endif	
    STK32U *stk;
	  stk=(STK32U*)&stkptr[stksize-1];
	  i=2;         //跳过空闲任务和任务管理器任务创建用户函数
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //查找空TCB
        i++;
    }
		OSTCBTbl[i].StkPtr = OSTaskStkInit(task,stk);
		OSTCBTbl[i].StkAdd = stkptr;
    OSTCBTbl[i].TaskState = taskstate;
		OSTCBTbl[i].TaskAdd=(INT32U)task;
		OSTCBTbl[i].TaskNum=i;
		OSTCBTbl[i].TaskPrio=taskprio;
		OSTCBTbl[i].TaskPrioBackup=OSTCBTbl[i].TaskPrio;
		OSTCBTbl[i].TaskTimeSlice=tasktimeslice;
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[i].StkSize=stksize;		
		for(j=0;j<TASK_NAME_LEN;j++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[i].TaskStr[j]=*taskstr++;
		}
#endif		
    OS_INT_EXIT();
}
void OS_TaskIdle(void) //空闲任务内容函数(禁止调用OS_delayMs函数)  (用以防止0个任务运行出错)
{
#if (OS_CMD_ALL_ENABLE == 1)
	INT16U CupOccRate;
#endif	
	while(1) //空闲任务用于统计cpu使用率
	{
/*------------------------------------------系统状态有关代码---------------------------------------------------*/		
#if (OS_CMD_ALL_ENABLE == 1)		
		if(OS_System.TaskTimeSliceCnt>1000)
		{
			CupOccRate=(INT16U)(OSTCBTbl[0].TaskCPUOccRateCnt*1000/OS_System.TaskTimeSliceCnt);
			if(CupOccRate<(1000-OS_System.TaskCPUOccRateMax))
			{
				OS_System.TaskCPUOccRateMax=1000-CupOccRate;
			}
		}
#endif	
/*-----------------------------------------------------------------------------------------------------------*/		
	}
}
/*------------------------------------------系统指令有关函数---------------------------------------------------*/
void OSCmdRxByte(INT8U RxByte)
{
#if (OS_CMD_ALL_ENABLE == 1)	
	if(OS_Cmd.RXOK_Flag==OS_FALSE)
	{
		if(OS_Cmd.RX_COUNT<OS_CMD_STR_LEN-1) 
			{
				OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT++]=RxByte;
				if((OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT-1]=='/')&&(OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT-2]=='/'))
				{
				  OS_Cmd.RXOK_Flag=OS_TRUE;
				}
			}
		else { OS_Cmd.RX_COUNT=0; OS_Cmd.RX_BUFF[OS_Cmd.RX_COUNT]=RxByte; }
	}
#else
  RxByte=RxByte;//防止警告
#endif	
}
#if (OS_CMD_ALL_ENABLE == 1)
INT32U OS_GetStringLength(char* p)//获取字符串p的长度
{
  INT32U i=0;
	while(*p!='\0')
	{
	  i++;
		p++;
	}
	return i;
}
INT8U CompareCmd(char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p内容是否一致
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLength(p));
}

INT32U OS_GetStringLengthWithChar(char* p,char ch)//获取字符ch在字符串p中的位置
{
  INT32U i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
	  i++;
		p++;
	}
	return i;
}
INT32* OS_GetStringNum(char* p)//从字符串p中提取数字保存到全局变量CmdNumBuff[]数组中
{
	INT8U NegFlag=OS_FALSE;
	INT16U i=0;
  for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  CmdNumBuff[i]=0;
	}
	for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  while(((*p<'0')||(*p>'9'))&&(*p!='\0')) p++;
		if(*(p-1)=='-') NegFlag=OS_TRUE;
		while((*p>='0')&&(*p<='9'))
		{
		  CmdNumBuff[i]=CmdNumBuff[i]*10+(*p-'0');
			p++;
		}
		if(NegFlag==OS_TRUE) CmdNumBuff[i]=~CmdNumBuff[i]+1;
	}
	return CmdNumBuff;
}
INT8U CompareCmdDig(char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，字符'='以前的内容是否一致
{
  return strncmp(OS_Cmd.RX_BUFF,p,OS_GetStringLengthWithChar(p,'='));
}
INT32 GrabCmdDig(INT8U n)//获取字符串p中的第n个数字(n=0开始)
{
	INT32* pf;
	pf=OS_GetStringNum(OS_Cmd.RX_BUFF);
  return pf[n];
}
#endif

#if (OS_CMD_ALL_ENABLE == 1)
void OSManage(void)
{
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
	INT8U  ShutDownTime_Second=0;
#endif	
	INT16U TaskCount;
	INT32U Count;
	INT32U StkFreeCount;
#if (OS_CMD_DISP_SYS_ENABLE == 1)	
	char *pf;
#endif	
	OS_INT_ENTER();
	for(TaskCount=0;TaskCount<OS_MAX_TASKS;TaskCount++)
	{
		if(OSTCBTbl[TaskCount].StkPtr != (STK32U*)0)
		{
			StkFreeCount=0;
			for(Count=0;Count<OSTCBTbl[TaskCount].StkSize;Count++)
			{
				if(OSTCBTbl[TaskCount].StkAdd[Count]==0)
				{
					StkFreeCount++;		
				}
				else break;
			}
			OSTCBTbl[TaskCount].StkFree=StkFreeCount;
			if(OSTCBTbl[TaskCount].StkSize>OSTCBTbl[TaskCount].StkFree)
			{
				OSTCBTbl[TaskCount].StkUsed=OSTCBTbl[TaskCount].StkSize-OSTCBTbl[TaskCount].StkFree;
			}
			else
			{
				OS_System.Error=OS_TRUE;
			}
		}
	}
	OS_INT_EXIT();
#if (OS_CMD_DISP_SYS_ENABLE == 1)
  OSprintf("+++++++++++++++++++++++++++++++++++系统状态信息++++++++++++++++++++++++++++++++++++\r\n");
	OSprintf("===================================================================================\r\n");
	switch(OS_System.RuningMode)
	{
	  case 0:pf="Order"; break;
		case 1:pf="Prio ";  break;
	}
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
	if(OS_System.ShutDownTime_second>0) ShutDownTime_Second=OS_System.ShutDownTime_second-1;
#endif	
  OSprintf("  运行模式 ：%s                          运行时间：%d年%d月%d日 %d时%d分%d秒 %dms\r\n",
	                                           pf,
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
																						 OS_System.RunTime_year,
																						 OS_System.RunTime_mouth,
																						 OS_System.RunTime_day,
																						 OS_System.RunTime_hour,
																						 OS_System.RunTime_minute,
																						 OS_System.RunTime_second,
																						 OS_System.RunTime_ms); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0,
	                                           0);
#endif	
	OSprintf("  时钟节拍 ：%dms                            系统关闭倒计时：%d年%d月%d日 %d时%d分%d秒\r\n",\
	                                           OSTIMER_LOAD/OSTIMER_CONT_1us/1000,
#if (OS_TIMECOUNTSYS_ENABLE == 1)
                                             OS_System.ShutDownTime_year,
																						 OS_System.ShutDownTime_mouth,
																						 OS_System.ShutDownTime_day,
																						 OS_System.ShutDownTime_hour,
																						 OS_System.ShutDownTime_minute,
																						 ShutDownTime_Second); 
#else
                                             0,
																						 0,
																						 0,
																						 0,
																						 0,
																						 0); 
#endif	
	OSprintf("  CPU占用率：%04.1f%%                          CPU最大占用率：%04.1f%% \r\r\n",100-(OSTCBTbl[0].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt),OS_System.TaskCPUOccRateMax/10.0);
	OSprintf("-----------------------------------------------------------------------------------\r\n");
	OSprintf("利用率   活跃度   使用栈   空闲栈   百分比   时间片    优先级   状态    任务名    \r\n");
	OSprintf("  CPU    Active    Used     Free     Per    TimeSlice   Prio    State   Taskname   \r\n\r\n");
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
		   OSprintf(" %04.1f%%    %03d       %-3d      %-3d    %04.1f%%     %04d     %05d     %01d     %s \r\n",\
									OSTCBTbl[TaskCount].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt,\
			            OSTCBTbl[TaskCount].TaskCPUOccRateCnt*1000/OS_System.SysTimerCnt,\
									OSTCBTbl[TaskCount].StkUsed,\
									OSTCBTbl[TaskCount].StkFree,\
									OSTCBTbl[TaskCount].StkUsed*100.0/OSTCBTbl[TaskCount].StkSize,\
									OSTCBTbl[TaskCount].TaskTimeSlice,\
			            OSTCBTbl[TaskCount].TaskPrio,\
			            OSTCBTbl[TaskCount].TaskState,\
									OSTCBTbl[TaskCount].TaskStr  );			                 
		}
	}	
	OSprintf("===================================================================================\r\n");
#endif
}
#endif

#if (OS_CMD_ALL_ENABLE == 1)
void OSDispMsg(void){}  //空函数
#if (OS_CMD_NSY_ENABLE == 1)	
#if (OS_CMD_DISP_NSY_ENABLE == 1)
#define OSDisp_task1open()   OSprintf("\r\n Task1 = RUNNUNG\r\n\r\n");
#define OSDisp_task2open()   OSprintf("\r\n Task2 = RUNNUNG\r\n\r\n");
#define OSDisp_task3open()   OSprintf("\r\n Task3 = RUNNUNG\r\n\r\n");
#define OSDisp_task4open()   OSprintf("\r\n Task4 = RUNNUNG\r\n\r\n");
#define OSDisp_task5open()   OSprintf("\r\n Task5 = RUNNUNG\r\n\r\n");
														 
#define OSDisp_task1close()  OSprintf("\r\n Task1 = PAUSING\r\n\r\n");
#define OSDisp_task2close()  OSprintf("\r\n Task2 = PAUSING\r\n\r\n");
#define OSDisp_task3close()  OSprintf("\r\n Task3 = PAUSING\r\n\r\n");
#define OSDisp_task4close()  OSprintf("\r\n Task4 = PAUSING\r\n\r\n");
#define OSDisp_task5close()  OSprintf("\r\n Task5 = PAUSING\r\n\r\n");
	
#define OSTask1PrioSet()     OSprintf("\r\n Task1 Prio Set OK!\r\n\r\n");
#define OSTask2PrioSet()     OSprintf("\r\n Task2 Prio Set OK!\r\n\r\n");
#define OSTask3PrioSet()     OSprintf("\r\n Task3 Prio Set OK!\r\n\r\n");
#define OSTask4PrioSet()     OSprintf("\r\n Task4 Prio Set OK!\r\n\r\n");
#define OSTask5PrioSet()     OSprintf("\r\n Task5 Prio Set OK!\r\n\r\n");
	
#define OSDisp_Task1TimeSlice()	 OSprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[2].TaskStr,OSTCBTbl[2].TaskTimeSlice);
#define OSDisp_Task2TimeSlice()	 OSprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[3].TaskStr,OSTCBTbl[3].TaskTimeSlice);
#define OSDisp_Task3TimeSlice()	 OSprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[4].TaskStr,OSTCBTbl[4].TaskTimeSlice);
#define OSDisp_Task4TimeSlice()	 OSprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[5].TaskStr,OSTCBTbl[5].TaskTimeSlice);
#define OSDisp_Task5TimeSlice()	 OSprintf("\r\n -----   %s TimeSlice = %dms   -----\r\n\r\n",OSTCBTbl[6].TaskStr,OSTCBTbl[6].TaskTimeSlice);

#define OSDisp_setsystime()	 OSprintf("\r\n SetSysTime OK!\r\n\r\n");
#else 
#define OSDisp_task1open()      OSDispMsg();
#define OSDisp_task2open()      OSDispMsg(); 
#define OSDisp_task3open()      OSDispMsg(); 
#define OSDisp_task4open()      OSDispMsg(); 
#define OSDisp_task5open()      OSDispMsg(); 
														    
#define OSDisp_task1close()     OSDispMsg();
#define OSDisp_task2close()     OSDispMsg();
#define OSDisp_task3close()     OSDispMsg();
#define OSDisp_task4close()     OSDispMsg();
#define OSDisp_task5close()     OSDispMsg();
														    
#define OSTask1PrioSet()        OSDispMsg();
#define OSTask2PrioSet()        OSDispMsg();
#define OSTask3PrioSet()        OSDispMsg();
#define OSTask4PrioSet()        OSDispMsg();
#define OSTask5PrioSet()        OSDispMsg();

#define OSDisp_Task1TimeSlice() OSDispMsg();
#define OSDisp_Task2TimeSlice() OSDispMsg();
#define OSDisp_Task3TimeSlice() OSDispMsg();
#define OSDisp_Task4TimeSlice() OSDispMsg();
#define OSDisp_Task5TimeSlice() OSDispMsg();

#define OSDisp_setsystime()     OSDispMsg();
#endif
#endif

#if (OS_CMD_DISP_SYS_ENABLE == 1)

#define OSDisp_runmode_order() OSprintf("\r\n -----$$ OS RUNNING MODE : ORDER $$-----\r\n\r\n");
#define OSDisp_runmode_prio()  OSprintf("\r\n -----$$ OS RUNNING MODE : PRIO  $$-----\r\n\r\n");
#else 
#define OSDisp_runmode_order() OSDispMsg();
#define OSDisp_runmode_prio()  OSDispMsg();
#endif
#endif
/************************************************************************************************/
void OS_TaskManage(void) //任务管理器任务内容函数
{	
	OSprintf("-@FSC_STOS_V4.7 Inside\r\n");
	OSprintf("OS Information，Please Send: cmd/osmanage//\r\n\r\n");
  while(1) 
	{
		OSSchedLock();	
#if (OS_FAULT_ENABLE == 1)	
		FSC_FAULT_SYSTEM();
#endif		
#if (OS_CMD_ALL_ENABLE == 1)		
		if(OS_Cmd.RXOK_Flag==OS_TRUE)
		{			
#if (OS_CMD_NSY_ENABLE == 1)
      /********************************************任务打开****************************************/	//任务1对应TCB[2]		
			if(CompareCmd(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
			if(CompareCmd(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
			if(CompareCmd(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
			if(CompareCmd(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
			if(CompareCmd(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
			//如任务超过5个请自行增加
			
			/*******************************************************************************************/	
			/********************************************任务关闭****************************************/	
			if(CompareCmd(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
			if(CompareCmd(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
			if(CompareCmd(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
			if(CompareCmd(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
			if(CompareCmd(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
			//如任务超过5个请自行增加
			
			/*******************************************************************************************/		
			/*******************************************任务优先级***************************************/	
			if(CompareCmdDig(cmd_task_prio_01)==0) { OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(1));OSTask1PrioSet(); }	                                                                            
			if(CompareCmdDig(cmd_task_prio_02)==0) { OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(1));OSTask2PrioSet(); }		                                       
			if(CompareCmdDig(cmd_task_prio_03)==0) { OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(1));OSTask3PrioSet(); }	                                       
			if(CompareCmdDig(cmd_task_prio_04)==0) { OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(1));OSTask4PrioSet(); }	                                      
			if(CompareCmdDig(cmd_task_prio_05)==0) { OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(1));OSTask5PrioSet(); }	                                       
			//如任务超过5个请自行增加
			/*******************************************************************************************/		
			/*******************************************任务时间片***************************************/	
			if(CompareCmdDig(cmd_task_timeslice_01)==0){ OSTCBTbl[2].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task1TimeSlice();}
			if(CompareCmdDig(cmd_task_timeslice_02)==0){ OSTCBTbl[3].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task2TimeSlice();}
			if(CompareCmdDig(cmd_task_timeslice_03)==0){ OSTCBTbl[4].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task3TimeSlice();}
			if(CompareCmdDig(cmd_task_timeslice_04)==0){ OSTCBTbl[5].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task4TimeSlice();}
			if(CompareCmdDig(cmd_task_timeslice_05)==0){ OSTCBTbl[6].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task5TimeSlice();}
			//如任务超过5个请自行增加
			/****************************************用户自定义指令区*************************************/
	    

			/*******************************************************************************************/
#endif	
			/*******************************************************************************************/
#if (OS_SYSTIME_ENABLE== 1)			
			if(CompareCmdDig(cmd_setsystime)==0) { RTC_SetTimes(GrabCmdDig(0),\
				                                                  GrabCmdDig(1),\
			                                                    GrabCmdDig(2),\
			                                                    GrabCmdDig(3),\
			                                                    GrabCmdDig(4),\
			                                                    GrabCmdDig(5)
			                                                   );
				                                                  OSDisp_setsystime(); }		
#endif			
			/*******************************************************************************************/	

			
			/*******************************************************************************************/	
			if(CompareCmd(cmd_osmanage)==0)      { OSManage(); }
      if(CompareCmd(cmd_runmode_order)==0) { OS_System.RuningMode=0; OSDisp_runmode_order(); }
			if(CompareCmd(cmd_runmode_prio)==0)  { OS_System.RuningMode=1; OSDisp_runmode_prio();  }
			/*******************************************************************************************/	
		}	
    OS_Cmd.RX_COUNT=0;
    OS_Cmd.RXOK_Flag=OS_FALSE;		 	
#endif		
		OSSchedUnlock();
	  OS_delayMs(1000);		
  }
} 
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------任务设置有关函数---------------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)||(OS_CMD_NSY_ENABLE == 1)
INT8U OSTaskStateSet(void* Taskx,INT8U TaskState) //任务状态设置函数(用户可调用) //返回OS_FALSE设置失败 OS_TRUE设置成功
{
	 OS_INT_ENTER();
	 INT16U i ,j;
	 INT8U  result=OS_FALSE;   
	 if(OSTCBCur->TaskAdd==(INT32U)Taskx) //如果是设置本任务，停止本任务，立即切换到其他任务运行
		{
			 for(j=0;j<MUTEX_SIZE;j++)//查找当前任务是否处于Mutex占用中
			 {
				 if(OS_System.MutexTaskNum[j]==OSTCBCur->TaskNum)
				 {
					 OS_INT_EXIT();  
           return OS_FALSE;
				 } 
			 }
			 OSTCBTbl[OSTCBCur->TaskNum].TaskState = TaskState; 
			 OSTCBCur = OSTCBNext;	 
			 OSPendSVPulse();
			 result= OS_TRUE;
			 
		}
	 else
	 {
		 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //跳过空闲任务，不能更改空闲任务的状态
		 {
			 for(i = 0; i < OS_MAX_TASKS; i++) 
				{  
					 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
					 {
						 do
						 {
							 for(j=0;j<MUTEX_SIZE;j++) //查找设置目标任务是否处于Mutex占用中
							 {
								 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)
								 { 
									 OS_INT_EXIT();
									 OSPendSVPulse();//把任务切换出去等待目标任务释放Mutex
									 OS_INT_ENTER(); 
									 if(OS_System.MutexTaskNum[j]==OSTCBTbl[i].TaskNum)//重新确认 
								   {						  
									   j--; //如果没有释放则重新等待
								   } 
									 else
									 {
										 break; //如果释放了，重新从0号Mutex扫描，保存证所有Mutex都不处于被目标任务使用状态
									 } 
								 }
							 } 
						 }
						 while(j<MUTEX_SIZE);
						 OSTCBTbl[i].TaskState = TaskState;
						 result= OS_TRUE;
					 }
				}
		 }
	 }
	 OS_INT_EXIT(); 
	 return result;
}
void OSTaskPrioSet(void* Taskx,INT16U Taskprio) //任务优先级设置函数(用户可调用)
{
	 INT16U i ;
	 OS_INT_ENTER();  	
	 for(i = 0; i < OS_MAX_TASKS; i++) 
		{  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
					OSTCBTbl[i].TaskPrio = Taskprio;
					break;
			 }
		}
	 OS_INT_EXIT(); 
}
void OSTimeSliceSet(INT32U time) //设置时间切片节拍数
{
   OS_System.TaskTimeSliceCnt=time;
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------系统获取与跳转有关函数---------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)
INT16U* OSRunTimeGet(void) //获取系统累计运行时间,指针成员分别为：ms、s、m、h、d、m、y
{
  return (INT16U*)&OS_System.RunTime_ms;
}
INT8U* OSFlagGroupPendTableGet(INT16U FGNum) //获取群等待成员
{
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U i;
	for(i=0;i<FLAG_SIZE;i++)
	{
		OS_System.ReturnFlagGroupTable[i]=OS_System.FlagGroupTable[FGNum][i];//复制成员到返回查询群列表
	}
	return (INT8U*)OS_System.ReturnFlagGroupTable;
#else 
	return (INT8U*)0;
#endif	
}
INT16U OSMutexBlockTaskGet(INT16U MNum)//获取Mutex当前阻塞的任务
{
#if (OS_SIGN_PP_ENABLE == 1)	
	return OS_System.MutexTaskNum[MNum];
#else 
	return 0;	
#endif	
}
INT8U OSTaskStateGet(void* Taskx) //获取任务状态
{
	 INT16U i ;
	 INT8U stat;
	 for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				 stat= OSTCBTbl[i].TaskState;
				 break;
			 }
		}
   return stat;		
}
INT16U OSTaskPrioGet(void* Taskx) //获取任务优先级
{
	 INT16U i ;
	 INT16U prio;
	 for(i = 0; i < OS_MAX_TASKS; i++) {  
			 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
			 {
				 prio= OSTCBTbl[i].TaskPrio;
				 break;
			 }
		}
   return prio;		
}
INT32U OSTimeSliceGet(void) //获取时间切片节拍数
{
    return OS_System.TaskTimeSliceCnt;
}
INT32U OSTimerValGet(void) //获取系统定时器当前计数值(默认为9MHz，每次计数用时1/9 us)
{
   return SysTick->VAL;  //返回系统定时器当前计数值(用于用户us级精确延时，计数单位为系统定时器主频，此处使用嘀嗒定时器，主频为72Mhz时为9MHz，其他主频时，按8分频计算)
}
void OSTaskSwitchBack(void* Taskx) //任务跳转 带返回
{
	 OS_INT_ENTER(); 
   INT16U i;	
	 if(OSTCBCur->TaskAdd!=(INT32U)Taskx) 
	 {
		 for(i = 0; i < OS_MAX_TASKS; i++) {  
				 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
				 {  
					 if(OSTCBTbl[i].TaskState==TASK_RUNNING)
					 {
						 OS_System.TaskSwitchBackNum=OSTCBCur->TaskNum;
						 OSTCBCur=&OSTCBTbl[i]; 
						 OS_System.TaskCtxSw=OS_TRUE;
						 OSTIMER_VAL=0;
						 OSPendSVPulse();					 
						 break;
					 }
				 }
		 }
	 }
	 OS_INT_EXIT(); 
}
void OSTaskSwitch(void* Taskx) //任务跳转  不带返回
{
	 OS_INT_ENTER(); 
   INT16U i;	
	 if(OSTCBCur->TaskAdd!=(INT32U)Taskx) 
	 {
		 for(i = 0; i < OS_MAX_TASKS; i++) {  
				 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
				 { 
           if(OSTCBTbl[i].TaskState==TASK_RUNNING)
					 {					 
						 OS_System.TaskNext=i;
						 OSTCBNext = &OSTCBTbl[OS_System.TaskNext];
						 OSTCBCur=&OSTCBTbl[i]; 
             OS_System.TaskCtxSw=OS_TRUE;						 
						 OSTIMER_VAL=0; 
						 OSPendSVPulse();					 
						 break;
					 }
				 }
		 }
	 }
	 OS_INT_EXIT(); 
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------系统不常用有关函数-----------------------------------------------*/
/********************主要用于中断优先级低于Systick定时器中断(系统定时器)的中断线********************/
#if (OS_REMOTE_ENABLE == 1)
void OSIntEnter(void) //进入中断函数必须在中断函数开头调用 (用户可调用)
{
	OS_INT_ENTER(); 
	if (OS_System.IntNesting < 255)  
		{ 
	    OS_System.IntNesting++; 
	  }
	OS_INT_EXIT(); 
}
void OSIntExit(void) //离开中断函数必须在中断函数末尾调用（OSIntExit必须和OSIntEnter成对出现） (用户可调用)
{
	OS_INT_ENTER();
	if (OS_System.IntNesting > 0) 
	  { 
	    OS_System.IntNesting--; 
	  }
	OS_INT_EXIT();
}
#endif
/****************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------系统虚拟定时器有关函数------------------------------------------------*/
#if (OS_TIM_SYS_ENABLE == 1)
void OSTimerReloadSet(INT16U TNum,INT32U time)//重装载定时值设置 
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSet(INT16U TNum,INT32U time)//自动重装载定时值设置 (time= 0-关闭自动重装载 ， >0启用自动重装载)
{
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSet(INT16U TNum,INT32U time)//定时值设置
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
void OSTimerAutoReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER_RELOAD[TNum]=time;
}
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
INT32U OSTimerValueGet(INT16U TNum) //返回当前计数值
{
  return OS_System.TIMER[TNum];
}
INT8U OSTimerStateGet(INT16U TNum) //返回OS_TRUE ：定时完成，  OS_FALSE：未完成
{
  if(OS_System.TIMER[TNum]==0) return OS_TRUE;
	else return OS_FALSE;
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------系统信号量有关函数-------------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagPost(INT16U FNum) //发送标志量
{
	OSSchedLock();
	INT16U i,j;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
    if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_Flag))//单个OSFlag处于等待信号标志状态
		{
			if(OSTCBTbl[i].FlagName==FNum)//判断该等待任务等待的OSFlag是否是当前准备发送的OSFlag
			{
				OSTCBTbl[i].TaskFlagBit&=~(1<<_BIT_Flag);//清除信号标志状态
				OS_System.FLAG[FNum][i]=OS_TRUE; //给等待OSFlag的任务发送标志量
				OS_System.POST_FLAG_COUNT[FNum]++;//等待处理信号次数累加
				OSTCBTbl[i].TaskDelayMs=1;  //等待延时置1，取消等待OSFlag任务的等待延时 
			}
		}
		if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_FlagGroup))//群OSFlag处于等待信号标志状态
		{
			for(j=0;j<FLAG_GROUP_SIZE;j++)
			{
				if(OS_System.FlagGroupTable[j][FNum]==OS_TRUE)//判断是否为群成员
				{
					OS_System.FLAG[FNum][i]=OS_TRUE; //给等待OSFlag群的任务发送成员标志量		
				}
			}
		}
	}
	OSSchedUnlock();
}

INT8U OSFlagPend(INT8U pendtype,INT16U FNum,INT32U timeout_ms) //等待标志量 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	INT8U result;
	OSSchedLock();
	OSTCBCur->FlagName=FNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_Flag);
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSFlag_BPN:
		{
			OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除旧标志量,等待新的标志量发送
			OS_delayMs(timeout_ms);
			OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
			if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
			{
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
				result = OS_FALSE;
			}
			else
			{	
				if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					result = OS_TRUE; 
				}
				else
				{
					result = OS_FALSE;
				}
			}
		} 
		break;
		case OSFlag_NBPN:
		{
			if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
			{
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				result = OS_TRUE; 
			}
			else
			{
				result = OS_FALSE;
			}
		}
		break;
		case OSFlag_BPO:
		{
		  if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_FALSE)//检测旧标志
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
				if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
					result = OS_FALSE;
				}
				else
				{	
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
				OSTCBCur->TaskDelayMs=0;//让其处于就绪状态
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_BPC:
		{
		  if(OS_System.POST_FLAG_COUNT[FNum]==0)//检测标志累计数量
			{
				OS_delayMs(timeout_ms);
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
				if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
				{
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
					result = OS_FALSE;
				}
				else
				{	
					OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
					OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
					result = OS_TRUE; 
				}		
			}
			else
			{
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_Flag);
				OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
				OSTCBCur->TaskDelayMs=0;//让其处于就绪状态
				OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
				result = OS_TRUE;
			}
		}
		break;
		case OSFlag_NBPC:
		{
			if(OS_System.POST_FLAG_COUNT[FNum]>0)
			{
		    OS_System.POST_FLAG_COUNT[FNum]--;//等待处理信号次数-1(已处理)
				result=OS_System.POST_FLAG_COUNT[FNum];//返回剩于等待处理次数
			}
			else
			{
			  result = OS_FALSE;
			}
		}
		break;
	}
	return result;
}

void OSFlagAddToGroup(INT16U FGNum,INT16U FNum)  //添加标志量到标志量群
{
	OSSchedLock();
  OS_System.FlagGroupTable[FGNum][FNum]=OS_TRUE;
	OSSchedUnlock();
}

INT8U OSFlagGroupPend(INT8U pendtype,INT16U FGNum,INT32U timeout_ms) //等待标志量群 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	INT8U result;
	OSSchedLock();
	OSTCBCur->FlagGroupName=FGNum;
	OS_System.FlagGroupNameInTask[OSTCBCur->TaskNum]=FGNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_FlagGroup);
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSFGroup_BPN:
		{
			OS_delayMs(timeout_ms);	
			OSTCBCur->TaskFlagBit&=~(1<<_BIT_FlagGroup);	
			if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
			{
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
				result = OS_FALSE;
			}
			else
			{
				OS_System.POST_FGROUP_COUNT[FGNum]--;
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_FlagGroup);//清除本任务FlagGroup群等待标志
				result = OS_TRUE;
			}
		}
		break;
		case OSFGroup_NBPN:
		{
			if(OS_System.GROUPRXFLAG[FGNum][OSTCBCur->TaskNum]==OS_TRUE)
			{
				OS_System.POST_FGROUP_COUNT[FGNum]--;
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_FlagGroup);
				OS_System.GROUPRXFLAG[FGNum][OSTCBCur->TaskNum]=OS_FALSE;
				result = OS_TRUE;
			}
			else
			{
				result = OS_FALSE;
			}
		}
		break;
	}
	return result;
}

void OSMutexPost(INT16U MNum) //发送互斥量 （主要用于解除mutex锁定状态,解除后所有mutex等待任务将重新竞争mutex权限）
{
	OSSchedLock();
	if(OSTCBCur->TaskNum==OS_System.MutexTaskNum[MNum])//判断当前任务是否具有Post权限
	{
		if((OSTCBCur->TaskFlagBit&(1<<_BIT_Mutex))==0)//判断当前任务是否处于非等待mutex状态中
		{
			if(OS_System.MUTEX[MNum]==OS_TRUE) //判断mutex是否为true(忙)
			{
				OS_System.MutexTaskNum[MNum]=0;//放弃Post权限
				OS_System.MUTEX[MNum]=OS_FALSE;//置mutex为OS_FALSE表示mutex空闲
			}
		}
	}
	OSSchedUnlock();
}
INT8U OSMutexPend(INT16U MNum,INT32U timeout_ms) //等待互斥量 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	OSSchedLock();
	OSTCBCur->MutexName=MNum;
	OS_System.MutexNameInTask[OSTCBCur->TaskNum]=MNum;
	if(OS_System.MutexTaskNum[MNum]==0) OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//任务获得Post权限
	OSTCBCur->TaskFlagBit|=(1<<_BIT_Mutex);//置位mutex标志
	OSSchedUnlock();
	OS_delayMs(timeout_ms);
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_Mutex);//清除mutex标志
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
	{	
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
		return OS_FALSE;
	}
	else
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志
		OS_System.MUTEX[MNum]=OS_TRUE;	
		OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//任务获得Post权限
		return OS_TRUE;
	}
}
void OSMboxPost(INT16U MNum,void* fp)  //发送邮件
{
	OSSchedLock();
	INT16U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
		if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_MBox))//判断该任务是否处于等待mbox状态
		{
			if(OSTCBTbl[i].MBoxName==MNum)//判断等待的mbox是否是当前准备发送的MNum
			{
				OS_System.MBOX[MNum*OS_MAX_TASKS+i]=fp;//给在等待邮件的任务发送邮件
				OS_System.POST_MBOX_COUNT[MNum]++;
				OS_System.MBQueue[OS_System.POST_MBQ_COUNT[MNum]+MNum*MBQ_SIZE]=fp;
				if(OS_System.POST_MBQ_COUNT[MNum]<MBQ_SIZE-1) OS_System.POST_MBQ_COUNT[MNum]++;
				else OS_System.POST_MBQ_COUNT[MNum]=0;
				OSTCBTbl[i].TaskDelayMs=0;//等待延时清0
			}
		}
	}
	OSSchedUnlock();
} 

void* OSMboxPend(INT8U pendtype,INT16U MNum,INT32U timeout_ms) //等待邮箱 带超时时间  ，返回非(void*)0：等待未超时  (void*)0：等待超时
{
	OSSchedLock();
	INT16U MBQCount;
	INT32U* _mbox;	
	OSTCBCur->MBoxName=MNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_MBox);
	OSSchedUnlock();
	switch(pendtype)
	{
		case OSMBox_BPN:
		{
			OS_delayMs(timeout_ms);	
			OSTCBCur->TaskFlagBit&=~(1<<_BIT_MBox);
			_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
			if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
			{
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
				_mbox=(void*)0;
			}
			else
			{  
				if(_mbox!=(void*)0)
				{
					OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
				}
				else
				{
				  OS_System.POST_MBOX_COUNT[MNum]--;
				}
			}
		}
		break;
		case OSMBox_BPQ:
		{
		  if(OS_System.POST_MBQ_COUNT[MNum]==0)
			{
				OS_delayMs(timeout_ms);	
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_MBox);
				_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
				if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
				{
					OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
					_mbox=(void*)0;
				}
				else
				{  
					if(_mbox!=(void*)0)
					{
						OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
					}
					else
					{
						_mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
						for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
						{
							OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
						}
						OS_System.POST_MBQ_COUNT[MNum]--;
					}
				}
			}
			else
			{
				_mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
				for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
				{
				  OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
				}
				OS_System.POST_MBQ_COUNT[MNum]--;
			}
		}
		break;
		case OSMBox_NBPN:
		{
			_mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
			if(_mbox!=(void*)0)
			{
				OS_System.POST_MBOX_COUNT[MNum]--;
				OSTCBCur->TaskFlagBit&=~(1<<_BIT_MBox);
				OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=(void*)0;  //清空本任务邮箱为(void*)0	
			}
			else
			{
				_mbox=(void*)0;
			}
		}
		break;
		case OSMBox_NBPQ:
		{
		  if(OS_System.POST_MBQ_COUNT[MNum]>0)
			{
			  _mbox=(INT32U*)OS_System.MBQueue[0+MNum*MBQ_SIZE];
				for(MBQCount=0;MBQCount<OS_System.POST_MBQ_COUNT[MNum]-1;MBQCount++)
				{
				  OS_System.MBQueue[MBQCount+MNum*MBQ_SIZE]=OS_System.MBQueue[MBQCount+1+MNum*MBQ_SIZE];
				}
				OS_System.POST_MBQ_COUNT[MNum]--;
			}
			else
			{
			  _mbox=(void*)0;
			}
		}
		break;
	}
	return _mbox;
}

#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------系统信号量处理有关函数------------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagGroupHandler(void)
{
	INT16U TaskCount,Count,NCount;
	for(TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_FlagGroup))//检测有FlagGroup等待的任务
		{	
			NCount=0;
			for(Count=0; Count < FLAG_SIZE; Count++)
			{
				if(OS_System.FlagGroupTable[OS_System.FlagGroupNameInTask[TaskCount]][Count]==OS_TRUE)
				{
					if(OS_System.FLAG[Count][TaskCount]==OS_TRUE) NCount++;
					else break;
				}
			}
			if((Count==FLAG_SIZE)&&(NCount!=0))
			{
				for(Count=0; Count < FLAG_SIZE; Count++)
				{
					if(OS_System.FlagGroupTable[OS_System.FlagGroupNameInTask[TaskCount]][Count]==OS_TRUE)
					{
						if(OS_System.FLAG[Count][TaskCount]==OS_TRUE) OS_System.FLAG[Count][TaskCount]=OS_FALSE;
					}
				}
				OSTCBTbl[TaskCount].TaskDelayMs=1;//准备退出延时，进入等待运行的状态(由优先级控制)
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志
			}
		}
	}
}
void OSMutexHandler_Order(void)
{
  INT16U TaskCount;	
	for(TaskCount=OS_System.TaskNext; TaskCount < OS_MAX_TASKS; TaskCount++)//检测NextTask以后的最靠前的任务是否有等待mutex
	{
		if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
			 (OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_Mutex))&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
			{
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_Mutex);//取消等待mutex标志
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeOut);//取消延时标志
				OSTCBTbl[TaskCount].TaskDelayMs=1;//进入准备运行状态(能否能立即运行取决于优先级)
				OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]=OS_TRUE;//当前任务的mutex置忙	
				break;
			}		
	}
}
void OSMutexHandler_Prio(void)
{
  INT16U TaskCount,Count;	
	for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_Mutex))&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
				{
					Count=TaskCount; //Count用来记录有mutex等待的最大优先级任务
				}
		}			
	}
	if(Count!=0)
	{
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_Mutex);//取消等待mutex标志
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_TimeOut);//取消延时标志
		OSTCBTbl[Count].TaskDelayMs=1;//进入准备运行状态(能否能立即运行取决于优先级)
		OS_System.MUTEX[OS_System.MutexNameInTask[Count]]=OS_TRUE;//当前任务的mutex置忙
	}
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------系统信号量处理有关函数------------------------------------------------*/
#if (OS_TIMECOUNTSYS_ENABLE == 1)
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second) //系统关闭定时时间设置(用户可用)
{			 
	OS_System.ShutDownTime_second=second;
  OS_System.ShutDownTime_minute=minute;
	OS_System.ShutDownTime_hour  =hour;
	OS_System.ShutDownTime_day   =day;	
	OS_System.ShutDownTime_mouth =mouth;
	OS_System.ShutDownTime_year  =year;
}
void OS_Run_Time(void) //系统运行时间统计(用户不可用)
{
	 if(OS_System.RunTime_ms<1000) OS_System.RunTime_ms++;
	 else
	 {
		 OS_System.RunTime_ms=0;
		 OS_System.RunTime_second++;
		 if(OS_System.RunTime_second>=60) {
  		  OS_System.RunTime_second=0;OS_System.RunTime_minute++;
		 if(OS_System.RunTime_minute>=60) { 
		    OS_System.RunTime_minute=0;OS_System.RunTime_hour++;
		 if(OS_System.RunTime_hour>=24)   { 
		    OS_System.RunTime_hour=0;OS_System.RunTime_day++;
		 if(OS_System.RunTime_hour>=24)   { 
		    OS_System.RunTime_hour=0;OS_System.RunTime_day++;
		 if(OS_System.RunTime_day>=30)    { 
		    OS_System.RunTime_day=0;OS_System.RunTime_mouth++;
		 if(OS_System.RunTime_mouth>=12)  { 
		    OS_System.RunTime_mouth=0;OS_System.RunTime_year++;
		 }}}}}} 
  
		 if((OS_System.ShutDownTime_second==0)&&(\
        (OS_System.ShutDownTime_minute!=0)||\
				(OS_System.ShutDownTime_hour  !=0)||\
				(OS_System.ShutDownTime_day   !=0)||\
				(OS_System.ShutDownTime_mouth !=0)||\
				(OS_System.ShutDownTime_year  !=0)))
		 {	
       OS_System.ShutDownTime_second=1;
		 }				
		 if((OS_System.ShutDownTime_second==2)&&\
        (OS_System.ShutDownTime_minute==0)&&\
				(OS_System.ShutDownTime_hour  ==0)&&\
				(OS_System.ShutDownTime_day   ==0)&&\
				(OS_System.ShutDownTime_mouth ==0)&&\
				(OS_System.ShutDownTime_year  ==0))
			{
				OS_System.Running=OS_FALSE;	
				OSprintf("定时关闭完成，系统停止运行\r\n");
			}
			else
			{
			 if(OS_System.ShutDownTime_second>1) OS_System.ShutDownTime_second--;	
			 if(OS_System.ShutDownTime_second==1) 
				 {
					OS_System.ShutDownTime_second=60;
				  if(OS_System.ShutDownTime_minute>0) 
					{
						OS_System.ShutDownTime_minute--;
					}
					else if(OS_System.ShutDownTime_hour>0) 
					{
					  OS_System.ShutDownTime_hour--;
						OS_System.ShutDownTime_minute=59;
					}
					else if(OS_System.ShutDownTime_day>1) 
					{
						OS_System.ShutDownTime_day--;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}		
					else if(OS_System.ShutDownTime_mouth>1) 
					{
						OS_System.ShutDownTime_mouth--;
						OS_System.ShutDownTime_day=30;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}		
					else if(OS_System.ShutDownTime_year>1) 
					{
						OS_System.ShutDownTime_year--;
						OS_System.ShutDownTime_mouth=12;
						OS_System.ShutDownTime_day=30;
					  OS_System.ShutDownTime_hour=23;
						OS_System.ShutDownTime_minute=59;
					}												
				}			
			}
	 }
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------不可剪裁函数 PageUp-----------------------------------------*/
void OSSchedLock(void)   //任务切换上锁(用户可调用) 上锁后不会切换任务，cpu会一直运行当前任务直到解锁
{
	if(OS_System.Running == OS_TRUE)  
		{                                
			OS_INT_ENTER();  
			if (OS_System.IntNesting == 0) 
				{                                          
					if (OS_System.LockNesting < 255) 
						{         
							OS_System.LockNesting++;                     
					  }
			  }
		  OS_INT_EXIT();   
	  }
}
void OSSchedUnlock(void)   //任务解锁（OSSchedLock和OSSchedUnlock必须成对出现）(用户可调用)
{
	if (OS_System.Running == OS_TRUE)
		{                            
			OS_INT_ENTER();   
			if (OS_System.LockNesting > 0){                       
			OS_System.LockNesting--;   
			if (OS_System.LockNesting == 0){                         
			if (OS_System.IntNesting == 0){                
					OS_INT_EXIT();  
					OSPendSVPulse();  
				}else OS_INT_EXIT();           	
				}else OS_INT_EXIT(); 
				}else OS_INT_EXIT();
		}
}
void OSORunModeProcess_Order(void) //顺序模式服务程序
{
	INT16U TaskCount;
	for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) //检测所有任务的延时时间是否有的到时
	{ 
		if(OSTCBTbl[TaskCount].TaskState==TASK_RUNNING) //跳过非运行态的任务
		{
			if( OSTCBTbl[TaskCount].TaskDelayMs ==1 )//检测延时时间是否到时
			 {
				 OSTCBCur=&OSTCBTbl[TaskCount]; 	        //跳转到延时完成的任务
				 OSTCBTbl[TaskCount].TaskDelayMs=0;	  //延时清0
				 OS_System.TaskNextRunFlag=OS_FALSE;    //顺序运行标志置FALSE	
				 break;
			 }
		 }	 
	}	
	if(TaskCount==OS_MAX_TASKS) OS_System.TaskNextRunFlag=OS_TRUE;	//顺序运行标志置TRUE			 
	if(OS_System.TaskNextRunFlag==OS_TRUE)	//判断是否可以按正常顺序运行
	{
		OSTCBCur = OSTCBNext;
		OS_System.TaskNext++;						
		while((OSTCBTbl[OS_System.TaskNext].StkPtr == (STK32U*)0)||(OSTCBTbl[OS_System.TaskNext].TaskState!=TASK_RUNNING)\
				 ||OSTCBTbl[OS_System.TaskNext].TaskDelayMs > 1) //跳过非法状态任务
		{ 
			OS_System.TaskNext++;
			if(OS_System.TaskNext == OS_MAX_TASKS) 
			{
				OS_System.TaskNext = 0;
				OS_System.RunOneRound=OS_TRUE;
			}
		}
		OSTCBNext = &OSTCBTbl[OS_System.TaskNext];	//更新Next任务
	}	
}
INT16U OSHighestPrioTaskNumGet(void)//获取所有任务中的最高优先级任务
{
  INT16U TaskCount,Count;	
	for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)
				{
					Count=TaskCount; //Count用来记录最大优先级
				}
		}				
	}	
	return Count;
}
INT16U OSRunStateHighestPrioTaskNumGet(void)//获取处于运行状态的最高优先级任务
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0))
				{
					HighestPrioTask=TaskCount; //Count用来记录最大优先级
				}
		}				
	}	
	return HighestPrioTask;
}
INT16U OSRdyHighestPrioTaskNumGet(void)//获取运行就绪最高优先级任务
{
  INT16U TaskCount,HighestPrioTask;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrio用来记录最大优先级
				}
		}				
	}	
	return HighestPrioTask;
}

/***************************************************************************************
*任务名：OSNextRdyHighestPrioTaskNumGet
*作用  ：获取运行就绪最高优先级任务，具有相同优先级的多个任务轮询
*特点  ：如果扫描到本次最高优先级和上次相同且具有该优先级的任务除上次刚运行过的任务以外还有其他任务
        则返回其他任务(其他任务具有2个及以上时，按顺序返回，实现具有相同优先级的多个任务轮询)
***************************************************************************************/
INT16U OSNextRdyHighestPrioTaskNumGet(void)//获取运行就绪最高优先级任务
{
  INT16U TaskCount,HighestPrioTask,LastPrioTaskCount=0,SecdyHigestPrioTask,SecdyCheckFlag=0,MainCheckFlag=0;	
	for(HighestPrioTask=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
	{ 
		if(OSTCBTbl[HighestPrioTask].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
		{
			if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
				 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
				{
					HighestPrioTask=TaskCount; //HighestPrioTask用来记录扫描到的最大优先级任务
				}
		}			
	}	
	if(OSTCBTbl[HighestPrioTask].TaskPrio>OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
	  
	}
	else if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[OS_System.RunLastTask].TaskPrio)
	{
		for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //统计正在运行的常规最高优先级任务数量(上次运行过的除外)
		{ 
			if(TaskCount==OS_System.RunLastTask) continue ;
			if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[TaskCount].TaskPrio) 
			{
				if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
					 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
					{
						LastPrioTaskCount++; //LastPrioTaskCount用来记录最大优先级任务数量(上次运行过的除外)
					}
			}			
		}	
		if(LastPrioTaskCount>0) //如果扫描到和上次运行的任务优先级相同的任务超过0个(不包含上次任务本身)
		{
		
			for(TaskCount=0; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
			{ 
				if(TaskCount==OS_System.RunLastTask) continue ;
				if(OSTCBTbl[HighestPrioTask].TaskPrio==OSTCBTbl[TaskCount].TaskPrio) 
				{
					if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
						 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
						{
							if(SecdyCheckFlag==0)
							{
								if(TaskCount<OS_System.RunLastTask) 
								{
									SecdyCheckFlag=1;
									SecdyHigestPrioTask=TaskCount;
								}
							}
							if(TaskCount>OS_System.RunLastTask) 
							{
								MainCheckFlag=1;
								HighestPrioTask=TaskCount;
								break;
							}
						}
				 }			
			}	 
			if(MainCheckFlag==0)  HighestPrioTask=SecdyHigestPrioTask;
		}
	}
	return HighestPrioTask;
}
void OSORunModeProcess_Prio(void)//优先级模式服务程序
{
	INT16U HighestPrioTask;
	HighestPrioTask=OSNextRdyHighestPrioTaskNumGet();
	if(OS_System.TaskHighestPrioLast!=OSTCBTbl[HighestPrioTask].TaskPrio)
	{
		OS_System.TaskHighestPrioLast=OSTCBTbl[HighestPrioTask].TaskPrio;
	  OS_System.RunLastTask=OS_System.TaskHighestRunLast[OS_System.TaskHighestPrioLast];
		HighestPrioTask=OSNextRdyHighestPrioTaskNumGet();	
	}
	if(OS_System.RunLastTask!=HighestPrioTask)
	{
	  OS_System.RunLastTask=HighestPrioTask;
		OS_System.TaskHighestRunLast[OS_System.TaskHighestPrioLast]=OS_System.RunLastTask;
	}
	OSTCBCur = OSTCBNext;	
	OSTCBNext=&OSTCBTbl[HighestPrioTask];//运行最高优先级任务
	OSTCBTbl[HighestPrioTask].TaskDelayMs=0;//延时清0	
	if(HighestPrioTask==0) OS_System.RunOneRound=OS_TRUE;
}

void OS_Timer_Handler(void) //任务切换核心函数
{
	 OS_INT_ENTER();
	 INT16U TaskCount,Count;	
	 if(OS_System.Running==OS_TRUE)
	 {
#if (OS_TIMECOUNTSYS_ENABLE == 1)		 
		 OS_Run_Time();
#endif		 
		 OS_System.SysTimerCnt++;
		 for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) 
		 {   
	#if (OS_FAULT_ENABLE == 1)		 
			 FSC_FAULT_SYSTEM_OS_Timer_Handler(TaskCount);
	#endif		 
			 if(( OSTCBTbl[TaskCount].TaskDelayMs >1 )&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0)) 
			 {
				 OSTCBTbl[TaskCount].TaskDelayMs--;
				 if(OSTCBTbl[TaskCount].TaskDelayMs==1) OSTCBTbl[TaskCount].TaskFlagBit|=(1<<_BIT_TimeOut);//标记延时完成
			 }
		 }	
	#if (OS_TIM_SYS_ENABLE == 1)		 
		 for(Count = 0; Count < TIMER_SIZE; Count++)
		 {
			 if(OS_System.TIMER[Count]>0) OS_System.TIMER[Count]--;
			 if(OS_System.TIMER[Count]==0) 
			 {
				 if(OS_System.TIMER_RELOAD[Count]!=0) 
				 {
					 OS_System.TIMER[Count]=OS_System.TIMER_RELOAD[Count];
				 }
			 }
		 }
	#endif	
		 OS_System.TimeSliceCnt++;
		 if((OS_System.TimeSliceCnt>=OSTCBCur->TaskTimeSlice)||(OS_System.TaskCtxSw==OS_TRUE))
		 {
			 if(OS_System.TaskCtxSw==OS_TRUE) OS_System.TaskCtxSw=OS_FALSE;
  #if (OS_FAULT_ENABLE == 1) 				 
			 OSTaskEndlessLoopsCheck();//任务陷入死循环检测
	#endif			 
			 OS_System.TimeSliceCnt=0; 
	#if (OS_CMD_ALL_ENABLE == 1) 		 
			 OSTCBCur->TaskCPUOccRateCnt++;
			 OS_System.TaskTimeSliceCnt++;
			 if(OS_System.SysTimerCnt==0) OS_System.TaskTimeSliceCnt=0;
			 if(OS_System.TaskTimeSliceCnt==0)
			 {
				 for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++)
				 {
					 if(OSTCBTbl[OS_System.TaskNext].StkPtr != (STK32U*)0)
					 {
						 OSTCBTbl[TaskCount].TaskCPUOccRateCnt=0;
					 }
				 }
				 OS_System.TaskTimeSliceCnt=1;//从1开始计数(0不能作除数)		 
			 }
	#endif		 
			 if((OS_System.LockNesting == 0) && (OS_System.IntNesting == 0))
			 {				 
				 OSPendSVPulse(); 
	#if (OS_FAULT_ENABLE == 1)				 
	RESCHEDULE:	
	#endif			
	#if (OS_GET_SET_SW_ENABLE == 1)			 
				 if(OS_System.TaskSwitchBackNum>0) //是否有跳转发生完成
				 {
					 OS_System.TaskNext=OS_System.TaskSwitchBackNum;	//更新顺序运行断点
					 OSTCBNext = &OSTCBTbl[OS_System.TaskSwitchBackNum]; //返回跳转前的任务
					 OS_System.TaskSwitchBackNum=0;                      //清0
					 OS_System.TaskNextRunFlag=OS_TRUE; //顺序运行标志置TRUE			 
				 }	
				 else
	#endif			 
				 {	
	#if (OS_SIGN_PP_ENABLE == 1)     					
					 OSFlagGroupHandler();//OSFlagGroup有关
	#endif
					 if(OS_System.RuningMode==0)//运行模式:Order	
					 {		 
						 /*******************************************可抢断顺序运行*******************************************/
	#if (OS_SIGN_PP_ENABLE == 1) 				 
						 OSMutexHandler_Order();//OSMutex有关函数
	#endif				 
						 OSORunModeProcess_Order();
					 }
					 else				
						{		
							/*******************************************抢断式优先级******************************************/	
	#if (OS_SIGN_PP_ENABLE == 1)     					
							OSMutexHandler_Prio();//OSMutex有关函数
	#endif					
							OSORunModeProcess_Prio();
						}		
				 }				
			 }	
	#if (OS_FAULT_ENABLE == 1)	
			 OS_Fault.tcblast=OSTCBCur;	 
			 OS_Fault.tcbnext=OSTCBNext;	 
			 if(OS_FAULT_STK_CHECK(OSTCBCur)==OS_TRUE)
			 {
				 goto RESCHEDULE;
			 }
	#endif	 
		 }	 
	 }
	 else if(OS_System.Running==OS_FALSE)
	 {
	   OSTIMER_OFF();//关闭ostimer定时器
	 }
   OS_INT_EXIT();  
}


void delay_us(INT32U nus) //微秒延时函数 (用户可调用,可在中断中使用)
{   	
	OSTIMER_INT_OFF();//关闭系统定时器中断
	INT32U Ticks,TCntOld,TCntNow,TCnt=0;
	INT32U ReloadTimerValue=OSTIMER_LOAD;       
	Ticks=OSTIMER_CONT_1us*nus;   
	TCntOld=OSTIMER_VAL;
	while(1)
	{
			TCntNow=OSTIMER_VAL;  
			if(TCntNow!=TCntOld)
			{       
					if(TCntNow<TCntOld) TCnt += TCntOld-TCntNow;
					else TCnt += ReloadTimerValue-TCntNow+TCntOld;   
					TCntOld = TCntNow;
					if(TCnt>=Ticks) break;
			} 			
	} 
  if((OS_System.RunDriverCode==OS_FALSE)&&(OS_System.Running==OS_TRUE))
	{
		OSTIMER_INT_ON(); 	//打开系统定时器中断
	}
}
void delay_ms(INT32U nms)  //毫秒延时函数 (用户可调用,中断中不可用)
{     
	OSTCBCur->TaskDelayMs=nms+1;
  while(OSTCBCur->TaskDelayMs>1);
	OSTCBCur->TaskDelayMs=0;
}
void OS_delayMs(volatile INT32U nms) //OS延时函数(用户可调用,中断中不可用)
{
	while((OS_System.IntNesting != 0) && (OS_System.LockNesting != 0));
	OS_INT_ENTER(); 
	if(nms==0){nms=2;//赋一个比1大的值即可
	OSTCBCur->TaskFlagBit|=(1<<_BIT_TimeLess);} //置位无限等待标志
  else OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志	
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//取消计时完成标志
	OSTCBCur->TaskDelayMs=nms+1;
  OSPendSVPulse(); 
	OS_INT_EXIT(); 
	while(OSTCBCur->TaskDelayMs);
}
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec)//日时分秒延时，<=49天17时2分47秒  ,中断中不可用
{
	INT32U time;
	if(day>49) day=49;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
	OS_delayMs(time);
}
/*--------------------------------------------不可剪栽函数 PageDown---------------------------------------------*/

/**************************************************望穿秋水*****************************************************/
