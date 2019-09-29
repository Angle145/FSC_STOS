#ifndef _OS_LIST_
#define _OS_LIST_
#include "os.h"

//�����ʹ��
void os_timer_list_init(void); //��ʱ�������ʹ��
void os_thread_timer_list_init(void);
void os_thread_timer_create(void); //thread��ʱ������
//OS_TCB����
void os_tcb_list_base_head_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb);//�ڲο�����֮ǰ����os_tcb�ڵ�
void os_tcb_list_base_behind_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb);//�ڲο�����֮������os_tcb�ڵ�
void os_tcb_list_erase(os_tcb *list_head,os_tcb *del_tcb);//os_tcb�ڵ�ɾ��
//OS_TCB_PRIO����
void os_tcb_prio_sort_table_list_insert(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct,u32 num); //����λ�ò���
void os_tcb_prio_sort_table_list_erase(u32 num); //����λ��ɾ��
void os_tcb_prio_sort_table_list_struct_erase(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct); //����ṹ��ɾ��
void os_tcb_prio_sort_table_list_push_head(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct);//ͷ������
void os_tcb_prio_sort_table_list_pop_head(void);//ͷ��ɾ��
void os_tcb_prio_sort_table_list_push_rear(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct);//β������
void os_tcb_prio_sort_table_list_pop_rear(void);//β��ɾ��
//��ʱ����
void os_timer_list_insert(os_timer *os_timer_struct,u32 num); //����λ�ò���
void os_timer_list_erase(u32 num); //����λ��ɾ��
void os_timer_list_strutc_erase(os_timer *os_timer_struct); //����ṹ��ɾ��
void os_timer_list_push_head(os_timer *os_timer_struct);//ͷ������
void os_timer_list_pop_head(void);//ͷ��ɾ��
void os_timer_list_push_rear(os_timer *os_timer_struct);//β������
void os_timer_list_pop_rear(void);//β��ɾ��
void os_thread_timer_erase(os_timer* os_thread_timer_struct); //thread��ʱ��ʱ��ɾ��
os_timer* os_thread_timer_free_get(void); //app��ʱ��ʱ�����л�ȡ
void os_thread_timer_recycle(os_timer *os_thread_timer_struct); //thread��ʱ��ʱ������
void os_timer_add(os_timer* timer);//��Ӷ�ʱ

#endif
