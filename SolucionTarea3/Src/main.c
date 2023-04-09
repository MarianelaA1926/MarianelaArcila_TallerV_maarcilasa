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
//---------------------------------------------------------ENCODER-------------------------------------------------------


GPIO_Handler_t handlerEncoderLed = {0};

GPIO_Handler_t handlerEncoderButton = {0};
EXTI_Config_t handlerExtiButton;

uint8_t buttonPressed;

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
//------------------------------------------------Encoder-----------------------------------------------------

	// Pórt GPIOC pin 8
	handlerEncoderLed.pGPIOx = 											GPIOC;
	handlerEncoderLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_8;
	handlerEncoderLed.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_OUT;
	handlerEncoderLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerEncoderLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_PULLDOWN;
	handlerEncoderLed.GPIO_PinConfig.GPIO_PinSpeed = 					GPIO_OSPEED_FAST;


	//We load the configuration of the specific pin
	//GPIO_WritePin(&handlerEncoderLed, SET);
	GPIO_Config(&handlerEncoderLed);


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerEncoderButton.pGPIOx	= 										GPIOB;
	handlerEncoderButton.GPIO_PinConfig.GPIO_PinNumber = 				PIN_8;
	handlerEncoderButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerEncoderButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);

}









//Callback function with Exti
void callback_extInt6(void){


}



//Callback function with state led

void BasicTimer2_Callback(void){

	/* To create a status LED, we need to link the timer with the GPIO.
	 * We can do this by calling the Toggle function in the Timer Callback.
	 * This generates the interrupt that will toggle the LED state.*/
	GPIOxTooglePin(&handlerStateLed);


	// Lectura del botón
	buttonPressed = GPIO_ReadPin(&handlerEncoderButton);
    if (buttonPressed == RESET) { // Si el botón está presionado
    	GPIO_WritePin(&handlerEncoderLed, RESET); // Apagamos el LED asociado
    } else { // Si el botón no está presionado
    	GPIO_WritePin(&handlerEncoderLed, SET); // Encendemos el LED asociado
    }


}

