/*
 * WaveformGeneratorCcode.c
 *
 * Created: 3/1/2019 9:24:05 AM
 * Author : clayt
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000UL

int main(void)
{
    DDRB = 0xFF; // Configure Port B as an output
    while (1) 
    {
		PORTB |= (1<<PB2);
		_delay_ms(435);
		PORTB &= ~(1<<PB2);
		_delay_ms(290);
    }
}

