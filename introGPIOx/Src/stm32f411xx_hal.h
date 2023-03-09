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


typedef struct
{
	volatile uint32_t CR;							// Clock Control Register                   ADDR_OFFSET:  0x00
	volatile uint32_t PLLCFGR;                      // PLL Configuration rregister 		        ADDR_OFFSET:  0x04
	volatile uint32_t CFGR;							// Clock Configuration register 			ADDR_OFFSET:  0x08
	volatile uint32_t CIR;							// Clock Interrupt register 				ADDR_OFFSET:  0x0C
	volatile uint32_t AHB1RSTR;						// AHN1 Peripheral reset register 			ADDR_OFFSET:  0x10
	volatile uint32_t AHB2RSTR;						// AHN2 Peripheral reset register 			ADDR_OFFSET:  0x14
	volatile uint32_t reserved0;					// reserved									ADDR_OFFSET:  0x18
	volatile uint32_t reserved1;					// reserved									ADDR_OFFSET:  0x1C
	volatile uint32_t APB1RSTR;						// APB1 peropheral reset register 			ADDR_OFFSET:  0x20
	volatile uint32_t APB2RSTR;						// APB2 peripheral reset register 			ADDR_OFFSET:  0x24
	volatile uint32_t reserved2;					// reserved									ADDR_OFFSET:  0x28
	volatile uint32_t reserved3;					// reserved									ADDR_OFFSET:  0x2C
	volatile uint32_t AHB1ENR;						// AHB1 peripheral clock enable register 	ADDR_OFFSET:  0x30
	volatile uint32_t AHB2ENR;						// AHB2 peripheral clock enalble register   ADDR_OFFSET:  0x34
	volatile uint32_t reserved4;					// reserved 								ADDR_OFFSET:  0x38
	volatile uint32_t reserved5;					// reserved									ADDR_OFFSET:  0x3C
	volatile uint32_t APB1ENR;						// APB1 peripheral clock enable register    ADDR_OFFSET:  0x40
	volatile uint32_t APB2ENR;						// APB2 peripheral clock enable register 	ADDR_OFFSET:  0x44
 	volatile uint32_t reserved6;					// reserved									ADDR_OFFSET:  0x48
	volatile uint32_t reserved7;					// reserved									ADDR_OFFSET:  0x4C
	volatile uint32_t AHB1LPENR;					// AHB1 clock enable low power register 	ADDR_OFFSET:  0x50
	volatile uint32_t AHB2LPENR;					// AHB2 clock enable low power register		ADDR_OFFSET:  0x54
	volatile uint32_t reserved8;					// reserved									ADDR_OFFSET:  0x58
	volatile uint32_t reserved9;					// reserved									ADDR_OFFSET:  0x5C
	volatile uint32_t APB1LPENR;					// APB1 clock enable low power register		ADDR_OFFSET:  0x60
	volatile uint32_t APB2LPENR;					// APB2 clock enable low power register		ADDR_OFFSET:  0x64
	volatile uint32_t reserved10;					// reserved									ADDR_OFFSET:  0x68
	volatile uint32_t reserved11;					// reserved									ADDR_OFFSET:  0x6C
	volatile uint32_t BDCR;							// Backup domain control register			ADDR_OFFSET:  0x70
	volatile uint32_t CSR;							// clock control & status register			ADDR_OFFSET:  0x74
	volatile uint32_t reserved12;
	volatile uint32_t reserverd13;
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
	volatile uint32_t reserved14;
	volatile uint32_t DCKCFGR;

} RCC_RegDef_t;

/*Como se vio en la clase de introduccion de las estructuras, hacemos un puntero a RCC_RegDef_t que
 * apunta a la posicion exavta del periferico RCC, de forma que cada miembro de la estructura coincide
 * con cada uno de los SRF en la memoria del MCU. Esta accion la estamos haciendo en un MACRO, de forma que el
 * nuevo elemento "RCC" queda disponible para cada clse .c (Archivo .c) que incluya este archivo
 *
 */
#define RCC ((RCC_RegDef_t * ) RCC_BASE_ADDR)

























































































































#endif /* STM32F411XX_HAL_H_ */
