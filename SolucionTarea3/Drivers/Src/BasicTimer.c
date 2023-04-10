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
		RCC -> APB1ENR |=  RCC_APB1ENR_TIM2EN;
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM3){
		// We write 1 (SET)  in the position corresponds to TIM3
		RCC -> APB1ENR |=  RCC_APB1ENR_TIM3EN;
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM4){
		// We write 1 (SET)  in the position corresponds to TIM4
		RCC -> APB1ENR |=  RCC_APB1ENR_TIM4EN;
	}
	else if(ptrBTimerHandler -> ptrTIMx == TIM5){
		// We write 1 (SET)  in the position corresponds to TIM5
		RCC -> APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else{
			__NOP();
	}

	/* The third step is to configure the prescaler. We use the PSC register
	 * to divide the frequency of the clock signal and obtain a timer with a
	 * frequency that is higher or lower than the original signal, depending
	 * on the value loaded into the PSC register. */
	ptrBTimerHandler -> ptrTIMx -> PSC = ptrBTimerHandler ->TIMx_Config.TIMx_speed -1;

	/* For a timer in count up mode, we need to define the ARR (Auto-Reload Register).
	 * This sets the maximum value for the counter, and when the counter reaches this
	 * maximum value, it automatically restarts to 0.*/
	ptrBTimerHandler -> ptrTIMx -> ARR = ptrBTimerHandler -> TIMx_Config.TIMx_period -1;

	/*In the fourth step we choose the direction of the counter (up/down)*/
	if(ptrBTimerHandler -> TIMx_Config.TIMx_mode == BTIMER_MODE_UP){

		/*First we configure the counter to count up whit the CR1-DIR register*/
		ptrBTimerHandler -> ptrTIMx -> CR1 &= ~TIM_CR1_DIR;


		/* To restart the counter, we need to configure the CNT (Counter) register. The CNT
		 * register is used to count the number of pulses of the clock signal, and when we
		 *  use this register with the ARR register, we define the maximum value that it can
		 *  reach before automatically restarting*/
		ptrBTimerHandler -> ptrTIMx -> CNT = 0;

	}
	else{
		/*We configure the Down Mode whit the CR1-DIR register in 1*/
		ptrBTimerHandler -> ptrTIMx -> CR1 |= TIM_CR1_DIR;


		/* we restart the maximun value whit the CNT register */
		ptrBTimerHandler->ptrTIMx->CNT = ptrBTimerHandler->TIMx_Config.TIMx_period - 1;

	}




	/*In the fifth step we configure the CR1-CEN register. With this register,
	 * we can enable (1) or disable (0) the counter*/

	if(ptrBTimerHandler->TIMx_Config.TIMx_interrupEnable !=  BTIMER_DISABLE){

			ptrBTimerHandler-> ptrTIMx -> CR1 |= TIM_CR1_CEN;
			ptrBTimerHandler-> ptrTIMx -> DIER |= TIM_DIER_UIE;

		}
		else{
			ptrBTimerHandler-> ptrTIMx -> CR1 &= ~TIM_CR1_CEN;
			ptrBTimerHandler-> ptrTIMx -> DIER &= ~TIM_DIER_UIE;
		}




	/* The sixth step is to configure the DIER-UIE register. This register
	 * allows you to modify the state of the interrupt update. (0 disabled, 1 enabled)*/

	ptrBTimerHandler-> ptrTIMx -> DIER |= TIM_DIER_UIE;


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


	/* In the eighth step we re-enable system interrupts.*/
	__enable_irq();

}

void starTimer (BasicTimer_Handler_t *ptrBTimerHandler){

	//We activate the timer so that it starts to count.
	ptrBTimerHandler ->ptrTIMx->CR1 |= TIM_CR1_CEN;
}

void stopTimer(BasicTimer_Handler_t *ptrBTimerHandler){

	//We disable the timer so that it stops incrementing.
	ptrBTimerHandler-> ptrTIMx -> CR1 &= ~(TIM_CR1_CEN);
}

/* In the nineth step, we declare the Callback function. This function defines
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


/* Finally, we need to create another function that knows which functions need
 * to be executed when the interruption happens. To do this, we create a new function
 * with the same name used in the interruption vector.
 *
 * The interruption vector is a table that contains the memory addresses of interrupt
 * handling functions. When an interrupt occurs, the microcontroller looks up this table
 * to get the address of the corresponding interrupt handling function and then jumps to
 * that memory address to execute the function."*/


void TIM2_IRQHandler(void){

	/* We clear the interruption flag of TIM2, which indicates that an interrupt has occurred.
	 * This is done to prevent an interrupt from being re-triggered immediately after the current
	 * interrupt has been handled. We use the SR register to access the TIM flag (SR_UIF).*/
	ptrTimerUsed-> SR  &= ~TIM_SR_UIF;

	/* We call the callback function which is defined by the user and contains the actions that
	 * the user has defined for their program*/

	BasicTimer2_Callback();
}


void TIM3_IRQHandler(void){

	/* We clear the interruption flag of TIM3, which indicates that an interrupt has occurred.
	 * This is done to prevent an interrupt from being re-triggered immediately after the current
	 * interrupt has been handled. We use the SR register to access the TIM flag (SR_UIF).*/
	ptrTimerUsed -> SR &= ~TIM_SR_UIF;

	/* We call the callback function which is defined by the user and contains the actions that
	 * the user has defined for their program*/

	BasicTimer3_Callback();
}


void TIM4_IRQHandler(void){

	/* We clear the interruption flag of TIM4, which indicates that an interrupt has occurred.
	 * This is done to prevent an interrupt from being re-triggered immediately after the current
	 * interrupt has been handled. We use the SR register to access the TIM flag (SR_UIF).*/
	ptrTimerUsed-> SR  &= ~TIM_SR_UIF;

	/* We call the callback function which is defined by the user and contains the actions that
	 * the user has defined for their program*/

	BasicTimer4_Callback();
}


void TIM5_IRQHandler(void){

	/* We clear the interruption flag of TIM5, which indicates that an interrupt has occurred.
	 * This is done to prevent an interrupt from being re-triggered immediately after the current
	 * interrupt has been handled. We use the SR register to access the TIM flag (SR_UIF).*/
	ptrTimerUsed-> SR &= ~TIM_SR_UIF;

	/* We call the callback function which is defined by the user and contains the actions that
	 * the user has defined for their program*/

	BasicTimer5_Callback();
}



