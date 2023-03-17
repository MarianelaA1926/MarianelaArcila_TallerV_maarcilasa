/*
 * GPIOxDriver.c
 *
 *  Created on: 13/03/2023
 *      Author: Marianela Arcila Sanchez
 *  In this program, we write about the control of the GPIO peripheral.
 *  This control aims to be as generic as possible.
 */


/*First, we need to activate the clock signal. This is very important because
 * the clock signal is used to synchronize the different components of the
 *  microcontroller. (RCC)
 */
#include "GPIOxDriver.h"

void GPIO_Config (GPIO_Handler_t *pGPIOHandler ){

	//create variable
	uint32_t auxConfig = 0;
	uint32_t auxPosition = 0;

	// Activate Peripheric
	// For GPIOA
	if(pGPIOHandler -> pGPIOx == GPIOA){
	// We write 1 (SET)  in the position corresponds to GPIOA
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIOAEN);
	}
	// For GPIOB
	else if(pGPIOHandler -> pGPIOx == GPIOB){
	// We write 1 (SET)  in the position corresponds to GPIOB
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIOBEN);
	}
	// For GPIOC
	else if(pGPIOHandler -> pGPIOx == GPIOC	){
	// We write 1 (SET)  in the position corresponds to GPIOC
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIOCEN);
	}
	// For GPIOD
	else if(pGPIOHandler -> pGPIOx == GPIOD ){
	// We write 1 (SET)  in the position corresponds to GPIOD
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIODEN);
	}
	// For GPIOE
	else if(pGPIOHandler -> pGPIOx == GPIOE	){
	// We write 1 (SET)  in the position corresponds to GPIOE
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIOEEN);
	}
	// For GPIOH
	else if(pGPIOHandler -> pGPIOx == GPIOH	){
	// We write 1 (SET)  in the position corresponds to GPIOH
		RCC -> AHB1ENR |= (SET << RCC_AHB1ENR_GPIOHEN);
	}

	/* Then we star to configure the  GPIO registers*/




	/* The second step is to configure the GPIOx_MODER register.
	 * This register indicates the mode of operation of the pin
	 * that you want to use. (input, output, alternative function, analog mode)
	 */

	/*"We read the 'config' given by the user,
	 * and then we move 'Pin Number' times to  the right using a 'shift left' operation."
	 * This is in auxConfig
	 */

	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinMode << 2 * pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/* We clear the bits in the register before loading the new value.
	 * for this step, we use a mask and operation Bitwise AND (0b000)*
	 */
	pGPIOHandler -> pGPIOx -> MODER &= ~(0b11 << 2 *pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

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





	/* The fourth step is to configure the speed of the output,
	 *  register  GPIOx_OSPEEDR*/

	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinSpeed << 2*pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We clear before to loading the new value*/
	pGPIOHandler ->pGPIOx ->OSPEEDR &=  ~(0b11 << 2* pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We load the new value for the register*/
	pGPIOHandler -> pGPIOx ->OSPEEDR |= auxConfig;




	/*The fifth step is to configure the GPIOx_PUPDR register,
	 *in which we establish a defined state for the signal. (Pull-up, pull-down) */

	auxConfig = (pGPIOHandler -> GPIO_PinConfig.GPIO_PinPuPdControl << 2*pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We clear the register position before loading the new configuration*/
	pGPIOHandler -> pGPIOx -> PUPDR &= ~ (0b11 << 2* pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber);

	/*We Load the new value*/
	pGPIOHandler ->pGPIOx ->PUPDR |= auxConfig;



	/*The sixth step is to configurate the alternative funtions GPIOx_ALTFN
	 * The alternative functions are additional functions such as serial communication (USART),
	 * analog interfaces (ADC), among others, which are assigned to specific pins."
	 */
	if(pGPIOHandler -> GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){

		/* We select the register AFRL for the pins from 0 to 7 or
		 * the register AFRH for the pins from 8 to 15  */
		if(pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber < 8){
			//Register AFRL
			auxPosition = 4 * pGPIOHandler ->GPIO_PinConfig.GPIO_PinNumber;

			//We clear the position to the register AFRL
			pGPIOHandler -> pGPIOx -> AFR[0] &= ~(0B1111 << auxPosition);

			// We write the value into the AFRL register
			pGPIOHandler -> pGPIOx ->AFR[0] |= (pGPIOHandler -> GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}
		else{

			//Register AFRH
			auxPosition = 4 * (pGPIOHandler -> GPIO_PinConfig.GPIO_PinNumber -8);
			// We clear the position to the register AFRH
			pGPIOHandler ->pGPIOx -> AFR[1] &= ~(0b1111 << auxPosition);

			// We write the value into the AFRH register
			pGPIOHandler ->pGPIOx ->AFR[1] |= (pGPIOHandler ->GPIO_PinConfig.GPIO_PinAltFunMode << auxPosition);
		}

	}

} // The end GPIO_config



/*
 * Then we create a new function. In this function,
 * we change the state of the selected pin according
 * to the user's input in the handler.
 */

void GPIO_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState){

	//We clear the position we need
	//pPinHandler ->pGPIOx ->ODR &= ~(SET << pPinHandler -> GPIO_PinConfig.GPIO_PinNumber);
	if(newState == SET){
		// The lower part of the register.
		pPinHandler -> pGPIOx ->BSRR |= (SET << pPinHandler -> GPIO_PinConfig.GPIO_PinNumber);
	}
	else{
		// The higher part of the register
		pPinHandler -> pGPIOx ->BSRR |= (SET << (pPinHandler -> GPIO_PinConfig.GPIO_PinNumber + 16));
	}

}



/* We create the function that reads the status of a specific pin.*/

uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler){
	//create a new auxiliary variable
	uint32_t pinValue = 0;
	/*We load the value into the GPIOx_IDR,
	 * then shift it to the right. This register is used to store information
	 * when the pin is an input.*/
	pinValue = (pPinHandler -> pGPIOx -> IDR >> pPinHandler -> GPIO_PinConfig.GPIO_PinNumber);
	return pinValue;

}


