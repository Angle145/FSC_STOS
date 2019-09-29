#ifndef _OS_SHELL_
#define _OS_SHELL_
#include "os.h"
#include "string.h"

#define os_shell_printf(fmt, ...) { os_thread_sched_lock(); printf( fmt, ##__VA_ARGS__); os_thread_sched_unlock();}//OS提供的打印输出函数
#define shell_rx_buff_len 50
#define shell_rx_dig_buff_len 10
typedef struct
{
  char rx_buff[shell_rx_buff_len];
	os_u32 rx_dig_buff[shell_rx_dig_buff_len];
	os_u32 rx_counter;
	os_bool bool_rx_ok;
	os_bool bool_enter_sys;
}OS_SHELL_CMD;
extern OS_SHELL_CMD os_shell_cmd;

typedef struct
{
	os_u16 runtime_ms;
	
	os_u32 runtime_year;
	os_u32 shutdowntime_year;
	
	os_u8  runtime_second;
	os_u8  runtime_minute;
	os_u8  runtime_hour;
	os_u8  runtime_day;
	os_u8  runtime_mouth;
	
  os_u8  shutdowntime_second;
	os_u8  shutdowntime_minute;
	os_u8  shutdowntime_hour;
	os_u8  shutdowntime_day;
	os_u8  shutdowntime_mouth;
}OS_SHELL_TIME;
extern OS_SHELL_TIME os_shell_time;

void os_shell_input(os_u8 rx_byte);
void os_thread_manager(void);
void os_shell_handle_process(void);
void os_cpu_occrate_calculation_process(void); //app/thread cou占用率计算程序
void os_shell_cpu_occrate_counter_process(void); //app/thread cou占用率统计程序（放在systick中断中）
void os_stk_calculation_process(void); //app/thread 栈占用率计算程序
void os_shell_run_time_counter_process(void); //系统运行时间及倒计时计算程序(放在systick中断中)
void os_shutdown_delaytime_set_ymdhms(os_u32 year,os_u8 mouth,os_u8 day,os_u8 hour,os_u8 minute,os_u8 second); //系统关闭定时时间设置
#endif
