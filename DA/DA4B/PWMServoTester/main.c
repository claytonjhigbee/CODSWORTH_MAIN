/*
 * PWMServoTester.c
 *
 * Created: 4/15/2019 9:26:42 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
    // Configure Timer 1
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	/*
	Non inverted PWM
	Prescalar = 64
	WGM Mode 14 - Fast PWM
	*/
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
	ICR1 = 4999;
	/*
	PWM Frequency = 50Hz, Period = 20ms standard for servo motor operation
	*/
	DDRB |= (1<<PORTB1);
    while (1) 
    {
		OCR1A = 97; // Angle = 0
		_delay_ms(1000);
		OCR1A = 316; // Angle = 90
		_delay_ms(1000);
		OCR1A = 425; // Angle = 135
		_delay_ms(1000);
		OCR1A = 535; // Angle = 180
		_delay_ms(1000);
    }
}

