/*
 * DA2C_T32.c
 *
 * Created: 3/16/2019 3:22:39 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
volatile int OVFCount; // initialize global overflow counter variable

int main(void)
{
	OVFCount = 0; // initialize Overflow counter
	DDRB = 0xFF; // Configure Port B as an output
	DDRC = 0x00; // Configure Port C as an Input
	PORTC = 0xFF; // Turn Port C Pull up resistors
	OCR0A = 253; // Load compare Reg Value
	TCCR0A |= (1<<WGM01); // Set to CTC Mode
	TIMSK0 |= (1<<OCIE2A); // Set interrupt on compare match
	TCCR0B = 0b101; // Set prescalar to 1024 and starts PWM
	sei(); // Enable Global Interrupts
	
	while(1)
	{
		PORTB = 0b11111111; // Set LED PortB.2 to Low (REVERSE LOGIC)
		if(PINC == 0x7D) // If switch push is detected, then set LEDs
		{
			OVFCount = 0; // initialize Overflow value for this button press
			TCNT0 = 0x0; // Reset Timer for Delay
			PORTB = 0xFB; // Set LED Port B to High (REVERSE LOGIC)
			while(OVFCount < 77)
			{
				// Do nothing O.o, waiting for entire overflow count to occur
			}
		}
	}
}
// Continue to increment counter until it reaches 77, then turn LED off
ISR(TIMER0_COMPA_vect){
	OVFCount++; // Increment overflow flag counter
}

