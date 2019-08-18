#include "signal.h"

os_sem os_sem_struct_list_front;
os_sem *os_sem_list_front=&os_sem_struct_list_front;

os_flag os_flag_struct_list_front;
os_flag *os_flag_list_front=&os_flag_struct_list_front;

os_mutex os_mutex_struct_list_front;
os_mutex *os_mutex_list_front=&os_mutex_struct_list_front;

os_mbox os_mbox_struct_list_front;
os_mbox *os_mbox_list_front=&os_mbox_struct_list_front;

os_sem* os_sem_create(void)
{
	 os_sem *os_sem_temp=os_sem_list_front;
	 os_sem *os_sem_id=(os_sem*)os_mem_malloc(sizeof(os_sem));
	 os_sem_id->os_tcb_id_list=NULL;
	 os_sem_id->value=0;
	 os_sem_id->next=NULL;
	 if(os_sem_temp==NULL) os_sem_list_front=os_sem_id;
	 else
	 {
		 while(os_sem_temp->next!=NULL)
		 {
			 os_sem_temp=os_sem_temp->next;
		 }	
		 os_sem_temp->next=os_sem_id;
	 } 
	 return os_sem_id;
}
os_flag* os_flag_create(void)
{
	 os_flag *os_flag_temp=os_flag_list_front;
	 os_flag *os_flag_id=(os_flag*)os_mem_malloc(sizeof(os_flag));
   os_flag_id->value=os_false;
	 os_flag_id->next=NULL;
	 if(os_flag_temp==NULL) os_flag_list_front=os_flag_id;
	 else
	 {
		 while(os_flag_temp->next!=NULL)
		 {
			 os_flag_temp=os_flag_temp->next;
		 }	
		 os_flag_temp->next=os_flag_id;
	 } 
	 return os_flag_id;
}
os_mutex* os_mutex_create(void)
{
	 os_mutex *os_mutex_temp=os_mutex_list_front;
	 os_mutex *os_mutex_id=(os_mutex*)os_mem_malloc(sizeof(os_mutex));
	 os_mutex_id->using_tcb_id=NULL;
	 os_mutex_id->os_tcb_id_list=NULL;
	 os_mutex_id->lock=os_false;
	 os_mutex_id->next=NULL;
	 if(os_mutex_temp==NULL) os_mutex_list_front=os_mutex_id;
	 else
	 {
		 while(os_mutex_temp->next!=NULL)
		 {
			 os_mutex_temp=os_mutex_temp->next;
		 }	
		 os_mutex_temp->next=os_mutex_id;
	 } 
	 return os_mutex_id;
}
os_mbox* os_mbox_create(void)
{
	 os_mbox *os_mbox_temp=os_mbox_list_front->next;
	 os_mbox *os_mbox_id=(os_mbox*)os_mem_malloc(sizeof(os_mbox));
	
	 while(os_mbox_temp->next!=NULL)
	 {
		 os_mbox_temp=os_mbox_temp->next;
	 }	
	 os_mbox_temp->next=os_mbox_id;
	 os_mbox_id->next=NULL;
	 return os_mbox_id;
}
void os_sem_delete(os_sem *os_sem_id)
{
	 os_sem *os_sem_temp=os_sem_list_front;

	 while(os_sem_temp->next!=NULL)
	 {
		 if(os_sem_temp->next==os_sem_id)
		 {
			 os_sem_temp->next=os_sem_id->next;
			 break;
		 }
		 os_sem_temp=os_sem_temp->next;
	 } 		 
	 os_mem_free(os_sem_id);
}
void os_flag_delete(os_flag *os_flag_id)
{
	 os_flag *os_flag_temp=os_flag_list_front;

	 while(os_flag_temp->next!=NULL)
	 {
		 if(os_flag_temp->next==os_flag_id)
		 {
			 os_flag_temp->next=os_flag_id->next;
			 break;
		 }
		 os_flag_temp=os_flag_temp->next;
	 } 		 
	 os_mem_free(os_flag_id);
}
void os_mutex_delete(os_mutex *os_mutex_id)
{
	 os_mutex *os_mutex_temp=os_mutex_list_front;

	 while(os_mutex_temp->next!=NULL)
	 {
		 if(os_mutex_temp->next==os_mutex_id)
		 {
			 os_mutex_temp->next=os_mutex_id->next;
			 break;
		 }
		 os_mutex_temp=os_mutex_temp->next;
	 } 		 
	 os_mem_free(os_mutex_id);
}
void os_mbox_delete(os_mbox *os_mbox_id)
{
	 os_mbox *os_mbox_temp=os_mbox_list_front;

	 while(os_mbox_temp->next!=NULL)
	 {
		 if(os_mbox_temp->next==os_mbox_id)
		 {
			 os_mbox_temp->next=os_mbox_id->next;
			 break;
		 }
		 os_mbox_temp=os_mbox_temp->next;
	 } 		 
	 os_mem_free(os_mbox_id);
}

os_u32 os_sem_pend(os_sem *os_sem_id,os_pend_type ptype,os_u32 ptime)
{
	 os_u32 result=os_false;
	 os_u32 Ptype=ptype;
	 os_signal_tcb *os_signal_tcb_struct=(os_signal_tcb*)os_mem_malloc(sizeof(os_signal_tcb));
	 os_signal_tcb *os_signal_tcb_temp;
	 os_signal_tcb_struct->os_tcb_id=os_tcb_cur;
	 os_signal_tcb_struct->next=NULL;
   if(os_sem_id->os_tcb_id_list==NULL) os_sem_id->os_tcb_id_list=os_signal_tcb_struct;
	 else 
	 {
		 os_signal_tcb_temp=os_sem_id->os_tcb_id_list;
		 while(os_signal_tcb_temp->next!=NULL)
		 {
		    os_signal_tcb_temp=os_signal_tcb_temp->next;
		 }
		 os_signal_tcb_temp->next=os_signal_tcb_struct;
	 }
	 if(os_sem_id->value<0xffffffff) os_sem_id->value++;

	 switch(Ptype)
	 {
	   case pend_type__block: 
			 
		 break;
		 case pend_type__unblock: 
			 
		 break;
		 default:break;
	 }
	 return result; 
}
void os_sem_post(os_sem *os_sem_id)
{
	os_tcb *os_sem_tcb_cur=os_tcb_cur;
	os_signal_tcb *os_signal_tcb_temp=os_sem_id->os_tcb_id_list;
  if(os_sem_id->os_tcb_id_list->os_tcb_id==os_sem_tcb_cur) 
	{
		os_mem_free(os_sem_id->os_tcb_id_list);
		os_sem_id->os_tcb_id_list=os_sem_id->os_tcb_id_list->next;
    if(os_sem_id->value>0) os_sem_id->value--;		
	}
	else
	{
		os_signal_tcb_temp=os_sem_id->os_tcb_id_list;
		while(os_signal_tcb_temp->next!=NULL)
		{
			if(os_signal_tcb_temp->next->os_tcb_id==os_sem_tcb_cur)
			{
				os_mem_free(os_signal_tcb_temp->next);
				os_signal_tcb_temp->next=os_signal_tcb_temp->next->next;
				if(os_sem_id->value>0) os_sem_id->value--;
				break;
			}
			os_signal_tcb_temp=os_signal_tcb_temp->next;
		}
	}
}

os_u32 os_flag_pend(os_flag *os_flag_id,os_pend_type ptype,os_u32 ptime)//һ��Flag���ֻ�ܱ�һ���߳�pend
{
	 os_u32 result=os_false;
	 os_u32 os_timer_para[2];
	 os_u32 Ptype=ptype;  
	 os_flag_id->os_tcb_id=os_tcb_cur;//��¼�ȴ�os_flag������
	 if(os_flag_id->value==os_true) 
	 {
		 os_flag_id->value=os_false;
	   result=os_true;
	 }
	 else
	 {
		 switch(Ptype)
		 {
			 case pend_type__block:
         os_thread_sched_lock();	
         os_tcb_cur->state=os_thread_state_blocking;
			   os_tcb_cur->delaytime=ptime;		
			   if(ptime>0)
				 {
					 ptime=ptime+1;
					 os_tcb_cur->delaytime=ptime;			 
					 os_timer_para[0]=os_tcb_cur->prio; //���ݲ���0:thread prio
					 os_timer_para[1]=os_tcb_cur->global_id; //���ݲ���1:thread global_id
					 os_timer_add(timer_type__signal,os_timer_para, ptime); 
				 }
			   os_thread_sched_unlock();
			   os_thread_highest_prio_sched_and_switch();
				 while(os_tcb_cur->delaytime!=1){}
				 os_flag_id->value=os_false;
			   result=os_true;
			 break;
			 case pend_type__unblock: 

			 break;
			 default:break;
		 }
	 }
	 return result;
}
void os_flag_post(os_flag *os_flag_id)
{
	os_flag_id->value=os_true;
	os_flag_id->os_tcb_id->delaytime=0;
	os_flag_id->os_tcb_id->state=os_thread_state_readying;
}
os_u32 os_mutex_pend(os_mutex *os_mutex_id,os_pend_type ptype,os_u32 ptime)
{
	 os_u32 result=os_false;
	 os_u32 os_timer_para[2];
	 os_u32 Ptype=ptype;
	 os_signal_tcb *os_signal_tcb_struct=(os_signal_tcb*)os_mem_malloc(sizeof(os_signal_tcb));
	 os_signal_tcb *os_signal_tcb_temp;
	 os_signal_tcb_struct->os_tcb_id=os_tcb_cur;
	 os_signal_tcb_struct->next=NULL;
   if(os_mutex_id->os_tcb_id_list==NULL) os_mutex_id->os_tcb_id_list=os_signal_tcb_struct;
	 else 
	 {
		 os_signal_tcb_temp=os_mutex_id->os_tcb_id_list;
		 while(os_signal_tcb_temp->next!=NULL)
		 {
		    os_signal_tcb_temp=os_signal_tcb_temp->next;
		 }
		 os_signal_tcb_temp->next=os_signal_tcb_struct;
	 }
	 if(os_mutex_id->lock==os_false) //mutex���ڿ���
	 {
		 os_mutex_id->using_tcb_id=os_tcb_cur;//��¼��ȡ��os_mutex���߳�(��Ч��ȡPostȨ��)
		 os_mutex_id->lock=os_true; //��Ǹ�mutex�ѱ�ʹ��
		 result=os_true;
	 }
	 else //mutex�ѱ�ʹ��
	 {
		 switch(Ptype) //mutex�ȴ�ģʽѡ��
		 {
			 case pend_type__block: //�����ȴ�
				 os_thread_sched_lock();
				 os_tcb_cur->state=os_thread_state_blocking;
			   os_tcb_cur->delaytime=ptime;		
			   if(ptime>0)
				 {
					 ptime=ptime+1;			 
					 os_tcb_cur->delaytime=ptime;
					 os_timer_para[0]=os_tcb_cur->prio; //���ݲ���0:thread prio
					 os_timer_para[1]=os_tcb_cur->global_id; //���ݲ���1:thread global_id
					 os_timer_add(timer_type__signal,os_timer_para, ptime);
				 }
			   os_thread_sched_unlock();
			   os_thread_highest_prio_sched_and_switch();
			   while((os_tcb_cur->delaytime!=1)&&(os_mutex_id->lock==os_true));
			   os_thread_sched_lock();
				 if(os_mutex_id->lock==os_false) //mutex���ڿ���
				 {
					 os_mutex_id->using_tcb_id=os_tcb_cur;//��¼��ȡ��os_mutex���߳�(��Ч��ȡPostȨ��)
					 os_mutex_id->lock=os_true; //��Ǹ�mutex�ѱ�ʹ��
					 result=os_true;
				 }
				 else //��ʱ
				 {
				   result=os_false;	 
				 }
				 os_thread_sched_unlock();
			 break;
			 case pend_type__unblock: //�������ȴ�
				 result=os_false;  
			 break;
			 default:break;
		 }
	 }
	 return result;
}
void os_mutex_post(os_mutex *os_mutex_id)
{
	os_u32 prio=0;
	os_u8 prio_flag=os_false;
	os_tcb *os_mutex_pend_tcb_highest_prio;
	os_signal_tcb *os_signal_tcb_temp=os_mutex_id->os_tcb_id_list;
	os_thread_sched_lock();
	if(os_mutex_id->using_tcb_id==os_tcb_cur)//PostȨ�޼��
	{
	  os_mutex_id->lock=os_false;//ȡ������(��ʾ����)
    //�õȴ�mutex��������ȼ��߳�Ϊreadying̬,��ʱ��0
		if(os_mutex_id->os_tcb_id_list->os_tcb_id==os_mutex_id->using_tcb_id) 
		{
			os_mem_free(os_mutex_id->os_tcb_id_list);
			os_mutex_id->os_tcb_id_list=os_mutex_id->os_tcb_id_list->next;
		}
		else
		{
			os_signal_tcb_temp=os_mutex_id->os_tcb_id_list;
			while(os_signal_tcb_temp->next!=NULL)
			{
				if(os_signal_tcb_temp->next->os_tcb_id==os_mutex_id->using_tcb_id)
				{
					os_mem_free(os_signal_tcb_temp->next);
					os_signal_tcb_temp->next=os_signal_tcb_temp->next->next;
					break;
				}
				os_signal_tcb_temp=os_signal_tcb_temp->next;
			}
		}
		os_mutex_id->using_tcb_id=(void*)0;  //���Ȩ��
		
		os_signal_tcb_temp=os_mutex_id->os_tcb_id_list;
		while(os_signal_tcb_temp!=NULL) //��ȡpend�е�������ȼ��̵߳�id
		{
		  if(prio < os_signal_tcb_temp->os_tcb_id->global_id)
			{
			  prio=os_signal_tcb_temp->os_tcb_id->global_id;
				os_mutex_pend_tcb_highest_prio=os_signal_tcb_temp->os_tcb_id;
				prio_flag=os_true;
			}
			os_signal_tcb_temp=os_signal_tcb_temp->next;
		}
		if(prio_flag==os_true)
		{
			os_mutex_pend_tcb_highest_prio->state=os_thread_state_readying;//��pend�е�������ȼ��̵߳�״̬Ϊreadying̬
			os_mutex_pend_tcb_highest_prio->delaytime=0;
		}
	}
	os_thread_sched_unlock();
}
void* os_mbox_pend(os_mbox *os_mbox_id,os_pend_type ptype,os_u32 ptime)
{
	 void *p;
	 os_u32 Ptype=ptype;  
	 os_mbox_id->os_tcb_id=os_tcb_cur;//��¼�ȴ�os_mbox������
	 
	 switch(Ptype)
	 {
	   case pend_type__block: 
			 
		   os_thread_highest_prio_sched_and_switch();
		 break;
		 case pend_type__unblock: 
			 
		 break;
		 default:break;
	 }
	 return p;
}
void os_mbox_post(os_mbox *os_mbox_id)
{
	
	
}

