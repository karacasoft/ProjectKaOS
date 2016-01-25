# Shell script to add files to Systems formatted with KSFileSystem v0.2
# This shell script 
#	- Creates a file table with null terminator
#	- Writes the file table to a file named boot.img
#	- Writes all the files to their respective positions on the file boot.img
#
# The ability to add directories, will be implemented later.


dir=$1

rm -f rootFileTable.asm

currentLBA=36 #We start from LBA 36. Leave some room for tables
currentOffset=0 

for file in $dir/*; do
	filename="${file##*/}"
	namelength="${#filename}"
	filesize=$(stat -c%s "$file")
	echo "db \"$filename\"" >> rootFileTable.asm 						#Print file name
	echo "times $((256 - $namelength)) db 0x0" >> rootFileTable.asm 	#Fill file name field
	echo "dd $currentLBA" >> rootFileTable.asm							#Print LBA position
	echo "dd $currentOffset" >> rootFileTable.asm						#Print byte offset
	echo "dd $filesize" >> rootFileTable.asm							#Print file size
	echo "" >> rootFileTable.asm

	currentOffset=$(( $currentOffset + $filesize + 12 ))
	currentLBA=$(( $currentLBA + $(( $currentOffset / 512 )) ))
	currentOffset=$(( $currentOffset % 512 ))
done

echo "times 256 db 0x0" >> rootFileTable.asm						#End table with a null terminator
echo "dd $currentLBA" >> rootFileTable.asm
echo "dd $currentOffset" >> rootFileTable.asm
echo "dd 0x0" >> rootFileTable.asm

nasm rootFileTable.asm -o FileTable.bin

dd if="FileTable.bin" of="boot.img" seek=18 count=18 conv=notrunc

currentLBA=36
currentOffset=0 

for file in $dir/*; do
	filename="${file##*/}"
	namelength="${#filename}"
	filesize=$(stat -c%s "$file")
	
	dd if="$file" of=boot.img oflag=seek_bytes seek=$(( $currentLBA * 512 + $currentOffset )) count=$(( $filesize / 512 + 1 )) conv=notrunc

	currentOffset=$(( $currentOffset + $filesize + 12 ))
	currentLBA=$(( currentLBA + $(( $currentOffset / 512 )) ))
	currentOffset=$(( $currentOffset % 512 ))
done

