#ifndef _EEPROM_24XX_H_
#define _EEPROM_24XX_H_

#include "stm32f10x.h"
#include "fsc_stos.h"
#include "i2c_gpio.h"

/*----------------------��������-----------------------*/

//����AT24C16��ѡ��128���,ע�ⲻҪ����ʵ�ʿռ伴�ɡ�
//<=16�ǵ���ַ��>16��˫���ֽڵ�ַ(����ַ������)����������
//����128�������޸�128��Ӧ�Ĳ�����

#define AT24C02
//#define AT24C128 
/*----------------------------------------------------*/

#ifdef AT24C02
	#define EE_MODEL_NAME		"AT24C02"
	#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
	#define EE_PAGE_SIZE		8			/* ҳ���С(�ֽ�) */
	#define EE_SIZE				  256		/* ������(�ֽ�) */
	#define EE_ADDR_BYTES		1			/* ��ַ�ֽڸ��� */
#endif

#ifdef AT24C128
	#define EE_MODEL_NAME		"AT24C128"
	#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
	#define EE_PAGE_SIZE		64			/* ҳ���С(�ֽ�) */
	#define EE_SIZE				(16*1024)	/* ������(�ֽ�) */
	#define EE_ADDR_BYTES		2			/* ��ַ�ֽڸ��� */
#endif

uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
void EEPROM_Config(void);//EEPROM ��ʹ��
#endif


/*
ʹ�����̣�

1.�������ݻ�����ı��� 

uint8_t temp[10];

void Task2(void) //����2
{
	uint8_t arr[10];
	OSSchedLock();//���������л�
  EEPROM_Config();
	temp[0]=32;
	temp[1]=35;
	temp[2]=39;
	temp[3]=87;
	EEPROM_WriteBytes(temp, 0x0001, 4);//��eeprom��0001��ַ(Ҳ����Ϊ������ַ)��ʼ����д��4������
	OSSchedUnlock();//���������л�
	while(1) 
	 {	 
		  OSSchedLock();//���������л�
		  EEPROM_ReadBytes(arr, 0x0001, 4);//��eeprom��0001��ַ��ʼ������ȡ4�����ݱ��浽arr[]����
		  OSprintf("arr[0]-arr[3]= %d %d %d %d\r\n",arr[0],arr[1],arr[2],arr[3]);//��ʾarr�����ǰ4����Ա��ֵ
		  OSSchedUnlock();//���������л�	
  		OS_delayMs(1000);//1���ȡһ��	
	 }
}

��д�����ݺ󣬰�����Ŀ�ʼд����ע�͵�����eeprom�ϵ�һ�Σ�Ȼ���eeprom��ȡ����
void Task2(void) //����2
{
	uint8_t arr[10];
	OSSchedLock();//���������л�
  EEPROM_Config();
//	temp[0]=32;  //ע��
//	temp[1]=35;  //ע��
//	temp[2]=39;  //ע��
//	temp[3]=87;  //ע��
//	EEPROM_WriteBytes(temp, 0x0001, 4);//��eeprom��0001��ַ(Ҳ����Ϊ������ַ)��ʼ����д��4������  //ע��
  OSSchedUnlock();//���������л�
	while(1) 
	 {	 
		  OSSchedLock();//���������л�
		  EEPROM_ReadBytes(arr, 0x0001, 4);//��eeprom��0001��ַ��ʼ������ȡ4�����ݱ��浽arr[]����
		  OSprintf("arr[0]-arr[3]= %d %d %d %d\r\n",arr[0],arr[1],arr[2],arr[3]);//��ʾarr�����ǰ4����Ա��ֵ
		  OSSchedUnlock();//���������л�	
  		OS_delayMs(1000);//1���ȡһ��	
	 }
}

���ֶ�ȡ����ֵ��δ�ı䣬˵��д��ɹ���


2.�ṹ�����ݵı���(�Ƽ�ʹ�ýṹ�巽ʽ������Ҫ�������Ҫ����)

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //����2
{
	TypeDefSaveData arr;//�����ȡ��ֵ�õ���ʱ����
	OSSchedLock();//���������л�
  EEPROM_Config();
	NeedSaveData.tee=56;
	NeedSaveData.neot=3564;
	NeedSaveData.weight=835447;
	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
	OSSchedUnlock();//���������л�
	while(1) 
	 {	 
		  OSSchedLock();//���������л�
		  EEPROM_ReadBytes((uint8_t*)(&arr), 0x0001, sizeof(NeedSaveData));
		  OSprintf("arr[0]-arr[2]= %d %d %d \r\n",arr.tee,arr.neot,arr.weight);
		  OSSchedUnlock();//���������л�
  		OS_delayMs(1000);//1���ȡһ��	
	 }
}


��������ǲ��ԣ����ʹ����ʵ��Ӧ���У�����ʱ��Ҫ��eeprom�ж�ȡ�����ݲ����浽ԭ���ı����У������ڻָ�����

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //����2
{
	OSSchedLock();//���������л�
  EEPROM_Config();
	NeedSaveData.tee=56;
	NeedSaveData.neot=3564;
	NeedSaveData.weight=835447;
	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
	OSSchedUnlock();//���������л�
	while(1) 
	 {	 
		  OSSchedLock();//���������л�
		  EEPROM_ReadBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));
		  OSprintf("���� tee  neot  weight = %d %d %d \r\n",NeedSaveData.tee,NeedSaveData.neot,NeedSaveData.weight);
		  OSSchedUnlock();//���������л�
  		OS_delayMs(1000);//1���ȡһ��	
	 }
}

��д��������ֵ���Ա�ʾ��д�Ƿ�ɹ������Ը�Ϊ

typedef struct
{
  uint8_t tee;
	uint32_t neot;
	uint32_t weight;
}TypeDefSaveData;
TypeDefSaveData NeedSaveData; 

void Task2(void) //����2
{
	OSSchedLock();//���������л�
  EEPROM_Config();
//	NeedSaveData.tee=56;
//	NeedSaveData.neot=3564;
//	NeedSaveData.weight=835447;
//	EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));

	if(EEPROM_ReadBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData))==1) //ֻ��Ҫ�ڿ���ʱ�ָ�һ��
	{
		 OSprintf("���ݻָ��ɹ��� \r\n");
	}
	else
	{
		OSprintf("���ݻָ�ʧ�ܣ� \r\n");
	}
	OSprintf("���� tee  neot  weight = %d %d %d \r\n",NeedSaveData.tee,NeedSaveData.neot,NeedSaveData.weight);
	OSSchedUnlock();//���������л�
	while(1) 
	 {	 
	    OSSchedLock();//���������л�
	    EEPROM_WriteBytes((uint8_t*)(&NeedSaveData), 0x0001, sizeof(NeedSaveData));//��ʱ������ж����ݱ仯���ٱ���
			OSSchedUnlock();//���������л�
	    OS_delayMs(1000);//1�뱣��һ��	
	 }
}

*/


