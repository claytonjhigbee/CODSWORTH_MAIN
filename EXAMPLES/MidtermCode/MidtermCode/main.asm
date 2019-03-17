;
; MidtermCode.asm
;
; Created: 3/15/2019 8:36:14 AM
; Author : clayt
;


; Replace with your application code
.ORG 0

CHECKTIMER:
	IN R17, TCNT0
	CPI R17, 0x00
	BRNE CHECKTIMER
	RCALL SETREG

END: JMP END

SETREG
	LDI R18, 123

IN R17, SREG
EOR R17, 0b10000000 ; Set Global Enable
OUT SREG, R17
