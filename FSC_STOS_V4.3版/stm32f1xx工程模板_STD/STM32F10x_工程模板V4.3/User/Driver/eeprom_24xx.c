#include "eeprom_24xx.h"

/*
*********************************************************************************************************
*	�� �� ��: EEPROM_CheckOk
*	����˵��: �жϴ���EERPOM�Ƿ�����
*	��    ��:  ��
*	�� �� ֵ: 0 ��ʾ������ 1 ��ʾ������
*********************************************************************************************************
*/
uint8_t EEPROM_CheckOk(void)
{
	if(IIC_CheckDevice(EE_DEV_ADDR) == 1) //�쳣
	{
		IIC_Stop();/* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
		return 1;
	}
	else //����
	{
		return 0;
	}
}
void EEPROM_Config(void)
{
  if(EEPROM_CheckOk()==0)
	{
		OSprintf("Eeprom Device is OK!\r\n\r\n");
	}
	else
	{	
		OSprintf("Eeprom Device is Error!\r\n\r\n");  
	}
}

/*
*********************************************************************************************************
*	�� �� ��: EEPROM_ReadBytes
*	����˵��: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
*	��    ��:  _usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pReadBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t EEPROM_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i;

	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

	/* ��1��������I2C���������ź� */
	IIC_Start();

	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */

	/* ��3�����ȴ�ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	if (EE_ADDR_BYTES == 1)
	{
		IIC_Send_Byte((uint8_t)_usAddress);
		if (IIC_Wait_Ack() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}
	}
	else
	{
		IIC_Send_Byte(_usAddress >> 8);
		if (IIC_Wait_Ack() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		IIC_Send_Byte(_usAddress);
		if (IIC_Wait_Ack() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}
	}

	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC_Start();

	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC_Send_Byte(EE_DEV_ADDR | I2C_RD);	/* �˴��Ƕ�ָ�� */

	/* ��8��������ACK */
	if (IIC_Wait_Ack() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}

	/* ��9����ѭ����ȡ���� */
	for (i = 0; i < _usSize; i++)
	{
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if (i != _usSize - 1)
		{
			_pReadBuf[i] = IIC_Read_Byte(ACK);	/* ��1���ֽ� */	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			_pReadBuf[i] = IIC_Read_Byte(NACK);	/* ��1���ֽ� */	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: eeprom_WriteBytes
*	����˵��: ����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
*	��    ��:  _usAddress : ��ʼ��ַ
*			 _usSize : ���ݳ��ȣ���λΪ�ֽ�
*			 _pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
*	�� �� ֵ: 0 ��ʾʧ�ܣ�1��ʾ�ɹ�
*********************************************************************************************************
*/
uint8_t EEPROM_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;

	/*
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ÿд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/

	usAddr = _usAddress;
	for (i = 0; i < _usSize; i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			IIC_Stop();

			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms
				CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
			*/
			for (m = 0; m < 1000; m++)
			{
				/* ��1��������I2C���������ź� */
				IIC_Start();

				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				IIC_Send_Byte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */

				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if (IIC_Wait_Ack() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM����д��ʱ */
			}

			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			if (EE_ADDR_BYTES == 1)
			{
				IIC_Send_Byte((uint8_t)usAddr);
				if (IIC_Wait_Ack() != 0)
				{
					goto cmd_fail;	/* EEPROM������Ӧ�� */
				}
			}
			else
			{
				IIC_Send_Byte(usAddr >> 8);
				if (IIC_Wait_Ack() != 0)
				{
					goto cmd_fail;	/* EEPROM������Ӧ�� */
				}

				IIC_Send_Byte(usAddr);
				if (IIC_Wait_Ack() != 0)
				{
					goto cmd_fail;	/* EEPROM������Ӧ�� */
				}
			}
		}

		/* ��6������ʼд������ */
		IIC_Send_Byte(_pWriteBuf[i]);

		/* ��7��������ACK */
		if (IIC_Wait_Ack() != 0)
		{
			goto cmd_fail;	/* EEPROM������Ӧ�� */
		}

		usAddr++;	/* ��ַ��1 */
	}

	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	IIC_Stop();
	return 1;

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC_Stop();
	return 0;
}

