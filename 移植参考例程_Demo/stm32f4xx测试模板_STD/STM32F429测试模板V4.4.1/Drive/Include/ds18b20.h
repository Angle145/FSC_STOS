#ifndef __DS18B20_H_
#define __DS18B20_H _
#include "stm32f10x.h"

#define DS18B20_NUM        1        //DS18B20 IO口数量
#define WarnTemperatureMax 30.0     //默认最高报警温度值
#define WarnTemperatureMin 0.0      //默认最低报警温度值
typedef struct
{
	float Val[DS18B20_NUM];        //当前温度
	float ValMax[DS18B20_NUM];     //历史最高温度
	float ValMin[DS18B20_NUM];     //历史最低温度
	float ValMaxWarn[DS18B20_NUM]; //最高报警温度值
	float ValMinWarn[DS18B20_NUM]; //最高报警温度值
}DS18B20;
extern DS18B20 Ds18b20;         //数组温度数据对应初使化顺序

uint8_t DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);    //初始化DS18B20,输入参数：IO口，如(GPIOB,GPIO_Pin_12)
float   DS18B20_Update(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);  //获取DS18B20温度 或 更新温度(为结构体更新数据)  
#endif

/*
例：

#define DS18B20_NUM     2      //设为2个 
    
void Task1(void)    //任务1用作更新数据
{
	DS18B20_Config(GPIOB,GPIO_Pin_12); //第一个初使化，PB12对应结构体数组0
	DS18B20_Config(GPIOB,GPIO_Pin_6);  //第二个初使化，PB6对应结构体数组1
	while(1)
	{
		OSSchedLock();                       //读取数据时，一定要关闭任务切换，防止切换到下个任务
		DS18B20_Update(GPIOB,GPIO_Pin_12); //更新温度数据, 更新数据时IO口无顺序要求
		DS18B20_Update(GPIOB,GPIO_Pin_6); 
    OSSchedUnlock();		                 //读取数据完闭，恢复任务切换
		OS_delayMs(100); //100ms更新一次温度
	}
}
void Task2(void)  //任务2用作处理数据
{
	while(1)
	{
		OSprintf("PB12当前温度：%0.1f℃  \r\n",Ds18b20.Val[0]);//数组0对应PB12的温度
		OSprintf("PB6当前温度： %0.1f℃  \r\n",Ds18b20.Val[1]); //数组1对应PB6的温度
		
		if(Ds18b20.Val[1]>30.0) //判断第二个初使化的DS118B20温度，即PB6线上的DS18B20
		{
		  OSprintf("PB6温度超高！  \r\n");
			//xxxxx();//关闭加热器(xxxx()为关闭加热函数，根据实际加热器自己写)
		}
		OS_delayMs(1000);//1秒查询一次
	}
}


*/













