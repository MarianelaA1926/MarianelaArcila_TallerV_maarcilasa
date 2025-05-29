/*
 * SysTickDriver.h
 *
 *  Created on: 22/05/2023
 *      Author: Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for defining the control of the SysTick .
 *  (control funtions)
 */

#ifndef SYSTICKDRIVER_H_
#define SYSTICKDRIVER_H_



#include <stm32f4xx.h>

#define SYSTICK_LOAD_VALUE_16MHz_1ms		16000    //Number of cycles 1ms.
#define SYSTICK_LOAD_VALUE_100MHz_1ms	100000	 //Number of cycles 1ms.

void config_SysTick_ms (uint8_t systemClock) ;
uint64_t getTicks_ms(void);
void delay_ms(uint32_t wait_time_ms);

#endif /* SYSTICKDRIVER_H_ */
