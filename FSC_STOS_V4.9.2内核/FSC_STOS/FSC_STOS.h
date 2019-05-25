/*--------------------------------------------版本: V4.9.2--------------------------------------------*/
#ifndef _FSC_STOS_H_
#define _FSC_STOS_H_

/*****************头文件选择*****************/
//#include "stm32l0xx.h"  //stm32l0  STD库
//#include "stm32l1xx.h"  //stm32l1  STD库

//#include "stm32f0xx_hal.h"//stm32f0  HAL库
//#include "stm32f0xx.h"  //stm32f0  STD库
//#include "stm32f1xx_hal.h"//stm32f1  HAL库
#include "stm32f10x.h"  //stm32f1  STD库
//#include "stm32f4xx_hal.h"//stm32f4  HAL库
//#include "stm32f4xx.h"  //stm32f4  STD库
//#include "stm32f7xx_hal.h"//stm32f7  HAL库
/********************************************/
      
/****************************************用户可自定义***********************************************************/
#define OS_RINNING_MODE          0        //OS运行模式：0-时间片运行  1-优先级运行  2-时间片+优先级运行
#define OS_HighSpeed_MODE        0        //0-低速模式(低内存占用)  1-高速模式(高内存占用) （高速模式暂时无用）
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/  
#define OS_MAX_TASKS             10        //任务数=用户任务数+2  任务数:3-65535  用户根据实际需要的任务数量修改
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_MEMORYPOOL_SIZE       4*1024   //内存池大小,单位：Byte(用户只需修改1024的大小)
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TIMER_SIZE               4        //系统虚拟定时器数量 (根据需要配置，1-65535，以下同)
#define FLAG_SIZE                2        //标志数量            
#define FLAG_GROUP_SIZE          2        //标志群数量         
#define MUTEX_SIZE               2        //互斥数量           
#define MBOX_SIZE                2        //邮箱数量  
#define MBQ_SIZE                 2        //邮箱队列缓存数量
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define OS_CMD_ALL_ENABLE        1        //剪裁全部系统指令(0-剪裁，1-不剪裁,下同)
#define OS_CMD_NSY_ENABLE        1        //剪裁系统无关的指令
#define OS_CMD_DISP_SYS_ENABLE   1        //剪裁系统状态信息指令显示(关闭显示能节省大量内存)
#define OS_CMD_DISP_NSY_ENABLE   1        //剪裁系统状态信息以外无关的指令显示
#define OS_TIMECOUNTSYS_ENABLE   1        //剪裁系统时间统计系统(剪裁可提高时间片模式下的内核切换速度)
#define OS_GET_SET_SW_ENABLE     1        //剪裁任务设置、获取、跳转
#define OS_SIGN_PP_ENABLE        1        //剪裁信号量(可提高内核切换速度)
#define OS_TIM_SYS_ENABLE        1        //剪裁系统虚拟定时器(可提高内核切换速度)
#define OS_REMOTE_ENABLE         0        //剪裁不常用函数
#define OS_SYSTIME_ENABLE        0        //剪裁系统时间RTC
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define TASK_NAME_LEN            32       //任务名       字符最大长度(如任务名超过32个字符请修改)
#define OS_CMD_STR_LEN           64       //系统指令     字符最大长度
#define OS_CMD_NUM_LEN           8        //系统指令字符串允许包含数字个数("prio=89//"为一个数字)
#define OS_FAULT_BACKUP_DATA_LEN 1        //系统故障数据备份长度
/*----------------------------系统任务参数--------------------------------*/			
#define OS_TIMESLICE_IDLE        1        //空闲任务时间切片，单位:微秒 ms
#define OS_TIMESLICE_MANAGER     1        //任务管理器任务时间切片,单位:微秒 ms

#define TaskIDLE_StkSize         32       //空闲任务堆栈大小
#define TaskManager_StkSize      160      //任务管理器任务堆栈大小

#define TaskIDLE_Prio            0        //空闲任务优先级(不能修改,放在此是为了让用户了解空闲任务的优先级)
#define TaskManager_Prio         1        //任务管理器任务优先级(如想指令能快速反应可提高优先级)
/*----------------------------故障检测系统--------------------------------*/
#define OS_FAULT_ENABLE          0        //剪裁故障检测系统(需要在stm32fxxx_it.c中注释掉HardFault_Handler()中断函数)
#define OS_FAULT_DISP_ENABLE     0        //剪裁故障检测系统提示
#define OS_Reset_Delay_Time      10       //系统重启延时,单位：s
#define OS_Shutdown_Delay_Time   10       //系统关闭延时,单位：s
#define OS_EndlessLoops_Times    1000     //连续运行该次数，则认为该任务陷入死循环无法切换到其他任务运行,单位：次数
#define OSFAULT_Reset_Enable     1        //是否允许故障后自动重启,0-不允许 1-允许
/***************************************************************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++状态定义区+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*---------------------任务状态(用户可用)----------------------*/
//State->LSB:
#define TASK_CREATING            0        //创建态
#define TASK_RUNNING             1        //运行态
#define TASK_PAUSING             2        //暂停态
#define TASK_BACKRUNNING         4        //后台运行态
#define TASK_DELETING            5        //删除态

//State->MSB:
#define TASK_UNLOCK              0        //无锁态
#define TASK_LOCK                1        //锁定态
/*--------信号量Pend函数第一个参数：等待类型(用户可用)---------*/
#define OSFlag_BPN               0        //阻塞等待新信号量(只响应Pend之后的信号)
#define OSFlag_BPO               1        //阻塞等待含旧信号量(含响应在Pend之前Post过的信号)
#define OSFlag_BPC               2        //阻塞等待响应累计信号量(累计Post所有次数，Pend读取一次，累计将减1次，有助于同步响应全部Post信号)
#define OSFlag_NBPC              3        //非阻塞等待响应累计信号量(累计Post所有次数，Pend读取一次，累计将减1次，有助于同步响应全部Post信号)(非阻塞时超时时间无效)
#define OSFlag_NBPN              4        //非阻塞等待新信号量(只响应Pend之后的信号)(非阻塞时超时时间无效)
																				 
#define OSFGroup_BPN             0        //阻塞等待新信号量(Pend读取最新的FlagGroup全部接收完成信号)
#define OSFGroup_NBPN            1        //非阻塞等待新信号量(Pend读取最新的FlagGroup全部接收完成信号)(非阻塞时超时时间无效)
																				 
#define OSMBox_BPN               0        //阻塞等待新信号量(读取Pend之后Post的邮件)
#define OSMBox_BPQ               1        //阻塞等待响应队列信号量(所有Post的邮件按队列保存起来，Pend读取时读取先发送的，先发先读，后发后读)
#define OSMBox_NBPN              2        //非阻塞等待响应新信号量(非队列)(读取Pend之后Post的邮件)(非阻塞时超时时间无效)
#define OSMBox_NBPQ              3        //非阻塞等待响应队列信号量(所有Post的邮件按队列保存起来，Pend读取时读取先发送的，先发先读，后发后读)(非阻塞时超时时间无效)
/*-----------------信号量系统标志位(用户不可用)------------------*/												     
#define _BIT_TimeOut             (1<<0)   //标志位0：延时时间超时
#define _BIT_TimeLess            (1<<1)   //标志位1：无限等待
#define _BIT_Flag                (1<<2)   //标志位2：OSFlag标志量等待
#define _BIT_FlagGroup           (1<<3)   //标志位3：OSFlagGroup标志量群等待
#define _BIT_Mutex               (1<<4)   //标志位4：OSMutex互锁量等待  
#define _BIT_MBox                (1<<5)   //标志位5：OSMBox邮件等待
/*-------------------系统逻辑状态(用户可用)--------------------*/	
#define OS_FALSE                 0        //假
#define OS_TRUE                  1        //真
/*-------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------OSprintf-------------------------------------------------------*/
#define OSprintf(fmt, ...) { OSSchedLock(); printf( fmt, ##__VA_ARGS__); OSSchedUnlock();}//OS提供的打印输出函数
/***************************************************************************************************************/
#define  FSC_STOS   //用于嵌入FSC_STOS的标识(在其他文件中判断FSC_STOS是否已定义来判断是否嵌入了FSC_STOS)
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
typedef  INT8  OSMEM;
#define  OSSTK __align(8) STK32U

#if (OS_CMD_ALL_ENABLE == 1) 
typedef struct
{
	INT8U  RXOK_Flag;
	INT8U  RX_COUNT;
	char   RX_BUFF[OS_CMD_STR_LEN];	
	INT32  Rx_Dig_Buff[OS_CMD_NUM_LEN];
}OS_CMD;
extern OS_CMD OS_Cmd; //系统指令数据接收(用户不可用)  
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
	INT8U  RunTime_second;//时间数据为了对齐均用INT8U 
	INT8U  RunTime_minute;
	INT8U  RunTime_hour;
	INT8U  RunTime_day;
	INT8U  RunTime_mouth;
	INT32U RunTime_year;//时间数据为了对齐均用INT16U,最后一个可为其他类型
  INT8U  ShutDownTime_second;//时间数据为了对齐均用INT8U 
	INT8U  ShutDownTime_minute;
	INT8U  ShutDownTime_hour;
	INT8U  ShutDownTime_day;
	INT8U  ShutDownTime_mouth;
	INT32U ShutDownTime_year;//时间数据为了对齐均用INT16U,最后一个可为其他类型
#endif	
	INT16U RunLastTask;
	INT16U TimeSliceCounter;
	INT32U ClockCounter;    //节拍累计
	INT32U ClockOverCounter;//节拍累计溢出累计
	INT16U IntNesting; 
	INT16U LockNesting; 
	INT32U CtxSwCtr; 
	INT16U TaskHighestRunLast[OS_MAX_TASKS];
	INT16U TaskHighestPrioLast;
	INT16U TaskRdyHighestPrio;
	INT16U TaskRunStateHighestPrio;
	INT16U TaskAllStateHighestPrio;
	INT32U TaskContinuRunTimeCounter;
	INT16U TaskNext;
	INT8U  TaskNextRunFlag;
#if (OS_GET_SET_SW_ENABLE == 1)		
	INT16U TaskSwitchBackNum;  
#endif	
	INT16U TaskCPUOccRateMax;
	INT32U TaskTimeSliceCounter;
	
#if (OS_TIM_SYS_ENABLE == 1)
	INT32U TIMER[TIMER_SIZE]; 
	INT32U TIMER_RELOAD[TIMER_SIZE]; 
	INT32U TIMER_ClockCounter[TIMER_SIZE];    //节拍累计
	INT32U TIMER_ClockOverCounter[TIMER_SIZE];//节拍累计溢出累计
#endif		

#if (OS_SIGN_PP_ENABLE == 1)	
	INT32U  POST_FLAG_COUNT[FLAG_SIZE];
	INT32U  POST_FGROUP_COUNT[FLAG_GROUP_SIZE];
	INT32U  POST_MBOX_COUNT[MBOX_SIZE];
	INT16U  POST_MBQ_COUNT[MBOX_SIZE];
	
	INT16U  FLAG[FLAG_SIZE][OS_MAX_TASKS];//不同任务可以等待相同的Flag  
	INT16U  FLAGPendType[FLAG_SIZE][OS_MAX_TASKS];//OSFlag等待类型
	INT16U  GROUPRXFLAG[FLAG_GROUP_SIZE][OS_MAX_TASKS];//标志量群全部接收标志
	INT8U   FlagGroupTable[FLAG_GROUP_SIZE][FLAG_SIZE];//1bit
	INT8U   ReturnFlagGroupTable[FLAG_SIZE];//查询FlagGroup成员情况的缓存数组
	INT16U  FlagGroupNameInTask[OS_MAX_TASKS];
	
  INT16U  MUTEX[MUTEX_SIZE]; 	
	INT16U  MutexTaskNum[MUTEX_SIZE];
	INT16U  MutexNameInTask[MUTEX_SIZE];
	
	INT16U  MBOXPendType[MBOX_SIZE][OS_MAX_TASKS];
	INT32U *MBOX[OS_MAX_TASKS*MBOX_SIZE];
	INT32U *MBQueue[MBQ_SIZE*MBOX_SIZE];
#endif	
}volatile OS_SYSTEM;
extern OS_SYSTEM OS_System; 

typedef struct 
{
	INT16U Prio;
	INT16U Amount;
	INT16U LastRunTaskNum;
	INT16U NextRunTaskNum;
	INT16U TaskTable[OS_MAX_TASKS];
}OS_TASKPRIOGROUP;
extern OS_TASKPRIOGROUP TaskPrioGroup[OS_MAX_TASKS];

typedef struct 
{
	STK32U    *StkPtr;//任务栈指针必须是第一个 
  STK32U    *StkAdd;	
	INT32U    StkSize;
#if (OS_CMD_ALL_ENABLE == 1) 		
	INT32U    StkFree;
	INT32U    StkUsed;
	INT32U    StkUsedMax;
	INT32U    TaskCPUOccRateCounter;	
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
	INT8U     TaskFlagBit;//bit0-超时 bit1-无限等待 bit2-等待OSFlag bit3-等待OSFlagGroup bit4-等待OSMutex bit5-等待MBox
}volatile OS_TCB; 
extern OS_TCB OSTCBTbl[OS_MAX_TASKS];  
extern OS_TCB *OSTCBRun; 
extern OS_TCB *OSTCBCur; 
extern OS_TCB *OSTCBNext;
/*
关于以上任务控制块结构体说明：
1.OSTCBTbl[OS_MAX_TASKS]
  OSTCBTbl[0]：空闲任务
  OSTCBTbl[1]：任务管理器任务
  OSTCBTbl[2]-OSTCBTbl[OS_MAX_TASKS-1]：用户任务
2.OSTCBRun当前正在运行的任务
3.OSTCBCur当前准备就绪的任务
4.OSTCBNext顺序模式下，正常情况下次要运行的任务(只在顺序模式下使用到)
5.说明：在系统触发上下文之后和产生新的调度之前的这段时间,OSTCBRun和OSTCBCur指向同一任务--当前正在运行的任务，
       在系统产生新的调度之后和触发上下文之前的这段时间,OSTCBRun指向--当前正在运行的任务
                                                   OSTCBCur指向--(准备就绪的任务)切换之后要运行任务 
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
extern OS_FAULT OS_Fault; //系统故障信息(用户可用)
#endif

/***********************************************系统相关函数***********************************************************/ 
                                             
/*---------------------------------------------有关系统移植函数-------------------------------------------------------*/
void OS_MAIN(void);//放在main函数里
void OSCmdRxByte(INT8U RxByte);//Cmd指令数据接收入口函数(放到芯片串口接收中断里,输入形参为串口接收字节数据)
/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------用户不可调用函数-------------------------------------------------------*/
void OS_INT_ENTER(void); //关闭总中断  (汇编接口)         
void OS_INT_EXIT(void);  //打开总中断  (汇编接口)  
void HardReset(void);    //硬件重启
void OSPendSVPulse(void);
void OSPendSVInit(void);
STK32U* OSTaskStkInit(void (*task),STK32U *p_tos);
void OS_CreatTaskIdle(void);
void OS_TaskIdle(void);
void OS_TaskManager(void);
void OSCreatTaskManager(void);
void OS_Information_Process(void);
INT16U OSHighestPrioTaskNumGet(void);           //获取已创建的所有状态任务中的最高优先级任务
INT16U OSRunStateHighestPrioTaskNumGet(void);   //获取处于运行态的最高优先级任务
INT16U OSRdyHighestPrioTaskNumGet(void);        //获取准备运行就绪最高优先级任务(返回先创建的最高优先级任务(如有同级存在))
INT16U OSNextRdyHighestPrioTaskNumGet(void);    //获取准备运行就绪最高优先级任务(返回未运行过的最高优先级任务(该优先级一个轮询周期内))
void OSStartUp(void); 
void OSInit(void); 
void OSSysTickInit(void);
void OS_SysTick_Handler(void);
void OSTaskCreate(char* taskstr,void (*task),INT32U stksize,INT32U tasktimeslice,INT16U taskprio,INT8U taskstate);//动态创建任务函数
void OSTaskDelete(void (*Taskx));               //动态删除任务函数
void OSTaskResume(void (*Taskx),INT8U taskstate);//动态恢复已删除的任务函数
void OSTaskCreateStatic(char* taskstr,void (*task),STK32U *stkptr,INT32U stksize,INT32U tasktimeslice,INT16U taskprio,INT8U taskstate);//静态创建任务函数
/*--------------------------------------用户可调用的与系统无关函数----------------------------------------------*/
INT32U OS_GetStringLength(char* p);             //获取字符串p的长度
INT8U CompareCmd(char* p);                      //比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p内容是否一致
INT32U OS_GetStringLengthWithChar(char* p,char ch);//获取字符ch在字符串p中的位置
INT32* OS_GetStringNum(char* p);                //从字符串p中提取数字保存到全局变量CmdNumBuff[]数组中
INT8U CompareCmdDig(char ch,char* p);           //比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，字符ch以前的内容是否一致
INT32 GrabCmdDig(INT8U n);                      //获取字符串p中的第n个数字(n=0开始)
/*-------------------------------------用户可调用与系统有关函数-------------------------------------------------*/
/*--------------常用系统功能函数-------------*/
void OSTimeSliceCounterReset(void);             //时间切片计数器清0(手动切换任务时，可选择需要将时间切片计数清0)
void OSSchedSwitch(void);                       //任务调度并切换任务(如不将时间切片清0，切换后的任务运行可能少于完整时间片时间)
void OSContextExchangeToTask(OS_TCB* tcb);      //切换到指定任务,输入参数，任务控制块 (该函数效率比OSTaskSwitch高)
void OSScheduler_Process(void);                 //任务调度器（只调度不切换）
void OSContextExchange(void);                   //上下文切换(切换任务环境,下个任务运行OSTCBCur指向的任务)
void OSSchedLock(void);                         //任务切换上锁函数(进入半裸机模式，OS部分功能失效)
void OSSchedUnlock(void);                       //任务切换解锁函数
void OSTaskSwitchBack(void* Taskx);             //任务跳转  带返回
void OSTaskSwitch(void* Taskx);                 //任务跳转  不带返回
void OSTaskPrioSet(void* Taskx,INT16U Taskprio);//任务优先级设置函数(每调用一次就会自动备份一次)
void OSTaskPrioBackup(void* Taskx);             //任务优先级备份函数
void OSTaskPrioBackupSet(void* Taskx,INT16U Taskprio);//任务备份优先级设置函数
INT16U OSTaskPrioBackupGet(void* Taskx);        //获取任务备份优先级
INT8U OSTaskStateSet(void* Taskx,INT8U TaskState);//设置任务状态,返回OS_FALSE-设置失败,OS_TRUE-设置成功

void OSEnterDriver(void);                       //开始保护驱动程序 驱动代码开头调用 (进入准裸机模式,此时只有delay_us()函数能正常使用)
void OSExitDriver(void);                        //结束保护驱动程序 驱动代码结尾调用 (恢复OS模式)

void OSStartRun(void);                          //系统开始运行(进入OS模式)
void OSStopRun(void);                           //系统停止运行(进入全裸机模式，不能使用OS所有资源，包括延时函数也不可使用)
void OSSleep(void);                             //系统休眠
void OSAwaken(void);                            //系统唤醒

INT16U* OSRunTimeGet(void);                     //获取系统累计运行时间,指针成员(偏移得到)分别为：ms、s、m、h、d、m、y。
INT8U OSTaskStateGet(void* Taskx);              //获取任务状态 (高4位为锁定状态，低4位为运行状态)
INT16U OSTaskPrioGet(void* Taskx);              //获取任务优先级
INT8U OSLockStateGet(void);                     //获取系统中是否有任务处于锁定状态，返回OS_TRUE是，OS_FALSE否
void OSLockStateReset(void);                    //解锁任务锁定函数
INT32U OSTimerValGet(void);                     //获取系统定时器当前计数值(默认为芯片主频/8(8分频)，每次计数用时(主频/8)us)
INT32U OSTimeSliceValGet(void);                 //获取OS节拍计数器计数值(节拍固定为1ms，可利用此获取延时时间)
void OSTimeSliceValSet(INT32U time);            //设置时间切片节拍计数器计数值
INT32U OSClockValGet(void);                     //获取系统节拍计数值
void OSClockValSet(INT32U time);                //设置系统节拍计数值

/*-----------=--动态内存管理函数--------------*/
void *FSC_Malloc(size_t noOfBytes);//内存申请(单位:byte)
void FSC_Free(void* ptr);//内存释放(输入地址必须是申请得到的地址)
INT32U FSC_MemoryFreeSizeGet(void);//获取内存池剩余内存(单位：Byte)

/*--------------虚拟定时器函数---------------*/
void OSTimerReloadSet(INT16U TNum,INT32U time); //设置虚拟系统定时器自动重装载定时值(虚拟系统定时器中断中不可用)
void OSTimerValueSet(INT16U TNum,INT32U time);  //设置虚拟系统定时器定时值
void OSTimerReloadSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//虚拟系统定时器自动重装载定时值日时分秒设置,<=49天17时2分47秒
void OSTimerValueSetDHMS(INT16U TNum,INT8U day,INT8U hour,INT8U min,INT8U sec);//虚拟系统定时器定时值日时分秒设置,<=49天17时2分47秒
INT32U OSTimerValueGet(INT16U TNum);            //获取虚拟系统定时器当前计数值
INT8U OSTimerStateGet(INT16U TNum);             //获取虚拟系统定时器当前计时状态,OS_TRUE-定时完成，OS_FALSE-定时未完成

/*---------------信号量函数-----------------*/
void OSFlagPost(INT16U FNum);                   //发送标志量
INT8U OSFlagPend(INT8U pendtype,INT16U FNum,INT32U timeout);//等待标志量,返回OS_FALSE-等待超时，OS_TRUE-接收到Post
void OSFlagAddToGroup(INT16U FGNum,INT16U FNum);//添加标志量成员至标志群
INT8U OSFlagGroupPend(INT8U pendtype,INT16U FGNum,INT32U timeout);//标志量群等待,返回OS_FALSE-等待超时，OS_TRUE-接收到所有成员Post
INT8U* OSFlagGroupPendTableGet(INT16U FGNum);   //获取群等待成员
void OSMutexUnlockPost(INT16U MNum);            //发送放弃互斥量--解锁
INT8U OSMutexLockPend(INT16U MNum,INT32U timeout);//等待获取互斥量--锁定,返回OS_FALSE-等待超时，OS_TRUE-得到互斥使用权
INT16U OSMutexBlockTaskGet(INT16U MNum);        //获取Mutex当前阻塞的任务
void OSMboxPost(INT16U MNum,void* fp);          //发送邮件(地址)
void* OSMboxPend(INT8U pendtype,INT16U MNum,INT32U timeout);//等待邮件,返回OS_FALSE-等待超时，OS_TRUE-接收到Post

/*---------------延时函数------------------*/
//普通延时函数(中断里可用)
void delay_us(INT32U nus);                      //微秒延时函数,中断中可用
void delay_ms(INT32U nms);	                    //毫秒延时函数,中断中可用
//任务延时函数
void OS_delayMs(volatile INT32U nms);           //任务延时函数,中断中不可用
void OS_delayDHMS(INT8U day,INT8U hour,INT8U min,INT8U sec);//日时分秒延时，<=49天17时2分47秒,中断中不可用

/*---------系统关闭倒计时设置函数-----------*/
void OS_ShutDown_DelayTime_Set_YMDHMS(INT32U year,INT8U mouth,INT16U day,INT8U hour,INT8U minute,INT8U second); //设置系统关闭定时时间
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/
#endif

/**************************************************望穿秋水*************************************************************/
