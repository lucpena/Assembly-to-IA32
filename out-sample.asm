section .data
    ACC dd 0
    var28 equ 2
    var29 dd 69

section .bss
    var30 resd 0
    var31 resd 0

section .text
    global _start

_start:
    ;INPUT
    mov [ACC], [var29]
LABEL4:
    mov ecx, [ACC]
    mov eax, [var28]
    cdq
    idiv ebx
    mov [ACC], eax
    mov [var30], [ACC]
    mov eax, [ACC]
    mov ebx, [var28]
    imul ebx
    mov [ACC], eax
    mov [var31], [ACC]
    mov [ACC], [var29]
    sub [ACC], [var31]
    mov [var31], [ACC]
    ;OUTPUT [var31]
    mov [var30], [var29]
    mov [ACC], [var29]
    cmp [ACC], 0
    jg LABEL4
    mov eax, 1
    xor ebx, ebx
    int 0x80
