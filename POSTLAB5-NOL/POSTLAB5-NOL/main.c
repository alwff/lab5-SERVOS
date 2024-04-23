/* Universidad del Valle de Guatemala
IE2023:: Programación de Microcontroladores
POSTLAB5.c
Autor: Alejandra Cardona
Hardware: ATMEGA328P
Creado: 21/04/2024
Última modificación: 22/04/2024

****************************************************************** */

// LIBRERÍAS

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "PWM1/PWM1.h" //Incluir mis librerías

void initADC(void); //Configuración de ADC
float map(float, float, float, float, float); //Mapeo del servo

//Variable Formato: Tipo nombre = Valor
//Variables:
uint8_t canales = 0;

int main(void)
{
	cli(); //Apaga las interrupciones
	
	//Configuración para TIMER1---------------------------------------------------------
	
	DDRB |= (1 << PORTB2) | (1 << PORTB1); //PB2 y PB1 como salidas (OC0A and OCR0B)
	DDRC = 0; //Puerto C como salida -- SERVOS
	
	//Set los registros del timer1 en 0 para facilitar la configuración
	TCCR1A = 0;
	TCCR1B = 0;
	
	/*
	//Configurando modo y canal: No invertido y  FAST PWM (TOP = ICR1)
	TCCR1A |= (1<<WGM11)|(1<<COM1B1)|(1<<COM1A1);
	TCCR1B |= (1<<WGM12)|(1<<WGM13);
	
	//TCCR1B |= (1<<CS11); //Prescaler 8 bits
	//ICR1 = 39999; //Valor de TOP
	*/
	
	modeTIMER1(); //Establecer modo fast PWM para los canales A y B
	chanelSelectANDinvertedORnot(1, no_invertido); //Seleccionar canal A y No Invertido
	chanelSelectANDinvertedORnot(2, no_invertido); //Seleccionar canal B y No Invertido
	prescalerFORchanel(); //Establecer prescaler de 8 bits
	
	//FIN TIMER1------------------------------------------------------------------------
	
	//Configuración para TIMER0---------------------------------------------------------
	//TIMER0 para manipular brillo de LED
	
	DDRD |= (1<<PORTD5); //PD5 como salida (OC0B)
	
	//Set los registros del timer0 en 0 para facilitar la configuración
	TCCR0A = 0;
	TCCR0B = 0;
	
	//Configurando modo y canal: No invertido y  FAST PWM (TOP = 255)
	TCCR0A |= (1<<WGM00)|(1<<WGM01)|(1<<COM0B1);
	
	TCCR0B = 0x05; //Prescaler 1024
	
	//FIN TIMER0------------------------------------------------------------------------
	
	initADC(); //Llama initADC
	sei(); //Enciende las interrupciones
	ADCSRA |= (1 << ADSC); //Se inicia la interrupción

	while (1)
	{
		
	}
}

void initADC(void){
	ADMUX = 0;
	//Selecciona la referencia (Vref)=AVCC=5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	//ADLAR - Justificación hacia la izquierda 8b
	//Valores de 0 a 255 -- 8 bits
	//Valores de 0 a 1023 -- 10 bits
	ADMUX |= (1<<ADLAR); //Si 0=10 bits, si 1=8bits
	
	ADCSRA = 0; //Set registro en 0
	// ADEN - Enciende ADC
	ADCSRA |= (1<<ADEN);
	// ADIE - Habilita ISR ADC
	ADCSRA |= (1<<ADIE);
	
	// Ya que nos encontramos en el rango permitido, es posible usar alta resolución
	// Prescaler 128 > 16MHz / 128 = 125KHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	DIDR0 |= (1<<ADC0D);	//Deshabilita las entradas digitales en PC0
}

//Interrupciones
ISR (ADC_vect){

	ADCSRA |= (1 << ADIF);	//Apaga la bandera
	
	if (canales==0){ //canales set en 0 al inicio del programa - canal0
		canales=1; //Al entrar en el canal 0 se cambia al canal 1
		ADMUX = (ADMUX&0xF0); //ADMUX en canal 0
		OCR1A = map(ADCH, 0, 255, 1000, 4000); // Escala (valorminbits, valormaxbits, minpulso, maxpulso)
		//ADCH solo lee los registros del HIGH
	}
	else if(canales==1){
		canales=2; //Al entrar en el canal 1 se cambia al canal 2
		ADMUX = (ADMUX&0xF0)|1; //ADMUX en canal 1
		OCR1B = map(ADCH, 0, 255, 1000, 4000);
	}
	else if(canales==2){
		canales = 0; //Al entrar en el canal 2 se cambia al canal 0
		ADMUX = (ADMUX&0xF0)|2; //ADMUX en canal 2
		OCR0B = ADCH;
	}
	ADCSRA |= (1<<ADSC); //Inicia la conversión - Es decir, vuelve a leer
}

//Mapeo del servo
float map(float position, float minimo, float maximo, float minimopwm, float maximopwm){
	return ((position - minimo)*(maximopwm - minimopwm)/(maximo - minimo)) + minimopwm;
}
