#ifndef _I2C_GPIO_H_
#define _I2C_GPIO_H_
#include "stm32f10x.h"
#include "fsc_stos.h"

/*******************IICӲ���ӿ�**********************/
//IO����
#define SDA_IN()   PB15_IN //SDA����ģʽ
#define SDA_OUT()  PB15_OUT//SDA���ģʽ
//IO����
#define IIC_SDA    PB15    //SDA���	 
#define READ_SDA   PB15in  //��ȡSDA

//IO����
#define SCL_IN()   PB14_IN //SCL����ģʽ
#define SCL_OUT()  PB14_OUT//SCL���ģʽ
//IO����
#define IIC_SCL    PB14    //SCL���
#define READ_SCL   PB14in  //��ȡSCL

/***************************************************/

//Ӧ��
#define ACK 1    //����Ӧ��
#define NACK 0   //��Ӧ��

//��д����
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

//IIC���в�������
void IIC_Config(void);//��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
u8 IIC_CheckDevice(u8 _Address);//IIC�豸���,����1-ʧ�ܣ�0-���� 
#endif

