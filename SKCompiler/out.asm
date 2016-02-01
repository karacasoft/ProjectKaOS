global _main
extern _printf

section .text
_main:
mov eax, 33
push eax
mov eax, [test]
add eax, DWORD [esp]
add esp, 4
push eax
mov eax, 22
sub eax, DWORD [esp]
neg eax
add esp, 4

push msg
call _printf
add esp, 4
ret

test: dd 0
msg: db "Test", 10, 0