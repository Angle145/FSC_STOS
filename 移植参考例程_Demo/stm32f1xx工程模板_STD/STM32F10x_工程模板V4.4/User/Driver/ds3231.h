#ifndef __DS3231_H_
#define __DS3231_H_

#include "i2c_gpio.h"
#include "stm32f10x.h"
#include "fsc_stos.h"

#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位) 

#define SECOND DS3231_SECOND        //秒
#define MINUTE DS3231_MINUTE        //分
#define HOUR DS3231_HOUR            //时
#define WEEK DS3231_WEEK            //星期
#define DAY DS3231_DAY              //日
#define MONTH DS3231_MONTH          //月
#define YEAR DS3231_YEAR            //年

typedef struct
{
	u8 Year;	
	u8 Month;
	u8 Day;
	u8 Week;
  u8 Hour;
	u8 Minute;
	u8 Second;
	u8 DateString[8];
	u8 TimeString[8];
	u8 SecCount;
} SYSTEMTIME;		

extern SYSTEMTIME CurrentTime;	//当前实时时间结构体
extern u8 const mon_table[12];	//月份日期数据表

u8 BCD_HEX(u8 val);
u16 HEX_BCD(u8 val);
void DS3231_I2cByteWrite(u8 addr,u8 bytedata);
u8 DS3231_I2cByteRead(u8 addr);
void DS3231_SetOneTime(u8 Address, u8 Value);
void DS3231_SetTime(u16 year,u8 month,u8 day,u8 week,u8 hour,u8 minute,u8 second);
u8 DS3231_Read(u8 TtypeAddr);
u8 GetCurrentTime(u8 TtypeAddr);
void DateToStr(SYSTEMTIME *Time);
void TimeToStr(SYSTEMTIME *Time);
void DS3231_Update(SYSTEMTIME *Time);
void DS3231_Config(void);

#endif


/*
1.使用例程：

void Task2(void) //任务2 (用作时间数据刷新)
{
	OSSchedLock();//锁定任务切换
	DS3231_Config();                                       //初使化DS3231（主要用于检测DS3231是否正常）
	DS3231_SetTime(2019,3,18,1,9,32,0);                    //设置初使时间(如IC时间是准的，可不用设置)
	OSSchedUnlock();//解锁任务切换
	while(1) 
	 {		
		  OSSchedLock();//锁定任务切换
		  DS3231_Update(&CurrentTime); //更新时间数据(从DS3231读取数据并保存到结构体CurrentTime中)
		  OSSchedUnlock();//解锁任务切换
		  OS_delayMs(100);//100ms刷新一次数据	
	 }
}
void Task3(void) //任务3 (用作时间显示)
{
	while(1) 
	 {	
     if(CurrentTime.SecCount!=CurrentTime.Second) //检测秒钟变化了没有，如变化了就更新显示内容
			{ 
				CurrentTime.SecCount=CurrentTime.Second;
				OSprintf("%d 年%d 月%d 日\r\n",CurrentTime.Year+2000,CurrentTime.Month,CurrentTime.Day);
				OSprintf("%d 时%d 分%d 秒\r\n",CurrentTime.Hour,CurrentTime.Minute,CurrentTime.Second);
				OSprintf("星期%d\r\n\r\n",CurrentTime.Week);
			}			
		 OS_delayMs(1000); //1秒钟显示一次数据（显示间隔时间要大于数据刷新时间才有意义）	
	 }
}


*/

