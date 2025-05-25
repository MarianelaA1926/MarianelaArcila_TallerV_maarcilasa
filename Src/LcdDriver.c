/*
 * LcdDriver.c
 *
 *  Created on: 29/05/2023
 *      Author: marianela
 */

#include <stdint.h>
#include "LcdDriver.h"

/*

    The pins for I2C (SDA and SCL) need to be configured,
    for which the GPIO module and the pins must be configured
    in Alternate Function mode.
    Additionally, these pins should be configured as open-drain outputs
    with pull-up resistors enabled.
    */
void LCD_Config (LCD_Handler_t *ptrHandlerLCD){
	/* 1. We enable the clock signal for the selected I2C module */
	if (ptrHandlerLCD->ptrLCDx == I2C1){
		RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;
	}
	else if(ptrHandlerLCD ->ptrLCDx== I2C2){
		RCC -> APB1ENR |= RCC_APB1ENR_I2C2EN;
	}
	else if (ptrHandlerLCD -> ptrLCDx == I2C3){
		RCC -> APB1ENR |= RCC_APB1ENR_I2C3EN;
	}

	/* 2. We reset the peripheral, so that it starts in a known state */
	ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_SWRST;
	__NOP();
	ptrHandlerLCD -> ptrLCDx -> CR1 &= ~ I2C_CR1_SWRST;

	/* 3. We specify the frequency of the main clock, which is the signal used
	* by the peripheral to generate the clock signal for the I2C bus */

	ptrHandlerLCD-> ptrLCDx -> CR2 &= ~(0b111111 << I2C_CR2_FREQ_Pos); // Borramos la configuración
	ptrHandlerLCD-> ptrLCDx -> CR2 |= (MAIN_CLOCK_16_MHz_FOR_LCD << I2C_CR2_FREQ_Pos);

	/* We configure the I2C mode in which the system operates.

	    This configuration includes the clock speed and the maximum signal change time (T-Rise).
	    Everything starts with the registers set to 0 */

	ptrHandlerLCD-> ptrLCDx -> CCR = 0;
	ptrHandlerLCD-> ptrLCDx -> TRISE = 0;

	if(ptrHandlerLCD -> modeLCD == LCD_MODE_SM){
		// We are in "standard" mode (SM MODE)
		// Select the standard mode
		ptrHandlerLCD-> ptrLCDx-> CCR &= ~ I2C_CCR_FS;

		// We configure the register responsible for generating the clock signal

		ptrHandlerLCD -> ptrLCDx -> CCR |= (LCD_MODE_SM_SPEED_100KHz<< I2C_CCR_CCR_Pos);

		// We configure the register that controls the maximum T-Rise time
		ptrHandlerLCD->ptrLCDx -> TRISE |= LCD_MAX_RISE_TIME_SM;
	}
	else{

		// We are in "Fast" mode (FM Mode)
		// Select the fast mode

		ptrHandlerLCD -> ptrLCDx -> CCR |= I2C_CCR_FS;


		// We configure the register responsible for generating the clock signal
		ptrHandlerLCD -> ptrLCDx -> CCR |= (LCD_MODE_FM_SPEED_400KHz << I2C_CCR_CCR_Pos);

		// We configure the register that controls the maximum T-Rise time
		ptrHandlerLCD -> ptrLCDx -> TRISE |= LCD_MAX_RISE_TIME_FM;
	}

	/* 5. We enable the I2C module */
	ptrHandlerLCD -> ptrLCDx ->CR1 |= I2C_CR1_PE;
}

uint8_t LCD_ReadByte (LCD_Handler_t *ptrHandlerLCD,uint8_t memAddr){

	/* 0. We define an auxiliary variable */

	uint8_t auxByte = 0;
	(void) auxByte;

	/* 1. We check if the line is not busy - "busy" bit in I2C_CR2 */
	while (ptrHandlerLCD -> ptrLCDx -> SR2 & I2C_SR2_BUSY){
		__NOP();
	}

	/* 2. We generate the "start" signal */
	ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_START;
	/* 2a. We wait for the "start" event flag to be raised /
	/ While waiting, the value of SB is 0, so the negation (!) is 1 */
	while (!(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_SB)){
		__NOP();
	}

	/* 3. We send the address of the Slave and the bit indicating that we want to write (0)

	    (in the next step, we send the memory address to be read) */

	ptrHandlerLCD -> ptrLCDx -> DR = (ptrHandlerLCD -> slaveAddressLCD <<1)  | LCD_WRITE_DATA;

	/* 3.1 We wait for the "addr" event flag to be raised

	    (this indicates that the address was successfully sent) */

	while( !(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_ADDR)){
		__NOP();
	}

	/* 3.2 We need to clear the ACK reception flag from the "addr" event, for which

	    we need to read sequentially first the I2C_SR1 and then I2C_SR2 */
	auxByte = ptrHandlerLCD -> ptrLCDx -> SR1;
	auxByte = ptrHandlerLCD -> ptrLCDx -> SR2;

	/* 4. We send the memory address we want to read */

	ptrHandlerLCD -> ptrLCDx -> DR = memAddr;

	/* 4.1 We wait until the byte is transmitted */

	while ( !(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_TXE)){
		__NOP();
	}

	/* 5. We need to generate a RESTART signal, that is, send a new START */

	ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_START;

	/* 5.1 We wait for the "start" event flag to be raised /
	/ While waiting, the value of SB is 0, so the negation (!) is 1 */

	while( !(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_SB)){
		__NOP();
	}

	/* 6. We send the address of the Slave, but now with the indication to read */

	ptrHandlerLCD -> ptrLCDx -> DR = (ptrHandlerLCD -> slaveAddressLCD << 1) | LCD_READ_DATA ;

	/* 6.1 We wait for the "addr" event flag to be raised

	    (this indicates that the address was successfully sent) */

	while ( !(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_ADDR)){
		__NOP();
	}

	/* 6.2 We need to clear the ACK reception flag from the "addr" event, for which

	    we need to read sequentially first the I2C_SR1 and then I2C_SR2 */

	auxByte = ptrHandlerLCD -> ptrLCDx -> SR1;
	auxByte = ptrHandlerLCD -> ptrLCDx -> SR2;

	/* 7. We activate the no-ACK indication (indication for the Slave to terminate)

	    (We must write zero to the ACK bit position in the control register 1) */
	ptrHandlerLCD -> ptrLCDx-> CR1 &= ~ I2C_CR1_ACK;

	/* 8. We generate the stop condition */

	ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_STOP;

	/* 9. We wait until the incoming byte is received */

	while( !(ptrHandlerLCD-> ptrLCDx -> SR1 & I2C_SR1_RXNE)){
		__NOP();
	}
	ptrHandlerLCD -> dataLCD = ptrHandlerLCD -> ptrLCDx -> DR;
	return ptrHandlerLCD -> dataLCD;
}

void LCD_WriteByte (LCD_Handler_t *ptrHandlerLCD, uint8_t dataToWrite, uint8_t addres){
	/* 0. We define an auxiliary variable */

	uint8_t auxByte = 0;
	(void) auxByte;

	/* 1. We check if the line is not busy - "busy" bit in I2C_SR2 */
	while(ptrHandlerLCD->ptrLCDx->SR2 & I2C_SR2_BUSY){
		__NOP();
	}

	/* 2. We generate the "start" signal */

	ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_START;

	/* 2a. We wait for the "start" event flag to be raised /
	/ While waiting, the value of SB is 0, so the negation (!) is 1 */

	while ( !(ptrHandlerLCD->ptrLCDx -> SR1 & I2C_SR1_SB)){
		__NOP();
	}

	/* 3. We send the address of the Slave and the bit indicating that we want to write (0)

	    (in the next step, we send the memory address to be written) */
	ptrHandlerLCD -> ptrLCDx -> DR = (addres <<1) | LCD_WRITE_DATA ;

	/* 3. We send the address of the Slave and the bit indicating that we want to write (0)

	    (in the next step, we send the memory address to be written) */

while (!(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_ADDR)){
	__NOP();
}

/* 3.2 We need to clear the ACK reception flag from the "addr" event, for which

    we need to read sequentially first the I2C_SR1 and the I2C_SR2 */

auxByte = ptrHandlerLCD->  ptrLCDx -> SR1;
auxByte = ptrHandlerLCD -> ptrLCDx -> SR2;

/* 5. We load the value we want to write */

ptrHandlerLCD -> ptrLCDx -> DR = dataToWrite;


/* 6. We wait until the byte is transmitted */

while(!(ptrHandlerLCD -> ptrLCDx -> SR1 & I2C_SR1_BTF)){
	__NOP();

}

/* 7. We generate the stop condition */

ptrHandlerLCD -> ptrLCDx -> CR1 |= I2C_CR1_STOP;

}

void LCD_sendCMD (LCD_Handler_t *ptrHandlerLCD, char cmd, uint8_t addres){

	char _U;
	char _L;
	uint8_t _T[4];
	_U=(cmd & 0xf0);
	_L=((cmd<<4) & 0xf0);
	_T[0] = _U|0x0C;
	LCD_WriteByte(ptrHandlerLCD, _T[0],addres);
	_T[1] = _U|0x08;
	LCD_WriteByte(ptrHandlerLCD, _T[1],addres);
	_T[2] = _L|0x0C;
	LCD_WriteByte(ptrHandlerLCD, _T[2],addres);
	_T[3] = _L|0x08;
	LCD_WriteByte(ptrHandlerLCD, _T[3],addres);

}

void LCD_sendata(LCD_Handler_t *ptrHandlerLCD, char data,uint8_t addres){

	char _U;
	char _L;
	uint8_t _T[4];
	_U=(data & 0xf0);
	_L=((data<<4) & 0xf0);
	_T[0] = _U|0x0D;
	LCD_WriteByte(ptrHandlerLCD, _T[0],addres);
	_T[1] = _U|0x09;
	LCD_WriteByte(ptrHandlerLCD, _T[1],addres);
	_T[2] = _L|0x0D;
	LCD_WriteByte(ptrHandlerLCD, _T[2],addres);
	_T[3] = _L|0x09;
	LCD_WriteByte(ptrHandlerLCD, _T[3],addres);

}

void LCD_Clear(LCD_Handler_t *ptrHandlerLCD,uint8_t addres) {

	LCD_sendata (ptrHandlerLCD, 0x00,addres);
	for (int i=0; i<100; i++) {
		LCD_sendata (ptrHandlerLCD,' ',addres);
	}
}

void LCD_Init (LCD_Handler_t *ptrHandlerLCD,uint8_t addres) {

	delay_50();
	LCD_sendCMD (ptrHandlerLCD, 0x30,addres);
	delay_5();
	LCD_sendCMD (ptrHandlerLCD, 0x30,addres);
	delay_1();
	LCD_sendCMD (ptrHandlerLCD, 0x30,addres);
	delay_10();
	LCD_sendCMD (ptrHandlerLCD, 0x20,addres);
	delay_10();
	LCD_sendCMD (ptrHandlerLCD, 0x28,addres);
	delay_1();
	LCD_sendCMD (ptrHandlerLCD, 0x08,addres);
	delay_1();
	LCD_sendCMD (ptrHandlerLCD, 0x01,addres);
	delay_1();
	delay_1();
	LCD_sendCMD (ptrHandlerLCD, 0x06,addres);
	delay_1();
	LCD_sendCMD (ptrHandlerLCD, 0x0C,addres);

}

void LCD_sendSTR(LCD_Handler_t *ptrHandlerLCD, char *str,uint8_t addres) {

	while (*str) LCD_sendata (ptrHandlerLCD, *str++, addres);
}

void LCD_setCursor(LCD_Handler_t *ptrHandlerLCD, uint8_t x, uint8_t y,uint8_t addres) {

	uint8_t cursor;
	switch (x) {
	case 0 :                          /* Case for the first row of the LCD */
		switch (y) {
		/* Cases for the cells of the first row */
		case 0 : cursor = 0x00; break;
		case 1 : cursor = 0x01; break;
		case 2 : cursor = 0x02; break;
		case 3 : cursor = 0x03; break;
		case 4 : cursor = 0x04; break;
		case 5 : cursor = 0x05; break;
		case 6 : cursor = 0x06; break;
		case 7 : cursor = 0x07; break;
		case 8 : cursor = 0x08; break;
		case 9 : cursor = 0x09; break;
		case 10 : cursor = 0x0A; break;
		case 11 : cursor = 0x0B; break;
		case 12 : cursor = 0x0C; break;
		case 13 : cursor = 0x0D; break;
		case 14 : cursor = 0x0E; break;
		case 15 : cursor = 0x0F; break;
		case 16 : cursor = 0x10; break;
		case 17 : cursor = 0x11; break;
		case 18 : cursor = 0x12; break;
		case 19 : cursor = 0x13; break;
		}
		break;

		case 1 :                              /* Case for the first row of the LCD */

			switch (y) {
			/* Cases for the cells of the first row */
			case 0 : cursor = 0x40; break;
			case 1 : cursor = 0x41; break;
			case 2 : cursor = 0x42; break;
			case 3 : cursor = 0x43; break;
			case 4 : cursor = 0x44; break;
			case 5 : cursor = 0x45; break;
			case 6 : cursor = 0x46; break;
			case 7 : cursor = 0x47; break;
			case 8 : cursor = 0x48; break;
			case 9 : cursor = 0x49; break;
			case 10 : cursor = 0x4A; break;
			case 11 : cursor = 0x4B; break;
			case 12 : cursor = 0x4C; break;
			case 13 : cursor = 0x4D; break;
			case 14 : cursor = 0x4E; break;
			case 15 : cursor = 0x4F; break;
			case 16 : cursor = 0x50; break;
			case 17 : cursor = 0x51; break;
			case 18 : cursor = 0x52; break;
			case 19 : cursor = 0x53; break;
			}
			break;

			case 2 :                          /* Case for the first row of the LCD */

				switch (y) {
				/* Cases for the cells of the first row */
				case 0 : cursor = 0x14; break;
				case 1 : cursor = 0x15; break;
				case 2 : cursor = 0x16; break;
				case 3 : cursor = 0x17; break;
				case 4 : cursor = 0x18; break;
				case 5 : cursor = 0x19; break;
				case 6 : cursor = 0x1A; break;
				case 7 : cursor = 0x1B; break;
				case 8 : cursor = 0x1C; break;
				case 9 : cursor = 0x1D; break;
				case 10 : cursor = 0x1E; break;
				case 11 : cursor = 0x1F; break;
				case 12 : cursor = 0x20; break;
				case 13 : cursor = 0x21; break;
				case 14 : cursor = 0x22; break;
				case 15 : cursor = 0x23; break;
				case 16 : cursor = 0x24; break;
				case 17 : cursor = 0x25; break;
				case 18 : cursor = 0x26; break;
				case 19 : cursor = 0x27; break;
				}
				break;

				case 3 :                           /* Cases for the cells of the first row */
					switch (y) {
					/* Cases for the cells of the first row */
					case 0 : cursor = 0x54; break;
					case 1 : cursor = 0x55; break;
					case 2 : cursor = 0x56; break;
					case 3 : cursor = 0x57; break;
					case 4 : cursor = 0x58; break;
					case 5 : cursor = 0x59; break;
					case 6 : cursor = 0x5A; break;
					case 7 : cursor = 0x5B; break;
					case 8 : cursor = 0x5C; break;
					case 9 : cursor = 0x5D; break;
					case 10 : cursor = 0x5E; break;
					case 11 : cursor = 0x5F; break;
					case 12 : cursor = 0x60; break;
					case 13 : cursor = 0x61; break;
					case 14 : cursor = 0x62; break;
					case 15 : cursor = 0x63; break;
					case 16 : cursor = 0x64; break;
					case 17 : cursor = 0x65; break;
					case 18 : cursor = 0x66; break;
					case 19 : cursor = 0x67; break;
					}
					break;
	}

	LCD_sendCMD(ptrHandlerLCD, 0x80|cursor,addres);

}

void delay_50 (void){
	for (int i=0;i<62500;i++){
		__NOP();
	}
}

void delay_5 (void){

	for (int i=0; i<6250; i++){
		__NOP();
	}
}

void delay_1 (void){

	for (int i=0;i<1250;i++){
		__NOP();
	}
}

void delay_10 (void){

	for (int i=0;i<12500;i++){
		__NOP();
	}
}

