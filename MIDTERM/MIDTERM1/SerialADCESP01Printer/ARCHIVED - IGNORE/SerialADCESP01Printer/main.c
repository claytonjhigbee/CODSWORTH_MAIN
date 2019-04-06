/*
 * SerialADCESP01Printer.c
 *
 * Created: 4/4/2019 8:30:31 PM
 * Author : clayt
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void read_adc(void);        // Function Declarations
void adc_init(void);
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
void ESP_send( char *data );
volatile unsigned int adc_temp;
volatile unsigned int tempF;
volatile unsigned int tempC;
volatile unsigned int tempCold;
char outs[20];
char sender[51];
int main(void)
{
	adc_init();             // Initialize the ADC
	USART_init(BAUD_PRESCALLER);  // Initialize the USART
	USART_tx_string("Connected!\r\n");    // we're alive!
	_delay_ms(125);         // wait a bit
	tempC = 0;
	tempF = 0;
	tempCold = 0;
	while(1)
	{
		read_adc();
		adc_temp = (adc_temp*500)/1023;
		tempC = adc_temp;
		tempF = (tempC*1.8)+32;
		if(1 >= tempC){
			tempC = tempC*(-1);
		}
		snprintf(outs,sizeof(outs),"%3d\r\n", tempF);  // print it
		USART_tx_string(outs);
		ESP_send(outs);
		_delay_ms(2000);        // wait a bit
	}
}
/* INIT ADC */
void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)|    // Reference Selection Bits
	(1<<REFS0)|    // AVcc - external cap at AREF
	(0<<ADLAR)|    // ADC Left Adjust Result
	(1<<MUX2)|     // Analog Channel Selection Bits
	(0<<MUX1)|     // ADC4 (PC4 PIN27)
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|    // ADC ENable
	(0<<ADSC)|     // ADC Start Conversion
	(0<<ADATE)|    // ADC Auto Trigger Enable
	(0<<ADIF)|     // ADC Interrupt Flag
	(0<<ADIE)|     // ADC Interrupt Enable
	(1<<ADPS2)|    // ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}
/* READ ADC PINS */
void read_adc(void)
{
	unsigned char i = 8;
	adc_temp = 0;
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp / 8;  // Average a few samples
}
/* INIT USART (RS-232)  */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);     // Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);    //asynchronous 8 N 1
}
/* SEND A STRING TO THE RS-232 */
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

void ESP_send( char *data )
{
	char sender[] = "AT\r\n";
	//char sender[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80 \r\n";
	USART_tx_string(sender);
	//_delay_ms(2000);
	//char sender2[] = "AT+CIPSEND=51 \r\n";
	//USART_tx_string(sender2);
	//_delay_ms(2000);
	//char sender3[] = "GET /update?key=DTJWRPL9C5AEE0YQ&field1=30 \r\n";
	//USART_tx_string(sender3);
	//_delay_ms(100);
	//char sender4[] = "\r\n";
	//USART_tx_string(sender4);
	//char sender5[] = "AT+CIPCLOSE";	//USART_tx_string(sender5);	//_delay_ms(2000);
}