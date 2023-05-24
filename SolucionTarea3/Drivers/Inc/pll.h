/*
 * pll.h
 *
 *  Created on: 24/05/2023
 *      Author: marianela
 */

#ifndef INC_PLL_H_
#define INC_PLL_H_



#include <stm32f4xx.h>
#include "GPIOxDriver.h"


#define PLLP_2  0
#define PLLP_4  1
#define PLLP_6  2
#define PLLP_8  3

#define PLLN_50  50
#define PLLN_99  99
#define PLLN_100 200
#define PLLN_200 200

#define PLLM_2  2
#define PLLM_3  3
#define PLLM_4  4
#define PLLM_16 16

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
	uint8_t hsiRdy;
	uint8_t pll;
	uint8_t pllRdy;
	uint8_t latency;
	uint8_t ahb;
	uint8_t apb1;
	uint8_t apb2;
	uint8_t pllON;
	uint8_t sw;
	uint8_t sws;
	uint8_t mco1;
	uint8_t mco1Pre;
	uint8_t vos;

}PLL_Config_t;

typedef struct{

	PLL_Config_t  PLL_Config;
	PWR_TypeDef  *pPWRx;

}PLL_Handler_t;

void rcc_config(PLL_Handler_t *ptrFrequency);
void set_frecuency (PLL_Handler_t *ptrFrequency);





#endif /* INC_PLL_H_ */
