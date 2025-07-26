/*
 * BasicTimer.h
 *
 *  Created on: 22/03/2023
 *      Author: Marianela Arcila Sanchez 			maarcilasa
 * In this program, we define the functions needed to configure
 * the general-purpose timer peripheral "TIMx.
 *
 */

#ifndef INC_BASICTIMER_H_
#define INC_BASICTIMER_H_

#include "stm32f4xx.h"

#define BTIMER_MODE_UP 				0
#define BTIMER_MODE_DOWN			1


#define BTIMER_SPEED_1us			16
#define BTIMER_SPEED_10us			160
#define BTIMER_SPEED_100us			1600
#define BTIMER_SPEED_1ms			16000


#define BTIMER_DISABLE  0
#define BTIMER_ENABLE   1

/*This structure contains the definition of the variables that
 * the user has chosen. */

typedef struct{

	uint8_t 			TIMx_mode;						//Up or Down
	uint32_t 			TIMx_speed;						// Speed at which the timer increases
	uint32_t 			TIMx_period;					// Value for the period of the timer in miliseconds (ms
	uint8_t				TIMx_interrupEnable;			// Timer interrupt mode (On, Off)

}BasicTimer_Config_t;

/*Timer handler*/

typedef struct{

	TIM_TypeDef 					*ptrTIMx;
	BasicTimer_Config_t 			TIMx_Config;

}BasicTimer_Handler_t;


void BasicTimer_Config(BasicTimer_Handler_t *ptrBTimerHandler);
void BasicTimer2_Callback(void); /*This function overwrites the 'main' so the system works.*/
void BasicTimer3_Callback(void); /*This function overwrites the 'main' so the system works.*/
void BasicTimer4_Callback(void); /*This function overwrites the 'main' so the system works.*/
void BasicTimer5_Callback(void); /*This function overwrites the 'main' so the system works.*/

void starTimer (BasicTimer_Handler_t *ptrBTimerHandler);

void stopTimer(BasicTimer_Handler_t *ptrBTimerHandler);















#endif /* INC_BASICTIMER_H_ */
