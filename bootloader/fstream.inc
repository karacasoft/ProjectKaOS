;=========================================
;	KaOS File I/O System
;		-Routines that read/write files in
;	an Anarchy FileSystem.
;=========================================

org 0x0

;=========================================
;	Constants
;=========================================

FileNameMaxSize dw 256


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