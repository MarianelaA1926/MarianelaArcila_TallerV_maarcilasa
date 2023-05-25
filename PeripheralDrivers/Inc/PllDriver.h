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


#define PLLP_2  0
#define PLLN_80 80
#define PLLM_4  4


#define PLL_OFF  O
#define PLL_ON   1

#define AHB    1
#define APB1   5
#define APB2   4

#define SW  2
#define SWS 2

#define LATENCY  1

#define MCO1      1
#define MCO1_PRE  1

#define VOS  1

typedef struct{

	uint8_t frequency;
	uint8_t prescaler;
	uint8_t PLLNfactor;
	uint8_t PLLMfactor;

	uint8_t mco1;

}PLL_Config_t;

typedef struct{

	PLL_Config_t  PLL_Config;

}PLL_Handler_t;

void rcc_config(PLL_Handler_t *ptrFrequency);
void set_frecuency (PLL_Handler_t *ptrFrequency);



#endif /* PLLDRIVER_H_ */
