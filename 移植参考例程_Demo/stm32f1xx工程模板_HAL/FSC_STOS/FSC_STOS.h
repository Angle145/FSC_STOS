//���ߣ�������ˮ
#ifndef _FSC_STOS_H_
#define _FSC_STOS_H_

/*****************ͷ�ļ�ѡ��*****************/
//#include "stm32f0xx.h"  //stm32f0  STD��
#include "stm32f1xx_hal.h"//stm32f1  HAL��
//#include "stm32f10x.h"  //stm32f1  STD��
//#include "stm32f4xx.h"  //stm32f4  STD��
/********************************************/
      
/****************************************�û����Զ���***********************************************************/
#define OS_RINNING_MODE         1        //OS����ģʽ��0 -- �ɴ�ϵ�˳������   1 -- ���ϵ����ȼ�����
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CLOCK_TIME           1000     //����ʱ����Ƭ,ÿ������������е�ʱ�䣬��λ:΢�� us  
#define OS_MAX_TASKS            8        //������=�û�������+2  ������:3-65535  �û�����ʵ����Ҫ�����������޸�
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TIMER_SIZE              4        //ϵͳ���ⶨʱ������ (������Ҫ���ã�1-65535������ͬ)
#define FLAG_SIZE               4        //��־����            
#define FLAG_GROUP_SIZE         4        //��־Ⱥ����         
#define MUTEX_SIZE              4        //��������           
#define MBOX_SIZE               4        //��������            
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CMD_DISP_NSY_ENABLE  1        //����ϵͳ״̬��Ϣ�����޹ص�ָ����ʾ(0-���ã�1-������,��ͬ)
#define OS_CMD_DISP_SYS_ENABLE  1        //����ϵͳ״̬��Ϣָ����ʾ(�ر���ʾ�ܽ�ʡ�����ڴ�)
#define OS_CMD_NSY_ENABLE       0        //����ϵͳ�޹ص�ָ��
#define OS_CMD_ALL_ENABLE       1        //����ȫ��ϵͳָ��
#define OS_GET_SET_SW_ENABLE    0        //�����������á���ȡ����ת
#define OS_SIGN_PP_ENABLE       1        //�����ź���
#define OS_TIM_SYS_ENABLE       0        //����ϵͳ���ⶨʱ��
#define OS_REMOTE_ENABLE        0        //���ò����ú���
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TASK_NAME_SIZE          32       //������       �ַ���󳤶�(������������32���ַ����޸�)
#define OS_PERIP_USART_BUFF_LEN 32       //ϵͳָ��     �ַ���󳤶�
#define OS_CMD_NUM_LEN          2        //ϵͳָ���ַ�������������ָ���("prio=89//"Ϊһ������)
                                         
#define TaskIDLE_StkSize        64       //���������ջ��С
#define TaskManage_StkSize      160      //��������������ջ��С
/***************************************************************************************************************/
/***************************************************************************************************************/
#define TASK_CREATING           0        //����̬
#define TASK_RUNNING            1        //����̬
#define TASK_PAUSING            2        //��̬ͣ
#define TASK_BACKRUNNING        3        //��̨����̬
#define TASK_DELETING           4        //ɾ��̬
																		     
#define OS_FALSE                0        //��
#define OS_TRUE                 1        //��

#define _BIT_TimeOut            0        //��־λ0����ʱʱ�䳬ʱ
#define _BIT_TimeLess           1        //��־λ1�����޵ȴ�
#define _BIT_Flag               2        //��־λ2��OSFlag��־���ȴ�
#define _BIT_FlagGroup          3        //��־λ3��OSFlagGroup��־��Ⱥ�ȴ�
#define _BIT_Mutex              4        //��־λ4��OSMutex�������ȴ�  
#define _BIT_MBox               5        //��־λ5��OSMBox�ʼ��ȴ�

#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS�ṩ�Ĵ�ӡ�������
/***************************************************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

typedef unsigned char  INT8U;            
typedef unsigned short INT16U;           
typedef unsigned int   INT32U;           
typedef unsigned int   STK32U;
#define OSSTK __align(8) STK32U

#if (OS_CMD_ALL_ENABLE == 1) 
typedef struct
{
	INT8U  CmdRxFlag;
	INT8U  USART_COUNT;
	char   USART_RX_BUFF[OS_PERIP_USART_BUFF_LEN];	
}OS_PERIP;
extern OS_PERIP OS_Perip; //ϵͳ����ָ�����(�û�������)
#endif

typedef struct
{
	INT8U  Error;
	INT8U  Running;
	INT8U  RuningMode;
	INT16U IntNesting; 
	INT16U LockNesting; 
	INT32U CtxSwCtr; 
	INT16U TaskNext;
	INT8U  TaskNextRunFlag;
#if (OS_GET_SET_SW_ENABLE == 1)		
	INT16U TaskSwitchBackNum;  
#endif	
	INT16U TaskCPUOccRateMax;
	INT32U TaskTimeSliceCnt;
#if (OS_TIM_SYS_ENABLE == 1)
	INT32U TIMER[TIMER_SIZE]; 
#endif		
#if (OS_SIGN_PP_ENABLE == 1)		
	INT16U  FLAG[FLAG_SIZE][OS_MAX_TASKS];//��ͬ������Եȴ���ͬ��Flag
	INT8U   FlagGroupTable[FLAG_GROUP_SIZE][FLAG_SIZE];
	INT16U  FlagGroupNameInTask[OS_MAX_TASKS];
	INT8U   ReturnFlagGroupTable[FLAG_SIZE];//��ѯFlagGroup��Ա����Ļ�������
  INT16U  MUTEX[MUTEX_SIZE]; 	
	INT16U  MutexTaskNum[MUTEX_SIZE];
	INT16U  MutexNameInTask[MUTEX_SIZE];
	INT32U *MBOX[OS_MAX_TASKS*MBOX_SIZE];
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
	char      TaskStr[TASK_NAME_SIZE];	
#endif	
#if (OS_SIGN_PP_ENABLE == 1)	
	INT16U    FlagName;
	INT16U    FlagGroupName;
	INT16U    MutexName;
	INT16U    MBoxName;
#endif	
  INT32U    TaskAdd;	
	INT32U    TaskDelayMs;     
	INT16U    TaskNum; 
	INT16U    TaskPrio; 
	INT8U     TaskState;
	INT8U     TaskFlagBit;//bit0-��ʱ bit1-���޵ȴ� bit2-�ȴ�OSFlag bit3-�ȴ�OSFlagGroup bit4-�ȴ�OSMutex bit5-�ȴ�MBox
}volatile OS_TCB; 
extern OS_TCB OSTCBTbl[OS_MAX_TASKS];  
extern OS_TCB *OSTCBRun; 
extern OS_TCB *OSTCBCur; 
extern OS_TCB *OSTCBNext;

/***********************************************ϵͳ��غ���***********************************************************/ 
                                             
/*---------------------------------------------�й�ϵͳ��ֲ����-------------------------------------------------------*/
void OS_MAIN(void);//����main������
void OSCmdUsartReceiveByte(INT8U UsartRxByte);//Cmdָ�����ݽ�����ں���(�ŵ�оƬ���ڽ����ж���,�����β�Ϊ���ڽ����ֽ�����)
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
void OSTimerInit(INT16U Nus);
void OS_Timer_Handler(void);
/*--------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------�û��ɵ��ú���--------------------------------------------------------*/
void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT16U taskprio);//���񴴽�����
INT8U OSTaskStateGet(void* Taskx);              //��ȡ����״̬
void OSTaskStateSet(void* Taskx,INT8U TaskState);//����״̬����
INT16U OSTaskPrioGet(void* Taskx);              //��ȡ�������ȼ�
void OSTaskPrioSet(void* Taskx,INT16U Taskprio);//�������ȼ�����
void OSTaskSwitchBack(void* Taskx);             //������ת  ������
void OSTaskSwitch(void* Taskx);                 //������ת  ��������
void OSSchedLock(void);                         //�����л���������(�еĴ������һ������ɣ���;���ܱ��л���ȥ,���봦�����������OSSchedUnlock()���)  
void OSSchedUnlock(void);                       //�����л���������
void OSTimerReloadSet(INT16U TNum,INT32U time); //����ϵͳ��ʱ����ֵ����
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//����ϵͳ��ʱ����ֵ��ʱ��������,<=49��17ʱ2��47��
INT8U OSTimerStateGet(INT16U TNum);             //����ϵͳ��ʱ����ʱ״̬
void OSFlagPost(INT16U FNum);                   //���ͱ�־��
INT8U OSFlagPend(INT16U FNum,INT32U timeout);   //�ȴ���־������ʱʱ��
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum);//��ӱ�־����Ա����־Ⱥ
INT8U OSFlagGroupPend(INT16U FGNum,INT32U timeout);//��־��Ⱥ�ȴ�
INT8U* OSFlagGroupPendTableGet(INT16U FGNum);   //��ȡȺ�ȴ���Ա
void OSMutexPost(INT16U MNum);                  //���ͻ�����
INT8U OSMutexPend(INT16U MNum,INT32U timeout);  //�ȴ�����������ʱʱ��
INT16U OSMutexBlockTaskGet(INT16U MNum);        //��ȡMutex��ǰ����������
void OSMboxPost(INT16U MNum,void* fp);          //�����ʼ�(��ַ)
void* OSMboxPend(INT16U MNum,INT32U timeout);   //�ȴ��������ʱʱ��
INT32U OSTimeSliceGet(void);                    //��ȡOS������
INT32U OSTimerValGet(void);                     //��ȡϵͳ��ʱ����ǰ����ֵ(Ĭ��Ϊ9MHz��ÿ�μ�����ʱ1/9 us)
void delay_us(INT32U nus);                      //΢����ʱ����
void delay_ms(INT32U nms);	                    //������ʱ����
void OS_delayMs(volatile INT32U nms);           //������ʱ����
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec);//��ʱ������ʱ��<=49��17ʱ2��47��
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif

/**************************************************������ˮ*************************************************************/
