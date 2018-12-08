#ifndef __ADXRS810_H
#define __ADXRS810_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "main.h"

#include "spi.h"
#include "usart.h"
#include "gpio.h"
     
#define ADXRS810_CS_SET     HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
#define ADXRS810_CS_RESET      HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);

/*********************************************************************************************************
**	陀螺仪控制数据定义
*********************************************************************************************************/
#define ADXRS810_STARTUP_DELAY	0.05 /* sec */

/* The MSB for the spi commands */
#define ADXRS810_SENSOR_DATA	0x20
#define ADXRS810_WRITE_DATA		0x40
#define ADXRS810_READ_DATA		0x80

/* Memory register map */
#define ADXRS810_RATE1			0x00	// Rate Registers
#define ADXRS810_TEMP1			0x02	// Temperature Registers
#define ADXRS810_LOCST1			0x04	// Low CST Memory Registers
#define ADXRS810_HICST1			0x06	// High CST Memory Registers
#define ADXRS810_QUAD1			0x08	// Quad Memory Registers
#define ADXRS810_FAULT1			0x0A	// Fault Registers
#define ADXRS810_PID1			0x0C	// Part ID Register 1
#define ADXRS810_SNH			0x0E	// Serial Number Registers, 4 bytes
#define ADXRS810_SNL			0x10

/* Check bits */
#define ADXRS810_P				0x01	// Parity bit
#define ADXRS810_CHK			0x02	
#define ADXRS810_CST			0x04
#define ADXRS810_PWR			0x08
#define ADXRS810_POR			0x10
#define ADXRS810_NVM			0x20
#define ADXRS810_Q				0x40
#define ADXRS810_PLL			0x80
#define ADXRS810_UV				0x100
#define ADXRS810_OV				0x200
#define ADXRS810_AMP			0x400
#define ADXRS810_FAIL			0x800

#define ADXRS810_WRERR_MASK		(0x7 << 29)
#define ADXRS810_GET_ST(a)		((a >> 26) & 0x3)  // Status bits



/*********************************************************************************************************
**	陀螺仪控制函数声明
*********************************************************************************************************/
uint8_t SPI_Read(uint8_t* data,uint8_t bytesNumber);
uint8_t SPI_Write(uint8_t *data,uint8_t bytesNumber);
uint8_t ADXRS810_ParityBit(uint32_t data);
extern uint8_t ADXRS810_Init(void);
extern int16_t ADXRS810_Data(void);
extern uint16_t ADXRS810_GetRegisterValue(uint8_t regAddress);
extern void ADXRS810_SetRegisterValue(uint8_t regAddress, uint16_t regData);
extern uint8_t ADXRS810_GetTemperature(void);
     
     
     
#ifdef __cplusplus
}
#endif
#endif