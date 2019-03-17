;
; SwitchControlledLED.asm
;
; Created: 3/1/2019 9:50:32 AM
; Author : clayt
;

.ORG 0
	LDI R19, 4 ; Value to set clock prescalar to 64
	STS TCCR1B, R19 ; Set clock prescalar
	LDI R20, 0xFF ; Set direction register as output
	LDI R21, 0x00 ; Set direction register as input
	OUT DDRB, R20 ; Set Port B as an output
	OUT DDRC, R21 ; Set Port C as input
	OUT PORTC, R21 ; Set Pull-Up Resistors to ON It wont let me do this part?

START:
	IN R29, PINC ; Look for an input in PIN C Register
	CPI R29, 0b00000010 ; Compare to see if switch at PC1 button is pushed
	BRNE START ; Keep going to start and checking the switch location
	RJMP DELAYSTART ; Else, delay for 1.25s

DELAYSTART:
	LDI R20, 0x00 ; Used for resetting counter
	STS TCNT1H, R20 ; Set counter high byte to zero
	STS TCNT1L, R20 ; set counter low byte to zero 
	LDI R18, 0b00111100 ; Used to set all the LEDs High in Port B
	OUT PORTB, R18 ; Set LEDs high in Port B
DELAY0:
	; Hold the Function HIGH for 60% of the overall time: 0s to 435ms
	LDS R29, TCNT1H ; Load upper byte of Clock
	LDS R28, TCNT1L ; Load lower byte of Clock
	CPI R28, 0xC9 ; Compare lower byte to see if time was reached
	BRSH DELAY1 ; If lower byte has been reached, check upper byte
	RJMP DELAY0 ; Else continue to check lower byte

DELAY1:
	CPI R29, 0x99 ; Check if Upper byte has reached desired time
	BRGE DELAY0 ; If not check lower byte again
	; If upper byte has reached, then set PORTB.2 LOW
	LDI R18, 0x00 ; Used to set all the LEDs Low in Port B
	OUT PORTB, R18 ; Set LEDs low in Port B
	RJMP START 