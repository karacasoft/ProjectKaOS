bits 16

org 0x00

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
TestNumber dw 125

StrTest db "s   ", 0x0D, 0x0A, 0x00

TestComplete db "Test Completed", 0x0D, 0x0A, 0x00

;=====================================
;	Stage 2 Entry Point
;=====================================

main:
	
	cli
	push cs
	pop ds

	mov ax, 0x9000
 	mov ss, ax
 	mov sp, 0xFFFF
	sti

	mov si, MsgStarting		; Display Starting message
	call Puts16

	push WORD [TestNumber]
	push StrTest
	call intToString16
	add sp, 4

	mov si, StrTest
	call Puts16

	mov si, TestComplete
	call Puts16

	cli						; Stop for now
	hlt