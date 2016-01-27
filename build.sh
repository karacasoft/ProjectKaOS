rm -f build/*

nasm bootloader/Boot1.asm -o build/Boot1.bin
nasm bootloader/Stage2.asm -o build/Stage2.bin -i bootloader/