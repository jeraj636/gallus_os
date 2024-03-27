bits 32
mov edi, 0xb8000
mov al, 'A'
mov ah, 0x0f
mov [edi], ax
jmp $
