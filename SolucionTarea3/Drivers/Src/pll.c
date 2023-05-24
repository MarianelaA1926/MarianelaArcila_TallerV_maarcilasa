/*
 * pll.c
 *
 *  Created on: 24/05/2023
 *      Author: marianela
 */



#include <stm32f4xx.h>
#include  "pll.h"

/* First, we need to configure the RCC (Reset and Clock Control) register.
 * so, we activate the PWR (Power Control) peripheral for proper control of the
 * energy in the PLL.
 */
void configPll(PLL_Handler_t * prtFrequency){

	RCC  -> APB1ENR |= RCC_APB1ENR_PWREN; //PWR clock signal

	/* 1) We configure the regulator voltage scaling output.
	 * This register controls the internal voltage. In this case,
	 * since we need a frequency of 80MHz, we use the Scale 2 mode.
	 */
	PWR -> CR |= (2<<PWR_CR_VOS_Pos);

	/* 2) We configure the latency with the wait states in the Flash Memory.
	 *    This is done in the ACR register, where the wait states are set to
	 *    ensure proper functioning of the memory.  (3.4.1 MR)
	 *    In this particular case, we use the MOS in scale to. As a result,
	 *    the wait states are set to 2 for the HCLK voltage between 2.7V and 3.6V.
	 */
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;

	/*3) We activate the PLL clock signal in the CR register to the RCC_PLLON
	 * and wait for the clock to stabilize
	 */
	RCC->CR |= RCC_CR_PLLON;
	while(! (RCC->CR & RCC_CR_PLLRDY ));

	/*4)We activate the PLL clock in the CFGR register SW*/

	RCC -> CFGR |= RCC_CFGR_SW_1;
	while( !(RCC->CFGR & RCC_CFGR_SW_1));

	/*We clear the preescaler in the CFGR-> HPRE register*/
	RCC -> CFGR &= ~(RCC_CFGR_HPRE);

	/*We configure the prescaler in the MC01 output, We divide the prescaler in 5*/
	RCC->CFGR |= (0b111 << RCC_CFGR_MCO1_Pos );

	/*We activate the output MC01*/
	if(ptrFrecuencia->PLL_Config.mco1 == MCO1){
	RCC->CFGR |=  RCC_CFGR_MCO1_1  ;
	}

	/*we configure the prescaler*/
	RCC->CFGR |= f(0b100 << 10);


}
