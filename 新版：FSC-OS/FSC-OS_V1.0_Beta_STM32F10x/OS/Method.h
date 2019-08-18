#ifndef _OS_METHOD_
#define _OS_METHOD_
#include "stdio.h"
#include "type.h"

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

os_error_code_type os_app_name_set(os_type_app_id *app,os_str8 *name_str); //APP��������
os_error_code_type os_app_prio_set(os_type_app_id *app,os_u32 prio); //APP���ȼ�����
os_error_code_type os_app_state_set(os_acb *os_acb_app,os_app_state app_state);//APP״̬���ú���
os_error_code_type os_app_thread_state_set(os_acb *os_acb_app,os_tcb *os_tcb_thread,os_thread_state thread_state); //APP�����߳�״̬����
os_error_code_type os_app_all_thread_state_set(os_acb *os_acb_app,os_thread_state thread_state); //APP�����߳�״̬����
os_error_code_type os_app_open(os_acb *os_acb_app);                //APP�������� ����ָ����APP����
os_error_code_type os_app_close(os_acb *os_acb_app);               //APP�رպ��� �ر�ָ����APP��ֹͣ����

#endif
