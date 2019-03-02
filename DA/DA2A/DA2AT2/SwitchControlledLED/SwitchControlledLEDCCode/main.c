/*
 * SwitchControlledLEDCCode.c
 *
 * Created: 3/2/2019 8:25:24 AM
 * Author : clayt
 */ 

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
 	DDRB = 0xFF; // Set Port B as an output
	DDRC = 0x00; // Set Port C as an input
    PORTC = 0xFF; // Set Pull up resistors in Port C
	
    while (1) 
    {
		if(PINC == 0x7D)
		{
			PORTB = 0b00111100;
			_delay_ms(1250);
			PORTB = 0;
		}
		
    }
}

