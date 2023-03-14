/*
 * GPIOxDriver.c
 *
 *  Created on: 13/03/2023
 *      Author: Marianela Arcila Sanchez
 *  In this program, we write about the control of the GPIO peripheral.
 *  This control aims to be as generic as possible.
 */

#include "GPIOxDriver.h"

/*First, we need to activate the clock signal. This is very important because
 * the clock signal is used to synchronize the different components of the
 *  microcontroller. (RCC)
 */


void GPIO_Config (GPIO_Handler_t *pGPIOHandler ){

	//create variable
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	// Activate Peripheric
	// For GPIOA
	if(pGPIOHandler -> pGPIOx == GPIOA){
	// We write 1 (SET)  in the position corresponds to GPIOA
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOA_EN);
	}
	// For GPIOB
	else if(pGPIOHandler -> pGPIOx == GPIOB){
	// We write 1 (SET)  in the position corresponds to GPIOB
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOB_EN);
	}
	// For GPIOC
	else if(pGPIOHandler -> pGPIOx == GPIOC	){
	// We write 1 (SET)  in the position corresponds to GPIOC
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOC_EN);
	}
	// For GPIOD
	else if(pGPIOHandler -> pGPIOx == GPIOD ){
	// We write 1 (SET)  in the position corresponds to GPIOD
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOD_EN);
	}
	// For GPIOC
	else if(pGPIOHandler -> pGPIOx == GPIOE	){
	// We write 1 (SET)  in the position corresponds to GPIOE
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOE_EN);
	}
	// For GPIOC
	else if(pGPIOHandler -> pGPIOx == GPIOH	){
	// We write 1 (SET)  in the position corresponds to GPIOH
		RCC -> AHB1ENR |= (SET << RCC_AHB1EMR_GPIOH_EN);
	}

	/* Then we star to configure the  GPIO registers*/




	/* The second step is to configure the GPIOx_MODER register.
	 * This register indicates the mode of operation of the pin
	 * that you want to use. (input, output, alternative function, analog mode)
	 */

	/*"We read the 'config' given by the user,
	 * and then we move 'Pin Number' times to  the right using a 'shift left' operation."
	 * This in auxConfig
	 */

	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/* We clear the bits in the register before loading the new value.
	 * for this step, we use a mask and operation Bitwise AND (0b000)*
	 */
	pGPIOHandler -> pGPIOx -> MODER &= ~(0b11 << * pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We load auxConfig to the MODER register*/

	pGPIOHandler -> pGPIOx -> MODER |= auxConfig;




	/* The third step is to configure the GPIOx_TYPER register. In this register,
	 * we define whether the pin will be configured as an output push-pull (0 - 1) or an output open-drain.
	 * We read and move the value "PinNumber" times
	 */
	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinOPType << pGPIOHandler ->GPIO_PinConfig.GPIO_PinNumber);

	/*We clear before to loading the new value*/
	pGPIOHandler -> pGPIOx ->OTYPER &= ~(SET << pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We load the new value for the register */
	pGPIOHandler -> pGPIOx ->OTYPER |= auxConfig;





	/* The fourth step is to configure the speed of the output, register  GPIOx_OSPEEDR*/
	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We clear before to loading the new value*/
	pGPIOHandler ->pGPIOx ->OSPEEDR &=  ~(0b11 << 2* pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We load the new value for the register*/
	pGPIOHandler -> pGPIOx ->OSPEEDR |= auxConfig;



	/*tThe fifth step is to configure the GPIOx_PUPDR register,
	 *in which we establish a defined state for the signal. (Pull-up, pull-down) */

















}
