;=========================================
;	KaOS File I/O System
;		-Routines that read/write files in
;	an Anarchy FileSystem.
;=========================================

org 0x0

bits 16

;=========================================
;	Constants
;=========================================

FileNameSize dw 256


;=========================================
;	LoadDirFileTable()
;		-Loads a directory file table
;	 into the memory.
;	AX => Directory LBA
;	CX => ByteOffset
;	ES:BX => Place to load table
;=========================================

LoadDirFileTable:
	push ax
	push cx
	push es
	push bx

.ReadDrive:
	mov ax, 0x9C0
	mov es, ax
	xor bx, bx

	mov ah, 0x02					; Function read
	mov al, 1						; Read 2 sectors
	mov ch, 0						; Cylinder
	mov cl, 2						; Sector
	mov dh, 0						; Head
	mov dl, BYTE [DriveNumber]
	int 0x13
	jc .Fail	


	pop bx
	pop es
	pop cx
	pop ax


;=========================================
;	LBACHS()
;	AX => LBA Value
;=========================================

LBACHS:
	push ax

	pop ax