#ifndef _OS_LIST_
#define _OS_LIST_
#include "os.h"

//链表初使化
void os_timer_list_init(void); //定时器链表初使化
void os_thread_timer_list_init(void);
void os_thread_timer_create(void); //thread定时器创建
//OS_TCB链表
void os_tcb_list_base_head_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb);//在参考对象之前增加os_tcb节点
void os_tcb_list_base_behind_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb);//在参考对象之后增加os_tcb节点
void os_tcb_list_erase(os_tcb *list_head,os_tcb *del_tcb);//os_tcb节点删除
//OS_TCB_PRIO链表
void os_tcb_prio_sort_table_list_insert(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct,u32 num); //任意位置插入
void os_tcb_prio_sort_table_list_erase(u32 num); //任意位置删除
void os_tcb_prio_sort_table_list_struct_erase(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct); //任意结构体删除
void os_tcb_prio_sort_table_list_push_head(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct);//头部插入
void os_tcb_prio_sort_table_list_pop_head(void);//头部删除
void os_tcb_prio_sort_table_list_push_rear(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct);//尾部插入
void os_tcb_prio_sort_table_list_pop_rear(void);//尾部删除
//延时链表
void os_timer_list_insert(os_timer *os_timer_struct,u32 num); //任意位置插入
void os_timer_list_erase(u32 num); //任意位置删除
void os_timer_list_strutc_erase(os_timer *os_timer_struct); //任意结构体删除
void os_timer_list_push_head(os_timer *os_timer_struct);//头部插入
void os_timer_list_pop_head(void);//头部删除
void os_timer_list_push_rear(os_timer *os_timer_struct);//尾部插入
void os_timer_list_pop_rear(void);//尾部删除
void os_thread_timer_erase(os_timer* os_thread_timer_struct); //thread延时定时器删除
os_timer* os_thread_timer_free_get(void); //app延时定时器空闲获取
void os_thread_timer_recycle(os_timer *os_thread_timer_struct); //thread延时定时器回收
void os_timer_add(os_timer* timer);//添加定时

#endif
