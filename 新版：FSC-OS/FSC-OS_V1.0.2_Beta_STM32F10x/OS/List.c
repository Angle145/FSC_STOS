#include "list.h"


void os_timer_list_init(void) //��ʱ�������ʹ��
{
		os_timer_list_head->last=NULL;      
	  os_timer_list_head->next=os_timer_list_rear; 
    os_timer_list_rear->last=os_timer_list_head;
    os_timer_list_rear->next=NULL;     
}
void os_thread_timer_list_init(void)
{
		os_thread_timer_list_head->last=NULL;      
	  os_thread_timer_list_head->next=os_thread_timer_list_rear;  
    os_thread_timer_list_rear->last=os_thread_timer_list_head;  
    os_thread_timer_list_rear->next=NULL; 
}
void os_thread_timer_create(void) //thread��ʱ������
{
	 os_timer *os_thread_timer_temp=(os_timer*)(os_mem_malloc(sizeof(os_timer))); 
	 
	 os_thread_timer_temp->type=timer_type__thread;
	 os_thread_timer_temp->clock_counter=0;
	 os_thread_timer_temp->id=os_thread_p.global_id_counter;
	 
	 os_thread_timer_temp->last=os_thread_timer_list_rear->last;
	 os_thread_timer_temp->next=os_thread_timer_list_rear;
	 os_thread_timer_list_rear->last->next=os_thread_timer_temp;
	 os_thread_timer_list_rear->last=os_thread_timer_temp;
	
	 os_timer_p.thread_timer_list_len++;
}
//os_tcb����
void os_tcb_list_base_head_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb)//�ڲο�����֮ǰ����os_tcb�ڵ�
{
  os_tcb *os_tcb_temp=list_head;
	while(os_tcb_temp!=NULL)
	{
	  if(os_tcb_temp==base_tcb)
		{
		  add_tcb->last=os_tcb_temp->last;
			add_tcb->next=os_tcb_temp;
			os_tcb_temp->last->next=add_tcb;
			os_tcb_temp->last=add_tcb;
			break;
		}
		os_tcb_temp=os_tcb_temp->next;
	}
}
void os_tcb_list_base_behind_insert(os_tcb *list_head,os_tcb *base_tcb,os_tcb *add_tcb)//�ڲο�����֮������os_tcb�ڵ�
{
  os_tcb *os_tcb_temp=list_head;
	while(os_tcb_temp!=NULL)
	{
	  if(os_tcb_temp==base_tcb)
		{
		  add_tcb->last=os_tcb_temp;
			add_tcb->next=os_tcb_temp->next;
			os_tcb_temp->next->last=add_tcb;
			os_tcb_temp->next=add_tcb;
			break;
		}
		os_tcb_temp=os_tcb_temp->next;
	}
}
void os_tcb_list_erase(os_tcb *list_head,os_tcb *del_tcb)//os_tcb�ڵ�ɾ��
{
  os_tcb *os_tcb_temp=list_head;
	while(os_tcb_temp!=NULL)
	{
	  if(os_tcb_temp==del_tcb)
		{
		  if(os_tcb_temp->last!=NULL) os_tcb_temp->last=os_tcb_temp->next;
			if(os_tcb_temp->next!=NULL) os_tcb_temp->next->last=os_tcb_temp->last;
			
			break;
		}
		os_tcb_temp=os_tcb_temp->next;
	}
}

//os_tcb_prio_sort_table����
void os_tcb_prio_sort_table_list_insert(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct,os_u32 num) //����λ�ò���
{	

		os_thread_p.tcb_list_len++;
}
void os_tcb_prio_sort_table_list_erase(os_u32 num) //����λ��ɾ��
{	

	
	  os_thread_p.tcb_list_len--;
}
void os_tcb_prio_sort_table_list_struct_erase(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct) //����ṹ��ɾ��
{	

	  os_thread_p.tcb_list_len--;
}
void os_tcb_prio_sort_table_list_push_head(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct)//ͷ������
{	  

	
	  os_thread_p.tcb_list_len++;
}
void os_tcb_prio_sort_table_list_pop_head(void)//ͷ��ɾ��
{	

	  os_thread_p.tcb_list_len--;
}
void os_tcb_prio_sort_table_list_push_rear(os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct)//β������
{	

	
	  os_thread_p.tcb_list_len++;
}
void os_tcb_prio_sort_table_list_pop_rear(void)//β��ɾ��
{	

	
	  os_thread_p.tcb_list_len--;
}

//��ʱ����
void os_timer_list_insert(os_timer *os_timer_struct,os_u32 num) //����λ�ò���
{	
	  os_u32 list_len=0,i=0;
	  os_timer *os_timer_temp=os_timer_list_head->next;
	  while(os_timer_temp!=os_timer_list_rear)
		{
			 list_len++;
			 os_timer_temp=os_timer_temp->next;
		}
		//if(num>list_len) while(1){os_printf("os_timer_list_insert error\r\n");} 
		if(num<=list_len)
		{
			os_timer_temp=os_timer_list_head->next;
			while(os_timer_temp!=os_timer_list_rear)
			{
				 if(i==num)
				 {
					 break;
				 }
				 i++;
				 os_timer_temp=os_timer_temp->next;
			}
			if(num==0)
			{	
				os_timer_struct->last=os_timer_list_head;
				os_timer_struct->next=os_timer_list_head->next;
				os_timer_list_head->next->last=os_timer_struct;
				os_timer_list_head->next=os_timer_struct;
				
				os_timer_p.timer_list_len++;
			}
			else if(num==list_len)
			{
				os_timer_struct->last=os_timer_list_rear->last;
				os_timer_struct->next=os_timer_list_rear;
				os_timer_list_rear->last->next=os_timer_struct;
				os_timer_list_rear->last=os_timer_struct;
				
				os_timer_p.timer_list_len++;
			}
			else
			{
				os_timer_struct->last=os_timer_temp->last;
				os_timer_struct->next=os_timer_temp;	     
				os_timer_temp->last->next=os_timer_struct;
				os_timer_temp->last=os_timer_struct;
				
				os_timer_p.timer_list_len++;
			}
		}
}
void os_timer_list_erase(os_u32 num) //����λ��ɾ��
{	
	  os_u32 i;
	  os_timer *os_timer_temp=os_timer_list_head->next;
	  i=0;
	  while(i<num)
		{
			os_timer_temp=os_timer_temp->next;
			i++;
		}
	  os_timer_temp->last->next=os_timer_temp->next;
		os_timer_temp->next->last=os_timer_temp->last;
		
	  os_timer_p.timer_list_len--;
}
void os_timer_list_strutc_erase(os_timer *os_timer_struct) //����ṹ��ɾ��
{	
	  os_timer *os_timer_temp=os_timer_list_head->next;
	  while(os_timer_temp!=os_timer_list_rear)
		{
			if(os_timer_temp==os_timer_struct)
			{
				os_timer_temp->last->next=os_timer_temp->next;
				os_timer_temp->next->last=os_timer_temp->last;
				
				os_timer_p.timer_list_len--;
				break;
			}
			os_timer_temp=os_timer_temp->next;
		}
}
void os_timer_list_push_head(os_timer *os_timer_struct)//ͷ������
{	
		os_timer_struct->last=os_timer_list_head;
		os_timer_struct->next=os_timer_list_head->next;
	  os_timer_list_head->next->last=os_timer_struct;
	  os_timer_list_head->next=os_timer_struct;
	
	  os_timer_p.timer_list_len++;
}
void os_timer_list_pop_head(void)//ͷ��ɾ��
{	
	  os_timer_list_head->next=os_timer_list_head->next->next;
    os_timer_list_head->next->next->last=os_timer_list_head;
	
	  os_timer_p.timer_list_len--;
}
void os_timer_list_push_rear(os_timer *os_timer_struct)//β������
{	
	  os_timer_list_rear->last->next=os_timer_struct;
		os_timer_struct->last=os_timer_list_rear->last;
		os_timer_struct->next=os_timer_list_rear;
		os_timer_list_rear->last=os_timer_struct;
	
	  os_timer_p.timer_list_len++;
}
void os_timer_list_pop_rear(void)//β��ɾ��
{	
	  os_timer_list_rear->last=os_timer_list_rear->last->last;
		os_timer_list_rear->last->last->next=os_timer_list_rear;
	
	  os_timer_p.timer_list_len--;
}

void os_thread_timer_erase(os_timer* os_thread_timer_struct) //app��ʱ��ʱ��ɾ��
{
	  os_timer *os_thread_timer_temp=os_thread_timer_list_head->next;
	
	  //if(os_app_timer_struct==os_thread_timer_list_rear) while(1){os_printf("os_app_timer_erase() error\r\n");}
		while(os_thread_timer_temp!=os_thread_timer_list_rear)
		{
			if(os_thread_timer_temp==os_thread_timer_struct)
			{
				os_thread_timer_temp->last->next=os_thread_timer_temp->next;
				os_thread_timer_temp->next->last=os_thread_timer_temp->last;
				break;
			}
			os_thread_timer_temp=os_thread_timer_temp->next;
		}	
}
os_timer* os_thread_timer_free_get(void) //app��ʱ��ʱ�����л�ȡ
{
	  os_timer *os_thread_timer_temp=os_thread_timer_list_head->next;
	  if(os_thread_timer_temp!=os_thread_timer_list_rear) os_thread_timer_erase(os_thread_timer_temp);
	  //else while(1){os_printf("os_app_timer_free_get() error\r\n");};
	  else os_thread_timer_temp=os_thread_timer_list_rear->last;//ע��!!!!!!!!!!
		return os_thread_timer_temp;
}
void os_thread_timer_recycle(os_timer *os_thread_timer_struct) //app��ʱ��ʱ������
{
		os_thread_timer_struct->last=os_thread_timer_list_head;
	  os_thread_timer_struct->next=os_thread_timer_list_head->next;
	  os_thread_timer_list_head->next->last=os_thread_timer_struct;
	  os_thread_timer_list_head->next=os_thread_timer_struct;
}

void os_timer_add(os_timer_type timer_id,os_u32* para,os_u32 time_ticks)
{
		os_u32 time_sum=0,time_sum_last=0;
		os_u32 i=0,j=0;

	  os_thread_sched_lock();
		os_timer *os_timer_insert=os_thread_timer_free_get();
	  os_timer *os_timer_temp=os_timer_list_head->next;
	  os_timer_insert->type=timer_id;
		for(i=0;i<8;i++){
			os_timer_insert->para[i]=para[i];
		}	
		if(os_timer_temp==os_timer_list_rear)
    {
			os_timer_insert->clock_counter=time_ticks+1;os_timer_list_insert(os_timer_insert,0);		
	  }
	  else
    {		
			i=0;
			while(os_timer_temp!=os_timer_list_rear)
			{	
				time_sum+=os_timer_temp->clock_counter;		
				if(time_sum>time_ticks)
				{
					os_timer_insert->clock_counter=time_ticks-time_sum_last+1;
					os_timer_list_insert(os_timer_insert,i);
					os_timer_insert->next->clock_counter=time_sum-time_ticks+1;//+1��Ϊ�˱�֤��СֵΪ1(==0���Ϊδʹ��,==1Ϊ��Сʹ��ֵ)
					break;
				}
				time_sum_last=time_sum;
				i++;
				os_timer_temp=os_timer_temp->next;
			}
			if(os_timer_temp==os_timer_list_rear)
			{	
				i=0;
				os_timer_temp=os_timer_list_head->next;
				while(os_timer_temp!=os_timer_list_rear)
				{	
					i++;
					if(os_timer_temp->clock_counter>0) 
					{
						j=i;
					}	
					os_timer_temp=os_timer_temp->next;
				}
				os_timer_insert->clock_counter=time_ticks-time_sum+1;
				os_timer_list_insert(os_timer_insert,j);
			}
		}
		os_thread_sched_unlock();
}

