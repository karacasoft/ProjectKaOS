bits 16

;**************************************
;	puts16(String);
;		Prints a null terminated String
;	Stack usage: 2 bytes
;**************************************

puts16:
	push bp
	push si
	push ax
	mov bp, sp
	mov si, [bp+8]					;[LOCAL.0]
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
	pop bp
	ret

;**************************************
;	intToString(int, returnPlace);
;		Converts integer to String if 
;	possible.
;	Stack usage: 4 bytes
;**************************************

decToStr: db "0123456789"

const10: dw 10

intToString16:
	push bp
	xor bx, bx
	mov bp, sp
	mov ax, [bp+6]					;[LOCAL.0]
	mov si, [bp+4]					;[LOCAL.1]
.Loop1:
	xor dx, dx						; dx <- 0
	div WORD [const10]				; ax <- dx:ax / 10,  dx <- dx:ax % 10
	or ax, ax						; if(ax == 0)
	jz .Done						; jump to end

	push bx
	mov bx, dx
	mov cl, [decToStr+bx]			; bl <- [decToStr + dx]
	pop bx
	mov [si+bx], cl

	inc bx
	jmp .Loop1

.Done:
	mov sp, bp
	pop bp
	ret