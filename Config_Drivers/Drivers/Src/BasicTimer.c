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

	// First we deactivate global interruptions while we configure the timer system
	__disable_irq();

	// The second step is to activate the signal clock of the required pheriferal
	if(ptrBTimerHandler -> ptrTIMx == TIM2){
		// We write 1 (SET)  in the position corresponds to TIM2
		RCC -> APB1ENR |= (SET << RCC_APB1ENR_TIM2EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM3){
			// We write 1 (SET)  in the position corresponds to TIM3
			RCC -> APB1ENR |= (SET << RCC_APB1ENR_TIM3EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM4){
			// We write 1 (SET)  in the position corresponds to TIM4
			RCC -> APB1ENR |= (SET << RCC_APB1ENR_TIM4EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM5){
			// We write 1 (SET)  in the position corresponds to TIM5
			RCC -> APB1ENR |= (SET << RCC_APB1ENR_TIM5EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM9){
			// We write 1 (SET)  in the position corresponds to TIM10
			RCC -> APB2ENR |= (SET << RCC_APB2ENR_TIM9EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM10){
			// We write 1 (SET)  in the position corresponds to TIM2
			RCC -> APB2ENR |= (SET << RCC_APB2ENR_TIM10EN);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM11){
			// We write 1 (SET)  in the position corresponds to TIM2
			RCC -> APB1ENR |= (SET << RCC_APB2ENR_TIM9EN);
	}
	else{
			__NOP();
	}

	/* The third step is to configure the prescaler. We use the PSC register
	 * to divide the frequency of the clock signal and obtain a timer with a
	 * frequency that is higher or lower than the original signal, depending
	 * on the value loaded into the PSC register. */
	ptrBTimerHandler -> ptrTIMx -> PSC = ptrBTimerHandler ->TIMx_Config.TIMx_speed;








}





