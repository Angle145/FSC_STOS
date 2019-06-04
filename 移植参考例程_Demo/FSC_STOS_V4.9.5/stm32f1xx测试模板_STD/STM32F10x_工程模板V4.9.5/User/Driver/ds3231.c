#include "ds3231.h" 
#include <stdio.h>  

extern void delay_us(uint32_t nus);//微秒延时函数
extern void delay_ms(uint32_t nms);//毫秒延时函数

SYSTEMTIME CurrentTime;	
u8 const mon_table[12];	//月份日期数据表


u8 BCD_HEX(u8 val)
{
    u8 i;
    i= val&0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

u16 HEX_BCD(u8 val)
{
  u8 i,j,k;
  i=val/10;
  j=val%10;
  k=j+(i<<4);
  return k;
}


void DS3231_I2cByteWrite(u8 addr,u8 bytedata)
{
  IIC_Start();
  delay_us(5);
  IIC_Send_Byte(DS3231_WriteAddress);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Send_Byte(addr);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Send_Byte(bytedata);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Stop();
}	


u8 DS3231_I2cByteRead(u8 addr)
{
  u8 Dat=0;
  
  IIC_Start();
  IIC_Send_Byte(DS3231_WriteAddress);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Send_Byte(addr);
  IIC_Wait_Ack();
  delay_us(5);
  IIC_Start();
  IIC_Send_Byte(DS3231_ReadAddress);
  IIC_Wait_Ack();
  delay_us(5);
  Dat=IIC_Read_Byte(ACK);
  IIC_Stop();
  return Dat;
} 

void DS3231_SetOneTime(u8 Address, u8 Value)
{
	u8 Timetemp;
	Timetemp=HEX_BCD(Value);
	DS3231_I2cByteWrite(Address,Timetemp); 
}

void DS3231_SetTime(u16 year,u8 month,u8 day,u8 week,u8 hour,u8 minute,u8 second)  // 设置时间函数
{
	year=(u8)(year-2000);
  DS3231_SetOneTime(YEAR,year);
	DS3231_SetOneTime(MONTH,month);
	DS3231_SetOneTime(DAY,day);
	DS3231_SetOneTime(WEEK,week);
	DS3231_SetOneTime(HOUR,hour);
	DS3231_SetOneTime(MINUTE,minute);
	DS3231_SetOneTime(SECOND,second);
}
u8 DS3231_Read(u8 TtypeAddr)   //获取某一类型的时间 如 Sencond Minute Hour
{
  u8 ucData;
	ucData=DS3231_I2cByteRead(TtypeAddr);
	ucData=BCD_HEX(ucData);
	return ucData;
}
u8 GetCurrentTime(u8 TtypeAddr)
{
	return (DS3231_Read(TtypeAddr));
}

void DS3231_Update(SYSTEMTIME *Time)
{
	Time->Second=DS3231_Read(DS3231_SECOND);
	Time->Minute=DS3231_Read(DS3231_MINUTE);
	Time->Hour=DS3231_Read(DS3231_HOUR)&0x3f;
	Time->Day=DS3231_Read(DS3231_DAY);
	Time->Week=DS3231_Read(DS3231_WEEK);
	Time->Month=DS3231_Read(DS3231_MONTH);
	Time->Year=DS3231_Read(DS3231_YEAR);
}
void DateToStr(SYSTEMTIME *Time)   //将日期信息转化为字符串 
{
	Time->DateString[0]= 2;
  Time->DateString[1]= 0;
  Time->DateString[2]= Time->Year/10 ;
	Time->DateString[3]= Time->Year%10 ;
	Time->DateString[4]= Time->Month/10;
	Time->DateString[5]= Time->Month%10;
	Time->DateString[6]= Time->Day/10;
	Time->DateString[7]= Time->Day%10;
}

void TimeToStr(SYSTEMTIME *Time)   //将时间信息转化为字符串 
{
	Time->TimeString[0] = Time->Hour/10;
	Time->TimeString[1] = Time->Hour%10;
	Time->TimeString[2] = Time->Minute/10;
	Time->TimeString[3] = Time->Minute%10;
	Time->TimeString[4] = Time->Second/10;
	Time->TimeString[5] = Time->Second%10;
	Time->TimeString[6] = Time->Week%10;
	Time->TimeString[7] = 10;

}

void DS3231_Config(void)
{
	if(IIC_CheckDevice(DS3231_WriteAddress)==1) 
	{
		IIC_Stop();
		OSprintf("DS3231 Device is Error!\r\n\r\n");
	}
	else 
	{
		OSprintf("DS3231 Device is OK!\r\n\r\n");
		DS3231_I2cByteWrite(0x0e,0);
		delay_ms(2); 
		DS3231_I2cByteWrite(0x0f,0x00);
		delay_ms(2); 
	}
}
