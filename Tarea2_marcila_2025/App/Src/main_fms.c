/*
 * main_fms.c
 *
 *  Created on: 24/05/2025
 *      Author: marianela
 */

/* ******************************************************************************
 * @file           : main.c
 * @author         : doramirezag
 * @brief          : Desarrollo del driver para el USART
 ******************************************************************************
 * - Implementación de las librerías CMSIS
 * - Migración del GPIO driver
 * - Desarrollo de las librerías para el manejo de USART
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "stm32_assert.h"
#include "USARTxDriver.h"
#include "SysTickDriver.h"
#include "AdcDriver.h"

//-------------------------------------------------Led State---------------------------------------------//
// Status LED control variables
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};

//---------------------------------------------------Reset Button-----------------------------------------------
GPIO_Handler_t handlerResetButton = {0};
EXTI_Config_t handlerExtiButton;

//-----------------------------------------------RGB LED--------------------------------------------------------

GPIO_Handler_t handlerLedRed = {0};
GPIO_Handler_t handlerLedGreen = {0};
GPIO_Handler_t handlerLedBlue = {0};

GPIO_Handler_t handlerButtonSW = {0};
EXTI_Config_t handlerButtonSWExti = {0};

uint8_t currentState;
uint8_t buttonPressed;
uint8_t currentColor;

//----------------------------------------- 7 SEGMENTOS DEL DISPLAY----------------------------------------------------

//------------------------------------------ DIGITOS DISPLAY ------------------
GPIO_Handler_t handlerDigit4 = {0};
GPIO_Handler_t handlerDigit3 = {0};
GPIO_Handler_t handlerDigit2 = {0};
GPIO_Handler_t handlerDigit1 = {0};
// ---------------------------------------- 7 SEGMENTOS-------------------------
GPIO_Handler_t  handlerPunto   = {0};
GPIO_Handler_t  handlerPinLedA = {0};
GPIO_Handler_t  handlerPinLedB = {0};
GPIO_Handler_t  handlerPinLedC = {0};
GPIO_Handler_t  handlerPinLedD = {0};
GPIO_Handler_t  handlerPinLedE = {0};
GPIO_Handler_t  handlerPinLedF = {0};
GPIO_Handler_t  handlerPinLedG = {0};

//--------------------------------------TIMER INTERRUPCION-----------------------
BasicTimer_Handler_t  handlerTimerSG = {0};
GPIO_Handler_t handlerEncoderCLK = {0};
EXTI_Config_t handlerExtiEncoderCK   = {0};
GPIO_Handler_t handlerEncoderDT = {0};
EXTI_Config_t handlerExtiEncoderDT = {0};

int16_t contadorGlobal = 0;
uint8_t arrayDisplay[4] = {'N', 'N', 'N', 'N'};
uint8_t basic_timer_amount = 0;
//----------------------------------------ADC------------------------------------------------------------------------------
ADC_Config_t adcConfig = {0};

GPIO_Handler_t handlerPinAdc = {0};

// --- Variables globales del sistema ---
uint32_t current_sampling_freq = 44100;      // Frecuencia de muestreo
uint16_t current_fft_size = 1024;            // Tamaño FFT
char tx_buffer[256];                         // Buffer de transmisión por UART

#define FFT_MAX_SIZE 2048

// Buffers de señal
uint16_t adcRawData_data[FFT_MAX_SIZE];           // Señal muestreada
uint16_t fftMagnitudes[FFT_MAX_SIZE / 2];    // Magnitudes de la FFT (sin float)

//-----------------------------------IMPRIMIR EN CONSOLA----------------------------
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};
char tiempoMs = {0};

#define MAX_BUFFER_SIZE 32
#define BTIMER_SPEED_1us			16

char rxBuffer[MAX_BUFFER_SIZE];
uint8_t rxIndex = 0;
uint8_t commandReady = 0;
//----------------------------Variables  comandos usart---------------------------------------------
char rxBuffer[MAX_BUFFER_SIZE];

float frecuencia = 0;  // Hz inicial → 250 ms
uint8_t ledEnabled = 1;  // Controla si el parpadeo está activo o no



//-------------------------------------------------------FSM----------------------------------------------------

// Definición de los estados de la FSM
typedef enum {

	STATE_IDLE,
	STATE_BLINKY,
	STATE_RGB

	// Agrega aquí otros estados que necesites
} State_t;


// Definicion Colores Led
typedef enum {
    COLOR_OFF = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_CYAN,    // azul + verde
    COLOR_MAGENTA, // azul + rojo
    COLOR_YELLOW,  // rojo + verde
    COLOR_WHITE    // rojo + verde + azul
} Color_t;


// Aquí la defines como **variable global**
volatile Color_t currentColor = COLOR_OFF;


/* Pin configuration function  */
void initSystem(void){

	// --------------------------------- State Led--------------------------------------------------//
	handlerStateLed.pGPIOx = 											GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = 						GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = 						GPIO_OSPEED_FAST;

	GPIO_Config(&handlerStateLed);



	//------------------------------------------------Reset Butom-----------------------------------------------------


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerResetButton.pGPIOx	= 										GPIOA;
	handlerResetButton.GPIO_PinConfig.GPIO_PinNumber = 					PIN_8;
	handlerResetButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerResetButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerResetButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);


	//------------------------------------------------RGB LED-----------------------------------------------------


	handlerLedRed.pGPIOx = GPIOB;
	handlerLedRed.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerLedRed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedRed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedRed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedRed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedRed);

	// LED Verde (A4)
	handlerLedGreen.pGPIOx = GPIOB;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedGreen);

	// LED Azul (A5)
	handlerLedBlue.pGPIOx = GPIOB;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlue);

    //----------------------------------------------------Encoder------------------------------------------------------
	// Botón SW en A1
	handlerButtonSW.pGPIOx = GPIOA;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinNumber = PIN_10;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	GPIO_Config(&handlerButtonSW);

	// Configuración de la EXTI para A1
	handlerButtonSWExti.pGPIOHandler = &handlerButtonSW;
	handlerButtonSWExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE; // o RISING_EDGE
	extInt_Config(&handlerButtonSWExti);

//----------------------------------------- 7 SEGMENTOS DEL DISPLAY----------------------------------------------------

	//--------------------------- DIGITOS DISPLAY (TRANSISTORES)-------------

	handlerDigit4.pGPIOx = GPIOA;
	handlerDigit4.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	handlerDigit4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit4);

	handlerDigit3.pGPIOx = GPIOB;
	handlerDigit3.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
	handlerDigit3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit3);

	handlerDigit2.pGPIOx = GPIOB;
	handlerDigit2.GPIO_PinConfig.GPIO_PinNumber = PIN_15;
	handlerDigit2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit2);

	handlerDigit1.pGPIOx = GPIOB;
	handlerDigit1.GPIO_PinConfig.GPIO_PinNumber = PIN_14;
	handlerDigit1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit1);



	//-----------------------------------------------------------LEDS---------------------------------------------------
	handlerPunto.pGPIOx = 												GPIOA;
	handlerPunto.GPIO_PinConfig.GPIO_PinNumber =						PIN_12;
	handlerPunto.GPIO_PinConfig.GPIO_PinMode  =							GPIO_MODE_OUT;
	handlerPunto.GPIO_PinConfig.GPIO_PinOPType =         				GPIO_OTYPE_PUSHPULL;
	handlerPunto.GPIO_PinConfig.GPIO_PinPuPdControl =					GPIO_PUPDR_NOTHING;
	handlerPunto.GPIO_PinConfig.GPIO_PinSpeed=				            GPIO_OSPEED_FAST;

	//Ahora se carga la configuración
	GPIO_Config(&handlerPunto);

	handlerPinLedA.pGPIOx=                                  			GPIOB;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinNumber=           			PIN_6;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinMode=             			GPIO_MODE_OUT;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinOPType=          				GPIO_OTYPE_PUSHPULL;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinPuPdControl=    				GPIO_PUPDR_NOTHING;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinSpeed=            			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedA);


	handlerPinLedB.pGPIOx=                                   			GPIOA;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinNumber=            			PIN_7;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinPuPdControl=      			GPIO_PUPDR_NOTHING;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;

	GPIO_Config(&handlerPinLedB);

	handlerPinLedC.pGPIOx=                                   			GPIOC;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinNumber=            			PIN_5;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedC);


	handlerPinLedD.pGPIOx=                                   			GPIOA;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinNumber=            			PIN_11;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedD);


	handlerPinLedE.pGPIOx=                                   			GPIOA;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinNumber=            			PIN_6;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedE);


	handlerPinLedF.pGPIOx=                                   			GPIOB;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinNumber=            			PIN_12;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedF);


	handlerPinLedG.pGPIOx=                                  			GPIOB;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinNumber=            			PIN_9;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedG);




	//------------------------------------------ENCODER---------------------------------------------------------------
	//---CLK-----------------------------------------------------

	handlerEncoderCLK.pGPIOx	= 										GPIOA;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinNumber = 				    PIN_1;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinPuPdControl = 			    GPIO_PUPDR_PULLUP;

	//ExtiConfiguration
	handlerExtiEncoderCK.pGPIOHandler= 									&handlerEncoderCLK;
	handlerExtiEncoderCK.edgeType = 									EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderCK);


	//-----DT--------------------------------------------------------


	handlerEncoderDT.pGPIOx	= 										GPIOA;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinNumber = 				PIN_0;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;

	//ExtiConfiguration
	handlerExtiEncoderDT.pGPIOHandler= 									&handlerEncoderDT;
	handlerExtiEncoderDT.edgeType = 									EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderDT);


	//------------------------TIMER-----------------------------------------
	handlerTimerSG.ptrTIMx = 										TIM3;
	handlerTimerSG.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerSG.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerSG.TIMx_Config.TIMx_period = 						5;
	handlerTimerSG.TIMx_Config.TIMx_interrupEnable=				BTIMER_DISABLE;
	// We load the configuration
	BasicTimer_Config(&handlerTimerSG);
	// We activate the TIM3
	starTimer(&handlerTimerSG);


//------------------------------------------------------------Imprimir consola-------------------------------------
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


	// We configure TIMER 2 as the counter for the stateLed
	handlerTimerStateLed.ptrTIMx = 										TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 						250;
	handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable=				BTIMER_ENABLE;

	BasicTimer_Config(&handlerTimerStateLed);

	// We activate the TIM2
	starTimer(&handlerTimerStateLed);

//--------------------------------------------------------------ADC-------------------------------------------------------


	adcConfig.channel = 10;  // Por ejemplo, canal 10 → PC0
	adcConfig.resolution = ADC_RESOLUTION_12_BIT;
	adcConfig.dataAlignment = ADC_ALIGNMENT_RIGHT;
	// ... otras configuraciones ...
	adc_Config(&adcConfig);  // Llama a tu driver




	handlerPinAdc.pGPIOx = GPIOC;
	handlerPinAdc.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
	handlerPinAdc.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ANALOG;
	handlerPinAdc.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;    // Opcional
	handlerPinAdc.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerPinAdc.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_LOW;         // No importa realmente
	handlerPinAdc.GPIO_PinConfig.GPIO_PinAltFunMode = 0;                  // No aplica

	GPIO_Config(&handlerPinAdc);

//_--------------------------------------------------------_ADC-------------------------------------------



}



//-------------------------------------------------------FSM function--------------------------------------------

void buildArrayDisplay()
{
	if (contadorGlobal >= 0 && contadorGlobal < 10) {
		arrayDisplay[0] = 'N';
		arrayDisplay[1] = 'N' ;
		arrayDisplay[2] = 'N';
		arrayDisplay[3] = contadorGlobal;

	} else if (contadorGlobal >= 10 && contadorGlobal < 100) {
		// Mostrar dos dígitos
		int unidades = contadorGlobal % 10;
		int decenas = contadorGlobal / 10;
		arrayDisplay[0] = 'N';
		arrayDisplay[1] = 'N';
		arrayDisplay[2] = decenas;
		arrayDisplay[3] = unidades;


	} else if (contadorGlobal >= 100 && contadorGlobal < 1000) {
		// Mostrar tres dígitos
		int unidades = contadorGlobal % 10;
		int decenas = (contadorGlobal / 10) % 10;
		int centenas = contadorGlobal / 100;
		arrayDisplay[0] = 'N';
		arrayDisplay[1] = centenas;
		arrayDisplay[2] = decenas;
		arrayDisplay[3] = unidades;


	} else if (contadorGlobal >= 1000 && contadorGlobal < 10000) {
		// Mostrar cuatro dígitos
		int unidades = contadorGlobal % 10;
		int decenas = (contadorGlobal / 10) % 10;
		int centenas = (contadorGlobal / 100) % 10;
		int millares = contadorGlobal / 1000;
		arrayDisplay[0] = millares;
		arrayDisplay[1] = centenas;
		arrayDisplay[2] = decenas;
		arrayDisplay[3] = unidades;

	}
}

void writeSegments(uint8_t number){

    switch (number) {

    case 0:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 1:
        GPIO_WritePin(&handlerPinLedA, SET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;	GPIO_Config(&handlerEncoderDT);

    case 2:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, SET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 3:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 4:
        GPIO_WritePin(&handlerPinLedA, SET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 5:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 6:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 7:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 8:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 9:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 'E':
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, SET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto,   RESET);
        break;

    case 'N':
    	GPIO_WritePin(&handlerPinLedA, SET);
		GPIO_WritePin(&handlerPinLedB, SET);
		GPIO_WritePin(&handlerPinLedC, SET);
		GPIO_WritePin(&handlerPinLedD, SET);
		GPIO_WritePin(&handlerPinLedE, SET);
		GPIO_WritePin(&handlerPinLedF, SET);
		GPIO_WritePin(&handlerPinLedG, SET);
		GPIO_WritePin(&handlerPunto,   SET);

    default:
        // Opcional: Manejar otros casos si es necesario.
        break;
    }
}

// Implementación de la FSM
void fsm(void) {
	switch (currentState) {

        case STATE_IDLE:
            // MCU en espera
            break;

        case STATE_BLINKY:	GPIO_Config(&handlerEncoderDT);
            GPIOxTooglePin(&handlerStateLed);
            currentState = STATE_IDLE;
            break;

        case STATE_RGB:
            // Avanza al siguiente color
        	writeSegments(0);
            currentColor++;
            if (currentColor > COLOR_WHITE){
                currentColor = COLOR_OFF;
            }

            // Configura los LEDs según el color actual
            switch(currentColor){
                case COLOR_RED:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    GPIO_WritePin(&handlerDigit4, SET);
                    GPIO_WritePin(&handlerDigit3, SET);
                    GPIO_WritePin(&handlerDigit2, SET);
                    GPIO_WritePin(&handlerDigit1, SET);
                    break;
                case COLOR_GREEN:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    GPIO_WritePin(&handlerDigit4, RESET);
                    GPIO_WritePin(&handlerDigit3, RESET);
                    GPIO_WritePin(&handlerDigit2, RESET);
                    GPIO_WritePin(&handlerDigit1, RESET);
                    break;
                case COLOR_BLUE:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_CYAN:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_MAGENTA:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_YELLOW:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break;
                case COLOR_WHITE:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_OFF:
                default:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break  ;
            }

            // Vuelve a estado IDLE
            currentState = STATE_IDLE;
            break;

        default:
            currentState = STATE_IDLE;
            break;
    }
}



void write_digit(int digit, uint8_t state){

	switch (digit) {
	case 1:
		GPIO_WritePin(&handlerDigit1, state);
	case 2:
		GPIO_WritePin(&handlerDigit2, state);
	case 3:
		GPIO_WritePin(&handlerDigit3, state);
	case 4:
		GPIO_WritePin(&handlerDigit4, state);
	default:
		break;

	}


}
//-------------------------------------------------------Main-----------------------------------------------------


void processCommand(void){
    if (!commandReady) return;

    commandReady = 0;

    if (strcmp(rxBuffer, "hola") == 0){
        writeMsg(&handlerUsart2, "hola marianela\r\n");


    } else if (strcmp(rxBuffer, "led on") == 0) {
        ledEnabled = 1;
        GPIO_WritePin(&handlerLedRed, SET);
        GPIO_WritePin(&handlerLedGreen, SET);
        GPIO_WritePin(&handlerLedBlue, SET);

        writeMsg(&handlerUsart2, "LED activado\r\n");
    } else if (strcmp(rxBuffer, "led off") == 0) {
        ledEnabled = 0;
        GPIO_WritePin(&handlerLedRed, RESET);
        GPIO_WritePin(&handlerLedGreen, RESET);
        GPIO_WritePin(&handlerLedBlue, RESET);   // Apaga el LED
        writeMsg(&handlerUsart2, "LED desactivado\r\n");
    }
    else if (strncmp(rxBuffer, "period ", 7) == 0) {
        uint16_t periodo = atoi(&rxBuffer[7]);

        if (periodo >= 10 && periodo <= 10000) {  // entre 10ms (100Hz) y 10s (0.1Hz)
            handlerTimerStateLed.TIMx_Config.TIMx_period = periodo;
            BasicTimer_Config(&handlerTimerStateLed);
            stopTimer(&handlerTimerStateLed); // Reinicio limpio
            starTimer(&handlerTimerStateLed);

            char msg[64];
            sprintf(msg, "Periodo actualizado a %d ms\r\n", periodo);
            writeMsg(&handlerUsart2, msg);
        } else {
            writeMsg(&handlerUsart2, "Periodo inválido (10 - 10000 ms)\r\n");
        }


    }

    else if (strncmp(rxBuffer, "sample ", 7) == 0) {
        uint32_t frecuencia = atoi(&rxBuffer[7]);

        if (frecuencia == 44100 || frecuencia == 48000 ||
            frecuencia == 96000 || frecuencia == 128000) {

            uint16_t periodo = 1000000 / frecuencia;

            // modificacion del  PSC para obtener valores cercanos (84 MHz → 1 µs)
            handlerTimerStateLed.TIMx_Config.TIMx_speed  = 84;  // PSC = 84 → 1 µs
            handlerTimerStateLed.TIMx_Config.TIMx_period = periodo;

            stopTimer(&handlerTimerStateLed);
            BasicTimer_Config(&handlerTimerStateLed);
            starTimer(&handlerTimerStateLed);

            char msg[64];
            sprintf(msg, "Frecuencia configurada: %lu Hz (ARR = %u)\r\n", frecuencia, periodo);
            writeMsg(&handlerUsart2, msg);
        } else {
            writeMsg(&handlerUsart2, "Frecuencia no soportada. Usa 44100, 48000, 96000 o 128000.\r\n");
        }
    }
    // --- Comandos de la FFT ---
    else if (strncmp(rxBuffer, "set fftsize ", 12) == 0) {
        int nuevoTam = atoi(&rxBuffer[12]);
        if (nuevoTam == 1024 || nuevoTam == 2048) {
            current_fft_size = nuevoTam;
            sprintf(tx_buffer, "Tamaño de FFT configurado a %d\r\n", nuevoTam);
        } else {
            sprintf(tx_buffer, "Tamaño inválido. Usa 1024 o 2048.\r\n");
        }
        writeMsg(&handlerUsart2, tx_buffer);
    }

    else if (strcmp(rxBuffer, "print raw") == 0) {
        for (int i = 0; i < current_fft_size; i++) {
            sprintf(tx_buffer, "%d\r\n", adcRawData_data[i]);
            writeMsg(&handlerUsart2, tx_buffer);
        }
    }

    else if (strcmp(rxBuffer, "print config") == 0) {
        sprintf(tx_buffer, "Frecuencia de muestreo: %.0f Hz\r\nTamaño de FFT: %d\r\n",  current_sampling_freq, current_fft_size);
        writeMsg(&handlerUsart2, tx_buffer);
    }

    else if (strcmp(rxBuffer, "print fft") == 0) {
        for (int i = 0; i < current_fft_size / 2; i++) {
            float freq = current_sampling_freq * i / current_fft_size;
            sprintf(tx_buffer, "%.2f Hz : %.2f\r\n", freq, adcRawData_data[i]);
            writeMsg(&handlerUsart2, tx_buffer);
        }
    }
    else if (strncmp(rxBuffer, "set fftsize ", 12) == 0) {
        int nuevoTam = atoi(&rxBuffer[12]);
        if (nuevoTam == 1024 || nuevoTam == 2048) {
            current_fft_size = nuevoTam;
            sprintf(tx_buffer, "FFT configurada a %d\r\n", nuevoTam);
        } else {
            sprintf(tx_buffer, "Tamaño inválido. Usa 1024 o 2048.\r\n");
        }
        writeMsg(&handlerUsart2, tx_buffer);
    }

    else if (strcmp(rxBuffer, "print raw") == 0) {
        for (int i = 0; i < current_fft_size; i++) {
            sprintf(tx_buffer, "%u\r\n", adcRawData_data[i]);
            writeMsg(&handlerUsart2, tx_buffer);
        }
    }

    else if (strcmp(rxBuffer, "print config") == 0) {
        sprintf(tx_buffer,
            "Frecuencia muestreo: %lu Hz\r\nTamaño FFT: %u\r\n",
            current_sampling_freq, current_fft_size);
        writeMsg(&handlerUsart2, tx_buffer);
    }

    else if (strcmp(rxBuffer, "print fft") == 0) {
        for (int i = 0; i < current_fft_size / 2; i++) {
            uint32_t freq_scaled = (i * current_sampling_freq * 100) / current_fft_size;
            sprintf(tx_buffer, "%lu.%02lu Hz : %u\r\n", freq_scaled / 100, freq_scaled % 100, fftMagnitudes[i]);
            writeMsg(&handlerUsart2, tx_buffer);
        }
    }

    else if (strcmp(rxBuffer, "print summary") == 0) {
        uint32_t suma = 0;
        for (int i = 0; i < current_fft_size; i++) suma += adcRawData_data[i];
        uint32_t offset = suma / current_fft_size;

        uint32_t magnitudMax = 0;
        uint16_t indexMax = 1;
        for (int i = 1; i < current_fft_size / 2; i++) {
            if (fftMagnitudes[i] > magnitudMax) {
                magnitudMax = fftMagnitudes[i];
                indexMax = i;
            }
        }

        uint32_t freq_scaled = (indexMax * current_sampling_freq * 100) / current_fft_size;

        uint64_t potencia = 0;
        for (int i = 0; i < current_fft_size / 2; i++) {
            potencia += (uint32_t)(fftMagnitudes[i]) * (uint32_t)(fftMagnitudes[i]);
        }

        sprintf(tx_buffer,
            "Frecuencia dominante: %lu.%02lu Hz\r\nPotencia: %llu\r\nOffset ADC: %lu\r\n",
            freq_scaled / 100, freq_scaled % 100, potencia, offset);
        writeMsg(&handlerUsart2, tx_buffer);
    }

    else if (strcmp(rxBuffer, "help") == 0) {
        sprintf(tx_buffer,
            "Comandos disponibles:\r\n"
            "- hola\r\n"
            "- led on / led off\r\n"
            "- period <ms> / periodo <ms>\r\n"
            "- sample <frecuencia>\r\n"
            "- set fftsize <1024|2048>\r\n"
            "- print raw\r\n"
            "- print config\r\n"
            "- print fft\r\n"
            "- print summary\r\n"
            "- help\r\n");
        writeMsg(&handlerUsart2, tx_buffer);
    }
}

void actualizarTimer(int freq){
    if (freq < 1) freq = 1;
    if (freq > 500) freq = 500;

    uint16_t newPeriod = 1000 / freq;

    // Detener el timer antes de reconfigurarlo
    TIM2->CR1 &= ~TIM_CR1_CEN;
    stopTimer(&handlerTimerStateLed);
    handlerTimerStateLed.ptrTIMx = TIM2;
    handlerTimerStateLed.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
    handlerTimerStateLed.TIMx_Config.TIMx_speed = BTIMER_SPEED_1ms;
    handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable = BTIMER_ENABLE;
    handlerTimerStateLed.TIMx_Config.TIMx_period = newPeriod;


    BasicTimer_Config(&handlerTimerStateLed);

    starTimer(&handlerTimerStateLed);         // ACTIVA el timer

    // Imprimir por consola para confirmar
    char msg[64];
    sprintf(msg, "Frecuencia actualizada: %d Hz → periodo = %d ms\r\n", freq, newPeriod);
    writeMsg(&handlerUsart2, msg);


}

int main(void){
	initSystem();

	//Se configura a 16MHz
	config_SysTick_ms(0);

/*
	if(GPIO_ReadPin(&handlerButtonSW) == 0){
		char buffer[64];

		writeChar(&handlerUsart2, getTicks_ms());

		// Delay para rebote (¡opcional!)
		for(volatile int i=0; i<50000; i++);
	}*/
	writeMsg(&handlerUsart2, "STM32 listo para recibir\r\n");

	while(1){
		fsm(); // Llamada a la FSM
		processCommand();
		buildArrayDisplay();
/*		//writeChar(&handlerUsart2,tiempoMs );
		if(GPIO_ReadPin(&handlerButtonSW) == 0){
			tiempoMs = getTicks_ms();
			writeChar(&handlerUsart2, getTicks_ms());

			// Delay para rebote (¡opcional!)
			for(volatile int i=0; i<50000; i++);
		}*/
		if(GPIO_ReadPin(&handlerButtonSW) == 0){
			uint16_t tiempoMs = (uint16_t)getTicks_ms()/1000;

		    char buffer[128];
		    sprintf(buffer, "Tiempo: %u m\n", tiempoMs);
		    writeMsg(&handlerUsart2, buffer);

		    // Delay para rebote (¡opcional!)
		    for(volatile int i=0; i<50000; i++);
		}




	}


	return 0;
}

#include "arm_math.h"      // Para las funciones de CMSIS-DSP
#include "stm32f4xx_hal.h" // O la cabecera HAL de tu familia de STM32

#include <stdio.h>         // Para la función sprintf()
#include <string.h>        // Para la función strlen()

//==============================================================================
// PARÁMETROS DEL SISTEMA Y BUFFERS
//==============================================================================

// --- Parámetros de la Señal y la FFT ---
#define SAMPLING_FREQ   44100.0f  // Frecuencia de muestreo en Hz
#define FFT_SIZE        1024      // Tamaño de la FFT (debe ser potencia de 2)
#define NUM_MAGNITUDES  (FFT_SIZE / 2) // Número de magnitudes de frecuencia (N/2)

// --- Buffers para el procesamiento ---
// Estos buffers deben ser globales o estáticos para no desbordar la pila.

// Buffer de entrada: Llenado con datos del ADC ya convertidos a float32_t
// Se asume que este buffer es llenado por otra parte de tu código.
float32_t dsp_input_buffer[FFT_SIZE];

// Buffer para el resultado intermedio de la FFT (formato complejo empaquetado)
float32_t fft_output_buffer[FFT_SIZE];

// Buffer para el resultado final de las magnitudes del espectro
float32_t fft_magnitudes[NUM_MAGNITUDES];

// Buffer para formatear la cadena de texto a enviar por UART
char tx_buffer[100];


// --- Recursos Externos (Handles) ---
// Estas variables son creadas por STM32CubeIDE en 'main.c'
// Las declaramos como 'extern' para que esta función pueda usarlas.
extern UART_HandleTypeDef huart2;
extern arm_rfft_fast_instance_f32 rfft_instance;


/**
 * @brief  Realiza el análisis FFT completo y transmite la frecuencia dominante por UART.
 * @note   Esta función asume que:
 *         1. La instancia 'rfft_instance' ha sido inicializada con arm_rfft_fast_init_f32().
 *         2. El buffer 'dsp_input_buffer' contiene 'FFT_SIZE' muestras válidas.
 *         3. El periférico USART2 (huart2) ha sido inicializado.
 */
void ejecutar_fft(void) {

    // --- PASO 1: Ejecutar la Transformada Rápida de Fourier (FFT) ---
    // Convierte los datos del dominio del tiempo al dominio de la frecuencia.
    arm_rfft_fast_f32(
        &rfft_instance,
        dsp_input_buffer,
        fft_output_buffer,
        0 // '0' para FFT directa, '1' para FFT inversa
    );

    // --- PASO 2: Calcular las Magnitudes del Espectro ---
    // Convierte el resultado complejo de la FFT en un espectro de potencia (magnitudes).
    arm_cmplx_mag_f32(
        fft_output_buffer,
        fft_magnitudes,
        NUM_MAGNITUDES
    );

    // --- PASO 3: Encontrar la Frecuencia Dominante ---
    // Busca el valor máximo en el array de magnitudes para encontrar el pico de frecuencia.
    float32_t max_magnitude;
    uint32_t  max_index;

    // Usamos arm_max_f32 para encontrar el pico.
    // IMPORTANTE: Empezamos la búsqueda desde el índice 1 para ignorar la componente DC (0 Hz),
    // que a menudo es muy grande y no es de interés para señales AC.
    arm_max_f32(
        &fft_magnitudes[1],     // Puntero al segundo elemento del array
        NUM_MAGNITUDES - 1,     // Número de elementos a buscar
        &max_magnitude,         // Variable para guardar el valor máximo
        &max_index              // Variable para guardar el índice del máximo
    );

    // El índice devuelto es relativo al subarray que le pasamos, así que sumamos 1
    // para obtener el índice correcto en el array 'fft_magnitudes' completo.
    max_index = max_index + 1;

    // --- PASO 4: Convertir el Índice a una Frecuencia Real (Hz) ---
    float32_t frequency_resolution = SAMPLING_FREQ / FFT_SIZE;
    float32_t dominant_frequency = max_index * frequency_resolution;

    // --- PASO 5: Reportar el Resultado por UART ---
    // Formatear la cadena de texto con el resultado
    sprintf(tx_buffer, "Frecuencia Dominante: %.2f Hz (Magnitud: %.2f)\r\n", dominant_frequency, max_magnitude);

    // Transmitir la cadena de texto usando las librerías HAL
    HAL_UART_Transmit(
        &huart2,
        (uint8_t*)tx_buffer,
        strlen(tx_buffer),
        100 // Timeout de 100 ms
    );
}

//-------------------------------------------------------Callbacks-------------------------------------------------


void adcComplete_Callback(void) {
    static uint16_t sampleIndex = 0;

    adcRawData_data[sampleIndex++] = getADC();

    if (sampleIndex >= current_fft_size) {
        sampleIndex = 0;
        ejecutarFFT();  // Llama tu función FFT
        writeMsg(&handlerUsart2, "FFT ejecutada (por ADC)\r\n");
    }
}


void BasicTimer3_Callback(void){

	basic_timer_amount++;

	switch (basic_timer_amount)
	{
		case 1:
		{
			write_digit(1, RESET);
			write_digit(2, SET);
			write_digit(3, SET);
			write_digit(4, SET);
			writeSegments(arrayDisplay[3]);
			break;
		}
		case 2:
		{
			write_digit(1, SET);
			write_digit(2, RESET);
			write_digit(3, SET);
			write_digit(4, SET);
			writeSegments(arrayDisplay[2]);
			break;
		}
		case 3:
		{
			write_digit(1, SET);
			write_digit(2, SET);
			write_digit(3, RESET);
			write_digit(4, SET);
			writeSegments(arrayDisplay[1]);
			break;
		}


		case 4:
		{
			write_digit(1, SET);
			write_digit(2, SET);
			write_digit(3, SET);
			write_digit(4, RESET);
			writeSegments(arrayDisplay[0]);
			break;
		}
		default:
			break;
	}

	if (basic_timer_amount >= 4)
		basic_timer_amount = 0;

}


void BasicTimer2_Callback(void){

	 currentState = STATE_BLINKY;

}



/*
void SysTick_Handler(void) {
    tiempoMs++;
}
*/
void callback_extInt10(void){

    // delay por rebote (opcional)
    for (volatile int i=0; i<100000; i++);

    // Cambia el estado para que la FSM actualice el color
    currentState = STATE_RGB;
    // Handler de la interrupción SysTick

}

void usart2Rx_Callback(void)
{
    char byte = getRxData();


    if (byte == '\r' || byte == '\n') {
        rxBuffer[rxIndex] = '\0';
        // Fin del string
        commandReady = 1;
        rxIndex = 0;

    } else {
        if (rxIndex < MAX_BUFFER_SIZE - 1) {
            rxBuffer[rxIndex++] = byte;
        } else {
            rxIndex = 0;  // Evita desbordamiento
        }
    }

}

void callback_extInt0(void){

	uint8_t currDataState;
	uint8_t currClockState;

	currDataState = GPIO_ReadPin(&handlerEncoderDT);
	currClockState = GPIO_ReadPin(&handlerEncoderCLK);

	if(currDataState == SET ){

		if(currClockState == SET){
			contadorGlobal++;
			if(contadorGlobal > 4095){
				contadorGlobal = 0;
			}

		}
		else{

			contadorGlobal--;
			if(contadorGlobal < 0){
				contadorGlobal = 4095;
			}
		}

	}

}




// Callback function with External Button (pin A2)
void callback_extInt8(void){
    buttonPressed = GPIO_ReadPin(&handlerResetButton);

    if (buttonPressed == RESET) {
        // Apaga el LED antes de reiniciar
        GPIO_WritePin(&handlerStateLed, RESET);

        // ¡Pequeño retardo para que el LED realmente se apague!
        for (volatile int i=0; i<100000; i++);

        NVIC_SystemReset();


    }
}





void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}
}
