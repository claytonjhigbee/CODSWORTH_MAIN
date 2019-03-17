/*
 * DA2C_T2.c
 *
 * Created: 3/16/2019 11:23:22 AM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile int OVFCount; // Setup global overflow counter 
int main(void){
	OVFCount = 0; // Initialize global overflow counter
	DDRB = 0b100; // Configure PORTB.2 as an output
	TIMSK0 |= (1<<TOIE0); // Activate Timer 0 Interrupt Flag
	TCNT0 = 56; // Desired timer initial value to count up to 255
	sei(); // Activate global interrupts
	TCCR0B |= (1<< CS02)|(1<<CS00); // Set prescalar to 1024 and starts timer
	while(1){
	// Do nothing and wait for the interrupt to take place O.o	
	}	
}

// Interrupt C Code
ISR(TIMER0_OVF_vect) // Timer0 overflow interrupt sequence
{
OVFCount++; // Increment overflow flag counter
int count; // Initialize local counter
count = OVFCount; // Let local counter mimic global counter variable
// First delay period 
if(count < 27){
	TCNT0 = 0x4; // Reset timer0 for first delay
	TIFR0 = 0x01; // Clearing Timer1 overflow flag
}
// If first delay done, then set PinB.2 low
if(count == 27) {
	PORTB = 0x00;
}
// Second delay period
if(27 < count & count < 45) {
	TCNT0 = 0x4; // Reset timer0 for second delay
	TIFR0 = 0x01; // Clearing Timer1 overflow flag
}
// If second delay done, set PinB.2 low and start counter over 
if(count == 45) {
	PORTB = 0xFF; // Set PinB.2 Low
	OVFCount = 0; // Reset Counter
}
}
