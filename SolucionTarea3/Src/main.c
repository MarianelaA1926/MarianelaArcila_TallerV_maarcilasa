/*
 * ExtiConfig_Main.c
 *
 *  Created on: 24/03/2023
 *      Author: marianela
 */
/**
 ******************************************************************************
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

/* Variables del proyecto */


//-------------------------------------------------------Led State---------------------------------------------
// Status LED control variables
// handlers  GPIOA pin A5 y  de TIM2
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};
//---------------------------------------------------------Buton-------------------------------------------------------


GPIO_Handler_t handlerExternalLed = {0};

GPIO_Handler_t handlerExternalButton = {0};
EXTI_Config_t handlerExtiButton;

uint8_t buttonPressed;
//------------------------------------------------------Encoder-----------------------------------------------------------------

GPIO_Handler_t handlerEncoderClock = {0};
EXTI_Config_t handlerExtiEncoderCK = {0};
uint8_t prevDataState = 0;


GPIO_Handler_t handlerEncoderDT = {0};
uint8_t prevDTState = 0;
uint8_t prevCLKState = 0;


GPIO_Handler_t handlerEncoderSW = {0};
EXTI_Config_t handlerExtiEncoderSW = {0};


//-------------------------------------------------7Segmentos------------------------------------------------------------

GPIO_Handler_t  handlerPunto   = {0};
GPIO_Handler_t  handlerPinLedA = {0};
GPIO_Handler_t  handlerPinLedB = {0};
GPIO_Handler_t  handlerPinLedC = {0};
GPIO_Handler_t  handlerPinLedD = {0};
GPIO_Handler_t  handlerPinLedE = {0};
GPIO_Handler_t  handlerPinLedF = {0};
GPIO_Handler_t  handlerPinLedG = {0};


/* Principal Functions */
void initSystem(void);

void BasicTimer2_Callback(void);


int main(void)
{

	initSystem();

    /* Principal cicle  */
	while(1){

	}
	return 0;
}

/* Pin configuration function  */

void initSystem(void){

	// --------------------------------- State Led--------------------------------------------------//
	// Pórt GPIOA pin A5
	handlerStateLed.pGPIOx = 											GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = 						GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = 						GPIO_OSPEED_FAST;


	//We load the configuration of the specific pin

	GPIO_Config(&handlerStateLed);


	// We configure TIMER 2 as the counter for the stateLed
	handlerTimerStateLed.ptrTIMx = 										TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 						250;

	// We load the configuration
	BasicTimer_Config(&handlerTimerStateLed);
	// We activate the TIM2
	starTimer(&handlerTimerStateLed);
//------------------------------------------------External Butom-----------------------------------------------------

	// Pórt GPIOC pin 8
	handlerExternalLed.pGPIOx = 											GPIOC;
	handlerExternalLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_8;
	handlerExternalLed.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_OUT;
	handlerExternalLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerExternalLed.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;
	handlerExternalLed.GPIO_PinConfig.GPIO_PinSpeed = 					GPIO_OSPEED_FAST;


	//We load the configuration of the specific pin
	//GPIO_WritePin(&handlerExternalLed, SET);
	GPIO_Config(&handlerExternalLed);


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerExternalButton.pGPIOx	= 										GPIOB;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinNumber = 				PIN_8;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerExternalButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);



//------------------------------------------Encoder----------------------------------------------------------------
//---CLOCK-----------------------------------------------------
	/*ENCODER CLOCK configuration as external interruption*/
	//GPIO configuration
	handlerEncoderClock.pGPIOx	= 										GPIOA;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinNumber = 				PIN_0;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiEncoderCK.pGPIOHandler= 									&handlerEncoderClock;
	handlerExtiEncoderCK.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderCK);


//-----DT--------------------------------------------------------
	/*ENCODER DT configuration as external interruption*/
	//GPIO configuration
	handlerEncoderDT.pGPIOx	= 										GPIOA;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinNumber = 				PIN_1;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;



//-----SW------------------------------------------------------------
	/*ENCODER SW configuration as external interruption*/
	//GPIO configuration
	handlerEncoderSW.pGPIOx	= 										GPIOA;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinNumber = 				PIN_10;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


//-------------------------------------------------LED 7 SEGMENTOS----------------------------------------------------------

//------LEDS-------------------------------
	handlerPunto.pGPIOx = 												GPIOB; 									//El puerto que se esta utilizando es el puerto B
    handlerPunto.GPIO_PinConfig.GPIO_PinNumber =						PIN_5; //EL pin que se esta utilizando es el pin 8
    handlerPunto.GPIO_PinConfig.GPIO_PinMode  =							GPIO_MODE_OUT; //EL pin esta en modo de salida
    handlerPunto.GPIO_PinConfig.GPIO_PinOPType =         				GPIO_OTYPE_PUSHPULL; //PushPull para poder que me de cero y uno
    handlerPunto.GPIO_PinConfig.GPIO_PinPuPdControl =					GPIO_PUPDR_NOTHING; // YA tiene res. No necesita de otra, por lo cual se pone NOTHING
    handlerPunto.GPIO_PinConfig.GPIO_PinSpeed=				            GPIO_OSPEED_MEDIUM; //Configuro la velocida

    //Ahora se carga la configuración
    GPIO_Config(&handlerPunto);

	handlerPinLedA.pGPIOx=                                  			GPIOC;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinNumber=           			PIN_0;// pin para led externo;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinMode=             			GPIO_MODE_OUT;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinOPType=          				GPIO_OTYPE_PUSHPULL;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinPuPdControl=    				GPIO_PUPDR_NOTHING;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinSpeed=            			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedA);


	handlerPinLedB.pGPIOx=                                   			GPIOA;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinNumber=            			PIN_1;// pin para led externo;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinPuPdControl=      			GPIO_PUPDR_NOTHING;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;

	GPIO_Config(&handlerPinLedB);

	handlerPinLedC.pGPIOx=                                   			GPIOA;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinNumber=            			PIN_4;// pin para led externo;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedC);

	handlerPinLedD.pGPIOx=                                   			GPIOA;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinNumber=            			PIN_6;// pin para led externo;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedD);

	handlerPinLedE.pGPIOx=                                   			GPIOA;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinNumber=            			PIN_7;// pin para led externo;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedE);

	handlerPinLedF.pGPIOx=                                   			GPIOA;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinNumber=            			PIN_8;// pin para led externo;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedF);

	handlerPinLedG.pGPIOx=                                  				GPIOA;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinNumber=            			PIN_9;// pin para led externo;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_MEDIUM;


	GPIO_Config(&handlerPinLedG);

//----------------------Transistores




}



//






void callback_extInt0(void){

	uint8_t currDataState;
	currDataState = GPIO_ReadPin(&handlerEncoderDT);

	if(currDataState != prevDataState){
		//se determina la direccion de giro del encoder
		if(currDataState == SET){
			//Giro en direccion horaria
			GPIO_WritePin(&handlerExternalLed, RESET);

		}
		else{
			//Giro direccion antihoraria
			GPIO_WritePin(&handlerExternalLed, SET);
		}

		prevDataState = currDataState;
	}


}


//Callback function with state led

void BasicTimer2_Callback(void){

	/* To create a status LED, we need to link the timer with the GPIO.
	 * We can do this by calling the Toggle function in the Timer Callback.
	 * This generates the interrupt that will toggle the LED state.*/
	GPIOxTooglePin(&handlerStateLed);


	// Lectura del botón
	buttonPressed = GPIO_ReadPin(&handlerExternalButton);
    if (buttonPressed == RESET) { // Si el botón está presionado
    	GPIO_WritePin(&handlerExternalLed, RESET); // Apagamos el LED asociado
    } else { // Si el botón no está presionado
    	GPIO_WritePin(&handlerExternalLed, SET); // Encendemos el LED asociado
    }

}


//Callback function with Exti
void callback_extInt6(void){
}

//Callback function with Exti


