/*
 * main_2025.c
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

//-------------------------------------------------------Led State---------------------------------------------
// Status LED control variables
// handlers  GPIOA pin A5 y  de TIM2
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};
//---------------------------------------------------------Buton-------------------------------------------------------

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
	handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable=				BTIMER_ENABLE;
	// We load the configuration
	BasicTimer_Config(&handlerTimerStateLed);
	// We activate the TIM2
	starTimer(&handlerTimerStateLed);





}



int main(void)
{
	initSystem();
	while(1){


	}
	return 0;
}


void BasicTimer2_Callback(void){

	/* To create a status LED, we need to link the timer with the GPIO.
	 * We can do this by calling the Toggle function in the Timer Callback.
	 * This generates the interrupt that will toggle the LED state.*/
	GPIOxTooglePin(&handlerStateLed);



}


void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}

}
