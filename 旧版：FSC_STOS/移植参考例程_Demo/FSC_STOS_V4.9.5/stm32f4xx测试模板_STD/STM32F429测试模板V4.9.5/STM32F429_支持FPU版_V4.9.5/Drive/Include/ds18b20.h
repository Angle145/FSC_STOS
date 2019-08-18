#ifndef __DS18B20_H_
#define __DS18B20_H _
#include "stm32f10x.h"

#define DS18B20_NUM        1        //DS18B20 IO������
#define WarnTemperatureMax 30.0     //Ĭ����߱����¶�ֵ
#define WarnTemperatureMin 0.0      //Ĭ����ͱ����¶�ֵ
typedef struct
{
	float Val[DS18B20_NUM];        //��ǰ�¶�
	float ValMax[DS18B20_NUM];     //��ʷ����¶�
	float ValMin[DS18B20_NUM];     //��ʷ����¶�
	float ValMaxWarn[DS18B20_NUM]; //��߱����¶�ֵ
	float ValMinWarn[DS18B20_NUM]; //��߱����¶�ֵ
}DS18B20;
extern DS18B20 Ds18b20;         //�����¶����ݶ�Ӧ��ʹ��˳��

uint8_t DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);    //��ʼ��DS18B20,���������IO�ڣ���(GPIOB,GPIO_Pin_12)
float   DS18B20_Update(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);  //��ȡDS18B20�¶� �� �����¶�(Ϊ�ṹ���������)  
#endif

/*
����

#define DS18B20_NUM     2      //��Ϊ2�� 
    
void Task1(void)    //����1������������
{
	DS18B20_Config(GPIOB,GPIO_Pin_12); //��һ����ʹ����PB12��Ӧ�ṹ������0
	DS18B20_Config(GPIOB,GPIO_Pin_6);  //�ڶ�����ʹ����PB6��Ӧ�ṹ������1
	while(1)
	{
		OSSchedLock();                       //��ȡ����ʱ��һ��Ҫ�ر������л�����ֹ�л����¸�����
		DS18B20_Update(GPIOB,GPIO_Pin_12); //�����¶�����, ��������ʱIO����˳��Ҫ��
		DS18B20_Update(GPIOB,GPIO_Pin_6); 
    OSSchedUnlock();		                 //��ȡ������գ��ָ������л�
		OS_delayMs(100); //100ms����һ���¶�
	}
}
void Task2(void)  //����2������������
{
	while(1)
	{
		OSprintf("PB12��ǰ�¶ȣ�%0.1f��  \r\n",Ds18b20.Val[0]);//����0��ӦPB12���¶�
		OSprintf("PB6��ǰ�¶ȣ� %0.1f��  \r\n",Ds18b20.Val[1]); //����1��ӦPB6���¶�
		
		if(Ds18b20.Val[1]>30.0) //�жϵڶ�����ʹ����DS118B20�¶ȣ���PB6���ϵ�DS18B20
		{
		  OSprintf("PB6�¶ȳ��ߣ�  \r\n");
			//xxxxx();//�رռ�����(xxxx()Ϊ�رռ��Ⱥ���������ʵ�ʼ������Լ�д)
		}
		OS_delayMs(1000);//1���ѯһ��
	}
}


*/













