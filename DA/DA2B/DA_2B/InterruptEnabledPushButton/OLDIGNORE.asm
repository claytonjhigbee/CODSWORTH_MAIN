;
; InterruptEnabledPushButton.asm
;
; Created: 3/6/2019 9:22:47 PM
; Author : clayt
;

; Task 1: Connect a switch to PORTD.2 (active high - turn on the pull up transistor) to poll for an event to turn on the led at PORTB.2 for 1.250 sec after the event.
.ORG 0; LOCATION FOR RESET
JMP START

.ORG 0x02 ; Location for external interrupt stack location
JMP EXO_ISR

START:
	; Intialize Stackpointer
	LDI R20, HIGH(RAMEND)
	OUT SPH, R20 ; LOAD HIGH BYTE OF STACKPOINTER TO INITIALIZE
	LDI R20, LOW(RAMEND) 
	OUT SPL, R20

	;Initialize I/O Pins
	LDI R22, 0 ; Load immediate 0 into R22
	LDI R16, 0xFF ; Load immediate 255 into R16
	OUT DDRB, R16 ; Set DDRB as output
	OUT DDRD, R22 ; Set DDRD as input
	SBI PORTD, 2 ; Set PortD.2 Pull up resistors for input for interrupt
	OUT PORTB, R16 ; Set Port B and related Pins HIGH to set LEDs to LOW (Reverse Logic)

	;Initialize Interrupt
	LDI R20, 0x2 ; CONFIGURE INTERRUPT TO OCCUR INT0 FOR FALLING EDGE TRIGGER
	STS EICRA, R20 ; LOAD CONFIGURE INTO REGISTER
	LDI R20, 1<< INT0 ; ENABLE EXTERNAL INTERRUPT INT0(PD2)
	OUT EIMSK, R20 ; LOAD ENABLE INTO REGISTER
	SEI ; GLOBAL INTERRUPT ENABLE COMMAND

	; Initialize Timers
	LDI R20, 5 ; Set Clock prescalar to 1024
	STS TCCR1B, R20 ; Input Clock prescalar

HERE: JMP HERE ; LOOP HERE UNTIL THE INTERRUPT IS OCCURS

; INTERRUPT SUBROUTINE ---------------------------------------
EXO_ISR:
	LDI R25, 0x00 ; Reset Timer
	STS TCNT1H, R25 ; Reset Timer HIGH
	STS TCNT1L, R25 ; Reset Timer LOW
	LDI R19, 0b11111011 ; Load immediate into R19 to Set PortB2 LED ON
	OUT PORTB, R19 ; Set LED Port B to Low (REVERSE LOGIC)

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
	OUT PORTB, R16 ; Set LED Port B to High (REVERSE LOGIC) to turn LED off
	RETI ; RETURN FROM INTERRUPT

