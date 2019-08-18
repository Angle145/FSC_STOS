#include "os.h"

//TCB����
OS_TCB OSTCBTbl[OS_MAX_TASKS]; 
OS_TCB *OSTCBCur; 
OS_TCB *OSTCBNext;

//ϵͳ����
SYSTEM System;

//��������ջ(������ʵ��)
OS_STK TASK_IDLE_STK[STK_IDLE_SIZE];

/*��ʱ����ʹ��
*CTRL�Ĵ�����bit0-��ʱ��ʹ�� bit1-�ж�ʹ�� bit2-ʱ��Դѡ��(=1ϵͳ��Ƶ��=0ϵͳ��Ƶ/8)  bit16-�������λ*/
void OSSysTickInit(void)//����OS�ļ�ʱ����
{ 
	//Systick��ʱ����ʹ��(ʹ��������ʱ��ʱ�����޸�Ϊ������ʱ��)
	char *Systick_priority = (char *)0xe000ed23;       //Systick�ж����ȼ��Ĵ���
	SysTick->LOAD  = (SystemCoreClock/8/1000000)* 1000; //Systick��ʱ����װ�ؼ���ֵ:1ms(�̶�ֵ)
	*Systick_priority = 0x00;                           //Systick��ʱ���ж����ȼ�
	SysTick->VAL   = 0;                                 //Systick��ʱ����������0
	SysTick->CTRL = 0x3;//Systick�򿪲�ʹ���ж���ʹ���ⲿ����ʱ��,8��Ƶ  72MHz/8=9MHz  ����9000��=1ms  ����9��=1us
}
void SysTick_Handler(void) //��ʱ���жϺ���(��ֲʱ��Ҫע�͵�ԭ�����е�SysTick_Handler�жϺ���)
{
	INT8U i;
	for(i=0;i<OS_MAX_TASKS;i++)
	{
	  if(OSTCBTbl[i].DelayTime>1) OSTCBTbl[i].DelayTime--;
	}
	 System.TimeMS++;  //ϵͳʱ�ӽ����ۼ�
	 if((--System.TaskTimeSlice) == 0)  
		 {
			 System.TaskTimeSlice = TASK_TIME_SLICE;//����ʱ��Ƭ��ֵ
			 OSSchedSwitch();	//���Ȳ��л�����					
		 }  
}

/*PendSV��ʹ���������жϺ���(�ж��������񻷾��л�����)
*��ֲʱ��Ҫע��ԭ�����е�PendSV_Handler�жϺ���*/
void OSPendSVInit(void)//PendSV��ʹ��
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //���ȼ��Ĵ�����ַ
	*NVIC_SYSPRI14=0xFF;//����PendSV�ж����ȼ���� 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //�жϿ��ƼĴ�����ַ
void OSPendSVPulse(void)//����PendSV�ж�
{
	OS_CORE_ENTER();
  *NVIC_INT_CTRL=0x10000000; 
	OS_CORE_EXIT();	
}

/*
*�˵���������򵥵ĵ�������ʵ���������������0-����nѭ�����С�
*�˷�ʽ��ɵ�ϣ�ʧȥ��OS�����Ч��֯�������ԣ�������������ơ�
 ����1����TCB�������������������ʱ�ۼƱ�����ֻҪ�ñ���ֵ
			 ��Ϊ0(����ʱ��������ֵ)���ñ����ڶ�ʱ���ж��в��ϵݼ���ͬʱ��
			 ��ü�ʱ�Ƿ�ʱ(=0)��δ��ʱ���������ͣ����������в����Լ�
			 ������񣬰�����Ȩ������ʱ�̻�û����ʱ�������絽ʱ��֪ͨ����
			 �������л������������С�
 ����2����TCB��������������������ȼ������������������Ȱ�����Ȩ��
       ��������ȼ��������С�
 ����3��������ʵ�ָ����ȼ������������У���ͬ���ȼ���ͬ��������ȼ���
       �����С�
 ����4����TCB�����������������ʱ��Ƭ�������������м����������Ƶ����
       �ȣ���������ʱ��Ƭ������
*��������OS�ĺ��ģ���ͬ�ĵ��ȷ�ʽ���OS��ͬ����ɫ��
*������Ҫ��1.�ܿ����ҳ��¸�Ҫ���е�����(������Ҫ�����㷨֧��);
            2.���ȶ���֯�������������Ч���У���������CPU����
              ����ת�����
            3.���ж������Ƿ������ʱ��������(����ͬ����ʱ)�����ܴ���
              ����ʱ����cpu��Ч���С�
*�����������а취��OSTCBNextָ��ָ���ĸ������TCB�����ַ�����л�������
        �ͻ������ĸ�����
*/
void OSScheduler(void)  //���������(OS����)
{
/*-----------------------------------˳������ģʽ---------------------------------------------*/
//	if(System.OSTaskNext<OS_MAX_TASKS-1) System.OSTaskNext++; //��һ����ֱ���ֵ��¸�����
//	else System.OSTaskNext = 0;//�ӿ�������ʼѭ��
//	while((OSTCBTbl[System.OSTaskNext].StkPtr == (OS_STK*)0)&&     //��������
//		    (OSTCBTbl[System.OSTaskNext].State!=TASK_STATE_RUNNING)) //������״̬���
//	{ 
//		System.OSTaskNext++;//��������
//		if(System.OSTaskNext >= OS_MAX_TASKS) //����ѭ������
//			{
//				System.OSTaskNext = 0;//�ӿ�������ʼѭ��
//			}
//	}
//  OSTCBNext = &OSTCBTbl[System.OSTaskNext];//����OSTCBNext	
/*-----------------------------------------------------------------------------------------*/		
	
/*-----------------------------------���ȼ�����ģʽ------------------------------------------*/	
	INT8U i,HighestPrio;
	for(HighestPrio=0,i=1;i<OS_MAX_TASKS;i++)//����������ȼ�����
	{
		if((OSTCBTbl[i].StkPtr != (OS_STK*)0)&&     //�ǿ�������
			 (OSTCBTbl[i].DelayTime==1)&&             //��ʱ��ɼ��
			 (OSTCBTbl[i].State==TASK_STATE_RUNNING)) //����״̬���
		{
			if(HighestPrio<OSTCBTbl[i].Prio)
			{
				HighestPrio=OSTCBTbl[i].Prio;
			}
		}
	}
	if(OSTCBTbl[HighestPrio].DelayTime==1) OSTCBTbl[HighestPrio].DelayTime=0;
	OSTCBNext = &OSTCBTbl[HighestPrio];//����OSTCBNext	
/*-----------------------------------------------------------------------------------------*/	
}
void OSSchedSwitch(void)//������Ȳ��л�����
{
	if(System.OSLockNesting == 0) //�����л�������⣬>0˵��������==0����
	{
		OSScheduler();   //�ȵ��ȣ�OSTCBNext��ȡ����Ҫ���е�����
		if(OSTCBNext!=OSTCBCur) OSPendSVPulse(); //���񻷾��л�����OSTCBNext�л���OSTCBCur����	
	}
}

//�����ջ��ʹ��
//ֻҪ����PendSV�ж�,��ת�жϴ�����ǰ xPSR,PC,LR,R12,R3-R0���Զ����浽ϵͳ������ջ��(�˲�����PendSV�ж�Ӳ������)��
//�������ĸ�ջȡ���ڵ�ǰ��SP���ͣ�����MSP�򱣴浽ϵͳջ������PSP�򱣴浽����ջ����OS�Ǳ���������ջ����R11-R4��Ҫ�ֶ����浽����ջ��
//��ջ˳��ջ��->ջβ xPSR,PC,LR,R12,R3-R0,R4-R11��16��(SP(R13)������TCB�׸���Ա������)��
OS_STK* OSTaskStkInit(void* task,OS_STK *topstkptr)
{
    OS_STK *stk;
    stk = topstkptr;
    *(stk)    = (INT32U)0x01000000L;//xPSR ����״̬�Ĵ�����xPSR Tλ(��24λ)������1,�����һ��ִ������ʱ����Fault�ж�                                                     
    *(--stk)  = (INT32U)task;       //PC   ��ʹ��ʱָ���������׵�ַ�������л�ʱ������ָ���������м�ĳ��ַ��            
    *(--stk)  = (INT32U)0xFFFFFFFEL;//LR   �����Ÿ�����ת�ķ������ӵ�ַ,��ʹ��Ϊ���4λΪE����һ���Ƿ�ֵ����ҪĿ���ǲ���ʹ��R14���������ǲ��ܷ��ص�             
    *(--stk)  = (INT32U)0x12121212L;//R12  ͨ�üĴ���(12121212Ϊ��Чֵ��ֻ�𵽱��ΪR12�����ã����޸�Ϊ0,����ͬ)                                                       
    *(--stk)  = (INT32U)0x03030303L;//R3   ͨ�üĴ���                                                       
    *(--stk)  = (INT32U)0x02020202L;//R2   ͨ�üĴ���                                                        
    *(--stk)  = (INT32U)0x01010101L;//R1   ͨ�üĴ���                                                        
    *(--stk)  = (INT32U)0;          //R0   ͨ�üĴ���            
    *(--stk)  = (INT32U)0x04040404L;//R4   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x05050505L;//R5   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x06060606L;//R6   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x07070707L;//R7   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x08080808L;//R8   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x09090909L;//R9   ͨ�üĴ���             
    *(--stk)  = (INT32U)0x10101010L;//R10  ͨ�üĴ���             
    *(--stk)  = (INT32U)0x11111111L;//R11  ͨ�üĴ���
    //����R12-R11�Ĺ��̿��Լ�Ϊ��  
    //stk-=13;	
    return stk;
}

void OSInit(void) //ϵͳ��ʹ��
{
    INT8U i;
    for(i = 0; i < OS_MAX_TASKS; i++) {
        OSTCBTbl[i].StkPtr = (OS_STK*)0;
			  OSTCBTbl[i].DelayTime=1;//����Ϊ1 (��ƣ�Ϊ0ʱΪ������)
        OSTCBTbl[i].State = TASK_STATE_CREATING;
    }
		System.TimeMS=0;
		System.TaskTimeSlice = TASK_TIME_SLICE;
		System.OSRunning=OS_TRUE;
    OSInitTaskIdle();
    OSTCBCur = &OSTCBTbl[0];
    OSTCBNext = &OSTCBTbl[0];		
}
void OSTaskCreate(void* task,OS_STK *topstkptr,INT8U prio)//���񴴽�����
{
    OS_STK * stkptr;
    INT8U i = 1;
    while(OSTCBTbl[i].StkPtr != (OS_STK*)0) { //���ҿ�����
        i++;
    }
    stkptr = OSTaskStkInit(task,topstkptr);
		OSTCBTbl[i].StkPtr=stkptr;
		OSTCBTbl[i].Prio=prio;
    OSTCBTbl[i].State = TASK_STATE_RUNNING;//����ʱΪ����̬(Ҳ��Ϊ����̬TASK_STATE_CREATING,��������⵽��������ʱ��תΪ����̬)
}
void OSInitTaskIdle(void)//�������񴴽�����
{
    OSTCBTbl[0].StkPtr = OSTaskStkInit(OS_TaskIdle,(OS_STK*)&TASK_IDLE_STK[STK_IDLE_SIZE - 1]);
	  OSTCBTbl[0].Prio=0;
    OSTCBTbl[0].State = TASK_STATE_RUNNING;//����������̬TASK_STATE_RUNNING
}
void OS_TaskIdle(void) //�����������ڷ�ֹ�û����񶼴�����ͣ��Ϊ����Ϊ0ʱ����
{
	while(1) 
	{
		
	}
}

void OSSchedLock(void) //�����л�����  
{
	OS_CORE_ENTER();
	if(System.OSRunning == OS_TRUE)  
		{                                                                       
			if (System.OSLockNesting < 255u) System.OSLockNesting++;	     
	  }
	OS_CORE_EXIT();
}


void OSSchedUnlock(void) //�����л����� 
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
    return System.TimeMS;  //ϵͳʱ�ӽ����ۼƱ���
}


//uS��ʱ

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
//mS��ʱ
void delay_ms(INT32U ms)
{   
  delay_us(ms*1000);			
}

void OS_delayMs(INT32U ms)  //��������CPU����Ȩ,ms=0 ���÷�����ms>0����һ��ʱ���ָ�(��ʱ)
{
	 if(ms>0) ms=ms+1;
   OSTCBCur->DelayTime=ms;
	 OSSchedSwitch();
	 while(OSTCBCur->DelayTime);
}
