/*
 * PllDriver.c
 *
 *  Created on: 23/05/2023
 *      Author: Marianela Arcila Sanchez
 *  In this program, we write about the control of the PLL peripheral.
 *  This control aims to be as generic as possible.
 */
#include <stm32f4xx.h>
#include  "PllDriver.h"

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

	PWR -> CR |= (0b10<<PWR_CR_VOS_Pos);

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

	/*4)We activate the PLL clock in the CFGR register SW  (SYSCLK)*/

	RCC -> CFGR |= RCC_CFGR_SW_1;
	while( !(RCC->CFGR & RCC_CFGR_SW_1));

	/*We clear the preescaler in the CFGR-> HPRE register AHB*/
	RCC -> CFGR &= ~(RCC_CFGR_HPRE);

	/*We configure the prescaler in the MC01 output, We divide the prescaler in 5*/
	RCC->CFGR |= (RCC_CFGR_MCO1PRE_Msk);

	/*We activate the output MC01*/
	if(prtFrequency->PLL_Config.mco1 == MCO1){
	RCC->CFGR |=  RCC_CFGR_MCO1;
	}

	/*we configure the prescaler APB1*/
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV16;

}

void frequency(PLL_Handler_t * prtFrequency){

	/* Now we configure the frequency of the MCU. We establish three important values:
	 * PLLN as 80, PLLM as 4. Then we calculate VCO as 16MHz * (80/4) = 320,
	 * and calculate PLLP as 80MHz / VCO = 4."*/
	if(prtFrequency->PLL_Config.frequency == MCU_FREQUENCY_80MHz){
		//PLLP Register
		RCC->PLLCFGR &= ~(0b11<<RCC_PLLCFGR_PLLP_Pos);
		RCC->PLLCFGR |= (0b01<<RCC_PLLCFGR_PLLP_Pos );
	    //PLPN Register
		RCC->PLLCFGR &= ~(0b111111111 << RCC_PLLCFGR_PLLN_Pos);
		// We clear the register
		RCC->PLLCFGR |= (0b1010000 << RCC_PLLCFGR_PLLN_Pos);

		//PLLM Register
		RCC->PLLCFGR &= ~(0b111111 << RCC_PLLCFGR_PLLM_Pos);
		//We clear the register
		RCC->PLLCFGR |= (4<< RCC_PLLCFGR_PLLM_Pos);
	}

}
