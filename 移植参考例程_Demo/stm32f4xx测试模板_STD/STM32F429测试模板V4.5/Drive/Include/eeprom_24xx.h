#ifndef _EEPROM_24XX_H_
#define _EEPROM_24XX_H_

#include "stm32f10x.h"
#include "fsc_stos.h"
#include "i2c_gpio.h"

/*----------------------容量设置-----------------------*/

//大于AT24C16请选择128版的,注意不要超过实际空间即可。
//<=16是单地址，>16是双两字节地址(单地址不够用)，仅此区别。
//大于128请自行修改128对应的参数。

#define AT24C02
//#define AT24C128 
/*----------------------------------------------------*/

#ifdef AT24C02
	#define EE_MODEL_NAME		"AT24C02"
	#define EE_DEV_ADDR			0xA0		/* 设备地址 */
	#define EE_PAGE_SIZE		8			/* 页面大小(字节) */
	#define EE_SIZE				  256		/* 总容量(字节) */
	#define EE_ADDR_BYTES		1			/* 地址字节个数 */
#endif

#ifdef AT24C128
	#define EE_MODEL_NAME		"AT24C128"
	#define EE_DEV_ADDR			0xA0		/* 设备地址 */
	#define EE_PAGE_SIZE		64			/* 页面大小(字节) */
	#define EE_SIZE				(16*1024)	/* 总容量(字节) */
	#define EE_ADDR_BYTES		2			/* 地址字节个数 */
#endif

uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void EEPROM_Config(void);//EEPROM 初使化
#endif


/*
使用例程：

1.单个数据或数组的保存 

uint8_t temp[10];

void Task2(void) //任务2
{
	uint8_t arr[10];
	OSSchedLock();//锁定任务切换
  EEPROM_Config();
	temp[0]=32;
	temp[1]=35;
	temp[2]=39;
	temp[3]=87;
	EEPROM_WriteBytes(temp, 0x0001, 4);//从eeprom的0001地址(也可以为其他地址)开始连续写入4个数据
	OSSchedUnlock();//解锁任务切换
	while(1) 
	 {	 
		  OSSchedLock();//锁定任务切换
		  EEPROM_ReadBytes(arr, 0x0001, 4);//从eeprom的0001地址开始连续读取4个数据保存到arr[]数组
		  OSprintf("arr[0]-arr[3]= %d %d %d %d\r\n",arr[0],arr[1],arr[2],arr[3]);//显示arr数组的前4个成员的值
		  OSSchedUnlock();//解锁任务切换	
  		OS_delayMs(1000);//1秒读取一次	
	 }
}

当写入数据后，把上面的开始写数据注释掉，让eeprom断电一次，然后从eeprom读取数据
void Task2(void) //任务2
{
	uint8_t arr[10];
	OSSchedLock();//锁定任务切换
  EEPROM_Config();
//	temp[0]=32;  //注释
//	temp[1]=35;  //注释
//	temp[2]=39;  //注释
//	temp[3]=87;  //注释
//	EEPROM_WriteBytes(temp, 0x0001, 4);//从eeprom的0001地址(也可以为其他地址)开始连续写入4个数据  //注释
  OSSchedUnlock();//解锁任务切换
	while(1) 
	 {	 
		  OSSchedLock();//锁定任务切换
		  EEPROM_ReadBytes(arr, 0x0001, 4);//从eeprom的0001地址开始连续读取4个数据保存到arr[]数组
		  OSprintf("arr[0]-arr[3]= %d %d %d %d\r\n",arr[0],arr[1],arr[2],arr[3]);//显示arr数组的前4个成员的值
		  OSSchedUnlock();//解锁任务切换	
  		OS_delayMs(1000);//1秒读取一次	
	 }
}

发现读取出的值仍未改变，说明写入成功。


2.结构体数据的保存(推荐使用结构体方式创建需要保存的重要数据)

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //任务2
{
	TypeDefSaveData arr;//缓存读取的值用的临时变量
	OSSchedLock();//锁定任务切换
  EEPROM_Config();
	NeedSaveData.tee=56;
	NeedSaveData.neot=3564;
	NeedSaveData.weight=835447;
	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
	OSSchedUnlock();//解锁任务切换
	while(1) 
	 {	 
		  OSSchedLock();//锁定任务切换
		  EEPROM_ReadBytes((uint8_t*)(&arr), 0x0001, sizeof(NeedSaveData));
		  OSprintf("arr[0]-arr[2]= %d %d %d \r\n",arr.tee,arr.neot,arr.weight);
		  OSSchedUnlock();//解锁任务切换
  		OS_delayMs(1000);//1秒读取一次	
	 }
}


上面代码是测试，如果使用于实际应用中，开机时需要从eeprom中读取出数据并保存到原来的变量中，即等于恢复数据

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //任务2
{
	OSSchedLock();//锁定任务切换
  EEPROM_Config();
	NeedSaveData.tee=56;
	NeedSaveData.neot=3564;
	NeedSaveData.weight=835447;
	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
	OSSchedUnlock();//解锁任务切换
	while(1) 
	 {	 
		  OSSchedLock();//锁定任务切换
		  EEPROM_ReadBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
		  OSprintf("数据 tee  neot  weight = %d %d %d \r\n",NeedSaveData.tee,NeedSaveData.neot,NeedSaveData.weight);
		  OSSchedUnlock();//解锁任务切换
  		OS_delayMs(1000);//1秒读取一次	
	 }
}

读写函数返回值可以表示读写是否成功，可以改为

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //任务2
{
	OSSchedLock();//锁定任务切换
  EEPROM_Config();
//	NeedSaveData.tee=56;
//	NeedSaveData.neot=3564;
//	NeedSaveData.weight=835447;
//	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));

	if(EEPROM_ReadBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData))==1) //只需要在开机时恢复一次
	{
		 OSprintf("数据恢复成功！ \r\n");
	}
	else
	{
		OSprintf("数据恢复失败！ \r\n");
	}
	OSprintf("数据 tee  neot  weight = %d %d %d \r\n",NeedSaveData.tee,NeedSaveData.neot,NeedSaveData.weight);
	OSSchedUnlock();//解锁任务切换
	while(1) 
	 {	 
	    OSSchedLock();//锁定任务切换
	    EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));//定时保存或判断数据变化了再保存
			OSSchedUnlock();//解锁任务切换
	    OS_delayMs(1000);//1秒保存一次	
	 }
}

*/


