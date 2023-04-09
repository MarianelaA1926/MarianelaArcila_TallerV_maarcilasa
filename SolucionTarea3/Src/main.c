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
uint8_t lastClkState;


GPIO_Handler_t handlerEncoderDT = {0};
uint8_t lastDTState;


GPIO_Handler_t handlerEncoderSW = {0};
EXTI_Config_t handlerExtiEncoderSW = {0};


/* Principal Functions */
void initSystem(void);
void BasicTimer2_Callback(void);
void BasicTimer3_Callback(void);

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
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_PULLDOWN;
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
	handlerExternalLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_PULLDOWN;
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
	handlerExtiEncoderCK.edgeType = 										EXTERNAL_INTERRUPT_FALLING_EDGE;
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
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinNumber = 				PIN_2;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiEncoderSW.pGPIOHandler= 									&handlerEncoderSW;
	handlerExtiEncoderSW.edgeType = 										EXTERNAL_INTERRUPT_FALLING_EDGE;
	extInt_Config(&handlerExtiEncoderSW);


}








/*
//Callback function with Exti
void callback_extInt6(void){


}
*/
//Callback function with Exti
void callback_extInt0(void){

//Se lee el estado del clok para determina si su funcionamiento es correcto
	uint8_t clkState;
	clkState = GPIO_ReadPin(&handlerEncoderClock);

	//Se detecta si hay un flanco de subida o de bajada
	if ( clkState != lastClkState){
		lastClkState = clkState;

		//Se lee el estado actual del pin DT con el fin de determinar como gira el encoder
		lastDTState = GPIO_ReadPin(&handlerEncoderDT);

		if(lastDTState == clkState){
			//Giro en sentido horario se enciende el Led externo
			GPIO_WritePin(&handlerExternalLed, SET);
		}
		else{
			//Giro en sentido antiohorario se apaga el led
			GPIO_WritePin(&handlerExternalLed, RESET);

		}



	}
}

//Callback function with Exti
void callback_extInt2(void){


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

