#!/bin/bash

echo zacetek grajenja

echo comp: boot.asm
nasm -f bin src/zagonski_nalagalink/zagonski_nalagalnik.asm -o src/zagonski_nalagalink/zagonski_nalagalnik.bin

echo comp: vstop_v_keernel.asm
nasm -f elf src/zagonski_nalagalink/vstop_v_kernel.asm -o src/zagonski_nalagalink/vstop_v_kernel.o

echo comp: kernel.c
i686-elf-gcc -ffreestanding -c src/kernel/kernel.c -o src/kernel/kernel.o

echo comp: preprost_vga_gonilnik.c
i686-elf-gcc -ffreestanding -c src/kernel/preprost_vga_gonilnik/preprost_vga_gonilnik.c -o src/kernel/preprost_vga_gonilnik/preprost_vga_gonilnik.o

echo link: 
i686-elf-ld -o build/kernel.bin -Ttext 0x7e00 src/zagonski_nalagalink/vstop_v_kernel.o src/kernel/kernel.o src/kernel/preprost_vga_gonilnik/preprost_vga_gonilnik.o --oformat binary

cat src/zagonski_nalagalink/zagonski_nalagalnik.bin build/kernel.bin > build/gallus_os.bin

rm src/zagonski_nalagalink/zagonski_nalagalnik.bin
rm src/zagonski_nalagalink/vstop_v_kernel.o
rm build/kernel.bin
rm src/kernel/kernel.o

echo cistim
echo konec grajenja

