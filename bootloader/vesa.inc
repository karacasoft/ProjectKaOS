;****************************************
;	VESA routines
;****************************************
bits 16
;****************************************
;		Data
;****************************************

MsgVesaEn db "Supported Video Modes:", 0x0D, 0x0A, 0x00
MsgVesaDi db "No Vesa interface!", 0x0D, 0x0A, 0x00

MsgNoSupportedMode db "No supported mode found.", 0x0D, 0x0A, 0x00

Res1Str 	db "         "
			db "x"
Res2Str 	db "         "
NOPlanes 	db "   | ", 0x00


;Struct Vesa info block[256 byte]
VesaBuffer:
Name		dd 0x00
Version		dw 0x00
OEMStr		dd 0x00
Capable 	dd 0x00
VidModes	dd 0x00
TotalMem	dw 0x00

;reserved
times 236 	db 0x0


;Struct vesa mode info block[256 byte]
VesaModeInfo:
ModeAttrib		dw 0x00
WinAAttrib		db 0x00
WinBAttrib		db 0x00
WinGranulrty	dw 0x00
WinSize			dw 0x00
WinASegment		dw 0x00
WinBSegment		dw 0x00
WinFuncPtr		dd 0x00
BytesPerSLine	dw 0x00

XRes 			dw 0x00
YRes			dw 0x00
XCharSize 		db 0x00
YCharSize 		db 0x00
NumberOfPlanes	db 0x00
BitsPerPixel 	db 0x00
NumberOfBanks	db 0x00
MemoryModel 	db 0x00
BankSize 		db 0x00
NumOfImgPages	db 0x00
				db 0x00

RedMaskSize 	db 0x00
RedFieldPos		db 0x00
GreenMaskSize	db 0x00
GreenFieldPos 	db 0x00
BlueMaskSize	db 0x00
BlueFieldPos 	db 0x00
RsvdMaskSize 	db 0x00
RsvdFieldPos	db 0x00
DCMInfo			db 0x00

PhysBasePtr		dd 0x00
OffScrMemOffset dd 0x00
OffScrMemSize 	dw 0x00

;reserved
times 206 		db 0x00


; Vesa best mode selector block
VesaSelector:
Pref_ModeAttrib	dw 155
Pref_XRes		dw 1024
Pref_YRes		dw 768
Pref_BitsPerP	db 32
;****************************************
;		Functions
;****************************************


;******************
;	ax -> color
;	
;
;	No parameters
;******************

; Test mode, change this behaviour later



;******************
;	No parameters
;******************
VesaLoadModes:
	mov		ah, 0x4F
	mov 	al, 0x00
	mov 	di, VesaBuffer

	int 	0x10
	ret


;***********************
;	bx -> video mode
;		D15:	Clear flag
;		D14-0:	Video mode
;	ax <- status
;		0x004F = success
;***********************
VesaSetMode:
	mov 	ah, 0x4F
	mov		al, 0x02

	int 	0x10
	ret

;****************************
;	bx <- best video mode	
;
;	VesaLoadModes must be
; called beforehand.
;****************************
VesaFindBestMode:
	mov 	di, [VidModes]
	mov 	es, [VidModes+2]
.VesaCheckMode:
	mov 	cx, [es:di]

	cmp 	cx, 0xFFFF;
	je		.FindModeNoneFound

	push 	di
	call 	VesaLoadModeAttrib

	pop 	di
	add		di, 2

	mov 	ax, [ModeAttrib]
	and 	ax, [Pref_ModeAttrib]
	cmp 	ax, [Pref_ModeAttrib]
	jne 	.VesaCheckMode

	mov 	ax, [XRes]
	cmp 	ax, [Pref_XRes]
	jne 	.VesaCheckMode

	mov 	ax, [YRes]
	cmp 	ax, [Pref_YRes]
	jne 	.VesaCheckMode

	mov 	al, [BitsPerPixel]
	cmp 	al, [Pref_BitsPerP]
	jne 	.VesaCheckMode

	mov 	eax, [PhysBasePtr]
	mov 	[VideoMemAddress], eax

	mov 	bx, cx

	ret
.FindModeNoneFound
	mov  	si, MsgNoSupportedMode
	call 	Puts16

	;TODO remove this later
	cli
	hlt

	ret



;****************************
;	cx -> Video mode to load
;	
;****************************
VesaLoadModeAttrib:
	mov		ah, 0x4F
	mov 	al, 0x01

	xor		dx, dx
	mov 	es, dx
	mov 	di, VesaModeInfo

	int 	0x10
	ret

;****************************
;		No parameters
;	VesaLoadModes must be
; called beforehand.
;****************************
VesaListSupportedModes:

	mov  	si, MsgVesaEn
	call 	Puts16

	mov 	di, [VidModes]
	mov 	es, [VidModes+2]
.VesaCheckMode:
	mov 	cx, [es:di]

	cmp 	cx, 0xFFFF;
	je		.CheckModeDone

	push 	di
	call 	VesaLoadModeAttrib

	pop 	di
	add		di, 2

	call 	FreeStrings

	push 	WORD [OffScrMemSize]
	push 	Res1Str
	call 	intToString16
	add 	sp, 4

	push 	WORD [OffScrMemOffset+2]
	push 	Res2Str
	call 	intToString16
	add 	sp, 4

	mov 	ax, 0x00
	mov 	al, BYTE [BitsPerPixel]
	push 	ax
	push 	NOPlanes
	call 	intToString16
	add 	sp, 4

	mov 	si, Res1Str
	call 	Puts16

	jmp 	.VesaCheckMode

.CheckModeDone:
	ret