[bits 64]

context_set:
    mov rbx, [rcx + 000h]
    mov rsp, [rcx + 008h]
    mov rbp, [rcx + 010h]
    mov rsi, [rcx + 018h]
    mov rdi, [rcx + 020h]
    mov r12, [rcx + 028h]
    mov r13, [rcx + 030h]
    mov r14, [rcx + 038h]
    mov r15, [rcx + 040h]

    movdqu xmm6 , [rcx + 050h]
    movdqu xmm7 , [rcx + 060h]
    movdqu xmm8 , [rcx + 070h]
    movdqu xmm9 , [rcx + 080h]
    movdqu xmm10, [rcx + 090h]
    movdqu xmm11, [rcx + 0A0h]
    movdqu xmm12, [rcx + 0B0h]
    movdqu xmm13, [rcx + 0C0h]
    movdqu xmm14, [rcx + 0D0h]
    movdqu xmm15, [rcx + 0E0h]

    gs mov  r10, [030h]

    ; stack top
    mov rax, [rcx + 0F0h]
    mov [r10+08h], rax

    ; stack bottom
    mov rax, [rcx + 0F8h]
    mov [r10+10h], rax

    ldmxcsr [rcx + 100h]
    fldcw   [rcx + 104h]

    mov rax, 1
    ; return address we saved previously
    jmp [rcx + 048h]

