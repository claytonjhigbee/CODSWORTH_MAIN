;
; LAB_1B.asm
;
; Created: 2/22/2019 9:35:24 AM
; Author : clayt
;


.EQU STARTADDS = 0x0200

.ORG 0
	;X SRAM Location: 0x0200 - Store all 99 numbers
	LDI R26, LOW(STARTADDS) ; Initialize XL Register
	LDI R27, HIGH(STARTADDS) ; Initialize XH Register
	; Y SRAM Location: 0x0400 - Store only numbers divisible by 3
	LDI R28, 0x00 ; Initialize YL Register
	LDI R29, 0x04 ; Initialize YH Register
	; Z SRAM Location: 0x0600 - Store only numbers not divisible by 3
	LDI R30, 0x00 ; Initialize ZL Register
	LDI R31, 0x06 ; Initialize ZH Register
	; Add the Total sum of numbers stored from 0x0400
	LDI R16, 0 ; Initialize Sum Low
	LDI R17, 0 ; Initialize Sum High
	; Add the Total sum of numbers stored from 0x0600
	LDI R18, 0 ; Initialize Sum Low 
	LDI R19, 0 ; Initialize Sum High
	; Counter of how many total numbers have been stored
	LDI R21, 99 ; 99 total numbers to be stored
	; 99 Numbers to be between 10 and 255
	LDI R20 , 11 ; First Number to be stored
START:
	ST X+, R20 ; Store the current number in the STARTADDS Location
	; Check if the number is divisible by 3 or not
	RJMP DIVIS3CHECK
MAIN:
	INC R20 ; Increment the Number to be counted
	DEC R21 ; Decrement Counter 
	CPI R21, 0 ; Compare to zero to see if counter is finished
	BREQ END ; If count is done, END program
	RJMP START

END: JMP END ; Infinite Loop, do nothing

DIVIS3CHECK:
	MOV R22, R20
LA1:
	SUBI R22, 3
	IN R23, 0x3F ; Get the entire SREG from IO Registers
	; Check if Z flag(bit 1) or C Flag(bit 0) are currently set
	MOV  R24, R23 ; Store SREG in R24 for inspection
	SUBI R24, 0b00110101 ; Check to see if not divisible by 3, If Carry bit is set after subtraction, then not divisible
	BREQ NOTDIVISIBLE
	MOV R24, R23 ; Store SREG in R24 for inspection
	SUBI R24, 0b00000010 ; Check to see if divisible by 3, If Zero bit is set after subtraction, then its divisible
	BREQ DIVISIBLE
	; If neither are met, then continue to subtract by 3
	RJMP LA1

DIVISIBLE:
	ST   Y+, R20 ; Store current value in Divisible List
	LDI R25, 0 ; For Convenience
	ADD R16, R20 ; Add to total sum LOW Byte
	ADC R17, R25 ; Add to total sum HIGH Byte
	RJMP MAIN

NOTDIVISIBLE:
	ST   Z+, R20 ; Store current value in Not Divisible List
	LDI R25, 0 ; For Convenience
	ADD R18, R20 ; Add to total sum LOW Byte
	ADC R19, R25 ; Add to total sum HIGH Byte
	RJMP MAIN
