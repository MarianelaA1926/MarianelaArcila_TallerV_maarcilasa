/*
 * main_taller7.c
 *
 *  Created on: 23/03/2023
 *      Author: marianela
 */



#include <stdint.h>


int main(void){

	uint32_t dato = 0xABCD;
	uint32_t *pDato = &dato;

	uint32_t addrDato = (uint32_t)pDato;

	*pDato = 0x1234;
	pDato ++;
	*pDato = 0x5678;

	uint32_t miPrimerArreglo[5] = {12, 15, 0xABC, 0b11111011}; // el 5 no esta relacionado
	uint32_t variable1 = *miPrimerArreglo;

	for(uint8_t i=0; i<5; i++){ //Accede a vada valor de cada arreglo
		uint32_t variableAux= miPrimerArreglo[i];

	}

	// for para ver las diferentes posiciones del arreglo

	for(uint8_t i = 0; i<5; i++){

		uint32_t *auxPunter =*( miPrimerArreglo +i);
		//al ser un puntero de un arreglo no se puede sobre escribir la variable mi primer arreglo
		//ya que esta hace parte del arreglo por defecto por lo tanto se puede crear una
		// nueva variable para poder acceder


	}

	//Dentro de las estructuras podemos tener arreglos

	typedef struct{
		uint8_t edad;
		uint16_t estatura;
		uint32_t notas[10];

	}Estudiante_t; //Estudiante_t se convierte en el tipo de dato


	Estudiante_t estudiante1 = {0};
	// para acceder a una estructura se utiliza un punto, cuando no es
	//estructura se usa una flecha ->

	estudiante1.edad = 22;
	estudiante1.estatura = 173;
	*(estudiante1.notas) = 45;
	*(estudiante.notas +1) = 38;


	//Puntero que apunta a estructura
	Estudiante_t *pEsttudiante1= &estudiante1;

	//Para almacenar palabras
	//Char siempre para palabra ya que se almacenan en 8bits
	//Los strigs siempre almacenan el elemento nulo "\0" nos puede
	//ayudar a saber cuando acaba la frase o palabra
	char miString []= "Linux es lo mejor";



}
