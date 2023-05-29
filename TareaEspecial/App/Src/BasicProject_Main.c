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
#include "I2CDriver.h"


// Definicion de los handlers necesarios
GPIO_Handler_t handlerStateLed = {0};
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Timer encargado del Blinky
BasicTimer_Handler_t handlerStateTimer = {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart1 = {0};

//Cambiamos la frecuencia del miro
PLL_Handler_t handlerFrequency = {0};
PLL_Handler_t handlerPll = {0};
GPIO_Handler_t handlerMCO1Pin = {0};

uint8_t txData = 0;
uint8_t rxData = 0;
char bufferData[64];


// I2C acelerometro
USART_Handler_t handlerUsart2 = {0};
GPIO_Handler_t handlerI2cSDA = {0};
GPIO_Handler_t handlerI2cSCL = {0};
I2C_Handler_t handlerAccelerometer = {0};
uint8_t i2cBuffer = 0;
// Direcciones de memora del acelerometro MPU 6050

// Dirección del dispositivo ADXL354
#define ADXL354_SLAVE_ADDRESS 0x53

// Registro de configuración del ADXL354
#define ADXL354_CONFIG_REG 0x2D

// Valor de configuración para el registro de configuración
#define ADXL354_CONFIG_VALUE 0x01

// Variables a enviar a través del USART2
unsigned int numeroGrande = 1682539;

/* Definición de prototipos de funciones */
void InitSystem(void);
void ADXL354_Config();
/**
 * Funcion principal del programa.
 * Esta funcion es el corazón del programa!!
 *
 * */
int main(void) {

	// Inicializamos todos los elementos del sistema
	InitSystem();
	// Función para configurar el ADXL354 como esclavo
	// Enviar la configuración al ADXL354
	i2c_writeSingleRegister(&handlerAccelerometer,  ADXL354_CONFIG_REG, ADXL354_CONFIG_VALUE);

	/* Loop forever */
	while (1) {
		//Se envia un mensaje de prueba para comprobar la configuracion TX
		//if(rxData == 'x'){
			//sprintf(bufferData, "Hola mundo%u \n", numeroGrande);
			//writeMsg(&handlerUsart1, bufferData);
		//}

	//	sprintf(bufferData, "Axis X data (r)\n");
		//writeMsg(&handlerUsart2, bufferData);
		/*	uint8_t AccelX_low = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_XOUT_L);
		uint8_t AccelX_high = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_XOUT_H);
		int16_t AccelX = AccelX_high <<8 | AccelX_low;
		sprintf(bufferData, "AccelX = %d\n", (int)AccelX);
		writeMsg(&handlerUsart2, bufferData);
		rxData = '\0';

		else if(rxData == 'y'){
			sprintf(bufferData, "Axis Y data (r)\n");
			writeMsg(&handlerUsart2, bufferData);
			uint8_t AccelY_low = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_YOUT_L);
			uint8_t AccelY_high = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_YOUT_H);
			int16_t AccelY = AccelY_high << 8 | AccelY_low;

			sprintf(bufferData, "AccelY= %d\n", (int)AccelY);
			writeMsg(&handlerUsart2, bufferData);
			rxData = '\0';
		}
		else if(rxData == 'z'){

			sprintf(bufferData, "Axis Z data (r)\n");
			writeMsg(&handlerUsart2, bufferData);
			uint8_t AccelZ_low = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_ZOUT_L);
			uint8_t AccelZ_high = i2c_readSingleRegister(&handlerAccelerometer, ACCEL_ZOUT_H);
			int16_t AccelZ = AccelZ_high << 8 | AccelZ_low;

			sprintf(bufferData, "AccelZ = %d\n", (int)AccelZ);
			writeMsg(&handlerUsart2, bufferData);
			rxData = '\0';
		}*/


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
/*	handlerMCO1Pin.pGPIOx = GPIOA;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerMCO1Pin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	// Se carga la configuracion del pin A8
	GPIO_Config(&handlerMCO1Pin);

	// Se configura los parametros para la frecuencia
	handlerFrequency. PLL_Config.frequency = MCU_FREQUENCY_80MHz;


	// se carga la configuracion de los parametros de la nueva frecuencia

	frequency(&handlerPll);
	//configPll(&handlerPll);







	*/
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
/*	handlerPinTx.pGPIOx 							= GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_9;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);*/

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	/*handlerPinRx.pGPIOx 							= GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_10;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);*/

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	/*handlerUsart1.ptrUSARTx 							= USART1;
	handlerUsart1.USART_Config.MCU_frecuency  		    = USART_MCU_FREQUENCY_16MHz;
	handlerUsart1.USART_Config.USART_baudrate			= USART_BAUDRATE_115200;
	handlerUsart1.USART_Config.USART_datasize			= USART_DATASIZE_8BIT;
	handlerUsart1.USART_Config.USART_parity				= USART_PARITY_NONE;
	handlerUsart1.USART_Config.USART_stopbits			= USART_STOPBIT_2;
	handlerUsart1.USART_Config.USART_mode				= USART_MODE_TX;
	handlerUsart1.USART_Config.USART_enableInterrupt 	= USART_TX_INTERRUP_ENABLE;
	handlerUsart1.USART_Config.USART_interruptTx 		= USART_TXE_INTERRUP_EVENT;

	// Cargamos la configuración del USART
	USART_Config(&handlerUsart1);*/


//____________________________________________________________________________________________________________________________________________


	//------------------------Acelerometro-----------------------------------------------------------
	// Se configura los pines sobre los que trabaj el I2C2
	handlerI2cSCL.pGPIOx = GPIOA;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinAltFunMode = AF4;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	// Se garga la configuracion del I2C
	GPIO_Config(&handlerI2cSCL);

	// Se configura los pines sobre los que trabaj el I2C2
	handlerI2cSDA.pGPIOx = GPIOC;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinAltFunMode = AF4;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinNumber = PIN_9;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	// Se garga la configuracion del I2C
	GPIO_Config(&handlerI2cSDA);

	// Se configura el Acelerometro
	handlerAccelerometer.ptrI2Cx = I2C3;
	handlerAccelerometer.modeI2C = I2C_MODE_FM;
	handlerAccelerometer.slaveAddress = ADXL354_SLAVE_ADDRESS;

	i2c_config(&handlerAccelerometer);

// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)

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

	handlerUsart2.ptrUSARTx 							= USART2;
	handlerUsart2.USART_Config.MCU_frecuency  		    = USART_MCU_FREQUENCY_16MHz;
	handlerUsart2.USART_Config.USART_baudrate			= USART_BAUDRATE_115200;
	handlerUsart2.USART_Config.USART_datasize			= USART_DATASIZE_8BIT;
	handlerUsart2.USART_Config.USART_parity				= USART_PARITY_NONE;
	handlerUsart2.USART_Config.USART_stopbits			= USART_STOPBIT_2;
	handlerUsart2.USART_Config.USART_mode				= USART_MODE_RXTX;
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
	txData = getTxData();
}

void usart2_Callback(void){
	// Leemos el valor del registro DR, donde se almacena el dato que llega.
	// Esto además debe bajar la bandera de la interrupción
	rxData = getTxData();
}
