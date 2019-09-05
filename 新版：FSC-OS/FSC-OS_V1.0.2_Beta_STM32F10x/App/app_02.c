#include "app_02.h"
#include "STM32F10x_C51Mode.h"

extern os_type_app_id *App_01;
extern os_type_app_id *App_02;

static void thread_01(void);
static void thread_02(void);
static void thread_03(void);


os_type_app_id* os_app_create__App_02(void) //创建APP_02
{
	  os_type_app_id *app_id=os_app_new_init();
		os_app_name_set(app_id,"APP_02");
	  os_app_prio_set(app_id,2);
	  os_app_state_set(app_id,os_app_state_creating);
	
	  //                    (不变)   (线程函数名)  (线程名称)  (线程堆栈大小)(时间片)(优先级)   (状态)                (参数)  //时间片和参数暂时没用
	  //                    app_id   app_thread app_thread_name stk_size slice  prio        state                para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //创建线程1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //创建线程2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    5,  os_thread_state_readying,  (void*)0  ); //创建线程3
	
	  os_app_new_create(app_id);
	  return app_id;
}
	
static void thread_01(void)  //APP_02的线程01
{   
	while(1) 
	 {	 		 
     os_printf("APP_02  thread_01\r\n");
		 os_thread_delay(4000);  	//1000ms执行一次任务，这个时间越小，本任务执行的间隔越小！
   } 
}

static void thread_02(void)  //APP_02的线程02
{	
	while(1) 
	{
		os_printf("APP_02  thread_02\r\n");
		os_thread_delay(8000);			//1000ms执行一次任务 	
	}
}

static void thread_03(void) //APP_02的线程03
{
	os_u8 *p;
	while(1) 
	 {
		  os_printf("APP_02  thread_03\r\n");	
		  os_thread_delay(2000);  
		  p=os_mbox_pend(os_mbox_01,pend_type__block,0);
		  if(p!=(void*)0) { os_printf("%s\r\n",p);}	
	 }			
}


