#ifndef _OS_SIGNAL_
#define _OS_SIGNAL_
#include "type.h"
#include "mem.h"

os_sem* os_sem_create(void);            //�����ź����������ź�������ID(ID����ʶ������һ���ź���)
os_flag* os_flag_create(void);          //������־�������ر�־������ID(ID����ʶ������һ���ź���)
os_mutex* os_mutex_create(void);        //���������������ػ���������ID(ID����ʶ������һ���ź���) 
os_mbox* os_mbox_create(void);          //��������������������������ID(ID����ʶ������һ���ź���)

void os_sem_delete(os_sem *os_sem_id);           //ɾ���ź����������ź�������ID
void os_flag_delete(os_flag *os_flag_id);        //ɾ����־���������־������ID
void os_mutex_delete(os_mutex *os_mutex_id);     //ɾ�������������뻥��������ID
void os_mbox_delete(os_mbox *os_mbox_id);        //ɾ������������������������ID

os_u32 os_sem_pend(os_sem *os_sem_id,os_pend_type ptype,os_u32 ptime);      //�ȴ��ź���
os_u32 os_flag_pend(os_flag *os_flag_id,os_pend_type ptype,os_u32 ptime);   //�ȴ���־��
os_u32 os_mutex_pend(os_mutex *os_mutex_id,os_pend_type ptype,os_u32 ptime);//�ȴ�������
void* os_mbox_pend(os_mbox *os_mbox_id,os_pend_type ptype,os_u32 ptime);   //�ȴ�������

void os_sem_post(os_sem *os_sem_id);                 //�����ź���
void os_flag_post(os_flag *os_flag_id);              //���ͱ�־��
void os_mutex_post(os_mutex *os_mutex_id);           //���ͻ�����
void os_mbox_post(os_mbox *os_mbox_id);              //����������

//void os_signal_state_check_process(void);
#endif
