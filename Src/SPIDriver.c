/*
 * SPIDriver.c
 *
 *  Created on: 18/06/2023
 *      Author: marianela
 */



#include "SPIDriver.h"
#include "GPIOxDriver.h"

void spi_config(SPI_Handler_t ptrHandlerSPI){
	// 1. Activamos la señal de relok del periferico
	if(ptrHandlerSPI.ptrSPIx == SPI1){
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if(ptrHandlerSPI.ptrSPIx == SPI2){
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	}
	else if(ptrHandlerSPI.ptrSPIx == SPI3){
		RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	}
	else if(ptrHandlerSPI.ptrSPIx == SPI4){
		RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
	}
	else if(ptrHandlerSPI.ptrSPIx == SPI5){
		RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;
	}

	// 2. Limpiamos el registro de configuracion para comenzar de cero
	ptrHandlerSPI.ptrSPIx->CR1 = 0x00;

	// 3. Configuramos la velocidad a la que se maneja el reloj del SPI
	ptrHandlerSPI.ptrSPIx->CR1 |= (ptrHandlerSPI.SPI_Config.SPI_baudrate << SPI_CR1_BR_Pos);

	// 4. Configuramos el modo
	switch(ptrHandlerSPI.SPI_Config.SPI_mode){
	case 0:
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPHA;
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPOL;
		break;
	case 1:
		ptrHandlerSPI.ptrSPIx->CR1 |=  SPI_CR1_CPHA;
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPOL;
		break;
	case 2:
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPHA;
		ptrHandlerSPI.ptrSPIx->CR1 |=  SPI_CR1_CPOL;
		break;
	case 3:
		ptrHandlerSPI.ptrSPIx->CR1 |=  SPI_CR1_CPHA;
		ptrHandlerSPI.ptrSPIx->CR1 |=  SPI_CR1_CPOL;
		break;

	default:
		// Configuramos le modo 00 como casi por defecto
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPHA;
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_CPOL;
		break;
	}

	// 5. Configuramos si fullDuplex o solo recepcion
	if(ptrHandlerSPI.SPI_Config.SPI_fullDupplexEnable == SPI_FULL_DUPPLEX){
		// Selecciona fullDupplex
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_RXONLY;
		// Selecciona modo uni-direccional (cada linea solo tiene una direccion)
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_BIDIMODE;
	}
	else{
		// selecciona solo RX activado
		ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_RXONLY;
	}

	// 6. Modo de tranferencia en msb-first
	ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_LSBFIRST;

	// 7. Activamos para que el equipo se comporte como el maestro de la red
	ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_MSTR;

	// 8. Configuramos el formato del dato (tamaño) para que sea de 8-bit
	if(ptrHandlerSPI.SPI_Config.SPI_dataSize == SPI_DATASIZE_8_BIT){
		// frame de datos de 8-bit
		ptrHandlerSPI.ptrSPIx->CR1 &= ~SPI_CR1_DFF;
	}
	else{
		// frame de datos de 16-bit
		ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_DFF;
	}

	/* 9. Configuramos para que el contro del pin SS (seleccion del slave) sea controlado por
	 *    software (nosotro debemos hacer ese contro), de la otra forma, será el hardware
	 *    el que se controla la seleccion del slave
	 */
	ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_SSM;
	ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_SSI;

	// 10. Activamos el periferico SPI
	ptrHandlerSPI.ptrSPIx->CR1 |= SPI_CR1_SPE;
}

void spi_transmit(SPI_Handler_t ptrHandlerSPI, uint8_t *ptrData, uint32_t dataSize){

	uint8_t auxData;
	(void) auxData;

	while (dataSize > 0){
		// Esperamos a que el buffer esté vacio
		while( !(ptrHandlerSPI.ptrSPIx->SR & SPI_SR_TXE)){
			__NOP();
		}
		//Enviamos el dato al que apunta el puntero
		ptrHandlerSPI.ptrSPIx->DR = (uint8_t) (0xFF & *ptrData);
		// Actualizamos el puntero y el numero de datos que faltan por enviar
		ptrData++;
		dataSize--;
	}
	// Esperamos de nuevo a que el buffer esté vacio
	while ( !(ptrHandlerSPI.ptrSPIx->SR & SPI_SR_TXE)){
		__NOP();
	}
	// Esperamos a que la bandera de ocupado (busy) baje (observar que la logica cambia)
	while((ptrHandlerSPI.ptrSPIx->SR & SPI_SR_BSY)){
		__NOP();
	}
	/*Debemos limpiar la bandera de OverRun (que a veces se levanta)
	 * Para esto debemos leer el DR y luego leer el SR del modulo SPI (pag 599)
	 */
	auxData = ptrHandlerSPI.ptrSPIx->DR;
	auxData = ptrHandlerSPI.ptrSPIx->SR;
}

void spi_receive(SPI_Handler_t ptrHandlerSPI, uint8_t *ptrData, uint32_t dataSize){

	while(dataSize){
		//Esperamos de nuevo a que el buffer esté vacio
		while( !(ptrHandlerSPI.ptrSPIx->SR & SPI_SR_TXE)){
			__NOP();
		}
		//Enviamos el valor dummy
		ptrHandlerSPI.ptrSPIx->DR = 0x00;
		// Esperremos de nuevo a que el buffer tenga un dato que leer
		while( !(ptrHandlerSPI.ptrSPIx->SR & SPI_SR_RXNE)){
			__NOP();
		}
		// Cargamos el valor en el puntero
		*ptrData = ptrHandlerSPI.ptrSPIx->DR;
		// Actualizamos el puntero y el tamaño del dato
		ptrData++;
		dataSize--;
	}
}

// Seleccionamos el esclavo llevando el pin SS a GND
void spi_selecSlave(SPI_Handler_t* ptrHandlerSPI){
	GPIO_WritePin(&ptrHandlerSPI -> SPI_slavePin, RESET);
}

// Seleccionamos el esclato llevando el pin SS a Vcc
void spi_unSelecSlave(SPI_Handler_t* ptrHandlerSPI){
	GPIO_WritePin(&ptrHandlerSPI -> SPI_slavePin, SET);
}
