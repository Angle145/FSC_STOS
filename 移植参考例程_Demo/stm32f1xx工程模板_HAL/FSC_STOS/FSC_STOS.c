#include "fsc_stos.h" 

/**********************************************可修改部分*********************************************/

/*---------------------------------自定义系统指令-----------------------------------*/
//如任务超过5个，请自行增加指令,并到OS_TaskManage()任务管理器任务中增加相应的代码(在本页约425行)。
#if (OS_CMD_ALL_ENABLE == 1)
#if (OS_CMD_NSY_ENABLE == 1)
/*---------------任务开关----------------*/
char cmd_task_open_01[]={"cmd/Task1/open//"};//只要保证指令以//结尾即可，如{"打开任务1//"}
char cmd_task_open_02[]={"cmd/Task2/open//"};
char cmd_task_open_03[]={"cmd/Task3/open//"};
char cmd_task_open_04[]={"cmd/Task4/open//"};
char cmd_task_open_05[]={"cmd/Task5/open//"};

char cmd_task_close_01[]={"cmd/Task1/close//"};
char cmd_task_close_02[]={"cmd/Task2/close//"};
char cmd_task_close_03[]={"cmd/Task3/close//"};
char cmd_task_close_04[]={"cmd/Task4/close//"};
char cmd_task_close_05[]={"cmd/Task5/close//"};
/*----------------------------------------*/
/*--------------任务优先级----------------*/
char cmd_task_prio_01[]={"cmd/Task1/prio=%d//"};//%d表示整型数字,例：cmd/Task1/prio=8//
char cmd_task_prio_02[]={"cmd/Task2/prio=%d//"};
char cmd_task_prio_03[]={"cmd/Task3/prio=%d//"};
char cmd_task_prio_04[]={"cmd/Task4/prio=%d//"};
char cmd_task_prio_05[]={"cmd/Task5/prio=%d//"};
/*----------------------------------------*/
#endif
/*--------------系统有关------------------*/
char cmd_osmanage[]={"cmd/osmanage//"};
char cmd_runmode_0[]={"cmd/runmode/order//"};//顺序运行模式
char cmd_runmode_1[]={"cmd/runmode/prio//"}; //优先级运行模式
/*----------------------------------------*/
/*----------------------------------------------------------------------------------*/
#endif
/*--------------------------------------系统定时器----------------------------------*/
//默认使用SysTick定时器
#define OSTIMER_INT_OFF()  SysTick->CTRL&=~(1<<1)     //关闭系统定时器中断
#define OSTIMER_INT_ON()   SysTick->CTRL|=(1<<1)      //打开系统定时器中断
#define OSTIMER_CONT_1us   (SystemCoreClock/8/1000000)//系统定时器1us计数次数 
#define OSTIMER_LOAD       SysTick->LOAD              //系统定时器重装载值寄存器
#define OSTIMER_VAL        SysTick->VAL               //系统定时器当前值寄存器
void OSTimerInit(INT16U Nus)//用作OS的计时脉冲，OS的时间片=Nus个脉冲，Nus可调
{ 
	OS_INT_ENTER();
	
	//Systick定时器初使化(使用其他定时器时，请修改为其他定时器)
	char * Systick_priority = (char *)0xe000ed23;     //Systick中断优先级寄存器
	SysTick->LOAD  = OSTIMER_CONT_1us* Nus;           //Systick定时器重装载计数值
	*Systick_priority = 0x00;                         //Systick定时器中断优先级
	SysTick->VAL   = 0;                               //Systick定时器计数器清0
	SysTick->CTRL = 0x3;                              //Systick使用外部晶振时钟，8分频  72MHz/8=9MHz  计数9000次=1ms  计数900次=100us
	
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
/*----------------------------------------------------------------------------------*/
/********************************************************************************************************/
 


OSSTK TASK_IDLE_STK[TaskIDLE_StkSize];    //空闲任务堆栈
OSSTK TASK_MANAGE_STK[TaskManage_StkSize];//任务管理器任务堆栈


#if (OS_SIGN_PP_ENABLE == 1) 
INT32U Null='\0';
#endif

#if (OS_CMD_ALL_ENABLE == 1) 
INT16U CmdNumBuff[OS_CMD_NUM_LEN]={0};
OS_PERIP OS_Perip;
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
    *(stk)    = (INT32U)0x01000000L;   // xPSR                                               
    *(--stk)  = (INT32U)task;          // Entry Point   空间保存任务函数入口地址             
    *(--stk)  = (INT32U)0xFFFFFFFEL;   // R14 (LR)          
	  stk -= 13; //R0-R12
    return stk;
}

void OSInit(void) //系统初使化，初使化各个任务的任务控制块变量的参数
{ 
	  OS_INT_ENTER();             //关闭总中断
    INT32U i;	
#if (OS_TIM_SYS_ENABLE == 1)	
	  for(i = 0; i < TIMER_SIZE; i++) {
	      OS_System.TIMER[i]=0;
		}
#endif	
#if (OS_SIGN_PP_ENABLE == 1)
		INT16U j;
		for(i = 0; i < FLAG_SIZE; i++) {
			  for(j = 0; j < OS_MAX_TASKS; j++) {
					  OS_System.FLAG[i][j]=OS_FALSE;				  
				}
		}
    for(i = 0; i < FLAG_GROUP_SIZE; i++) {
			  OS_System.FlagGroupNameInTask[i]=0;
			  OS_System.ReturnFlagGroupTable[i]=OS_FALSE;
			  for(j = 0; j < FLAG_SIZE; j++) {		
		      OS_System.FlagGroupTable[i][j]=OS_FALSE;
				}
		}
		for(i = 0; i < MUTEX_SIZE; i++) {
			  OS_System.MUTEX[i]=OS_FALSE;
			  OS_System.MutexTaskNum[i]=0;  		 
		}		
		for(i = 0; i < MBOX_SIZE; i++) {
		    OS_System.MBOX[i]=&Null;
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
		OS_Perip.CmdRxFlag=OS_FALSE;
		OS_System.TaskCPUOccRateMax=0;
		OS_System.TaskTimeSliceCnt=1; //从1开始计数(0不能作除数)
		OS_Perip.USART_COUNT=0;
#endif
#if (OS_GET_SET_SW_ENABLE == 1)			
		OS_System.TaskSwitchBackNum=0;
#endif		
		OS_System.TaskNextRunFlag=OS_TRUE;
		OS_System.TaskNext=0;  //Next任务计数初使化
		OS_CreatTaskIdle();      //创建任务0  -空闲任务
    OSCreatTaskManage();     //创建任务1  -任务管理器任务		
		OSTCBRun = OSTCBCur;     //运行TCB指向Cur
		OSTCBCur = &OSTCBTbl[0]; //从任务0开始运行
		OSTCBNext = &OSTCBTbl[1];//下一任务赋给Next
		OS_System.Running=OS_TRUE;//OS打开运行
			
		OSPendSVInit();             //PendSV初使化
		OSTimerInit(OS_CLOCK_TIME); //系统定时器初使化
		
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
#if (OS_CMD_ALL_ENABLE== 1)		
    OSTCBTbl[0].StkSize=TaskIDLE_StkSize;	
	  for(i=0;i<TASK_NAME_SIZE;i++)
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
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[1].StkSize=TaskManage_StkSize;	
		for(i=0;i<TASK_NAME_SIZE;i++)
		{
			if(*taskstr=='\0') break;
		  OSTCBTbl[1].TaskStr[i]=taskstr[i];
		}
#endif		
    OS_INT_EXIT();
}

void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT16U taskprio)//任务创建函数(创建存放内存区域(堆栈)等)
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
#if (OS_CMD_ALL_ENABLE== 1)	
    OSTCBTbl[i].StkSize=stksize;		
		for(j=0;j<TASK_NAME_SIZE;j++)
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
	while(1)             //空闲任务用于统计cpu使用率
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
void OSCmdUsartReceiveByte(INT8U UsartRxByte)
{
#if (OS_CMD_ALL_ENABLE == 1)	
	if(OS_Perip.CmdRxFlag==OS_FALSE)
	{
		if(OS_Perip.USART_COUNT<OS_PERIP_USART_BUFF_LEN-1) 
			{
				OS_Perip.USART_RX_BUFF[OS_Perip.USART_COUNT++]=UsartRxByte;
				if((OS_Perip.USART_RX_BUFF[OS_Perip.USART_COUNT-1]=='/')&&(OS_Perip.USART_RX_BUFF[OS_Perip.USART_COUNT-2]=='/'))
				{
				  OS_Perip.CmdRxFlag=OS_TRUE;
				}
			}
		else { OS_Perip.USART_COUNT=0; OS_Perip.USART_RX_BUFF[OS_Perip.USART_COUNT]=UsartRxByte; }
	}
#else
  UsartRxByte=UsartRxByte;//防止警告
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
  return strncmp(OS_Perip.USART_RX_BUFF,p,OS_GetStringLength(p));
}

#if (OS_CMD_NSY_ENABLE == 1)
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
INT16U* OS_GetStringNum(char* p)//从字符串p中提取数字保存到全局变量CmdNumBuff[]数组中
{
	INT16U i=0;
  for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  CmdNumBuff[i]=0;
	}
	for(i=0;i<OS_CMD_NUM_LEN;i++)
	{
	  while(((*p<'0')||(*p>'9'))&&(*p!='\0')) p++;
		while((*p>='0')&&(*p<='9'))
		{
		  CmdNumBuff[i]=CmdNumBuff[i]*10+(*p-'0');
			p++;
		}
	}
	return CmdNumBuff;
}
INT8U CompareCmdDig(char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，字符'='以前的内容是否一致
{
  return strncmp(OS_Perip.USART_RX_BUFF,p,OS_GetStringLengthWithChar(p,'='));
}
INT16U GrabCmdDig(char* p,INT8U n)//获取字符串p中的第n个数字(n=0开始)
{
	INT16U* pf;
	pf=OS_GetStringNum(p);
  return pf[n];
}
#endif
#endif
#if (OS_CMD_ALL_ENABLE == 1)
void OSManage(void)
{
	INT16U TaskCount;
	INT32U Count;
	INT32U StkFreeCount;
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
  OSprintf("++++++++++++++++++++++++系统状态信息++++++++++++++++++++++++++\r\n");
	OSprintf("==============================================================\r\n");
	switch(OS_System.RuningMode)
	{
	  case 0: OSprintf("  OS Running Mode:  Order   \r\n"); break;
		case 1: OSprintf("  OS Running Mode:  Prio    \r\n"); break;
	}
	OSprintf("  TaskTimeSliceCnt: %d \r\n",OS_System.TaskTimeSliceCnt);
	OSprintf("  CPU占用率：%0.1f%%          CPU最大占用率：%0.1f%% \r\r\n",100-(OSTCBTbl[0].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt),OS_System.TaskCPUOccRateMax/10.0);
	OSprintf("--------------------------------------------------------------\r\n");
	OSprintf("利用率   使用栈   空闲栈   百分比   优先级   状态     任务名    \r\n");
	OSprintf("  CPU     Used     Free     Per      Prio    State   Taskname   \r\n\r\n");
	for(TaskCount = 0; TaskCount < OS_MAX_TASKS; TaskCount++) 
	{
		if(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)
		{	
		   OSprintf(" %0.1f%%     %-3d      %-3d     %0.1f%%      %-2d      %-2d     %s \r\n",\
									OSTCBTbl[TaskCount].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt,\
									OSTCBTbl[TaskCount].StkUsed,\
									OSTCBTbl[TaskCount].StkFree,\
									OSTCBTbl[TaskCount].StkUsed*100.0/OSTCBTbl[TaskCount].StkSize,\
									OSTCBTbl[TaskCount].TaskPrio,\
			            OSTCBTbl[TaskCount].TaskState,\
									OSTCBTbl[TaskCount].TaskStr  );			                 
		}
	}	
	OSprintf("==============================================================\r\n");
#endif
}
#endif

void OSDispMsg(void){}  //空函数
#if (OS_CMD_DISP_NSY_ENABLE == 1)
#define OSDisp_task1open()  OSprintf("\r\n Task1 = RUNNUNG  \r\n\r\n");
#define OSDisp_task2open()  OSprintf("\r\n Task2 = RUNNUNG  \r\n\r\n");
#define OSDisp_task3open()  OSprintf("\r\n Task3 = RUNNUNG  \r\n\r\n");
#define OSDisp_task4open()  OSprintf("\r\n Task4 = RUNNUNG  \r\n\r\n");
#define OSDisp_task5open()  OSprintf("\r\n Task5 = RUNNUNG  \r\n\r\n");

#define OSDisp_task1close() OSprintf("\r\n Task1 = PAUSING  \r\n\r\n");
#define OSDisp_task2close() OSprintf("\r\n Task2 = PAUSING  \r\n\r\n");
#define OSDisp_task3close() OSprintf("\r\n Task3 = PAUSING  \r\n\r\n");
#define OSDisp_task4close() OSprintf("\r\n Task4 = PAUSING  \r\n\r\n");
#define OSDisp_task5close() OSprintf("\r\n Task5 = PAUSING  \r\n\r\n");

#define OSDisp_runmode_order() OSprintf("\r\n -----$$ OS RUNNING MODE : ORDER $$------  \r\n\r\n");
#define OSDisp_runmode_prio()  OSprintf("\r\n -----$$ OS RUNNING MODE : PRIO  $$------  \r\n\r\n");
#else 
#define OSDisp_task1open()   OSDispMsg();
#define OSDisp_task2open()   OSDispMsg(); 
#define OSDisp_task3open()   OSDispMsg(); 
#define OSDisp_task4open()   OSDispMsg(); 
#define OSDisp_task5open()   OSDispMsg(); 

#define OSDisp_task1close()  OSDispMsg();
#define OSDisp_task2close()  OSDispMsg();
#define OSDisp_task3close()  OSDispMsg();
#define OSDisp_task4close()  OSDispMsg();
#define OSDisp_task5close()  OSDispMsg();

#define OSDisp_runmode_order() OSDispMsg();
#define OSDisp_runmode_prio()  OSDispMsg();
#endif

/************************************************************************************************/
void OS_TaskManage(void) //任务管理器任务内容函数
{
  while(1) 
	{
#if (OS_CMD_ALL_ENABLE == 1)
		OSSchedLock();		
		if(OS_Perip.CmdRxFlag==OS_TRUE)
		{			
#if (OS_CMD_NSY_ENABLE == 1)
      /*******************************************************************************************/	//任务1对应TCB[2]		
			if(CompareCmd(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
			if(CompareCmd(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
			if(CompareCmd(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
			if(CompareCmd(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
			if(CompareCmd(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
			//如任务超过5个请自行增加
			
			/*******************************************************************************************/	
			/*******************************************************************************************/	
			if(CompareCmd(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
			if(CompareCmd(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
			if(CompareCmd(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
			if(CompareCmd(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
			if(CompareCmd(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
			//如任务超过5个请自行增加
			
			/*******************************************************************************************/	
			
			/*******************************************************************************************/	
			if(CompareCmdDig(cmd_task_prio_01)==0) OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));		                                                                            
			if(CompareCmdDig(cmd_task_prio_02)==0) OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));	                                       
			if(CompareCmdDig(cmd_task_prio_03)==0) OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));	                                       
			if(CompareCmdDig(cmd_task_prio_04)==0) OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));                                      
			if(CompareCmdDig(cmd_task_prio_05)==0) OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));                                       
			//如任务超过5个请自行增加
			
			/*******************************************************************************************/	
#endif
			
			/*******************************************************************************************/	
			if(CompareCmd(cmd_osmanage)==0)  { OSManage(); }
      if(CompareCmd(cmd_runmode_0)==0) { OS_System.RuningMode=0; OSDisp_runmode_order(); }
			if(CompareCmd(cmd_runmode_1)==0) { OS_System.RuningMode=1; OSDisp_runmode_prio();  }
			/*******************************************************************************************/	
		}	
    OS_Perip.USART_COUNT=0;
    OS_Perip.CmdRxFlag=OS_FALSE;		
	  OSSchedUnlock();	
#endif		
	  OS_delayMs(1000);		
  }
} 
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------任务设置有关函数---------------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)||(OS_CMD_NSY_ENABLE == 1)
void OSTaskStateSet(void* Taskx,INT8U TaskState) //任务状态设置函数(用户可调用)
{
	 INT16U i ;
	 OS_INT_ENTER();  
	 if(OSTCBCur->TaskAdd==(INT32U)Taskx) //如果是设置本任务，停止本任务，立即切换到其他任务运行
		{
			 OSTCBTbl[OSTCBCur->TaskNum].TaskState = TaskState; 
			 OSTCBCur = OSTCBNext;	 
			 OSPendSVPulse();
		}
	 else
	 {
		 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //跳过空闲任务，不能更改空闲任务的状态
		 {
			 for(i = 0; i < OS_MAX_TASKS; i++) 
				{  
					 if( OSTCBTbl[i].TaskAdd == (INT32U)Taskx ) 
					 {
							OSTCBTbl[i].TaskState = TaskState;
							break;
					 }
				}
		 }
	 }
	 OS_INT_EXIT(); 
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
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------系统获取与跳转有关函数---------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)
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
					 OS_System.TaskSwitchBackNum=OSTCBCur->TaskNum;
					 OSTCBCur=&OSTCBTbl[i]; 
					 OSTIMER_VAL=0;
					 OSPendSVPulse();					 
					 break;
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
					 OS_System.TaskNext=i;
					 OSTCBNext = &OSTCBTbl[OS_System.TaskNext];
					 OSTCBCur=&OSTCBTbl[i];     
					 OSTIMER_VAL=0; 
					 OSPendSVPulse();					 
					 break;
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
void OSTimerReloadSet(INT16U TNum,INT32U time)
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49天17时2分47秒
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
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
				OSTCBTbl[i].TaskDelayMs=0;  //等待延时清0，取消等待OSFlag任务的等待延时 
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
INT8U OSFlagPend(INT16U FNum,INT32U timeout_ms) //等待标志量 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	OSSchedLock();
	OSTCBCur->FlagName=FNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_Flag);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
		return OS_FALSE;
	}
	else
	{
		if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
		{
			OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//清除本任务的标志量
			return OS_TRUE;
		}
		else return OS_FALSE;
	}
}
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum)  //添加标志量到标志量群
{
	OSSchedLock();
  OS_System.FlagGroupTable[FGNum][FNum]=OS_TRUE;
	OSSchedUnlock();
}
INT8U OSFlagGroupPend(INT16U FGNum,INT32U timeout_ms) //等待标志量群 带超时时间  ，返回OS_TRUE：等待未超时  OS_FALSE：等待超时
{
	OSSchedLock();
	OSTCBCur->FlagGroupName=FGNum;
	OS_System.FlagGroupNameInTask[OSTCBCur->TaskNum]=FGNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_FlagGroup);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);		
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
		return OS_FALSE;
	}
	else
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_FlagGroup);//清除本任务FlagGroup群等待标志
	  return OS_TRUE;
	}
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
				OSTCBTbl[i].TaskDelayMs=0;//等待延时清0
			}
		}
	}
	OSSchedUnlock();
} 
void* OSMboxPend(INT16U MNum,INT32U timeout_ms) //等待邮箱 带超时时间  ，返回非(void*)0：等待未超时  (void*)0：等待超时
{
	OSSchedLock();
	INT32U* _mbox;	
	OSTCBCur->MBoxName=MNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_MBox);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);	
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //超时检测
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//清除超时标志位
		return (void*)0;
	}
	else
	{
    _mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
		OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=&Null;  //清空本任务邮箱为'\0'
		return _mbox;
	}
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


/*--------------------------------------------------不可剪裁函数 PageUp-----------------------------------------*/


void  OSSchedLock (void)   //任务切换上锁(用户可调用) 上锁后不会切换任务，cpu会一直运行当前任务直到解锁
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
void  OSSchedUnlock (void)   //任务解锁（OSSchedLock和OSSchedUnlock必须成对出现）(用户可调用)
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

void OS_Timer_Handler(void) //任务切换核心函数
{
	 OS_INT_ENTER();
	 INT16U TaskCount,Count;
	 for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) 
	 {   
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
	 }
#endif		 
#if (OS_CMD_ALL_ENABLE == 1) 		 
	 OS_System.TaskTimeSliceCnt++;
	 OSTCBCur->TaskCPUOccRateCnt++;
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
	 if((OS_System.IntNesting == 0) && (OS_System.LockNesting == 0))
	 {		 
		 OSPendSVPulse(); 
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
						if(OS_System.TaskNext == OS_MAX_TASKS)  OS_System.TaskNext = 0;
					}
					OSTCBNext = &OSTCBTbl[OS_System.TaskNext];	//更新Next任务
	      }	
			 }
			 else				
				{		
					/*******************************************抢断式优先级******************************************/	
#if (OS_SIGN_PP_ENABLE == 1)     					
          OSMutexHandler_Prio();//OSMutex有关函数
#endif					
					for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //查找正在运行的常规最高优先级任务
					{ 
						if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
						{
							if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
								 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
								{
									Count=TaskCount; //Count用来记录最大优先级
								}
						}				
					}
					OSTCBCur = OSTCBNext;	
					OSTCBNext=&OSTCBTbl[Count];//运行最高优先级任务
					OSTCBTbl[Count].TaskDelayMs=0;	  //延时清0						
				}		
		 }				
	 }				
   OS_INT_EXIT();  
}


void delay_us(INT32U nus) //微秒延时函数 (用户可调用)
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
  OSTIMER_INT_ON(); 	//打开系统定时器中断
}
void delay_ms(INT32U nms)  //毫秒延时函数 (用户可调用)
{     
	OSTCBCur->TaskDelayMs=(nms*1000/OS_CLOCK_TIME)+1;
  while(OSTCBCur->TaskDelayMs>1);
	OSTCBCur->TaskDelayMs=0;
}
void OS_delayMs(volatile INT32U nms) //OS延时函数(用户可调用)
{
	while((OS_System.IntNesting != 0) && (OS_System.LockNesting != 0));
	OS_INT_ENTER(); 
	if(nms==0){nms=2;//赋一个比1大的值即可
	OSTCBCur->TaskFlagBit|=(1<<_BIT_TimeLess);} //置位无限等待标志
  else OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//取消无限等待标志	
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//取消计时完成标志
	OSTCBCur->TaskDelayMs=(nms*1000/OS_CLOCK_TIME)+1;
  OSPendSVPulse(); 
	OS_INT_EXIT(); 
	while(OSTCBCur->TaskDelayMs);
}
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec)//日时分秒延时，<=49天17时2分47秒
{
	INT32U time;
	if(day>49) day=49;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
	OS_delayMs(time);
}
/*--------------------------------------------不可剪栽函数 PageDown---------------------------------------------*/

/**************************************************望穿秋水*****************************************************/
