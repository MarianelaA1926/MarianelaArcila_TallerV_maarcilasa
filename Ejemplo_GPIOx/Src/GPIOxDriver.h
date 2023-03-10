/*
 * GPIOxDriver.h
 *
 *  Created on: 10/03/2023
 *      Author: marianela
 *  Este programa se encarga de las definiciones relacionadas unicament
 *  con el debido manejo y control del periferico GPIOx(Funciones del controlador)
 */

#ifndef GPIOXDRIVER_H_
#define GPIOXDRIVER_H_

#include "stm32f411xx_hal.h"

typedef struct{

	uint8_t GPIO_PinNumber; 			// Pin con el que deseamos trabajar
	uint8_t GPIO_PinMode;  				// Modo de la configuracion: entrada, salida, analogo, f.alternativa
	uint8_t GPIO_PinSpeed; 				// Velocidad de respuesta del IN (Solo para digital)
	uint8_t GPIO_PinPuPdControl; 		// Seleccionamos si deseamos una salida Pull-up, Pull-down, o "Libre"
	uint8_t GPIO_PinOPType;				//Trabaja de la mano con el anterior, selecciona salida PUPD o OpenDrain
	uint8_t GPIO_PinAltFunMode;			// Selecciona cual es la funcion alternativa que se esta configurando
}GPIO_PinConfig_t;


/*
 * Esta es una estructura que contiene dos elementos:
 * -La direccion del puerto que se esta utilizandio (La referencua al puerto)
 * -La configuracion especifica del PIN que se esta utilizando
 */

typedef struct{
	GPIO_RegDef_t 			*pGPIOx     			/* !< Direccion del puerto al que el PIN corresponde >*/
	GPIO_PinConfig_t 		GPIO_PinConfig; 		/*!< Configuracion del PIN >*/
}GPIO_Handler_t;


/* Definicion de las cabeceras de las funciones del GPIOxDriver */

void GPIO_Config (GPIO_Handler_t * pGPIOHandler);
void GPIO_Config (GPIO_Handler_t *pPinHandler, uint8_t newState);
uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler);







#endif /* GPIOXDRIVER_H_ */
