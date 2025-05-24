/*
 * RtcDriver.h
 *
 *  Created on: 10/06/2023
 *      Author: marianela
 */



#ifndef RTCDRIVER_H_
#define RTCDRIVER_H_

#include <stm32f4xx.h>
#define NOP()  asm("NOP")

#define  LUNES          0b001
#define  MARTES         0b010
#define  MIERCOLES      0b011
#define  JUEVES         0b100
#define  VIERNES        0b101
#define  SABADO         0b110
#define  DOMINGO        0b111

#define AM               0
#define PM               1

#define ENERO            1
#define FEBRERO          2
#define MARZO            3
#define ABRIL            4
#define MAYO             5
#define JUNIO            6
#define JULIO            7
#define AGOSTO           8
#define SEPTIEMBRE       9
#define OCTUBRE          10
#define NOVIEMBRE        11
#define DICIEMBRE        12

#define  HOUR_FORMAT_24  0
#define  HOUR_FORMAT_12  1



typedef struct
{

	uint8_t               unidades;
	uint8_t               decenas;

}BDC_Form;

typedef struct
{
	uint16_t           year;
	uint8_t           month;
	uint8_t           dayWeek;
	uint8_t           day;
	uint8_t           hour;
	uint8_t           format_hour;
	uint8_t           AM_PM;
	uint8_t           minute;
	uint8_t           seconds;
}handlerRTC_t;

void setTime (handlerRTC_t *informacionFecha);
void  convertidorBCD (BDC_Form *BDC, uint8_t dato);
void getTime(handlerRTC_t *fechaActual);
uint8_t convertidorBinario( uint8_t decenas, uint8_t unidades);


#endif /* RTCDRIVER_H_ */
