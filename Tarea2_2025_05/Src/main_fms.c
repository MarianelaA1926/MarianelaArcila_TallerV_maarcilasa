/*
 * main_fms.c
 *
 *  Created on: 24/05/2025
 *      Author: marianela
 */

/* ******************************************************************************
 * @file           : main.c
 * @author         : doramirezag
 * @brief          : Desarrollo del driver para el USART
 ******************************************************************************
 * - Implementación de las librerías CMSIS
 * - Migración del GPIO driver
 * - Desarrollo de las librerías para el manejo de USART
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "stm32_assert.h"

//-------------------------------------------------------Led State---------------------------------------------
// Status LED control variables
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};

//---------------------------------------------------Reset Button-----------------------------------------------
GPIO_Handler_t handlerResetButton = {0};
EXTI_Config_t handlerExtiButton;

//-----------------------------------------------RGB LED--------------------------------------------------------

GPIO_Handler_t handlerLedRed = {0};
GPIO_Handler_t handlerLedGreen = {0};
GPIO_Handler_t handlerLedBlue = {0};


GPIO_Handler_t handlerButtonSW = {0};
EXTI_Config_t handlerButtonSWExti = {0};

uint8_t currentState;
uint8_t buttonPressed;
uint8_t currentColor;
//-------------------------------------------------------FSM----------------------------------------------------

// Definición de los estados de la FSM
typedef enum {

	STATE_IDLE,
	STATE_BLINKY,
	STATE_RGB

	// Agrega aquí otros estados que necesites
} State_t;


// Definicion Colores Led
typedef enum {
    COLOR_OFF = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_CYAN,    // azul + verde
    COLOR_MAGENTA, // azul + rojo
    COLOR_YELLOW,  // rojo + verde
    COLOR_WHITE    // rojo + verde + azul
} Color_t;


// Aquí la defines como **variable global**
volatile Color_t currentColor = COLOR_OFF;


/* Pin configuration function  */
void initSystem(void){

	// --------------------------------- State Led--------------------------------------------------//
	handlerStateLed.pGPIOx = 											GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = 						GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = 						GPIO_OSPEED_FAST;

	GPIO_Config(&handlerStateLed);

	// We configure TIMER 2 as the counter for the stateLed
	handlerTimerStateLed.ptrTIMx = 										TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 						650;
	handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable=				BTIMER_ENABLE;

	BasicTimer_Config(&handlerTimerStateLed);

	// We activate the TIM2
	starTimer(&handlerTimerStateLed);



	//------------------------------------------------Reset Butom-----------------------------------------------------


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerResetButton.pGPIOx	= 										GPIOA;
	handlerResetButton.GPIO_PinConfig.GPIO_PinNumber = 					PIN_8;
	handlerResetButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerResetButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerResetButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);


	//------------------------------------------------RGB LED-----------------------------------------------------


	handlerLedRed.pGPIOx = GPIOB;
	handlerLedRed.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
	handlerLedRed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedRed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedRed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedRed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedRed);

	// LED Verde (A4)
	handlerLedGreen.pGPIOx = GPIOB;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedGreen.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedGreen);

	// LED Azul (A5) — ya lo tenías como LED de estado, cámbialo si quieres.
	handlerLedBlue.pGPIOx = GPIOB;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlue);


	// Botón SW en A1
	handlerButtonSW.pGPIOx = GPIOA;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinNumber = PIN_10;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP; // o PULLDOWN según tu conexión
	GPIO_Config(&handlerButtonSW);

	// Configuración de la EXTI para A1
	handlerButtonSWExti.pGPIOHandler = &handlerButtonSW;
	handlerButtonSWExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE; // o RISING_EDGE
	extInt_Config(&handlerButtonSWExti);


}

//-------------------------------------------------------FSM function--------------------------------------------

// Implementación de la FSM
void fsm(void) {
    switch (currentState) {

        case STATE_IDLE:
            // MCU en espera (opcional: __WFI())
            break;

        case STATE_BLINKY:
            GPIOxTooglePin(&handlerStateLed);
            currentState = STATE_IDLE;
            break;

        case STATE_RGB:
            // Avanza al siguiente color
            currentColor++;
            if (currentColor > COLOR_WHITE){
                currentColor = COLOR_OFF;
            }

            // Configura los LEDs según el color actual
            switch(currentColor){
                case COLOR_RED:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break;
                case COLOR_GREEN:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break;
                case COLOR_BLUE:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_CYAN:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_MAGENTA:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_YELLOW:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break;
                case COLOR_WHITE:
                    GPIO_WritePin(&handlerLedRed, SET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, SET);
                    break;
                case COLOR_OFF:
                default:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, RESET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    break;
            }

            // Vuelve a estado IDLE
            currentState = STATE_IDLE;
            break;

        default:
            currentState = STATE_IDLE;
            break;
    }
}


//-------------------------------------------------------Main-----------------------------------------------------

int main(void){
	initSystem();


	while(1){
		fsm(); // Llamada a la FSM
	}
	return 0;
}

//-------------------------------------------------------Callbacks-------------------------------------------------

void BasicTimer2_Callback(void){

	 currentState = STATE_BLINKY;

}



void callback_extInt10(void){

    // delay por rebote (opcional)
    for (volatile int i=0; i<100000; i++);

    // Cambia el estado para que la FSM actualice el color
    currentState = STATE_RGB;
}

// Callback function with External Button (pin A2)
void callback_extInt8(void){
    buttonPressed = GPIO_ReadPin(&handlerResetButton);

    if (buttonPressed == RESET) {
        // Apaga el LED antes de reiniciar
        GPIO_WritePin(&handlerStateLed, RESET);

        // ¡Pequeño retardo para que el LED realmente se apague!
        for (volatile int i=0; i<100000; i++);

        NVIC_SystemReset();
    }
}








void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}
}
