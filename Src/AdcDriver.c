/*
 * AdcDriver.c
 *
 *  Created on: 11/06/2023
 *      Author: marianela
 */


#include "AdcDriver.h"
#include "GPIOxDriver.h"

GPIO_Handler_t handlerAdcPin = {0};
uint16_t	adcRawData = 0;

void adc_Config(ADC_Config_t *adcConfig){
	/* 1. Configuramos el PinX para que cumpla la función de canal análogo deseado. */
	configAnalogPin(adcConfig->channel);

	/* 2. Activamos la señal de reloj para el periférico ADC1 (bus APB2)*/
	// Escriba su código acá
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Limpiamos los registros antes de comenzar a configurar
	ADC1->CR1 = 0;
	ADC1->CR2 = 0;

	/* Comenzamos la configuración del ADC1 */
	/* 3. Resolución del ADC */
	switch(adcConfig->resolution){
	case ADC_RESOLUTION_12_BIT:
	{

		ADC1->CR1 &= ~ADC_CR1_RES_0;
		ADC1->CR1 &= ~ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_10_BIT:
	{

		ADC1->CR1 |= ADC_CR1_RES_0;
		ADC1->CR1 &= ~ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_8_BIT:
	{

		ADC1->CR1 &= ~ADC_CR1_RES_0;
		ADC1->CR1 |= ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_6_BIT:
	{
		ADC1->CR1 |= ADC_CR1_RES_0;
		ADC1->CR1 |= ADC_CR1_RES_1;
		break;
	}

	default:
	{
		break;
	}
	}

	/* 4. Configuramos el modo Scan como desactivado */

	ADC1->CR1 &= ~ADC_CR1_SCAN;

	/* 5. Configuramos la alineación de los datos (derecha o izquierda) */
	if(adcConfig->dataAlignment == ADC_ALIGNMENT_RIGHT){
		// Alineación a la derecha (esta es la forma "natural")

		ADC1->CR2 &= ~ADC_CR2_ALIGN;
	}
	else{

		// Alineación a la izquierda (para algunos cálculos matemáticos)
		// Escriba su código acá
		ADC1->CR2 &= ~ADC_CR2_ALIGN;
	}

	/* 6. Desactivamos el "continuos mode" */
	// Escriba su código acá
	ADC1->CR2 &= ~ADC_CR2_CONT;

	/* 7. Acá se debería configurar el sampling...*/
	if(adcConfig->channel < ADC_CHANNEL_9){
		// Escriba su código acá
		ADC1->SMPR2 |= adcConfig->samplingPeriod << (3*adcConfig->channel);
	}
	else{
		// Escriba su código acá
		ADC1->SMPR1 |= adcConfig->samplingPeriod << (3*adcConfig->channel);
	}

	/* 8. Configuramos la secuencia y cuantos elementos hay en la secuencia */
	// Al hacerlo todo 0, estamos seleccionando solo 1 elemento en el conteo de la secuencia
	ADC1->SQR1 = 0;

	// Asignamos el canal de la conversión a la primera posición en la secuencia
	ADC1->SQR3 |= (adcConfig->channel << 0);

	/* 9. Configuramos el preescaler del ADC en 2:1 (el mas rápido que se puede tener */
	ADC->CCR |= ADC_CCR_ADCPRE_0;

	/* 10. Desactivamos las interrupciones globales */
	// Escriba su código acá
	__disable_irq();

	/* 11. Activamos la interrupción debida a la finalización de una conversión EOC (CR1)*/
	// Escriba su código acá
	ADC1->CR1 |= ADC_CR1_EOCIE;

	/* 11a. Matriculamos la interrupción en el NVIC*/
	// Escriba su código acá
	__NVIC_EnableIRQ(ADC_IRQn);

	/* 11b. Configuramos la prioridad para la interrupción ADC */
	// Escriba su código acá
	__NVIC_SetPriority(ADC_IRQn, 1);
	/* 12. Activamos el modulo ADC */
	// Escriba su código acá
	ADC1->CR2 |= ADC_CR2_ADON;

	/* 13. Activamos las interrupciones globales */
	// Escriba su código acá
	__enable_irq();
}

/*
 * Esta función lanza la conversion ADC y si la configuración es la correcta, solo se hace
 * una conversion ADC.
 * Al terminar la conversion, el sistema lanza una interrupción y el dato es leido en la
 * función callback, utilizando la funciona getADC().
 *
 * */
void startSingleADC(void){
	/* Desactivamos el modo continuo de ADC */
	// Escriba su código acá
	ADC1->CR2 &= ~ADC_CR2_CONT;

	/* Limpiamos el bit del overrun (CR1) */
	// Escriba su código acá
	ADC1->CR1 &= ~ADC_CR1_OVRIE;

	/* Iniciamos un ciclo de conversión ADC (CR2)*/
	// Escriba su código acá
	ADC1->CR2 |= ADC_CR2_SWSTART;

}

/*
 * Esta función habilita la conversion ADC de forma continua.
 * Una vez ejecutada esta función, el sistema lanza una nueva conversion ADC cada vez que
 * termina, sin necesidad de utilizar para cada conversion el bit SWSTART del registro CR2.
 * Solo es necesario activar una sola vez dicho bit y las conversiones posteriores se lanzan
 * automaticamente.
 * */
void startContinousADC(void){

	/* Activamos el modo continuo de ADC */
	// Escriba su código acá
	ADC1->CR2 |= ADC_CR2_CONT;

	/* Iniciamos un ciclo de conversión ADC */
	// Escriba su código acá
	ADC1->CR2 |= ADC_CR2_SWSTART;

}

/*
 * Función que retorna el ultimo dato adquirido por la ADC
 * La idea es que esta función es llamada desde la función callback, de forma que
 * siempre se obtiene el valor mas actual de la conversión ADC.
 * */
uint16_t getADC(void){
	// Esta variable es actualizada en la ISR de la conversión, cada vez que se obtiene
	// un nuevo valor.
	return adcRawData;
}

/*
 * Esta es la ISR de la interrupción por conversión ADC
 */
void ADC_IRQHandler(void){
	// Evaluamos que se dio la interrupción por conversión ADC
	if(ADC1->SR & ADC_SR_EOC){
		// Leemos el resultado de la conversión ADC y lo cargamos en una variale auxiliar
		// la cual es utilizada en la función getADC()
		// Escriba su código acá
		adcRawData = ADC1->DR;

		// Hacemos el llamado a la función que se ejecutará en el main
		adcComplete_Callback();
	}

}

/* Función debil, que debe ser sobreescrita en el main. */
__attribute__ ((weak)) void adcComplete_Callback(void){
	__NOP();
}

/*
 * Con esta función configuramos que pin deseamos que funcione como canal ADC
 * Esta funcion trabaja con el GPIOxDriver, por lo cual requiere que se incluya
 * dicho driver.
 */
void configAnalogPin(uint8_t adcChannel) {

	// Con este switch seleccionamos el canal y lo configuramos como análogo.
	switch (adcChannel) {

	case ADC_CHANNEL_0: {
		// Es el pin PA0
		handlerAdcPin.pGPIOx 						= GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
		// Nota: Para el ejercicio inicial solo se necesita este canal, los demas
		// se necesitan para trabajos posteriores.
		break;
	}


	case ADC_CHANNEL_1: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
		break;
	}

	case ADC_CHANNEL_2: {
		// Buscar y configurar adecuadamente
        handlerAdcPin.pGPIOx = GPIOA;
        handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
		break;
	}

	case ADC_CHANNEL_3: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
		break;
	}

	case ADC_CHANNEL_4: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
		break;
	}

	case ADC_CHANNEL_5: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
		break;
	}
	case ADC_CHANNEL_6: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_6;
		break;
	}
	case ADC_CHANNEL_7: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOA;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_7;
		break;
	}
	case ADC_CHANNEL_8: {
		//Es el pin PB0
		handlerAdcPin.pGPIOx 						= GPIOB;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
		break;
	}
	case ADC_CHANNEL_9: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOB;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
		break;
	}
	case ADC_CHANNEL_10: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_0;
		break;
	}
	case ADC_CHANNEL_11: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_1;
		break;
	}
	case ADC_CHANNEL_12: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_2;
		break;
	}
	case ADC_CHANNEL_13: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_3;
		break;
	}
	case ADC_CHANNEL_14: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_4;
		break;
	}
	case ADC_CHANNEL_15: {
		// Buscar y configurar adecuadamente
		handlerAdcPin.pGPIOx = GPIOC;
		handlerAdcPin.GPIO_PinConfig.GPIO_PinNumber = PIN_5;
		break;
	}
	default: {
		break;
	}

	}
	// Despues de configurar el canal adecuadamente, se define este pin como Analogo y se
	// carga la configuración con el driver del GPIOx
	handlerAdcPin.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ANALOG;
	GPIO_Config(&handlerAdcPin);
}

/* configuramos otros canales */

void multiChannelConfig (ADC_Config_t *adcConfig, uint8_t numConversiones){
	/* 1. Configuramos el PinX para que cumpla la función de canal análogo deseado. */

	for (uint8_t i = 0; i < numConversiones; i++){
		configAnalogPin (adcConfig ->channels[i]);

	}

	/* 2. Activamos la señal de reloj para el periférico ADC1 (bus APB2)*/
	// Escriba su código acá
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	// Limpiamos los registros antes de comenzar a configurar
	ADC1->CR1 = 0;
	ADC1->CR2 = 0;

	/* Comenzamos la configuración del ADC1 */
	/* 3. Resolución del ADC */
	switch(adcConfig->resolution){
	case ADC_RESOLUTION_12_BIT:
	{

		ADC1->CR1 &= ~ADC_CR1_RES_0;
		ADC1->CR1 &= ~ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_10_BIT:
	{

		ADC1->CR1 |= ADC_CR1_RES_0;
		ADC1->CR1 &= ~ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_8_BIT:
	{

		ADC1->CR1 &= ~ADC_CR1_RES_0;
		ADC1->CR1 |= ADC_CR1_RES_1;
		break;
	}

	case ADC_RESOLUTION_6_BIT:
	{
		ADC1->CR1 |= ADC_CR1_RES_0;
		ADC1->CR1 |= ADC_CR1_RES_1;
		break;
	}

	default:
	{
		break;
	}
	}

	/* 4. Configuramos el modo Scan como activado para indicarle al multiplexor la secuencia*/
	    ADC1->CR1 |= ADC_CR1_SCAN;

	/* 4.1 Lanza la interrupcion despues de cada conversion */
	    ADC1->CR2 |= ADC_CR2_EOCS;

	/* 5. Configuramos la alineación de los datos (derecha o izquierda) */
	if(adcConfig->dataAlignment == ADC_ALIGNMENT_RIGHT){
		// Alineación a la derecha (esta es la forma "natural")
		// Escriba su código acá
		ADC1->CR2 &= ~ADC_CR2_ALIGN;
	}
	else{

		// Alineación a la izquierda (para algunos cálculos matemáticos)
		// Escriba su código acá
		ADC1->CR2 |= ADC_CR2_ALIGN;
	}

	/* 6. Desactivamos el "continuos mode" */
	// Escriba su código acá
	ADC1->CR2 &= ~ADC_CR2_CONT;

	/* 7. Acá se debería configurar el sampling...*/

	for (uint8_t i = 0; i < numConversiones; i++){
		if(adcConfig->channel < ADC_CHANNEL_10){
			// Escriba su código acá
			ADC1->SMPR2 |= adcConfig->samplingPeriod << (3*adcConfig->channels[i]);
	}
	else{
		// Escriba su código acá
		ADC1->SMPR1 |= adcConfig->samplingPeriod << (3*adcConfig->channels[i] - 10);
	}

	}


	/* 8. Configuramos la secuencia y cuantos elementos hay en la secuencia */
	// Al hacerlo todo 0, estamos seleccionando solo 1 elemento en el conteo de la secuencia
	ADC1->SQR1 |=  (numConversiones - 1) << ADC_SQR1_L_Pos;

	for (uint8_t i = 0; i < numConversiones; i++){

		if (adcConfig ->channels[i] <= 6){
			ADC1->SQR3 |= (adcConfig ->channels[i] << (i) * 5);
		}

		if (adcConfig->channels[i] > 6 && adcConfig->channels[i] <= 12){
			ADC1->SQR2 |= (adcConfig ->channels[i] << (i - 7) * 5);
		}

		if (adcConfig->channels[i] > 12){
			ADC1->SQR1 |= (adcConfig ->channels[i] << (i - 13) * 5);

		}

	}


	/* 9. Configuramos el preescaler del ADC en 2:1 (el mas rápido que se puede tener */
	ADC->CCR |= ADC_CCR_ADCPRE_0;

	/* 10. Desactivamos las interrupciones globales */
	// Escriba su código acá
	__disable_irq();

	/* 11. Activamos la interrupción debida a la finalización de una conversión EOC (CR1)*/
	// Escriba su código acá
	ADC1->CR1 |= ADC_CR1_EOCIE;

	/* 11a. Matriculamos la interrupción en el NVIC*/
	// Escriba su código acá
	__NVIC_EnableIRQ(ADC_IRQn);

	/* 11b. Configuramos la prioridad para la interrupción ADC */
	// Escriba su código acá
	__NVIC_SetPriority(ADC_IRQn, 4);

	/* 12. Activamos el modulo ADC */
	// Escriba su código acá
	ADC1->CR2 |= ADC_CR2_ADON;

	/* 13. Activamos las interrupciones globales */
	// Escriba su código acá
	__enable_irq();



}

void externalTimerEvent(ADC_Config_t *adcConfig){

	// 1) External trigger enable for regular channels
	switch (adcConfig -> dataTrigger) {

		case 0:
			//Trigger detection disabled
			ADC1 ->CR2 &= ~(0b11 << ADC_CR2_EXTEN_Pos);
			break;

		case 1:
			//clean register
			ADC1 ->CR2 &= ~(0b11 << ADC_CR2_EXTEN_Pos);
			//Trigger detection on the rising edge
			ADC1 ->CR2 |= (0b01 << ADC_CR2_EXTEN_Pos);
			break;

		case 2:
			//clean register
			ADC1 ->CR2 &= ~(0b11 << ADC_CR2_EXTEN_Pos);
			//Trigger detection on the falling edge
			ADC1 ->CR2 |= (0b10 << ADC_CR2_EXTEN_Pos);
			break;

		case 3:
			//clean register
			ADC1 ->CR2 &= ~(0b11 << ADC_CR2_EXTEN_Pos);
			//Trigger detection on both the rising and falling edges
			ADC1 ->CR2 |= (0b11 << ADC_CR2_EXTEN_Pos);
			break;

	}

	//2) External event select for regular group, we select the Timer 3 CC1 event
	ADC1 -> CR2 &= ~(0b1111  << ADC_CR2_EXTSEL_Pos );
	ADC1 -> CR2 |= (0b0111  << ADC_CR2_EXTSEL_Pos );


}
