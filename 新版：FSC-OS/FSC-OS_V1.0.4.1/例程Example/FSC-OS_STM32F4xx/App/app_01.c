#include "app_01.h"

#include "led.h"  	//用户代码头文件

static void thread_01(void);//线程函数声明
static void thread_02(void);
static void thread_03(void);

os_type_app_id* os_app_new_create__App_01(void) //创建APP_01
{
	  os_type_app_id *app_id=os_app_new_create();//获取一个新的APP ID
		os_app_name_set(app_id,"APP_01");  //设置APP字符串名字
	  os_app_prio_set(app_id,1);					//设置APP优先级
	  os_app_state_set(app_id,os_app_state_creating);//设置APP状态
	
		//创建APP线程
	  //                    (不变)   (线程函数名)  (线程名称)  (线程堆栈大小)(时间片)(优先级)     (状态)               (参数)  //时间片和参数暂时没用
	  //                    app_id   app_thread app_thread_name stk_size slice  prio         state                para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //创建线程1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //创建线程2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //创建线程3

	  os_app_new_init(app_id); //按照以上参数初使化APP并将APP加入系统
	  return app_id;
}
	
static void thread_01(void)  //APP_01的线程01
{  
	while(1) 
	 {	 		 
     os_printf("APP_01  thread_01\r\n"); //示例代码，使用时删除
		 os_thread_delay(4000);  	//4000ms执行一次任务，这个时间越小，本任务执行的间隔越小！ //示例代码，使用时删除
   } 
}
static void thread_02(void)  //APP_01的线程02
{	
	while(1)
	{
		//示例代码，使用时删除
		LED1_ON();							//LED闪烁(可能需要移植才能工作)
		os_thread_delay(500);		//闪烁频率1s
		LED1_OFF();
		os_thread_delay(500);		
		os_printf("APP_01  thread_02\r\n");
	}
}
static void thread_03(void) //APP_01的线程03
{
	while(1) 
	 {
		 os_printf("APP_01  thread_03\r\n");	//示例代码，使用时删除
		 os_thread_delay(2000);			//2000ms执行一次任务		//示例代码，使用时删除 
	 }			
}


