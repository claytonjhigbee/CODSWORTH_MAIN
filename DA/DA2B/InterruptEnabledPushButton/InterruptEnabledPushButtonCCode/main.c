/*
 * InterruptEnabledPushButtonCCode.c
 *
 * Created: 3/6/2019 9:23:38 PM
 * Author : clayt
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

int main()
{
	DDRB = 1<<5; // Set PB5 as an output
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
	PORTB ^= (1<<5); // XOR to toggle the value in PORTB.5
}