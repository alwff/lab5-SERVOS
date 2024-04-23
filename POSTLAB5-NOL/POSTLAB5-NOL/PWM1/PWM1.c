/* Universidad del Valle de Guatemala
IE2023:: Programación de Microcontroladores
PWM1.c
Autor: Alejandra Cardona
Hardware: ATMEGA328P
 */ 

#include "PWM1.h"
//invertido 1
//no_invertido 0

void chanelSelectANDinvertedORnot(uint8_t chanelS, uint8_t invertOR){ //Seleccionar canal y si canal invertido o no invertido
	if (chanelS==1){ //1=Canal A
		if (invertOR==1){
			TCCR1A |= (1<<COM1A0)|(1<<COM1A1); //Invertido Canal A
		}
		else{
			TCCR1A |= (1<<COM1A1); //No Invertido Canal A
		}
	}
		
	else if (chanelS==2){ //1=Canal B
		if (invertOR==1){
			TCCR1A |= (1<<COM1B0)|(1<<COM1B1); //Invertido Canal B
		}
		else{
			TCCR1A |= (1<<COM1B1); //No Invertido Canal B
		}
	}
}


void modeTIMER1(void){ //Establecer modo fast PWM para los canales A y B

TCCR1A |= (1<<WGM11);
TCCR1B |= (1<<WGM12)|(1<<WGM13);	

}

void prescalerFORchanel(void){ //Establecer prescaler de 8 bits
TCCR1B |= (1<<CS11); //Prescaler 8 bits
ICR1 = 39999; //Valor de TOP	
}


