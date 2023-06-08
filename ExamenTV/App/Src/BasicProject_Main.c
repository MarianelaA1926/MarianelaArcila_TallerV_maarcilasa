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

#include <stdint.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "USARTxDriver.h"
#include "PllDriver.h"
// Definicion de los handlers necesarios
GPIO_Handler_t handlerStateLed = {0};
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Timer encargado del Blinky
BasicTimer_Handler_t handlerStateTimer = {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};

//Cambiamos la frecuencia del miro
CLOCK_Handler_t handlerFrequency = {0};
CLOCK_Handler_t handlerPllMCO = {0};
GPIO_Handler_t handlerMCO1Pin = {0};

uint8_t txData = 0;
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
		//Se envia un mensaje
		sprintf(bufferData, "Hola mundo%u \n", numeroGrande);
		writeMsg(&handlerUsart2, bufferData);




	}

	return 0;
}

/*
 * Funcion encargada de la inicializacion de los elementos del sistema
 */
void InitSystem(void){


	//--------------PLL-----------------------------------------------
	// Se configura el pin A8 para que por este salga la frecuencia del reloj principal
	//Esto es opcional
	handlerMCO1Pin.pGPIOx = GPIOA;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	// Se carga la configuracion del pin A8
	GPIO_Config(&handlerMCO1Pin);

	// Se configura los parametros para la frecuencia
	handlerPllMCO.CLOCK_Config.frequency = MCU_FREQUENCY_80MHz;
	frequencyClock(&handlerPllMCO);
	configPll(&handlerPllMCO);




//__________________________________________________________________________


	// Configurando el Timer2 para que funcione con el blinky
	handlerStateTimer.ptrTIMx 						= TIM2;
	handlerStateTimer.TIMx_Config.TIMx_mode			= BTIMER_MODE_UP;
	handlerStateTimer.TIMx_Config.TIMx_speed		= BTIMER_80SPEED_1ms;
	handlerStateTimer.TIMx_Config.TIMx_period		= 250;


	// Cargamos la configuración del timer
	BasicTimer_Config(&handlerStateTimer);

	// Activamos el TIM2
	starTimer(&handlerStateTimer);

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
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_9;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	handlerPinRx.pGPIOx 							= GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_10;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	handlerUsart2.ptrUSARTx 							= USART1;
	handlerUsart2.USART_Config.MCU_frecuency  		    = USART_MCU_FREQUENCY_16MHz;
	handlerUsart2.USART_Config.USART_baudrate			= USART_BAUDRATE_115200;
	handlerUsart2.USART_Config.USART_datasize			= USART_DATASIZE_8BIT;
	handlerUsart2.USART_Config.USART_parity				= USART_PARITY_NONE;
	handlerUsart2.USART_Config.USART_stopbits			= USART_STOPBIT_2;
	handlerUsart2.USART_Config.USART_mode				= USART_MODE_TX;
	handlerUsart2.USART_Config.USART_enableInterrupt 	= USART_TX_INTERRUP_ENABLE;
	handlerUsart2.USART_Config.USART_interruptTx 		= USART_TXE_INTERRUP_EVENT;

	// Cargamos la configuración del USART
	USART_Config(&handlerUsart2);





}

/* Callback del Timer2 - Hacemos un blinky... */
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerStateLed);		// Encendido y apagado StateLED
}

/* Callback relacionado con la recepción del USART2
 * El puerto es leido en la ISR (para bajar la bandera de la interrupción)
 * El caracter que se lee es devuelto por la función getRxData
 */
void usart1_Callback(void){
	// Leemos el valor del registro DR, donde se almacena el dato que llega.
	// Esto además debe bajar la bandera de la interrupción
	rxData = getTxData();
}
