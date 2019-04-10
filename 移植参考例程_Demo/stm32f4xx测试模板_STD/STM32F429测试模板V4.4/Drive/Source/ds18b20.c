#include "ds18b20.h"

struct DS18B20_SORT 
{
	uint8_t NUM;
	GPIO_TypeDef* GPIO[DS18B20_NUM];
  uint16_t      Pin[DS18B20_NUM];
}DS18B20_Sort={0,0,0};

GPIO_TypeDef* DS18B20_PORT_GPIO;
uint16_t      DS18B20_PORT_Pin;

DS18B20 Ds18b20;
void DS18B20_GPIO_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);
void DS18B20_Mode_IPU(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);
void DS18B20_Mode_Out(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);
void DS18B20_Write_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,u8 dat);//写入一个字节
u8 DS18B20_Read_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//读出一个字节
u8 DS18B20_Read_Bit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//读出一个位
u8 DS18B20_Answer_Check(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//检测是否存在DS18B20
void DS18B20_Rst(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//复位DS18B20  
extern void delay_us(unsigned int nus);
	
/*
 * 函数名：DS18B20_GPIO_Config
 * 描述  ：配置DS18B20用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
 void DS18B20_GPIO_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	DS18B20_Sort.GPIO[DS18B20_Sort.NUM]=GPIOx;
	DS18B20_Sort.Pin[DS18B20_Sort.NUM]=GPIO_Pin_x;
	DS18B20_Sort.NUM++;
	DS18B20_PORT_GPIO=GPIOx;
	DS18B20_PORT_Pin=GPIO_Pin_x;
	if(GPIOx==GPIOA)      RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	 //使能PORTA口时钟  
	else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 //使能PORTB口时钟  
	else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	 //使能PORTC口时钟  
	else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD, ENABLE );	 //使能PORTD口时钟  
	else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOE, ENABLE );	 //使能PORTE口时钟  
	else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOF, ENABLE );	 //使能PORTF口时钟  
	else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	 //使能PORTG口时钟  
 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

	GPIO_SetBits(GPIOx,GPIO_Pin_x); //引脚输出高
}
/*
 * 函数名：DS18B20_Mode_IPU
 * 描述  ：使DS18B20-DATA引脚变为输入模式
 * 输入  ：无
 * 输出  ：无
 */
void DS18B20_Mode_IPU(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输入
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
/*
 * 函数名：DS18B20_Mode_Out
 * 描述  ：使DS18B20-DATA引脚变为输出模式
 * 输入  ：无
 * 输出  ：无
 */
void DS18B20_Mode_Out(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

}



/*
 *主机给从机发送复位脉冲
 */
void DS18B20_Rst(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)	   
{        
    /* IO设置为推挽输出*/	
	  DS18B20_Mode_Out(GPIOx,GPIO_Pin_x); 
	  /*产生至少480us的低电平复位信号 */
    GPIO_ResetBits(GPIOx,GPIO_Pin_x);  
		delay_us(480);  
    /* 在产生复位信号后，需将总线拉高 */	
    GPIO_SetBits(GPIOx,GPIO_Pin_x); 
	  delay_us(15);    
}

/*
 * 检测从机给主机返回的应答脉冲
 *从机接收到主机的复位信号后，会在15~60us后给主机发一个应答脉冲
 * 0：成功
 * 1：失败
 */
u8 DS18B20_Answer_Check(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x) 	   
{   
	u8 delay=0;
	/* 主机设置为上拉输入 */
	DS18B20_Mode_IPU(GPIOx,GPIO_Pin_x); 
	/* 等待应答脉冲（一个60~240us的低电平信号 ）的到来
	 * 如果100us内，没有应答脉冲，退出函数，注意：从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x)&&delay<100)
	{
		delay++;
		delay_us(1);
	}	 
	/*经过100us后，如果没有应答脉冲，退出函数*/	
	if(delay>=100)
		return 1;
	else 
		delay=0;
	/*有应答脉冲，且存在时间不超过240us */
	while (!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x)&&delay<240)
	{
		delay++;
		delay_us(1);
	}
	if(delay>=240)
		return 1;	    
	  return 0;
}

//从DS18B20读取一个位
//返回值：1/0
u8 DS18B20_Read_Bit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x) 			 // read one bit
{
  u8 data;
	DS18B20_Mode_Out(GPIOx,GPIO_Pin_x);
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
  GPIO_ResetBits(GPIOx,GPIO_Pin_x); 
	delay_us(2);
	GPIO_SetBits(GPIOx,GPIO_Pin_x); 
	delay_us(12);
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	DS18B20_Mode_IPU(GPIOx,GPIO_Pin_x);

	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x))
		data=1;
  else 
		data=0;	 
  delay_us(50);           
  return data;
}
//从DS18B20读取一个字节
//返回值：读到的数据
u8 DS18B20_Read_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit(GPIOx,GPIO_Pin_x);		
		dat = (dat) | (j<<i);
	}					    
    return dat;
}
/*
 * 写一个字节到DS18B20
 */
void DS18B20_Write_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,u8 dat)     
{             
  u8 j;
  u8 testb;
	DS18B20_Mode_Out(GPIOx,GPIO_Pin_x);//SET PA0 OUTPUT;
   for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            GPIO_ResetBits(GPIOx,GPIO_Pin_x);// Write 1
            delay_us(10);                            
            GPIO_SetBits(GPIOx,GPIO_Pin_x);
            delay_us(50);             
        }
        else 
        {
            GPIO_ResetBits(GPIOx,GPIO_Pin_x);// Write 0
            delay_us(60);             
            GPIO_SetBits(GPIOx,GPIO_Pin_x);   ///释放总线
            delay_us(2);                          
        }
    }
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	u8 i=0;
	
	while(i<DS18B20_NUM)
	{
		Ds18b20.ValMax[i]=-300; //不可修改
		Ds18b20.ValMin[i]=1000; //不可修改
		Ds18b20.ValMaxWarn[i]=WarnTemperatureMax;
		Ds18b20.ValMinWarn[i]=WarnTemperatureMin;
		i++;
	}
  DS18B20_GPIO_Config(GPIOx, GPIO_Pin_x);
	DS18B20_Rst(GPIOx,GPIO_Pin_x);
	return DS18B20_Answer_Check(GPIOx,GPIO_Pin_x);
}  
//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
float DS18B20_Update(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{   
    u8 TL,TH,i=0,j=0,Dnum=0;
	
	  short Temperature;
	  float Temperature1;
	  while(i<DS18B20_NUM)
		{
	    if(GPIOx==DS18B20_Sort.GPIO[i]){
			j=0;
			while(j<DS18B20_NUM){
			if(GPIO_Pin_x==DS18B20_Sort.Pin[j]){
		  if(i==j) {
			Dnum=i;
			break; }}
			j++;}
			i++;}
		}
    DS18B20_Rst(GPIOx,GPIO_Pin_x);	   
	  DS18B20_Answer_Check(GPIOx,GPIO_Pin_x);	 
    DS18B20_Write_Byte(GPIOx,GPIO_Pin_x,0xcc);// skip rom
    DS18B20_Write_Byte(GPIOx,GPIO_Pin_x,0x44);// convert                 // ds1820 start convert
    DS18B20_Rst(GPIOx,GPIO_Pin_x);
    DS18B20_Answer_Check(GPIOx,GPIO_Pin_x);	 
    DS18B20_Write_Byte(GPIOx,GPIO_Pin_x,0xcc);// skip rom
    DS18B20_Write_Byte(GPIOx,GPIO_Pin_x,0xbe);// convert	    
    TL=DS18B20_Read_Byte(GPIOx,GPIO_Pin_x); // LSB   
    TH=DS18B20_Read_Byte(GPIOx,GPIO_Pin_x); // MSB  
		if( TH&0xfc)
		{
			Temperature=(TH<<8)|TL;
			Temperature1=(~ Temperature)+1;
			Temperature1*=0.0625;
		}
		else
		{
			Temperature1=((TH<<8)|TL)*0.0625;
		}
		if((Temperature1>-50)&&(Temperature1<150)) Ds18b20.Val[Dnum]=Temperature1;
		if(Ds18b20.Val[Dnum]>Ds18b20.ValMax[Dnum]) Ds18b20.ValMax[Dnum]=Ds18b20.Val[Dnum];
		if(Ds18b20.Val[Dnum]<Ds18b20.ValMin[Dnum]) Ds18b20.ValMin[Dnum]=Ds18b20.Val[Dnum];
		return Temperature1; 	
} 

