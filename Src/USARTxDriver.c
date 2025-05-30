/*
 * USARTxDriver.c
 *
 *  Created on: 28/05/2023
 *      Author: marianela
 */


#include <stm32f4xx.h>
#include "USARTxDriver.h"
uint8_t auxRxData = 0;
/* Variable que guarda la referencia del periférico que se esta utilizando*/
USART_TypeDef *ptrUsartUsed;

void USART_Config(USART_Handler_t *ptrUsartHandler){


	//Guardaos la relferencia del USART que se esta usando
	ptrUsartUsed = ptrUsartHandler ->ptrUSARTx;
	//Se desactivan las inerrupciones del sistema
	__disable_irq();
	/* 1. Activamos la señal de reloj que viene desde el BUS al que pertenece el periferico */
		/* Lo debemos hacer para cada uno de las posibles opciones que tengamos (USART1, USART2, USART6) */
	    /* 1.1 Configuramos el USART1 */
	if(ptrUsartHandler->ptrUSARTx == USART1){
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
	}
	else if (ptrUsartHandler -> ptrUSARTx == USART2){
		//PUERTO SERIAL SIN SALIDA
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;

	}
	else{
		RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;
	}

	/* 2. Configuramos el tamaño del dato, la paridad y los bit de parada */
	/* En el CR1 estan parity (PCE y PS) y tamaño del dato (M) */
	/* Mientras que en CR2 estan los stopbit (STOP)*/
	/* Configuracion del Baudrate (registro BRR) */
	/* Configuramos el modo: only TX, only RX, o RXTX */
	/* Por ultimo activamos el modulo USART cuando todo esta correctamente configurado */

	// 2.1 Comienzo por limpiar los registros, para cargar la configuración desde cero
	ptrUsartHandler->ptrUSARTx->CR1 = 0;
	ptrUsartHandler->ptrUSARTx->CR2 = 0;

	// 2.2 Configuracion del Parity:
	// Verificamos si el parity esta activado o no
    // Tenga cuidado, el parity hace parte del tamaño de los datos...
	if(ptrUsartHandler->USART_Config.USART_parity != USART_PARITY_NONE){

		// Verificamos si se ha seleccionado ODD or EVEN
		if(ptrUsartHandler->USART_Config.USART_parity == USART_PARITY_EVEN){
			/* Cargamos la configuracion en el bit 9 para el registro
			   CR1, al ser configuracion EVEN se coloca un 0 en la posicion
		    */
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PCE;
			ptrUsartHandler ->ptrUSARTx -> CR1 &= ~USART_CR1_PS;

		}else{
			/* Cuando la configuracicon es ODD en este caso se coloca
			 * un 1 en el bit 9 para el registro CR1 */
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PCE;
			ptrUsartHandler ->ptrUSARTx -> CR1 |= USART_CR1_PS;
		}
	}else{
		// Si llegamos aca, es porque no deseamos tener el parity-check
		// Se valida con el bit 10 PCE del registro CR1

			ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_PCE;

	}

	// 2.3 Configuramos el tamaño del dato
	ptrUsartHandler -> ptrUSARTx -> CR1 |= (ptrUsartHandler->USART_Config.USART_datasize << USART_CR1_M_Pos);

	// 2.4 Configuramos los stop bits (SFR USART_CR2)
	switch(ptrUsartHandler->USART_Config.USART_stopbits){
	case USART_STOPBIT_1: {
		// Debemos cargar el valor 0b00 en los dos bits de STOP
		ptrUsartHandler ->ptrUSARTx -> CR2 &= ~USART_CR2_STOP_0;
		ptrUsartHandler ->ptrUSARTx -> CR2 &= ~USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_0_5: {
		// Debemoscargar el valor 0b01 en los dos bits de STOP
		ptrUsartHandler -> ptrUSARTx -> CR2 |= USART_CR2_STOP_0;
		ptrUsartHandler -> ptrUSARTx -> CR2 &= ~USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_2: {
		// Debemoscargar el valor 0b10 en los dos bits de STOP
		ptrUsartHandler -> ptrUSARTx -> CR2 &= ~USART_CR2_STOP_0;
		ptrUsartHandler -> ptrUSARTx -> CR2 |= USART_CR2_STOP_1;
		break;
	}
	case USART_STOPBIT_1_5: {
		// Debemoscargar el valor 0b11 en los dos bits de STOP
		ptrUsartHandler -> ptrUSARTx -> CR2 |= USART_CR2_STOP_0;
		ptrUsartHandler -> ptrUSARTx -> CR2 |= USART_CR2_STOP_1;
		break;
	}
	default: {
		// En el casopor defecto seleccionamos 1 bit de parada
		ptrUsartHandler ->ptrUSARTx -> CR2 &= ~USART_CR2_STOP_0;
		ptrUsartHandler ->ptrUSARTx -> CR2 &= ~USART_CR2_STOP_1;
		break;
	}
	}

	// 2.5 Configuracion del Baudrate (SFR USART_BRR)
	// Ver tabla de valores (Tabla 73), Frec = 16MHz, overr = 0;
	if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_9600){
		// El valor a cargar es 104.1875 -> Mantiza = 104,fraction = 0.1875
		// Mantiza = 104 = 0x68, fraction = 16 * 0.1875 = 3
		// Valor a cargar 0x0683
		// Configurando el Baudrate generator para una velocidad de 9600bps
		ptrUsartHandler->ptrUSARTx->BRR = 0x0683;
	}

	else if (ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_19200) {
		// El valor a cargar es 52.0625 -> Mantiza = 52,fraction = 0.0625
		// Mantiza = 52 = 0x34, fraction = 16 * 0.1875 = 1
		// Valor a cargar 0x0341
		ptrUsartHandler -> ptrUSARTx ->BRR = 0x0341;
	}

	else if(ptrUsartHandler->USART_Config.USART_baudrate == USART_BAUDRATE_115200){
		/*El valor a cargar es 8.6875 -> Mantiza = 8, fraccion = 0.6875
		 * Mantiza = 8 = 0x08; Fraccion = 16* 0.6875 = 11
		 * Valor a cargar 0x08B
		*/
		ptrUsartHandler -> ptrUSARTx -> BRR = 0x08B;
	}


	// 2.6 Configuramor el modo: TX only, RX only, RXTX, disable
	// TE y RE son dos bits seguidos siendo menos significativo RE por lo tanto usaremos la posición RE
	switch(ptrUsartHandler -> USART_Config.USART_mode)
	{
	case USART_MODE_TX:
		// Se activa solo la interrupcion de TXEIE
		ptrUsartHandler -> ptrUSARTx-> CR1 |= (USART_CR1_TE);
		break;
	case USART_MODE_RX:
		// Se activa solo la interrupcion de RXNEIE
		ptrUsartHandler -> ptrUSARTx -> CR1 |= (USART_CR1_RE);
		break;
	case USART_MODE_RXTX:
		// Se activan ambas interrupciones
		ptrUsartHandler -> ptrUSARTx-> CR1 |= (USART_CR1_TE);
		ptrUsartHandler -> ptrUSARTx-> CR1 |= (USART_CR1_RE);
		break;
	case USART_MODE_DISABLE:
		// y se desactivan ambas
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_TE;
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_RE;
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_UE;
		break;
	default:
		// por defecto se desactivan ambos
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_TE;
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_RE;
		ptrUsartHandler -> ptrUSARTx -> CR1 &= ~USART_CR1_UE;
		break;
	}

	/* 2.8 Verificamos la configuracion de las interrupciones
	 * a)Interrupcion por recepcion	 */
	if(ptrUsartHandler -> USART_Config.USART_enableIntRx== USART_RX_INTERRUP_ENABLE){
		/*COmo esta activada, debemos configurar la interrupcion por recepcion
		 * Debemos activar la interrupcion RX en la configuracion del USART */
		ptrUsartHandler -> ptrUSARTx -> CR1 |= USART_CR1_RXNEIE;

		/*Se matricula la interrupcion en la NVIC
		 * Se hace por cada una de las opciones que tenemos( USART 1, USART 2, USART 6)
		 */
		if(ptrUsartHandler -> ptrUSARTx == USART1){
			__NVIC_EnableIRQ(USART1_IRQn);
		}
		else if(ptrUsartHandler -> ptrUSARTx == USART2){
			__NVIC_EnableIRQ(USART2_IRQn);
		}
		else if(ptrUsartHandler -> ptrUSARTx == USART6){
			__NVIC_EnableIRQ(USART6_IRQn);
		}
		else{

		}

	}
	// 2.8 Activamos el modulo serial.
	if(ptrUsartHandler->USART_Config.USART_mode != USART_MODE_DISABLE){
		ptrUsartHandler -> ptrUSARTx -> CR1 |= (USART_CR1_UE);
	}

	/*Se activan nuevamente las interrupciones del sistema */
	__enable_irq();


}

/* funcion para escribir un solo char */
int writeChar(USART_Handler_t *ptrUsartHandler, int dataToSend ){
	while( !(ptrUsartHandler->ptrUSARTx->SR & USART_SR_TXE)){
		__NOP();
	}

	ptrUsartHandler->ptrUSARTx->DR = dataToSend;


	return dataToSend;
}

void writeMsg(USART_Handler_t *ptrUsartHandler, char *msgToSed ){

	while(*msgToSed != '\0'){

		writeChar(ptrUsartHandler, *msgToSed);
		msgToSed ++;
	}

}


uint8_t getRxData(void){
	return auxRxData;

}

/*Handler de la interrupcion del USART2
 * aca deben estar todas las interrupciones asociadas TX, RX, PE
 */
void USART2_IRQHandler(void){
	if(USART2 ->SR & USART_SR_RXNE ){
		auxRxData = (uint8_t) USART2 -> DR;
		usart2Rx_Callback();
	}

}

__attribute__((weak)) void usart2Rx_Callback(void){
	  	__NOP();
}

/*Handler de la interrupcion del USART1
 * aca deben estar todas las interrupciones asociadas TX, RX, PE
 */
void USART1_IRQHandler(void){
	if(USART1 ->SR & USART_SR_RXNE ){
		auxRxData = (uint8_t) USART1 -> DR;
		usart1Rx_Callback();
	}

}

__attribute__((weak)) void usart1Rx_Callback(void){
	  	__NOP();
}



/*Handler de la interrupcion del USART6
 * aca deben estar todas las interrupciones asociadas TX, RX, PE
 */
void USART6_IRQHandler(void){
	if(USART6->SR & USART_SR_RXNE ){
		auxRxData = (uint8_t) USART6 -> DR;
		usart6Rx_Callback();
	}

}

__attribute__((weak)) void usart6Rx_Callback(void){
	  	__NOP();
}

/*
 * No se encuentra la solución a la tarea5 ni tampoco el driver que controle el cambio al PLL o la configuración del USART
 * Calificación = 0.0
 */
