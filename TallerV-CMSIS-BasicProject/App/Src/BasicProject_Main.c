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
 * @brief          : The basic solution of a project with external libraries.
 ******************************************************************************
 * -
 * -
 * -
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
// Definicion de los handlers necesarios
GPIO_Handler_t handlerStateLed = {0};
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Timer encargado del Blinky
BasicTimer_Handler_t handlerStateTimer = {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};

uint8_t rxData = 0;
char bufferData[64];

// Variables a enviar a través del USART2
unsigned int numeroGrande = 1682539;

/* Definición de prototipos de funciones */
void InitSystem(void);

/**
 * Funcion principal del programa.
 * Esta funcion es el corazón del programa!!
 *
 * */
int main(void) {

	// Inicializamos todos los elementos del sistema
	InitSystem();

	/* Loop forever */
	while (1) {

		// El sistema siempre está verificando si el valor de rxData ha cambiado
        // (lo cual sucede en la ISR de la recepcion (RX).
        // Si este valor deja de ser '\0' significa que se recibio un caracter
        // por lo tanto entra en el bloque if para analizar que se recibio
		if(rxData != '\0'){
			// Imprimimos el caracter recibido (ECO)
            writeChar(&handlerUsart2, rxData);

			if(rxData == 's'){

				// ****** // ESCRIBA AQUI SU CODIGO // ****** //

				// Función SprintF paa convertir a strig una cadena y poder enviarla
				//Se parte el strig
				sprintf(bufferData, "Hola mundo%u \n", numeroGrande);
				writeMsg(&handlerUsart2, bufferData);
				}


			// Bajamos la bandera
			rxData = '\0';
		}
	}

	return 0;
}

/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void InitSystem(void){

	// Configurando el pin para el Led_Blinky
	handlerStateLed.pGPIOx 								= GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber		= PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;

	// Cargamos la configuración del Pin del led de estado
	GPIO_Config(&handlerStateLed);

	// Llevamos el Led a un estado de encendido
	GPIO_WritePin(&handlerStateLed, RESET);

	/* Configurando los pines sobre los que funciona el USART2 (TX) */
	handlerPinTx.pGPIOx 							= GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_2;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	handlerPinRx.pGPIOx 							= GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	handlerUsart2.ptrUSARTx 					= USART2;
	handlerUsart2.USART_Config.USART_baudrate	= USART_BAUDRATE_115200;
	handlerUsart2.USART_Config.USART_datasize	= USART_DATASIZE_8BIT;
	handlerUsart2.USART_Config.USART_parity		= USART_PARITY_NONE;
	handlerUsart2.USART_Config.USART_stopbits	= USART_STOPBIT_1;
	handlerUsart2.USART_Config.USART_mode		= USART_MODE_RXTX;
	handlerUsart2.USART_Config.USART_enableIntRx = USART_RX_INTERRUP_ENABLE ;

	// Cargamos la configuración del USART
	USART_Config(&handlerUsart2);

	// Configurando el Timer2 para que funcione con el blinky
	handlerStateTimer.ptrTIMx 						= TIM2;
	handlerStateTimer.TIMx_Config.TIMx_mode			= BTIMER_MODE_UP;
	handlerStateTimer.TIMx_Config.TIMx_speed		= BTIMER_SPEED_1ms;
	handlerStateTimer.TIMx_Config.TIMx_period		= 250;

	// Cargamos la configuración del timer
	BasicTimer_Config(&handlerStateTimer);

	// Activamos el TIM2
	starTimer(&handlerStateTimer);

}

/* Callback del Timer2 - Hacemos un blinky... */
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerStateLed);		// Encendido y apagado StateLED
}

/* Callback relacionado con la recepción del USART2
 * El puerto es leido en la ISR (para bajar la bandera de la interrupción)
 * El caracter que se lee es devuelto por la función getRxData
 */
void usart2Rx_Callback(void){
	// Leemos el valor del registro DR, donde se almacena el dato que llega.
	// Esto además debe bajar la bandera de la interrupción
	rxData = getRxData();
}
