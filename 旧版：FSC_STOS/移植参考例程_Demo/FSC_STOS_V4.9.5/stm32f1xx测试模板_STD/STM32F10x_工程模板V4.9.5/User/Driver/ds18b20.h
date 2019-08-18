#ifndef __DS18B20_H_
#define __DS18B20_H _
#include "stm32f10x.h" //基于stm32f1_std库，如为其他请修改为其他头文件并到DS18B20.c中修改3个配置函数
extern void delay_us(unsigned int nus);//需要用户从外部实现一个us级精确延时函数

/*------------------------用户参数--------------------------------*/
#define DS18B20_NUM         1        //单位：个。DS18B20 IO口数量
#define WarnTemperatureMax  30.0     //单位：度。默认最高报警温度值
#define WarnTemperatureMin  0.0      //单位：度。默认最低报警温度值
#define Deviatvalue        -3.7      //单位：度。偏差值(校正值,和标准的温度值之间的差值)

#define C_Time              10       //单位：次。连续读取C_Time次数据之间的差值小于D_Value值则用作第一次读取出的正确值            
#define D_Value             1.0      //单位：度。两次读取值的差值(视测温对象而定，如测环境温度则1度即可，测加热器则设为10度左右，视加热速度而定)
#define TempMax             100.0    //单位：度。测温上限
#define TempMin            -20.0     //单位：度。测温下限 
/*---------------------------------------------------------------*/

typedef struct
{
	float Val[DS18B20_NUM];         //单位：度。当前温度
	float ValAvg[DS18B20_NUM];      //单位：度。平均温度
	float ValMax[DS18B20_NUM];      //单位：度。历史最高温度
	float ValMin[DS18B20_NUM];      //单位：度。历史最低温度
	float ValMaxWarn[DS18B20_NUM];  //单位：度。最高报警温度值
	float ValMinWarn[DS18B20_NUM];  //单位：度。最低报警温度值
	unsigned int Avgcounter;
}DS18B20;
extern DS18B20 Ds18b20;         //数组温度数据对应初使化顺序

uint8_t DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//初始化DS18B20,输入参数：IO口，如(GPIOB,GPIO_Pin_12)
float   DS18B20_Update(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//获取DS18B20温度 或 更新温度(为结构体更新数据)  
#endif

/*
例1：(操作系统环境)

#define DS18B20_NUM     2      //设为2个 
    
void Task1(void)    //任务1用作更新数据
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //第一个初使化，PB12对应结构体数组0的数据
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //第二个初使化，PB6对应结构体数组1的数据，以此类推
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	while(1)
	{
		OSSchedLock();                       //读取数据时，一定要关闭任务切换，防止切换到下个任务
		DS18B20_Update(GPIOB,GPIO_Pin_12); //更新温度数据, 更新数据时IO口无顺序要求，先更新哪个IO的数据都可以
		DS18B20_Update(GPIOB,GPIO_Pin_6); 
    OSSchedUnlock();		                 //读取数据完闭，恢复任务切换
		OS_delayMs(100); //100ms更新一次温度
	}
}
void Task2(void)  //任务2用作处理数据
{
	while(1)
	{
		OSprintf("PB12当前温度：%0.1f℃  \r\n",Ds18b20.Val[0]);//数组0对应PB12的温度的数据
		OSprintf("PB6当前温度： %0.1f℃  \r\n",Ds18b20.Val[1]); //数组1对应PB6的温度的数据
		
		if(Ds18b20.Val[1]>30.0) //判断第二个初使化的DS18B20温度，即PB6线上的DS18B20
		{
		  OSprintf("PB6温度超高！  \r\n");
			//xxxxx();//关闭加热器(xxxx()为关闭加热函数，根据实际加热器自己写)
		}
		OS_delayMs(1000);//1秒查询一次
	}
}



例2：(裸机环境)

//main方式
int main(void)
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //第一个初使化，PB12对应结构体数组0的数据
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //第二个初使化，PB6对应结构体数组1的数据，以此类推
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	while(1)
	{
	  DS18B20_Update(GPIOB,GPIO_Pin_12); //更新温度数据, 更新数据时IO口无顺序要求，先更新哪个IO的数据都可以
		DS18B20_Update(GPIOB,GPIO_Pin_6);  //更新温度数据
    printf("PB12当前温度：%0.1f℃  \r\n",Ds18b20.Val[0]);//数组0对应PB12的温度
		printf("PB6当前温度： %0.1f℃  \r\n",Ds18b20.Val[1]); //数组1对应PB6的温度
		if(Ds18b20.Val[1]>30.0) //判断第二个初使化的DS18B20温度，即PB6线上的DS18B20
		{
		  printf("PB6温度超高！  \r\n");
			//xxxxx();//关闭加热器(xxxx()为关闭加热函数，根据实际加热器自己写)
		}  
		delay_ms(100);//100ms扫描一次(如main中有其他程序，则可以考虑定时器扫描方式)
	}
}

//定时器方式
int main(void)
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //第一个初使化，PB12对应结构体数组0的数据
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //第二个初使化，PB6对应结构体数组1的数据，以此类推
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	TIMER_Config(TIM1,10);//定时器初使化10ms中断一次(此函数不提供，用户自己实现)
	while(1)
	{
	  //其他程序代码
	}
}

int Timer1_10ms_Cnt=0;
void TIM1_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	
	Timer1_10ms_Cnt++;
	if(Timer1_10ms_Cnt>=50)//50*10ms=500ms更新一次温度数据
	{
	  Timer1_10ms_Cnt=0;
		DS18B20_Update(GPIOB,GPIO_Pin_12); //更新温度数据, 更新数据时IO口无顺序要求，先更新哪个IO的数据都可以
		DS18B20_Update(GPIOB,GPIO_Pin_6);  //更新温度数据
		printf("PB12当前温度：%0.1f℃  \r\n",Ds18b20.Val[0]);//数组0对应PB12的温度
		printf("PB6当前温度： %0.1f℃  \r\n",Ds18b20.Val[1]); //数组1对应PB6的温度
		if(Ds18b20.Val[1]>30.0) //判断第二个初使化的DS18B20温度，即PB6线上的DS18B20
		{
			printf("PB6温度超高！  \r\n");
			//xxxxx();//关闭加热器(xxxx()为关闭加热函数，根据实际加热器自己写)
		}  
	}
}

-by Angle145
*/













