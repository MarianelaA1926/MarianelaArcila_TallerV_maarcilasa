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




#define CLOCK_HSI		0
#define CLOCK_LSE		1


#define MCU_FREQUENCY_80MHz  	0
#define MCU_FREQUENCY_100MHz  	1

#define NO_DIVISION			0
#define DIVISION_BY2		1
#define DIVISION_BY3		2
#define DIVISION_BY4		3
#define DIVISION_BY5		4



#define MCO1 1

typedef struct{

	uint8_t frequency;
	uint8_t clock;
	uint8_t prescaler;

}CLOCK_Config_t;

typedef struct{

	CLOCK_Config_t  CLOCK_Config;

}CLOCK_Handler_t;

void configPll(CLOCK_Handler_t * prtPLL);

void frequencyClock(CLOCK_Handler_t * ptrClock);
void typeClock(CLOCK_Handler_t *ptrClock);
void prescalerClock(CLOCK_Handler_t *ptrClock);

#endif /* PLLDRIVER_H_ */
