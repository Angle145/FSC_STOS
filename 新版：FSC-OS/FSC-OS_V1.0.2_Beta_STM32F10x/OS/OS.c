#include "os.h"

//ACB����
os_acb *os_acb_list_head;
os_acb *os_acb_list_rear;
os_acb *os_acb_cur; 
os_acb *os_acb_rdy;

//TCB����(prio_sort->�����ȼ������TCB�б�)
os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_head;
os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_rear; 
os_tcb *os_tcb_cur; 
os_tcb *os_tcb_rdy; 

//timer��ʱ������
os_timer os_thread_timer_struct_list_head;
os_timer os_thread_timer_struct_list_rear;
os_timer os_timer_struct_list_head;
os_timer os_timer_struct_list_rear;
os_timer *os_timer_list_head=&os_timer_struct_list_head;
os_timer *os_timer_list_rear=&os_timer_struct_list_rear;
os_timer *os_thread_timer_list_head=&os_thread_timer_struct_list_head;
os_timer *os_thread_timer_list_rear=&os_thread_timer_struct_list_rear;

//ϵͳ����
os_para os_p;
os_app_para os_app_p;
os_thread_para os_thread_p;
os_thread_prio_sort_table_para os_thread_prio_sort_table_p;
os_timer_para os_timer_p;

//��cpuȫ���ж�
void os_cpu_interrupt_enable(void)
{
  if(os_p.os_irq_lock>0) os_p.os_irq_lock--;
	if(os_p.os_irq_lock==0) os_core_exit();
}
//�ر�cpuȫ���ж�
void os_cpu_interrupt_disable(void)
{
	os_core_enter();
  if(os_p.os_irq_lock<0xFFFFFFFF) os_p.os_irq_lock++;
}

/*------------------------------------��ֲӲ���й�(Page Up)----------------------------------------------*/
/*1.ϵͳʱ�ӽ���os_clocks*/
/*Ĭ��ʹ��SysTick��ʱ��   
*SysTick->CTRL: bit0-��ʱ��ʹ�� bit1-�ж�ʹ�� bit2-ʱ��Դѡ��(=1ϵͳ��Ƶ��=0ϵͳ��Ƶ/8)  bit16-�������λ*/
#define system_core_clock SystemCoreClock //���оƬ�ļ�û���ṩSystemCoreClock��������ʹ������ע�͵��Զ���
//#define system_core_clock 72000000  //��Ϊ������Ƶ�������úö�Ӧ����Ƶ(�������ļ��Ѷ��������趨��)
#define os_systick_off()      SysTick->CTRL&=~(1<<0)     //�ر�ϵͳ��ʱ���ж�
#define os_systick_on()       SysTick->CTRL|=(1<<0)      //��ϵͳ��ʱ���ж�
#define os_systick_irq_off()  SysTick->CTRL&=~(1<<1)     //�ر�ϵͳ��ʱ���ж�
#define os_systick_irq_on()   SysTick->CTRL|=(1<<1)      //��ϵͳ��ʱ���ж�
#define os_systick_count_1us  (system_core_clock/8/1000000)//ϵͳ��ʱ��1us�������� (�˴�systick��ʱ��ʹ����Ƶ8��Ƶ)
#define os_systick_load       SysTick->LOAD              //ϵͳ��ʱ����װ��ֵ�Ĵ���
#define os_systick_val        SysTick->VAL               //ϵͳ��ʱ����ǰֵ�Ĵ���
/*��ʱ����ʹ��
*CTRL�Ĵ�����bit0-��ʱ��ʹ�� bit1-�ж�ʹ�� bit2-ʱ��Դѡ��(=1ϵͳ��Ƶ��=0ϵͳ��Ƶ/8)  bit16-�������λ*/
void os_systick_init(void)//����OS�ļ�ʱ����  
{ 
	//Systick��ʱ����ʹ��(ʹ��������ʱ��ʱ�����޸�Ϊ������ʱ��)
	char *Systick_priority = (char *)0xe000ed23;       //Systick�ж����ȼ��Ĵ���
	SysTick->LOAD  = (os_systick_count_1us)* 1000; //Systick��ʱ����װ�ؼ���ֵ:1ms(�̶�ֵ)
	*Systick_priority = 0x00;                           //Systick��ʱ���ж����ȼ�
	SysTick->VAL   = 0;                                 //Systick��ʱ����������0
	SysTick->CTRL = 0x3;//Systick�򿪲�ʹ���ж���ʹ���ⲿ����ʱ��,8��Ƶ  72MHz/8=9MHz  ����9000��=1ms  ����9��=1us
}

/*PendSV��ʹ���������жϺ���(�ж��������񻷾��л�����)
*��ֲʱ��Ҫע��ԭ�����е�PendSV_Handler�жϺ���,stm32��������stm32fxxx_it.c��*/
void os_pendsv_init(void)//PendSV��ʹ�� 
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //���ȼ��Ĵ�����ַ
	*NVIC_SYSPRI14=0xFF;//����PendSV�ж����ȼ���� 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //�жϿ��ƼĴ�����ַ
void os_pendsv_pulse(void)//����PendSV�ж�
{
	os_cpu_interrupt_disable();
  *NVIC_INT_CTRL=0x10000000; 
	os_cpu_interrupt_enable();	
}
/*systick��ʱ���жϺ���
*��ֲʱ��Ҫע�͵�ԭ�����е�SysTick_Handler�жϺ���,stm32��������stm32fxxx_it.c��*/
void SysTick_Handler(void) 
{
	os_p.os_clock_counter++;  //ϵͳʱ�ӽ����ۼ�
	os_timer_counter_process();//ϵͳ��ʱ������
  os_shell_run_time_counter_process();//ϵͳ����ʱ�估����ʱ�������
  if((os_p.thread_time_slice--)== 0)  
	 {
		 os_shell_cpu_occrate_counter_process();//app/thread couռ����ͳ�Ƴ��� 
		 os_p.thread_time_slice = THREAD_TIME_SLICE;//����ʱ��Ƭ��ֵ
		 os_thread_same_prio_sched_and_switch();	//���Ȳ��л�����		 
	 }  
}
os_bool os_tcb_prio_sort_table_rdy_check(os_tcb_prio_sort_table * os_tcb_prio_sort_table_struct)//��������tcb�б��Ƿ�����һ���̴߳��ھ���״̬
{
	os_bool result=os_false;
  os_tcb *os_tcb_temp=os_tcb_prio_sort_table_struct->list_same_prio_head;
  while(os_tcb_temp!=NULL)
	{
		if(os_tcb_temp->state==os_thread_state_readying)
		{
			result=os_true;
		  break;
		}
	  os_tcb_temp=os_tcb_temp->next;
	}
	return result;
}
os_tcb_prio_sort_table* os_tcb_prio_sort_table_highest_prio_rdy_get(void)//�����ȼ������tcb�б��в����Ѿ�����������ȼ��߳�
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_highestrio=os_tcb_prio_sort_table_list_head;
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_list_head;

	while(os_tcb_prio_sort_table_temp!=NULL)
	{
		if(os_tcb_prio_sort_table_temp->list_same_prio_head!=NULL)
		{
			if(os_tcb_prio_sort_table_rdy_check(os_tcb_prio_sort_table_temp)==os_true)//�������ȼ������Ƿ�������һ���̴߳��ھ���״̬(һ����û�б�ʾ�÷���û���߳������У�����Ҫ���в���������ȼ�)
			{
				if(os_tcb_prio_sort_table_highestrio->prio < os_tcb_prio_sort_table_temp->prio)
				{
					os_tcb_prio_sort_table_highestrio=os_tcb_prio_sort_table_temp;
				}
			}
		}
		os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_temp->next;//�ƶ�����һ����
	}
	os_p.os_tcb_highest_prio_sort_table=os_tcb_prio_sort_table_highestrio;
	return os_tcb_prio_sort_table_highestrio;
}
os_tcb* os_tcb_same_prio_next_rdy_get(void)//�ӵ�ǰ������ȼ����л�ȡ�¸�˳�����е��߳�
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_highestrio=os_p.os_tcb_highest_prio_sort_table;
  os_tcb *os_tcb_temp=os_tcb_prio_sort_table_highestrio->list_same_prio_cur->next;
	while(os_tcb_temp!=NULL)  
	{
		if(os_tcb_temp->state==os_thread_state_readying)
		{
		  os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_temp;
			break;
		}	
		os_tcb_temp=os_tcb_temp->next;
	}
	if(os_tcb_temp==NULL) os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_prio_sort_table_highestrio->list_same_prio_head;
	return os_tcb_prio_sort_table_highestrio->list_same_prio_cur;
}
os_tcb* os_tcb_highest_prio_next_rdy_get(void)//�ȴ�������ȼ����в���������ȼ����飬�ٴ�������ȼ������л�ȡ�¸�˳�����е��߳�
{
	os_tcb *os_tcb_highestrio_rdy;

	os_tcb_prio_sort_table_highest_prio_rdy_get();
	os_tcb_highestrio_rdy = os_tcb_same_prio_next_rdy_get();
	return os_tcb_highestrio_rdy;
}
//�߳�ʱ��Ƭ������
void os_thread_same_prio_scheduler(void)  //�̵߳�����(OS����)
{	
/*-----------------------------------ʱ��Ƭ------------------------------------------*/	
	if(os_p.current_prio==os_p.os_tcb_highest_prio_sort_table->prio)
	{
    os_tcb_rdy = os_tcb_same_prio_next_rdy_get(); //os_tcb_rdy
	}
	else //Ϊ��ʵ�ֵ�һ�����п���������Ҫ��
	{
	  os_thread_highest_prio_scheduler();
	}
/*-----------------------------------------------------------------------------------------*/	
}
//�߳����ȼ�������
void os_thread_highest_prio_scheduler(void)  //�̵߳�����(OS����)
{	
/*-----------------------------------���ȼ�------------------------------------------*/	
   os_tcb_rdy = os_tcb_highest_prio_next_rdy_get(); //os_tcb_rdy
/*-----------------------------------------------------------------------------------------*/	
}
void os_thread_same_prio_sched_and_switch(void)//�̵߳��Ȳ��л�����
{
	os_cpu_interrupt_disable();
	if(os_thread_p.sche_lock == 0) //�����л�������⣬>0˵��������==0����
	{
		os_thread_same_prio_scheduler();   //�ȵ��ȣ�OSTCBNext��ȡ����Ҫ���е�����
		if(os_tcb_rdy!=os_tcb_cur) 
		{
			os_pendsv_pulse(); //���񻷾��л�����OSTCBNext�л���OSTCBCur����	
		}
	}
	os_cpu_interrupt_enable();
}
void os_thread_highest_prio_sched_and_switch(void)//�̵߳��Ȳ��л�����
{
	os_cpu_interrupt_disable();
	if(os_thread_p.sche_lock == 0) //�����л�������⣬>0˵��������==0����
	{
		os_thread_highest_prio_scheduler();   //�ȵ��ȣ�OSTCBNext��ȡ����Ҫ���е�����
		if(os_tcb_rdy!=os_tcb_cur) 
		{
			os_p.current_prio=os_p.os_tcb_highest_prio_sort_table->prio;
			os_pendsv_pulse(); //���񻷾��л�����OSTCBNext�л���OSTCBCur����	
		}
	}
	os_cpu_interrupt_enable();
}

void os_timer_timeout_handle(void) //��ʱ����ʱ��ɴ���
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_list_head;
	os_tcb *os_tcb_temp=NULL;
	while(os_tcb_prio_sort_table_temp!=NULL)
	{
		if(os_timer_list_head->next->para[0]==os_tcb_prio_sort_table_temp->prio) break;
		os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_temp->next;
	}
	os_tcb_temp=os_tcb_prio_sort_table_temp->list_same_prio_head;
	while(os_tcb_temp!=NULL)
	{
	 if(os_timer_list_head->next->para[1]==os_tcb_temp->global_id)
		{	
		 break;
		}
	 os_tcb_temp=os_tcb_temp->next;
	}
	if(os_tcb_temp!=NULL)
	{
		switch(os_timer_list_head->next->type)
		{
			case timer_type__app: 
			{

			}
			break;
			case timer_type__thread:
			{
				 if(os_tcb_temp->state==os_thread_state_delaying)
				 {
					 os_tcb_temp->state=os_thread_state_readying;
				 }
				 os_tcb_temp->delaytime=1;
				 os_p.thread_time_slice = THREAD_TIME_SLICE;//����ʱ��Ƭ��ֵ
      }
			break;		
			case timer_type__softtimer: 
			{
				
			}
			break;
			case timer_type__signal: 
			{
				 if(os_tcb_temp->state==os_thread_state_blocking)
				 {
					 if(os_tcb_temp->delaytime>0)
					 {
						 os_tcb_temp->state=os_thread_state_readying;
						 os_tcb_temp->delaytime=1;
						 os_p.thread_time_slice = THREAD_TIME_SLICE;//����ʱ��Ƭ��ֵ
					 }
				 }
			}
			break;		
			default: 
			break;
		}
	}
}

void os_timer_counter_process(void) //ϵͳ��ʱ������
{
	os_timer *os_timer_temp=os_timer_list_head->next;

	if(os_timer_list_head->next->clock_counter>1) os_timer_list_head->next->clock_counter--;//��Сֵ���Ϊ1
	else 
	{
		while(os_timer_list_head->next->clock_counter==1)
		{	
			os_timer_list_head->next->clock_counter=0;
		  os_timer_timeout_handle();  		
      switch(os_timer_list_head->next->type)		
			{	
				case timer_type__app: 
				{
					os_timer_list_strutc_erase(os_timer_list_head->next);//�Ӷ�ʱ����ʱ�б���ɾ��	    
				}
        case timer_type__thread:
				{					
					os_timer_temp=os_timer_list_head->next;//����timer_list_head	
					os_timer_list_strutc_erase(os_timer_list_head->next);//�Ӷ�ʱ����ʱ�б���ɾ��	
					os_thread_timer_recycle(os_timer_temp);//�� �Ӷ�ʱ����ʱ�б���ɾ���Ķ�ʱ�����յ�app��ʱ��ʱ���б���		
				}
				break;				
				case timer_type__softtimer: 
				{
					os_timer_list_strutc_erase(os_timer_list_head->next);//�Ӷ�ʱ����ʱ�б���ɾ��	
				}
				break;
				case timer_type__signal: //�����ź���ʱʹ�õ����̵߳�ר����ʱ��ͬtimer_type__threadһ����Ҫ����
				{
					os_timer_temp=os_timer_list_head->next;//����timer_list_head	
					os_timer_list_strutc_erase(os_timer_list_head->next);//�Ӷ�ʱ����ʱ�б���ɾ��	
					os_thread_timer_recycle(os_timer_temp);//�� �Ӷ�ʱ����ʱ�б���ɾ���Ķ�ʱ�����յ�app��ʱ��ʱ���б���		
				}
				break;
				default: os_timer_list_strutc_erase(os_timer_list_head->next);//�Ӷ�ʱ����ʱ�б���ɾ��	
				break;
			}		
      os_thread_highest_prio_sched_and_switch(); 			
			if(os_timer_list_head->next==os_timer_list_rear) break;
    }
	}
}

//�����ջ��ʹ��
//ֻҪ����PendSV�ж�,��ת�жϴ�����ǰ xPSR,PC,LR,R12,R3-R0���Զ����浽ϵͳ������ջ��(�˲�����PendSV�ж�Ӳ������)��
//�������ĸ�ջȡ���ڵ�ǰ��SP���ͣ�����MSP�򱣴浽ϵͳջ������PSP�򱣴浽����ջ����OS�Ǳ���������ջ����R11-R4��Ҫ�ֶ����浽����ջ��
//��ջ˳��ջ��->ջβ xPSR,PC,LR,R12,R3-R0,R4-R11��16��(SP(R13)������TCB�׸���Ա������)��
stk32* os_thread_stk_init(void* thread,stk32 *topstkptr)
{
    stk32 *stk;
    stk = topstkptr;
    *(stk)    = (os_u32)0x01000000L;//xPSR ����״̬�Ĵ�����xPSR Tλ(��24λ)������1,�����һ��ִ������ʱ����Fault�ж�                                                     
    *(--stk)  = (os_u32)thread;       //PC   ��ʹ��ʱָ���������׵�ַ�������л�ʱ������ָ���������м�ĳ��ַ��            
    *(--stk)  = (os_u32)0xFFFFFFFEL;//LR   �����Ÿ�����ת�ķ������ӵ�ַ,��ʹ��Ϊ���4λΪE����һ���Ƿ�ֵ����ҪĿ���ǲ���ʹ��R14���������ǲ��ܷ��ص�              
    stk-=13;	
    return stk;
}
void os_tcb_ptr_init(void)
{
	//����ֱ������������ȼ��߳�
	  //os_tcb_cur=os_tcb_prio_sort_table_highest_prio_rdy_get()->list_same_prio_cur;//����ֱ������������ȼ��߳�
	  //os_tcb_rdy = os_tcb_cur;
	//���������п����߳�
    //os_tcb_cur = os_tcb_prio_sort_table_list_head->list_same_prio_head; //���������п����߳�
    //os_tcb_rdy = os_tcb_cur;
	
	  
	  os_tcb_cur=os_tcb_prio_sort_table_list_head->list_same_prio_head; //���������п����߳�
	  os_tcb_rdy=os_tcb_cur;
	  os_tcb_prio_sort_table_highest_prio_rdy_get();//���������̺߳�����������ȼ��߳�
}
void os_init_and_startup(void)
{
	os_p.state=os_state_running;
	os_p.os_clock_counter=0;
	os_p.thread_time_slice = THREAD_TIME_SLICE;
	os_p.app_time_slice = APP_TIME_SLICE;	
  os_timer_list_init();	
	os_tcb_ptr_init();
  os_systick_init();
	os_user_init();
	os_pendsv_init();
	os_psp_reset();
	os_pendsv_pulse();
	while(1);//�ȴ������л�PendSV�ж�
}

os_type_app_id* os_app_new_init(void)
{
  os_acb *os_acb_temp=(os_acb*)os_mem_malloc(sizeof(os_acb));
	os_thread_p.id_counter=0;
  os_acb_temp->thread_len=0;	
	os_acb_temp->tcb_list=NULL;
  return os_acb_temp;	
}
void os_app_new_create(os_type_app_id* app_id)
{
	if(os_app_p.acb_list_len==0)
	{
		app_id->last=NULL;
		app_id->next=NULL;
		os_acb_list_head=app_id;
		os_acb_list_rear=	app_id;
	}
	else
	{
		app_id->next=NULL;
    os_acb_list_rear->next=app_id;
		os_acb_list_rear=app_id;
	}
  
	os_app_p.acb_list_len++;
}
os_type_thread_id* os_thread_new_create(os_type_app_id* app,void* thread,os_str8 *str_name,os_u32 stk_size,os_u32 slice_tick,os_u32 prio,os_thread_state state,void *para)//���񴴽�����
{
	  os_u32 i=0;
	  os_tcb_prio_sort_table *os_tcb_prio_sort_table_temp=NULL;
	  os_tcb_prio_sort_table *os_tcb_prio_sort_table_struct=NULL;
	  os_app_tcb *os_app_tcb_temp=app->tcb_list;
	  os_app_tcb *os_app_tcb_thread=(os_app_tcb*)os_mem_malloc(sizeof(os_app_tcb));
	  os_tcb *os_tcb_thread=(os_tcb*)os_mem_malloc(sizeof(os_tcb));
	  stk32 *stkptr=(stk32*)os_mem_malloc(sizeof(stk32)*stk_size);  
    for(i=0;i<stk_size;i++) { stkptr[i]=0; }
		os_tcb_thread->stk_addr=(os_u32*)stkptr;
	  os_app_tcb_thread->next=NULL;
	  os_tcb_thread->last=NULL;
	  os_tcb_thread->next=NULL;
    stkptr=stkptr+stk_size-1;  
    stkptr = os_thread_stk_init(thread,stkptr);
		os_tcb_thread->stk_ptr=stkptr;
	  os_tcb_thread->stk_size_sum=stk_size;
		os_tcb_thread->prio=prio+app->prio;
		os_tcb_thread->timeslice=slice_tick;
	  os_tcb_thread->delaytime=1;
		os_tcb_thread->state=state;
	  os_tcb_thread->id=os_thread_p.id_counter;
	  os_tcb_thread->global_id=os_thread_p.global_id_counter;
    os_tcb_thread->state = os_thread_state_readying;//����ʱΪ����̬(Ҳ��Ϊ����̬,��������⵽��������ʱ��תΪ����̬)
	  i=0;
		while(str_name[i]!='\0')
		{
			os_tcb_thread->thread_name[i]=str_name[i];
			i++;
		}
		
		if(os_thread_prio_sort_table_p.list_len==0)
		{
		  os_thread_prio_sort_table_p.prio=os_tcb_thread->prio;
			os_tcb_prio_sort_table_struct=(os_tcb_prio_sort_table*)os_mem_malloc(sizeof(os_tcb_prio_sort_table));
			os_tcb_prio_sort_table_struct->last=NULL;
			os_tcb_prio_sort_table_struct->next=NULL;
			os_tcb_prio_sort_table_list_head=os_tcb_prio_sort_table_struct;
			os_tcb_prio_sort_table_list_rear=os_tcb_prio_sort_table_struct;
	
			os_tcb_prio_sort_table_struct->list_same_prio_head=os_tcb_thread;
			os_tcb_prio_sort_table_struct->list_same_prio_rear=os_tcb_thread;
			os_tcb_prio_sort_table_struct->list_same_prio_cur=os_tcb_prio_sort_table_struct->list_same_prio_rear;
			
			os_tcb_prio_sort_table_struct->state=os_thread_prio_state_readying;
			os_tcb_prio_sort_table_struct->prio=os_tcb_thread->prio;
			os_tcb_prio_sort_table_struct->id=os_thread_prio_sort_table_p.id_counter;
			os_tcb_prio_sort_table_struct->list_same_prio_len++;
			os_thread_prio_sort_table_p.id_counter++;
			os_thread_prio_sort_table_p.list_len++;
			
			os_thread_timer_list_init();//��ʹ��thread�̶߳�ʱ������			
		}
		else
		{
			os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_list_head;
			while(os_tcb_prio_sort_table_temp!=NULL)
			{
				if(os_tcb_thread->prio == os_tcb_prio_sort_table_temp->prio) break;
				os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_temp->next;
			}
			if(os_tcb_prio_sort_table_temp!=NULL) 
			{
				os_tcb_prio_sort_table_temp->list_same_prio_rear->next=os_tcb_thread;
				os_tcb_thread->last=os_tcb_prio_sort_table_temp->list_same_prio_rear;
				os_tcb_prio_sort_table_temp->list_same_prio_rear=os_tcb_thread;
				os_tcb_prio_sort_table_temp->list_same_prio_cur=os_tcb_prio_sort_table_temp->list_same_prio_rear;
				
				os_tcb_prio_sort_table_temp->list_same_prio_len++;
			}
			else
			{
				os_tcb_prio_sort_table_struct=(os_tcb_prio_sort_table*)os_mem_malloc(sizeof(os_tcb_prio_sort_table));
				os_tcb_prio_sort_table_struct->last=NULL;
				os_tcb_prio_sort_table_struct->next=NULL;			
				os_tcb_prio_sort_table_list_rear->next=os_tcb_prio_sort_table_struct;
				os_tcb_prio_sort_table_struct->last=os_tcb_prio_sort_table_list_rear;
				os_tcb_prio_sort_table_list_rear=os_tcb_prio_sort_table_struct;
			
				os_tcb_prio_sort_table_struct->list_same_prio_head=os_tcb_thread;
				os_tcb_prio_sort_table_struct->list_same_prio_rear=os_tcb_thread;
				os_tcb_prio_sort_table_struct->list_same_prio_cur=os_tcb_prio_sort_table_struct->list_same_prio_rear;
				
				os_tcb_prio_sort_table_struct->state=os_thread_prio_state_readying;
				os_tcb_prio_sort_table_struct->prio=os_tcb_thread->prio;
				os_tcb_prio_sort_table_struct->id=os_thread_prio_sort_table_p.id_counter;
				os_tcb_prio_sort_table_struct->list_same_prio_len++;
				os_thread_prio_sort_table_p.id_counter++;
				os_thread_prio_sort_table_p.list_len++;
			}
			os_app_tcb_thread->thread=os_tcb_thread;
		}
		os_app_tcb_thread->thread=os_tcb_thread;
		if(os_app_tcb_temp==NULL)
		{
			app->tcb_list=os_app_tcb_thread;
		}
		else
		{
			while(os_app_tcb_temp->next!=NULL) 
			{
				os_app_tcb_temp=os_app_tcb_temp->next;
			}
			os_app_tcb_temp->next=os_app_tcb_thread;
		}		
		app->thread_len++;
		os_thread_p.id_counter++;
	  os_thread_p.global_id_counter++;
		
		app->stk_size_sum+=os_tcb_thread->stk_size_sum;
		
		os_thread_timer_create();//ͬ���̴߳���һ��threadר�õ���ʱ��ʱ��
		
	  return os_tcb_thread;
}

void os_thread_sched_lock(void) //�����л�����  
{
	os_core_enter();
  if(os_thread_p.sche_lock<0xFFFFFFFF) os_thread_p.sche_lock++;  
	os_core_exit();
}

void os_thread_sched_unlock(void) //�����л����� 
{
	os_core_enter();                           
	if(os_thread_p.sche_lock>0) os_thread_p.sche_lock--;  
	os_core_exit();
} 


u32 os_clock_counter_get(void)
{
  return os_p.os_clock_counter;  //ϵͳʱ�ӽ����ۼƱ���
}


//uS��ʱ
void delay_us(os_u32 us)
{       
    os_u32 ticks;
    os_u32 told,tnow,tcnt=0;
    os_u32 reload=SysTick->LOAD;       
    ticks=(os_systick_count_1us)*us;      
    tcnt=0;
    told=SysTick->VAL;
    while(1)
    {
			tnow=SysTick->VAL;  
			if(tnow!=told)
			{       
				if(tnow<told) tcnt += told-tnow;
				else tcnt += reload-tnow+told;      
				told = tnow;
				if(tcnt>=ticks) break;
			} 					
    }
}
//mS��ʱ
void delay_ms(os_u32 ms)
{   
  delay_us(ms*1000);			
}

void os_thread_delay(os_u32 time_ticks)  //��������CPU����Ȩ,time_ticks=0 ���÷�����time_ticks>0����һ��ʱ���ָ�(��ʱ)
{
	 os_u32 os_timer_para[8];
	 os_thread_sched_lock();
	 if(time_ticks>0) time_ticks=time_ticks+1;
   os_tcb_cur->delaytime=time_ticks;
	 os_tcb_cur->state=os_thread_state_delaying;
	 if(time_ticks>0)
	 {
		 os_timer_para[0]=os_tcb_cur->prio; //���ݲ���0:thread prio
		 os_timer_para[1]=os_tcb_cur->global_id; //���ݲ���1:thread global_id
		 os_timer_add(timer_type__thread,os_timer_para, time_ticks);
	 }
	 os_thread_sched_unlock();
	 os_thread_highest_prio_sched_and_switch();
	 while(os_tcb_cur->delaytime!=1);
}
