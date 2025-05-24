/*
 * PwmDriver.h
 *
 *  Created on: 21/05/2023
 *      Author: Marianela Arcila Sanchez		maarcilasa
 *  This program is responsible for defining the control of the PWM peripheral.
 *  (control funtions)
 */

#ifndef PWMDRIVER_H_
#define PWMDRIVER_H_

#include "stm32f4xx.h"

#define PWM_CHANNEL_1	0
#define PWM_CHANNEL_2	1
#define PWM_CHANNEL_3	2
#define PWM_CHANNEL_4	3

#define PWM_DUTTY_0_PERCENT		0
#define PWM_DUTTY_100_PERCENT	100

/**/
typedef struct
{
	uint8_t		channel; 		// PWM channel related to the TIMER
	uint32_t	prescaler;		// Timer increment speed
	uint16_t	period;		// Indicates the number of times the Timer is incremented, the period of the frequency is given by Time_Fosc * PSC * ARR
	uint16_t	dutyCycle;		// Percentage (%) value of the time the signal is high
} PWM_Config_t;

/**/
typedef struct
{
	TIM_TypeDef		*ptrTIMx;	// Timer associated with the PWM
	PWM_Config_t	config;	// Initial PWM configuration
} PWM_Handler_t;

/* Function prototypes */
void pwm_Config(PWM_Handler_t *ptrPwmHandler);
void setFrequency(PWM_Handler_t *ptrPwmHandler);
void updateFrequency(PWM_Handler_t *ptrPwmHandler, uint16_t newFreq);
void setDuttyCycle(PWM_Handler_t *ptrPwmHandler);
void updateDutyCycle(PWM_Handler_t *ptrPwmHandler, uint16_t newDuty);
void enableOutput(PWM_Handler_t *ptrPwmHandler);
void startPwmSignal(PWM_Handler_t *ptrPwmHandler);
void stopPwmSignal(PWM_Handler_t *ptrPwmHandler);


#endif /* PWMDRIVER_H_ */
