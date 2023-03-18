/*
 * GPIOxDriver.h
 *
 *  Created on: 10/03/2023
 *      Author: Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for defining the control of the GPIOx peripheral. (control funtions)
 */

#ifndef GPIOXDRIVER_H_
#define GPIOXDRIVER_H_

#include <stm32f4xx.h>


/* These are the standard values for the configuration */
/* 8.4.1 GPIOx_MODER (Two bits for the Pin) */

#define GPIO_MODE_IN                0
#define GPIO_MODE_OUT				1
#define GPIO_MODE_ALTFN				2
#define GPIO_MODE_ANALOG			3

/* 8.4.2 GPIOx_OTYPER (_One bit for PIN)*/

#define GPIO_OTYPE_PUSHPULL     0
#define GPIO_OTYPE_OPENDRAIN	1

/*8.4.3 GPIOx_OSPEEDR (Two bits for the Pin) */
#define GPIO_OSPEED_LOW  		0
#define GPIO_OSPEED_MEDIUM		1
#define GPIO_OSPEED_FAST		2
#define GPIO_OSPEED_HIGH		3

/* 8.4.4 GPIOx_PUPDR (Two bits for the Pin) */
#define GPIO_PUPDR_NOTHING       0
#define GPIO_PUPDR_PULLUP		 1
#define GPIO_PUPDR_PULLDOWN		 2
#define GPIO_PUPDR_RESERVED		 3

/* 8.4.5 GPIOx_IDR( One bit for PIN) - This register is used to read the state of the pin. */

/*8.4.6 GPIOx_ODR (un bit por PIN) This register is used to read the state of the pin.(1 o 0).
 *"This register can be written to and read from using software, but it doesn't support 'atomic write'.
 *This is why we use the BSRR register." */

/*Pin names */

#define PIN_0
#define PIN_1
#define PIN_2
#define PIN_3
#define PIN_4
#define PIN_5
#define PIN_6
#define PIN_7
#define PIN_8
#define PIN_9
#define PIN_10
#define PIN_11
#define PIN_12
#define PIN_13
#define PIN_14
#define PIN_15

/* Alternative Funtions */

#define AF0				0b0000
#define AF1             0b0001
#define AF2             0b0010
#define AF3				0b0011
#define AF4				0b0100
#define AF5				0b0101
#define AF6				0b0110
#define AF7				0b0111
#define AF8				0b1000
#define AF9				0b1001
#define AF10			0b1010
#define AF11			0b1011
#define AF12			0b1100
#define AF13			0b1101
#define AF14			0b1110
#define AF15			0b1111

typedef struct{

	uint8_t GPIO_PinNumber; 			// Pin which we use to work
	uint8_t GPIO_PinMode;  				// mode to configuration: input, output, mode analog, alternative function
	uint8_t GPIO_PinSpeed; 				// Speed to response (only for digital mode)
	uint8_t GPIO_PinPuPdControl; 		// if we need an output with Pull-up Pull-down  or free
	uint8_t GPIO_PinOPType;				//Select the output with PUPD or OpenDrain.
	uint8_t GPIO_PinAltFunMode;			// Select the alternative function
}GPIO_PinConfig_t;


/*
 * "This structure contains two elements:

    The address used in the port
    The configuration specific to the pin that we can use
 */

typedef struct{
	GPIO_TypeDef			*pGPIOx;     			/* !< Address of the port to which the PIN corresponds >*/
	GPIO_PinConfig_t 		GPIO_PinConfig; 		/*!< PIN configuration >*/

}GPIO_Handler_t;


/* Definition of the headers of the GPIOxDriver functions */

void GPIO_Config (GPIO_Handler_t * pGPIOHandler);
void GPIO_WritePin(GPIO_Handler_t *pPinHandler, uint8_t newState);
uint32_t GPIO_ReadPin(GPIO_Handler_t *pPinHandler);







#endif /* GPIOXDRIVER_H_ */
