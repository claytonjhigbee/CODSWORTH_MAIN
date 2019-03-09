;
; SwitchControlledLED.asm
;
; Created: 3/1/2019 9:50:32 AM
; Author : clayt
;

 .ORG 0
	LDI R22, 0 ; Load immediate into R22
	LDI R16, 0xFF ; Load immediate into R16
	OUT DDRB, R16 ; Set DDRB as output
	OUT DDRC, R22 ; Set DDRC as input
	OUT PORTC, R16 ; Set Port C pull up resistors
	LDI R17, 0 ; Register needed to toggle the LED
	OUT PORTB, R16 ; Set Port B and related Pins HIGH to set LEDs to LOW
	LDI R20, 5 ; Set Clock prescalar to 1024
	STS TCCR1B, R20 ; Input Clock prescalar
	
CHECK:
	IN R26, PINC ; Check current PinC input
	COM R26 ; Take compliment of R26
	ANDI R26, 0x04 ; Logical AND with immediate
	CPI R26, 0x04 ; Compare with immediate
	BRNE CHECK ; Check to see if PinC.2 has been pressed, if not, CHECK again

BEGIN: ; If switched pressed, begin delay
	LDI R25, 0x00 ; Reset Timer
	STS TCNT1H, R25 ; Reset Timer HIGH
	STS TCNT1L, R25 ; Reset Timer LOW
	OUT PORTB, R22 ; Set LED Port B to Low (REVERSE LOGIC)

DELAY0:
	LDS R29, TCNT1H ; Loading upper byte of Timer to R29
	LDS R28, TCNT1L ; Loading lower byte of Timer to R28
	CPI R28, 0x4A ; Compare with Immediate 
	BRSH DELAY1 ; Branch if same or higher to DELAY1 label
	RJMP DELAY0 ; Else contine to check on Timer Lower byte

DELAY1:
	CPI R29, 0x4C ; Compare to immediate
	BRSH DONE ; If timer is met, Branch to DONE label
	RJMP DELAY0 ; Else check Timer low byte again

DONE:
	; Reset HIGH and LOW bytes for Timer
	LDI R25, 0x00
	STS TCNT1H, R25
	STS TCNT1L, R25
	OUT PORTB, R16 ; Set LED Port B to High (REVERSE LOGIC)
	RJMP CHECK ; Return to CHECK label 

