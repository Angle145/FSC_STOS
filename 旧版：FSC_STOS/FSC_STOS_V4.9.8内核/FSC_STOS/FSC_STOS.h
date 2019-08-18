/*--------------------------------------------�汾: V4.9.8--------------------------------------------*/
#ifndef _FSC_STOS_H_
#define _FSC_STOS_H_

/*****************ͷ�ļ�ѡ��*****************/
//#include "stm32l0xx.h"  //stm32l0  STD��
//#include "stm32l1xx.h"  //stm32l1  STD��

//#include "stm32f0xx_hal.h"//stm32f0  HAL��
//#include "stm32f0xx.h"  //stm32f0  STD��

//#include "stm32f1xx_hal.h"//stm32f1  HAL��
#include "stm32f10x.h"  //stm32f1  STD��

//#include "stm32f4xx_hal.h"//stm32f4  HAL��
//#include "stm32f4xx.h"  //stm32f4  STD��
//#include "stm32f7xx_hal.h"//stm32f7  HAL��
/********************************************/
      
/****************************************�û����Զ���***********************************************************/
#define OS_RINNING_MODE          0        //OS����ģʽ��0-ʱ��Ƭ��ѯ  1-��ռʽ���ȼ�  2-ʱ��Ƭ+���ȼ�
#define OS_MAX_TASKS             16       //������=�û�������+2  ������:3-65535  �û�����ʵ����Ҫ�����������޸�
#define OS_MEMORYPOOL_SIZE       4*1024*1 //�ڴ�ش�С,��λ��Byte
#define OS_UCGUI_SUPPORT         0        //�������ˢuCGUI֧��  0-��֧��  1-֧�� (���鵥����ʹ��GUI������Ϊ0)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TIMER_SIZE               4        //ϵͳ���ⶨʱ������ (������Ҫ���ã�1-65535������ͬ)
#define FLAG_SIZE                4        //��־����            
#define FLAG_GROUP_SIZE          4        //��־Ⱥ����         
#define MUTEX_SIZE               4        //��������           
#define MBOX_SIZE                4        //��������  
#define MBQ_SIZE                 4        //������л�������
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CMD_ALL_ENABLE        1        //����ȫ��ϵͳָ��(0-���ã�1-������,��ͬ)
#define OS_CMD_NSY_ENABLE        1        //����ϵͳ�޹ص�ָ��
#define OS_CMD_DISP_SYS_ENABLE   1        //����ϵͳ״̬��Ϣָ����ʾ(�ر���ʾ�ܽ�ʡ�����ڴ�)
#define OS_CMD_DISP_NSY_ENABLE   1        //����ϵͳ״̬��Ϣ�����޹ص�ָ����ʾ
#define OS_TIMECOUNTSYS_ENABLE   1        //����ϵͳʱ��ͳ��ϵͳ(���ÿ����ʱ��Ƭģʽ�µ��ں��л��ٶ�)
#define OS_GET_SET_SW_ENABLE     1        //�����������á���ȡ����ת
#define OS_SIGN_PP_ENABLE        1        //�����ź���(������ں��л��ٶ�)
#define OS_TIM_SYS_ENABLE        1        //����ϵͳ���ⶨʱ��(������ں��л��ٶ�)
#define OS_SYSTIME_ENABLE        0        //����ϵͳʱ��RTC
#define OS_REMOTE_ENABLE         0        //���ò����ú���
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TASK_NAME_LEN            32       //������       �ַ���󳤶�(������������32���ַ����޸�)
#define OS_CMD_STR_LEN           64       //ϵͳָ��     �ַ���󳤶�
#define OS_CMD_NUM_LEN           8        //ϵͳָ���ַ�������������ָ���("prio=89//"Ϊһ������)
#define OS_FAULT_BACKUP_DATA_LEN 1        //ϵͳ�������ݱ��ݳ���
/*----------------------------ϵͳͳ�Ʋ���--------------------------------*/
#define OS_StartUpOccRateDlyMs   1000     //ϵͳ��������ʱͳ����Ϣʱ�䣬��λ��ms (��ʱԽ��ͳ����ϢԽ��ȷ)     
/*----------------------------ϵͳ�������--------------------------------*/			
#define OS_TIMESLICE_IDLE        1        //��������ʱ����Ƭ����λ:΢�� ms
#define OS_TIMESLICE_MANAGER     1        //�������������ʱ����Ƭ,��λ:΢�� ms

#define TaskIDLE_StkSize         64       //���������ջ��С
#define TaskManager_StkSize      160      //��������������ջ��С

#define TaskIDLE_Prio            0        //�����������ȼ�(�����޸�,���ڴ���Ϊ�����û��˽������������ȼ�)
#define TaskManager_Prio         1        //����������������ȼ�(����ָ���ܿ��ٷ�Ӧ��������ȼ�)

#define TaskManager_DelayMs      0        //���������������ʱ(ƽʱ�����У�ֻ�н��յ�cmdʱ��������һ��)
/*----------------------------���ϼ��ϵͳ--------------------------------*/
#define OS_FAULT_ENABLE          0        //���ù��ϼ��ϵͳ(��Ҫ��stm32fxxx_it.c��ע�͵�HardFault_Handler()�жϺ���)
#define OS_FAULT_DISP_ENABLE     0        //���ù��ϼ��ϵͳ��ʾ
#define OS_Reset_Delay_Time      10       //ϵͳ������ʱ,��λ��s
#define OS_Shutdown_Delay_Time   10       //ϵͳ�ر���ʱ,��λ��s
#define OS_EndlessLoops_Times    1000     //�������иô���������Ϊ������������ѭ���޷��л���������������,��λ������
#define OSFAULT_Reset_Enable     1        //�Ƿ�������Ϻ��Զ�����,0-������ 1-����
/***************************************************************************************************************/
/*----------------------------�ڴ����ϵͳ--------------------------------*/
#define OS_MEM_IDLE_MERGE_ENABLE 0        //����ϵͳ����ʱ�����ڴ���Ƭ���� 0-������ 1-���� (����ʵʱ�����ⲿ��Ϣʱ��ر�)
#define OS_MEM_IDLE_TIME         10       //�����������ʱʣ��ʱ����ڸ�ֵʱ��Ϊϵͳ���� 
/*+++++++++++++++++++++++++++++++++++++++++++++״̬������+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*---------------------����״̬(�û�����)----------------------*/
//State->LSB:
typedef enum{
  TASK_CREATING,                          //����̬
  TASK_RUNNING,                           //����̬
  TASK_PAUSING,                           //��̬ͣ
  TASK_BACKRUNNING,                       //��̨����̬
  TASK_DELETING,                          //ɾ��̬
}TASK_STATE;
//State->MSB:
typedef enum{
  TASK_UNLOCK,                            //����̬
  TASK_LOCK,                              //����̬
}TASK_LOCK_STATE;
/*--------�ź���Pend������һ���������ȴ�����(�û�����)---------*/
typedef enum{
  OSFlag_BPN=0,                           //�����ȴ����ź���(ֻ��ӦPend֮����ź�)
  OSFlag_BPO,                             //�����ȴ������ź���(����Ӧ��Pend֮ǰPost�����ź�)
  OSFlag_BPC,                             //�����ȴ���Ӧ�ۼ��ź���(�ۼ�Post���д�����Pend��ȡһ�Σ��ۼƽ���1�Σ�������ͬ����Ӧȫ��Post�ź�)
  OSFlag_NBPC,                            //�������ȴ���Ӧ�ۼ��ź���(�ۼ�Post���д�����Pend��ȡһ�Σ��ۼƽ���1�Σ�������ͬ����Ӧȫ��Post�ź�)(������ʱ��ʱʱ����Ч)
  OSFlag_NBPN,                            //�������ȴ����ź���(ֻ��ӦPend֮����ź�)(������ʱ��ʱʱ����Ч)
}OSFlagPendMode;
typedef enum{
  OSFGroup_BPN=10,                        //�����ȴ����ź���(Pend��ȡ���µ�FlagGroupȫ����������ź�)
  OSFGroup_NBPN,                          //�������ȴ����ź���(Pend��ȡ���µ�FlagGroupȫ����������ź�)(������ʱ��ʱʱ����Ч)
}OSFGroupPendMode;
typedef enum{
  OSMutex_BPN=20,                         //�����ȴ��ź���
  OSMutex_NBPN,                           //�������ȴ��ź���(������ʱ��ʱʱ����Ч)
}OSMutexPendMode;
typedef enum{
  OSMBox_BPN=30,                          //�����ȴ����ź���(��ȡPend֮��Post���ʼ�)
  OSMBox_BPQ,                             //�����ȴ���Ӧ�����ź���(����Post���ʼ������б���������Pend��ȡʱ��ȡ�ȷ��͵ģ��ȷ��ȶ����󷢺��)
  OSMBox_NBPN,                            //�������ȴ���Ӧ���ź���(�Ƕ���)(��ȡPend֮��Post���ʼ�)(������ʱ��ʱʱ����Ч)
  OSMBox_NBPQ,                            //�������ȴ���Ӧ�����ź���(����Post���ʼ������б���������Pend��ȡʱ��ȡ�ȷ��͵ģ��ȷ��ȶ����󷢺��)(������ʱ��ʱʱ����Ч)
}OSMBoxPendMode;
/*-----------------�ź���ϵͳ��־λ(�û�������)------------------*/												     
#define _BIT_TimeOut          (1<<0)      //��־λ0����ʱʱ�䳬ʱ
#define _BIT_TimeLess         (1<<1)      //��־λ1�����޵ȴ�
#define _BIT_Flag             (1<<2)      //��־λ2��OSFlag��־���ȴ�
#define _BIT_FlagGroup        (1<<3)      //��־λ3��OSFlagGroup��־��Ⱥ�ȴ�
#define _BIT_Mutex            (1<<4)      //��־λ4��OSMutex�������ȴ�  
#define _BIT_MBox             (1<<5)      //��־λ5��OSMBox�ʼ��ȴ�
/*-------------------ϵͳ�߼�״̬(�û�����)--------------------*/	
typedef enum
{
 OS_FALSE,                     //��
 OS_TRUE,                      //��
}OS_BOOL;
/*-------------------------------------------------------------------------------------------------------------*/
/***************************************************************************************************************/
#define  FSC_STOS   //����Ƕ��FSC_STOS�ı�ʶ(�������ļ����ж�FSC_STOS�Ƿ��Ѷ������ж��Ƿ�Ƕ����FSC_STOS)
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

/*----------�����������Ͷ���-----------*/
typedef char  INT8;            
typedef short INT16;           
typedef int   INT32; 
typedef long  INT64;
typedef unsigned char  INT8U;            
typedef unsigned short INT16U;           
typedef unsigned int   INT32U; 
typedef unsigned long  INT64U;
/*-----------------------------------*/
/*----------ϵͳ�������Ͷ���-----------*/
typedef INT32U   STK32U;
typedef INT8     OSMEM;
/*-----------------------------------*/
#define  OSSTK __align(8) STK32U

#if (OS_CMD_ALL_ENABLE == 1) 
typedef struct
{ 
	INT8U  RuningMode;
	INT8U  RunTime[7];
	INT32U Clock;
	INT8U  ShutDownTime[6];
	INT16U CPUOccRate[3];
	INT32U Memory[3];
	INT16U TaskMsg[OS_MAX_TASKS][8];
	INT16U TaskStr[OS_MAX_TASKS][TASK_NAME_LEN/2];
}OS_Information_Backup_Struct;
typedef struct
{
	INT8U  Enter_Flag;
	INT8U  RXOK_Flag;
	INT8U  RX_COUNT;
	char   RX_BUFF[OS_CMD_STR_LEN];	
	INT32  Rx_Dig_Buff[OS_CMD_NUM_LEN];
}OS_CMD;
extern OS_CMD OS_Cmd; //ϵͳָ�����ݽ���(�û�������)  
/*--------------------------------------------OSCMDprintf-----------------------------------------------------*/
#define OSCMDprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OSCMDר�õĴ�ӡ�������
/*---------------------------------------------OSprintf-------------------------------------------------------*/ 
#define OSprintf(fmt, ...) { OSSchedLock(); if(OS_Cmd.Enter_Flag==OS_FALSE) printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS�ṩ���û���ӡ�������
#else
#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS�ṩ���û���ӡ�������
#endif
/*---------------------------------------------OSprintf-------------------------------------------------------*/
typedef struct
{
	INT8U  Error;
	INT8U  FirstEnter;
	INT8U  TaskCtxSw;
	INT8U  RunOneRound;
	INT8U  Running;
	INT8U  RuningMode;
	INT8U  RunDriverCode;  
	INT16U RunTime_ms;
#if (OS_TIMECOUNTSYS_ENABLE == 1)	
	INT8U  RunTime_second;//ʱ������Ϊ�˶������INT8U 
	INT8U  RunTime_minute;
	INT8U  RunTime_hour;
	INT8U  RunTime_day;
	INT8U  RunTime_mouth;
	INT32U RunTime_year;//ʱ������Ϊ�˶������INT16U,���һ����Ϊ��������
  INT8U  ShutDownTime_second;//ʱ������Ϊ�˶������INT8U 
	INT8U  ShutDownTime_minute;
	INT8U  ShutDownTime_hour;
	INT8U  ShutDownTime_day;
	INT8U  ShutDownTime_mouth;
	INT32U ShutDownTime_year;//ʱ������Ϊ�˶������INT16U,���һ����Ϊ��������
#endif	
	INT16U RunLastTask;
	INT16U TimeSliceCnt;
	INT32U ClockCnt;
	INT16U IntNesting; 
	INT16U LockNesting; 
	INT32U CtxSwCtr; 
	INT16U TaskHighestRunLast[OS_MAX_TASKS];
	INT16U TaskHighestPrioLast;
	INT32U TaskContinuRunTimeCnt;
	INT16U TaskNext;
	INT8U  TaskNextRunFlag;
	INT8U  TaskDelayTimeOutFlag;
#if (OS_GET_SET_SW_ENABLE == 1)		
	INT16U TaskSwitchBackNum;  
#endif	
	INT16U TaskCPUOccRateMax;
	INT16U UserTaskCPUOccRateMax;
	INT32U TaskTimeSliceCnt;
#if (OS_TIM_SYS_ENABLE == 1)
  INT16U TIMER_APPLY_COUNT;
	INT32U TIMER[TIMER_SIZE]; 
	INT32U TIMER_RELOAD[TIMER_SIZE]; 
	INT8U  TIMER_BitFreeFlag[TIMER_SIZE/8+1];
#endif		
#if (OS_SIGN_PP_ENABLE == 1)	
  INT8U   FLAG_BitFreeFlag[FLAG_SIZE/8+1];
	INT8U   FLAGGROUP_BitFreeFlag[FLAG_GROUP_SIZE/8+1];
	INT8U   MUTEX_BitFreeFlag[MUTEX_SIZE/8+1];
	INT8U   MBOX_BitFreeFlag[MBOX_SIZE/8+1];
	INT16U  FLAG_APPLY_COUNT;
	INT16U  FLAGGROUP_APPLY_COUNT;
	INT16U  MUTEX_APPLY_COUNT;
	INT16U  MBOX_APPLY_COUNT;
	INT32U  POST_FLAG_COUNT[FLAG_SIZE];
	INT32U  POST_FGROUP_COUNT[FLAG_GROUP_SIZE];
	INT32U  POST_MBOX_COUNT[MBOX_SIZE];
	INT16U  POST_MBQ_COUNT[MBOX_SIZE];
	INT16U  FLAG[FLAG_SIZE][OS_MAX_TASKS];//��ͬ������Եȴ���ͬ��Flag
	INT16U  FLAGPendType[FLAG_SIZE][OS_MAX_TASKS];//OSFlag�ȴ�����
	INT16U  GROUPRXFLAG[FLAG_GROUP_SIZE][OS_MAX_TASKS];//��־��Ⱥȫ�����ձ�־
	INT8U   FlagGroupTable[FLAG_GROUP_SIZE][FLAG_SIZE];
	INT16U  FlagGroupNameInTask[OS_MAX_TASKS];
	INT8U   ReturnFlagGroupTable[FLAG_SIZE];//��ѯFlagGroup��Ա����Ļ�������
  INT16U  MUTEX[MUTEX_SIZE]; 	
	INT16U  MutexTaskNum[MUTEX_SIZE];
	INT16U  MutexPendType[MUTEX_SIZE][OS_MAX_TASKS];//OSMutex�ȴ�����
	INT16U  MutexNameInTask[MUTEX_SIZE];
	INT16U  MBOXPendType[MBOX_SIZE][OS_MAX_TASKS];
	INT32U *MBOX[OS_MAX_TASKS*MBOX_SIZE];
	INT32U *MBQueue[MBQ_SIZE*MBOX_SIZE];
#endif	
  INT8U  MemFragFlag;
}volatile OS_SYSTEM;
extern OS_SYSTEM OS_System;   

typedef struct 
{
	STK32U      *StkPtr;//����ջָ������ǵ�һ�� 
  STK32U      *StkAdd;	
	INT32U      StkSize;
#if (OS_CMD_ALL_ENABLE == 1) 	
	INT32U      StkFree;
	INT32U      StkUsed;
	INT32U      StkUsedMax;
	INT32U      TaskCPUOccRateCnt;	
#endif	
#if (OS_CMD_ALL_ENABLE == 1)||(OS_FAULT_ENABLE == 1)	
  char        TaskStr[TASK_NAME_LEN];	
#endif		
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U      FlagName;
	INT16U      FlagGroupName;
	INT16U      MutexName;
	INT16U      MBoxName;
#endif	      
  INT32U      TaskAdd;	
	INT32U      TaskDelayMs;
  INT32U      TaskTimeSlice;	
	INT16U      TaskNum; 
	INT16U      TaskPrio; 
	INT16U      TaskPrioBackup; 
	TASK_STATE  TaskState;
	INT8U       TaskFlagBit;//bit0-��ʱ bit1-���޵ȴ� bit2-�ȴ�OSFlag bit3-�ȴ�OSFlagGroup bit4-�ȴ�OSMutex bit5-�ȴ�MBox
}volatile OS_TCB; 
extern OS_TCB OSTCBTbl[OS_MAX_TASKS];  
extern OS_TCB *OSTCBRun; 
extern OS_TCB *OSTCBCur; 
extern OS_TCB *OSTCBNext;
/*
��������������ƿ�ṹ��˵����
1.OSTCBTbl[OS_MAX_TASKS]
  OSTCBTbl[0]����������
  OSTCBTbl[1]���������������
  OSTCBTbl[2]-OSTCBTbl[OS_MAX_TASKS-1]���û�����
2.OSTCBRun��ǰ�������е�����
3.OSTCBCur��ǰ׼������������
4.OSTCBNext˳��ģʽ�£���������´�Ҫ���е�����(ֻ��˳��ģʽ��ʹ�õ�)
5.˵������ϵͳ����������֮��Ͳ����µĵ���֮ǰ�����ʱ��,OSTCBRun��OSTCBCurָ��ͬһ����--��ǰ�������е�����
       ��ϵͳ�����µĵ���֮��ʹ���������֮ǰ�����ʱ��,OSTCBRunָ��--��ǰ�������е�����
                                                   OSTCBCurָ��--(׼������������)�л�֮��Ҫ�������� 
*/

#if (OS_FAULT_ENABLE == 1) 
typedef struct
{
	OS_TCB* tcb;
	OS_TCB* tcblast;
	OS_TCB* tcbnext;
	INT16U  runLastTask;
	INT16U  task[OS_MAX_TASKS];
	INT32U  osresetimecnt;
	INT32U  backupdata[OS_FAULT_BACKUP_DATA_LEN];
}OS_FAULT;
extern OS_FAULT OS_Fault; //ϵͳ������Ϣ(�û�����)
#endif

/***********************************************ϵͳ��غ���***********************************************************/ 
                                             
/*---------------------------------------------�й�ϵͳ��ֲ����-------------------------------------------------------*/
void OS_MAIN(void);//����main������
void OSCmdRxByte(INT8U RxByte);//Cmdָ�����ݽ�����ں���(�ŵ�оƬ���ڽ����ж���,�����β�Ϊ���ڽ����ֽ�����)
/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------�û����ɵ��ú���-------------------------------------------------------*/
void OS_INT_ENTER(void); //�ر����ж�  (���ӿ�)         
void OS_INT_EXIT(void);  //�����ж�  (���ӿ�)  
void HardReset(void);    //Ӳ������
void OSPendSVPulse(void);
void OSPendSVInit(void);
STK32U* OSTaskStkInit(void (*task),STK32U *p_tos);
void OS_CreatTaskIdle(void);
void OS_TaskIdle(void);
void OS_TaskManager(void);
void OSCreatTaskManager(void);
void OS_TaskStackCounter_Process(void);
void OS_Information_Process(void);
#if (OS_CMD_ALL_ENABLE == 1)
void OS_Information_Backup(OS_Information_Backup_Struct* Struct);//����ϵͳ��Ϣ
void OS_Information_Backup_Process(OS_Information_Backup_Struct* Struct);//ϵͳ������Ϣ��ʾ
#endif
INT16U OSHighestPrioTaskNumGet(void);           //��ȡ�Ѵ���������״̬�����е�������ȼ�����
INT16U OSRunStateHighestPrioTaskNumGet(void);   //��ȡ��������̬��������ȼ�����
INT16U OSRdyHighestPrioTaskNumGet(void);        //��ȡ׼�����о���������ȼ�����(�����ȴ�����������ȼ�����(����ͬ������))
INT16U OSNextRdyHighestPrioTaskNumGet(void);    //��ȡ׼�����о���������ȼ�����(����δ���й���������ȼ�����(�����ȼ�һ����ѯ������))
void OSStartUp(void); 
void OSInit(void); 
void OSSysTickInit(void);
void OS_SysTick_Handler(void);
void OSTaskCreate(char* taskstr,void (*task),INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate);//��̬����������
void OSTaskDelete(void (*Taskx));               //��̬ɾ��������
void OSTaskResume(void (*Taskx),TASK_STATE taskstate);//��̬�ָ���ɾ����������
void OSTaskCreateStatic(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT32U tasktimeslice,INT16U taskprio,TASK_STATE taskstate);//��̬����������
/*--------------------------------------�û��ɵ��õ���ϵͳ�޹غ���----------------------------------------------*/
INT32U OS_GetStringLength(char* p);             //��ȡ�ַ���p�ĳ���
INT8U CompareCmd(char* p);                      //�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�����Ƿ�һ��
INT32U OS_GetStringLengthWithChar(char* p,char ch);//��ȡ�ַ�ch���ַ���p�е�λ��
INT32* OS_GetStringNum(char* p);                //���ַ���p����ȡ���ֱ��浽ȫ�ֱ���CmdNumBuff[]������
INT8U CompareCmdDig(char ch,char* p);           //�Ƚ��ַ���OS_Perip.OS_USART_RX_BUFF���ַ���p�У��ַ�ch��ǰ�������Ƿ�һ��
INT32 GrabCmdDig(INT8U n);                      //��ȡ�ַ���p�еĵ�n������(n=0��ʼ)
/*-------------------------------------�û��ɵ�����ϵͳ�йغ���-------------------------------------------------*/
/*--------------����ϵͳ���ܺ���-------------*/
void OSTimeSliceCounterReset(void);             //ʱ����Ƭ��������0(�ֶ��л�����ʱ����ѡ����Ҫ��ʱ����Ƭ������0)
void OSSchedSwitch(void);                       //������Ȳ��л�����(�粻��ʱ����Ƭ��0���л�����������п�����������ʱ��Ƭʱ��)
void OSContextExchangeToTask(OS_TCB* tcb);      //�л���ָ������,���������������ƿ� (�ú���Ч�ʱ�OSTaskSwitch��)
void OSScheduler_Process(void);                 //�����������ֻ���Ȳ��л���
void OSContextExchange(void);                   //�������л�(�л����񻷾�,�¸���������OSTCBCurָ�������)
void OSSchedLock(void);                         //�����л���������(��������ģʽ��OS���ֹ���ʧЧ)
void OSSchedUnlock(void);                       //�����л���������
void OSTaskSwitchBack(void* Taskx);             //������ת  ������
void OSTaskSwitch(void* Taskx);                 //������ת  ��������
void OSTaskPrioSet(void* Taskx,INT16U Taskprio);//�������ȼ����ú���(ÿ����һ�ξͻ��Զ�����һ��)
void OSTaskPrioBackup(void* Taskx);             //�������ȼ����ݺ���
void OSTaskPrioBackupSet(void* Taskx,INT16U Taskprio);//���񱸷����ȼ����ú���
INT16U OSTaskPrioBackupGet(void* Taskx);        //��ȡ���񱸷����ȼ�
OS_BOOL OSTaskStateSet(void* Taskx,TASK_STATE TaskState);//��������״̬,����OS_FALSE-����ʧ��,OS_TRUE-���óɹ�

void OSEnterDriver(void);                       //��ʼ������������ �������뿪ͷ���� (����׼���ģʽ,��ʱֻ��delay_us()����������ʹ��)
void OSExitDriver(void);                        //���������������� ���������β���� (�ָ�OSģʽ)

void OSStartRun(void);                          //ϵͳ��ʼ����(����OSģʽ)
void OSStopRun(void);                           //ϵͳֹͣ����(����ȫ���ģʽ������ʹ��OS������Դ��������ʱ����Ҳ����ʹ��)
void OSSleep(void);                             //ϵͳ����
void OSAwaken(void);                            //ϵͳ����

INT16U* OSRunTimeGet(void);                     //��ȡϵͳ�ۼ�����ʱ��,ָ���Ա(ƫ�Ƶõ�)�ֱ�Ϊ��ms��s��m��h��d��m��y��
INT8U OSTaskStateGet(void* Taskx);              //��ȡ����״̬ (��4λΪ����״̬����4λΪ����״̬)
INT16U OSTaskPrioGet(void* Taskx);              //��ȡ�������ȼ�
OS_BOOL OSLockStateGet(void);                   //��ȡϵͳ���Ƿ�������������״̬������OS_TRUE�ǣ�OS_FALSE��
void OSLockStateReset(void);                    //����������������
INT32U OSTimerValGet(void);                     //��ȡϵͳ��ʱ����ǰ����ֵ(Ĭ��ΪоƬ��Ƶ/8(8��Ƶ)��ÿ�μ�����ʱ(��Ƶ/8)us)
INT32U OSTimeSliceValGet(void);                 //��ȡOS���ļ���������ֵ(���Ĺ̶�Ϊ1ms�������ô˻�ȡ��ʱʱ��)
void OSTimeSliceValSet(INT32U time);            //����ʱ����Ƭ���ļ���������ֵ
INT32U OSClockValGet(void);                     //��ȡϵͳ���ļ���ֵ
void OSClockValSet(INT32U time);                //����ϵͳ���ļ���ֵ

/*-----------=--��̬�ڴ������--------------*/
void FSC_FragMerge(void);
OS_BOOL FSC_FragCheck(void);
void *FSC_Malloc(size_t noOfBytes);             //�ڴ�����(��λ:byte)
void FSC_Free(void* ptr);                       //�ڴ��ͷ�(�����ַ����������õ��ĵ�ַ)
INT32U FSC_MemoryFreeSizeGet(void);             //��ȡ�ڴ��ʣ���ڴ�(��λ��Byte)

/*--------------���ⶨʱ������---------------*/
INT16U OSTimerCreate(void);                     //����OSTimer������Timer_ID(�벻Ҫ�ظ����������Ӻ����д��������ظ�����)
void OSTimerDelete(INT16U Timer_ID);            //ɾ��OSTimer�����룺Timer_ID(�ظ�����ʱ���뿼��ʹ����ɺ�ɾ��)
void OSTimerReloadSet(INT16U TNum,INT32U time); //��������ϵͳ��ʱ���Զ���װ�ض�ʱֵ(����ϵͳ��ʱ���ж��в�����)
void OSTimerValueSet(INT16U TNum,INT32U time);  //��������ϵͳ��ʱ����ʱֵ
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//����ϵͳ��ʱ���Զ���װ�ض�ʱֵ��ʱ��������,<=49��17ʱ2��47��
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//����ϵͳ��ʱ����ʱֵ��ʱ��������,<=49��17ʱ2��47��
INT32U OSTimerValueGet(INT16U TNum);            //��ȡ����ϵͳ��ʱ����ǰ����ֵ
OS_BOOL OSTimerStateGet(INT16U TNum);           //��ȡ����ϵͳ��ʱ����ǰ��ʱ״̬,OS_TRUE-��ʱ��ɣ�OS_FALSE-��ʱδ���

/*---------------�ź�������-----------------*/
INT16U OSFlagCreate(void);                      //����OSFlag������Flag_ID
INT16U OSFlagGroupCreate(void);                 //����OSFlagGroup������FlagGroup_ID
INT16U OSMutexCreate(void);                     //����OSMutex������Mutex_ID
INT16U OSMBoxCreate(void);                      //����OSMBox������MBox_ID
void OSFlagDelete(INT16U Flag_ID);              //ɾ��OSFlag�����룺Flag_ID
void OSFlagGroupDelete(INT16U FlagGroup_ID);    //ɾ��OSFlagGroup�����룺FlagGroup_ID
void OSMutexDelete(INT16U Mutex_ID);            //ɾ��OSMutex�����룺Mutex_ID
void OSMBoxDelete(INT16U MBox_ID);              //ɾ��OSMBox�����룺MBox_ID
void OSFlagPost(INT16U FNum);                   //���ͱ�־��
INT8U OSFlagPend(OSFlagPendMode pendtype,INT16U FNum,INT32U timeout);//�ȴ���־��,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ�Post (NBPCģʽʱ�����ź����ۼ�����)
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum);//��ӱ�־����Ա����־Ⱥ
OS_BOOL OSFlagGroupPend(OSFGroupPendMode pendtype,INT16U FGNum,INT32U timeout);//��־��Ⱥ�ȴ�,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ����г�ԱPost
INT8U* OSFlagGroupPendTableGet(INT16U FGNum);   //��ȡȺ�ȴ���Ա
void OSMutexPost(INT16U MNum);                  //���ͷ���������--����
OS_BOOL OSMutexPend(OSMutexPendMode pendtype,INT16U MNum,INT32U timeout);//�ȴ���ȡ������--����,����OS_FALSE-�ȴ���ʱ��OS_TRUE-�õ�����ʹ��Ȩ
INT16U OSMutexBlockTaskGet(INT16U MNum);        //��ȡMutex��ǰ����������
void OSMBoxPost(INT16U MNum,void* fp);          //�����ʼ�(��ַ)
void* OSMBoxPend(OSMBoxPendMode pendtype,INT16U MNum,INT32U timeout);//�ȴ��ʼ�,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ�Post

/*---------------��ʱ����------------------*/
//��ͨ��ʱ����(�ж������)
void delay_us(INT32U nus);                      //΢����ʱ����,�ж��п���
void delay_ms(INT32U nms);	                    //������ʱ����,�ж��п���
//������ʱ����
void OS_delayMs(volatile INT32U nms);           //������ʱ����,�ж��в�����
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec);//��ʱ������ʱ��<=49��17ʱ2��47��,�ж��в�����
void OSTaskDelayMsSet(void* Taskx,INT32U nms);  //������ʱ�޸ĺ���(��һ���������޸���һ��������ʱ���������ʱֵ,�����ӳ�������Ŀ���������ʱ)

/*---------ϵͳ�رյ���ʱ���ú���-----------*/
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second); //����ϵͳ�رն�ʱʱ��
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/
#endif

/**************************************************������ˮ*************************************************************/
