/*--------------------------------------------�汾: V4.7--------------------------------------------*/
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
#define OS_RINNING_MODE          0        //OS����ģʽ��0-˳������  1-���ȼ�����(���ֵ����OS_MAX_TASKS-1)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
#define OS_MAX_TASKS             8        //������=�û�������+2  ������:3-65535  �û�����ʵ����Ҫ�����������޸�
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TIMER_SIZE               4        //ϵͳ���ⶨʱ������ (������Ҫ���ã�1-65535������ͬ)
#define FLAG_SIZE                4        //��־����            
#define FLAG_GROUP_SIZE          4        //��־Ⱥ����         
#define MUTEX_SIZE               4        //��������           
#define MBOX_SIZE                4        //��������  
#define MBQ_SIZE                 4        //������л�������
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CMD_ALL_ENABLE        1        //����ȫ��ϵͳָ��
#define OS_CMD_NSY_ENABLE        1        //����ϵͳ�޹ص�ָ��
#define OS_CMD_DISP_SYS_ENABLE   1        //����ϵͳ״̬��Ϣָ����ʾ(�ر���ʾ�ܽ�ʡ�����ڴ�)
#define OS_CMD_DISP_NSY_ENABLE   1        //����ϵͳ״̬��Ϣ�����޹ص�ָ����ʾ(0-���ã�1-������,��ͬ)
#define OS_GET_SET_SW_ENABLE     1        //�����������á���ȡ����ת
#define OS_SIGN_PP_ENABLE        1        //�����ź���
#define OS_TIM_SYS_ENABLE        1        //����ϵͳ���ⶨʱ��
#define OS_REMOTE_ENABLE         0        //���ò����ú���
#define OS_FAULT_ENABLE          0        //���ù��ϼ��ϵͳ(��Ҫ��stm32fxxx_it.c��ע�͵�HardFault_Handler()�жϺ���)
#define OS_FAULT_DISP_ENABLE     0        //���ù��ϼ��ϵͳ��ʾ
#define OS_TIMECOUNTSYS_ENABLE   0        //����ϵͳʱ��ͳ��ϵͳ
#define OS_SYSTIME_ENABLE        0        //����ϵͳʱ��RTC
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TASK_NAME_LEN            32       //������       �ַ���󳤶�(������������32���ַ����޸�)
#define OS_CMD_STR_LEN           64       //ϵͳָ��     �ַ���󳤶�
#define OS_CMD_NUM_LEN           8        //ϵͳָ���ַ�������������ָ���("prio=89//"Ϊһ������)
#define OS_FAULT_BACKUP_DATA_LEN 1        //ϵͳ�������ݱ��ݳ���
      
/*----------------------------ϵͳ�������--------------------------------*/			
#define OS_TIMESLICE_IDLE        1        //��������ʱ����Ƭ����λ:΢�� ms
#define OS_TIMESLICE_MANAGE      1        //�������������ʱ����Ƭ,��λ:΢�� ms

#define TaskIDLE_StkSize         32       //���������ջ��С
#define TaskManage_StkSize       160      //��������������ջ��С
/*----------------------------���ϼ��ϵͳ--------------------------------*/
#define  OS_Reset_Delay_Time     10       //ϵͳ������ʱ,��λ��s
#define  OS_Shutdown_Delay_Time  10       //ϵͳ�ر���ʱ,��λ��s
#define  OS_EndlessLoops_Times   1000     //�������иô���������Ϊ������������ѭ���޷��л���������������,��λ������
#define  OSFAULT_Reset_Enable    1        //�Ƿ�������Ϻ��Զ�����,0-������ 1-����
/***************************************************************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++״̬������+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*---------------------ϵͳ״̬(�û�����)----------------------*/
#define TASK_CREATING           0        //����̬
#define TASK_RUNNING            1        //����̬
#define TASK_PAUSING            2        //��̬ͣ
#define TASK_BACKRUNNING        3        //��̨����̬
#define TASK_DELETING           4        //ɾ��̬
/*-------------------�ź����ȴ�״̬(�û�����)------------------*/
#define OSFlag_BPN              0        //�����ȴ����ź���(ֻ��ӦPend֮����ź�)
#define OSFlag_BPO              1        //�����ȴ������ź���(����Ӧ��Pend֮ǰPost�����ź�)
#define OSFlag_BPC              2        //�����ȴ���Ӧ�ۼ��ź���(�ۼ�Post���д�����Pend��ȡһ�Σ��ۼƽ���1�Σ�������ͬ����Ӧȫ��Post�ź�)
#define OSFlag_NBPC             3        //�������ȴ���Ӧ�ۼ��ź���(ͬ��)
#define OSFlag_NBPN             4        //�������ȴ����ź���(ͬ��)

#define OSFGroup_BPN            0        //�����ȴ����ź���(Pend��ȡ���µ�FlagGroupȫ����������ź�)
#define OSFGroup_NBPN           1        //�������ȴ����ź���(Pend��ȡ���µ�FlagGroupȫ����������ź�)

#define OSMBox_BPN              0        //�����ȴ����ź���(��ȡPend֮��Post���ʼ�)
#define OSMBox_BPQ              1        //�����ȴ���Ӧ�����ź���(����Post���ʼ������б���������Pend��ȡʱ��ȡ�ȷ��͵ģ��ȷ��ȶ����󷢺��)
#define OSMBox_NBPN             2        //�������ȴ����ź���(ͬ��)
#define OSMBox_NBPQ             3        //�������ȴ���Ӧ�����ź���(ͬ��)
/*-----------------�ź���ϵͳ��־λ(�û�������)------------------*/												     
#define _BIT_TimeOut            0        //��־λ0����ʱʱ�䳬ʱ
#define _BIT_TimeLess           1        //��־λ1�����޵ȴ�
#define _BIT_Flag               2        //��־λ2��OSFlag��־���ȴ�
#define _BIT_FlagGroup          3        //��־λ3��OSFlagGroup��־��Ⱥ�ȴ�
#define _BIT_Mutex              4        //��־λ4��OSMutex�������ȴ�  
#define _BIT_MBox               5        //��־λ5��OSMBox�ʼ��ȴ�
/*-------------------ϵͳ�߼�״̬(�û�����)--------------------*/	
#define OS_FALSE                0        //��
#define OS_TRUE                 1        //��
/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------OSprintf-------------------------------------------------------*/
#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS�ṩ�Ĵ�ӡ�������
/***************************************************************************************************************/
#define  FSC_STOS   //����Ƕ��FSC_STOS�ı�ʶ(�������ļ����ж�FSC_STOS�Ƿ��Ѷ������ж��Ƿ�Ƕ����FSC_STOS)
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

typedef char  INT8;            
typedef short INT16;           
typedef int   INT32; 
typedef unsigned char  INT8U;            
typedef unsigned short INT16U;           
typedef unsigned int   INT32U; 
typedef unsigned int   STK32U;
#define OSSTK __align(8) STK32U

#if (OS_CMD_ALL_ENABLE == 1) 
typedef struct
{
	INT8U  RXOK_Flag;
	INT8U  RX_COUNT;
	char   RX_BUFF[OS_CMD_STR_LEN];	
}OS_CMD;
extern OS_CMD OS_Cmd; //ϵͳָ�����ݽ���(�û�������)
#endif

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
	INT32U SysTimerCnt;
	INT16U IntNesting; 
	INT16U LockNesting; 
	INT32U CtxSwCtr; 
	INT16U TaskHighestRunLast[OS_MAX_TASKS];
	INT16U TaskHighestPrioLast;
	INT32U TaskContinuRunTimeCnt;
	INT16U TaskNext;
	INT8U  TaskNextRunFlag;
#if (OS_GET_SET_SW_ENABLE == 1)		
	INT16U TaskSwitchBackNum;  
#endif	
	INT16U TaskCPUOccRateMax;
	INT32U TaskTimeSliceCnt;
#if (OS_TIM_SYS_ENABLE == 1)
	INT32U TIMER[TIMER_SIZE]; 
	INT32U TIMER_RELOAD[TIMER_SIZE]; 
#endif		
#if (OS_SIGN_PP_ENABLE == 1)	
	INT32U  POST_FLAG_COUNT[FLAG_SIZE];
	INT32U  POST_FGROUP_COUNT[FLAG_GROUP_SIZE];
	INT32U  POST_MBOX_COUNT[MBOX_SIZE];
	INT16U  POST_MBQ_COUNT[MBOX_SIZE];
	INT16U  FLAG[FLAG_SIZE][OS_MAX_TASKS];//��ͬ������Եȴ���ͬ��Flag
	INT16U  GROUPRXFLAG[FLAG_GROUP_SIZE][OS_MAX_TASKS];//��־��Ⱥȫ�����ձ�־
	INT8U   FlagGroupTable[FLAG_GROUP_SIZE][FLAG_SIZE];
	INT16U  FlagGroupNameInTask[OS_MAX_TASKS];
	INT8U   ReturnFlagGroupTable[FLAG_SIZE];//��ѯFlagGroup��Ա����Ļ�������
  INT16U  MUTEX[MUTEX_SIZE]; 	
	INT16U  MutexTaskNum[MUTEX_SIZE];
	INT16U  MutexNameInTask[MUTEX_SIZE];
	INT32U *MBOX[OS_MAX_TASKS*MBOX_SIZE];
	INT32U *MBQueue[MBQ_SIZE*MBOX_SIZE];
#endif	
}volatile OS_SYSTEM;
extern OS_SYSTEM OS_System;   

typedef struct 
{
	STK32U    *StkPtr;//����ջָ������ǵ�һ�� 
  STK32U    *StkAdd;	
#if (OS_CMD_ALL_ENABLE == 1) 	
	INT32U    StkSize;
	INT32U    StkFree;
	INT32U    StkUsed;
	INT32U    StkUsedMax;
	INT32U    TaskCPUOccRateCnt;	
#endif	
#if (OS_CMD_ALL_ENABLE == 1)||(OS_FAULT_ENABLE == 1)	
  char      TaskStr[TASK_NAME_LEN];	
#endif		
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U    FlagName;
	INT16U    FlagGroupName;
	INT16U    MutexName;
	INT16U    MBoxName;
#endif	
  INT32U    TaskAdd;	
	INT32U    TaskDelayMs;
  INT32U    TaskTimeSlice;	
	INT16U    TaskNum; 
	INT16U    TaskPrio; 
	INT16U    TaskPrioBackup; 
	INT8U     TaskState;
	INT8U     TaskFlagBit;//bit0-��ʱ bit1-���޵ȴ� bit2-�ȴ�OSFlag bit3-�ȴ�OSFlagGroup bit4-�ȴ�OSMutex bit5-�ȴ�MBox
}volatile OS_TCB; 
extern OS_TCB OSTCBTbl[OS_MAX_TASKS];  
extern OS_TCB *OSTCBRun; 
extern OS_TCB *OSTCBCur; 
extern OS_TCB *OSTCBNext;

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
void OSIntEnter(void);
void OSIntExit(void);
void OSPendSVPulse(void);
void OSPendSVInit(void);
void OSSysTickInit(INT16U Nms);
STK32U* OSTaskStkInit(void (*task),STK32U *p_tos);
void OS_CreatTaskIdle(void);
void OS_TaskIdle(void);
void OS_TaskManage(void);
void OSCreatTaskManage(void);
void OSStartUp(void); 
void OSInit(void); 
void OSTimerInit(void);
void OS_Timer_Handler(void);
void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT32U tasktimeslice,INT16U taskprio);//���񴴽�����
/*--------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------�û��ɵ��ú���--------------------------------------------------------*/
void OSSchedLock(void);                         //�����л���������(��������ģʽ��OS���ֹ���ʧЧ)
void OSSchedUnlock(void);                       //�����л���������
void OSEnterDriver(void);                       //������������ ���� (����׼���ģʽ,��ʱֻ��delay_us()����������ʹ��)
void OSExitDriver(void);                        //�뿪�������� ���� (�ָ�OSģʽ)
void OSStartRun(void);                          //ϵͳ��ʼ����(����OSģʽ)
void OSStopRun(void);                           //ϵͳֹͣ����(����ȫ���ģʽ������ʹ��OS������Դ��������ʱ����Ҳ����ʹ��)
void OSSupendCtxSw(void);                       //���������л�����
void OSForceSupendCtxSw(void);                  //ǿ�ƴ��������л�����(������������������л�����,���ܲ����������)
void OSTaskSwitchBack(void* Taskx);             //������ת  ������
void OSTaskSwitch(void* Taskx);                 //������ת  ��������
INT16U OSHighestPrioTaskNumGet(void);           //��ȡ���������е�������ȼ�����
INT16U OSRunStateHighestPrioTaskNumGet(void);   //��ȡ��������״̬��������ȼ�����
INT16U OSRdyHighestPrioTaskNumGet(void);        //��ȡ���о���������ȼ�����
INT16U* OSRunTimeGet(void);                     //��ȡϵͳ�ۼ�����ʱ��,ָ���Ա�ֱ�Ϊ��ms��s��m��h��d��m��y��
INT8U OSTaskStateGet(void* Taskx);              //��ȡ����״̬
INT16U OSTaskPrioGet(void* Taskx);              //��ȡ�������ȼ�
INT32U OSTimerValueGet(INT16U TNum);            //��ȡ����ϵͳ��ʱ����ǰ����ֵ
INT8U OSTimerStateGet(INT16U TNum);             //��ȡ����ϵͳ��ʱ����ǰ��ʱ״̬,OS_TRUE-��ʱ��ɣ�OS_FALSE-��ʱδ���
INT32U OSTimerValGet(void);                     //��ȡϵͳ��ʱ����ǰ����ֵ(Ĭ��Ϊ9MHz��ÿ�μ�����ʱ1/9 us)
INT8U* OSFlagGroupPendTableGet(INT16U FGNum);   //��ȡȺ�ȴ���Ա
INT32U OSTimeSliceGet(void);                    //��ȡOS������
void OSTimeSliceSet(INT32U time);               //����ʱ����Ƭ������
void OSTaskPrioSet(void* Taskx,INT16U Taskprio);//�����������ȼ�
void OSTimerReloadSet(INT16U TNum,INT32U time); //��������ϵͳ��ʱ���Զ���װ�ض�ʱֵ(����ϵͳ��ʱ���ж��в�����)
void OSTimerValueSet(INT16U TNum,INT32U time);  //��������ϵͳ��ʱ����ʱֵ
INT8U OSTaskStateSet(void* Taskx,INT8U TaskState);//��������״̬,����OS_FALSE-����ʧ��,OS_TRUE-���óɹ�
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//����ϵͳ��ʱ���Զ���װ�ض�ʱֵ��ʱ��������,<=49��17ʱ2��47��
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//����ϵͳ��ʱ����ʱֵ��ʱ��������,<=49��17ʱ2��47��
void OSFlagPost(INT16U FNum);                   //���ͱ�־��
INT8U OSFlagPend(INT8U pendtype,INT16U FNum,INT32U timeout);//�ȴ���־��,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ�Post
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum);//��ӱ�־����Ա����־Ⱥ
INT8U OSFlagGroupPend(INT8U pendtype,INT16U FGNum,INT32U timeout);//��־��Ⱥ�ȴ�,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ����г�ԱPost
void OSMutexPost(INT16U MNum);                  //���ͻ�����
INT8U OSMutexPend(INT16U MNum,INT32U timeout);  //�ȴ�������,����OS_FALSE-�ȴ���ʱ��OS_TRUE-�õ�����ʹ��Ȩ
INT16U OSMutexBlockTaskGet(INT16U MNum);        //��ȡMutex��ǰ����������
void OSMboxPost(INT16U MNum,void* fp);          //�����ʼ�(��ַ)
void* OSMboxPend(INT8U pendtype,INT16U MNum,INT32U timeout);//�ȴ��ʼ�,����OS_FALSE-�ȴ���ʱ��OS_TRUE-���յ�Post
void delay_us(INT32U nus);                      //΢����ʱ����,�ж��п���
void delay_ms(INT32U nms);	                    //������ʱ����,�ж��в�����
void OS_delayMs(volatile INT32U nms);           //������ʱ����,�ж��в�����
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec);//��ʱ������ʱ��<=49��17ʱ2��47��,�ж��в�����
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second); //����ϵͳ�رն�ʱʱ��
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/
#endif

/**************************************************������ˮ*************************************************************/
