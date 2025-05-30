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

#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "stm32_assert.h"
#include "USARTxDriver.h"
#include "SysTickDriver.h"
//-------------------------------------------------------Led State---------------------------------------------
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


//-----------------------------------IMPRIMIR EN CONSOLA----------------------------
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};
char tiempoMs = {0};

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

	while(1){
		fsm(); // Llamada a la FSM
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








//-------------------------------------------------------Callbacks-------------------------------------------------




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
