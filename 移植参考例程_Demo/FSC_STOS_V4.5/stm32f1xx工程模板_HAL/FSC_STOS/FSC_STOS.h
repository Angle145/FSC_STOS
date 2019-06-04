//编者：望穿秋水
#ifndef _FSC_STOS_H_
#define _FSC_STOS_H_

/*****************头文件选择*****************/
//#include "stm32f0xx.h"  //stm32f0  STD库
#include "stm32f1xx_hal.h"//stm32f1  HAL库
//#include "stm32f10x.h"  //stm32f1  STD库
//#include "stm32f4xx.h"  //stm32f4  STD库
/********************************************/
      
/****************************************用户可自定义***********************************************************/
#define OS_RINNING_MODE         1        //OS运行模式：0 -- 可打断的顺序运行   1 -- 抢断的优先级运行
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CLOCK_TIME           1000     //任务时间切片,每个任务持续运行的时间，单位:微秒 us  
#define OS_MAX_TASKS            8        //任务数=用户任务数+2  任务数:3-65535  用户根据实际需要的任务数量修改
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TIMER_SIZE              4        //系统虚拟定时器数量 (根据需要配置，1-65535，以下同)
#define FLAG_SIZE               4        //标志数量            
#define FLAG_GROUP_SIZE         4        //标志群数量         
#define MUTEX_SIZE              4        //互斥数量           
#define MBOX_SIZE               4        //邮箱数量            
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CMD_DISP_NSY_ENABLE  1        //剪裁系统状态信息以外无关的指令显示(0-剪裁，1-不剪裁,下同)
#define OS_CMD_DISP_SYS_ENABLE  1        //剪裁系统状态信息指令显示(关闭显示能节省大量内存)
#define OS_CMD_NSY_ENABLE       0        //剪裁系统无关的指令
#define OS_CMD_ALL_ENABLE       1        //剪裁全部系统指令
#define OS_GET_SET_SW_ENABLE    0        //剪裁任务设置、获取、跳转
#define OS_SIGN_PP_ENABLE       1        //剪裁信号量
#define OS_TIM_SYS_ENABLE       0        //剪裁系统虚拟定时器
#define OS_REMOTE_ENABLE        0        //剪裁不常用函数
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TASK_NAME_SIZE          32       //任务名       字符最大长度(如任务名超过32个字符请修改)
#define OS_PERIP_USART_BUFF_LEN 32       //系统指令     字符最大长度
#define OS_CMD_NUM_LEN          2        //系统指令字符串允许包含数字个数("prio=89//"为一个数字)
                                         
#define TaskIDLE_StkSize        64       //空闲任务堆栈大小
#define TaskManage_StkSize      160      //任务管理器任务堆栈大小
/***************************************************************************************************************/
/***************************************************************************************************************/
#define TASK_CREATING           0        //创建态
#define TASK_RUNNING            1        //运行态
#define TASK_PAUSING            2        //暂停态
#define TASK_BACKRUNNING        3        //后台运行态
#define TASK_DELETING           4        //删除态
																		     
#define OS_FALSE                0        //假
#define OS_TRUE                 1        //真

#define _BIT_TimeOut            0        //标志位0：延时时间超时
#define _BIT_TimeLess           1        //标志位1：无限等待
#define _BIT_Flag               2        //标志位2：OSFlag标志量等待
#define _BIT_FlagGroup          3        //标志位3：OSFlagGroup标志量群等待
#define _BIT_Mutex              4        //标志位4：OSMutex互锁量等待  
#define _BIT_MBox               5        //标志位5：OSMBox邮件等待

#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS提供的打印输出函数
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
extern OS_PERIP OS_Perip; //系统串口指令接收(用户不可用)
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
	INT16U  FLAG[FLAG_SIZE][OS_MAX_TASKS];//不同任务可以等待相同的Flag
	INT8U   FlagGroupTable[FLAG_GROUP_SIZE][FLAG_SIZE];
	INT16U  FlagGroupNameInTask[OS_MAX_TASKS];
	INT8U   ReturnFlagGroupTable[FLAG_SIZE];//查询FlagGroup成员情况的缓存数组
  INT16U  MUTEX[MUTEX_SIZE]; 	
	INT16U  MutexTaskNum[MUTEX_SIZE];
	INT16U  MutexNameInTask[MUTEX_SIZE];
	INT32U *MBOX[OS_MAX_TASKS*MBOX_SIZE];
#endif	
}volatile OS_SYSTEM;
extern OS_SYSTEM OS_System;  

typedef struct 
{
	STK32U    *StkPtr;//任务栈指针必须是第一个 
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
	INT8U     TaskFlagBit;//bit0-超时 bit1-无限等待 bit2-等待OSFlag bit3-等待OSFlagGroup bit4-等待OSMutex bit5-等待MBox
}volatile OS_TCB; 
extern OS_TCB OSTCBTbl[OS_MAX_TASKS];  
extern OS_TCB *OSTCBRun; 
extern OS_TCB *OSTCBCur; 
extern OS_TCB *OSTCBNext;

/***********************************************系统相关函数***********************************************************/ 
                                             
/*---------------------------------------------有关系统移植函数-------------------------------------------------------*/
void OS_MAIN(void);//放在main函数里
void OSCmdUsartReceiveByte(INT8U UsartRxByte);//Cmd指令数据接收入口函数(放到芯片串口接收中断里,输入形参为串口接收字节数据)
/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------用户不可调用函数-------------------------------------------------------*/
void OS_INT_ENTER(void); //关闭总中断  (汇编接口)         
void OS_INT_EXIT(void);  //打开总中断  (汇编接口)  
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
/*----------------------------------------------用户可调用函数--------------------------------------------------------*/
void OSTaskCreate(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT8U taskstate,INT16U taskprio);//任务创建函数
INT8U OSTaskStateGet(void* Taskx);              //获取任务状态
void OSTaskStateSet(void* Taskx,INT8U TaskState);//任务状态设置
INT16U OSTaskPrioGet(void* Taskx);              //获取任务优先级
void OSTaskPrioSet(void* Taskx,INT16U Taskprio);//任务优先级设置
void OSTaskSwitchBack(void* Taskx);             //任务跳转  带返回
void OSTaskSwitch(void* Taskx);                 //任务跳转  不带返回
void OSSchedLock(void);                         //任务切换上锁函数(有的代码必须一次性完成，中途不能被切换出去,代码处理完后必须调用OSSchedUnlock()解除)  
void OSSchedUnlock(void);                       //任务切换解锁函数
void OSTimerReloadSet(INT16U TNum,INT32U time); //虚拟系统定时器初值设置
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//虚拟系统定时器初值日时分秒设置,<=49天17时2分47秒
INT8U OSTimerStateGet(INT16U TNum);             //虚拟系统定时器计时状态
void OSFlagPost(INT16U FNum);                   //发送标志量
INT8U OSFlagPend(INT16U FNum,INT32U timeout);   //等待标志量带超时时间
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum);//添加标志量成员至标志群
INT8U OSFlagGroupPend(INT16U FGNum,INT32U timeout);//标志量群等待
INT8U* OSFlagGroupPendTableGet(INT16U FGNum);   //获取群等待成员
void OSMutexPost(INT16U MNum);                  //发送互斥量
INT8U OSMutexPend(INT16U MNum,INT32U timeout);  //等待互斥量带超时时间
INT16U OSMutexBlockTaskGet(INT16U MNum);        //获取Mutex当前阻塞的任务
void OSMboxPost(INT16U MNum,void* fp);          //发送邮件(地址)
void* OSMboxPend(INT16U MNum,INT32U timeout);   //等待邮箱带超时时间
INT32U OSTimeSliceGet(void);                    //获取OS节拍数
INT32U OSTimerValGet(void);                     //获取系统定时器当前计数值(默认为9MHz，每次计数用时1/9 us)
void delay_us(INT32U nus);                      //微秒延时函数
void delay_ms(INT32U nms);	                    //毫秒延时函数
void OS_delayMs(volatile INT32U nms);           //任务延时函数
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec);//日时分秒延时，<=49天17时2分47秒
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif

/**************************************************望穿秋水*************************************************************/
