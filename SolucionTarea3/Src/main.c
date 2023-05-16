/*
 * ExtiConfig_Main.c
 *
 *  Created on: 24/03/2023
 *      Author: marianela
 */
/**
 ******************************************************************************
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

/* Variables del proyecto */


//-------------------------------------------------------Led State---------------------------------------------
// Status LED control variables
// handlers  GPIOA pin A5 y  de TIM2
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};
//---------------------------------------------------------Buton-------------------------------------------------------


GPIO_Handler_t handlerExternalLed = {0};

GPIO_Handler_t handlerExternalButton = {0};
EXTI_Config_t handlerExtiButton;

uint8_t buttonPressed;
//------------------------------------------------------Encoder-----------------------------------------------------------------

GPIO_Handler_t handlerEncoderClock = {0};
EXTI_Config_t handlerExtiEncoderCK = {0};


GPIO_Handler_t handlerEncoderDT = {0};


GPIO_Handler_t handlerEncoderSW = {0};
EXTI_Config_t handlerExtiEncoderSW = {0};


//-------------------------------------------------7Segmentos------------------------------------------------------------

GPIO_Handler_t  handlerPunto   = {0};
GPIO_Handler_t  handlerPinLedA = {0};
GPIO_Handler_t  handlerPinLedB = {0};
GPIO_Handler_t  handlerPinLedC = {0};
GPIO_Handler_t  handlerPinLedD = {0};
GPIO_Handler_t  handlerPinLedE = {0};
GPIO_Handler_t  handlerPinLedF = {0};
GPIO_Handler_t  handlerPinLedG = {0};
GPIO_Handler_t  handlerTransitor1 = {0};
GPIO_Handler_t  handlerTransitor2 = {0};


BasicTimer_Handler_t handlerTimerSG = {0};


// Variables globales para almacenar el valor actual y el máximo permitido
uint8_t count = 0;
uint8_t decenas;
uint8_t unidades;
int activo = 1;



/* Principal Functions */
void initSystem(void);


void numeros(int numero);
void Delay(uint32_t value);


int main(void)
{
	initSystem();
	while(1){


	}
	return 0;
}

/* Pin configuration function  */

void initSystem(void){

	// --------------------------------- State Led--------------------------------------------------//
	// Pórt GPIOA pin A5
	handlerStateLed.pGPIOx = 											GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = 						GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = 						GPIO_OSPEED_FAST;


	//We load the configuration of the specific pin

	GPIO_Config(&handlerStateLed);


	// We configure TIMER 2 as the counter for the stateLed
	handlerTimerStateLed.ptrTIMx = 										TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 						250;
	handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable=				BTIMER_ENABLE;
	// We load the configuration
	BasicTimer_Config(&handlerTimerStateLed);
	// We activate the TIM2
	starTimer(&handlerTimerStateLed);
//------------------------------------------------External Butom-----------------------------------------------------


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerExternalButton.pGPIOx	= 										GPIOA;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinNumber = 				PIN_8;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerExternalButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerExternalButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);



//------------------------------------------Encoder----------------------------------------------------------------
//---CLOCK-----------------------------------------------------
	/*ENCODER CLOCK configuration as external interruption*/
	//GPIO configuration
	handlerEncoderClock.pGPIOx	= 										GPIOA;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinNumber = 				PIN_0;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderClock.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiEncoderCK.pGPIOHandler= 									&handlerEncoderClock;
	handlerExtiEncoderCK.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderCK);


//-----DT--------------------------------------------------------
	/*ENCODER DT configuration as external interruption*/
	//GPIO configuration
	handlerEncoderDT.pGPIOx	= 										GPIOA;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinNumber = 				PIN_1;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderDT.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;



//-----SW------------------------------------------------------------
	/*ENCODER SW configuration as external interruption*/
	//GPIO configuration
	handlerEncoderSW.pGPIOx	= 										GPIOA;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinNumber = 				PIN_4;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerEncoderSW.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;
	//ExtiConfiguration
	handlerExtiEncoderSW.pGPIOHandler= 									&handlerEncoderSW;
	handlerExtiEncoderSW.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiEncoderSW);



//-------------------------------------------------LED 7 SEGMENTOS----------------------------------------------------------

//------LEDS-------------------------------
	handlerPunto.pGPIOx = 												GPIOB; 									//El puerto que se esta utilizando es el puerto B
    handlerPunto.GPIO_PinConfig.GPIO_PinNumber =						PIN_9; //EL pin que se esta utilizando es el pin 8
    handlerPunto.GPIO_PinConfig.GPIO_PinMode  =							GPIO_MODE_OUT; //EL pin esta en modo de salida
    handlerPunto.GPIO_PinConfig.GPIO_PinOPType =         				GPIO_OTYPE_PUSHPULL; //PushPull para poder que me de cero y uno
    handlerPunto.GPIO_PinConfig.GPIO_PinPuPdControl =					GPIO_PUPDR_NOTHING; // YA tiene res. No necesita de otra, por lo cual se pone NOTHING
    handlerPunto.GPIO_PinConfig.GPIO_PinSpeed=				            GPIO_OSPEED_FAST; //Configuro la velocida

    //Ahora se carga la configuración
    GPIO_Config(&handlerPunto);

	handlerPinLedA.pGPIOx=                                  			GPIOB;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinNumber=           			PIN_6;// pin para led externo;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinMode=             			GPIO_MODE_OUT;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinOPType=          				GPIO_OTYPE_PUSHPULL;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinPuPdControl=    				GPIO_PUPDR_NOTHING;
	handlerPinLedA.GPIO_PinConfig.GPIO_PinSpeed=            			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedA);


	handlerPinLedB.pGPIOx=                                   			GPIOA;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinNumber=            			PIN_6;// pin para led externo;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinPuPdControl=      			GPIO_PUPDR_NOTHING;
	handlerPinLedB.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;

	GPIO_Config(&handlerPinLedB);

	handlerPinLedC.pGPIOx=                                   			GPIOC;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinNumber=            			PIN_8;// pin para led externo;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedC.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedC);


	handlerPinLedD.pGPIOx=                                   			GPIOC;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinNumber=            			PIN_6;// pin para led externo;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedD.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedD);


	handlerPinLedE.pGPIOx=                                   			GPIOC;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinNumber=            			PIN_5;// pin para led externo;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedE.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedE);


	handlerPinLedF.pGPIOx=                                   			GPIOA;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinNumber=            			PIN_7;// pin para led externo;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedF.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedF);


	handlerPinLedG.pGPIOx=                                  			GPIOC;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinNumber=            			PIN_9;// pin para led externo;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinMode=              			GPIO_MODE_OUT;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinOPType=            			GPIO_OTYPE_PUSHPULL;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinPuPdControl=       			GPIO_PUPDR_NOTHING;
	handlerPinLedG.GPIO_PinConfig.GPIO_PinSpeed=             			GPIO_OSPEED_FAST;


	GPIO_Config(&handlerPinLedG);

//----------------------Transistores


	/*Se define la configuración para el transitor que controla las unidades,
	* este se conectara al pin 9 del puerto b
	*
	*/
	handlerTransitor1.pGPIOx = 											GPIOC; 	//El puerto que se esta utilizando es el puerto B
	handlerTransitor1.GPIO_PinConfig.GPIO_PinNumber = 					PIN_1; //EL pin que se esta utilizando es el pin 9
	handlerTransitor1.GPIO_PinConfig.GPIO_PinMode=						GPIO_MODE_OUT; //EL pin esta en modo de salida
	handlerTransitor1.GPIO_PinConfig.GPIO_PinOPType= 					GPIO_OTYPE_PUSHPULL; //PushPull para poder que me de cero y uno
	handlerTransitor1.GPIO_PinConfig.GPIO_PinPuPdControl=				GPIO_PUPDR_NOTHING; // YA tiene res. No necesita de otra, por lo cual se pone NOTHING
	handlerTransitor1.GPIO_PinConfig.GPIO_PinSpeed=						GPIO_OSPEED_FAST; //Configuro la velocidad


	//Ahora se carga la configuración
	GPIO_Config(&handlerTransitor1);

	/*Se define la configuración para el transitor que controla las decenas,
	* este se conectara al pin 8 del puerto c
	*
	*/
	handlerTransitor2.pGPIOx = 											GPIOB; //El puerto que se esta utilizando es el puerto B
	handlerTransitor2.GPIO_PinConfig.GPIO_PinNumber=          			PIN_4; //EL pin que se esta utilizando es el pin 9
	handlerTransitor2.GPIO_PinConfig.GPIO_PinMode=            			GPIO_MODE_OUT; //EL pin esta en modo de salida
	handlerTransitor2.GPIO_PinConfig.GPIO_PinOPType=          			GPIO_OTYPE_PUSHPULL; //PushPull para poder que me de cero y uno
	handlerTransitor2.GPIO_PinConfig.GPIO_PinPuPdControl=     			GPIO_PUPDR_NOTHING; // YA tiene res. No necesita de otra, por lo cual se pone NOTHING
	handlerTransitor2.GPIO_PinConfig.GPIO_PinSpeed=           			GPIO_OSPEED_FAST; //Configuro la velocidad


	//Ahora se carga la configuración
	GPIO_Config(&handlerTransitor2);
	GPIOxTooglePin(&handlerTransitor2);

//TIMER
	// We configure TIMER 3 as the counter for the stateLed
	handlerTimerSG.ptrTIMx = 										TIM3;
	handlerTimerSG.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerSG.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_1ms;
	handlerTimerSG.TIMx_Config.TIMx_period = 						10;
	handlerTimerSG.TIMx_Config.TIMx_interrupEnable=				BTIMER_DISABLE;
	// We load the configuration
	BasicTimer_Config(&handlerTimerSG);
	// We activate the TIM3
	starTimer(&handlerTimerSG);

}






void callback_extInt4(void){
   	// Lectura del botón para reiniciar a 0 la pantalla
	buttonPressed = GPIO_ReadPin(&handlerEncoderSW);
	if (buttonPressed == RESET) { // Si el botón está presionado

		numeros(0);
		 // Apagamos el LED asociado
	}



}




void callback_extInt0(void){


	uint8_t prevDataState = 0;
	uint8_t currDataState;
	uint8_t currClockState;


	currDataState = GPIO_ReadPin(&handlerEncoderDT);
	currClockState = GPIO_ReadPin(&handlerEncoderClock);

	if(currDataState != prevDataState ){


		if(currClockState == SET){
			count ++;
			if(count > 99){
				count = 0;
			}

		}
		else{

			count --;
			if(count < 0){
				count = 99;
			}
		}

	}

	if (count < 10) {



		    unidades = count;
		    decenas = RESET;


	}





	 else {
		 numeros(0);
	    // Si el contador es mayor o igual a 10, se escriben ambos dígitos


	    decenas = count / 10;
	    unidades = count % 10;


	}

	prevDataState = currDataState;
}


void BasicTimer3_Callback(void){

	uint8_t transistor1;
	transistor1 = GPIO_ReadPin(&handlerTransitor1);

	if (transistor1 == SET){
		numeros(unidades);

	}
	else{
		numeros(decenas);
	}
GPIOxTooglePin(&handlerTransitor1);
GPIOxTooglePin(&handlerTransitor2);

}




//Callback function with External Butom
void callback_extInt8(void){
	buttonPressed = GPIO_ReadPin(&handlerExternalButton);
    if (buttonPressed == RESET) { // Si el botón está presionado

    	numeros(0);
    	 // Apagamos el LED asociado
    }

}



//Callback function with state led

void BasicTimer2_Callback(void){

	/* To create a status LED, we need to link the timer with the GPIO.
	 * We can do this by calling the Toggle function in the Timer Callback.
	 * This generates the interrupt that will toggle the LED state.*/
	GPIOxTooglePin(&handlerStateLed);



}


//funcion para que el sistema espere un tiempo prudente antes de marcar el otro numero
void Delay(uint32_t value){
  for(uint32_t i=0; i<value; i++);
}



//Funcion que contiene todos los numeros

void numeros(int numero) {
switch (numero) {
   case 0:{

	   GPIO_WritePin(&handlerPinLedA, RESET);
       GPIO_WritePin(&handlerPinLedB, RESET);
       GPIO_WritePin(&handlerPinLedC, RESET);
       GPIO_WritePin(&handlerPinLedD, RESET);
       GPIO_WritePin(&handlerPinLedE, RESET);
       GPIO_WritePin(&handlerPinLedF, RESET);
       GPIO_WritePin(&handlerPinLedG, SET);
       GPIO_WritePin(&handlerPunto, SET);
    break;
        }
  case 1: {
      GPIO_WritePin(&handlerPinLedA, SET);
      GPIO_WritePin(&handlerPinLedB, RESET);
      GPIO_WritePin(&handlerPinLedC, RESET);
      GPIO_WritePin(&handlerPinLedD, SET);
      GPIO_WritePin(&handlerPinLedE, SET);
      GPIO_WritePin(&handlerPinLedF, SET);
      GPIO_WritePin(&handlerPinLedG, SET);
      GPIO_WritePin(&handlerPunto, SET);
    break;

       }
  case 2:{
     GPIO_WritePin(&handlerPinLedA, RESET);
     GPIO_WritePin(&handlerPinLedB, RESET);
     GPIO_WritePin(&handlerPinLedC, SET);
     GPIO_WritePin(&handlerPinLedD, RESET);
     GPIO_WritePin(&handlerPinLedE, RESET);
     GPIO_WritePin(&handlerPinLedF, SET);
     GPIO_WritePin(&handlerPinLedG, RESET);
     GPIO_WritePin(&handlerPunto, SET);
    break;
      }
  case 3:{
     GPIO_WritePin(&handlerPinLedA, RESET);
     GPIO_WritePin(&handlerPinLedB, RESET);
     GPIO_WritePin(&handlerPinLedC, RESET);
     GPIO_WritePin(&handlerPinLedD, RESET);
     GPIO_WritePin(&handlerPinLedE, SET);
     GPIO_WritePin(&handlerPinLedF, SET);
     GPIO_WritePin(&handlerPinLedG, RESET);
     GPIO_WritePin(&handlerPunto, SET);
  break;
      }
  case 4:{
     GPIO_WritePin(&handlerPinLedA, SET);
     GPIO_WritePin(&handlerPinLedB, RESET);
     GPIO_WritePin(&handlerPinLedC, RESET);
     GPIO_WritePin(&handlerPinLedD, SET);
     GPIO_WritePin(&handlerPinLedE, SET);
     GPIO_WritePin(&handlerPinLedF, RESET);
     GPIO_WritePin(&handlerPinLedG, RESET);
     GPIO_WritePin(&handlerPunto, SET);
  break;

     }
  case 5:{
     GPIO_WritePin(&handlerPinLedA, RESET);
     GPIO_WritePin(&handlerPinLedB, SET);
     GPIO_WritePin(&handlerPinLedC, RESET);
     GPIO_WritePin(&handlerPinLedD, RESET);
     GPIO_WritePin(&handlerPinLedE, SET);
     GPIO_WritePin(&handlerPinLedF, RESET);
     GPIO_WritePin(&handlerPinLedG, RESET);
     GPIO_WritePin(&handlerPunto, SET);
  break;
     }
  case 6:{
     GPIO_WritePin(&handlerPinLedA, RESET);
     GPIO_WritePin(&handlerPinLedB, SET);
     GPIO_WritePin(&handlerPinLedC, RESET);
     GPIO_WritePin(&handlerPinLedD, RESET);
     GPIO_WritePin(&handlerPinLedE, RESET);
     GPIO_WritePin(&handlerPinLedF, RESET);
     GPIO_WritePin(&handlerPinLedG, RESET);
     GPIO_WritePin(&handlerPunto, SET);
  break;

     }
   case 7:{
      GPIO_WritePin(&handlerPinLedA, RESET);
      GPIO_WritePin(&handlerPinLedB, RESET);
      GPIO_WritePin(&handlerPinLedC, RESET);
      GPIO_WritePin(&handlerPinLedD, SET);
      GPIO_WritePin(&handlerPinLedE, SET);
      GPIO_WritePin(&handlerPinLedF, SET);
      GPIO_WritePin(&handlerPinLedG, SET);
      GPIO_WritePin(&handlerPunto, SET);
    break;

     }
   case 8:{
      GPIO_WritePin(&handlerPinLedA, RESET);
      GPIO_WritePin(&handlerPinLedB, RESET);
      GPIO_WritePin(&handlerPinLedC, RESET);
      GPIO_WritePin(&handlerPinLedD, RESET);
      GPIO_WritePin(&handlerPinLedE, RESET);
      GPIO_WritePin(&handlerPinLedF, RESET);
      GPIO_WritePin(&handlerPinLedG, RESET);
      GPIO_WritePin(&handlerPunto, SET);
   break;
     }
   case 9:{
      GPIO_WritePin(&handlerPinLedA, RESET);
      GPIO_WritePin(&handlerPinLedB, RESET);
      GPIO_WritePin(&handlerPinLedC, RESET);
      GPIO_WritePin(&handlerPinLedD, RESET);
      GPIO_WritePin(&handlerPinLedE, SET);
      GPIO_WritePin(&handlerPinLedF, RESET);
      GPIO_WritePin(&handlerPinLedG, RESET);
      GPIO_WritePin(&handlerPunto, SET);
    break;
     }
   case 'O':{
      GPIO_WritePin(&handlerPinLedA, SET);
      GPIO_WritePin(&handlerPinLedB, SET);
      GPIO_WritePin(&handlerPinLedC, SET);
      GPIO_WritePin(&handlerPinLedD, SET);
      GPIO_WritePin(&handlerPinLedE, SET);
      GPIO_WritePin(&handlerPinLedF, SET);
      GPIO_WritePin(&handlerPinLedG, SET);
      GPIO_WritePin(&handlerPunto, SET);
     }
   case 'E':{
      GPIO_WritePin(&handlerPinLedA, RESET);
      GPIO_WritePin(&handlerPinLedB, SET);
      GPIO_WritePin(&handlerPinLedC, SET);
      GPIO_WritePin(&handlerPinLedD, RESET);
      GPIO_WritePin(&handlerPinLedE, RESET);
      GPIO_WritePin(&handlerPinLedF, RESET);
      GPIO_WritePin(&handlerPinLedG, RESET);
      GPIO_WritePin(&handlerPunto, RESET);
      }

     }

	}


