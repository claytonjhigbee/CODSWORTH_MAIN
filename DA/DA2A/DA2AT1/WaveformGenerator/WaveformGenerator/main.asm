;
; WaveformGenerator.asm
;
; Created: 2/26/2019 8:13:42 PM
; Author : clayt
;

.ORG 0
	LDI R20, 0xFF ; Load Immediate into R20
	OUT DDRB, R20 ; Set all Pins of Port B as output only
	LDI R19, 4 ; Value to set clock prescalar to 256
	STS TCCR1B, R19 ; Set clock prescalar

START:
	LDI R20, 0x00 ; Used for resetting counter
	STS TCNT1H, R20 ; Set counter high byte to zero
	STS TCNT1L, R20 ; set counter low byte to zero 
	LDI R21, 0b00000100 ; Load immediate binary number into R21, to be used to set PORTB.2 HIGH
	OUT PORTB, R21 ; Set PORTB.2 HIGH
	RJMP DELAYHIGH0 ; Delay for 60% of total time

L1:
	LDI R21, 0b00000000 ; Load immediate binary number into R21, to be used to set PORTB.2 LOW
	LDI R29, 0 ; Load 0 into R29 to reset High byte of Clock
	LDI R28, 0 ; Load 0 into R28 to reset Low byte of Clock
	STS TCNT1H, R29 ; Load upper byte of Clock
	STS TCNT1L, R28 ; Load lower byte of Clock
	OUT PORTB, R21
	RJMP DELAYLOW0 ; Delay for 40% of total time

DELAYHIGH0:
	; Hold the Function HIGH for 60% of the overall time: 0s to 435ms
	LDS R29, TCNT1H ; Load upper byte of Clock
	LDS R28, TCNT1L ; Load lower byte of Clock
	CPI R28, 0x01 ; Compare lower byte to see if time was reached
	BRSH DELAYHIGH1 ; If lower byte has been reached, check upper byte
	RJMP DELAYHIGH0 ; Else continue to check lower byte

DELAYHIGH1:
	CPI R29, 0x6A ; Check if Upper byte has reached desired time
	BRLT DELAYHIGH0 ; If not check lower byte again
	RJMP L1 ; If upper byte has reached, then set PORTB.2 LOW

DELAYLOW0:
	; Hold the Function HIGH for 40% of the overall time: 435ms to 725ms
	LDS R29, TCNT1H ; Load upper byte of Clock
	LDS R28, TCNT1L ; Load lower byte of Clock

	CPI R28, 0xFF ; Compare lower byte to see if time was reached
	BRSH DELAYLOW1 ; If lower byte has been reached, check upper byte
	RJMP DELAYLOW0 ; Else continue to check lower byte

DELAYLOW1:
	CPI R29, 0x46 ; Check if Upper byte has reached desired time
	BRLT DELAYLOW0 ; If not check lower byte again
	RJMP START ; If upper byte has reached, then set PORTB.2 HIGH and start over





