; Test file to immitate the new filesystem.

db "Boot2.bin"
times 247 db 0x0
dd 0x0024
dd 0x0
dd 0xCD

; Null terminator file entry
times 256 db 0x0	
dd 0x24				; Null terminator should define where the next file should be placed.
					; In this case its 36th sector
dd 0xD9				; Length of the last file + 12 bytes
dd 0x0				; Size 0