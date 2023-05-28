/*
 * PllDriver.h
 *
 *  Created on: 23/05/2023
 *      Author: Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for
 *  defining the control of the PLL peripheral. (control funtions)
 */

#ifndef PLLDRIVER_H_
#define PLLDRIVER_H_


#include <stm32f4xx.h>
#include "GPIOxDriver.h"






#define MCU_FREQUENCY_80MHz  0

#define MCO1 1

typedef struct{

	uint8_t frequency;

	uint8_t mco1;

}PLL_Config_t;

typedef struct{

	PLL_Config_t  PLL_Config;

}PLL_Handler_t;

void configPll(PLL_Handler_t * prtFrequency);

void frequency(PLL_Handler_t * prtFrequency);


#endif /* PLLDRIVER_H_ */
