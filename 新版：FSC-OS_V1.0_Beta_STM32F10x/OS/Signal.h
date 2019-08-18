#ifndef _OS_SIGNAL_
#define _OS_SIGNAL_
#include "type.h"
#include "mem.h"

os_sem* os_sem_create(void);            //创建信号量，返回信号量类型ID(ID用于识别是哪一个信号量)
os_flag* os_flag_create(void);          //创建标志量，返回标志量类型ID(ID用于识别是哪一个信号量)
os_mutex* os_mutex_create(void);        //创建互斥量，返回互斥量类型ID(ID用于识别是哪一个信号量) 
os_mbox* os_mbox_create(void);          //创建邮箱量，返回邮箱量类型ID(ID用于识别是哪一个信号量)

void os_sem_delete(os_sem *os_sem_id);           //删除信号量，输入信号量类型ID
void os_flag_delete(os_flag *os_flag_id);        //删除标志量，输入标志量类型ID
void os_mutex_delete(os_mutex *os_mutex_id);     //删除互斥量，输入互斥量类型ID
void os_mbox_delete(os_mbox *os_mbox_id);        //删除邮箱量，输入邮箱量类型ID

os_u32 os_sem_pend(os_sem *os_sem_id,os_pend_type ptype,os_u32 ptime);      //等待信号量
os_u32 os_flag_pend(os_flag *os_flag_id,os_pend_type ptype,os_u32 ptime);   //等待标志量
os_u32 os_mutex_pend(os_mutex *os_mutex_id,os_pend_type ptype,os_u32 ptime);//等待互斥量
void* os_mbox_pend(os_mbox *os_mbox_id,os_pend_type ptype,os_u32 ptime);   //等待邮箱量

void os_sem_post(os_sem *os_sem_id);                 //发送信号量
void os_flag_post(os_flag *os_flag_id);              //发送标志量
void os_mutex_post(os_mutex *os_mutex_id);           //发送互斥量
void os_mbox_post(os_mbox *os_mbox_id);              //发送邮箱量

//void os_signal_state_check_process(void);
#endif
