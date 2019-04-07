#ifndef _STM32F10xPeripMode_H_
#define _STM32F10xPeripMode_H_
#include "stm32f10x.h"
#include "fsc_stos.h"
#include <stdio.h>
#include "STM32F10x_C51Mode.h" //位带绑定IO操作，用于RS485使能发送接收引脚(如用不到ModBus模块可不必包含此头文件)

/*+++++++++++++++++++++++++++++++++++++++++++++++供用户调用的外设函数+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define MODE_UNUSE 0
#define MODE_USE   1
//重要说明：每个模块第一个宏定义为模块开关 0 - 不使用  1 - 使用

/****************************************************外部中断模块*****************************************************************/
//示例: EXTI_Config(GPIOB,GPIO_Pin_5,0);//PB5用作外部中断引脚,0-下降沿触发中断，1-上升沿触发中断，2-下降沿/上升沿触发中断
#define USE_EXTI  1     //0--不使用外部中断  1--使用
#if (USE_EXTI == 1)
void EXTI_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t EXTI_Trigger_Mode);//外部中断引脚初使化
#endif 

/****************************************************定时器模块*******************************************************************/ 
//示例：TIMER_Config(TIM2,50000);//使用定时器2，定时50ms 定时最大值65535us (TIM1产生PWM，如使用PWM，则TIM1不能用作定时器)
//      TIMER__Config(TIM2,72,1000);//使用定时器2，定时器频率设为72M/72=1M,计时1000次。(72MHz是stm32f1主频，一般为固定值)
//      TIMx  =   TIM1、TIM2、TIM3、TIM4   共4个 (目前重封装库函数仅支持4个定时器)
//      取值范围： Prescaler : 1-65535   ,   Period  :  1-65535
#define USE_TIMER  1   //0--不使用定时器  1--使用
#if (USE_TIMER == 1)
void TIMER_Config(TIM_TypeDef* TIMx,uint16_t TimePeriod);//定时器初使化(TimePeriod=1-65535,单位:us)
void TIMER_Reload(TIM_TypeDef* TIMx,uint16_t Period); //设置自动重装载定时值
void TIMER__Config(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period);//定时器频率(主频)可调的定时器初使化
void TIMER__Reload(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period);//定时器频率(主频)可调的设置自动重装载定时值
void TIMER_ON(TIM_TypeDef* TIMx); //打开定时器   例：TIMER_ON(TIM2);//打开定时器2
void TIMER_OFF(TIM_TypeDef* TIMx);//关闭定时器   例：TIMER_OFF(TIM2);//关闭定时器2
#endif 
//PS:Prescaler参数为stm32主频分频给定时器作为主频的分频系数值。Period为定时器计数次数，计数完成后会产生一次中断表示定时完成。


/******************************************************串口模块*******************************************************************/  
//示例：USART2_Config(9600);//波特率：9600，常用的波特率均有效
#define USE_USART                 1     //0--不使用串口  1--使用
#if (USE_USART == 1)
#define printf_use_usartx         2     //printf  串口选择 
#define USART_RX_BUFF_LEN         256   //接收缓存长度
#define USART_TX_BUFF_LEN         32    //发送缓存长度
typedef struct
{
	uint8_t RX_BUFF[USART_RX_BUFF_LEN];  //接收缓存
	uint8_t TX_BUFF[USART_TX_BUFF_LEN];  //发送缓存
	uint32_t ByteCnt;                 //字节计数(用于接收，当检测它>0时表示有接收到数据,处理完接收数据要把它清0)
}TypeUsart;
TypeUsart Usart1,Usart2,Usart3;  //3个串口数据结构体
void USART1_Config(u32 baudRate);//串口1初使化         //默认  1位停止，数据位8位，无奇偶校验，用到其他可到函数内部修改
void USART2_Config(u32 baudRate);//串口2初使化         //默认  1位停止，数据位8位，无奇偶校验，用到其他可到函数内部修改
void USART3_Config(u32 baudRate);//串口3初使化         //默认  1位停止，数据位8位，无奇偶校验，用到其他可到函数内部修改
//串口1: TX:PA9  RX:PA10
void USART1_SendByte(uint8_t onebyte);//串口1发送一个字节
void USART1_Send(uint8_t *p,uint16_t len);//串口1发送多个字节(适用数组)
void USART1_SendStr(uint8_t *str);    //串口1发送字符串(仅适用字符串)
uint8_t USART1_ReadByte(void);        //串口1读取接收的一个字节
uint8_t* USART1_Read(uint16_t len);//串口1读取接收的多个字节
char* USART1_ReadStr(void);//串口1读取接收的字符串

//串口2: TX:PA2  RX:PA3
void USART2_SendByte(uint8_t onebyte); //串口2发送一个字节
void USART2_Send(uint8_t *p,uint16_t len);//串口2发送多个字节(适用数组)
void USART2_SendStr(uint8_t *str);     //串口2发送字符串(仅适用字符串) 
uint8_t USART2_ReadByte(void);         //串口2读取接收的一个字节
uint8_t* USART2_Read(uint16_t len);//串口1读取接收的多个字节
char* USART2_ReadStr(void);//串口2读取接收的字符串

//串口3: TX:PB10  RX:PB11
void USART3_SendByte(uint8_t onebyte); //串口3发送一个字节
void USART3_Send(uint8_t *p,uint16_t len);//串口3发送多个字节(适用数组)
void USART3_SendStr(uint8_t *str);     //串口3发送字符串(仅适用字符串) 
uint8_t USART3_ReadByte(void);         //串口3读取接收的一个字节
uint8_t* USART3_Read(uint16_t len);//串口1读取接收的多个字节
char* USART3_ReadStr(void);//串口3读取接收的字符串

void STM32F10xPeripMode_Usart1RxCheck(void);//放在串口1中断函数里，串口检测（放在中断函数最前面）
void STM32F10xPeripMode_Usart2RxCheck(void);//放在串口2中断函数里，串口检测（放在中断函数最前面）
void STM32F10xPeripMode_Usart3RxCheck(void);//放在串口3中断函数里，串口检测（放在中断函数最前面）

void STM32F10xPeripMode_Usart1_UsartReceiveByte(uint8_t UsartRxByte); //放在串口1中断函数里，输入形参为接收的1字节数据
void STM32F10xPeripMode_Usart2_UsartReceiveByte(uint8_t UsartRxByte); //放在串口2中断函数里，输入形参为接收的1字节数据
void STM32F10xPeripMode_Usart3_UsartReceiveByte(uint8_t UsartRxByte); //放在串口3中断函数里，输入形参为接收的1字节数据
#endif


/*************************************************ADC模块(12bit ADC)***********************************************************/  
//PA0-PA7,PB0,PB1,PC0-PC5共16个引脚
#define USE_ADC  1     //0--不使用ADC  1--使用
#if (USE_ADC == 1)
void ADC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//ADC初使化
float ReadADC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//读取ADC转换的电压值(0-3.3000)
uint16_t ReadADC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//读取ADC转换的数值(0-4095)
#endif

/*************************************************DAC模块(12bit DAC)***********************************************************/
//PA4、PA5共2个引脚
#define USE_DAC  0     //0--不使用DAC  1--使用
#if (USE_DAC == 1)
void DAC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//DAC初使化
void WriteDAC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,float volt);//设置DAC电压值(0-3.3000)
void WriteDAC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint16_t hex);//设置DAC整型值(0-4095)
#endif

/*************************************************PWM脉宽调制模块***********************************************************/
//输出PWM引脚：单路PA8 ，双路互补：PA8、PB13
#define USE_PWM  0       //0--不使用PWM  1--使用
#if (USE_PWM == 1)
void PWM_TIM1_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse);//单路PWM输出
void PWM_TIM1_OC_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse,uint8_t DTime);//双路互补PWM输出(带死区)
void PWM_TIM1_ON(void); //打开PWM输出
void PWM_TIM1_OFF(void);//关闭PWM输出
void PWM_TIM1_OC_ON(void); //打开双路PWM输出
void PWM_TIM1_OC_OFF(void);//关闭双路PWM输出
#endif

/*****************************************************频率计模块***********************************************************/
#define USE_FREQMETER         0     //1--使用频率计 0--不使用
#if (USE_FREQMETER == 1)
#define USE_FREQMETER_PRINTF  1     //1--使用串口打印输出频率计信息  0--不使用
#define FREQMETER_PRINTF_TIME 5000  //串口发送延时时间
typedef struct 
{
	uint32_t FreqCh[4];//通道频率
  uint8_t DutyCh[4];//通道占空比 (0-100)
}FREQMETER;
FREQMETER FreqMeter;  //频率计输出数据  
//输入参数分别为:分频系数,输入通道数量
void FreqMeter_TIM2(uint16_t Prescaler,uint8_t CHNum);//CH1:PA0  CH2:PA1  CH3:PA2  CH4:PA3 
void FreqMeter_ON(void);  //打开频率计
void FreqMeter_OFF(void); //关闭频率计
#endif

/*********************************************ModBusRTU 主站功能模块*******************************************************/
//读取数据时，接收的数据保存在RS485,RX_BUFF[]缓存数组里，最大接收长度RS485_BUFF_LEN
#define USE_MODEBUS     0      //0--不使用modbus 1--使用
#if (USE_MODEBUS == 1)
#define RS485_USARTTx   2      //=1使用串口1作为RS485发送,=2 USART2  =3 USART3
#define RS485_USARTRx   2      //=1使用串口1作为RS485接收,=2 USART2  =3 USART3
#define RS485_BUFF_LEN  32     //接收和发送缓存长度1-255(最大255)
#define RS485_EN        PB15   //默认PB15为RS485使能引脚（可根据实际需要修改,需要位带绑定IO操作）

TypeUsart RS485;        //串口结构体定义RS485类型发送和接收缓存
typedef struct
{
	/***************用户需要操作的值*******************/
	uint8_t  SlaveID;  //设备ID 8bit         //从机码
	uint8_t  Function;  //功能 8bit          //功能码
	uint16_t Address;   //数据地址 16bit     //数据地址码
	uint16_t Word;      //数据/数量 16bit    //数据码
	uint16_t WordNum;   //读取数据个数 16bit //数据个数码(仅读取数据时使用)
	uint16_t RegNum;    //寄存器数量 16bit   //寄存器数量码(仅对多个寄存器操作时使用)
  uint8_t  ByteNum;   //字节数 8bit        //多寄存器或多对象操作时的字节数码(读或写都使用这个)
	uint8_t  BUFF[RS485_BUFF_LEN];  //多个数据放在此处 8bit ,如果多寄存器读写的数量超过16可修改为更大的值（同时也要修改串口接收缓存的大小，前提是单片机内存足够）
	/**************************************************/
	uint16_t CRC16;   
	uint8_t  EnFlag;  
	uint8_t  FshFlag;
	uint8_t  ErrFlag;
	uint8_t  ByteCnt; 
  uint8_t  WordCnt; 	
}modbusRTU;
modbusRTU ModBusRTU_TX; //发送和接收缓存
void ModBusRTU_Config(void);   //ModBus初使化
void ModBusRTU_Send(modbusRTU modbus);  //发送modbusRTU结构体类型格式的数据(无需要输入CRC数据，函数会自动计算和发送CRC)
void ModBusRTU_SendCmd(uint8_t *p,uint16_t len);//发送自定义数组结构类型格式的数据(无需要输入CRC数据，函数会自动计算和发送CRC)

void STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(uint8_t UsartRxByte);//放在需要的串口中断里，需要哪个串口就放在哪个串口中断
#endif


/****************************************************数据处理模块**********************************************************/
#define USE_DigHand  0      //0--不使用数据处理，1--使用
#if (USE_DigHand == 1)
//阶梯数字滤波算法:                     //用于增量控制
double Ladder(double step,double value);//过滤掉增量小于step的值,输出值是step的倍数值
//带通数字滤波算法：                   //km增长率调节，在1.000附近适当取值。
double BandPass_M(double km,double max,double value); //过滤掉max以上的值
double BandPass_L(double km,double min,double value); //过滤掉min以下的值
double BandPass_ML(double km,double min,double max,double value); //过滤掉min以下的值且过滤掉max以上的值
double BandPass_MLN(double km,double min,double max,double value); //过滤掉min-max范围内的值
//卡尔曼数字滤波算法：
/*      Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏,参考值：0.02
        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好，参考值：7.0000    */ 
double KalmanFilter(double ProcessNiose_Q,double MeasureNoise_R,const double ResrcData);//ResrcData-保存采集信号的变量(静态常量，常驻内存)
#endif


/****************************************************内部Flash模块**********************************************************/
//重要的数据变化之后调用此模块把数据保存到内部flash
#define USE_FLASHWR   0       //0--不使用内部flash读写  1--使用
#if  (USE_FLASHWR == 1)
void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord);//向内部flash写n个字节
void FLASH_ReadNWord(uint16_t *pBuffer, uint32_t ReadAddr, uint16_t nWord);//向内部flash读取n个字节
#endif


/****************************************************独立看门狗模块**********************************************************/
//防止死机
#define USE_IWDG 0           //0--不使用看门狗  1--使用
#if  (USE_IWDG == 1)
void IWDG_Config(uint16_t nms);//独立看门狗初使化，参数：喂狗超时时间(ms)，范围：2ms-2000ms
void IWDG_Feed(void);//喂狗   (在超时前喂狗)
#endif


/****************************************************断电检测模块**********************************************************/
//电压范围：2.2V-2.9V  (检测到芯片电压低于电压阀值时产生掉电中断,在中断里做紧急处理)
#define USE_BKP   0          //0--不使用断电检测  1--使用
#if  (USE_BKP == 1)
void BKP_Config(float voltage); //初使化函数 示例：BKP_Config(2.7); //芯片低于2.7V时产生掉电中断 (掉电中断函数  PVD_IRQHandler() 在本页底部)
#endif
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++具体说明++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
//以下计算是基于系统时钟为72MHz，如为其他频率，请将72000000Hz换成实际频率即可
模块使用示例：
  
1.PWM模块
  示例1：PWM_TIM1_Config(参数1,参数2,参数3);
	                 **参数1：分频系数1
									 **参数2: 分频系数2  
									 **参数3: 占空值     (占空比=参数3/参数2)
									 
				          三个参数的取值范围均为:0-65535
									PWM输出频率=72MHz/分频系数1/分频系数2
									
  示例2：PWM_TIM1_OC_Config(参数1,参数2,参数3,参数4);
		               **参数1：分频系数1
									 **参数2: 分频系数2  
									 **参数3: 占空值             占空比=参数3/参数2
									 **参数4：死区时间**         通过死区计算器算出(在说明-工具文件夹里)


2.内部Flash模块
   提示:flash有擦写次数上限，请慎用此模块(不要频繁使用，警惕无限循环擦写),使用flash作为代码区时，请远离代码范围读写。

   示例：FLASH_WriteNWord((uint16_t*)(&StructMod)，0x00,sizeof(StructMod));//把结构体StructMod数据保存到flash
	       FLASH_ReadNWord((uint16_t*)(&StructMod)，0x00,sizeof(StructMod)); //读取flash中结构体StructMod数据到结构体StructMod

*/



















































/**********************************************外设驱动源码区*************************************************/


/*************************************************外部中断***********************************************************/
/*********************************************************************************
*函数名:EXTI_Config() 外部引脚中断初使化函数
*参数:  GPIOx             -- x=A,B,C,D,E,F      IO口组
        GPIO_Pin_x        -- x=0,1,2,...14,15   IO口引脚号
        EXTI_Trigger_Mode --                    中断触发方式
                              =0 下降沿触发 
                              =1上升沿触发 
                              =2上升沿和下降沿均可触发   
**********************************************************************************/
#if (USE_EXTI == 1)
void EXTI_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t EXTI_Trigger_Mode) //外部中断初使化函数//(用户可调用)
{
	uint32_t RCC_APB2Periph_Source;
	uint8_t  GPIO_PortSource;
	uint8_t  GPIO_PinSource;
	uint32_t EXTI_Line;
	uint8_t  EXTI_IRQn;
	EXTITrigger_TypeDef  EXTI_Trigger;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(GPIOx==GPIOA)      { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOA;GPIO_PortSource=GPIO_PortSourceGPIOA;}
	else if(GPIOx==GPIOB) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOB;GPIO_PortSource=GPIO_PortSourceGPIOB;}
	else if(GPIOx==GPIOC) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOC;GPIO_PortSource=GPIO_PortSourceGPIOC;}
	else if(GPIOx==GPIOD) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOD;GPIO_PortSource=GPIO_PortSourceGPIOD;}
	else if(GPIOx==GPIOE) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOE;GPIO_PortSource=GPIO_PortSourceGPIOE;}
	else if(GPIOx==GPIOF) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOF;GPIO_PortSource=GPIO_PortSourceGPIOF;}
	else if(GPIOx==GPIOG) { RCC_APB2Periph_Source=RCC_APB2Periph_GPIOG;GPIO_PortSource=GPIO_PortSourceGPIOG;}
	
	if(GPIO_Pin_x==GPIO_Pin_0)       { GPIO_PinSource=GPIO_PinSource0; EXTI_Line=EXTI_Line0; EXTI_IRQn=EXTI0_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_1)  { GPIO_PinSource=GPIO_PinSource1; EXTI_Line=EXTI_Line1; EXTI_IRQn=EXTI1_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_2)  { GPIO_PinSource=GPIO_PinSource2; EXTI_Line=EXTI_Line2; EXTI_IRQn=EXTI2_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_3)  { GPIO_PinSource=GPIO_PinSource3; EXTI_Line=EXTI_Line3; EXTI_IRQn=EXTI3_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_4)  { GPIO_PinSource=GPIO_PinSource4; EXTI_Line=EXTI_Line4; EXTI_IRQn=EXTI4_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_5)  { GPIO_PinSource=GPIO_PinSource5; EXTI_Line=EXTI_Line5; EXTI_IRQn=EXTI9_5_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_6)  { GPIO_PinSource=GPIO_PinSource6; EXTI_Line=EXTI_Line6; EXTI_IRQn=EXTI9_5_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_7)  { GPIO_PinSource=GPIO_PinSource7; EXTI_Line=EXTI_Line7; EXTI_IRQn=EXTI9_5_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_8)  { GPIO_PinSource=GPIO_PinSource8; EXTI_Line=EXTI_Line8; EXTI_IRQn=EXTI9_5_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_9)  { GPIO_PinSource=GPIO_PinSource9; EXTI_Line=EXTI_Line9; EXTI_IRQn=EXTI9_5_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_10) { GPIO_PinSource=GPIO_PinSource10;EXTI_Line=EXTI_Line10;EXTI_IRQn=EXTI15_10_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_11) { GPIO_PinSource=GPIO_PinSource11;EXTI_Line=EXTI_Line11;EXTI_IRQn=EXTI15_10_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_12) { GPIO_PinSource=GPIO_PinSource12;EXTI_Line=EXTI_Line12;EXTI_IRQn=EXTI15_10_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_13) { GPIO_PinSource=GPIO_PinSource13;EXTI_Line=EXTI_Line13;EXTI_IRQn=EXTI15_10_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_14) { GPIO_PinSource=GPIO_PinSource14;EXTI_Line=EXTI_Line14;EXTI_IRQn=EXTI15_10_IRQn; }
	else if(GPIO_Pin_x==GPIO_Pin_15) { GPIO_PinSource=GPIO_PinSource15;EXTI_Line=EXTI_Line15;EXTI_IRQn=EXTI15_10_IRQn; }

	/* 开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_Source,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	 
	GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);//选择GPIO管脚用作外部中断线路
	//此处一定要记住给端口管脚加上中断外部线路
	/* 设置外部中断的模式 */ 
	if(EXTI_Trigger_Mode==0)      EXTI_Trigger=EXTI_Trigger_Falling;        //下降沿触发
	else if(EXTI_Trigger_Mode==1) EXTI_Trigger=EXTI_Trigger_Rising;         //上升沿触发
	else if(EXTI_Trigger_Mode==2) EXTI_Trigger=EXTI_Trigger_Rising_Falling; //上升/下降沿触发
	EXTI_InitStructure.EXTI_Line=EXTI_Line;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger; //外部引脚触发方式
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/* 设置NVIC参数 */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQn; 	//打开EXTI2的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //使能
	NVIC_Init(&NVIC_InitStructure); 		
}
#endif



















/*************************************************定时器***********************************************************/
#if (USE_TIMER == 1)
void TIMER_Config(TIM_TypeDef* TIMx,uint16_t TimePeriod) //定时器向上计数，计数1次用时1us，最大655335us
{
	uint32_t RCC_APBPeriph_Source;
	uint8_t TIM_IRQn;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //声明一个结构体变量，用来初始化GPIO
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源
	
	if(TIMx==TIM1)      { RCC_APBPeriph_Source=RCC_APB2Periph_TIM1;TIM_IRQn=TIM1_UP_IRQn; }                      
	else if(TIMx==TIM2) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM2;TIM_IRQn=TIM2_IRQn; }
	else if(TIMx==TIM3) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM3;TIM_IRQn=TIM3_IRQn; }
	else if(TIMx==TIM4) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM4;TIM_IRQn=TIM4_IRQn; }
	
	/* 开启定时器时钟 */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_Source,ENABLE);
  
	TIM_TimeBaseInitStructure.TIM_Period = TimePeriod-1;//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//设置用来作为TIMx时钟频率预分频值，1000khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);	
	TIM_Cmd(TIMx,ENABLE); //使能或者失能TIMx外设
	/* 设置中断参数，并打开中断 */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );	//使能或者失能指定的TIM中断
	
	/* 设置NVIC参数 */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM_IRQn; //打开TIM_IRQn的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_Init(&NVIC_InitStructure);	
}
void TIMER__Config(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period)
{
	uint32_t RCC_APBPeriph_Source;
	uint8_t TIM_IRQn;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //声明一个结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源
	
	if(TIMx==TIM1)      { RCC_APBPeriph_Source=RCC_APB2Periph_TIM1;TIM_IRQn=TIM1_UP_IRQn; }                      
	else if(TIMx==TIM2) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM2;TIM_IRQn=TIM2_IRQn; }
	else if(TIMx==TIM3) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM3;TIM_IRQn=TIM3_IRQn; }
	else if(TIMx==TIM4) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM4;TIM_IRQn=TIM4_IRQn; }
	
	/* 开启定时器时钟 */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_Source,ENABLE);
  
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;//设置用来作为TIMx时钟频率预分频值，1000khz计数频率
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);	
	TIM_Cmd(TIMx,ENABLE); //使能或者失能TIMx外设
	/* 设置中断参数，并打开中断 */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );	//使能或者失能指定的TIM中断
	
	/* 设置NVIC参数 */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM_IRQn; //打开TIM_IRQn的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能
	NVIC_Init(&NVIC_InitStructure);	
}
void TIMER_Reload(TIM_TypeDef* TIMx,uint16_t Period) //设置自动重装载定时值
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
}
void TIMER__Reload(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period) //设置自动重装载定时值
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
}
void TIMER_ON(TIM_TypeDef* TIMx)  //打开定时器
{
  TIM_Cmd(TIMx,ENABLE);	
}
void TIMER_OFF(TIM_TypeDef* TIMx) //关闭定时器
{
  TIM_Cmd(TIMx,DISABLE);	
}
#endif










/*************************************************PWM脉宽调制***********************************************************/
#if (USE_PWM == 1)
void PWM_TIM1_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;// |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period =Period-1 ;//设置自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;//设置用来作为TIMx时钟频率预分频值
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; //模式1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM输出使能
	//TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//PWM输出使能
	//TIM_OCInitStructure.TIM_OutputState=TIM_OutputNState_Disable; //PWM输出失能
	TIM_OCInitStructure.TIM_Pulse=Pulse-1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;  //定时器停止工作时IO口的状态为低(TIM_OCPolarity_High为高)
	//TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	//TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //输出反相(Low同相)
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set; //空闲时输出高
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//空闲时输出低  
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	//TIM_Cmd(TIM1,ENABLE);//

	//下面这句是高级定时器TIM1和TIM8才有，输出PWM必须打开
	//TIM_CtrlPWMOutputs(TIM1, ENABLE); //pwm输出
}

void PWM_TIM1_OC_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse,uint8_t DTime) 
{
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_BDTRInitTypeDef      TIM_BDTRInitStructure;	
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;// |GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = Period-1; //计数周期，向上记到此数，计数值清零
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1;//定时器分频系数，Ftimer = 72M/(TIM_Prescaler+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//定时器时钟再分频，供数字滤波器使用（死区发生器也使用此时钟）
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;//
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;//
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;         //
  TIM_BDTRInitStructure.TIM_DeadTime = DTime;                      //
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;                 //
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;//
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;//
  TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure); 

	 /* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//PWM2模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//比较互补输出使能
	TIM_OCInitStructure.TIM_Pulse = Pulse-1;   //比较值，即占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//互补输出极性
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//指定空闲状态下的TIM输出比较的引脚状态。
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//指定空闲状态下的TIM互补输出比较的引脚状态。
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);   //初始化通道二比较输出
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);      //配置通道一，自动重装载使能  

	TIM_ARRPreloadConfig(TIM1, ENABLE);//重载装载值 ENABLE 立即生效，DISABLE 下一个比较周期生效

	/* TIM1 enable counter */
	//TIM_Cmd(TIM1, ENABLE);//使能定时器1
	
	//TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输出 
}

void PWM_TIM1_ON(void)
{
  TIM_Cmd(TIM1, ENABLE);//使能定时器1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输出
}
void PWM_TIM1_OFF(void)
{
  TIM_Cmd(TIM1, DISABLE);//失能定时器1
	TIM_CtrlPWMOutputs(TIM1, DISABLE);//使能PWM外围输出
}
void PWM_TIM1_OC_ON(void)
{
  TIM_Cmd(TIM1, ENABLE);//使能定时器1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//使能PWM外围输出
}
void PWM_TIM1_OC_OFF(void)
{
  TIM_Cmd(TIM1, DISABLE);//失能定时器1
	TIM_CtrlPWMOutputs(TIM1, DISABLE);//使能PWM外围输出
}
#endif














/****************************************************频率计*************************************************************/
//CAPTURE_STA_TIM2CH[]捕获状态位说明
//[7]:0,没有成功捕获；1,成功捕获到一次完整周期
//[6]:0,还没捕获到低电平;1,已经捕获到低电平.
//[5]:0,还没捕获到低电平;1,已经捕获到高电平.
//[4:0]:捕获低电平后溢出第次数（这里未使用，而是直接采用累加求和）
#if (USE_FREQMETER == 1)
uint16_t CAPTURE_STA_TIM2CH[4] = {0};
uint32_t CAPTURE_DOWNVAL_TIM2CH[4];
uint32_t CAPTURE_UPVAL_TIM2CH[4];
uint16_t CAPTURE_CYCLE_TIM2CH[4];
uint16_t CAPTURE_UP_TIM2CH[4]; 
uint16_t CAPTURE_DOWN_TIM2CH[4];
uint8_t  FreqMeter_CH_NUM;
uint16_t FreqMeter_Prescaler;
uint32_t OSTimerValue; 
uint8_t  FreqMeterTimerSW=0;
void FreqMeter_TIM2(uint16_t Prescaler,uint8_t CHNum)
{	
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
	   
	  FreqMeter_Prescaler=Prescaler;
	  FreqMeter_CH_NUM=CHNum;
	
    //开启TIM2和GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //GPIO初始化
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;            //下拉输入
	  switch(CHNum) 
		{
		  case 1: GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; break;
			case 2: GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; break;
			case 3: GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; break;
			case 4: GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; break;
			default:break;
		}
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //TIM2定时器初始化
    TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF -1 ; //设为最大
    TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler-1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;        //向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
    //TIM2_CH1输入捕获初始化
		if(CHNum>0)
		{		
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC1
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
		if(CHNum>1)
		{
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC2
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
		if(CHNum>2)
		{
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC3
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
   
		if(CHNum>3)
		{		
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //不滤波
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //上升沿捕获
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //输入器不分频
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //映射到IC4
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
    //中断分组初始化
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);    
    
		switch(CHNum) 
		{
		  case 1: TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);  break;
			case 2: TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2, ENABLE);  break;
			case 3: TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3, ENABLE);  break;
			case 4: TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);  break;
			default:break;
		}
    //TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);            //更新中断和CC1IE捕获中断
    
    //TIM_Cmd(TIM2, ENABLE);
		#if (USE_FREQMETER_PRINTF==1 ) 
    printf("Init FreqMeter_TIM2 Success ! \r\n"); 
		#endif
}
void FreqMeter_ON(void)
{
  TIM_Cmd(TIM2, ENABLE);//使能定时器
}
void FreqMeter_OFF(void)
{
  TIM_Cmd(TIM2, DISABLE);//失能定时器
}
void FreqMeterTIM2_IRQHandler(void) 
{     
		if(FreqMeter_CH_NUM>0)
		{	
			if((CAPTURE_STA_TIM2CH[0]&0x80) == 0) {                    //还未捕获成功
					if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {    //捕获1发生捕获事件
							if(CAPTURE_STA_TIM2CH[0]&0x40) {                    //捕获到一个上升沿
									CAPTURE_STA_TIM2CH[0] |= 0x80;                    //标记成功捕获到一次完整周期
									CAPTURE_CYCLE_TIM2CH[0] = TIM_GetCapture1(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[0] >= CAPTURE_DOWN_TIM2CH[0]) 
											CAPTURE_DOWNVAL_TIM2CH[0] = CAPTURE_CYCLE_TIM2CH[0] - CAPTURE_DOWN_TIM2CH[0];
									else 
											CAPTURE_DOWNVAL_TIM2CH[0] = 0xffff + CAPTURE_CYCLE_TIM2CH[0] - CAPTURE_DOWN_TIM2CH[0];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获 //重新开始捕获周期脉冲
								}									
							else if(CAPTURE_STA_TIM2CH[0]&0x20) {                    //捕获到一个下降沿
									CAPTURE_STA_TIM2CH[0] |= 0x40;                    //标记成功捕获到一次高电平脉宽
									CAPTURE_DOWN_TIM2CH[0] = TIM_GetCapture1(TIM2);
									if(CAPTURE_DOWN_TIM2CH[0] >= CAPTURE_UP_TIM2CH[0]) 
											CAPTURE_UPVAL_TIM2CH[0] = CAPTURE_DOWN_TIM2CH[0] - CAPTURE_UP_TIM2CH[0];
									else 
											CAPTURE_UPVAL_TIM2CH[0] = 0xffff + CAPTURE_DOWN_TIM2CH[0] - CAPTURE_UP_TIM2CH[0];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
							} else {                                        //还未开始，第一次捕获上升沿
									CAPTURE_STA_TIM2CH[0] = 0;                    //清空
								  CAPTURE_DOWNVAL_TIM2CH[0]=0;
									CAPTURE_UPVAL_TIM2CH[0] = 0;
									CAPTURE_UP_TIM2CH[0] = TIM_GetCapture1(TIM2);
									CAPTURE_STA_TIM2CH[0] |= 0x20;                //标记捕获到了上升沿
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC1);                                //清除状态标志
					}
			} 
		}

		if(FreqMeter_CH_NUM>1)
		{
      if((CAPTURE_STA_TIM2CH[1]&0x80) == 0) {                    //还未捕获成功
        if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {            //捕获2发生捕获事件
							if(CAPTURE_STA_TIM2CH[1]&0x40) {                    //捕获到一个上升沿
									CAPTURE_STA_TIM2CH[1] |= 0x80;                    //标记成功捕获到一次完整周期
									CAPTURE_CYCLE_TIM2CH[1] = TIM_GetCapture2(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[1] >= CAPTURE_DOWN_TIM2CH[1]) 
											CAPTURE_DOWNVAL_TIM2CH[1] = CAPTURE_CYCLE_TIM2CH[1] - CAPTURE_DOWN_TIM2CH[1];
									else 
											CAPTURE_DOWNVAL_TIM2CH[1] = 0xffff + CAPTURE_CYCLE_TIM2CH[1] - CAPTURE_DOWN_TIM2CH[1];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获 //重新开始捕获周期脉冲
								}
            else if(CAPTURE_STA_TIM2CH[1]&0x20) {                        //捕获到一个下降沿
                CAPTURE_STA_TIM2CH[1] |= 0x40;                        //标记成功捕获到一次高电平脉宽
                CAPTURE_DOWN_TIM2CH[1] = TIM_GetCapture2(TIM2);        //获取捕获2计数
                if(CAPTURE_DOWN_TIM2CH[1] >= CAPTURE_UP_TIM2CH[1])
                    CAPTURE_UPVAL_TIM2CH[1] = CAPTURE_DOWN_TIM2CH[1] - CAPTURE_UP_TIM2CH[1];
                else
                    CAPTURE_UPVAL_TIM2CH[1] = 0xffff + CAPTURE_DOWN_TIM2CH[1] - CAPTURE_UP_TIM2CH[1];
                TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
            } else {                                                //还未开始，第一次捕获上升沿
                CAPTURE_STA_TIM2CH[1] = 0;                            //清空
							  CAPTURE_DOWNVAL_TIM2CH[1]=0;
                CAPTURE_UPVAL_TIM2CH[1] = 0;
                CAPTURE_UP_TIM2CH[1] = TIM_GetCapture2(TIM2);
                CAPTURE_STA_TIM2CH[1] |= 0x20;                //标记捕获到了上升沿
                TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
            }
            TIM_ClearFlag(TIM2, TIM_FLAG_CC2);                                //清除状态标志    
        }
      } 
		}
    
		if(FreqMeter_CH_NUM>2)
		{
			if((CAPTURE_STA_TIM2CH[2]&0x80) == 0) {                    //还未捕获成功
					if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {    //捕获1发生捕获事件
							if(CAPTURE_STA_TIM2CH[2]&0x40) {                    //捕获到一个下降沿
									CAPTURE_STA_TIM2CH[2] |= 0x80;                    //标记成功捕获到一次高电平脉宽
									CAPTURE_CYCLE_TIM2CH[2] = TIM_GetCapture3(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[2] >= CAPTURE_DOWN_TIM2CH[2]) 
											CAPTURE_DOWNVAL_TIM2CH[2] = CAPTURE_CYCLE_TIM2CH[2] - CAPTURE_DOWN_TIM2CH[2];
									else 
											CAPTURE_DOWNVAL_TIM2CH[2] = 0xffff + CAPTURE_CYCLE_TIM2CH[2] - CAPTURE_DOWN_TIM2CH[2];
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获 //重新开始捕获周期脉冲
								}									
							else if(CAPTURE_STA_TIM2CH[2]&0x20) {                    //捕获到一个下降沿
									CAPTURE_STA_TIM2CH[2] |= 0x40;                    //标记成功捕获到一次高电平脉宽
									CAPTURE_DOWN_TIM2CH[2] = TIM_GetCapture3(TIM2);
									if(CAPTURE_DOWN_TIM2CH[2] >= CAPTURE_UP_TIM2CH[2]) 
											CAPTURE_UPVAL_TIM2CH[2] = CAPTURE_DOWN_TIM2CH[2] - CAPTURE_UP_TIM2CH[2];
									else 
											CAPTURE_UPVAL_TIM2CH[2] = 0xffff + CAPTURE_DOWN_TIM2CH[2] - CAPTURE_UP_TIM2CH[2];
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
							} else {                                        //还未开始，第一次捕获上升沿
									CAPTURE_STA_TIM2CH[2] = 0;                    //清空
								  CAPTURE_DOWNVAL_TIM2CH[2]=0;
									CAPTURE_UPVAL_TIM2CH[2] = 0;
									CAPTURE_UP_TIM2CH[2] = TIM_GetCapture3(TIM2);
									CAPTURE_STA_TIM2CH[2] |= 0x20;                //标记捕获到了上升沿
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC3);                                //清除状态标志
					}
			} 
		}
		
		if(FreqMeter_CH_NUM>3)
		{
			if((CAPTURE_STA_TIM2CH[3]&0x80) == 0) {                    //还未捕获成功
					if(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) {    //捕获1发生捕获事件
							if(CAPTURE_STA_TIM2CH[3]&0x40) {                    //捕获到一个下降沿
									CAPTURE_STA_TIM2CH[3] |= 0x80;                    //标记成功捕获到一次高电平脉宽
									CAPTURE_CYCLE_TIM2CH[3] = TIM_GetCapture4(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[3] >= CAPTURE_DOWN_TIM2CH[3]) 
											CAPTURE_DOWNVAL_TIM2CH[3] = CAPTURE_CYCLE_TIM2CH[3] - CAPTURE_DOWN_TIM2CH[3];
									else 
											CAPTURE_DOWNVAL_TIM2CH[3] = 0xffff + CAPTURE_CYCLE_TIM2CH[3] - CAPTURE_DOWN_TIM2CH[3];
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获 //重新开始捕获周期脉冲
								}									
							else if(CAPTURE_STA_TIM2CH[3]&0x20) {                    //捕获到一个下降沿
									CAPTURE_STA_TIM2CH[3] |= 0x40;                    //标记成功捕获到一次高电平脉宽
									CAPTURE_DOWN_TIM2CH[3] = TIM_GetCapture4(TIM2);
									if(CAPTURE_DOWN_TIM2CH[3] >= CAPTURE_UP_TIM2CH[3]) 
											CAPTURE_UPVAL_TIM2CH[3] = CAPTURE_DOWN_TIM2CH[3] - CAPTURE_UP_TIM2CH[3];
									else 
											CAPTURE_UPVAL_TIM2CH[3] = 0xffff + CAPTURE_DOWN_TIM2CH[3] - CAPTURE_UP_TIM2CH[3];
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 设置为上升沿捕获
							} else {                                        //还未开始，第一次捕获上升沿
									CAPTURE_STA_TIM2CH[3] = 0;                    //清空
								  CAPTURE_DOWNVAL_TIM2CH[3]=0;
									CAPTURE_UPVAL_TIM2CH[3] = 0;
									CAPTURE_UP_TIM2CH[3] = TIM_GetCapture4(TIM2);
									CAPTURE_STA_TIM2CH[3] |= 0x20;                //标记捕获到了上升沿
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 设置为下降沿捕获
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC4);                                //清除状态标志
					}
			} 
		}
  
    //处理帧数据
		if((FreqMeterTimerSW&0x80)==0)
		{
			FreqMeterTimerSW|=0x80;
			if(GetOSSliceTime()+ FREQMETER_PRINTF_TIME< 0xFFFFFFFF) OSTimerValue=GetOSSliceTime(); 
			else  OSTimerValue=FREQMETER_PRINTF_TIME+GetOSSliceTime()-0xFFFFFFFF;
		}
		if(FreqMeter_CH_NUM>0)  
		{	
			if(CAPTURE_STA_TIM2CH[0]&0x80) {                                //成功捕获到了一次周期        
				  FreqMeter.FreqCh[0] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[0]+CAPTURE_DOWNVAL_TIM2CH[0]);           //溢出时间总和,得到总的周期时间
					FreqMeter.DutyCh[0] = 100*CAPTURE_UPVAL_TIM2CH[0]/(CAPTURE_UPVAL_TIM2CH[0]+CAPTURE_DOWNVAL_TIM2CH[0]);                  
					#if (USE_FREQMETER_PRINTF==1 )
					if(GetOSSliceTime()>OSTimerValue+FREQMETER_PRINTF_TIME)
					{
						FreqMeterTimerSW&=0x7F;
						FreqMeterTimerSW|=0x01;
						printf("FreqMeter CH1 FREQ:%d\t   \r\n", FreqMeter.FreqCh[0]);	
						printf("FreqMeter CH1 DUTY:%d\t   \r\n", FreqMeter.DutyCh[0]);	
					}						
					#endif
					CAPTURE_STA_TIM2CH[0] = 0;
			} 
		}
		if(FreqMeter_CH_NUM>1)
		{
			if(CAPTURE_STA_TIM2CH[1]&0x80) {                                //成功捕获到了一次周期     
				  FreqMeter.FreqCh[1] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[1]+CAPTURE_DOWNVAL_TIM2CH[1]);           //溢出时间总和,得到总的周期时间
					FreqMeter.DutyCh[1] = 100*CAPTURE_UPVAL_TIM2CH[1]/(CAPTURE_UPVAL_TIM2CH[1]+CAPTURE_DOWNVAL_TIM2CH[1]);                           
					#if (USE_FREQMETER_PRINTF==1 ) 
					if(GetOSSliceTime()>OSTimerValue+FREQMETER_PRINTF_TIME)
					{
						FreqMeterTimerSW&=0x7F;
						FreqMeterTimerSW|=0x02;
						printf("FreqMeter CH2 FREQ:%d\t  \r\n", FreqMeter.FreqCh[1]);	
						printf("FreqMeter CH2 DUTY:%d\t  \r\n", FreqMeter.DutyCh[1]);
					}
					#endif
					CAPTURE_STA_TIM2CH[1] = 0;
			} 
		}
		if(FreqMeter_CH_NUM>2)
		{
			if(CAPTURE_STA_TIM2CH[2]&0x80) {                                //成功捕获到了一次周期       
				  FreqMeter.FreqCh[2] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[2]+CAPTURE_DOWNVAL_TIM2CH[2]);           //溢出时间总和,得到总的周期时间
					FreqMeter.DutyCh[2] = 100*CAPTURE_UPVAL_TIM2CH[2]/(CAPTURE_UPVAL_TIM2CH[2]+CAPTURE_DOWNVAL_TIM2CH[2]);                            
					#if (USE_FREQMETER_PRINTF==1 ) 
					if(GetOSSliceTime()>OSTimerValue+FREQMETER_PRINTF_TIME)	
					{
						FreqMeterTimerSW&=0x7F;
						FreqMeterTimerSW|=0x04;
						printf("FreqMeter CH3 FREQ:%d\t  \r\n", FreqMeter.FreqCh[2]);	
						printf("FreqMeter CH3 DUTY:%d\t  \r\n", FreqMeter.DutyCh[2]);
					}
					#endif
					CAPTURE_STA_TIM2CH[2] = 0;
			} 
		}
		if(FreqMeter_CH_NUM>3)
		{
			if(CAPTURE_STA_TIM2CH[3]&0x80) {                                //成功捕获到了一次周期         
				  FreqMeter.FreqCh[3] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[3]+CAPTURE_DOWNVAL_TIM2CH[3]);           //溢出时间总和,得到总的周期时间
					FreqMeter.DutyCh[3] = 100*CAPTURE_UPVAL_TIM2CH[3]/(CAPTURE_UPVAL_TIM2CH[3]+CAPTURE_DOWNVAL_TIM2CH[3]);                           
					#if (USE_FREQMETER_PRINTF==1 ) 
					if(GetOSSliceTime()>OSTimerValue+FREQMETER_PRINTF_TIME)
					{
						FreqMeterTimerSW&=0x7F;
						FreqMeterTimerSW|=0x08;
						printf("FreqMeter CH4 FREQ:%d\t  \r\n", FreqMeter.FreqCh[3]);	
						printf("FreqMeter CH4 DUTY:%d\t  \r\n", FreqMeter.DutyCh[3]);
					}
					#endif
					CAPTURE_STA_TIM2CH[3] = 0;
			}
		}
		if(FreqMeterTimerSW&0x0F) FreqMeterTimerSW=0;
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
	}
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/**************************************************串口模块******************************************************/	
#if (USE_USART == 1)
#pragma import(__use_no_semihosting)                             
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;    
void _sys_exit(int x) 
{ 
	x = x; 
}
#if (printf_use_usartx == 1)
USART_TypeDef *PRINTF_USARTx = USART1; 
#endif
#if (printf_use_usartx == 2)
USART_TypeDef *PRINTF_USARTx = USART2; 
#endif
#if (printf_use_usartx == 3)
USART_TypeDef *PRINTF_USARTx = USART3; 
#endif
int fputc(int ch, FILE *f)
{	         
	USART_SendData(PRINTF_USARTx, (uint8_t) ch);
	while (USART_GetFlagStatus(PRINTF_USARTx, USART_FLAG_TC) != SET);
	return ch;	
} 

/********************************串口接收检测***************************************/
void STM32F10xPeripMode_Usart1RxCheck(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_PE) != RESET)     
  {        
	  USART_ReceiveData(USART1);    
    USART_ClearFlag(USART1, USART_FLAG_PE);  
  } 
   if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)  
  {
     USART_ReceiveData(USART1);   
     USART_ClearFlag(USART1, USART_FLAG_ORE); 
  }     
   if(USART_GetFlagStatus(USART1, USART_FLAG_FE) != RESET)
  {      
	  USART_ReceiveData(USART1);  
	  USART_ClearFlag(USART1, USART_FLAG_FE);   
	} 
}
void STM32F10xPeripMode_Usart2RxCheck(void)
{
	if(USART_GetFlagStatus(USART2, USART_FLAG_PE) != RESET)     
  {        
	  USART_ReceiveData(USART2);    
    USART_ClearFlag(USART2, USART_FLAG_PE);  
  } 
   if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
  {
     USART_ReceiveData(USART2);   
     USART_ClearFlag(USART2, USART_FLAG_ORE); 
  }     
   if(USART_GetFlagStatus(USART2, USART_FLAG_FE) != RESET)
  {      
	  USART_ReceiveData(USART2);  
	  USART_ClearFlag(USART2, USART_FLAG_FE);   
	}
}
void STM32F10xPeripMode_Usart3RxCheck(void)
{
	if(USART_GetFlagStatus(USART3, USART_FLAG_PE) != RESET)     
  {        
	  USART_ReceiveData(USART3);    
    USART_ClearFlag(USART3, USART_FLAG_PE);  
  } 
   if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
  {
     USART_ReceiveData(USART3);   
     USART_ClearFlag(USART3, USART_FLAG_ORE); 
  }     
   if(USART_GetFlagStatus(USART3, USART_FLAG_FE) != RESET)
  {      
	  USART_ReceiveData(USART3);  
	  USART_ClearFlag(USART3, USART_FLAG_FE);   
	}
}
/**********************************************************************************/

/**********************************串口接收据据************************************/
void STM32F10xPeripMode_Usart1_UsartReceiveByte(uint8_t UsartRxByte)
{
	if(Usart1.ByteCnt<USART_RX_BUFF_LEN-1) Usart1.RX_BUFF[Usart1.ByteCnt++]=UsartRxByte;
	else { Usart1.ByteCnt=0; Usart1.RX_BUFF[Usart1.ByteCnt]=UsartRxByte;}
}
void STM32F10xPeripMode_Usart2_UsartReceiveByte(uint8_t UsartRxByte)
{
	if(Usart2.ByteCnt<USART_RX_BUFF_LEN-1) Usart2.RX_BUFF[Usart2.ByteCnt++]=UsartRxByte;
	else { Usart2.ByteCnt=0; Usart2.RX_BUFF[Usart2.ByteCnt]=UsartRxByte;}
}
void STM32F10xPeripMode_Usart3_UsartReceiveByte(uint8_t UsartRxByte)
{
	if(Usart3.ByteCnt<USART_RX_BUFF_LEN-1) Usart3.RX_BUFF[Usart3.ByteCnt++]=UsartRxByte;
	else { Usart3.ByteCnt=0; Usart3.RX_BUFF[Usart3.ByteCnt]=UsartRxByte;}
}
/**********************************************************************************/
/************************************************************************
*串口1引脚: PA9  --  TXD
            PA10 --  RXD
*停止位：1  
*数据位：8
*奇偶校验：无
************************************************************************/
void USART1_Config(u32 baudRate) //串口1初使化  使用printf函数进行发送数据 （用户可调用）
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                     
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b：8位数据，USART_WordLength_9b：9位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5：0.5个停止位，USART_StopBits_1：1个，USART_StopBits_1_5：1.5个，USART_StopBits_2：2个
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No：无奇偶校验，USART_Parity_Even：偶校验，USART_Parity_Odd：奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式，此处收发均使能
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	//开启接收中断
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//开启发送中断
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void USART2_Config(u32 baudRate)   //(用户可调用)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;
 
	//注：USART2用的是APB1的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                     
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b：8位数据，USART_WordLength_9b：9位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5：0.5个，USART_StopBits_1：1个停止位，USART_StopBits_1_5：1.5个，USART_StopBits_2：2个
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No：无奇偶校验，USART_Parity_Even：偶校验，USART_Parity_Odd：奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式，此处收发均使能
	USART_Init(USART2, &USART_InitStructure); 
 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 	//开启接收中断
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	//开启发送中断
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
}

void USART3_Config(u32 baudRate)   //(用户可调用) //PB10(TX) PB11(RX)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;
 
	//注：USART3用的是APB1的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                     
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;                    
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b：8位数据，USART_WordLength_9b：9位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5：0.5个，USART_StopBits_1：1个停止位，USART_StopBits_1_5：1.5个，USART_StopBits_2：2个
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No：无奇偶校验，USART_Parity_Even：偶校验，USART_Parity_Odd：奇校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式，此处收发均使能
	USART_Init(USART3, &USART_InitStructure); 
 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 	//开启接收中断
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);	//开启发送中断
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
}

void USART1_SendByte(uint8_t onebyte)
{
  USART_SendData(USART1,onebyte);//通过外设USARTx发送单个数据                    
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
}
void USART1_SendStr(uint8_t *str)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(*str=='\0') break; //发送时，遇到'\0'字符则结束发送
		USART_SendData(USART1,*str);//通过外设USARTx发送单个数据                    
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
		str++;
	}
}
void USART1_Send(uint8_t *p,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1,*p++);//通过外设USARTx发送单个数据                    
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t USART1_ReadByte(void)
{
  return (USART_ReceiveData(USART1));
}
uint8_t* USART1_Read(uint16_t len)//串口1读取接收的多个字节
{
  uint16_t i;
	uint8_t xbuff[len+1];
	uint8_t* pf=xbuff;
	for(i=0;i<len;i++)
	{
		xbuff[i]=Usart1.RX_BUFF[i];
	}
	xbuff[len]='\0';
	return pf;
}
char* USART1_ReadStr(void)//串口1读取接收的字符串
{
  uint16_t i,j;
	for(i=0;i<USART_RX_BUFF_LEN;i++)
	{
		if(Usart1.RX_BUFF[i]=='\0') break;
	}
	char xbuff[i+1];
	char* pf=xbuff;
	for(j=0;j<i+1;j++)
	{
		xbuff[j]=Usart1.RX_BUFF[j];
	}		
	return pf;
}
	
void USART2_SendByte(uint8_t onebyte)
{                   
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==Bit_RESET);
}
void USART2_SendStr(uint8_t *str)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(*str!='\0') //发送时，遇到'\0'字符则结束发送
		{
			USART_SendData(USART2,*str);//通过外设USARTx发送单个数据                    
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==Bit_RESET);
			str++;
		}
		else break;
	}
}
uint8_t USART2_ReadByte(void)
{
   return (USART_ReceiveData(USART2));
}
void USART2_Send(uint8_t *p,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART2,*p++);//通过外设USARTx发送单个数据                    
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t* USART2_Read(uint16_t len)//串口2读取接收的多个字节
{
  uint16_t i;
	uint8_t xbuff[len+1];
	uint8_t* pf=xbuff;
	for(i=0;i<len;i++)
	{
		xbuff[i]=Usart2.RX_BUFF[i];
	}
	xbuff[len]='\0';
	return pf;
}
char* USART2_ReadStr(void)//串口2读取接收的字符串
{
  uint16_t i,j;
	for(i=0;i<USART_RX_BUFF_LEN;i++)
	{
		if(Usart2.RX_BUFF[i]=='\0') break;
	}
	char xbuff[i+1];
	char* pf=xbuff;
	for(j=0;j<i+1;j++)
	{
		xbuff[j]=Usart2.RX_BUFF[j];
	}		
	return pf;
}
void USART3_SendByte(uint8_t onebyte)
{
  USART_SendData(USART3,onebyte);//通过外设USARTx发送单个数据                    
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==Bit_RESET);
}
void USART3_SendStr(uint8_t *str)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(*str!='\0') //发送时，遇到'\0'字符则结束发送
		{
			USART_SendData(USART3,*str);//通过外设USARTx发送单个数据                    
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==Bit_RESET);
			str++;
		}
		else break;
	}
}
uint8_t USART3_ReadByte(void)
{
   return (USART_ReceiveData(USART3));
}
void USART3_Send(uint8_t *p,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART3,*p++);//通过外设USARTx发送单个数据                    
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t* USART3_Read(uint16_t len)//串口3读取接收的多个字节
{
  uint16_t i;
	uint8_t xbuff[len+1];
	uint8_t* pf=xbuff;
	for(i=0;i<len;i++)
	{
		xbuff[i]=Usart3.RX_BUFF[i];
	}
	xbuff[len]='\0';
	return pf;
}
char* USART3_ReadStr(void)//串口3读取接收的字符串
{
  uint16_t i,j;
	for(i=0;i<USART_RX_BUFF_LEN;i++)
	{
		if(Usart3.RX_BUFF[i]=='\0') break;
	}
	char xbuff[i+1];
	char* pf=xbuff;
	for(j=0;j<i+1;j++)
	{
		xbuff[j]=Usart3.RX_BUFF[j];
	}		
	return pf;
}

#endif















/*************************************************ModBusRTU主站***********************************************************/
#if (USE_MODEBUS == 1)
#define RS485_RX_EN  RS485_EN=0   //RS485接收使能   
#define RS485_TX_EN  RS485_EN=1   //RS485发送使能 


void STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(uint8_t UsartRxByte)
{
  if(RS485.ByteCnt<RS485_BUFF_LEN-1) RS485.RX_BUFF[RS485.ByteCnt++]=UsartRxByte;
  else { RS485.ByteCnt=0; RS485.RX_BUFF[RS485.ByteCnt]=UsartRxByte; }
}


void ModBusRTU_Config(void)
{
  RS485.ByteCnt=0;  //清空RS485接收缓存
	RS485_TX_EN;      //RS485设置为发送模式
}
void RS485_Send(uint8_t *p,uint16_t num)
{
	RS485_TX_EN;
	if(RS485_USARTTx==1) USART1_Send(p,num); //使用串口1发送 
	else if(RS485_USARTTx==2)USART2_Send(p,num); //使用串口2发送
	else if(RS485_USARTTx==3)USART3_Send(p,num); //使用串口3发送
	RS485_RX_EN;
}
uint16_t CRC16_ByteCheck(uint16_t crc16)
{
	uint8_t i;

	for(i=0;i<8;i++)
	{
		if(crc16&0x0001) { crc16>>=1; crc16^=0xA001; } //先判断再移位
		else crc16>>=1;			
	}
	return crc16;
}
uint16_t CRC16_NBYTE(uint8_t *p,uint8_t num)
{
	uint8_t i;
	uint16_t CRC16=0xFFFF;
	for(i=0;i<num;i++)
	{
		CRC16^=p[i];    
		CRC16=CRC16_ByteCheck(CRC16);
	}
	CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);//CRC高低字节互换
	return CRC16;
}
void ModBusRTU_Send(modbusRTU modbus)
{
	uint16_t i;
	uint16_t CRC16;
	
	ModBusRTU_TX=modbus;
	if((ModBusRTU_TX.Function==15)||(ModBusRTU_TX.Function==16))
	{
	  /*********************CRC计算****************************/	
		CRC16=0xFFFF;   //初使化为0xFFFF(必须是0xFFFF)
		CRC16^=ModBusRTU_TX.SlaveID;    //校验从机ID码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Function;     //校验功能码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address>>8;     //校验高8位数据地址码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address&0x00FF;     //校验低8位数据地址码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.RegNum>>8;     //校验高8位读取寄存器数量
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.RegNum&0x00FF;     //校验低8位读取寄存器数量
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.ByteNum;       //校验8位数据字节数
		CRC16=CRC16_ByteCheck(CRC16);
		for(i=0;i<ModBusRTU_TX.ByteNum;i++)
		{
			CRC16^=ModBusRTU_TX.BUFF[i];       //校验8位 ByteNum个数据
		  CRC16=CRC16_ByteCheck(CRC16);
		}		
		CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);   //CRC高低字节互换
    /******************************************/
		RS485.TX_BUFF[0]=ModBusRTU_TX.SlaveID;    //发送设备ID码
		RS485.TX_BUFF[1]=ModBusRTU_TX.Function;    //发送功能码
		RS485.TX_BUFF[2]=ModBusRTU_TX.Address>>8;   //发送高8位数据地址码
		RS485.TX_BUFF[3]=ModBusRTU_TX.Address;      //发送低8位数据地址码
		RS485.TX_BUFF[4]=ModBusRTU_TX.RegNum>>8;  //发送高8位读取数据个数码
		RS485.TX_BUFF[5]=ModBusRTU_TX.RegNum;      //发送低8位读取数据个数码
		RS485.TX_BUFF[6]=ModBusRTU_TX.ByteNum;      //发送低8位 字节数
		for(i=0;i<ModBusRTU_TX.ByteNum;i++)
		{
		  RS485.TX_BUFF[i+7]=ModBusRTU_TX.BUFF[i];
		}
		RS485.TX_BUFF[ModBusRTU_TX.ByteNum+7]=CRC16>>8;   //发送CRC高字节
		RS485.TX_BUFF[ModBusRTU_TX.ByteNum+8]=CRC16;      //发送CRC低字节
		RS485.ByteCnt=0;
	  RS485_Send(RS485.TX_BUFF,ModBusRTU_TX.ByteNum+9);
	}
	else
	{
	/*********************CRC计算****************************/	
		CRC16=0xFFFF;   //初使化为0xFFFF(必须是0xFFFF)
		CRC16^=ModBusRTU_TX.SlaveID;    //校验从机ID码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Function;     //校验功能码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address>>8;     //校验高8位数据地址码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address&0x00FF;     //校验低8位数据地址码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Word>>8;     //校验高8位数据码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Word&0x00FF;     //校验低8位数据码
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);   //CRC高低字节互换
	/******************************************/
		RS485.TX_BUFF[0]=ModBusRTU_TX.SlaveID;    //发送设备ID码
		RS485.TX_BUFF[1]=ModBusRTU_TX.Function;    //发送功能码
		RS485.TX_BUFF[2]=ModBusRTU_TX.Address>>8;   //发送高8位数据地址码
		RS485.TX_BUFF[3]=ModBusRTU_TX.Address;      //发送低8位数据地址码
		RS485.TX_BUFF[4]=ModBusRTU_TX.Word>>8;      //发送高8位数据码
		RS485.TX_BUFF[5]=ModBusRTU_TX.Word;         //发送低8位数据码
		RS485.TX_BUFF[6]=CRC16>>8;                 //发送CRC高字节
		RS485.TX_BUFF[7]=CRC16;                    //发送CRC低字节
		RS485.ByteCnt=0;
	  RS485_Send(RS485.TX_BUFF,8);
	}
	RS485.ByteCnt=0;  //清0准备接收
	delay_ms(4); //帧间隔时间
}
void ModBusRTU_SendCmd(uint8_t *p,uint16_t len)
{
	uint16_t i;
	uint16_t CRC16;
	CRC16=CRC16_NBYTE(p,len);
	for(i=0;i<len;i++)
	{
	  RS485.TX_BUFF[i]=p[i];
	}
	RS485.TX_BUFF[i]=CRC16>>8;
	RS485.TX_BUFF[i+1]=CRC16;
	RS485_Send(RS485.TX_BUFF,len+2);
	RS485.ByteCnt=0;  //清0准备接收
	delay_ms(4); //帧间隔时间
}
#endif
















/*************************************************数据处理***********************************************************/
#if (USE_DigHand == 1)
/**************PID控制算法**************/
//(暂时未有)


/**************阶梯数字滤波算法**************/
double Ladder_last=0;
double Ladder(double step,double value)
{
  if((value-Ladder_last)>0)
	{
		if((value-Ladder_last)>=step) value=Ladder_last+step;
		else value=Ladder_last;
	}
	else if((value-Ladder_last)<0)
	{
		if((Ladder_last-value)>=step) value=Ladder_last-step;
		else value=Ladder_last;
	}
	Ladder_last=value;
  return value;
}
/***********带通数字滤波算法**************/
typedef struct 
{
  double Ek;
	double Ek_1;
	double Ea;
	double Ea_1;
}BANDPASS;
BANDPASS bandpass=
{
  .Ek=0,
	.Ek_1=0,
	.Ea=0,
	.Ea_1=0,
};

double Growth_k(double km,double value)
{
  value=km*bandpass.Ek/bandpass.Ek_1*bandpass.Ek;
	return value;
}
double Growth_a(double km,double value)
{
  value=km*bandpass.Ea/bandpass.Ea_1*bandpass.Ea;
	return value;
}
double BandPass_M(double km,double max,double value) //过滤掉max以上的值
{
  if(value>max) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_L(double km,double min,double value) //过滤掉min以下的值
{
  if(value<min) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_ML(double km,double min,double max,double value) //过滤掉min以下的值且过滤掉max以上的值
{
  if((value>max)||(value<min)) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_MLN(double km,double min,double max,double value) //过滤掉min-max范围内的值
{
  if((value<max)&&(value>min)) 
	{
		if(value>=(max+min)/2) value=Growth_k(km,value);
		else  value=Growth_a(km,value);
	}
	if(value>=max) bandpass.Ek_1=bandpass.Ek;
	else if(value<=min) bandpass.Ea_1=bandpass.Ea;
	return value;
}

/******卡尔曼数字滤波算法***************************/
/*       
        Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
        R:测量噪声，R增大，动态响应变慢，收敛稳定性变好       
*/
//卡尔曼滤波的两个参数

/* 卡尔曼滤波处理 */
double KalmanFilter(double ProcessNiose_Q,double MeasureNoise_R,const double ResrcData)
{
	  double R = MeasureNoise_R;
    double Q = ProcessNiose_Q;
	
    static double x_last;
    double x_mid = x_last;
    double x_now;

    static double p_last;
    double p_mid ;
    double p_now;

    double kg;

    x_mid=x_last;                       //x_last=x(k-1|k-1),x_mid=x(k|k-1)
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声

    /*
     *  卡尔曼滤波的五个重要公式
     */
    kg=p_mid/(p_mid+R);                 //kg为kalman filter，R 为噪声
    x_now=x_mid+kg*(ResrcData-x_mid);   //估计出的最优值
    p_now=(1-kg)*p_mid;                 //最优值对应的covariance
    p_last = p_now;                     //更新covariance 值
    x_last = x_now;                     //更新系统状态值

    return x_now;

}

#endif

















/*************************************************ADC模数转换***********************************************************/
#if (USE_ADC == 1)
void ADC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	  uint32_t RCC_APB2Periph_GPIOx;
	
		ADC_InitTypeDef ADC_InitStructure; 
		GPIO_InitTypeDef GPIO_InitStructure;

	  if(GPIOx==GPIOA) RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOA;
	  else if(GPIOx==GPIOB) RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOB;
	  else if(GPIOx==GPIOC) RCC_APB2Periph_GPIOx=RCC_APB2Periph_GPIOC;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx  |RCC_APB2Periph_ADC1, ENABLE );
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入引脚
		GPIO_Init(GPIOx, &GPIO_InitStructure); 
	
		//使能ADC1通道时钟
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
		//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
		ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
		ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
			
		ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC
		ADC_ResetCalibration(ADC1);	//使能复位校准  	 
		while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束 
		ADC_StartCalibration(ADC1);	//开启AD校准
		while(ADC_GetCalibrationStatus(ADC1));	//等待校准结束 		 
 }

float ReadADC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)   
{
	uint8_t ch,t,times=10;   //每次读取10个数据，返回平均值
	float adsum=0;
	
	OSSchedLock();             //任务切换上锁
	if(GPIOx==GPIOA)
	{
	  switch(GPIO_Pin_x)
		{
			case GPIO_Pin_0: ch=0; break;
			case GPIO_Pin_1: ch=1; break;
			case GPIO_Pin_2: ch=2; break;
			case GPIO_Pin_3: ch=3; break;
			case GPIO_Pin_4: ch=4; break;
			case GPIO_Pin_5: ch=5; break;
			case GPIO_Pin_6: ch=6; break;
			case GPIO_Pin_7: ch=7; break;
			default: break;
		}
	}
	else if(GPIOx==GPIOB)
	{
		  switch(GPIO_Pin_x)
		{
			case GPIO_Pin_0: ch=8; break;
			case GPIO_Pin_1: ch=9; break;
			default: break;
		}
	}
	else if(GPIOx==GPIOC)
	{
	  switch(GPIO_Pin_x)
		{
			case GPIO_Pin_0: ch=10; break;
			case GPIO_Pin_1: ch=11; break;
			case GPIO_Pin_2: ch=12; break;
			case GPIO_Pin_3: ch=13; break;
			case GPIO_Pin_4: ch=14; break;
			case GPIO_Pin_5: ch=15; break;
			default: break;
		}
	}
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	 	
  for(t=0;t<times;t++)	
	{	
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//使能指定的ADC1的软件转换启动功能
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
		adsum=adsum+ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	}
	adsum=(float)(adsum/times); //取平均值
	adsum=(float)(adsum*3.3/4095);
	OSSchedUnlock();      //任务切换解锁
	return adsum;
} 

uint16_t ReadADC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)   
{
   return (uint16_t)((ReadADC_Volt(GPIOx,GPIO_Pin_x)/3.3)*4095);
}

#endif



















/*******************************************DAC模块*******************************************************/
#if (USE_DAC == 1)
void DAC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)	//DAC初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_x;//DAC_1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟量输入
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	GPIO_SetBits(GPIOx,GPIO_Pin_x);//输出高
	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;//不使用触发功能
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用三角波
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0; //屏蔽 幅值设置
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable; //关闭缓存
	if(GPIO_Pin_x==GPIO_Pin_4)
	{
		DAC_Init(DAC_Channel_1,&DAC_InitStructure);//初始化DAC通道1
		DAC_Cmd(DAC_Channel_1,ENABLE);//使能DAC1
		DAC_SetChannel1Data(DAC_Align_12b_R,0);//12位 右对齐 写0数据
	}
	else 
	if(GPIO_Pin_x==GPIO_Pin_5)
	{
		DAC_Init(DAC_Channel_2,&DAC_InitStructure);//初始化DAC通道1
		DAC_Cmd(DAC_Channel_2,ENABLE);//使能DAC1
		DAC_SetChannel2Data(DAC_Align_12b_R,0);//12位 右对齐 写0数据
	}
}
void WriteDAC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,float volt)
{
	uint16_t temp;
	temp=(uint16_t)(volt*4095/3.3);
	if(GPIO_Pin_x==GPIO_Pin_4) DAC_SetChannel1Data(DAC_Align_12b_R,temp);
  else if(GPIO_Pin_x==GPIO_Pin_5) DAC_SetChannel2Data(DAC_Align_12b_R,temp);
}

void WriteDAC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint16_t hex)
{
  if(GPIO_Pin_x==GPIO_Pin_4) DAC_SetChannel1Data(DAC_Align_12b_R,hex);
  else if(GPIO_Pin_x==GPIO_Pin_5) DAC_SetChannel2Data(DAC_Align_12b_R,hex);
}
#endif















/***************************************内部Flash读写**************************************************/
#if (USE_FLASHWR == 1)
/*--------------------------------Flash-------------------------------------*/
#define FLASH_ADDR_FLAG           0x08010000     //FLASH地址 - 标志位
#define FLASH_ADDR_DATA           0x08010002     //FLASH地址 - 数据
#define LENGTH_DATA               10             //数据长度
#define FLASH_PAGE_SIZE           ((uint16_t)0x800)        //页大小   - 2K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)        //类型大小 - 2字节
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01

//uint8_t WriteBuf[LENGTH_DATA];                   //写入的数据
//uint8_t ReadBuf[LENGTH_DATA];                    //读出的数据
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
/*------------------------------类型定义-------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/*---------------------------------------------------------------------------*/
/************************************************
函数名称 ： FLASH_PageWrite
功    能 ： 写一页FLASH
参    数 ： pBuffer ----- 数据缓冲区
            WriteAddr --- 写地址
返 回 值 ： 无
*************************************************/
void FLASH_PageWrite(uint16_t *pBuffer, uint32_t WriteAddr) 
{
  uint16_t cnt_tmp;
	OSSchedLock();             //任务切换上锁 
  for(cnt_tmp=0; (cnt_tmp<FLASH_PAGE_LENGTH) && (FLASHStatus == FLASH_COMPLETE); cnt_tmp++)
  {
    FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
    WriteAddr += 2;
    pBuffer++;
  }
	OSSchedUnlock();      //任务切换解锁
}

/************************************************
函数名称 ： FLASH_WriteNWord
功    能 ： 写N字节
参    数 ： pBuffer ----- 数据缓冲区
            WriteAddr --- 写地址
            nWord ------- 长度
返 回 值 ： 无
*************************************************/
void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord) //(用户可调用)
{
  static uint16_t buf_tmp[FLASH_PAGE_LENGTH];
  uint32_t NbrOfPage = 0x00;                     //页数（读写）

  uint32_t length_beyond_start;                  //开始页超出的长度(半字)
  uint32_t length_remain_start;                  //开始页剩余的长度(半字)

  uint32_t addr_first_page;                      //第一页(起始)地址
  uint32_t addr_last_page;                       //最后页(起始)地址
  uint16_t *pbuf_tmp;                            //buf指针
  uint16_t cnt_length;                           //计数 - 长度
  uint16_t cnt_page;                             //计数 - 页数
  uint32_t prog_addr_start;                      //编程地址
  uint32_t length_beyond_last;                   //最后页超出的长度(半字)
  uint8_t  flag_last_page_fill;                  //最后一页装满标志

  OSSchedLock();             //任务切换上锁 
  length_beyond_start = ((WriteAddr % FLASH_PAGE_SIZE) / FLASH_TYPE_LENGTH);
  length_remain_start = FLASH_PAGE_LENGTH - length_beyond_start;
  addr_first_page     = WriteAddr - (WriteAddr % FLASH_PAGE_SIZE);

  /* 擦除(写操作)的页数 */
  if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH)
  {
    flag_last_page_fill = FLAG_OK;               //最后一页刚好
    NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
  }
  else
  {
    flag_last_page_fill = FLAG_NOOK;             //···跨页
    NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
  }

  /* 解锁 */
  FLASH_UnlockBank1();

  /* 清除标志位 */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

  /* 操作第一页 */
  FLASH_ReadNWord(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
  FLASHStatus = FLASH_ErasePage(addr_first_page);
  /* 只有1页 */
  if(1 == NbrOfPage)
  {
    pbuf_tmp = pBuffer;                          //复制地址(指针)
    for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++)
    {
      buf_tmp[cnt_length] = *pbuf_tmp;
      pbuf_tmp++;
    }
    FLASH_PageWrite(buf_tmp, addr_first_page);
  }
  /* 大于1页 */
  else
  {
    /* 第一页 */
    pbuf_tmp = pBuffer;
    for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++)
    {
      buf_tmp[cnt_length] = *pbuf_tmp;
      pbuf_tmp++;
    }
    FLASH_PageWrite(buf_tmp, addr_first_page);

    /* 最后一页刚好装满，不用读取最后一页数据 */
    if(FLAG_OK == flag_last_page_fill)
    {
      for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
      {                                          //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
        prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
        FLASHStatus = FLASH_ErasePage(prog_addr_start);
                                                 //(cnt_page - 1):为下一页地址
        FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
      }
    }
    else
    {
      /* 中间页 */
      for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
      {                                          //这里编程地址为字节地址(故FLASH_PAGE_SIZE)
        prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
        FLASHStatus = FLASH_ErasePage(prog_addr_start);
                                                 //(cnt_page - 1):为下一页地址
        FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
      }

      /* 最后一页 */
      addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;

      FLASH_ReadNWord(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
      FLASHStatus = FLASH_ErasePage(addr_last_page);
                                                 //NbrOfPage - 2: 首页 + 最后一页 共两页(-2)
      pbuf_tmp = pBuffer + length_remain_start + (NbrOfPage - 2)*(FLASH_PAGE_SIZE/2);
      length_beyond_last   = (nWord - length_remain_start) % FLASH_PAGE_LENGTH;
      for(cnt_length=0; cnt_length<length_beyond_last; cnt_length++)
      {
        buf_tmp[cnt_length] = *pbuf_tmp;
        pbuf_tmp++;
      }
      FLASH_PageWrite(buf_tmp, addr_last_page);
    }
  }
	OSSchedUnlock();      //任务切换解锁
}

/************************************************
函数名称 ： FLASH_ReadNWord
功    能 ： 读N字节
参    数 ： pBuffer ----- 数据缓冲区
            ReadAddr ---- 读地址
            nWord ------- 长度
返 回 值 ： 无
*************************************************/
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord) //(用户可调用)
{
	OSSchedLock();             //任务切换上锁 
  while(nWord--)
  {
    *pBuffer = (*(__IO uint16_t*)ReadAddr);
    ReadAddr += 2;
    pBuffer++;
  }
	OSSchedUnlock();      //任务切换解锁
}
#endif












/*************************************************程序看门狗***********************************************************/
/**
 * 初始化独立看门狗（时钟：大概40KHZ）
 * prer:分频数:0~7(只有低 3 位有效!)
 * 分频因子=4*2^prer.但最大值只能是 256!
 * rlr:重装载寄存器值:低 11 位有效.
 * 时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).
 */
#if (USE_IWDG == 1)
void IWDG_Config(uint16_t nms) //单位：ms
{
	uint8_t i=0,prersum=1;
  uint8_t prer=4;       //默认使用时钟: 40KHz/(4*2^4) = 625Hz =1.6ms
	uint16_t rlr;     
	while(i<2+prer) { prersum*=2; i++;}
	rlr=nms*40/prersum;
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
	IWDG_SetPrescaler(prer);    /*设置IWDG预分频值:设置IWDG预分频值*/
	IWDG_SetReload(rlr);     /*设置IWDG重装载值*/
	IWDG_ReloadCounter();    /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
	IWDG_Enable();        /*使能IWDG*/
}
void IWDG_Feed(void)  //独立看门狗 - 喂狗
{
  IWDG_ReloadCounter(); 
}
#endif












/*************************************************BKP掉电保存***********************************************************/
/*****************************************************************
*断电检测
*EXTI_InitStructure.EXTI_Trigger的赋值可选项： 
*       EXTI_Trigger_Rising---表示电压从高下降到低于设定阀值时产生中断； 
*       EXTI_Trigger_Falling---表示电压从低上升到高于设定阀值时产生中断； 
*       EXTI_Trigger_Rising_Falling---表示电压上升或下降越过设定阀值时都产生中断。 
*
*当工作电压低于设定阀值时，将产生PVD中断，在中断程序中进行相应的处理： 
*电压阀值： (2.2V-2.9V)

            2.2V   PWR_PVDLevel_2V2                
            2.3V   PWR_PVDLevel_2V3                  
            ...    ...       
            2.9V   PWR_PVDLevel_2V9       
                   IS_PWR_PVD_LEVEL(LEVEL)
******************************************************************/
#if (USE_BKP == 1)
void BKP_Config(float voltage)  //(用户可调用)
{
	uint32_t VolLimit;
  EXTI_InitTypeDef EXTI_InitStructure;
	voltage=voltage*10;
	switch((uint8_t)voltage)
	{
	  case 22:  VolLimit=PWR_PVDLevel_2V2; break;
		case 23:  VolLimit=PWR_PVDLevel_2V3; break;
		case 24:  VolLimit=PWR_PVDLevel_2V4; break;
		case 25:  VolLimit=PWR_PVDLevel_2V5; break;
		case 26:  VolLimit=PWR_PVDLevel_2V6; break;
		case 27:  VolLimit=PWR_PVDLevel_2V7; break;
		case 28:  VolLimit=PWR_PVDLevel_2V8; break;
		case 29:  VolLimit=PWR_PVDLevel_2V9; break;
		default:  break;
	}	
  PWR_PVDLevelConfig(VolLimit); // 设定监控阀值 
  PWR_PVDCmd(ENABLE); // 使能PVD  
  EXTI_StructInit(&EXTI_InitStructure);  
  EXTI_InitStructure.EXTI_Line = EXTI_Line16; // PVD连接到中断线16上  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //使用中断模式  
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//电压低于阀值时产生中断  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能中断线  
  EXTI_Init(&EXTI_InitStructure); // 初始化  
}

#endif






















/*****************************************STM32中断函数********************************************

*提示：中断触发后会打断所有任务运行，然后转移到此处对应的中断函数运行，运行完此处的函数会返回
       被打断的任务处继续运行，所以为了任务不被打断太久，建议中断函数处理的内容尽量不要使用延
       时(代码可以多，但尽量不要使用延时)
*说明：

*********************************************************************************************/
void TIM1_IRQHandler(void)	  //定时器1中断函数 (高级定时器)  //定时时间到产生中断
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	#if (USE_PWM == 0)          //使用PWM时，用作普通定时无效
	// 用户添加定时器1的中断代码 
	/*************************/



	/**************************/	
	#endif
}
void TIM2_IRQHandler(void)	  //定时器2中断函数 (标准(通用)定时器)  //定时时间到产生中断
{
#if (USE_FREQMETER==1)        
	FreqMeterTIM2_IRQHandler();
#else                         // 使用频率计时, 用作普通定时无效
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	// 用户添加定时器2的中断代码 
	/*************************/
   


	/**************************/	
#endif
}
void TIM3_IRQHandler(void)	  //定时器3中断函数 (标准(通用)定时器)  //定时时间到产生中断
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	// 用户添加定时器3的中断代码 
	/*************************/

  

	/**************************/	
}
void TIM4_IRQHandler(void)	  //定时器4中断函数 (标准(通用)定时器)  //定时时间到产生中断
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	// 用户添加定时器4的中断代码 
	/*************************/

  

	/**************************/	
}

void EXTI0_IRQHandler(void)	   //外部中断函数                    中断引脚0   //外部引脚0检测到触发电平时产生中断，其他引脚同理
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET) //检查中断标志位是否置1
	{
   	EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI线路挂起位
		// 用户添加引脚0的中断代码 
		/*************************/



		/**************************/	
	}		
}
void EXTI1_IRQHandler(void)	   //外部中断函数                    中断引脚1  
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET) //检查中断标志位是否置1
	{
   	EXTI_ClearITPendingBit(EXTI_Line1);//清除EXTI线路挂起位
		// 用户添加引脚1的中断代码 
		/*************************/



		/**************************/	
	}		
}
void EXTI2_IRQHandler(void)	   //外部中断函数                     中断引脚2   
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET) //检查中断标志位是否置1
	{
   	EXTI_ClearITPendingBit(EXTI_Line2);//清除EXTI线路挂起位
		// 用户添加引脚2的中断代码 
		/*************************/



		/**************************/	
	}		
}
void EXTI3_IRQHandler(void)	   //外部中断函数                     中断引脚3   
{
	if(EXTI_GetITStatus(EXTI_Line3)==SET) //检查中断标志位是否置1
	{
   	EXTI_ClearITPendingBit(EXTI_Line3);//清除EXTI线路挂起位
		// 用户添加引脚3的中断代码 
		/*************************/



		/**************************/		
	}		
}
//void EXTI4_IRQHandler(void)	   //外部中断函数                     中断引脚4   
//{
//	if(EXTI_GetITStatus(EXTI_Line4)==SET) //检查中断标志位是否置1
//	{
//   	EXTI_ClearITPendingBit(EXTI_Line4);//清除EXTI线路挂起位
//		// 用户添加引脚4的中断代码 
//		/*************************/



//		/**************************/		
//	}		
//}
void EXTI9_5_IRQHandler(void)	   //外部中断函数   引脚5~9   
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET) //检查中断标志位是否置1   中断引脚5   
	{
   	EXTI_ClearITPendingBit(EXTI_Line5);//清除EXTI线路挂起位
		// 用户添加引脚5的中断代码 
		/*************************/

		

		/**************************/	
	}	

	if(EXTI_GetITStatus(EXTI_Line6)==SET) //检查中断标志位是否置1    中断引脚6   
	{
   	EXTI_ClearITPendingBit(EXTI_Line6);//清除EXTI线路挂起位
		// 用户添加引脚6的中断代码 
		/*************************/



		/**************************/		
	}	
 
	if(EXTI_GetITStatus(EXTI_Line7)==SET) //检查中断标志位是否置1    中断引脚7   
	{
   	EXTI_ClearITPendingBit(EXTI_Line7);//清除EXTI线路挂起位
		// 用户添加引脚7的中断代码 
		/*************************/



		/**************************/		
	}	

	if(EXTI_GetITStatus(EXTI_Line8)==SET) //检查中断标志位是否置1    中断引脚8   
	{
   	EXTI_ClearITPendingBit(EXTI_Line8);//清除EXTI线路挂起位
		// 用户添加引脚8的中断代码 
		/*************************/



		/**************************/			
	}	

	if(EXTI_GetITStatus(EXTI_Line9)==SET) //检查中断标志位是否置1    中断引脚9   
	{
   	EXTI_ClearITPendingBit(EXTI_Line9);//清除EXTI线路挂起位
		// 用户添加引脚9的中断代码 
		/*************************/



		/**************************/				
	}		
}
void EXTI15_10_IRQHandler(void)	   //外部中断函数   引脚10~15 
{ 
	if(EXTI_GetITStatus(EXTI_Line10)==SET) //检查中断标志位是否置1   中断引脚10   
	{
   	EXTI_ClearITPendingBit(EXTI_Line10);//清除EXTI线路挂起位
		// 用户添加引脚10的中断代码 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line11)==SET) //检查中断标志位是否置1   中断引脚11   
	{
   	EXTI_ClearITPendingBit(EXTI_Line11);//清除EXTI线路挂起位
		// 用户添加引脚11的中断代码 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line12)==SET) //检查中断标志位是否置1   中断引脚12   
	{
   	EXTI_ClearITPendingBit(EXTI_Line12);//清除EXTI线路挂起位
		// 用户添加引脚12的中断代码 
		/*************************/



		/**************************/			
	}		
	
	if(EXTI_GetITStatus(EXTI_Line13)==SET) //检查中断标志位是否置1   中断引脚13   
	{
   	EXTI_ClearITPendingBit(EXTI_Line13);//清除EXTI线路挂起位
		// 用户添加引脚13的中断代码 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line14)==SET) //检查中断标志位是否置1   中断引脚14   
	{
   	EXTI_ClearITPendingBit(EXTI_Line14);//清除EXTI线路挂起位
		// 用户添加引脚14的中断代码 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line15)==SET) //检查中断标志位是否置1   中断引脚15  
	{
   	EXTI_ClearITPendingBit(EXTI_Line15);//清除EXTI线路挂起位
		// 用户添加引脚15的中断代码 
		/*************************/



		/**************************/		
	}		
}

void USART1_IRQHandler(void)  //串口1中断  PA9(TX) PA10(RX) //接收到新字节时产生中断
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //STM32F10x重封装库USART1模块接收检测
  STM32F10xPeripMode_Usart1RxCheck();    
	#endif
	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //检查指定的USART中断发生与否
	{
    UsartRxdata= USART1_ReadByte();	//串口接收1字节数据
		USART_ClearFlag(USART1, USART_FLAG_RXNE);    //清除接收中断标志
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); //以下为自带的接收缓存,在示例说明TXT里有使用说明	
		
		#ifdef USE_USART  //STM32F10x重封装库USART1模块
		#if (USE_USART == MODE_USE)	
		STM32F10xPeripMode_Usart1_UsartReceiveByte(UsartRxdata);
		#endif
		#endif
		
#if (printf_use_usartx == 1)		
		//系统指令函数//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //使用串口1作为系统指令(放在串口1中断里就使用串口1，放在其他的就使用其他的串口)
    #endif
#endif		
		// 用户添加串口1接收中断代码 
		/*************************/	


		/**************************/	    	
	}
}
void USART2_IRQHandler(void)  //串口2中断  PA2(TX) PA3(RX) //接收到新字节时产生中断
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //重封装库USART2模块接收检测
  STM32F10xPeripMode_Usart2RxCheck();   
	#endif
	
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //检查指定的USART中断发生与否
	{       
		UsartRxdata= USART2_ReadByte();	//串口接收1字节数据
		USART_ClearFlag(USART2, USART_FLAG_RXNE);    //清除接收中断标志
    USART_ClearITPendingBit(USART2, USART_IT_RXNE); //以下为自带的接收缓存,在示例说明TXT里有使用说明
		
		#ifdef USE_USART //STM32F10x重封装库USART2模块
		#if (USE_USART == MODE_USE)	
		STM32F10xPeripMode_Usart2_UsartReceiveByte(UsartRxdata);
		#endif
		#endif	
		
		#ifdef USE_MODEBUS //STM32F10x重封装库ModeBus模块
		#if (USE_MODEBUS == MODE_USE)
		STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(UsartRxdata);	
		#endif	
		#endif
		
#if (printf_use_usartx == 2)		
		//系统指令函数//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //使用串口1作为系统指令(放在串口1中断里就使用串口1，放在其他的就使用其他的串口)
    #endif
#endif
		// 用户添加串口2接收中断代码 
		/*************************/


		/**************************/	
	}
}
void USART3_IRQHandler(void)  // 串口3中断  PB10(TX) PB11(RX) //接收到新字节时产生中断
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //STM32F10x重封装库USART3模块接收检测
  STM32F10xPeripMode_Usart3RxCheck(); 
  #endif  
	
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{    
    UsartRxdata= USART3_ReadByte();	//串口接收1字节数据		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);   
    USART_ClearITPendingBit(USART3, USART_IT_RXNE); //以下为自带的接收缓存,在示例说明TXT里有使用说明
		
		#ifdef USE_USART
		#if (USE_USART == MODE_USE)	 //STM32F10x重封装库USART3模块
		STM32F10xPeripMode_Usart3_UsartReceiveByte(UsartRxdata);
		#endif
		#endif
		
#if (printf_use_usartx == 3)		
		//系统指令函数//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //使用串口1作为系统指令(放在串口1中断里就使用串口1，放在其他的就使用其他的串口)
    #endif
#endif		
		// 用户添加串口3接收中断代码 
		/*************************/



		/**************************/	
	}
}


/*****************************************************************
*PVD中断,掉电中断函数
*如需要使用请在任务初使化里调用BKP_Config();后此中断即可生效
*****************************************************************/
void PVD_IRQHandler(void)  
{ 
  EXTI_ClearITPendingBit(EXTI_Line16); 
   // 用户添加掉电紧急保存数据代码 
	/*************************/
	
	
	
	/**************************/
}
/************************************************************************************/


#endif


