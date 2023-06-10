/*
 * PllDriver.c
 *
 *  Created on: 23/05/2023
 *      Author: Marianela Arcila Sanchez
 *  In this program, we write about the control of the PLL peripheral.
 *  This control aims to be as generic as possible.
 */
#include <stm32f4xx.h>
#include "PllDriver.h"

/* First, we need to configure the RCC (Reset and Clock Control) register.
 * so, we activate the PWR (Power Control) peripheral for proper control of the
 * energy in the PLL.
 */

void frequencyClock(CLOCK_Handler_t *ptrClock) {

    /* Now we configure the frequency of the MCU. We establish three important values:
     * PLLN as 80, PLLM as 4. Then we calculate VCO as 16MHz * (80/4) = 320,
     * and calculate PLLP as 80MHz / VCO = 4."*/

    switch (ptrClock->CLOCK_Config.frequency) {

        case MCU_FREQUENCY_80MHz:

            // PLLP Register
            RCC->PLLCFGR &= ~(0b11 << RCC_PLLCFGR_PLLP_Pos);
            RCC->PLLCFGR |= (0b01 << RCC_PLLCFGR_PLLP_Pos);

            // PLLN Register
            RCC->PLLCFGR &= ~(0b111111111 << RCC_PLLCFGR_PLLN_Pos);
            RCC->PLLCFGR |= (0x1010000 << RCC_PLLCFGR_PLLN_Pos); // Use hexadecimal value for PLLN (80 in decimal)

            // PLLM Register
            RCC->PLLCFGR &= ~(0b111111 << RCC_PLLCFGR_PLLM_Pos);
            RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
            break;

        case MCU_FREQUENCY_100MHz:

            //
            // PLLP Register
			RCC->PLLCFGR &= ~(0b11 << RCC_PLLCFGR_PLLP_Pos);
			RCC->PLLCFGR |= (0b00 << RCC_PLLCFGR_PLLP_Pos);

			// PLLN Register
			RCC->PLLCFGR &= ~(0b111111111 << RCC_PLLCFGR_PLLN_Pos);
			RCC->PLLCFGR |= (0b001100100<< RCC_PLLCFGR_PLLN_Pos); // Use hexadecimal value for PLLN (80 in decimal)
				 // PLLM Register
			RCC->PLLCFGR &= ~(0b111111 << RCC_PLLCFGR_PLLM_Pos);
			RCC->PLLCFGR |= (8 << RCC_PLLCFGR_PLLM_Pos);
			break;
    }
}

void configPll(CLOCK_Handler_t *ptrClock) {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN; // Enable PWR clock signal

    /* 1) We configure the regulator voltage scaling output.
     * This register controls the internal voltage. In this case,
     * since we need a frequency of 80MHz, we use the Scale 2 mode.
     */
    if(ptrClock -> CLOCK_Config.frequency ==MCU_FREQUENCY_80MHz){
    	PWR->CR &= ~(0b11 << PWR_CR_VOS_Pos);
    	PWR->CR |= (0b10 << PWR_CR_VOS_Pos);
    }
    else{
    	PWR->CR &= ~(0b11 << PWR_CR_VOS_Pos);
    	PWR -> CR |= (0b11<<PWR_CR_VOS_Pos);
    }

    /* 2) We configure the latency with the wait states in the Flash Memory.
     *    This is done in the ACR register, where the wait states are set to
     *    ensure proper functioning of the memory. (3.4.1 MR)
     *    In this particular case, we use the MOS in scale to. As a result,
     *    the wait states are set to 2 for the HCLK voltage between 2.7V and 3.6V.
     */
    if(ptrClock -> CLOCK_Config.frequency ==MCU_FREQUENCY_80MHz){
    	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
    }
    else{
    	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
    }


    /* 3) We activate the PLL clock signal in the CR register to RCC_PLLON
     *    and wait for the clock to stabilize.
     */
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* 4) We activate the PLL clock in the CFGR register SW (SYSCLK). */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* We clear the prescaler in the CFGR-> HPRE register AHB. */
    RCC->CFGR &= ~(RCC_CFGR_HPRE);

    /* We configure the MCO pin. */
    /* We configure the prescaler in the MC01 output, dividing the prescaler by 5. */
    //RCC->CFGR |= (0b111 <<  RCC_CFGR_MCO1PRE_Pos  );

    //prescalerClock(ptrClock);

    /* We activate the output MC01. */
    // We clear de register
    RCC->CFGR &= ~(0b11 << RCC_CFGR_MCO1_Pos );
    RCC->CFGR |= RCC_CFGR_MCO1;

    /* We configure the prescaler APB1. */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV16;
}

void typeClock(CLOCK_Handler_t *ptrClock){


	if( ptrClock -> CLOCK_Config.clock == CLOCK_HSI ){
		 RCC->CFGR &= ~(0b11 << RCC_CFGR_MCO1_Pos );


	}
	else if (ptrClock -> CLOCK_Config.clock == CLOCK_LSE ){
		//We clear de register
		RCC->CFGR &= ~(0b11 << RCC_CFGR_MCO1_Pos );
		//We select de
		RCC->CFGR |= (0b10 << RCC_CFGR_MCO1_Pos );


	}


}

void prescalerClock(CLOCK_Handler_t *ptrClock){

	if( ptrClock -> CLOCK_Config.prescaler == NO_DIVISION ){
		/* We configure the prescaler in the MC01 output, dividing the prescaler by 0. */
		//We clear de register


		RCC->CFGR &= ~(0b111 <<  RCC_CFGR_MCO1PRE_Pos);
	}
	else if( ptrClock -> CLOCK_Config.prescaler == DIVISION_BY2 ){
		//We clear de register
		RCC->CFGR &= ~(0b111 <<  RCC_CFGR_MCO1PRE_Pos);
		/* We configure the prescaler in the MC01 output, dividing the prescaler by 2. */
		RCC->CFGR |= (0b100 <<  RCC_CFGR_MCO1PRE_Pos);
	}
	else if( ptrClock -> CLOCK_Config.prescaler == DIVISION_BY3){
		//We clear de register
		RCC->CFGR &= ~(0b111 <<  RCC_CFGR_MCO1PRE_Pos);
		/* We configure the prescaler in the MC01 output, dividing the prescaler by 3. */
		RCC->CFGR |= (0b101 <<  RCC_CFGR_MCO1PRE_Pos);
	}
	else if( ptrClock -> CLOCK_Config.prescaler == DIVISION_BY4 ){
		//We clear de register
		RCC->CFGR &= ~(0b111 <<  RCC_CFGR_MCO1PRE_Pos);
		/* We configure the prescaler in the MC01 output, dividing the prescaler by 4. */
		RCC->CFGR |= (0b110 <<  RCC_CFGR_MCO1PRE_Pos);
	}
	else if( ptrClock -> CLOCK_Config.prescaler == DIVISION_BY5 ){
		//We clear de register
		RCC->CFGR &= ~(0b111 <<  RCC_CFGR_MCO1PRE_Pos);
		/* We configure the prescaler in the MC01 output, dividing the prescaler by 5. */
		RCC->CFGR |=  (0b111 <<  RCC_CFGR_MCO1PRE_Pos);
	}
}


