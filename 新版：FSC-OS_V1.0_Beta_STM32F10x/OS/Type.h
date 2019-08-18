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

//�������
typedef enum{
  error_code_type__01,
	error_code_type__02,
}os_error_code_type;

//APP״̬
typedef enum{
  os_app_state_creating,                         //����̬
	os_app_state_readying,                         //����̬
	os_app_state_pausing,                          //��̬ͣ
	os_app_state_delaying,                         //��ʱ̬	
	os_app_state_blocking,                         //����̬
  os_app_state_backrunning,                      //��̨����̬
	os_app_state_frontrunning,                     //ǰ̨����̬
	os_app_state_stopping,                         //ֹͣ����̬
  os_app_state_deleting,                         //ɾ��̬
}os_app_state;
//Thread״̬
typedef enum{
  os_thread_state_creating,                       //����̬
  os_thread_state_pausing,                        //��̬ͣ
	os_thread_state_readying,                       //����̬	
	os_thread_state_delaying,                       //��ʱ̬
	os_thread_state_blocking,                       //����̬
  os_thread_state_backrunning,                    //��̨����̬
	os_thread_state_frontrunning,                   //ǰ̨����̬
	os_thread_state_stopping,                       //ֹͣ����̬
  os_thread_state_deleting,                       //ɾ��̬
}os_thread_state;
//���ȼ�����״̬
typedef enum{
  os_thread_prio_state_readying,                       //����̬
  os_thread_prio_state_running,                        //����̬
  os_thread_prio_state_pausing,                        //��̬ͣ
  os_thread_prio_state_backrunning,                    //��̨����̬
	os_thread_prio_state_frontrunning,                   //ǰ̨����̬
	os_thread_prio_state_stopping,                       //ֹͣ����̬
  os_thread_prio_state_deleting,                       //ɾ��̬
}os_thread_prio_state;

//��ʱ��״̬
typedef enum{
  os_tiemr_state_running,                        //����̬
  os_tiemr_state_pausing,                        //��̬ͣ
	os_tiemr_state_stopping,                       //ֹ̬ͣ
}os_tiemr_state;

//ϵͳ״̬
typedef enum{
  os_state_running,                        //����̬
  os_state_pausing,                        //��̬ͣ
	os_state_stopping,                       //ֹ̬ͣ
}os_state;

//ϵͳ����ֵ
/*-------------------ϵͳ�߼�״̬(�û�����)--------------------*/	
typedef enum
{
 os_false,                     //��
 os_true,                      //��
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
	thread_type__app,   //Ӧ�ü��߳�
  thread_type__system,//ϵͳ���߳�
	thread_type__other,	//����
}os_thread_type;
typedef enum 
{
	app_type__app,   //Ӧ�ü�Ӧ��
  app_type__system,//ϵͳ��Ӧ��
	app_type__other,	//����
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
	os_u32 type;                 //��ʱ������
	os_u32 para[8];            //����
	os_u32 clock_counter;      //��ʱ���ۼƱ���
	os_u32 id;             //��ʱ���б�ڵ���
	struct os_timer_struct *last;
	struct os_timer_struct *next;
};
typedef struct os_timer_struct os_timer;


//�߳̿��ƿ����Ͷ���
struct os_tcb_struct
{
	stk32      *stk_ptr;   //�߳�ջָ�� (�����ǵ�һ��)  
	os_u32     *thread;    //�߳��׵�ַ
	os_u32     *stk_addr;
	os_u32     stk_size_sum;
	os_u32     stk_size_used;	
	os_u32     stk_size_free;
  os_u32     delaytime;  //�߳���ʱʱ��	
	os_u32     prio;       //�߳����ȼ�
	os_u32     prio_backup;//�������ȼ�
	os_u32     app_prio;   //�������ȼ�		
	os_u32     timeslice;  //�߳�ʱ��Ƭ
	os_u32     cpu_occrate_counter; //cpuռ����ͳ��
	os_u32     id;         //�ֲ߳̾����(APP��)
	os_u32     global_id;  //�߳�ȫ�ֱ��(ϵͳ��ΨһID)     
	os_u32     *app_acb_addr; //�߳�����app��ACB��ַ
	os_u16     cpu_occrate_per100; //cpuռ����(�ٷֱ�)
	os_u8      thread_name[THREAD_NAME_LEN];//�߳���
	os_u8      wr;         //�̶߳�дȨ��
	os_u8      lock;       //�߳�����
	os_thread_type type;   //�߳�����	
	os_thread_state     state;      //�߳�״̬
	struct os_tcb_struct *last;//�ϸ��߳�
	struct os_tcb_struct *next;//�¸��߳�
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
	struct os_tcb_prio_sort_table_struct *last;//�ϸ��߳�
	struct os_tcb_prio_sort_table_struct *next;//�¸��߳�
}; 
typedef struct os_tcb_prio_sort_table_struct os_tcb_prio_sort_table;

struct os_app_tcb_struct
{ 
	os_tcb *thread;
	struct os_app_tcb_struct *next;//�¸��߳�
};
typedef struct os_app_tcb_struct os_app_tcb;
	
struct os_acb_struct
{ 
	os_app_state  state;    //app״̬
	os_u32     *app;	
	os_app_tcb  *tcb_list;
	os_u32     thread_len;
	os_u32     stk_size_sum;
	os_u32     stk_size_used;
	os_u32     stk_size_free;
  os_u32     delaytime;//app��ʱʱ��	
	os_u32     thread_delaytime;//app��ʱʱ��	
	os_u32     prio;     //app���ȼ���ֻ��ϵͳ��Ӧ�ó�����ͨӦ�ó��򲻿��ţ�
	os_u32     prio_backup;//�������ȼ�
	os_u32     id;       //app���id
	os_u32     cpu_occrate_per100; //cpuռ����(�ٷֱ�)
	os_u8      app_name[APP_NAME_LEN];
	os_u8      wr;       //app��дȨ��
	os_u8      lock;     //app����
	os_app_type type; //app����
	struct os_acb_struct *last;//�ϸ�app
	struct os_acb_struct *next;//�¸�app
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


//ϵͳ�������Ͷ���
typedef struct 
{
	os_state state;                   //osϵͳ״̬
	os_bool os_bool__first_enter;     //��һ�ν���ϵͳ��־
	os_u32 current_prio;
	os_tcb_prio_sort_table *os_tcb_highest_prio_sort_table;
	os_u32 os_irq_lock;                //os�жϻ���������
	os_u32 app_time_slice; 
	os_u32 thread_time_slice; 
	os_u32 os_clock_counter;           //osʱ�ӽ����ۼƱ���
	os_u32 cpu_occrate_counter;        //��cpuռ����ͳ��
	os_u16 cpu_occrate_per100;         //��cpuռ����
	os_u16 cpu_occrate_per100_max;     //��cpu��ʷ���ռ����
}os_para;
  
typedef struct 
{
	os_u32 run_state;               //app���б�־����
	os_u32 sche_lock;               //thread�л��������� 
	os_u32 acb_list_len;            //app���ƿ�������
	os_u32 id_counter;              //app id��ʶ
}os_app_para;

typedef struct 
{
	os_thread_state state;               //thread���б�־����
	os_u32 sche_lock;               //thread�л��������� 
	os_u32 time_slice;              //����ʱ����Ƭ(Ϊϵͳʱ�ӽ��ı���,����������е�ʱ��)
	os_u32 tcb_list_len;            //thread���ƿ�������
	os_u32 id_counter;              //thread �ֲ�id��ʶ
	os_u32 global_id_counter;       //thread ȫ��id��ʶ
}os_thread_para; 

typedef struct 
{
	os_u32 prio;
	os_u32 list_len;                //thread���ƿ�������
	os_u32 id_counter;              //thread �ֲ�id��ʶ
}os_thread_prio_sort_table_para;

typedef struct 
{
	os_tiemr_state state;            //thread���б�־����
	os_u32 timer_list_len;           //��ʱ��������
	os_u32 thread_timer_list_len;    //thread��ʱ��ʱ��������
	os_u32 app_timer_list_len;       //app��ʱ��ʱ��������
}os_timer_para;

#endif
