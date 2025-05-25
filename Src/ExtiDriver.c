/*
 * ExtiDriver.c
 *
 *  Created on: 6/04/2023
 *      Author: Marianela Arcila Sanchez
 * In this program, we write about the control of the external interruptions peripheral.
 * This control aims to be as generic as possible.
 *
 */



#include "ExtiDriver.h"
#include "GPIOxDriver.h"



void extInt_Config(EXTI_Config_t *extiConfig){

	/* We configure the PIN as a 'simple input'  to receive the external event.
	 * To do this, we call the GPIO_Config function from the GPIO Driver." */


	GPIO_Config(extiConfig -> pGPIOHandler );

	/* 2.0 We activate the signal clock of the required pheriferal */
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* 3.0  We create a 'switch case' to assign the PIN and port to the EXTI channel.
	 * There are 16 lines for interruption, but these are configured with 4 registers (CR1, CR2, CR3, CR4) in the SYSCFG.
     * In the SYSCFG (System Configuration), we configure the external interruptions. */

	switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {
	/* We configure for all GPIOx_0 pins.*/
	case 0: {
		/* SYSCFG_EXTICR1 */
		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI0_Pos);

		// We configure the port that corresponds to the pin 0
		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PH);

		} else {
			__NOP();
		}

		break;
	}

	/* We configure for all GPIOx_1 pins*/
	/* SYSCFG_EXTICR1 */
	case 1: {
		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI1_Pos);
		// We configure the port that corresponds to the pin 1
		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI1_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_2 pins*/
	/* SYSCFG_EXTICR1 */
	case 2: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI2_Pos);
		// We configure the port that corresponds to the pin 2

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI2_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_3 pins*/
	/* SYSCFG_EXTICR1 */
	case 3: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[0] &= ~(0xF << SYSCFG_EXTICR1_EXTI3_Pos);
		// We configure the port that corresponds to the pin 3

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI3_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_4 pins*/
	/* SYSCFG_EXTICR2 */
	case 4: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI4_Pos);
		// We configure the port that corresponds to the pin 4

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI4_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_5 pins*/
	/* SYSCFG_EXTICR2 */
	case 5: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI5_Pos);
		// We configure the port that corresponds to the pin 5

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI5_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_6 pins*/
	/* SYSCFG_EXTICR2 */
	case 6: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI6_Pos);
		// We configure the port that corresponds to the pin 6

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI6_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_7 pins*/
	/* SYSCFG_EXTICR2 */
	case 7: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[1] &= ~(0xF << SYSCFG_EXTICR2_EXTI7_Pos);
		// We configure the port that corresponds to the pin 7

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[1] |= (SYSCFG_EXTICR2_EXTI7_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_8 pins*/
	/* SYSCFG_EXTICR3 */
	case 8: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI8_Pos);
		// We configure the port that corresponds to the pin 8

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI8_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_9 pins*/
	/* SYSCFG_EXTICR3 */
	case 9: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI9_Pos);
		// We configure the port that corresponds to the pin 9

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI9_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_10 pins*/
	/* SYSCFG_EXTICR3 */
	case 10: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI10_Pos);
		// We configure the port that corresponds to the pin 10

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI10_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_11 pins*/
	/* SYSCFG_EXTICR3 */
	case 11: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[2] &= ~(0xF << SYSCFG_EXTICR3_EXTI11_Pos);
		// We configure the port that corresponds to the pin 11

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[2] |= (SYSCFG_EXTICR3_EXTI11_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_12 pins*/
	/* SYSCFG_EXTICR4 */
	case 12: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI12_Pos);
		// We configure the port that corresponds to the pin 12

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI12_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_13 pins*/
	/* SYSCFG_EXTICR4 */
	case 13: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI13_Pos);

		// We configure the port that corresponds to the pin 13

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG -> EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI13_PH);
		} else {
					__NOP();
				}
		break;
	}

	/* We configure for all GPIOx_14 pins*/
	/* SYSCFG_EXTICR4 */
	case 14: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI14_Pos);
		// We configure the port that corresponds to the pin 14

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI14_PH);
		} else {
					__NOP();
				}
		break;
	}


	/* We configure for all GPIOx_15 pins*/
	/* SYSCFG_EXTICR4 */
	case 15: {

		// We first clear the bit position that we want to configure
		SYSCFG->EXTICR[3] &= ~(0xF << SYSCFG_EXTICR4_EXTI15_Pos);

		// We configure the port that corresponds to the pin 15

		if (extiConfig->pGPIOHandler->pGPIOx == GPIOA) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PA);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOB) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PB);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOC) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PC);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOD) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PD);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOE) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PE);

		} else if (extiConfig->pGPIOHandler->pGPIOx == GPIOH) {
			SYSCFG->EXTICR[3] |= (SYSCFG_EXTICR4_EXTI15_PH);

		} else {
			__NOP();
		}
		break;
	}

	default: {
		__NOP();
		break;
	}

	}// End switch-case




	/* 4.0 We select the type of edge. We use the FTSR (Falling Trigger Selection Register)
	 * and RTSR (Rising Trigger Selection Register) to detect the edge for the external interrupt*/
	if(extiConfig->edgeType == EXTERNAL_INTERRUPT_FALLING_EDGE){


		/* Falling Trigger selection register*/
		EXTI -> FTSR |= SET << (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) ;
		EXTI -> RTSR &= ~SET <<((extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber));
	}

	else{
		EXTI -> RTSR |= SET <<(extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);
		EXTI -> FTSR &= ~SET << (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) ;

	}



	/* 6.0 We activate the external interrupt with the IMR register */
	// Interrupt Mask register
	EXTI -> IMR |= SET << (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber);

	/* 5.0 We deactivate global interruptions while we configure the EXTI system */
	__disable_irq();

	/* 6.1 We enable the interrupt in the NVIC for the corresponding channel,
	 * where channel 0 corresponds to EXTI_0, channel 1 to EXTI_1, etc.
	 *
	 * Note: Observe that some EXTI channels share the same interrupt vector.
	 */
	switch (extiConfig->pGPIOHandler->GPIO_PinConfig.GPIO_PinNumber) {
	case 0: {
		__NVIC_EnableIRQ(EXTI0_IRQn);
		break;
	}

	case 1: {
		__NVIC_EnableIRQ(EXTI1_IRQn);
		break;
	}
	case 2: {
		__NVIC_EnableIRQ(EXTI2_IRQn);
		break;
	}
	case 3: {
		__NVIC_EnableIRQ(EXTI3_IRQn);
		break;
	}
	case 4: {
		__NVIC_EnableIRQ(EXTI4_IRQn);
		break;
	}
	case 5: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}
	case 6: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}
	case 7: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}
	case 8: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}
	case 9: {
		__NVIC_EnableIRQ(EXTI9_5_IRQn);
		break;
	}

	case 10: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}
	case 11: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}
	case 12: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}
	case 13: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}
	case 14: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}
	case 15: {
		__NVIC_EnableIRQ(EXTI15_10_IRQn);
		break;
	}

	default: {
		break;
	}

	}

	/* 7.0 we re-enable system interrupts */
	__enable_irq();
}


/*we declare the Callback function. This function defines
 * an action that is executed in response to an interrupt event.
 * It can perform actions such as updating a variable, activating a peripheral,
 * or any other action needed to handle the  interrupt.
 *
 * The attribute ((weak)) indicates that it is a "loosely bound function".
 * If this function is defined elsewhere in the program, the function in the
 * current file is modified and replaced with the other definition found in the
 * program*/

__attribute__ ((weak)) void callback_extInt0(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt1(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt2(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt3(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt4(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt5(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt6(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt7(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt8(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt9(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt10(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt11(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt12(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt13(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt14(void){
	__NOP();
}
__attribute__ ((weak)) void callback_extInt15(void){
	__NOP();
}


/*we need to create another function that knows which functions need
 * to be executed when the interruption happens. To do this, we create a new function
 * with the same name used in the interruption vector.
 */

/* ISR  Chanel 0*/
void EXTI0_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_0 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR0){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR0;

		// we call the callback function
		callback_extInt0();
	}
}

void EXTI1_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_1 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR1){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR1;

		// we call the callback function
		callback_extInt1();
	}
}
void EXTI2_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_2 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR2){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR2;

		// we call the callback function
		callback_extInt2();
	}
}
void EXTI3_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_3 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR3){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR3;

		// we call the callback function
		callback_extInt3();
	}
}
void EXTI4_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_4 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR4){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR4;

		// we call the callback function
		callback_extInt4();
	}
}


/* We need to create the ISR for the interrupt channels 9 to 5.
 * It's important to add all possible cases, which are identified
 * by an if() block that analyzes the pending register (EXTI_PR) flag.
 */
void EXTI9_5_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_5 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR5){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR5;

		// we call the callback function
		callback_extInt5();

	}
	//We check if the interrupt that was triggered corresponds to PIN_6 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR6){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR6;

		// we call the callback function
		callback_extInt6();

	}
	//We check if the interrupt that was triggered corresponds to PIN_7 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR7){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR7;

		// we call the callback function
		callback_extInt7();

	}
	//We check if the interrupt that was triggered corresponds to PIN_8 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR8){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR8;

		// we call the callback function
		callback_extInt8();

	}
	//We check if the interrupt that was triggered corresponds to PIN_9 of the GPIO_X.
	else {
		EXTI->PR |= EXTI_PR_PR9;
		// we call the callback function
		callback_extInt9();

	}
}

/* We need to create the ISR for the interrupt channels 10to 15. It's important to add
 * all possible cases, which are identified by an if() block that analyzes the pending
 * register (EXTI_PR) flag.
 */
void EXTI15_10_IRQHandler(void){
	// We check if the interrupt that was triggered corresponds to PIN_10 of the GPIO_X.
	if(EXTI->PR & EXTI_PR_PR10){
		// We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR10;

		// we call the callback function
		callback_extInt10();

	}
	//We check if the interrupt that was triggered corresponds to PIN_11 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR11){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR11;

		// we call the callback function
		callback_extInt11();

	}
	//We check if the interrupt that was triggered corresponds to PIN_12 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR12){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR12;

		// we call the callback function
		callback_extInt12();

	}
	//We check if the interrupt that was triggered corresponds to PIN_13 of the GPIO_X.
	else if(EXTI->PR & EXTI_PR_PR13){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR13;

		// we call the callback function
		callback_extInt13();

	}
	//We check if the interrupt that was triggered corresponds to PIN_14 of the GPIO_X.
	else if (EXTI->PR & EXTI_PR_PR14){
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR14;
		// we call the callback function
		callback_extInt14();

	}
	//We check if the interrupt that was triggered corresponds to PIN_15 of the GPIO_X.
	else {
		//We clear the corresponding flag.
		EXTI->PR |= EXTI_PR_PR15;
		// we call the callback function
		callback_extInt15();

	}


}
