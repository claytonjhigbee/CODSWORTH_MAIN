/*
 * SwitchControlledLEDCCode.c
 *
 * Created: 3/2/2019 8:25:24 AM
 * Author : clayt
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
 	DDRB = 0xFF; // Set Port B as an output
	DDRC = 0x00; // Set Port C as an input
    PORTC = 0xFF; // Set Pull up resistors in Port C
	
    while (1) 
    {
		PORTB = 0b00111100; // Set LED Port B to Low (REVERSE LOGIC)
		if(PINC == 0x7D) ; // If switch push is detected, then set LEDs
		{
			PORTB = 0b00000000; // Set LED Port B to High (REVERSE LOGIC)
			_delay_ms(1250); // Delay for 1.25 seconds
		}
		
    }
}

