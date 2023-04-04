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
	else{
			__NOP();
	}

	/* The third step is to configure the prescaler. We use the PSC register
	 * to divide the frequency of the clock signal and obtain a timer with a
	 * frequency that is higher or lower than the original signal, depending
	 * on the value loaded into the PSC register. */
	ptrBTimerHandler -> ptrTIMx -> PSC = ptrBTimerHandler ->TIMx_Config.TIMx_speed;

	/*In the fourth step we choose the direction of the counter (up/down)*/
	if(ptrBTimerHandler -> TIMx_Config.TIMx_mode == BTIMER_MODE_UP){

		/*First we configure the counter to count up whit the CR1-DIR register*/
		ptrBTimerHandler -> ptrTIMx -> CR1 &= ~TIM_CR1_DIR;

		/* For a timer in count up mode, we need to define the ARR (Auto-Reload Register).
		 * This sets the maximum value for the counter, and when the counter reaches this
		 * maximum value, it automatically restarts to 0.*/
		ptrBTimerHandler -> ptrTIMx -> ARR = ptrBTimerHandler -> TIMx_Config.TIMx_period -1;

		/* To restart the counter, we need to configure the CNT (Counter) register. The CNT
		 * register is used to count the number of pulses of the clock signal, and when we
		 *  use this register with the ARR register, we define the maximum value that it can
		 *  reach before automatically restarting*/
		ptrBTimerHandler -> ptrTIMx -> CNT = 0;

	}
	else{
		/*We configure the Down Mode whit the CR1-DIR register in 1*/
		ptrBTimerHandler -> ptrTIMx -> CR1 |= TIM_CR1_DIR;

		/* Since the counter is in descending mode, we need to set the
		 * counter cap using the ARR (Auto-Reload Register) register.*/
		ptrBTimerHandler -> ptrTIMx -> ARR = 0;


		/* we restart the maximun value whit the CNT register */
		ptrBTimerHandler -> ptrTIMx -> CNT = ptrBTimerHandler -> TIMx_Config.TIMx_period -1;

	}

	/*In the fifth step we configure the CR1-CEN register. With this register,
	 * we can enable (1) or disable (0) the counter*/

	ptrBTimerHandler -> ptrTIMx -> CR1 |= TIM_CR1_CEN;

	/* The sixth step is to configure the DIER-UIE register. This register
	 * allows you to modify the state of the interrupt update. (0 disabled, 1 enabled)*/

	ptrBTimerHandler -> ptrTIMx -> DIER |= TIM_DIER_UIE;

	/* In the seventh step, we activate the NVC to read the interrupt of each counter. */

	if(ptrBTimerHandler -> ptrTIMx ==  TIM2){
		//For  TIM2
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM3){
		// For TIM3
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM4){
		//For TIM4
		NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM5){
		//For TIM5
		NVIC_EnableIRQ(TIM5_IRQn);
	}
	else{
		__NOP();
	}

	/* In the eighth step we re-enable system interrupts.*/
	__enable_irq();

}

/* "Finally, we declare the Callback function. This function defines
 * an action that is executed in response to an interrupt event.
 * It can perform actions such as updating a variable, activating a peripheral,
 * or any other action needed to handle the timer interrupt. In this particular
 * case, this function is used to handle the interrupt for the Basic Timer.
 *
 * The attribute ((weak)) indicates that it is a "loosely bound function".
 * If this function is defined elsewhere in the program, the function in the
 * current file is modified and replaced with the other definition found in the
 * program." */

//For TIM2
__attribute__((weak)) void BasicTimer2_Callback(void){

	__NOP();
}


//For TIM3
__attribute__((weak)) void BasicTimer3_Callback(void){

	__NOP();
}


//For TIM4
__attribute__((weak)) void BasicTimer4_Callback(void){

	__NOP();
}


//For TIM5
__attribute__((weak)) void BasicTimer5_Callback(void){

	__NOP();
}

