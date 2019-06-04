#ifndef __DS18B20_H_
#define __DS18B20_H _
#include "stm32f10x.h" //����stm32f1_std�⣬��Ϊ�������޸�Ϊ����ͷ�ļ�����DS18B20.c���޸�3�����ú���
extern void delay_us(unsigned int nus);//��Ҫ�û����ⲿʵ��һ��us����ȷ��ʱ����

/*------------------------�û�����--------------------------------*/
#define DS18B20_NUM         1        //��λ������DS18B20 IO������
#define WarnTemperatureMax  30.0     //��λ���ȡ�Ĭ����߱����¶�ֵ
#define WarnTemperatureMin  0.0      //��λ���ȡ�Ĭ����ͱ����¶�ֵ
#define Deviatvalue        -3.7      //��λ���ȡ�ƫ��ֵ(У��ֵ,�ͱ�׼���¶�ֵ֮��Ĳ�ֵ)

#define C_Time              10       //��λ���Ρ�������ȡC_Time������֮��Ĳ�ֵС��D_Valueֵ��������һ�ζ�ȡ������ȷֵ            
#define D_Value             1.0      //��λ���ȡ����ζ�ȡֵ�Ĳ�ֵ(�Ӳ��¶����������⻷���¶���1�ȼ��ɣ������������Ϊ10�����ң��Ӽ����ٶȶ���)
#define TempMax             100.0    //��λ���ȡ���������
#define TempMin            -20.0     //��λ���ȡ��������� 
/*---------------------------------------------------------------*/

typedef struct
{
	float Val[DS18B20_NUM];         //��λ���ȡ���ǰ�¶�
	float ValAvg[DS18B20_NUM];      //��λ���ȡ�ƽ���¶�
	float ValMax[DS18B20_NUM];      //��λ���ȡ���ʷ����¶�
	float ValMin[DS18B20_NUM];      //��λ���ȡ���ʷ����¶�
	float ValMaxWarn[DS18B20_NUM];  //��λ���ȡ���߱����¶�ֵ
	float ValMinWarn[DS18B20_NUM];  //��λ���ȡ���ͱ����¶�ֵ
	unsigned int Avgcounter;
}DS18B20;
extern DS18B20 Ds18b20;         //�����¶����ݶ�Ӧ��ʹ��˳��

uint8_t DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//��ʼ��DS18B20,���������IO�ڣ���(GPIOB,GPIO_Pin_12)
float   DS18B20_Update(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//��ȡDS18B20�¶� �� �����¶�(Ϊ�ṹ���������)  
#endif

/*
��1��(����ϵͳ����)

#define DS18B20_NUM     2      //��Ϊ2�� 
    
void Task1(void)    //����1������������
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //��һ����ʹ����PB12��Ӧ�ṹ������0������
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //�ڶ�����ʹ����PB6��Ӧ�ṹ������1�����ݣ��Դ�����
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	while(1)
	{
		OSSchedLock();                       //��ȡ����ʱ��һ��Ҫ�ر������л�����ֹ�л����¸�����
		DS18B20_Update(GPIOB,GPIO_Pin_12); //�����¶�����, ��������ʱIO����˳��Ҫ���ȸ����ĸ�IO�����ݶ�����
		DS18B20_Update(GPIOB,GPIO_Pin_6); 
    OSSchedUnlock();		                 //��ȡ������գ��ָ������л�
		OS_delayMs(100); //100ms����һ���¶�
	}
}
void Task2(void)  //����2������������
{
	while(1)
	{
		OSprintf("PB12��ǰ�¶ȣ�%0.1f��  \r\n",Ds18b20.Val[0]);//����0��ӦPB12���¶ȵ�����
		OSprintf("PB6��ǰ�¶ȣ� %0.1f��  \r\n",Ds18b20.Val[1]); //����1��ӦPB6���¶ȵ�����
		
		if(Ds18b20.Val[1]>30.0) //�жϵڶ�����ʹ����DS18B20�¶ȣ���PB6���ϵ�DS18B20
		{
		  OSprintf("PB6�¶ȳ��ߣ�  \r\n");
			//xxxxx();//�رռ�����(xxxx()Ϊ�رռ��Ⱥ���������ʵ�ʼ������Լ�д)
		}
		OS_delayMs(1000);//1���ѯһ��
	}
}



��2��(�������)

//main��ʽ
int main(void)
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //��һ����ʹ����PB12��Ӧ�ṹ������0������
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //�ڶ�����ʹ����PB6��Ӧ�ṹ������1�����ݣ��Դ�����
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	while(1)
	{
	  DS18B20_Update(GPIOB,GPIO_Pin_12); //�����¶�����, ��������ʱIO����˳��Ҫ���ȸ����ĸ�IO�����ݶ�����
		DS18B20_Update(GPIOB,GPIO_Pin_6);  //�����¶�����
    printf("PB12��ǰ�¶ȣ�%0.1f��  \r\n",Ds18b20.Val[0]);//����0��ӦPB12���¶�
		printf("PB6��ǰ�¶ȣ� %0.1f��  \r\n",Ds18b20.Val[1]); //����1��ӦPB6���¶�
		if(Ds18b20.Val[1]>30.0) //�жϵڶ�����ʹ����DS18B20�¶ȣ���PB6���ϵ�DS18B20
		{
		  printf("PB6�¶ȳ��ߣ�  \r\n");
			//xxxxx();//�رռ�����(xxxx()Ϊ�رռ��Ⱥ���������ʵ�ʼ������Լ�д)
		}  
		delay_ms(100);//100msɨ��һ��(��main����������������Կ��Ƕ�ʱ��ɨ�跽ʽ)
	}
}

//��ʱ����ʽ
int main(void)
{
  char errchk1,errchk2;
	errchk1=DS18B20_Config(GPIOB,GPIO_Pin_12); //��һ����ʹ����PB12��Ӧ�ṹ������0������
	errchk2=DS18B20_Config(GPIOB,GPIO_Pin_6);  //�ڶ�����ʹ����PB6��Ӧ�ṹ������1�����ݣ��Դ�����
  if(errchk1) OSprintf("DS18B20 PB12 Config error! \r\n");
  if(errchk2) OSprintf("DS18B20 PB6 Config error! \r\n");
	TIMER_Config(TIM1,10);//��ʱ����ʹ��10ms�ж�һ��(�˺������ṩ���û��Լ�ʵ��)
	while(1)
	{
	  //�����������
	}
}

int Timer1_10ms_Cnt=0;
void TIM1_IRQHandler(void)
{
  TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	
	Timer1_10ms_Cnt++;
	if(Timer1_10ms_Cnt>=50)//50*10ms=500ms����һ���¶�����
	{
	  Timer1_10ms_Cnt=0;
		DS18B20_Update(GPIOB,GPIO_Pin_12); //�����¶�����, ��������ʱIO����˳��Ҫ���ȸ����ĸ�IO�����ݶ�����
		DS18B20_Update(GPIOB,GPIO_Pin_6);  //�����¶�����
		printf("PB12��ǰ�¶ȣ�%0.1f��  \r\n",Ds18b20.Val[0]);//����0��ӦPB12���¶�
		printf("PB6��ǰ�¶ȣ� %0.1f��  \r\n",Ds18b20.Val[1]); //����1��ӦPB6���¶�
		if(Ds18b20.Val[1]>30.0) //�жϵڶ�����ʹ����DS18B20�¶ȣ���PB6���ϵ�DS18B20
		{
			printf("PB6�¶ȳ��ߣ�  \r\n");
			//xxxxx();//�رռ�����(xxxx()Ϊ�رռ��Ⱥ���������ʵ�ʼ������Լ�д)
		}  
	}
}

-by Angle145
*/













