# ProjectKaOS
Playing around to learn how to develop operating systems.

My goal here is to learn how to develop an operating system from absolutely nothing in hand. This means I will (at least try) not to use any ready-made component in my operating system. Except, nasm, dd, sh, <any other "basic" linux shell command> will be used. I can't instruct the cpu directly to write my files into a disk image. :D Ready-made components conclude file systems like FAT, ext4; bootloaders like grub, or compilers that compile code for x86 systems (I'll be writing my own compiler to compile (most probably) my own programming language :D).

# To operating systems masters...
Teach me EVERYTHING! How should I manage memory? What should I include in a file system? How could I generate machine instructions from my own programming language? Please pass me your vast knowledge. I will appreciate any help.

# "I want to try your OS"
Unfortunately, it does nothing considerable for now. You can use VirtualBox to emulate it though. Use "boot.img" as floppy image. You don't need a virtual disk image. So don't create any. (I'll change this text whenever you need a virtual disk image)
You can use a bootable media to try it on a real machine, too. Use:
<code>
  dd if=boot.img of=(device-you-want-to-use) seek=0 count=(any-number-higher-than-1440)
</code>
BEWARE: Use this ONLY if you know what you are doing. I would really recommend you to use a virtual machine. My code may destroy your device, rendering it unusable. Or it may clear your important data. I don't take any responsibility if anything happens to your machine. Also, the command above will corrupt your device data. It will make it unreadable by Windows/Unix/Mac. You can still make it readable by formatting the device.
