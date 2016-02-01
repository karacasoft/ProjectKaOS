bits 16

;**************************************
;	puts16(String);
;		Prints a null terminated String
;	Stack usage: 2 bytes
;**************************************

puts16:
	push si
	push ax
	mov si, [sp + 6]		;[LOCAL.0]
.Loop1:
	lodsb
	or al, al
	jz .Done
	mov ah, 0x0e
	int 0x10
	jmp .Loop1

.Done:
	pop ax
	pop si
	ret

;**************************************
;	intToString(int, returnPlace);
;		Converts integer to String if 
;	possible.
;	Stack usage: 2 bytes
;**************************************

decToStr: db "0123456789"

intToString16:
	push si
	push ax
	push dx
	mov ax, [sp + 10]		;[LOCAL.0]
.Loop1:
	xor dx, dx
	;TODO print int on screen


.Done
	pop dx
	pop ax
	pop si
	ret