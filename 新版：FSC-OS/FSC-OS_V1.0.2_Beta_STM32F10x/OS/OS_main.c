/*
注：stm32f1xx系列芯片，请保证每个线程至少释放>=2ms的时间才能保证线程正常调转运行
*/
#include "os.h"  //使用内核
#include "usart.h" 
#include "app_01.h" 
#include "app_02.h" 
	
static void os_thread_idle(void);

os_type_app_id* os_app_create__App_System(void) //创建App_System （系统APP）
{
	  os_type_app_id *app_id=os_app_new_init();
		os_app_name_set(app_id,"APP_SYSTEM");
	  os_app_prio_set(app_id,0);
	  os_app_state_set(app_id,os_app_state_creating);
	
    //                                (不变)   (线程函数名)  (线程名称)  (线程堆栈大小)(时间片)(优先级)    (状态)       (参数)  //时间片和参数暂时没用
	  //                                app_id   app_thread app_thread_name stk_size slice  prio         state       para
    thread_idle=os_thread_new_create( app_id, os_thread_idle, "thread_idle", 128, 1, 0, os_thread_state_readying, (void*)0 ); //创建空闲线程 
	  thread_manager=os_thread_new_create( app_id, os_thread_manager, "thread_manager", 256, 1, (os_u32)-1, os_thread_state_readying, (void*)0 ); //创建系统管理线程 
	
	  os_app_new_create(app_id);
	  return app_id;
}
static void os_thread_idle(void) //空闲任务，用于防止用户任务都处于暂停或为数量为0时出错(禁止调用os_thread_delay())
{
	os_printf("app_idle is running\r\n");
	while(1) 
	{
    //禁止调用os_thread_delay()
	}
}
void os_user_init(void) //用户代码初使化函数（用户所有初使化函数均放在此函数内,初使化函数禁止调用os_thread_delay()和禁止调度）
{
	  USART1_Config(115200); //串口1初使化
	  printf("-@OS Inside\r\n");
	
	  os_mbox_01=os_mbox_create();
}	
void os_main(void)
{                     
	  App_System=os_app_create__App_System(); //创建系统APP（系统自带，用户不可更改）
    App_01=os_app_create__App_01();         //创建用户APP_01
	  App_02=os_app_create__App_02();         //创建用户APP_02
	
    os_init_and_startup();//系统初使化并启动
}
