#ifndef _I2C_GPIO_H_
#define _I2C_GPIO_H_
#include "stm32f10x.h"
#include "fsc_stos.h"

/*******************IIC硬件接口**********************/
//IO方向
#define SDA_IN()   PB15_IN //SDA输入模式
#define SDA_OUT()  PB15_OUT//SDA输出模式
//IO操作
#define IIC_SDA    PB15    //SDA输出	 
#define READ_SDA   PB15in  //读取SDA

//IO方向
#define SCL_IN()   PB14_IN //SCL输入模式
#define SCL_OUT()  PB14_OUT//SCL输出模式
//IO操作
#define IIC_SCL    PB14    //SCL输出
#define READ_SCL   PB14in  //读取SCL

/***************************************************/

//应答
#define ACK 1    //主动应答
#define NACK 0   //不应答

//读写控制
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

//IIC所有操作函数
void IIC_Config(void);//初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
u8 IIC_CheckDevice(u8 _Address);//IIC设备检测,返回1-失败，0-正常 
#endif

