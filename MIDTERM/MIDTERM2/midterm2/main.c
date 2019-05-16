/*
APDSESPSerialWrite.c
Created: 5/11/2019 3:37:39 PM
Author : higbee
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c_master.h"
#include "uart.h"
#include "apds.h"
#define BAUD 9600
#define BAUD_PRESCALLER F_CPU/16/BAUD-1 // Baud rate prescaller
#define BRGVAL (F_CPU/16/BAUD) - 1

#ifndef APDS_H
#define APDS_H

#include <avr/io.h>
#include "i2c_master.h"
#include "apds.h"

#define APDS_WRITE	(0x39 << 1) | 0
#define APDS_READ	(0x39 << 1) | 1


/* APDS-9960 I2C address */
#define APDS9960_I2C_ADDR       0x39

/* Error code for returned values */
#define ERROR                   0xFF

/* Acceptable device IDs */
#define APDS9960_ID_1           0xAB
#define APDS9960_ID_2           0x9C

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9960 register addresses */
#define APDS9960_ENABLE         0x80
#define APDS9960_ATIME          0x81
#define APDS9960_WTIME          0x83
#define APDS9960_PERS           0x8C
#define APDS9960_CONFIG1        0x8D
#define APDS9960_PPULSE         0x8E
#define APDS9960_CONFIG2        0x90
#define APDS9960_ID             0x92
#define APDS9960_WDATAL         0x94
#define APDS9960_WDATAH         0x95
#define APDS9960_RDATAL         0x96
#define APDS9960_RDATAH         0x97
#define APDS9960_GDATAL         0x98
#define APDS9960_GDATAH         0x99
#define APDS9960_BDATAL         0x9A
#define APDS9960_BDATAH         0x9B
#define APDS9960_POFFSET_UR     0x9D
#define APDS9960_POFFSET_DL     0x9E
#define APDS9960_CONFIG3        0x9F

/* Bit fields */
#define APDS9960_PON            0b00000001
#define APDS9960_AEN            0b00000010
#define APDS9960_PEN            0b00000100
#define APDS9960_WEN            0b00001000
#define APSD9960_AIEN           0b00010000
#define APDS9960_PIEN           0b00100000
#define APDS9960_GEN            0b01000000
#define APDS9960_GVALID         0b00000001

/* On/Off definitions */
#define OFF                     0
#define ON                      1

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
/*#define PROXIMITY               2*/
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* LED Boost values */
#define LED_BOOST_100           0
#define LED_BOOST_150           1
#define LED_BOOST_200           2
#define LED_BOOST_300           3

/* Default values */
#define DEFAULT_ATIME           219     // 103ms
#define DEFAULT_WTIME           246     // 27ms
#define DEFAULT_PROX_PPULSE     0x87    // 16us, 8 pulses
#define DEFAULT_POFFSET_UR      0       // 0 offset
#define DEFAULT_POFFSET_DL      0       // 0 offset
#define DEFAULT_CONFIG1         0x60    // No 12x wait (WTIME) factor
#define DEFAULT_LDRIVE          LED_DRIVE_100MA
#define DEFAULT_PGAIN           PGAIN_4X
#define DEFAULT_AGAIN           AGAIN_4X
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x11    // 2 consecutive prox or ALS for int.
#define DEFAULT_CONFIG2         0x01    // No saturation interrupts or LED boost
#define DEFAULT_CONFIG3         0       // Enable all photodiodes, no SAI
#define DEFAULT_GLDRIVE         LED_DRIVE_100MA
#define DEFAULT_GWTIME          GWTIME_2_8MS

void apds_init();
void readColor();

#endif


FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL , _FDEV_SETUP_WRITE);
char results[256];
char outs[256]; // String array used for sending USART commands
volatile char received_data; // String array used for receiving USART communication

void init_UART();
int uart_putchar( char c, FILE *stream);
void USART_init( unsigned int ubrr ); // Initialize USART communication
void USART_tx_string(char *data); //Print String USART


int main(void){
	uint16_t red = 0, green = 0, blue = 0, white = 0;
	
	i2c_init();
	USART_init(BAUD_PRESCALLER); // Initialize the USART (RS232 interface)
	_delay_ms(500); // Delay to allow hardware to initialize
	stdout = &str_uart;
	apds_init();	
    while (1) {
		
		/* Initialize AT commands */
		//AT connect check
		unsigned char AT[] = "AT\r\n";
		//Set device mode, 1 = Station mode
		unsigned char AT_CWMODE[] = "AT+CWMODE=1\r\n";
		//Perform Wifi connection, provide SSID and Password
		unsigned char AT_CWJAP[] = "AT+CWJAP=\"Higbee_Pixel\",\"Higbee19\"\r\n";
		//Set device for single IP Address Mode
		unsigned char AT_CIPMUX[] = "AT+CIPMUX=0\r\n";
		// Start TCP connection to Thingspeak.com at port 80
		unsigned char AT_CIPSTART[] = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
		// Give upcoming string length
		unsigned char AT_CIPSEND[] = "AT+CIPSEND=130\r\n";
		
		// Send commands listed from above
		_delay_ms(200);
		USART_tx_string(AT);
		_delay_ms(5000);
		USART_tx_string(AT_CWMODE);
		_delay_ms(5000);
		USART_tx_string(AT_CWJAP);
		_delay_ms(15000);
		USART_tx_string(AT_CIPMUX);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSTART);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSEND);
		_delay_ms(5000);
		
		// Read the next colors
		readColor(&red, &green, &blue, &white);
		
		// Print Data to Thingspeak using provided link, website channel key, and field location
		// Send Red
		snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=IKAXX2LP9LJMS0IU&field1=%3d\r\n", red);
		USART_tx_string(outs);//send data
		_delay_ms(15000);
		
		// Send Green
		USART_tx_string(AT_CIPMUX);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSTART);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSEND);
		_delay_ms(5000);
		snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=IKAXX2LP9LJMS0IU&field2=%3d\r\n", green);
		USART_tx_string(outs);//send data
		_delay_ms(15000);
		
		// Send Green
		USART_tx_string(AT_CIPMUX);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSTART);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSEND);
		_delay_ms(5000);
		snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=IKAXX2LP9LJMS0IU&field3=%3d\r\n", blue);
		USART_tx_string(outs);//send data
		_delay_ms(15000);
		
		// Send White
		USART_tx_string(AT_CIPMUX);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSTART);
		_delay_ms(10000);
		USART_tx_string(AT_CIPSEND);
		_delay_ms(5000);
		snprintf(outs,sizeof(outs),"GET https://api.thingspeak.com/update?api_key=IKAXX2LP9LJMS0IU&field4=%3d\r\n", white);
		USART_tx_string(outs);//send data
		_delay_ms(15000);
    }
}

void apds_init(){
	uint8_t setup;
	i2c_readReg(APDS_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	i2c_writeReg(APDS_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	i2c_writeReg(APDS_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	i2c_writeReg(APDS_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	i2c_writeReg(APDS_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	i2c_writeReg(APDS_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	i2c_writeReg(APDS_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	i2c_writeReg(APDS_WRITE, APDS9960_CONFIG3, &setup, 1);
}

void readColor(uint16_t *red, uint16_t *green, uint16_t *blue, uint16_t *white){
	uint8_t whitel, whiteh;
	uint8_t redl, redh;
	uint8_t greenl, greenh;
	uint8_t bluel, blueh;
	i2c_readReg(APDS_WRITE, 0x94, &whitel, 1);
	i2c_readReg(APDS_WRITE, 0x95, &whiteh, 1);
	i2c_readReg(APDS_WRITE, APDS9960_RDATAL, &redl, 1);
	i2c_readReg(APDS_WRITE, APDS9960_RDATAH, &redh, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAL, &greenl, 1);
	i2c_readReg(APDS_WRITE, APDS9960_GDATAH, &greenh, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAL, &bluel, 1);
	i2c_readReg(APDS_WRITE, APDS9960_BDATAH, &blueh, 1);
	*white = whiteh << 8 | whiteh;
	*red = redh << 8 | redl;
	*green = greenh << 8 | greenl;
	*blue = blueh << 8 | bluel;
}

void init_UART(void){
	//Set baud rate
	uint16_t baud_rate = BRGVAL;
	UBRR0H = baud_rate >> 8;
	UBRR0L = baud_rate & 0xFF;
	
	//Enable receiver and transmitter
	UCSR0B = ( 1 <<RXEN0)|( 1 <<TXEN0);
	
	// Set frame format: 8data, 1stop bit
	UCSR0C = (3 <<UCSZ00);
}

int uart_putchar(char c, FILE *stream){
	//wait until buffer empty
	while ( !( UCSR0A & ( 1 <<UDRE0)) );
	
	//Put data into buffer
	UDR0 = c;
	return 0;
}

/* Initialize USART communication (RS-232) */
void USART_init( unsigned int ubrr ) {
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0)| ( 1 << RXCIE0); // Enable receiver, transmitter & RX interrupt
	UCSR0C |= (1<<UCSZ01) | (1 << UCSZ00);
}

/* USART string sender (RS-232) */
void USART_tx_string( char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}
