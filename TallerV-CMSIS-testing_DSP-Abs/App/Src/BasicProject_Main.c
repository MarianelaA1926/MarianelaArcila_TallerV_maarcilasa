/*
 *
 *
 *  Created on: 24/03/2023
 *      Author: marianela
 */
/**
 ******************************************************************************
 * @file           : BasicProyect_Main.c
 * @author         : Marianela Arcila Sanchez- maarcilasa
 * @brief          : The basic solution of a project with external libraries.
 ******************************************************************************
 * -
 * -
 * -
 ******************************************************************************
 */

#include <stdint.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"



int main(void)

{
	/* Activamos el coprocesador Matematico FPU*/
	SCB -> CPACR |= (0xF << 20);

	while(1){


	}
	return 0;
}
