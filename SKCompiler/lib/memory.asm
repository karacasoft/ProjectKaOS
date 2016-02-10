bits 32

;**************************************
;	Memory Management Routines
;**************************************

;**************************************
;		pointer alloc(int)
;	Allocate <int> bytes of memory.
;	Return a pointer to the first byte.
;**************************************

alloc:
	push ebp
	mov ebp, esp




	mov esp, ebp
	pop ebp
	ret



;**************************************
;		
;**************************************