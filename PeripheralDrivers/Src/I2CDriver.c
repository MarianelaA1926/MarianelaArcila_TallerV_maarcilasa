/*
 * I2CDriver.c
 *
 *  Created on: 22/05/2023
 *      Author: Marianela Arcila Sanchez	maarcilasa
 *  In this program, we write about the control of the I2C peripheral.
 *  This control aims to be as generic as possible.
 */



#include <stdint.h>
#include "I2CDriver.h"

/* We configure the pins for the I2C (SDA, SCL) with the GPIO module.
 * The pins will be configured in Alternative Function mode, set as output open-drain,
 * and enabled with pull-up resistors.
 */

void i2c_config(I2C_Handler_t *ptrHandlerI2C){


	/* 1 We activate the clock signal for the selected I2C mode. */

	if(ptrHandlerI2C -> ptrI2Cx == I2C1){
		RCC ->AHB1ENR |= RCC_APB1ENR_I2C1EN;
	}

	else if(ptrHandlerI2C -> ptrI2Cx == I2C2){
		RCC ->AHB1ENR |= RCC_APB1ENR_I2C1EN;
	}
	else if(ptrHandlerI2C -> ptrI2Cx == I2C3){
		RCC ->AHB1ENR |= RCC_APB1ENR_I2C3EN;
		}

	/* 2. We restart the peripheral to ensure it starts in a known state. */
	ptrHandlerI2C -> ptrI2Cx -> CR1 |= I2C_CR1_SWRST;
	__NOP();
	ptrHandlerI2C -> ptrI2Cx -> CR2 &= ~ I2C_CR1_SWRST;

	/*3 We indicate the speed of the main clock, which is the frequency
	 * used by the peripheral to generate the clock signal for the I2C bus. */

	ptrHandlerI2C -> ptrI2Cx -> CR2 &= ~(0b111111 << I2C_CR2_FREQ_Pos); //Delete the previous configuration
	ptrHandlerI2C -> ptrI2Cx -> CR2 |= (MAIN_CLOCK_16_MHz_FOR_I2C << I2C_CR2_FREQ_Pos);

	/* 4.We configure the I2C mode in which the system works. This configuration
	 * includes the clock speed and the maximum time for the signal change (T-Rise).
	 * It starts with setting the appropriate registers for these values */

	ptrHandlerI2C -> ptrI2Cx -> CCR = 0;
	ptrHandlerI2C -> ptrI2Cx -> TRISE = 0;

	if(ptrHandlerI2C -> modeI2C == I2C_MODE_SM){

		// "standar" (SM Mode)
		// We select the standard mode for the I2C configuration.
		ptrHandlerI2C -> ptrI2Cx -> CCR &= ~ I2C_CCR_FS;

		// We configure the register that is responsible for generating the clock signal
		ptrHandlerI2C -> ptrI2Cx -> CCR |= (I2C_MODE_SM_SPEED_100KHz << I2C_CCR_CCR_Pos);

		// We configure the register that controls the maximum T-Rise time.
		ptrHandlerI2C ->ptrI2Cx -> TRISE |= I2C_MAX_RISE_TIME_SM;


	}
	else{

		//"FAST" (FM mode)
		// We select the FAST mode

		ptrHandlerI2C -> ptrI2Cx -> CCR |= I2C_CCR_FS;

		//We configure the register that is responsible for generating the clock signal.
		ptrHandlerI2C -> ptrI2Cx -> CCR |= (I2C_MODE_FM_SPEED_400KHz << I2C_CCR_CCR_Pos);

		//We configure the register that controls the maximum T-Rise time.
		ptrHandlerI2C -> ptrI2Cx -> TRISE |= I2C_MAX_RISE_TIME_FM;
	}

	/*5 We activate the I2C mode.*/
	ptrHandlerI2C -> ptrI2Cx -> CR1 |= I2C_CR1_PE;




}

/*  Generamos la condicion de Stop */
void i2c_stopTransaction(I2C_Handler_t *ptrHandlerI2C){

	ptrHandlerI2C -> ptrI2Cx -> CR1 |= I2C_CR1_STOP;

}

/* 1 We verify that the line is not busy by checking the "busy" bit in the I2C_CR2 register.
 * 2 We generate the "start" signal.
 * 2a We wait for the "Start" event flag to rise.
 * While we wait, the value of SB is 0, so the negation (!) is 1. */

void i2c_startTransaction(I2C_Handler_t *ptrHandlerI2C){

	/* 1. We verify that the line is not busy by checking the "busy" bit in the I2C_CR2 register.  */
	while(!(ptrHandlerI2C ->ptrI2Cx ->SR2 & I2C_SR1_SB)){
		__NOP();
	}
	/* 2. We generate the "Start" signal */
	ptrHandlerI2C -> ptrI2Cx -> CR1 |= I2C_CR1_START;

	/*2a While we wait for the "Start" event flag to rise, the value of SB is 0,
	 * so the negation (!) is 1. */
	while(!(ptrHandlerI2C -> ptrI2Cx -> SR1 & I2C_SR1_SB)){
		__NOP();
	}

}

void i2c_reStartTrnasaction(I2C_Handler_t *ptrHandlerI2C){

	/* 2. We generate the "Start" signal */
	ptrHandlerI2C -> ptrI2Cx -> CR1 = I2C_CR1_START;


	/* 2a.While we wait for the "Start" event flag to rise,
	 * the value of SB is 0, so the negation (!) is 1. */
	while(!(ptrHandlerI2C -> ptrI2Cx -> SR1 & I2C_SR1_SB)){
		__NOP();

	}
}

/*7a We activate the indication for no-ACK (indicating the Slave to terminate.) */
void i2c_sedNoAck(I2C_Handler_t *ptrHandlerI2C){

	/* We need to write a "0" in the ACK position of the control register 1.*/
	ptrHandlerI2C -> ptrI2Cx -> CR1 &= ~I2C_CR1_ACK;
}

/* 7b We activate the indication for no-ACK (indicating the Slave to terminate.) */

void i2c_sedAck(I2C_Handler_t *ptrHandlerI2C){
	/* We need to write a "0" in the ACK position of the control register 1. */
	ptrHandlerI2C -> ptrI2Cx -> CR1 |= I2C_CR1_ACK;

}

/**/

void i2c_sedSlaveAddressRW(I2C_Handler_t *ptrHandlerI2C, uint8_t slaveAddress, uint8_t readOrWrite){

	/* We define an auxiliary variable.*/
	uint8_t auxByte = 0;
	(void) auxByte;

	/* 3. We send the address of the Slave and the bit that indicates that we want to write (0).
	 * In the next step, we send the memory address that we want to write.*/
	ptrHandlerI2C -> ptrI2Cx -> DR = (slaveAddress << 1) | readOrWrite;

	/* 3.1 We wait until the "addr" event flag is raised. This indicates that the address
	 * was sent successfully.*/

	while(!(ptrHandlerI2C -> ptrI2Cx -> SR1 & I2C_SR1_ADDR)){
		__NOP();
	}

	/* 3.2 We need to clear the ACK reception flag of the "addr"
	 *  by first reading the sequence I2C_SR1 and then I2C_SR2. */
	 auxByte = ptrHandlerI2C -> ptrI2Cx -> SR1;
	 auxByte = ptrHandlerI2C -> ptrI2Cx -> SR2;
}

/**/
void i2c_sedMemoryAddress(I2C_Handler_t *ptrHandlerI2C, uint8_t  memAddr){
	/* 4 We send the memory address that we want to read. */
	ptrHandlerI2C -> ptrI2Cx -> DR = memAddr;

	/* 4.1 We wait until the byte is transmitted.*/
	while(!(ptrHandlerI2C -> ptrI2Cx -> SR1 & I2C_SR1_TXE)){
		__NOP();
	}
}

/**/
void i2c_sedDataByte(I2C_Handler_t *ptrHandlerI2C, uint8_t dataToWrite){

	/* 5. We load the value that we want to write.*/
	ptrHandlerI2C ->ptrI2Cx -> DR = dataToWrite;

	/* 6. We wait until the byte is transmitted.*/
	while(!(ptrHandlerI2C ->ptrI2Cx ->SR1 & I2C_SR1_BTF)){
		__NOP();
	}
}


uint8_t i2c_readDataByte(I2C_Handler_t *ptrHandlerI2C){
	/*We wait until the incoming byte is received.*/
	while(!(ptrHandlerI2C -> ptrI2Cx ->SR1 & I2C_SR1_RXNE)){
		__NOP();
	}
		ptrHandlerI2C -> dataI2C = ptrHandlerI2C ->ptrI2Cx ->DR;
		return ptrHandlerI2C -> dataI2C;
}


uint8_t i2c_readSingleRegister(I2C_Handler_t *ptrHandlerI2C, uint8_t regToRead){

	/* 0. We create an auxiliary variable to receive the data we read. */
	uint8_t auxRead = 0;

	/*1. We generate the Start condition*/
	i2c_startTransaction(ptrHandlerI2C);

	/*2.We send the address of the slave and the indication to Write*/
	i2c_sedSlaveAddressRW(ptrHandlerI2C,ptrHandlerI2C -> slaveAddress, I2C_WRITE_DATA);

	/*3. We send the memory address that we want to read.*/
	i2c_sedMemoryAddress(ptrHandlerI2C, regToRead);

	/*4 We create a restart condition*/
	i2c_reStartTrnasaction(ptrHandlerI2C);

	/*5.We send the address of the slave and the READ indication.*/
	i2c_sedSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C -> slaveAddress, I2C_READ_DATA);

	/*6. We generate the NoAck condition, where the Master does not respond and the slave only sends 1 byte.*/
	i2c_sedNoAck(ptrHandlerI2C);

	/*7. We generate the Stop condition, so that the slave stops after sending 1 byte.*/
	i2c_stopTransaction(ptrHandlerI2C);

	/* 8. We read the data sent by the slave. */
	auxRead = i2c_readDataByte(ptrHandlerI2C);

	return auxRead;




}


void i2c_writeSingleRegister(I2C_Handler_t *ptrHandlerI2C, uint8_t redToRead,uint8_t newValue){

	/*1. We generate the Start condition.*/
	i2c_startTransaction(ptrHandlerI2C);
	/*2. We send the slave address and the WRITE indication.*/
	i2c_sedSlaveAddressRW(ptrHandlerI2C, ptrHandlerI2C -> slaveAddress , I2C_WRITE_DATA);

	/*3. We send the memory address that we want to write.*/
	i2c_sedMemoryAddress(ptrHandlerI2C, redToRead);

	/*4. We send the value that we want to write in the selected record.*/
	i2c_sedDataByte(ptrHandlerI2C, newValue);

	/*5. We generate the Stop condition, so that the slave stops after sending 1 byte.*/
	i2c_stopTransaction(ptrHandlerI2C);
}


