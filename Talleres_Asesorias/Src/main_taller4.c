/*
 * main_taller4.c
 *
 *  Created on: Mar 2, 2023
 *      Author: marianela
 */

#include <stdint.h>

char var1=0; //8bits
int var2 = 0; // 32bits
short var3 = 0; //16 bits

uint8_t var5 = 0; //Variable sin signo de 8 bits
int8_t var6 = 0; //Variable entera de 8 bits
int16_t var7 = 0; // Variable entera de 16 bits
int64_t var9= 0;


int main(void){

	uint16_t testShift = 0b000011010110101;
	uint16_t testMask = 0b0000000000011000;

	while(1){

		uint16_t resultado = testShift | testMask;
		resultado = testShift & (-testMask);


		//Hacer una operacion shift que corre a la izquierda un valor a la vez
		//Variable testShift
		//Shift a la derecha es una division n veces
		//Shift a la izquierda es una multiplicacion n veces
		//testShift = testShift >> 1;
		//var9= testShift;
		//testMask = ~testMask;
	//	testShift = testShift & testMask;



	}


}
