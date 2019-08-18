#ifndef _OS_TYPE_
#define _OS_TYPE_
#include "conf.h"

typedef char  os_str8;

typedef signed char  os_s8;            
typedef signed short os_s16;           
typedef signed int   os_s32;   
typedef signed long  os_s64; 
 
typedef unsigned char  os_u8;            
typedef unsigned short os_u16;           
typedef unsigned int   os_u32;  
typedef unsigned long  os_u64;    

typedef unsigned int   stk32;

typedef unsigned char  os_mem;

/*-----------------------------------*/
#define  os_stk32 __align(8) stk32

//错误代码
typedef enum{
  error_code_type__01,
	error_code_type__02,
}os_error_code_type;

//APP状态
typedef enum{
  os_app_state_creating,                         //创建态
	os_app_state_readying,                         //就绪态
	os_app_state_pausing,                          //暂停态
	os_app_state_delaying,                         //延时态	
	os_app_state_blocking,                         //阻塞态
  os_app_state_backrunning,                      //后台运行态
	os_app_state_frontrunning,                     //前台运行态
	os_app_state_stopping,                         //停止运行态
  os_app_state_deleting,                         //删除态
}os_app_state;
//Thread状态
typedef enum{
  os_thread_state_creating,                       //创建态
  os_thread_state_pausing,                        //暂停态
	os_thread_state_readying,                       //就绪态	
	os_thread_state_delaying,                       //延时态
	os_thread_state_blocking,                       //阻塞态
  os_thread_state_backrunning,                    //后台运行态
	os_thread_state_frontrunning,                   //前台运行态
	os_thread_state_stopping,                       //停止运行态
  os_thread_state_deleting,                       //删除态
}os_thread_state;
//优先及链表状态
typedef enum{
  os_thread_prio_state_readying,                       //就绪态
  os_thread_prio_state_running,                        //运行态
  os_thread_prio_state_pausing,                        //暂停态
  os_thread_prio_state_backrunning,                    //后台运行态
	os_thread_prio_state_frontrunning,                   //前台运行态
	os_thread_prio_state_stopping,                       //停止运行态
  os_thread_prio_state_deleting,                       //删除态
}os_thread_prio_state;

//定时器状态
typedef enum{
  os_tiemr_state_running,                        //运行态
  os_tiemr_state_pausing,                        //暂停态
	os_tiemr_state_stopping,                       //停止态
}os_tiemr_state;

//系统状态
typedef enum{
  os_state_running,                        //运行态
  os_state_pausing,                        //暂停态
	os_state_stopping,                       //停止态
}os_state;

//系统布尔值
/*-------------------系统逻辑状态(用户可用)--------------------*/	
typedef enum
{
 os_false,                     //假
 os_true,                      //真
}os_bool;
/*-------------------------------------------------------------------------------------------------------------*/
typedef enum 
{
  fault_id__01,
	fault_id__02,
	fault_id__03,	
}os_type_fault;

typedef enum 
{
	thread_type__app,   //应用级线程
  thread_type__system,//系统级线程
	thread_type__other,	//其他
}os_thread_type;
typedef enum 
{
	app_type__app,   //应用级应用
  app_type__system,//系统级应用
	app_type__other,	//其他
}os_app_type;

typedef enum 
{
  timer_type__app,
	timer_type__thread,
	timer_type__softtimer,	
	timer_type__signal,
}os_timer_type;

typedef enum 
{
  pend_type__block,
	pend_type__unblock,	
}os_pend_type;

struct os_timer_struct
{
	os_u32 type;                 //定时器类型
	os_u32 para[8];            //参数
	os_u32 clock_counter;      //定时器累计变量
	os_u32 id;             //定时器列表节点编号
	struct os_timer_struct *last;
	struct os_timer_struct *next;
};
typedef struct os_timer_struct os_timer;


//线程控制块类型定义
struct os_tcb_struct
{
	stk32      *stk_ptr;   //线程栈指针 (必须是第一个)  
	os_u32     *thread;    //线程首地址
	os_u32     *stk_addr;
	os_u32     stk_size_sum;
	os_u32     stk_size_used;	
	os_u32     stk_size_free;
  os_u32     delaytime;  //线程延时时间	
	os_u32     prio;       //线程优先级
	os_u32     prio_backup;//备份优先级
	os_u32     app_prio;   //任务优先级		
	os_u32     timeslice;  //线程时间片
	os_u32     cpu_occrate_counter; //cpu占用率统计
	os_u32     id;         //线程局部编号(APP内)
	os_u32     global_id;  //线程全局编号(系统内唯一ID)     
	os_u32     *app_acb_addr; //线程所属app的ACB地址
	os_u16     cpu_occrate_per100; //cpu占用率(百分比)
	os_u8      thread_name[THREAD_NAME_LEN];//线程名
	os_u8      wr;         //线程读写权限
	os_u8      lock;       //线程上锁
	os_thread_type type;   //线程类型	
	os_thread_state     state;      //线程状态
	struct os_tcb_struct *last;//上个线程
	struct os_tcb_struct *next;//下个线程
}; 
typedef struct os_tcb_struct os_tcb,os_type_thread_id;

struct os_tcb_prio_sort_table_struct
{
	os_thread_prio_state state;
	os_u32 id;
  os_u32 prio;
	os_u32 list_same_prio_len;
	os_tcb *list_same_prio_cur;
	os_tcb *list_same_prio_front;	
	os_tcb *list_same_prio_rear;
	struct os_tcb_prio_sort_table_struct *last;//上个线程
	struct os_tcb_prio_sort_table_struct *next;//下个线程
}; 
typedef struct os_tcb_prio_sort_table_struct os_tcb_prio_sort_table;

struct os_app_tcb_struct
{ 
	os_tcb *thread;
	struct os_app_tcb_struct *next;//下个线程
};
typedef struct os_app_tcb_struct os_app_tcb;
	
struct os_acb_struct
{ 
	os_app_state  state;    //app状态
	os_u32     *app;	
	os_app_tcb  *tcb_list;
	os_u32     thread_len;
	os_u32     stk_size_sum;
	os_u32     stk_size_used;
	os_u32     stk_size_free;
  os_u32     delaytime;//app延时时间	
	os_u32     thread_delaytime;//app延时时间	
	os_u32     prio;     //app优先级（只限系统级应用程序，普通应用程序不开放）
	os_u32     prio_backup;//备份优先级
	os_u32     id;       //app编号id
	os_u32     cpu_occrate_per100; //cpu占用率(百分比)
	os_u8      app_name[APP_NAME_LEN];
	os_u8      wr;       //app读写权限
	os_u8      lock;     //app上锁
	os_app_type type; //app类型
	struct os_acb_struct *last;//上个app
	struct os_acb_struct *next;//下个app
}; 
typedef struct os_acb_struct os_acb,os_type_app_id;

struct os_signal_tcb_struct
{
	os_tcb *os_tcb_id;
	struct os_signal_tcb_struct *next;
};
typedef struct os_signal_tcb_struct os_signal_tcb;

struct os_sem_struct
{
	os_signal_tcb *os_tcb_id_list;
	os_u32 value;
	struct os_sem_struct *next;
};
typedef struct os_sem_struct os_sem;

struct os_flag_struct 
{
	os_tcb *os_tcb_id;
	os_u32 value;
	struct os_flag_struct *next;	
};
typedef struct os_flag_struct os_flag;

struct os_mutex_struct 
{
	os_signal_tcb *os_tcb_id_list;
	os_tcb *using_tcb_id;
	os_u32 lock;
	struct os_mutex_struct *next;
};
typedef struct os_mutex_struct os_mutex;

struct os_mbox_struct 
{
	os_tcb *os_tcb_id;
	os_u32 value;
	os_u32 *pointer;
	struct os_mbox_struct *next;
};
typedef struct os_mbox_struct os_mbox;


//系统变量类型定义
typedef struct 
{
	os_state state;                   //os系统状态
	os_bool os_bool__first_enter;     //第一次进入系统标志
	os_u32 current_prio;
	os_tcb_prio_sort_table *os_tcb_highest_prio_sort_table;
	os_u32 os_irq_lock;                //os中断换锁定变量
	os_u32 app_time_slice; 
	os_u32 thread_time_slice; 
	os_u32 os_clock_counter;           //os时钟节拍累计变量
	os_u32 cpu_occrate_counter;        //总cpu占用率统计
	os_u16 cpu_occrate_per100;         //总cpu占用率
	os_u16 cpu_occrate_per100_max;     //总cpu历史最大占用率
}os_para;
  
typedef struct 
{
	os_u32 run_state;               //app运行标志变量
	os_u32 sche_lock;               //thread切换锁定变量 
	os_u32 acb_list_len;            //app控制块链表长度
	os_u32 id_counter;              //app id标识
}os_app_para;

typedef struct 
{
	os_thread_state state;               //thread运行标志变量
	os_u32 sche_lock;               //thread切换锁定变量 
	os_u32 time_slice;              //任务时间切片(为系统时钟节拍倍数,任务持续运行的时间)
	os_u32 tcb_list_len;            //thread控制块链表长度
	os_u32 id_counter;              //thread 局部id标识
	os_u32 global_id_counter;       //thread 全局id标识
}os_thread_para; 

typedef struct 
{
	os_u32 prio;
	os_u32 list_len;                //thread控制块链表长度
	os_u32 id_counter;              //thread 局部id标识
}os_thread_prio_sort_table_para;

typedef struct 
{
	os_tiemr_state state;            //thread运行标志变量
	os_u32 timer_list_len;           //定时器链表长度
	os_u32 thread_timer_list_len;    //thread延时定时器链表长度
	os_u32 app_timer_list_len;       //app延时定时器链表长度
}os_timer_para;

#endif
