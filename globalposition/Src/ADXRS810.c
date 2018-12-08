#include "ADXRS810.h"

/*********************************************************************************************************
**	����ԭ��:	u8 SPI_Read(u8* data,u8 bytesNumber)
**	��������:	������SPI���ݶ�ȡ����
**	�������:	*data: 			�����͵�����
				bytesNumber: 	�����������ֽ���
**	�������:	���ܵ�������data;
**	���ز���:	return 1:		���ճɹ�
				return 0:		����ʧ��
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint8_t SPI_Read(uint8_t *data,uint8_t bytesNumber)
{	
	uint8_t byte = 0;
	uint8_t writeBuffer[4]  = {0, 0, 0, 0};	

	for(byte = 0; byte < bytesNumber; byte++)
	{
		 writeBuffer[byte] = data[byte];
	}
	ADXRS810_CS_RESET;
    /*
	for(byte = 0; byte < bytesNumber; byte++)
	{
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		SPI_I2S_SendData(SPI3, writeBuffer[byte]); //ͨ������SPIx����һ������
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		data[byte] = SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����
	}*/
    HAL_SPI_Transmit(&hspi2, writeBuffer, 4, 1000);
    ADXRS810_CS_SET;
    ADXRS810_CS_RESET;
    HAL_SPI_Receive(&hspi2, data, 4, 1000);
    //uprintf("%d %d %d %d ",data[3],data[2],data[1],data[0]);
	ADXRS810_CS_SET;
	return bytesNumber;
}

/*********************************************************************************************************
**	����ԭ��:	u8 SPI_Write(u8* data,u8 bytesNumber)
**	��������:	������SPI���ݶ�ȡ����
**	�������:	*data: 			�����͵�����
				bytesNumber: 	�����������ֽ���
**	�������:	��
**	���ز���:	return 1:		���ͳɹ�
				return 0:		����ʧ��
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint8_t SPI_Write(uint8_t *data,uint8_t bytesNumber)
{		
	//uint8_t byte = 0;
	//uint8_t tempByte = 0;
	ADXRS810_CS_RESET;
    /*
	for(byte = 0; byte < bytesNumber; byte++)
	{
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		SPI_I2S_SendData(SPI3, data[byte]); 									//ͨ������SPIx����һ������
		while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		tempByte = SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����
	}*/
    HAL_SPI_Transmit(&hspi2, data, 4, 1000);
    if(HAL_SPI_Receive(&hspi2, data, 4, 1000) != HAL_OK)
    {
        uprintf("error\r\n");
    }
	ADXRS810_CS_SET;
	return bytesNumber;
}

/*********************************************************************************************************
**	����ԭ��:	u8 ADXRS810_ParityBit(u32 data)
**	��������:	���û������ż����λʹ��dataΪ��
**	�������:	����data
**	�������:	��
**	���ز���:	��ż����λ
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint8_t ADXRS810_ParityBit(uint32_t data)
{
	uint8_t parityBit = 0;
    uint8_t bitIndex  = 0;
    uint8_t sum       = 0;

    for(bitIndex = 0; bitIndex < 32; bitIndex++)
    {
        sum += ((data >> bitIndex) & 0x1);
    }
    if (! (sum % 2))
    {
        parityBit |= 0x1;
    }

    return parityBit;
}

/*********************************************************************************************************
**	����ԭ��:	u8 ADXRS810_Init(void)
**	��������:	�����ǳ�ʼ������
**	�������:	��
**	�������:	��
**	���ز���:	��ʼ��״̬
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint8_t ADXRS810_Init(void)
{
	uint8_t status  = 0x1;
	uint8_t data[4] = {0, 0, 0, 0};

	data[3] = 3;
	data[2] = 0;
    data[1] = 0;
	data[0] = 2;
	SPI_Write(data, 4);
	HAL_Delay(50);
	data[3] = 0;
	data[2] = 0;
    data[1] = 0;
	data[0] = 2;
	SPI_Write(data, 4);
	HAL_Delay(50);
	data[3] = 0;
	data[2] = 0;
    data[1] = 0;
	data[0] = 2;
	SPI_Write(data, 4);
    HAL_Delay(50);
	data[3] = 0;
	data[2] = 0;
    data[1] = 0;
	data[0] = 2;
	SPI_Write(data, 4);
    HAL_Delay(100);
    uint16_t PID = ADXRS810_GetRegisterValue(ADXRS810_PID1);
	if((PID >> 8) != 0x52)
	{
		status = 0x0;
	}
	
	return status;	
}

/*********************************************************************************************************
**	����ԭ��:	u32 ADXRS810_Data(void)
**	��������:	��ȡ�����ǵ�����
**	�������:	��
**	�������:	��
**	���ز���:	��ȡ������������
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
int16_t ADXRS810_Data(void)
{
	uint8_t data[4]      = {0, 0, 0, 0};
	uint32_t receivedData = 0x00;
	data[0] = ADXRS810_SENSOR_DATA;
	SPI_Read(data, 4);
	receivedData += ((uint32_t)data[0] << 24);
	receivedData += ((uint32_t)data[1] << 16);
	receivedData += (data[2] << 8);
	receivedData = ((receivedData >> 10) & 0xFFFF);
	
	return receivedData;	
}

/*********************************************************************************************************
**	����ԭ��:	u16 ADXRS810_GetRegisterValue(u8 regAddress)
**	��������:	��ȡ�����ǼĴ�����ֵ
**	�������:	��
**	�������:	��
**	���ز���:	��ȡ�������ǼĴ���ֵ
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint16_t ADXRS810_GetRegisterValue(uint8_t regAddress)
{
	uint8_t data[4]      = {0, 0, 0, 0};
	uint32_t receivedData = 0x00;	
	uint32_t dataToSend   = 0;
	
	data[0] = ADXRS810_READ_DATA | (regAddress >> 7);
	data[1] = regAddress << 1;
	dataToSend += ((uint32_t)data[0] << 24);
	dataToSend += ((uint32_t)data[1] << 16);
	data[3] = ADXRS810_ParityBit(dataToSend);
	SPI_Write(data, 4);
	SPI_Read(data, 4);									// because the device answers only on the next SPI word
	receivedData += ((uint32_t)data[1] << 16);
	receivedData += ((uint32_t)data[2] << 8);
	receivedData += (data[3] << 0);
	receivedData = ((receivedData >> 5) & 0xFFFF);
	
	return (uint16_t)receivedData;
}

/*********************************************************************************************************
**	����ԭ��:	void ADXRS810_SetRegisterValue(u8 regAddress, u16 regData)
**	��������:	���������ǼĴ�����ֵ
**	�������:	��
**	�������:	��
**	���ز���:	��
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
void ADXRS810_SetRegisterValue(uint8_t regAddress, uint16_t regData)
{
	uint8_t data[4]    = {0, 0, 0, 0};
	uint32_t dataToSend = 0;
	
	data[0] = ADXRS810_READ_DATA | (regAddress >> 7);
	data[1] = (regAddress << 1) | (regData >> 15);
	data[2] = (regData & 0x7F80) >> 7;
	data[3] = (regData & 0xFF) << 1;
	dataToSend += ((uint32_t)data[0] << 24);
	dataToSend += ((uint32_t)data[1] << 16);
	data[3] = ADXRS810_ParityBit(dataToSend);
	SPI_Write(data, 4);
}

/*********************************************************************************************************
**	����ԭ��:	u8 ADXRS810_GetTemperature(void)
**	��������:	���������ǵ��¶�
**	�������:	��
**	�������:	��
**	���ز���:	�¶�
**	�޸�ʱ��:	2014-5-9	10:45
*********************************************************************************************************/
uint8_t ADXRS810_GetTemperature(void)
{
	uint16_t temp = 0;
	
	temp = ADXRS810_GetRegisterValue(ADXRS810_TEMP1);
	temp >>= 6;	
	temp -= 0x31f;
	temp /= 5;
	
	return (uint8_t)temp;
}

