# Shell script to add files to Systems formatted with KSFileSystem v0.2
# This shell script 
#	- Creates a file table with null terminator entry
#	- Writes the file table to a file given as second parameter
#	- Writes all the files to their respective positions on given disk image file
#
# The ability to add directories, will be implemented later.


dir=$1
imageFile=$2

rootFileTable="build/rootFileTable.asm"

currentLBA=36 #We start from LBA 36. Leave some room for tables
currentOffset=0 

for file in $dir/*; do
	filename="${file##*/}"
	namelength="${#filename}"
	filesize=$(stat -c%s "$file")
	echo "db \"$filename\"" >> $rootFileTable 						#Print file name
	echo "times $((256 - $namelength)) db 0x0" >> $rootFileTable 	#Fill file name field
	echo "dd $currentLBA" >> $rootFileTable							#Print LBA position
	echo "dd $currentOffset" >> $rootFileTable						#Print byte offset
	echo "dd $filesize" >> $rootFileTable							#Print file size
	echo "" >> $rootFileTable

	currentOffset=$(( $currentOffset + $filesize + 12 ))
	currentLBA=$(( $currentLBA + $(( $currentOffset / 512 )) ))
	currentOffset=$(( $currentOffset % 512 ))
done

echo "times 256 db 0x0" >> $rootFileTable						#End table with a null terminator
echo "dd $currentLBA" >> $rootFileTable
echo "dd $currentOffset" >> $rootFileTable
echo "dd 0x0" >> $rootFileTable

nasm $rootFileTable -o build/FileTable.bin

dd if="build/FileTable.bin" of=$imageFile seek=18 count=18 conv=notrunc

currentLBA=36
currentOffset=0 

for file in $dir/*; do
	filename="${file##*/}"
	namelength="${#filename}"
	filesize=$(stat -c%s "$file")
	
	dd if="$file" of=$imageFile oflag=seek_bytes seek=$(( $currentLBA * 512 + $currentOffset )) count=$(( $filesize / 512 + 1 )) conv=notrunc

	currentOffset=$(( $currentOffset + $filesize + 12 ))
	currentLBA=$(( currentLBA + $(( $currentOffset / 512 )) ))
	currentOffset=$(( $currentOffset % 512 ))
done

