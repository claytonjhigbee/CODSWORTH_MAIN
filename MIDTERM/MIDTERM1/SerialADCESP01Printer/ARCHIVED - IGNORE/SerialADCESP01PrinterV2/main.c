/*
 * SerialADCESP01PrinterV2.c
 *
 * Created: 4/5/2019 8:06:57 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>

void read_adc(void);  //Read ADC
void adc_init(void);  //initialize ADC
void USART_init( unsigned int ubrr ); //initialize USART
void USART_tx_string(char *data); //Print String USART
volatile unsigned int adc_temp;
char outs[256]; //array

volatile char received_data;

void usart_send(unsigned char ascii) //send data to USART
{
	while(!(UCSR0A & (1<<UDRE0 )));
	UDR0 = ascii;
	
}

unsigned char usart_receive(void) //received data
{
	while(!(UCSR0A & (1<< RXC0)));
	return received_data;
}

int main(void) {

	adc_init(); // Initialize the ADC (Analog / Digital Converter)
	USART_init(MYUBRR); // Initialize the USART (RS232 interface)
	_delay_ms(500); // wait a bit
	sei(); //interrupt

	
	while(1)
	{

	}
}


void adc_init(void) //initialize ADC
{
	
	ADMUX = (0<<REFS1)| // Reference Selection Bits

	(1<<REFS0)| // AVcc - external cap at AREF
	(0<<ADLAR)| // ADC Left Adjust Result
	(0<<MUX2)| // ANalog Channel Selection Bits
	(1<<MUX1)| // ADC2 (PC2 PIN25)
	(0<<MUX0);

	ADCSRA = (1<<ADEN)| // ADC ENable

	(0<<ADSC)| // ADC Start Conversion
	(0<<ADATE)| // ADC Auto Trigger Enable
	(0<<ADIF)| // ADC Interrupt Flag
	(0<<ADIE)| // ADC Interrupt Enable
	(1<<ADPS2)| // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);

	// Timer/Counter1 Interrupt Mask Register

	TIMSK1 |= (1<<TOIE1); // enable overflow interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); // native clock
	TCNT1 = 49911; //((16MHz/1024)*1)-1 = 15624

}


void read_adc(void) {
	unsigned char i =4;
	adc_temp = 0; //initialize
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 8; // Average a few samples

}

/* INIT USART (RS-232) */
void USART_init( unsigned int ubrr ) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0)| ( 1 << RXCIE0); // Enable receiver, transmitter & RX interrupt
	UCSR0C |= (1<<UCSZ01) | (1 << UCSZ00);
}

void USART_tx_string( char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}


ISR(TIMER1_OVF_vect) //timer overflow interrupt to delay for 1 second
{
	char TEMP[256];
	unsigned char AT[] = "AT\r\n"; //AT Commands
	unsigned char CWMODE[] = "AT+CWMODE=1\r\n"; //Set MODE
	unsigned char CWJAP[] = "AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n"; //MUST CHANGE WIFI AND PASSWORD
	unsigned char CIPMUX[] = "AT+CIPMUX=0\r\n";
	unsigned char CIPSTART[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	unsigned char CIPSEND[] = "AT+CIPSEND=100\r\n";

	_delay_ms(200);
	USART_tx_string(AT); //send commands
	
	_delay_ms(5000);
	USART_tx_string(CWMODE); //set mode
	
	_delay_ms(5000);
	USART_tx_string(CWJAP); //connect to Wifi
	
	_delay_ms(15000);
	USART_tx_string(CIPMUX); //select MUX
	
	_delay_ms(10000);
	USART_tx_string(CIPSTART);//connect TCP
	
	_delay_ms(10000);
	USART_tx_string(CIPSEND);//send size
	
	_delay_ms(5000);
	
	
	PORTC^=(1<<5);
	read_adc(); //read ADC
	snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=DTJWRPL9C5AEE0YQ&field1=%3d\r\n", adc_temp);// print it
	USART_tx_string(outs);//send data
	_delay_ms(10000);
	TCNT1 = 49911; //reset
	
}
