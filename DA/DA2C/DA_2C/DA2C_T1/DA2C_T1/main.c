/*
 * DA2C_T1.c
 *
 * Created: 3/15/2019 10:24:15 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#define LED 2
// from vanki
int main() {
	uint8_t OVFCount = 0; // Setup Placeholder for overflow counter
	DDRB |= 1 << LED; // Configure PORTB.2 as an output
	TCNT0 = 0x4; // Reset timer0 for first delay
	TCCR0B = (1<<CS02)|(1<<CS00); // Setting prescaler value for timer0 to 1024 and start timer
	
	while (1){
		while((TIFR0 & 0x01) == 0){} // When the flag is not set, do nothing and wait
		OVFCount++; // Increment overflow flag counter
		// Count up during the first delay period	
		if(OVFCount < 27){
		TCNT0 = 0x4; // Reset timer0 for first delay
		TIFR0 = 0x01; // Clearing Timer1 overflow flag	
		}
		// If count is met, set PinB.2 low
		if(OVFCount == 27) {
			PORTB = 0x00; // Set PinB.2 Low 
		}
		// Count up for next delay
		if(27 < OVFCount < 45) {
			TCNT0 = 0x4; // Reset timer0 for second delay
			TIFR0 = 0x01; // Clearing Timer1 overflow flag	
		}
		// If count met, set high and start delay period over 
		if(OVFCount == 45) {
			PORTB = 0xFF; // Set PinB.2 high
			OVFCount = 0; // Start delay counter over
		}
	}
}