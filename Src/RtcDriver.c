/*
 * RtcDriver.c
 *
 *  Created on: 10/06/2023
 *      Author: Marianela Arcila Sanchez  maarcilasa
 */


#include "RtcDriver.h"




void setTime (handlerRTC_t *informacionFecha){


	BDC_Form estructuraAuxiliar = {0};
	uint32_t RTC_TR_DATA = 0;
	uint32_t RTC_DR_DATA = 0;

	/*Lo primero que se debe hacer si se quiere cambiar los registros del tiempo, es desbloquear
	 * la protección que tiene el sistema para la escritura
	 * En caso de reinicio total del sistema, se debe habilitar el bit DBP (Disable backup domain write protection)
	 *  del PWR,y por tanto, primero se debe activar la señal de reloj al periferico  PWR
	 */
	RCC-> APB1ENR |= RCC_APB1ENR_PWREN;
	//Ya se puede habilitar el bit  DBP del PWR
	//
	PWR->CR |= PWR_CR_DBP;
	PWR->CSR |= PWR_CSR_BRE;

	RTC->WPR |= (RTC_WPR_KEY&0xCA);
	RTC->WPR |= (RTC_WPR_KEY&0x53);

	/*Ya habilitado el bit DBP, se puede escoger la fuente para el RTC, esto se hace en el registro RCC_BDCR (backup domain control
	 * register) y los bits para seleccionar la fuente del RTC  son los bits 9 y 8 (RTCSEL)
	 * Para seleccionar el crsital de la board (LSE) se debe poner 01 en estos bits
	 *
	 */
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

	RCC->BDCR |= RCC_BDCR_RTCSEL_0;


	/*Ya seleccionado el LSE como fuente para el RTC, se debe encender este reloj, esto se hace poniendo 1 en el bit 0 (LSEON)
	 *  del RCC_BDCR
	 *  */

	RCC->BDCR |= RCC_BDCR_LSEON;

	//Esperamos hasta que el oscilador este estable

	while (!(RCC->BDCR & RCC_BDCR_LSERDY)){
		NOP();
	}

	//Por ultimo, encendemos el RTC, esto se hace poniendo 1 en el bit 15 (RTCEN) del RCC_BDCR

	RCC->BDCR |= RCC_BDCR_RTCEN;

	/*Esperamos a que los registros se sincronicen, esto*/
	while (!(RTC -> ISR & RTC_ISR_RSF)){
			NOP();
		}

	/*Ya con la fuente y el reloj encendido, se pueden configuarar los registros para el tiempo
	 * Se debe habilitar el bit 7 (init) del RTC_ISR (Initialization and status register).
	 * Habilitar este bit permite entrar al modo de inicialización para asi programar los registros del
	 * tiempo y la fecha. Habilitar este bit detiene el contador
	 * 	 */
	RTC->WPR |= (RTC_WPR_KEY&0xCA);
	RTC->WPR |= (RTC_WPR_KEY&0x53);

	RTC->ISR |= RTC_ISR_INIT;
	//Se espera hasta que la bandera de la inicializacion se levante

	while (!(RTC -> ISR & RTC_ISR_INITF)){
		NOP();
	}

	/*AHora se cargan las configuraciones iniciales para la fecha y hora en los
	 * registros TR y CR. Estos registros van a gusrdar la ultima escritura que se les hizo antes de que
	 * se desabilite el bit INIT, por lo cual se deben escribir todos los datos del registro de una sola vez,
	 * para esto se van a ir almacenando  los datos neccesarios en una variable auxiliar a medida que se van
	 * obteniendo su forma BCD. YA una vez la configuración inicial del registro se almacena en la variable,
	 * se escribe en el registro,tanto el DR como el TR
	 */
	/*Se almacena el formato elegido en la variable RTC_TR_DATA conservado la posicion que debe tener en
	 * el registro
	 */

	 RTC_TR_DATA |= (informacionFecha->AM_PM<<RTC_TR_PM_Pos);

	 //Se obtiene la forma BCD  de la hora
	convertidorBCD (&estructuraAuxiliar,informacionFecha->hour);

	//Se almacena en la variable RTC_TR_DATA conservado la posicion que debe tener en el registro
	 RTC_TR_DATA |= ((estructuraAuxiliar.decenas << RTC_TR_HT_Pos)|(estructuraAuxiliar.unidades << RTC_TR_HU_Pos));

	 //Se obtiene la forma BCD  de lOS minutos
	 convertidorBCD (&estructuraAuxiliar,informacionFecha->minute);

	 //Se almacena en la variable RTC_DR_DATA conservado la posicion que debe tener en el registro
	 RTC_TR_DATA |=  ((estructuraAuxiliar.decenas << RTC_TR_MNT_Pos)|(estructuraAuxiliar.unidades << RTC_TR_MNU_Pos));

	 //Se obtiene la forma BCD  de los segundos
	 convertidorBCD (&estructuraAuxiliar,informacionFecha->seconds);
	 //Se almacena en la variable RTC_DR_DATA conservado la posicion que debe tener en el registro
	 RTC_TR_DATA |=  ((estructuraAuxiliar.decenas << RTC_TR_ST_Pos)|(estructuraAuxiliar.unidades << RTC_TR_SU_Pos));

	 //Se escriben todas las configuraciones guardadas en la variable RTC_DR_DATA en el registro TR
	 RTC->TR |= RTC_TR_DATA;
	 convertidorBCD (&estructuraAuxiliar,informacionFecha->year); //Se obtiene la forma BCD  del año

	//Se almacena en la variable RTC_DR_DATA conservado la posicion que debe tener en el registro
	RTC_DR_DATA |= ((estructuraAuxiliar.decenas << RTC_DR_YU_Pos)|(estructuraAuxiliar.unidades << RTC_DR_YT_Pos));

	//Se almacena el dia de la semana en la posicion que debe tener en el registro

	RTC_DR_DATA |= ((informacionFecha->dayWeek)<<RTC_DR_WDU_Pos);

	convertidorBCD (&estructuraAuxiliar,informacionFecha->month);  //Se obtiene la forma BCD  del mes

	//Se almacena en la variable RTC_DR_DATA conservado la posicion que debe tener en el registro
	RTC_DR_DATA |= ((estructuraAuxiliar.decenas << RTC_DR_MT_Pos)|((estructuraAuxiliar.unidades) << RTC_DR_MU_Pos));

	convertidorBCD (&estructuraAuxiliar,informacionFecha->day);  //Se obtiene la forma BCD  del dia

	//Se almacena en la variable RTC_DR_DATA conservado la posicion que debe tener en el registro
	RTC_DR_DATA |= ((estructuraAuxiliar.decenas << RTC_DR_DT_Pos)|((estructuraAuxiliar.unidades) << RTC_DR_DU_Pos));

	/*Se escriben todas las configuraciones guardadas en la variable RTC_DR_DATA en el registro DR
	 Se espera hasta que la bandera de la inicializacion se levante*/

	RTC->DR =  RTC_DR_DATA;
	RTC->TR = RTC_TR_DATA;
	RTC->ISR &= ~RTC_ISR_INIT;
}








void getTime(handlerRTC_t *fechaActual){

	uint32_t RTC_TR_DATA = 0;
	uint32_t RTC_DR_DATA = 0;

	// Esperamos a que los registros se sincronicen
	while (!(RTC->ISR & RTC_ISR_RSF));

	RTC_TR_DATA |= RTC->TR;
	RTC_DR_DATA |= RTC->DR;

	fechaActual->AM_PM = (RTC_TR_DATA & RTC_TR_PM) >> RTC_TR_PM_Pos;
	fechaActual->hour = convertidorBinario((RTC_TR_DATA & RTC_TR_HT) >> RTC_TR_HT_Pos, (RTC_TR_DATA & RTC_TR_HU) >> RTC_TR_HU_Pos);
	fechaActual->minute = convertidorBinario((RTC_TR_DATA & RTC_TR_MNT) >> RTC_TR_MNT_Pos, (RTC_TR_DATA & RTC_TR_MNU) >> RTC_TR_MNU_Pos);
	fechaActual->seconds = convertidorBinario((RTC_TR_DATA & RTC_TR_ST) >> RTC_TR_ST_Pos, (RTC_TR_DATA & RTC_TR_SU) >> RTC_TR_SU_Pos);
	fechaActual->year = convertidorBinario((RTC_DR_DATA & RTC_DR_YT) >> RTC_DR_YT_Pos, (RTC_DR_DATA & RTC_DR_YU) >> RTC_DR_YU_Pos);
	fechaActual->dayWeek = (RTC_DR_DATA & RTC_DR_WDU) >> RTC_DR_WDU_Pos;
	fechaActual->month = convertidorBinario((RTC_DR_DATA & RTC_DR_MT) >> RTC_DR_MT_Pos, (RTC_DR_DATA & RTC_DR_MU) >> RTC_DR_MU_Pos);
	fechaActual->day = convertidorBinario((RTC_DR_DATA & RTC_DR_DT) >> RTC_DR_DT_Pos, (RTC_DR_DATA & RTC_DR_DU) >> RTC_DR_DU_Pos);

}

void convertidorBCD(BDC_Form *BDC, uint8_t dato) {
	uint8_t variableAuxiliar = 0;
	variableAuxiliar = dato/10;
	BDC->decenas = variableAuxiliar;
	variableAuxiliar = dato-variableAuxiliar*10;
	BDC->unidades = variableAuxiliar;
}
uint8_t convertidorBinario(uint8_t decenas, uint8_t unidades) {
    uint8_t numeroBinario = 0;
    numeroBinario = (decenas << 4) | unidades;
    return numeroBinario;
}
