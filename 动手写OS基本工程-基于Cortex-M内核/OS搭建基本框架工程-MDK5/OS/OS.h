#ifndef _OS_H_
#define _OS_H_
#include "stm32f10x.h"
#include <stdio.h>
typedef unsigned char  INT8U;            
typedef unsigned short INT16U;           
typedef unsigned int   INT32U;           
typedef unsigned int   OS_STK;            

/*******************************OS��������********************************************/
#define OS_MAX_TASKS        16         //���������(1-255)
#define TASK_TIME_SLICE     10         //����ʱ����Ƭ,��λ��1ms   
#define STK_IDLE_SIZE       18         //��������ջ����
/**************************************************************************************/

//�ط�װprintf����
#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS�ṩ�Ĵ�ӡ�������

//����״̬
#define TASK_STATE_CREATING   0
#define TASK_STATE_RUNNING    1
#define TASK_STATE_PAUSING    2

//ϵͳ����ֵ
#define OS_FALSE 0 
#define OS_TRUE  1 

//������ƿ����Ͷ���(����ѧϰ����޸�Ϊ˫������)
typedef struct 
{
	OS_STK    *StkPtr;  //����ջָ�� (�����ǵ�һ��)  
	INT8U     State;    //����״̬
  //INT32U  DelayTime;//������ʱʱ��	
	//INT8U   Prio;     //�������ȼ�
	//INT32U  TimeSlice;//����ʱ��Ƭ
	//struct OS_TCB *last;//�ϸ�����
	//struct OS_TCB *next;//�¸�����
} OS_TCB; 

extern OS_TCB OSTCBTbl[OS_MAX_TASKS]; //���������TCB�����鷽ʽ(���0-�������񣬱��1-Task1--���n-Taskn)
extern OS_TCB *OSTCBCur;  //��ǰ�������е�����TCB
extern OS_TCB *OSTCBNext; //�¸��������е�����TCB

//ϵͳ�������Ͷ���
typedef struct 
{
	INT8U OSRunning;    //���б�־����
	INT8U OSLockNesting;//�����л���������ͳ�Ʊ��� 
	INT8U  OSTaskNext;  //�¸����������TCB�������
	INT32U TimeMS;      //ϵͳʱ�ӽ����ۼƱ���
	INT32U TaskTimeSlice;//����ʱ����Ƭ(Ϊϵͳʱ�ӽ��ı���,����������е�ʱ��)
}SYSTEM;
extern SYSTEM System;//ϵͳ����

void OS_CORE_ENTER(void);                            //�ر�ȫ���жϺ���(�ڻ����)            
void OS_CORE_EXIT(void);                             //��ȫ���жϺ���(�ڻ����) 
void OSInit(void);                                   //ϵͳ��ʹ������
void OSStartUp(void);                                //ϵͳ��������(�ڻ����) 
void OSScheduler(void);                              //���������
void OSPendSVPulse(void);                            //���񻷾��л����� 
void OSSchedSwitch(void);                            //������Ȳ��л�����        
OS_STK* OSTaskStkInit(void* task,OS_STK *topstkptr); //�����ջ��ʹ������
void OSTaskCreate(void* task,OS_STK *topstkptr);     //���񴴽�����
void OSInitTaskIdle(void);                           //�������񴴽�����
void OS_TaskIdle(void);                              //������������
void OSScheduler(void);                              //���������
void OSPendSVPulse(void);                            //���񻷾��л�����
void OSSchedSwitch(void);                            //������Ȳ��л�����                            
void OSSchedLock (void);                             //�����л���������                         
void OSSchedUnlock (void);                           //�����л���������
INT32U GetTime(void);                                //��ȡϵͳʱ�ӽ���������
void delay_us(INT32U us);                            //΢����ʱ����
void delay_ms(INT32U ms);	                           //������ʱ����
void OS_delayMs(INT32U ms);                          //������ʱ����
#endif
