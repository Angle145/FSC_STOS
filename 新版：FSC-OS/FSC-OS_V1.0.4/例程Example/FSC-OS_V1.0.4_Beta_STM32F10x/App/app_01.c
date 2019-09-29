#include "app_01.h"
#include "STM32F10x_C51Mode.h"

static void thread_01(void);
static void thread_02(void);
static void thread_03(void);

os_type_app_id* os_app_new_create__App_01(void) //创建APP_01
{
	  os_type_app_id *app_id=os_app_new_create();
		os_app_name_set(app_id,"APP_01");
	  os_app_prio_set(app_id,1);
	  os_app_state_set(app_id,os_app_state_creating);
	
	  //                    (不变)   (线程函数名)  (线程名称)  (线程堆栈大小)(时间片)(优先级)     (状态)               (参数)  //时间片和参数暂时没用
	  //                    app_id   app_thread app_thread_name stk_size slice  prio         state                para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //创建线程1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //创建线程2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //创建线程3

	  os_app_new_init(app_id); 
	  return app_id;
}
	
static void thread_01(void)  //APP_01的线程01
{  
	while(1) 
	 {	 		 
     os_printf("APP_01  thread_01\r\n");
		 os_thread_delay(25000);  	//1000ms执行一次任务，这个时间越小，本任务执行的间隔越小！
   } 
}

static void timer_print_callback(void)	//timer_print定时完成回调函数
{
	os_printf("You are so good!---- \r\n");
}
static void timer_delte_callback(void)	//timer_delte定时完成回调函数
{
	os_timer_delete(timer_print);//删除定时器timer_print
	os_timer_delete(timer_delte);//删除定时器timer_delte本身
}
static void thread_02(void)  //APP_01的线程02
{	
	//例程：软件定时器
	//功能：两个定时器，一个定时打印输出，一个定时删除上一个定时器
	timer_print=os_timer_new_create(timer_print_callback,timer_mode_type__cycle,5000);//初使化创建定时器timer_print，定时5000ms
	timer_delte=os_timer_new_create(timer_delte_callback,timer_mode_type__once,25000);//初使化创建定时器timer_delte，定时25000ms
	while(1)
	{
		os_printf("APP_01  thread_02\r\n");
		os_thread_delay(1000);			//1000ms执行一次任务
	}
}

static void thread_03(void) //APP_01的线程03
{
	os_u8 arr[]="hello world!";
	while(1) 
	 {
		 os_printf("APP_01  thread_03\r\n");	
		 os_thread_delay(3000);			//2000ms执行一次任务	
     os_mbox_post(os_mbox_01,arr,sizeof(arr));		 
	 }			
}


