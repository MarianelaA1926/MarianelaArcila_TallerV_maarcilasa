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
#include "PwmDriver.h"
#include "LcdDriver.h"


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

//---PWM
PWM_Handler_t PWM_HandlerX = {0};
GPIO_Handler_t PWM_HandlerXPin = {0};

PWM_Handler_t PWM_HandlerY = {0};
GPIO_Handler_t PWM_HandlerYPin = {0};

PWM_Handler_t PWM_HandlerZ = {0};
GPIO_Handler_t PWM_HandlerZPin = {0};
BasicTimer_Handler_t PWM = {0};


uint16_t duttyR = 50;
uint16_t duttyG = 50;
uint16_t duttyB = 50;
uint16_t dutty = 50;

uint8_t contador = 10;

uint8_t banderaAcelerometro = 0;

//---LCD
GPIO_Handler_t handlerLCDI2cSDA = {0};
GPIO_Handler_t handlerLCDI2cSCL = {0};
LCD_Handler_t handlerLCD = {0};
uint8_t commandLCD = 0;
uint8_t commandLED = 0;
uint8_t caso = 0;
uint64_t tic = 0;




// Dirección del dispositivo ADXL354
#define ADXL354_SLAVE_ADDRESS 0x53

#define ACCEL_XOUT_H   50
#define ACCEL_XOUT_L   51
#define ACCEL_YOUT_H   52
#define ACCEL_YOUT_L   53
#define ACCEL_ZOUT_H   54
#define ACCEL_ZOUT_L   55

#define ADXL354_CONFIG_REG 0x2D

// Variables a enviar a través del USART1
unsigned int numeroGrande = 1682539;


//LCD
// Direccion de memoria del LCD LCM2004A1
#define LCD_ADRESS   0b100111


void initSystem (void);

void parseCommands(char *ptrBufferReception);

void LCDBienvenida(void);





/* Definición de prototipos de funciones */
void InitSystem(void);
void LCDBienvenida(void);





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


		writeChar(&handlerUsart1, 's');

		//Prueba LSD
		LCDBienvenida();


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
	handlerFrequency. PLL_Config.frequency = MCU_FREQUENCY_80MHz;
	// se carga la configuracion de los parametros de la nueva frecuencia
	frequency(&handlerPll);

	//Se configura el MCO




	//configPll(&handlerPll);








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
	handlerPinTx.pGPIOx 							= GPIOC;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_6;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	handlerPinRx.pGPIOx 							= GPIOC;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_7;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	handlerUsart1.ptrUSARTx 							= USART1;
	handlerUsart1.USART_Config.MCU_frecuency  		    = USART_MCU_FREQUENCY_80MHz;
	handlerUsart1.USART_Config.USART_baudrate			= USART_BAUDRATE_115200;
	handlerUsart1.USART_Config.USART_datasize			= USART_DATASIZE_8BIT;
	handlerUsart1.USART_Config.USART_parity				= USART_PARITY_NONE;
	handlerUsart1.USART_Config.USART_stopbits			= USART_STOPBIT_2;
	handlerUsart1.USART_Config.USART_mode				= USART_MODE_TX;
	handlerUsart1.USART_Config.USART_enableInterrupt 	= USART_TX_INTERRUP_ENABLE;


	// Cargamos la configuración del USART
	USART_Config(&handlerUsart1);


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



//________________________________________________________________________________________________________



//------------------------------PWM-------------------------------------------------------------

	// Configuracion para el eje X
	PWM_HandlerXPin.pGPIOx = GPIOB;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_OPENDRAIN;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	PWM_HandlerXPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	GPIO_Config(&PWM_HandlerZPin);

	PWM_HandlerX.ptrTIMx = TIM3;
	PWM_HandlerX.config.channel = PWM_CHANNEL_4;
	PWM_HandlerX.config.dutyCycle = 15;
	PWM_HandlerX.config.period = 30;
	PWM_HandlerX.config.prescaler = 1600;

	pwm_Config(&PWM_HandlerX);
	//startPwmSignal(&PWM_HandlerR);
	enableOutput(&PWM_HandlerX);

	//Configuracion para el eje Y
	PWM_HandlerYPin.pGPIOx = GPIOB;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_OPENDRAIN;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	PWM_HandlerYPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	GPIO_Config(&PWM_HandlerYPin);

	PWM_HandlerY.ptrTIMx = TIM3;
	PWM_HandlerY.config.channel = PWM_CHANNEL_3;
	PWM_HandlerY.config.dutyCycle = 15;
	PWM_HandlerY.config.period = 30;
	PWM_HandlerY.config.prescaler = 1600;

	pwm_Config(&PWM_HandlerY);
	//startPwmSignal(&PWM_HandlerG);
	enableOutput(&PWM_HandlerY);

	// Configuracion para el Eje Z
	PWM_HandlerZPin.pGPIOx = GPIOB;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinAltFunMode = AF2;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_OPENDRAIN;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	PWM_HandlerZPin.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;

	GPIO_Config(&PWM_HandlerZPin);

	PWM_HandlerZ.ptrTIMx = TIM3;
	PWM_HandlerZ.config.channel = PWM_CHANNEL_1;
	PWM_HandlerZ.config.dutyCycle = 15;
	PWM_HandlerZ.config.period = 30;
	PWM_HandlerZ.config.prescaler = 1600;

	pwm_Config(&PWM_HandlerZ);
	//startPwmSignal(&PWM_HandlerB);
	enableOutput(&PWM_HandlerZ);


//________________________________________________________________________________________________
	//------------------------------------LCD------------------------------------------------------

   	handlerLCDI2cSCL.pGPIOx                                 = GPIOB ;//puerto B para recepcion de datos
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinNumber          = PIN_6;//pin 6 activado para recepcion de datos
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_ALTFN ; // salida
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_OPENDRAIN;
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_PULLUP;
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
   	handlerLCDI2cSCL.GPIO_PinConfig.GPIO_PinAltFunMode      = AF4 ; // funcion alternativa

   	GPIO_Config(&handlerLCDI2cSCL);


   	//configuramos los pines sobre los que funciona el 	LCD
   	handlerLCDI2cSDA.pGPIOx                                 = GPIOB ;//puerto B para recepcion de datos
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinNumber          = PIN_7;//pin 7 activado para recepcion de datos
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinMode            = GPIO_MODE_ALTFN ; // salida
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinOPType          = GPIO_OTYPE_OPENDRAIN;
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinPuPdControl     = GPIO_PUPDR_PULLUP;
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinSpeed           = GPIO_OSPEED_FAST;
   	handlerLCDI2cSDA.GPIO_PinConfig.GPIO_PinAltFunMode      = AF4 ; // funcion alternativa

   	GPIO_Config(&handlerLCDI2cSDA);

   	handlerLCD.ptrLCDx         = I2C1 ;//
   	handlerLCD.modeLCD         = I2C_MODE_SM;//I2C_MODE_FM
   	handlerLCD.slaveAddressLCD = LCD_ADRESS;//0b0100111

   	LCD_Config(&handlerLCD);


}



void LCDBienvenida(void){
	LCD_setCursor(&handlerLCD,0,0,LCD_ADRESS);
	LCD_sendSTR(&handlerLCD,"Bienvenida M",LCD_ADRESS);

	LCD_setCursor(&handlerLCD,2,0,LCD_ADRESS);
	LCD_sendSTR(&handlerLCD,"Escribe lo que quieras ",LCD_ADRESS);

			for (int i=0;i<10000;i++);

}

/* Callback del Timer2 - Hacemos un blinky... */
void BasicTimer2_Callback(void){
	GPIOxTooglePin(&handlerStateLed);		// Encendido y apagado StateLED
}

/* Callback relacionado con la recepción del USART2
 * El puerto es leido en la ISR (para bajar la bandera de la interrupción)
 * El caracter que se lee es devuelto por la función getRxData
 */
/*void usart1_Callback(void){
	// Leemos el valor del registro DR, donde se almacena el dato que llega.
	// Esto además debe bajar la bandera de la interrupción
	txData = getTxData();
}
*/
void usart2_Callback(void){
	// Leemos el valor del registro DR, donde se almacena el dato que llega.
	// Esto además debe bajar la bandera de la interrupción
	rxData = getTxData();
}


