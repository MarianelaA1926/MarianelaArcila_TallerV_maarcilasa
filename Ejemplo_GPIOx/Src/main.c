/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Marianela Arcila Sanchez     maarcilasa
 * @brief          : Main program body
 ******************************************************************************
 * Description:
 * In this project you will find the different
 * drivers necessary for the creation of projects
 * with the STM32 microcontroller.
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f411xx_hal.h"



int main(void){
	//Configuracion incial del MCU
	RCC -> AHB1ENR &= ~(1<<0); // Limpiando la posicion 0 del registro
	RCC -> AHB1ENR |= (1<< 0); // Activamos la Señal del reloj del GPIOA

	// Configurando el pin PA5 como salida
	GPIOA -> MODER &= ~(0b11 << 10); //Limpiando las posiciones 11:10 del MODER
	GPIOA -> MODER |= (0b01 << 10); //Configuracion del pin A5 como salida

	// Configuracion si es push pull
	GPIOA -> OTYPER &= ~(1<<5); //Limpiamos la posicion, configuracion push-pull

	//Config OSPEED
	GPIOA -> OSPEEDR &= ~(0b11 << 10); // lIMPIANDO pOSICIONES 11:10
	GPIOA -> OSPEEDR |= (0B10 << 10); // Velocidad de salida en Fast

	//Configuracion de las resistencias de PU-PD
	GPIOA -> PUPDR &= ~(0b11 << 10); // Limpiamos las posiciones 10:11, no-PUPD

	GPIOA ->ODR &= ~(1 << 5); //Limpiamos la salida PA5, apaga el LED
	GPIOA ->ODR |= (1 <<5); // Enciende el LED



}
