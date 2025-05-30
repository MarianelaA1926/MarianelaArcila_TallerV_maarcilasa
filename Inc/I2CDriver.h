/*
 * I2CDriver.h
 *
 *  Created on: 22/05/2023
 *      Author: Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for defining the control of the I2C peripheral.
 *  (control funtions)
 */

#ifndef I2CDRIVER_H_
#define I2CDRIVER_H_


#include <stm32f4xx.h>

#define I2C_WRITE_DATA                          0
#define I2C_READ_DATA 							1


#define MAIN_CLOCK_4_MHz_FOR_I2C				4
#define MAIN_CLOCK_16_MHz_FOR_I2C				16
#define MAIN_CLOCK_20_MHz_FOR_I2C				20


#define I2C_MODE_SM								0
#define I2C_MODE_FM								1



#define I2C_MODE_SM_SPEED_100KHz				80
#define I2C_MODE_FM_SPEED_400KHz				14


#define I2C_MAX_RISE_TIME_SM					17
#define I2C_MAX_RISE_TIME_FM					6

typedef struct{

	I2C_TypeDef 		*ptrI2Cx;
	uint8_t 			slaveAddress;
	uint8_t 			modeI2C;
	uint8_t 			dataI2C;

}I2C_Handler_t;

/*The public functions of the prototype.*/

void i2c_config(I2C_Handler_t *ptrHandlerI2C);
void i2c_startTransaction(I2C_Handler_t *ptrHandlerI2C);
void i2c_reStartTransation(I2C_Handler_t *ptrHandlerI2C);
void i2c_sedSlaveAddressRW(I2C_Handler_t *ptrHandlerI2C, uint8_t slaveAddress, uint8_t readOrWrite);
void i2c_sedMemoryAddress(I2C_Handler_t *ptrHandlerI2C, uint8_t memAddr);
void i2c_sedDataByte(I2C_Handler_t *ptrHandlerI2C, uint8_t dataToWrite);
uint8_t i2c_readDataByte(I2C_Handler_t *ptrHandlerI2C);
void i2c_stopTransaction(I2C_Handler_t *ptrHandlerI2C);
void i2cSedAck(I2C_Handler_t *ptrHandlerI2C);
void i2c_sedNoAck(I2C_Handler_t *ptrHandlerI2C);

uint8_t i2c_readSingleRegister(I2C_Handler_t *ptrHandlerI2C, uint8_t regToRead);
void i2c_writeSingleRegister(I2C_Handler_t *ptrHandlerI2C, uint8_t redToRead, uint8_t newValue);


#endif /* I2CDRIVER_H_ */
