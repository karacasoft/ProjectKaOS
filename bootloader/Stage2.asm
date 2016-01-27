bits 16

org 0x0

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
	mov ax, 0x9C0
	mov ds, ax
	mov es, ax

	mov ax, 0x9000
	mov ss, ax
	mov sp, 0xFFFF
	sti

	mov si, MsgStarting		; Display Starting message
	call Puts16

	cli						; Stop for now
	hlt