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

//----------------------------------------- 7 SEGMENTOS DEL DISPLAY----------------------------------------------------

//------------------------------------------ DIGITOS DISPLAY ------------------
GPIO_Handler_t handlerDigit4 = {0};
GPIO_Handler_t handlerDigit3 = {0};
GPIO_Handler_t handlerDigit2 = {0};
GPIO_Handler_t handlerDigit1 = {0};
// ---------------------------------------- 7 SEGMENTOS-------------------------
GPIO_Handler_t  handlerPunto   = {0};
GPIO_Handler_t  handlerPinLedA = {0};
GPIO_Handler_t  handlerPinLedB = {0};
GPIO_Handler_t  handlerPinLedC = {0};
GPIO_Handler_t  handlerPinLedD = {0};
GPIO_Handler_t  handlerPinLedE = {0};
GPIO_Handler_t  handlerPinLedF = {0};
GPIO_Handler_t  handlerPinLedG = {0};

//--------------------------------------TIMER INTERRUPCION-----------------------
BasicTimer_Handler_t  handlerTimerSG = {0};
GPIO_Handler_t handlerEncoderCLK = {0};
EXTI_Config_t handlerExtiEncoderCK   = {0};
GPIO_Handler_t handlerEncoderDT = {0};
EXTI_Config_t handlerExtiEncoderDT = {0};
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

	// LED Azul (A5)
	handlerLedBlue.pGPIOx = GPIOB;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerLedBlue.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerLedBlue);

    //----------------------------------------------------Encoder------------------------------------------------------
	// Botón SW en A1
	handlerButtonSW.pGPIOx = GPIOA;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinNumber = PIN_10;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	handlerButtonSW.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_PULLUP;
	GPIO_Config(&handlerButtonSW);

	// Configuración de la EXTI para A1
	handlerButtonSWExti.pGPIOHandler = &handlerButtonSW;
	handlerButtonSWExti.edgeType = EXTERNAL_INTERRUPT_RISING_EDGE; // o RISING_EDGE
	extInt_Config(&handlerButtonSWExti);

//----------------------------------------- 7 SEGMENTOS DEL DISPLAY----------------------------------------------------

	//--------------------------- DIGITOS DISPLAY (TRANSISTORES)-------------

	handlerDigit4.pGPIOx = GPIOC;
	handlerDigit4.GPIO_PinConfig.GPIO_PinNumber = PIN_9;
	handlerDigit4.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit4.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit4.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit4.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit4);

	handlerDigit3.pGPIOx = GPIOC;
	handlerDigit3.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerDigit3.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit3.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit3.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit3.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit3);

	handlerDigit2.pGPIOx = GPIOB;
	handlerDigit2.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	handlerDigit2.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit2.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit2.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit2);

	handlerDigit1.pGPIOx = GPIOC;
	handlerDigit1.GPIO_PinConfig.GPIO_PinNumber = PIN_6;
	handlerDigit1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	handlerDigit1.GPIO_PinConfig.GPIO_PinOPType = GPIO_OTYPE_PUSHPULL;
	handlerDigit1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;
	handlerDigit1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_OSPEED_FAST;
	GPIO_Config(&handlerDigit1);



	//-----------------------------------------------------------LEDS---------------------------------------------------
	handlerPunto.pGPIOx = 												GPIOA;
	handlerPunto.GPIO_PinConfig.GPIO_PinNumber =						PIN_12;
	handlerPunto.GPIO_PinConfig.GPIO_PinMode  =							GPIO_MODE_OUT;
	handlerPunto.GPIO_PinConfig.GPIO_PinOPType =         				GPIO_OTYPE_PUSHPULL;
	handlerPunto.GPIO_PinConfig.GPIO_PinPuPdControl =					GPIO_PUPDR_NOTHING;
	handlerPunto.GPIO_PinConfig.GPIO_PinSpeed=				            GPIO_OSPEED_FAST;

	//Ahora se carga la configuración
	GPIO_Config(&handlerPunto);

	handlerPinLedA.pGPIOx=                                  			GPIOB;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinNumber=           			PIN_6;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinMode=             			GPIO_MODE_OUT;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinOPType=          				GPIO_OTYPE_PUSHPULL;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinPuPdControl=    				GPIO_PUPDR_NOTHING;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinSpeed=            			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedA);


	handlerPinLedB.pGPIOx=                                   			GPIOA;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinNumber=            			PIN_7;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinPuPdControl=      			GPIO_PUPDR_NOTHING;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;

	GPIO_Config(&handlerPinLedB);

	handlerPinLedC.pGPIOx=                                   			GPIOC;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinNumber=            			PIN_5;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedC);


	handlerPinLedD.pGPIOx=                                   			GPIOA;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinNumber=            			PIN_11;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedD);


	handlerPinLedE.pGPIOx=                                   			GPIOA;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinNumber=            			PIN_6;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedE);


	handlerPinLedF.pGPIOx=                                   			GPIOB;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinNumber=            			PIN_12;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedF);


	handlerPinLedG.pGPIOx=                                  			GPIOB;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinNumber=            			PIN_9;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedG);


	//------------------------------------------------------------TIMER INTERRUPCION----------------------------------
/*

	handlerTimerSG.ptrTIMx = 										TIM3;
	handlerTimerSG.TIMx_Config.TIMx_mode = 						    BTIMER_MODE_UP;
	handlerTimerSG.TIMx_Config.TIMx_speed= 						    BTIMER_SPEED_1ms;
	handlerTimerSG.TIMx_Config.TIMx_period = 						10;
	handlerTimerSG.TIMx_Config.TIMx_interrupEnable=				    BTIMER_DISABLE;
	// We load the configuration
	BasicTimer_Config(&handlerTimerSG);
	// We activate the TIM3
	starTimer(&handlerTimerSG);

*/

	//------------------------------------------ENCODER---------------------------------------------------------------
	//---CLK-----------------------------------------------------

	handlerEncoderCLK.pGPIOx	= 										GPIOA;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinNumber = 				PIN_1;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderCLK.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiEncoderCK.pGPIOHandler= 									&handlerEncoderCLK;
	handlerExtiEncoderCK.edgeType = 									EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderCK);


	//-----DT--------------------------------------------------------


	handlerEncoderDT.pGPIOx	= 										GPIOA;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinNumber = 				PIN_2;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiEncoderDT.pGPIOHandler= 									&handlerEncoderDT;
	handlerExtiEncoderDT.edgeType = 									EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderCK);




}

//-------------------------------------------------------FSM function--------------------------------------------

// Implementación de la FSM
void fsm(void) {
    switch (currentState) {

        case STATE_IDLE:
            // MCU en espera
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
                    GPIO_WritePin(&handlerDigit4, SET);
                    GPIO_WritePin(&handlerDigit3, SET);
                    GPIO_WritePin(&handlerDigit2, SET);
                    GPIO_WritePin(&handlerDigit1, SET);
                    break;
                case COLOR_GREEN:
                    GPIO_WritePin(&handlerLedRed, RESET);
                    GPIO_WritePin(&handlerLedGreen, SET);
                    GPIO_WritePin(&handlerLedBlue, RESET);
                    GPIO_WritePin(&handlerDigit4, RESET);
                    GPIO_WritePin(&handlerDigit3, RESET);
                    GPIO_WritePin(&handlerDigit2, RESET);
                    GPIO_WritePin(&handlerDigit1, RESET);
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
                    break  ;
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
void writeSegments(int number){
    switch (number) {

    case 0:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 1:
        GPIO_WritePin(&handlerPinLedA, SET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 2:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, SET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 3:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 4:
        GPIO_WritePin(&handlerPinLedA, SET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 5:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 6:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 7:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 8:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 9:
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, RESET);
        GPIO_WritePin(&handlerPinLedC, RESET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 'O':
        GPIO_WritePin(&handlerPinLedA, SET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, SET);
        GPIO_WritePin(&handlerPinLedD, SET);
        GPIO_WritePin(&handlerPinLedE, SET);
        GPIO_WritePin(&handlerPinLedF, SET);
        GPIO_WritePin(&handlerPinLedG, SET);
        GPIO_WritePin(&handlerPunto, SET);
        break;

    case 'E':
        GPIO_WritePin(&handlerPinLedA, RESET);
        GPIO_WritePin(&handlerPinLedB, SET);
        GPIO_WritePin(&handlerPinLedC, SET);
        GPIO_WritePin(&handlerPinLedD, RESET);
        GPIO_WritePin(&handlerPinLedE, RESET);
        GPIO_WritePin(&handlerPinLedF, RESET);
        GPIO_WritePin(&handlerPinLedG, RESET);
        GPIO_WritePin(&handlerPunto, RESET);
        break;

    default:
        // Opcional: Manejar otros casos si es necesario.
        break;
    }
}















int main(void){
	initSystem();


	while(1){
		fsm(); // Llamada a la FSM
		writeSegments(8);
	}
	return 0;
}








//-------------------------------------------------------Callbacks-------------------------------------------------




void BasicTimer2_Callback(void){

	 currentState = STATE_BLINKY;

}


void BasicTimer3_Callback(void){

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
