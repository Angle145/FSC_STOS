#ifndef _OS_METHOD_
#define _OS_METHOD_
#include "stdio.h"
#include "type.h"

//timer链表
extern os_timer os_timer_struct_list_front;
extern os_timer os_timer_struct_list_rear;
extern os_timer *os_timer_list_front;
extern os_timer *os_timer_list_rear;
extern os_timer *os_thread_timer_list_front;
extern os_timer *os_thread_timer_list_rear;


//ACB链表
extern os_acb *os_acb_list_front;
extern os_acb *os_acb_list_rear;
extern os_acb *os_acb_cur;  //当前正在运行的应用程序ACB
extern os_acb *os_acb_rdy;  //下个即将运行的应用程序ACB


//TCB链表
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_front;
extern os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_rear;

extern os_tcb *os_tcb_cur;  //当前正在运行的线程TCB
extern os_tcb *os_tcb_rdy;  //下个即将运行的线程TCB


extern os_para os_p;//系统变量
extern os_app_para os_app_p;
extern os_thread_para os_thread_p;
extern os_thread_prio_sort_table_para os_thread_prio_sort_table_p;
extern os_timer_para os_timer_p;

os_error_code_type os_app_name_set(os_type_app_id *app,os_str8 *name_str); //APP名字设置
os_error_code_type os_app_prio_set(os_type_app_id *app,os_u32 prio); //APP优先级设置
os_error_code_type os_app_state_set(os_acb *os_acb_app,os_app_state app_state);//APP状态设置函数
os_error_code_type os_app_thread_state_set(os_acb *os_acb_app,os_tcb *os_tcb_thread,os_thread_state thread_state); //APP单个线程状态设置
os_error_code_type os_app_all_thread_state_set(os_acb *os_acb_app,os_thread_state thread_state); //APP所有线程状态设置
os_error_code_type os_app_open(os_acb *os_acb_app);                //APP启动函数 启动指定的APP运行
os_error_code_type os_app_close(os_acb *os_acb_app);               //APP关闭函数 关闭指定的APP，停止运行

#endif
