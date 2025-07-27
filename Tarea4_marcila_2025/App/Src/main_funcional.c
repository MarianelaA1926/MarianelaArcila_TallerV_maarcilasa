/*
 * main_fms.c
 *
 *  Created on: 24/05/2025
 *      Author: marianela
 */

/* ******************************************************************************
 * @file           : main.c
 * @author         : doramirezag
 * @brief          : Desarrollo del driver para el USART
 ******************************************************************************
 * - Implementación de las librerías CMSIS
 * - Migración del GPIO driver
 * - Desarrollo de las librerías para el manejo de USART
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>
#include "GPIOxDriver.h"
#include "BasicTimer.h"
#include "ExtiDriver.h"
#include "stm32_assert.h"
#include "USARTxDriver.h"
#include "AdcDriver.h"
#include "I2CDriver.h"
#include "LCD.h"

//-------------------------------------------------Led State---------------------------------------------//
// Status LED control variables
GPIO_Handler_t handlerStateLed = {0};
BasicTimer_Handler_t handlerTimerStateLed = {0};

//---------------------------------------------------Reset Button-----------------------------------------------
GPIO_Handler_t handlerResetButton = {0};
EXTI_Config_t handlerExtiButton;


uint8_t currentState;
uint8_t buttonPressed;
uint8_t currentColor;


// --- Variables globales del sistema ---
uint32_t current_sampling_freq = 44100;      // Frecuencia de muestreo
uint16_t current_fft_size = 1024;            // Tamaño FFT
char tx_buffer[256];                         // Buffer de transmisión por UART

#define FFT_MAX_SIZE 2048

// Buffers de señal
uint16_t adcRawData_data[FFT_MAX_SIZE];           // Señal muestreada
uint16_t fftMagnitudes[FFT_MAX_SIZE / 2];    // Magnitudes de la FFT (sin float)

//-----------------------------------IMPRIMIR EN CONSOLA----------------------------
GPIO_Handler_t handlerPinTx		= {0};
GPIO_Handler_t handlerPinRx		= {0};

// Utiliza la conexion USB
USART_Handler_t handlerUsart2 = {0};
char tiempoMs = {0};

#define MAX_BUFFER_SIZE 32
#define BTIMER_SPEED_1us	16

char rxBuffer[MAX_BUFFER_SIZE];
uint8_t rxIndex = 0;
uint8_t commandReady = 0;
//----------------------------Variables  comandos usart---------------------------------------------
char rxBuffer[MAX_BUFFER_SIZE];

float frecuencia = 0;  // Hz inicial → 250 ms
uint8_t ledEnabled = 1;  // Controla si el parpadeo está activo o no


//-----------------------------------------------------I2C---------------------------------------------

I2C_Handler_t handlerAccelerometer = {0};
GPIO_Handler_t handlerI2cSDA = {0};
GPIO_Handler_t handlerI2cSCL = {0};
#define ACCEL_ADDRESS 0x53



//---------------------------------------------  DISPLAY -------------------------------------------

GPIO_Handler_t handlerI2cSDA2 = {0};
GPIO_Handler_t handlerI2cSCL2 = {0};
LCD_Handler_t  lcd = {0};

//-------------------------------------------------------FSM----------------------------------------------------



/* Pin configuration function  */
void initSystem(void){

	// --------------------------------- State Led--------------------------------------------------//
	handlerStateLed.pGPIOx = 											GPIOA;
	handlerStateLed.GPIO_PinConfig.GPIO_PinNumber = 					PIN_5;
	handlerStateLed.GPIO_PinConfig.GPIO_PinMode = 						GPIO_MODE_OUT;
	handlerStateLed.GPIO_PinConfig.GPIO_PinOPType = 					GPIO_OTYPE_PUSHPULL;
	handlerStateLed.GPIO_PinConfig.GPIO_PinPuPdControl = 				GPIO_PUPDR_NOTHING;
	handlerStateLed.GPIO_PinConfig.GPIO_PinSpeed = 						GPIO_OSPEED_FAST;

	GPIO_Config(&handlerStateLed);



	//------------------------------------------------Reset Butom-----------------------------------------------------


	/*Button configuration as external interruption*/
	//GPIO configuration
	handlerResetButton.pGPIOx	= 										GPIOA;
	handlerResetButton.GPIO_PinConfig.GPIO_PinNumber = 					PIN_8;
	handlerResetButton.GPIO_PinConfig.GPIO_PinMode = 					GPIO_MODE_IN;
	handlerResetButton.GPIO_PinConfig.GPIO_PinPuPdControl = 			GPIO_PUPDR_PULLUP;


	//ExtiConfiguration
	handlerExtiButton.pGPIOHandler= 									&handlerResetButton;
	handlerExtiButton.edgeType = 										EXTERNAL_INTERRUPT_RISING_EDGE;
	extInt_Config(&handlerExtiButton);




//------------------------------------------------------------Imprimir consola-------------------------------------
	/* Configurando los pines sobre los que funciona el USART2 (TX) */
	handlerPinTx.pGPIOx 							= GPIOA;
	handlerPinTx.GPIO_PinConfig.GPIO_PinNumber		= PIN_2;
	handlerPinTx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinTx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinTx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinTx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinTx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinTx);

	/* Configurando los pines sobre los que funciona el USART2 (RX) */
	handlerPinRx.pGPIOx 							= GPIOA;
	handlerPinRx.GPIO_PinConfig.GPIO_PinNumber		= PIN_3;
	handlerPinRx.GPIO_PinConfig.GPIO_PinMode		= GPIO_MODE_ALTFN;
	handlerPinRx.GPIO_PinConfig.GPIO_PinOPType		= GPIO_OTYPE_PUSHPULL;
	handlerPinRx.GPIO_PinConfig.GPIO_PinPuPdControl	= GPIO_PUPDR_NOTHING;
	handlerPinRx.GPIO_PinConfig.GPIO_PinSpeed		= GPIO_OSPEED_FAST;
	handlerPinRx.GPIO_PinConfig.GPIO_PinAltFunMode	= AF7;
	GPIO_Config(&handlerPinRx);

	// Configurando la comunicación serial (Cable verde es TX, Cable Blanco es RX)
	handlerUsart2.ptrUSARTx 					= USART2;
	handlerUsart2.USART_Config.USART_baudrate	= USART_BAUDRATE_115200;
	handlerUsart2.USART_Config.USART_datasize	= USART_DATASIZE_8BIT;
	handlerUsart2.USART_Config.USART_parity		= USART_PARITY_NONE;
	handlerUsart2.USART_Config.USART_stopbits	= USART_STOPBIT_1;
	handlerUsart2.USART_Config.USART_mode		= USART_MODE_RXTX;
	handlerUsart2.USART_Config.USART_enableIntRx = USART_RX_INTERRUP_ENABLE ;

	// Cargamos la configuración del USART
	USART_Config(&handlerUsart2);


	// We configure TIMER 2 as the counter for the stateLed
	handlerTimerStateLed.ptrTIMx = 										TIM2;
	handlerTimerStateLed.TIMx_Config.TIMx_mode = 						BTIMER_MODE_UP;
	handlerTimerStateLed.TIMx_Config.TIMx_speed= 						BTIMER_SPEED_100us;
	handlerTimerStateLed.TIMx_Config.TIMx_period = 						2500;
	handlerTimerStateLed.TIMx_Config.TIMx_interrupEnable=				BTIMER_ENABLE;

	BasicTimer_Config(&handlerTimerStateLed);

	// We activate the TIM2
	starTimer(&handlerTimerStateLed);

	//Se configura a 16MHz
	config_SysTick_ms(0);


//-------------------------------------------I2C----------------------------------------------------------


	handlerI2cSCL.pGPIOx 							  = GPIOB;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinNumber		  = PIN_8;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinMode		  = GPIO_MODE_ALTFN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinOPType		  = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_PULLUP;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinSpeed		  = GPIO_OSPEED_FAST;
	handlerI2cSCL.GPIO_PinConfig.GPIO_PinAltFunMode	  = AF4;
	GPIO_Config(&handlerI2cSCL);

	handlerI2cSDA.pGPIOx 							  = GPIOB;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinNumber		  = PIN_9;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinMode		  = GPIO_MODE_ALTFN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinOPType		  = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_PULLUP;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinSpeed		  = GPIO_OSPEED_FAST;
	handlerI2cSDA.GPIO_PinConfig.GPIO_PinAltFunMode	  = AF4;
	GPIO_Config(&handlerI2cSDA);

	handlerAccelerometer.ptrI2Cx      = I2C1;
	handlerAccelerometer.modeI2C      = I2C_MODE_FM;
	handlerAccelerometer.slaveAddress = ACCEL_ADDRESS;
	i2c_config(&handlerAccelerometer);

	i2c_writeSingleRegister(&handlerAccelerometer, 45, 8); // POWEWR_CTL: Modo edicion
	i2c_writeSingleRegister(&handlerAccelerometer, 49, 11); // DATA_FORMAT: 16g, full resolution
	i2c_writeSingleRegister(&handlerAccelerometer, 44, 9); // BW_RATE: 50hz

//_---------------------------------------------lCD-------------------------------------------------------------


	handlerI2cSCL2.pGPIOx 							   = GPIOB;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinNumber	   = PIN_10;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinMode		   = GPIO_MODE_ALTFN;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinOPType	   = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_PULLUP;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinSpeed		   = GPIO_OSPEED_FAST;
	handlerI2cSCL2.GPIO_PinConfig.GPIO_PinAltFunMode   = AF4;
	GPIO_Config(&handlerI2cSCL2);

	handlerI2cSDA2.pGPIOx 							   = GPIOB;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinNumber	   = PIN_3;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinMode		   = GPIO_MODE_ALTFN;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinOPType	   = GPIO_OTYPE_OPENDRAIN;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinPuPdControl  = GPIO_PUPDR_PULLUP;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinSpeed		   = GPIO_OSPEED_FAST;
	handlerI2cSDA2.GPIO_PinConfig.GPIO_PinAltFunMode   = AF9;
	GPIO_Config(&handlerI2cSDA2);

	lcd.ptrLCDx = I2C2; // O el I2C que uses
	lcd.slaveAddressLCD = 0x20; // Cambia si tu LCD usa otra dirección
	lcd.modeLCD = LCD_MODE_SM; // Modo estándar
	LCD_Config(&lcd);
	LCD_Init(&lcd, lcd.slaveAddressLCD);


}



//-------------------------------------------------------Main-----------------------------------------------------

struct Filtro
{
	uint16_t count;
	int32_t  suma;

} filX, filY, filZ;


int16_t promedio(struct Filtro *fil, int16_t value)
{
	fil->count++;
	fil->suma += value;

	int16_t prom = fil->suma / fil->count;

	if (fil->count > 500) {
		fil->count = 0;
		fil->suma = 0;
	}

	return prom;
}

int16_t readAccelX()
{
	int16_t scale = 39; //0.0039 * 10000

	uint8_t rax_low  = i2c_readSingleRegister(&handlerAccelerometer, 0x32);
	uint8_t rax_high = i2c_readSingleRegister(&handlerAccelerometer, 0x33);
	int16_t rax = (int16_t)((rax_high << 8) | rax_low);

	return promedio(&filX, rax * scale);

}


int16_t readAccelY(){
	int16_t scale = 39; //0.0039 * 10000

	uint8_t ray_low  = i2c_readSingleRegister(&handlerAccelerometer, 0x34);
	uint8_t ray_high = i2c_readSingleRegister(&handlerAccelerometer, 0x35);
	int16_t ray = (int16_t)((ray_high << 8) | ray_low);

	return promedio(&filY, ray * scale);
}


int16_t readAccelZ()
{
	int16_t scale = 39; //0.0039 * 10000

	uint8_t raz_low  = i2c_readSingleRegister(&handlerAccelerometer, 0x36);
	uint8_t raz_high = i2c_readSingleRegister(&handlerAccelerometer, 0x37);
	int16_t raz = (int16_t)((raz_high << 8) | raz_low);

	return promedio(&filZ, raz * scale);

}




int main(void){
	initSystem();  // Configura todo, incluidos pines y acelerómetro

	//writeMsg(&handlerUsart2, "Probando lectura de DEVID del ADXL345\r\n");

	uint8_t dev_id = i2c_readSingleRegister(&handlerAccelerometer, 0x00);  // Registro DEVID debe devolver 0xE5
	if(dev_id != 0xE5)
	{
		writeMsg(&handlerUsart2, "Dispositivo no encontrado\n");
	}

	char buffer[64];
	sprintf(buffer, "DEVID = 0x%02X\r\n", dev_id);
	writeMsg(&handlerUsart2, buffer);


	while(1)
	{
		int16_t acel_x = readAccelX();
		int16_t acel_y = readAccelY();
		int16_t acel_z = readAccelZ();

		// Imprimir por consola
		char buffer[64];
		sprintf(buffer, "%d\r\n", acel_x);
		writeMsg(&handlerUsart2, buffer);


		// Mostrar X en la primera fila
		LCD_setCursor(&lcd, 0, 0, lcd.slaveAddressLCD);
		sprintf(buffer, "Accel X: %5d", acel_x);
		LCD_sendSTR(&lcd, buffer, lcd.slaveAddressLCD);

		// Mostrar Y en la segunda fila
		LCD_setCursor(&lcd, 1, 0, lcd.slaveAddressLCD);
		sprintf(buffer, "Accel Y: %5d", acel_y);
		LCD_sendSTR(&lcd, buffer, lcd.slaveAddressLCD);

		// Mostrar Z en la tercera fila
		LCD_setCursor(&lcd, 2, 0, lcd.slaveAddressLCD);
		sprintf(buffer, "Accel Z: %5d", acel_z);
		LCD_sendSTR(&lcd, buffer, lcd.slaveAddressLCD);

	}
}




void BasicTimer2_Callback(void){

	 GPIOxTooglePin(&handlerStateLed);
}





void usart2Rx_Callback(void)
{
    char byte = getRxData();


    if (byte == '\r' || byte == '\n') {
        rxBuffer[rxIndex] = '\0';
        // Fin del string
        commandReady = 1;
        rxIndex = 0;

    } else {
        if (rxIndex < MAX_BUFFER_SIZE - 1) {
            rxBuffer[rxIndex++] = byte;
        } else {
            rxIndex = 0;  // Evita desbordamiento
        }
    }

}



// Callback function with External Button (pin A2)
void callback_extInt8(void){
    buttonPressed = GPIO_ReadPin(&handlerResetButton);

    if (buttonPressed == RESET) {
        // Apaga el LED antes de reiniciar
        GPIO_WritePin(&handlerStateLed, RESET);

        // ¡Pequeño retardo para que el LED realmente se apague!
        for (volatile int i=0; i<100000; i++);

        NVIC_SystemReset();


    }
}





void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}
}
