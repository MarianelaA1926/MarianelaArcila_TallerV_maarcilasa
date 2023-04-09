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



// Variables de control del led de estado
// handlers de GPIOA pin A5 y  de TIM2
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};
GPIO_Handler_t handlerUserButton = {0};

EXTI_Config_t handlerExtiButton;
uint8_t buttonPressed;

/* Funciones principales del proyecto */
void initSystem(void);
void BasicTimer2_Callback(void);

int main(void)
{
	// Incicializamos pines y USART
	initSystem();

    /* Ciclo principal  */
	while(1){


	}
	return 0;
}

/* Función de configuración de pines y USART */

void initSystem(void){

	// --------------------------------- Led de estado --------------------------------------------------//
	// Deseamos trabajar con el puerto GPIOA pin A5
	handlerStateLed.pGPIOx = GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	handlerStateLed.GPIO_PinConfig.GPIO_PinAltFunMode = AF0;

	//Cargamos la configuración del pin específico
	//GPIO_WritePin(&handlerStateLed, SET);
	GPIO_Config(&handlerStateLed);


	// Configuramos el TIMER 2 como el contador para el stateLed
	handlerTimerStateLed.ptrTIMx = TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= BTIMER_SPEED_1ms;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 250;

	// Cargamos la configuración
	BasicTimer_Config(&handlerTimerStateLed);
	// Activamos el TIM2
	starTimer(&handlerTimerStateLed);

	/*Configuracion del Boton como intrrupcion esterna*/
	//Paso 1 del exti
	handlerUserButton.pGPIOx	= GPIOC;
	handlerUserButton.GPIO_PinConfig.GPIO_PinNumber = PIN_13;
	handlerUserButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerUserButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerUserButton.GPIO_PinConfig.GPIO_PinAltFunMode		=AF0;




	handlerExtiButton.pGPIOHandler= &handlerUserButton;
	handlerExtiButton.edgeType = EXTERNAL_INTERRUPT_FALLING_EDGE;
	extInt_Config(&handlerExtiButton);

}
// Funcion de interrupcion del TIMER2
void BasicTimer2_Callback(void){

	buttonPressed = GPIO_ReadPin(&handlerUserButton);
	if(buttonPressed == SET){
	        // Si el botón está presionado, apagar el LED
			GPIOxTooglePin(&handlerStateLed);
	    }
	    else {
	        // Si el botón está suelto, encender el LED
	        GPIO_WritePin(&handlerStateLed, SET);
	 }
}

void callback_extInt13(void){

}


