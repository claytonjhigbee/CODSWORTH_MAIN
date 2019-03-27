/*
 * SerialReaderProgram.c
 *
 * Created: 3/26/2019 3:08:03 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

// Function Declarations
volatile int OVFCount; // initialize global overflow counter variable
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
char outs[25];
int mynumber;
char str[] = "SUP";
char empty[] = " ";
float hernumber = 27.3653442500000003;

int main(void)
{
	OVFCount = 0; // initialize Overflow counter
	OCR0A = 220; // Load compare Reg Value
	TCCR0A |= (1<<WGM01); // Set to CTC Mode
	TIMSK0 |= (1<<OCIE2A); // Set interrupt on compare match
	TCCR0B = 0b101; // Set prescalar to 1024 and starts PWM
	sei(); // Enable Global Interrupts
	USART_init(BAUD_PRESCALLER);  // Initialize the USART
	USART_tx_string("Connected!\r\n");    // Send initial connection print for verification
	
	while(1)
	{
			// Get three desired values
			// First Desired Value, Print a string
			USART_tx_string(str); // Print SUP
			USART_tx_string(empty); // Print empty space
			
			// Second Desire value, Print a random integer
			mynumber = rand();
			snprintf(outs,sizeof(outs),"%3d\r\n", mynumber);  // print it
			USART_tx_string(outs); // Print random integer number
			USART_tx_string(empty); // Print empty space
			
			// Third desired Value, Print a floating point number
			sprintf(outs, "%g", hernumber);
			//snprintf(outs,sizeof(outs),"%3d\r\n", hernumber);  // print it
			USART_tx_string(outs); // Print random integer number
			USART_tx_string(empty); // Print empty space
			OVFCount = 0; // initialize Overflow value for this button press
			TCNT0 = 0x0; // Reset Timer for Delay
			while(OVFCount < 71)
			{
				// Do nothing O.o, waiting for entire overflow count to occur
			}
	}
}
// Continue to increment counter until it reaches 77, then turn LED off
ISR(TIMER0_COMPA_vect){
	OVFCount++; // Increment overflow flag counter
}

/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}

/* SEND A STRING THROUGH RS-232  BY FTDI BOARD*/
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

