/*
 * ExtiDriver.h
 *
 *  Created on: 6/04/2023
 *      Author: Marianela Arcila Sanchez 			maarcilasa
 *
 * In this program, we define the functions needed to configure
 * the external interruptions peripheral .
 */

#ifndef EXTIDRIVER_H_
#define EXTIDRIVER_H_

#include "stm32f4xx.h"
#include "GPIOxDriver.h"

#include<stm32f4xx.h>
#include "GPIOxDriver.h"



#define EXTERNAL_INTERRUPT_FALLING_EDGE		0
#define EXTERNAL_INTERRUPT_RISING_EDGE		1

typedef struct
{
	GPIO_Handler_t *pGPIOHandler;	// we need to configure the PIN where the signal will enter since the is a external signal
	uint8_t			edgeType;		// Select rising edge or falling edge
}EXTI_Config_t;


void extInt_Config(EXTI_Config_t *extiConfig);
void callback_extInt0(void);
void callback_extInt1(void);
void callback_extInt2(void);
void callback_extInt3(void);
void callback_extInt4(void);
void callback_extInt5(void);
void callback_extInt6(void);
void callback_extInt7(void);
void callback_extInt8(void);
void callback_extInt9(void);
void callback_extInt10(void);
void callback_extInt11(void);
void callback_extInt12(void);
void callback_extInt13(void);
void callback_extInt14(void);
void callback_extInt15(void);

#endif /* EXTIDRIVER_H_ */

