;*****************
;	Gdt.inc
;		-GDT Routines
;	  KaOS
;*****************

%ifndef __GDT_INC_INCLUDED__
%define __GDT_INC_INCLUDED__

bits 16

;********Routine : InstallGDT()
;	-Install KaOS GDT
;
;******************

InstallGDT:
	cli
	pusha
	lgdt [toc]
	sti
	popa
	ret

;*********************
;		GDT
;*********************

gdt_data:
	dd 0	; null descriptor
	dd 0

;code descriptor:
	dw 0ffffh
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

;data descriptor:
	dw 0FFFFh
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0

end_of_gdt:
toc:
	dw end_of_gdt - gdt_data - 1
	dd gdt_data

%endif ;  __GDT_INC_INCLUDED__