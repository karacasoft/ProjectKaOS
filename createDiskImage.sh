imageFile=$1

rm -f $imageFile

dd if=/dev/zero of=$imageFile bs=1024 count=1440
dd if=build/Boot1.bin of=$imageFile seek=0 count=1 conv=notrunc
dd if=build/Stage2.bin of=$imageFile seek=1 count=17 conv=notrunc