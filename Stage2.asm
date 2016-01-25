bits 16

org 0x0
;There's a problem here. (Segment registers and some other shit)
jmp main

;=====================================
;	Preprocessor Directives
;=====================================

%include "stdio.inc"
%include "gdt.inc"

;=====================================
;	Data Block
;=====================================

MsgStarting db "Starting Boot Stage 2", 0x0D, 0x0A, 0x00

;=====================================
;	Stage 2 Entry Point
;=====================================

main:
	
	cli
	push cs
	pop ds
	sti
	

	mov si, MsgStarting		; Display Starting message
	call Puts16

	cli						; Stop for now
	hlt