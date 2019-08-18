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
void DS18B20_Write_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x,u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//����һ���ֽ�
u8 DS18B20_Read_Bit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//����һ��λ
u8 DS18B20_Answer_Check(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//����Ƿ����DS18B20
void DS18B20_Rst(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x);//��λDS18B20  
extern void delay_us(unsigned int nus);
	
/*
 * ��������DS18B20_GPIO_Config
 * ����  ������DS18B20�õ���I/O��
 * ����  ����
 * ���  ����
 */
 void DS18B20_GPIO_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	DS18B20_Sort.GPIO[DS18B20_Sort.NUM]=GPIOx;
	DS18B20_Sort.Pin[DS18B20_Sort.NUM]=GPIO_Pin_x;
	DS18B20_Sort.NUM++;
	DS18B20_PORT_GPIO=GPIOx;
	DS18B20_PORT_Pin=GPIO_Pin_x;
	if(GPIOx==GPIOA)      RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	 //ʹ��PORTA��ʱ��  
	else if(GPIOx==GPIOB) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	 //ʹ��PORTB��ʱ��  
	else if(GPIOx==GPIOC) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOC, ENABLE );	 //ʹ��PORTC��ʱ��  
	else if(GPIOx==GPIOD) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOD, ENABLE );	 //ʹ��PORTD��ʱ��  
	else if(GPIOx==GPIOE) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOE, ENABLE );	 //ʹ��PORTE��ʱ��  
	else if(GPIOx==GPIOF) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOF, ENABLE );	 //ʹ��PORTF��ʱ��  
	else if(GPIOx==GPIOG) RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	 //ʹ��PORTG��ʱ��  
 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

	GPIO_SetBits(GPIOx,GPIO_Pin_x); //���������
}
/*
 * ��������DS18B20_Mode_IPU
 * ����  ��ʹDS18B20-DATA���ű�Ϊ����ģʽ
 * ����  ����
 * ���  ����
 */
void DS18B20_Mode_IPU(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //��������
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}
/*
 * ��������DS18B20_Mode_Out
 * ����  ��ʹDS18B20-DATA���ű�Ϊ���ģʽ
 * ����  ����
 * ���  ����
 */
void DS18B20_Mode_Out(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

}



/*
 *�������ӻ����͸�λ����
 */
void DS18B20_Rst(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)	   
{        
    /* IO����Ϊ�������*/	
	  DS18B20_Mode_Out(GPIOx,GPIO_Pin_x); 
	  /*��������480us�ĵ͵�ƽ��λ�ź� */
    GPIO_ResetBits(GPIOx,GPIO_Pin_x);  
		delay_us(480);  
    /* �ڲ�����λ�źź��轫�������� */	
    GPIO_SetBits(GPIOx,GPIO_Pin_x); 
	  delay_us(15);    
}

/*
 * ���ӻ����������ص�Ӧ������
 *�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ��Ӧ������
 * 0���ɹ�
 * 1��ʧ��
 */
u8 DS18B20_Answer_Check(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x) 	   
{   
	u8 delay=0;
	/* ��������Ϊ�������� */
	DS18B20_Mode_IPU(GPIOx,GPIO_Pin_x); 
	/* �ȴ�Ӧ�����壨һ��60~240us�ĵ͵�ƽ�ź� ���ĵ���
	 * ���100us�ڣ�û��Ӧ�����壬�˳�������ע�⣺�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x)&&delay<100)
	{
		delay++;
		delay_us(1);
	}	 
	/*����100us�����û��Ӧ�����壬�˳�����*/	
	if(delay>=100)
		return 1;
	else 
		delay=0;
	/*��Ӧ�����壬�Ҵ���ʱ�䲻����240us */
	while (!GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x)&&delay<240)
	{
		delay++;
		delay_us(1);
	}
	if(delay>=240)
		return 1;	    
	  return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x) 			 // read one bit
{
  u8 data;
	DS18B20_Mode_Out(GPIOx,GPIO_Pin_x);
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
  GPIO_ResetBits(GPIOx,GPIO_Pin_x); 
	delay_us(2);
	GPIO_SetBits(GPIOx,GPIO_Pin_x); 
	delay_us(12);
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU(GPIOx,GPIO_Pin_x);

	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin_x))
		data=1;
  else 
		data=0;	 
  delay_us(50);           
  return data;
}
//��DS18B20��ȡһ���ֽ�
//����ֵ������������
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
 * дһ���ֽڵ�DS18B20
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
            GPIO_SetBits(GPIOx,GPIO_Pin_x);   ///�ͷ�����
            delay_us(2);                          
        }
    }
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_x)
{
	u8 i=0;
	
	while(i<DS18B20_NUM)
	{
		Ds18b20.ValMax[i]=-300; //�����޸�
		Ds18b20.ValMin[i]=1000; //�����޸�
		Ds18b20.ValMaxWarn[i]=WarnTemperatureMax;
		Ds18b20.ValMinWarn[i]=WarnTemperatureMin;
		i++;
	}
  DS18B20_GPIO_Config(GPIOx, GPIO_Pin_x);
	DS18B20_Rst(GPIOx,GPIO_Pin_x);
	return DS18B20_Answer_Check(GPIOx,GPIO_Pin_x);
}  
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
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

