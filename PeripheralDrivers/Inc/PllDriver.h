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






#define MCU_FREQUENCY_80MHz  	0
#define MCU_FREQUENCY_100MHz  	1

#define PLL_CLOCK 				0
#define HSI_CLOCK				1
#define LSE_CLOCK				2


#define MCO1 1

typedef struct{

	uint8_t frequency;
	uint8_t clock;

	uint8_t mco1;

}CLOCK_Config_t;

typedef struct{

	CLOCK_Config_t  CLOCK_Config;

}CLOCK_Handler_t;

void configPll(CLOCK_Handler_t * prtPLL);



void configMCO( CLOCK_Handler_t * ptrClock);
#endif /* PLLDRIVER_H_ */
