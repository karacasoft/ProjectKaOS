;*******
;  stdio.in
;		-Input/Output routines
;	KaOS
;*******

%ifndef __STDIO_INC_INCLUDED__
%define __STDIO_INC_INCLUDED__

%define VIDMEM 0xB8000
; Mode 7 defaults
%define COLS 80
%define LINES 25
%define CHAR_ATTRIB 00000010b

;****Routine: Puts16()
;		-Prints a null terminated string
;	DS=>SI: 0 terminated string
;****
bits 16

Puts16:
	pusha

.Loop1:
	lodsb
	or al, al
	jz Puts16Done
	mov ah, 0x0e
	int 0x10
	jmp .Loop1

Puts16Done:
	popa
	ret

;**************************************
;	intToString16(int, returnPlace);
;		Converts integer to String if 
;	possible.
;	Stack usage: 4 bytes
;**************************************

decToStr db "0123456789"

const10 dw 10

intToString16:
	push bp
	xor bx, bx
	mov bp, sp
	mov ax, [bp+6]					;[LOCAL.0]
	mov si, [bp+4]					;[LOCAL.1]
.Loop1:
	xor dx, dx						; dx <- 0
	div WORD [const10]				; ax <- dx:ax / 10,  dx <- dx:ax % 10
	
	push bx
	mov bx, dx
	mov cl, BYTE [decToStr+bx]			; cl <- [decToStr + bx]
	pop bx
	mov BYTE [si], cl

	or ax, ax						; if(ax == 0)
	jz .Done						; jump to end

	push bx
.Loop2:
	dec bx							; bx--
	mov cl, BYTE [si+bx]
	mov BYTE [si+bx+1], cl 			; [si+bx+1] <- [si+bx]
	
	or bx, bx
	jnz .Loop2
	pop bx

	inc bx
	jmp .Loop1

.Done:
	mov sp, bp
	pop bp
	ret


;*********************Routine: Putch32()
;		-Prints a char.
;	Works on pMode too.
;*********************************************

_CurX db 0
_CurY db 2

bits 32

Putch32:
	pusha
	mov edi, VIDMEM

	xor eax, eax

	mov ecx, COLS*2
	mov al, BYTE [_CurY]
	mul ecx
	push eax

	mov al, BYTE [_CurX]
	mov cl, 2
	mul cl
	pop ecx
	add eax, ecx

	xor ecx, ecx
	add edi, eax

	cmp bl, 0x0A
	je .Putch32Row

	mov dl, bl
	mov dh, CHAR_ATTRIB
	mov WORD [edi], dx

	inc BYTE [_CurX]
	cmp BYTE [_CurX], COLS
	je .Putch32Row
	jmp .Putch32Done

.Putch32Row:
	mov BYTE [_CurX], 0
	inc BYTE [_CurY]

.Putch32Done:
	popa
	ret

;**************Routine: Puts32()
;		-Prints a null terminated string.
;*********************************************

Puts32:
	pusha

.Puts32Loop1:
	lodsb
	or al, al
	jz .Puts32Done
	mov bl, al
	call Putch32
	jmp .Puts32Loop1

.Puts32Done:
	popa
	ret




%endif ; __STDIO_INC_INCLUDED__