section .data
    ACC dd 0
    var28 equ 2
    buffer db 0

section .bss
    var29 resd 0
    var30 resd 0
    var31 resd 0

section .text
    global _start

_start:
    ; INPUT
    mov eax, 3
    mov ebx, 1
    mov ecx, var29
    mov edx, 32
    int 0x80
    
    push var29
    call atoi
    mov [var29], eax
    
    ; LOAD
    mov [ACC], [var29]
    
LABEL4:
    ; DIV
    mov ecx, [ACC]
    mov eax, [var28]
    cdq
    idiv ebx
    mov [ACC], eax
    
    ; STORE
    mov [var30], [ACC]
    
    ; MUL
    mov eax, [ACC]
    mov ebx, [var28]
    imul ebx
    mov [ACC], eax
    
    ; STORE
    mov [var31], [ACC]
    
    ; LOAD
    mov [ACC], [var29]
    
    ; SUB
    sub [ACC], [var31]
    ; STORE
    mov [var31], [ACC]
    
    ; OUTPUT
    mov edi, buffer + 30
    mov eax, [var31]
    call itoa
    
    mov eax, buffer + 31
    sub eax, edi
    mov edx, eax
    mov eax, 4
    mov ebx, 1
    lea ecx, [edi + 1]
    int 0x80
    
    ; COPY
    mov [var30], [var29]
    
    ; LOAD
    mov [ACC], [var29]
    
    ; JMPP
    cmp [ACC], 0
    jg LABEL4
    
    ; STOP
    mov eax, 1
    xor ebx, ebx
    int 0x80

atoi:
   %define VALUE [EBP + 8]
   enter 0,0
   sub eax, eax
   sub ebx, ebx
   mov esi, VALUE
   atoi_loop:
       mov bl, byte[esi]
       cmp bl, 0xA
       je atoi_loop_end
       cmp ebx, 0
       je atoi_loop_end
       sub bl, '0'
       imul eax, eax, 10
       add eax, ebx
       inc esi
       jmp atoi_loop
   atoi_loop_end:
   leave
   ret 4

itoa:
    enter 0,0
    itoa_loop:
        sub edx, edx
        mov ecx, 10
        div ecx
        add dl, '0'
        mov [edi], dl
        dec edi
        test eax, eax
        jnz itoa_loop
    leave
    ret

