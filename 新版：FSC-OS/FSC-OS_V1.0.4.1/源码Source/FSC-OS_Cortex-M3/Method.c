#include "method.h"

os_error_code_type os_app_name_set(os_type_app_id *app,os_str8 *name_str)
{
	os_error_code_type error_code_type;
	os_u32 i=0;
	while(name_str[i]!='\0')
	{
	  app->app_name[i]=name_str[i];
		i++;
	}
	return error_code_type;
}
os_error_code_type os_app_prio_set(os_type_app_id *app,os_u32 prio)
{
	os_error_code_type error_code_type;
  app->prio=prio;

	return error_code_type;
}
os_error_code_type os_app_thread_state_set(os_acb *os_acb_app,os_tcb *os_tcb_thread,os_thread_state thread_state) //app单个线程状态设置
{
	os_error_code_type error_code_type;
  os_app_tcb *os_app_tcb_temp=os_acb_app->tcb_list;
  while(os_app_tcb_temp!=NULL)
  {
	  if(os_app_tcb_temp->thread==os_tcb_thread)
	  {
		  os_app_tcb_temp->thread->state=thread_state;
		  break;
	  }
	  os_app_tcb_temp=os_app_tcb_temp->next;
  }
	return error_code_type;
}
os_error_code_type os_app_all_thread_state_set(os_acb *os_acb_app,os_thread_state thread_state) //app所有线程状态设置
{
	os_error_code_type error_code_type;
	os_app_tcb *os_app_tcb_temp=os_acb_app->tcb_list;
	while(os_app_tcb_temp!=NULL)
	{
		os_app_tcb_temp->thread->state=thread_state;
	 os_app_tcb_temp=os_app_tcb_temp->next;
	}
	return error_code_type; 
}
//APP启动器
os_error_code_type os_app_open(os_acb *os_acb_app)//APP启动函数 启动指定的APP运行
{
	os_error_code_type error_code_type;
	if(os_app_p.sche_lock == 0) //app切换锁定检测，>0说明有锁，==0无锁
	{	  
		os_acb_app->state=os_app_state_readying;	
		os_app_all_thread_state_set(os_acb_app,os_thread_state_readying);
	}
	return error_code_type;
}
//APP关闭器
os_error_code_type os_app_close(os_acb *os_acb_app)//APP关闭函数 关闭指定的APP，停止运行
{
	os_error_code_type error_code_type;	
	if(os_app_p.sche_lock == 0) //app切换锁定检测，>0说明有锁，==0无锁
	{
	  os_acb_app->state=os_app_state_stopping;
    os_app_all_thread_state_set(os_acb_app,os_thread_state_stopping);
	}
	return error_code_type;
}
os_error_code_type os_app_state_set(os_acb *os_acb_app,os_app_state app_state)//APP状态设置函数 
{
	os_error_code_type error_code_type;
	os_acb_app->state=app_state;
	switch(app_state)
	{
	  case os_app_state_creating:     os_app_all_thread_state_set(os_acb_app,os_thread_state_creating);     break;
		case os_app_state_readying:     os_app_all_thread_state_set(os_acb_app,os_thread_state_readying);     break;
		case os_app_state_pausing:      os_app_all_thread_state_set(os_acb_app,os_thread_state_pausing);      break;
		case os_app_state_delaying:     os_app_all_thread_state_set(os_acb_app,os_thread_state_delaying);     break;
		case os_app_state_backrunning:  os_app_all_thread_state_set(os_acb_app,os_thread_state_backrunning);  break;
		case os_app_state_headrunning:  os_app_all_thread_state_set(os_acb_app,os_thread_state_headrunning);  break;
		case os_app_state_stopping:     os_app_all_thread_state_set(os_acb_app,os_thread_state_stopping);     break;
		case os_app_state_deleting:     os_app_all_thread_state_set(os_acb_app,os_thread_state_deleting);     break;
		default:break;
	}
	return error_code_type;
}
