bits 16

org 0x0

jmp loader

;====================================
;	Data block
;====================================

; Variables
DriveNumber db 0


; Strings
MsgLoading 		db "Loading...", 0x0D, 0x0A, 0x00
MsgReadingDrive db "Reading Drive...", 0x0D, 0x0A, 0x00

MsgAllOK 		db "All OK", 0x0D, 0x0A, 0x00
MsgSuccess 		db "Successful", 0x0D, 0x0A, 0x00
MsgFail 		db "Failed", 0x0D, 0x0A, 0x00

MsgFileFound	db "File found", 0x0D, 0x0A, 0x00

;====================================
;	Routines
;====================================

;	Prints a null terminated string
Print:
	lodsb
	or al, al
	jz .PrintDone
	mov ah, 0x0e
	int 0x10
	jmp Print
.PrintDone:
	ret

;====================================
;	Main Boot Block
;====================================

loader:

	cli
	mov ax, 0x7C0		; We are loaded at memory 0x7C00
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ax, 0x0000		; Setting up stack
	mov ss, ax
	mov sp, 0xFFFF
	sti

	; Save drive number for later use.
	mov BYTE [DriveNumber], dl

	; Printing loading message
	mov si, MsgLoading
	call Print

.ResetDrive:
	; Attempt to reset drive
	mov ah, 0x00
	mov dl, BYTE [DriveNumber]
	int 0x13
	; Jump if it fails
	jc .Fail

	mov si, MsgReadingDrive
	call Print

.ReadDrive:
	mov ax, 0x9C0
	mov es, ax
	xor bx, bx

	mov ah, 0x02					; Function read
	mov al, 2						; Read 2 sectors
	mov ch, 0						; Cylinder : (Aim for the) root directory :V
	mov cl, 2						; Sector
	mov dh, 0						; Head
	mov dl, BYTE [DriveNumber]
	int 0x13
	jc .Fail

.Success:
	; Print success message
	mov si, MsgAllOK
	call Print

	jmp 0x9C00

.Fail:
	; Print fail message
	mov si, MsgFail
	call Print

.Stop:
	cli
	hlt

times 510 - ($-$$) db 0x0

dw 0xaa55