;
; AssemblerApplication1.asm
;
; Created: 2/12/2019 3:22:46 PM
; Author : clayt
;


;Comment the following code and determine the final values in Registers X,Y,Z and memory 0x160?

QDELAY:
    LDI R20, 16

D0:
 LDI R21, 200

D1: 
	LDI R22, 250
	;3 clock cycles here
D2:  
	NOP
	NOP
	DEC R22
	BRNE D2
	DEC R21
	BRNE D1
	DEC R20
	BRNE D0
	RET
