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
void delay(int seconds);


/*We define the point one and two function*/

/* 1) "The GPIO_ReadPin function reads the status of a specific pin. However,
 *     it contains an error since it only moves the corresponding value selected by the user.
 *     We also need to shift the values of the other pins that were activated before the selected pin.
 *     These values are located to the left of the selected pin."
 *
 *    To correct this problem, we need to clear the other positions after selecting the corresponding
 *    value for the pin in PinValue using a Bitwise AND operation (GPIOxDriver.c)
 *	  This way, we can read the least significant bit.
 *    PinValue &= 0b1
 *
 */

/* 2) The Toogle function will be used to change the state. In order for this function to be general
 *    and reusable in other projects, we will write it in the GPIOxDriver.c file,
 *    and we call the function in the main*/


// function in charge of initializing system elements

// We define and initialize the handlers that we need
GPIO_Handler_t handlerToogleTestLed = {0};
GPIO_Handler_t handlerLedC9 = {0};
GPIO_Handler_t handlerLedC6 = {0};
GPIO_Handler_t handlerLedB8 = {0};
GPIO_Handler_t handlerLedA6 = {0};
GPIO_Handler_t handlerLedC7 = {0};
GPIO_Handler_t handlerLedC8 = {0};
GPIO_Handler_t handlerLedA7 = {0};
GPIO_Handler_t handlerUserButtomBlue = {0};




void InitSystem(void){

	// this is the configuration of Led_TestToogle,
	handlerToogleTestLed.pGPIOx									= GPIOA;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinNumber 			= PIN_5;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerToogleTestLed.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	// We load the configuration to the LED
	GPIO_Config(&handlerToogleTestLed);

	//We write in the register to activate the GREN LED and check the GPIO_ReadPin and Toogle function
	GPIO_WritePin(&handlerToogleTestLed, SET);

//============================================== Task Point three================================================

	// this is the configuration of PC9,
	handlerLedC9.pGPIOx									= GPIOC;
	handlerLedC9.GPIO_PinConfig.GPIO_PinNumber 			= PIN_9;
	handlerLedC9.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedC9.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedC9.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedC9.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLDOWN;
	// We load the configuration to the LED
	GPIO_Config(&handlerLedC9);




	// this is the configuration of PC6,
	handlerLedC6.pGPIOx									= GPIOC;
	handlerLedC6.GPIO_PinConfig.GPIO_PinNumber 			= PIN_6;
	handlerLedC6.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedC6.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedC6.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedC6.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedC6);




	// this is the configuration of PB8,
	handlerLedB8.pGPIOx									= GPIOB;
	handlerLedB8.GPIO_PinConfig.GPIO_PinNumber 			= PIN_8;
	handlerLedB8.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedB8.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedB8.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedB8.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedB8);




	// this is the configuration of PA6,
	handlerLedA6.pGPIOx									= GPIOA;
	handlerLedA6.GPIO_PinConfig.GPIO_PinNumber 			= PIN_6;
	handlerLedA6.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedA6.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedA6.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedA6.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedA6);




	// this is the configuration of PC7,
	handlerLedC7.pGPIOx									= GPIOC;
	handlerLedC7.GPIO_PinConfig.GPIO_PinNumber 			= PIN_7;
	handlerLedC7.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedC7.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedC7.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedC7.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedC7);



	// this is the configuration of PC8,
	handlerLedC8.pGPIOx									= GPIOC;
	handlerLedC8.GPIO_PinConfig.GPIO_PinNumber 			= PIN_8;
	handlerLedC8.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedC8.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedC8.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedC8.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedC8);




	// this is the configuration of PA7,
	handlerLedA7.pGPIOx									= GPIOA;
	handlerLedA7.GPIO_PinConfig.GPIO_PinNumber 			= PIN_7;
	handlerLedA7.GPIO_PinConfig.GPIO_PinMode			= GPIO_MODE_OUT;
	handlerLedA7.GPIO_PinConfig.GPIO_PinOPType			= GPIO_OTYPE_PUSHPULL;
	handlerLedA7.GPIO_PinConfig.GPIO_PinSpeed			= GPIO_OSPEED_FAST;
	handlerLedA7.GPIO_PinConfig.GPIO_PinPuPdControl		= GPIO_PUPDR_PULLUP;

	//We load the configuration to the LED
	GPIO_Config(&handlerLedA7);



	// this is the configuration of Buttom Blue Pin C13
	handlerUserButtomBlue.pGPIOx 									=GPIOC;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinNumber  			=PIN_13;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinMode 				=GPIO_MODE_IN;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinOPType  			=GPIO_OTYPE_OPENDRAIN;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinPuPdControl  		=GPIO_PUPDR_NOTHING;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinSpeed  			=GPIO_OSPEED_MEDIUM;
	handlerUserButtomBlue.GPIO_PinConfig.GPIO_PinAltFunMode 		= AF0;
	//Cargamos la configuración del pin específico
	GPIO_Config(&handlerUserButtomBlue);

}






int main(void)
{
	// We call the prototype functions with the pin configuration.
	InitSystem();


	/*To test  'read' and Toogle function, we use the LD2 LED. This green LED is
	 * connected to pin A5, so we configure pin 5 in the peripheral GPIOA.
	 */

	//We need to check that the GPIO_ReadPin function is working properly
	GPIO_ReadPin(&handlerToogleTestLed);

	/*We call the toggle function to activate or deactivate the green
	LED according to the state written in the GPIO_WritePin function.*/
	GPIOxTooglePin(&handlerToogleTestLed);

//============================= task point tree ============================================0

	int count = 1;
	// the count starts at 1 so that the LED starts activated
	while(1){

	/*We call the function and write the count, the idea is that
	 * the system reactivates every 60 if we press the blue button,
	 *  the count starts at 1 and ends at 60
	 *  But if we don't press it, the counter
	 *   starts at 60 and ends at 1
	 */

		GPIO_WritePin(&handlerLedC9, count);
		GPIO_WritePin(&handlerLedC6, count);
		GPIO_WritePin(&handlerLedB8, count);
		GPIO_WritePin(&handlerLedA6, count);
		GPIO_WritePin(&handlerLedC7, count);
		GPIO_WritePin(&handlerLedC8, count);
		GPIO_WritePin(&handlerLedA7, count);

		// we call the delay of 1
		delay(1);
		// we generate the count up to 60


		if(count > 60){
			count = 1;
		} else if (count < 1){
			count = 60;
		}

		// 	We create the condition that it increases and decreases according to the state of the button


		uint32_t button_Pressed = GPIO_ReadPin(&handlerUserButtomBlue);
		if (button_Pressed == RESET){

			count --;
		}
		else {

			count ++;
		}


	}


}


//We create a new  variable  that introduces a one-second delay

void delay(int seconds){

/* 	If the processor executes at least 1 billion
 *  cycles per second, we need 10 billion iterations in the for loop.
 *  The seconds variable is where the time the user wants to delay is stored.
 */
	// for cycle

	for (int i = 0; i< seconds * 1000000; i++){


	}
}


