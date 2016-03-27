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

DriveNumber db 0

SectorsPerTrack db 18
NumberOfHeads db 2

DA_PACK:
			db 0x10
			db 0
blocksRead:	dw 16
d_address:	dw 0x7C00
			dw 0
d_lba:		dd 18
			dd 0


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

	mov [DriveNumber], dl

	mov ah, 0x41
	mov bx, 0x55AA
	mov dl, 0x80
	int 0x13
	jnc .ReadDriveExtension

	;and dl, 0x80
	;jnz .ReadDrive

	jmp .GetDiskValues

.ReadDriveExtension:
	mov si, DA_PACK
	mov ah, 0x42
	mov dl, [DriveNumber] 
	int 0x13
	jnc .ReadDone
	

.GetDiskValues:
	mov ah, 0x8
	mov dl, [DriveNumber]
	int 0x13

	add dh, 1 					; Arbitrary addition(?)
	mov [NumberOfHeads], dh
	and cl, 0x3f
	mov [SectorsPerTrack], cl

	xor ax, ax
	mov al, [NumberOfHeads]

	push ax
	push StrTest
	call intToString16

	mov si, StrTest
	call Puts16

	mov al, [SectorsPerTrack]


	push ax
	push StrTest
	call intToString16

	mov si, StrTest
	call Puts16

.ReadDrive:
	

.ReadDone:

	cli						; Stop for now
	hlt