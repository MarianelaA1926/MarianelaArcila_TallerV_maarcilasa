/*
 * BasicTimer.c
 *
 *  Created on: 22/03/2023
 *      Author: Marianela Arcila Sanchez 				maarcilasa
 *  In this program, we write about the control of the TIMx peripheral.
 *  This control aims to be as generic as possible.
 *
 */

#include <stm32f4xx.h>
#include "BasicTimer.h"


/* This variable saves the reference of the peripheral that we are using*/

TIM_TypeDef            *ptrTimerUsed;

/* In the 'Basic_Timer_Config' function, we load the timer configuration.
 * We activate the clock signal of the peripheral that we are using.
 * We need to be careful with interruptions, and before configuring a new interruption,
 * we need to deactivate the global system of interruptions.
 * Then,we activate the specific IRQ and load the system.
 */

void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler){

	//We keep a reference to the peripheral that we are using
	ptrTimerUsed = ptrBTimerHandler -> ptrTIMx;


	__disable_irq();





}





