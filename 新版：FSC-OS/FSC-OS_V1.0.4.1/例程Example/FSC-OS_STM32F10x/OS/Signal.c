#include "signal.h"

os_sem os_sem_struct_list_head;
os_sem *os_sem_list_head=&os_sem_struct_list_head;

os_flag os_flag_struct_list_head;
os_flag *os_flag_list_head=&os_flag_struct_list_head;

os_mutex os_mutex_struct_list_head;
os_mutex *os_mutex_list_head=&os_mutex_struct_list_head;

os_mbox os_mbox_struct_list_head;
os_mbox *os_mbox_list_head=&os_mbox_struct_list_head;

os_sem* os_sem_create(void)
{
	 os_sem *os_sem_temp=os_sem_list_head;
	 os_sem *os_sem_id=(os_sem*)os_mem_malloc(sizeof(os_sem));
	 os_sem_id->os_tcb_id_list=NULL;
	 os_sem_id->value=0;
	 os_sem_id->next=NULL;
	 if(os_sem_temp==NULL) os_sem_list_head=os_sem_id;
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
	 os_flag *os_flag_temp=os_flag_list_head;
	 os_flag *os_flag_id=(os_flag*)os_mem_malloc(sizeof(os_flag));
   os_flag_id->vbool=os_false;
	 os_flag_id->next=NULL;
	 if(os_flag_temp==NULL) os_flag_list_head=os_flag_id;
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
	 os_mutex *os_mutex_temp=os_mutex_list_head;
	 os_mutex *os_mutex_id=(os_mutex*)os_mem_malloc(sizeof(os_mutex));
	 os_mutex_id->using_tcb_id=NULL;
	 os_mutex_id->os_tcb_id_list=NULL;
	 os_mutex_id->lock=os_false;
	 os_mutex_id->next=NULL;
	 if(os_mutex_temp==NULL) os_mutex_list_head=os_mutex_id;
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
	 os_mbox *os_mbox_temp=os_mbox_list_head;
	 os_mbox *os_mbox_id=(os_mbox*)os_mem_malloc(sizeof(os_mbox));
	 while(os_mbox_temp->next!=NULL)
	 {
		 os_mbox_temp=os_mbox_temp->next;
	 }	
	 os_mbox_temp->next=os_mbox_id;
	 os_mbox_id->os_tcb_id=NULL;
	 os_mbox_id->next=NULL;
	 os_mbox_id->mail_list_head=NULL;
	 os_mbox_id->mail_list_rear=NULL;
	 return os_mbox_id;
}
void os_sem_delete(os_sem *os_sem_id)
{
	 os_sem *os_sem_temp=os_sem_list_head;

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
	 os_flag *os_flag_temp=os_flag_list_head;

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
	 os_mutex *os_mutex_temp=os_mutex_list_head;

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
	 os_mbox *os_mbox_temp=os_mbox_list_head;

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

os_u32 os_flag_pend(os_flag *os_flag_id,os_pend_type ptype,os_u32 ptime)//一个Flag最多只能被一个线程pend
{
	 os_u32 result=os_false;
	 os_u32 Ptype=ptype;  
	 os_timer *timer_new=(os_timer*)os_mem_malloc(sizeof(os_timer));
	 os_flag_id->os_tcb_id=os_tcb_cur;//记录等待os_flag的任务
	 if(os_flag_id->vbool==os_true) 
	 {
		 os_flag_id->vbool=os_false;
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
					 timer_new->thread_prio=os_tcb_cur->prio; //传递参数0:thread prio
					 timer_new->thread_global_prio=os_tcb_cur->global_id; //传递参数1:thread global_id
					 os_timer_add(timer_new); 
				 }
			   os_thread_sched_unlock();
			   os_thread_highest_prio_sched_and_switch();
				 while(os_tcb_cur->delaytime!=1){}
				 os_flag_id->vbool=os_false;
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
	os_flag_id->vbool=os_true;
	os_flag_id->os_tcb_id->delaytime=1;
	os_flag_id->os_tcb_id->state=os_thread_state_readying;
}
os_u32 os_mutex_pend(os_mutex *os_mutex_id,os_pend_type ptype,os_u32 ptime)
{
	 os_u32 result=os_false;
	 os_u32 Ptype=ptype;
	 os_timer *timer_new=(os_timer*)os_mem_malloc(sizeof(os_timer));
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
	 if(os_mutex_id->lock==os_false) //mutex处于空闲
	 {
		 os_mutex_id->using_tcb_id=os_tcb_cur;//记录获取到os_mutex的线程(等效获取Post权限)
		 os_mutex_id->lock=os_true; //标记该mutex已被使用
		 result=os_true;
	 }
	 else //mutex已被使用
	 {
		 switch(Ptype) //mutex等待模式选择
		 {
			 case pend_type__block: //阻塞等待
				 os_thread_sched_lock();
				 os_tcb_cur->state=os_thread_state_blocking;
			   os_tcb_cur->delaytime=ptime;		
			   if(ptime>0)
				 {
					 ptime=ptime+1;			 
					 os_tcb_cur->delaytime=ptime;
					 timer_new->thread_prio=os_tcb_cur->prio; //传递参数0:thread prio
					 timer_new->thread_global_prio=os_tcb_cur->global_id; //传递参数1:thread global_id
					 os_timer_add(timer_new);
				 }
			   os_thread_sched_unlock();
			   os_thread_highest_prio_sched_and_switch();
			   while((os_tcb_cur->delaytime!=1)&&(os_mutex_id->lock==os_true));
			   os_thread_sched_lock();
				 if(os_mutex_id->lock==os_false) //mutex处于空闲
				 {
					 os_mutex_id->using_tcb_id=os_tcb_cur;//记录获取到os_mutex的线程(等效获取Post权限)
					 os_mutex_id->lock=os_true; //标记该mutex已被使用
					 result=os_true;
				 }
				 else //超时
				 {
				   result=os_false;	 
				 }
				 os_thread_sched_unlock();
			 break;
			 case pend_type__unblock: //非阻塞等待
				 result=os_false;  
			 break;
			 case pend_type__queue_block:
				 
			 break;
			 case pend_type__queue_unblock:
				 
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
	if(os_mutex_id->using_tcb_id==os_tcb_cur)//Post权限检测
	{
	  os_mutex_id->lock=os_false;//取消锁定(表示空闲)
    //置等待mutex的最高优先级线程为readying态,延时置0
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
		os_mutex_id->using_tcb_id=(void*)0;  //清空权限
		
		os_signal_tcb_temp=os_mutex_id->os_tcb_id_list;
		while(os_signal_tcb_temp!=NULL) //获取pend中的最高优先级线程的id
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
			os_mutex_pend_tcb_highest_prio->state=os_thread_state_readying;//置pend中的最高优先级线程的状态为readying态
			os_mutex_pend_tcb_highest_prio->delaytime=0;
		}
	}
	os_thread_sched_unlock();
}
void* os_mbox_pend(os_mbox *os_mbox_id,os_pend_type ptype,os_u32 ptime)
{
	 os_u8 *p_mail=(void*)0;
	 os_timer *timer_new=(os_timer*)os_mem_malloc(sizeof(os_timer));	
	 os_u32 Ptype=ptype;  
	 os_mbox_id->pend_type=ptype;
	 os_mbox_id->os_tcb_id=os_tcb_cur;//记录等待os_mbox的任务
	 if(os_mbox_id->vbool==os_true) 
	 {
			os_mbox_id->vbool=os_false;
		 	p_mail=(os_u8*)(os_mbox_id->mail_list_head->pointer);//取队列最前项
			os_mbox_id->mail_list_len--;
		  if((Ptype==pend_type__queue_block)||(Ptype==pend_type__queue_unblock))
			{
				os_mem_free(os_mbox_id->mail_list_head->pointer);
				os_mbox_id->mail_list_head=os_mbox_id->mail_list_head->next;//队列最前项指向下一项
			}	
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
					 timer_new->thread_prio=os_tcb_cur->prio; //传递参数0:thread prio
					 timer_new->thread_global_prio=os_tcb_cur->global_id; //传递参数1:thread global_id
					 os_timer_add(timer_new); 
				 }
					os_thread_sched_unlock();
					os_thread_highest_prio_sched_and_switch();
					while(os_tcb_cur->delaytime!=1){}
					if(os_mbox_id->vbool==os_true)
					{
						os_mbox_id->vbool=os_false;
						p_mail=(os_u8*)(os_mbox_id->mail_list_head->pointer);//取队列最前项	
						os_mbox_id->mail_list_len--;	
					}
					else 
					{
					  p_mail=(void*)0;
					}
			 break;
			 case pend_type__unblock: 
						
			 break;
			 case pend_type__queue_block: 
				 os_thread_sched_lock();	
				 os_tcb_cur->state=os_thread_state_blocking;
				 os_tcb_cur->delaytime=ptime;		
				 if(ptime>0)
				 {
					 ptime=ptime+1;
					 os_tcb_cur->delaytime=ptime;			 
					 timer_new->thread_prio=os_tcb_cur->prio; //传递参数0:thread prio
					 timer_new->thread_global_prio=os_tcb_cur->global_id; //传递参数1:thread global_id
					 os_timer_add(timer_new); 
				 }
					os_thread_sched_unlock();
					os_thread_highest_prio_sched_and_switch();
					while(os_tcb_cur->delaytime!=1){}
					if(os_mbox_id->vbool==os_true)
					{
						os_mbox_id->vbool=os_false;
						if(os_mbox_id->mail_list_head!=NULL) 
						{
							p_mail=(os_u8*)(os_mbox_id->mail_list_head->pointer);//取队列最前项
							os_mem_free(os_mbox_id->mail_list_head->pointer);
							os_mbox_id->mail_list_head=os_mbox_id->mail_list_head->next;//队列最前项指向下一项
							os_mbox_id->mail_list_len--;
						}	
						os_mbox_id->mail_list_len--;
					}
					else
					{
					  p_mail=(void*)0;
					}
			 break;		
			 case pend_type__queue_unblock:
				 
			 break;				 
			 default:break;
		 }
	 }
	 os_mbox_id->os_tcb_id=NULL;
	 return p_mail;
}
void os_mbox_post(os_mbox *os_mbox_id,os_u8 *mail,os_u32 mail_len)
{ 
	os_u32 i;
	os_signal_mail_list *mail_struct;
	os_u8 *p_mail;
	if(os_mbox_id->mail_list_head==NULL)
	{
		mail_struct=(os_signal_mail_list*)os_mem_malloc(sizeof(os_signal_mail_list));
		p_mail=(os_u8*)os_mem_malloc(mail_len);
		i=0;
		while(i<mail_len) //拷贝
		{
			p_mail[i]=mail[i];
			i++;
		}
		mail_struct->pointer=p_mail;	
		os_mbox_id->mail_list_head=mail_struct;	
		os_mbox_id->mail_list_rear=mail_struct;	
	}
	else	
	{
		if((os_mbox_id->pend_type==pend_type__queue_block)||(os_mbox_id->pend_type==pend_type__queue_unblock))
		{
			mail_struct=(os_signal_mail_list*)os_mem_malloc(sizeof(os_signal_mail_list));
			p_mail=(os_u8*)os_mem_malloc(mail_len);
			i=0;
			while(i<mail_len) //拷贝
			{
				p_mail[i]=mail[i];
				i++;
			}
			mail_struct->pointer=p_mail;	
			os_mbox_id->mail_list_rear->next=mail_struct;	
			os_mbox_id->mail_list_rear=mail_struct;
			os_mbox_id->mail_list_len++;			
		}
		else
		{
			os_mbox_id->mail_list_head->pointer=mail;			
		}
	}
	if(os_mbox_id->os_tcb_id!=NULL)
	{
		os_mbox_id->vbool=os_true;
		os_mbox_id->os_tcb_id->delaytime=1;
		os_mbox_id->os_tcb_id->state=os_thread_state_readying;
	}
}
