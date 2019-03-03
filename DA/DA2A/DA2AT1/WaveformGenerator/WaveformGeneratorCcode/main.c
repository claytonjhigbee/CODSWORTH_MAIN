/*
 * WaveformGeneratorCcode.c
 *
 * Created: 3/1/2019 9:24:05 AM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB = 0xFF; // Configure Port B as an output
    while (1) 
    {
		PORTB |= (1<<PB2); // Set Port B High
		_delay_ms(435); // Delay 435ms
		PORTB &= ~(1<<PB2); // Set Port B Low
		_delay_ms(290); // Delay 290ms
    } // Repeat
}

