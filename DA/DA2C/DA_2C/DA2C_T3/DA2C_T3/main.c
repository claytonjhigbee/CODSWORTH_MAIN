/*
 * DA2C_T3.c
 *
 * Created: 3/16/2019 1:11:32 PM
 * Author : clayt
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
volatile int OVFCount; // Initialize global overflow counter variable

int main(void)
{
    OVFCount = 0; // initialize Overflow counter
	DDRB = 0xFF; // Configure Port B as an output
	OCR0A = 252; // Load compare register 0A Value
	TCCR0A |= (1<<WGM01); // Set to CTC Mode
	TIMSK0 |= (1<<OCIE2A); // Set interrupt on compare match
	TCCR0B = 0b101; // Set prescalar to 1024
	sei(); // Enable Global Interrupts
	
	while(1)
	{
	// Do nothing O.o
	}
}
ISR(TIMER0_COMPA_vect){
	OVFCount++; // Increment global overflow flag counter
	int count; // Initialize local overflow counter variable 
	count = OVFCount; // let local counter mimic global overflow counter
	// If counter reaches 27, set PinB.2 low and continue counting
	if(count == 27) {
		PORTB = 0x00; // Set PinB.2 low
	}
	// If counter reaches 45, set PinB.2 high and restart counter
	if(count == 45) {
		PORTB = 0xFF; // Set PinB.2 high
		OVFCount = 0; // Restart Overflow counter 
	}
}

