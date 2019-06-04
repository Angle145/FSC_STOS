/***
	***************************************************************************
	*	@file  	delay.c
	*	@version V1.0.0	
	*	@brief   delay接口相关函数
   ***************************************************************************
   *  @description
	*
	*  SysTick定时器配置为1ms中断，实现毫秒延时
	* 	
	***************************************************************************
***/

#include "delay.h"

static __IO u32 TimingDelay;  //计数变量


//	函数：延时初始化
//	说明：配置 SysTick 为1ms中断，并启动定时器
//
void Delay_Init(void)
{
	SysTick_Config(SystemCoreClock / 1000);  //配置SysTick时钟为1ms中断
}

//	函数：计时函数
//	说明：在 SysTick 中断服务函数里被调用
//
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0)
	{ 
		TimingDelay--;
	}
}

//	函数：毫秒延时
// 参数：nTime - 延时时间，单位ms
//	说明：每次调用都会重新给TimingDelay赋值，实现 n 毫秒的延时，最大延时 4294967295 ms。	
//
void Delay_ms(u32 nTime)
{ 
	TimingDelay = nTime;

	while(TimingDelay != 0);
}




