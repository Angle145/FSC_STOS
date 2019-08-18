#ifndef _OS_H_
#define _OS_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "conf.h"
#include "type.h"
#include "mem.h"
#include "list.h"
#include "signal.h"
#include "global.h"
#include "method.h"
#include "shell.h"

//�ط�װprintf����
#define os_printf(fmt, ...) { os_thread_sched_lock(); if(os_shell_cmd.bool_enter_sys==os_false) printf( fmt, ##__VA_ARGS__); os_thread_sched_unlock();}//OS�ṩ�Ĵ�ӡ�������

//timer����
extern os_timer os_timer_struct_list_front;
extern os_timer os_timer_struct_list_rear;
extern os_timer *os_timer_list_front;
extern os_timer *os_timer_list_rear;
extern os_timer *os_thread_timer_list_front;
extern os_timer *os_thread_timer_list_rear;


//ACB����
extern os_acb *os_acb_list_front;
extern os_acb *os_acb_list_rear;
extern os_acb *os_acb_cur;  //��ǰ�������е�Ӧ�ó���ACB
extern os_acb *os_acb_rdy;  //�¸��������е�Ӧ�ó���ACB


//TCB����
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_front;
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_rear;

extern os_tcb *os_tcb_cur;  //��ǰ�������е��߳�TCB
extern os_tcb *os_tcb_rdy;  //�¸��������е��߳�TCB


extern os_para os_p;//ϵͳ����
extern os_app_para os_app_p;
extern os_thread_para os_thread_p;
extern os_thread_prio_sort_table_para os_thread_prio_sort_table_p;
extern os_timer_para os_timer_p;

void os_main(void);
void os_core_enter(void);                             //�ر�ȫ���жϺ���(�ڻ���У��û�����ʹ��)           
void os_core_exit(void);                              //��ȫ���жϺ���(�ڻ���У��û�����ʹ��)  
void os_psp_reset(void);                              //��ʹ��PSPָ��(�ڻ����) 
void os_cpu_interrupt_disable(void);                  //�ر�ȫ���жϺ���
void os_cpu_interrupt_enable(void);                   //��ȫ���жϺ���
void os_user_init(void);
void os_init_and_startup(void);                       //ϵͳ��ʹ������������(�ڻ����) 
void os_thread_same_prio_scheduler(void);             //�̵߳�����(OS����)
void os_thread_highest_prio_scheduler(void);          //�̵߳�����(OS����)
void os_pendsv_pulse(void);                           //���������л�
void os_thread_same_prio_sched_and_switch(void);      //�̵߳��Ȳ��л�����  
void os_thread_highest_prio_sched_and_switch(void);   //�̵߳��Ȳ��л����� 
void os_timer_counter_process(void);                  //ϵͳ��ʱ������
stk32* os_thread_stk_init(void* thread,stk32 *topstkptr); //�̶߳�ջ��ʹ������
os_type_app_id* os_app_new_init(void);
void os_app_new_create(os_type_app_id* app_id);
os_type_thread_id* os_thread_new_create(os_type_app_id* app,void* thread,os_str8 *str_name,os_u32 stk_size,os_u32 slice_tick,os_u32 prio,void *para);     //���񴴽�����                           
void os_thread_sched_lock(void);                             //�߳��л���������                         
void os_thread_sched_unlock (void);                          //�߳��л���������
void os_tcb_ptr_init(void);
os_tcb* os_tcb_same_prio_next_rdy_get(void);//�ӵ�ǰ������ȼ����л�ȡ�¸�˳�����е��߳�
os_tcb* os_tcb_highest_prio_next_rdy_get(void);//�ȴ�������ȼ����в���������ȼ����飬�ٴ�������ȼ������л�ȡ�¸�˳�����е��߳�
os_u32 os_clock_counter_get(void);                          //��ȡϵͳʱ�ӽ���������
void delay_us(os_u32 us);                            //΢����ʱ����
void delay_ms(os_u32 ms);	                           //������ʱ����
void os_thread_delay(os_u32 time_ticks);             //�߳���ʱ����
#endif
