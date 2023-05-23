/*
 * PwmDriver.c
 *
 *  Created on: 21/05/2023
 *      Author: Marianela Arcila Sanchez	maarcilasa
 *  In this program, we write about the control of the PWM peripheral.
 *  This control aims to be as generic as possible.
 */

#include "PwmDriver.h"

/**/
void pwm_Config(PWM_Handler_t *ptrPwmHandler){

	/* 1. Enable the clock signal of the required peripheral */
	if(ptrPwmHandler->ptrTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(ptrPwmHandler->ptrTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else{
		__NOP();
	}

	/* 1. Load the desired frequency */
	setFrequency(ptrPwmHandler);

	/* 2. Load the duty cycle value */
	setDutyCycle(ptrPwmHandler);

	/* 2a. We are in UP_Mode, the limit is loaded in ARR and starts from 0 */
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_DIR;

	/* 3. Configure the CCxS bits of the TIMy_CCMR1 register to set the output mode
	 * (there is a CCxS set for each channel)
	 * 4. Additionally, in the same "case" we can configure the PWM mode, its polarity...
	 *
	 * 5. We also activate the preload bit, so that every time an update event occurs,
	 * the value loaded in CCRx will be reloaded in the PWM shadow register */
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		// Select channel as output
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC1S;

		// Configure the channel as PWM
		ptrPwmHandler->ptrTIMx->CCMR1 |= (0x06 << TIM_CCMR1_OC1M_Pos);

		// Activate the preload functionality
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC1PE;

		break;
	}

	case PWM_CHANNEL_2:{
		// Select channel as output
		ptrPwmHandler->ptrTIMx->CCMR1 &= ~TIM_CCMR1_CC2S;
		// Configure the channel as PWM
		ptrPwmHandler->ptrTIMx->CCMR1 |= (0x06 << TIM_CCMR1_OC2M_Pos);

		// Activate the preload functionality
		ptrPwmHandler->ptrTIMx->CCMR1 |= TIM_CCMR1_OC2PE;
		break;
	}
	case PWM_CHANNEL_3:{
		// Select channel as output
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_CC3S;

		// Configure the channel as PWM
		ptrPwmHandler->ptrTIMx->CCMR2 |= (0x06 << TIM_CCMR2_OC3M_Pos);

		// Activate the preload functionality
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC3PE;
		break;

	}
	case PWM_CHANNEL_4:{
		// Select channel as output
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_CC4S;

		 // Configure the channel as PWM
		ptrPwmHandler->ptrTIMx->CCMR2 |= (0x06 << TIM_CCMR2_OC4M_Pos);

		// Activate the preload functionality
		ptrPwmHandler->ptrTIMx->CCMR2 &= ~TIM_CCMR2_OC4PE;
		break;
	}


	default:{
		break;
	}

	/* 6. Enable the selected output */
	enableOutput(ptrPwmHandler);

	}// end of switch-case
}

/* Function to activate the Timer and start the PWM module */
void startPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	ptrPwmHandler->ptrTIMx->CR1 |= TIM_CR1_CEN;
}

/* Function to deactivate the Timer and stop the PWM module */
void stopPwmSignal(PWM_Handler_t *ptrPwmHandler) {
	ptrPwmHandler->ptrTIMx->CR1 &= ~TIM_CR1_CEN;
	ptrPwmHandler->ptrTIMx->DIER &= ~TIM_DIER_UIE;
}

/* Function to enable each of the channels in TimerX */
void enableOutput(PWM_Handler_t *ptrPwmHandler) {
	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {
		// Enable channel 1 output
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC1E;
		break;
	}
	case PWM_CHANNEL_2: {
		// Enable channel 2 output
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC2E;
		break;
	}
	case PWM_CHANNEL_3: {
		// Enable channel 3 output
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC3E;
		break;
	}
	case PWM_CHANNEL_4: {
		// Enable channel 4 output
		ptrPwmHandler->ptrTIMx->CCER |= TIM_CCER_CC4E;
		break;
	}
	default: {
		break;
	}
	}
}

/* Function to disable each of the channels in TimerX */
void disableOutput(PWM_Handler_t *ptrPwmHandler) {
	switch (ptrPwmHandler->config.channel) {
	case PWM_CHANNEL_1: {
		// Disable channel 1 output
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC1E;
		break;
	}
	case PWM_CHANNEL_2: {
		// Disable channel 2 output
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC2E;
		break;
	}
	case PWM_CHANNEL_3: {
		// Disable channel 3 output
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC3E;
		break;
	}
	case PWM_CHANNEL_4: {
		// Disable channel 4 output
		ptrPwmHandler->ptrTIMx->CCER &= ~TIM_CCER_CC4E;
		break;
	}
	default: {
		break;
	}
	}
}
/*
 * The frequency is defined by the combination of the prescaler (PSC)
 * and the limit value reached by the Timer (ARR). These two parameters
 * determine the frequency.
 */
void setFrequency(PWM_Handler_t *ptrPwmHandler){

	// Load the value of the prescaler, which defines the speed (in ns) at which
	// the Timer increments
	ptrPwmHandler->ptrTIMx->PSC = ptrPwmHandler->config.prescaler - 1;

	// Load the value of ARR, which is the limit of Timer increments
	// before performing an update and reload.
	ptrPwmHandler->ptrTIMx->ARR = ptrPwmHandler->config.period - 1;
}


/* Function to update the frequency, works in conjunction with setFrequency */
void updateFrequency(PWM_Handler_t *ptrPwmHandler, uint16_t newFreq){
	// Update the register that handles the period
	newFreq =
	ptrPwmHandler->ptrTIMx->ARR = newFreq;

	// Call the function that changes the frequency
	setFrequency(ptrPwmHandler);
}

/* The duty value must be given in % values, between 0% and 100% */
void setDuttyCycle(PWM_Handler_t *ptrPwmHandler){

	// Select the channel to configure its duty cycle
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.dutyCycle;

		break;
	}
	case PWM_CHANNEL_2:{
		ptrPwmHandler->ptrTIMx->CCR1 = ptrPwmHandler->config.dutyCycle;

		break;
	}
	case PWM_CHANNEL_3:{
		ptrPwmHandler->ptrTIMx->CCR2 = ptrPwmHandler->config.dutyCycle;

		break;
	}
	case PWM_CHANNEL_4:{
		ptrPwmHandler->ptrTIMx->CCR2 = ptrPwmHandler->config.dutyCycle;

		break;
	}

	default:{
		break;
	}

	}// end of switch-case

}

/* Function to update the duty cycle, works in conjunction with setDuttyCycle */
void updateDuttyCycle(PWM_Handler_t *ptrPwmHandler, uint16_t newDutty){
	// Update the register that handles the duty cycle
	  ptrPwmHandler->config.period = newDutty ;
	// Call the function that changes the duty cycle and load the new value
	setDuttyCycle(  ptrPwmHandler);
}


void enableEvent(PWM_Handler_t *ptrPwmHandler){
// Select the channel to configure its duty cycle
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		ptrPwmHandler->ptrTIMx->EGR |= TIM_EGR_CC1G;

		break;
	}
	case PWM_CHANNEL_2:{
		ptrPwmHandler->ptrTIMx->EGR |= TIM_EGR_CC2G;

		break;
	}
	case PWM_CHANNEL_3:{
		ptrPwmHandler->ptrTIMx->EGR |= TIM_EGR_CC3G;

		break;
	}
	case PWM_CHANNEL_4:{
		ptrPwmHandler->ptrTIMx->EGR |= TIM_EGR_CC4G;

		break;
	}

	default:{
		break;
	}

	}// end of switch-case

}

void disableEvent(PWM_Handler_t *ptrPwmHandler){
// Select the channel to configure its duty cycle
	switch(ptrPwmHandler->config.channel){
	case PWM_CHANNEL_1:{
		ptrPwmHandler->ptrTIMx->EGR &= ~TIM_EGR_CC1G;

		break;
	}
	case PWM_CHANNEL_2:{
		ptrPwmHandler->ptrTIMx->EGR &= ~TIM_EGR_CC2G;

		break;
	}
	case PWM_CHANNEL_3:{
		ptrPwmHandler->ptrTIMx->EGR &= ~TIM_EGR_CC3G;

		break;
	}
	case PWM_CHANNEL_4:{
		ptrPwmHandler->ptrTIMx->EGR &= ~TIM_EGR_CC4G;

		break;
	}

	default:{
		break;
	}

	}// end of switch-case

}
