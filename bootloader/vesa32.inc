;****************************************
;	32-bit VESA routines
;****************************************
bits 32

ConstXRes 	dd 1024*4

;****************************************
;	ebx -> 32-bit color
; 	esp + 4 -> y
;	esp + 6 -> x
;****************************************
;not working
VesaPutPixel:
	xor 	eax, eax
	mov 	ax, [esp + 4]
	mul 	DWORD [ConstXRes]
	xor 	edx, edx
	mov 	dx, WORD [esp + 6]
	add 	eax, edx
	mov 	ebx, eax
	ret

;****************************************
;	ebx <- pixel address
; 	esp + 4 -> y
;	esp + 6 -> x
;****************************************
VesaGetPixelAddress:
	xor 	eax, eax
	mov 	ax, [esp + 4]
	mul 	DWORD [ConstXRes]
	xor 	edx, edx
	mov 	dx, WORD [esp + 6]
	imul 	dx, 4
	add 	eax, edx
	mov 	ebx, eax
	ret
