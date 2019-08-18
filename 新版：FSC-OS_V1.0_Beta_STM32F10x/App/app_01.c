#include "app_01.h"
#include "STM32F10x_C51Mode.h"

static void thread_01(void);
static void thread_02(void);
static void thread_03(void);

os_type_app_id* os_app_create__App_01(void) //创建APP_01
{
	  os_type_app_id *app_id=os_app_new_init();
		os_app_name_set(app_id,"APP_01");
	  os_app_prio_set(app_id,1);
	  os_app_state_set(app_id,os_app_state_creating);
	
	  //                    (不变)   (线程函数名)  (线程名称)  (线程堆栈大小)(时间片)(优先级) (参数)  //时间片和参数暂时没用
	  //                    app_id   app_thread app_thread_name stk_size slice  prio    para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    2,   (void*)0  ); //创建线程1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    2,   (void*)0  ); //创建线程2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    3,   (void*)0  ); //创建线程3

	  os_app_new_create(app_id); 
	  return app_id;
}
	
static void thread_01(void)  //APP_01的线程01
{   
	while(1) 
	 {	 		 
     os_printf("APP_01  thread_01\r\n");
		 os_thread_delay(500);  	//1000ms执行一次任务，这个时间越小，本任务执行的间隔越小！
   } 
}

static void thread_02(void)  //APP_01的线程02
{	
	while(1) 
	{
		os_printf("APP_01  thread_02\r\n");
		os_thread_delay(1000);			//1000ms执行一次任务
	}
}

static void thread_03(void) //APP_01的线程03
{
	while(1) 
	 {
		 os_printf("APP_01  thread_03\r\n");	
		 os_thread_delay(2000);			//2000ms执行一次任务				
	 }			
}


