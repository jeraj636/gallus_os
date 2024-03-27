#!/bin/bash

echo zacetek grajenja

echo comp: boot.asm
nasm -f bin src/zagonski_nalagalink/boot.asm -o src/zagonski_nalagalink/boot.bin

echo comp: vstop_v_keernel.asm
nasm -f elf src/zagonski_nalagalink/vstop_v_kernel.asm -o src/zagonski_nalagalink/vstop_v_kernel.o

echo comp: kernel.c
i686-elf-gcc -ffreestanding -c src/kernel/kernel.c -o src/kernel/kernel.o

echo link: kernel.o + vstop_v_kernel.o
i686-elf-ld -o build/kernel.bin -Ttext 0x7e00 src/zagonski_nalagalink/vstop_v_kernel.o src/kernel/kernel.o --oformat binary

cat src/zagonski_nalagalink/boot.bin build/kernel.bin > build/gallus_os.bin

rm src/zagonski_nalagalink/boot.bin
rm src/zagonski_nalagalink/vstop_v_kernel.o
rm build/kernel.bin
rm src/kernel/kernel.o

echo cistim
echo konec grajenja

