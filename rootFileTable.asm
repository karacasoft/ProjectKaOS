db "abc.txt"
times 249 db 0x0
dd 36
dd 0
dd 5

db "Stage2.bin"
times 246 db 0x0
dd 36
dd 17
dd 193

times 256 db 0x0
dd 36
dd 222
dd 0x0
