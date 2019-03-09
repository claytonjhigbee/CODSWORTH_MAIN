/*
 * InterruptEnabledPushButtonCCode.c
 *
 * Created: 3/6/2019 9:23:38 PM
 * Author : clayt
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main()
{
	DDRB = 0xFF; // Set PortB as an output
	PORTB = 0xFF; // Set all PinB HIGH (REVERSE LOGIC LOW), turn all LEDs off
	PORTD = 1<<2; // Set pull up resistor in PortD pin 2
	EICRA = 0x2; // Set INT0 to be falling edge triggered
	
	EIMSK = (1<<INT0); // Enable External Interrupt 0
	sei(); // Global Enable for interrupts, notice that the command here is cap sensitive
	
	while (1) // Wait here and do nothing until the interrupt occurs
	{
	}
}

ISR (INT0_vect) // ISR for External Interrupt 0
{
	PORTB =  0b11111011; // Set PinB 2 Low (REVERSE LOGIC HIGH), Turn PB2 LED on
	_delay_ms(1250);
	PORTB = 0xFF; // Set all PinB HIGH (REVERSE LOGIC LOW), Turn all LEDs off
}