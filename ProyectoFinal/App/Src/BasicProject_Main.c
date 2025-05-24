/*
 *
 *
 *  Created on: 24/03/2023
 *      Author: marianela
 */
/**
 ******************************************************************************
 * @file           : BasicProyect_Main.c
 * @author         : Marianela Arcila Sanchez- maarcilasa
 * @brief          : Test CMSIS -> DSP -> Absolute value
 ******************************************************************************
 * -
 * -
 * -
 ******************************************************************************
 */

#include "stm32f4xx.h"
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "USARTxDriver.h"

// Definicion de los handlers necesarios
GPIO_Handler_t handlerStateLed = {0};
GPIO_Handler_t handlerPinTx = {0};
GPIO_Handler_t handlerPinRx = {0};

GPIO_Handler_t handlerPinTx1 = {0};
GPIO_Handler_t handlerPinRx1 = {0};

// Timer encargado del Blinky
BasicTimer_Handler_t handlerStateTimer = {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};
USART_Handler_t handlerUsart1 = {0};
// Variables para recibir la respuesta
uint8_t rxData = 0;
char bufferData[64];

// Variables para recibir la respuesta del ESP-05
uint8_t rxData1 = 0;
char bufferData1[64];

// Variables a enviar a través del USART2
unsigned int numeroGrande = 1682539;


/* Definición de prototipos de funciones */
void InitSystem(void);


int main(void) {

	InitSystem();

	// Enviar el comando AT al módulo ESP-05 a través de USART1
	writeMsg(&handlerUsart1, "AT\r\n");

	while (1) {

		// Esperar respuesta del módulo ESP-05
		uint8_t receivedData = getRxData();

		// Verificar si se ha recibido un dato
		if (receivedData != 0) {
			// Enviar el dato recibido a través del USART2
			writeChar(&handlerUsart2, receivedData);
			// Verificar si se ha recibido el fin de línea '\n'
			if (receivedData == '\n') {
				// Imprimir la respuesta recibida
				printf("Response from ESP-05: %s\n", bufferData1);
			}
		}

	}

	return 0;
}
/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void InitSystem(void) {
    // Configurando el pin para el Led_Blinky
    handlerStateLed.pGPIOx = GPIOA;
    handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
    handlerStateLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
    handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
    handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

    // Cargamos la configuración del Pin del led de estado
    GPIO_Config(&handlerStateLed);

    // Llevamos el Led a un estado de encendido
    GPIO_WritePin(&handlerStateLed, RESET);

    /* Configurando los pines sobre los que funciona el USART2 (TX) */
    handlerPinTx.pGPIOx = GPIOA;
    handlerPinTx.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
    handlerPinTx.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    handlerPinTx.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
    handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
    handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;
    handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
    GPIO_Config(&handlerPinTx);

    /* Configurando los pines sobre los que funciona el USART2 (RX) */
    handlerPinRx.pGPIOx = GPIOA;
    handlerPinRx.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
    handlerPinRx.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    handlerPinRx.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
    handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLDOWN;
    handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
    handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode = AF7;
    GPIO_Config(&handlerPinRx);

    // Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
    handlerUsart2.ptrUSARTx = USART2;
    handlerUsart2.USART_Config.USART_baudrate = USART_BAUDRATE_115200;
    handlerUsart2.USART_Config.USART_datasize = USART_DATASIZE_8BIT;
    handlerUsart2.USART_Config.USART_parity = USART_PARITY_NONE;
    handlerUsart2.USART_Config.USART_stopbits = USART_STOPBIT_1;
    handlerUsart2.USART_Config.USART_mode = USART_MODE_RXTX;
    handlerUsart2.USART_Config.USART_enableInterrupt = USART_RX_INTERRUP_ENABLE;

    // Cargamos la configuración del USART
    USART_Config(&handlerUsart2);


	/* Configurando los pines sobre los que funciona el USART1 (TX) */
	handlerPinTx1.pGPIOx 							= GPIOA;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinNumber		= PIN_9;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx1.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx1);

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	handlerPinRx1.pGPIOx 							= GPIOA;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinNumber		= PIN_10;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx1.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx1);

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	handlerUsart1.ptrUSARTx 					= USART1;
	handlerUsart1.USART_Config.USART_baudrate	= USART_BAUDRATE_115200;
	handlerUsart1.USART_Config.USART_datasize	= USART_DATASIZE_8BIT;
	handlerUsart1.USART_Config.USART_parity		= USART_PARITY_NONE;
	handlerUsart1.USART_Config.USART_stopbits	= USART_STOPBIT_1;
	handlerUsart1.USART_Config.USART_mode		= USART_MODE_RXTX;
	handlerUsart1.USART_Config.USART_enableInterrupt = USART_RX_INTERRUP_ENABLE ;

	// Cargamos la configuración del USART
	USART_Config(&handlerUsart1);






    // Configurando el Timer2 para que funcione con el blinky
    handlerStateTimer.ptrTIMx = TIM2;
    handlerStateTimer.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
    handlerStateTimer.TIMx_Config.TIMx_speed = BTIMER_SPEED_1ms;
    handlerStateTimer.TIMx_Config.TIMx_period = 250;

    // Cargamos la configuración del timer
    BasicTimer_Config(&handlerStateTimer);

    // Activamos el TIM2
    starTimer(&handlerStateTimer);
}

/* Callback del Timer2 - Hacemos un blinky... */
void BasicTimer2_Callback(void) {
    handlerStateLed.pGPIOx->ODR ^= GPIO_ODR_OD5; // Encendido y apagado StateLED
}


/* Callback relacionado con la recepción del USART1 (ESP-05) */
void usart1_Callback(void) {
    // Leer el dato recibido
    uint8_t receivedData = USART_ReceiveData(&handlerUsart1);

    // Almacenar el dato en el bufferData1
    static uint8_t bufferIndex1 = 0;
    bufferData1[bufferIndex1] = receivedData;
    bufferIndex1++;

    // Verificar si se ha recibido el fin de línea '\n'
    if (receivedData == '\n') {
        // Agregar el terminador nulo al final de la cadena
        bufferData1[bufferIndex1] = '\0';
        bufferIndex1 = 0;
        // Enviar el dato recibido a través del USART2
        USART_TransmitData(&handlerUsart2, receivedData);
    }
}

/* Callback relacionado con la recepción del USART2 (PC) */
void usart2_Callback(void) {
    // Leer el dato recibido
    uint8_t receivedData = USART_ReceiveData(&handlerUsart2);

    // Procesar el dato recibido según tus necesidades
    // Por ejemplo, puedes imprimir el dato recibido
    printf("Received data from PC: %c\n", receivedData);

    // Realizar cualquier otra operación adicional que desees
    // ...

    // Puedes agregar más código aquí según tus necesidades

    // Enviar el dato recibido a través del USART1
    USART_TransmitData(&handlerUsart1, receivedData);
}















