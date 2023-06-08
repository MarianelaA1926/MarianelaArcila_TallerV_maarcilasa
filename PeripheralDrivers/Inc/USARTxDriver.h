/*
 * USARTxDriver.h
 *
 *  Created on: 21/05/2023
 *      Author:Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for
 *  defining the control of the USARTx peripheral. (control funtions)
 *
 */

#ifndef USARTXDRIVER_H_
#define USARTXDRIVER_H_

#include <stm32f4xx.h>
#include <stdio.h>


#define USART_MODE_TX		0
#define USART_MODE_RX		1
#define USART_MODE_RXTX		2
#define USART_MODE_DISABLE	3

#define USART_BAUDRATE_9600		0
#define USART_BAUDRATE_19200	1
#define USART_BAUDRATE_115200	2

#define USART_DATASIZE_8BIT		0
#define USART_DATASIZE_9BIT		1

#define USART_PARITY_NONE	0
#define USART_PARITY_ODD	1
#define USART_PARITY_EVEN	2

#define USART_STOPBIT_1		0
#define USART_STOPBIT_0_5	1
#define USART_STOPBIT_2		2
#define USART_STOPBIT_1_5	3

#define USART_RX_INTERRUP_ENABLE  0
#define USART_TX_INTERRUP_ENABLE  1

#define USART_TC_INTERRUP_EVENT 	0
#define USART_TXE_INTERRUP_EVENT	1

#define USART_MCU_FREQUENCY_16MHz 0
#define USART_MCU_FREQUENCY_80MHz 1
#define USART_MCU_FREQUENCY_100MHz 2

/* Structure for communication configuration:
   Baudrate
   Data size
   Error control
   Stop bit
*/


typedef struct
{
	uint8_t MCU_frecuency;
	uint8_t USART_mode;
	uint8_t USART_baudrate;
	uint8_t USART_datasize;
	uint8_t USART_parity;
	uint8_t USART_stopbits;
	uint8_t USART_enableInterrupt;
	uint8_t USART_interruptTx;
}USART_Config_t;

/*
 * Definition of the Handler for a USART:
 *
 * - Structure that contains the Special
 * 	 Function Registers (SFR) that control the peripheral
 * - Structure that contains the specific configuration of the object
 * - Data reception buffer
 * - Element indicating how many data have been received
 * - Data transmission buffer
 * - Element indicating how many data need to be sent.
 */
typedef struct
{
	USART_TypeDef	*ptrUSARTx;
	USART_Config_t	USART_Config;
	uint8_t			receptionBuffer[64];
	uint8_t			dataInputSize;
	uint8_t			transmisionBuffer[64];
	uint8_t			dataOutputSize;
}USART_Handler_t;



/* Definition of prototypes for USART functions */
void usart1_Callback(void);
void usart2_Callback(void);
void usart6_Callback(void);


void USART_Config(USART_Handler_t *ptrUsartHandler);
int writeChar(USART_Handler_t *ptrUsartHandler, int dataToSend );
void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSed);
uint8_t getRxData(void);
uint8_t getTxData(void);
#endif /* USARTXDRIVER_H_ */
