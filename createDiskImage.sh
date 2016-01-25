dir=$1

rm -f build/Boot1.bin boot.img

nasm Boot1.asm -o build/Boot1.bin
#nasm Stage2.asm -o build/Stage2.bin
#nasm KSFileSystemSchema.asm -o build/Filesystem.bin

dd if=/dev/zero of=boot.img bs=1024 count=1440
dd if=build/Boot1.bin of=boot.img seek=0 count=1 conv=notrunc

./AddFilesToImage.sh $dir