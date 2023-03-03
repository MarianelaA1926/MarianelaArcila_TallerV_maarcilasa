/*
 * stm32f411xx_hal.h
 *
 *  Created on: Mar 3, 2023
 *      Author: marianela
 */

#ifndef STM32F411XX_HAL_H_
#define STM32F411XX_HAL_H_


#include <stdint.h>
#include <stddef.h>

#define HSI_CLOCK_SPEED		16000000		// Value for the main clock signal (HSI -> High Speed Internal)
#define HSE_CLOCK_SPEED		4000000			// Value for the main clock signal (HSE -> High Speed External)


#define NOP() 			asm("NOP")
#define __weak       __attribute__((weak))

/*
 * Base addresses of Flash and SRAM memories
 * Datasheet, Memory Map, Figure 14
 * (Remember, 1KByte = 1024 bytes)
 */

#define FLASH_BASE_ADDR      0x08000000U   //Esta es la memoria del programa, 512KB
#define SRAM_BASE_ADDR		 0x20000000U   // Esta es la meoria RAM 128KB


/* NOTA: Observar que existen unos registros especificos del Cortex M$
 * en la region 0xE0000000U
 * los controladores de las interrupciones se encuentran alli, por ejemplo
 * esto se vera a su debido tiempo*/

/*
 * NOTA:
 * Ahora agregamos la direccion de memoria base para cada uno de los perifericos
 * que posee el micro en el "datasheet" del micro, Figura 14(Memory Map)
 * encontramos el mapa de los buses:
 * APB1(Advance Peripheral Bus)
 * APB2
 * AHB1 (Advance High-performance Bus)
 * AHB2
 */



/*
 * AHBx and APBx Bus Peripeherals base addresses
 */
#define APB1_BASE_ADDR 				0x40000000U
#define APB2_BASE_ADDR				0x40010000U
#define AHB1_BASE_ADDR				0x40020000U
#define AHB2_BASE_ADDR				0x50000000U

/**
 * Y ahora debemos hacer lo mismo pero cada una de las posiciones de memoria
 * de cada uno de los perifericos descritos en la Tabla 1 del manual de referencia
 * del micro, observe que en dicha tabla esta a su vez dividida en cuatro segmentos,
 * cada uno correspondiente a APB1 APB2 AHB1 AHB2
 *
 * Comenzar de arriba hacia abajo como se uestra en la tabla inica USB_OTG_FS (AHB2)
 *
 * NOTA: Solo lo vamos a hacer con elementos que necesitamos en el ejercicio: RCC y GPIOx
 */


/* Posiciones de memoria para perifericos del AHB1
 * Observar que No esta completa
 */
#define RCC_BASE_ADDR		(AHB1_BASE_ADDR + 0x3800U)
#define GPIOH_BASE_ADDR		(AHB1_BASE_ADDR + 0x1C00U)
#define GPIOE_BASE_ADDR     (AHB1_BASE_ADDR + 0x1000U)
#define GPIOD_BASE_ADDR	    (AHB1_BASE_ADDR + 0x0C00U)
#define GPIOC_BASE_ADDR	    (AHB1_BASE_ADDR + 0x0800U)
#define GPIOB_BASE_ADDR	    (AHB1_BASE_ADDR + 0x0400U)
#define GPIOA_BASE_ADDR	    (AHB1_BASE_ADDR + 0x0000U)


/**
 * Macros Genericos
 */

#define ENABLE			1
#define DISABLE			0
#define SET				ENABLE
#define CLEAR			DISABLE
#define RESET			DISABLE
#define GPIO_PIN_SET	SET
#define GPIO_PIN_RESET	RESET
#define FLAG_SET		SET
#define FLAG_RESET		RESET
#define I2C_WHIRE		0
#define I2C_READ		1

/* +++=================INICIO DE LA DESCRIPCION DE LOS ELEMENTOS QUE COMPONEN EL PERIFERICO ===============================+++++*/

/*Definicion de la estructura de datos que representa cada uno de los registros
 * que componen el periferico RCC
 *
 * Debido a los temas que van a maneajar en el curso, solo se deben definir los
 * bits de los registros:
 * 6.3.1 (RCC_CSR)
 * NOTA: La posicion de memria Offset debe encajar percectamente con la posicion de memoria indicada
 * en la hoja de datos del equipo. Observe que los elementos "Reservados" tambien estan presentes ahi
 */




























































































































#endif /* STM32F411XX_HAL_H_ */
