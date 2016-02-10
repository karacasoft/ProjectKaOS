bits 32

;**************************************
;		String Routines
;	SK Library
;	- Import with "use <string>"
;**************************************

;**************************************
;		pointer allocString(int)
;		Allocates (int) bytes from memory.
;	Returns the address of the 
;	first byte.
;**************************************

allocString:
	push ebp
	mov ebp, esp




	mov esp, ebp
	pop ebp
	ret