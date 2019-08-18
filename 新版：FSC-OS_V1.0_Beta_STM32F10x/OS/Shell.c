#include "shell.h"

//全局指令
char cmd_help[]={"cmd/help//"};   //指令帮助
char cmd_enter[]={"cmd/enter//"}; //进入指令系统
char cmd_exit[]={"cmd/exit//"};   //退出指令系统

//系统指令
char cmd_osinformation[]={"osinformation//"};//查看系统状态
char cmd_hardreset[]={"hardreset//"};        //硬件重启
char cmd_oson[]={"oson//"};                  //系统关闭
char cmd_osoff[]={"osoff//"};                //系统打开

OS_SHELL_CMD os_shell_cmd;
OS_SHELL_TIME os_shell_time;

void os_shell_input(os_u8 rx_byte)
{
	if(os_shell_cmd.bool_rx_ok==os_false)
	{
		if(os_shell_cmd.rx_counter<shell_rx_buff_len-1) 
			{
				os_shell_cmd.rx_buff[os_shell_cmd.rx_counter++]=rx_byte;
				if((os_shell_cmd.rx_buff[os_shell_cmd.rx_counter-1]=='/')&&(os_shell_cmd.rx_buff[os_shell_cmd.rx_counter-2]=='/'))
				{
				  os_shell_cmd.bool_rx_ok=os_true;	
					os_shell_cmd.rx_buff[os_shell_cmd.rx_counter]='\0';
          os_app_thread_state_set(App_System,thread_manager,os_thread_state_readying);
				}
			}
		else { os_shell_cmd.rx_counter=0; os_shell_cmd.rx_buff[os_shell_cmd.rx_counter]=rx_byte; }
	}
}

void os_thread_manager(void)
{
	 os_u32 delaytime;
   while(1)
	 {	 
      os_shell_handle_process(); 
		  if(os_p.state==os_state_running)
		  {
			  os_app_thread_state_set(App_System,thread_manager,os_thread_state_stopping);
			  os_thread_highest_prio_sched_and_switch();
			}
			else
			{
			  delaytime=1000000*1;
		    while(delaytime--);
			}
		}
}

void os_hard_reset(void) //硬件重启
{
	__disable_fault_irq();
  NVIC_SystemReset();
}
void os_hard_reset_tips(void)
{
  os_shell_printf("\r\n -----$$ HardReset $$-----\r\n\r\n");
}
void os_off(void) //关闭系统
{
  os_p.state=os_state_stopping;
}
void os_off_tips(void)
{
	os_shell_printf("\r\n -----$$ OS SLEEP  $$-----\r\n\r\n");
}
void os_on(void) //打开系统
{
  os_p.state=os_state_running;
}
void os_on_tips(void)
{
  os_shell_printf("\r\n -----$$ OS AWAKEN  $$-----\r\n\r\n");
}

void os_information_process(void)
{	
	os_u32 mem_free;
	os_u32 mem_frag;
	os_acb *os_acb_temp=os_acb_list_front;
	os_app_tcb *os_app_tcb_temp;
	os_thread_sched_lock();
	mem_free=os_mem_free_bytes_get();
	mem_frag=os_mem_fragment_rate_get();
	os_cpu_occrate_calculation_process();
	os_stk_calculation_process();
	os_thread_sched_unlock();
	
	os_shell_printf("++++++++++++++++++++++++++++++++++++++++++系统状态信息+++++++++++++++++++++++++++++++++++++++++++\r\n");
	os_shell_printf("=================================================================================================\r\n");
	os_shell_printf("  Designed @Angle145\r\n");
	os_shell_printf("  运行模式 ：oder+prio                                  运行时间：%-d年%-d月%-d日 %-d时%-d分%-d秒 %-dms\r\n",																				 
																								 os_shell_time.runtime_year,
																								 os_shell_time.runtime_mouth,
																								 os_shell_time.runtime_day,
																								 os_shell_time.runtime_hour,
																								 os_shell_time.runtime_minute,
																								 os_shell_time.runtime_second,
																								 os_shell_time.runtime_ms); 
	os_shell_printf("  时钟节拍 ：%-3dms                                      系统关闭倒计时：%-d年%-d月%-d日 %-d时%-d分%-d秒\r\n",
																								 THREAD_TIME_SLICE,
																								 os_shell_time.shutdowntime_year,
																								 os_shell_time.shutdowntime_mouth,
																								 os_shell_time.shutdowntime_day,
																								 os_shell_time.shutdowntime_hour,
																								 os_shell_time.shutdowntime_minute,
																								 os_shell_time.shutdowntime_second); 
			os_shell_printf("  CPU总占用率：%-4.1f%%                                    CPU最大占用率：%-4.1f%% \r\n",os_p.cpu_occrate_per100/10.0,os_p.cpu_occrate_per100_max/10.0);
			os_shell_printf("  内存容量 ：%-dByte                                  内存碎片 ：%-dByte   %4.1f%%\r\n",OS_MEMORYPOOL_SIZE,os_mem_fragment_rate_get(),mem_frag*100.0/OS_MEMORYPOOL_SIZE);      
			os_shell_printf("  已用内存 ：%-dByte                                   剩余内存 ：%-dByte\r\n",OS_MEMORYPOOL_SIZE-mem_free,mem_free);
			os_shell_printf("-------------------------------------------------------------------------------------------------\r\n");
			os_shell_printf("占用率   延时ms   使用栈   空闲栈   百分比   时间片    优先级    状态    线程名       程序名  \r\n");
			os_shell_printf("  CPU   DlyTime    Used     Free     Per    TimeSlice   Prio    State   ThreadName   AppName \r\n\r\n");
      while(os_acb_temp!=NULL)
			{
				os_shell_printf(" %4.1f%%   %-6d    %-6d   %-6d %5.1f%%   %5d    %5d       %d:%d                 %s\r\n",
				                 os_acb_temp->cpu_occrate_per100/10.0,
				                 0,
				                 os_acb_temp->stk_size_used,
				                 os_acb_temp->stk_size_free,
				                 os_acb_temp->stk_size_used*100.0/os_acb_temp->stk_size_sum,
				                 0,
				                 os_acb_temp->prio,
				                 os_acb_temp->wr,os_acb_temp->state,
				                 os_acb_temp->app_name);
				os_app_tcb_temp=os_acb_temp->tcb_list;
				while(os_app_tcb_temp!=NULL)
				{
					os_shell_printf(" %4.1f%%   %-6d    %-6d   %-6d %5.1f%%   %5d    %5d       %d:%d    %s\r\n",\
											os_app_tcb_temp->thread->cpu_occrate_per100/10.0,
											os_app_tcb_temp->thread->delaytime-1,
											os_app_tcb_temp->thread->stk_size_used,
											os_app_tcb_temp->thread->stk_size_sum-os_app_tcb_temp->thread->stk_size_used,
											os_app_tcb_temp->thread->stk_size_used*100.0/os_app_tcb_temp->thread->stk_size_sum,
											os_app_tcb_temp->thread->timeslice,
											os_app_tcb_temp->thread->prio,
											os_app_tcb_temp->thread->wr,os_app_tcb_temp->thread->state,
											os_app_tcb_temp->thread->thread_name  );	        					
					os_app_tcb_temp=os_app_tcb_temp->next;
				}
				os_shell_printf("\r\n");
				os_acb_temp=os_acb_temp->next;
			}
			os_shell_printf("=================================================================================================\r\n");
}

void os_stk_calculation_process(void) //app/thread 栈占用率计算程序
{
	os_u32 i;
	os_acb *os_acb_temp=os_acb_list_front;
	os_app_tcb *os_app_tcb_temp;
	while(os_acb_temp!=NULL)
	{
		os_acb_temp->stk_size_used=0;
		os_app_tcb_temp=os_acb_temp->tcb_list;
		while(os_app_tcb_temp!=NULL)
		{
			os_app_tcb_temp->thread->stk_size_free=0;
			for(i=0;i<os_app_tcb_temp->thread->stk_size_sum;i++)
			{
			  if(os_app_tcb_temp->thread->stk_addr[i]==0)
				{
				  os_app_tcb_temp->thread->stk_size_free++;
				}
				else
				{
					os_app_tcb_temp->thread->stk_size_used=os_app_tcb_temp->thread->stk_size_sum-os_app_tcb_temp->thread->stk_size_free;
				  break;
				}
			}
			os_acb_temp->stk_size_used+=os_app_tcb_temp->thread->stk_size_used;
			os_acb_temp->stk_size_free=os_acb_temp->stk_size_sum-os_acb_temp->stk_size_used;
			os_app_tcb_temp=os_app_tcb_temp->next;
		}
		os_acb_temp=os_acb_temp->next;
	}	
}
void os_cpu_occrate_calculation_process(void) //app/thread cou占用率计算程序
{
	os_acb *os_acb_temp=os_acb_list_front;
	os_app_tcb *os_app_tcb_temp;
	if(os_p.os_clock_counter>1000)
	{
		while(os_acb_temp!=NULL)
		{
			os_acb_temp->cpu_occrate_per100=0;
			os_app_tcb_temp=os_acb_temp->tcb_list;
			while(os_app_tcb_temp!=NULL)
			{
				os_app_tcb_temp->thread->cpu_occrate_per100=os_app_tcb_temp->thread->cpu_occrate_counter*1000/os_p.cpu_occrate_counter;        
				if((os_acb_temp==os_acb_list_front)&&(os_app_tcb_temp==os_acb_temp->tcb_list)) 
				{
					os_p.cpu_occrate_per100=1000-os_app_tcb_temp->thread->cpu_occrate_per100;
					if(os_p.cpu_occrate_per100_max<os_p.cpu_occrate_per100) os_p.cpu_occrate_per100_max=os_p.cpu_occrate_per100;
				}
				os_acb_temp->cpu_occrate_per100+=os_app_tcb_temp->thread->cpu_occrate_per100;
				os_app_tcb_temp=os_app_tcb_temp->next;
			}
			os_acb_temp=os_acb_temp->next;
		}	
	} 
}
void os_shell_cpu_occrate_counter_process(void) //app/thread cou占用率统计程序（放在systick中断中）
{	
	 os_acb *os_acb_temp=os_acb_list_front;
	 os_app_tcb *os_app_tcb_temp;
	 os_tcb_cur->cpu_occrate_counter++;
	 os_p.cpu_occrate_counter++;
	 if(os_p.os_clock_counter==0) os_p.cpu_occrate_counter=0;
	 if(os_p.cpu_occrate_counter==0)
	 {
      while(os_acb_temp!=NULL)
			{
				os_app_tcb_temp=os_acb_temp->tcb_list;
				while(os_app_tcb_temp!=NULL)
				{
				  os_app_tcb_temp->thread->cpu_occrate_counter=0;        
					os_app_tcb_temp=os_app_tcb_temp->next;
				}
				os_acb_temp=os_acb_temp->next;
			}
		 os_p.cpu_occrate_counter=1;//从1开始计数(0不能作除数)		 
	 }  
}
void os_shell_run_time_counter_process(void) //系统运行时间及倒计时计算程序(放在systick中断中)
{
	 if(os_shell_time.runtime_ms<1000) os_shell_time.runtime_ms++;
	 else
	 {
		 os_shell_time.runtime_ms=0;
		 os_shell_time.runtime_second++;
		 if(os_shell_time.runtime_second>=60) {
  		  os_shell_time.runtime_second=0;os_shell_time.runtime_minute++;
		 if(os_shell_time.runtime_minute>=60) { 
		    os_shell_time.runtime_minute=0;os_shell_time.runtime_hour++;
		 if(os_shell_time.runtime_hour>=24)   { 
		    os_shell_time.runtime_hour=0;os_shell_time.runtime_day++;
		 if(os_shell_time.runtime_hour>=24)   { 
		    os_shell_time.runtime_hour=0;os_shell_time.runtime_day++;
		 if(os_shell_time.runtime_day>=30)    { 
		    os_shell_time.runtime_day=0;os_shell_time.runtime_mouth++;
		 if(os_shell_time.runtime_mouth>=12)  { 
		    os_shell_time.runtime_mouth=0;os_shell_time.runtime_year++;
		 }}}}}} 
  
		 if((os_shell_time.shutdowntime_second==0)&&(\
        (os_shell_time.shutdowntime_minute!=0)||\
				(os_shell_time.shutdowntime_hour  !=0)||\
				(os_shell_time.shutdowntime_day   !=0)||\
				(os_shell_time.shutdowntime_mouth !=0)||\
				(os_shell_time.shutdowntime_year  !=0)))
		 {	
       os_shell_time.shutdowntime_second=1;
		 }				
		 if((os_shell_time.shutdowntime_second==2)&&\
        (os_shell_time.shutdowntime_minute==0)&&\
				(os_shell_time.shutdowntime_hour  ==0)&&\
				(os_shell_time.shutdowntime_day   ==0)&&\
				(os_shell_time.shutdowntime_mouth ==0)&&\
				(os_shell_time.shutdowntime_year  ==0))
			{
				os_shell_time.shutdowntime_second=0;
				os_p.state=os_state_stopping;
				os_app_thread_state_set(App_System,thread_manager,os_thread_state_readying);
			  os_thread_highest_prio_sched_and_switch();
				os_shell_printf("定时关闭完成，系统停止运行\r\n");
			}
			else
			{
			 if(os_shell_time.shutdowntime_second>1) os_shell_time.shutdowntime_second--;	
			 if(os_shell_time.shutdowntime_second==1) 
				 {
					os_shell_time.shutdowntime_second=60;
				  if(os_shell_time.shutdowntime_minute>0) 
					{
						os_shell_time.shutdowntime_minute--;
					}
					else if(os_shell_time.shutdowntime_hour>0) 
					{
					  os_shell_time.shutdowntime_hour--;
						os_shell_time.shutdowntime_minute=59;
					}
					else if(os_shell_time.shutdowntime_day>1) 
					{
						os_shell_time.shutdowntime_day--;
					  os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}		
					else if(os_shell_time.shutdowntime_mouth>1) 
					{
						os_shell_time.shutdowntime_mouth--;
						os_shell_time.shutdowntime_day=30;
					  os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}		
					else if(os_shell_time.shutdowntime_year>1) 
					{
						os_shell_time.shutdowntime_year--;
						os_shell_time.shutdowntime_mouth=12;
						os_shell_time.shutdowntime_day=30;
					  os_shell_time.shutdowntime_hour=23;
						os_shell_time.shutdowntime_minute=59;
					}												
				}			
			}
	 }
}
void os_shutdown_delaytime_set_ymdhms(os_u32 year,os_u8 mouth,os_u8 day,os_u8 hour,os_u8 minute,os_u8 second) //系统关闭定时时间设置
{			 
	os_shell_time.shutdowntime_second=second;
  os_shell_time.shutdowntime_minute=minute;
	os_shell_time.shutdowntime_hour  =hour;
	os_shell_time.shutdowntime_day   =day;	
	os_shell_time.shutdowntime_mouth =mouth;
	os_shell_time.shutdowntime_year  =year;
}
void os_shell_rx_buff_clean(void)
{
  os_u8 i;
	for(i=0;i<strlen(os_shell_cmd.rx_buff);i++)
	{
	  os_shell_cmd.rx_buff[i]=0;
	}
}
os_u32 os_shell_tring_len_get(char* p)//获取字符串p的长度
{
  os_u32 i=0;
	while(*p!='\0')
	{
	  i++;
		p++;
	}
	return i;
}
os_u8 os_shell_compare(char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p内容是否一致
{
  return strncmp(os_shell_cmd.rx_buff,p,os_shell_tring_len_get(p));
}

os_u32 os_char_in_string_position_get(char* p,char ch)//获取字符ch在字符串p中的位置
{
  os_u32 i=0;
	while((*p!=ch)&&(*p!='\0'))
	{
	  i++;
		p++;
	}
	return i;
}
os_u32* os_shell_tring_num_get(char* p)//从字符串p中提取数字保存到全局变量CmdNumBuff[]数组中
{
	os_bool neg_flag=os_false;
	os_u16 i=0;
  for(i=0;i<shell_rx_buff_len;i++)
	{
	  os_shell_cmd.rx_dig_buff[i]=0;
	}
	for(i=0;i<shell_rx_buff_len;i++)
	{
	  while(((*p<'0')||(*p>'9'))&&(*p!='\0')) p++;
		if(*(p-1)=='-') neg_flag=os_true;
		while((*p>='0')&&(*p<='9'))
		{
		  os_shell_cmd.rx_dig_buff[i]=os_shell_cmd.rx_dig_buff[i]*10+(*p-'0');
			p++;
		}
		if(neg_flag==os_true) os_shell_cmd.rx_dig_buff[i]=~os_shell_cmd.rx_dig_buff[i]+1;
	}
	return os_shell_cmd.rx_dig_buff;
}
os_u8 os_shell_compare_dig(char ch,char* p)//比较字符串OS_Perip.OS_USART_RX_BUFF和字符串p中，字符ch以前的内容是否一致
{
  return strncmp(os_shell_cmd.rx_buff,p,os_char_in_string_position_get(p,ch));
}
os_u32 os_shell_grab_dig(os_u8 n)//获取字符串p中的第n个数字(n=0开始)
{
	os_u32* pf;
	pf=os_shell_tring_num_get(os_shell_cmd.rx_buff);
  return pf[n];
}
void os_shell_handle_process(void)
{
		if(os_shell_cmd.bool_rx_ok==os_true)
		{	
			os_shell_printf("%s\r\n",os_shell_cmd.rx_buff);
			if(os_shell_compare(cmd_help)==0)		{ /*OS_CmdHelp_Process(); */}			
			if(os_shell_compare(cmd_enter)==0)  { os_shell_cmd.bool_enter_sys=os_true; }		
      if(os_shell_compare(cmd_exit)==0)   { os_shell_cmd.bool_enter_sys=os_false; }	
      if(os_shell_cmd.bool_enter_sys==os_true)
			{				
	#if (OS_CMD_NSY_ENABLE == 1)
				/********************************************任务打开****************************************/	//任务1对应TCB[2]		
				if(os_shell_compare(cmd_task_open_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_RUNNING);OSDisp_task1open(); }                     
				if(os_shell_compare(cmd_task_open_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_RUNNING);OSDisp_task2open(); }																		
				if(os_shell_compare(cmd_task_open_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_RUNNING);OSDisp_task3open(); }                                    
				if(os_shell_compare(cmd_task_open_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_RUNNING);OSDisp_task4open(); }	                                    
				if(os_shell_compare(cmd_task_open_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_RUNNING);OSDisp_task5open(); }			                                  
				//如任务超过5个请自行增加
				
				/*******************************************************************************************/	
				/********************************************任务关闭****************************************/	
				if(os_shell_compare(cmd_task_close_01)==0) { OSTaskStateSet((void*)OSTCBTbl[2].TaskAdd,TASK_PAUSING);OSDisp_task1close(); }
				if(os_shell_compare(cmd_task_close_02)==0) { OSTaskStateSet((void*)OSTCBTbl[3].TaskAdd,TASK_PAUSING);OSDisp_task2close(); }
				if(os_shell_compare(cmd_task_close_03)==0) { OSTaskStateSet((void*)OSTCBTbl[4].TaskAdd,TASK_PAUSING);OSDisp_task3close(); }
				if(os_shell_compare(cmd_task_close_04)==0) { OSTaskStateSet((void*)OSTCBTbl[5].TaskAdd,TASK_PAUSING);OSDisp_task4close(); }
				if(os_shell_compare(cmd_task_close_05)==0) { OSTaskStateSet((void*)OSTCBTbl[6].TaskAdd,TASK_PAUSING);OSDisp_task5close(); }
				//如任务超过5个请自行增加
				
				/*******************************************************************************************/		
				/*******************************************任务优先级***************************************/	
				if(os_shell_compare_dig('=',cmd_task_prio_01)==0) { OSTaskPrioSet((void*)OSTCBTbl[2].TaskAdd,GrabCmdDig(1));OSTask1PrioSet(); }	                                                                            
				if(os_shell_compare_dig('=',cmd_task_prio_02)==0) { OSTaskPrioSet((void*)OSTCBTbl[3].TaskAdd,GrabCmdDig(1));OSTask2PrioSet(); }		                                       
				if(os_shell_compare_dig('=',cmd_task_prio_03)==0) { OSTaskPrioSet((void*)OSTCBTbl[4].TaskAdd,GrabCmdDig(1));OSTask3PrioSet(); }	                                       
				if(os_shell_compare_dig('=',cmd_task_prio_04)==0) { OSTaskPrioSet((void*)OSTCBTbl[5].TaskAdd,GrabCmdDig(1));OSTask4PrioSet(); }	                                      
				if(os_shell_compare_dig('=',cmd_task_prio_05)==0) { OSTaskPrioSet((void*)OSTCBTbl[6].TaskAdd,GrabCmdDig(1));OSTask5PrioSet(); }	                                       
				//如任务超过5个请自行增加
				/*******************************************************************************************/		
				/*******************************************任务时间片***************************************/	
				if(os_shell_compare_dig('=',cmd_task_timeslice_01)==0){ OSTCBTbl[2].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task1TimeSlice();}
				if(os_shell_compare_dig('=',cmd_task_timeslice_02)==0){ OSTCBTbl[3].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task2TimeSlice();}
				if(os_shell_compare_dig('=',cmd_task_timeslice_03)==0){ OSTCBTbl[4].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task3TimeSlice();}
				if(os_shell_compare_dig('=',cmd_task_timeslice_04)==0){ OSTCBTbl[5].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task4TimeSlice();}
				if(os_shell_compare_dig('=',cmd_task_timeslice_05)==0){ OSTCBTbl[6].TaskTimeSlice=GrabCmdDig(1);OSDisp_Task5TimeSlice();}
				//如任务超过5个请自行增加
				/****************************************用户自定义指令区*************************************/
				
				

				/*******************************************************************************************/
	#endif	
				/*******************************************************************************************/
	#if (OS_SYSTIME_ENABLE== 1)			
				if(os_shell_compare_dig('=',cmd_systime_set)==0) { RTC_SetTimes(os_shell_grab_dig(0),\
																														os_shell_grab_dig(1),\
																														os_shell_grab_dig(2),\
																														os_shell_grab_dig(3),\
																														os_shell_grab_dig(4),\
																														os_shell_grab_dig(5)
																													 );
																														OSDisp_systimeset(); }		
	#endif			
				/*******************************************************************************************/	
	#if (OS_TIMECOUNTSYS_ENABLE == 1)			
				if(os_shell_compare_dig('=',cmd_osoff_delaytime_set)==0) { OS_ShutDown_DelayTime_Set_YMDHMS(\
																														os_shell_grab_dig(0),\
																														os_shell_grab_dig(1),\
																														os_shell_grab_dig(2),\
																														os_shell_grab_dig(3),\
																														os_shell_grab_dig(4),\
																														os_shell_grab_dig(5)
																													 );
																														OSDisp_osoff_delaytime(); }		
	#endif																													
				/*******************************************************************************************/	
				if(os_shell_compare(cmd_osinformation)==0) { os_information_process();  }		
        if(os_shell_compare(cmd_hardreset)==0)     { os_hard_reset_tips();  os_hard_reset(); }
				if(os_shell_compare(cmd_oson)==0)                { os_on_tips();  os_on(); }
				if(os_shell_compare(cmd_osoff)==0)               { os_off_tips();  os_off(); }				
//				if(os_shell_compare(cmd_runmode_order)==0) { OS_System.RuningMode=0; OSDisp_runmode_order(); }
//				if(os_shell_compare(cmd_runmode_prio)==0)  { OS_System.RuningMode=1; OSDisp_runmode_prio();  }
//				if(os_shell_compare(cmd_runmode_order_prio)==0){ OS_System.RuningMode=2; OSDisp_runmode_order_prio();  }			

				
				/*******************************************************************************************/	
				os_shell_rx_buff_clean();
				os_shell_printf("cmd->");
			}		
	  }
    os_shell_cmd.rx_counter=0;
    os_shell_cmd.bool_rx_ok=os_false;		 	
}	
