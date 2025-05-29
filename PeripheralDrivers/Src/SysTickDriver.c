/*
 * SysTickDriver.c
 *
 *  Created on: 22/05/2023
 *      Author:  Marianela Arcila Sanchez	maarcilasa
 *  In this program, we write about the control of the SysTick peripheral.
 *  This control aims to be as generic as possible.
 */



#include <stm32f4xx.h>
#include "SysTickDriver.h"

uint64_t ticks = 0;
uint64_t ticks_start = 0;
uint64_t ticks_counting = 0;

void config_SysTick_ms(uint8_t systemClock){
	// We reset the value of the variable that counts time.
	ticks = 0;
	// Loading the limit value of increments representing lms.
	switch (systemClock) {
	// Case for HSI 16MHz clock
	case 0:
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break ;

	// Case for HSE clock
	case 1:
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break ;
		// Case for PLL clock at 100MHz
	case 2:

		SysTick->LOAD = SYSTICK_LOAD_VALUE_100MHz_1ms;
		break ;
	// In case a different value is entered
	default:
		SysTick->LOAD = SYSTICK_LOAD_VALUE_16MHz_1ms;
		break;
	}
	// We clear the current value of the SysTick
	SysTick->VAL = 0;

	// Set the internal clock as the clock for the Timer
	SysTick->CTRL |=  SysTick_CTRL_CLKSOURCE_Msk;

	//Disable global interrupts
	__disable_irq();

	// Register the interruption in the NVIC
	NVIC_EnableIRQ(SysTick_IRQn);

    // We activate the interruption due to counting to zero of the SysTick
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

	// We activate the Timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	// Activate global interrupts again
	__enable_irq();
}


void SysTick_Handler(void){

	/*	// Verificamos que la interrupcion se lanzo
	if(	SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){

		//Limpiamos la bandera
		SysTick -> CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;}
*/
		//Incrementamos en 1 el contador




	ticks ++;
}

uint64_t getTicks_ms(void){
	return ticks;
}
void delay_ms(uint32_t wait_time_ms){

	// Capture the first time value to compare
	ticks_start = getTicks_ms();

	// Capture the second time value to compare.
	ticks_counting= getTicks_ms();

	// Compare: the "counting" value is less than the "start + wait"
	// update the "counting" value.
	// Repeat this operation until counting is greater (times out)
	while(ticks_counting < (ticks_start + (uint64_t) wait_time_ms)){

		// update the value
		ticks_counting = getTicks_ms();
	}
}
