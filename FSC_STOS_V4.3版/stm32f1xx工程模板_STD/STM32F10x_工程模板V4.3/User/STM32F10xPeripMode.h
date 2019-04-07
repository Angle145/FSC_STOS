#ifndef _STM32F10xPeripMode_H_
#define _STM32F10xPeripMode_H_
#include "stm32f10x.h"
#include "fsc_stos.h"
#include <stdio.h>
#include "STM32F10x_C51Mode.h" //λ����IO����������RS485ʹ�ܷ��ͽ�������(���ò���ModBusģ��ɲ��ذ�����ͷ�ļ�)

/*+++++++++++++++++++++++++++++++++++++++++++++++���û����õ����躯��+++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define MODE_UNUSE 0
#define MODE_USE   1
//��Ҫ˵����ÿ��ģ���һ���궨��Ϊģ�鿪�� 0 - ��ʹ��  1 - ʹ��

/****************************************************�ⲿ�ж�ģ��*****************************************************************/
//ʾ��: EXTI_Config(GPIOB,GPIO_Pin_5,0);//PB5�����ⲿ�ж�����,0-�½��ش����жϣ�1-�����ش����жϣ�2-�½���/�����ش����ж�
#define USE_EXTI  1     //0--��ʹ���ⲿ�ж�  1--ʹ��
#if (USE_EXTI == 1)
void EXTI_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t EXTI_Trigger_Mode);//�ⲿ�ж����ų�ʹ��
#endif 

/****************************************************��ʱ��ģ��*******************************************************************/ 
//ʾ����TIMER_Config(TIM2,50000);//ʹ�ö�ʱ��2����ʱ50ms ��ʱ���ֵ65535us (TIM1����PWM����ʹ��PWM����TIM1����������ʱ��)
//      TIMER__Config(TIM2,72,1000);//ʹ�ö�ʱ��2����ʱ��Ƶ����Ϊ72M/72=1M,��ʱ1000�Ρ�(72MHz��stm32f1��Ƶ��һ��Ϊ�̶�ֵ)
//      TIMx  =   TIM1��TIM2��TIM3��TIM4   ��4�� (Ŀǰ�ط�װ�⺯����֧��4����ʱ��)
//      ȡֵ��Χ�� Prescaler : 1-65535   ,   Period  :  1-65535
#define USE_TIMER  1   //0--��ʹ�ö�ʱ��  1--ʹ��
#if (USE_TIMER == 1)
void TIMER_Config(TIM_TypeDef* TIMx,uint16_t TimePeriod);//��ʱ����ʹ��(TimePeriod=1-65535,��λ:us)
void TIMER_Reload(TIM_TypeDef* TIMx,uint16_t Period); //�����Զ���װ�ض�ʱֵ
void TIMER__Config(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period);//��ʱ��Ƶ��(��Ƶ)�ɵ��Ķ�ʱ����ʹ��
void TIMER__Reload(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period);//��ʱ��Ƶ��(��Ƶ)�ɵ��������Զ���װ�ض�ʱֵ
void TIMER_ON(TIM_TypeDef* TIMx); //�򿪶�ʱ��   ����TIMER_ON(TIM2);//�򿪶�ʱ��2
void TIMER_OFF(TIM_TypeDef* TIMx);//�رն�ʱ��   ����TIMER_OFF(TIM2);//�رն�ʱ��2
#endif 
//PS:Prescaler����Ϊstm32��Ƶ��Ƶ����ʱ����Ϊ��Ƶ�ķ�Ƶϵ��ֵ��PeriodΪ��ʱ������������������ɺ�����һ���жϱ�ʾ��ʱ��ɡ�


/******************************************************����ģ��*******************************************************************/  
//ʾ����USART2_Config(9600);//�����ʣ�9600�����õĲ����ʾ���Ч
#define USE_USART                 1     //0--��ʹ�ô���  1--ʹ��
#if (USE_USART == 1)
#define printf_use_usartx         2     //printf  ����ѡ�� 
#define USART_RX_BUFF_LEN         256   //���ջ��泤��
#define USART_TX_BUFF_LEN         32    //���ͻ��泤��
typedef struct
{
	uint8_t RX_BUFF[USART_RX_BUFF_LEN];  //���ջ���
	uint8_t TX_BUFF[USART_TX_BUFF_LEN];  //���ͻ���
	uint32_t ByteCnt;                 //�ֽڼ���(���ڽ��գ��������>0ʱ��ʾ�н��յ�����,�������������Ҫ������0)
}TypeUsart;
TypeUsart Usart1,Usart2,Usart3;  //3���������ݽṹ��
void USART1_Config(u32 baudRate);//����1��ʹ��         //Ĭ��  1λֹͣ������λ8λ������żУ�飬�õ������ɵ������ڲ��޸�
void USART2_Config(u32 baudRate);//����2��ʹ��         //Ĭ��  1λֹͣ������λ8λ������żУ�飬�õ������ɵ������ڲ��޸�
void USART3_Config(u32 baudRate);//����3��ʹ��         //Ĭ��  1λֹͣ������λ8λ������żУ�飬�õ������ɵ������ڲ��޸�
//����1: TX:PA9  RX:PA10
void USART1_SendByte(uint8_t onebyte);//����1����һ���ֽ�
void USART1_Send(uint8_t *p,uint16_t len);//����1���Ͷ���ֽ�(��������)
void USART1_SendStr(uint8_t *str);    //����1�����ַ���(�������ַ���)
uint8_t USART1_ReadByte(void);        //����1��ȡ���յ�һ���ֽ�
uint8_t* USART1_Read(uint16_t len);//����1��ȡ���յĶ���ֽ�
char* USART1_ReadStr(void);//����1��ȡ���յ��ַ���

//����2: TX:PA2  RX:PA3
void USART2_SendByte(uint8_t onebyte); //����2����һ���ֽ�
void USART2_Send(uint8_t *p,uint16_t len);//����2���Ͷ���ֽ�(��������)
void USART2_SendStr(uint8_t *str);     //����2�����ַ���(�������ַ���) 
uint8_t USART2_ReadByte(void);         //����2��ȡ���յ�һ���ֽ�
uint8_t* USART2_Read(uint16_t len);//����1��ȡ���յĶ���ֽ�
char* USART2_ReadStr(void);//����2��ȡ���յ��ַ���

//����3: TX:PB10  RX:PB11
void USART3_SendByte(uint8_t onebyte); //����3����һ���ֽ�
void USART3_Send(uint8_t *p,uint16_t len);//����3���Ͷ���ֽ�(��������)
void USART3_SendStr(uint8_t *str);     //����3�����ַ���(�������ַ���) 
uint8_t USART3_ReadByte(void);         //����3��ȡ���յ�һ���ֽ�
uint8_t* USART3_Read(uint16_t len);//����1��ȡ���յĶ���ֽ�
char* USART3_ReadStr(void);//����3��ȡ���յ��ַ���

void STM32F10xPeripMode_Usart1RxCheck(void);//���ڴ���1�жϺ�������ڼ�⣨�����жϺ�����ǰ�棩
void STM32F10xPeripMode_Usart2RxCheck(void);//���ڴ���2�жϺ�������ڼ�⣨�����жϺ�����ǰ�棩
void STM32F10xPeripMode_Usart3RxCheck(void);//���ڴ���3�жϺ�������ڼ�⣨�����жϺ�����ǰ�棩

void STM32F10xPeripMode_Usart1_UsartReceiveByte(uint8_t UsartRxByte); //���ڴ���1�жϺ���������β�Ϊ���յ�1�ֽ�����
void STM32F10xPeripMode_Usart2_UsartReceiveByte(uint8_t UsartRxByte); //���ڴ���2�жϺ���������β�Ϊ���յ�1�ֽ�����
void STM32F10xPeripMode_Usart3_UsartReceiveByte(uint8_t UsartRxByte); //���ڴ���3�жϺ���������β�Ϊ���յ�1�ֽ�����
#endif


/*************************************************ADCģ��(12bit ADC)***********************************************************/  
//PA0-PA7,PB0,PB1,PC0-PC5��16������
#define USE_ADC  1     //0--��ʹ��ADC  1--ʹ��
#if (USE_ADC == 1)
void ADC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//ADC��ʹ��
float ReadADC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//��ȡADCת���ĵ�ѹֵ(0-3.3000)
uint16_t ReadADC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//��ȡADCת������ֵ(0-4095)
#endif

/*************************************************DACģ��(12bit DAC)***********************************************************/
//PA4��PA5��2������
#define USE_DAC  0     //0--��ʹ��DAC  1--ʹ��
#if (USE_DAC == 1)
void DAC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//DAC��ʹ��
void WriteDAC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,float volt);//����DAC��ѹֵ(0-3.3000)
void WriteDAC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint16_t hex);//����DAC����ֵ(0-4095)
#endif

/*************************************************PWM�������ģ��***********************************************************/
//���PWM���ţ���·PA8 ��˫·������PA8��PB13
#define USE_PWM  0       //0--��ʹ��PWM  1--ʹ��
#if (USE_PWM == 1)
void PWM_TIM1_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse);//��·PWM���
void PWM_TIM1_OC_Config(uint16_t Prescaler,uint16_t Period,uint16_t Pulse,uint8_t DTime);//˫·����PWM���(������)
void PWM_TIM1_ON(void); //��PWM���
void PWM_TIM1_OFF(void);//�ر�PWM���
void PWM_TIM1_OC_ON(void); //��˫·PWM���
void PWM_TIM1_OC_OFF(void);//�ر�˫·PWM���
#endif

/*****************************************************Ƶ�ʼ�ģ��***********************************************************/
#define USE_FREQMETER         0     //1--ʹ��Ƶ�ʼ� 0--��ʹ��
#if (USE_FREQMETER == 1)
#define USE_FREQMETER_PRINTF  1     //1--ʹ�ô��ڴ�ӡ���Ƶ�ʼ���Ϣ  0--��ʹ��
#define FREQMETER_PRINTF_TIME 5000  //���ڷ�����ʱʱ��
typedef struct 
{
	uint32_t FreqCh[4];//ͨ��Ƶ��
  uint8_t DutyCh[4];//ͨ��ռ�ձ� (0-100)
}FREQMETER;
FREQMETER FreqMeter;  //Ƶ�ʼ��������  
//��������ֱ�Ϊ:��Ƶϵ��,����ͨ������
void FreqMeter_TIM2(uint16_t Prescaler,uint8_t CHNum);//CH1:PA0  CH2:PA1  CH3:PA2  CH4:PA3 
void FreqMeter_ON(void);  //��Ƶ�ʼ�
void FreqMeter_OFF(void); //�ر�Ƶ�ʼ�
#endif

/*********************************************ModBusRTU ��վ����ģ��*******************************************************/
//��ȡ����ʱ�����յ����ݱ�����RS485,RX_BUFF[]��������������ճ���RS485_BUFF_LEN
#define USE_MODEBUS     0      //0--��ʹ��modbus 1--ʹ��
#if (USE_MODEBUS == 1)
#define RS485_USARTTx   2      //=1ʹ�ô���1��ΪRS485����,=2 USART2  =3 USART3
#define RS485_USARTRx   2      //=1ʹ�ô���1��ΪRS485����,=2 USART2  =3 USART3
#define RS485_BUFF_LEN  32     //���պͷ��ͻ��泤��1-255(���255)
#define RS485_EN        PB15   //Ĭ��PB15ΪRS485ʹ�����ţ��ɸ���ʵ����Ҫ�޸�,��Ҫλ����IO������

TypeUsart RS485;        //���ڽṹ�嶨��RS485���ͷ��ͺͽ��ջ���
typedef struct
{
	/***************�û���Ҫ������ֵ*******************/
	uint8_t  SlaveID;  //�豸ID 8bit         //�ӻ���
	uint8_t  Function;  //���� 8bit          //������
	uint16_t Address;   //���ݵ�ַ 16bit     //���ݵ�ַ��
	uint16_t Word;      //����/���� 16bit    //������
	uint16_t WordNum;   //��ȡ���ݸ��� 16bit //���ݸ�����(����ȡ����ʱʹ��)
	uint16_t RegNum;    //�Ĵ������� 16bit   //�Ĵ���������(���Զ���Ĵ�������ʱʹ��)
  uint8_t  ByteNum;   //�ֽ��� 8bit        //��Ĵ������������ʱ���ֽ�����(����д��ʹ�����)
	uint8_t  BUFF[RS485_BUFF_LEN];  //������ݷ��ڴ˴� 8bit ,�����Ĵ�����д����������16���޸�Ϊ�����ֵ��ͬʱҲҪ�޸Ĵ��ڽ��ջ���Ĵ�С��ǰ���ǵ�Ƭ���ڴ��㹻��
	/**************************************************/
	uint16_t CRC16;   
	uint8_t  EnFlag;  
	uint8_t  FshFlag;
	uint8_t  ErrFlag;
	uint8_t  ByteCnt; 
  uint8_t  WordCnt; 	
}modbusRTU;
modbusRTU ModBusRTU_TX; //���ͺͽ��ջ���
void ModBusRTU_Config(void);   //ModBus��ʹ��
void ModBusRTU_Send(modbusRTU modbus);  //����modbusRTU�ṹ�����͸�ʽ������(����Ҫ����CRC���ݣ��������Զ�����ͷ���CRC)
void ModBusRTU_SendCmd(uint8_t *p,uint16_t len);//�����Զ�������ṹ���͸�ʽ������(����Ҫ����CRC���ݣ��������Զ�����ͷ���CRC)

void STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(uint8_t UsartRxByte);//������Ҫ�Ĵ����ж����Ҫ�ĸ����ھͷ����ĸ������ж�
#endif


/****************************************************���ݴ���ģ��**********************************************************/
#define USE_DigHand  0      //0--��ʹ�����ݴ���1--ʹ��
#if (USE_DigHand == 1)
//���������˲��㷨:                     //������������
double Ladder(double step,double value);//���˵�����С��step��ֵ,���ֵ��step�ı���ֵ
//��ͨ�����˲��㷨��                   //km�����ʵ��ڣ���1.000�����ʵ�ȡֵ��
double BandPass_M(double km,double max,double value); //���˵�max���ϵ�ֵ
double BandPass_L(double km,double min,double value); //���˵�min���µ�ֵ
double BandPass_ML(double km,double min,double max,double value); //���˵�min���µ�ֵ�ҹ��˵�max���ϵ�ֵ
double BandPass_MLN(double km,double min,double max,double value); //���˵�min-max��Χ�ڵ�ֵ
//�����������˲��㷨��
/*      Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵,�ο�ֵ��0.02
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա�ã��ο�ֵ��7.0000    */ 
double KalmanFilter(double ProcessNiose_Q,double MeasureNoise_R,const double ResrcData);//ResrcData-����ɼ��źŵı���(��̬��������פ�ڴ�)
#endif


/****************************************************�ڲ�Flashģ��**********************************************************/
//��Ҫ�����ݱ仯֮����ô�ģ������ݱ��浽�ڲ�flash
#define USE_FLASHWR   0       //0--��ʹ���ڲ�flash��д  1--ʹ��
#if  (USE_FLASHWR == 1)
void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord);//���ڲ�flashдn���ֽ�
void FLASH_ReadNWord(uint16_t *pBuffer, uint32_t ReadAddr, uint16_t nWord);//���ڲ�flash��ȡn���ֽ�
#endif


/****************************************************�������Ź�ģ��**********************************************************/
//��ֹ����
#define USE_IWDG 0           //0--��ʹ�ÿ��Ź�  1--ʹ��
#if  (USE_IWDG == 1)
void IWDG_Config(uint16_t nms);//�������Ź���ʹ����������ι����ʱʱ��(ms)����Χ��2ms-2000ms
void IWDG_Feed(void);//ι��   (�ڳ�ʱǰι��)
#endif


/****************************************************�ϵ���ģ��**********************************************************/
//��ѹ��Χ��2.2V-2.9V  (��⵽оƬ��ѹ���ڵ�ѹ��ֵʱ���������ж�,���ж�������������)
#define USE_BKP   0          //0--��ʹ�öϵ���  1--ʹ��
#if  (USE_BKP == 1)
void BKP_Config(float voltage); //��ʹ������ ʾ����BKP_Config(2.7); //оƬ����2.7Vʱ���������ж� (�����жϺ���  PVD_IRQHandler() �ڱ�ҳ�ײ�)
#endif
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


/*++++++++++++++++++++++++++++++++++++++++++++++++����˵��++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
//���¼����ǻ���ϵͳʱ��Ϊ72MHz����Ϊ����Ƶ�ʣ��뽫72000000Hz����ʵ��Ƶ�ʼ���
ģ��ʹ��ʾ����
  
1.PWMģ��
  ʾ��1��PWM_TIM1_Config(����1,����2,����3);
	                 **����1����Ƶϵ��1
									 **����2: ��Ƶϵ��2  
									 **����3: ռ��ֵ     (ռ�ձ�=����3/����2)
									 
				          ����������ȡֵ��Χ��Ϊ:0-65535
									PWM���Ƶ��=72MHz/��Ƶϵ��1/��Ƶϵ��2
									
  ʾ��2��PWM_TIM1_OC_Config(����1,����2,����3,����4);
		               **����1����Ƶϵ��1
									 **����2: ��Ƶϵ��2  
									 **����3: ռ��ֵ             ռ�ձ�=����3/����2
									 **����4������ʱ��**         ͨ���������������(��˵��-�����ļ�����)


2.�ڲ�Flashģ��
   ��ʾ:flash�в�д�������ޣ������ô�ģ��(��ҪƵ��ʹ�ã���������ѭ����д),ʹ��flash��Ϊ������ʱ����Զ����뷶Χ��д��

   ʾ����FLASH_WriteNWord((uint16_t*)(&StructMod)��0x00,sizeof(StructMod));//�ѽṹ��StructMod���ݱ��浽flash
	       FLASH_ReadNWord((uint16_t*)(&StructMod)��0x00,sizeof(StructMod)); //��ȡflash�нṹ��StructMod���ݵ��ṹ��StructMod

*/



















































/**********************************************��������Դ����*************************************************/


/*************************************************�ⲿ�ж�***********************************************************/
/*********************************************************************************
*������:EXTI_Config() �ⲿ�����жϳ�ʹ������
*����:  GPIOx             -- x=A,B,C,D,E,F      IO����
        GPIO_Pin_x        -- x=0,1,2,...14,15   IO�����ź�
        EXTI_Trigger_Mode --                    �жϴ�����ʽ
                              =0 �½��ش��� 
                              =1�����ش��� 
                              =2�����غ��½��ؾ��ɴ���   
**********************************************************************************/
#if (USE_EXTI == 1)
void EXTI_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,uint8_t EXTI_Trigger_Mode) //�ⲿ�жϳ�ʹ������//(�û��ɵ���)
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

	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_Source,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	 
	GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);//ѡ��GPIO�ܽ������ⲿ�ж���·
	//�˴�һ��Ҫ��ס���˿ڹܽż����ж��ⲿ��·
	/* �����ⲿ�жϵ�ģʽ */ 
	if(EXTI_Trigger_Mode==0)      EXTI_Trigger=EXTI_Trigger_Falling;        //�½��ش���
	else if(EXTI_Trigger_Mode==1) EXTI_Trigger=EXTI_Trigger_Rising;         //�����ش���
	else if(EXTI_Trigger_Mode==2) EXTI_Trigger=EXTI_Trigger_Rising_Falling; //����/�½��ش���
	EXTI_InitStructure.EXTI_Line=EXTI_Line;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger; //�ⲿ���Ŵ�����ʽ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/* ����NVIC���� */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI_IRQn; 	//��EXTI2��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //ʹ��
	NVIC_Init(&NVIC_InitStructure); 		
}
#endif



















/*************************************************��ʱ��***********************************************************/
#if (USE_TIMER == 1)
void TIMER_Config(TIM_TypeDef* TIMx,uint16_t TimePeriod) //��ʱ�����ϼ���������1����ʱ1us�����655335us
{
	uint32_t RCC_APBPeriph_Source;
	uint8_t TIM_IRQn;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //����һ���ṹ�������������ʼ��GPIO
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ
	
	if(TIMx==TIM1)      { RCC_APBPeriph_Source=RCC_APB2Periph_TIM1;TIM_IRQn=TIM1_UP_IRQn; }                      
	else if(TIMx==TIM2) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM2;TIM_IRQn=TIM2_IRQn; }
	else if(TIMx==TIM3) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM3;TIM_IRQn=TIM3_IRQn; }
	else if(TIMx==TIM4) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM4;TIM_IRQn=TIM4_IRQn; }
	
	/* ������ʱ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_Source,ENABLE);
  
	TIM_TimeBaseInitStructure.TIM_Period = TimePeriod-1;//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72-1;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ��1000khz����Ƶ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);	
	TIM_Cmd(TIMx,ENABLE); //ʹ�ܻ���ʧ��TIMx����
	/* �����жϲ����������ж� */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );	//ʹ�ܻ���ʧ��ָ����TIM�ж�
	
	/* ����NVIC���� */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM_IRQn; //��TIM_IRQn��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}
void TIMER__Config(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period)
{
	uint32_t RCC_APBPeriph_Source;
	uint8_t TIM_IRQn;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	 //����һ���ṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);//���TIMx���жϴ�����λ:TIM �ж�Դ
	
	if(TIMx==TIM1)      { RCC_APBPeriph_Source=RCC_APB2Periph_TIM1;TIM_IRQn=TIM1_UP_IRQn; }                      
	else if(TIMx==TIM2) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM2;TIM_IRQn=TIM2_IRQn; }
	else if(TIMx==TIM3) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM3;TIM_IRQn=TIM3_IRQn; }
	else if(TIMx==TIM4) { RCC_APBPeriph_Source=RCC_APB1Periph_TIM4;TIM_IRQn=TIM4_IRQn; }
	
	/* ������ʱ��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APBPeriph_Source,ENABLE);
  
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ��1000khz����Ƶ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);	
	TIM_Cmd(TIMx,ENABLE); //ʹ�ܻ���ʧ��TIMx����
	/* �����жϲ����������ж� */
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );	//ʹ�ܻ���ʧ��ָ����TIM�ж�
	
	/* ����NVIC���� */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel=TIM_IRQn; //��TIM_IRQn��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;	//��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;  //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}
void TIMER_Reload(TIM_TypeDef* TIMx,uint16_t Period) //�����Զ���װ�ض�ʱֵ
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
}
void TIMER__Reload(TIM_TypeDef* TIMx,uint16_t Prescaler,uint16_t Period) //�����Զ���װ�ض�ʱֵ
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
	TIM_TimeBaseInitStructure.TIM_Period = Period-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
}
void TIMER_ON(TIM_TypeDef* TIMx)  //�򿪶�ʱ��
{
  TIM_Cmd(TIMx,ENABLE);	
}
void TIMER_OFF(TIM_TypeDef* TIMx) //�رն�ʱ��
{
  TIM_Cmd(TIMx,DISABLE);	
}
#endif










/*************************************************PWM�������***********************************************************/
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period =Period-1 ;//�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler-1;//����������ΪTIMxʱ��Ƶ��Ԥ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; //ģʽ1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//PWM���ʹ��
	//TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//PWM���ʹ��
	//TIM_OCInitStructure.TIM_OutputState=TIM_OutputNState_Disable; //PWM���ʧ��
	TIM_OCInitStructure.TIM_Pulse=Pulse-1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;  //��ʱ��ֹͣ����ʱIO�ڵ�״̬Ϊ��(TIM_OCPolarity_HighΪ��)
	//TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
	//TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_High; //�������(Lowͬ��)
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set; //����ʱ�����
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//����ʱ�����  
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	//TIM_Cmd(TIM1,ENABLE);//

	//��������Ǹ߼���ʱ��TIM1��TIM8���У����PWM�����
	//TIM_CtrlPWMOutputs(TIM1, ENABLE); //pwm���
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;//|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = Period-1; //�������ڣ����ϼǵ�����������ֵ����
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1;//��ʱ����Ƶϵ����Ftimer = 72M/(TIM_Prescaler+1) 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//��ʱ��ʱ���ٷ�Ƶ���������˲���ʹ�ã�����������Ҳʹ�ô�ʱ�ӣ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
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
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//PWM2ģʽ
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�Ƚϻ������ʹ��
	TIM_OCInitStructure.TIM_Pulse = Pulse-1;   //�Ƚ�ֵ����ռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //�������
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;//�����������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;//ָ������״̬�µ�TIM����Ƚϵ�����״̬��
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;//ָ������״̬�µ�TIM��������Ƚϵ�����״̬��
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);   //��ʼ��ͨ�����Ƚ����
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);      //����ͨ��һ���Զ���װ��ʹ��  

	TIM_ARRPreloadConfig(TIM1, ENABLE);//����װ��ֵ ENABLE ������Ч��DISABLE ��һ���Ƚ�������Ч

	/* TIM1 enable counter */
	//TIM_Cmd(TIM1, ENABLE);//ʹ�ܶ�ʱ��1
	
	//TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��PWM��Χ��� 
}

void PWM_TIM1_ON(void)
{
  TIM_Cmd(TIM1, ENABLE);//ʹ�ܶ�ʱ��1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��PWM��Χ���
}
void PWM_TIM1_OFF(void)
{
  TIM_Cmd(TIM1, DISABLE);//ʧ�ܶ�ʱ��1
	TIM_CtrlPWMOutputs(TIM1, DISABLE);//ʹ��PWM��Χ���
}
void PWM_TIM1_OC_ON(void)
{
  TIM_Cmd(TIM1, ENABLE);//ʹ�ܶ�ʱ��1
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//ʹ��PWM��Χ���
}
void PWM_TIM1_OC_OFF(void)
{
  TIM_Cmd(TIM1, DISABLE);//ʧ�ܶ�ʱ��1
	TIM_CtrlPWMOutputs(TIM1, DISABLE);//ʹ��PWM��Χ���
}
#endif














/****************************************************Ƶ�ʼ�*************************************************************/
//CAPTURE_STA_TIM2CH[]����״̬λ˵��
//[7]:0,û�гɹ�����1,�ɹ�����һ����������
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ.
//[5]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽸ߵ�ƽ.
//[4:0]:����͵�ƽ������ڴ���������δʹ�ã�����ֱ�Ӳ����ۼ���ͣ�
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
	
    //����TIM2��GPIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    //GPIO��ʼ��
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;            //��������
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
    
    //TIM2��ʱ����ʼ��
    TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF -1 ; //��Ϊ���
    TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler-1;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;        
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;        //���ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    
    //TIM2_CH1���벶���ʼ��
		if(CHNum>0)
		{		
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC1
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
		if(CHNum>1)
		{
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC2
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
		if(CHNum>2)
		{
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_3;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC3
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
   
		if(CHNum>3)
		{		
			TIM_ICInitStruct.TIM_Channel = TIM_Channel_4;
			TIM_ICInitStruct.TIM_ICFilter = 0x00;                            //���˲�
			TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;        //�����ز���
			TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;                //����������Ƶ
			TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;    //ӳ�䵽IC4
			TIM_ICInit(TIM2, &TIM_ICInitStruct);
		}
    
    //�жϷ����ʼ��
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
    //TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4, ENABLE);            //�����жϺ�CC1IE�����ж�
    
    //TIM_Cmd(TIM2, ENABLE);
		#if (USE_FREQMETER_PRINTF==1 ) 
    printf("Init FreqMeter_TIM2 Success ! \r\n"); 
		#endif
}
void FreqMeter_ON(void)
{
  TIM_Cmd(TIM2, ENABLE);//ʹ�ܶ�ʱ��
}
void FreqMeter_OFF(void)
{
  TIM_Cmd(TIM2, DISABLE);//ʧ�ܶ�ʱ��
}
void FreqMeterTIM2_IRQHandler(void) 
{     
		if(FreqMeter_CH_NUM>0)
		{	
			if((CAPTURE_STA_TIM2CH[0]&0x80) == 0) {                    //��δ����ɹ�
					if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {    //����1���������¼�
							if(CAPTURE_STA_TIM2CH[0]&0x40) {                    //����һ��������
									CAPTURE_STA_TIM2CH[0] |= 0x80;                    //��ǳɹ�����һ����������
									CAPTURE_CYCLE_TIM2CH[0] = TIM_GetCapture1(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[0] >= CAPTURE_DOWN_TIM2CH[0]) 
											CAPTURE_DOWNVAL_TIM2CH[0] = CAPTURE_CYCLE_TIM2CH[0] - CAPTURE_DOWN_TIM2CH[0];
									else 
											CAPTURE_DOWNVAL_TIM2CH[0] = 0xffff + CAPTURE_CYCLE_TIM2CH[0] - CAPTURE_DOWN_TIM2CH[0];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز��� //���¿�ʼ������������
								}									
							else if(CAPTURE_STA_TIM2CH[0]&0x20) {                    //����һ���½���
									CAPTURE_STA_TIM2CH[0] |= 0x40;                    //��ǳɹ�����һ�θߵ�ƽ����
									CAPTURE_DOWN_TIM2CH[0] = TIM_GetCapture1(TIM2);
									if(CAPTURE_DOWN_TIM2CH[0] >= CAPTURE_UP_TIM2CH[0]) 
											CAPTURE_UPVAL_TIM2CH[0] = CAPTURE_DOWN_TIM2CH[0] - CAPTURE_UP_TIM2CH[0];
									else 
											CAPTURE_UPVAL_TIM2CH[0] = 0xffff + CAPTURE_DOWN_TIM2CH[0] - CAPTURE_UP_TIM2CH[0];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز���
							} else {                                        //��δ��ʼ����һ�β���������
									CAPTURE_STA_TIM2CH[0] = 0;                    //���
								  CAPTURE_DOWNVAL_TIM2CH[0]=0;
									CAPTURE_UPVAL_TIM2CH[0] = 0;
									CAPTURE_UP_TIM2CH[0] = TIM_GetCapture1(TIM2);
									CAPTURE_STA_TIM2CH[0] |= 0x20;                //��ǲ�����������
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 ����Ϊ�½��ز���
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC1);                                //���״̬��־
					}
			} 
		}

		if(FreqMeter_CH_NUM>1)
		{
      if((CAPTURE_STA_TIM2CH[1]&0x80) == 0) {                    //��δ����ɹ�
        if(TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {            //����2���������¼�
							if(CAPTURE_STA_TIM2CH[1]&0x40) {                    //����һ��������
									CAPTURE_STA_TIM2CH[1] |= 0x80;                    //��ǳɹ�����һ����������
									CAPTURE_CYCLE_TIM2CH[1] = TIM_GetCapture2(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[1] >= CAPTURE_DOWN_TIM2CH[1]) 
											CAPTURE_DOWNVAL_TIM2CH[1] = CAPTURE_CYCLE_TIM2CH[1] - CAPTURE_DOWN_TIM2CH[1];
									else 
											CAPTURE_DOWNVAL_TIM2CH[1] = 0xffff + CAPTURE_CYCLE_TIM2CH[1] - CAPTURE_DOWN_TIM2CH[1];
									TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز��� //���¿�ʼ������������
								}
            else if(CAPTURE_STA_TIM2CH[1]&0x20) {                        //����һ���½���
                CAPTURE_STA_TIM2CH[1] |= 0x40;                        //��ǳɹ�����һ�θߵ�ƽ����
                CAPTURE_DOWN_TIM2CH[1] = TIM_GetCapture2(TIM2);        //��ȡ����2����
                if(CAPTURE_DOWN_TIM2CH[1] >= CAPTURE_UP_TIM2CH[1])
                    CAPTURE_UPVAL_TIM2CH[1] = CAPTURE_DOWN_TIM2CH[1] - CAPTURE_UP_TIM2CH[1];
                else
                    CAPTURE_UPVAL_TIM2CH[1] = 0xffff + CAPTURE_DOWN_TIM2CH[1] - CAPTURE_UP_TIM2CH[1];
                TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز���
            } else {                                                //��δ��ʼ����һ�β���������
                CAPTURE_STA_TIM2CH[1] = 0;                            //���
							  CAPTURE_DOWNVAL_TIM2CH[1]=0;
                CAPTURE_UPVAL_TIM2CH[1] = 0;
                CAPTURE_UP_TIM2CH[1] = TIM_GetCapture2(TIM2);
                CAPTURE_STA_TIM2CH[1] |= 0x20;                //��ǲ�����������
                TIM_OC2PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 ����Ϊ�½��ز���
            }
            TIM_ClearFlag(TIM2, TIM_FLAG_CC2);                                //���״̬��־    
        }
      } 
		}
    
		if(FreqMeter_CH_NUM>2)
		{
			if((CAPTURE_STA_TIM2CH[2]&0x80) == 0) {                    //��δ����ɹ�
					if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {    //����1���������¼�
							if(CAPTURE_STA_TIM2CH[2]&0x40) {                    //����һ���½���
									CAPTURE_STA_TIM2CH[2] |= 0x80;                    //��ǳɹ�����һ�θߵ�ƽ����
									CAPTURE_CYCLE_TIM2CH[2] = TIM_GetCapture3(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[2] >= CAPTURE_DOWN_TIM2CH[2]) 
											CAPTURE_DOWNVAL_TIM2CH[2] = CAPTURE_CYCLE_TIM2CH[2] - CAPTURE_DOWN_TIM2CH[2];
									else 
											CAPTURE_DOWNVAL_TIM2CH[2] = 0xffff + CAPTURE_CYCLE_TIM2CH[2] - CAPTURE_DOWN_TIM2CH[2];
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز��� //���¿�ʼ������������
								}									
							else if(CAPTURE_STA_TIM2CH[2]&0x20) {                    //����һ���½���
									CAPTURE_STA_TIM2CH[2] |= 0x40;                    //��ǳɹ�����һ�θߵ�ƽ����
									CAPTURE_DOWN_TIM2CH[2] = TIM_GetCapture3(TIM2);
									if(CAPTURE_DOWN_TIM2CH[2] >= CAPTURE_UP_TIM2CH[2]) 
											CAPTURE_UPVAL_TIM2CH[2] = CAPTURE_DOWN_TIM2CH[2] - CAPTURE_UP_TIM2CH[2];
									else 
											CAPTURE_UPVAL_TIM2CH[2] = 0xffff + CAPTURE_DOWN_TIM2CH[2] - CAPTURE_UP_TIM2CH[2];
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز���
							} else {                                        //��δ��ʼ����һ�β���������
									CAPTURE_STA_TIM2CH[2] = 0;                    //���
								  CAPTURE_DOWNVAL_TIM2CH[2]=0;
									CAPTURE_UPVAL_TIM2CH[2] = 0;
									CAPTURE_UP_TIM2CH[2] = TIM_GetCapture3(TIM2);
									CAPTURE_STA_TIM2CH[2] |= 0x20;                //��ǲ�����������
									TIM_OC3PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 ����Ϊ�½��ز���
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC3);                                //���״̬��־
					}
			} 
		}
		
		if(FreqMeter_CH_NUM>3)
		{
			if((CAPTURE_STA_TIM2CH[3]&0x80) == 0) {                    //��δ����ɹ�
					if(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) {    //����1���������¼�
							if(CAPTURE_STA_TIM2CH[3]&0x40) {                    //����һ���½���
									CAPTURE_STA_TIM2CH[3] |= 0x80;                    //��ǳɹ�����һ�θߵ�ƽ����
									CAPTURE_CYCLE_TIM2CH[3] = TIM_GetCapture4(TIM2);
									if(CAPTURE_CYCLE_TIM2CH[3] >= CAPTURE_DOWN_TIM2CH[3]) 
											CAPTURE_DOWNVAL_TIM2CH[3] = CAPTURE_CYCLE_TIM2CH[3] - CAPTURE_DOWN_TIM2CH[3];
									else 
											CAPTURE_DOWNVAL_TIM2CH[3] = 0xffff + CAPTURE_CYCLE_TIM2CH[3] - CAPTURE_DOWN_TIM2CH[3];
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز��� //���¿�ʼ������������
								}									
							else if(CAPTURE_STA_TIM2CH[3]&0x20) {                    //����һ���½���
									CAPTURE_STA_TIM2CH[3] |= 0x40;                    //��ǳɹ�����һ�θߵ�ƽ����
									CAPTURE_DOWN_TIM2CH[3] = TIM_GetCapture4(TIM2);
									if(CAPTURE_DOWN_TIM2CH[3] >= CAPTURE_UP_TIM2CH[3]) 
											CAPTURE_UPVAL_TIM2CH[3] = CAPTURE_DOWN_TIM2CH[3] - CAPTURE_UP_TIM2CH[3];
									else 
											CAPTURE_UPVAL_TIM2CH[3] = 0xffff + CAPTURE_DOWN_TIM2CH[3] - CAPTURE_UP_TIM2CH[3];
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Rising);    //CC1P=0 ����Ϊ�����ز���
							} else {                                        //��δ��ʼ����һ�β���������
									CAPTURE_STA_TIM2CH[3] = 0;                    //���
								  CAPTURE_DOWNVAL_TIM2CH[3]=0;
									CAPTURE_UPVAL_TIM2CH[3] = 0;
									CAPTURE_UP_TIM2CH[3] = TIM_GetCapture4(TIM2);
									CAPTURE_STA_TIM2CH[3] |= 0x20;                //��ǲ�����������
									TIM_OC4PolarityConfig(TIM2, TIM_ICPolarity_Falling);    //CC1P=1 ����Ϊ�½��ز���
							}    
							TIM_ClearFlag(TIM2, TIM_FLAG_CC4);                                //���״̬��־
					}
			} 
		}
  
    //����֡����
		if((FreqMeterTimerSW&0x80)==0)
		{
			FreqMeterTimerSW|=0x80;
			if(GetOSSliceTime()+ FREQMETER_PRINTF_TIME< 0xFFFFFFFF) OSTimerValue=GetOSSliceTime(); 
			else  OSTimerValue=FREQMETER_PRINTF_TIME+GetOSSliceTime()-0xFFFFFFFF;
		}
		if(FreqMeter_CH_NUM>0)  
		{	
			if(CAPTURE_STA_TIM2CH[0]&0x80) {                                //�ɹ�������һ������        
				  FreqMeter.FreqCh[0] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[0]+CAPTURE_DOWNVAL_TIM2CH[0]);           //���ʱ���ܺ�,�õ��ܵ�����ʱ��
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
			if(CAPTURE_STA_TIM2CH[1]&0x80) {                                //�ɹ�������һ������     
				  FreqMeter.FreqCh[1] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[1]+CAPTURE_DOWNVAL_TIM2CH[1]);           //���ʱ���ܺ�,�õ��ܵ�����ʱ��
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
			if(CAPTURE_STA_TIM2CH[2]&0x80) {                                //�ɹ�������һ������       
				  FreqMeter.FreqCh[2] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[2]+CAPTURE_DOWNVAL_TIM2CH[2]);           //���ʱ���ܺ�,�õ��ܵ�����ʱ��
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
			if(CAPTURE_STA_TIM2CH[3]&0x80) {                                //�ɹ�������һ������         
				  FreqMeter.FreqCh[3] = SystemCoreClock/FreqMeter_Prescaler/(CAPTURE_UPVAL_TIM2CH[3]+CAPTURE_DOWNVAL_TIM2CH[3]);           //���ʱ���ܺ�,�õ��ܵ�����ʱ��
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
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
	}
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/**************************************************����ģ��******************************************************/	
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

/********************************���ڽ��ռ��***************************************/
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

/**********************************���ڽ��վݾ�************************************/
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
*����1����: PA9  --  TXD
            PA10 --  RXD
*ֹͣλ��1  
*����λ��8
*��żУ�飺��
************************************************************************/
void USART1_Config(u32 baudRate) //����1��ʹ��  ʹ��printf�������з������� ���û��ɵ��ã�
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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b��8λ���ݣ�USART_WordLength_9b��9λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5��0.5��ֹͣλ��USART_StopBits_1��1����USART_StopBits_1_5��1.5����USART_StopBits_2��2��
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No������żУ�飬USART_Parity_Even��żУ�飬USART_Parity_Odd����У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ���˴��շ���ʹ��
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	//���������ж�
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	//���������ж�
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1,USART_FLAG_TC);
}

void USART2_Config(u32 baudRate)   //(�û��ɵ���)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;
 
	//ע��USART2�õ���APB1��ʱ��
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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b��8λ���ݣ�USART_WordLength_9b��9λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5��0.5����USART_StopBits_1��1��ֹͣλ��USART_StopBits_1_5��1.5����USART_StopBits_2��2��
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No������żУ�飬USART_Parity_Even��żУ�飬USART_Parity_Odd����У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ���˴��շ���ʹ��
	USART_Init(USART2, &USART_InitStructure); 
 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 	//���������ж�
	//USART_ITConfig(USART2, USART_IT_TXE, ENABLE);	//���������ж�
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2,USART_FLAG_TC);
}

void USART3_Config(u32 baudRate)   //(�û��ɵ���) //PB10(TX) PB11(RX)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	USART_InitTypeDef USART_InitStructure;
 
	//ע��USART3�õ���APB1��ʱ��
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
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//USART_WordLength_8b��8λ���ݣ�USART_WordLength_9b��9λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//USART_StopBits_0_5��0.5����USART_StopBits_1��1��ֹͣλ��USART_StopBits_1_5��1.5����USART_StopBits_2��2��
	USART_InitStructure.USART_Parity = USART_Parity_No;  //USART_Parity_No������żУ�飬USART_Parity_Even��żУ�飬USART_Parity_Odd����У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ���˴��շ���ʹ��
	USART_Init(USART3, &USART_InitStructure); 
 
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 	//���������ж�
	//USART_ITConfig(USART3, USART_IT_TXE, ENABLE);	//���������ж�
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
}

void USART1_SendByte(uint8_t onebyte)
{
  USART_SendData(USART1,onebyte);//ͨ������USARTx���͵�������                    
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
}
void USART1_SendStr(uint8_t *str)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(*str=='\0') break; //����ʱ������'\0'�ַ����������
		USART_SendData(USART1,*str);//ͨ������USARTx���͵�������                    
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
		str++;
	}
}
void USART1_Send(uint8_t *p,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(USART1,*p++);//ͨ������USARTx���͵�������                    
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t USART1_ReadByte(void)
{
  return (USART_ReceiveData(USART1));
}
uint8_t* USART1_Read(uint16_t len)//����1��ȡ���յĶ���ֽ�
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
char* USART1_ReadStr(void)//����1��ȡ���յ��ַ���
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
		if(*str!='\0') //����ʱ������'\0'�ַ����������
		{
			USART_SendData(USART2,*str);//ͨ������USARTx���͵�������                    
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
		USART_SendData(USART2,*p++);//ͨ������USARTx���͵�������                    
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t* USART2_Read(uint16_t len)//����2��ȡ���յĶ���ֽ�
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
char* USART2_ReadStr(void)//����2��ȡ���յ��ַ���
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
  USART_SendData(USART3,onebyte);//ͨ������USARTx���͵�������                    
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==Bit_RESET);
}
void USART3_SendStr(uint8_t *str)
{
	uint16_t i;
	for(i=0;i<65535;i++)
	{
		if(*str!='\0') //����ʱ������'\0'�ַ����������
		{
			USART_SendData(USART3,*str);//ͨ������USARTx���͵�������                    
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
		USART_SendData(USART3,*p++);//ͨ������USARTx���͵�������                    
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==Bit_RESET);
	}
}
uint8_t* USART3_Read(uint16_t len)//����3��ȡ���յĶ���ֽ�
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
char* USART3_ReadStr(void)//����3��ȡ���յ��ַ���
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















/*************************************************ModBusRTU��վ***********************************************************/
#if (USE_MODEBUS == 1)
#define RS485_RX_EN  RS485_EN=0   //RS485����ʹ��   
#define RS485_TX_EN  RS485_EN=1   //RS485����ʹ�� 


void STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(uint8_t UsartRxByte)
{
  if(RS485.ByteCnt<RS485_BUFF_LEN-1) RS485.RX_BUFF[RS485.ByteCnt++]=UsartRxByte;
  else { RS485.ByteCnt=0; RS485.RX_BUFF[RS485.ByteCnt]=UsartRxByte; }
}


void ModBusRTU_Config(void)
{
  RS485.ByteCnt=0;  //���RS485���ջ���
	RS485_TX_EN;      //RS485����Ϊ����ģʽ
}
void RS485_Send(uint8_t *p,uint16_t num)
{
	RS485_TX_EN;
	if(RS485_USARTTx==1) USART1_Send(p,num); //ʹ�ô���1���� 
	else if(RS485_USARTTx==2)USART2_Send(p,num); //ʹ�ô���2����
	else if(RS485_USARTTx==3)USART3_Send(p,num); //ʹ�ô���3����
	RS485_RX_EN;
}
uint16_t CRC16_ByteCheck(uint16_t crc16)
{
	uint8_t i;

	for(i=0;i<8;i++)
	{
		if(crc16&0x0001) { crc16>>=1; crc16^=0xA001; } //���ж�����λ
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
	CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);//CRC�ߵ��ֽڻ���
	return CRC16;
}
void ModBusRTU_Send(modbusRTU modbus)
{
	uint16_t i;
	uint16_t CRC16;
	
	ModBusRTU_TX=modbus;
	if((ModBusRTU_TX.Function==15)||(ModBusRTU_TX.Function==16))
	{
	  /*********************CRC����****************************/	
		CRC16=0xFFFF;   //��ʹ��Ϊ0xFFFF(������0xFFFF)
		CRC16^=ModBusRTU_TX.SlaveID;    //У��ӻ�ID��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Function;     //У�鹦����
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address>>8;     //У���8λ���ݵ�ַ��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address&0x00FF;     //У���8λ���ݵ�ַ��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.RegNum>>8;     //У���8λ��ȡ�Ĵ�������
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.RegNum&0x00FF;     //У���8λ��ȡ�Ĵ�������
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.ByteNum;       //У��8λ�����ֽ���
		CRC16=CRC16_ByteCheck(CRC16);
		for(i=0;i<ModBusRTU_TX.ByteNum;i++)
		{
			CRC16^=ModBusRTU_TX.BUFF[i];       //У��8λ ByteNum������
		  CRC16=CRC16_ByteCheck(CRC16);
		}		
		CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);   //CRC�ߵ��ֽڻ���
    /******************************************/
		RS485.TX_BUFF[0]=ModBusRTU_TX.SlaveID;    //�����豸ID��
		RS485.TX_BUFF[1]=ModBusRTU_TX.Function;    //���͹�����
		RS485.TX_BUFF[2]=ModBusRTU_TX.Address>>8;   //���͸�8λ���ݵ�ַ��
		RS485.TX_BUFF[3]=ModBusRTU_TX.Address;      //���͵�8λ���ݵ�ַ��
		RS485.TX_BUFF[4]=ModBusRTU_TX.RegNum>>8;  //���͸�8λ��ȡ���ݸ�����
		RS485.TX_BUFF[5]=ModBusRTU_TX.RegNum;      //���͵�8λ��ȡ���ݸ�����
		RS485.TX_BUFF[6]=ModBusRTU_TX.ByteNum;      //���͵�8λ �ֽ���
		for(i=0;i<ModBusRTU_TX.ByteNum;i++)
		{
		  RS485.TX_BUFF[i+7]=ModBusRTU_TX.BUFF[i];
		}
		RS485.TX_BUFF[ModBusRTU_TX.ByteNum+7]=CRC16>>8;   //����CRC���ֽ�
		RS485.TX_BUFF[ModBusRTU_TX.ByteNum+8]=CRC16;      //����CRC���ֽ�
		RS485.ByteCnt=0;
	  RS485_Send(RS485.TX_BUFF,ModBusRTU_TX.ByteNum+9);
	}
	else
	{
	/*********************CRC����****************************/	
		CRC16=0xFFFF;   //��ʹ��Ϊ0xFFFF(������0xFFFF)
		CRC16^=ModBusRTU_TX.SlaveID;    //У��ӻ�ID��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Function;     //У�鹦����
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address>>8;     //У���8λ���ݵ�ַ��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Address&0x00FF;     //У���8λ���ݵ�ַ��
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Word>>8;     //У���8λ������
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16^=ModBusRTU_TX.Word&0x00FF;     //У���8λ������
		CRC16=CRC16_ByteCheck(CRC16);
		CRC16=((CRC16&0x00FF)<<8)|((CRC16&0xFF00)>>8);   //CRC�ߵ��ֽڻ���
	/******************************************/
		RS485.TX_BUFF[0]=ModBusRTU_TX.SlaveID;    //�����豸ID��
		RS485.TX_BUFF[1]=ModBusRTU_TX.Function;    //���͹�����
		RS485.TX_BUFF[2]=ModBusRTU_TX.Address>>8;   //���͸�8λ���ݵ�ַ��
		RS485.TX_BUFF[3]=ModBusRTU_TX.Address;      //���͵�8λ���ݵ�ַ��
		RS485.TX_BUFF[4]=ModBusRTU_TX.Word>>8;      //���͸�8λ������
		RS485.TX_BUFF[5]=ModBusRTU_TX.Word;         //���͵�8λ������
		RS485.TX_BUFF[6]=CRC16>>8;                 //����CRC���ֽ�
		RS485.TX_BUFF[7]=CRC16;                    //����CRC���ֽ�
		RS485.ByteCnt=0;
	  RS485_Send(RS485.TX_BUFF,8);
	}
	RS485.ByteCnt=0;  //��0׼������
	delay_ms(4); //֡���ʱ��
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
	RS485.ByteCnt=0;  //��0׼������
	delay_ms(4); //֡���ʱ��
}
#endif
















/*************************************************���ݴ���***********************************************************/
#if (USE_DigHand == 1)
/**************PID�����㷨**************/
//(��ʱδ��)


/**************���������˲��㷨**************/
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
/***********��ͨ�����˲��㷨**************/
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
double BandPass_M(double km,double max,double value) //���˵�max���ϵ�ֵ
{
  if(value>max) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_L(double km,double min,double value) //���˵�min���µ�ֵ
{
  if(value<min) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_ML(double km,double min,double max,double value) //���˵�min���µ�ֵ�ҹ��˵�max���ϵ�ֵ
{
  if((value>max)||(value<min)) value=Growth_k(km,value);
	bandpass.Ek_1=bandpass.Ek;
	return value;
}
double BandPass_MLN(double km,double min,double max,double value) //���˵�min-max��Χ�ڵ�ֵ
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

/******�����������˲��㷨***************************/
/*       
        Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
        R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��       
*/
//�������˲�����������

/* �������˲����� */
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
    p_mid=p_last+Q;                     //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����

    /*
     *  �������˲��������Ҫ��ʽ
     */
    kg=p_mid/(p_mid+R);                 //kgΪkalman filter��R Ϊ����
    x_now=x_mid+kg*(ResrcData-x_mid);   //���Ƴ�������ֵ
    p_now=(1-kg)*p_mid;                 //����ֵ��Ӧ��covariance
    p_last = p_now;                     //����covariance ֵ
    x_last = x_now;                     //����ϵͳ״ֵ̬

    return x_now;

}

#endif

















/*************************************************ADCģ��ת��***********************************************************/
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ����������
		GPIO_Init(GPIOx, &GPIO_InitStructure); 
	
		//ʹ��ADC1ͨ��ʱ��
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);   
		//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
		ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
		ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
			
		ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC
		ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  	 
		while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼���� 
		ADC_StartCalibration(ADC1);	//����ADУ׼
		while(ADC_GetCalibrationStatus(ADC1));	//�ȴ�У׼���� 		 
 }

float ReadADC_Volt(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)   
{
	uint8_t ch,t,times=10;   //ÿ�ζ�ȡ10�����ݣ�����ƽ��ֵ
	float adsum=0;
	
	OSSchedLock();             //�����л�����
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
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	 	
  for(t=0;t<times;t++)	
	{	
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//ʹ��ָ����ADC1�����ת����������
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
		adsum=adsum+ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
	}
	adsum=(float)(adsum/times); //ȡƽ��ֵ
	adsum=(float)(adsum*3.3/4095);
	OSSchedUnlock();      //�����л�����
	return adsum;
} 

uint16_t ReadADC_Hex(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)   
{
   return (uint16_t)((ReadADC_Volt(GPIOx,GPIO_Pin_x)/3.3)*4095);
}

#endif



















/*******************************************DACģ��*******************************************************/
#if (USE_DAC == 1)
void DAC_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)	//DAC��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_x;//DAC_1
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ��������
	GPIO_Init(GPIOx,&GPIO_InitStructure);
	GPIO_SetBits(GPIOx,GPIO_Pin_x);//�����
	
	DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;//��ʹ�ô�������
	DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�����ǲ�
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0; //���� ��ֵ����
	DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable; //�رջ���
	if(GPIO_Pin_x==GPIO_Pin_4)
	{
		DAC_Init(DAC_Channel_1,&DAC_InitStructure);//��ʼ��DACͨ��1
		DAC_Cmd(DAC_Channel_1,ENABLE);//ʹ��DAC1
		DAC_SetChannel1Data(DAC_Align_12b_R,0);//12λ �Ҷ��� д0����
	}
	else 
	if(GPIO_Pin_x==GPIO_Pin_5)
	{
		DAC_Init(DAC_Channel_2,&DAC_InitStructure);//��ʼ��DACͨ��1
		DAC_Cmd(DAC_Channel_2,ENABLE);//ʹ��DAC1
		DAC_SetChannel2Data(DAC_Align_12b_R,0);//12λ �Ҷ��� д0����
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















/***************************************�ڲ�Flash��д**************************************************/
#if (USE_FLASHWR == 1)
/*--------------------------------Flash-------------------------------------*/
#define FLASH_ADDR_FLAG           0x08010000     //FLASH��ַ - ��־λ
#define FLASH_ADDR_DATA           0x08010002     //FLASH��ַ - ����
#define LENGTH_DATA               10             //���ݳ���
#define FLASH_PAGE_SIZE           ((uint16_t)0x800)        //ҳ��С   - 2K
#define FLASH_TYPE_LENGTH         ((uint16_t)0x002)        //���ʹ�С - 2�ֽ�
#define FLASH_PAGE_LENGTH         (FLASH_PAGE_SIZE/FLASH_TYPE_LENGTH)
#define FLAG_OK                   0x00
#define FLAG_NOOK                 0x01

//uint8_t WriteBuf[LENGTH_DATA];                   //д�������
//uint8_t ReadBuf[LENGTH_DATA];                    //����������
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

void FLASH_WriteNWord(uint16_t* pBuffer, uint32_t WriteAddr, uint16_t nWord);
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord);
/*------------------------------���Ͷ���-------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/*---------------------------------------------------------------------------*/
/************************************************
�������� �� FLASH_PageWrite
��    �� �� дһҳFLASH
��    �� �� pBuffer ----- ���ݻ�����
            WriteAddr --- д��ַ
�� �� ֵ �� ��
*************************************************/
void FLASH_PageWrite(uint16_t *pBuffer, uint32_t WriteAddr) 
{
  uint16_t cnt_tmp;
	OSSchedLock();             //�����л����� 
  for(cnt_tmp=0; (cnt_tmp<FLASH_PAGE_LENGTH) && (FLASHStatus == FLASH_COMPLETE); cnt_tmp++)
  {
    FLASHStatus = FLASH_ProgramHalfWord(WriteAddr, *pBuffer);
    WriteAddr += 2;
    pBuffer++;
  }
	OSSchedUnlock();      //�����л�����
}

/************************************************
�������� �� FLASH_WriteNWord
��    �� �� дN�ֽ�
��    �� �� pBuffer ----- ���ݻ�����
            WriteAddr --- д��ַ
            nWord ------- ����
�� �� ֵ �� ��
*************************************************/
void FLASH_WriteNWord(uint16_t *pBuffer, uint32_t WriteAddr, uint16_t nWord) //(�û��ɵ���)
{
  static uint16_t buf_tmp[FLASH_PAGE_LENGTH];
  uint32_t NbrOfPage = 0x00;                     //ҳ������д��

  uint32_t length_beyond_start;                  //��ʼҳ�����ĳ���(����)
  uint32_t length_remain_start;                  //��ʼҳʣ��ĳ���(����)

  uint32_t addr_first_page;                      //��һҳ(��ʼ)��ַ
  uint32_t addr_last_page;                       //���ҳ(��ʼ)��ַ
  uint16_t *pbuf_tmp;                            //bufָ��
  uint16_t cnt_length;                           //���� - ����
  uint16_t cnt_page;                             //���� - ҳ��
  uint32_t prog_addr_start;                      //��̵�ַ
  uint32_t length_beyond_last;                   //���ҳ�����ĳ���(����)
  uint8_t  flag_last_page_fill;                  //���һҳװ����־

  OSSchedLock();             //�����л����� 
  length_beyond_start = ((WriteAddr % FLASH_PAGE_SIZE) / FLASH_TYPE_LENGTH);
  length_remain_start = FLASH_PAGE_LENGTH - length_beyond_start;
  addr_first_page     = WriteAddr - (WriteAddr % FLASH_PAGE_SIZE);

  /* ����(д����)��ҳ�� */
  if(0 == (length_beyond_start + nWord)%FLASH_PAGE_LENGTH)
  {
    flag_last_page_fill = FLAG_OK;               //���һҳ�պ�
    NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH;
  }
  else
  {
    flag_last_page_fill = FLAG_NOOK;             //��������ҳ
    NbrOfPage = (length_beyond_start + nWord) / FLASH_PAGE_LENGTH + 1;
  }

  /* ���� */
  FLASH_UnlockBank1();

  /* �����־λ */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

  /* ������һҳ */
  FLASH_ReadNWord(buf_tmp, addr_first_page, FLASH_PAGE_LENGTH);
  FLASHStatus = FLASH_ErasePage(addr_first_page);
  /* ֻ��1ҳ */
  if(1 == NbrOfPage)
  {
    pbuf_tmp = pBuffer;                          //���Ƶ�ַ(ָ��)
    for(cnt_length=length_beyond_start; cnt_length<(length_beyond_start + nWord); cnt_length++)
    {
      buf_tmp[cnt_length] = *pbuf_tmp;
      pbuf_tmp++;
    }
    FLASH_PageWrite(buf_tmp, addr_first_page);
  }
  /* ����1ҳ */
  else
  {
    /* ��һҳ */
    pbuf_tmp = pBuffer;
    for(cnt_length=length_beyond_start; cnt_length<FLASH_PAGE_LENGTH; cnt_length++)
    {
      buf_tmp[cnt_length] = *pbuf_tmp;
      pbuf_tmp++;
    }
    FLASH_PageWrite(buf_tmp, addr_first_page);

    /* ���һҳ�պ�װ�������ö�ȡ���һҳ���� */
    if(FLAG_OK == flag_last_page_fill)
    {
      for(cnt_page=1; (cnt_page<NbrOfPage)  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
      {                                          //�����̵�ַΪ�ֽڵ�ַ(��FLASH_PAGE_SIZE)
        prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
        FLASHStatus = FLASH_ErasePage(prog_addr_start);
                                                 //(cnt_page - 1):Ϊ��һҳ��ַ
        FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
      }
    }
    else
    {
      /* �м�ҳ */
      for(cnt_page=1; (cnt_page<(NbrOfPage - 1))  && (FLASHStatus == FLASH_COMPLETE); cnt_page++)
      {                                          //�����̵�ַΪ�ֽڵ�ַ(��FLASH_PAGE_SIZE)
        prog_addr_start = addr_first_page + cnt_page*FLASH_PAGE_SIZE;
        FLASHStatus = FLASH_ErasePage(prog_addr_start);
                                                 //(cnt_page - 1):Ϊ��һҳ��ַ
        FLASH_PageWrite((pBuffer + length_remain_start + (cnt_page - 1)*FLASH_PAGE_LENGTH), prog_addr_start);
      }

      /* ���һҳ */
      addr_last_page = addr_first_page + (NbrOfPage - 1)*FLASH_PAGE_SIZE;

      FLASH_ReadNWord(buf_tmp, addr_last_page, FLASH_PAGE_LENGTH);
      FLASHStatus = FLASH_ErasePage(addr_last_page);
                                                 //NbrOfPage - 2: ��ҳ + ���һҳ ����ҳ(-2)
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
	OSSchedUnlock();      //�����л�����
}

/************************************************
�������� �� FLASH_ReadNWord
��    �� �� ��N�ֽ�
��    �� �� pBuffer ----- ���ݻ�����
            ReadAddr ---- ����ַ
            nWord ------- ����
�� �� ֵ �� ��
*************************************************/
void FLASH_ReadNWord(uint16_t* pBuffer, uint32_t ReadAddr, uint16_t nWord) //(�û��ɵ���)
{
	OSSchedLock();             //�����л����� 
  while(nWord--)
  {
    *pBuffer = (*(__IO uint16_t*)ReadAddr);
    ReadAddr += 2;
    pBuffer++;
  }
	OSSchedUnlock();      //�����л�����
}
#endif












/*************************************************�����Ź�***********************************************************/
/**
 * ��ʼ���������Ź���ʱ�ӣ����40KHZ��
 * prer:��Ƶ��:0~7(ֻ�е� 3 λ��Ч!)
 * ��Ƶ����=4*2^prer.�����ֵֻ���� 256!
 * rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч.
 * ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).
 */
#if (USE_IWDG == 1)
void IWDG_Config(uint16_t nms) //��λ��ms
{
	uint8_t i=0,prersum=1;
  uint8_t prer=4;       //Ĭ��ʹ��ʱ��: 40KHz/(4*2^4) = 625Hz =1.6ms
	uint16_t rlr;     
	while(i<2+prer) { prersum*=2; i++;}
	rlr=nms*40/prersum;
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����*/
	IWDG_SetPrescaler(prer);    /*����IWDGԤ��Ƶֵ:����IWDGԤ��Ƶֵ*/
	IWDG_SetReload(rlr);     /*����IWDG��װ��ֵ*/
	IWDG_ReloadCounter();    /*����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������*/
	IWDG_Enable();        /*ʹ��IWDG*/
}
void IWDG_Feed(void)  //�������Ź� - ι��
{
  IWDG_ReloadCounter(); 
}
#endif












/*************************************************BKP���籣��***********************************************************/
/*****************************************************************
*�ϵ���
*EXTI_InitStructure.EXTI_Trigger�ĸ�ֵ��ѡ� 
*       EXTI_Trigger_Rising---��ʾ��ѹ�Ӹ��½��������趨��ֵʱ�����жϣ� 
*       EXTI_Trigger_Falling---��ʾ��ѹ�ӵ������������趨��ֵʱ�����жϣ� 
*       EXTI_Trigger_Rising_Falling---��ʾ��ѹ�������½�Խ���趨��ֵʱ�������жϡ� 
*
*��������ѹ�����趨��ֵʱ��������PVD�жϣ����жϳ����н�����Ӧ�Ĵ��� 
*��ѹ��ֵ�� (2.2V-2.9V)

            2.2V   PWR_PVDLevel_2V2                
            2.3V   PWR_PVDLevel_2V3                  
            ...    ...       
            2.9V   PWR_PVDLevel_2V9       
                   IS_PWR_PVD_LEVEL(LEVEL)
******************************************************************/
#if (USE_BKP == 1)
void BKP_Config(float voltage)  //(�û��ɵ���)
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
  PWR_PVDLevelConfig(VolLimit); // �趨��ط�ֵ 
  PWR_PVDCmd(ENABLE); // ʹ��PVD  
  EXTI_StructInit(&EXTI_InitStructure);  
  EXTI_InitStructure.EXTI_Line = EXTI_Line16; // PVD���ӵ��ж���16��  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //ʹ���ж�ģʽ  
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//��ѹ���ڷ�ֵʱ�����ж�  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; // ʹ���ж���  
  EXTI_Init(&EXTI_InitStructure); // ��ʼ��  
}

#endif






















/*****************************************STM32�жϺ���********************************************

*��ʾ���жϴ�������������������У�Ȼ��ת�Ƶ��˴���Ӧ���жϺ������У�������˴��ĺ����᷵��
       ����ϵ����񴦼������У�����Ϊ�����񲻱����̫�ã������жϺ�����������ݾ�����Ҫʹ����
       ʱ(������Զ࣬��������Ҫʹ����ʱ)
*˵����

*********************************************************************************************/
void TIM1_IRQHandler(void)	  //��ʱ��1�жϺ��� (�߼���ʱ��)  //��ʱʱ�䵽�����ж�
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	#if (USE_PWM == 0)          //ʹ��PWMʱ��������ͨ��ʱ��Ч
	// �û���Ӷ�ʱ��1���жϴ��� 
	/*************************/



	/**************************/	
	#endif
}
void TIM2_IRQHandler(void)	  //��ʱ��2�жϺ��� (��׼(ͨ��)��ʱ��)  //��ʱʱ�䵽�����ж�
{
#if (USE_FREQMETER==1)        
	FreqMeterTIM2_IRQHandler();
#else                         // ʹ��Ƶ�ʼ�ʱ, ������ͨ��ʱ��Ч
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	// �û���Ӷ�ʱ��2���жϴ��� 
	/*************************/
   


	/**************************/	
#endif
}
void TIM3_IRQHandler(void)	  //��ʱ��3�жϺ��� (��׼(ͨ��)��ʱ��)  //��ʱʱ�䵽�����ж�
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	// �û���Ӷ�ʱ��3���жϴ��� 
	/*************************/

  

	/**************************/	
}
void TIM4_IRQHandler(void)	  //��ʱ��4�жϺ��� (��׼(ͨ��)��ʱ��)  //��ʱʱ�䵽�����ж�
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	// �û���Ӷ�ʱ��4���жϴ��� 
	/*************************/

  

	/**************************/	
}

void EXTI0_IRQHandler(void)	   //�ⲿ�жϺ���                    �ж�����0   //�ⲿ����0��⵽������ƽʱ�����жϣ���������ͬ��
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET) //����жϱ�־λ�Ƿ���1
	{
   	EXTI_ClearITPendingBit(EXTI_Line0);//���EXTI��·����λ
		// �û��������0���жϴ��� 
		/*************************/



		/**************************/	
	}		
}
void EXTI1_IRQHandler(void)	   //�ⲿ�жϺ���                    �ж�����1  
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET) //����жϱ�־λ�Ƿ���1
	{
   	EXTI_ClearITPendingBit(EXTI_Line1);//���EXTI��·����λ
		// �û��������1���жϴ��� 
		/*************************/



		/**************************/	
	}		
}
void EXTI2_IRQHandler(void)	   //�ⲿ�жϺ���                     �ж�����2   
{
	if(EXTI_GetITStatus(EXTI_Line2)==SET) //����жϱ�־λ�Ƿ���1
	{
   	EXTI_ClearITPendingBit(EXTI_Line2);//���EXTI��·����λ
		// �û��������2���жϴ��� 
		/*************************/



		/**************************/	
	}		
}
void EXTI3_IRQHandler(void)	   //�ⲿ�жϺ���                     �ж�����3   
{
	if(EXTI_GetITStatus(EXTI_Line3)==SET) //����жϱ�־λ�Ƿ���1
	{
   	EXTI_ClearITPendingBit(EXTI_Line3);//���EXTI��·����λ
		// �û��������3���жϴ��� 
		/*************************/



		/**************************/		
	}		
}
//void EXTI4_IRQHandler(void)	   //�ⲿ�жϺ���                     �ж�����4   
//{
//	if(EXTI_GetITStatus(EXTI_Line4)==SET) //����жϱ�־λ�Ƿ���1
//	{
//   	EXTI_ClearITPendingBit(EXTI_Line4);//���EXTI��·����λ
//		// �û��������4���жϴ��� 
//		/*************************/



//		/**************************/		
//	}		
//}
void EXTI9_5_IRQHandler(void)	   //�ⲿ�жϺ���   ����5~9   
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET) //����жϱ�־λ�Ƿ���1   �ж�����5   
	{
   	EXTI_ClearITPendingBit(EXTI_Line5);//���EXTI��·����λ
		// �û��������5���жϴ��� 
		/*************************/

		

		/**************************/	
	}	

	if(EXTI_GetITStatus(EXTI_Line6)==SET) //����жϱ�־λ�Ƿ���1    �ж�����6   
	{
   	EXTI_ClearITPendingBit(EXTI_Line6);//���EXTI��·����λ
		// �û��������6���жϴ��� 
		/*************************/



		/**************************/		
	}	
 
	if(EXTI_GetITStatus(EXTI_Line7)==SET) //����жϱ�־λ�Ƿ���1    �ж�����7   
	{
   	EXTI_ClearITPendingBit(EXTI_Line7);//���EXTI��·����λ
		// �û��������7���жϴ��� 
		/*************************/



		/**************************/		
	}	

	if(EXTI_GetITStatus(EXTI_Line8)==SET) //����жϱ�־λ�Ƿ���1    �ж�����8   
	{
   	EXTI_ClearITPendingBit(EXTI_Line8);//���EXTI��·����λ
		// �û��������8���жϴ��� 
		/*************************/



		/**************************/			
	}	

	if(EXTI_GetITStatus(EXTI_Line9)==SET) //����жϱ�־λ�Ƿ���1    �ж�����9   
	{
   	EXTI_ClearITPendingBit(EXTI_Line9);//���EXTI��·����λ
		// �û��������9���жϴ��� 
		/*************************/



		/**************************/				
	}		
}
void EXTI15_10_IRQHandler(void)	   //�ⲿ�жϺ���   ����10~15 
{ 
	if(EXTI_GetITStatus(EXTI_Line10)==SET) //����жϱ�־λ�Ƿ���1   �ж�����10   
	{
   	EXTI_ClearITPendingBit(EXTI_Line10);//���EXTI��·����λ
		// �û��������10���жϴ��� 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line11)==SET) //����жϱ�־λ�Ƿ���1   �ж�����11   
	{
   	EXTI_ClearITPendingBit(EXTI_Line11);//���EXTI��·����λ
		// �û��������11���жϴ��� 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line12)==SET) //����жϱ�־λ�Ƿ���1   �ж�����12   
	{
   	EXTI_ClearITPendingBit(EXTI_Line12);//���EXTI��·����λ
		// �û��������12���жϴ��� 
		/*************************/



		/**************************/			
	}		
	
	if(EXTI_GetITStatus(EXTI_Line13)==SET) //����жϱ�־λ�Ƿ���1   �ж�����13   
	{
   	EXTI_ClearITPendingBit(EXTI_Line13);//���EXTI��·����λ
		// �û��������13���жϴ��� 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line14)==SET) //����жϱ�־λ�Ƿ���1   �ж�����14   
	{
   	EXTI_ClearITPendingBit(EXTI_Line14);//���EXTI��·����λ
		// �û��������14���жϴ��� 
		/*************************/



		/**************************/				
	}		
	
	if(EXTI_GetITStatus(EXTI_Line15)==SET) //����жϱ�־λ�Ƿ���1   �ж�����15  
	{
   	EXTI_ClearITPendingBit(EXTI_Line15);//���EXTI��·����λ
		// �û��������15���жϴ��� 
		/*************************/



		/**************************/		
	}		
}

void USART1_IRQHandler(void)  //����1�ж�  PA9(TX) PA10(RX) //���յ����ֽ�ʱ�����ж�
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //STM32F10x�ط�װ��USART1ģ����ռ��
  STM32F10xPeripMode_Usart1RxCheck();    
	#endif
	
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //���ָ����USART�жϷ������
	{
    UsartRxdata= USART1_ReadByte();	//���ڽ���1�ֽ�����
		USART_ClearFlag(USART1, USART_FLAG_RXNE);    //��������жϱ�־
    USART_ClearITPendingBit(USART1, USART_IT_RXNE); //����Ϊ�Դ��Ľ��ջ���,��ʾ��˵��TXT����ʹ��˵��	
		
		#ifdef USE_USART  //STM32F10x�ط�װ��USART1ģ��
		#if (USE_USART == MODE_USE)	
		STM32F10xPeripMode_Usart1_UsartReceiveByte(UsartRxdata);
		#endif
		#endif
		
#if (printf_use_usartx == 1)		
		//ϵͳָ���//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //ʹ�ô���1��Ϊϵͳָ��(���ڴ���1�ж����ʹ�ô���1�����������ľ�ʹ�������Ĵ���)
    #endif
#endif		
		// �û���Ӵ���1�����жϴ��� 
		/*************************/	


		/**************************/	    	
	}
}
void USART2_IRQHandler(void)  //����2�ж�  PA2(TX) PA3(RX) //���յ����ֽ�ʱ�����ж�
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //�ط�װ��USART2ģ����ռ��
  STM32F10xPeripMode_Usart2RxCheck();   
	#endif
	
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //���ָ����USART�жϷ������
	{       
		UsartRxdata= USART2_ReadByte();	//���ڽ���1�ֽ�����
		USART_ClearFlag(USART2, USART_FLAG_RXNE);    //��������жϱ�־
    USART_ClearITPendingBit(USART2, USART_IT_RXNE); //����Ϊ�Դ��Ľ��ջ���,��ʾ��˵��TXT����ʹ��˵��
		
		#ifdef USE_USART //STM32F10x�ط�װ��USART2ģ��
		#if (USE_USART == MODE_USE)	
		STM32F10xPeripMode_Usart2_UsartReceiveByte(UsartRxdata);
		#endif
		#endif	
		
		#ifdef USE_MODEBUS //STM32F10x�ط�װ��ModeBusģ��
		#if (USE_MODEBUS == MODE_USE)
		STM32F10xPeripMode_ModeBusRTU_UsartReceiveByte(UsartRxdata);	
		#endif	
		#endif
		
#if (printf_use_usartx == 2)		
		//ϵͳָ���//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //ʹ�ô���1��Ϊϵͳָ��(���ڴ���1�ж����ʹ�ô���1�����������ľ�ʹ�������Ĵ���)
    #endif
#endif
		// �û���Ӵ���2�����жϴ��� 
		/*************************/


		/**************************/	
	}
}
void USART3_IRQHandler(void)  // ����3�ж�  PB10(TX) PB11(RX) //���յ����ֽ�ʱ�����ж�
{
	uint8_t UsartRxdata;
	
	#ifdef USE_USART  //STM32F10x�ط�װ��USART3ģ����ռ��
  STM32F10xPeripMode_Usart3RxCheck(); 
  #endif  
	
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{    
    UsartRxdata= USART3_ReadByte();	//���ڽ���1�ֽ�����		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);   
    USART_ClearITPendingBit(USART3, USART_IT_RXNE); //����Ϊ�Դ��Ľ��ջ���,��ʾ��˵��TXT����ʹ��˵��
		
		#ifdef USE_USART
		#if (USE_USART == MODE_USE)	 //STM32F10x�ط�װ��USART3ģ��
		STM32F10xPeripMode_Usart3_UsartReceiveByte(UsartRxdata);
		#endif
		#endif
		
#if (printf_use_usartx == 3)		
		//ϵͳָ���//
    #if (OS_CMD_ALL_ENABLE == 1)		
		OSCmdUsartReceiveByte(UsartRxdata); //ʹ�ô���1��Ϊϵͳָ��(���ڴ���1�ж����ʹ�ô���1�����������ľ�ʹ�������Ĵ���)
    #endif
#endif		
		// �û���Ӵ���3�����жϴ��� 
		/*************************/



		/**************************/	
	}
}


/*****************************************************************
*PVD�ж�,�����жϺ���
*����Ҫʹ�����������ʹ�������BKP_Config();����жϼ�����Ч
*****************************************************************/
void PVD_IRQHandler(void)  
{ 
  EXTI_ClearITPendingBit(EXTI_Line16); 
   // �û���ӵ�������������ݴ��� 
	/*************************/
	
	
	
	/**************************/
}
/************************************************************************************/


#endif


