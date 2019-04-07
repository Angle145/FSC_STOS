#ifndef __DS3231_H_
#define __DS3231_H_

#include "i2c_gpio.h"
#include "stm32f10x.h"
#include "fsc_stos.h"

#define DS3231_WriteAddress 0xD0    //����д��ַ 
#define DS3231_ReadAddress  0xD1    //��������ַ
#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��
//����1            
#define DS3231_SALARM1ECOND 0x07    //��
#define DS3231_ALARM1MINUTE 0x08    //��
#define DS3231_ALARM1HOUR   0x09    //ʱ
#define DS3231_ALARM1WEEK   0x0A    //����/��
//����2
#define DS3231_ALARM2MINUTE 0x0b    //��
#define DS3231_ALARM2HOUR   0x0c    //ʱ
#define DS3231_ALARM2WEEK   0x0d    //����/��
#define DS3231_CONTROL      0x0e    //���ƼĴ���
#define DS3231_STATUS       0x0f    //״̬�Ĵ���
#define BSY                 2       //æ
#define OSF                 7       //����ֹͣ��־
#define DS3231_XTAL         0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ) 

#define SECOND DS3231_SECOND        //��
#define MINUTE DS3231_MINUTE        //��
#define HOUR DS3231_HOUR            //ʱ
#define WEEK DS3231_WEEK            //����
#define DAY DS3231_DAY              //��
#define MONTH DS3231_MONTH          //��
#define YEAR DS3231_YEAR            //��

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

extern SYSTEMTIME CurrentTime;	//��ǰʵʱʱ��ṹ��
extern u8 const mon_table[12];	//�·��������ݱ�

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
1.ʹ�����̣�

void Task2(void) //����2 (����ʱ������ˢ��)
{
	OSSchedLock();//���������л�
	DS3231_Config();                                       //��ʹ��DS3231����Ҫ���ڼ��DS3231�Ƿ�������
	DS3231_SetTime(2019,3,18,1,9,32,0);                    //���ó�ʹʱ��(��ICʱ����׼�ģ��ɲ�������)
	OSSchedUnlock();//���������л�
	while(1) 
	 {		
		  OSSchedLock();//���������л�
		  DS3231_Update(&CurrentTime); //����ʱ������(��DS3231��ȡ���ݲ����浽�ṹ��CurrentTime��)
		  OSSchedUnlock();//���������л�
		  OS_delayMs(100);//100msˢ��һ������	
	 }
}
void Task3(void) //����3 (����ʱ����ʾ)
{
	while(1) 
	 {	
     if(CurrentTime.SecCount!=CurrentTime.Second) //������ӱ仯��û�У���仯�˾͸�����ʾ����
			{ 
				CurrentTime.SecCount=CurrentTime.Second;
				OSprintf("%d ��%d ��%d ��\r\n",CurrentTime.Year+2000,CurrentTime.Month,CurrentTime.Day);
				OSprintf("%d ʱ%d ��%d ��\r\n",CurrentTime.Hour,CurrentTime.Minute,CurrentTime.Second);
				OSprintf("����%d\r\n\r\n",CurrentTime.Week);
			}			
		 OS_delayMs(1000); //1������ʾһ�����ݣ���ʾ���ʱ��Ҫ��������ˢ��ʱ��������壩	
	 }
}


*/

