#include "fsc_stos.h" 

/**********************************************���޸Ĳ���*********************************************/

/*---------------------------------�Զ���ϵͳָ��-----------------------------------*/
//�����񳬹�5��������������ָ��,����OS_TaskManage()���������������������Ӧ�Ĵ���(�ڱ�ҳԼ425��)��
#if (OS_CMD_ALL_ENABLE == 1)
#if (OS_CMD_NSY_ENABLE == 1)
/*---------------���񿪹�----------------*/
char cmd_task_open_01[]={"cmd/Task1/open//"};//ֻҪ��ָ֤����//��β���ɣ���{"������1//"}
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
/*--------------�������ȼ�----------------*/
char cmd_task_prio_01[]={"cmd/Task1/prio=%d//"};//%d��ʾ��������,����cmd/Task1/prio=8//
char cmd_task_prio_02[]={"cmd/Task2/prio=%d//"};
char cmd_task_prio_03[]={"cmd/Task3/prio=%d//"};
char cmd_task_prio_04[]={"cmd/Task4/prio=%d//"};
char cmd_task_prio_05[]={"cmd/Task5/prio=%d//"};
/*----------------------------------------*/
#endif
/*--------------ϵͳ�й�------------------*/
char cmd_osmanage[]={"cmd/osmanage//"};
char cmd_runmode_0[]={"cmd/runmode/order//"};//˳������ģʽ
char cmd_runmode_1[]={"cmd/runmode/prio//"}; //���ȼ�����ģʽ
/*----------------------------------------*/
/*----------------------------------------------------------------------------------*/
#endif
/*--------------------------------------ϵͳ��ʱ��----------------------------------*/
//Ĭ��ʹ��SysTick��ʱ��
#define OSTIMER_INT_OFF()  SysTick->CTRL&=~(1<<1)     //�ر�ϵͳ��ʱ���ж�
#define OSTIMER_INT_ON()   SysTick->CTRL|=(1<<1)      //��ϵͳ��ʱ���ж�
#define OSTIMER_CONT_1us   (SystemCoreClock/8/1000000)//ϵͳ��ʱ��1us�������� 
#define OSTIMER_LOAD       SysTick->LOAD              //ϵͳ��ʱ����װ��ֵ�Ĵ���
#define OSTIMER_VAL        SysTick->VAL               //ϵͳ��ʱ����ǰֵ�Ĵ���
void OSTimerInit(INT16U Nus)//����OS�ļ�ʱ���壬OS��ʱ��Ƭ=Nus�����壬Nus�ɵ�
{ 
	OS_INT_ENTER();
	
	//Systick��ʱ����ʹ��(ʹ��������ʱ��ʱ�����޸�Ϊ������ʱ��)
	char * Systick_priority = (char *)0xe000ed23;     //Systick�ж����ȼ��Ĵ���
	SysTick->LOAD  = OSTIMER_CONT_1us* Nus;           //Systick��ʱ����װ�ؼ���ֵ
	*Systick_priority = 0x00;                         //Systick��ʱ���ж����ȼ�
	SysTick->VAL   = 0;                               //Systick��ʱ����������0
	SysTick->CTRL = 0x3;                              //Systickʹ���ⲿ����ʱ�ӣ�8��Ƶ  72MHz/8=9MHz  ����9000��=1ms  ����900��=100us
	
	OS_INT_EXIT();
}
void SysTick_Handler(void) //Systick��ʱ���жϺ���(ʹ��������ʱ��ʱ,���޸�Ϊ������ʱ�����жϺ�����)
{
  OS_Timer_Handler();
}
/*----------------------------------------------------------------------------------*/

/*--------------------------------------PendSV��ʹ��--------------------------------*/
void OSPendSVInit(void)//PendSV��ʹ��
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //���ȼ��Ĵ�����ַ
	*NVIC_SYSPRI14=0xFF;//����PendSV�ж����ȼ���� 
}
void OSPendSVPulse(void)//����PendSV�ж�
{
	int* NVIC_INT_CTRL= (int *)0xE000ED04;  //�жϿ��ƼĴ�����ַ
  *NVIC_INT_CTRL=0x10000000;  
}
/*----------------------------------------------------------------------------------*/
/********************************************************************************************************/
 


OSSTK TASK_IDLE_STK[TaskIDLE_StkSize];    //���������ջ
OSSTK TASK_MANAGE_STK[TaskManage_StkSize];//��������������ջ


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
    *(--stk)  = (INT32U)task;          // Entry Point   �ռ䱣����������ڵ�ַ             
    *(--stk)  = (INT32U)0xFFFFFFFEL;   // R14 (LR)          
	  stk -= 13; //R0-R12
    return stk;
}

void OSInit(void) //ϵͳ��ʹ������ʹ�����������������ƿ�����Ĳ���
{ 
	  OS_INT_ENTER();             //�ر����ж�
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
		OS_System.TaskTimeSliceCnt=1; //��1��ʼ����(0����������)
		OS_Perip.USART_COUNT=0;
#endif
#if (OS_GET_SET_SW_ENABLE == 1)			
		OS_System.TaskSwitchBackNum=0;
#endif		
		OS_System.TaskNextRunFlag=OS_TRUE;
		OS_System.TaskNext=0;  //Next���������ʹ��
		OS_CreatTaskIdle();      //��������0  -��������
    OSCreatTaskManage();     //��������1  -�������������		
		OSTCBRun = OSTCBCur;     //����TCBָ��Cur
		OSTCBCur = &OSTCBTbl[0]; //������0��ʼ����
		OSTCBNext = &OSTCBTbl[1];//��һ���񸳸�Next
		OS_System.Running=OS_TRUE;//OS������
			
		OSPendSVInit();             //PendSV��ʹ��
		OSTimerInit(OS_CLOCK_TIME); //ϵͳ��ʱ����ʹ��
		
		OS_INT_EXIT();      //�����ж�
}
void OS_CreatTaskIdle(void)//������������(��������)
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
void OSCreatTaskManage(void)//�����������������(��������)
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

void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT16U taskprio)//���񴴽�����(��������ڴ�����(��ջ)��)
{
	  OS_INT_ENTER();
    INT16U i;
#if (OS_CMD_ALL_ENABLE== 1)		
	  INT16U j; 
#endif	
    STK32U *stk;
	  stk=(STK32U*)&stkptr[stksize-1];
	  i=2;         //�������������������������񴴽��û�����
		while(OSTCBTbl[i].StkPtr != (STK32U*)0) {   //���ҿ�TCB
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
void OS_TaskIdle(void) //�����������ݺ���(��ֹ����OS_delayMs����)  (���Է�ֹ0���������г���)
{
#if (OS_CMD_ALL_ENABLE == 1)
	INT16U CupOccRate;
#endif	
	while(1)             //������������ͳ��cpuʹ����
	{
/*------------------------------------------ϵͳ״̬�йش���---------------------------------------------------*/		
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

/*------------------------------------------ϵͳָ���йغ���---------------------------------------------------*/
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
  UsartRxByte=UsartRxByte;//��ֹ����
#endif	
}
#if (OS_CMD_ALL_ENABLE == 1)
INT32U OS_GetStringLength(char* p)//��ȡ�ַ���p�ĳ���
{
  INT32U i=0;
	while(*p!='\0')
	{
	  i++;
		p++;
	}
	return i;
}
INT8U CompareCmd(char* p)//�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�����Ƿ�һ��
{
  return strncmp(OS_Perip.USART_RX_BUFF,p,OS_GetStringLength(p));
}

#if (OS_CMD_NSY_ENABLE == 1)
INT32U OS_GetStringLengthWithChar(char* p,char ch)//��ȡ�ַ�ch���ַ���p�е�λ��
{
  INT32U i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
	  i++;
		p++;
	}
	return i;
}
INT16U* OS_GetStringNum(char* p)//���ַ���p����ȡ���ֱ��浽ȫ�ֱ���CmdNumBuff[]������
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
INT8U CompareCmdDig(char* p)//�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�У��ַ�'='��ǰ�������Ƿ�һ��
{
  return strncmp(OS_Perip.USART_RX_BUFF,p,OS_GetStringLengthWithChar(p,'='));
}
INT16U GrabCmdDig(char* p,INT8U n)//��ȡ�ַ���p�еĵ�n������(n=0��ʼ)
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
  OSprintf("++++++++++++++++++++++++ϵͳ״̬��Ϣ++++++++++++++++++++++++++\r\n");
	OSprintf("==============================================================\r\n");
	switch(OS_System.RuningMode)
	{
	  case 0: OSprintf("  OS Running Mode:  Order   \r\n"); break;
		case 1: OSprintf("  OS Running Mode:  Prio    \r\n"); break;
	}
	OSprintf("  TaskTimeSliceCnt: %d \r\n",OS_System.TaskTimeSliceCnt);
	OSprintf("  CPUռ���ʣ�%0.1f%%          CPU���ռ���ʣ�%0.1f%% \r\r\n",100-(OSTCBTbl[0].TaskCPUOccRateCnt*100.0/OS_System.TaskTimeSliceCnt),OS_System.TaskCPUOccRateMax/10.0);
	OSprintf("--------------------------------------------------------------\r\n");
	OSprintf("������   ʹ��ջ   ����ջ   �ٷֱ�   ���ȼ�   ״̬     ������    \r\n");
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

void OSDispMsg(void){}  //�պ���
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
void OS_TaskManage(void) //����������������ݺ���
{
  while(1) 
	{
#if (OS_CMD_ALL_ENABLE == 1)
		OSSchedLock();		
		if(OS_Perip.CmdRxFlag==OS_TRUE)
		{			
#if (OS_CMD_NSY_ENABLE == 1)
      /*******************************************************************************************/	//����1��ӦTCB[2]		
			if(CompareCmd(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
			if(CompareCmd(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
			if(CompareCmd(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
			if(CompareCmd(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
			if(CompareCmd(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
			//�����񳬹�5������������
			
			/*******************************************************************************************/	
			/*******************************************************************************************/	
			if(CompareCmd(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
			if(CompareCmd(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
			if(CompareCmd(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
			if(CompareCmd(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
			if(CompareCmd(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
			//�����񳬹�5������������
			
			/*******************************************************************************************/	
			
			/*******************************************************************************************/	
			if(CompareCmdDig(cmd_task_prio_01)==0) OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));		                                                                            
			if(CompareCmdDig(cmd_task_prio_02)==0) OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));	                                       
			if(CompareCmdDig(cmd_task_prio_03)==0) OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));	                                       
			if(CompareCmdDig(cmd_task_prio_04)==0) OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));                                      
			if(CompareCmdDig(cmd_task_prio_05)==0) OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(OS_Perip.USART_RX_BUFF,1));                                       
			//�����񳬹�5������������
			
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
/*------------------------------------------���������йغ���---------------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)||(OS_CMD_NSY_ENABLE == 1)
void OSTaskStateSet(void* Taskx,INT8U TaskState) //����״̬���ú���(�û��ɵ���)
{
	 INT16U i ;
	 OS_INT_ENTER();  
	 if(OSTCBCur->TaskAdd==(INT32U)Taskx) //��������ñ�����ֹͣ�����������л���������������
		{
			 OSTCBTbl[OSTCBCur->TaskNum].TaskState = TaskState; 
			 OSTCBCur = OSTCBNext;	 
			 OSPendSVPulse();
		}
	 else
	 {
		 if( OSTCBTbl[0].TaskAdd != (INT32U)Taskx ) //�����������񣬲��ܸ��Ŀ��������״̬
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
void OSTaskPrioSet(void* Taskx,INT16U Taskprio) //�������ȼ����ú���(�û��ɵ���)
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
/*------------------------------------------ϵͳ��ȡ����ת�йغ���---------------------------------------------*/
#if (OS_GET_SET_SW_ENABLE == 1)
INT8U* OSFlagGroupPendTableGet(INT16U FGNum) //��ȡȺ�ȴ���Ա
{
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U i;
	for(i=0;i<FLAG_SIZE;i++)
	{
		OS_System.ReturnFlagGroupTable[i]=OS_System.FlagGroupTable[FGNum][i];//���Ƴ�Ա�����ز�ѯȺ�б�
	}
	return (INT8U*)OS_System.ReturnFlagGroupTable;
#else 
	return (INT8U*)0;
#endif	
}
INT16U OSMutexBlockTaskGet(INT16U MNum)//��ȡMutex��ǰ����������
{
#if (OS_SIGN_PP_ENABLE == 1)	
	return OS_System.MutexTaskNum[MNum];
#else 
	return 0;	
#endif	
}
INT8U OSTaskStateGet(void* Taskx) //��ȡ����״̬
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
INT16U OSTaskPrioGet(void* Taskx) //��ȡ�������ȼ�
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
INT32U OSTimeSliceGet(void) //��ȡʱ����Ƭ������
{
    return OS_System.TaskTimeSliceCnt;
}
INT32U OSTimerValGet(void) //��ȡϵͳ��ʱ����ǰ����ֵ(Ĭ��Ϊ9MHz��ÿ�μ�����ʱ1/9 us)
{
   return SysTick->VAL;  //����ϵͳ��ʱ����ǰ����ֵ(�����û�us����ȷ��ʱ��������λΪϵͳ��ʱ����Ƶ���˴�ʹ����શ�ʱ������ƵΪ72MhzʱΪ9MHz��������Ƶʱ����8��Ƶ����)
}
void OSTaskSwitchBack(void* Taskx) //������ת ������
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
void OSTaskSwitch(void* Taskx) //������ת  ��������
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
/*--------------------------------------------ϵͳ�������йغ���-----------------------------------------------*/
/********************��Ҫ�����ж����ȼ�����Systick��ʱ���ж�(ϵͳ��ʱ��)���ж���********************/
#if (OS_REMOTE_ENABLE == 1)
void OSIntEnter(void) //�����жϺ����������жϺ�����ͷ���� (�û��ɵ���)
{
	OS_INT_ENTER(); 
	if (OS_System.IntNesting < 255)  
		{ 
	    OS_System.IntNesting++; 
	  }
	OS_INT_EXIT(); 
}
void OSIntExit(void) //�뿪�жϺ����������жϺ���ĩβ���ã�OSIntExit�����OSIntEnter�ɶԳ��֣� (�û��ɵ���)
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
/*---------------------------------------ϵͳ���ⶨʱ���йغ���------------------------------------------------*/
#if (OS_TIM_SYS_ENABLE == 1)
void OSTimerReloadSet(INT16U TNum,INT32U time)
{
  OS_System.TIMER[TNum]=time;
}
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec)//<=49��17ʱ2��47��
{
	INT32U time;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
  OS_System.TIMER[TNum]=time;
}
INT8U OSTimerStateGet(INT16U TNum) //����OS_TRUE ����ʱ��ɣ�  OS_FALSE��δ���
{
  if(OS_System.TIMER[TNum]==0) return OS_TRUE;
	else return OS_FALSE;
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------ϵͳ�ź����йغ���-------------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagPost(INT16U FNum) //���ͱ�־��
{
	OSSchedLock();
	INT16U i,j;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
    if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_Flag))//����OSFlag���ڵȴ��źű�־״̬
		{
			if(OSTCBTbl[i].FlagName==FNum)//�жϸõȴ�����ȴ���OSFlag�Ƿ��ǵ�ǰ׼�����͵�OSFlag
			{
				OSTCBTbl[i].TaskFlagBit&=~(1<<_BIT_Flag);//����źű�־״̬
				OS_System.FLAG[FNum][i]=OS_TRUE; //���ȴ�OSFlag�������ͱ�־��
				OSTCBTbl[i].TaskDelayMs=0;  //�ȴ���ʱ��0��ȡ���ȴ�OSFlag����ĵȴ���ʱ 
			}
		}
		if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_FlagGroup))//ȺOSFlag���ڵȴ��źű�־״̬
		{
			for(j=0;j<FLAG_GROUP_SIZE;j++)
			{
				if(OS_System.FlagGroupTable[j][FNum]==OS_TRUE)//�ж��Ƿ�ΪȺ��Ա
				{
					OS_System.FLAG[FNum][i]=OS_TRUE; //���ȴ�OSFlagȺ�������ͳ�Ա��־��		
				}
			}
		}
	}
	OSSchedUnlock();
}
INT8U OSFlagPend(INT16U FNum,INT32U timeout_ms) //�ȴ���־�� ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ
{
	OSSchedLock();
	OSTCBCur->FlagName=FNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_Flag);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //��ʱ���
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//�����ʱ��־λ
		return OS_FALSE;
	}
	else
	{
		if(OS_System.FLAG[FNum][OSTCBCur->TaskNum]==OS_TRUE)
		{
			OS_System.FLAG[FNum][OSTCBCur->TaskNum]=OS_FALSE;//���������ı�־��
			return OS_TRUE;
		}
		else return OS_FALSE;
	}
}
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum)  //��ӱ�־������־��Ⱥ
{
	OSSchedLock();
  OS_System.FlagGroupTable[FGNum][FNum]=OS_TRUE;
	OSSchedUnlock();
}
INT8U OSFlagGroupPend(INT16U FGNum,INT32U timeout_ms) //�ȴ���־��Ⱥ ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ
{
	OSSchedLock();
	OSTCBCur->FlagGroupName=FGNum;
	OS_System.FlagGroupNameInTask[OSTCBCur->TaskNum]=FGNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_FlagGroup);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);		
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //��ʱ���
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//�����ʱ��־λ
		return OS_FALSE;
	}
	else
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_FlagGroup);//���������FlagGroupȺ�ȴ���־
	  return OS_TRUE;
	}
}

void OSMutexPost(INT16U MNum) //���ͻ����� ����Ҫ���ڽ��mutex����״̬,���������mutex�ȴ��������¾���mutexȨ�ޣ�
{
	OSSchedLock();
	if(OSTCBCur->TaskNum==OS_System.MutexTaskNum[MNum])//�жϵ�ǰ�����Ƿ����PostȨ��
	{
		if((OSTCBCur->TaskFlagBit&(1<<_BIT_Mutex))==0)//�жϵ�ǰ�����Ƿ��ڷǵȴ�mutex״̬��
		{
			if(OS_System.MUTEX[MNum]==OS_TRUE) //�ж�mutex�Ƿ�Ϊtrue(æ)
			{
				OS_System.MutexTaskNum[MNum]=0;//����PostȨ��
				OS_System.MUTEX[MNum]=OS_FALSE;//��mutexΪOS_FALSE��ʾmutex����
			}
		}
	}
	OSSchedUnlock();
}
INT8U OSMutexPend(INT16U MNum,INT32U timeout_ms) //�ȴ������� ����ʱʱ��  ������OS_TRUE���ȴ�δ��ʱ  OS_FALSE���ȴ���ʱ
{
	OSSchedLock();
	OSTCBCur->MutexName=MNum;
	OS_System.MutexNameInTask[OSTCBCur->TaskNum]=MNum;
	if(OS_System.MutexTaskNum[MNum]==0) OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//������PostȨ��
	OSTCBCur->TaskFlagBit|=(1<<_BIT_Mutex);//��λmutex��־
	OSSchedUnlock();
	OS_delayMs(timeout_ms);
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_Mutex);//���mutex��־
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //��ʱ���
	{	
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//�����ʱ��־λ
		return OS_FALSE;
	}
	else
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//ȡ�����޵ȴ���־
		OS_System.MUTEX[MNum]=OS_TRUE;	
		OS_System.MutexTaskNum[MNum]=OSTCBCur->TaskNum;//������PostȨ��
		return OS_TRUE;
	}
}
void OSMboxPost(INT16U MNum,void* fp)  //�����ʼ�
{
	OSSchedLock();
	INT16U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
		if(OSTCBTbl[i].TaskFlagBit&(1<<_BIT_MBox))//�жϸ������Ƿ��ڵȴ�mbox״̬
		{
			if(OSTCBTbl[i].MBoxName==MNum)//�жϵȴ���mbox�Ƿ��ǵ�ǰ׼�����͵�MNum
			{
				OS_System.MBOX[MNum*OS_MAX_TASKS+i]=fp;//���ڵȴ��ʼ����������ʼ�
				OSTCBTbl[i].TaskDelayMs=0;//�ȴ���ʱ��0
			}
		}
	}
	OSSchedUnlock();
} 
void* OSMboxPend(INT16U MNum,INT32U timeout_ms) //�ȴ����� ����ʱʱ��  �����ط�(void*)0���ȴ�δ��ʱ  (void*)0���ȴ���ʱ
{
	OSSchedLock();
	INT32U* _mbox;	
	OSTCBCur->MBoxName=MNum;
	OSTCBCur->TaskFlagBit|=(1<<_BIT_MBox);
	OSSchedUnlock();
	OS_delayMs(timeout_ms);	
	if(OSTCBCur->TaskFlagBit&(1<<_BIT_TimeOut)) //��ʱ���
	{
		OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//�����ʱ��־λ
		return (void*)0;
	}
	else
	{
    _mbox=(INT32U*)OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum];
		OS_System.MBOX[MNum*OS_MAX_TASKS+OSTCBCur->TaskNum]=&Null;  //��ձ���������Ϊ'\0'
		return _mbox;
	}
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------ϵͳ�ź��������йغ���------------------------------------------------*/
#if (OS_SIGN_PP_ENABLE == 1)
void OSFlagGroupHandler(void)
{
	INT16U TaskCount,Count,NCount;
	for(TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++)
	{
		if(OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_FlagGroup))//�����FlagGroup�ȴ�������
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
				OSTCBTbl[TaskCount].TaskDelayMs=1;//׼���˳���ʱ������ȴ����е�״̬(�����ȼ�����)
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeOut);//�����ʱ��־
			}
		}
	}
}
void OSMutexHandler_Order(void)
{
  INT16U TaskCount;	
	for(TaskCount=OS_System.TaskNext; TaskCount < OS_MAX_TASKS; TaskCount++)//���NextTask�Ժ���ǰ�������Ƿ��еȴ�mutex
	{
		if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[TaskCount].StkPtr!=(STK32U*)0)&&\
			 (OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_Mutex))&&(OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]==OS_FALSE) )
			{
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_Mutex);//ȡ���ȴ�mutex��־
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeLess);//ȡ�����޵ȴ���־
				OSTCBTbl[TaskCount].TaskFlagBit&=~(1<<_BIT_TimeOut);//ȡ����ʱ��־
				OSTCBTbl[TaskCount].TaskDelayMs=1;//����׼������״̬(�ܷ�����������ȡ�������ȼ�)
				OS_System.MUTEX[OS_System.MutexNameInTask[TaskCount]]=OS_TRUE;//��ǰ�����mutex��æ	
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
					Count=TaskCount; //Count������¼��mutex�ȴ���������ȼ�����
				}
		}			
	}
	if(Count!=0)
	{
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_Mutex);//ȡ���ȴ�mutex��־
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_TimeLess);//ȡ�����޵ȴ���־
		OSTCBTbl[Count].TaskFlagBit&=~(1<<_BIT_TimeOut);//ȡ����ʱ��־
		OSTCBTbl[Count].TaskDelayMs=1;//����׼������״̬(�ܷ�����������ȡ�������ȼ�)
		OS_System.MUTEX[OS_System.MutexNameInTask[Count]]=OS_TRUE;//��ǰ�����mutex��æ
	}
}
#endif
/*-------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------���ɼ��ú��� PageUp-----------------------------------------*/


void  OSSchedLock (void)   //�����л�����(�û��ɵ���) �����󲻻��л�����cpu��һֱ���е�ǰ����ֱ������
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
void  OSSchedUnlock (void)   //���������OSSchedLock��OSSchedUnlock����ɶԳ��֣�(�û��ɵ���)
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

void OS_Timer_Handler(void) //�����л����ĺ���
{
	 OS_INT_ENTER();
	 INT16U TaskCount,Count;
	 for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) 
	 {   
	 	 if(( OSTCBTbl[TaskCount].TaskDelayMs >1 )&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0)) 
		 {
			 OSTCBTbl[TaskCount].TaskDelayMs--;
			 if(OSTCBTbl[TaskCount].TaskDelayMs==1) OSTCBTbl[TaskCount].TaskFlagBit|=(1<<_BIT_TimeOut);//�����ʱ���
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
     OS_System.TaskTimeSliceCnt=1;//��1��ʼ����(0����������)		 
	 }
#endif		 
	 if((OS_System.IntNesting == 0) && (OS_System.LockNesting == 0))
	 {		 
		 OSPendSVPulse(); 
#if (OS_GET_SET_SW_ENABLE == 1)			 
		 if(OS_System.TaskSwitchBackNum>0) //�Ƿ�����ת�������
		 {
			 OS_System.TaskNext=OS_System.TaskSwitchBackNum;	//����˳�����жϵ�
			 OSTCBNext = &OSTCBTbl[OS_System.TaskSwitchBackNum]; //������תǰ������
			 OS_System.TaskSwitchBackNum=0;                      //��0
			 OS_System.TaskNextRunFlag=OS_TRUE; //˳�����б�־��TRUE			
		 }	
		 else
#endif			 
		 {	
#if (OS_SIGN_PP_ENABLE == 1)     					
       OSFlagGroupHandler();//OSFlagGroup�й�
#endif
			 if(OS_System.RuningMode==0)//����ģʽ:Order	
		   {		 
				 /*******************************************������˳������*******************************************/
#if (OS_SIGN_PP_ENABLE == 1) 				 
				OSMutexHandler_Order();//OSMutex�йغ���
#endif				 
				for(TaskCount = 1; TaskCount < OS_MAX_TASKS; TaskCount++) //��������������ʱʱ���Ƿ��еĵ�ʱ
				{ 
					if(OSTCBTbl[TaskCount].TaskState==TASK_RUNNING) //����������̬������
					{
						if( OSTCBTbl[TaskCount].TaskDelayMs ==1 )//�����ʱʱ���Ƿ�ʱ
						 {
							 OSTCBCur=&OSTCBTbl[TaskCount]; 	        //��ת����ʱ��ɵ�����
							 OSTCBTbl[TaskCount].TaskDelayMs=0;	  //��ʱ��0
							 OS_System.TaskNextRunFlag=OS_FALSE;    //˳�����б�־��FALSE	
							 break;
						 }
					 }	 
				}	
        if(TaskCount==OS_MAX_TASKS) OS_System.TaskNextRunFlag=OS_TRUE;	//˳�����б�־��TRUE			 
			 	if(OS_System.TaskNextRunFlag==OS_TRUE)	//�ж��Ƿ���԰�����˳������
				{
					OSTCBCur = OSTCBNext;
					OS_System.TaskNext++;						
					while((OSTCBTbl[OS_System.TaskNext].StkPtr == (STK32U*)0)||(OSTCBTbl[OS_System.TaskNext].TaskState!=TASK_RUNNING)\
							 ||OSTCBTbl[OS_System.TaskNext].TaskDelayMs > 1) //�����Ƿ�״̬����
					{ 
						OS_System.TaskNext++;
						if(OS_System.TaskNext == OS_MAX_TASKS)  OS_System.TaskNext = 0;
					}
					OSTCBNext = &OSTCBTbl[OS_System.TaskNext];	//����Next����
	      }	
			 }
			 else				
				{		
					/*******************************************����ʽ���ȼ�******************************************/	
#if (OS_SIGN_PP_ENABLE == 1)     					
          OSMutexHandler_Prio();//OSMutex�йغ���
#endif					
					for(Count=0,TaskCount=1; TaskCount < OS_MAX_TASKS; TaskCount++) //�����������еĳ���������ȼ�����
					{ 
						if(OSTCBTbl[Count].TaskPrio<OSTCBTbl[TaskCount].TaskPrio) 
						{
							if((OSTCBTbl[TaskCount].TaskState==TASK_RUNNING)&&(OSTCBTbl[OS_System.TaskNext].StkPtr!=(STK32U*)0)&&\
								 (OSTCBTbl[TaskCount].TaskDelayMs <= 1)&&((OSTCBTbl[TaskCount].TaskFlagBit&(1<<_BIT_TimeLess))==0) )
								{
									Count=TaskCount; //Count������¼������ȼ�
								}
						}				
					}
					OSTCBCur = OSTCBNext;	
					OSTCBNext=&OSTCBTbl[Count];//����������ȼ�����
					OSTCBTbl[Count].TaskDelayMs=0;	  //��ʱ��0						
				}		
		 }				
	 }				
   OS_INT_EXIT();  
}


void delay_us(INT32U nus) //΢����ʱ���� (�û��ɵ���)
{   	
	OSTIMER_INT_OFF();//�ر�ϵͳ��ʱ���ж�
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
  OSTIMER_INT_ON(); 	//��ϵͳ��ʱ���ж�
}
void delay_ms(INT32U nms)  //������ʱ���� (�û��ɵ���)
{     
	OSTCBCur->TaskDelayMs=(nms*1000/OS_CLOCK_TIME)+1;
  while(OSTCBCur->TaskDelayMs>1);
	OSTCBCur->TaskDelayMs=0;
}
void OS_delayMs(volatile INT32U nms) //OS��ʱ����(�û��ɵ���)
{
	while((OS_System.IntNesting != 0) && (OS_System.LockNesting != 0));
	OS_INT_ENTER(); 
	if(nms==0){nms=2;//��һ����1���ֵ����
	OSTCBCur->TaskFlagBit|=(1<<_BIT_TimeLess);} //��λ���޵ȴ���־
  else OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeLess);//ȡ�����޵ȴ���־	
	OSTCBCur->TaskFlagBit&=~(1<<_BIT_TimeOut);//ȡ����ʱ��ɱ�־
	OSTCBCur->TaskDelayMs=(nms*1000/OS_CLOCK_TIME)+1;
  OSPendSVPulse(); 
	OS_INT_EXIT(); 
	while(OSTCBCur->TaskDelayMs);
}
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec)//��ʱ������ʱ��<=49��17ʱ2��47��
{
	INT32U time;
	if(day>49) day=49;
	time=(day*24*3600+hour*3600+min*60+sec)*1000;
	OS_delayMs(time);
}
/*--------------------------------------------���ɼ��Ժ��� PageDown---------------------------------------------*/

/**************************************************������ˮ*****************************************************/
