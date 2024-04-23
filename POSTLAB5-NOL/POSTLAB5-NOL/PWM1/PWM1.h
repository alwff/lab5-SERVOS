/* Universidad del Valle de Guatemala
IE2023:: Programación de Microcontroladores
PWM1.h
Autor: Alejandra Cardona
Hardware: ATMEGA328P
 */ 


#ifndef PWM1_H_
#define PWM1_H_

// LIBRERÍAS
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

// Palabras reservadas

#define invertido 1
#define no_invertido 0

// FUN

void chanelSelectANDinvertedORnot(uint8_t chanelS, uint8_t invertOR); //Seleccionar canal y si canal invertido o no invertido
void modeTIMER1(void); //Establecer modo fast PWM
void prescalerFORchanel(void); //Establecer prescaler de 8 bits

#endif /* PWM1_H_ */