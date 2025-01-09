savedcmd_/home/user/shared/ext2-lite/ext2-lite-helpcode/ext2-lite.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o /home/user/shared/ext2-lite/ext2-lite-helpcode/ext2-lite.o @/home/user/shared/ext2-lite/ext2-lite-helpcode/ext2-lite.mod  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --static-call --uaccess --prefix=16  --link  --module /home/user/shared/ext2-lite/ext2-lite-helpcode/ext2-lite.o

/home/user/shared/ext2-lite/ext2-lite-helpcode/ext2-lite.o: $(wildcard ./tools/objtool/objtool)
