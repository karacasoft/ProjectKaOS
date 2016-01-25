bits 16

org 0x0

jmp loader

;====================================
;	Data block
;====================================

; Variables
DriveNumber db 0

CurrentFile dw 0
IsNull db 0

FileLBA dw 0
FileOffset dw 0
FileSize dw 0

absoluteSector db 0
absoluteHead db 0
absoluteTrack db 0

sectorsToRead db 0


; Constants
sectorsPerTrack dw 18
headsPerCylinder dw 2
bytesPerSector dw 512


; Strings
MsgLoading 		db "Loading...", 0x0D, 0x0A, 0x00
MsgReadingDrive db "Reading Drive...", 0x0D, 0x0A, 0x00

MsgAllOK 		db "All OK", 0x0D, 0x0A, 0x00
MsgSuccess 		db "Successful", 0x0D, 0x0A, 0x00
MsgFail 		db "Failed", 0x0D, 0x0A, 0x00

MsgFileFound	db "File found", 0x0D, 0x0A, 0x00
MsgProgress		db ".", 0x00

BootFileName 	db "Stage2.bin", 0

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



ReadRootFileTable:
.Loop1:
	mov si, MsgProgress
	call Print

	; Attempt to read sectors from drive
	; Read into memory 0x9C00

	mov ax, 0x9C0
	mov es, ax
	xor bx, bx

	xor cx, cx

	mov ah, 0x02					; Function read
	mov al, 18						; Read 18 sectors
	mov ch, 0						; Cylinder : (Aim for the) root directory :V
	shl cx, 2
	add cl, 1						; Sector
	mov dh, 1						; Head
	mov dl, BYTE [DriveNumber]
	int 0x13
	jc .Fail

	call FindBoot2File

	.Fail:
		mov al, 0x0E
		ret



;	Looks for the boot file in the file table
FindBoot2File:
	mov ax, 0x09C0
	mov es, ax
	mov ax, 268
	mul WORD [CurrentFile]
	mov cx, ax
	

	mov si, BootFileName
.FindBootFileLoop1:
	lodsb

	mov bx, cx

	mov ah, BYTE [es:bx]
	or BYTE [IsNull], ah
	jz .EndOfFolder
	mov BYTE [IsNull], 1

	cmp al, ah
	jne .NextFile

	or al, al
	jz .FileFound

	inc cx
	jmp .FindBootFileLoop1


.NextFile:
	inc WORD [CurrentFile]
	mov BYTE [IsNull], 0
	jmp FindBoot2File

.FileFound:
	mov si, MsgFileFound
	call Print

	mov ax, 0x9C0
	mov es, ax

	mov ax, 268
	mul WORD [CurrentFile]
	add ax, 256
	mov bx, ax
	mov dx, WORD [es:bx]
	mov WORD [FileLBA], dx		;Store LBA no
	add bx, 4					;4 bytes forward
	mov dx, WORD [es:bx]
	mov WORD [FileOffset], dx	;Store byte offset
	add bx, 4
	mov dx, WORD [es:bx]
	mov WORD [FileSize], dx		;Store file size

	call ReadAndExecuteFile

.EndOfFolder:
	;re(k)t
	mov ah, 0xE
	ret




;	Reads and executes the file on FileLBA + FileOffset
ReadAndExecuteFile:
	xor dx, dx
	mov ax, WORD [FileSize]
	add ax, WORD [FileOffset]
	div WORD [bytesPerSector]

	inc al
	mov BYTE [sectorsToRead], al

	mov ax, WORD [FileLBA]
	call LBACHS
	mov ax, 0x9C0
	mov es, ax
	xor bx, bx

	mov ah, 0x02					; Function read
	mov al, BYTE [sectorsToRead]
	mov ch, BYTE [absoluteTrack]
	mov cl, BYTE [absoluteSector]
	mov dh, BYTE [absoluteHead]
	mov dl, BYTE [DriveNumber]		
	int 0x13
	jc .Fail

	mov bx, WORD [FileOffset]
	add bx, 0x9C00
	jmp bx

	.Fail:
		mov ah, 0xE
		ret




LBACHS:
          xor     dx, dx
          div     WORD [sectorsPerTrack]
          inc     dl
          mov     BYTE [absoluteSector], dl
          xor     dx, dx
          div     WORD [headsPerCylinder]
          mov     BYTE [absoluteHead], dl
          mov     BYTE [absoluteTrack], al
          ret


;====================================
;	Main Boot Block
;====================================

loader:

	cli
	mov ax, 0x7C0		; We are loaded at memory 0x7C0
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
	call ReadRootFileTable

	cmp ah, 0xE
	je .Fail

.Success:
	; Print success message
	mov si, MsgAllOK
	call Print

.Fail:
	; Print fail message
	mov si, MsgFail
	call Print

.Stop:
	cli
	hlt

times 510 - ($-$$) db 0x0

dw 0xaa55