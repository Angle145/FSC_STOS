#include "os.h"

//ACB链表
os_acb *os_acb_list_head;
os_acb *os_acb_list_rear;
os_acb *os_acb_cur; 
os_acb *os_acb_rdy;

//TCB链表(prio_sort->按优先级分组的TCB列表)
os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_head;
os_tcb_prio_sort_table *os_tcb_prio_sort_table_list_rear; 
os_tcb *os_tcb_cur; 
os_tcb *os_tcb_rdy; 

//timer定时器链表
os_timer os_thread_timer_struct_list_head;
os_timer os_thread_timer_struct_list_rear;
os_timer os_timer_struct_list_head;
os_timer os_timer_struct_list_rear;
os_timer *os_timer_list_head=&os_timer_struct_list_head;
os_timer *os_timer_list_rear=&os_timer_struct_list_rear;
os_timer *os_thread_timer_list_head=&os_thread_timer_struct_list_head;
os_timer *os_thread_timer_list_rear=&os_thread_timer_struct_list_rear;

//系统变量
os_para os_p;
os_app_para os_app_p;
os_thread_para os_thread_p;
os_thread_prio_sort_table_para os_thread_prio_sort_table_p;
os_timer_para os_timer_p;

//打开cpu全局中断
void os_cpu_interrupt_enable(void)
{
  if(os_p.os_irq_lock>0) os_p.os_irq_lock--;
	if(os_p.os_irq_lock==0) os_core_exit();
}
//关闭cpu全局中断
void os_cpu_interrupt_disable(void)
{
	os_core_enter();
  if(os_p.os_irq_lock<0xFFFFFFFF) os_p.os_irq_lock++;
}

/*------------------------------------移植硬件有关(Page Up)----------------------------------------------*/
/*1.系统时钟节拍os_clocks*/
/*默认使用SysTick定时器   
*SysTick->CTRL: bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位*/
#define system_core_clock SystemCoreClock //如果芯片文件没有提供SystemCoreClock变量，请使用下面注释的自定义
//#define system_core_clock 72000000  //如为其他主频，请设置好对应的主频(如果相关文件已定义则无需定义)
#define os_systick_off()      SysTick->CTRL&=~(1<<0)     //关闭系统定时器中断
#define os_systick_on()       SysTick->CTRL|=(1<<0)      //打开系统定时器中断
#define os_systick_irq_off()  SysTick->CTRL&=~(1<<1)     //关闭系统定时器中断
#define os_systick_irq_on()   SysTick->CTRL|=(1<<1)      //打开系统定时器中断
#define os_systick_count_1us  (system_core_clock/8/1000000)//系统定时器1us计数次数 (此处systick定时器使用主频8分频)
#define os_systick_load       SysTick->LOAD              //系统定时器重装载值寄存器
#define os_systick_val        SysTick->VAL               //系统定时器当前值寄存器
/*定时器初使化
*CTRL寄存器：bit0-定时器使能 bit1-中断使能 bit2-时钟源选择(=1系统主频，=0系统主频/8)  bit16-计数溢出位*/
void os_systick_init(void)//用作OS的计时脉冲  
{ 
	//Systick定时器初使化(使用其他定时器时，请修改为其他定时器)
	char *Systick_priority = (char *)0xe000ed23;       //Systick中断优先级寄存器
	SysTick->LOAD  = (os_systick_count_1us)* 1000; //Systick定时器重装载计数值:1ms(固定值)
	*Systick_priority = 0x00;                           //Systick定时器中断优先级
	SysTick->VAL   = 0;                                 //Systick定时器计数器清0
	SysTick->CTRL = 0x3;//Systick打开并使能中断且使用外部晶振时钟,8分频  72MHz/8=9MHz  计数9000次=1ms  计数9次=1us
}

/*PendSV初使化及触发中断函数(中断里有任务环境切换代码)
*移植时需要注释原工程中的PendSV_Handler中断函数,stm32工程中在stm32fxxx_it.c中*/
void os_pendsv_init(void)//PendSV初使化 
{
	char* NVIC_SYSPRI14= (char *)0xE000ED22;  //优先级寄存器地址
	*NVIC_SYSPRI14=0xFF;//设置PendSV中断优先级最低 
}
int* NVIC_INT_CTRL= (int *)0xE000ED04;  //中断控制寄存器地址
void os_pendsv_pulse(void)//触发PendSV中断
{
	os_cpu_interrupt_disable();
  *NVIC_INT_CTRL=0x10000000; 
	os_cpu_interrupt_enable();	
}
/*systick定时器中断函数
*移植时需要注释掉原工程中的SysTick_Handler中断函数,stm32工程中在stm32fxxx_it.c中*/
void SysTick_Handler(void) 
{
	os_p.os_clock_counter++;  //系统时钟节拍累加
	os_timer_counter_process();//系统定时器程序
  os_shell_run_time_counter_process();//系统运行时间及倒计时计算程序
  if((os_p.thread_time_slice--)== 0)  
	 {
		 os_shell_cpu_occrate_counter_process();//app/thread cou占用率统计程序 
		 os_p.thread_time_slice = THREAD_TIME_SLICE;//重置时间片初值
		 os_thread_same_prio_sched_and_switch();	//调度并切换任务		 
	 }  
}
os_bool os_tcb_prio_sort_table_rdy_check(os_tcb_prio_sort_table * os_tcb_prio_sort_table_struct)//检测输入的tcb列表是否至少一个线程处于就绪状态
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
os_tcb_prio_sort_table* os_tcb_prio_sort_table_highest_prio_rdy_get(void)//从优先级分组的tcb列表中查找已就绪的最高优先级线程
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_highestrio=os_tcb_prio_sort_table_list_head;
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_list_head;

	while(os_tcb_prio_sort_table_temp!=NULL)
	{
		if(os_tcb_prio_sort_table_temp->list_same_prio_head!=NULL)
		{
			if(os_tcb_prio_sort_table_rdy_check(os_tcb_prio_sort_table_temp)==os_true)//检测该优先级分组是否至少有一个线程处于就绪状态(一个都没有表示该分组没有线程在运行，无需要从中查找最高优先级)
			{
				if(os_tcb_prio_sort_table_highestrio->prio < os_tcb_prio_sort_table_temp->prio)
				{
					os_tcb_prio_sort_table_highestrio=os_tcb_prio_sort_table_temp;
				}
			}
		}
		os_tcb_prio_sort_table_temp=os_tcb_prio_sort_table_temp->next;//移动到下一分组
	}
	os_p.os_tcb_highest_prio_sort_table=os_tcb_prio_sort_table_highestrio;
	return os_tcb_prio_sort_table_highestrio;
}
os_tcb* os_tcb_same_prio_next_rdy_get(void)//从当前最高优先级组中获取下个顺序运行的线程
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
os_tcb* os_tcb_highest_prio_next_rdy_get(void)//先从最高优先级组中查找最高优先级分组，再从最高优先级分组中获取下个顺序运行的线程
{
	os_tcb *os_tcb_highestrio_rdy;

	os_tcb_prio_sort_table_highest_prio_rdy_get();
	os_tcb_highestrio_rdy = os_tcb_same_prio_next_rdy_get();
	return os_tcb_highestrio_rdy;
}
//线程时间片调度器
void os_thread_same_prio_scheduler(void)  //线程调度器(OS核心)
{	
/*-----------------------------------时间片------------------------------------------*/	
	if(os_p.current_prio==os_p.os_tcb_highest_prio_sort_table->prio)
	{
    os_tcb_rdy = os_tcb_same_prio_next_rdy_get(); //os_tcb_rdy
	}
	else //为了实现第一次运行空闲任务需要的
	{
	  os_thread_highest_prio_scheduler();
	}
/*-----------------------------------------------------------------------------------------*/	
}
//线程优先级调度器
void os_thread_highest_prio_scheduler(void)  //线程调度器(OS核心)
{	
/*-----------------------------------优先级------------------------------------------*/	
   os_tcb_rdy = os_tcb_highest_prio_next_rdy_get(); //os_tcb_rdy
/*-----------------------------------------------------------------------------------------*/	
}
void os_thread_same_prio_sched_and_switch(void)//线程调度并切换函数
{
	os_cpu_interrupt_disable();
	if(os_thread_p.sche_lock == 0) //任务切换锁定检测，>0说明有锁，==0无锁
	{
		os_thread_same_prio_scheduler();   //先调度，OSTCBNext获取到将要运行的任务
		if(os_tcb_rdy!=os_tcb_cur) 
		{
			os_pendsv_pulse(); //任务环境切换，将OSTCBNext切换到OSTCBCur运行	
		}
	}
	os_cpu_interrupt_enable();
}
void os_thread_highest_prio_sched_and_switch(void)//线程调度并切换函数
{
	os_cpu_interrupt_disable();
	if(os_thread_p.sche_lock == 0) //任务切换锁定检测，>0说明有锁，==0无锁
	{
		os_thread_highest_prio_scheduler();   //先调度，OSTCBNext获取到将要运行的任务
		if(os_tcb_rdy!=os_tcb_cur) 
		{
			os_p.current_prio=os_p.os_tcb_highest_prio_sort_table->prio;
			os_pendsv_pulse(); //任务环境切换，将OSTCBNext切换到OSTCBCur运行	
		}
	}
	os_cpu_interrupt_enable();
}

void os_timer_timeout_handle(void) //定时器定时完成处理
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
				 os_p.thread_time_slice = THREAD_TIME_SLICE;//重置时间片初值
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
						 os_p.thread_time_slice = THREAD_TIME_SLICE;//重置时间片初值
					 }
				 }
			}
			break;		
			default: 
			break;
		}
	}
}

void os_timer_counter_process(void) //系统定时器程序
{
	os_timer *os_timer_temp=os_timer_list_head->next;

	if(os_timer_list_head->next->clock_counter>1) os_timer_list_head->next->clock_counter--;//最小值设计为1
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
					os_timer_list_strutc_erase(os_timer_list_head->next);//从定时器定时列表中删除	    
				}
        case timer_type__thread:
				{					
					os_timer_temp=os_timer_list_head->next;//备份timer_list_head	
					os_timer_list_strutc_erase(os_timer_list_head->next);//从定时器定时列表中删除	
					os_thread_timer_recycle(os_timer_temp);//将 从定时器定时列表中删除的定时器回收到app延时定时器列表中		
				}
				break;				
				case timer_type__softtimer: 
				{
					os_timer_list_strutc_erase(os_timer_list_head->next);//从定时器定时列表中删除	
				}
				break;
				case timer_type__signal: //由于信号延时使用的是线程的专用延时，同timer_type__thread一样需要回收
				{
					os_timer_temp=os_timer_list_head->next;//备份timer_list_head	
					os_timer_list_strutc_erase(os_timer_list_head->next);//从定时器定时列表中删除	
					os_thread_timer_recycle(os_timer_temp);//将 从定时器定时列表中删除的定时器回收到app延时定时器列表中		
				}
				break;
				default: os_timer_list_strutc_erase(os_timer_list_head->next);//从定时器定时列表中删除	
				break;
			}		
      os_thread_highest_prio_sched_and_switch(); 			
			if(os_timer_list_head->next==os_timer_list_rear) break;
    }
	}
}

//任务堆栈初使化
//只要产生PendSV中断,跳转中断处理函数前 xPSR,PC,LR,R12,R3-R0被自动保存到系统或任务栈中(此步聚是PendSV中断硬件机制)，
//保存在哪个栈取决于当前的SP类型，如是MSP则保存到系统栈，如是PSP则保存到任务栈。本OS是保存于任务栈。而R11-R4需要手动保存到任务栈中
//入栈顺序：栈顶->栈尾 xPSR,PC,LR,R12,R3-R0,R4-R11共16个(SP(R13)保存在TCB首个成员变量中)。
stk32* os_thread_stk_init(void* thread,stk32 *topstkptr)
{
    stk32 *stk;
    stk = topstkptr;
    *(stk)    = (os_u32)0x01000000L;//xPSR 程序状态寄存器，xPSR T位(第24位)必须置1,否则第一次执行任务时进入Fault中断                                                     
    *(--stk)  = (os_u32)thread;       //PC   初使化时指向任务函数首地址（任务切换时，可能指向任务函数中间某地址）            
    *(--stk)  = (os_u32)0xFFFFFFFEL;//LR   保存着各种跳转的返回连接地址,初使化为最低4位为E，是一个非法值，主要目的是不让使用R14，即任务是不能返回的              
    stk-=13;	
    return stk;
}
void os_tcb_ptr_init(void)
{
	//开机直接运行最高优先级线程
	  //os_tcb_cur=os_tcb_prio_sort_table_highest_prio_rdy_get()->list_same_prio_cur;//开机直接运行最高优先级线程
	  //os_tcb_rdy = os_tcb_cur;
	//开机先运行空闲线程
    //os_tcb_cur = os_tcb_prio_sort_table_list_head->list_same_prio_head; //开机先运行空闲线程
    //os_tcb_rdy = os_tcb_cur;
	
	  
	  os_tcb_cur=os_tcb_prio_sort_table_list_head->list_same_prio_head; //开机先运行空闲线程
	  os_tcb_rdy=os_tcb_cur;
	  os_tcb_prio_sort_table_highest_prio_rdy_get();//开机空闲线程后运行最高优先级线程
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
	while(1);//等待环境切换PendSV中断
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
os_type_thread_id* os_thread_new_create(os_type_app_id* app,void* thread,os_str8 *str_name,os_u32 stk_size,os_u32 slice_tick,os_u32 prio,os_thread_state state,void *para)//任务创建函数
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
    os_tcb_thread->state = os_thread_state_readying;//创建时为就绪态(也可为其他态,调度器检测到就绪条件时再转为运行态)
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
			
			os_thread_timer_list_init();//初使化thread线程定时器链表			
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
		
		os_thread_timer_create();//同步线程创建一个thread专用的延时定时器
		
	  return os_tcb_thread;
}

void os_thread_sched_lock(void) //任务切换锁定  
{
	os_core_enter();
  if(os_thread_p.sche_lock<0xFFFFFFFF) os_thread_p.sche_lock++;  
	os_core_exit();
}

void os_thread_sched_unlock(void) //任务切换解锁 
{
	os_core_enter();                           
	if(os_thread_p.sche_lock>0) os_thread_p.sche_lock--;  
	os_core_exit();
} 


u32 os_clock_counter_get(void)
{
  return os_p.os_clock_counter;  //系统时钟节拍累计变量
}


//uS延时
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
//mS延时
void delay_ms(os_u32 ms)
{   
  delay_us(ms*1000);			
}

void os_thread_delay(os_u32 time_ticks)  //主动放弃CPU运行权,time_ticks=0 永久放弃，time_ticks>0放弃一段时间后恢复(延时)
{
	 os_u32 os_timer_para[8];
	 os_thread_sched_lock();
	 if(time_ticks>0) time_ticks=time_ticks+1;
   os_tcb_cur->delaytime=time_ticks;
	 os_tcb_cur->state=os_thread_state_delaying;
	 if(time_ticks>0)
	 {
		 os_timer_para[0]=os_tcb_cur->prio; //传递参数0:thread prio
		 os_timer_para[1]=os_tcb_cur->global_id; //传递参数1:thread global_id
		 os_timer_add(timer_type__thread,os_timer_para, time_ticks);
	 }
	 os_thread_sched_unlock();
	 os_thread_highest_prio_sched_and_switch();
	 while(os_tcb_cur->delaytime!=1);
}
