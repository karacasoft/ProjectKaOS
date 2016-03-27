bits 16

org 0x9C00

jmp main

;=====================================
;	Preprocessor Directives
;=====================================

%include "stdio.inc"
%include "gdt.inc"
%include "vesa.inc"


;=====================================
;	Data Block
;=====================================

MsgStarting db "Starting Boot Stage 2", 0x0D, 0x0A, 0x00
MsgVideo db "Looking for video modes...", 0x0D, 0x0A, 0x00
TestNumber dw 125

StrTest db "     ", 0x0D, 0x0A, 0x00

TestComplete db "Test Completed", 0x0D, 0x0A, 0x00

;=====================================
;	Stage 2 Entry Point
;=====================================

main:
	
	cli
	xor 	ax, ax
	mov 	ds, ax
	mov 	es, ax

	mov 	ax, 0x9000
 	mov 	ss, ax
 	mov 	sp, 0xFFFF
	sti

	mov 	si, MsgStarting		; Display Starting message
	call 	Puts16

	;Load vesa info block into memory
	call 	VesaLoadModes
	;List all the vesa modes available
	;call 	VesaListSupportedModes
	;jmp 	End

	call 	VesaFindBestMode

	or  	bx, 0x8000 ; (don't) clear video memory
	or 		bx, 0x4000 ; use lfb

	call 	VesaSetMode




	call 	InstallGDT
	
	cli
	mov 	eax, cr0
	or 		eax, 1
	mov		cr0, eax
	; Now in protected mode

	jmp 	0x08:pModeStart

FreeStrings:
	mov BYTE [Res1Str], " "
	mov BYTE [Res1Str+1], " "
	mov BYTE [Res1Str+2], " "
	mov BYTE [Res1Str+3], " "
	mov BYTE [Res1Str+4], " "
	
	mov BYTE [Res2Str], " "
	mov BYTE [Res2Str+1], " "
	mov BYTE [Res2Str+2], " "
	mov BYTE [Res2Str+3], " "
	mov BYTE [Res2Str+4], " "
	ret

bits 32


pModeStart:
	mov 	ax, 0x10
	mov 	ds, ax
	mov 	ss, ax
	mov		es, ax
	mov 	fs, ax
	mov 	gs, ax
	mov 	esp, 90000h

	mov 	ebx, 0x00AEFF80

	mov 	dx, 50
	

VesaPaintPixel:
	mov 	edi, 0xE0000000

	push 	dx
	push 	ebx

	push 	WORD 50
	push 	dx
	call 	VesaGetPixelAddress
	add 	esp, 4

	add 	edi, ebx

	pop 	ebx
	pop 	dx

	mov 	cx, 0
	inc 	dx
.PaintPixelLoop:
	cmp 	dx, 100
	je		.Done
	mov 	[edi], ebx
	add 	edi, 4
	cmp 	cx, 50
	je  	VesaPaintPixel

	inc 	cx
	jmp 	.PaintPixelLoop
.Done:
	

End:

	cli						; Stop for now
	hlt

%include "globals.inc"
%include "vesa32.inc"
