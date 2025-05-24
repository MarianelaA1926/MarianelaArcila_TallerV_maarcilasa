/*
 * USARTxDriver.c
 *
 *  Created on: 21/05/2023
 *      Author:Marianela Arcila Sanchez
 *  In this program, we write about the control of the USART peripheral.
 *  This control aims to be as generic as possible.
 *
 */

#include <stm32f4xx.h>
#include "USARTxDriver.h"



uint8_t auxRxData = 0;
uint8_t auxTxData = 0;
USART_TypeDef *ptrUsartUsed;

void USART_Config(USART_Handler_t *ptrUsartHandler){
	/* First, we need to activate the clock signal and the serial port
	 * in the USART_Config.
	 */

	//Save the reference of the USART that we use.
	ptrUsartUsed = ptrUsartHandler ->ptrUSARTx;
	//We deactivate global interruptions while we configure the USART system
	__disable_irq();

	/* For the signal clock configuration, we need to configure each one of the
	 * different clocks for the possible modules (USART1, USART2, USART6) */

	/* USART1 */
	if(ptrUsartHandler->ptrUSARTx == USART1){
		/* This is the bus that USART1 uses for the signal clock*/
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	/* USART2 */
	else if (ptrUsartHandler -> ptrUSARTx == USART2){
		/*This is the bus that USART2 uses for the signal clock*/
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

	}
	/* USART6 */
	else{
		/*This is the bus that USART6 uses for the signal clock*/
		RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;
	}


	/* We configure the data size, parity, and stop bit. */



	/* The second step is to configure the CR1 register. In this register,
	 * we define the parity, data size, and stop bit.
	 */
	//2.1 We clear the RC1 register
	ptrUsartHandler->ptrUSARTx->CR1 = 0;


	// 2.2 We configure the parity: We check if the parity is activated or not

	/* Parity is a method used to verify the integrity of data by adding an additional bit.
	 * When the receiver receives the information, it compares the received parity bit with
	 * the expected parity. If the comparison does not match, it is determined that an error
	 * has occurred during transmission, and it can be inferred that the received information
	 * is incorrect. */

	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE){

		// We verify the even parity.
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN){

			/* We load the configuration in bit 9 of the CR1 register.
			 * In the case of an EVEN parity configuration, a 0 is placed
			 * in that position.
		    */
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PCE;
			ptrUsartHandler ->ptrUSARTx -> CR1 &= ~USART_CR1_PS;

		}
		else{
			/* When the configuration is ODD, in this case,
			 * a 1 is placed in bit 9 of the CR1 register */
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PCE;
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PS;
		}
	}
	else{
		/* In this case, if we don't want parity-check,
		 * we simply leave the bit 9 of the CR1 register as 0, indicating
		 * that no parity-check is enabled.*/

			ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_PCE;

	}

	// 2.3 We configure the size of the data
	if(ptrUsartHandler -> USART_Config.USART_datasize == USART_DATASIZE_8BIT){

		// 8 Bits
		ptrUsartHandler ->ptrUSARTx -> CR1 &= ~USART_CR1_M;
	}
	else{
		// 9 Bits
		ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_M;
	}


	// 2.4 We configure the stop bits in the USART_CR2 register (SFR).
	/*
	 * This register is used to configure the USART settings
	 * related to stop bits. It allows selecting the number
	 * of stop bits used for serial communication. The CR2
	 * register has two stop bits (STOP[1:0]) that can be
	 * configured as follows:
	 *   - 00: 1 stop bit
	 *   - 01: 0.5 stop bit
	 *   - 10: 2 stop bits
	 *   - 11: 1.5 stop bits
	 *
	 * The code below clears the CR2 register to ensure a
	 * clean state. Then, based on the USART configuration,
	 * the appropriate value is loaded into the stop bit
	 * fields.
	 ******************************************************/

	// We clear the CR2 register
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	switch (ptrUsartHandler->USART_Config.USART_stopbits) {
	  case USART_STOPBIT_1: {
	    // Load 0b00 into the two stop bit fields
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_0;
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_1;
	    break;
	  }
	  case USART_STOPBIT_0_5: {
	    // Load 0b01 into the two stop bit fields
	    ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_0;
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_1;
	    break;
	  }
	  case USART_STOPBIT_2: {
	    // Load 0b10 into the two stop bit fields
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_0;
	    ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_1;
	    break;
	  }
	  case USART_STOPBIT_1_5: {
	    // Load 0b11 into the two stop bit fields
	    ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_0;
	    ptrUsartHandler->ptrUSARTx->CR2 |= USART_CR2_STOP_1;
	    break;
	  }
	  default: {
	    // Default case: select 1 stop bit
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_0;
	    ptrUsartHandler->ptrUSARTx->CR2 &= ~USART_CR2_STOP_1;
	    break;
	  }
	}


	// 2.5 Baudrate Configuration (USART_BRR )
	//We configure the frecuency of the Micro MCU_frecuency
	switch (ptrUsartHandler-> USART_Config.MCU_frecuency ){
	case USART_MCU_FREQUENCY_16MHz:

		// See value table (Table 73), Freq = 16MHz, overr = 0;
		if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600) {
			/* The value to load is 104.1875 -> Mantissa = 104, fraction = 0.1875
			 * Mantissa = 104 = 0x68, fraction = 16 * 0.1875 = 3
			 * Value to load: 0x0683
			 * Configuring the Baudrate generator for a speed of 9600bps
			 */
			ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200) {
			/* The value to load is 52.0625 -> Mantissa = 52, fraction = 0.0625
			 * Mantissa = 52 = 0x34, fraction = 16 * 0.0625 = 1
			 * Value to load: 0x0341
			 */
			ptrUsartHandler->ptrUSARTx->BRR = 0x0341;
		}

		else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200) {
			/* The value to load is 8.6875 -> Mantissa = 8, fraction = 0.6875
			 * Mantissa = 8 = 0x08; Fraction = 16 * 0.6875 = 11
			 * Value to load: 0x08B
			*/
			ptrUsartHandler->ptrUSARTx->BRR = 0x08B;
		}

		break;

	case USART_MCU_FREQUENCY_80MHz:



		if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
			/* The value to load is 43.4027778 -> Mantissa = 43 , fraction 0.4027778
			 * Mantissa = 43 -> 0x02B;    fraction 16*0.4027778 = 6,4444 aprx 6
			 * Value to load: 0x02B6			 *
			 */
			ptrUsartHandler->ptrUSARTx->BRR = 0x02B6;
		}

		break;
	case USART_MCU_FREQUENCY_100MHz:
		if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
			/* The value to load is 54.2534722 -> Mantissa = 54 , fraction 0.2534722
			 * Mantissa = 54 -> 0x02B;    fraction 16*0.2534722 = 4,055555555 aprx 4
			 * Value to load: 0x0364
			 */
			ptrUsartHandler->ptrUSARTx->BRR = 0x0364;

		}
		else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600) {
			/* The value to load is 651,041-> Mantissa = 651, fraction = 0.041 aprx 0
			 * Mantissa = 651 = 0x28B, fraction = 16 * 0 = 0
			 * Value to load: 0x028B0
			 * Configuring the Baudrate generator for a speed of 9600bps
			 */
			ptrUsartHandler->ptrUSARTx->BRR = 0x028B0;
		}




	}


	// 2.6 Configure the mode: TX only, RX only, RXTX, disable
	/* TE and RE are two consecutive bits, with RE being the
	*least significant, so we will use the RE position
	*/
	switch (ptrUsartHandler->USART_Config.USART_mode) {
		case USART_MODE_TX:
			// Enable only TX interrupt
			ptrUsartHandler->ptrUSARTx->CR1 |= (USART_CR1_TE);
			break;
		case USART_MODE_RX:
			// Enable only RX interrupt
			ptrUsartHandler->ptrUSARTx->CR1 |= (USART_CR1_RE);
			break;
		case USART_MODE_RXTX:
			// Enable both interrupts
			ptrUsartHandler->ptrUSARTx->CR1 |= (USART_CR1_TE);
			ptrUsartHandler->ptrUSARTx->CR1 |= (USART_CR1_RE);
			break;

		case USART_MODE_DISABLE:
			// Disable both interrupts
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
			break;
		default:
			// By default, disable both interrupts
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_TE;
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_RE;
			ptrUsartHandler->ptrUSARTx->CR1 &= ~USART_CR1_UE;
			break;
		}

	/* 2.7 Verify interrupt configuration
	a) Reception interrupt */
	switch	(ptrUsartHandler -> USART_Config.USART_enableInterrupt){

	case USART_RX_INTERRUP_ENABLE:
			/* Since it is enabled, we need to configure the reception interrupt
			*  We need to enable the RX interrupt in the USART configuration */
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_RXNEIE;
			/* Enable interrupt in NVIC

			This is done for each of the available options (USART 1, USART 2, USART 6) */
			if (ptrUsartHandler->ptrUSARTx == USART1) {
				__NVIC_EnableIRQ(USART1_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART2) {
				__NVIC_EnableIRQ(USART2_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART6) {
				__NVIC_EnableIRQ(USART6_IRQn);
			}
			else {

			}
			break;
		//We define the interruption when the Transmit data register is empty(TXE)
	case USART_TX_INTERRUP_ENABLE:

	 //We configure the TXE interrupt event in the CR1 Register

		if(ptrUsartHandler -> USART_Config.USART_interruptTx == USART_TXE_INTERRUP_EVENT){

			/*This is done for each of the available options (USART 1, USART 2, USART 6) */

			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TXEIE;

			if (ptrUsartHandler->ptrUSARTx == USART1) {
				__NVIC_EnableIRQ(USART1_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART2) {
				__NVIC_EnableIRQ(USART2_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART6) {
				__NVIC_EnableIRQ(USART6_IRQn);
			}
			else {
				break;
			}

		}
		else {
			ptrUsartHandler->ptrUSARTx->CR1 |= USART_CR1_TCIE;

			/*This is done for each of the available options (USART 1, USART 2, USART 6) */
			if (ptrUsartHandler->ptrUSARTx == USART1) {
				__NVIC_EnableIRQ(USART1_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART2) {
				__NVIC_EnableIRQ(USART2_IRQn);

			}
			else if (ptrUsartHandler->ptrUSARTx == USART6) {
				__NVIC_EnableIRQ(USART6_IRQn);
			}
			else {

			}

		}
		break;

	}


	// 2.8 Activate the serial module.
	if (ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE) {

		ptrUsartHandler->ptrUSARTx->CR1 |= (USART_CR1_UE);
	}

	/* Enable system interrupts again */
	__enable_irq();

}



/* Function to write a single character */
int writeChar(USART_Handler_t *ptrUsartHandler, int dataToSend) {
	while (!(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TXE)) {
		__NOP();
	}

	ptrUsartHandler->ptrUSARTx->DR = dataToSend;

	return dataToSend;
}

void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSed) {
	while (*msgToSed != '\0') {
		writeChar(ptrUsartHandler, *msgToSed);
		msgToSed++;
	}

}

uint8_t getRxData(void){
	return auxRxData;

}
uint8_t getTxData(void){
	return auxTxData;

}

/* USART2 interrupt handler
 * All associated interrupts (TX, RX, PE) should be handled here*/
void USART2_IRQHandler(void) {

	if (USART2->SR & USART_SR_RXNE) {
		auxRxData = (uint8_t)USART2->DR;
		usart2_Callback();

		}

	else if (USART2->SR & USART_SR_TXE) {
		auxTxData = (uint8_t)USART2 ->DR;
		usart2_Callback();
		/*  The system requests to clean the DR (Data Register) every time a character
		 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
		USART2->DR &= 0;
		USART2->CR1 &= ~(USART_CR1_TXEIE);

	}


	else if (USART2->SR & USART_SR_TC) {
		auxTxData = (uint8_t)USART2 -> DR;
		usart2_Callback();

		/*  The system requests to clean the DR (Data Register) every time a character
		 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
		USART2->DR &= 0;
		USART2->CR1 &= ~(USART_CR1_TCIE);

	}

}




/* USART1 interrupt handler
 * All associated interrupts (TX, RX, PE) should be handled here*/
void USART1_IRQHandler(void) {

	if (USART1->SR & USART_SR_RXNE) {
		auxRxData = (uint8_t)USART1->DR;
		usart1_Callback();
		}

	if (USART1->SR & USART_SR_TXE) {
		auxTxData = (uint8_t)USART1 ->DR;
		usart1_Callback();
		/*  The system requests to clean the DR (Data Register) every time a character
		 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
		USART1->DR &= 0;
		USART1->CR1 &= ~(USART_CR1_TXEIE);


	}


	if (USART1->SR & USART_SR_TC) {
		auxTxData = (uint8_t)USART1 -> DR;
		usart1_Callback();}
	/*  The system requests to clean the DR (Data Register) every time a character
	 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
	    USART1->DR &= 0;
		USART1->CR1 &= ~(USART_CR1_TCIE);


}



/* USART6 interrupt handler
 * All associated interrupts (TX, RX, PE) should be handled here */

void USART6_IRQHandler(void) {
	if (USART6->SR & USART_SR_RXNE) {
		auxRxData = (uint8_t)USART6->DR;
		usart6_Callback();
		}
	if (USART6->SR & USART_SR_TXE) {
		auxTxData = (uint8_t)USART6 ->DR;
		usart6_Callback();
		/*  The system requests to clean the DR (Data Register) every time a character
		 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
		USART6->DR &= 0;
		USART6->CR1 &= ~(USART_CR1_TXEIE);


	}


	if (USART6->SR & USART_SR_TC) {
		auxTxData = (uint8_t)USART6 -> DR;
		usart6_Callback();
		/*  The system requests to clean the DR (Data Register) every time a character
		 *  is about to be sent, therefore clearing the DR register and setting the event to 0.*/
		USART6->DR &= 0;
		USART6->CR1 &= ~(USART_CR1_TCIE);

	}

    }



__attribute__((weak)) void usart1_Callback(void){
	  	__NOP();
}

__attribute__((weak)) void usart2_Callback(void){
	  	__NOP();
}
__attribute__((weak)) void usart6_Callback(void){
	  	__NOP();
}


