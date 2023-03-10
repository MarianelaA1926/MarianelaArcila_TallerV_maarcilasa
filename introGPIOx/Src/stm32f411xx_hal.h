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
	volatile uint32_t reserved12;					// reserved									ADDR_OFFSET:  0x78
	volatile uint32_t reserverd13;					// reserved									ADDR_OFFSET:  0x7C
	volatile uint32_t SSCGR;						// spread spectrum clock generation reg     ADDR_OFFSET:  0x80
	volatile uint32_t PLLI2SCFGR;					// PLLI2C configuration register			ADDR_OFFSET:  0x84
	volatile uint32_t reserved14;					// reserved									ADDR_OFFSET:  0x88
	volatile uint32_t DCKCFGR;						// Dedicated Clocks Configuration Reg		ADDR_OFFSET:  0x8C

} RCC_RegDef_t;

/*Como se vio en la clase de introduccion de las estructuras, hacemos un puntero a RCC_RegDef_t que
 * apunta a la posicion exavta del periferico RCC, de forma que cada miembro de la estructura coincide
 * con cada uno de los SRF en la memoria del MCU. Esta accion la estamos haciendo en un MACRO, de forma que el
 * nuevo elemento "RCC" queda disponible para cada clse .c (Archivo .c) que incluya este archivo
 *
 */
#define RCC ((RCC_RegDef_t * ) RCC_BASE_ADDR)


/*Descripcion bit a bit de cada uno de los registros del que componen el periferico RCC
 * en este primer ejercicio muchos de ellos no son necesarios, se dejan sin implementar
 * para este primer ejercicio
 */

/* 6.3.9 RCC_AHB1ENR*/
#define RCC_AHB1EMR_GPIOA_EN 				0
#define RCC_AHB1EMR_GPIOB_EN				1
#define RCC_AHB1EMR_GPIOC_EN				2
#define RCC_AHB1EMR_GPIOD_EN				3
#define RCC_AHB1EMR_GPIOE_EN				4
#define RCC_AHB1EMR_GPIOH_EN				7
#define RCC_AHB1EMR_CRCEN					12
#define RCC_AHB1EMR_DMA1_EN					21
#define	RCC_AHB1EMR_DMA2_EN					22


/* ==============Fin de la descripcion de los elementos que componene el periferico RCC =================*/

/* ===============INICIO DE LA DESCRIPCION DE LOS ELEMENTOS QUE COMPONENE LE PERIFERICO GPIOx==========*/

/* Definicion de la estructura de datos que representa cada uno de los registros que componen
 * el periferico GPIO
 *
 * Debido a que el periferico GPIOx es muy simple, no es my necesario crear la descripcion bit a bit de cada
 * uno de los registros que componen el dicho periferico, pero si es necesario comprender que hace cada registro,
 * para poder cargar correctamente la configuracion
 */

typedef struct
{
	volatile uint32_t MODER;				//Port mode register 		           ADDR_OFFSET:  0x00
	volatile uint32_t OTYPER;				//Port output type register			   ADDR_OFFSET:  0x04
	volatile uint32_t OSPEEDR;				//Port output speed register		   ADDR_OFFSET:  0x08
	volatile uint32_t PUPDR;				//Port pull-up/pull-down register      ADDR_OFFSET:  0x0C
	volatile uint32_t IDR;					//Port input data register			   ADDR_OFFSET:  0x10
	volatile uint32_t ODR;					//Port output data register 		   ADDR_OFFSET:  0x14
	volatile uint32_t BSRR;					//Port bit set/reset register 		   ADDR_OFFSET:  0x18
	volatile uint32_t LCKR;					//Port configuration lock register     ADDR_OFFSET:  0x1C
	volatile uint32_t AFRL;					//Alternate function low register      ADDR_OFFSET:  0x20
	volatile uint32_t AFRH;					//Alternate function Higt register	   ADDR_OFFSET:  0x24
} GPIO_RegDef_t;

/* Al igual que con el RCC, creamos un puntero a la estructura que define a GPIOx y debemos hacer
 * que cada GPIOx (A, B, C...) quede ubicado exavtamente sobre la posicion de memoria correcta.
 * Debido a que son varios perifericos GPIOx, es necesario hacer la definicion para cada uno.
 *
 *
 * NOTA: Tener cuidado que cada elemento coincida con su respectiva direccion base
 */

#define GPIOA		((GPIO_RegDef_t *) GPIOA_BASE_ADDR)
#define GPIOB		((GPIO_RegDef_t *) GPIOB_BASE_ADDR)
#define GPIOC		((GPIO_RegDef_t *) GPIOC_BASE_ADDR)
#define GPIOD		((GPIO_RegDef_t *) GPIOD_BASE_ADDR)
#define GPIOE		((GPIO_RegDef_t *) GPIOE_BASE_ADDR)
#define GPIOH		((GPIO_RegDef_t *) GPIOH_BASE_ADDR)

/* Valores estandar para las configuraciones */
/* 8.4.1 GPIOx_MODER (dos bit por cada PIN) */

#define GPIO_MODE_IN                0
#define GPIO_MODE_OUT				1
#define GPIO_MODE_ALTFN				2
#define GPIO_MODE_ANALOG			3

/* 8.4.2 GPIOx_OTYPER (un bit por PIN)*/

#define GPIO_OTYPE_PUSHPULL     0
#define GPIO_OTYPE_OPENDRAIN	1

/*8.4.3 GPIOx_OSPEEDR (dos bit por cada PIN) */
#define GPIO_OSPEED_LOW  		0
#define GPIO_OSPEED_MEDIUM		1
#define GPIO_OSPEED_FAST		2
#define GPIO_OSPEED_HIGH		3

/* 8.4.4 GPIOx_PUPDR (dos bits por cada PIN) */
#define GPIO_PUPDR_NOTHING       0
#define GPIO_PUPDR_PULLUP		 1
#define GPIO_PUPDR_PULLDOWN		 2
#define GPIO_PUPDR_RESERVED		 3

/* 8.4.5 GPIOx_IDR( un bit por PIN) - este es el registro para leer el estado del pin */

/*8.4.6 GPIOx_ODR (un bit por PIN) este es el registro para escribir el estado de un PIN (1 o 0).
 *Este registro puede ser escrito y leido desde el software, pero no garantiza una escritura "atomica"
 *por la cual es preferible el registro BSRR */

/* Definicion de los nombres de los pines */

#define PIN_0
#define PIN_1
#define PIN_2
#define PIN_3
#define PIN_4
#define PIN_5
#define PIN_6
#define PIN_7
#define PIN_8
#define PIN_9
#define PIN_10
#define PIN_11
#define PIN_12
#define PIN_13
#define PIN_14
#define PIN_15

/* Definicion de las funciones alternativas */

#define AF0				0b0000
#define AF1             0b0001
#define AF2             0b0010
#define AF3				0b0011
#define AF4				0b0100
#define AF5				0b0101
#define AF6				0b0110
#define AF7				0b0111
#define AF8				0b1000
#define AF9				0b1001
#define AF10			0b1010
#define AF11			0b1011
#define AF12			0b1100
#define AF13			0b1101
#define AF14			0b1110
#define AF14			0b1111



#endif /* STM32F411XX_HAL_H_ */
