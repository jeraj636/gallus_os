global _start

[bits 16]
_start:
zacetek:
; Nastavljanje  globalnega zamika
mov ax, 0x7c0
mov ds, ax

; Nastavljanje stacka
; bp (base pointer) kaže na dno stacka
; sp (stack pointer) kaže na vrh stacka
mov bp, 0x500 ; stack poiner je za zdaj kr neki
mov sp, bp ; ker je zazdaj stack prazen sp kaže na bp

cli

; test ali stack deluje?
push 'A'
pop ax
mov ah, 0xe
int 0x10 ; interupt, ki na zaslon zapiše znak



; nalaganje kernela v pomnilnik
; kernel naj bo naložen na naslov 0x7e00
disk_zacetek:
mov al, 'S'
mov ah, 0xe
int 0x10

pusha
push dx

mov ah, 0x02
mov al, 0x02
mov ch, 0x00
mov cl, 0x02
mov dh, 0x00
mov bx, 0x7e00
int 0x13
jmp  0x7e00

pop dx
popa
disk_konec:
mov al, 'D'
mov ah, 0xe
int 0x10

; Začetek definiranja GDT
; GDT segmenti vsak je velik po 8 bajtov
gdt_zacetek:


; 1. NULL segment
gdt_null:
dd 0
dd 0

; 2. code segment
gdt_koda:
dw 0xffff
dw 0x0
db 0x0
db 0b10011010
db 0b11001111
db 0x0

; 3. data segment
gdt_podatki:
dw 0xffff
dw 0x0
db 0x0
db 0b10010010
db 0b11001111
db 0x0

gdt_konec:

gdt_descriptor:
	dw gdt_konec - gdt_zacetek -1
	dd gdt_zacetek

KODA_SEG equ gdt_koda - gdt_zacetek
PODATKI_SEG equ gdt_podatki - gdt_zacetek




; je li to sploh potrebno?
jmp pojdi_v_pm

pojdi_v_pm: ;(protected mode)
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax
jmp	KODA_SEG:init_pm 

[bits 32]
init_pm:
	mov ax, PODATKI_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ebp, 0x0010000
	mov esp, ebp

;spodnjih sest vrstic naj ne bi več delovalo, če sem pravilno preklopil v pm
;mov eax, 0x0000e000
;mov al, 'B'
;int 0x10
;mov ah, 0xe
;mov al, 'B'
;int 0x10 

; Neskončna zanka
;jmp $

; preostanek kode za zapolni s ničlami
times 510 - ($ - $$) db 0

; Magična številka
dw 0xaa55
