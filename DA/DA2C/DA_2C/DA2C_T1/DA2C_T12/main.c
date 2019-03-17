/*
 * DA2C_T12.c
 *
 * Created: 3/16/2019 9:25:11 AM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
 	uint8_t OVFCount = 0; // Setup Placeholder for overflow counter
	DDRB = 0xFF; // Set Port B as an output
	DDRC = 0x00; // Set Port C as an input
    PORTC = 0xFF; // Set Pull up resistors in Port C
	TCNT0 = 0x2; // Reset timer0 for delay
	TCCR0B = (1<<CS02)|(1<<CS00); // Setting prescaler value for timer0
	while (1)
	{
	   OVFCount = 0; // Retain Overflow count to zero if the button has not been pushed
	   PORTB = 0b00111100; // Set LED Port B to Low (REVERSE LOGIC)
	   if(PINC == 0x7D) // If switch push is detected, then set LEDs and begin timer delay sequence
	   {
		   TCNT0 = 0x3; // Reset timer0 for delay
		   PORTB = 0b11111011; // Set LED Port B to High (REVERSE LOGIC)
		   while(OVFCount < 77){
			   while((TIFR0 & 0x01) == 0){
				   // When the flag is not set, do nothing O.o and wait
				   } 
			   OVFCount++; // When flag is set, increment overflow flag counter
			   if(OVFCount < 77){
				   TCNT0 = 0x3; // Reset timer0 for delay
				   TIFR0 = 0x01; // Clearing Timer1 overflow flag
			   }
		   }
	   }
    }
}

