/*
 * DA2C_T22.c
 *
 * Created: 3/16/2019 12:38:34 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile int OVFCount;
int main(void){
	OVFCount = 0; // initialize Overflow counter
	DDRB = 0xFF; // Configure PORTB as an output
	TIMSK0 |= (1<<TOIE0); // Activate Timer 0 Interrupt Flag
	TCNT0 = 56; // Desired timer initial value to count up to 255
	sei(); // Activate global interrupts
	TCCR0B |= (1<< CS02)|(1<<CS00); // Set prescalar to 1024 and starts timer
	
	while(1){
		PORTB = 0b11111111; // Set LED PortB.2 to Low (REVERSE LOGIC)
		if(PINC == 0x7D) // If switch push is detected, then set LEDs
		{
			OVFCount = 0; // initialize Overflow value for this button press
			TCNT0 = 0x2; // Reset timer0 for delay
			PORTB = 0xFB; // Set LED Port B to High (REVERSE LOGIC)
			while(OVFCount < 77)
			{
				// Do nothing O.o, waiting for entire overflow to occur
			}
		}
	}
}

// Interrupt C Code
ISR(TIMER0_OVF_vect) // Timer0 overflow interrupt sequence
{
	OVFCount++; // Increment overflow flag counter
	int count;
	count = OVFCount;
	if(count < 77){
		TCNT0 = 0x2; // Reset timer0 for delay
		TIFR0 = 0x01; // Clearing Timer1 overflow flag
	}
}
