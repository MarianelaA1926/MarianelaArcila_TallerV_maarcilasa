/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Marianela Arcila Sanchez     maarcilasa
 * @brief          : Main program body
 ******************************************************************************
 * Description:
 * In this project you will find the second task
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include  "GPIOxDriver.h"

/* We define the prototypes functions */
void InitSystem(void);

/* 1) "The GPIO_ReadPin function reads the status of a specific pin. However,
 *     it contains an error since it only moves the corresponding value selected by the user.
 *     We also need to shift the values of the other pins that were activated before the selected pin.
 *     These values are located to the left of the selected pin."
 *
 *    To correct this problem, we need to clear the other positions after selecting the corresponding
 *    value for the pin in PinValue using a Bitwise AND operation (GPIOxDriver.c)
 *    PinValue &= 0b1
 *
 */

/* 2) The Toggle function will be used to change the state. In order for this function to be general
 *    and reusable in other projects, we will write it in the GPIOxDriver.c file,
 *    and we call the function in the main*/


// function in charge of initializing system elements

// We define and initialize the handlers that we need
GPIO_Handler_t handlerToogleTestLed = {0};

void InitSystem(void){
	// Configurando el pin para el Led_Blinky
	handlerToogleTestLed.pGPIOx 								= GPIOA;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinNumber          = PIN_5 ;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_NOTHING;









}







int main(void)
{
    while(1){

    }
    return 0;
}
