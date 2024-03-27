[org 0x7c00]

	mov bp, 0x9000; stack pointer
	mov sp, bp

	mov al, 'R'
	call izpisi
		
	;call nalozi_kernel		
	
	push 'S' ;test ali stack deluje
	pop ax
	call izpisi

	
	
	call pojdi_v_pm

	jmp $; neskoncna zanka

; parameter al (zank ki ga želimo izpisati)
izpisi:
	mov ah, 0xe
	int 0x10
	ret

nalozi_kernel:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, 0x02
	mov ch, 0x00
	mov cl, 0x02
	mov dh, 0x00
	mov bx, 0x7e00
	int 0x13

	pop dx
	popa
	ret

gdt_zacetek:
gdt_null:
	dd 0x0
	dd 0x0
gdt_koda:
	dw 0xffff
	dw 0x0
	db 0x0
	db 0b10011010
	db 0b11001111
	db 0x0
gdt_podatki:
	dw 0xffff
	dw 0x0
	db 0x0
	db 0b10010010
	db 0b11001111
	db 0x0
gdt_konec:

gdt_descriptor:
	dw gdt_konec - gdt_zacetek - 1
	dd gdt_zacetek

KODA_SEG equ gdt_koda - gdt_zacetek
PODATKI_SEG equ gdt_podatki - gdt_zacetek	

[bits 16]
pojdi_v_pm:
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	mov edi, 0xb8000
	mov ah, 0x0f
	mov al, 'P'
	mov [edi], ax	

	jmp KODA_SEG:incializiraj_pm
[bits 32]
incializiraj_pm:
	mov ax, PODATKI_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ebp, 0x90000
	mov esp, ebp

	mov edi, 0xb8000
	mov al, 'G'
	mov ah, 0x0f
	mov [edi], ax	
	jmp $

times 510 - ($-$$) db 0
dw 0xaa55
